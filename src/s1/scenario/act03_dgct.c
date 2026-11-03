#include "act03_dgct.h"

static void script_0030(int32_t arg0);
static int32_t script_005C(int32_t arg0, int32_t arg1);
static int8_t script_008F(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_01FA(int32_t arg0, int32_t arg1);
static void script_03C3(int32_t arg0, int32_t arg1);
static void script_0599(int32_t arg0, int32_t arg1);
static void script_0951(void);
static int32_t script_0A18(void);
static void script_0AAD(float arg0, float arg1, float arg2, int32_t arg3);
static void script_0AF7(int32_t arg0);
static int8_t script_0B11(void);
static int8_t script_0B29(void);
static void script_0B58(void);
static void script_104C(const char *arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10F9(void);
static void script_1279(void);
static void script_1315(void);
static void script_1E4A(void);
static void script_1E4D(int32_t arg0);
static void script_1EFF(void);
static void script_1FCD(void);
static int32_t script_20CB(void);
static int32_t script_21D6(void);
static int32_t script_22C2(void);
static int32_t script_2320(void);
static int32_t script_2384(void);
static int32_t script_256F(void);
static int32_t script_2615(void);
static int32_t script_26A1(void);
static void script_2799(void);
static void script_280E(void);
static void script_2820(void);
static int32_t script_287D(void);
static int32_t script_296F(int32_t arg0);
static int32_t script_29B6(void);
static int32_t script_29FF(int32_t arg0);
static void script_2A1D(float arg0);
static int32_t script_2A3C(int32_t arg0, int32_t arg1);
static void script_2AB4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3066(int32_t arg0);
static void script_3137(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3147(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_324B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_33ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_354E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_36AF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_394D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3A1B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3A82(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3AE9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3B30(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3C66(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4527(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_4645(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4D0A(int32_t arg0, int32_t arg1);
static void script_4DE0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_5CF3(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_5D5F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6219(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_68D3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_81B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_88D4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_905E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_9136(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_95AD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A540(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A98A(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_AB16(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_AB4A(void);
static void script_ABB3(int32_t arg0, int32_t arg1);
static void script_AC50(int32_t arg0, int32_t arg1);
static void script_AC8A(void);
static void script_ACED(void);
static void script_AD50(int32_t arg0);
static void script_AD87(int32_t arg0);
static void script_ADBE(int32_t arg0);
static void script_B000(int32_t arg0);
static void script_B049(int32_t arg0);
static void script_B092(int32_t arg0, int32_t arg1);
static void script_B108(int32_t arg0);
static void script_B116(int32_t arg0);
static void script_B124(int32_t arg0);
static void script_B1EB(void);
static void script_B1F7(void);
static void script_B203(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_B246(void);
static int8_t script_B357(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_B3EF(int32_t arg0);
static void script_B5BE(int32_t arg0);
static int32_t script_B5EF(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_BCB8(int32_t arg0, int32_t arg1);
static void script_BCEA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_BE5A(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_C016(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_C1CA(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_C3E0(int32_t arg0, int32_t arg1);
static void script_C4AA(int32_t arg0, int32_t arg1);
static void script_C50B(int32_t arg0);
static void script_C6B0(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_C748(int32_t arg0, int32_t arg1);
static void script_C82A(int32_t arg0);
static void script_C881(void);
static void script_C9B9(int32_t arg0);
static void script_C9C5(int32_t arg0);
static void script_C9D1(int32_t arg0);
static void script_CACD(int32_t arg0);
static void script_CAE4(int32_t arg0);
static void script_CB25(int32_t arg0);
static void script_CB69(int32_t arg0);
static void script_CBE7(void);
static void script_CC66(int32_t arg0);
static void script_CE07(int8_t arg0, int8_t arg1);
static void script_CFF2(int32_t arg0, int32_t arg1);
static void script_D041(int32_t arg0, int32_t arg1);
static void script_D094(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static void script_D2CB(int32_t arg0);
static void script_D493(void);
static int32_t script_D5B4(void);
static int32_t script_DA5B(void);
static void script_DC73(void);
static int32_t script_DCF6(void);
static int32_t script_DE38(void);
static int32_t script_DE8C(void);
static int32_t script_E025(void);
static int32_t script_E094(void);
static int32_t script_E103(void);
static int32_t script_E1E2(void);
static void script_E358(int32_t arg0);
static void script_E372(int32_t arg0, float arg1);
static void script_E3AD(Vector3 *arg0, Vector3i *arg1);
static void script_E3F0(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_E3F3(void);
static void script_EA0B(void);
static int32_t script_EBC6(void);
static void script_ECE8(int32_t arg0);
static int32_t script_ED11(void);
static int32_t script_ED4E(void);
static int32_t script_EDD3(void);
static void script_EE07(void);
static void script_EE43(void);
static int32_t script_EE8B(int32_t arg0, const char *arg1);
static void script_EEE4(int32_t arg0, int32_t arg1);
static void script_EEFD(int32_t arg0);
static void script_F084(int32_t arg0);
static int32_t script_F0AA(void);
static int32_t script_F210(void);
static void script_F2D7(int32_t arg0);
static void script_F2F5(int32_t arg0);
static void script_F51D(int32_t arg0);
static void script_F65E(int32_t arg0);
static void script_F7FF(void);
static int32_t script_F828(int32_t arg0);
static void script_F971(void);
static void script_F9AC(int32_t arg0);
static int32_t script_FFDB(void);
static void script_1000A(int32_t arg0);
static void script_1005C(int32_t arg0);
static void script_104BF(int32_t arg0);
static int32_t script_105FA(void);
static void script_10617(void);
static void script_10656(void);
static void script_10729(void);
static void script_107E1(const Vector3 *arg0, const Vector3i *arg1);
static void script_1081D(void);
static void script_10933(int32_t arg0);
static void script_109F2(int32_t arg0);
static void script_10AD0(int32_t arg0, int32_t arg1);
static void script_10C20(int32_t arg0);
static void script_10CF9(int32_t arg0);
static int32_t script_10E17(int32_t arg0, int32_t arg1);
static int32_t script_10FFB(int32_t arg0);
static void script_112E8(int32_t arg0);
static void script_11488(int32_t arg0, int32_t arg1);
static void script_11643(int8_t arg0, int8_t arg1, int32_t arg2);
static void script_11762(int32_t arg0);
static int32_t script_11892(int32_t arg0);
static int32_t script_119DC(int32_t arg0);
static void script_119F3(int32_t arg0);
static void script_11A17(int32_t arg0);
static void script_11A9D(int32_t arg0);
static void script_11B8E(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3, int32_t arg4, const Vector3 *arg5, const Vector3 *arg6);
static void script_11C37(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_11C89(int32_t arg0, ...);
static void script_11D89(const char *arg0);
static void script_11D98(int32_t arg0, const char *arg1, const char *arg2, ...);
static void script_11E02(int32_t arg0, int32_t arg1);
static int32_t script_11FF8(int32_t arg0, int32_t arg1);
static void script_12024(int32_t arg0);
static void script_120EE(int32_t arg0);
static void script_12108(int32_t arg0, const Vector3 *arg1);
static int32_t script_1212C(float arg0, float arg1);
static int32_t script_12165(float arg0, float arg1);
static int32_t script_1227F(float arg0, float arg1);
static int32_t script_12330(void);
static int32_t script_12350(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_12826(int32_t arg0, int32_t arg1, const Vector3 *arg2);
static void script_12846(int32_t arg0, int32_t arg1, Vector3 *arg2);
static void script_12865(int32_t arg0, int32_t arg1, const Vector3i *arg2);
static void script_12887(int32_t arg0, Vector3 *arg1);
static void script_128CA(int32_t arg0, Vector3 *arg1);
static void script_12940(int32_t arg0, float *arg1);
static void script_12993(void);
static void script_129B8(void);
static int32_t script_129EE(int8_t arg0, int8_t arg1, int8_t arg2);
static int32_t script_12A2B(int8_t arg0, int8_t arg1, int8_t arg2);
static void script_12A86(void);
static void script_12DB9(int32_t arg0);
static void script_12E38(void);
static void script_12F87(void);
static void script_12FC5(void);
static void script_131E9(int32_t arg0);
static void script_132C4(void);
static void script_13455(void);
static void script_134A1(void);
static int32_t script_1355B(void);
static void script_13B41(void);
static void script_13B95(void);
static int32_t script_13BE9(void);
static void script_13C63(void);
static void script_13D90(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_13DDB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_13DF3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, ...);
static void script_13E0B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_13E23(void);
static void script_13EC6(int32_t arg0);
static void script_13F4A(int32_t arg0, int32_t arg1);
static int32_t script_140BA(int32_t arg0, int32_t arg1);
static float script_14446(int32_t arg0, int32_t arg1);
static int8_t script_1449F(int16_t arg0);
static void script_14577(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_145DD(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_14750(void);
static int32_t script_14841(void);
static void script_148CA(int8_t arg0, const char *arg1);
static void script_148EC(int32_t arg0, int8_t arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6);
static void script_14995(const void *arg0);
static int32_t script_149B5(void);
static int32_t script_14B3A(void);
static void script_14D22(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_14D58(int32_t arg0);
static void script_14D72(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_14D87(void);
static void script_14DB6(void);
static void script_14DE6(float arg0, float arg1, int32_t arg2);
static void script_14E30(Vector3 *arg0, float arg1, float arg2, float arg3);
static void script_14E4B(Vector3 *arg0, float arg1, float arg2, float arg3);
static int32_t script_14E78(void);
static int32_t script_14E97(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_14F02(float arg0, float arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_14F9E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_15011(int32_t arg0);
static void script_152AD(void);
static void script_15335(void);
static void script_1542F(void);
static void script_1546C(void);
static void script_15586(int32_t arg0, int32_t arg1);
static void script_15A17(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_15A8D(void);
static void script_15B3E(void);
static void script_15BB8(void);
static void script_15BF2(void);
static void script_15CA4(void);
static void script_15CB1(void);
static void script_15CC6(void);
static void script_15CDB(void);
static void script_15EF4(void);
static void script_15F27(void);
static void script_15FB6(void);
static void script_15FFB(void);
static void script_16161(void);
static void script_161D1(void);
static int32_t script_17646(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_178AC(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_17DB2(int32_t arg0);
static void script_17E21(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_17E39(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_18061(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_18190(void);
static void script_18207(int32_t arg0);
static void script_18413(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1849C(int32_t arg0);
static void script_184C2(int32_t arg0);
static void script_18660(void);
static void script_1867B(void);
static void script_1872D(void);
static void script_187F2(void);
static void script_188AA(void);
static void script_188D5(void);
static void script_18900(int32_t arg0, int32_t arg1, Vector3 *arg2, int32_t arg3);
static void script_189B1(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_189FC(int32_t arg0, int32_t arg1);
static int32_t script_18B3A(int32_t arg0, int32_t arg1);
static int32_t script_18BD1(int32_t arg0, int32_t arg1);
static void script_18CB8(int32_t arg0);
static int32_t script_18CC8(int32_t arg0);
static void script_18D94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_18E50(int32_t arg0, int32_t arg1);
static void script_18F06(int32_t arg0);
static int32_t script_18FA4(void);
static void script_1902D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1905A(int32_t arg0, float arg1, float arg2, float arg3);
static void script_190B1(Vector3i *arg0, float arg1, float arg2, float arg3, ...);
static int32_t script_190F6(int32_t arg0);
static void script_194ED(void);
static void script_19574(void);
static void script_196A3(int32_t arg0);
static void script_1970F(int32_t arg0);
static void script_197FF(void);
static int32_t script_198EE(void);
static void script_199FC(void);
static void script_19AF1(void);
static void script_19B84(void);
static void script_19C40(int32_t arg0);
static void script_19CF4(void);
static void script_19FA2(void);
static void script_19FFA(void);
static void script_1A0A2(void);
static void script_1A1B9(void);
static void script_1A262(void);
static void script_1A276(void);
static void script_1A28A(void);
static void script_1A330(void);
static void script_1A4BB(int32_t arg0);
static void script_1A546(int32_t arg0);
static void script_1A631(void);
static int32_t script_1A720(void);
static void script_1A7DA(int32_t arg0);
static void script_1A8B9(void);
static void script_1A93E(void);
static void script_1A9FA(int32_t arg0);
static void script_1AAD1(void);
static void script_1AC97(void);
static void script_1ACEF(void);
static void script_1AD6C(void);
static void script_1AD7B(void);
static void script_1AECF(void);
static void script_1AF7D(void);
static void script_1B023(void);
static void script_1B113(int32_t arg0, float arg1, float arg2, float arg3);
static void script_1B13B(int32_t arg0, float arg1, float arg2, float arg3);
static void script_1B18F(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1B1DF(int32_t arg0);
static void script_1B286(int32_t arg0, int32_t arg1);
static int32_t script_1B2A9(int32_t arg0);
static void script_1B2DC(int32_t arg0, int32_t arg1);
static void script_1B2FF(int32_t arg0, int32_t arg1);
static void script_1B35E(int32_t arg0);
static void script_1B373(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1B709(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1BD97(void);
static void script_1BDA6(void);
static void script_1BDB5(void);
static void script_1BDB8(void);
static void script_1BDBB(void);
static void script_1BDBE(void);
static void script_1BDC1(void);
static void script_1BDCE(void);
static void script_1BDDA(void);
static int32_t script_1BDE9(void);
static int32_t script_1BDF8(void);
static void script_1BE29(int32_t arg0, const char *arg1);
static void script_1BF4A(void);
void script_1C0AF(void);
static void script_1C3DE(int32_t arg0, const char *arg1);
static int32_t script_1C45D(void);
static int32_t script_1C829(int32_t arg0);
static void script_1C90E(void);

static int8_t task_0;
static int8_t task_1;
static int8_t task_2;
static uint8_t task_3;
static GameDateTime task_4;
static uint8_t task_C;
static int8_t task_D;
static int8_t task_E;
static int8_t task_F;
static int8_t task_10;
static int8_t task_11;
static int8_t task_12;
static int8_t task_13;
static int8_t task_14;
static uint8_t task_15;
static uint16_t task_16;
static int32_t task_18;
static int32_t task_1C;
static int32_t task_20;
static int32_t task_24;
static int32_t task_28;
static int32_t task_2C;
static int32_t task_30;
static int32_t task_34;
static int8_t task_38;
static uint8_t task_39;
static uint16_t task_3A;
static int32_t task_3C;
static int32_t task_40;
static int32_t task_44;
static int8_t task_48;
static int8_t task_49;
static int8_t task_4A;
static int8_t task_4B;
static int8_t task_4C;
static uint8_t task_4D;
static uint16_t task_4E;
static DSWKDoorEntry *task_50;
static int32_t *task_54;
static DoorResourceDescriptor *task_58;
static MapObjectRecord *task_5C;
static DoorSlotState task_60;
static int32_t task_6C;
static int32_t task_70[10];
static int32_t task_98[10];
static int32_t task_C0[10];
static int32_t task_E8[10];
static int32_t task_110[10];
static int32_t task_138[10];
static int32_t task_160[10];
static int32_t task_188[10];
static int32_t task_1B0;
static int32_t task_1B4;
static Vector3 task_1B8;
static int32_t task_1C4;
static int32_t task_1C8;
static int32_t task_1CC;
static int8_t task_1D0;
static uint8_t task_1D1;
static uint16_t task_1D2;
static int8_t task_1D4;
static uint8_t task_1D5;
static uint16_t task_1D6;
static int32_t task_1D8;
static int32_t task_1DC;
static int8_t task_1E0;
static uint8_t task_1E1;
static uint16_t task_1E2;
static Vector3 task_1E4;
static int8_t task_1F0;
static int8_t task_1F1;
static int8_t task_1F2;
static uint8_t task_1F3;
static int8_t task_1F4;
static int8_t task_1F5;
static int8_t task_1F6;
static int8_t task_1F7;
static int8_t task_1F8;
static int8_t task_1F9;
static int8_t task_1FA;
static uint8_t task_1FB;
static int32_t task_1FC;
static int32_t task_200;
static int32_t task_204;
static int32_t task_208;
static int32_t task_20C;
static int32_t task_210;
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
static int32_t task_240;
static int8_t task_244;
static uint8_t task_245;
static uint16_t task_246;
static int8_t task_248;
static uint8_t task_249;
static uint16_t task_24A;
static int32_t task_24C;
static int32_t task_250;
static int8_t task_254;
static uint8_t task_255;
static int16_t task_256;
static int32_t task_258;
static int8_t task_25C;
static int8_t task_25D;
static int8_t task_25E;
static int8_t task_25F;
static int32_t task_260;
static int32_t task_264;
static int8_t task_268;
static int8_t task_269;
static int8_t task_26A;
static uint8_t task_26B;
static int16_t task_26C;
static uint16_t task_26E;
static int32_t task_270;
static int32_t task_274;
static int32_t task_278;
static int8_t task_27C;
static uint8_t task_27D;
static uint16_t task_27E;
static int32_t task_280[5];
static int32_t task_294;
static int32_t task_298;
static int32_t task_29C[8];
static int32_t task_2BC;
static int8_t task_2C0;
static int8_t task_2C1;
static int8_t task_2C2;
static int8_t task_2C3;
static int8_t task_2C4;
static int8_t task_2C5;
static int8_t task_2C6;
static uint8_t task_2C7;
static float task_2C8;
static int8_t task_2CC;
static int8_t task_2CD;
static int8_t task_2CE;
static int8_t task_2CF;
static int8_t task_2D0;
static int8_t task_2D1;
static int8_t task_2D2;
static int8_t task_2D3;
static int32_t task_2D4;
static int32_t task_2D8;
static int32_t task_2DC;
static int8_t task_2E0;
static uint8_t task_2E1;
static uint16_t task_2E2;
static const Vector3 data_1C964 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_1C970[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_1C980[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_1C98C = -1;
static Vector3 data_1C990 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static Vector3 data_1C99C = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const char data_1C9A8[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1C9AD[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_1C9B6[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_1C9BF[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_1C9C8[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_1C9D1[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_1C9DA[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_1C9E3[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_1C9EC[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_1C9F5[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_1C9FE[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_1CA07[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_1CA10[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_1CA19[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1CA1E[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1CA23[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_1CA2C[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1CA31[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1CA36[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_1CA3F[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_1CA48[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_1CA51[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1CA56[6] __attribute__((scn_string_literal)) = "E1067";
static const char data_1CA5C[13] __attribute__((scn_string_literal)) = "e1jukbox.snd";
static const char data_1CA69[35] __attribute__((scn_string_literal)) = "Sound Pack Loading In JUKEBOX.src\012";
static const char data_1CA8C[16] __attribute__((scn_string_literal)) = "Volume = %3d";
static const Vector4 data_1CA9C = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const char data_1CAAC[10] __attribute__((scn_string_literal)) = "E1067A011";
static const char data_1CAB6[10] __attribute__((scn_string_literal)) = "E1067A001";
static const char data_1CAC0[10] __attribute__((scn_string_literal)) = "E1067A002";
static const char data_1CACA[10] __attribute__((scn_string_literal)) = "E1067A003";
static const char data_1CAD4[10] __attribute__((scn_string_literal)) = "E1067A004";
static const char data_1CADE[10] __attribute__((scn_string_literal)) = "E1067A005";
static const char data_1CAE8[10] __attribute__((scn_string_literal)) = "E1067A006";
static const char data_1CAF2[10] __attribute__((scn_string_literal)) = "E1067A007";
static const char data_1CAFC[10] __attribute__((scn_string_literal)) = "E1067A008";
static const char data_1CB06[10] __attribute__((scn_string_literal)) = "E1067A009";
static const char data_1CB10[10] __attribute__((scn_string_literal)) = "E1067A011";
static const char data_1CB1A[10] __attribute__((scn_string_literal)) = "E1067A009";
static const char data_1CB24[10] __attribute__((scn_string_literal)) = "E1067A010";
static const char data_1CB2E[10] __attribute__((scn_string_literal)) = "E1067A011";
static const char data_1CB38[] __attribute__((scn_string_literal)) = "ykm003.snd";
static const char data_1CB43[] __attribute__((scn_string_literal)) = "ykm002.snd";
static const char data_1CB4E[] __attribute__((scn_string_literal)) = "bgm045.snd";
static const char data_1CB59[] __attribute__((scn_string_literal)) = "tsm002.snd";
static const char data_1CB64[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1CB6F[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1CB7A[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1CB85[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1CB90[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1CB9B[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const char data_1CBA6[] __attribute__((scn_string_literal)) = "ykm003.snd";
static const char data_1CBB1[] __attribute__((scn_string_literal)) = "ykm002.snd";
static const char data_1CBBC[] __attribute__((scn_string_literal)) = "bgm045.snd";
static const char data_1CBC7[] __attribute__((scn_string_literal)) = "tsm002.snd";
static const char data_1CBD2[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1CBDD[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1CBE8[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1CBF3[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1CBFE[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1CC09[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const char data_1CC14[] __attribute__((scn_string_literal)) = "bgm061.snd";
static const char data_1CC1F[] __attribute__((scn_string_literal)) = "bgm058.snd";
static const char data_1CC2A[] __attribute__((scn_string_literal)) = "bgm057.snd";
static const char data_1CC35[] __attribute__((scn_string_literal)) = "bgm056.snd";
static const char data_1CC40[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1CC4B[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1CC56[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1CC61[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1CC6C[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1CC77[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const char data_1CC82[] __attribute__((scn_string_literal)) = "bgm125.snd";
static const char data_1CC8D[] __attribute__((scn_string_literal)) = "bgm074.snd";
static const char data_1CC98[] __attribute__((scn_string_literal)) = "bgm053.snd";
static const char data_1CCA3[] __attribute__((scn_string_literal)) = "bgm124.snd";
static const char data_1CCAE[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1CCB9[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1CCC4[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1CCCF[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1CCDA[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1CCE5[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const char data_1CCF0[] __attribute__((scn_string_literal)) = "bgm054.snd";
static const char data_1CCFB[] __attribute__((scn_string_literal)) = "bgm092.snd";
static const char data_1CD06[] __attribute__((scn_string_literal)) = "bgm086.snd";
static const char data_1CD11[] __attribute__((scn_string_literal)) = "bgm048.snd";
static const char data_1CD1C[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1CD27[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1CD32[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1CD3D[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1CD48[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1CD53[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const char data_1CD5E[] __attribute__((scn_string_literal)) = "bgm021.snd";
static const char data_1CD69[] __attribute__((scn_string_literal)) = "bgm060.snd";
static const char data_1CD74[] __attribute__((scn_string_literal)) = "bgm090.snd";
static const char data_1CD7F[] __attribute__((scn_string_literal)) = "bgm072.snd";
static const char data_1CD8A[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1CD95[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1CDA0[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1CDAB[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1CDB6[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1CDC1[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const uint32_t data_1CDCC = 0xA300A2;
static const int32_t data_1CDD0[9] = {10813604, 10944678, 11075752, 11206826, 11337900, 11468974, 11600048, 11731122, 11862196};
static const uint32_t data_1CDF4 = 0xFFFF0000;
typedef struct Row_1CDF8 { int32_t w0; uint32_t w1; float w2; float w3; float w4; int32_t w5; int32_t w6; int32_t w7; } Row_1CDF8;
static const Row_1CDF8 data_1CDF8[10] = {
  {1, 0x71C74000U, 0.38f, 1.35f, -5.96f, 0, 0, -65536},
  {2, 0x71C74000U, 0.48f, 1.44f, -6.77f, 0, 0, -65536},
  {3, 0x71C74000U, 0.4f, 1.43f, 1.735f, 0, 0, -65536},
  {4, 0x71C70000U, 3.32f, 1.37f, -7.67f, 0, 0, -65536},
  {5, 0x71C70000U, 4.24f, 1.23f, -7.87f, 0, 0, -65536},
  {6, 0x71C70000U, 4.24f, 1.63f, -7.87f, 0, 0, -65536},
  {7, 0x71C7C000U, 4.9f, 1.47f, -4.04f, 0, 0, 589823},
  {49152, 0xCCCD71C7U, 1.34f, -5.345f, 0.0f, 0, 0, 655359},
  {49152, 0xCCCD71C7U, 1.51f, -4.7f, 0.0f, 0, 0, 720895},
  {49152, 0xCCCD71C7U, 1.13f, -3.48f, 0.0f, 0, 0, 786431}
};
static const int32_t data_1CF38 = 49152;
static const uint32_t data_1CF3C = 0xCCCD71C7U;
static const float data_1CF40[4] = {1.33f, -2.916f, 0.0f, 0.0f};
static const int32_t data_1CF50 = -1;
static const char data_1CF54[24] __attribute__((scn_string_literal)) = "search_control is die!\012";
static DoorMotionSelectorBlock data_1CF6C[38] = {
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
static ScnDoorMotionDescriptor data_1D55C[72] = {
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
static uint32_t data_1DF7C = 0xFFFFFFFF;
typedef struct Row_1DF80 { float w0; float w1; float w2; float w3; int32_t w4; } Row_1DF80;
static const Row_1DF80 data_1DF80[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_1E00C[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_1E01C)(int32_t, int32_t, int32_t) = script_3137;
static void (*const data_1E020)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E024)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E028)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E02C)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E030)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E034)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E038)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E03C)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E040)(int32_t, int32_t, int32_t, int32_t) = script_33ED;
static void (*const data_1E044)(int32_t, int32_t, int32_t, int32_t) = script_354E;
static void (*const data_1E048)(int32_t, int32_t, int32_t, int32_t) = script_3A1B;
static void (*const data_1E04C)(int32_t, int32_t, int32_t, int32_t) = script_3A82;
static void (*const data_1E050)(int32_t, int32_t, int32_t, int32_t) = script_324B;
static void (*const data_1E054)(int32_t, int32_t, int32_t, int32_t) = script_324B;
static void (*const data_1E058)(int32_t, int32_t, int32_t, int32_t) = script_36AF;
static void (*const data_1E05C)(int32_t, int32_t, int32_t, int32_t) = script_394D;
static void (*const data_1E060)(int32_t, int32_t, int32_t, int32_t) = script_3AE9;
static void (*const data_1E064)(int32_t, int32_t, int32_t, int32_t) = script_33ED;
static void (*const data_1E068)(int32_t, int32_t, int32_t, int32_t) = script_354E;
static void (*const data_1E06C)(int32_t, int32_t, int32_t, int32_t) = script_36AF;
static void (*const data_1E070)(int32_t, int32_t, int32_t, int32_t) = script_33ED;
static void (*const data_1E074)(int32_t, int32_t, int32_t, int32_t) = script_354E;
static void (*const data_1E078)(int32_t, int32_t, int32_t, int32_t) = script_36AF;
static void (*const data_1E07C)(int32_t, int32_t, int32_t, int32_t) = script_3B30;
static void (*const data_1E080)(int32_t, int32_t, int32_t, int32_t) = script_3B30;
static void (*const data_1E084)(int32_t, int32_t, int32_t, int32_t) = script_3B30;
static void (*const data_1E088)(int32_t, int32_t, int32_t, int32_t) = script_3B30;
static void (*const data_1E08C)(int32_t, int32_t, int32_t, int32_t) = script_3C66;
static void (*const data_1E090)(int32_t, int32_t, int32_t, int32_t) = script_4527;
static void (*const data_1E094)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E098)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E09C)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E0A0)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E0A4)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E0A8)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E0AC)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static void (*const data_1E0B0)(int32_t, int32_t, int32_t, int32_t) = script_3147;
static const char data_1E0B4[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_1E0E2[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_1E10D[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_1E138[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_1E162[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_1E18C[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_1E1B5[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_1E1DE[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_1E209[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_1E234[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_1E25E[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_1E288[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_1E2B1[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_1E2DA[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_1E305[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_1E330[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_1E35A[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_1E384[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_1E3AD[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_1E3D6[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_1E401[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_1E42C[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_1E456[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_1E480[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_1E4A9[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_1E4D2[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_1E4FD[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_1E528[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_1E552[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_1E57C[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_1E5A5[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_1E5CE[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_1E5F9[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_1E624[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_1E64E[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_1E678[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_1E6A1[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_1E6CA[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_1E6F5[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_1E720[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_1E74A[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_1E774[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_1E79D[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_1E7C6[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_1E7F1[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_1E81C[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_1E846[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_1E870[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_1E899[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_1E8C2[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_1E8ED[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_1E918[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_1E942[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_1E96C[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_1E995[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_1E9BE[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_1E9EA[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_1EA16[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_1EA42[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_1EA6E[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_1EA9A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1EAB6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1EAD2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1EAEE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1EB0A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1EB26[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1EB42[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1EB5E[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_1EB6B[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_1EB76[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_1EB95[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_1EBAB[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_1EBCD[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_1EBE6[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_1EBFF[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_1EC0E[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_1EC1D[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_1EC2F[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_1EC42[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_1EC54[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_1EC68 = {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU};
static const int32_t data_1EC9C = -1;
static const uint8_t data_1ECA0[4] = {0};
static const int32_t data_1ECA4 = -1;
static char data_1ECA8[] = "AUTO_DOOR";
static const uint8_t data_1ECB2[2] = {0};
static char *const data_1ECB4 = data_1ECA8;
static const int32_t data_1ECB8[3] = {17, 656043, 721579};
static const MapObjectRecord data_1ECC4 = {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {2.7f, 0.0f, 0.003f}, .yawFixed16 = 32768};
static const uint32_t data_1ECE8 = 380;
static const int32_t data_1ECEC[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_1ED04 = 510;
static const int32_t data_1ED08[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_1ED20 = 150;
static const int32_t data_1ED24[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_1ED3C = 530;
static const int32_t data_1ED40[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_1ED58 = 911;
static const int32_t data_1ED5C[6] = {1, 155, 0, 1015, 0, -1};
static const char data_1ED74[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_1ED7B[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_1ED82[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_1ED89[7] __attribute__((scn_string_literal)) = "FREE01";
static const int8_t data_1ED90[16] = {12, 17, 22, 10, 19, 18, 11, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const int8_t data_1EDA0[16] = {12, 1, 2, 1, 3, 2, 2, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const uint32_t data_1EDB0 = 0xA800A7;
static const int32_t data_1EDB4[5] = {8847529, 8978568, 15073509, 15204583, 15335657};
static const uint32_t data_1EDC8 = 0x440043;
static const int32_t data_1EDCC[8] = {4587589, 4718663, 4849737, 4980811, 5111885, 5242959, 5374033, 85};
static const uint32_t data_1EDEC = 0x5C005B;
static const int32_t data_1EDF0[10] = {6160477, 6291551, 6422625, 6553699, 6684773, 6815847, 6946921, 7077995, 7209069, 7340143};
static const uint32_t data_1EE18 = 0x740073;
static const int32_t data_1EE1C[4] = {7798902, 7929976, 8061050, 8257660};
static const uint32_t data_1EE2C = 0x720071;
static const int32_t data_1EE30[9] = {10223750, 10354845, 10485919, 10616993, 10748067, 10879141, 11206826, 5767255, 180};
static const uint32_t data_1EE54 = 0x810080;
static const int32_t data_1EE58[8] = {8585346, 11403437, 11534511, 11665585, 11862195, 11993270, 12452024, 8650879};
static const uint32_t data_1EE78 = 0xB900BC;
static const int32_t data_1EE7C[5] = {12255418, 13238473, 13500621, 12517579, 200};
static const uint32_t data_1EE90 = 0x5A0059;
static const int32_t data_1EE94[3] = {5636268, 14221445, 5505107};
static const uint32_t data_1EEA0 = 0x7D0075;
static const int32_t data_1EEA4 = 12386442;
static const uint32_t data_1EEA8 = 0xC600C5;
static const int32_t data_1EEAC = 13369543;
static const uint32_t data_1EEB0 = 0x8C008B;
static const int32_t data_1EEB4[8] = {9306253, 9437327, 9633937, 148, 9961623, 10092697, 9568411, 149};
static const uint32_t data_1EED4 = 0xD700D6;
static const int32_t data_1EED8[3] = {14287064, 14876891, 228};
static const uint32_t data_1EEE4 = 0xDC00D3;
static const int32_t data_1EEE8[2] = {13893845, 14549213};
static const uint32_t data_1EEF0 = 0xC100C0;
static const int32_t data_1EEF4[2] = {12779714, 196};
static const uint32_t data_1EEFC = 0xD000CF;
static const int32_t data_1EF00 = 13762769;
static const uint32_t data_1EF04 = 0xE000DF;
static const int32_t data_1EF08 = 14811361;
static const char data_1EF0C[169] = "%GACR016G.MT5\000%GACK66BG.MT5\000%GACK66RG.MT5\000%GACR0BNG.MT5\000%GACR0B2G.MT5\000%GACR0B3G.MT5\000%SPBK6REG.MT5\000%SPBK6BRG.MT5\000%SPBK6YEG.MT5\000%SAIK6WHG.MT5\000%SAIK6BKG.MT5\000%SAIK6REG.MT5";
static const char data_1EFB5[315] = "%GACIAK1G.MT5\000%GACIAK2G.MT5\000%GACOJK1G.MT5\000%GACOJK2G.MT5\000%GACOSA1G.MT5\000%GACOSA2G.MT5\000%GACOLA1G.MT5\000%GACOLA2G.MT5\000%GACOPA1G.MT5\000%GACOPA2G.MT5\000%GACGWF1G.MT5\000%GACGWF2G.MT5\000%GACGJF1G.MT5\000%GACGJF2G.MT5\000%GACIKA1G.MT5\000%GACIKA2G.MT5\000%GACM401G.MT5\000%GACIAA1G.MT5\000%GACIAA2G.MT5\000D\000D\000D\000D\000D\000D\000D\000D\000D\000D\000%GACGJJ1G.MT5\000%GACGJJ2G.MT5";
static const char data_1F0F0[309] = "%GACIAKAG.MT5\000%GACIAKBG.MT5\000%GACGJKAG.MT5\000%GACGJKBG.MT5\000%GACOSAAG.MT5\000%GACOSABG.MT5\000%GACOLAAG.MT5\000%GACOLABG.MT5\000%GACOPAAG.MT5\000%GACOPABG.MT5\000%GACIWLAG.MT5\000%GACIWLBG.MT5\000%GACGJFAG.MT5\000%GACGJFBG.MT5\000%GACIKAAG.MT5\000%GACIKABG.MT5\000%GACGSNAG.MT5\000%GACGSNBG.MT5\000%GACILIAG.MT5\000%GACILIBG.MT5\000%GACIDUAG.MT5\000%GACIDUBG.MT5";
static const char data_1F225[167] = "%GACRSO1G.MT5\000%GACRSO2G.MT5\000%GACRTL1G.MT5\000%GACRNUCG.MT5\000%GACRBAKG.MT5\000%GACRBING.MT5\000%GACRFNGG.MT5\000%GACRAMYG.MT5\000%GACRESPG.MT5\000%GACREGGG.MT5\000D\000D\000D\000D\000D\000D\000%GACRFNNG.MT5";
static const char data_1F2CC[365] = "%GACRNT1G.MT5\000%GACRNT2G.MT5\000%GACRLOPG.MT5\000%GACRMYUG.MT5\000%GACRCKNG.MT5\000%GACRALFG.MT5\000%GACRRISG.MT5\000%GACRARXG.MT5\000%GACR0OPG.MT5\000%GACR0O2G.MT5\000%GACRPOPG.MT5\000%GACRCOBG.MT5\000%GACRIDAG.MT5\000%GACRSHPG.MT5\000%GACRMOBG.MT5\000%GACRROBG.MT5\000%GACK6TZG.MT5\000%GACRPANG.MT5\000%GACK6DYG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRAXXG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRRBBG.MT5\000%GACK6TSG.MT5\000%GACRPPNG.MT5\000%GACK6DSG.MT5";
static const char data_1F439[505] = "%GACK6HHG.MT5\000%GACK6HYG.MT5\000%GACK6HBG.MT5\000%GACK6HGG.MT5\000%GACH5BKG.MT5\000%KAB02FSG.MT5\000%GACO1K2G.MT5\000%GACO1K3G.MT5\000%GACO1K7G.MT5\000%GACO1K8G.MT5\000%GACO1K9G.MT5\000%GACO1K1G.MT5\000%GACO1K4G.MT5\000%GACO1K5G.MT5\000%GACO1K6G.MT5\000%GACS5BSG.MT5\000%GACK6HOG.MT5\000%GACK6HKG.MT5\000%GACK6XHG.MT5\000%GACK6XYG.MT5\000%GACK6XBG.MT5\000%GACK6XGG.MT5\000%GACK6BKG.MT5\000%KABK6FSG.MT5\000%GACOKK2G.MT5\000%GACOKK3G.MT5\000%GACOKK7G.MT5\000%GACOKK8G.MT5\000%GACOKK9G.MT5\000%GACOKK1G.MT5\000%GACOKK4G.MT5\000%GACOKK5G.MT5\000%GACOKK6G.MT5\000%GACS5BBG.MT5\000%GACK6XOG.MT5\000%GACK6XKG.MT5";
static const char data_1F632[211] = "%GACH5TKG.MT5\000%GACGTRXG.MT5\000%GACO1T1G.MT5\000%GACO1T2G.MT5\000%GACO1KOG.MT5\000%GACO1KBG.MT5\000%GACO1KJG.MT5\000%GACH5UKG.MT5\000%GACH5IKG.MT5\000%GACO1FOG.MT5\000%GACGSHEG.MT5\000%GACH5TTG.MT5\000%GACGTXXG.MT5\000%GACOTT1G.MT5\000%GACOTT2G.MT5";
static const char data_1F705[113] = "%YKW0201G.MT5\000%YWT0201G.MT5\000%GACRBONG.MT5\000%GACRRENG.MT5\000%GACK6SPG.MT5\000%GACG360G.MT5\000%GACRDR1G.MT5\000%GACRDR2G.MT5";
static const char data_1F776[77] = "%GACRSSCG.MT5\000%GACRMETG.MT5\000%GACRGBIG.MT5\000%GACH5TRG.MT5\000D\000D\000D\000%GACH5RRG.MT5";
static const char data_1F7C3[57] = "%GACS5FRG.MT5\000%GACS5FBG.MT5\000%GACO1KUG.MT5\000%GACO1FEG.MT5";
static const char data_1F7FC[239] = "%GACODG1G.MT5\000%GACODG2G.MT5\000%GACODG3G.MT5\000%GACOCT2G.MT5\000%GACOCATG.MT5\000%GACO1SHG.MT5\000%GACOKSHG.MT5\000%GACO1IRG.MT5\000%GACOKIRG.MT5\000%GACO1HAG.MT5\000%GACOHA2G.MT5\000%GACOHA3G.MT5\000%GACOHA4G.MT5\000%GACO1YGG.MT5\000%GACO1USG.MT5\000%GACOOSHG.MT5\000%GACOOIRG.MT5";
static const char data_1F8EB[99] = "%GACM400G.MT5\000%GACGSPHG.MT5\000%GACS5HGG.MT5\000%GACGDT1G.MT5\000%GACS5EQG.MT5\000%SYOGSH0G.MT5\000%SYOGHG0G.MT5";
static const char data_1F94E[85] = "%GACGCRYG.MT5\000%GACGBIRG.MT5\000%GACH5SLG.MT5\000%GACH5JKG.MT5\000%SYOGMD1G.MT5\000%SYOGMD5G.MT5";
static const char data_1F9A3[71] = "%GACS5N1G.MT5\000%GACS5N2G.MT5\000%GACS5N3G.MT5\000%GACS5N4G.MT5\000%GACS5N5G.MT5";
static const char data_1F9EA[57] = "%GACGXBNG.MT5\000%GACGYBNG.MT5\000%GACGABNG.MT5\000%GACGBBNG.MT5";
static const char data_1FA23[57] = "%GACRMEGG.MT5\000%GACR0GGG.MT5\000%GACRSATG.MT5\000%GACRCASG.MT5";
static const char data_1FA5C[10] __attribute__((scn_string_literal)) = "E1025A043";
static const char data_1FA66[10] __attribute__((scn_string_literal)) = "E1025B005";
static const char data_1FA70[10] __attribute__((scn_string_literal)) = "E1025B006";
static const char data_1FA7A[10] __attribute__((scn_string_literal)) = "E1026A026";
static const char data_1FA84[10] __attribute__((scn_string_literal)) = "E1026B005";
static const char data_1FA8E[10] __attribute__((scn_string_literal)) = "E1026B006";
static const char data_1FA98[10] __attribute__((scn_string_literal)) = "E1027A025";
static const char data_1FAA2[10] __attribute__((scn_string_literal)) = "E1027C001";
static const char data_1FAAC[10] __attribute__((scn_string_literal)) = "E1027C002";
static const char data_1FAB6[10] __attribute__((scn_string_literal)) = "E1027C003";
static const char data_1FAC0[10] __attribute__((scn_string_literal)) = "E1027C004";
static const char data_1FACA[10] __attribute__((scn_string_literal)) = "E1027C005";
static const char data_1FAD4[12] __attribute__((scn_string_literal)) = "E1027C006";
static const Vector4 data_1FAE0 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const Vector3i data_1FAF0 = {.x = 0, .y = 16384, .z = 0};
static const char data_1FAFC[13] __attribute__((scn_string_literal)) = "NITO LOADING";
static const char data_1FB09[2] __attribute__((scn_string_literal)) = ".";
static const char data_1FB0B[6] __attribute__((scn_string_literal)) = "END!\012";
static const char data_1FB11[2] __attribute__((scn_string_literal)) = ".";
static const char data_1FB13[5] __attribute__((scn_string_literal)) = "\012";
static const uint32_t data_1FB18 = 0x3E4CCCCD;
typedef struct Row_1FB1C { int32_t w0; float w1; } Row_1FB1C;
static const Row_1FB1C data_1FB1C[5] = {
  {0, -4.6f},
  {81920, 0.2f},
  {0, -3.7f},
  {16384, 0.0f},
  {0, 0.0f}
};
static const int32_t data_1FB44 = 0;
static const uint32_t data_1FB48 = SHEN_NAME('DART');
static const shen_name data_1FB4C[2] = {SHEN_NAME('DAR2'), SHEN_NAME('DAR3')};
static const uint32_t data_1FB54 = SHEN_NAME('YA_Y');
static const shen_name data_1FB58[4] = {SHEN_NAME('YA_R'), SHEN_NAME('YA_B'), SHEN_NAME('YA_G'), SHEN_NAME('YA_K')};
static const char data_1FB68[16] __attribute__((scn_string_literal)) = "/scene/03/DGCT/";
static const char data_1FB78[5] __attribute__((scn_string_literal)) = "DART";
static const char data_1FB7D[13] __attribute__((scn_string_literal)) = "HMOT_DRT.BIN";
static const char data_1FB8A[5] __attribute__((scn_string_literal)) = "DART";
static const char data_1FB8F[6] __attribute__((scn_string_literal)) = "E1025";
static const char data_1FB95[6] __attribute__((scn_string_literal)) = "E1025";
static const char data_1FB9B[] __attribute__((scn_string_literal)) = "E1025A001";
static const char data_1FBA5[] __attribute__((scn_string_literal)) = "E1025A002";
static const char data_1FBAF[] __attribute__((scn_string_literal)) = "E1025A003";
static const char data_1FBB9[] __attribute__((scn_string_literal)) = "E1025A004";
static const char data_1FBC3[] __attribute__((scn_string_literal)) = "E1025A005";
static const char data_1FBCD[] __attribute__((scn_string_literal)) = "E1025A009";
static const char data_1FBD7[] __attribute__((scn_string_literal)) = "E1025A010";
static const char data_1FBE1[] __attribute__((scn_string_literal)) = "E1025A011";
static const char data_1FBEB[] __attribute__((scn_string_literal)) = "E1025A006";
static const char data_1FBF5[] __attribute__((scn_string_literal)) = "E1025A007";
static const char data_1FBFF[] __attribute__((scn_string_literal)) = "E1025A008";
static const uint8_t data_1FC09[3] = {0};
static const Vector3 data_1FC0C = {.x = 0.0882f, .y = -0.0112f, .z = 0.0f};
static const Vector3i data_1FC18 = {.x = 0, .y = 0, .z = 0};
static const uint32_t data_1FC24 = 0x3E21CAC1;
static const float data_1FC28[14] = {0.009f, 0.004f, 0.145f, 0.01f, -0.002f, 0.128f, 0.017f, -0.007f, 0.135f, 0.008f, 0.0f, 0.15f, 0.009f, 0.004f};
static const uint8_t data_1FC60[4] = {0};
static const int32_t data_1FC64[14] = {2001, 15469, 0, 2001, 15469, 0, 2001, 15469, 0, 2001, 15469, 0, 2001, 15469};
static const Vector4 data_1FC9C = {.x = 0.0f, .y = -0.3f, .z = -0.3f, .w = -0.3f};
static const char data_1FCAC[] __attribute__((scn_string_literal)) = "E1025A022";
static const char data_1FCB6[] __attribute__((scn_string_literal)) = "E1025A023";
static const char data_1FCC0[] __attribute__((scn_string_literal)) = "E1025A024";
static const uint8_t data_1FCCA[2] = {0};
static const Vector4 data_1FCCC = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const char data_1FCDC[] __attribute__((scn_string_literal)) = "E1025A016";
static const char data_1FCE6[] __attribute__((scn_string_literal)) = "E1025A017";
static const char data_1FCF0[] __attribute__((scn_string_literal)) = "E1025A013";
static const char data_1FCFA[] __attribute__((scn_string_literal)) = "E1025A018";
static const char data_1FD04[] __attribute__((scn_string_literal)) = "E1025A048";
static const char data_1FD0E[] __attribute__((scn_string_literal)) = "E1025A018";
static const char data_1FD18[] __attribute__((scn_string_literal)) = "E1025A046";
static const char data_1FD22[] __attribute__((scn_string_literal)) = "E1025A012";
static const char data_1FD2C[] __attribute__((scn_string_literal)) = "E1025A047";
static const char data_1FD36[] __attribute__((scn_string_literal)) = "E1025A014";
static const char data_1FD40[] __attribute__((scn_string_literal)) = "E1025A015";
static const char data_1FD4A[] __attribute__((scn_string_literal)) = "E1025A050";
static const char data_1FD54[] __attribute__((scn_string_literal)) = "E1025A051";
static const char data_1FD5E[] __attribute__((scn_string_literal)) = "E1025A052";
static const char data_1FD68[] __attribute__((scn_string_literal)) = "E1025A037";
static const char data_1FD72[] __attribute__((scn_string_literal)) = "E1025A038";
static const char data_1FD7C[] __attribute__((scn_string_literal)) = "E1025A039";
static const char data_1FD86[] __attribute__((scn_string_literal)) = "E1025A040";
static const char data_1FD90[] __attribute__((scn_string_literal)) = "E1025A041";
static const char data_1FD9A[] __attribute__((scn_string_literal)) = "E1025A042";
static const Vector3 data_1FDA4 = {.x = -0.05f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_1FDB0 = {.x = -0.05f, .y = 0.0f, .z = 0.0f};
static const uint32_t data_1FDBC = 0xBCCCCCCD;
static const uint8_t pad_1FDC0[8] = {0};
static const uint32_t data_1FDC8 = 0x3CB78034;
static const float data_1FDCC[2] = {2.155f, -0.04f};
static const uint32_t data_1FDD4 = 0x3E16F007;
static const float data_1FDD8[2] = {2.138f, -0.04f};
static const uint32_t data_1FDE0 = 0x4009EB85;
static const float data_1FDE4[4] = {2.005f, 1.855f, 1.705f, 1.855f};
static const uint32_t data_1FDF4 = 0x4008D4FE;
static const float data_1FDF8[4] = {1.988f, 1.838f, 1.688f, 1.838f};
static const Vector3 data_1FE08 = {.x = 0.5f, .y = 0.5f, .z = 0.5f};
static const char data_1FE14[] __attribute__((scn_string_literal)) = "E1025A025";
static const char data_1FE1E[] __attribute__((scn_string_literal)) = "E1025A026";
static const char data_1FE28[] __attribute__((scn_string_literal)) = "E1025A027";
static const char data_1FE32[] __attribute__((scn_string_literal)) = "E1025A028";
static const char data_1FE3C[] __attribute__((scn_string_literal)) = "E1025A029";
static const char data_1FE46[] __attribute__((scn_string_literal)) = "E1025A030";
static const char data_1FE50[] __attribute__((scn_string_literal)) = "E1025A011";
static const char data_1FE5A[] __attribute__((scn_string_literal)) = "E1025A031";
static const char data_1FE64[] __attribute__((scn_string_literal)) = "E1025A032";
static const uint8_t data_1FE6E[2] = {0};
static const Vector3 data_1FE70 = {.x = -0.1061f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_1FE7C = {.x = -0.0474f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_1FE88 = {.x = 0.1344f, .y = 1.91f, .z = -0.0388f};
static const Vector3 data_1FE94 = {.x = -0.0585f, .y = 1.7258f, .z = -0.0388f};
static const Vector3 data_1FEA0 = {.x = 0.1905f, .y = 1.7258f, .z = -0.0388f};
static const Vector3 data_1FEAC = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const Vector3 data_1FEB8 = {.x = 1.8f, .y = 1.6f, .z = 1.0f};
static const uint32_t data_1FEC4 = 153;
static const int32_t data_1FEC8[4] = {154, 155, 156, 157};
static const Vector4 data_1FED8 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const int8_t data_1FEE8[20] = {20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5};
static const Vector4 data_1FEFC = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_1FF0C = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
static const char data_1FF1C[] __attribute__((scn_string_literal)) = "E1026A001";
static const char data_1FF26[] __attribute__((scn_string_literal)) = "E1026A002";
static const char data_1FF30[] __attribute__((scn_string_literal)) = "E1026A006";
static const char data_1FF3A[] __attribute__((scn_string_literal)) = "E1026A007";
static const char data_1FF44[] __attribute__((scn_string_literal)) = "E1026A008";
static const char data_1FF4E[] __attribute__((scn_string_literal)) = "E1026A009";
static const char data_1FF58[] __attribute__((scn_string_literal)) = "E1026A010";
static const char data_1FF62[] __attribute__((scn_string_literal)) = "E1026A011";
static const char data_1FF6C[11] __attribute__((scn_string_literal)) = "rim008.snd";
static const char data_1FF77[] __attribute__((scn_string_literal)) = "E1026A019";
static const char data_1FF81[] __attribute__((scn_string_literal)) = "E1026A019";
static const char data_1FF8B[13] __attribute__((scn_string_literal)) = "rim008.snd";
static const Vector3i data_1FF98 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_1FFA4 = {.x = 0.123f, .y = 0.027f, .z = -0.071f};
static const Vector3i data_1FFB0 = {.x = 4096, .y = 0, .z = 12288};
static const char data_1FFBC[] __attribute__((scn_string_literal)) = "E1026A003";
static const char data_1FFC6[] __attribute__((scn_string_literal)) = "E1026A004";
static const char data_1FFD0[] __attribute__((scn_string_literal)) = "E1026A005";
static const char data_1FFDA[] __attribute__((scn_string_literal)) = "E1026A012";
static const char data_1FFE4[] __attribute__((scn_string_literal)) = "E1026A013";
static const char data_1FFEE[] __attribute__((scn_string_literal)) = "E1026A014";
static const Vector4 data_1FFF8 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const char data_20008[] __attribute__((scn_string_literal)) = "E1026B001";
static const char data_20012[] __attribute__((scn_string_literal)) = "E1026B002";
static const char data_2001C[] __attribute__((scn_string_literal)) = "E1026B003";
static const char data_20026[] __attribute__((scn_string_literal)) = "E1026B004";
static const char data_20030[] __attribute__((scn_string_literal)) = "E1026A015";
static const char data_2003A[] __attribute__((scn_string_literal)) = "E1026A016";
static const char data_20044[] __attribute__((scn_string_literal)) = "E1026A017";
static const char data_2004E[14] __attribute__((scn_string_literal)) = "tsm007.snd";
static const Vector3 data_2005C = {.x = -0.015f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_20068 = {.x = 1.414f, .y = 1.414f, .z = 1.414f};
static const Vector3 data_20074 = {.x = 0.75f, .y = 0.75f, .z = 0.75f};
static const Vector3 data_20080 = {.x = -0.015f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_2008C = {.x = 1.414f, .y = 1.414f, .z = 1.414f};
typedef struct Row_20098 { int32_t w0; int32_t w1; float w2; } Row_20098;
static const Row_20098 data_20098[4] = {
  {0, 0, 0.015f},
  {0, 0, -0.015f},
  {0, 0, 0.015f},
  {0, 0, -0.015f}
};
static const Vector4 data_200C8 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const char data_200D8[8] __attribute__((scn_string_literal)) = "E1026";
static const Vector3 data_200E0 = {.x = 0.541f, .y = 0.0f, .z = -6.4012f};
static const Vector3i data_200EC = {.x = 0, .y = 0, .z = 0};
static const char data_200F8[6] __attribute__((scn_string_literal)) = "E1026";
static const char data_200FE[24] __attribute__((scn_string_literal)) = "--------EX_QTE--------\012";
static const char data_20116[13] __attribute__((scn_string_literal)) = "HI_SCORE\011%d\012";
static const char data_20123[25] __attribute__((scn_string_literal)) = "----------------------\012";
static const uint32_t data_2013C = 262145;
static const int32_t data_20140 = 4194320;
static const uint32_t data_20144[2] = {0x2000100U, 0x8000400U};
static const int32_t data_2014C[57] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0
};
static const int32_t data_20230[57] = {
  0, -364, -11650, -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0,
  -18750, 0, 0, -546, 0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728
};
static const char data_20314[] __attribute__((scn_string_literal)) = "E1027A001";
static const char data_2031E[] __attribute__((scn_string_literal)) = "E1027A002";
static const char data_20328[] __attribute__((scn_string_literal)) = "E1027A009";
static const char data_20332[] __attribute__((scn_string_literal)) = "E1027A010";
static const char data_2033C[] __attribute__((scn_string_literal)) = "E1027A011";
static const uint8_t data_20346[2] = {0};
static const Vector3 data_20348 = {.x = 0.129f, .y = 0.023f, .z = -0.076f};
static const Vector3i data_20354 = {.x = 455, .y = 0, .z = 43235};
static const char data_20360[] __attribute__((scn_string_literal)) = "E1027A003";
static const char data_2036A[] __attribute__((scn_string_literal)) = "E1027A004";
static const char data_20374[] __attribute__((scn_string_literal)) = "E1027A005";
static const char data_2037E[11] __attribute__((scn_string_literal)) = "bgm008.snd";
static const char data_20389[] __attribute__((scn_string_literal)) = "E1027A006";
static const char data_20393[] __attribute__((scn_string_literal)) = "E1027A007";
static const char data_2039D[] __attribute__((scn_string_literal)) = "E1027A008";
static const char data_203A7[] __attribute__((scn_string_literal)) = "qtebat.snd";
static const char data_203B2[22] __attribute__((scn_string_literal)) = "\012ＱＴＥ発生えらー\012";
static const Vector4 data_203C8 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const char data_203D8[8] __attribute__((scn_string_literal)) = "E1027";
static const Vector3 data_203E0 = {.x = 1.24f, .y = 0.0f, .z = -6.2612f};
static const Vector3i data_203EC = {.x = 0, .y = 0, .z = 0};
static const char data_203F8[6] __attribute__((scn_string_literal)) = "E1027";
static const char data_203FE[24] __attribute__((scn_string_literal)) = "--------TITLE---------\012";
static const char data_20416[13] __attribute__((scn_string_literal)) = "HI_SCORE\011%d\012";
static const char data_20423[24] __attribute__((scn_string_literal)) = "----------------------\012";
static const char data_2043B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_20440[] __attribute__((scn_string_literal)) = "E1027A001";
static const char data_2044A[] __attribute__((scn_string_literal)) = "E1027A023";
static const char data_20454[] __attribute__((scn_string_literal)) = "E1027A026";
static const char data_2045E[] __attribute__((scn_string_literal)) = "E1027A027";
static const char data_20468[] __attribute__((scn_string_literal)) = "E1027A009";
static const char data_20472[] __attribute__((scn_string_literal)) = "E1027A010";
static const char data_2047C[] __attribute__((scn_string_literal)) = "E1027A011";
static const char data_20486[] __attribute__((scn_string_literal)) = "E1027A009";
static const char data_20490[] __attribute__((scn_string_literal)) = "E1027A010";
static const char data_2049A[] __attribute__((scn_string_literal)) = "E1027A003";
static const char data_204A4[] __attribute__((scn_string_literal)) = "E1027A004";
static const char data_204AE[] __attribute__((scn_string_literal)) = "E1027A005";
static const char data_204B8[] __attribute__((scn_string_literal)) = "E1027A006";
static const char data_204C2[] __attribute__((scn_string_literal)) = "E1027A007";
static const char data_204CC[] __attribute__((scn_string_literal)) = "E1027A008";
static const uint8_t data_204D6[2] = {0};
static const Vector4 data_204D8 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const char data_204E8[6] __attribute__((scn_string_literal)) = "E1027";
static const char data_204EE[6] __attribute__((scn_string_literal)) = "E1027";
static const char data_204F4[] __attribute__((scn_string_literal)) = "E1027A001";
static const char data_204FE[] __attribute__((scn_string_literal)) = "E1027A024";
static const char data_20508[] __attribute__((scn_string_literal)) = "E1027A026";
static const char data_20512[] __attribute__((scn_string_literal)) = "E1027A027";
static const char data_2051C[] __attribute__((scn_string_literal)) = "E1027A009";
static const char data_20526[] __attribute__((scn_string_literal)) = "E1027A010";
static const char data_20530[] __attribute__((scn_string_literal)) = "E1027A011";
static const char data_2053A[] __attribute__((scn_string_literal)) = "E1027A009";
static const char data_20544[] __attribute__((scn_string_literal)) = "E1027A010";
static const char data_2054E[] __attribute__((scn_string_literal)) = "E1027A010";
static const char data_20558[] __attribute__((scn_string_literal)) = "E1027A003";
static const char data_20562[] __attribute__((scn_string_literal)) = "E1027A004";
static const char data_2056C[] __attribute__((scn_string_literal)) = "E1027A005";
static const char data_20576[] __attribute__((scn_string_literal)) = "E1027A006";
static const char data_20580[] __attribute__((scn_string_literal)) = "E1027A007";
static const char data_2058A[] __attribute__((scn_string_literal)) = "E1027A008";
static const Vector4 data_20594 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const char data_205A4[8] __attribute__((scn_string_literal)) = "E1027";
static const Vector3 data_205AC = {.x = 2.57f, .y = 0.0f, .z = -1.38f};
static const int32_t data_205B8[3] = {0, 16384, 0};
static const char data_205C4[24] = "E1027\000\000\000,\004\023\004-\004/\0042\0040\0041\004\207\003";
static const char data_205DC[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_205F0[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_20604[13] __attribute__((scn_string_literal)) = "HMOT_MIN.BIN";
static const char data_20611[] __attribute__((scn_string_literal)) = "E1025";
static const char data_20617[] __attribute__((scn_string_literal)) = "E1026";
static const char data_2061D[] __attribute__((scn_string_literal)) = "E1027";
static const char data_20623[] __attribute__((scn_string_literal)) = "E1027";
static const char data_20629[] __attribute__((scn_string_literal)) = "E1027";
static const char data_2062F[] __attribute__((scn_string_literal)) = "E1027";
static const char data_20635[] __attribute__((scn_string_literal)) = "E1027";
static const char data_2063B[] __attribute__((scn_string_literal)) = "E1027";
static const char data_20641[] __attribute__((scn_string_literal)) = "E1027";
static const char data_20647[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_20652[14] __attribute__((scn_string_literal)) = "scene/03/D000";
static const char data_20660[] __attribute__((scn_string_literal)) = "CYCLEMAN.BIN";
static const char data_2066D[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_20673[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_20679[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_2067F[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_20685[13] __attribute__((scn_string_literal)) = "HMOT_JBX.BIN";
static const char data_20692[18] __attribute__((scn_string_literal)) = "/scene/03/DGCT/";
static const uint32_t data_206A4 = SHEN_NAME('DART');
static const shen_name data_206A8 = SHEN_NAME('DAR2');
static uint32_t data_206AC = 0x3E4CCCCD;
static const float data_206B0[15] = {-4.6f, 0.2f, -3.7f, 1.24f, -7.292f, 0.355f, -7.35f, 2.2f, -6.95f, 2.6f, -6.95f, 1.69f, -1.4f, 1.3315f, -0.925f};
static const Vector3 data_206EC = {.x = 1.8f, .y = 0.0f, .z = -0.51f};
static const Vector3 data_206F8 = {.x = 3.6f, .y = 0.0f, .z = 0.52f};

static void script_0030(int32_t arg0) {
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

static int32_t script_005C(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int32_t local_F8;
  (void)((local_FF = 0) & 4);
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg1 >= arg0) {
    (void)((local_FF = -1) & 4);
  }
  return local_FF & 0x80;
}

static int8_t script_008F(int32_t arg0, int32_t arg1, int32_t arg2) {
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
      goto L0000FD;
    }
  }
  L0000FD:;
  local_F8 = local_F8 + local_F4;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_F8 = local_F8 + 69000;
    (void)((local_F3 = 0) & 1);
  } else if ((local_EC >= 2202) & (local_EC <= 2204)) {
    (void)((local_F3 = 0) & 1);
    local_F8 = local_F4 = 0;
    local_FF = 4;
  } else {
    (void)((local_F3 = -(script_005C(local_F8, arg1) != 0)) & 0x10);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) == 1) {
      local_F8 = local_F4 = 0;
      (void)((local_F3 = 0) & 1);
      local_FF = 5;
    }
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
      script_01FA(local_FF, arg0);
    }
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_1C9A8, 8, data_1C9EC, 9, data_1C9F5, 10, data_1C9FE, 11, data_1CA07, 12, data_1CA10, 13, data_1CA19)))
static void script_01FA(int32_t arg0, int32_t arg1) {
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

__attribute__((scn_string_bindings(0, data_1CA1E, 1, data_1CA23, 2, data_1CA2C, 3, data_1CA31, 4, data_1CA36, 5, data_1CA3F, 6, data_1CA48, 7, data_1CA51)))
static void script_03C3(int32_t arg0, int32_t arg1) {
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

static void script_0599(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  task_3C = arg1;
  task_40 = arg0;
  local_E4 = task_3C;
  switch (local_E4) {
    case 0:
      task_48 = 1;
      break;
    case 1:
      task_48 = 1;
      break;
    case 2:
      task_48 = 0;
      break;
    case 3:
      task_48 = 0;
      break;
    case 4:
      task_48 = 0;
      break;
    case 5:
      task_48 = 0;
      break;
  }
  Script_ccowUtils(SHEN_NAME('JBOX'), 2);
  Script_MoveObjectToObject(SHEN_NAME('JCNT'), 0x584F424A);
  Script_MoveObjectToObject(SHEN_NAME('ASTD'), 0x584F424A);
  SCNF_WaitFrames_0000(1);
  switch (task_40) {
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
  Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &data_1C990, 0, 3);
  if (task_48 == 1) {
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
    (&local_E8)->y = (&local_E8)->y - 3094;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
    local_E8.x = (&local_E8)->x + 236;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
  }
  Script_scenCreateTask(script_0A18, 0);
  task_20 = Script_scenCreateTask(script_22C2, 0);
  task_28 = Script_scenCreateTask(script_2320, 0);
  if (task_48 == 1) {
    task_F = 1;
    task_11 = 1;
  }
  Script_LoadSND(0, 5, "e1jukbox.snd", 0);
  Script_sprintf("Sound Pack Loading In JUKEBOX.src\012");
  (void)((task_4A = 0) & 1);
  return;
}

static void script_0951(void) {
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

static int32_t script_0A18(void) {
  int8_t local_FF;
  (void)((local_FF = 0) & 2);
  for (;;) {
    if (script_287D()) {
      if (Script_GetControllerInput(1) == 0x400) {
        (void)((local_FF = 0) & 2);
        script_0B58();
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
        Script_uiUtils(0, 37, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_0AAD(float arg0, float arg1, float arg2, int32_t arg3) {
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

static void script_0AF7(int32_t arg0) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int8_t script_0B11(void) {
  int8_t local_FF;
  local_FF = script_008F(0, 100, 1);
  return local_FF;
}

static int8_t script_0B29(void) {
  int8_t local_FF;
  uint8_t unused_F8[7];
  GameDateTime local_F0;
  local_FF = 0;
  Script_ReturnTimeDate(&local_F0);
  if (((&local_F0)->hours >= 23) & ((&local_F0)->minutes >= 15)) {
    local_FF = 1;
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_1CAAC, 9, data_1CB06, 10, data_1CB10, 11, data_1CB1A, 13, data_1CB2E)))
static void script_0B58(void) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  if (script_296F(0) == 0) {
    if (~task_4A) {
      (void)((task_4A = -1) & 8);
      Script_ShowUIMessageFromID("E1067A011");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else {
    Script_uiUtils(0, 7, 0);
    script_10F9();
    if (task_3C == 2) {
      Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1CA9C);
    }
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_EVT_GetPosition(SHEN_NAME('JBOX'), -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 1);
    L000BFA:;
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
      if (script_0B11() < 2) {
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
          goto L000FB4;
        }
        if (script_0B29()) {
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
          goto L000FB4;
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
        script_0AF7(CHARA_AKIR);
        Script_EVT_GetPosition(SHEN_NAME('D_TE'), -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(CHARA_AKIR, 11, &local_EC, 1);
        local_F8 = Script_shenGetStats(STATS_CASH);
        SCNF_SetCashValue(-100, 1);
        local_F8 = local_F8 - 100;
        Script_SetStats(STATS_CASH, local_F8);
        Script_shenClearFreeState(1);
        Script_uiUtils(0, 7, 0);
        script_1279();
        task_49 = 1;
        goto L000FB4;
      }
      goto L000FB4;
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
      goto L000FB4;
    }
    SCNF_WaitFrames_0000(1);
    goto L000BFA;
    L000FB4:;
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
    Script_uiUtils(0, 37, 0);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}

static void script_104C(const char *arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, arg2, arg0, 0x8000);
  task_F = 1;
  task_11 = 1;
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  task_10 = 4;
  SCNF_WaitFrames_0000(20);
  Script_executeAicaCommand(132265, 0, 0);
  SCNF_WaitFrames_0000(40);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, arg2, arg3);
  Script_PlayerStatsUtils(PS_SetIndexedU8Table1Slot, arg1, Script_PlayerStatsUtils(PS_GetIndexedU8Table1Slot, arg1) + 1);
  task_48 = 1;
  return;
}

static void script_10F9(void) {
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
  script_0AAD((&local_F4)->x, (&local_F4)->y, (&local_F4)->z, (&local_C4)->y);
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

static void script_1279(void) {
  L00127B:;
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_sicnUtils(0, 37, 1);
  Script_seqEnterEvtCam(152, SHEN_NAME('D_TE'), SHEN_NAME('D_TE'));
  task_4C = 1;
  Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 852, 0, 0, 0, 0, 1.0f, 0);
  Script_SetCameraFlags(-1);
  script_1315();
  Script_sicnUtils(1, 0, 0);
  Script_shenClearFreeState(0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  goto L001314;
  SCNF_WaitFrames_0000(1);
  goto L00127B;
  L001314:;
  return;
}

__attribute__((scn_string_bindings(0, data_1CB38, 1, data_1CB43, 2, data_1CB4E, 3, data_1CB59, 4, data_1CB64, 5, data_1CB6F, 6, data_1CB7A, 7, data_1CB85, 8, data_1CB90, 9, data_1CB9B, 10, data_1CBA6, 11, data_1CBB1, 12, data_1CBBC, 13, data_1CBC7, 14, data_1CBD2, 15, data_1CBDD, 16, data_1CBE8, 17, data_1CBF3, 18, data_1CBFE, 19, data_1CC09, 24, data_1CC40, 25, data_1CC4B, 26, data_1CC56, 27, data_1CC61, 28, data_1CC6C, 29, data_1CC77, 34, data_1CCAE, 35, data_1CCB9, 36, data_1CCC4, 37, data_1CCCF, 38, data_1CCDA, 39, data_1CCE5, 44, data_1CD1C, 45, data_1CD27, 46, data_1CD32, 47, data_1CD3D, 48, data_1CD48, 49, data_1CD53, 54, data_1CD8A, 55, data_1CD95, 56, data_1CDA0, 57, data_1CDAB, 58, data_1CDB6, 59, data_1CDC1)))
static void script_1315(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  int8_t local_E7;
  uint8_t unused_E4[3];
  int32_t local_E0;
  switch (task_40) {
    case 0:
      task_4B = 10;
      break;
    case 1:
      task_4B = 4;
      break;
    case 2:
      task_4B = 2;
      break;
  }
  for (;;) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
      break;
    }
    if ((Script_GetControllerInput(0) == 0x8000) | (Script_GetControllerInput(0) == 0x2000) | (Script_GetControllerInput(0) == 0xA000)) {
      task_0 = 0;
      task_1 = 0;
      task_2 = 0;
      local_E7 = 0;
      if (task_40 == 0) {
        if (task_4B <= 19) {
          while (local_E7 < 16) {
            Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
            local_F4.x = (&local_F4)->x + 0.002f;
            Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
            local_E7 = local_E7 + 1;
            SCNF_WaitFrames_0000(1);
          }
          task_4B = task_4B + 1;
          if (task_4B >= 20) {
            task_4B = 20;
          }
        }
      } else {
        if (task_4B <= 9) {
          while (local_E7 < 16) {
            Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
            local_F4.x = (&local_F4)->x + 0.002f;
            Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
            local_E7 = local_E7 + 1;
            SCNF_WaitFrames_0000(1);
          }
          task_4B = task_4B + 1;
          if (task_4B >= 10) {
            task_4B = 10;
          }
        }
      }
    }
    if ((Script_GetControllerInput(0) == 0x4000) | (Script_GetControllerInput(0) == 0x1000) | (Script_GetControllerInput(0) == 0x5000)) {
      task_0 = 0;
      task_1 = 0;
      task_2 = 0;
      local_E7 = 0;
      if (task_4B >= 2) {
        while (local_E7 < 16) {
          Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
          local_F4.x = (&local_F4)->x - 0.002f;
          Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
          local_E7 = local_E7 + 1;
          SCNF_WaitFrames_0000(1);
        }
        task_4B = task_4B - 1;
        if (task_4B <= 1) {
          task_4B = 1;
        }
      }
    }
    if (Script_GetControllerInput(1) == 0x400) {
      task_0 = 0;
      task_1 = 0;
      task_2 = 0;
      if (task_4B >= 11) {
        script_1E4A();
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
          script_1E4D(1);
          SCNF_WaitFrames_0000(1);
        }
        Script_executeAicaCommand(1193, 0, 0);
        local_E7 = 0;
        while (local_E7 < 10) {
          Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
          (&local_F4)->y = (&local_F4)->y + 0.0015f;
          Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
          local_E7 = local_E7 + 1;
          script_1E4D(0);
          SCNF_WaitFrames_0000(1);
        }
        if (task_44 != 0) {
          Script_srchControl((void *)task_44);
        }
        Script_executeAicaCommand(197801, 0, 0);
        task_38 = 0;
        script_1EFF();
        for (;;) {
          if (task_38 == 1) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        local_E0 = task_3C;
        switch (local_E0) {
          case 0:
            local_E0 = task_4B;
            switch (local_E0) {
              case 1:
                script_104C("ykm003.snd", 0, 2, 100);
                break;
              case 2:
                script_104C("ykm002.snd", 1, 2, 100);
                break;
              case 3:
                script_104C("bgm045.snd", 2, 2, 100);
                break;
              case 4:
                script_104C("tsm002.snd", 3, 2, 100);
                break;
              case 5:
                script_104C("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_104C("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_104C("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_104C("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_104C("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_104C("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 1:
            local_E0 = task_4B;
            switch (local_E0) {
              case 1:
                script_104C("ykm003.snd", 0, 2, 100);
                break;
              case 2:
                script_104C("ykm002.snd", 1, 2, 100);
                break;
              case 3:
                script_104C("bgm045.snd", 2, 2, 100);
                break;
              case 4:
                script_104C("tsm002.snd", 3, 2, 100);
                break;
              case 5:
                script_104C("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_104C("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_104C("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_104C("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_104C("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_104C("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 2:
            local_E0 = task_4B;
            switch (local_E0) {
              case 1:
                script_104C("bgm061.snd", 4, 2, 100);
                break;
              case 2:
                script_104C("bgm058.snd", 5, 2, 100);
                break;
              case 3:
                script_104C("bgm057.snd", 6, 2, 100);
                break;
              case 4:
                script_104C("bgm056.snd", 7, 2, 100);
                break;
              case 5:
                script_104C("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_104C("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_104C("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_104C("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_104C("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_104C("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 3:
            local_E0 = task_4B;
            switch (local_E0) {
              case 1:
                script_104C("bgm125.snd", 8, 2, 100);
                break;
              case 2:
                script_104C("bgm074.snd", 9, 2, 100);
                break;
              case 3:
                script_104C("bgm053.snd", 10, 2, 100);
                break;
              case 4:
                script_104C("bgm124.snd", 11, 2, 100);
                break;
              case 5:
                script_104C("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_104C("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_104C("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_104C("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_104C("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_104C("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 4:
            local_E0 = task_4B;
            switch (local_E0) {
              case 1:
                script_104C("bgm054.snd", 12, 2, 100);
                break;
              case 2:
                script_104C("bgm092.snd", 13, 2, 100);
                break;
              case 3:
                script_104C("bgm086.snd", 14, 2, 100);
                break;
              case 4:
                script_104C("bgm048.snd", 15, 2, 100);
                break;
              case 5:
                script_104C("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_104C("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_104C("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_104C("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_104C("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_104C("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 5:
            local_E0 = task_4B;
            switch (local_E0) {
              case 1:
                script_104C("bgm021.snd", 16, 2, 100);
                break;
              case 2:
                script_104C("bgm060.snd", 17, 2, 100);
                break;
              case 3:
                script_104C("bgm090.snd", 18, 2, 100);
                break;
              case 4:
                script_104C("bgm072.snd", 19, 2, 100);
                break;
              case 5:
                script_104C("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_104C("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_104C("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_104C("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_104C("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_104C("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
        }
        SCNF_WaitFrames_0000(40);
        script_1FCD();
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1E4A(void) {
  return;
}

static void script_1E4D(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  int32_t local_DC;
  local_E4 = -0.0015f;
  local_E0 = -0.00069f;
  local_DC = task_4B - 1;
  if ((local_DC > 20) | (local_DC < 0)) {
  } else {
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), *(int16_t *)((int8_t *)&data_1CDCC + (local_DC << 1)), &local_F4, &local_E8, 2);
    if (arg0) {
      (&local_F4)->y = (&local_F4)->y + local_E4;
      (&local_F4)->z = (&local_F4)->z + local_E0;
    } else {
      (&local_F4)->y = (&local_F4)->y - local_E4;
      (&local_F4)->z = (&local_F4)->z - local_E0;
    }
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), *(int16_t *)((int8_t *)&data_1CDCC + (local_DC << 1)), &local_F4, &local_E8, 0);
  }
  return;
}

static void script_1EFF(void) {
  Script_executeAicaCommand(0x1200A0, 2, 0);
  task_D = 0;
  task_E = 0;
  task_F = 0;
  task_11 = 0;
  task_18 = Script_scenCreateTask(script_20CB, 0);
  task_1C = Script_scenCreateTask(script_21D6, 0);
  task_24 = Script_scenCreateTask(script_2384, 0);
  task_2C = Script_scenCreateTask(script_256F, 0);
  task_30 = Script_scenCreateTask(script_2615, 0);
  task_34 = Script_scenCreateTask(script_26A1, 0);
  if (task_48 == 1) {
    task_10 = 1;
  } else {
    task_D = 1;
    Script_executeAicaCommand(394409, 0, 0);
    task_E = 1;
    Script_executeAicaCommand(66729, 0, 0);
  }
  return;
}

static void script_1FCD(void) {
  Script_srchControl((void *)task_18);
  Script_srchControl((void *)task_1C);
  Script_srchControl((void *)task_24);
  Script_srchControl((void *)task_2C);
  Script_srchControl((void *)task_30);
  Script_srchControl((void *)task_34);
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
  Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &data_1C990, 0, 3);
  Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &data_1C99C, 0, 3);
  return;
}

static int32_t script_20CB(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_D;
    switch (local_E0) {
      case 1:
        if (local_E4 < 90) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, 2);
          local_E8.x = (&local_E8)->x - 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_12 = 1;
          Script_executeAicaCommand(525481, 0, 0);
          task_13 = 1;
          task_14 = 1;
          task_D = 0;
        }
        break;
      case 2:
        if (local_E4 < 90) {
          if (local_E4 == 44) {
            task_E = 2;
          }
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, 2);
          local_E8.x = (&local_E8)->x + 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_D = 0;
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_21D6(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_E;
    switch (local_E0) {
      case 1:
        if (local_E4 < 45) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 154, &local_F4, &local_E8, 2);
          (&local_E8)->z = (&local_E8)->z + 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 154, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_E = 0;
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
          task_10 = 3;
          task_E = 0;
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_22C2(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  for (;;) {
    local_E0 = task_F;
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

static int32_t script_2320(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  for (;;) {
    local_E0 = task_11;
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

static int32_t script_2384(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_10;
    switch (local_E0) {
      case 1:
        task_F = 0;
        task_11 = 0;
        if (local_E4 < 13) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
          local_E8.x = (&local_E8)->x - 18;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_10 = 2;
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
          task_10 = 0;
          task_D = 1;
          Script_executeAicaCommand(394409, 0, 0);
          task_E = 1;
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
          task_10 = 0;
          task_38 = 1;
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
          task_10 = 0;
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_256F(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_12;
    switch (local_E0) {
      case 1:
        if (local_E4 < 32) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 159, &local_F4, &local_E8, 2);
          (&local_E8)->z = (&local_E8)->z - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 159, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_12 = 0;
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

static int32_t script_2615(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_13;
    switch (local_E0) {
      case 1:
        if (local_E4 < 32) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &local_F4, &local_E8, 2);
          (&local_E8)->z = (&local_E8)->z - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_13 = 0;
        }
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_26A1(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_14;
    switch (local_E0) {
      case 1:
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &data_1C99C, 0, 3);
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 155, &data_1C990, 0, 3);
        if (local_E4 < 32) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &local_F4, &local_E8, 2);
          (&local_E8)->z = (&local_E8)->z - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_14 = 0;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &data_1C990, 0, 3);
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 155, &data_1C99C, 0, 3);
          task_D = 2;
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

static void script_2799(void) {
  int32_t local_FC;
  Script_aicaGetOrSetState(8);
  for (;;) {
    local_FC = Script_aicaGetOrSetState(8);
    if (local_FC == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ReturnTimeDate(&task_4);
  if ((task_4.hours >= 23) & (task_4.minutes >= 30)) {
    Script_executeAicaCommand(0x1200A0, 2, 0);
    while (Script_aicaGetOrSetState(2)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchControl(0);
  return;
}

static void script_280E(void) {
  Script_SetShouldSleepFlag(1);
  Script_TimedTransitionUtils(TIMED_TRANSITION_SET_ENABLED, 1);
  return;
}

static void script_2820(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (task_48 == 0) {
  } else {
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
    local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
    task_F = 0;
    task_11 = 0;
    task_48 = 0;
  }
  return;
}

static int32_t script_287D(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Vector3 local_D8;
  Vector3i local_CC;
  Vector3 local_C0;
  local_FC = 0;
  if (task_4A & 0x80) {
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

static int32_t script_296F(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  GameDateTime local_F0;
  int32_t local_EC;
  local_FC = 1;
  Script_ReturnTimeDate(&local_F0);
  local_EC = arg0;
  if (local_EC == 0) {
    if (((&local_F0)->hours >= 23) & ((&local_F0)->minutes >= 15)) {
      local_FC = 0;
    }
  } else {
    local_FC = 0;
  }
  return local_FC;
}

static int32_t script_29B6(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_1CDF4);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    local_FC = Script_srchUtils(-1);
    local_F8 = local_FC;
    SCNF_WaitFrames_0000(1);
  }
  Script_sprintf("search_control is die!\012");
  return Script_srchControl(0);
}

static int32_t script_29FF(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_2A1D(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_2A3C(int32_t arg0, int32_t arg1) {
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

static void script_2AB4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_1B0 != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_98[arg2], arg3, arg4);
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

static int32_t script_3066(int32_t arg0) {
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

static void script_3137(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_BE5A(arg0, arg2, 0);
  return;
}

static void script_3147(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_B5EF(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_68D3(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_BE5A(arg0, arg2, 0);
  return;
}

static void script_324B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_B5EF(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_98[arg2]);
    script_68D3(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_BE5A(arg0, arg2, 0);
  return;
}

static void script_33ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_B5EF(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 4) == 0) {
      script_4DE0(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1B0) {
        script_81B3(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_BE5A(arg0, arg2, 2);
  return;
}

static void script_354E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_B5EF(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 4) == 0) {
      script_4DE0(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1B0) {
        script_81B3(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_BE5A(arg0, arg2, 2);
  return;
}

static void script_36AF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if ((Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_B5EF(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_98[arg2]);
      script_4DE0(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_1B0) {
        float local_F4;
        float local_F0;
        local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_B5EF(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_98[arg2]);
        script_81B3(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_BE5A(arg0, arg2, 2);
  return;
}

static void script_394D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 6);
    script_B5EF(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_BE5A(arg0, arg2, 0);
  return;
}

static void script_3A1B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_B5EF(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_5D5F(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_BE5A(arg0, arg2, 1);
  return;
}

static void script_3A82(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_B5EF(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_5D5F(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_BE5A(arg0, arg2, 1);
  return;
}

static void script_3AE9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    script_B5EF(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_5D5F(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_BE5A(arg0, arg2, 1);
  return;
}

static void script_3B30(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_B5EF(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_B5EF(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_6219(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_BE5A(arg0, arg2, 2);
  return;
}

static void script_3C66(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  if (arg0 == task_1B0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_FF7F = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_B5EF(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_BCB8(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_1B8)->x;
      (&local_9C)->y = task_1B8.y;
      (&local_9C)->z = task_1B8.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, 2);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, 2);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_1B0) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_1B8, *(&task_1C4 + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_1B8, *(&task_1C4 + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1B0) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1B0) {
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
      script_BCB8(arg0, 1);
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_1B0) {
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
    script_BE5A(arg0, arg2, 0);
  } else {
    script_324B(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_4527(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_B5EF(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_B5EF(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_88D4(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_BE5A(arg0, arg2, 0);
  return;
}

static int32_t script_4645(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_F4 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 6);
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
        local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_B5EF(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L004801;
            }
          }
          L004801:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_1C4 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_1B8)->x, (&local_E4)->z - task_1B8.z);
        script_905E(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1DF7C, 5, Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1DF7C, 5, Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_B5EF(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_98[arg2]);
      script_9136(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1DF7C, 5, Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1DF7C, 5, Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_B5EF(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_98[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_95AD(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1DF7C, 5, Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1DF7C, 5, Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_B5EF(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_98[arg2]);
          script_9136(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L004CFC;
      }
    }
  }
  L004CFC:;
  script_BE5A(arg0, arg2, 0);
  end:;
}

static void script_4D0A(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_3066(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1EA9A)))
static void script_4DE0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_1B0) {
        local_FF78 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_2AB4(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_BCB8(arg0, 0);
    local_C0.x = (&task_1B8)->x;
    (&local_C0)->y = task_1B8.y;
    (&local_C0)->z = task_1B8.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1B8, *(&task_1C4 + 1), local_FF78);
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
    if (task_1B0 == arg0) {
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
          script_4D0A(arg0, task_E8[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_1B0) {
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
                goto L005368;
              }
            }
            L005368:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L0053A6;
                }
              }
              L0053A6:;
              goto L0053A9;
            }
          }
        }
      }
      L0053A9:;
      if (task_1B0 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_1B0) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_1B0 != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_1B0 == arg0) {
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
                goto L005575;
              }
            }
            L005575:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L0055B3;
                }
              }
              L0055B3:;
              goto L0055B6;
            }
          }
          L0055B6:;
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
              goto L00560F;
            }
          }
        }
      }
      L00560F:;
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
      if (arg0 == task_1B0) {
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
  script_BCB8(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_98[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_1B0 == arg0) {
        local_80 = (Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_1B0 == arg0) {
        local_80 = (Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_50, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_50, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_50, 14, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_1B0 == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_50, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_50, 14, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_50, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_50, 14, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_50, 4, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L005CF2;
      }
    }
  }
  L005CF2:;
  return;
}

static void script_5CF3(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1B0 == arg0) {
    local_FC = task_98[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_50, 4, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_50, 4, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_5D5F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_1B8)->x;
  (&local_AC)->y = task_1B8.y;
  (&local_AC)->z = task_1B8.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
    local_80 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_1B0) {
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
        task_C0[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
        local_80 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_1B0) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_C0[arg2] = 0;
      }
    }
    local_88 = task_98[arg2];
    local_88 = Script_ScenDataUtil(task_50, 4, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_98[arg2];
      local_88 = Script_ScenDataUtil(task_50, 4, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_C0[arg2] = 0;
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
          goto L0061C9;
        }
      }
    }
    L0061C9:;
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

__attribute__((scn_string_bindings(0, data_1EAB6)))
static void script_6219(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_BCB8(arg0, 0);
  local_CC.x = (&task_1B8)->x;
  (&local_CC)->y = task_1B8.y;
  (&local_CC)->z = task_1B8.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1B8, *(&task_1C4 + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_1B0) {
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
  script_BCB8(arg0, 1);
  local_FC = task_98[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_50, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_50, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_50, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_50, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_50, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_50, 14, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1EAD2)))
static void script_68D3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      if (arg0 == task_1B0) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_1B0) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_E8[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_E8[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_1B0) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_1B0) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_1B0) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_E8[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_E8[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L0074C6;
      }
    }
  }
  L0074C6:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 7);
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
  script_2AB4(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_BCB8(arg0, 0);
  local_D0.x = (&task_1B8)->x;
  (&local_D0)->y = task_1B8.y;
  (&local_D0)->z = task_1B8.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1B8, *(&task_1C4 + 1), local_A4);
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
    if (arg0 == task_1B0) {
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
      if (arg0 == task_1B0) {
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
              script_4D0A(arg0, task_E8[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_1B0) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_1B0) {
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
        local_FF60 = script_2A3C((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L007ACB;
          }
        }
        L007ACB:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_2A3C(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_2A3C(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_1B0) {
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
          local_FF74 = (float)script_29FF(script_2A3C(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_1B0) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_1B0) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_98[arg2];
        if (arg0 == task_1B0) {
          if (task_1B0 == arg0) {
            local_A0 = (Script_ScenDataUtil(task_50, 4, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_50, 4, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_1B0) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L007F77;
            }
          }
        }
      }
      L007F77:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_1B0) {
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
      if (arg0 != task_1B0) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_BCB8(arg0, 1);
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
  if (arg0 == task_1B0) {
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
      goto L0081B2;
    }
  }
  L0081B2:;
  return;
}

__attribute__((scn_string_bindings(0, data_1EAEE)))
static void script_81B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_BCB8(arg0, 0);
    local_C4.x = (&task_1B8)->x;
    (&local_C4)->y = task_1B8.y;
    (&local_C4)->z = task_1B8.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1B8, *(&task_1C4 + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
          local_84 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_1B0) {
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
                goto L0084D7;
              }
            }
            L0084D7:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L008513;
                }
              }
              L008513:;
              goto L008516;
            }
          }
        }
      }
      L008516:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L00855B;
          }
        }
      }
      L00855B:;
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
  script_BCB8(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_98[arg2];
    local_9C = Script_ScenDataUtil(task_50, 4, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_50, 4, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_50, 4, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_50, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_50, 4, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_50, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_50, 4, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_50, 14, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1EB0A)))
static void script_88D4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_1D55C, 4, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_1D55C, 5, Script_ScenDataUtil(data_1CF6C, 1, arg3 * 40 + task_E8[arg2] * 4 + arg4) * 9 + 4);
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
  script_BCB8(arg0, 0);
  local_B0.x = (&task_1B8)->x;
  (&local_B0)->y = task_1B8.y;
  (&local_B0)->z = task_1B8.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1B8, *(&task_1C4 + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_1B0) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_1B0) {
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
  script_BCB8(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
  return;
}

static void script_905E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1B8, *(&task_1C4 + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_1EB26)))
static void script_9136(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_D4 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_1DF7C, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_1DF7C, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_1DF7C, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_1DF7C, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_1DF7C, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_1DF7C, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_1DF7C, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_1DF7C, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L009357;
    }
  }
  L009357:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_1DF7C, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_1DF7C, 5, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_1B8)->x;
  (&local_F4)->y = task_1B8.y;
  (&local_F4)->z = task_1B8.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1B8, *(&task_1C4 + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_1B0) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_1B0) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_1EB42)))
static void script_95AD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  task_1DC = 0;
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
        goto L0098B9;
      }
    }
  }
  L0098B9:;
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
  local_B8.x = (&task_1B8)->x;
  (&local_B8)->y = task_1B8.y;
  (&local_B8)->z = task_1B8.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1B8, *(&task_1C4 + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_1B0) {
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
      local_FF40 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1B0) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1B0) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FF6C = 0;
    L00A384:;
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
      if (!(local_FF6C >= 20)) goto L00A384;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_1B0) {
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

static void script_A540(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    if (arg0 != task_1B0) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_3066(arg5));
    }
  }
  return;
}

static void script_A98A(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_50 = arg1;
  task_54 = arg2;
  task_58 = arg3;
  task_5C = arg4;
  task_60.mode = 0;
  task_60.id = 0;
  task_60.entry_idx = 0;
  task_1B0 = arg0;
  (void)((task_1D4 = -1) & 1);
  task_1D0 = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L00A9F4:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_138[local_F8] = local_FC;
  task_70[local_F8] = -1;
  task_98[local_F8] = -1;
  task_C0[local_F8] = 0;
  task_E8[local_F8] = 0;
  task_110[local_F8] = 0;
  task_160[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L00A9F4;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_50, 4, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_50, 14, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_60, task_70, task_98, (shen_name *)task_C0, task_E8, arg0, arg5, data_1CF6C, data_1D55C, (shen_name *)task_138);
  Script_scenCreateTask(script_B246, 0);
  return;
}

static void script_AB16(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_50 = arg0;
  task_54 = arg1;
  task_58 = arg2;
  task_5C = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_1D0 = 1;
  return;
}

static void script_AB4A(void) {
  int32_t local_FC;
  local_FC = 0;
  L00AB52:;
  task_70[local_FC] = -1;
  task_98[local_FC] = -1;
  task_C0[local_FC] = 0;
  task_E8[local_FC] = 0;
  task_110[local_FC] = 0;
  task_160[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00AB52;
  return;
}

static void script_ABB3(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_1B0 == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_1E4, 0);
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

static void script_AC50(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_1B0 == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_1E4, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_AC8A(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_50, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_50, 4, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_50, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_ACED(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_50, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_50, 4, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_50, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_AD50(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_50, 4, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_50, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_AD87(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_50, 4, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_50, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_ADBE(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_98[local_F0] == arg0) {
      local_EC = task_138[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_50, 4, task_98[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_50, 4, task_98[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_50, 4, task_98[local_F0] * 13 + 1) & 0x20) {
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

static void script_B000(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_50, 4, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_50, 14, arg0 * 13 + 1, local_FC);
  script_B108(arg0);
  script_ADBE(arg0);
  return;
}

static void script_B049(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_50, 4, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_50, 14, arg0 * 13 + 1, local_FC);
  script_B116(arg0);
  script_ADBE(arg0);
  return;
}

static void script_B092(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_50, 4, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_50, 14, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_50, 4, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_50, 14, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_B108(int32_t arg0) {
  script_B092(arg0, 0);
  return;
}

static void script_B116(int32_t arg0) {
  script_B092(arg0, 1);
  return;
}

static void script_B124(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_50, 4, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_50, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_50, 14, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_50, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_50, 14, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_B1EB(void) {
  script_B124(1);
  return;
}

static void script_B1F7(void) {
  script_B124(0);
  return;
}

static void script_B203(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_50, 4, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_B246(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_1D0 == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_70[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_60)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FC * 9 + 1) * 4 + 1);
        script_C016(1, local_FC, local_F4, local_F8);
        if (script_C1CA(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_C016(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_C3E0(local_F4, local_F8);
    } else {
      task_1D0 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_B357(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_1B0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_1B0, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_1EBCD, 1, data_1EBE6)))
static void script_B3EF(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_188[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_188[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_5C, 4, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_5C, 4, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_5C, 4, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_5C, 4, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_5C, 4, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_5C, 4, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_5C, 4, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_58, 8, Script_ScenDataUtil(task_5C, 4, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_188[arg0] = -1;
    }
  } else {
    task_188[arg0] = -1;
  }
  return;
}

static void script_B5BE(int32_t arg0) {
  if (task_188[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_188[arg0], 1);
    task_188[arg0] = -1;
  }
  return;
}

static int32_t script_B5EF(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_1B0 == arg0) {
    local_F4.x = (&task_1E4)->x;
    (&local_F4)->y = task_1E4.y;
    (&local_F4)->z = task_1E4.z;
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
        if ((Script_ScenDataUtil(task_50, 4, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_50, 4, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_50, 4, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_50, 4, arg6 * 13 + 1) & 0x80)) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L00B7DA;
        }
        L00B7DA:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00B87D;
          }
        }
        L00B87D:;
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
            goto L00BA29;
          }
        }
      }
    }
    L00BA29:;
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
          task_1B8.x = (&local_F4)->x;
          task_1B8.y = (&local_F4)->y;
          task_1B8.z = (&local_F4)->z;
          task_1C4 = (&local_C4)->x;
          *(&task_1C4 + 1) = (&local_C4)->y;
          *(&task_1C4 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_1B8.x = (&local_F4)->x;
          task_1B8.y = (&local_F4)->y;
          task_1B8.z = (&local_F4)->z;
          task_1C4 = (&local_C4)->x;
          *(&task_1C4 + 1) = (&local_C4)->y;
          *(&task_1C4 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_1B8, 0);
          task_1C4 = (&local_C4)->x;
          *(&task_1C4 + 1) = (&local_C4)->y;
          *(&task_1C4 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_BCB8(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if (task_1B0 == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, 3);
      } else {
        Script_ccowUtils(arg0, 4);
      }
    }
  }
  return;
}

static void script_BCEA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_B5EF(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_1B0 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_50, 4, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_50, 4, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_50, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_50, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_50, 4, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_50, 4, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_50, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_50, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1EBFF)))
static void script_BE5A(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1B0 == arg0) {
    task_60.mode = 0;
    task_60.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_1B0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_1D4 & 0x20) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_98[arg1];
        local_FC = Script_ScenDataUtil(task_50, 4, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_50, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_98[arg1];
      local_FC = Script_ScenDataUtil(task_50, 4, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_50, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_70[arg1] = 1;
  task_160[arg1] = 0;
  task_110[arg1] = 0;
  task_C0[arg1] = 0;
  if (task_188[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_188[arg1], 1);
    task_188[arg1] = -1;
  }
  return;
}

static void script_C016(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_B357(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_B357(arg2, arg3, local_FC);
          goto L00C0DA;
        }
      }
      L00C0DA:;
    } else {
      if ((Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_B357(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_50, 4, task_98[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_1B0, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L00C175;
          }
        }
      }
    }
  }
  L00C175:;
  if (task_160[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_1E0) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_1E0) & task_1E0) {
    Script_sicnUtils(2, 4, 0);
  }
  task_1E0 = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_1EC0E)))
static int32_t script_C1CA(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_50, 4, task_98[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_50, 4, task_98[arg1] * 13 + 1) & 4) == 0)) {
      if (task_160[arg1] == 0) {
        task_70[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_C0[arg1] != task_1B0) & task_1E0) {
        if ((Script_ScenDataUtil(task_50, 4, task_98[arg1] * 13 + 1) & 2) == 0) {
          task_70[arg1] = 3;
          task_C0[arg1] = task_1B0;
          task_E8[arg1] = 0;
          task_60.mode = 1;
          task_60.entry_idx = task_98[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_50, 4, task_98[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_1B0, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_70[arg1] = 6;
              task_C0[arg1] = task_1B0;
              task_E8[arg1] = 0;
              task_60.mode = 0;
              task_60.entry_idx = task_98[arg1];
              local_FC = 0;
            }
            goto L00C37E;
          }
        }
        L00C37E:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_1B0, -1, &task_1E4, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_60.id = task_1B0;
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

static void script_C3E0(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L00C3E8:;
  local_F8 = task_70[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L00C408;
      L00C408:;
      script_C4AA(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L00C428;
      L00C428:;
      script_C50B(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L00C446;
      L00C446:;
      script_C6B0(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L00C468;
      L00C468:;
      script_C748(local_FC, arg1);
      break;
    case 2:
      script_C82A(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00C3E8;
  return;
}

static void script_C4AA(int32_t arg0, int32_t arg1) {
  if (task_160[arg0] == 0) {
    task_160[arg0] = Script_scenCreateTask(script_4645, 4, task_C0[arg0], task_138[arg0], arg0, arg1);
    task_110[arg0] = task_C0[arg0];
    script_B3EF(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1EC1D, 1, data_1EC2F)))
static void script_C50B(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_C0[arg0])) {
    if (task_160[arg0] == 0) {
      if (task_C0[arg0] == task_1B0) {
        task_60.mode = 1;
        task_60.entry_idx = task_98[arg0];
        goto L00C560;
      }
      L00C560:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_58, 4, Script_ScenDataUtil(task_5C, 4, local_FC * 9 + 1) * 4 + 1);
      script_BCEA(task_C0[arg0], task_138[arg0], task_98[arg0], local_F8);
      task_160[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1E01C, 8, local_F8), 4, task_C0[arg0], task_138[arg0], arg0, local_F8);
      task_110[arg0] = task_C0[arg0];
      if ((Script_ScenDataUtil(task_50, 4, task_98[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_B3EF(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_188[arg0] = -1;
      }
    }
  } else {
    if (task_70[arg0] != 5) {
      task_70[arg0] = 1;
      task_160[arg0] = 0;
      task_110[arg0] = 0;
      task_C0[arg0] = 0;
    }
  }
  return;
}

static void script_C6B0(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_160[arg0] == 0) & (arg2 >= 0)) {
    task_C0[arg0] = task_1B0;
    task_60.mode = 2;
    task_60.entry_idx = task_98[arg0];
    task_160[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1E01C, 8, arg2), 4, task_C0[arg0], task_138[arg0], arg0, arg2);
    task_110[arg0] = task_C0[arg0];
    script_B3EF(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1EC42, 1, data_1EC54)))
static void script_C748(int32_t arg0, int32_t arg1) {
  if ((task_160[arg0] == 0) & (arg1 >= 0)) {
    script_BCEA(task_C0[arg0], task_138[arg0], task_98[arg0], arg1);
    task_160[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1E01C, 8, arg1), 4, task_C0[arg0], task_138[arg0], arg0, arg1);
    task_110[arg0] = task_C0[arg0];
    if ((Script_ScenDataUtil(task_50, 4, task_98[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_B3EF(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_188[arg0] = -1;
    }
  }
  return;
}

static void script_C82A(int32_t arg0) {
  int32_t local_FC;
  task_160[arg0] = 0;
  task_110[arg0] = 0;
  task_70[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_98[arg0], 2);
  if (local_FC != -1) {
    task_C0[arg0] = 0;
  }
  return;
}

static void script_C881(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_1ED58, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_1ED20, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_1ECE8, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_1ED04, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_1ED3C, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_C9B9(int32_t arg0) {
  script_C9D1(arg0);
  return;
}

static void script_C9C5(int32_t arg0) {
  script_C9D1(arg0);
  return;
}

static void script_C9D1(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L00CA0A;
    }
  }
  L00CA0A:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_C881();
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

static void script_CACD(int32_t arg0) {
  script_C9C5(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_CAE4(int32_t arg0) {
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

static void script_CB25(int32_t arg0) {
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

static void script_CB69(int32_t arg0) {
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

static void script_CBE7(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_CB69(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_CB69(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_CB69(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_CB69(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_CB69(5);
  } else {
    script_CB69(0);
  }
  return;
}

static void script_CC66(int32_t arg0) {
  int32_t local_FC;
  if (arg0 != -1) {
    task_1F0 = arg0;
  }
  local_FC = task_1F0;
  if (local_FC == 0) {
    Script_gachaponUtils(0, data_1EF0C);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_1EFB5);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_1F0F0);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_1F225);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_1F2CC);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_1F439);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_1F632);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_1F705);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_1F776);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_1F7C3);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_1F7FC);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_1F8EB);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_1F94E);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_1F9A3);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_1F9EA);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_1FA23);
  } else {
    task_1F0 = -1;
  }
  return;
}

static void script_CE07(int8_t arg0, int8_t arg1) {
  int16_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_FE = -1;
  if (arg0 == -1) {
    local_FD = task_1F0;
  } else {
    local_FD = arg0;
  }
  local_F8 = local_FD;
  switch (local_F8) {
    case 0:
      local_FE = *(int16_t *)((int8_t *)&data_1EDB0 + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_1EDC8 + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_1EDEC + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_1EE18 + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_1EE2C + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_1EE54 + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_1EE78 + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_1EE90 + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_1EEA0 + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_1EEA8 + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_1EEB0 + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_1EED4 + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_1EEE4 + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_1EEF0 + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_1EEFC + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_1EF04 + (arg1 << 1));
      break;
  }
  (void)local_FE;
  return;
}

static void script_CFF2(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_1ED90[*(int32_t *)arg0] - data_1EDA0[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_1ED90[*(int32_t *)arg0] - data_1EDA0[*(int32_t *)arg0]);
  return;
}

static void script_D041(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_1ED90[arg0] - data_1EDA0[arg0]) {
    (void)1;
  } else if (arg0 == 15) {
    (void)3;
  } else {
    (void)2;
  }
  return;
}

static void script_D094(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
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
        goto L00D105;
      }
      L00D105:;
      break;
    case 3:
      local_EC = arg2;
      if (local_EC == 6) {
        (void)((local_F3 = -1) & 1);
        goto L00D12C;
      }
      L00D12C:;
      break;
    case 4:
      local_EC = arg2;
      if ((local_EC == 7) | (local_EC == 15) | (local_EC == 16) | (local_EC == 17) | (local_EC == 18)) {
        (void)((local_F3 = -1) & 1);
        goto L00D16B;
      }
      L00D16B:;
      break;
    case 5:
      (void)((local_F3 = -1) & 1);
      break;
    case 6:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 2) | (local_EC == 3)) {
        (void)((local_F3 = -1) & 1);
        goto L00D1B9;
      }
      L00D1B9:;
      break;
    case 8:
      local_EC = arg2;
      if (local_EC == 3) {
        (void)((local_F3 = -1) & 1);
        goto L00D1E0;
      }
      L00D1E0:;
      break;
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
  if (local_F3 & 8) {
    arg2 = arg2 + data_1ED90[arg1];
    local_F4.x = (&local_F4)->x * arg3;
    (&local_F4)->y = (&local_F4)->y * arg3;
    (&local_F4)->z = (&local_F4)->z * arg3;
  } else {
    local_F4.x = (&local_F4)->x * arg3 * 0.33333f;
    (&local_F4)->y = (&local_F4)->y * arg3 * 0.33333f;
    (&local_F4)->z = (&local_F4)->z * arg3 * 0.33333f;
  }
  if (local_F2 & 4) {
    script_CC66(arg1);
    Script_gachaponUtils(5, arg0, arg2);
  } else {
    script_CC66(arg1);
    Script_gachaponUtils(1, arg0, arg2);
  }
  if ((((scn3_bits){.f = (&local_F4)->x}).i != 0x3F800000) & (((scn3_bits){.f = (&local_F4)->x}).i != 0)) {
    Script_hmdlApplyScale(arg0, &local_F4);
  }
  return;
}

static void script_D2CB(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_D8[7];
  int32_t local_D4;
  Vector3 local_C8;
  Vector3i local_BC;
  int32_t local_B8;
  task_1F4 = arg0;
  local_FC = script_EBC6();
  local_D4 = Script_scenCharaGetMotAnimId(CHARA_NITO);
  Script_EVT_GetPosition(CHARA_NITO, -1, &local_C8, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_NITO, &local_BC, 0x40000000);
  script_E3F3();
  local_F8 = -1;
  local_F4 = -1;
  local_F4 = Script_scenCreateTask(script_DE8C, 0);
  local_B8 = arg0;
  switch (local_B8) {
    case 1:
      local_F8 = Script_scenCreateTask(script_E025, 0);
      break;
    case 2:
      local_F8 = Script_scenCreateTask(script_E094, 0);
      break;
    case 3:
      local_F8 = Script_scenCreateTask(script_E103, 0);
      break;
    case 4:
      local_F8 = Script_scenCreateTask(script_E103, 0);
      break;
    case 5:
      local_F8 = Script_scenCreateTask(script_E103, 0);
      break;
    case 6:
      local_F8 = Script_scenCreateTask(script_E103, 0);
      break;
    case 7:
      local_F8 = Script_scenCreateTask(script_E103, 0);
      break;
    case 8:
      local_F8 = Script_scenCreateTask(script_E103, 0);
      break;
    case 9:
      local_F8 = Script_scenCreateTask(script_E103, 0);
      break;
  }
  script_D493();
  script_EA0B();
  script_ECE8(local_FC);
  script_E3F0(local_D4, (int32_t)&local_C8, (int32_t)&local_BC);
  if (local_F8 != -1) {
    Script_srchControl((void *)local_F8);
  }
  if (local_F4 != -1) {
    Script_srchControl((void *)local_F4);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_D493(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_E8[3];
  Vector3 local_DC;
  int32_t unused_D0[3];
  Vector3i local_C4;
  local_FC = Script_scenCreateTask(script_D5B4, 0);
  local_F8 = Script_scenCreateTask(script_DA5B, 0);
  local_F4 = Script_scenCreateTask(script_E1E2, 0);
  local_DC.x = 5.0256f;
  (&local_DC)->y = 0.0f;
  (&local_DC)->z = -6.4795f;
  local_C4.x = 0;
  (&local_C4)->y = 0x4000;
  (&local_C4)->z = 0;
  SCNF_WaitFrames_0000(2);
  Script_EVT_SetPosition(CHARA_NITO, POS_XYZ, &local_DC);
  Script_EVT_SetRotation(CHARA_NITO, 0x38000000, &local_C4, 0);
  Script_PlayCharaMotion(CHARA_NITO, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(360);
  if (task_1F6 == 1) {
    SCNF_WaitFrames_0000(20);
  }
  if (task_1F7 == 3) {
    SCNF_WaitFrames_0000(240);
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_F4);
  Script_srchControl((void *)local_F8);
  Script_srchControl((void *)local_FC);
  return;
}

static int32_t script_D5B4(void) {
  int32_t local_F4[3];
  int32_t local_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  int32_t local_C4[3];
  int32_t local_B8[3];
  Vector3i local_AC;
  Vector3i local_A0;
  Vector3 local_94;
  int16_t local_92;
  int32_t local_8C;
  local_F4[0] = 0;
  local_F4[1] = 0;
  local_F4[2] = 0;
  local_C4[0] = 0;
  local_C4[1] = 0;
  local_C4[2] = 0;
  local_E8[0] = 0;
  local_E8[1] = 0;
  local_E8[2] = 0;
  local_B8[0] = 0;
  local_B8[1] = 0x4000;
  local_B8[2] = 0;
  local_DC.x = 0.106f;
  (&local_DC)->y = 0.016f;
  (&local_DC)->z = -0.042f;
  local_AC.x = 0xC000;
  (&local_AC)->y = 0;
  (&local_AC)->z = 60984;
  local_D0.x = 0.17f;
  (&local_D0)->y = 0.023f;
  (&local_D0)->z = -0.034f;
  local_A0.x = 35316;
  (&local_A0)->y = 0;
  (&local_A0)->z = 0x4000;
  local_94.x = 0.0f;
  (&local_94)->y = 0.0f;
  (&local_94)->z = 0.0f;
  local_92 = 0;
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  if (task_1F6 == 1) {
    SCNF_WaitFrames_0000(20);
  }
  for (;;) {
    local_8C = local_92;
    switch (local_8C) {
      case 93:
        script_DC73();
        break;
      case 153:
        local_8C = task_1F6;
        switch (local_8C) {
          case 0:
            task_1F2 = 10;
            break;
          case 1:
            task_1F2 = 14;
            break;
        }
        Script_EVT_GetPosition(SHEN_NAME('KIHN'), -1, &local_94, 0);
        Script_SetCharaLookAtPosition(CHARA_AKIR, 8, &local_94, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_94, 10);
        break;
      case 259:
        task_1F1 = 1;
        Script_CharHandlers(CHARA_AKIR, 0, 2, 16);
        Script_scenCreateTask(script_DCF6, 0);
        break;
      case 296:
        if (task_1F7 == 3) {
          Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.3f);
        }
        if (task_1F6 == 1) {
          Script_fixoAttachEx(SHEN_NAME('KIHN'), CHARA_AKIR, 18, &local_D0, &local_A0);
        } else {
          Script_charaDisplay(SHEN_NAME('KIHN'), CDM_Hide);
        }
        break;
      case 299:
        local_8C = task_1F6;
        if (local_8C == 0) {
          Script_CharHandlers(CHARA_AKIR, 0, 3, 16);
          goto L00D7FC;
        }
        L00D7FC:;
        break;
      case 313:
        if (task_1F7 == 3) {
          SCNF_WaitFrames_0000(50);
        }
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
        if (task_1F7 == 3) {
          Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_40);
        }
        break;
      case 320:
        if (task_1F6 == 0) {
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
        }
        break;
      case 343:
        if (task_1F6 == 1) {
          if (task_1F7 == 2) {
            Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
            task_1F2 = 15;
          }
        }
        break;
      case 350:
        if (task_1F6 == 1) {
          Script_charaDisplay(SHEN_NAME('KIHN'), CDM_Hide);
          Script_fixoClearAttachment(SHEN_NAME('KIHN'));
        }
        break;
      case 373:
        if (task_1F7 == 2) {
          Script_charaDisplay(SHEN_NAME('KIHN'), CDM_Hide);
          Script_fixoClearAttachment(SHEN_NAME('KIHN'));
        }
        break;
      case 430:
        if (task_1F7 == 3) {
          Script_CharHandlers(CHARA_AKIR, 0, 3, 16);
          task_1F2 = 13;
        }
        break;
      case 456:
        if (task_1F7 == 3) {
          Script_fixoAttachEx(SHEN_NAME('KHN2'), CHARA_AKIR, 18, &local_DC, &local_AC);
        }
        break;
      case 460:
        if (task_1F7 == 3) {
          task_1F1 = 1;
          Script_CharHandlers(CHARA_AKIR, 0, 2, 16);
          Script_EVT_GetPosition(SHEN_NAME('KHN2'), -1, &local_94, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_94, 10);
          Script_scenCreateTask(script_DE38, 0);
        }
        break;
      case 519:
        if (task_1F7 == 3) {
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
          Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        }
        break;
    }
    if ((local_92 > 460) & (local_92 < 518)) {
      Script_EVT_GetPosition(SHEN_NAME('KHN2'), -1, &local_94, 0);
      Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_94, 0);
    }
    local_92 = local_92 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_DA5B(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t local_DC[3];
  Vector3i local_D0;
  int16_t local_CE;
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  local_DC[0] = 0;
  local_DC[1] = 0;
  local_DC[2] = 0;
  local_E8.x = 5.0256f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = -6.5995f;
  local_D0.x = 0;
  (&local_D0)->y = 0x4000;
  (&local_D0)->z = 0;
  local_CE = 0;
  Script_EVT_SetPosition(CHARA_NITO, POS_XYZ, &local_E8);
  Script_EVT_SetRotation(CHARA_NITO, 0x38000000, &local_D0, 0);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  Script_motmSetBlendFrames(CHARA_NITO, MOTM_BLEND_FRAMES_30);
  Script_SetCharaLookAtPosition(CHARA_NITO, 10, &local_F4, 0);
  {
    uint8_t unused_CC[2];
    int32_t local_C8;
    local_C8 = task_1F6;
    switch (local_C8) {
      case 0:
        task_1F1 = 2;
        break;
      case 1:
        task_1F1 = 4;
        SCNF_WaitFrames_0000(20);
        break;
    }
  }
  for (;;) {
    uint8_t unused_CC[2];
    int32_t local_C8;
    local_C8 = local_CE;
    switch (local_C8) {
      case 60:
        task_1F2 = 12;
        Script_SetCharaLookAtPosition(CHARA_NITO, -6, 0, 0);
        break;
      case 133:
        local_C8 = task_1F6;
        switch (local_C8) {
          case 0:
            task_1F1 = 3;
            break;
          case 1:
            task_1F1 = 5;
            break;
        }
        break;
      case 155:
        Script_charaDisplay(SHEN_NAME('KIHN'), CDM_Show);
        break;
      case 315:
        if (task_1F7 == 3) {
          SCNF_WaitFrames_0000(50);
        }
        if (task_1F7 == 3) {
          Script_EVT_SetPosition(CHARA_NITO, POS_XYZ, &local_E8);
          Script_EVT_SetRotation(CHARA_NITO, 0x38000000, &local_D0, 0);
          task_1F2 = 12;
          task_1F1 = 6;
        }
        break;
      case 356:
        if (task_1F7 == 3) {
          task_1F1 = 7;
        }
        break;
      case 404:
        if (task_1F7 == 3) {
          Script_charaDisplay(SHEN_NAME('KHN2'), CDM_Show);
        }
        break;
    }
    local_CE = local_CE + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_DC73(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 3.4483f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -6.6f;
  local_E8.x = 0;
  (&local_E8)->y = 0xC000;
  (&local_E8)->z = 0;
  Script_InitializeOsag(CHARA_AKIR);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  return;
}

static int32_t script_DCF6(void) {
  int32_t local_FC;
  local_FC = task_1F4;
  switch (local_FC) {
    case 1:
      SCNF_StartIconSystemMenu(218);
      Script_GetCassetteFlags(218, 1, 0, 0);
      break;
    case 2:
      SCNF_StartIconSystemMenu(219);
      Script_GetCassetteFlags(219, 1, 0, 0);
      break;
    case 3:
      SCNF_StartIconSystemMenu(214);
      Script_GetCassetteFlags(214, 1, 0, 0);
      break;
    case 4:
      SCNF_StartIconSystemMenu(216);
      Script_GetCassetteFlags(216, 1, 0, 0);
      break;
    case 5:
      SCNF_StartIconSystemMenu(215);
      Script_GetCassetteFlags(215, 1, 0, 0);
      break;
    case 6:
      SCNF_StartIconSystemMenu(228);
      Script_GetCassetteFlags(228, 1, 0, 0);
      break;
    case 7:
      SCNF_StartIconSystemMenu(227);
      Script_GetCassetteFlags(227, 1, 0, 0);
      break;
    case 8:
      SCNF_StartIconSystemMenu(228);
      Script_GetCassetteFlags(228, 1, 0, 0);
      break;
    case 9:
      SCNF_StartIconSystemMenu(227);
      Script_GetCassetteFlags(227, 1, 0, 0);
      break;
  }
  return Script_srchControl(0);
}

static int32_t script_DE38(void) {
  int32_t local_FC;
  local_FC = task_1F4;
  switch (local_FC) {
    case 8:
      SCNF_StartIconSystemMenu(216);
      Script_GetCassetteFlags(216, 1, 0, 0);
      break;
    case 9:
      SCNF_StartIconSystemMenu(215);
      Script_GetCassetteFlags(215, 1, 0, 0);
      break;
  }
  return Script_srchControl(0);
}

static int32_t script_DE8C(void) {
  int32_t local_FC;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  while (1) {
    local_FC = task_1F2;
    switch (local_FC) {
      case 12:
        Script_PlayCharaMotion(CHARA_NITO, 31005, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        task_1F2 = 0;
        break;
      case 10:
        Script_PlayCharaMotion(CHARA_AKIR, 30981, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        task_1F2 = 0;
        break;
      case 11:
        Script_PlayCharaMotion(CHARA_AKIR, 30980, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        task_1F2 = 0;
        break;
      case 13:
        Script_PlayCharaMotion(CHARA_AKIR, 30981, 115.0f, 299.0f, 120.0f, 1.0f, 0);
        script_E358(CHARA_AKIR);
        task_1F2 = 0;
        break;
      case 14:
        Script_PlayCharaMotion(CHARA_AKIR, 30980, 0.0f, 160.0f, 0.0f, 1.0f, 0);
        script_E358(CHARA_AKIR);
        task_1F2 = 0;
        break;
      case 15:
        Script_PlayCharaMotion(CHARA_AKIR, 30980, 160.0f, 249.0f, 160.0f, 1.0f, 0);
        script_E358(CHARA_AKIR);
        task_1F2 = 0;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_E025(void) {
  int32_t local_FC;
  while (1) {
    local_FC = task_1F1;
    switch (local_FC) {
      case 1:
        Script_ShowUIMessageFromID("E1025A043");
        task_1F1 = 0;
        break;
      case 2:
        Script_ShowUIMessageFromID("E1025B005");
        task_1F1 = 0;
        break;
      case 3:
        Script_ShowUIMessageFromID("E1025B006");
        task_1F1 = 0;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_E094(void) {
  int32_t local_FC;
  while (1) {
    local_FC = task_1F1;
    switch (local_FC) {
      case 1:
        Script_ShowUIMessageFromID("E1026A026");
        task_1F1 = 0;
        break;
      case 2:
        Script_ShowUIMessageFromID("E1026B005");
        task_1F1 = 0;
        break;
      case 3:
        Script_ShowUIMessageFromID("E1026B006");
        task_1F1 = 0;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_E103(void) {
  int32_t local_FC;
  while (1) {
    local_FC = task_1F1;
    switch (local_FC) {
      case 1:
        Script_ShowUIMessageFromID("E1027A025");
        task_1F1 = 0;
        break;
      case 2:
        Script_ShowUIMessageFromID("E1027C001");
        task_1F1 = 0;
        break;
      case 3:
        Script_ShowUIMessageFromID("E1027C002");
        task_1F1 = 0;
        break;
      case 4:
        Script_ShowUIMessageFromID("E1027C003");
        task_1F1 = 0;
        break;
      case 5:
        Script_ShowUIMessageFromID("E1027C004");
        task_1F1 = 0;
        break;
      case 6:
        Script_ShowUIMessageFromID("E1027C005");
        task_1F1 = 0;
        break;
      case 7:
        Script_ShowUIMessageFromID("E1027C006");
        task_1F1 = 0;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_E1E2(void) {
  int16_t local_FE;
  int32_t local_F8;
  SCNF_WaitFrames_0000(2);
  local_FE = 2;
  if (task_1F6 == 1) {
    Script_seqEnterEvtCam(7664, 0, AUTH_None);
    SCNF_WaitFrames_0000(20);
  }
  while (1) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 2:
        Script_seqEnterEvtCam(7660, 0, AUTH_None);
        break;
      case 95:
        Script_seqEnterEvtCam(7661, 0, AUTH_None);
        break;
      case 155:
        if (task_1F6 == 1) {
          Script_seqEnterEvtCam(7662, 0, AUTH_None);
        } else {
          Script_seqEnterEvtCam(7670, 0, AUTH_None);
        }
        break;
      case 215:
        if (task_1F7 == 3) {
          Script_seqEnterEvtCam(7665, 0, AUTH_None);
        } else {
          Script_seqEnterEvtCam(7663, 0, AUTH_None);
        }
        break;
      case 290:
        if (task_1F7 == 3) {
          Script_seqEnterEvtCam(7666, 0, AUTH_None);
        }
        if (task_1F7 == 3) {
          SCNF_WaitFrames_0000(50);
        }
        break;
      case 350:
        if (task_1F7 == 3) {
          Script_seqEnterEvtCam(7667, 0, AUTH_None);
        }
        break;
      case 405:
        if (task_1F7 == 3) {
          Script_seqEnterEvtCam(7668, 0, AUTH_None);
        }
        break;
      case 456:
        if (task_1F7 == 3) {
          Script_seqEnterEvtCam(7669, 0, AUTH_None);
        }
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_E358(int32_t arg0) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_E372(int32_t arg0, float arg1) {
  float local_FC;
  local_FC = 0.0f;
  SCNF_WaitFrames_0000(1);
  local_FC = Script_GetCharaMotionRemainingFramesScaled(arg0);
  for (;;) {
    if (local_FC >= arg1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
    local_FC = Script_GetCharaMotionRemainingFramesScaled(arg0);
  }
  return;
}

static void script_E3AD(Vector3 *arg0, Vector3i *arg1) {
  int32_t local_FC;
  local_FC = -1;
  if (Script_scenCheckCharaExist(CHARA_NITO)) {
    local_FC = Script_scenCharaGetMotAnimId(CHARA_NITO);
    Script_EVT_GetPosition(CHARA_NITO, -1, arg0, 0);
    Script_mtwkGetSetRotation(CHARA_NITO, arg1, 0);
  }
  (void)local_FC;
  return;
}

static void script_E3F0(int32_t arg0, int32_t arg1, int32_t arg2) {
  return;
}

static void script_E3F3(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3i local_C4;
  Vector3i local_B8;
  task_1F7 = 2;
  task_1F1 = 0;
  task_1F2 = 0;
  task_1F5 = 0;
  script_14E30(&local_F4, 0.3f, 0.3f, 0.3f);
  script_14E30(&local_E8, 0.3f, 0.3f, 0.3f);
  script_14E30(&local_DC, 4.3f, 0.9f, -6.6f);
  script_14E30(&local_D0, 4.3f, 0.9f, -6.6f);
  script_190B1(&local_C4, 0.0f, 0.0f, 0.0f);
  script_190B1(&local_B8, 0.0f, 0.0f, 0.0f);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  if (Script_ccowUtils(CHARA_AKIR, 7)) {
    task_1F5 = 1;
  }
  if (task_1F5 == 1) {
    Script_ccowUtils(CHARA_AKIR, 2);
  }
  if (task_1F4 == 1) {
    Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  }
  {
    int32_t local_B4;
    local_B4 = task_1F4;
    if ((local_B4 == 1) | (local_B4 == 2) | (local_B4 == 3) | (local_B4 == 4) | (local_B4 == 5)) {
      task_1F6 = 0;
    } else {
      if ((local_B4 == 6) | (local_B4 == 7) | (local_B4 == 8) | (local_B4 == 9)) {
        task_1F6 = 1;
        goto L00E545;
      }
    }
  }
  L00E545:;
  {
    int32_t local_B4;
    local_B4 = task_1F4;
    switch (local_B4) {
      case 1:
        script_CC66(11);
        Script_gachaponUtils(1, 0x4E48494B, 3);
        script_14E30(&local_F4, 0.35f, 0.35f, 0.35f);
        script_14E30(&local_DC, 4.3f, 0.925f, -6.59f);
        script_190B1(&local_C4, 0.0f, 45.0f, 0.0f);
        break;
      case 2:
        script_CC66(11);
        Script_gachaponUtils(1, 0x4E48494B, 4);
        script_14E30(&local_DC, 4.3f, 0.925f, -6.59f);
        script_190B1(&local_C4, 0.0f, 0.0f, 0.0f);
        break;
      case 3:
        script_CC66(11);
        Script_gachaponUtils(1, 0x4E48494B, 0);
        script_14E30(&local_DC, 4.3f, 0.925f, -6.59f);
        script_190B1(&local_C4, 0.0f, 0.0f, 0.0f);
        break;
      case 4:
        script_CC66(11);
        Script_gachaponUtils(1, 0x4E48494B, 2);
        script_14E30(&local_F4, 0.25f, 0.25f, 0.25f);
        script_14E30(&local_DC, 4.3f, 0.925f, -6.59f);
        script_190B1(&local_C4, 0.0f, 270.0f, 0.0f);
        break;
      case 5:
        script_CC66(11);
        Script_gachaponUtils(1, 0x4E48494B, 1);
        script_14E30(&local_F4, 0.25f, 0.25f, 0.25f);
        script_14E30(&local_DC, 4.3f, 0.925f, -6.59f);
        script_190B1(&local_C4, 0.0f, 0.0f, 0.0f);
        break;
      case 6:
        script_CC66(11);
        Script_gachaponUtils(1, 0x4E48494B, 6);
        script_14E30(&local_F4, 0.5f, 0.5f, 0.5f);
        script_14E30(&local_DC, 4.326f, 0.901f, -6.6f);
        script_190B1(&local_C4, 270.0f, 270.0f, 0.0f);
        break;
      case 7:
        script_CC66(11);
        Script_gachaponUtils(1, 0x4E48494B, 5);
        script_14E30(&local_F4, 0.5f, 0.5f, 0.5f);
        script_14E30(&local_DC, 4.326f, 0.901f, -6.6f);
        script_190B1(&local_C4, 270.0f, 270.0f, 0.0f);
        break;
      case 8:
        script_CC66(11);
        Script_gachaponUtils(1, 0x4E48494B, 6);
        script_14E30(&local_F4, 0.5f, 0.5f, 0.5f);
        script_14E30(&local_DC, 4.326f, 0.901f, -6.6f);
        script_190B1(&local_C4, 270.0f, 270.0f, 0.0f);
        script_CC66(-1);
        Script_gachaponUtils(1, 0x324E484B, 2);
        script_14E30(&local_E8, 0.25f, 0.25f, 0.25f);
        script_14E30(&local_D0, 4.3f, 0.925f, -6.59f);
        script_190B1(&local_B8, 0.0f, 270.0f, 0.0f);
        task_1F7 = 3;
        break;
      case 9:
        script_CC66(11);
        Script_gachaponUtils(1, 0x4E48494B, 5);
        script_14E30(&local_F4, 0.5f, 0.5f, 0.5f);
        script_14E30(&local_DC, 4.326f, 0.901f, -6.6f);
        script_190B1(&local_C4, 270.0f, 270.0f, 0.0f);
        script_CC66(-1);
        Script_gachaponUtils(1, 0x324E484B, 1);
        script_14E30(&local_E8, 0.25f, 0.25f, 0.25f);
        script_14E30(&local_D0, 4.3f, 0.925f, -6.59f);
        script_190B1(&local_B8, 0.0f, 0.0f, 0.0f);
        task_1F7 = 3;
        break;
    }
  }
  Script_charaDisplay(SHEN_NAME('KIHN'), CDM_Hide);
  Script_hmdlApplyScale(SHEN_NAME('KIHN'), &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('KIHN'), POS_XYZ, &local_DC);
  Script_EVT_SetRotation(SHEN_NAME('KIHN'), 0x38000000, &local_C4, 0);
  if (task_1F7 == 3) {
    Script_charaDisplay(SHEN_NAME('KHN2'), CDM_Hide);
    Script_hmdlApplyScale(SHEN_NAME('KHN2'), &local_E8);
    Script_EVT_SetPosition(SHEN_NAME('KHN2'), POS_XYZ, &local_D0);
    Script_EVT_SetRotation(SHEN_NAME('KHN2'), 0x38000000, &local_B8, 0);
  }
  Script_SetCharaRefBoardPos(CHARA_NITO, &data_1FAE0);
  Script_charaSetRefboardFlagTo(CHARA_NITO, 1);
  Script_SetCPAMFlags(SHEN_NAME('KIHN'), 0);
  return;
}

static void script_EA0B(void) {
  int32_t local_FC;
  local_FC = task_1F4;
  switch (local_FC) {
    case 1:
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 942, 1);
      break;
    case 2:
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 940, 1);
      break;
    case 3:
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 941, 1);
      break;
    case 4:
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 939, 1);
      break;
    case 5:
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 938, 1);
      break;
    case 6:
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 968, 1);
      break;
    case 7:
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 967, 1);
      break;
    case 8:
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 968, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 939, 1);
      break;
    case 9:
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 967, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 938, 1);
      break;
  }
  Script_gachaponUtils(2, 0x4E48494B);
  if (task_1F7 == 3) {
    Script_gachaponUtils(2, 0x324E484B);
  }
  if (task_1F5 == 1) {
    Script_ccowUtils(CHARA_AKIR, 1);
  }
  if (task_1F4 == 1) {
    Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  }
  Script_SetCPAMFlags(CHARA_NITO, 1);
  Script_charaSetRefboardFlagTo(CHARA_NITO, 2);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_1FAF0, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  Script_motmSetBlendFrames(CHARA_NITO, MOTM_BLEND_FRAMES_5);
  Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(1, data_1FB09, 3, data_1FB11)))
static int32_t script_EBC6(void) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_FC = -1;
  local_EC.x = 4.9248f;
  (&local_EC)->y = 0.0f;
  (&local_EC)->z = -6.4176f;
  local_E0.x = 0;
  (&local_E0)->y = 8738;
  (&local_E0)->z = 0;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4F54494E, 1);
  Script_sprintf("NITO LOADING");
  while ((local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4F54494E)) == 0) {
    Script_sprintf(".");
    SCNF_WaitFrames_0000(1);
  }
  Script_sprintf("END!\012");
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    Script_sprintf(".");
    SCNF_WaitFrames_0000(1);
  }
  Script_sprintf("\012");
  local_F8 = Script_CycleManagerUtils(CYCLE_CMD_GET_VALUE_BY_CYCLE_ID, 0x4F54494E);
  Script_EVT_SetPosition(CHARA_NITO, POS_XYZ, &local_EC);
  Script_EVT_SetRotation(CHARA_NITO, 0x38000000, &local_E0, 0);
  Script_PlayCharaMotion(CHARA_NITO, local_F8, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return local_FC;
}

static void script_ECE8(int32_t arg0) {
  if ((arg0 != 0) & (arg0 != -1)) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg0);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4F54494E, 0);
  return;
}

static int32_t script_ED11(void) {
  script_13455();
  if (Script_GetHighScoreValue(Highscore_Unknown0) < 10000) {
    task_1FC = 10000;
  } else {
    task_1FC = Script_GetHighScoreValue(Highscore_Unknown0);
  }
  return task_200 = Script_GetHighScoreValue(Highscore_Unknown1);
}

static int32_t script_ED4E(void) {
  if (Script_GetHighScoreValue(Highscore_Unknown5) < 30000) {
    task_204 = 30000;
  } else {
    task_204 = Script_GetHighScoreValue(Highscore_Unknown5);
  }
  task_208 = Script_GetHighScoreValue(Highscore_Unknown6);
  script_15FFB();
  Script_scenToggleVisibleFor(SHEN_NAME('MIT1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('MIT2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('MIT3'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('QTL1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('QTL2'), 0);
  return Script_scenToggleVisibleFor(SHEN_NAME('QTL3'), 0);
}

static int32_t script_EDD3(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Script_EVT_GetPosition(SHEN_NAME('SPDM'), -1, &local_F4, 0);
  script_14E4B(&local_F4, 0.33f, 0.0f, 0.0f);
  Script_EVT_SetPosition(SHEN_NAME('SPDM'), POS_XYZ, &local_F4);
  end:;
}

static void script_EE07(void) {
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  return;
}

static void script_EE43(void) {
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_uiUtils(0, 37, 0);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  return;
}

static int32_t script_EE8B(int32_t arg0, const char *arg1) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, arg0, arg1, 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  if (arg0 == task_1F9) {
    task_1F9 = 0;
  }
  end:;
}

static void script_EEE4(int32_t arg0, int32_t arg1) {
  task_1F9 = arg0;
  Script_scenCreateTask(script_EE8B, 2, arg0, arg1);
  return;
}

__attribute__((scn_string_bindings(0, data_1FB68, 1, data_1FB78)))
static void script_EEFD(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3i local_D8;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/DGCT/", "DART") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_LoadHandMot(3, SHEN_NAME('DGCT'), "HMOT_DRT.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_F7FF();
  local_FC = 0;
  while (local_FC < 3) {
    local_F8 = 3;
    local_F4 = 1 << local_FC;
    local_F0 = *(&data_1FB48 + local_FC);
    if (local_F8 & local_F4) {
      local_E4.x = *(float *)((const char *)&data_1FB18 + local_FC * 16);
      (&local_E4)->y = *(float *)((const char *)&data_1FB18 + (local_FC * 16 + 4));
      (&local_E4)->z = *(float *)((const char *)&data_1FB18 + (local_FC * 16 + 8));
      local_D8.x = 0;
      (&local_D8)->y = *(int16_t *)((int8_t *)&data_1FB18 + (local_FC * 16 + 12));
      (&local_D8)->z = 0;
      Script_EVT_SetPosition(local_F0, POS_XYZ, &local_E4);
      Script_EVT_SetRotation(local_F0, 0x38000000, &local_D8, 0);
      Script_scenToggleVisibleFor(local_F0, 1);
      Script_charaDisplay(local_F0, CDM_Hide);
      task_220 = script_12350(local_F0, 0, 2, 0);
      task_224 = script_12350(local_F0, 0, 3, 0);
      script_119F3(local_F0);
    }
    local_FC = local_FC + 1;
  }
  if (arg0) {
    task_230 = Script_scenCreateTask(script_F210, 0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1FB8A)))
static void script_F084(int32_t arg0) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "DART");
  Script_FreeTaskData(task_22C);
  if (arg0) {
    Script_srchControl((void *)task_230);
  }
  return;
}

static int32_t script_F0AA(void) {
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
    local_D0 = 3;
    local_CC = 1 << local_D8;
    if (local_D0 & local_CC) {
      Script_EVT_GetPosition(*(&data_1FB48 + local_D8), -1, &local_C0, ENTITY_XFORM_WORLD_SPACE);
      local_B0 = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &local_C0);
      if (!(local_B0 >= 3.0f)) {
        Script_scenToggleVisibleFor(*(&data_1FB48 + local_D8), 1);
        if (Script_CheckTargetWithinAngleRange(&local_E8, 1.4f, (&local_DC)->y, 5461, &local_C0)) {
          Script_mtwkGetSetRotation(*(&data_1FB48 + local_D8), &local_B4, 0x40000000);
          if (Script_CheckTargetWithinAngleRange(&local_C0, 1.5f, (&local_B4)->y + 0x8000, 9102, &local_F4)) {
            local_D4 = *(&data_1FB48 + local_D8);
          }
        }
      } else {
        Script_scenToggleVisibleFor(*(&data_1FB48 + local_D8), 0);
      }
    }
    local_D8 = local_D8 + 1;
  }
  return local_D4;
}

static int32_t script_F210(void) {
  int8_t local_FF;
  int32_t local_F8;
  int8_t local_F7;
  (void)((local_FF = 0) & 2);
  for (;;) {
    (void)((local_F7 = 0) & 2);
    if (Script_IsPlayerAllowed(0)) {
      local_F8 = script_F0AA();
      if (local_F8) {
        (void)((local_F7 = -1) & 2);
        if (Script_GetControllerInput(1) == 0x400) {
          if (local_FF == -1) {
            (void)((local_FF = 0) & 2);
            SCNF_RequestIconNotification(0, 0, 0, 2);
          }
          script_F2D7(local_F8);
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
        goto L00F2CB;
      }
    }
    L00F2CB:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_F2D7(int32_t arg0) {
  script_F2F5(arg0);
  script_F51D(arg0);
  script_F65E(arg0);
  return;
}

__attribute__((scn_string_bindings(0, data_1FB8F)))
static void script_F2F5(int32_t arg0) {
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
  script_12826(CHARA_AKIR, arg0, &local_F4);
  script_12865(CHARA_AKIR, arg0, &local_E8);
  script_11C89(1, arg0);
  Script_InitializeOsag(CHARA_AKIR);
  script_12024(arg0);
  script_11E02(2, arg0);
  SCNF_WaitFrames_0000(5);
  script_EE07();
  if ((arg0 == 0x54524144) | (arg0 == 0x32524144)) {
    task_1F8 = 2;
    (void)((task_25D = -1) & 1);
  } else {
    if (arg0 == 0x33524144) {
      task_1F8 = 3;
      (void)((task_25D = 0) & 1);
      goto L00F423;
    }
  }
  L00F423:;
  task_220 = script_12350(arg0, 0, 2, 0);
  task_224 = script_12350(arg0, 0, 3, 0);
  (void)((task_25E = 0) & 1);
  task_25F = 15;
  Script_ccowUtils(CHARA_AKIR, 2);
  if (task_25D & 0x10) {
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
    Script_scenToggleVisibleFor(*(&data_1FB54 + local_E4), 1);
    Script_charaDisplay(*(&data_1FB54 + local_E4), CDM_Hide);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_F51D(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (script_F828(arg0)) {
    script_12993();
    local_F4 = Script_scenCreateTask(script_12330, 0);
    for (;;) {
      script_12350(arg0, 1, 0, script_12350(arg0, 0, 0, 0) + 1);
      script_F9AC(arg0);
      local_FC = Script_scenCreateTask(script_119DC, 1, arg0);
      script_1000A(arg0);
      local_F8 = Script_scenCreateTask(script_105FA, 0);
      if (task_25D & 0x10) {
        Script_executeAicaCommand(131496, 0, 0);
        goto L00F5AE;
      }
      L00F5AE:;
      script_1005C(arg0);
      if (task_25E & 0x20) {
        task_23C = 0;
        task_258 = 0;
        SCNF_WaitFrames_0000(1);
      } else {
        if (!(script_10FFB(arg0) == 0)) goto L00F5AE;
      }
      Script_executeAicaCommand(0x1200A0, 4, 0);
      script_104BF(arg0);
      Script_srchControl((void *)local_FC);
      Script_srchControl((void *)local_F8);
      if (task_25E & 0x20) {
        break;
      }
      if (script_11892(arg0) == 0) {
        break;
      }
    }
    Script_srchControl((void *)local_F4);
    script_12350(arg0, 1, 1, script_12350(arg0, 0, 1, 0) + task_240);
    script_129B8();
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1FB95)))
static void script_F65E(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  script_12350(arg0, 1, 2, task_220);
  script_12350(arg0, 1, 3, task_224);
  local_FC = 0;
  while (local_FC < 5) {
    Script_MoveObjectToObject(*(&data_1FB54 + local_FC), -1);
    Script_charaDisplay(*(&data_1FB54 + local_FC), CDM_Hide);
    Script_scenToggleVisibleFor(*(&data_1FB54 + local_FC), 0);
    script_11C37(arg0, local_FC, 0);
    local_FC = local_FC + 1;
  }
  Script_convSetCamInactiveFlag(1);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_11E02(0, arg0);
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
  script_12826(CHARA_AKIR, arg0, &local_F0);
  script_12865(CHARA_AKIR, arg0, &local_E4);
  Script_InitializeOsag(CHARA_AKIR);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  script_EE43();
  task_1F8 = 0;
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
  return;
}

static void script_F7FF(void) {
  task_258 = 0;
  task_238 = 0;
  task_23C = 0;
  (void)((task_25C = 0) & 1);
  task_25F = 15;
  return;
}

__attribute__((scn_string_bindings(7, data_1FBE1)))
static int32_t script_F828(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  int32_t local_F4;
  int32_t local_F0;
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_11FF8, 2, 9, arg0);
  script_11D98(2, "E1025A001", "E1025A002");
  SCNF_WaitFrames_0000(135);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 36, 47, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  if (local_FC == 36) {
    local_F4 = script_008F(0, 100, 1);
    local_F0 = local_F4;
    if (local_F0 == 0) {
      script_11D98(3, "E1025A003", "E1025A004", "E1025A005");
      while (Script_convGetIsActive((const void *)task_228)) {
        SCNF_WaitFrames_0000(1);
      }
      (void)((local_FB = -1) & 1);
    } else if (local_F0 == 1) {
      script_F971();
      (void)((local_FB = 0) & 1);
    } else {
      (void)((local_FB = 0) & 1);
    }
  } else {
    script_11E02(7, arg0);
    script_11D98(3, "E1025A009", "E1025A010", "E1025A011");
    while (Script_convGetIsActive((const void *)task_228)) {
      SCNF_WaitFrames_0000(1);
    }
    (void)((local_FB = 0) & 1);
  }
  Script_sicnUtils(2, 1, 0);
  Script_shenClearFreeState(0);
  return local_FB & 8;
}

static void script_F971(void) {
  script_11D98(3, "E1025A006", "E1025A007", "E1025A008");
  SCNF_SetCashValue(0, 1);
  while (Script_convGetIsActive((const void *)task_228)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_F9AC(int32_t arg0) {
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
  script_12826(CHARA_AKIR, arg0, &local_E8);
  script_12865(CHARA_AKIR, arg0, &local_D0);
  Script_PlayCharaMotion(CHARA_AKIR, 31012, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_InitializeOsag(CHARA_AKIR);
  script_11C89(0);
  local_C8 = 0;
  L00FA4E:;
  Script_charaDisplay(*(&data_1FB54 + local_C8), CDM_Hide);
  Script_MoveObjectToObject(*(&data_1FB54 + local_C8), arg0);
  local_C8 = local_C8 + 1;
  if (!(local_C8 == 5)) goto L00FA4E;
  local_F4.x = 1.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.5f;
  script_11E02(5, arg0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_11FF8, 2, 6, arg0);
  Script_scenCreateTask(script_FFDB, 0);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  for (;;) {
    local_CC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    } else {
      switch (local_CC) {
        int32_t local_C4;
        case 30:
          Script_executeAicaCommand(66985, 0, 0);
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
          task_23C = 0;
          task_238 = 0;
          task_258 = 100000;
          script_119F3(arg0);
          Script_executeAicaCommand(1449, 0, 0);
          Script_fixoClearAttachment(0x6E696F43);
          Script_charaDisplay(0x6E696F43, CDM_Hide);
          local_C4 = 0;
          while (local_C4 < 5) {
            script_11C37(arg0, local_C4, 1);
            local_C4 = local_C4 + 1;
          }
          break;
      }
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_1C964, 1);
    SCNF_WaitFrames_0000(1);
  }
  script_11E02(14, arg0);
  local_E8.x = -0.045f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.6f;
  script_12826(CHARA_AKIR, arg0, &local_E8);
  Script_SetNewHandMotion(CHARA_AKIR, 0, 863, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 864, 0, 0, 0, 0, 1.0f, 30);
  Script_PlayCharaMotion(CHARA_AKIR, 31013, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_executeAicaCommand(263593, 0, 0);
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
            Script_executeAicaCommand(329129, 0, 0);
            Script_EVT_GetPosition(CHARA_AKIR, 12, &local_E8, 0);
            local_C4 = 0;
            L00FD74:;
            Script_EVT_SetPosition(*(&data_1FB54 + local_C4), POS_XYZ, &local_E8);
            local_C4 = local_C4 + 1;
            if (!(local_C4 >= 5)) goto L00FD74;
            break;
          }
        case 56:
          {
            int32_t local_C4;
            local_C4 = 0;
            L00FDAF:;
            Script_charaDisplay(*(&data_1FB54 + local_C4), CDM_Show);
            Script_fixoSetAttachmentOffset(*(&data_1FB54 + local_C4), 0, &data_1FC0C, &data_1FC18);
            local_DC.x = *(float *)(&data_1FC24 + local_C4 * 3);
            (&local_DC)->y = *(float *)(&data_1FC24 + (local_C4 * 3 + 1));
            (&local_DC)->z = *(float *)(&data_1FC24 + (local_C4 * 3 + 2));
            local_D0.x = *(int32_t *)(data_1FC60 + ((local_C4 * 3) << 2));
            (&local_D0)->y = *(int32_t *)(data_1FC60 + ((local_C4 * 3 + 1) << 2));
            (&local_D0)->z = *(int32_t *)(data_1FC60 + ((local_C4 * 3 + 2) << 2));
            Script_fixoAttachEx(*(&data_1FB54 + local_C4), CHARA_AKIR, 12, &local_DC, &local_D0);
            local_C4 = local_C4 + 1;
            if (!(local_C4 >= 5)) goto L00FDAF;
            break;
          }
        case 125:
          script_11E02(10, arg0);
          break;
        case 150:
          Script_SetNewHandMotion(CHARA_AKIR, 1, 861, 0, 0, 0, 0, 1.0f, 20);
          break;
        case 180:
          Script_EVT_GetPosition(CHARA_AKIR, 18, &local_E8, 0);
          Script_EVT_SetPosition(SHEN_NAME('YA_Y'), POS_XYZ, &local_E8);
          break;
        case 181:
          Script_fixoSetAttachmentOffset(SHEN_NAME('YA_Y'), 0, &data_1FC0C, &data_1FC18);
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
  script_120EE(CHARA_AKIR);
  Script_shenClearFreeState(0);
  Script_sicnUtils(2, 1, 0);
  return;
}

static int32_t script_FFDB(void) {
  int32_t local_FC;
  local_FC = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(-100, 1);
  local_FC = local_FC - 100;
  Script_SetStats(STATS_CASH, local_FC);
  return Script_srchControl(0);
}

static void script_1000A(int32_t arg0) {
  task_244 = 5;
  if (task_25D & 0x10) {
    task_254 = 0;
  } else {
    task_254 = 3;
  }
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1FC9C);
  Script_charaSetRenderOpacity(CHARA_AKIR, 0.5f);
  Script_lightSetSlotEnabled(15, 1);
  task_256 = 0;
  return;
}

static void script_1005C(int32_t arg0) {
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
  local_FF = task_244 % 5;
  local_C0 = local_FF;
  switch (local_C0) {
    case 0:
      task_234 = 0x595F4159;
      break;
    case 4:
      task_234 = 0x525F4159;
      break;
    case 3:
      task_234 = 0x425F4159;
      break;
    case 2:
      task_234 = 0x475F4159;
      break;
    case 1:
      task_234 = 0x4B5F4159;
      break;
  }
  Script_charaDisplay(task_234, CDM_Show);
  if (task_244 == 1) {
    script_11D98(3, "E1025A022", "E1025A023", "E1025A024");
    while (Script_convGetIsActive((const void *)task_228)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  if (task_25D & 0x10) {
    task_24C = (int32_t)(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 360.0f) / 360.0f * (float)0x10000);
    task_250 = (int32_t)(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 360.0f) / 360.0f * (float)0x10000);
    Script_PlayCharaMotion(CHARA_AKIR, 31014, 0.0f, 44.0f, 27.0f, 0.0f, 0);
  } else {
    task_250 = 0;
    Script_PlayCharaMotion(CHARA_AKIR, 31014, 0.0f, 44.0f, 0.0f, 0.0f, 0);
  }
  Script_SetNewHandMotion(CHARA_AKIR, 1, 861, 0, 0, 0, 0, 1.0f, 0);
  local_DC.x = -0.015f;
  (&local_DC)->y = -0.002f;
  (&local_DC)->z = 1.8f;
  local_C4.x = 0;
  (&local_C4)->y = 0;
  (&local_C4)->z = 0;
  script_12826(CHARA_AKIR, arg0, &local_DC);
  script_12865(CHARA_AKIR, arg0, &local_C4);
  script_109F2(arg0);
  script_11E02(3, arg0);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  if (task_25D & 0x10) {
    Script_fixoClearAttachment(task_234);
    Script_MoveObjectToObject(CHARA_AKIR, task_234);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_D0, 0);
    Script_MoveObjectToObject(CHARA_AKIR, -1);
    Script_EVT_GetPosition(task_234, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_SetPosition(0x6E696F43, 0x78000000, &local_DC);
  } else {
    local_DC.x = (&local_DC)->x + 0.025f;
    script_12826(CHARA_AKIR, arg0, &local_DC);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  }
  task_258 = 100000;
  task_256 = 0;
  (void)((task_25C = -1) & 0x10);
  Script_sicnUtils(0, 4, 0);
  for (;;) {
    if (task_25D & 0x10) {
      script_10729();
      script_10656();
      script_1081D();
      script_107E1(&local_E8, &local_D0);
    } else {
      script_10933(arg0);
    }
    SCNF_WaitFrames_0000(1);
    local_F4 = Script_GetControllerInput(1);
    if (local_F4 == 0x400) {
      Script_sicnUtils(1, 0, 0);
      if (task_25D & 0x10) {
        script_10C20((int32_t)&local_B4);
      } else {
        script_10CF9((int32_t)&local_B4);
      }
      script_109F2(arg0);
      script_10AD0(arg0, (int32_t)&local_B4);
      break;
    } else if (local_F4 == 0x200) {
      (void)((task_25C = 0) & 2);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
      Script_charaDisplay(CHARA_AKIR, CDM_Hide);
      Script_charaDisplay(task_234, CDM_Hide);
      local_BC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
      if (local_BC != 37) {
        Script_sicnUtils(1, 0, 0);
        (void)((task_25E = -1) & 0x80);
        break;
      } else if (task_25D & 0x10) {
        task_24C = (int32_t)(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 360.0f) / 360.0f * (float)0x10000);
        task_250 = (int32_t)(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 360.0f) / 360.0f * (float)0x10000);
      } else {
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
      }
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      Script_charaDisplay(task_234, CDM_Show);
      (void)((task_25C = -1) & 4);
    } else {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
        Script_sicnUtils(1, 0, 0);
        (void)((task_25E = -1) & 4);
        break;
        goto L0104BB;
      }
    }
    L0104BB:;
  }
  return;
}

static void script_104BF(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1FCCC);
  Script_lightSetSlotEnabled(15, 0);
  Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  if (task_224 / 10000 >= 250) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 942) == 0) {
      script_D2CB(1);
      (void)((task_25E = -1) & 8);
    }
  }
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
  script_12826(CHARA_AKIR, arg0, &local_F4);
  script_12865(CHARA_AKIR, arg0, &local_E8);
  script_11C89(3);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_105FA(void) {
  for (;;) {
    if (task_25C & 8) {
      script_10617();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_10617(void) {
  if (~task_25D) {
  } else if (task_258 > 0) {
    task_258 = 100000 - task_256 * 166;
    task_256 = task_256 + 1;
  } else {
    task_258 = 0;
  }
  return;
}

static void script_10656(void) {
  float local_FC;
  float local_F8;
  float local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  local_F4 = 300.0f;
  local_D8 = Script_CEWPRelated(task_234);
  local_D4 = task_250;
  local_D0 = task_250 + (int32_t)(360.0f / local_F4 / 360.0f * (float)0x10000);
  local_FC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_D4) * 0.04f;
  local_F8 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_D0) * 0.04f;
  Script_EVT_GetPosition(task_234, -1, &local_E8, 0);
  local_E8.x = local_FC + -0.119f;
  Script_EVT_SetPosition(task_234, POS_XYZ, &local_E8);
  Script_MoveObjectToObject(0x6E696F43, local_D8);
  Script_EVT_GetPosition(0x6E696F43, -1, &local_DC, 0);
  local_DC.x = local_F8 + -0.119f;
  Script_EVT_SetPosition(0x6E696F43, POS_XYZ, &local_DC);
  Script_MoveObjectToObject(0x6E696F43, -1);
  task_250 = local_D0;
  return;
}

static void script_10729(void) {
  float local_FC;
  float local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  int32_t local_D4;
  local_D4 = Script_CEWPRelated(task_234);
  local_F4 = task_24C;
  local_F0 = task_24C + 18;
  script_12940(local_F4, &local_FC);
  script_12940(local_F0, &local_F8);
  Script_EVT_GetPosition(task_234, -1, &local_E4, 0);
  (&local_E4)->y = local_FC * 0.05f + 1.5891f;
  Script_EVT_SetPosition(task_234, POS_XYZ, &local_E4);
  Script_MoveObjectToObject(0x6E696F43, local_D4);
  Script_EVT_GetPosition(0x6E696F43, -1, &local_D8, 0);
  (&local_D8)->y = local_F8 * 0.05f + 1.5891f;
  Script_EVT_SetPosition(0x6E696F43, POS_XYZ, &local_D8);
  Script_MoveObjectToObject(0x6E696F43, -1);
  task_24C = local_F0;
  return;
}

static void script_107E1(const Vector3 *arg0, const Vector3i *arg1) {
  Script_MoveObjectToObject(CHARA_AKIR, task_234);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, arg0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, arg1, 0);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  return;
}

static void script_1081D(void) {
  Vector3 local_F4;
  int32_t local_E8[3];
  Vector3i local_DC;
  float local_D8;
  Script_EVT_GetPosition(task_234, -1, &local_F4, 0);
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
  Script_EVT_SetRotation(task_234, 0x38000000, &local_DC, 0);
  return;
}

static void script_10933(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3i local_EC;
  task_250 = task_250 % 90;
  if (task_250 <= 22) {
    local_F8 = task_250;
    local_FC = local_F8 * 41;
  } else if (task_250 <= 44) {
    local_F8 = task_250 - 22;
    local_FC = 910 - local_F8 * 41;
  } else if (task_250 <= 67) {
    local_F8 = task_250 - 44;
    local_FC = -local_F8 * 39;
  } else {
    local_F8 = task_250 - 67;
    local_FC = local_F8 * 39 + -910;
  }
  local_EC.x = 0;
  (&local_EC)->y = -local_FC;
  (&local_EC)->z = 0;
  script_12865(CHARA_AKIR, arg0, &local_EC);
  task_250 = task_250 + 1;
  return;
}

static void script_109F2(int32_t arg0) {
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.0882f;
    (&local_F4)->y = -0.0112f;
    (&local_F4)->z = 0.0f;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_fixoSetAttachmentOffset(task_234, 0, &local_F4, &local_E8);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    if (task_25D & 0x10) {
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
    Script_fixoAttachEx(task_234, CHARA_AKIR, 18, &local_F4, &local_E8);
  }
  return;
}

static void script_10AD0(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_PlayCharaMotion(CHARA_AKIR, 31015, 0.0f, 70.0f, 0.0f, 1.0f, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (local_FC == 26) {
      script_11E02(4, arg0);
      Script_executeAicaCommand(394665, 0, 0);
      Script_SetNewHandMotion(CHARA_AKIR, 1, 862, 0, 0, 0, 0, 1.0f, 0);
      Script_fixoClearAttachment(task_234);
      (void)((task_248 = 0) & 0x10);
      SCNF_WaitFrames_0000(1);
      Script_scenCreateTask(script_11FF8, 2, 8, arg0);
      Script_scenCreateTask(script_10E17, 2, arg0, arg1);
      break;
    }
  }
  for (;;) {
    if (task_248 == -1) {
      (void)((task_25C = 0) & 0x10);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_11C37(arg0, 5 - task_244, 0);
  task_244 = task_244 - 1;
  if (task_244 > 0) {
    Script_PlayCharaMotion(CHARA_AKIR, 31014, 0.0f, 0.0f, 0.0f, 0.0f, 0);
  } else {
    if (task_244 <= 0) {
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      goto L010C17;
    }
  }
  L010C17:;
  SCNF_WaitFrames_0000(15);
  return;
}

static void script_10C20(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_F0;
  float local_E8[2];
  Vector3 local_DC;
  int32_t local_D8;
  local_D8 = Script_CEWPRelated(task_234);
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

static void script_10CF9(int32_t arg0) {
  float local_FC;
  local_FC = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR);
  if (task_250 <= 22) {
    int32_t local_F8;
    local_F8 = task_250;
    *(float *)arg0 = (float)local_F8 * 0.0090909088f;
  } else if (task_250 <= 44) {
    int32_t local_F8;
    local_F8 = task_250 - 22;
    *(float *)arg0 = 0.2f - (float)local_F8 * 0.0090909088f;
  } else if (task_250 <= 67) {
    int32_t local_F8;
    local_F8 = task_250 - 44;
    *(float *)arg0 = (float)local_F8 * -0.00869565271f;
  } else {
    int32_t local_F8;
    local_F8 = task_250 - 67;
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

static int32_t script_10E17(int32_t arg0, int32_t arg1) {
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
  Script_EVT_SetRotation(task_234, 0x38000000, &local_C4, 0);
  local_E8[0] = *(float *)arg1;
  local_E8[1] = *(float *)(arg1 + 4);
  local_E8[2] = -0.0682000071f;
  if (task_25D & 0x10) {
    local_D8 = 0.0f;
    local_D4 = 0.0f;
  } else {
    local_D8 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.034f) - 0.017f;
    local_D4 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.034f) - 0.017f;
  }
  Script_EVT_GetPosition(task_234, -1, &local_F4, 0);
  local_DC[0] = (*local_E8 + local_D8 - (&local_F4)->x) / 13.0f;
  local_DC[1] = (local_E8[1] + local_D4 + 1.4174f - (&local_F4)->y) / 13.0f;
  local_DC[2] = (local_E8[2] - (&local_F4)->z) / 13.0f;
  local_D3 = Script_FloatUtils(ScriptMathOp_FloatToInt, 13.0f);
  for (;;) {
    local_F4.x = (&local_F4)->x + *local_DC;
    (&local_F4)->y = (&local_F4)->y + local_DC[1];
    (&local_F4)->z = (&local_F4)->z + local_DC[2];
    Script_EVT_SetPosition(task_234, POS_XYZ, &local_F4);
    local_D3 = local_D3 - 1;
    if (local_D3 == 0) {
      local_C0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      switch (local_C0) {
        case 0:
          Script_executeAicaCommand(132521, 0, 0);
          break;
        case 1:
          Script_executeAicaCommand(198057, 0, 0);
          break;
      }
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_GetPosition(task_234, -1, &local_F4, 0);
  task_238 = script_1212C((&local_F4)->x, (&local_F4)->y - 1.4174f);
  (void)((task_248 = -1) & 0x10);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(3, data_1FCFA, 5, data_1FD0E)))
static int32_t script_10FFB(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  if (task_25D & 0x10) {
    local_FC = task_238 / 10000;
  } else {
    local_FC = task_238 / 2;
  }
  SCNF_WaitFrames_0000(1);
  if (local_FC == 0) {
    script_11D98(2, "E1025A016", "E1025A017");
  } else if (local_FC <= 10) {
    script_11D89("E1025A013");
  } else if (local_FC <= 15) {
    script_11D98(2, "E1025A018", "E1025A048");
  } else if (local_FC <= 20) {
    script_11D98(2, "E1025A018", "E1025A046");
  } else if (local_FC <= 30) {
    script_11D98(2, "E1025A012", "E1025A047");
  } else if (local_FC <= 45) {
    script_11D89("E1025A014");
  } else {
    script_11D89("E1025A015");
  }
  while (Script_convGetIsActive((const void *)task_228)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(24);
  local_FC = task_238;
  script_11762((int32_t)&local_FC);
  SCNF_WaitFrames_0000(36);
  if (-(task_258 > 0) & task_25D) {
    task_25F = task_25F | 0x40 | 0x20;
    script_11762((int32_t)&task_258);
    task_25F = task_25F & -97;
    SCNF_WaitFrames_0000(30);
  }
  if (-(task_244 == 0) & task_25D) {
    task_254 = task_254 + 1;
    if ((task_254 <= 2) & (task_23C / 10000 >= task_254 * 120)) {
      uint8_t unused_F8[3];
      int32_t local_F4;
      script_112E8(arg0);
      task_244 = 5;
      local_F4 = 0;
      while (local_F4 < 5) {
        Script_charaDisplay(*(&data_1FB54 + local_F4), CDM_Hide);
        script_11C37(arg0, local_F4, 1);
        local_F4 = local_F4 + 1;
      }
      Script_executeAicaCommand(131496, 0, 0);
    }
  }
  if (task_244 == 0) {
    uint8_t unused_F8[3];
    int32_t local_F4;
    local_F4 = 0;
    L0111E7:;
    task_25F = 15;
    if (local_F4 % 4 < 2) {
      task_25F = task_25F & -2;
    }
    SCNF_WaitFrames_0000(1);
    local_F4 = local_F4 + 1;
    if (!(local_F4 == 30)) goto L0111E7;
    task_25F = 15;
    task_224 = task_23C;
    if (task_220 < task_23C) {
      int8_t local_F3;
      task_220 = task_23C;
      (void)((local_F3 = 0) & 2);
      if (task_25D & 0x10) {
        if (task_220 > 777777) {
          (void)((local_F3 = -1) & 2);
        }
      } else {
        if (task_220 > 50) {
          (void)((local_F3 = -1) & 2);
        }
      }
      if (local_F3 & 8) {
        Script_executeAicaCommand(65960, 0, 0);
        script_11D98(3, "E1025A050", "E1025A051", "E1025A052");
        task_23C = 0;
        SCNF_WaitFrames_0000(90);
      }
    }
    task_23C = 0;
    SCNF_WaitFrames_0000(15);
    (void)((local_FB = 0) & 2);
  } else {
    (void)((local_FB = -1) & 2);
  }
  return local_FB & 8;
}

static void script_112E8(int32_t arg0) {
  Vector3 local_F4;
  int8_t local_EC[8];
  int32_t local_E8;
  int32_t local_E4;
  int8_t local_E3;
  task_25F = 0;
  Script_executeAicaCommand(424, 0, 0);
  local_F4.x = -0.127600014f;
  (&local_F4)->y = 2.155f;
  (&local_F4)->z = -0.04f;
  local_E3 = task_254 + 1;
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
    script_11D98(3, "E1025A037", "E1025A038", "E1025A039");
  }
  if (local_E3 == 3) {
    script_11D98(3, "E1025A040", "E1025A041", "E1025A042");
  }
  local_E4 = 0;
  while (local_E4 < 80) {
    Script_GameUtils(arg0, GC_CommitStagedEntries, 0, 0, 0, 0);
    Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1FDA4, 9, 0);
    Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_E3, &local_F4, 1.0f, 0);
    local_E8 = 0;
    L01141B:;
    if (local_E4 >= local_E8 * 7) {
      script_11643(local_EC[local_E8], local_E8, arg0);
    }
    local_E8 = local_E8 + 1;
    if (!(local_E8 == 8)) goto L01141B;
    script_11488(arg0, 2);
    if (local_E4 % 4 < 2) {
      script_11488(arg0, 3);
    }
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  task_25F = 15;
  return;
}

static void script_11488(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  local_E4 = arg1;
  switch (local_E4) {
    case 0:
      local_F0 = task_220;
      if (local_F0 < 777777) {
        local_F0 = 777777;
      }
      break;
    case 1:
      local_F0 = task_224;
      break;
    case 2:
      local_F0 = task_258;
      break;
    case 3:
      local_F0 = task_23C;
      break;
    case 4:
      local_F0 = task_238;
      break;
  }
  local_EC = local_F0 / 10000;
  local_E8 = local_F0 - local_EC * 10000;
  if (-(arg1 == 3) & (task_25F & 0x20)) {
    local_E8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 10000);
  }
  if (-(arg1 == 2) & (task_25F & 0x40)) {
    local_E8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 10000);
  }
  local_F4.x = *(float *)&data_1FDC8;
  (&local_F4)->y = *(float *)(&data_1FDE0 + arg1);
  (&local_F4)->z = *(float *)(&data_1FDC8 + 2);
  Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1FDB0, 9999, 0);
  Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_EC, &local_F4, 1.0f, 0);
  local_F4.x = *(float *)&data_1FDBC * (float)4 + 0.1474f;
  (&local_F4)->y = *(float *)(&data_1FDF4 + arg1);
  (&local_F4)->z = -0.04f;
  Script_GameUtils(arg0, GC_StageNodeData, 336, &local_F4, 1.0f, &data_1FE08);
  local_F4.x = *(float *)&data_1FDD4;
  (&local_F4)->y = *(float *)(&data_1FDF4 + arg1);
  (&local_F4)->z = *(float *)(&data_1FDD4 + 2);
  script_11B8E(arg0, 0x100, (const Vector3 *)&data_1FDBC, 4, local_E8, &local_F4, &data_1FE08);
  return;
}

static void script_11643(int8_t arg0, int8_t arg1, int32_t arg2) {
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

static void script_11762(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  if (task_25D & 0x10) {
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
    if (task_25D & 0x10) {
      local_FC = local_FC * 10000;
      local_F8 = local_F8 * 10000;
    }
    local_F0 = local_F4;
    Script_executeAicaCommand(460201, 0, 0);
    for (;;) {
      if (local_F8 == 0) {
        Script_executeAicaCommand(525737, 0, 0);
        break;
      } else if (local_F0 == 0) {
        if (local_F8 < local_FC) {
          local_FC = local_F8;
        }
        local_F8 = local_F8 - local_FC;
        task_23C = task_23C + local_FC;
        *(int32_t *)arg0 = *(int32_t *)arg0 - local_FC;
        local_F0 = local_F4;
      } else {
        if (local_F0 != 0) {
          local_F0 = local_F0 - 1;
          goto L011874;
        }
      }
      L011874:;
      SCNF_WaitFrames_0000(1);
    }
  }
  task_23C = task_23C + *(int32_t *)arg0;
  *(int32_t *)arg0 = 0;
  return;
}

__attribute__((scn_string_bindings(6, data_1FE50)))
static int32_t script_11892(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  (void)((local_FB = 0) & 1);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  script_11E02(7, arg0);
  script_11C89(2, arg0);
  script_119F3(arg0);
  script_11D98(3, "E1025A025", "E1025A026", "E1025A027");
  while (Script_convGetIsActive((const void *)task_228)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    (void)0;
  } else {
    local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    local_FA = script_008F(0, 100, 1);
    if ((local_FC == 37) & (local_FA == 0)) {
      script_11D98(3, "E1025A028", "E1025A029", "E1025A030");
      while (Script_convGetIsActive((const void *)task_228)) {
        SCNF_WaitFrames_0000(1);
      }
      (void)((local_FB = -1) & 1);
    } else {
      switch (local_FA) {
        case 0:
          script_11D98(3, "E1025A011", "E1025A031", "E1025A032");
          while (Script_convGetIsActive((const void *)task_228)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 1:
          script_F971();
          break;
      }
    }
    (void)(local_FB & 8);
  }
  end:;
}

static int32_t script_119DC(int32_t arg0) {
  for (;;) {
    script_119F3(arg0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_119F3(int32_t arg0) {
  if (arg0 != 0x33524144) {
    script_11A17(arg0);
  } else {
    script_11A9D(arg0);
  }
  return;
}

static void script_11A17(int32_t arg0) {
  int8_t local_FF;
  local_FF = task_25F;
  if (task_25F) {
    Script_GameUtils(arg0, GC_CommitStagedEntries, 0, 0, 0, 0);
    if (local_FF & 2) {
      script_11488(arg0, 0);
    }
    if (local_FF & 4) {
      script_11488(arg0, 1);
    }
    if (local_FF & 8) {
      script_11488(arg0, 2);
    }
    if (local_FF & 1) {
      script_11488(arg0, 3);
    }
    if (local_FF & 0x10) {
      script_11488(arg0, 4);
    }
  }
  return;
}

static void script_11A9D(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FF = task_25F;
  local_F8 = task_220;
  local_F4 = task_224;
  local_F0 = task_23C;
  if (local_F8 < 50) {
    local_F8 = 50;
  }
  if (task_25F) {
    Script_GameUtils(arg0, GC_CommitStagedEntries, 0, 0, 0, 0);
    if (local_FF & 1) {
      Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1FE70, 9999, 0);
      Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_F0, &data_1FE88, 1.0f, &data_1FEB8);
    }
    if (local_FF & 4) {
      Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1FE7C, 9999, 0);
      Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_F4, &data_1FE94, 1.0f, &data_1FEAC);
    }
    if (local_FF & 2) {
      Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1FE7C, 9999, 0);
      Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_F8, &data_1FEA0, 1.0f, &data_1FEAC);
    }
  }
  return;
}

static void script_11B8E(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3, int32_t arg4, const Vector3 *arg5, const Vector3 *arg6) {
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

static void script_11C37(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 0.0f;
  local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
  if (arg2) {
    (&local_F4)->z = -10.05f;
  }
  Script_charaPosRotUtils(arg0, *(&data_1FEC4 + arg1), &local_F4, &local_E8, 0);
  return;
}

static void script_11C89(int32_t arg0, ...) {
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

static void script_11D89(const char *arg0) {
  task_228 = Script_ShowUIMessageFromID(arg0);
  return;
}

static void script_11D98(int32_t arg0, const char *arg1, const char *arg2, ...) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = Script_FloatUtils(ScriptMathOp_RandomIntBelow, arg0);
  local_F8 = local_FF;
  if (local_F8 == 1) {
    task_228 = Script_ShowUIMessageFromID(arg2);
  } else if (local_F8 == 2) {
    {
      va_list arguments0;
      va_start(arguments0, arg2);
      task_228 = Script_ShowUIMessageFromID(va_arg(arguments0, const char *));
      va_end(arguments0);
    }
  } else if (local_F8 == 3) {
    {
      va_list arguments1;
      va_start(arguments1, arg2);
      (void)va_arg(arguments1, const char *);
      task_228 = Script_ShowUIMessageFromID(va_arg(arguments1, const char *));
      va_end(arguments1);
    }
  } else {
    task_228 = Script_ShowUIMessageFromID(arg1);
  }
  return;
}

static void script_11E02(int32_t arg0, int32_t arg1) {
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
      if (task_25D & 0x10) {
        Script_seqEnterEvtCam(7201, arg1, arg1);
      } else {
        Script_seqEnterEvtCam(7218, arg1, arg1);
      }
      break;
    case 4:
      Script_seqEnterEvtCam(7202, task_234, task_234);
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

static int32_t script_11FF8(int32_t arg0, int32_t arg1) {
  for (;;) {
    if (Script_IsInEventCam() == 0) {
      script_11E02(arg0, arg1);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_12024(int32_t arg0) {
  Vector4 local_F0;
  int32_t local_E8[2];
  Vector3 local_DC;
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1FED8);
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
  script_12887(arg0, &local_DC);
  Script_LightUtils(15, 7, &local_DC);
  Script_lightSetSlotEnabled(15, 0);
  return;
}

static void script_120EE(int32_t arg0) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
  }
  return;
}

static void script_12108(int32_t arg0, const Vector3 *arg1) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, arg1, 1);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1212C(float arg0, float arg1) {
  int32_t local_FC;
  local_FC = 0;
  if (task_25D & 0x10) {
    local_FC = script_12165(arg0, arg1);
  } else {
    local_FC = script_1227F(arg0, arg1);
  }
  return local_FC;
}

static int32_t script_12165(float arg0, float arg1) {
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
    local_F0 = data_1FEE8[local_F3];
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

static int32_t script_1227F(float arg0, float arg1) {
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

static int32_t script_12330(void) {
  task_240 = 0;
  for (;;) {
    task_240 = task_240 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_12350(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FC = 0;
  while (arg0 != *(&data_1FB48 + local_FC)) {
    local_FC = local_FC + 1;
  }
  local_F4 = *((int8_t *)&data_1FB18 + (local_FC * 16 + 14));
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

static void script_12826(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_12846(int32_t arg0, int32_t arg1, Vector3 *arg2) {
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, arg2, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_12865(int32_t arg0, int32_t arg1, const Vector3i *arg2) {
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_12887(int32_t arg0, Vector3 *arg1) {
  Script_charaDisplay(0x6E696F43, CDM_Hide);
  Script_MoveObjectToObject(0x6E696F43, arg0);
  Script_EVT_SetPosition(0x6E696F43, POS_XYZ, arg1);
  Script_MoveObjectToObject(0x6E696F43, -1);
  Script_EVT_GetPosition(0x6E696F43, -1, arg1, 0);
  return;
}

static void script_128CA(int32_t arg0, Vector3 *arg1) {
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

static void script_12940(int32_t arg0, float *arg1) {
  *arg1 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg0 * 22) * 0.3f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg0 * 31) * 0.37f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg0 * 103) * 0.15f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg0 * 147) * 0.18f;
  return;
}

static void script_12993(void) {
  Script_scenCreateTask(script_129EE, 3, 3, 87, 1);
  Script_scenCreateTask(script_129EE, 3, 2, 100, 1);
  return;
}

static void script_129B8(void) {
  if (task_48) {
    script_2820();
  }
  Script_executeAicaCommand(24232, 0, 0);
  Script_executeAicaCommand(1184, 3, 0);
  Script_scenCreateTask(script_12A2B, 3, 3, 87, 1);
  return;
}

static int32_t script_129EE(int8_t arg0, int8_t arg1, int8_t arg2) {
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

static int32_t script_12A2B(int8_t arg0, int8_t arg1, int8_t arg2) {
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

__attribute__((scn_string_bindings(8, data_1FF6C, 9, data_1FF77, 10, data_1FF81, 11, data_1FF8B)))
static void script_12A86(void) {
  int32_t local_FC;
  int8_t local_FB;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  script_12DB9(CHARA_AKIR);
  script_14D72(7000, SHEN_NAME('QTEM'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('QTEM'), 1);
  script_152AD();
  SCNF_WaitFrames_0000(1);
  script_13455();
  SCNF_WaitFrames_0000(20);
  script_148EC(2, 0, "E1026A001", "E1026A002", 0, 0, 0);
  script_14D72(7010, SHEN_NAME('QTEM'), 80);
  script_14995((const void *)task_260);
  SCNF_WaitFrames_0000(1);
  (void)(local_FC == 34);
  local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 36, 47, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  if (local_FC == 36) {
    uint8_t unused_F8[3];
    int32_t local_F4;
    local_FB = script_008F(0, 100, 1);
    local_F4 = local_FB;
    if (local_F4 == 0) {
      script_12993();
      script_12F87();
    } else if (local_F4 == 1) {
      Script_scenCreateTask(script_14E78, 0);
      script_148EC(3, 1, "E1026A006", "E1026A007", "E1026A008", 0, 0);
      task_268 = 1;
    } else {
      task_268 = 1;
    }
  } else {
    script_148EC(3, 1, "E1026A009", "E1026A010", "E1026A011", 0, 0);
    task_268 = 1;
  }
  for (;;) {
    uint8_t unused_F8[3];
    int32_t local_F4;
    local_F4 = task_268;
    switch (local_F4) {
      case 1:
        goto L012D9C;
        break;
      case 2:
        script_131E9(1);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        if ((task_1FC == task_200) & (task_1FC > 0)) {
          Script_LoadSND(3, 0, 0, 0);
          Script_LoadSND(1, 2, "rim008.snd", 0x8000);
          script_14750();
          Script_lightSetGlobalSceneLighting(&data_1FEFC);
          if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 940) == 0) & (task_1FC >= 200000)) {
            while (Script_LoadSND(2, 0, 0, 0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_LoadSND(4, 0, 0, 0);
            script_D2CB(2);
          } else {
            script_132C4();
            SCNF_WaitFrames_0000(61);
            while (Script_LoadSND(2, 0, 0, 0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_LoadSND(4, 0, 0, 0);
          }
        } else {
          script_14750();
          Script_lightSetGlobalSceneLighting(&data_1FEFC);
          script_14D72(7070, SHEN_NAME('QTEM'), 0);
          script_148EC(2, 0, "E1026A019", "E1026A019", 0, 0, 0);
          Script_LoadSND(3, 0, 0, 0);
          Script_LoadSND(1, 2, "rim008.snd", 0x8000);
          SCNF_WaitFrames_0000(100);
          while (Script_LoadSND(2, 0, 0, 0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_LoadSND(4, 0, 0, 0);
        }
        script_13E23();
        script_129B8();
        goto L012D9C;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L012D9C:;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  task_268 = 0;
  script_15335();
  return;
}

static void script_12DB9(int32_t arg0) {
  Vector3 local_F4;
  Script_EVT_GetPosition(SHEN_NAME('QTEM'), -1, &local_F4, 0);
  script_14E4B(&local_F4, 0.0f, 0.0f, 0.718f);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_EVT_SetRotation(arg0, 0x38000000, &data_1FF98, 0);
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  return;
}

static void script_12E38(void) {
  if ((task_264 >= task_1FC) & (task_1FC > 0)) {
    script_14D58(CHARA_AKIR);
    Script_executeAicaCommand(591273, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
    SCNF_WaitFrames_0000(30);
    Script_PlayCharaMotion(CHARA_AKIR, 30996, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 3.0f);
    script_14D72(7060, SHEN_NAME('QTEM'), 0);
    Script_SetNewHandMotion(CHARA_AKIR, 1, 774, 0, 0, 0, 0, 1.0f, 0);
    Script_ConfigHandMotionIK(CHARA_AKIR, 1, 1, 2.0f);
    Script_SetNewHandMotion(CHARA_AKIR, 1, 772, 0, 0, 0, 0, 1.0f, 0);
    script_14D72(7065, SHEN_NAME('QTEM'), 30);
    SCNF_WaitFrames_0000(20);
    Script_FACE(CHARA_AKIR, 1, 0, 20);
    SCNF_WaitFrames_0000(40);
  } else {
    Script_executeAicaCommand(722345, 0, 0);
    Script_FACE(CHARA_AKIR, 10, 0, 8);
    SCNF_WaitFrames_0000(30);
  }
  Script_FACE(CHARA_AKIR, 0, -1, 4);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  return;
}

static void script_12F87(void) {
  script_12FC5();
  task_268 = 0;
  task_269 = 1;
  Script_scenCreateTask(script_149B5, 0);
  script_134A1();
  script_14DB6();
  SCNF_WaitFrames_0000(40);
  Script_executeAicaCommand(5032, 0, 0);
  return;
}

static void script_12FC5(void) {
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  script_14D22(CHARA_AKIR, 30995, 0);
  script_14D72(7020, SHEN_NAME('QTEM'), 0);
  SCNF_WaitFrames_0000(10);
  script_148EC(3, 0, "E1026A003", "E1026A004", "E1026A005", 0, 0);
  Script_scenCreateTask(script_14D87, 0);
  Script_fixoAttachEx(SHEN_NAME('YENS'), CHARA_AKIR, 18, &data_1FFA4, &data_1FFB0);
  for (;;) {
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)40}).i) {
      Script_SetNewHandMotion(CHARA_AKIR, 1, 771, 0, 0, 0, 0, 1.0f, 0);
    } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)50}).i) {
      Script_executeAicaCommand(66985, 0, 0);
      Script_charaDisplay(SHEN_NAME('YENS'), CDM_Show);
    } else {
      if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)115}).i) {
        Script_executeAicaCommand(1449, 0, 0);
        break;
        goto L0130E4;
      }
    }
    L0130E4:;
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('YENS'));
  Script_charaDisplay(SHEN_NAME('YENS'), CDM_Hide);
  script_14D72(7030, SHEN_NAME('QTEM'), 20);
  Script_SetNewHandMotion(CHARA_AKIR, 0, 773, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 773, 0, 0, 0, 0, 1.0f, 0);
  script_14D22(CHARA_AKIR, 30997, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_CharHandlers(CHARA_AKIR, 0, 2, 16);
  script_14D72(7040, SHEN_NAME('QTEM'), 30);
  SCNF_WaitFrames_0000(50);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  script_13E23();
  Script_lightSetGlobalSceneLighting(&data_1FF0C);
  script_14D72(7090, SHEN_NAME('QTEM'), 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  task_264 = 0;
  return;
}

static void script_131E9(int32_t arg0) {
  int32_t local_FC;
  Script_lightSetGlobalSceneLighting(&data_1FEFC);
  if (arg0 == 1) {
    script_14D72(7055, SHEN_NAME('QTEM'), 0);
  } else {
    script_14D72(7050, SHEN_NAME('QTEM'), 0);
  }
  script_148EC(3, 1, "E1026A012", "E1026A013", "E1026A014", 0, 0);
  local_FC = task_264;
  if (arg0 == 1) {
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
    Script_motmToggleFlags(CHARA_AKIR, 1, 8);
    script_14D22(CHARA_AKIR, 30998, 0);
  } else {
    task_264 = 0;
  }
  script_12E38();
  task_264 = local_FC;
  script_14D58(CHARA_AKIR);
  Script_lightSetGlobalSceneLighting(&data_1FF0C);
  script_14D72(7090, SHEN_NAME('QTEM'), 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 8);
  return;
}

static void script_132C4(void) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  int32_t unused_E4[2];
  int32_t local_E0;
  int32_t unused_D8[2];
  int32_t local_D4;
  local_FC = script_EBC6();
  Script_SetCharaRefBoardPos(CHARA_NITO, &data_1FFF8);
  Script_charaSetRefboardFlagTo(CHARA_NITO, 1);
  Script_SetCPAMFlags(CHARA_NITO, 0);
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(7095, 0, AUTH_None);
  Script_PlayCharaMotion(CHARA_NITO, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F0, 0);
  Script_SetCharaLookAtPosition(CHARA_NITO, 8, &local_F0, 0);
  script_148EC(4, 1, "E1026B001", "E1026B002", "E1026B003", "E1026B004", 0);
  script_14D72(7070, SHEN_NAME('QTEM'), 0);
  Script_EVT_GetPosition(CHARA_NITO, -1, &local_F0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F0, 10);
  script_14E30(&local_F0, 0.6f, 1.5f, -7.7f);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 9, &local_F0, 0);
  script_148EC(3, 1, "E1026A015", "E1026A016", "E1026A017", 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_NITO, -98312, 0, 0);
  Script_charaSetRefboardFlagTo(CHARA_NITO, 2);
  script_ECE8(local_FC);
  script_E3F0(local_EC, (int32_t)&local_E0, (int32_t)&local_D4);
  return;
}

static void script_13455(void) {
  script_13E23();
  script_13D90(SHEN_NAME('QTEM'), 162, 1);
  script_13D90(SHEN_NAME('QTEM'), 163, 1);
  script_13D90(SHEN_NAME('QTEM'), 164, 1);
  script_13D90(SHEN_NAME('QTEM'), 165, 1);
  return;
}

static void script_134A1(void) {
  int8_t local_FF;
  int8_t local_FE;
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 2, "tsm007.snd", 0x8000);
  SCNF_WaitFrames_0000(15);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  Script_executeAicaCommand(70568, 0, 0);
  local_FE = 1;
  local_FF = 0;
  script_13DDB(SHEN_NAME('QTEM'), 170, 3, 14, 0);
  script_13DF3(0x3D6147AE, 0x3FCA3D71, task_1FC, 3, 14, 0);
  SCNF_WaitFrames_0000(85);
  script_13E23();
  task_27C = 0;
  Script_scenCreateTask(script_14841, 0);
  return;
}

static int32_t script_1355B(void) {
  float local_FC;
  float local_F8;
  int16_t local_F6;
  int16_t local_F4;
  int16_t local_F2;
  int16_t local_F0;
  int16_t local_EE;
  int16_t local_EC;
  int16_t local_EA;
  int16_t local_E8;
  int32_t local_E4;
  int32_t unused_E0;
  int8_t local_DF;
  int8_t local_DE;
  int8_t local_DD;
  int8_t local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int8_t local_D9;
  uint8_t unused_D8;
  float local_D4;
  float local_D0;
  int32_t local_CC;
  local_FC = 8.0f;
  local_F8 = 3.0f;
  local_F6 = 40;
  local_F4 = local_F6;
  local_F2 = 3;
  local_F0 = 40;
  local_EE = local_F0;
  local_EC = 1;
  local_DE = 1;
  local_EA = 0;
  local_DD = 2;
  local_D0 = 0.0f;
  local_CC = 0;
  local_DB = 0;
  local_DA = 3;
  local_E8 = 3;
  local_D9 = 0;
  task_27C = 1;
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  script_13EC6(local_DD);
  local_E4 = Script_scenCreateTask(script_13BE9, 0);
  script_13C63();
  task_26C = 1000;
  script_13B41();
  for (;;) {
    local_DF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_IncrementQTEMinigameRand16(1, local_F8));
    if (-(local_DE == Script_FloatUtils(ScriptMathOp_FloatToInt, local_FC - 1.0f)) & (-(local_DB == 0) | -(local_DB == 5))) {
      SCNF_WaitFrames_0000(1);
    } else {
      SCNF_WaitFrames_0000(local_F4 + local_DB);
    }
    Script_scenCreateTask(script_140BA, 2, *(int16_t *)((int8_t *)task_280 + (local_DF << 1)), local_F4 + local_DB);
    local_DC = script_1449F(local_F4);
    local_DE = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_IncrementQTEMinigameRand16(1, local_FC));
    if (task_278 != 0xFFFF) {
      if (-(task_278 != 0xA000) & task_278 & *(int16_t *)((int8_t *)task_280 + (local_DF << 1)) & -(task_278 > 0x2000)) {
        local_D4 = script_14446(local_DC, local_F4);
        local_D0 = local_D0 + local_D4;
        if (local_CC < 0x5F5DD18) {
          local_CC = local_CC + task_26C;
        } else {
          if (local_CC >= 0x5F5DD18) {
            local_CC = 0x5F5DD18;
            goto L013709;
          }
        }
        L013709:;
        if (task_264 < 0x5F5DD18) {
          task_264 = local_CC + Script_FloatUtils(ScriptMathOp_FloatToInt, local_D0);
        }
        if (task_264 >= task_1FC) {
          task_1FC = task_264;
        }
        local_EA = local_EA + 1;
        if ((local_D9 == 6) & (task_269 == 9)) {
          local_FC = 1.0f;
          task_269 = 6;
          Script_executeAicaCommand(656809, 0, 0);
        } else if ((local_D9 == 3) & (task_269 == 5)) {
          local_FC = 2.0f;
          task_269 = 3;
          Script_executeAicaCommand(656809, 0, 0);
        } else {
          if ((local_D9 == 2) & (task_269 == 4)) {
            local_FC = 5.0f;
            task_269 = 2;
            Script_executeAicaCommand(656809, 0, 0);
            goto L0137D3;
          }
        }
        L0137D3:;
        if (local_D9 >= 8) {
          local_F8 = 8.0f;
          local_FC = 1.0f;
        } else {
          switch (local_D9) {
            case 7:
              local_F8 = 8.0f;
              local_FC = 2.0f;
              break;
            case 6:
              local_F8 = 8.0f;
              local_FC = 3.0f;
              break;
            case 5:
              local_F8 = 8.0f;
              local_FC = 4.0f;
              break;
            case 4:
              local_F8 = 8.0f;
              local_FC = 5.0f;
              break;
            case 3:
              local_F8 = 8.0f;
              local_FC = 6.0f;
              break;
            case 2:
              local_F8 = 6.0f;
              local_FC = 8.0f;
              break;
            case 1:
              local_F8 = 4.0f;
              local_FC = 8.0f;
              script_13B95();
              break;
            case 0:
              local_F8 = 3.0f;
              local_FC = 8.0f;
              script_13B41();
              break;
          }
        }
        switch (local_D9) {
          case 11:
            local_F4 = 17 - local_EA / 10 * 1;
            if (local_EA >= 49) {
              local_EA = -1;
              local_D9 = 11;
            }
            break;
          case 6:
            local_F4 = 19 - local_EA / 10 * 1;
            if (local_EA >= 49) {
              local_EA = -1;
              local_D9 = 11;
            }
            break;
          case 5:
            local_F4 = 21 - local_EA / 10 * 1;
            if (local_EA >= 49) {
              local_EA = -1;
              local_D9 = 6;
            }
            break;
          case 4:
            local_F4 = 23 - local_EA / 10 * 1;
            if (local_EA >= 49) {
              local_EA = -1;
              local_D9 = 5;
            }
            break;
          case 3:
            local_F4 = 25 - local_EA / 9 * 1;
            if (local_EA >= 49) {
              local_EA = -1;
              local_D9 = 4;
            }
            break;
          case 2:
            local_F4 = 30 - local_EA / 4 * 1;
            if (local_EA >= 29) {
              local_EA = -1;
              local_D9 = 3;
            }
            break;
          case 1:
            local_F4 = 35 - local_EA / 4 * 1;
            if (local_EA >= 29) {
              local_EA = -1;
              local_D9 = 2;
            }
            break;
          case 0:
            local_F4 = 40 - local_EA / 4 * 1;
            if (local_EA >= 29) {
              local_EA = -1;
              local_D9 = 1;
            }
            break;
        }
        script_145DD(local_DC, Script_FloatUtils(ScriptMathOp_FloatToInt, local_D4), local_F4);
      } else {
        Script_executeAicaCommand(672, 2, 0);
        local_DB = 6;
        local_DD = local_DD - 1;
        if (local_DD >= 0) {
          SCNF_WaitFrames_0000(10);
          script_131E9(0);
          script_13EC6(local_DD);
          SCNF_WaitFrames_0000(60);
          local_DE = Script_FloatUtils(ScriptMathOp_FloatToInt, local_FC - 1.0f);
          Script_executeAicaCommand(928, 2, 0);
        } else {
          break;
        }
      }
    }
    if (local_DB > 0) {
      local_DB = local_DB - 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(10);
  Script_srchControl((void *)local_E4);
  task_200 = task_264;
  task_268 = 2;
  script_13B41();
  task_27C = 2;
  Script_SetSfxOverlayTimeoutTuning(2, SfxMode_SessionAction);
  return Script_srchControl(0);
}

static void script_13B41(void) {
  *(int16_t *)task_280 = 0x100;
  *(int16_t *)((int8_t *)task_280 + 2) = 0x200;
  *(int16_t *)((int8_t *)task_280 + 4) = 0x400;
  *(int16_t *)((int8_t *)task_280 + 6) = 0x800;
  *(int16_t *)((int8_t *)task_280 + 8) = 4;
  *(int16_t *)((int8_t *)task_280 + 10) = 64;
  *(int16_t *)((int8_t *)task_280 + 12) = 16;
  *(int16_t *)((int8_t *)task_280 + 14) = 1;
  return;
}

static void script_13B95(void) {
  *(int16_t *)task_280 = 1;
  *(int16_t *)((int8_t *)task_280 + 2) = 16;
  *(int16_t *)((int8_t *)task_280 + 4) = 64;
  *(int16_t *)((int8_t *)task_280 + 6) = 4;
  *(int16_t *)((int8_t *)task_280 + 8) = 0x100;
  *(int16_t *)((int8_t *)task_280 + 10) = 0x200;
  *(int16_t *)((int8_t *)task_280 + 12) = 0x400;
  *(int16_t *)((int8_t *)task_280 + 14) = 0x800;
  return;
}

static int32_t script_13BE9(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  script_14E30(&local_F4, 0.055f, 1.528f, 0.02f);
  script_14E30(&local_E8, 0.055f, 1.498f, 0.02f);
  for (;;) {
    script_14DE6(-0.032f, 1.528f, task_264);
    script_14DE6(0.055f, 1.58f, task_1FC);
    script_14DE6(0.13f, 1.528f, task_200);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_13C63(void) {
  int32_t local_FC;
  script_13D90(SHEN_NAME('QTEM'), 169, 1);
  script_13D90(SHEN_NAME('QTEM'), 170, 1);
  script_13D90(SHEN_NAME('QTEM'), 176, 1);
  local_FC = task_269;
  switch (local_FC) {
    case 1:
      script_13D90(SHEN_NAME('QTEM'), 173, 0);
      script_13DDB(SHEN_NAME('QTEM'), 171, 4, 15, 1);
      task_269 = 4;
      break;
    case 2:
      script_13D90(SHEN_NAME('QTEM'), 171, 0);
      script_13DDB(SHEN_NAME('QTEM'), 172, 4, 15, 1);
      task_269 = 5;
      break;
    case 3:
      script_13D90(SHEN_NAME('QTEM'), 172, 0);
      script_13DDB(SHEN_NAME('QTEM'), 173, 4, 15, 1);
      task_269 = 9;
      break;
    case 6:
      script_13D90(SHEN_NAME('QTEM'), 173, 0);
      script_13DDB(SHEN_NAME('QTEM'), 182, 4, 15, 1);
      task_269 = 10;
      break;
    case 10:
      break;
  }
  return;
}

static void script_13D90(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Script_charaPosRotUtils(arg0, arg1, &local_F4, 0, 2);
  (&local_F4)->z = 0.0f;
  if (arg2 == 1) {
    (&local_F4)->z = 0.015f;
  } else {
    (&local_F4)->z = 0.0f;
  }
  Script_charaPosRotUtils(arg0, arg1, &local_F4, 0, 0);
  return;
}

static void script_13DDB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_14E97, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static void script_13DF3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, ...) {
  Script_scenCreateTask(script_14F02, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static void script_13E0B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_14F9E, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static void script_13E23(void) {
  int16_t local_FE;
  local_FE = 0;
  while (local_FE < 15) {
    uint8_t unused_FC[2];
    Vector3 local_F0;
    Script_charaPosRotUtils(SHEN_NAME('QTEM'), local_FE + 162, &local_F0, 0, 2);
    (&local_F0)->z = 0.0f;
    Script_charaPosRotUtils(SHEN_NAME('QTEM'), local_FE + 162, &local_F0, 0, 0);
    local_FE = local_FE + 1;
  }
  local_FE = 0;
  while (local_FE < 6) {
    uint8_t unused_FC[2];
    Vector3 local_F0;
    Script_charaPosRotUtils(SHEN_NAME('QTEM'), local_FE + 182, &local_F0, 0, 2);
    (&local_F0)->z = 0.0f;
    Script_charaPosRotUtils(SHEN_NAME('QTEM'), local_FE + 182, &local_F0, 0, 0);
    local_FE = local_FE + 1;
  }
  return;
}

static void script_13EC6(int32_t arg0) {
  script_13D90(SHEN_NAME('QTEM'), 175, 0);
  script_13D90(SHEN_NAME('QTEM'), 174, 0);
  script_13D90(SHEN_NAME('QTEM'), 167, 0);
  if (arg0 == 2) {
    script_13D90(SHEN_NAME('QTEM'), 175, 1);
  }
  if (arg0 >= 1) {
    script_13D90(SHEN_NAME('QTEM'), 174, 1);
  }
  if (arg0 >= 0) {
    script_13D90(SHEN_NAME('QTEM'), 167, 1);
  }
  return;
}

static void script_13F4A(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int16_t local_E6;
  int32_t local_E0;
  local_E0 = arg0;
  switch (local_E0) {
    case 1:
      local_E6 = 187;
      break;
    case 4:
      local_E6 = 187;
      break;
    case 16:
      local_E6 = 187;
      break;
    case 64:
      local_E6 = 187;
      break;
    case 0x100:
      local_E6 = 183;
      break;
    case 0x200:
      local_E6 = 184;
      break;
    case 0x400:
      local_E6 = 185;
      break;
    case 0x800:
      local_E6 = 186;
      break;
  }
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  if (arg1 == 0) {
    script_14E30(&local_F4, 0.0f, 0.0f, 0.0f);
  } else {
    script_14E30(&local_F4, 0.0f, -100.46f, 0.015f);
    switch (arg0) {
      case 4:
        local_E8.x = 0;
        (&local_E8)->y = 0;
        (&local_E8)->z = -0x4000;
        break;
      case 16:
        local_E8.x = 0;
        (&local_E8)->y = 0;
        (&local_E8)->z = 0x8000;
        break;
      case 64:
        local_E8.x = 0;
        (&local_E8)->y = 0;
        (&local_E8)->z = 0x4000;
        break;
    }
  }
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), local_E6, &local_F4, &local_E8, 0);
  return;
}

static int32_t script_140BA(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int16_t local_FA;
  int8_t local_F9;
  int8_t local_F8;
  int32_t local_EC[3];
  int32_t local_E0[3];
  int32_t local_D4[3];
  int32_t local_C8[3];
  local_EC[0] = 0;
  local_E0[0] = 0;
  local_D4[0] = 0;
  local_C8[0] = 0;
  local_EC[1] = 0;
  local_E0[1] = 0;
  local_D4[1] = 0;
  local_C8[1] = 0;
  local_EC[2] = 0;
  local_E0[2] = 0x8000;
  local_D4[2] = -0x4000;
  local_C8[2] = 0x4000;
  local_F9 = 0;
  local_F8 = 3;
  local_FA = 0;
  Script_executeAicaCommand(171, 0, 0);
  for (;;) {
    {
      int32_t local_C4;
      local_C4 = local_F8;
      if ((local_C4 == 0) | (local_C4 == 1) | (local_C4 == 2)) {
        if (local_F8 == 0) {
          Script_executeAicaCommand(171, 0, 0);
        }
        script_13F4A(arg0, 1);
      } else {
        if ((local_C4 == 3) | (local_C4 == 4)) {
          script_13F4A(arg0, 0);
          if (local_F8 == 4) {
            local_F8 = -1;
          }
          goto L0141A7;
        }
      }
    }
    L0141A7:;
    local_FC = Script_GetControllerInput(8);
    if ((local_FC != 0) & (local_FC != 0x800)) {
      int32_t local_C4;
      local_C4 = arg0;
      switch (local_C4) {
        case 1:
          if (local_FC == 0x1000) {
            Script_executeAicaCommand(589995, 0, 0);
            task_278 = 8193;
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            task_278 = 0;
          }
          goto L014434;
          break;
        case 4:
          if (local_FC == 0x8000) {
            Script_executeAicaCommand(589995, 0, 0);
            task_278 = 8196;
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            task_278 = 0;
          }
          goto L014434;
          break;
        case 16:
          if (local_FC == 0x2000) {
            Script_executeAicaCommand(589995, 0, 0);
            task_278 = 8208;
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            task_278 = 0;
          }
          goto L014434;
          break;
        case 64:
          if (local_FC == 0x4000) {
            Script_executeAicaCommand(589995, 0, 0);
            task_278 = 8256;
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            task_278 = 0;
          }
          goto L014434;
          break;
        case 0x100:
          if (local_FC == 0x400) {
            Script_executeAicaCommand(589995, 0, 0);
            task_278 = 0x2100;
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            task_278 = 0;
          }
          goto L014434;
          break;
        case 0x200:
          if (local_FC == 0x200) {
            Script_executeAicaCommand(589995, 0, 0);
            task_278 = 0x2200;
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            task_278 = 0;
          }
          goto L014434;
          break;
        case 0x400:
          if (local_FC == 4) {
            Script_executeAicaCommand(589995, 0, 0);
            task_278 = 0x2400;
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            task_278 = 0;
          }
          goto L014434;
          break;
        case 0x800:
          if (local_FC == 2) {
            Script_executeAicaCommand(589995, 0, 0);
            task_278 = 0x2800;
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            task_278 = 0;
          }
          goto L014434;
          break;
      }
    }
    if (local_F9 >= arg1) {
      Script_executeAicaCommand(655531, 0, 0);
      task_278 = 0xA000;
      break;
    }
    local_F9 = local_F9 + 1;
    local_F8 = local_F8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L014434:;
  script_13F4A(arg0, 0);
  return Script_srchControl(0);
}

static float script_14446(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  local_F8 = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, task_26C / 10);
  if (arg0 == 0) {
    local_FC = 0.0f;
  } else {
    local_FC = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, arg0 + 2) * local_F8 / Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, arg1);
  }
  if (!(local_FC < (float)100)) {
    local_FC = 100.0f;
  }
  return local_FC;
}

static int8_t script_1449F(int16_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  task_278 = -1;
  local_FF = arg0;
  for (;;) {
    if (task_278 != -1) {
      break;
    }
    if (local_FF > 0) {
      local_FF = local_FF - 1;
    }
    local_F8 = task_269;
    if ((local_F8 == 1) | (local_F8 == 4)) {
      script_14577(177, local_FF, arg0);
    } else if ((local_F8 == 2) | (local_F8 == 5)) {
      script_14577(180, local_FF, arg0);
    } else if ((local_F8 == 3) | (local_F8 == 9)) {
      script_14577(179, local_FF, arg0);
    } else {
      if ((local_F8 == 6) | (local_F8 == 10)) {
        script_14577(178, local_FF, arg0);
        goto L014554;
      }
    }
    L014554:;
    SCNF_WaitFrames_0000(1);
    Script_GameUtils(SHEN_NAME('QTEM'), GC_CommitStagedEntries, 0, 0, 0, 0);
  }
  return local_FF;
}

static void script_14577(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  script_14E30(&local_F4, -0.13f, 1.38f, 0.015f);
  arg1 = arg1 * 30 / arg2;
  for (;;) {
    if (arg1 > 0) {
      arg1 = arg1 - 1;
    } else {
      break;
    }
    Script_GameUtils(SHEN_NAME('QTEM'), GC_StageNodeData, arg0, &local_F4, 1.0f, 0);
    local_F4.x = (&local_F4)->x + 0.007f;
  }
  return;
}

static void script_145DD(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  float local_F0;
  int32_t unused_EC;
  int32_t local_E8;
  script_13C63();
  local_F0 = (float)(arg0 * 30 / arg2) * 0.007f + -0.11f;
  script_14E30(&local_F4, local_F0, 1.38f, 0.015f);
  Script_GameUtils(SHEN_NAME('QTEM'), GC_StageNodeData, 181, &local_F4, 1.0f, &data_20074);
  if (arg1 == 100) {
    script_14E30(&local_F4, local_F0 + 0.055f, 1.38f, 0.015f);
  } else if (arg1 >= 10) {
    script_14E30(&local_F4, local_F0 + 0.04f, 1.38f, 0.015f);
  } else {
    script_14E30(&local_F4, local_F0 + 0.025f, 1.38f, 0.015f);
  }
  Script_GameUtils(SHEN_NAME('QTEM'), GC_ConfigureNumericDisplay, 0x100, &data_2005C, 100, 0);
  Script_GameUtils(SHEN_NAME('QTEM'), GC_StageNumericDisplayValue, arg1, &local_F4, 1.0f, &data_20068);
  local_E8 = task_269;
  if ((local_E8 == 1) | (local_E8 == 4)) {
    script_14577(177, arg0, arg2);
  } else if ((local_E8 == 2) | (local_E8 == 5)) {
    script_14577(180, arg0, arg2);
  } else if ((local_E8 == 3) | (local_E8 == 9)) {
    script_14577(179, arg0, arg2);
  } else {
    if ((local_E8 == 6) | (local_E8 == 10)) {
      script_14577(178, arg0, arg2);
      goto L01474F;
    }
  }
  L01474F:;
  return;
}

static void script_14750(void) {
  int8_t local_FF;
  local_FF = 0;
  Script_GameUtils(SHEN_NAME('QTEM'), GC_CommitStagedEntries, 0, 0, 0, 0);
  script_13E23();
  Script_executeAicaCommand(0x1200A0, 2, 0);
  script_13D90(SHEN_NAME('QTEM'), 166, 1);
  if ((task_264 >= task_1FC) & (task_1FC > 0)) {
    script_13DDB(SHEN_NAME('QTEM'), 170, 5, 15, 0);
    script_13DF3(0x3D6147AE, 0x3FCA3D71, task_1FC, 5, 15);
    task_1FC = task_264;
    while (local_FF < 105) {
      local_FF = local_FF + 1;
      SCNF_WaitFrames_0000(1);
    }
  } else {
    script_13DDB(SHEN_NAME('QTEM'), 169, 5, 15, 0);
    script_13DF3(-0x42FCED91, 0x3FC39581, task_264, 5, 15);
    while (local_FF < 105) {
      local_FF = local_FF + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static int32_t script_14841(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FC = 0;
  local_F8 = 0;
  local_F4 = 0;
  local_F4 = Script_PlayerStatsUtils(PS_GetDartsModePlayCountSlot, 9);
  Script_PlayerStatsUtils(PS_SetDartsModePlayCountSlot, 9, local_F4 + 1);
  for (;;) {
    local_F0 = task_27C;
    switch (local_F0) {
      case 1:
        local_FC = local_FC + 1;
        break;
      case 2:
        goto L0148A7;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0148A7:;
  local_F8 = Script_PlayerStatsUtils(PS_GetDartsModeActiveFramesSlot, 9);
  Script_PlayerStatsUtils(PS_SetDartsModeActiveFramesSlot, 9, local_F8 + local_FC);
  return Script_srchControl(0);
}

static void script_148CA(int8_t arg0, const char *arg1) {
  task_228 = Script_ShowUIMessageFromID(arg1);
  if (arg0 == 1) {
    script_14995((const void *)task_260);
  }
  return;
}

static void script_148EC(int32_t arg0, int8_t arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6) {
  int8_t local_FF;
  local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, arg0)));
  switch (local_FF) {
    case 0:
      task_260 = Script_ShowUIMessageFromID(arg2);
      break;
    case 1:
      task_260 = Script_ShowUIMessageFromID(arg3);
      break;
    case 2:
      task_260 = Script_ShowUIMessageFromID(arg4);
      break;
    case 3:
      task_260 = Script_ShowUIMessageFromID(arg5);
      break;
    case 4:
      task_260 = Script_ShowUIMessageFromID(arg6);
      break;
  }
  if (arg1 == 1) {
    script_14995((const void *)task_260);
  }
  return;
}

static void script_14995(const void *arg0) {
  SCNF_WaitFrames_0000(1);
  while (Script_convGetIsActive(arg0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_149B5(void) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  local_FF = 0;
  local_FE = 1;
  while (local_FF < 70) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 1:
        if (Script_GetControllerInput(1) == 2) {
          local_FE = 2;
        }
        break;
      case 2:
        if (Script_GetControllerInput(1) == 2) {
          local_FE = 3;
        }
        break;
      case 3:
        if (Script_GetControllerInput(1) == 0x400) {
          local_FE = 4;
        }
        break;
      case 4:
        if (Script_GetControllerInput(1) == 0x400) {
          local_FE = 5;
        }
        break;
      case 5:
        if (Script_GetControllerInput(1) == 4) {
          local_FE = 6;
        }
        break;
      case 6:
        if (Script_GetControllerInput(1) == 0x200) {
          local_FE = 7;
        }
        break;
      case 7:
        if (Script_GetControllerInput(1) == 4) {
          local_FE = 8;
        }
        break;
      case 8:
        if (Script_GetControllerInput(1) == 0x200) {
          local_FE = 9;
        }
        break;
      case 9:
        if (Script_GetControllerInput(1) == 0x400) {
          local_FE = 10;
        }
        break;
      case 10:
        if (Script_GetControllerInput(1) == 0x200) {
          (void)((task_26A = -1) & 1);
          goto L014B33;
        }
        break;
    }
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
  }
  L014B33:;
  return Script_srchControl(0);
}

static int32_t script_14B3A(void) {
  int16_t local_FE;
  int32_t local_F8;
  int8_t local_F7;
  float local_F0;
  float local_EC;
  int32_t local_E8;
  int8_t local_E7;
  int8_t local_E6;
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  task_269 = 6;
  task_26C = 1000;
  local_E7 = 2;
  local_EC = 0.0f;
  local_E8 = 0;
  local_F8 = Script_scenCreateTask(script_13BE9, 0);
  script_13EC6(local_E7);
  script_13C63();
  script_13B41();
  for (;;) {
    local_FE = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 11.0f)) + 12;
    local_F7 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 8.0f));
    Script_scenCreateTask(script_140BA, 2, *(int16_t *)((int8_t *)task_280 + (local_F7 << 1)), local_FE);
    local_E6 = script_1449F(local_FE);
    if (task_278 != 0xFFFF) {
      if (-(task_278 != 0xA000) & task_278 & *(int16_t *)((int8_t *)task_280 + (local_F7 << 1)) & -(task_278 > 0x2000)) {
        local_F0 = script_14446(local_E6, local_FE);
        local_EC = local_EC + local_F0;
        if (local_E8 < 0x5F5DD18) {
          local_E8 = local_E8 + task_26C;
        } else {
          if (local_E8 >= 0x5F5DD18) {
            local_E8 = 0x5F5DD18;
            goto L014C5A;
          }
        }
        L014C5A:;
        if (task_264 < 0x5F5DD18) {
          task_264 = local_E8 + Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC);
        }
        if (task_264 >= task_1FC) {
          task_1FC = task_264;
        }
        script_145DD(local_E6, Script_FloatUtils(ScriptMathOp_FloatToInt, local_F0), local_FE);
      } else {
        Script_executeAicaCommand(672, 2, 0);
        local_E7 = local_E7 - 1;
        if (local_E7 >= 0) {
          SCNF_WaitFrames_0000(10);
          script_131E9(0);
          script_13EC6(local_E7);
          Script_executeAicaCommand(928, 2, 0);
          SCNF_WaitFrames_0000(60);
        } else {
          break;
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetSfxOverlayTimeoutTuning(2, SfxMode_SessionAction);
  task_200 = task_264;
  Script_srchControl((void *)local_F8);
  task_268 = 2;
  return Script_srchControl(0);
}

static void script_14D22(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_PlayCharaMotion(CHARA_AKIR, arg1, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (arg2 == 1) {
    script_14D58(arg0);
  }
  return;
}

static void script_14D58(int32_t arg0) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_14D72(int32_t arg0, int32_t arg1, int32_t arg2) {
  SCNF_WaitFrames_0000(arg2);
  Script_seqEnterEvtCam(arg0, arg1, arg1);
  return;
}

static int32_t script_14D87(void) {
  int32_t local_FC;
  local_FC = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(-100, 1);
  local_FC = local_FC - 100;
  Script_SetStats(STATS_CASH, local_FC);
  return Script_srchControl(0);
}

static void script_14DB6(void) {
  if (task_26A == -1) {
    Script_scenCreateTask(script_14B3A, 0);
    (void)((task_26A = 0) & 0x10);
  } else {
    Script_scenCreateTask(script_1355B, 0);
  }
  return;
}

static void script_14DE6(float arg0, float arg1, int32_t arg2) {
  Vector3 local_F4;
  Script_GameUtils(SHEN_NAME('QTEM'), GC_ConfigureNumericDisplay, 0x100, &data_20080, 0x5F5E0FF, 0);
  script_14E30(&local_F4, arg0, arg1, 0.015f);
  Script_GameUtils(SHEN_NAME('QTEM'), GC_StageNumericDisplayValue, arg2, &local_F4, 1.0f, &data_2008C);
  return;
}

static void script_14E30(Vector3 *arg0, float arg1, float arg2, float arg3) {
  arg0->x = arg1;
  arg0->y = arg2;
  arg0->z = arg3;
  return;
}

static void script_14E4B(Vector3 *arg0, float arg1, float arg2, float arg3) {
  arg0->x = arg0->x + arg1;
  arg0->y = arg0->y + arg2;
  arg0->z = arg0->z + arg3;
  return;
}

static int32_t script_14E78(void) {
  Script_shenCashUtils(1);
  SCNF_WaitFrames_0000(45);
  Script_shenCashUtils(0);
  return Script_srchControl(0);
}

static int32_t script_14E97(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int16_t local_FE;
  local_FE = 0;
  for (;;) {
    script_13D90(arg0, arg1, 1);
    SCNF_WaitFrames_0000(arg3);
    local_FE = local_FE + 1;
    if ((local_FE >= arg2) & (arg4 == 1)) {
      break;
    }
    script_13D90(arg0, arg1, 0);
    SCNF_WaitFrames_0000(arg3);
    if (local_FE >= arg2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_14F02(float arg0, float arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int16_t local_FE;
  int16_t local_FC;
  local_FE = 0;
  local_FC = 0;
  for (;;) {
    if (local_FC < arg4) {
      script_14DE6(arg0, arg1, arg2);
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
      continue;
    }
    local_FC = 0;
    while (local_FC < arg4) {
      Script_GameUtils(SHEN_NAME('QTEM'), GC_CommitStagedEntries, 0, 0, 0, 0);
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
    }
    local_FE = local_FE + 1;
    if (local_FE >= arg3) {
      break;
    }
    SCNF_WaitFrames_0000(1);
    local_FC = 0;
  }
  return Script_srchControl(0);
}

static int32_t script_14F9E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int16_t local_FE;
  local_FE = 0;
  for (;;) {
    script_13D90(arg0, arg1, 1);
    SCNF_WaitFrames_0000(arg4);
    script_13D90(arg0, arg2, 1);
    script_13D90(arg0, arg1, 0);
    SCNF_WaitFrames_0000(arg4);
    script_13D90(arg0, arg2, 0);
    local_FE = local_FE + 1;
    if (local_FE >= arg3) {
      break;
    }
  }
  SCNF_WaitFrames_0000(arg4);
  return Script_srchControl(0);
}

static void script_15011(int32_t arg0) {
  int32_t unused_F8[2];
  int32_t local_F4;
  int32_t local_EC[2];
  int32_t local_E4[2];
  int32_t unused_E0;
  local_EC[0] = 0x3FA66666;
  local_EC[1] = *(int32_t *)local_EC;
  local_E4[0] = 0x3F07AE14;
  local_E4[1] = *(int32_t *)local_E4;
  script_14E30((Vector3 *)&local_F4, 0.0f, 1.44f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 166, (Vector3 *)&local_F4, 0, 0);
  script_14E30((Vector3 *)&local_F4, -0.083418f, 1.55f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 169, (Vector3 *)&local_F4, 0, 0);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 169, (Vector3 *)local_EC, 0, 3);
  script_14E30((Vector3 *)&local_F4, 0.004f, 1.602f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 170, (Vector3 *)&local_F4, 0, 0);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 170, (Vector3 *)local_EC, 0, 3);
  script_14E30((Vector3 *)&local_F4, 0.083029f, 1.55f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 176, (Vector3 *)&local_F4, 0, 0);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 176, (Vector3 *)local_EC, 0, 3);
  script_14E30((Vector3 *)&local_F4, 0.0f, 1.335f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 171, (Vector3 *)&local_F4, 0, 0);
  script_14E30((Vector3 *)&local_F4, 0.0f, 1.335f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 172, (Vector3 *)&local_F4, 0, 0);
  script_14E30((Vector3 *)&local_F4, 0.0f, 1.335f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 173, (Vector3 *)&local_F4, 0, 0);
  script_14E30((Vector3 *)&local_F4, 0.0f, 1.335f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 182, (Vector3 *)&local_F4, 0, 0);
  script_14E30((Vector3 *)&local_F4, 0.0f, 0.0f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 183, (Vector3 *)&local_F4, 0, 0);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 183, (Vector3 *)local_E4, 0, 3);
  script_14E30((Vector3 *)&local_F4, 0.0f, 0.0f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 184, (Vector3 *)&local_F4, 0, 0);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 184, (Vector3 *)local_E4, 0, 3);
  script_14E30((Vector3 *)&local_F4, 0.0f, 0.0f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 185, (Vector3 *)&local_F4, 0, 0);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 185, (Vector3 *)local_E4, 0, 3);
  script_14E30((Vector3 *)&local_F4, 0.0f, 0.0f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 186, (Vector3 *)&local_F4, 0, 0);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 186, (Vector3 *)local_E4, 0, 3);
  script_14E30((Vector3 *)&local_F4, 0.0f, 0.0f, 0.0f);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 187, (Vector3 *)&local_F4, 0, 0);
  Script_charaPosRotUtils(SHEN_NAME('QTEM'), 187, (Vector3 *)local_E4, 0, 3);
  return;
}

__attribute__((scn_string_bindings(0, data_200D8)))
static void script_152AD(void) {
  int32_t unused_F4[3];
  task_1F8 = 4;
  (void)((task_26A = 0) & 8);
  script_EE07();
  Script_IncrementQTEMinigameRand16(0, 7);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_200C8);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 4);
  Script_LoadVoiceFile(1, "E1026");
  Script_SetCameraFlags(0);
  script_15011(1);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_uiUtils(0, 4, 0);
  Script_charaDisplay(SHEN_NAME('YENS'), CDM_Hide);
  return;
}

__attribute__((scn_string_bindings(0, data_200F8, 2, data_20116, 3, data_20123)))
static void script_15335(void) {
  int32_t unused_E8[6];
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_200E0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_200EC, 0);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_LoadVoiceFile(0, "E1026");
  Script_SetCameraFlags(1);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  script_13455();
  if (task_200 > Script_GetHighScoreValue(Highscore_Unknown0)) {
    Script_SetHighScoreValue(Highscore_Unknown0, task_200);
  }
  Script_SetHighScoreValue(Highscore_Unknown1, task_200);
  Script_scenToggleVisibleFor(SHEN_NAME('QTEM'), 0);
  Script_sprintf("--------EX_QTE--------\012");
  Script_sprintf("HI_SCORE\011%d\012", Script_GetHighScoreValue(Highscore_Unknown0));
  Script_sprintf("----------------------\012");
  script_EE43();
  Script_ryoControl(RYO_RestoreControl);
  task_1F8 = 0;
  return;
}

static void script_1542F(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_14D72(7300, SHEN_NAME('TITL'), 0);
  script_15B3E();
  script_194ED();
  SCNF_WaitFrames_0000(1);
  task_2C0 = 1;
  script_1546C();
  script_19574();
  return;
}

static void script_1546C(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_2C0;
    switch (local_FC) {
      case 1:
        script_15BB8();
        break;
      case 2:
        script_15BF2();
        break;
      case 6:
        script_15CA4();
        break;
      case 4:
        script_15CB1();
        break;
      case 5:
        script_15CC6();
        break;
      case 8:
        script_15F27();
        break;
      case 9:
        script_15FB6();
        break;
      case 10:
        script_18207(task_294);
        break;
      case 11:
        script_1872D();
        break;
      case 12:
        script_187F2();
        break;
      case 13:
        script_188AA();
        break;
      case 14:
        script_188D5();
        break;
      case 15:
        script_18660();
        break;
      case 16:
        goto L015585;
        break;
      case 18:
        script_129B8();
        goto L015585;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L015585:;
  return;
}

static void script_15586(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  (void)((task_2C6 = 0) & 0x40);
  script_15A8D();
  Script_SetMotnFlag(CHARA_AKIR, -1);
  local_F0 = arg0;
  switch (local_F0) {
    case 1:
      script_14E30(&local_F4, 0.14f, 0.0f, 1.36f);
      script_18900(SHEN_NAME('TITL'), CHARA_AKIR, &local_F4, 0);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_IMMEDIATE);
      if (arg1 == 2) {
        Script_PlayCharaMotion(CHARA_AKIR, 31000, 8.0f, 22.0f, 8.0f, 1.25f, 0);
      } else {
        Script_PlayCharaMotion(CHARA_AKIR, 31000, 0.0f, 22.0f, 0.0f, 1.25f, 0);
      }
      Script_executeAicaCommand(787881, 0, 0);
      switch (arg1) {
        case 2:
          if (task_2C5 == -1) {
            (void)((task_2C6 = -1) & 0x40);
            script_15A17(1, 1, SHEN_NAME('MIT3'), 0);
          }
          break;
        case 0:
          if (task_2C5 == -1) {
            (void)((task_2C6 = -1) & 0x40);
            script_15A17(1, 8, SHEN_NAME('MIT3'), 0);
          }
          break;
      }
      break;
    case 3:
      script_14E30(&local_F4, -0.034f, 0.0f, 1.32f);
      script_18900(SHEN_NAME('TITL'), CHARA_AKIR, &local_F4, 0);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_IMMEDIATE);
      if (arg1 == 2) {
        Script_PlayCharaMotion(CHARA_AKIR, 30999, 8.0f, 22.0f, 8.0f, 1.5f, 0);
      } else {
        Script_PlayCharaMotion(CHARA_AKIR, 30999, 0.0f, 22.0f, 0.0f, 1.5f, 0);
      }
      Script_executeAicaCommand(787881, 0, 0);
      switch (arg1) {
        case 2:
          if (task_2C4 == -1) {
            (void)((task_2C6 = -1) & 0x40);
            script_15A17(3, 1, SHEN_NAME('MIT2'), 1);
          }
          break;
        case 0:
          if (task_2C4 == -1) {
            (void)((task_2C6 = -1) & 0x40);
            script_15A17(3, 7, SHEN_NAME('MIT2'), 1);
          }
          break;
      }
      break;
    case 4:
      script_14E30(&local_F4, -0.1f, 0.0f, 1.36f);
      script_18900(SHEN_NAME('TITL'), CHARA_AKIR, &local_F4, 0);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_IMMEDIATE);
      if (arg1 == 2) {
        Script_PlayCharaMotion(CHARA_AKIR, 31000, 8.0f, 22.0f, 8.0f, 1.25f, 0);
      } else {
        Script_PlayCharaMotion(CHARA_AKIR, 31000, 0.0f, 22.0f, 0.0f, 1.25f, 0);
      }
      Script_executeAicaCommand(787881, 0, 0);
      switch (arg1) {
        case 2:
          if (task_2C4 == -1) {
            (void)((task_2C6 = -1) & 0x40);
            script_15A17(4, 1, SHEN_NAME('MIT2'), 1);
          }
          break;
        case 0:
          if (task_2C4 == -1) {
            (void)((task_2C6 = -1) & 0x40);
            script_15A17(4, 8, SHEN_NAME('MIT2'), 1);
          }
          break;
      }
      break;
    case 5:
      script_14E30(&local_F4, -0.3f, 0.0f, 1.32f);
      script_18900(SHEN_NAME('TITL'), CHARA_AKIR, &local_F4, 0);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_IMMEDIATE);
      if (arg1 == 2) {
        Script_PlayCharaMotion(CHARA_AKIR, 30999, 8.0f, 22.0f, 8.0f, 1.5f, 0);
      } else {
        Script_PlayCharaMotion(CHARA_AKIR, 30999, 0.0f, 22.0f, 0.0f, 1.5f, 0);
      }
      Script_executeAicaCommand(787881, 0, 0);
      switch (arg1) {
        case 2:
          if (task_2C3 == -1) {
            (void)((task_2C6 = -1) & 0x40);
            script_15A17(5, 1, SHEN_NAME('MIT1'), 2);
          }
          break;
        case 0:
          if (task_2C3 == -1) {
            (void)((task_2C6 = -1) & 0x40);
            script_15A17(5, 7, SHEN_NAME('MIT1'), 2);
          }
          break;
      }
      break;
  }
  if (arg1 == 4) {
    (void)((task_2C6 = -1) & 0x40);
    Script_scenCreateTask(script_190F6, 1, 7);
    script_14D58(CHARA_AKIR);
  }
  if (-(task_2C3 == 0) & -(task_2C4 == 0) & -(task_2C5 == 0)) {
    if (task_2C6 == -1) {
      script_14D58(CHARA_AKIR);
    }
  } else {
    if (task_2C6 == -1) {
      SCNF_WaitFrames_0000(6);
    }
  }
  (void)((task_2C6 = 0) & 0x40);
  return;
}

static void script_15A17(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  Script_scenCreateTask(script_190F6, 1, arg0);
  SCNF_WaitFrames_0000(arg1);
  script_18CB8(arg2);
  local_FC = arg3;
  switch (local_FC) {
    case 2:
      (void)((task_2C3 = 0) & 0x40);
      break;
    case 1:
      (void)((task_2C4 = 0) & 0x40);
      break;
    case 0:
      (void)((task_2C5 = 0) & 0x40);
      break;
  }
  Script_executeAicaCommand(853417, 0, 0);
  return;
}

static void script_15A8D(void) {
  int32_t local_FC;
  int8_t local_FB;
  int32_t local_F4;
  local_FC = task_294 / 1000;
  local_FB = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
  if (local_FC == 98) {
    Script_executeAicaCommand(489387, 0, 0);
  } else if (local_FC - local_FC / 10 * 10 == 9) {
    Script_executeAicaCommand(423851, 0, 0);
  } else {
    local_F4 = local_FB;
    switch (local_F4) {
      case 0:
        Script_executeAicaCommand(227243, 0, 0);
        break;
      case 1:
        Script_executeAicaCommand(292779, 0, 0);
        break;
      case 2:
        Script_executeAicaCommand(620459, 0, 0);
        break;
    }
  }
  return;
}

static void script_15B3E(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  script_14E30(&local_F4, 0.073f, 0.0f, 1.532f);
  script_190B1(&local_E8, 0.0f, 0.0f, 0.0f);
  script_18900(SHEN_NAME('TITL'), CHARA_AKIR, &local_F4, (int32_t)&local_E8);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  return;
}

__attribute__((scn_string_bindings(0, data_20314)))
static void script_15BB8(void) {
  script_148EC(2, 1, "E1027A001", "E1027A002", 0, 0, 0);
  SCNF_WaitFrames_0000(20);
  script_1867B();
  SCNF_WaitFrames_0000(50);
  task_2C0 = 2;
  return;
}

__attribute__((scn_string_bindings(0, data_20328, 1, data_20332, 2, data_2033C)))
static void script_15BF2(void) {
  int32_t local_FC;
  int8_t local_FB;
  int32_t local_F4;
  (void)(local_FC == 34);
  local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 36, 47, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  if (local_FC == 36) {
    local_FB = script_008F(0, 100, 1);
    local_F4 = local_FB;
    if (local_F4 == 0) {
      task_2C0 = 6;
    } else if (local_F4 == 1) {
      script_15EF4();
      task_2C0 = 4;
    } else {
      task_2C0 = 4;
    }
  } else {
    script_148EC(3, 1, "E1027A009", "E1027A010", "E1027A011", 0, 0);
    Script_executeAicaCommand(4512, 4, -10);
    task_2C0 = 4;
  }
  return;
}

static void script_15CA4(void) {
  script_12993();
  script_15CDB();
  return;
}

static void script_15CB1(void) {
  Script_executeAicaCommand(4512, 4, 10);
  task_2C0 = 16;
  return;
}

static void script_15CC6(void) {
  Script_executeAicaCommand(4512, 4, 10);
  task_2C0 = 16;
  return;
}

__attribute__((scn_string_bindings(0, data_20360, 1, data_2036A, 2, data_20374)))
static void script_15CDB(void) {
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  script_14D72(7344, SHEN_NAME('TITL'), 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 773, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 0, 773, 0, 0, 0, 0, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  script_14D22(CHARA_AKIR, 30979, 0);
  SCNF_WaitFrames_0000(10);
  script_148EC(3, 0, "E1027A003", "E1027A004", "E1027A005", 0, 0);
  Script_scenCreateTask(script_14D87, 0);
  Script_fixoAttachEx(SHEN_NAME('YENS'), CHARA_AKIR, 18, &data_20348, &data_20354);
  for (;;) {
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)60}).i) {
      Script_executeAicaCommand(66985, 0, 0);
      Script_charaDisplay(SHEN_NAME('YENS'), CDM_Show);
    } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)65}).i) {
      Script_SetNewHandMotion(CHARA_AKIR, 1, 771, 0, 0, 0, 0, 1.0f, 0);
    } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)169}).i) {
      Script_fixoClearAttachment(SHEN_NAME('YENS'));
      Script_charaDisplay(SHEN_NAME('YENS'), CDM_Hide);
    } else {
      if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)180}).i) {
        Script_executeAicaCommand(1449, 0, 0);
        break;
        goto L015E69;
      }
    }
    L015E69:;
    SCNF_WaitFrames_0000(1);
  }
  script_14D58(CHARA_AKIR);
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 2, "bgm008.snd", 0x4000);
  script_14D72(7345, SHEN_NAME('TITL'), 0);
  SCNF_WaitFrames_0000(10);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  script_16161();
  SCNF_WaitFrames_0000(80);
  task_2C0 = 8;
  return;
}

__attribute__((scn_string_bindings(0, data_20389, 1, data_20393, 2, data_2039D)))
static void script_15EF4(void) {
  Script_scenCreateTask(script_14E78, 0);
  script_148EC(3, 1, "E1027A006", "E1027A007", "E1027A008", 0, 0);
  task_2C0 = 16;
  return;
}

static void script_15F27(void) {
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_20230, 8);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2014C, 8);
  script_EEE4(6, (int32_t)"qtebat.snd");
  script_14D72(7342, SHEN_NAME('TITL'), 0);
  script_14D22(CHARA_AKIR, 30989, 0);
  script_14D72(7343, SHEN_NAME('TITL'), 40);
  script_14D58(CHARA_AKIR);
  SCNF_WaitFrames_0000(30);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  task_2C0 = 9;
  return;
}

static void script_15FB6(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  script_161D1();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_15FFB(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3i local_AC;
  Vector3i local_A0;
  script_18D94(task_294, task_204, task_208, task_20C, 0);
  script_14E30(&local_F4, -0.275f, 1.085f, 0.6656f);
  script_14E30(&local_E8, 0.0f, 1.085f, 0.6656f);
  script_14E30(&local_DC, 0.275f, 1.085f, 0.6656f);
  script_14E30(&local_D0, -0.275f, 1.9311f, 0.3872f);
  script_14E30(&local_C4, 0.0f, 1.9311f, 0.3872f);
  script_14E30(&local_B8, 0.275f, 1.9311f, 0.3872f);
  script_190B1(&local_AC, -45.0f, 0.0f, 0.0f, 0);
  script_190B1(&local_A0, 0.0f, 0.0f, 0.0f, 0);
  script_18900(SHEN_NAME('TITL'), SHEN_NAME('MIT1'), &local_F4, (int32_t)&local_AC);
  script_18900(SHEN_NAME('TITL'), SHEN_NAME('MIT2'), &local_E8, (int32_t)&local_AC);
  script_18900(SHEN_NAME('TITL'), SHEN_NAME('MIT3'), &local_DC, (int32_t)&local_AC);
  script_18900(SHEN_NAME('TITL'), SHEN_NAME('QTL1'), &local_D0, (int32_t)&local_A0);
  script_18900(SHEN_NAME('TITL'), SHEN_NAME('QTL2'), &local_C4, (int32_t)&local_A0);
  script_18900(SHEN_NAME('TITL'), SHEN_NAME('QTL3'), &local_B8, (int32_t)&local_A0);
  Script_charaDisplay(SHEN_NAME('QTL1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('QTL2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('QTL3'), CDM_Hide);
  return;
}

static void script_16161(void) {
  SCNF_WaitFrames_0000(30);
  task_294 = 0;
  task_298 = 0;
  task_2C8 = 0.0f;
  task_2BC = 1000;
  task_2C2 = 0;
  script_18D94(task_294, task_204, task_208, task_20C, 0);
  Script_charaDisplay(SHEN_NAME('QTL1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('QTL2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('QTL3'), CDM_Show);
  Script_scenCreateTask(script_18FA4, 0);
  return;
}

static void script_161D1(void) {
  int32_t local_FC;
  float local_F8;
  int32_t unused_EC[3];
  float local_E0[3];
  int16_t local_DE;
  int16_t local_DC;
  int16_t local_DA;
  int8_t local_D9;
  int8_t local_D8;
  int8_t local_D7;
  int8_t local_D6;
  int8_t local_D5;
  int8_t local_CF[6];
  int8_t local_C9[6];
  int8_t local_C3[6];
  int8_t local_C1[2];
  int8_t local_C0;
  int8_t local_BF;
  uint8_t unused_BC[3];
  int8_t local_BB;
  int8_t local_BA;
  int8_t local_B9;
  int8_t local_B8;
  int8_t local_B7;
  uint8_t unused_B6;
  int8_t local_B5;
  int8_t local_B4;
  Vector3 local_A8;
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  local_F8 = 3.0f;
  local_E0[0] = 3.0f;
  local_E0[1] = 2.0f;
  local_E0[2] = 2.0f;
  local_DE = 31;
  local_DC = 1;
  local_C1[0] = 0;
  local_C1[1] = 0;
  local_DA = 0;
  local_D9 = 0;
  local_D8 = 0;
  local_D7 = 0;
  (void)((task_2C3 = 0) & 1);
  (void)((task_2C4 = 0) & 1);
  (void)((task_2C5 = 0) & 1);
  local_BF = 31;
  local_C0 = 3;
  local_B8 = 0;
  local_FC = 0;
  local_B7 = 0;
  local_B5 = 0;
  local_B4 = 0;
  task_2C2 = 1;
  local_BB = 0;
  for (;;) {
    local_C9[local_BB] = 0;
    local_BB = local_BB + 1;
    if (local_BB == 6) {
      break;
    }
  }
  Script_executeAicaCommand(139176, 0, 0);
  for (;;) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
    script_14D72(7340, SHEN_NAME('TITL'), 0);
    if (local_B8 == 1) {
      SCNF_WaitFrames_0000(31);
      local_B8 = 0;
    }
    local_BB = 0;
    if ((local_C1[local_BB] == 0) | (local_C1[local_BB] == 3)) {
      local_D5 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_IncrementQTEMinigameRand16(1, *local_E0));
      switch (local_D5) {
        case 0:
          local_C3[local_BB * 3] = 1;
          local_C3[local_BB * 3 + 1] = 5;
          local_C3[local_BB * 3 + 2] = 3;
          break;
        case 1:
          local_C3[local_BB * 3] = 3;
          local_C3[local_BB * 3 + 1] = 5;
          local_C3[local_BB * 3 + 2] = 1;
          break;
        case 2:
          local_C3[local_BB * 3] = 5;
          local_C3[local_BB * 3 + 1] = 4;
          local_C3[local_BB * 3 + 2] = 1;
          break;
      }
    } else if ((local_C1[local_BB] == 1) | (local_C1[local_BB] == 2)) {
      local_D5 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_IncrementQTEMinigameRand16(1, local_E0[1]));
      switch (local_D5) {
        case 0:
          local_C3[local_BB * 3] = 3;
          local_C3[local_BB * 3 + 1] = 1;
          local_C3[local_BB * 3 + 2] = 5;
          break;
        case 1:
          local_C3[local_BB * 3] = 5;
          local_C3[local_BB * 3 + 1] = 4;
          local_C3[local_BB * 3 + 2] = 1;
          break;
        case 2:
          local_C3[local_BB * 3] = 5;
          local_C3[local_BB * 3 + 1] = 1;
          local_C3[local_BB * 3 + 2] = 4;
          break;
        case 3:
          local_C3[local_BB * 3] = 1;
          local_C3[local_BB * 3 + 1] = 3;
          local_C3[local_BB * 3 + 2] = 5;
          break;
        case 4:
          local_C3[local_BB * 3] = 4;
          local_C3[local_BB * 3 + 1] = 5;
          local_C3[local_BB * 3 + 2] = 1;
          break;
        case 5:
          local_C3[local_BB * 3] = 1;
          local_C3[local_BB * 3 + 1] = 5;
          local_C3[local_BB * 3 + 2] = 3;
          break;
      }
    } else {
      if (local_C1[local_BB] == 4) {
        local_D5 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_IncrementQTEMinigameRand16(1, local_E0[2]));
        switch (local_D5) {
          case 0:
            local_C3[local_BB * 3] = 5;
            local_C3[local_BB * 3 + 1] = 3;
            local_C3[local_BB * 3 + 2] = 1;
            break;
          case 1:
            local_C3[local_BB * 3] = 1;
            local_C3[local_BB * 3 + 1] = 3;
            local_C3[local_BB * 3 + 2] = 5;
            break;
          case 2:
            local_C3[local_BB * 3] = 5;
            local_C3[local_BB * 3 + 1] = 1;
            local_C3[local_BB * 3 + 2] = 3;
            break;
          case 3:
            local_C3[local_BB * 3] = 1;
            local_C3[local_BB * 3 + 1] = 5;
            local_C3[local_BB * 3 + 2] = 4;
            break;
          case 4:
            local_C3[local_BB * 3] = 4;
            local_C3[local_BB * 3 + 1] = 5;
            local_C3[local_BB * 3 + 2] = 1;
            break;
          case 5:
            local_C3[local_BB * 3] = 3;
            local_C3[local_BB * 3 + 1] = 1;
            local_C3[local_BB * 3 + 2] = 5;
            break;
        }
        goto L0166AB;
      }
    }
    L0166AB:;
    script_17E21(local_C3[0], local_C3[1], local_C3[2], local_C1[0], local_BF);
    if (local_BF < 10) {
      SCNF_WaitFrames_0000(local_BF);
    } else {
      SCNF_WaitFrames_0000(10);
    }
    local_BB = 0;
    local_B9 = 0;
    for (;;) {
      if (local_C3[local_BB] == 5) {
        local_CF[local_BB] = 6;
      } else if (local_C3[local_BB] == 3) {
        local_CF[local_BB] = 4;
      } else if (local_C3[local_BB] == 4) {
        local_CF[local_BB] = 4;
      } else {
        if (local_C3[local_BB] == 1) {
          local_CF[local_BB] = 5;
          goto L016752;
        }
      }
      L016752:;
      local_BB = local_BB + 1;
      if (local_BB == 3) {
        break;
      }
    }
    local_BB = 0;
    local_BA = 0;
    local_FC = local_FC + 1;
    if ((local_C1[0] == 1) & (local_C1[1] == 0)) {
      script_178AC(*(int16_t *)((int8_t *)&data_2013C + (local_CF[0] << 1)), *(int16_t *)((int8_t *)&data_2013C + (local_CF[1] << 1)), local_DE);
      if (*task_29C != 0xA000) {
        if (*task_29C & *(int16_t *)((int8_t *)&data_2013C + (local_CF[0] << 1))) {
          local_C9[0] = 0;
        } else {
          local_C9[0] = 1;
        }
      } else {
        local_C9[0] = 2;
      }
      local_BA = 1;
      if (local_C9[0] == 0) {
        if (task_29C[1] != 0xA000) {
          if (task_29C[1] & *(int16_t *)((int8_t *)&data_2013C + (local_CF[1] << 1))) {
            local_C9[1] = 0;
          } else {
            local_C9[1] = 1;
          }
        } else {
          local_C9[1] = 2;
        }
        local_BA = 2;
      }
    } else {
      for (;;) {
        if (-(local_BB == 0) & -(local_C1[0] == 0) & -(local_C1[1] == 0)) {
          local_B9 = 1;
        } else if (-(local_BB == 0) & -(local_C1[0] == 3) & -(local_C1[1] == 0)) {
          local_B9 = 1;
        } else if (-(local_BB == 1) & -(local_C1[0] == 2) & -(local_C1[1] == 0)) {
          local_B9 = 1;
        } else if (-(local_BB == 2) & -(local_C1[0] == 4) & -(local_C1[1] == 0)) {
          local_B9 = 1;
        } else {
          if (local_BB > 5) {
            Script_sprintf("\012ＱＴＥ発生えらー\012");
            goto L0168FD;
          }
        }
        L0168FD:;
        task_29C[local_BB] = script_17646(*(int16_t *)((int8_t *)&data_2013C + (local_CF[local_BB] << 1)), local_DE, local_B9);
        if (task_29C[local_BB] != 0xA000) {
          if (task_29C[local_BB] & *(int16_t *)((int8_t *)&data_2013C + (local_CF[local_BB] << 1))) {
            local_C9[local_BB] = 0;
          } else {
            local_C9[local_BB] = 1;
          }
        } else {
          local_C9[local_BB] = 2;
        }
        local_BB = local_BB + 1;
        local_BA = local_BA + 1;
        if (local_C9[local_BB - 1] != 0) {
          local_B9 = 1;
          Script_GameUtils(SHEN_NAME('QTEM'), GC_CommitStagedEntries, 0, 0, 0, 0);
        }
        if (local_B9 == 1) {
          break;
        }
      }
    }
    Script_charaDisplay(CHARA_AKIR, CDM_Show);
    local_BB = 0;
    for (;;) {
      if (local_C9[local_BB] == 0) {
        if (local_BB == 0) {
          script_15586(local_C3[local_BB], 0);
        } else {
          script_15586(local_C3[local_BB], 2);
        }
        if (task_294 < 0x5F5E0FF) {
          task_294 = task_294 + task_2BC;
        }
        if (task_294 + Script_FloatUtils(ScriptMathOp_FloatToInt, task_2C8) >= task_204) {
          task_204 = task_294 + Script_FloatUtils(ScriptMathOp_FloatToInt, task_2C8);
        }
        if (task_294 + Script_FloatUtils(ScriptMathOp_FloatToInt, task_2C8) == 0x5F5E0FF) {
          break;
        }
        script_18D94(task_294 + Script_FloatUtils(ScriptMathOp_FloatToInt, task_2C8), task_204, task_208, task_20C, 0);
      } else {
        local_B7 = 5;
        local_B8 = 1;
        local_FC = local_FC - 1;
        local_C0 = local_C0 - 1;
        if (local_C9[local_BB] == 1) {
          if ((local_BB == 0) & (local_C1[0] == 4)) {
            script_15586(local_C3[2], 4);
          } else if (local_BB != 0) {
            script_15586(local_C3[0], 0);
          } else {
            script_15586(local_C3[2], 0);
          }
        }
        script_18D94(task_294 + Script_FloatUtils(ScriptMathOp_FloatToInt, task_2C8), task_204, task_208, task_20C, 0);
        if (local_C0 > 0) {
          script_14D72(7303, SHEN_NAME('TITL'), 0);
        } else {
          script_14D72(7300, SHEN_NAME('TITL'), 0);
        }
        if (local_C0 > 0) {
          script_1849C(local_C0);
        } else {
          break;
        }
      }
      local_BB = local_BB + 1;
      if (local_BB == local_BA) {
        break;
      }
    }
    if (local_BB < local_BA) {
      break;
    }
    local_BB = 0;
    for (;;) {
      local_C9[local_BB] = 0;
      local_BB = local_BB + 1;
      if (local_BB == 6) {
        break;
      }
    }
    if (local_FC > 850) {
      local_FC = 711;
    }
    if ((local_FC > 840) & (local_FC <= 850)) {
      local_DA = 145;
    } else if ((local_FC > 830) & (local_FC <= 840)) {
      local_DA = 140;
    } else if ((local_FC > 820) & (local_FC <= 830)) {
      local_DA = 135;
    } else if ((local_FC > 810) & (local_FC <= 820)) {
      local_DA = 130;
    } else if ((local_FC > 800) & (local_FC <= 810)) {
      local_DA = 125;
    } else if ((local_FC > 790) & (local_FC <= 800)) {
      local_DA = 115;
      local_D7 = 1;
      local_F8 = 2.0f;
    } else if ((local_FC > 770) & (local_FC <= 790)) {
      local_E0[2] = 6.0f;
      local_DA = 130;
    } else if ((local_FC > 750) & (local_FC <= 770)) {
      local_E0[2] = 4.0f;
      local_DA = 125;
    } else if ((local_FC > 730) & (local_FC <= 750)) {
      local_E0[2] = 3.0f;
      local_DA = 120;
      local_D9 = 2;
      local_B4 = 0;
    } else if ((local_FC > 710) & (local_FC <= 730)) {
      local_E0[2] = 2.0f;
      local_DA = 115;
      local_D9 = 0;
      local_D7 = 0;
      local_F8 = 3.0f;
      if (local_B4 == 0) {
        local_B4 = 1;
      }
    } else if ((local_FC > 700) & (local_FC <= 710)) {
      local_DA = 130;
    } else if ((local_FC > 690) & (local_FC <= 700)) {
      local_DA = 125;
    } else if ((local_FC > 680) & (local_FC <= 690)) {
      local_DA = 120;
    } else if ((local_FC > 670) & (local_FC <= 680)) {
      local_DA = 115;
    } else if ((local_FC > 660) & (local_FC <= 670)) {
      local_DA = 110;
    } else if ((local_FC > 650) & (local_FC <= 660)) {
      local_DA = 100;
      local_D7 = 1;
      local_F8 = 2.0f;
    } else if ((local_FC > 630) & (local_FC <= 650)) {
      local_E0[2] = 6.0f;
      local_DA = 130;
    } else if ((local_FC > 610) & (local_FC <= 630)) {
      local_E0[2] = 4.0f;
      local_DA = 120;
    } else if ((local_FC > 590) & (local_FC <= 610)) {
      local_E0[2] = 3.0f;
      local_DA = 110;
      local_D9 = 2;
      local_B4 = 0;
    } else if ((local_FC > 570) & (local_FC <= 590)) {
      local_E0[2] = 2.0f;
      local_DA = 100;
      local_D7 = 0;
      local_F8 = 3.0f;
      if (local_B4 == 0) {
        local_B4 = 1;
      }
    } else if ((local_FC > 560) & (local_FC <= 570)) {
      local_DA = 120;
    } else if ((local_FC > 550) & (local_FC <= 560)) {
      local_DA = 115;
    } else if ((local_FC > 540) & (local_FC <= 550)) {
      local_DA = 105;
    } else if ((local_FC > 530) & (local_FC <= 540)) {
      local_DA = 95;
    } else if ((local_FC > 520) & (local_FC <= 530)) {
      local_DA = 85;
    } else if ((local_FC > 510) & (local_FC <= 520)) {
      local_DA = 75;
      local_D7 = 1;
      local_F8 = 2.0f;
    } else if ((local_FC > 490) & (local_FC <= 510)) {
      local_E0[2] = 6.0f;
      local_DA = 115;
    } else if ((local_FC > 470) & (local_FC <= 490)) {
      local_E0[2] = 4.0f;
      local_DA = 105;
    } else if ((local_FC > 450) & (local_FC <= 470)) {
      local_E0[2] = 3.0f;
      local_DA = 95;
      local_D9 = 2;
      local_B4 = 0;
    } else if ((local_FC > 430) & (local_FC <= 450)) {
      local_E0[2] = 2.0f;
      local_DA = 75;
      local_D9 = 0;
      local_D7 = 0;
      local_F8 = 3.0f;
      if (local_B4 == 0) {
        local_B4 = 1;
      }
    } else if ((local_FC > 420) & (local_FC <= 430)) {
      local_DA = 110;
    } else if ((local_FC > 410) & (local_FC <= 420)) {
      local_DA = 105;
    } else if ((local_FC > 400) & (local_FC <= 410)) {
      local_DA = 100;
    } else if ((local_FC > 390) & (local_FC <= 400)) {
      local_DA = 95;
    } else if ((local_FC > 380) & (local_FC <= 390)) {
      local_DA = 85;
    } else if ((local_FC > 370) & (local_FC <= 380)) {
      local_DA = 75;
      local_D7 = 1;
      local_F8 = 2.0f;
    } else if ((local_FC > 350) & (local_FC <= 370)) {
      local_E0[2] = 6.0f;
      local_DA = 110;
    } else if ((local_FC > 330) & (local_FC <= 350)) {
      local_E0[2] = 4.0f;
      local_DA = 90;
    } else if ((local_FC > 310) & (local_FC <= 330)) {
      local_E0[2] = 3.0f;
      local_DA = 70;
      local_D9 = 1;
    } else if ((local_FC > 290) & (local_FC <= 310)) {
      local_E0[2] = 2.0f;
      local_DA = 50;
      local_D8 = 1;
    } else if ((local_FC > 270) & (local_FC <= 290)) {
      local_E0[1] = 6.0f;
      local_DA = 100;
    } else if ((local_FC > 250) & (local_FC <= 270)) {
      local_E0[1] = 4.0f;
      local_DA = 75;
      local_B4 = 0;
    } else if ((local_FC > 230) & (local_FC <= 250)) {
      local_E0[1] = 2.0f;
      local_DA = 50;
      local_D8 = 0;
      local_D7 = 0;
      local_F8 = 3.0f;
      if (local_B4 == 0) {
        local_B4 = 1;
      }
    } else if ((local_FC > 220) & (local_FC <= 230)) {
      local_DA = 100;
    } else if ((local_FC > 210) & (local_FC <= 220)) {
      local_DA = 95;
    } else if ((local_FC > 200) & (local_FC <= 210)) {
      local_DA = 90;
    } else if ((local_FC > 190) & (local_FC <= 200)) {
      local_DA = 85;
    } else if ((local_FC > 180) & (local_FC <= 190)) {
      local_DA = 80;
    } else if ((local_FC > 170) & (local_FC <= 180)) {
      local_DA = 75;
      local_D7 = 1;
      local_F8 = 2.0f;
    } else if ((local_FC > 150) & (local_FC <= 170)) {
      local_E0[2] = 6.0f;
      local_DA = 100;
    } else if ((local_FC > 130) & (local_FC <= 150)) {
      local_E0[2] = 4.0f;
      local_DA = 75;
    } else if ((local_FC > 110) & (local_FC <= 130)) {
      local_E0[2] = 3.0f;
      local_DA = 50;
    } else if ((local_FC > 90) & (local_FC <= 110)) {
      local_E0[2] = 2.0f;
      local_DA = 25;
      local_D8 = 1;
    } else if ((local_FC > 70) & (local_FC <= 90)) {
      local_E0[1] = 6.0f;
      local_DA = 100;
    } else if ((local_FC > 50) & (local_FC <= 70)) {
      local_E0[1] = 4.0f;
      local_DA = 75;
    } else if ((local_FC > 30) & (local_FC <= 50)) {
      local_E0[1] = 2.0f;
      local_DA = 50;
      local_F8 = 3.0f;
    } else if ((local_FC > 20) & (local_FC <= 30)) {
      local_DA = 100;
    } else if ((local_FC > 10) & (local_FC <= 20)) {
      local_DA = 50;
    } else {
      local_DA = 0;
    }
    local_D6 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_IncrementQTEMinigameRand16(1, local_F8));
    if ((local_D6 == 0) & (local_FC > 30)) {
      local_D6 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_IncrementQTEMinigameRand16(1, 6.0f));
      if (-(local_D6 == 0) & -(local_D7 > 0) & -(local_B7 <= 0)) {
        local_C1[0] = 1;
        local_C1[1] = 0;
      } else if (-(local_D6 == 1) & -(local_D9 > 0) & -(local_B7 <= 0)) {
        local_C1[0] = 3;
        local_C1[1] = 0;
        if (local_DA >= 115) {
          local_DA = 115;
        }
      } else if (-(local_D6 == 2) & -(local_D9 > 1) & -(local_B7 <= 0)) {
        local_C1[0] = 3;
        local_C1[1] = 0;
        if (local_DA >= 115) {
          local_DA = 115;
        }
      } else if ((local_D6 == 3) & (local_D8 > 0)) {
        local_C1[0] = 4;
        local_C1[1] = 0;
      } else if ((local_D6 == 4) & (local_D8 > 0)) {
        local_C1[0] = 4;
        local_C1[1] = 0;
      } else {
        local_C1[0] = 2;
        local_C1[1] = 0;
      }
    } else {
      local_C1[0] = 0;
      local_C1[1] = 0;
    }
    if (local_DA < 145) {
      local_DE = 31 - local_DA / 5 / 1;
    } else {
      local_DE = 1;
    }
    if (local_DA < 145) {
      local_BF = 31 - local_DA / 5;
    } else {
      local_BF = 1;
    }
    if (local_B4 == 1) {
      task_2BC = task_2BC + 500;
      local_B4 = 2;
    }
    local_DE = local_DE + local_B7;
    if (local_B7 > 0) {
      local_B7 = local_B7 - 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1100A0, 2, 0);
  script_14D58(CHARA_AKIR);
  local_C0 = 0;
  if ((task_2C3 == -1) | (task_2C4 == -1) | (task_2C5 == -1)) {
    script_1849C(local_C0);
  }
  Script_SetSfxOverlayTimeoutTuning(2, SfxMode_SessionAction);
  task_2C2 = 2;
  script_14E30(&local_A8, -0.073f, 0.0f, 1.532f);
  script_18900(SHEN_NAME('TITL'), CHARA_AKIR, &local_A8, 0);
  script_14D72(7341, SHEN_NAME('TITL'), 0);
  SCNF_WaitFrames_0000(20);
  task_2C0 = 10;
  return;
}

static int32_t script_17646(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  int8_t local_F6;
  local_F7 = 0;
  local_F6 = 0;
  SCNF_WaitFrames_0000(1);
  for (;;) {
    script_18D94(task_294 + Script_FloatUtils(ScriptMathOp_FloatToInt, task_2C8), task_204, task_208, task_20C, 0);
    {
      uint8_t unused_F4[2];
      int32_t local_F0;
      local_F0 = local_F6;
      if ((local_F0 == 0) | (local_F0 == 1) | (local_F0 == 2)) {
        if (local_F6 == 0) {
          Script_executeAicaCommand(171, 0, 0);
        }
        switch (arg0) {
          case 0x100:
            script_18F06(0);
            break;
          case 0x200:
            script_18F06(1);
            break;
          case 0x400:
            script_18F06(2);
            break;
        }
      } else {
        if ((local_F0 == 3) | (local_F0 == 4)) {
          if (local_F6 == 4) {
            local_F6 = -1;
          }
          goto L017711;
        }
      }
    }
    L017711:;
    local_F8 = Script_GetControllerInput(9);
    if ((local_F8 == 1028) | (local_F8 == 516) | (local_F8 == 0x600) | (local_F8 == 4) | (local_F8 == 0x400) | (local_F8 == 0x200)) {
      uint8_t unused_F4[2];
      int32_t local_F0;
      local_F0 = arg0;
      switch (local_F0) {
        case 0x100:
          if (local_F8 == 0x400) {
            Script_executeAicaCommand(589995, 0, 0);
            local_FC = 0x2100;
            script_18413(arg1, local_F7, 0x100, arg2);
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            local_FC = 0;
          }
          goto L017888;
          break;
        case 0x200:
          if (local_F8 == 0x200) {
            Script_executeAicaCommand(589995, 0, 0);
            local_FC = 0x2200;
            script_18413(arg1, local_F7, 0x200, arg2);
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            local_FC = 0;
          }
          goto L017888;
          break;
        case 0x400:
          if (local_F8 == 4) {
            Script_executeAicaCommand(589995, 0, 0);
            local_FC = 0x2400;
            script_18413(arg1, local_F7, 0x400, arg2);
          } else {
            Script_executeAicaCommand(655531, 0, 0);
            local_FC = 0;
          }
          goto L017888;
          break;
      }
    }
    if (local_F7 >= arg1) {
      Script_executeAicaCommand(655531, 0, 0);
      local_FC = 0xA000;
      break;
    }
    local_F7 = local_F7 + 1;
    local_F6 = local_F6 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L017888:;
  script_18D94(task_294 + Script_FloatUtils(ScriptMathOp_FloatToInt, task_2C8), task_204, task_208, task_20C, 0);
  return local_FC;
}

static void script_178AC(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  int8_t local_ED;
  int8_t local_EC;
  int8_t local_EB;
  int8_t local_EA;
  local_FC = 0;
  local_F8 = 0;
  local_EE = 0;
  local_ED = 0;
  local_EC = 0;
  local_EB = 0;
  local_EA = 0;
  task_29C[0] = 0;
  task_29C[1] = 0;
  local_EF = 0;
  SCNF_WaitFrames_0000(1);
  if ((arg0 == 0x100) | (arg1 == 0x100)) {
    if ((arg0 == 0x200) | (arg1 == 0x200)) {
      local_F0 = 0;
    } else {
      local_F0 = 1;
    }
  } else {
    local_F0 = 2;
  }
  for (;;) {
    script_18D94(task_294 + Script_FloatUtils(ScriptMathOp_FloatToInt, task_2C8), task_204, task_208, task_20C, 0);
    {
      uint8_t unused_E8[2];
      int32_t local_E4;
      local_E4 = local_ED;
      if ((local_E4 == 0) | (local_E4 == 1) | (local_E4 == 2)) {
        if (local_ED == 0) {
          Script_executeAicaCommand(171, 0, 0);
        }
        if ((arg0 == 0x100) | (arg1 == 0x100)) {
          script_18F06(0);
        }
        if ((arg0 == 0x200) | (arg1 == 0x200)) {
          script_18F06(1);
        }
        if ((arg0 == 0x400) | (arg1 == 0x400)) {
          script_18F06(2);
        }
      } else {
        if ((local_E4 == 3) | (local_E4 == 4)) {
          if (local_ED == 4) {
            local_ED = -1;
          }
          goto L0179F2;
        }
      }
    }
    L0179F2:;
    local_F4 = Script_GetControllerInput(9);
    if ((local_F4 == 1028) | (local_F4 == 516) | (local_F4 == 0x600)) {
      uint8_t unused_E8[2];
      int32_t local_E4;
      local_E4 = local_F0;
      switch (local_E4) {
        case 0:
          if (local_F4 == 0x600) {
            if (arg0 == 0x100) {
              local_FC = 0x2100;
              local_F8 = 0x2200;
            } else {
              local_FC = 0x2200;
              local_F8 = 0x2100;
            }
          } else {
            local_FC = 0;
            local_F8 = 0;
          }
          break;
        case 1:
          if (local_F4 == 1028) {
            if (arg0 == 0x100) {
              local_FC = 0x2100;
              local_F8 = 0x2400;
            } else {
              local_FC = 0x2400;
              local_F8 = 0x2100;
            }
          } else {
            local_FC = 0;
            local_F8 = 0;
          }
          break;
        case 2:
          if (local_F4 == 516) {
            if (arg0 == 0x200) {
              local_FC = 0x2200;
              local_F8 = 0x2400;
            } else {
              local_FC = 0x2400;
              local_F8 = 0x2200;
            }
          } else {
            local_FC = 0;
            local_F8 = 0;
          }
          break;
      }
      if ((local_FC == 0) & (local_F8 == 0)) {
        Script_executeAicaCommand(655531, 0, 0);
        break;
      } else {
        if (local_FC != 0) {
          task_29C[0] = local_FC;
          local_EB = local_EE;
        }
        if (local_F8 != 0) {
          task_29C[1] = local_F8;
          local_EA = local_EE;
        }
      }
    }
    if ((local_F4 == 4) | (local_F4 == 0x400) | (local_F4 == 0x200)) {
      {
        uint8_t unused_E8[2];
        int32_t local_E4;
        local_E4 = arg0;
        switch (local_E4) {
          case 0x100:
            if (local_F4 == 0x400) {
              local_FC = 0x2100;
            } else {
              local_FC = 0;
            }
            break;
          case 0x200:
            if (local_F4 == 0x200) {
              local_FC = 0x2200;
            } else {
              local_FC = 0;
            }
            break;
          case 0x400:
            if (local_F4 == 4) {
              local_FC = 0x2400;
            } else {
              local_FC = 0;
            }
            break;
        }
      }
      {
        uint8_t unused_E8[2];
        int32_t local_E4;
        local_E4 = arg1;
        switch (local_E4) {
          case 0x100:
            if (local_F4 == 0x400) {
              local_F8 = 0x2100;
            } else {
              local_F8 = 0;
            }
            break;
          case 0x200:
            if (local_F4 == 0x200) {
              local_F8 = 0x2200;
            } else {
              local_F8 = 0;
            }
            break;
          case 0x400:
            if (local_F4 == 4) {
              local_F8 = 0x2400;
            } else {
              local_F8 = 0;
            }
            break;
        }
      }
      if ((local_FC == 0) & (local_F8 == 0)) {
        Script_executeAicaCommand(655531, 0, 0);
        break;
      } else {
        if (local_FC != 0) {
          task_29C[0] = local_FC;
          local_EB = local_EE;
        }
        if (local_F8 != 0) {
          task_29C[1] = local_F8;
          local_EA = local_EE;
        }
      }
    }
    if ((local_EB != 0) & (local_EA != 0)) {
      local_EC = local_EB - local_EA;
      if (local_EC < 0) {
        local_EC = local_EC * -1;
      }
    }
    if (local_F4 == 1540) {
      Script_executeAicaCommand(655531, 0, 0);
      break;
    }
    if ((local_EE >= arg2) | (local_EC > 5)) {
      Script_executeAicaCommand(655531, 0, 0);
      if (local_FC == 0) {
        task_29C[0] = 0xA000;
      }
      if (local_F8 == 0) {
        task_29C[1] = 0xA000;
      }
      break;
    }
    if ((*task_29C != 0) & (task_29C[1] != 0)) {
      Script_executeAicaCommand(589995, 0, 0);
      script_18413(arg2, local_EE, 0x100, 1);
      break;
    }
    local_EE = local_EE + 1;
    local_ED = local_ED + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_18D94(task_294 + Script_FloatUtils(ScriptMathOp_FloatToInt, task_2C8), task_204, task_208, task_20C, 0);
  return;
}

static void script_17DB2(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 2:
      Script_charaDisplay(SHEN_NAME('QTL3'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('QTL2'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('QTL3'), CDM_Hide);
      break;
    case 0:
      Script_charaDisplay(SHEN_NAME('QTL1'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('QTL2'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('QTL3'), CDM_Hide);
      break;
  }
  return;
}

static void script_17E21(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_17E39, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_17E39(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_18061(arg0, arg1, arg2, arg3);
  if (arg3 == 3) {
    int32_t local_FC;
    local_FC = arg2;
    if ((local_FC == 5) | (local_FC == 6)) {
      script_189B1(SHEN_NAME('MIT1'), 3, arg4);
    } else if ((local_FC == 3) | (local_FC == 4)) {
      script_189B1(SHEN_NAME('MIT2'), 3, arg4);
    } else {
      if ((local_FC == 1) | (local_FC == 2)) {
        script_189B1(SHEN_NAME('MIT3'), 3, arg4);
        goto L017EBF;
      }
    }
    L017EBF:;
    SCNF_WaitFrames_0000(arg4 / 2);
  }
  {
    int32_t local_FC;
    local_FC = arg0;
    if ((local_FC == 5) | (local_FC == 6)) {
      script_189B1(SHEN_NAME('MIT1'), 1, arg4);
    } else if ((local_FC == 3) | (local_FC == 4)) {
      script_189B1(SHEN_NAME('MIT2'), 1, arg4);
    } else {
      if ((local_FC == 1) | (local_FC == 2)) {
        script_189B1(SHEN_NAME('MIT3'), 1, arg4);
        goto L017F36;
      }
    }
  }
  L017F36:;
  if ((arg3 == 2) | (arg3 == 4)) {
    SCNF_WaitFrames_0000(arg4 / 2);
  }
  if ((arg3 == 1) | (arg3 == 2) | (arg3 == 4)) {
    int32_t local_FC;
    local_FC = arg1;
    if ((local_FC == 5) | (local_FC == 6)) {
      script_189B1(SHEN_NAME('MIT1'), 1, arg4);
    } else if ((local_FC == 3) | (local_FC == 4)) {
      script_189B1(SHEN_NAME('MIT2'), 1, arg4);
    } else {
      if ((local_FC == 1) | (local_FC == 2)) {
        script_189B1(SHEN_NAME('MIT3'), 1, arg4);
        goto L017FD1;
      }
    }
  }
  L017FD1:;
  if (arg3 == 4) {
    SCNF_WaitFrames_0000(arg4 / 2);
  }
  if (arg3 == 4) {
    int32_t local_FC;
    local_FC = arg2;
    if ((local_FC == 5) | (local_FC == 6)) {
      script_189B1(SHEN_NAME('MIT1'), 1, arg4);
    } else if ((local_FC == 3) | (local_FC == 4)) {
      script_189B1(SHEN_NAME('MIT2'), 1, arg4);
    } else {
      if ((local_FC == 1) | (local_FC == 2)) {
        script_189B1(SHEN_NAME('MIT3'), 1, arg4);
        goto L01805A;
      }
    }
  }
  L01805A:;
  return Script_srchControl(0);
}

static void script_18061(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  {
    int32_t local_FC;
    local_FC = arg0;
    if ((local_FC == 5) | (local_FC == 6)) {
      (void)((task_2C3 = -1) & 4);
    } else if ((local_FC == 3) | (local_FC == 4)) {
      (void)((task_2C4 = -1) & 4);
    } else {
      if ((local_FC == 1) | (local_FC == 2)) {
        (void)((task_2C5 = -1) & 4);
        goto L0180BD;
      }
    }
  }
  L0180BD:;
  if ((arg3 == 1) | (arg3 == 2) | (arg3 == 4)) {
    int32_t local_FC;
    local_FC = arg1;
    if ((local_FC == 5) | (local_FC == 6)) {
      (void)((task_2C3 = -1) & 4);
    } else if ((local_FC == 3) | (local_FC == 4)) {
      (void)((task_2C4 = -1) & 4);
    } else {
      if ((local_FC == 1) | (local_FC == 2)) {
        (void)((task_2C5 = -1) & 4);
        goto L01812C;
      }
    }
  }
  L01812C:;
  if (arg3 == 4) {
    int32_t local_FC;
    local_FC = arg2;
    if ((local_FC == 5) | (local_FC == 6)) {
      (void)((task_2C3 = -1) & 4);
    } else if ((local_FC == 3) | (local_FC == 4)) {
      (void)((task_2C4 = -1) & 4);
    } else {
      if ((local_FC == 1) | (local_FC == 2)) {
        (void)((task_2C5 = -1) & 4);
        goto L01818F;
      }
    }
  }
  L01818F:;
  return;
}

static void script_18190(void) {
  SCNF_WaitFrames_0000(1);
  if (task_2C3 == -1) {
    script_189B1(SHEN_NAME('MIT1'), 2, 0);
    (void)((task_2C3 = 0) & 4);
  }
  if (task_2C4 == -1) {
    script_189B1(SHEN_NAME('MIT2'), 2, 0);
    (void)((task_2C4 = 0) & 4);
  }
  if (task_2C5 == -1) {
    script_189B1(SHEN_NAME('MIT3'), 2, 0);
    (void)((task_2C5 = 0) & 4);
  }
  return;
}

static void script_18207(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int32_t local_EC;
  local_F3 = 0;
  local_F4 = 0;
  local_FC = arg0 / 1000;
  local_F8 = task_298 / 44;
  if (local_F8 < 1) {
    local_F8 = 1;
  }
  task_294 = task_294 + Script_FloatUtils(ScriptMathOp_FloatToInt, task_2C8);
  if ((task_294 >= task_204) & (task_294 > 0)) {
    local_F3 = 1;
    task_204 = task_294;
  }
  if (task_294 == 0x5F5E0FF) {
    Script_executeAicaCommand(8104, 0, 0);
  } else {
    if ((local_F3 == 1) & (task_294 > 0)) {
      Script_executeAicaCommand(1440, 2, -2);
      Script_executeAicaCommand(73640, 0, 0);
      goto L0182B1;
    }
  }
  L0182B1:;
  local_F2 = 0;
  for (;;) {
    local_EC = local_F3;
    if (local_EC == 1) {
      script_18D94(task_294, task_204, task_208, task_20C, 2);
    } else {
      script_18D94(task_294, task_204, task_208, task_20C, 1);
    }
    SCNF_WaitFrames_0000(20);
    script_18D94(task_294, task_204, task_208, task_20C, 0);
    SCNF_WaitFrames_0000(20);
    if (local_F2 == 4) {
      break;
    }
    local_F2 = local_F2 + 1;
    SCNF_WaitFrames_0000(1);
  }
  task_208 = task_294;
  if (-(local_F3 == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 941) == 0) & -(task_294 >= 300000)) {
    task_2C0 = 15;
  } else {
    script_14D72(7301, SHEN_NAME('TITL'), 0);
    if (local_FC >= 231) {
      task_2C0 = 14;
    } else if (local_FC >= 171) {
      task_2C0 = 13;
    } else if (local_FC >= 91) {
      task_2C0 = 12;
    } else if (local_F3 == 0) {
      task_2C0 = 11;
    } else {
      task_2C0 = 12;
    }
    if (local_F3 == 1) {
      script_14D22(CHARA_AKIR, 30986, 0);
    } else {
      script_14D22(CHARA_AKIR, 31001, 0);
    }
    SCNF_WaitFrames_0000(20);
  }
  Script_executeAicaCommand(1440, 2, 2);
  return;
}

static void script_18413(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  float local_FC;
  script_18D94(task_294, task_204, task_208, task_20C, 0);
  if (arg0 == arg1) {
    local_FC = 0.0f;
  } else if (arg1 == 0) {
    local_FC = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, task_2BC) * 0.1f;
  } else {
    if (arg0 - arg1 != 0) {
      local_FC = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, arg0 - arg1 + 1) / Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, arg0) * (float)(task_2BC / 10);
      goto L018490;
    }
  }
  L018490:;
  task_2C8 = task_2C8 + local_FC;
  return;
}

static void script_1849C(int32_t arg0) {
  Script_executeAicaCommand(0x1805A9, 0, 0);
  script_18190();
  SCNF_WaitFrames_0000(60);
  script_184C2(arg0);
  return;
}

static void script_184C2(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  {
    int32_t local_F8;
    local_F8 = arg0;
    switch (local_F8) {
      case 2:
        Script_charaDisplay(SHEN_NAME('QTL3'), CDM_Hide);
        break;
      case 1:
        Script_charaDisplay(SHEN_NAME('QTL2'), CDM_Hide);
        break;
      case 0:
        Script_charaDisplay(SHEN_NAME('QTL1'), CDM_Hide);
        break;
    }
  }
  local_FF = 0;
  local_FE = Script_charaDisplay(SHEN_NAME('QTL1'), CDM_Query);
  local_FD = Script_charaDisplay(SHEN_NAME('QTL2'), CDM_Query);
  local_FC = Script_charaDisplay(SHEN_NAME('QTL3'), CDM_Query);
  if (arg0 > 0) {
    for (;;) {
      int32_t local_F8;
      if (local_FF == 2) {
        switch (arg0) {
          case 2:
            Script_executeAicaCommand(0x2605A9, 0, 0);
            break;
          case 1:
            Script_executeAicaCommand(0x2405A9, 0, 0);
            break;
        }
      }
      local_F8 = arg0;
      switch (local_F8) {
        case 2:
          Script_charaDisplay(SHEN_NAME('QTL2'), CDM_Hide);
          Script_charaDisplay(SHEN_NAME('QTL3'), CDM_Hide);
          break;
        case 1:
          Script_charaDisplay(SHEN_NAME('QTL1'), CDM_Hide);
          Script_charaDisplay(SHEN_NAME('QTL2'), CDM_Hide);
          Script_charaDisplay(SHEN_NAME('QTL3'), CDM_Hide);
          break;
      }
      SCNF_WaitFrames_0000(20);
      if (local_FE != 0) {
        Script_charaDisplay(SHEN_NAME('QTL1'), CDM_Show);
      }
      if (local_FD != 0) {
        Script_charaDisplay(SHEN_NAME('QTL2'), CDM_Show);
      }
      if (local_FC != 0) {
        Script_charaDisplay(SHEN_NAME('QTL3'), CDM_Show);
      }
      SCNF_WaitFrames_0000(20);
      local_FF = local_FF + 1;
      if (local_FF == 3) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(20);
    Script_executeAicaCommand(0x2505A9, 0, 0);
  }
  return;
}

static void script_18660(void) {
  SCNF_WaitFrames_0000(30);
  script_D2CB(3);
  task_2C0 = 18;
  return;
}

static void script_1867B(void) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
  local_F8 = local_FF;
  switch (local_F8) {
    case 0:
      Script_executeAicaCommand(0x1105A9, 0, 0);
      break;
    case 1:
      Script_executeAicaCommand(0x1205A9, 0, 0);
      SCNF_WaitFrames_0000(30);
      Script_executeAicaCommand(0x1305A9, 0, 0);
      break;
    case 2:
      Script_executeAicaCommand(0x1405A9, 0, 0);
      break;
    case 3:
      Script_executeAicaCommand(0x1505A9, 0, 0);
      break;
    case 4:
      Script_executeAicaCommand(0x1605A9, 0, 0);
      break;
  }
  return;
}

static void script_1872D(void) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
  local_F8 = local_FF;
  switch (local_F8) {
    case 0:
      Script_executeAicaCommand(0x1905A9, 0, 0);
      SCNF_WaitFrames_0000(30);
      Script_executeAicaCommand(0x1A05A9, 0, 0);
      break;
    case 1:
      Script_executeAicaCommand(0x1905A9, 0, 0);
      SCNF_WaitFrames_0000(30);
      Script_executeAicaCommand(0x1B05A9, 0, 0);
      break;
    case 2:
      Script_executeAicaCommand(0x1905A9, 0, 0);
      SCNF_WaitFrames_0000(30);
      Script_executeAicaCommand(0x1C05A9, 0, 0);
      break;
  }
  script_14D58(CHARA_AKIR);
  SCNF_WaitFrames_0000(20);
  task_2C0 = 18;
  return;
}

static void script_187F2(void) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
  local_F8 = local_FF;
  switch (local_F8) {
    case 0:
      Script_executeAicaCommand(0x1D05A9, 0, 0);
      break;
    case 1:
      Script_executeAicaCommand(0x1E05A9, 0, 0);
      break;
    case 2:
      Script_executeAicaCommand(0x1F05A9, 0, 0);
      break;
    case 3:
      Script_executeAicaCommand(0x2005A9, 0, 0);
      break;
    case 4:
      Script_executeAicaCommand(0x2105A9, 0, 0);
      break;
  }
  script_14D58(CHARA_AKIR);
  SCNF_WaitFrames_0000(20);
  task_2C0 = 18;
  return;
}

static void script_188AA(void) {
  Script_executeAicaCommand(0x2205A9, 0, 0);
  script_14D58(CHARA_AKIR);
  SCNF_WaitFrames_0000(20);
  task_2C0 = 18;
  return;
}

static void script_188D5(void) {
  Script_executeAicaCommand(0x2305A9, 0, 0);
  script_14D58(CHARA_AKIR);
  SCNF_WaitFrames_0000(20);
  task_2C0 = 18;
  return;
}

static void script_18900(int32_t arg0, int32_t arg1, Vector3 *arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (arg3 != 0) {
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    script_1902D(arg3, (&local_E8)->x, (&local_E8)->y, (&local_E8)->z);
    if (arg1 == 0x52494B41) {
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_IMMEDIATE);
    }
    Script_EVT_SetRotation(arg1, 0x38000000, (const Vector3i *)arg3, 0);
  }
  if (arg2 != 0) {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    script_14E4B(arg2, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
    if (arg1 == 0x52494B41) {
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_IMMEDIATE);
      Script_InitializeOsag(CHARA_AKIR);
    }
    Script_EVT_SetPosition(arg1, POS_XYZ, arg2);
  }
  return;
}

static void script_189B1(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg1 == 1) {
    Script_scenCreateTask(script_189FC, 2, arg0, arg2);
  }
  if (arg1 == 2) {
    Script_scenCreateTask(script_18B3A, 2, arg0, arg2);
  }
  if (arg1 == 3) {
    Script_scenCreateTask(script_18BD1, 2, arg0, arg2);
  }
  return;
}

static int32_t script_189FC(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  {
    int8_t local_F3;
    local_F3 = 0;
    script_190B1(&local_F4, 45.0f / (float)(arg1 + 1), 0.0f, 0.0f, 0);
    Script_executeAicaCommand(984489, 0, 0);
    for (;;) {
      Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
      local_F3 = local_F3 + 1;
      if ((arg1 <= 10) & (local_F3 >= arg1 - 1)) {
        break;
      } else {
        if ((arg1 > 10) & (local_F3 >= arg1 - 5)) {
          break;
          goto L018A74;
        }
      }
      L018A74:;
      if (task_2C6 == -1) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  {
    int8_t local_F3;
    local_F3 = 0;
    if (task_2C6 == -1) {
      script_190B1(&local_F4, 0.0f, 0.0f, 0.0f, 0);
      script_18900(SHEN_NAME('TITL'), arg0, 0, (int32_t)&local_F4);
    } else {
      for (;;) {
        script_190B1(&local_F4, -1.0f, 0.0f, 0.0f, 0);
        script_18900(SHEN_NAME('TITL'), arg0, 0, (int32_t)&local_F4);
        SCNF_WaitFrames_0000(2);
        script_190B1(&local_F4, 0.0f, 0.0f, 0.0f, 0);
        script_18900(SHEN_NAME('TITL'), arg0, 0, (int32_t)&local_F4);
        SCNF_WaitFrames_0000(2);
        local_F3 = local_F3 + 1;
        if (local_F3 == 2) {
          break;
        }
      }
    }
  }
  return Script_srchControl(0);
}

static int32_t script_18B3A(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  int8_t local_F3;
  int8_t local_F2;
  local_F3 = 0;
  if (arg1 == 0) {
    local_F2 = 60;
  } else {
    local_F2 = arg1;
  }
  script_190B1(&local_F4, -45.0f / (float)local_F2, 0.0f, 0.0f, 0);
  for (;;) {
    Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
    local_F3 = local_F3 + 1;
    if (local_F3 == local_F2 - 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_190B1(&local_F4, -45.0f, 0.0f, 0.0f, 0);
  script_18900(SHEN_NAME('TITL'), arg0, 0, (int32_t)&local_F4);
  return Script_srchControl(0);
}

static int32_t script_18BD1(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  int8_t local_F3;
  int8_t local_F2;
  if (arg1 == 0) {
    local_F2 = 60;
  } else {
    local_F2 = arg1;
  }
  local_F3 = 0;
  script_190B1(&local_F4, 45.0f / (float)local_F2, 0.0f, 0.0f, 0);
  for (;;) {
    Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
    local_F3 = local_F3 + 1;
    if (local_F3 >= local_F2 / 2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F3 = 0;
  script_190B1(&local_F4, -45.0f / (float)local_F2, 0.0f, 0.0f, 0);
  for (;;) {
    Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
    local_F3 = local_F3 + 1;
    if (local_F3 >= local_F2 / 2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_190B1(&local_F4, -45.0f, 0.0f, 0.0f, 0);
  script_18900(SHEN_NAME('TITL'), arg0, 0, (int32_t)&local_F4);
  return Script_srchControl(0);
}

static void script_18CB8(int32_t arg0) {
  Script_scenCreateTask(script_18CC8, 1, arg0);
  return;
}

static int32_t script_18CC8(int32_t arg0) {
  Vector3i local_F4;
  {
    int8_t local_F3;
    local_F3 = 0;
    script_190B1(&local_F4, -22.5f, 0.0f, 0.0f, 0);
    for (;;) {
      Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
      local_F3 = local_F3 + 1;
      if (local_F3 == 1) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  {
    int8_t local_F3;
    local_F3 = 0;
    for (;;) {
      script_190B1(&local_F4, -44.0f, 0.0f, 0.0f, 0);
      script_18900(SHEN_NAME('TITL'), arg0, 0, (int32_t)&local_F4);
      SCNF_WaitFrames_0000(2);
      script_190B1(&local_F4, -45.0f, 0.0f, 0.0f, 0);
      script_18900(SHEN_NAME('TITL'), arg0, 0, (int32_t)&local_F4);
      SCNF_WaitFrames_0000(2);
      local_F3 = local_F3 + 1;
      if (local_F3 == 2) {
        break;
      }
    }
  }
  return Script_srchControl(0);
}

static void script_18D94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  Script_GameUtils(SHEN_NAME('TITL'), GC_CommitStagedEntries, 0, 0, 0, 0);
  local_FC = arg4;
  switch (local_FC) {
    case 0:
      script_18E50(arg0, 5);
      script_18E50(arg1, 6);
      script_18E50(arg2, 7);
      break;
    case 1:
      script_18E50(arg1, 6);
      script_18E50(arg2, 7);
      break;
    case 2:
      script_18E50(arg2, 7);
      break;
    case 3:
      script_18E50(arg1, 6);
      break;
    case 4:
      script_18E50(arg1, 6);
      script_18E50(arg2, 7);
      break;
  }
  return;
}

static void script_18E50(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  script_14E30(&local_E8, -0.0302f, 0.0f, 0.0f);
  local_E4 = arg1;
  switch (local_E4) {
    case 5:
      script_14E30(&local_F4, -0.1736f, 1.8164f, 0.245f);
      break;
    case 6:
      script_14E30(&local_F4, 0.1076f, 1.8164f, 0.245f);
      break;
    case 7:
      script_14E30(&local_F4, 0.3888f, 1.8164f, 0.245f);
      break;
  }
  Script_GameUtils(SHEN_NAME('TITL'), GC_ConfigureNumericDisplay, 0x100, &local_E8, 0x5F5E0FF, 0);
  Script_GameUtils(SHEN_NAME('TITL'), GC_StageNumericDisplayValue, arg0, &local_F4, 1.0f, 0);
  return;
}

static void script_18F06(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  int32_t local_E0;
  script_14E30(&local_E8, 0.4f, 0.4f, 0.4f);
  script_14E30(&local_F4, 0.0f, 1.39f, 0.7155f);
  local_E0 = arg0;
  switch (local_E0) {
    case 0:
      local_F4.x = 0.0f;
      local_E4 = 292;
      break;
    case 1:
      local_F4.x = 0.275f;
      local_E4 = 293;
      break;
    case 2:
      local_F4.x = -0.275f;
      local_E4 = 294;
      break;
  }
  Script_GameUtils(SHEN_NAME('TITL'), GC_StageNodeData, local_E4, &local_F4, 1.0f, &local_E8);
  return;
}

static int32_t script_18FA4(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FC = 0;
  local_F8 = 0;
  local_F4 = 0;
  local_F4 = Script_PlayerStatsUtils(PS_GetDartsModePlayCountSlot, 10);
  Script_PlayerStatsUtils(PS_SetDartsModePlayCountSlot, 10, local_F4 + 1);
  for (;;) {
    local_F0 = task_2C2;
    switch (local_F0) {
      case 1:
        local_FC = local_FC + 1;
        break;
      case 2:
        goto L01900A;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L01900A:;
  local_F8 = Script_PlayerStatsUtils(PS_GetDartsModeActiveFramesSlot, 10);
  Script_PlayerStatsUtils(PS_SetDartsModeActiveFramesSlot, 10, local_F8 + local_FC);
  return Script_srchControl(0);
}

static void script_1902D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  *(int32_t *)arg0 = *(int32_t *)arg0 + arg1;
  *(int32_t *)(arg0 + 4) = *(int32_t *)(arg0 + 4) + arg2;
  *(int32_t *)(arg0 + 8) = *(int32_t *)(arg0 + 8) + arg3;
  return;
}

static void script_1905A(int32_t arg0, float arg1, float arg2, float arg3) {
  *(int32_t *)arg0 = *(int32_t *)arg0 + (int32_t)(arg1 / 360.0f * (float)0x10000);
  *(int32_t *)(arg0 + 4) = *(int32_t *)(arg0 + 4) + (int32_t)(arg2 / 360.0f * (float)0x10000);
  *(int32_t *)(arg0 + 8) = *(int32_t *)(arg0 + 8) + (int32_t)(arg3 / 360.0f * (float)0x10000);
  return;
}

static void script_190B1(Vector3i *arg0, float arg1, float arg2, float arg3, ...) {
  arg0->x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  arg0->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  arg0->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  return;
}

static int32_t script_190F6(int32_t arg0) {
  int8_t local_FF;
  for (;;) {
    local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 6.0f));
    if (local_FF != task_2C1) {
      task_2C1 = local_FF;
      break;
    }
  }
  switch (arg0) {
    case 1:
      {
        uint8_t unused_FC[3];
        int32_t local_F8;
        local_F8 = local_FF;
        switch (local_F8) {
          case 0:
            script_14D72(7350, SHEN_NAME('TITL'), 0);
            break;
          case 1:
            script_14D72(7351, SHEN_NAME('TITL'), 0);
            break;
          case 2:
            script_14D72(7352, SHEN_NAME('TITL'), 0);
            break;
          case 3:
            script_14D72(7353, SHEN_NAME('TITL'), 0);
            break;
          case 4:
            script_14D72(7354, SHEN_NAME('TITL'), 0);
            break;
          case 5:
            script_14D72(7355, SHEN_NAME('TITL'), 0);
            break;
        }
        break;
      }
    case 3:
      {
        uint8_t unused_FC[3];
        int32_t local_F8;
        local_F8 = local_FF;
        switch (local_F8) {
          case 0:
            script_14D72(7360, SHEN_NAME('TITL'), 0);
            break;
          case 1:
            script_14D72(7361, SHEN_NAME('TITL'), 0);
            break;
          case 2:
            script_14D72(7362, SHEN_NAME('TITL'), 0);
            break;
          case 3:
            script_14D72(7363, SHEN_NAME('TITL'), 0);
            break;
          case 4:
            script_14D72(7364, SHEN_NAME('TITL'), 0);
            break;
          case 5:
            script_14D72(7365, SHEN_NAME('TITL'), 0);
            break;
        }
        break;
      }
    case 5:
      {
        uint8_t unused_FC[3];
        int32_t local_F8;
        local_F8 = local_FF;
        switch (local_F8) {
          case 0:
            script_14D72(7370, SHEN_NAME('TITL'), 0);
            break;
          case 1:
            script_14D72(7371, SHEN_NAME('TITL'), 0);
            break;
          case 2:
            script_14D72(7372, SHEN_NAME('TITL'), 0);
            break;
          case 3:
            script_14D72(7373, SHEN_NAME('TITL'), 0);
            break;
          case 4:
            script_14D72(7374, SHEN_NAME('TITL'), 0);
            break;
          case 5:
            script_14D72(7375, SHEN_NAME('TITL'), 0);
            break;
        }
        break;
      }
    case 4:
      {
        uint8_t unused_FC[3];
        int32_t local_F8;
        local_F8 = local_FF;
        switch (local_F8) {
          case 0:
            script_14D72(7380, SHEN_NAME('TITL'), 0);
            break;
          case 1:
            script_14D72(7381, SHEN_NAME('TITL'), 0);
            break;
          case 2:
            script_14D72(7382, SHEN_NAME('TITL'), 0);
            break;
          case 3:
            script_14D72(7384, SHEN_NAME('TITL'), 0);
            break;
          case 4:
            script_14D72(7383, SHEN_NAME('TITL'), 0);
            break;
          case 5:
            script_14D72(7385, SHEN_NAME('TITL'), 0);
            break;
        }
        break;
      }
    case 7:
      {
        uint8_t unused_FC[3];
        int32_t local_F8;
        local_F8 = local_FF;
        switch (local_F8) {
          case 0:
            script_14D72(7390, CHARA_AKIR, 0);
            break;
          case 1:
            script_14D72(7391, CHARA_AKIR, 0);
            break;
          case 2:
            script_14D72(7392, CHARA_AKIR, 0);
            break;
          case 3:
            script_14D72(7390, CHARA_AKIR, 0);
            break;
          case 4:
            script_14D72(7391, CHARA_AKIR, 0);
            break;
          case 5:
            script_14D72(7392, CHARA_AKIR, 0);
            break;
        }
        break;
      }
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_203D8)))
static void script_194ED(void) {
  int32_t unused_E8[6];
  task_1F8 = 4;
  script_EE07();
  Script_LoadVoiceFile(1, "E1027");
  Script_IncrementQTEMinigameRand16(0, 8);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_203C8);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_motmToggleFlags(CHARA_AKIR, 2, 4);
  Script_SetCameraFlags(0);
  Script_uiUtils(0, 27, 0);
  Script_charaDisplay(SHEN_NAME('YENS'), CDM_Hide);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  task_2C0 = 1;
  return;
}

__attribute__((scn_string_bindings(0, data_203F8, 2, data_20416, 3, data_20423)))
static void script_19574(void) {
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_203E0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_203EC, 0);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_LoadVoiceFile(0, "E1027");
  Script_SetCameraFlags(1);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  if (task_294 > Script_GetHighScoreValue(Highscore_Unknown5)) {
    Script_SetHighScoreValue(Highscore_Unknown5, task_294);
  }
  Script_SetHighScoreValue(Highscore_Unknown6, task_208);
  Script_SetHighScoreValue(Highscore_Unknown7, task_20C);
  Script_sprintf("--------TITLE---------\012");
  Script_sprintf("HI_SCORE\011%d\012", Script_GetHighScoreValue(Highscore_Unknown5));
  Script_sprintf("----------------------\012");
  script_EE43();
  Script_LoadSND(0, 2, "FREE", 0);
  Script_ryoControl(RYO_RestoreControl);
  task_1F8 = 0;
  return;
}

static void script_196A3(int32_t arg0) {
  if (arg0 == 1) {
    task_2CC = Script_ryoSleepUtils(3, 0, 4);
    task_2CD = Script_ryoSleepUtils(3, 0, 5);
    task_2CF = Script_ryoSleepUtils(3, 0, 6);
  } else {
    Script_ryoSleepUtils(3, 1, 4, task_2CC);
    Script_ryoSleepUtils(3, 1, 5, task_2CD);
    Script_ryoSleepUtils(3, 1, 6, task_2CF);
  }
  return;
}

static void script_1970F(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_E4[6];
  if (arg0 == 3) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_SLEEP_TRANSITION_MODAL_ACTIVE, 1);
  } else {
    if (arg0 != 3) {
      for (;;) {
        if (Script_ryoControl(PCCTL_QueryShouldRun)) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      goto L01974B;
    }
  }
  L01974B:;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_2CE = arg0;
  if (task_2CE == 3) {
    task_2CD = 16;
  } else {
    task_2CD = 13;
  }
  local_FC = Script_scenCreateTask(script_198EE, 0);
  if (task_2CE == 3) {
    Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  script_1A28A();
  script_199FC();
  if (task_2CE == 3) {
    task_2CC = 10;
  } else {
    task_2CC = 1;
  }
  script_197FF();
  Script_srchControl((void *)local_FC);
  if (arg0 == 3) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_SLEEP_TRANSITION_MODAL_ACTIVE, 0);
  }
  SCNF_WaitFrames_0000(1);
  script_1A330();
  script_196A3(0);
  return;
}

static void script_197FF(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_2CC;
    switch (local_FC) {
      case 1:
        script_19AF1();
        break;
      case 2:
        script_19B84();
        break;
      case 3:
        script_19C40(0);
        break;
      case 4:
        script_19C40(1);
        break;
      case 5:
        script_19C40(2);
        break;
      case 6:
        script_19C40(3);
        break;
      case 7:
        script_19FA2();
        break;
      case 8:
        script_19CF4();
        break;
      case 9:
        script_19FFA();
        break;
      case 10:
        script_1A0A2();
        break;
      case 11:
        script_1A1B9();
        break;
      case 12:
        goto L0198ED;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0198ED:;
  return;
}

static int32_t script_198EE(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_2CD;
    switch (local_FC) {
      case 13:
        script_14D72(7400, SHEN_NAME('SPDM'), 0);
        task_2CD = 0;
        break;
      case 14:
        if (task_2CE == 0) {
          script_14D72(7411, SHEN_NAME('SPDM'), 0);
        } else {
          script_14D72(7410, SHEN_NAME('SPDM'), 0);
        }
        task_2CD = 0;
        break;
      case 15:
        if (task_2CE == 0) {
          script_14D72(7421, SHEN_NAME('SPDM'), 0);
        } else {
          script_14D72(7420, SHEN_NAME('SPDM'), 0);
        }
        task_2CD = 0;
        break;
      case 16:
        if (task_2CE == 0) {
          script_14D72(7431, SHEN_NAME('SPDM'), 0);
        } else {
          script_14D72(7430, SHEN_NAME('SPDM'), 0);
        }
        task_2CD = 0;
        break;
      case 17:
        goto L0199FB;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0199FB:;
}

static void script_199FC(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  switch (task_2CE) {
    case 1:
      script_14E30(&local_F4, 0.6024f, 0.0f, 0.17f);
      script_190B1(&local_E8, 0.0f, 90.0f, 0.0f);
      break;
    case 0:
      script_14E30(&local_F4, -0.6024f, 0.0f, 0.17f);
      script_190B1(&local_E8, 0.0f, 270.0f, 0.0f);
      break;
    case 3:
      script_14E30(&local_F4, 0.0f, 0.0f, 0.4415f);
      script_190B1(&local_E8, 0.0f, 0.0f, 0.0f);
      break;
  }
  script_18900(SHEN_NAME('SPDM'), CHARA_AKIR, &local_F4, (int32_t)&local_E8);
  if (task_2CE != 3) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_20440)))
static void script_19AF1(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Script_EVT_GetPosition(SHEN_NAME('SPDM'), -1, &local_F4, 0);
  script_14E4B(&local_F4, 0.0f, 0.8f, -0.5f);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_F4, 1);
  script_148EC(2, 1, "E1027A001", "E1027A023", 0, 0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_InitializeOsag(CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  task_2CD = 14;
  SCNF_WaitFrames_0000(20);
  task_2CF = 0;
  task_2CC = 2;
  return;
}

static void script_19B84(void) {
  int32_t local_FC;
  int8_t local_FB;
  int32_t local_F4;
  (void)(local_FC == 34);
  local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 36, 47, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) == 0)) {
    task_2CC = 6;
  } else if (local_FC == 36) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -2, 0, 0);
    local_FB = script_008F(0, 100, 1);
    local_F4 = local_FB;
    if (local_F4 == 0) {
      task_2CC = 8;
    } else if (local_F4 == 1) {
      task_2CC = 7;
    } else {
      task_2CC = 12;
    }
  } else {
    task_2CC = 5;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_20454, 1, data_2045E, 2, data_20468, 3, data_20472, 4, data_2047C, 5, data_20486, 6, data_20490)))
static void script_19C40(int32_t arg0) {
  Script_SetCharaLookAtPosition(CHARA_AKIR, -2, 0, 0);
  Script_executeAicaCommand(4512, 4, -10);
  switch (arg0) {
    case 0:
      script_148CA(1, "E1027A026");
      break;
    case 1:
      script_148CA(1, "E1027A027");
      break;
    case 2:
      script_148EC(3, 1, "E1027A009", "E1027A010", "E1027A011", 0, 0);
      break;
    case 3:
      script_148EC(2, 1, "E1027A009", "E1027A010", 0, 0, 0);
      break;
  }
  Script_executeAicaCommand(4512, 4, 10);
  task_2CC = 12;
  return;
}

__attribute__((scn_string_bindings(0, data_2049A, 1, data_204A4, 2, data_204AE)))
static void script_19CF4(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  local_FC = 0;
  local_F8 = 0;
  local_F4 = 900;
  local_FC = Script_shenGetStats(STATS_CASH);
  if (Script_StepUtils(GET_STEP_HIGHER, 0) >= 11) {
    if (local_FC < 1220) {
      local_F4 = (local_FC - 320) / 100 * 100;
    }
  } else {
    if (local_FC < 900) {
      local_F4 = local_FC / 100 * 100;
    }
  }
  local_F8 = SCNF_StartCashOverlaySelector(local_F4, 0, 0, 0);
  Script_SetStats(STATS_CASH, local_FC - local_F8);
  task_2CF = local_F8 / 100;
  if (local_F8 == 0) {
    script_19C40(2);
  } else {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -2, 0, 0);
    switch (task_2CE) {
      case 1:
        script_14E30(&local_E8, 0.712f, 0.0f, 0.385f);
        break;
      case 0:
        script_14E30(&local_E8, -0.691f, 0.0f, 0.384f);
        break;
    }
    script_190B1(&local_DC, 0.0f, 0.0f, 0.0f);
    script_18900(SHEN_NAME('SPDM'), CHARA_AKIR, &local_E8, (int32_t)&local_DC);
    Script_SetNewHandMotion(CHARA_AKIR, 1, 773, 0, 0, 0, 0, 1.0f, 0);
    Script_SetNewHandMotion(CHARA_AKIR, 0, 773, 0, 0, 0, 0, 1.0f, 0);
    task_2CD = 15;
    SCNF_WaitFrames_0000(1);
    switch (task_2CE) {
      case 1:
        script_14D22(CHARA_AKIR, 30977, 0);
        break;
      case 0:
        script_14D22(CHARA_AKIR, 30978, 0);
        break;
    }
    SCNF_WaitFrames_0000(10);
    script_148EC(3, 0, "E1027A003", "E1027A004", "E1027A005", 0, 0);
    script_14E30(&local_E8, 0.125f, 0.012f, -0.078f);
    script_190B1(&local_DC, 0.0f, 0.0f, 90.0f);
    Script_fixoAttachEx(SHEN_NAME('YENS'), CHARA_AKIR, 18, &local_E8, &local_DC);
    for (;;) {
      if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)30}).i) {
        Script_executeAicaCommand(66985, 0, 0);
        Script_charaDisplay(SHEN_NAME('YENS'), CDM_Show);
      } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)35}).i) {
        Script_SetNewHandMotion(CHARA_AKIR, 1, 771, 0, 0, 0, 0, 1.0f, 0);
      } else {
        if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)150}).i) {
          Script_SetNewHandMotion(CHARA_AKIR, 0, 775, 0, 0, 0, 0, 1.0f, 60);
          break;
          goto L019F8F;
        }
      }
      L019F8F:;
      SCNF_WaitFrames_0000(1);
    }
    task_2CC = 9;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_204B8, 1, data_204C2, 2, data_204CC)))
static void script_19FA2(void) {
  Script_SetCharaLookAtPosition(CHARA_AKIR, -2, 0, 0);
  Script_executeAicaCommand(4512, 4, -10);
  Script_scenCreateTask(script_14E78, 0);
  script_148EC(3, 1, "E1027A006", "E1027A007", "E1027A008", 0, 0);
  Script_executeAicaCommand(4512, 4, 10);
  task_2CC = 12;
  return;
}

static void script_19FFA(void) {
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 1, 64);
  Script_executeAicaCommand(2720, 2, 64);
  Script_executeAicaCommand(2720, 3, 64);
  Script_executeAicaCommand(65696, 0, 0);
  Script_charaDisplay(SHEN_NAME('YENS'), CDM_Hide);
  Script_fixoClearAttachment(SHEN_NAME('YENS'));
  Script_SetNewHandMotion(CHARA_AKIR, 1, 774, 0, 0, 0, 0, 1.0f, 0);
  Script_SetEmulatorFlags(0, task_2CF);
  MODU_Script_shenRequestGameModule(Harrier, 3, 0x54434744, 20);
  return;
}

static void script_1A0A2(void) {
  int32_t local_FC;
  int32_t local_F8;
  task_2CD = 16;
  Script_SetNewHandMotion(CHARA_AKIR, 1, 771, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 0, 774, 0, 0, 0, 0, 1.0f, 60);
  if (task_2CE == 0) {
    Script_motmToggleFlags(CHARA_AKIR, 1, 0x80);
  }
  script_1A276();
  if (task_2CE == 0) {
    Script_motmToggleFlags(CHARA_AKIR, 2, 0x80);
  }
  if (task_2CE == 3) {
    Script_cdttLoadScrUtils(1, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  if (task_2CE == 3) {
    local_FC = Script_SetEmulatorFlags(1);
    if (local_FC > 0) {
      local_F8 = Script_shenGetStats(STATS_CASH);
      local_F8 = local_F8 + local_FC * 100;
      SCNF_SetCashValue(local_FC * 100, 1);
      Script_SetStats(STATS_CASH, local_F8);
    }
  }
  script_14D58(CHARA_AKIR);
  task_2CC = 11;
  return;
}

static void script_1A1B9(void) {
  if (task_2CE == 3) {
    if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 967) == 0) & -(Script_SetEmulatorFlags(3) != 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 938) == 0) & -(Script_SetEmulatorFlags(2) != 0)) {
      script_D2CB(9);
    } else if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 967) == 0) & (Script_SetEmulatorFlags(3) != 0)) {
      script_D2CB(7);
    } else {
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 938) == 0) & (Script_SetEmulatorFlags(2) != 0)) {
        script_D2CB(5);
        goto L01A25A;
      }
    }
  }
  L01A25A:;
  task_2CC = 12;
  return;
}

static void script_1A262(void) {
  script_14D22(CHARA_AKIR, 30982, 0);
  return;
}

static void script_1A276(void) {
  script_14D22(CHARA_AKIR, 31003, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_204E8)))
static void script_1A28A(void) {
  int32_t unused_E8[6];
  task_1F8 = 4;
  script_EE07();
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_204D8);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_motmToggleFlags(CHARA_AKIR, 2, 4);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_LoadVoiceFile(1, "E1027");
  Script_SetCameraFlags(0);
  Script_uiUtils(0, 52, 0);
  Script_charaDisplay(SHEN_NAME('YENS'), CDM_Hide);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_204EE)))
static void script_1A330(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3i local_DC;
  if (task_2CE == 0) {
    script_14E30(&local_F4, 1.68f, 0.0f, -6.59f);
    script_190B1(&local_DC, 0.0f, 270.0f, 0.0f);
  } else if (task_2CE == 1) {
    script_14E30(&local_F4, 3.42f, 0.0f, -6.68f);
    script_190B1(&local_DC, 0.0f, 90.0f, 0.0f);
  } else {
    script_14E30(&local_F4, 3.42f, 0.0f, -6.68f);
    script_190B1(&local_DC, 0.0f, 270.0f, 0.0f);
  }
  script_EE43();
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_LoadVoiceFile(0, "E1027");
  Script_SetCameraFlags(1);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_DC, 0);
  Script_InitializeOsag(CHARA_AKIR);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(5);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ryoControl(RYO_RestoreControl);
  task_1F8 = 0;
  return;
}

static void script_1A4BB(int32_t arg0) {
  if (arg0 == 1) {
    task_2D0 = Script_ryoSleepUtils(3, 0, 0);
    task_2D1 = Script_ryoSleepUtils(3, 0, 1);
    task_2D2 = Script_ryoSleepUtils(3, 0, 2);
    task_2D3 = Script_ryoSleepUtils(3, 0, 3);
  } else {
    Script_ryoSleepUtils(3, 1, 0, task_2D0);
    Script_ryoSleepUtils(3, 1, 1, task_2D1);
    Script_ryoSleepUtils(3, 1, 2, task_2D2);
    Script_ryoSleepUtils(3, 1, 3, task_2D3);
  }
  return;
}

static void script_1A546(int32_t arg0) {
  int32_t unused_E8[6];
  if (arg0 != 1) {
    for (;;) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  } else {
    if (arg0 == 1) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_SLEEP_TRANSITION_MODAL_ACTIVE, 1);
      goto L01A582;
    }
  }
  L01A582:;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  if (arg0 == 0) {
    task_2D2 = 13;
  }
  task_2D1 = arg0;
  Script_scenCreateTask(script_1A720, 0);
  if (task_2D1 == 1) {
    Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  script_1A7DA(task_2D1);
  script_1AF7D();
  switch (task_2D1) {
    case 1:
      task_2D0 = 10;
      break;
    case 0:
      task_2D0 = 1;
      break;
  }
  script_1A631();
  task_2D2 = 17;
  if (arg0 == 1) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_SLEEP_TRANSITION_MODAL_ACTIVE, 0);
  }
  script_1B023();
  script_1A4BB(0);
  return;
}

static void script_1A631(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_2D0;
    switch (local_FC) {
      case 1:
        script_1A8B9();
        break;
      case 2:
        script_1A93E();
        break;
      case 3:
        script_1A9FA(0);
        break;
      case 4:
        script_1A9FA(1);
        break;
      case 5:
        script_1A9FA(2);
        break;
      case 6:
        script_1A9FA(3);
        break;
      case 7:
        script_1AC97();
        break;
      case 8:
        script_1AAD1();
        break;
      case 9:
        script_1ACEF();
        break;
      case 10:
        script_1AD6C();
        break;
      case 11:
        script_1AECF();
        break;
      case 12:
        goto L01A71F;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L01A71F:;
  return;
}

static int32_t script_1A720(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_2D2;
    switch (local_FC) {
      case 13:
        script_14D72(7500, SHEN_NAME('HODM'), 0);
        task_2D2 = 0;
        break;
      case 14:
        script_14D72(7510, SHEN_NAME('HODM'), 0);
        task_2D2 = 0;
        break;
      case 15:
        script_14D72(7520, SHEN_NAME('HODM'), 0);
        task_2D2 = 0;
        break;
      case 16:
        script_14D72(7530, SHEN_NAME('HODM'), 0);
        task_2D2 = 0;
        break;
      case 17:
        goto L01A7D3;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L01A7D3:;
  return Script_srchControl(0);
}

static void script_1A7DA(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  switch (task_2D1) {
    case 1:
      script_14E30(&local_F4, 0.1036f, 0.015f, -0.339f);
      script_190B1(&local_E8, 0.0f, 90.0f, 0.0f);
      break;
    case 0:
      script_14E30(&local_F4, 0.6024f, 0.015f, 0.17f);
      script_190B1(&local_E8, 0.0f, 90.0f, 0.0f);
      break;
  }
  script_18900(SHEN_NAME('HODM'), CHARA_AKIR, &local_F4, (int32_t)&local_E8);
  if (task_2D1 == 0) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  }
  task_2D3 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_204F4)))
static void script_1A8B9(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  script_148EC(2, 0, "E1027A001", "E1027A024", 0, 0, 0);
  SCNF_WaitFrames_0000(80);
  Script_EVT_GetPosition(SHEN_NAME('HODM'), -1, &local_F4, 0);
  script_14E4B(&local_F4, -0.5f, 1.114f, 0.279f);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_F4, 1);
  SCNF_WaitFrames_0000(40);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  task_2D2 = 14;
  SCNF_WaitFrames_0000(20);
  task_2D0 = 2;
  return;
}

static void script_1A93E(void) {
  int32_t local_FC;
  int8_t local_FB;
  int32_t local_F4;
  (void)(local_FC == 34);
  local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 36, 47, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) == 0)) {
    task_2D0 = 6;
  } else if (local_FC == 36) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -2, 0, 0);
    local_FB = script_008F(0, 100, 1);
    local_F4 = local_FB;
    if (local_F4 == 0) {
      task_2D0 = 8;
    } else if (local_F4 == 1) {
      task_2D0 = 7;
    } else {
      task_2D0 = 12;
    }
  } else {
    task_2D0 = 5;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_20508, 1, data_20512, 2, data_2051C, 3, data_20526, 4, data_20530, 5, data_2053A, 6, data_20544, 7, data_2054E)))
static void script_1A9FA(int32_t arg0) {
  Script_SetCharaLookAtPosition(CHARA_AKIR, -2, 0, 0);
  Script_executeAicaCommand(4512, 4, -10);
  switch (arg0) {
    case 0:
      script_148CA(1, "E1027A026");
      break;
    case 1:
      script_148CA(1, "E1027A027");
      break;
    case 2:
      script_148EC(3, 1, "E1027A009", "E1027A010", "E1027A011", 0, 0);
      break;
    case 3:
      script_148EC(2, 1, "E1027A009", "E1027A010", 0, 0, 0);
      break;
    case 4:
      script_148CA(1, "E1027A010");
      break;
  }
  Script_executeAicaCommand(4512, 4, 10);
  if (arg0 != 4) {
    task_2D0 = 12;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_20558, 1, data_20562, 2, data_2056C)))
static void script_1AAD1(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  local_FC = 0;
  local_F8 = 0;
  local_F4 = 900;
  script_14E30(&local_E8, 0.671f, 0.0f, 0.543f);
  script_190B1(&local_DC, 0.0f, 90.0f, 0.0f);
  script_18900(SHEN_NAME('HODM'), CHARA_AKIR, &local_E8, (int32_t)&local_DC);
  task_2D2 = 15;
  script_148EC(3, 0, "E1027A003", "E1027A004", "E1027A005", 0, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 773, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 0, 773, 0, 0, 0, 0, 1.0f, 0);
  script_14D22(CHARA_AKIR, 30993, 0);
  for (;;) {
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)70}).i) {
      Script_SetNewHandMotion(CHARA_AKIR, 1, 774, 0, 0, 0, 0, 1.0f, 0);
      Script_SetNewHandMotion(CHARA_AKIR, 0, 774, 0, 0, 0, 0, 1.0f, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_14D58(CHARA_AKIR);
  local_FC = Script_shenGetStats(STATS_CASH);
  if (Script_StepUtils(GET_STEP_HIGHER, 0) >= 11) {
    if (local_FC < 1220) {
      local_F4 = (local_FC - 320) / 100 * 100;
    }
  } else {
    if (local_FC < 900) {
      local_F4 = local_FC / 100 * 100;
    }
  }
  local_F8 = SCNF_StartCashOverlaySelector(local_F4, 0, 0, 0);
  Script_SetStats(STATS_CASH, local_FC - local_F8);
  task_2D3 = local_F8 / 100;
  if (local_F8 == 0) {
    script_1A9FA(4);
    task_2D0 = 10;
  } else {
    task_2D0 = 9;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_20576, 1, data_20580, 2, data_2058A)))
static void script_1AC97(void) {
  Script_SetCharaLookAtPosition(CHARA_AKIR, -2, 0, 0);
  Script_executeAicaCommand(4512, 4, -10);
  Script_scenCreateTask(script_14E78, 0);
  script_148EC(3, 1, "E1027A006", "E1027A007", "E1027A008", 0, 0);
  Script_executeAicaCommand(4512, 4, 10);
  task_2D0 = 12;
  return;
}

static void script_1ACEF(void) {
  Script_executeAicaCommand(2720, 1, 64);
  Script_executeAicaCommand(2720, 2, 64);
  Script_executeAicaCommand(2720, 3, 64);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(65696, 0, 0);
  Script_SetEmulatorFlags(0, task_2D3);
  MODU_Script_shenRequestGameModule(HangOn, 3, 0x54434744, 10);
  task_2D0 = 12;
  return;
}

static void script_1AD6C(void) {
  script_1AD7B();
  task_2D0 = 11;
  return;
}

static void script_1AD7B(void) {
  int32_t local_FC;
  int32_t local_F8;
  task_2D2 = 16;
  script_14D22(CHARA_AKIR, 30994, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 776, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 0, 776, 0, 0, 0, 0, 1.0f, 0);
  if (task_2D1 == 1) {
    Script_cdttLoadScrUtils(1, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  for (;;) {
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR)}).i == ((scn3_bits){.f = (float)80}).i) {
      Script_SetNewHandMotion(CHARA_AKIR, 1, 773, 0, 0, 0, 0, 1.0f, 0);
      Script_SetNewHandMotion(CHARA_AKIR, 0, 773, 0, 0, 0, 0, 1.0f, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  if (task_2D1 == 1) {
    local_FC = Script_SetEmulatorFlags(1);
    if (local_FC > 0) {
      local_F8 = Script_shenGetStats(STATS_CASH);
      local_F8 = local_F8 + local_FC * 100;
      SCNF_SetCashValue(local_FC * 100, 1);
      Script_SetStats(STATS_CASH, local_F8);
    }
  }
  script_14D58(CHARA_AKIR);
  return;
}

static void script_1AECF(void) {
  if (task_2D1 == 1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Show);
    if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 968) == 0) & Script_SetEmulatorFlags(3) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 939) == 0) & -(Script_SetEmulatorFlags(2) != 0)) {
      script_D2CB(8);
    } else if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 968) == 0) & Script_SetEmulatorFlags(3)) {
      script_D2CB(6);
    } else {
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 939) == 0) & (Script_SetEmulatorFlags(2) != 0)) {
        script_D2CB(4);
        goto L01AF75;
      }
    }
  }
  L01AF75:;
  task_2D0 = 12;
  return;
}

__attribute__((scn_string_bindings(0, data_205A4)))
static void script_1AF7D(void) {
  int32_t unused_E8[6];
  task_1F8 = 4;
  script_EE07();
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_20594);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_motmToggleFlags(CHARA_AKIR, 2, 4);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_LoadVoiceFile(1, "E1027");
  Script_SetCameraFlags(0);
  Script_uiUtils(0, 53, 0);
  Script_charaDisplay(SHEN_NAME('YENS'), CDM_Hide);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
  return;
}

static void script_1B023(void) {
  script_EE43();
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_LoadVoiceFile(0, data_205C4);
  Script_SetCameraFlags(1);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_205AC);
  Script_InitializeOsag(CHARA_AKIR);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(5);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ryoControl(RYO_RestoreControl);
  task_1F8 = 0;
  return;
}

static void script_1B113(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_1B13B(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_1B18F(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_1B1DF(int32_t arg0) {
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

static void script_1B286(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_1B1DF(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_1B2A9(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_1B2DC(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_1B2A9(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_1B2FF(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_1B2A9(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_1B35E(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_1B373(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_1B709(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

__attribute__((scn_string_bindings(0, data_205DC)))
static void script_1BD97(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_205F0)))
static void script_1BDA6(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_1BDB5(void) {
  return;
}

static void script_1BDB8(void) {
  return;
}

static void script_1BDBB(void) {
  return;
}

static void script_1BDBE(void) {
  return;
}

static void script_1BDC1(void) {
  (void)((task_2E0 = 0) & 0x20);
  return;
}

static void script_1BDCE(void) {
  task_2D4 = Script_freeTalkGetCurrentChara();
  return;
}

static void script_1BDDA(void) {
  *(&task_2D4 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_1BDE9(void) {
  return *(&task_2D4 + 2) = Script_freeTalkGetAndResetState();
}

static int32_t script_1BDF8(void) {
  int8_t local_FF;
  int32_t local_F8;
  script_1BD97();
  for (;;) {
    local_FF = script_1BDE9();
    local_F8 = local_FF;
    if (local_FF != -1) {
      script_1BDC1();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1BE29(int32_t arg0, const char *arg1) {
  int32_t local_FC;
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_LoadVoiceFile(1, arg1);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      script_D2CB(1);
      break;
    case 1:
      script_D2CB(2);
      break;
    case 2:
      script_D2CB(3);
      break;
    case 3:
      script_D2CB(4);
      break;
    case 4:
      script_D2CB(5);
      break;
    case 5:
      script_D2CB(6);
      break;
    case 6:
      script_D2CB(7);
      break;
    case 7:
      script_D2CB(8);
      break;
    case 8:
      script_D2CB(9);
      break;
  }
  Script_LoadVoiceFile(0, arg1);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  return;
}

__attribute__((scn_string_bindings(1, data_20611, 2, data_20617, 3, data_2061D, 4, data_20623, 5, data_20629, 6, data_2062F, 7, data_20635, 8, data_2063B, 9, data_20641)))
static void script_1BF4A(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  task_214 = Script_LoadHandMot(3, SHEN_NAME('DGCT'), "HMOT_MIN.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  local_FC = Script_GetCurrentEntryPoint();
  script_EEFD(0);
  Script_scenCreateTask(script_ED11, 0);
  Script_scenCreateTask(script_ED4E, 0);
  Script_scenCreateTask(script_EDD3, 0);
  if (local_FC != 0) {
    Script_ccowUtils(CHARA_AKIR, 2);
  }
  local_F8 = local_FC;
  switch (local_F8) {
    case 11:
      script_1BE29(0, "E1025");
      break;
    case 12:
      script_1BE29(1, "E1026");
      break;
    case 13:
      script_1BE29(2, "E1027");
      break;
    case 14:
      script_1BE29(3, "E1027");
      break;
    case 15:
      script_1BE29(4, "E1027");
      break;
    case 16:
      script_1BE29(5, "E1027");
      break;
    case 17:
      script_1BE29(6, "E1027");
      break;
    case 18:
      script_1BE29(7, "E1027");
      break;
    case 19:
      script_1BE29(8, "E1027");
      break;
  }
  return;
}

__attribute__((scn_string_bindings(3, data_2066D, 4, data_20673, 5, data_20679, 6, data_2067F)))
void script_1C0AF(void) {
  int32_t local_FC;
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  task_1FA = 0;
  task_1F8 = 0;
  Script_SetMapFlags(0, 0);
  Script_uiUtils(0, 37, 0);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 37);
  task_1FA = Script_ryoSleepUtils(0);
  script_A98A(CHARA_AKIR, (DSWKDoorEntry *)&data_1EC68, (int32_t *)data_1ECA0, (DoorResourceDescriptor *)&data_1ECB4, (MapObjectRecord *)&data_1ECC4, 1, 0);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  Script_mapmSetRenderFlag(1, 0);
  Script_motiLoad(3, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/03/D000", (int32_t)"CYCLEMAN.BIN", 0x54434744);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 6.0016f, 0.7321f, -0.5389f, 0.6345f, -0.3203f, -8.3725f, 5.7614f, -8.3778f, -0.4f, 3.0f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetGlobalSceneSlotValue34(2, 1);
  Script_SetupLightFromTable(0);
  if (~Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 3, "MPK00")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  }
  script_1BF4A();
  if (~Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 3, "MPK00")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_LoadHandMot(Script_GetSceneID(), SHEN_NAME('DGCT'), "HMOT_JBX.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_0599(2, 2);
  Script_PREP_Related(-1, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_PREP_Related(2, 0);
  Script_scenCreateTask(script_1C829, 1, CHARA_AKIR);
  Script_scenCreateTask(script_1BDF8, 0);
  Script_scenCreateTask(script_29B6, 0);
  Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x30303044, 6, (const void *)-1, 959, 6);
  if (task_1FA != 0) {
    script_1A4BB(1);
    script_196A3(1);
    Script_ryoSleepUtils(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(1, 0);
  if (task_1FA == 0) {
    local_FC = Script_GetCurrentEntryPoint();
    if (local_FC == 10) {
      script_1A546(1);
    } else if (local_FC == 20) {
      script_1970F(3);
    } else {
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
    }
  } else {
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ryoControl(RYO_RestoreControl);
  }
  Script_shenClearFreeState(1);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ryoSleepUtils(2, 1);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenCreateTask(script_1C45D, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_20692)))
static void script_1C3DE(int32_t arg0, const char *arg1) {
  for (;;) {
    if (arg0 != 0) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
        break;
      }
    }
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/DGCT/", arg1) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      if (arg0 == 0) {
        break;
      }
    }
  }
  return;
}

static int32_t script_1C45D(void) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  int8_t local_FB;
  uint8_t unused_E0[27];
  int8_t local_DF;
  int8_t local_DE;
  uint8_t unused_DC[2];
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3i local_B8;
  int32_t unused_A8[4];
  GameDateTime local_A0;
  local_FF = 0;
  local_FB = 0;
  local_FC = -1;
  local_FD = 0;
  local_C4.x = 0.0f;
  (&local_C4)->y = 0.0f;
  (&local_C4)->z = 0.0f;
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    local_DF = 23;
    local_DE = 59;
  } else {
    local_DF = 23;
    local_DE = 29;
  }
  for (;;) {
    local_FE = 0;
    Script_ReturnTimeDate(&local_A0);
    if (((&local_A0)->hours >= local_DF) & ((&local_A0)->minutes >= local_DE)) {
      if (local_FB == 1) {
        Script_sicnUtils(1, 0, 0);
        Script_uiUtils(0, 37, 0);
        local_FB = 0;
      }
      for (;;) {
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_B8, 0x42000000);
    local_D0.x = (&local_D0)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_B8)->y) * 0.3f;
    (&local_D0)->z = (&local_D0)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_B8)->y) * 0.3f;
    local_FD = local_FD % 8;
    if (local_FB == 0) {
      *(int32_t *)&local_C4.x = Script_ScenDataUtil(&data_206AC, 5, local_FD * 2);
      *(int32_t *)&(&local_C4)->z = Script_ScenDataUtil(&data_206AC, 5, local_FD * 2 + 1);
    }
    if (Script_IsPlayerAllowed(0)) {
      if (Script_CheckTargetWithinAngleRange(&local_D0, 1.4f, (&local_B8)->y, 7281, &local_C4) != 0) {
        local_FE = 1;
        if (local_FB == 0) {
          int32_t local_9C;
          local_FB = 1;
          Script_sicnUtils(0, 5, 1);
          local_9C = local_FD;
          if ((local_9C == 0) | (local_9C == 1)) {
            Script_uiUtils(0, 47, 0);
          } else if (local_9C == 2) {
            Script_uiUtils(0, 27, 0);
          } else if (local_9C == 3) {
            Script_uiUtils(0, 4, 0);
          } else if ((local_9C == 4) | (local_9C == 5)) {
            Script_uiUtils(0, 52, 0);
          } else {
            if ((local_9C == 6) | (local_9C == 7)) {
              Script_uiUtils(0, 53, 0);
              goto L01C691;
            }
          }
          L01C691:;
          local_FC = local_FD;
        }
      }
    }
    if (local_FE == 0) {
      local_FD = local_FD + 1;
      if (local_FB == 1) {
        Script_sicnUtils(1, 0, 0);
        Script_uiUtils(0, 37, 0);
        local_FB = 0;
      }
    } else {
      if (Script_GetControllerInput(1) == 0x400) {
        int32_t local_9C;
        if (local_FB == 1) {
          Script_sicnUtils(1, 0, 0);
          local_FB = 0;
        }
        local_9C = local_FD;
        if ((local_9C == 0) | (local_9C == 1)) {
          script_F2D7(*(&data_206A4 + local_FD));
        } else if (local_9C == 2) {
          script_1542F();
        } else if (local_9C == 3) {
          script_12A86();
        } else if ((local_9C == 4) | (local_9C == 5)) {
          Vector3 local_90;
          int8_t local_8F;
          if (local_FD == 4) {
            *(int32_t *)&local_90.x = Script_ScenDataUtil(&data_206AC, 5, 10);
            (&local_90)->y = 0.0f;
            *(int32_t *)&(&local_90)->z = Script_ScenDataUtil(&data_206AC, 5, 11);
            {
              local_8F = 5;
            }
          } else {
            *(int32_t *)&local_90.x = Script_ScenDataUtil(&data_206AC, 5, 8);
            (&local_90)->y = 0.0f;
            *(int32_t *)&(&local_90)->z = Script_ScenDataUtil(&data_206AC, 5, 9);
            {
              local_8F = 4;
            }
          }
          if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_D0, &local_C4) <= Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_D0, &local_90))) {
            local_FD = local_8F;
          }
          script_1970F(local_FD - 4);
        } else {
          if ((local_9C == 6) | (local_9C == 7)) {
            script_1A546(0);
            goto L01C81D;
          }
        }
      }
    }
    L01C81D:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1C829(int32_t arg0) {
  script_C9C5(arg0);
  if (Script_GetCurrentEntryPoint() == 1) {
    Vector3 local_F4;
    local_F4.x = 2.8f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -1.35f;
    Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, 0, MN_AKI_AKI_WALK_LP, 0);
  }
  if (task_1FA != 0) {
    script_1C90E();
  }
  for (;;) {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    if (!((&local_F4)->z <= 0.2f)) {
      Script_shenClearFreeState(0);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(5, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenRequestGameModuleTransition(3, SHEN_NAME('D000'), 6);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1C90E(void) {
  Vector3 local_F4;
  if (Script_GetCurrentEntryPoint() == 0) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_206EC, &data_206F8, 0)) {
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      (&local_F4)->z = -0.85f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F4);
    }
  }
  return;
}
