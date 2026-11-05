#include "act03_msbs.h"

static void script_0030(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_0039(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_0048(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L000074;
    }
  }
  L000074:;
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

static void script_00D5(int32_t arg0) {
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

static int32_t script_0125(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0153(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0181(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01B5(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01E9(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0207(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_0241(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static int32_t script_0275(void) {
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
  end:;
}

static void script_036D(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_039D(void) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  script_07E3();
  local_F4 = Script_charaSetClipAngle(CHARA_AKIR, -1.0f);
  Script_charaSetClipAngle(CHARA_AKIR, 10.0f);
  local_FC = Script_scenCreateTask(script_0447, 0);
  local_F8 = Script_scenCreateTask(script_0275, 0);
  Script_shenSeqUtils(0);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_charaSetClipAngle(CHARA_AKIR, local_F4);
  script_083E();
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  return;
}

static int32_t script_0447(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_charaVisUtils(CHARA_AKIR, 1);
  local_E4 = 0;
  for (;;) {
    if (local_E4 == 0) {
      Script_FACE_LCG(CHARA_AKIR, FACE_BlinkClose, 0, 0);
      Script_FACE_LCG(CHARA_IWAO, FACE_BlinkClose, 0, 0);
    }
    if (local_E4 == 560) {
      Script_WeatherUtils_ForFIXC(13, 0);
      Script_WeatherUtils_ForFIXC(14, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 0, 0, 1, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 1, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 1, 0);
      Script_WeatherTaskUtil(ToggleFogTask, 0);
      Script_WeatherTaskUtil(ToggleCloudXYFlags, 0);
      Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
      Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('DDO1'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('DDO2'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Hide);
      Script_SpriteUtils(Scroll_Read3, 0, 0);
      Script_mapmSetRenderFlag(7, 0);
      Script_SetupLightFromTable(1);
      Script_FACE_LCG(CHARA_IWAO, FACE_BlinkOpen, 14, 0);
      Script_SetFogEnabled(1);
      Script_SetFogColour(&data_2280);
      Script_SetFogFlags(0, data_2290);
    }
    if (local_E4 == 584) {
      Script_handSetFloatsFor(CHARA_IWAO, 1, data_1524, 10);
    }
    if (local_E4 == 629) {
      Script_handSetFloatsFor(CHARA_IWAO, 1, data_1FD4, 10);
    }
    if (local_E4 == 670) {
      Script_handSetFloatsFor(CHARA_IWAO, 1, data_20B8, 10);
      Script_handSetFloatsFor(CHARA_IWAO, 0, data_219C, 10);
      local_E8.x = -1092;
      (&local_E8)->y = 910;
      (&local_E8)->z = -2366;
      Script_SetHandRootRotation(CHARA_IWAO, 1, 0x15, &local_E8);
    }
    if (local_E4 == 747) {
      local_F4.x = -4.87f;
      (&local_F4)->y = 2.13f;
      (&local_F4)->z = -23.87f;
      Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookAtPosition, &local_F4, 15);
    }
    if (local_E4 == 790) {
      Script_shenSetLightingMode(1);
      Script_LightUtils(0, 0, 2);
      Script_lightSetSlotEnabled(0, 1);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 0, 1, 0, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 0, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 1, 0, 0);
      Script_WeatherTaskUtil(ToggleFogTask, 1);
      Script_WeatherTaskUtil(ToggleCloudXYFlags, 1);
      Script_WeatherTaskUtil(ToggleSceneLightingFlags, 1);
      Script_WeatherTaskUtil(LoadSPR);
      Script_WeatherUtils_ForFIXC(13, 1);
      Script_WeatherUtils_ForFIXC(14, 1);
      Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('DDO1'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('DDO2'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Show);
      Script_mapmSetRenderFlag(7, 1);
    }
    if (local_E4 == 810) {
      Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 15, 0);
    }
    if (local_E4 == 1080) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_1524, 30);
    }
    if (local_E4 == 1110) {
      break;
    }
    local_E4 = local_E4 + 1;
    Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 4.3f, -13.5f, -16.1f, -11.6f, -17.8f, -31.4f, 1.2f, -33.0f, 0.0f, 10.0f);
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 2, 40);
  Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_07E3(void) {
  int32_t unused_E8[6];
  script_0859(CHARA_AKIR);
  script_0859(CHARA_IWAO);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetVector4ByOffset(&data_22A0);
  (void)1;
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_22A0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
  return;
}

static void script_083E(void) {
  script_0949(CHARA_AKIR);
  script_0949(CHARA_IWAO);
  return;
}

static void script_0859(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_1FD4, 10);
  Script_handSetFloatsFor(arg0, 1, data_1FD4, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaFlags(arg0, 4);
  Script_scenToggleActiveFor(arg0, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  return;
}

static void script_08FD(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_0926(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  return;
}

static void script_0949(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static void script_098F(void) {
  return;
}

static void script_0992(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = 0;
  script_0030();
  script_0AE3();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_22B0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_22BC, CHARA_PART_Set);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 160.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 80.0f);
  Script_SetShadowRange(160.0f);
  Script_SetCharaFlags(SHEN_NAME('AKI2'), 4);
  Script_SetCharaFlags(SHEN_NAME('CAR7'), 2);
  Script_SetCharaFlags(SHEN_NAME('CAR7'), 100, 1, &data_22C8);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 0:
        Script_executeAicaCommand(0x1D04A9, 0, 0);
        break;
      case 68:
        Script_scenCreateTask(script_0ED7, 0);
        break;
      case 100:
        Script_scenCreateTask(script_0BF7, 1, 0);
        break;
      case 125:
        Script_executeAicaCommand(0x1E04A9, 0, 0);
        Script_scenToggleActiveFor(SHEN_NAME('AKI2'), 0);
        local_F8 = Script_scenCreateTask(script_0E81, 1, SHEN_NAME('BUS_'));
        break;
      case 260:
        Script_srchControl((void *)local_F8);
        goto L000AE2;
        break;
    }
    local_FC = local_FC + 1;
    script_0EF4();
    SCNF_WaitFrames_0000(1);
  }
  L000AE2:;
  return;
}

static void script_0AE3(void) {
  Vector3 local_F4;
  int32_t unused_E4[4];
  GameDateTime local_DC;
  (void)1;
  Script_SetVector4ByOffset(&data_22E0);
  script_0048(SHEN_NAME('AKI2'));
  script_0048(CHARA_BUSS);
  Script_facemotUtils(SHEN_NAME('AKI2'), 1, 0);
  Script_SetCharaFlags(SHEN_NAME('BUS_'), 2);
  Script_SetCharaFlags(SHEN_NAME('BUS_'), 100, 1, &data_22D4);
  Script_scenToggleActiveFor(CHARA_BUSS, 0);
  Script_ReturnTimeDate(&local_DC);
  if ((&local_DC)->hours < 19) {
    local_F4.x = 0.0f;
    (&local_F4)->y = -100.0f;
    (&local_F4)->z = 0.0f;
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, CHARA_PART_Set);
  }
  return;
}

static void script_0B84(void) {
  Vector3 local_F4;
  (void)0;
  local_F4.x = 0.0f;
  (&local_F4)->y = -100.0f;
  (&local_F4)->z = 0.0f;
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, CHARA_PART_Set);
  script_00D5(SHEN_NAME('AKI2'));
  script_00D5(CHARA_BUSS);
  Script_scenToggleActiveFor(SHEN_NAME('AKI2'), 1);
  Script_FaceEx(SHEN_NAME('AKI2'), FACE_ExpressionEnabled, 0, 2);
  Script_SetCharaFlags(SHEN_NAME('AKI2'), 4);
  return;
}

static int32_t script_0BF7(int32_t arg0) {
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
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &local_F0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &local_E4, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &local_D8, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &local_CC, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &local_C0, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &local_B4, CHARA_PART_Set);
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_0E81(int32_t arg0) {
  while (Script_shenSeqUtils(-1)) {
    Script_charaPosRotUtils(arg0, 163, 0, &data_22F0, CHARA_PART_Add);
    Script_charaPosRotUtils(arg0, 173, 0, &data_22F0, CHARA_PART_Add);
    Script_charaPosRotUtils(arg0, 182, 0, &data_22FC, CHARA_PART_Add);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_0ED7(void) {
  SCNF_SetCashValue(-160, 1);
  SCNF_WaitFrames_0000(200);
  return Script_srchControl(0);
}

static void script_0EF4(void) {
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

void script_0F98(void) {
  Script_scenCreateTask(script_0FA6, 0);
  return;
}

__attribute__((scn_string_bindings(2, data_231D, 6, data_2345)))
static int32_t script_0FA6(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  GameDateTime local_EC;
  int32_t local_E8;
  local_FC = Script_GetCurrentEntryPoint();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ReturnTimeDate(&local_EC);
  (&local_EC)->hours = 10;
  (&local_EC)->minutes = 0;
  Script_SetNewDateTime(&local_EC);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(60);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MSBS/", "MSBS");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  while (1) {
    local_E8 = local_FC;
    if (local_E8 == 0) {
      script_1226();
      Script_LoadVoiceFile(1, "01MSO");
      Script_LoadSND(0, 2, "bgm103.snd", 0);
      Script_LoadSND(0, 4, "a1_mso.snd", 0);
      Script_executeAicaCommand(16296, 0, 0);
      Script_executeAicaCommand(1184, 2, 100);
      local_F8 = Script_motiLoad(3, SHEN_NAME('MSBS'), "M_01MSO.BIN");
      script_039D();
      Script_ReallocateUNFO(local_F8);
      Script_LoadVoiceFile(0, "01MSO");
      Script_charaDisplay(CHARA_AKIR, CDM_Hide);
      script_12CF();
      local_F8 = Script_motiLoad(3, SHEN_NAME('MSBS'), "M_01BUS.BIN");
      Script_LoadSND(0, 4, "a1_busno.snd", 0);
      Script_scenCreateTask(script_0125, 0);
      script_0992(1);
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenSeqUtils(-4);
      Script_ReallocateUNFO(local_F8);
      script_0B84();
      Script_executeAicaCommand(0x1200A0, 2, 0);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 935, 1);
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
      Script_shenRequestGameModuleTransition(3, SHEN_NAME('MFSY'), 4);
      SCNF_WaitFrames_0000(1);
      goto L0011FB;
    }
    L0011FB:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1207(int32_t arg0, const Vector3 *arg1, const Vector3i *arg2) {
  Script_EVT_SetPosition(arg0, POS_XYZ, arg1);
  Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
  return;
}

static void script_1226(void) {
  int32_t unused_E8[6];
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 1);
  Script_WeatherTaskUtil(InitTaskData);
  SCNF_WaitFrames_0000(1);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('AKI2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('AKI2'), CDM_Show);
      goto L001271;
    }
  }
  L001271:;
  Script_scenToggleVisibleFor(SHEN_NAME('AKI2'), 0);
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetCamFlags(0, 0);
  return;
}

static void script_12CF(void) {
  if (0) {
    Script_charaDisplay(SHEN_NAME('AKI2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('AKI2'), CDM_Show);
      goto L0012F9;
    }
  }
  L0012F9:;
  Script_scenToggleVisibleFor(SHEN_NAME('AKI2'), 1);
  if (-1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L00132D;
    }
  }
  L00132D:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 0);
  Script_mapmSetCamFlags(0, 1);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 1);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 1);
  Script_mapmSetRenderFlag(8, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_mapmSetRenderFlag(10, 1);
  Script_mapmSetRenderFlag(15, 1);
  Script_mapmSetRenderFlag(16, 1);
  Script_mapmSetRenderFlag(17, 1);
  Script_mapmSetRenderFlag(18, 1);
  Script_mapmSetRenderFlag(19, 1);
  Script_mapmSetRenderFlag(23, 1);
  return;
}
