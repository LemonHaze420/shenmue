#include "act03_ydma.h"

static int32_t script_0030(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = Script_GetInputData(0, 0) & 0xF7FF;
    if (local_FC & 0x400) {
      if (Script_ProcessDialogueData(2) == 0) {
        Script_ProcessDialogueData(0);
      }
      local_FC = local_FC & 0xFBFF;
    } else {
      if (Script_ProcessDialogueData(2)) {
        Script_ProcessDialogueData(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0091(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_009A(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_00A9(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L0000D5;
    }
  }
  L0000D5:;
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

static void script_0136(int32_t arg0) {
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

static int32_t script_0186(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_01B4(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01E2(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0216(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_024A(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0268(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_02A2(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_02D6(void) {
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

static void script_03CE(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_03FE(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  int32_t unused_DC;
  int32_t local_D8;
  script_0091();
  script_0609();
  script_06ED(CHARA_AKIR, SHEN_NAME('ABA1'));
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_2ACC);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2574, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2904, 0);
  local_EC.x = -0.216f;
  (&local_EC)->y = 0.339f;
  (&local_EC)->z = -0.072f;
  local_E0.x = 3183;
  (&local_E0)->y = 53887;
  (&local_E0)->z = 48590;
  Script_fixoAttachEx(SHEN_NAME('ABA1'), CHARA_AKIR, 18, &local_EC, &local_E0);
  Script_shenSeqUtils(arg0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_D8 = local_FC;
    switch (local_D8) {
      case 0:
        Script_FACE(CHARA_AKIR, 3, 0, 1);
        local_E0.x = 3458;
        (&local_E0)->y = 5279;
        (&local_E0)->z = 0;
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E0);
        break;
      case 135:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2820, 30);
        break;
      case 152:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2820, 30);
        break;
      case 230:
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 6, 0);
        break;
      case 433:
        Script_FACE(CHARA_AKIR, 2, 0, 20);
        break;
      case 730:
        goto L0005F5;
        break;
    }
    if ((local_FC > 150) & (local_FC < 213)) {
      if (local_FC & 1) {
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2658, 20);
      } else {
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2820, 20);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L0005F5:;
  Script_fixoClearAttachment(SHEN_NAME('ABA1'));
  Script_SetRyoMotionDetails(CHARA_AKIR);
  return;
}

static void script_0609(void) {
  script_00A9(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_mapmSetCamFlags(0, 1);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_SetVector4ByOffset(&data_2AF0);
  (void)1;
  return;
}

static void script_06A3(void) {
  script_0136(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_SetGameStateTo(AUTH_None);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_mapmSetCamFlags(0, 1);
  return;
}

static void script_06ED(int32_t arg0, int32_t arg1) {
  Script_ccowUtils(arg1, CCOW_DisableAll);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_RequestFightAnimation(arg0, 657, 0.0f, 14, 1);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, 18, &data_2B00, &data_2B0C);
  Script_CharHandlers(arg0, 0, 0, 16);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_charaDisplay(arg1, CDM_Show);
  return;
}

static void script_0751(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  script_0091();
  script_0EAB();
  Script_ccowSetHeightMode(SHEN_NAME('ABAG'), CCOW_KeepHeight);
  Script_scenCreateTask(script_0186, 0);
  Script_shenSeqUtils(0);
  local_F4 = Script_scenCreateTask(script_0F9A, 0);
  local_FC = 0;
  local_F8 = 127;
  while (Script_shenSeqUtils(-1)) {
    local_D8 = local_FC;
    switch (local_D8) {
      case 0:
        Script_motmGetCharaFlags(CHARA_AKIR, MOTM_PoseNone, SCEN_Write);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
        Script_mapmSetCamFlags(0, 0);
        Script_scenToggleActiveFor(SHEN_NAME('FUTO'), 0);
        Script_FACE(CHARA_AKIR, 10, 0, 1);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2C0C, 10);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2C0C, 10);
        local_DC.x = 546;
        (&local_DC)->y = 1638;
        (&local_DC)->z = -1274;
        Script_SetHandRootRotation(CHARA_FUKU, 0, 0x15, &local_DC);
        local_DC.x = 546;
        (&local_DC)->y = -1638;
        (&local_DC)->z = -1820;
        Script_SetHandRootRotation(CHARA_FUKU, 1, 0x15, &local_DC);
        Script_motmSetBlendFrames(CHARA_FUKU, MOTM_BLEND_FRAMES_10);
        Script_charaDisplay(CHARA_INE_, CDM_Hide);
        Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        Script_charaDisplay(CHARA_FUKU, CDM_Hide);
        break;
      case 3:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2B28, 0);
        local_DC.x = -2366;
        (&local_DC)->y = -7827;
        (&local_DC)->z = 364;
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_DC);
        Script_charaDisplay(SHEN_NAME('ABAG'), CDM_Show);
        break;
      case 150:
        Script_mapmSetCamFlags(0, 1);
        Script_charaDisplay(CHARA_AKIR, CDM_Show);
        Script_charaDisplay(CHARA_FUKU, CDM_Show);
        break;
      case 230:
        local_E8.x = 0.198f;
        (&local_E8)->y = 0.012f;
        (&local_E8)->z = -0.061f;
        local_DC.x = 47331;
        (&local_DC)->y = 15109;
        (&local_DC)->z = 63715;
        Script_fixoAttachEx(SHEN_NAME('FUTO'), CHARA_FUKU, 18, &local_E8, &local_DC);
        Script_charaDisplay(SHEN_NAME('FUTO'), CDM_Show);
        break;
      case 240:
        Script_FACE(CHARA_AKIR, 7, 0, 7);
        break;
      case 288:
        Script_FACE(CHARA_FUKU, 8, 5, 10);
        break;
      case 330:
        break;
      case 370:
        Script_FACE(CHARA_FUKU, 3, 5, 10);
        break;
      case 390:
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2DD4, 10);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2CF0, 10);
        break;
      case 542:
        local_DC.x = 546;
        (&local_DC)->y = 910;
        (&local_DC)->z = -182;
        Script_SetHandRootRotation(CHARA_FUKU, 0, 0x15, &local_DC);
        break;
      case 630:
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        break;
      case 649:
        Script_fixoClearAttachment(SHEN_NAME('FUTO'));
        local_E8.x = 0.148f;
        (&local_E8)->y = 0.067f;
        (&local_E8)->z = 0.08f;
        local_DC.x = 2733;
        (&local_DC)->y = 8648;
        (&local_DC)->z = 33226;
        Script_fixoAttachEx(SHEN_NAME('FUTO'), CHARA_AKIR, 12, &local_E8, &local_DC);
        Script_charaDisplay(SHEN_NAME('FUTO'), CDM_Show);
        Script_FACE(CHARA_AKIR, 3, 4, 9);
        Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
        Script_SetNewHandMotion(CHARA_AKIR, 0, 611, 0, 0, 0, 0, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        while (Script_GetHandControlBit3(CHARA_AKIR, 0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 700:
        Script_ResetHandMotion(CHARA_FUKU, 0, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 1, 0);
        break;
      case 780:
        local_DC.x = 0;
        (&local_DC)->y = 0;
        (&local_DC)->z = 0;
        Script_SetHandRootRotation(CHARA_FUKU, 1, 0x15, &local_DC);
        local_DC.x = 0;
        (&local_DC)->y = 0;
        (&local_DC)->z = 0;
        Script_SetHandRootRotation(CHARA_FUKU, 0, 0x15, &local_DC);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2C0C, 10);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2C0C, 10);
        break;
      case 1307:
        Script_mapmSetCamFlags(0, 0);
        break;
      case 1308:
        Script_charaDisplay(SHEN_NAME('ABAG'), CDM_Hide);
        break;
      case 1383:
        Script_charaDisplay(SHEN_NAME('ABAG'), CDM_Show);
        break;
      case 1384:
        Script_mapmSetCamFlags(0, 1);
        break;
      case 1590:
        Script_FACE(CHARA_AKIR, 2, 0, 10);
        break;
      case 1563:
        Script_mapmSetCamFlags(0, 0);
        break;
      case 1659:
        Script_mapmSetCamFlags(0, 1);
        break;
      case 1730:
        Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        break;
      case 1750:
        Script_charaDisplay(CHARA_INE_, CDM_Show);
        Script_ResetHandMotion(CHARA_INE_, 1, 0, 0);
        Script_ResetHandMotion(CHARA_INE_, 1, 1, 0);
        local_DC.x = 2002;
        (&local_DC)->y = 546;
        (&local_DC)->z = 182;
        Script_SetHandRootRotation(CHARA_INE_, 1, 0x15, &local_DC);
        local_DC.x = -1274;
        (&local_DC)->y = -546;
        (&local_DC)->z = 364;
        Script_SetHandRootRotation(CHARA_INE_, 0, 0x15, &local_DC);
        Script_handSetFloatsFor(CHARA_INE_, 0, data_2EB8, 10);
        Script_handSetFloatsFor(CHARA_INE_, 1, data_2EB8, 10);
        break;
      case 1775:
        Script_charaDisplay(SHEN_NAME('FUTO'), CDM_Hide);
        break;
      case 1877:
        Script_FACE_LCG(CHARA_INE_, FACE_BlinkClose, 15, 0);
        break;
      case 2135:
        Script_FACE_LCG(CHARA_INE_, FACE_BlinkOpen, 15, 0);
        break;
      case 2185:
        Script_FACE_LCG(CHARA_INE_, FACE_BlinkClose, 2, 0);
        break;
      case 2187:
        Script_FACE_LCG(CHARA_INE_, FACE_BlinkOpen, 2, 0);
        break;
      case 2268:
        goto L000E52;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L000E52:;
  Script_executeAicaCommand(2720, 2, 64);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_F4);
  SCNF_WaitFrames_0000(1);
  Script_fixoClearAttachment(SHEN_NAME('FUTO'));
  script_0F57();
  Script_shenSeqUtils(-6);
  return;
}

static void script_0EAB(void) {
  script_00A9(CHARA_AKIR);
  script_00A9(CHARA_FUKU);
  script_00A9(CHARA_INE_);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_FUKU, 1, 0);
  Script_facemotUtils(CHARA_INE_, 1, 0);
  Script_SetVector4ByOffset(&data_2F9C);
  (void)1;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2F9C);
  Script_charaSetRefboardFlagTo(CHARA_FUKU, 1);
  Script_SetCharaRefBoardPos(CHARA_FUKU, &data_2F9C);
  Script_charaSetRefboardFlagTo(CHARA_INE_, 1);
  Script_SetCharaRefBoardPos(CHARA_INE_, &data_2F9C);
  return;
}

static void script_0F57(void) {
  script_0136(CHARA_AKIR);
  script_0136(CHARA_FUKU);
  script_0136(CHARA_INE_);
  Script_SetGameStateTo(AUTH_None);
  Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  Script_charaDisplay(CHARA_INE_, CDM_Hide);
  return;
}

static int32_t script_0F9A(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (local_FC == 150) {
      Script_tmnmUtils(SHEN_NAME('ABAG'), 5659, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    }
    if (local_FC == 650) {
      Script_tmnmUtils(SHEN_NAME('ABAG'), 5660, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    }
    if (local_FC == 1411) {
      Script_tmnmUtils(SHEN_NAME('ABAG'), 5658, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

void script_102C(void) {
  Script_scenCreateTask(script_103A, 0);
  return;
}

__attribute__((scn_string_bindings(2, data_2FC1, 3, data_2FCC, 4, data_2FD1, 5, data_2FDE, 6, data_2FEB, 7, data_2FF0, 8, data_2FF5, 9, data_2FFB, 10, data_3006, 11, data_3013, 12, data_301F, 14, data_3030, 15, data_3035, 16, data_303A, 17, data_303F)))
static int32_t script_103A(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FC = Script_GetCurrentEntryPoint();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/YDMA/", "YDMA");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  script_13AB();
  while (1) {
    if (local_FC == 99) {
    } else {
      if (Script_GetSceneID() == 99) {
        Script_scenCreateTask(script_0030, 0);
      }
      script_13AB();
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_LoadSND_Wrapper(0, 0, "bgm107.snd", "FREE", "a1_kstab.snd", "a1_tabid.snd", "FREE", "FREE");
      Script_LoadVoiceFile(1, "A0180");
      local_F4 = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_0180.BIN");
      local_F0 = Script_LoadHandMot(3, SHEN_NAME('YDMA'), "HMOT0156.BIN");
      local_F8 = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_01152.BIN");
      Script_executeAicaCommand(16296, 0, 0);
      Script_executeAicaCommand(1184, 2, 95);
      script_0751();
      Script_executeAicaCommand(0x1200A0, 2, 0);
      Script_LoadVoiceFile(0, "A0180");
      SCNF_WaitFrames_0000(1);
      script_14CB();
      SCNF_WaitFrames_0000(1);
      Script_LoadSND_Wrapper(0, 0, "bgm071.snd", "FREE", "FREE", "FREE", "FREE", data_3044);
      Script_executeAicaCommand(19880, 0, 0);
      Script_executeAicaCommand(1184, 2, 105);
      Script_scenCreateTask(script_0186, 0);
      script_03FE(1);
      Script_executeAicaCommand(2720, 2, 60);
      Script_filmCheckOrSetTransitionUtil(80, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenSeqUtils(-4);
      Script_executeAicaCommand(0x1200A0, 2, 0);
      SCNF_WaitFrames_0000(60);
      Script_ReallocateUNFO(local_F8);
      Script_ReallocateUNFO(local_F4);
      Script_FreeTaskData(local_F0);
      script_06A3();
      Script_SetGameStateTo(AUTH_None);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 937, 1);
      if (Script_GetSceneID() == 99) {
        Script_shenRequestGameModuleTransition(99, SHEN_NAME('YDMA'), 0);
        SCNF_WaitFrames_0000(1);
      } else {
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('DXMS'), 0);
        SCNF_WaitFrames_0000(1);
      }
    }
  }
  end:;
}

static void script_12D3(void) {
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(0, 1);
    Script_mapmSetRenderFlag(1, 1);
    Script_mapmSetRenderFlag(2, 0);
    Script_mapmSetRenderFlag(3, 1);
    Script_mapmSetRenderFlag(4, 0);
    Script_mapmSetRenderFlag(5, 1);
    Script_mapmSetRenderFlag(6, 1);
    Script_mapmSetRenderFlag(7, 1);
    Script_mapmSetRenderFlag(8, 1);
    Script_mapmSetRenderFlag(9, 0);
    Script_mapmSetRenderFlag(10, 1);
  } else {
    Script_mapmSetRenderFlag(0, 1);
    Script_mapmSetRenderFlag(1, 1);
    Script_mapmSetRenderFlag(2, 1);
    Script_mapmSetRenderFlag(3, 0);
    Script_mapmSetRenderFlag(4, 1);
    Script_mapmSetRenderFlag(5, 0);
    Script_mapmSetRenderFlag(6, 0);
    Script_mapmSetRenderFlag(7, 1);
    Script_mapmSetRenderFlag(8, 1);
    Script_mapmSetRenderFlag(9, 1);
    Script_mapmSetRenderFlag(10, 0);
  }
  return;
}

static void script_13AB(void) {
  Vector4 local_F0;
  script_12D3();
  Script_mapmSetCamFlags(0, 0);
  Script_SetDynamicMapTransform(0, &data_305C, 0);
  Script_SetDynamicMapTransform(1, &data_305C, 0);
  Script_SetDynamicMapTransform(2, &data_305C, 0);
  Script_SetDynamicMapTransform(3, &data_305C, 0);
  Script_SetDynamicMapTransform(4, &data_305C, 0);
  Script_SetDynamicMapTransform(5, &data_305C, 0);
  Script_SetDynamicMapTransform(6, &data_305C, 0);
  Script_SetDynamicMapTransform(7, &data_305C, 0);
  Script_SetDynamicMapTransform(8, &data_305C, 0);
  Script_SetDynamicMapTransform(9, &data_305C, 0);
  Script_SetDynamicMapTransform(10, &data_305C, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  local_F0.x = 1.0f;
  (&local_F0)->y = 0.2f;
  (&local_F0)->z = 0.2f;
  (&local_F0)->w = 0.2f;
  Script_SetVector4ByOffset(&local_F0);
  (void)1;
  Script_EVT_SetPosition(SHEN_NAME('RDR2'), POS_XYZ, &data_3068);
  Script_EVT_SetRotation(SHEN_NAME('RDR2'), 0x38000000, &data_3074, 0);
  return;
}

static void script_14CB(void) {
  Script_mapmSetCamFlags(0, 0);
  Script_SetDynamicMapTransform(0, &data_3098, 0);
  Script_SetDynamicMapTransform(1, &data_3098, 0);
  Script_SetDynamicMapTransform(2, &data_3098, 0);
  Script_SetDynamicMapTransform(3, &data_3098, 0);
  Script_SetDynamicMapTransform(4, &data_3098, 0);
  Script_SetDynamicMapTransform(5, &data_3098, 0);
  Script_SetDynamicMapTransform(6, &data_3098, 0);
  Script_SetDynamicMapTransform(7, &data_3080, 22022);
  Script_SetDynamicMapTransform(8, &data_3080, 22022);
  Script_SetDynamicMapTransform(9, &data_3080, 22022);
  Script_SetDynamicMapTransform(10, &data_3080, 22022);
  Script_mapmSetRenderFlag(9, 0);
  Script_mapmSetRenderFlag(10, 0);
  Script_EVT_SetPosition(SHEN_NAME('RDR2'), POS_XYZ, &data_308C);
  Script_EVT_SetRotation(SHEN_NAME('RDR2'), 0x38000000, &data_30A4, 0);
  return;
}

__attribute__((scn_string_bindings(3, data_30BF, 4, data_30CB, 5, data_30D6, 6, data_30E3, 7, data_30EE, 8, data_30FB, 9, data_3101, 10, data_310C, 11, data_3119, 12, data_311F, 13, data_312A, 14, data_3137, 15, data_313D, 16, data_3148, 17, data_3155, 18, data_315B, 19, data_3167, 20, data_3172)))
static void script_15AF(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F8 = 0;
  while (1) {
    Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_GetControllerInput(0) != 0x400) {
      int32_t local_EC;
      if (Script_GetControllerInput(1) == 64) {
        local_F8 = local_F8 - 1;
      }
      if (Script_GetControllerInput(1) == 128) {
        local_F8 = local_F8 + 1;
      }
      if (local_F8 > 2) {
        local_F8 = 0;
      }
      if (local_F8 < 0) {
        local_F8 = 2;
      }
      local_EC = local_F8;
      switch (local_EC) {
        case 0:
          Script_PrintDebugString(3, 25, "01152.c");
          break;
        case 1:
          Script_PrintDebugString(3, 25, ".c");
          break;
        case 2:
          Script_PrintDebugString(3, 25, "ALL");
          break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    {
      int32_t local_EC;
      local_EC = local_F8;
      switch (local_EC) {
        case 0:
          script_14CB();
          local_FC = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_01152.BIN");
          Script_LoadSND_Wrapper(0, 0, "bgm107.snd", 0, "a1_tabid.snd", 0, 0, 0);
          Script_executeAicaCommand(16296, 0, 0);
          Script_executeAicaCommand(1184, 2, 105);
          Script_scenCreateTask(script_0186, 0);
          script_03FE(1);
          Script_executeAicaCommand(2720, 2, 60);
          Script_filmCheckOrSetTransitionUtil(80, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_shenSeqUtils(-4);
          Script_executeAicaCommand(0x1200A0, 2, 0);
          SCNF_WaitFrames_0000(60);
          Script_ReallocateUNFO(local_FC);
          script_06A3();
          Script_SetGameStateTo(AUTH_None);
          SCNF_WaitFrames_0000(1);
          break;
        case 1:
          script_13AB();
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_LoadSND_Wrapper(0, 0, "tsm003.snd", 0, "a1_kstab.snd", 0, 0, 0);
          Script_LoadVoiceFile(1, "A0180");
          local_F4 = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_0180.BIN");
          local_F0 = Script_LoadHandMot(3, SHEN_NAME('YDMA'), "HMOT0156.BIN");
          script_0751();
          Script_ReallocateUNFO(local_F4);
          Script_FreeTaskData(local_F0);
          Script_LoadVoiceFile(0, "A0180");
          Script_executeAicaCommand(0x1200A0, 2, 0);
          Script_executeAicaCommand(1184, 2, 0);
          SCNF_WaitFrames_0000(1);
          break;
        case 2:
          script_13AB();
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_LoadSND_Wrapper(0, 0, "tsm003.snd", 0, "a1_kstab.snd", 0, 0, 0);
          Script_LoadVoiceFile(1, "A0180");
          local_F4 = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_0180.BIN");
          local_F0 = Script_LoadHandMot(3, SHEN_NAME('YDMA'), "HMOT0156.BIN");
          script_0751();
          Script_ReallocateUNFO(local_F4);
          Script_FreeTaskData(local_F0);
          Script_LoadVoiceFile(0, "A0180");
          Script_executeAicaCommand(0x1200A0, 2, 0);
          Script_executeAicaCommand(1184, 2, 0);
          script_14CB();
          local_FC = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_01152.BIN");
          Script_LoadSND_Wrapper(0, 0, "bgm107.snd", 0, "a1_tabid.snd", 0, 0, 0);
          Script_executeAicaCommand(16296, 0, 0);
          Script_executeAicaCommand(1184, 2, 105);
          Script_scenCreateTask(script_0186, 0);
          script_03FE(1);
          Script_executeAicaCommand(2720, 2, 60);
          Script_filmCheckOrSetTransitionUtil(80, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_shenSeqUtils(-4);
          Script_executeAicaCommand(0x1200A0, 2, 0);
          SCNF_WaitFrames_0000(60);
          Script_ReallocateUNFO(local_FC);
          script_06A3();
          break;
      }
    }
  }
  return;
}
