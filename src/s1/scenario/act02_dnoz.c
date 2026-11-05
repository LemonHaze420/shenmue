#include "act02_dnoz.h"

static int32_t script_0030(void) {
  for (;;) {
    Script_ClearAccumulator_00FE(1, 0x3F000000, 10, 2, 2);
    Script_ClearAccumulator_00FE(3, &data_11DC, 8, 9);
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
  script_0625();
  local_FC = 0;
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_042F(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_0086();
  script_0625();
  Script_shenSeqUtils(arg0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_FACE(CHARA_HRSK, 0, 0, 32);
        break;
      case 170:
        Script_executeAicaCommand(19112, 0, 0);
        Script_executeAicaCommand(1184, 2, 100);
        break;
      case 331:
        Script_FACE(CHARA_HRSK, 3, 0, 32);
        break;
      case 829:
        Script_FACE(CHARA_HRSK, 0, 0, 32);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_04E4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_shenSeqUtils(-7) == 0) {
    local_FC = 0;
    script_07BB(CHARA_AKIR);
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      local_F8 = local_FC;
      switch (local_F8) {
        case 0:
          Script_FACE(CHARA_HRSK, 3, 0, 32);
          break;
        case 710:
          Script_FACE(CHARA_HRSK, 0, 0, 28);
          break;
        case 825:
          Script_FACE(CHARA_HRSK, 3, 0, 32);
          break;
        case 1871:
          Script_FACE(CHARA_HRSK, 7, 0, 19);
          break;
        case 1900:
          script_0780(CHARA_AKIR);
          break;
        case 2247:
          Script_FACE(CHARA_HRSK, 0, 0, 32);
          break;
        case 2780:
          Script_handSetFloatsFor(CHARA_HRSK, 1, data_1DB8, 16);
          break;
        case 2801:
          Script_FACE(CHARA_HRSK, 3, 0, 19);
          break;
        case 2979:
          goto L000624;
          break;
      }
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  L000624:;
  return;
}

static void script_0625(void) {
  script_009E(CHARA_AKIR);
  script_009E(CHARA_HRSK);
  Script_SetMaxInterpSpeed(0.4f);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1E9C);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_1EAC);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_HRSK, 1, 0);
  script_0780(CHARA_AKIR);
  script_0780(CHARA_HRSK);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_HRSK, -1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_HRSK, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  return;
}

static void script_071A(void) {
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_HRSK, 0);
  script_07BB(CHARA_AKIR);
  script_07BB(CHARA_HRSK);
  script_012B(CHARA_AKIR);
  script_012B(CHARA_HRSK);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  return;
}

static void script_0780(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_1860, 8);
  Script_handSetFloatsFor(arg0, 0, data_1860, 8);
  return;
}

static void script_07BB(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_07E5(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F0[3];
  int32_t local_EC;
  Script_executeAicaCommand(19112, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_0086();
  script_0A2B();
  local_FC = 0;
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_FC;
    switch (local_EC) {
      case 0:
        Script_FACE(CHARA_HRSK, 3, 0, 16);
        break;
      case 400:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
        break;
      case 950:
        Script_FACE(CHARA_HRSK, 0, 0, 16);
        break;
      case 1472:
        Script_FACE(CHARA_HRSK, 2, 0, 16);
        break;
      case 1560:
        Script_FACE(CHARA_HRSK, 3, 0, 16);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_08C2(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  if (Script_shenSeqUtils(-7) == 0) {
    script_0086();
    script_0A2B();
    local_FC = 0;
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      local_EC = local_FC;
      switch (local_EC) {
        case 0:
          Script_FACE(CHARA_HRSK, 3, 0, 16);
          break;
        case 143:
          script_07BB(CHARA_AKIR);
          script_07BB(CHARA_HRSK);
          break;
        case 570:
          Script_scenToggleActiveFor(CHARA_AKIR, 0);
          Script_scenToggleActiveFor(CHARA_HRSK, 0);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
          break;
        case 1000:
          Script_FACE_LCG(CHARA_HRSK, FACE_BlinkClose, 16, 0);
          break;
        case 1080:
          Script_FACE(CHARA_HRSK, 0, 0, 16);
          break;
        case 1162:
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
          break;
        case 1500:
          goto L000A1A;
          break;
      }
      if ((local_FC >= 896) & (local_FC <= 1161)) {
        Script_EVT_GetPosition(CHARA_HRSK, 18, &local_F0, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 8);
      }
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
    }
    L000A1A:;
    Script_FACE_LCG(CHARA_HRSK, FACE_BlinkAutomatic, 0, 0);
  }
  return;
}

static void script_0A2B(void) {
  script_009E(CHARA_AKIR);
  script_009E(CHARA_HRSK);
  Script_SetMaxInterpSpeed(0.4f);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1EBC);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_1ECC);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_HRSK, 1, 0);
  script_0780(CHARA_AKIR);
  script_0780(CHARA_HRSK);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_HRSK, -1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_HRSK, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
  return;
}

static void script_0B36(void) {
  script_0BBF(CHARA_AKIR);
  Script_SetCharaFlags(CHARA_HRSK, 0);
  script_012B(CHARA_AKIR);
  script_012B(CHARA_HRSK);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  return;
}

static void script_0B84(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_1860, 8);
  Script_handSetFloatsFor(arg0, 0, data_1860, 8);
  return;
}

