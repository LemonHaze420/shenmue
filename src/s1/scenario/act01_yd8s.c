#include "act01_yd8s.h"

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
  Script_ccowUtils(arg0, CCOW_DisableAll);
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
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_012B(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
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
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
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
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
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
