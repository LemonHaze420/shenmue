#include "act80_yuk1.h"

static int32_t script_0030(void) {
  for (;;) {
    Script_ClearAccumulator_00FE(1, 0x3F000000, 10, 2, 2);
    Script_ClearAccumulator_00FE(3, &data_0814, 8, 9);
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
  Script_set_val_to_1(3, 0);
  return;
}

static void script_0097(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L0000C3;
    }
  }
  L0000C3:;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 5, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_0131(int32_t arg0) {
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

static int32_t script_0181(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_01AF(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01DD(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0211(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0245(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0263(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_029D(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_02D1(void) {
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

static void script_03C9(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_03F9(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_0086();
  script_055A();
  local_FC = 0;
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 181:
        Script_FACE(CHARA_AKIR, 1, 0, 16);
        break;
      case 281:
        Script_FACE(CHARA_AKIR, 2, 0, 16);
        break;
      case 351:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 641:
        Script_FACE(CHARA_AKIR, 2, 0, 16);
        break;
      case 781:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 1390:
        Script_handSetFloatsFor(CHARA_TOM_, 1, data_0F7C, 8);
        break;
      case 1434:
        Script_handSetFloatsFor(CHARA_TOM_, 1, data_0E98, 16);
        break;
      case 1661:
        Script_FACE(CHARA_AKIR, 1, 0, 16);
        break;
      case 1811:
        Script_FACE(CHARA_AKIR, 2, 0, 16);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_055A(void) {
  int32_t unused_F4[3];
  script_0097(CHARA_AKIR);
  script_0097(CHARA_TOM_);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_TOM_, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_TOM_, 1, 0);
  script_0602(CHARA_AKIR);
  script_0602(CHARA_TOM_);
  Script_SetMaxInterpSpeed(0.4f);
  Script_motmToggleFlags(CHARA_TOM_, SCEN_Disable, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  return;
}

static void script_05E7(void) {
  script_0131(CHARA_AKIR);
  script_0131(CHARA_TOM_);
  return;
}

static void script_0602(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_0E98, 8);
  Script_handSetFloatsFor(arg0, 0, data_0E98, 8);
  return;
}

static void script_063D(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

void script_0667(void) {
  Script_scenCreateTask(script_067A, 1, 0x52494B41);
  return;
}

static int32_t script_067A(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  local_FC = Script_GetCurrentEntryPoint();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_0780();
  Script_AuthorUtils(0, 0, "SEQDATA1.BIN", "SCENE/80/YUK1");
  Script_LoadSND(0, 4, "a1_yuka3.snd", 0);
  Script_LoadVoiceFile(1, "01YUK");
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(60);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (1) {
    local_F8 = Script_motiLoad(80, SHEN_NAME('YUK1'), "M_01YUK.BIN");
    Script_scenCreateTask(script_0181, 0);
    script_03F9(0);
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(60);
    Script_ReallocateUNFO(local_F8);
    script_05E7();
    Script_SetGameStateTo(AUTH_None);
  }
  end:;
}

static void script_0780(void) {
  int32_t unused_F4[3];
  Script_shenSetLightingMode(1);
  Script_lightSetGlobalSceneLighting(&data_142C);
  Script_lightDirLightEnable(0, &data_144C);
  Script_LightUtils(0, 2, &data_143C);
  Script_LightUtils(0, 4, 1.0f);
  Script_lightSetSlotEnabled(0, 1);
  Script_refbSetActive(1);
  Script_SetVector4ByOffset(&data_1458);
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 40.0f);
  return;
}