static void script_0BBF(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

void script_0BE9(void) {
  Script_scenCreateTask(script_0BFC, 1, 0x52494B41);
  return;
}

__attribute__((scn_string_bindings(0, data_1EDC, 2, data_1EEF, 4, data_1F07, 6, data_1F22, 8, data_1F3C, 9, data_1F42, 12, data_1F62, 14, data_1F7D, 16, data_1F98, 17, data_1FA3)))
static int32_t script_0BFC(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  local_FC = Script_GetCurrentEntryPoint();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 1);
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(8, 0);
    Script_mapmSetRenderFlag(9, 1);
    Script_mapmSetRenderFlag(10, 0);
    Script_mapmSetRenderFlag(11, 1);
    Script_mapmSetRenderFlag(12, 1);
  } else {
    Script_mapmSetRenderFlag(8, 1);
    Script_mapmSetRenderFlag(9, 0);
    Script_mapmSetRenderFlag(10, 1);
    Script_mapmSetRenderFlag(11, 0);
    Script_mapmSetRenderFlag(12, 0);
  }
  Script_WeatherUtils_ForFIXC(13, 0);
  Script_WeatherUtils_ForFIXC(14, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 1, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleCloudXYFlags, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(60);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (1) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 0:
        script_1010();
        Script_LoadVoiceFile(1, "01SKI");
        Script_LoadSND(0, 4, "a1_nozok.snd", 0);
        Script_LoadSND(0, 2, "bgm068.snd", 0);
        Script_AuthorUtils(0, 0, "SEQDATA4.BIN", "SCENE/02/DNOZ");
        Script_AuthorUtils(0, 1, "SEQDATA5.BIN", "SCENE/02/DNOZ");
        local_F8 = Script_motiLoad(2, SHEN_NAME('DNOZ'), "M_01SKI.BIN");
        Script_scenCreateTask(script_017B, 0);
        script_042F(0);
        script_04E4(1);
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        script_071A();
        Script_LoadVoiceFile(0, "01SKI");
        SCNF_WaitFrames_0000(60);
        Script_AuthorUtils(AUTH_FREE, 0);
        Script_AuthorUtils(AUTH_FREE, 1);
        Script_ReallocateUNFO(local_F8);
        Script_SetGameStateTo(AUTH_None);
        break;
      case 1:
        script_10E7();
        Script_LoadVoiceFile(1, "01FULB");
        local_F8 = Script_motiLoad(2, SHEN_NAME('DNOZ'), "M_01FUL.BIN");
        Script_AuthorUtils(0, 0, "SEQDATA2.BIN", "SCENE/02/DNOZ");
        Script_AuthorUtils(0, 1, "SEQDATA3.BIN", "SCENE/02/DNOZ");
        Script_LoadSND(0, 4, "a1_nonam.snd", 0);
        Script_LoadSND(0, 2, "bgm068.snd", 0);
        Script_scenCreateTask(script_017B, 0);
        script_07E5(0);
        script_08C2(1);
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        script_0B36();
        Script_LoadVoiceFile(0, "01FULB");
        SCNF_WaitFrames_0000(30);
        Script_AuthorUtils(AUTH_FREE, 0);
        Script_AuthorUtils(AUTH_FREE, 1);
        Script_ReallocateUNFO(local_F8);
        Script_SetGameStateTo(AUTH_None);
        break;
    }
    Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 1, 0);
    Script_WeatherTaskUtil(ToggleFogTask, 1);
    Script_WeatherTaskUtil(ToggleCloudXYFlags, 1);
    Script_WeatherTaskUtil(ToggleSceneLightingFlags, 1);
    SCNF_WaitFrames_0000(3);
    Script_WeatherTaskUtil(LoadSPR);
    Script_WeatherUtils_ForFIXC(13, 1);
    Script_WeatherUtils_ForFIXC(14, 1);
    if (local_FC == 0) {
      Script_ReturnTimeDate(&local_E8);
      (&local_E8)->hours = 23;
      (&local_E8)->minutes = 30;
      (&local_E8)->seconds = 0;
      Script_SetNewDateTime(&local_E8);
      Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
      SCNF_WaitFrames_0000(1);
    }
    Script_executeAicaCommand(0x1200A0, 2, 0);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_2014, 1, data_201B)))
static void script_1010(void) {
  Vector3 local_F4;
  (void)1;
  Script_SetVector4ByOffset(&data_1FF4);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_2004);
  Script_SetFogColour(&data_1FAC);
  Script_SetFogFlags(0, data_1FBC);
  Script_shenSetLightingMode(2);
  Script_lightSetGlobalSceneLighting(&data_1FCC);
  local_F4.x = 20.61f;
  (&local_F4)->y = 3.39f;
  (&local_F4)->z = -4.08f;
  Script_lightEnablePointLightAt(1, &local_F4);
  Script_LightUtils(1, 2, &data_1FDC);
  Script_LightUtils(1, 4, 1.0f);
  Script_LightUtils(1, 5, 3.69f);
  Script_LightUtils(1, 6, data_1FEC);
  Script_lightSetSlotEnabled(1, 1);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL27.SPR");
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
  return;
}

__attribute__((scn_string_bindings(0, data_2090, 1, data_2097)))
static void script_10E7(void) {
  Vector3 local_F4;
  (void)1;
  Script_SetVector4ByOffset(&data_2070);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_2080);
  Script_SetFogColour(&data_2028);
  Script_SetFogFlags(0, data_2038);
  Script_shenSetLightingMode(2);
  Script_lightSetGlobalSceneLighting(&data_2048);
  local_F4.x = 20.61f;
  (&local_F4)->y = 3.39f;
  (&local_F4)->z = -4.08f;
  Script_lightEnablePointLightAt(1, &local_F4);
  Script_LightUtils(1, 2, &data_2058);
  Script_LightUtils(1, 4, 1.0f);
  Script_LightUtils(1, 5, 3.69f);
  Script_LightUtils(1, 6, data_2068);
  Script_lightSetSlotEnabled(1, 1);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL27.SPR");
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
  return;
}
