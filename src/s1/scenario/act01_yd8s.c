#include "act01_yd8s.h"

static int32_t script_0030(void);
static void script_0060(void);
static void script_0086(void);
static void script_008F(void);
static void script_009E(int32_t arg0);
static void script_012B(int32_t arg0);
static int32_t script_017B(void);
static void script_01A9(int32_t arg0);
static void script_01D7(void);
static void script_020B(int32_t arg0);
static void script_023F(int32_t arg0);
static void script_025D(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0297(int32_t arg0, int32_t arg1);
static void script_02CB(void);
static void script_03C3(int32_t arg0, int32_t arg1);
static void script_03F3(int32_t arg0);
static void script_04CB(void);
static void script_067C(void);
static void script_072F(int32_t arg0);
static void script_076A(int32_t arg0);
void script_0794(void);
static int32_t script_07A7(int32_t arg0);
static void script_08ED(void);
static void script_095F(void);

static const float data_0B24[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_0B30[4] = {16777215, 16777215, 16777215, 16777215};
static const uint32_t data_0B40 = 0xC3D48000;
static const float data_0B44[2] = {-1.8f, -210.0f};
static const char data_0B4C[] __attribute__((scn_string_literal)) = "scene/02/model";
static const char data_0B5B[] __attribute__((scn_string_literal)) = "C1305S3G.MT5";
static const char data_0B68[] __attribute__((scn_string_literal)) = "C1305S1G.MT5";
static const uint8_t pad_0B75[5] = {0};
static const uint8_t data_0B7A[16] = {
  0xA0, 0x41, 0x00, 0x00, 0x48, 0x44, 0x00, 0x00, 0x80, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};
static const uint8_t pad_0B8A[226] = {0};
static const int32_t data_0C6C[342] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
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
static const int32_t data_11C4[342] = {
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
static const Vector4 data_171C = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_172C[13] __attribute__((scn_string_literal)) = "SEQDATA2.BIN";
static const char data_1739[14] __attribute__((scn_string_literal)) = "SCENE/01/YD8S";
static const char data_1747[13] __attribute__((scn_string_literal)) = "a1_sokos.snd";
static const char data_1754[6] __attribute__((scn_string_literal)) = "01GAR";
static const char data_175A[12] __attribute__((scn_string_literal)) = "M_01GAR.BIN";
static const char data_1766[6] __attribute__((scn_string_literal)) = "01GAR";
static const FogColourData data_176C = {.red = 20.0f, .green = 775.0f, .blue = 0.36f, .colour = 0xFFFFFFFFU};
static const uint32_t data_177C[4] = {0, 0, 0xECFFFFFFU, 0xECFFFFFFU};
static const Vector4 data_178C = {.x = 0.3f, .y = 0.3f, .z = 0.27f, .w = 0.3f};
static const Vector4 data_179C = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
static const Vector3 data_17AC = {.x = 0.5f, .y = -0.82f, .z = 0.67f};
static const char data_17B8[14] __attribute__((scn_string_literal)) = "0  01_GAR_2.c";
static const char data_17C6[13] __attribute__((scn_string_literal)) = "SEQDATA2.BIN";
static const char data_17D3[14] __attribute__((scn_string_literal)) = "SCENE/01/YD8S";
static const char data_17E1[13] __attribute__((scn_string_literal)) = "a1_sokos.snd";
static const char data_17EE[6] __attribute__((scn_string_literal)) = "01GAR";
static const char data_17F4[12] __attribute__((scn_string_literal)) = "M_01GAR.BIN";
static const char data_1800[8] __attribute__((scn_string_literal)) = "01GAR";

static int32_t script_0030(void) {
  for (;;) {
    Script_ClearAccumulator_00FE(1, 0x3F000000, 10, 2, 2);
    Script_ClearAccumulator_00FE(3, &data_0B40, 8, 9);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0060(void) {
  Script_ClearAccumulator_00FE(0, "scene/02/model", "C1305S3G.MT5", "C1305S1G.MT5", 1);
  Script_scenCreateTask(script_0030, 0);
  return;
}

static void script_0086(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_008F(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_009E(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L0000CA;
    }
  }
  L0000CA:;
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

static void script_012B(int32_t arg0) {
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

static int32_t script_017B(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_01A9(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01D7(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_020B(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_023F(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_025D(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_0297(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_02CB(void) {
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

static void script_03C3(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_03F3(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_0086();
  script_04CB();
  Script_shenSeqUtils(arg0);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 723:
        Script_scenToggleActiveFor(CHARA_AKIR, 1);
        break;
      case 740:
        Script_FACE(CHARA_AKIR, 1, 0, 8);
        break;
      case 760:
        Script_FACE(CHARA_AKIR, 2, 0, 8);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_0C6C, 8);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_0C6C, 8);
        break;
      case 799:
        script_076A(CHARA_AKIR);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_04CB(void) {
  script_009E(CHARA_AKIR);
  script_009E(CHARA_NGSM);
  script_009E(CHARA_KEBJ);
  script_009E(CHARA_KEBK);
  script_009E(CHARA_GORO);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_NGSM, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KEBJ, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KEBK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_GORO, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_NGSM, -1);
  Script_SetMotnFlag(CHARA_GORO, -1);
  Script_SetMotnFlag(CHARA_KEBJ, -1);
  Script_SetMotnFlag(CHARA_KEBK, -1);
  Script_SetMaxInterpSpeed(0.4f);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  script_072F(CHARA_AKIR);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_171C);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_NGSM, &data_171C);
  Script_charaSetRefboardFlagTo(CHARA_NGSM, 1);
  Script_SetCharaRefBoardPos(CHARA_KEBJ, &data_171C);
  Script_charaSetRefboardFlagTo(CHARA_KEBJ, 1);
  Script_SetCharaRefBoardPos(CHARA_KEBK, &data_171C);
  Script_charaSetRefboardFlagTo(CHARA_KEBK, 1);
  Script_SetCharaRefBoardPos(CHARA_GORO, &data_171C);
  Script_charaSetRefboardFlagTo(CHARA_GORO, 1);
  Script_SetShadowRange(30.0f);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_NGSM, 0);
  Script_SetCharaFlags(CHARA_KEBJ, 0);
  Script_SetCharaFlags(CHARA_KEBK, 0);
  Script_SetCharaFlags(CHARA_GORO, 0);
  return;
}

static void script_067C(void) {
  script_076A(CHARA_AKIR);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_FACE(CHARA_AKIR, 0, 0, 1);
  script_012B(CHARA_AKIR);
  script_012B(CHARA_NGSM);
  script_012B(CHARA_KEBJ);
  script_012B(CHARA_KEBK);
  script_012B(CHARA_GORO);
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetShadowRange(20.0f);
  return;
}

static void script_072F(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_11C4, 8);
  Script_handSetFloatsFor(arg0, 0, data_11C4, 8);
  return;
}

static void script_076A(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

void script_0794(void) {
  Script_scenCreateTask(script_07A7, 1, 0x52494B41);
  return;
}

__attribute__((scn_string_bindings(0, data_172C, 1, data_1739, 2, data_1747, 3, data_1754, 4, data_175A, 5, data_1766)))
static int32_t script_07A7(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_GetCurrentEntryPoint();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_08ED();
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(60);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (1) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 0:
        Script_AuthorUtils(0, 0, "SEQDATA2.BIN", "SCENE/01/YD8S");
        Script_LoadSND(0, 4, "a1_sokos.snd", 0);
        Script_LoadVoiceFile(1, "01GAR");
        local_F8 = Script_motiLoad(1, SHEN_NAME('YD8S'), "M_01GAR.BIN");
        Script_scenCreateTask(script_017B, 0);
        script_03F3(0);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        SCNF_WaitFrames_0000(30);
        Script_LoadVoiceFile(0, "01GAR");
        Script_AuthorUtils(AUTH_FREE, 0);
        Script_ReallocateUNFO(local_F8);
        script_067C();
        Script_SetGameStateTo(AUTH_None);
        break;
      case 99:
        script_095F();
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_08ED(void) {
  Script_SetFogColour(&data_176C);
  Script_shenSetLightingMode(1);
  Script_lightSetGlobalSceneLighting(&data_178C);
  Script_lightDirLightEnable(0, &data_17AC);
  Script_LightUtils(0, 2, &data_179C);
  Script_LightUtils(0, 4, 1.0f);
  Script_SetFogFlags(0, data_177C);
  Script_lightSetSlotEnabled(0, 1);
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
  return;
}

__attribute__((scn_string_bindings(1, data_17C6, 2, data_17D3, 3, data_17E1, 4, data_17EE, 5, data_17F4, 6, data_1800)))
static void script_095F(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (1) {
    Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_GetControllerInput(0) != 0x400) {
      int32_t local_F4;
      if (Script_GetControllerInput(1) == 64) {
        local_FC = local_FC - 1;
      }
      if (Script_GetControllerInput(1) == 128) {
        local_FC = local_FC + 1;
      }
      if (local_FC > 0) {
        local_FC = 0;
      }
      if (local_FC < 0) {
        local_FC = 0;
      }
      local_F4 = local_FC;
      if (local_F4 == 0) {
        Script_PrintDebugString(3, 25, "0  01_GAR_2.c");
        goto L000A13;
      }
      L000A13:;
      SCNF_WaitFrames_0000(1);
    }
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(30);
    {
      int32_t local_F4;
      local_F4 = local_FC;
      if (local_F4 == 0) {
        Script_AuthorUtils(0, 0, "SEQDATA2.BIN", "SCENE/01/YD8S");
        Script_LoadSND(0, 4, "a1_sokos.snd", 0);
        Script_LoadVoiceFile(1, "01GAR");
        local_F8 = Script_motiLoad(1, SHEN_NAME('YD8S'), "M_01GAR.BIN");
        Script_scenCreateTask(script_017B, 0);
        script_03F3(0);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        SCNF_WaitFrames_0000(30);
        Script_LoadVoiceFile(0, "01GAR");
        Script_AuthorUtils(AUTH_FREE, 0);
        Script_ReallocateUNFO(local_F8);
        script_067C();
        Script_SetGameStateTo(AUTH_None);
        goto L000B16;
      }
    }
    L000B16:;
    SCNF_WaitFrames_0000(1);
  }
  return;
}
