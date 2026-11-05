#include "act80_gmct.h"

static void script_0030(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_set_val_to_1(3, 0);
  return;
}

static void script_0041(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00006D;
    }
  }
  L00006D:;
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

static void script_00DB(int32_t arg0) {
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

static void script_012B(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0159(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0187(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01BB(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01EF(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_020D(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_0247(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_027B(void) {
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

static void script_0373(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static int32_t script_03A3(void) {
  for (;;) {
    task_6C = task_6C + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_03BC(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  return;
}

static void script_03EB(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return;
}

static void script_0474(void) {
  Script_SetupLightFromTable(3);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_EIIU, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 4);
  Script_SetShadowFlags(CHARA_EIIU, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 4);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 4);
  Script_SetVector4ByOffset(&data_1648);
  Script_refbSetActive(1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1648);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_STSI, &data_1648);
  Script_charaSetRefboardFlagTo(CHARA_STSI, 1);
  Script_SetCharaRefBoardPos(CHARA_EIIU, &data_1648);
  Script_charaSetRefboardFlagTo(CHARA_EIIU, 1);
  return;
}

static void script_053F(void) {
  script_03EB(CHARA_AKIR);
  script_03EB(CHARA_EIIU);
  script_03BC(CHARA_STSI);
  Script_SetMotnFlag(CHARA_STSI, -1);
  Script_motmToggleFlags(CHARA_STSI, SCEN_Disable, 0x40);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  script_0474();
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_1564, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_1564, 10);
  Script_handSetFloatsFor(CHARA_EIIU, 0, data_1564, 10);
  Script_handSetFloatsFor(CHARA_EIIU, 1, data_1564, 10);
  Script_mapmSetRenderFlag(1, 0);
  return;
}

static int32_t script_05E0(void) {
  for (;;) {
    {
      int32_t unused_F4[3];
      int32_t local_F0;
      local_F0 = task_6C;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 8);
          break;
        case 550:
          Script_FACE(CHARA_AKIR, 2, 0, 8);
          break;
        case 601:
          Script_FACE(CHARA_AKIR, 5, 0, 40);
          break;
        case 641:
          Script_FACE(CHARA_AKIR, 2, 0, 100);
          break;
        case 741:
          Script_FACE(CHARA_AKIR, 0, 0, 40);
          break;
      }
    }
    {
      int32_t unused_F4[3];
      int32_t local_F0;
      local_F0 = task_6C;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_EIIU, 3, 4, 8);
          break;
        case 171:
          Script_FACE(CHARA_EIIU, 7, 0, 8);
          break;
        case 191:
          Script_FACE(CHARA_EIIU, 7, 5, 8);
          break;
        case 276:
          Script_FACE(CHARA_EIIU, 4, 0, 55);
          break;
        case 441:
          Script_FACE(CHARA_EIIU, 0, 0, 20);
          break;
        case 861:
          Script_FACE(CHARA_EIIU, 10, 0, 8);
          break;
        case 931:
          Script_FACE(CHARA_EIIU, 0, 0, 40);
          break;
        case 971:
          Script_FACE(CHARA_EIIU, 7, 0, 110);
          break;
        case 1311:
          Script_FACE(CHARA_EIIU, 1, 0, 70);
          break;
        case 1381:
          Script_FACE(CHARA_EIIU, 9, 0, 8);
          break;
      }
    }
    {
      int32_t unused_F4[3];
      int32_t local_F0;
      local_F0 = task_6C;
      if (local_F0 == 1391) {
        Script_handSetFloatsFor(CHARA_EIIU, 1, data_0AB4, 15);
        goto L0007C7;
      }
    }
    L0007C7:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

void script_07D3(void) {
  Script_scenCreateTask(script_07E6, 1, CHARA_AKIR);
  return;
}

static int32_t script_07E6(int32_t arg0) {
  int32_t unused_FC;
  int32_t local_F8;
  Script_ryoSetChara(arg0);
  while (1) {
    local_F8 = Script_GetCurrentEntryPoint();
    switch (local_F8) {
      case 0:
        script_087D(0);
        break;
      case 1:
        script_087D(1);
        break;
      case 98:
        script_087D(0);
        Script_shenRequestGameModuleTransition(80, SHEN_NAME('SENM'), 90);
        break;
      case 99:
        script_087D(1);
        Script_shenRequestGameModuleTransition(80, SHEN_NAME('SENM'), 90);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  end:;
}

__attribute__((scn_string_bindings(0, data_1658, 4, data_1682)))
static void script_087D(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Script_LoadVoiceFile(1, "01YUK");
  local_F4 = Script_motiLoad(80, SHEN_NAME('GMCT'), "M_01YUK.BIN");
  Script_LoadSND(0, 3, "amb024.snd", 0);
  Script_LoadSND(0, 4, "a1_yuka4.snd", 0);
  Script_executeAicaCommand(24232, 0, 0);
  Script_executeAicaCommand(1184, 3, 127);
  script_053F();
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_6C = 0;
  local_FC = Script_scenCreateTask(script_05E0, 0);
  local_F8 = Script_scenCreateTask(script_03A3, 0);
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_set_val_to_1(2, 0);
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  Script_ReallocateUNFO(local_F4);
  Script_LoadVoiceFile(0, "01YUK");
  return;
}
