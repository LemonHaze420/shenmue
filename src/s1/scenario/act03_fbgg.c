#include "act03_fbgg.h"

static void script_0030(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_3168, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_3130, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_30F8, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_3114, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_314C, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_0168(int32_t arg0) {
  script_0180(arg0);
  return;
}

static void script_0174(int32_t arg0) {
  script_0180(arg0);
  return;
}

static void script_0180(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L0001B9;
    }
  }
  L0001B9:;
  Script_ryoSetChara(arg0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_0030();
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
      Script_LoadVoiceFile(1, data_3199);
    }
  }
  Script_CPCTRelated(arg0, 32, 89);
  Script_clmdSetFlags(arg0, 8, 1);
  return;
}

static void script_0295(int32_t arg0) {
  script_0174(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_02AC(int32_t arg0) {
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

static void script_02ED(int32_t arg0) {
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

static void script_0331(int32_t arg0) {
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

static void script_03AF(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_0331(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_0331(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_0331(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_0331(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_0331(5);
  } else {
    script_0331(0);
  }
  return;
}

static void script_042E(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_0437(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_0446(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L000472;
    }
  }
  L000472:;
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

static void script_04D3(int32_t arg0) {
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

static void script_0523(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0551(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_057F(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_05B3(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_05E7(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0605(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_063F(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_0673(void) {
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

static void script_076B(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_079B(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_07C3(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_0817(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1)) {
      Script_EVT_GetPosition(arg1, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(arg0, 16, &local_F4, arg2);
      Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 30);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_0867(int32_t arg0) {
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

static void script_090E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_0867(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_0931(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_0964(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_0931(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_0987(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_0931(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_09E6(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_09FB(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = 0;
  for (;;) {
    switch (local_E4) {
      case 1:
        local_F4.x = 0.1996f;
        (&local_F4)->y = 0.1647f;
        (&local_F4)->z = 0.472100019f;
        local_E8.x = 0;
        (&local_E8)->y = 0;
        (&local_E8)->z = 0;
        Script_fixoAttachEx(SHEN_NAME('DMMY'), SHEN_NAME('BIKE'), 0, &local_F4, &local_E8);
        local_F4.x = 0.1896f;
        (&local_F4)->y = 0.3047f;
        (&local_F4)->z = 0.8963f;
        local_E8.x = 0;
        (&local_E8)->y = 0;
        (&local_E8)->z = 0;
        Script_fixoAttachEx(SHEN_NAME('DMM2'), SHEN_NAME('BIKE'), 0, &local_F4, &local_E8);
        break;
      case 766:
        Script_FACE(CHARA_AKIR, 1, 0, 10);
        break;
      case 840:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        break;
      case 1211:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_36F8, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_36F8, 0);
        break;
      case 1266:
        local_F4.x = 0.023f;
        (&local_F4)->y = 0.002f;
        (&local_F4)->z = 0.145f;
        local_E8.x = 17019;
        (&local_E8)->y = 94;
        (&local_E8)->z = 48610;
        Script_fixoAttachEx(SHEN_NAME('HELM'), CHARA_AKIR, 5, &local_F4, &local_E8);
        break;
      case 1550:
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    }
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('DMMY'));
  Script_fixoClearAttachment(SHEN_NAME('DMM2'));
  Script_fixoClearAttachment(SHEN_NAME('HELM'));
  script_0D34(CHARA_AKIR);
  return;
}

static void script_0BF6(void) {
  script_0CA4(CHARA_AKIR);
  script_0D0B(CHARA_ITOH);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_ITOH, -1);
  Script_ccowSetHeightMode(SHEN_NAME('BIKE'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('SUJI'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('HIKA'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('HELM'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('TAIL'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('KEY_'), CCOW_KeepHeight);
  Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('HELM'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('TAIL'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KEY_'), CDM_Show);
  Script_SetVector4ByOffset(&data_3D34);
  (void)1;
  return;
}

static void script_0CA4(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_facemotUtils(arg0, 1, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_3530, 10);
  Script_handSetFloatsFor(arg0, 1, data_3530, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return;
}

static void script_0D0B(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_0D34(int32_t arg0) {
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static void script_0D72(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  float local_D8;
  float local_D4;
  int32_t unused_D0;
  Vector3i local_C4;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  for (;;) {
    local_F4.x = (&local_E8)->x;
    (&local_F4)->y = (&local_E8)->y;
    (&local_F4)->z = (&local_E8)->z;
    Script_EVT_GetPosition(SHEN_NAME('BIKE'), -1, &local_E8, 0);
    local_DC[0] = (&local_E8)->x - (&local_F4)->x;
    local_DC[1] = (&local_E8)->y - (&local_F4)->y;
    local_DC[2] = (&local_E8)->z - (&local_F4)->z;
    local_DC[0] = *local_DC * *local_DC;
    local_DC[1] = local_DC[1] * local_DC[1];
    local_DC[2] = local_DC[2] * local_DC[2];
    local_D8 = *local_DC * *local_DC + local_DC[1] * local_DC[1] + local_DC[2] * local_DC[2];
    local_D4 = local_D8 * 1000000.0f;
    if (!(local_D4 <= 300.0f)) {
      local_D4 = 300.0f;
    }
    if (!(local_D4 >= 0.5f)) {
      local_D4 = 0.0f;
    }
    local_C4.x = -(int32_t)(local_D4 / 360.0f * (float)0x10000);
    (&local_C4)->y = 0;
    (&local_C4)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('BIKE'), 23, 0, &local_C4, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('BIKE'), 18, 0, &local_C4, CHARA_PART_Add);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

void script_0ED5(void) {
  script_1052();
  script_1117();
  script_1245();
  script_1263();
  (void)((task_20 = -1) & 1);
  (void)((task_24 = -1) & 1);
  Script_scenCreateTask(script_127E, 0);
  while (task_20 & 2) {
    SCNF_WaitFrames_0000(1);
  }
  script_1324();
  script_132A();
  script_14EC();
  script_1507();
  SCNF_WaitFrames_0000(1);
  script_1478();
  Script_CtrlTimeLimit(InitAndReset, 0);
  Script_CtrlTimeLimit(Update, 120);
  task_50 = Script_EffectUtils(0, &data_3D44, &data_3D50, 100, 0.001f, 0, 3);
  task_54 = Script_SetEFPT(10, &data_3D44, &data_3D50, 1000, 0.1f, 0, -2.0f, 0, 0);
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (1) {
    script_1327();
    script_1577();
    script_1542();
    script_1714();
    script_1778();
    if (~task_1A) {
      script_1B13();
      script_1F0A();
      if (script_1AD5()) {
        (void)((task_1A = -1) & 4);
        Script_scenCreateTask(script_1D31, 0);
      } else {
        if (script_16F3()) {
          if (task_16 == 0) {
            (void)((task_16 = -1) & 4);
            (void)((task_1A = -1) & 4);
            Script_scenCreateTask(script_17BB, 1, 0);
          }
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1052(void) {
  int32_t local_FC;
  (void)((task_15 = 0) & 4);
  (void)((task_1B = -1) & 4);
  (void)((task_1D = 0) & 4);
  (void)((task_17 = 0) & 4);
  (void)((task_18 = 0) & 4);
  (void)((task_1E = 0) & 4);
  (void)((task_1F = 0) & 4);
  (void)((task_19 = -1) & 4);
  task_14 = 0;
  (void)((task_20 = 0) & 4);
  (void)((task_24 = 0) & 4);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE, 1);
  script_1A6C();
  local_FC = Script_GetCurrentEntryPoint();
  if (local_FC == 99) {
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 27, 0);
  } else if (local_FC == 0) {
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 27, 0);
    (void)((task_17 = -1) & 4);
  } else {
    (void)((task_1B = 0) & 4);
  }
  return;
}

static void script_1117(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_TimedTransitionUtils(TIMED_TRANSITION_SET_ENABLED, 0);
  script_1518(3, 0);
  script_0180(CHARA_AKIR);
  Script_WeatherUtils_ForFIXC(13, 0);
  Script_SetShouldSleepFlag(0);
  Script_refbSetActive(1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 400.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 200.0f);
  Script_SetCameraFlags(0);
  Script_LoadVoiceFile(1, "SA1101");
  Script_SetMapFlags(0, 1);
  return;
}

static void script_1199(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  Script_ReturnTimeDate(&local_E8);
  {
    uint8_t unused_E7;
    int8_t local_E6;
    local_E6 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 18);
    (&local_F4)->month = (local_E6 >> 5) & 7;
    if ((&local_F4)->month == 0) {
      (&local_F4)->month = 12;
    }
    (&local_F4)->day = local_E6 & 0x1F;
  }
  (&local_E8)->month = (&local_F4)->month;
  (&local_E8)->day = (&local_F4)->day;
  {
    int32_t local_E4;
    local_E4 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 26);
    (&local_F4)->hours = local_E4 * 6 / 60;
    (&local_F4)->minutes = local_E4 * 6 % 60;
  }
  (&local_E8)->hours = (&local_F4)->hours;
  (&local_E8)->minutes = (&local_F4)->minutes;
  Script_SetNewDateTime(&local_E8);
  return;
}

static void script_1245(void) {
  task_C = Script_GetSceneID();
  Script_motiLoad(task_C, SHEN_NAME('FREE'), "M_BIKE.BIN");
  return;
}

__attribute__((scn_string_bindings(0, data_3D6E)))
static void script_1263(void) {
  Script_LoadSND_Wrapper("system1.snd", 0, 0, 0, 0, 0, 0, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_3D7A, 2, data_3D90)))
static int32_t script_127E(void) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  script_12CD("/scene/03/FBGG", "FBGGST");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  (void)((task_20 = 0) & 0x10);
  script_12CD("/scene/03/FBGG", "FBGGCAR");
  return (task_24 = 0) & 0x10;
}

static void script_12CD(const char *arg0, const char *arg1) {
  int32_t local_FC;
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1)) {
  } else {
    while (1) {
      local_FC = Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1);
      if (local_FC) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static void script_1324(void) {
  return;
}

static void script_1327(void) {
  return;
}

static void script_132A(void) {
  int32_t unused_F4[3];
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t local_CC;
  Script_SetupClothData(CHARA_AKIR, 8);
  local_CC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 27);
  Script_vehiUtils(VehicleCommand_SetGaugeDecayTier, SHEN_NAME('BIKS'), local_CC);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE)) {
    local_CC = local_CC + 1;
    local_CC = local_CC & 0xFF;
  }
  if (local_CC) {
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 27, local_CC);
  }
  Script_SetLampState(0, SHEN_NAME('BIKS'), 1);
  Script_MoveObjectToObject(SHEN_NAME('DMB0'), 0x534B4942);
  Script_MoveObjectToObject(SHEN_NAME('LIPA'), 0x534B4942);
  local_DC.x = 0.0f;
  (&local_DC)->y = 0.01f;
  (&local_DC)->z = -6.0f;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  local_D0.x = (&local_D0)->y = (&local_D0)->z = 0;
  Script_EVT_SetPosition(SHEN_NAME('LIPA'), POS_XYZ, &local_E8);
  Script_EVT_SetRotation(SHEN_NAME('LIPA'), 0x38000000, &local_D0, 0);
  Script_EVT_SetPosition(SHEN_NAME('DMB0'), POS_XYZ, &local_DC);
  Script_EVT_SetRotation(SHEN_NAME('DMB0'), 0x38000000, &local_D0, 0);
  task_0 = 40.0f;
  task_8 = 40.0f;
  Script_atckUtils(0, 0, 0);
  Script_atckUtils(SetStyle, 0, 0);
  Script_atckUtils(SetGauge, &task_0);
  Script_atckUtils(SetFlag, 1, 1);
  return;
}

static void script_1478(void) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, 0);
    Script_EVT_SetPosition(SHEN_NAME('METT'), POS_XYZ, &local_F4);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.014f;
    (&local_F4)->y = 0.003f;
    (&local_F4)->z = 0.152f;
    local_E8.x = 16560;
    (&local_E8)->y = 0;
    (&local_E8)->z = 48261;
    Script_fixoAttachEx(SHEN_NAME('METT'), CHARA_AKIR, 5, &local_F4, &local_E8);
  }
  return;
}

static void script_14EC(void) {
  Script_WeatherTaskUtil(AndNotInstructionFlags, 0x784);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_3DA8, 3);
  return;
}

static void script_1507(void) {
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  return;
}

static void script_1518(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  (&local_F4)->hours = arg0;
  (&local_F4)->minutes = arg1;
  (&local_F4)->seconds = 0;
  Script_SetNewDateTime(&local_F4);
  return;
}

static void script_1542(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_vehiUtils(VehicleCommand_GetShadowProxyTransform, SHEN_NAME('BIKS'), &local_F4, &local_E8);
  Script_EVT_SetPosition(SHEN_NAME('SHD1'), 0x78000000, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('SHD1'), 0x78000000, &local_E8, 0);
  return;
}

static void script_1577(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3i local_DC;
  Script_vehiUtils(VehicleCommand_GetLienProxyTransform, SHEN_NAME('BIKS'), &local_F4, &local_DC);
  Script_EVT_SetPosition(SHEN_NAME('LIEN'), 0x78000000, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('LIEN'), 0x78000000, &local_DC, 0);
  if (Script_GetControllerInput(4)) {
    if (task_18 & 2) {
      if (~task_19) {
        (void)((task_19 = -1) & 1);
        Script_SetLampState(0, SHEN_NAME('BIKS'), 1);
      }
    }
  } else {
    if (task_18 & 2) {
      if (task_19 & 4) {
        (void)((task_19 = 0) & 1);
        Script_SetLampState(0, SHEN_NAME('BIKS'), 0);
      }
    }
  }
  return;
}

static void script_160E(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  float local_D8;
  float local_D4;
  Script_EVT_GetPosition(SHEN_NAME('BIKS'), -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_GetPosition(SHEN_NAME('DMB0'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_SetPosition(SHEN_NAME('LIEN'), 0x78000000, &local_F4);
  local_DC.x = (&local_DC)->z = 0;
  local_D8 = (&local_F4)->x - (&local_E8)->x;
  local_D4 = (&local_F4)->z - (&local_E8)->z;
  (&local_DC)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, local_D8, local_D4);
  Script_EVT_SetRotation(SHEN_NAME('LIEN'), 0x78000000, &local_DC, 0);
  if (Script_GetControllerInput(4)) {
    if (task_18 & 2) {
      if (~task_19) {
        (void)((task_19 = -1) & 8);
        Script_SetLampState(0, SHEN_NAME('BIKS'), 1);
      }
    }
  } else {
    if (task_18 & 2) {
      if (task_19 & 4) {
        (void)((task_19 = 0) & 8);
        Script_SetLampState(0, SHEN_NAME('BIKS'), 0);
      }
    }
  }
  return;
}

static int32_t script_16F3(void) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_CtrlTimeLimit(GetTimeLeft, 0)) {
  } else {
    local_FC = 1;
  }
  return local_FC;
}

static void script_1714(void) {
  Vector3 local_F4;
  float local_F0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  local_F0 = -1332.8f;
  if (!(local_F0 >= (&local_F4)->z)) {
    if (task_15 == 0) {
      (void)((task_15 = -1) & 1);
      Script_SetupLightFromTable(0);
    }
  } else {
    if (task_15 == -1) {
      (void)((task_15 = 0) & 1);
      Script_SetupLightFromTable(2);
    }
  }
  return;
}

static void script_1778(void) {
  if (task_10) {
    if (task_10 == 300) {
      script_1A98();
    }
    task_10 = task_10 - 1;
    if (task_10 == 0) {
      script_1ABC();
    }
  }
  return;
}

static int32_t script_17AC(void) {
  return Script_ShowUIMessageFromID("SA1101A008");
}

static int32_t script_17B8(void) {
  end:;
}

__attribute__((scn_string_bindings(0, data_3E5B, 1, data_3E67, 2, data_3E6C, 3, data_3E71, 4, data_3E76, 5, data_3E7B, 6, data_3E80, 7, data_3E85)))
static int32_t script_17BB(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0;
  if (local_F8 == 0) {
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    Script_scenCreateTask(script_17AC, 0);
  } else if (local_F8 == 1) {
    Script_scenCreateTask(script_17B8, 0);
  } else {
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    Script_scenCreateTask(script_17AC, 0);
  }
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (arg0 != 1) {
    Script_vehiUtils(VehicleCommand_RequestDismount, SHEN_NAME('BIKS'));
    Script_executeAicaCommand(262304, 0, 0);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
    Script_shenClearFreeState(0);
  }
  Script_atckUtils(SetFlag, 1, 0);
  Script_CtrlTimeLimit(Disable, 0);
  Script_EffectUtils(3, task_50);
  Script_SetEFPT(3, task_54);
  Script_uiUtils(0, 22, 0);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  SCNF_WaitFrames_0000(90);
  script_1199();
  Script_LoadSND_Wrapper("system1.snd", "FREE", "FREE", "FREE", "FREE", "FREE", "FREE", "FREE");
  Script_shenRequestGameModuleTransition(task_C, SHEN_NAME('JD00'), 40);
  while (1) {
    SCNF_WaitFrames_0000(1);
  }
  script_1518(3, 0);
  Script_MoveObjectToObject(SHEN_NAME('BIKS'), -1);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  script_1E93();
  Script_SetGameStateTo(AUTH_None);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  local_FC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 27);
  Script_vehiUtils(VehicleCommand_SetGaugeDecayTier, SHEN_NAME('BIKS'), local_FC);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE)) {
    local_FC = local_FC + 1;
    local_FC = local_FC & 0xFF;
  }
  if (local_FC) {
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 27, local_FC);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 21014, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_atckUtils(SetFlag, 1, 1);
  Script_CtrlTimeLimit(Update, 0);
  Script_CtrlTimeLimit(Update, 120);
  Script_filmCheckOrSetTransitionUtil(90, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(1);
  script_1A6C();
  end:;
}

static void script_1A6C(void) {
  task_10 = 300;
  (void)((task_16 = 0) & 0x80);
  (void)((task_1A = 0) & 0x80);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 48.0f);
  return;
}

static void script_1A98(void) {
  Script_sicnUtils(0, 1, 0);
  Script_sicnUtils(0, 64, 0);
  Script_uiUtils(0, 22, 0);
  return;
}

static void script_1ABC(void) {
  Script_sicnUtils(1, 0, 0);
  Script_sicnUtils(2, 64, 0);
  return;
}

static int32_t script_1AD5(void) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F0 = 0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  if (!((&local_F4)->z >= (&local_F4)->x * -0.73345f + -3087.37793f)) {
    local_F0 = 1;
  }
  return local_F0;
}

static void script_1B13(void) {
  float local_FC;
  int32_t local_F8;
  int32_t local_F4;
  task_8 = *&task_0;
  Script_vehiUtils(VehicleCommand_GetGauge, &task_0);
  if (!(*&task_0 <= 157.0f)) {
    local_FC = *&task_0 - 157.0f;
    task_4 = *&task_0 - Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, local_FC);
    Script_atckUtils(SetGauge, &task_4);
  } else {
    Script_atckUtils(SetGauge, &task_0);
  }
  script_2ABB(*&task_0);
  task_34 = (&task_28)->x;
  *(&task_34 + 1) = task_28.y;
  *(&task_34 + 2) = task_28.z;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &task_28, ENTITY_XFORM_WORLD_SPACE);
  task_40 = (&task_28)->x - *&task_34;
  *(&task_40 + 1) = task_28.y - *(&task_34 + 1);
  *(&task_40 + 2) = task_28.z - *(&task_34 + 2);
  task_4C = Script_MathUtils(SCRIPT_MATH_SQRT, *&task_40 * *&task_40 + *(&task_40 + 2) * *(&task_40 + 2));
  if (Script_vehiUtils(VehicleCommand_GetTrailEffectFlag, SHEN_NAME('BIKS'))) {
    task_5C = Script_scenCreateTask(script_20E7, 0);
  } else {
    (void)((task_1F = 0) & 1);
  }
  if (!(task_8 - *&task_0 <= 1.2f)) {
    task_58 = Script_scenCreateTask(script_1F45, 1, 0);
  }
  local_F8 = Script_GetControllerInput(5);
  local_F4 = (Script_GetControllerInput(0) & 0x4000) | (Script_GetControllerInput(0) & 0x8000) | -(local_F8 > 144) | -(local_F8 < 112);
  if (local_F4 & -!(*&task_0 <= 90.0f) & -(Script_GetControllerInput(3) != 0)) {
    if (task_60 > 10) {
      task_58 = Script_scenCreateTask(script_1F45, 1, 0);
    }
    task_60 = task_60 + 1;
  } else if (local_F4 & -!(*&task_0 <= 90.0f) & -(Script_GetControllerInput(3) != 0)) {
    if (task_60 > 10) {
      task_58 = Script_scenCreateTask(script_1F45, 1, 0);
    }
    task_60 = task_60 + 1;
  } else {
    task_60 = 0;
  }
  return;
}

static int32_t script_1D31(void) {
  Script_atckUtils(SetFlag, 1, 0);
  Script_CtrlTimeLimit(Disable, 0);
  script_2207();
  if (task_1B == 0) {
    Script_EffectUtils(3, task_50);
    Script_SetEFPT(3, task_54);
    if (~task_17) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE, 1);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(1);
      Script_shenRequestGameModuleTransition(task_C, CHARA_TERY, 99);
      SCNF_WaitFrames_0000(1);
      while (1) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      script_17BB(1);
      Script_executeAicaCommand(20136, 0, 0);
      Script_executeAicaCommand(1184, 2, 110);
      Script_executeAicaCommand(2464, 2, 30);
    }
  } else {
    script_17BB(1);
    Script_executeAicaCommand(20136, 0, 0);
    Script_executeAicaCommand(1184, 2, 110);
    Script_executeAicaCommand(2464, 2, 30);
  }
  end:;
}

static void script_1E1B(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  if (!((&local_F4)->z >= (&local_F4)->x * -1.096f + -2932.634033f)) {
    Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 1, 0x3F000000);
  } else if (!((&local_F4)->z >= -2701.0f)) {
    Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 1, 0);
  } else {
    Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 1, 0x3F000000);
  }
  return;
}

static void script_1E93(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = -4.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -4.5f;
  local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_EVT_SetPosition(SHEN_NAME('BIKS'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('BIKS'), 0x38000000, &local_E8, 0);
  return;
}

static void script_1F0A(void) {
  if (task_14) {
    task_14 = task_14 - 1;
  } else {
    if (Script_GetControllerInput(1) == 0x1000) {
      task_14 = 15;
      Script_executeAicaCommand(263337, 0, 0);
    }
  }
  return;
}

static int32_t script_1F45(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  if (task_1E & 0x80) {
  } else {
    Script_EVT_GetPosition(SHEN_NAME('LIPA'), -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    local_F4.x = (&task_28)->x - ((&task_28)->x - (&local_E8)->x) * 1.0f + *&task_40 * 1.0f;
    (&local_F4)->y = task_28.y + 0.15f;
    (&local_F4)->z = task_28.z - (task_28.z - (&local_E8)->z) * 1.0f + *(&task_40 + 2) * 1.0f;
    Script_EffectUtils(5, task_50, &local_F4, &data_3E8C);
    Script_EffectUtils(12, task_50, 32);
    Script_EffectUtils(9, task_50, 64, 62, 60);
    Script_EffectUtils(11, task_50, 255);
    Script_EffectUtils(10, task_50, 0.262745112f);
    Script_EffectUtils(8, task_50, 2.54901958f, 1.254902f);
    Script_EffectUtils(15, task_50, 55.863998f, 1.41176474f);
    Script_EffectUtils(14, task_50, 0.125490203f, 0.00784313772f);
    local_DC.x = *&task_40 / task_4C * (*&task_0 - 40.0f) / 100.0f;
    (&local_DC)->y = 0.01f;
    (&local_DC)->z = *(&task_40 + 2) / task_4C * (*&task_0 - 40.0f) / 100.0f;
    local_D0.x = 0.0f;
    (&local_D0)->y = 0.0f;
    (&local_D0)->z = 0.0f;
    Script_EffectUtils(6, task_50, &local_DC, &local_D0);
    Script_EffectUtils(7, task_50, 0.0627451017f);
    Script_EffectUtils(1, task_50);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_20E7(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t unused_D8;
  if (task_1F & 1) {
  } else {
    Script_charaSetupXForm(SHEN_NAME('BIKS'), 23, &local_DC);
    local_E8.x = 0.01f;
    (&local_E8)->y = 0.05f;
    (&local_E8)->z = 0.01f;
    local_F4.x = 0.05f;
    (&local_F4)->y = 0.05f;
    (&local_F4)->z = 0.05f;
    Script_SetEFPT(5, task_54, &local_DC, &data_3E98);
    local_E8.x = (&local_E8)->x + ((&local_DC)->x - *&task_34) / task_4C * (*&task_0 - 40.0f) / 100.0f;
    (&local_E8)->z = (&local_E8)->z + ((&local_DC)->z - *(&task_34 + 2)) / task_4C * (*&task_0 - 40.0f) / 100.0f;
    Script_SetEFPT(11, task_54, 255, 240, 100, 0x10);
    Script_SetEFPT(6, task_54, &local_E8, &local_F4);
    Script_SetEFPT(8, task_54, 0.01f);
    Script_SetEFPT(7, task_54, 0.0125490194f);
    Script_SetEFPT(9, task_54, (int32_t)&data_3EA4);
    Script_SetEFPT(1, task_54);
  }
  end:;
}

static void script_2207(void) {
  Vector3 local_F4;
  float local_F0;
  int8_t local_EF;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 100.0f);
  Script_executeAicaCommand(262304, 0, 0);
  Script_vehiUtils(VehicleCommand_RequestDismount, SHEN_NAME('BIKS'));
  Script_charaDisplay(SHEN_NAME('SHD1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHD2'), CDM_Hide);
  Script_CreateNPCImageModule(SHEN_NAME('BIKS'), 0, 0);
  while (task_24 & 4) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CreateNPCImageModule(SHEN_NAME('BIKS'), "BIKS509G", 0);
  local_EF = Script_ExecuteAicaCommand(1, 259);
  (void)((task_1D = -1) & 2);
  Script_executeAicaCommand(166, local_EF, 127);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ccowUtils(SHEN_NAME('BIKS'), CCOW_DisableAll);
  Script_MoveObjectToObject(SHEN_NAME('BIKS'), 0x31424D44);
  Script_FACE(CHARA_AKIR, 5, 0, 1);
  Script_MoveObjectToObject(CHARA_AKIR, 0x31424D44);
  Script_MoveObjectToObject(SHEN_NAME('DMB2'), 0x534B4942);
  Script_MoveObjectToObject(SHEN_NAME('DMB3'), 0x534B4942);
  local_F4.x = 0.235f;
  (&local_F4)->y = 0.377f;
  (&local_F4)->z = -0.46f;
  Script_EVT_SetPosition(SHEN_NAME('DMB2'), POS_XYZ, &local_F4);
  local_F4.x = 0.415f;
  (&local_F4)->y = 1.16f;
  (&local_F4)->z = -0.271f;
  Script_EVT_SetPosition(SHEN_NAME('DMB3'), POS_XYZ, &local_F4);
  Script_FACE(CHARA_AKIR, 5, 0, 1);
  local_F0 = 60.0f;
  local_F0 = script_2496(local_F0);
  local_F0 = 120.0f;
  local_F0 = script_2654(local_F0);
  Script_scenCreateTask(script_2FDD, 3, local_EF, 127, 2);
  local_F0 = script_2859(local_F0);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  local_F0 = script_28FB(local_F0);
  if (task_1D & 0x40) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ExecuteAicaCommand(0, local_EF);
  Script_CreateNPCImageModule(SHEN_NAME('BIKS'), 0, 0);
  Script_CreateNPCImageModule(SHEN_NAME('BIKS'), "BIKS504G", 0);
  Script_charaDisplay(SHEN_NAME('SHD1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SHD2'), CDM_Show);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 48.0f);
  return;
}

static float script_2496(float arg0) {
  Vector3 local_F4;
  Script_executeAicaCommand(197801, 0, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  local_F4.x = 260.14f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -3248.6f;
  {
    Vector3i local_E8;
    local_E8.x = (&local_E8)->z = 0;
    (&local_E8)->y = 60074;
    Script_EVT_SetPosition(SHEN_NAME('DMB1'), 0x78000000, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('DMB1'), 0x78000000, &local_E8, 0);
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 0.0f;
  {
    Vector3i local_E8;
    local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('BIKS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('BIKS'), 0x38000000, &local_E8, 0);
    local_F4.x = -0.0439f;
    (&local_F4)->y = 0.0146f;
    (&local_F4)->z = 0.215f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_scenCreateTask(script_2D56, 0);
  Script_seqEnterEvtCam(13, CHARA_AKIR, CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, 20993, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.5f);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  while (1) {
    Script_EVT_GetPosition(SHEN_NAME('DMB0'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_F4)->z >= -3279.8f)) {
      break;
    }
    script_160E();
    script_2952(0);
    script_2A28(arg0, 0);
    script_2ABB(arg0);
    SCNF_WaitFrames_0000(1);
    script_2BAB();
  }
  return arg0;
}

static float script_2654(float arg0) {
  Vector3 local_F4;
  Script_executeAicaCommand(328873, 0, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  local_F4.x = 277.83f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -3279.8f;
  {
    Vector3i local_E8;
    local_E8.x = (&local_E8)->z = 0;
    (&local_E8)->y = 64079;
    Script_EVT_SetPosition(SHEN_NAME('DMB1'), 0x78000000, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('DMB1'), 0x78000000, &local_E8, 0);
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 0.0f;
  {
    Vector3i local_E8;
    local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('BIKS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('BIKS'), 0x38000000, &local_E8, 0);
    local_F4.x = -0.0397f;
    (&local_F4)->y = 0.092f;
    (&local_F4)->z = 0.1971f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_scenCreateTask(script_302E, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 20993, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 2.0f);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) > 42.0f) {
      break;
    }
    script_160E();
    script_2952(0);
    script_2A28(arg0, 0);
    script_2ABB(arg0);
    SCNF_WaitFrames_0000(1);
    script_2B1D();
  }
  return arg0;
}

static void script_27F6(float arg0) {
  int32_t local_FC;
  Script_executeAicaCommand(2464, 3, 30);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
  local_FC = 30;
  while (local_FC) {
    script_160E();
    script_2952(0);
    script_2A28(arg0, 0);
    script_2ABB(arg0);
    local_FC = local_FC - 1;
    SCNF_WaitFrames_0000(1);
    script_2B1D();
  }
  (void)arg0;
  return;
}

static float script_2859(float arg0) {
  int32_t local_FC;
  float local_F8;
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
  local_FC = 60;
  local_F8 = 0.01f;
  while (local_FC) {
    Script_SetLampState(1, SHEN_NAME('BIKS'), 128);
    script_160E();
    script_2952((int32_t)(local_F8 / 360.0f * (float)0x10000));
    script_2A28(arg0, (int32_t)(((float)0 - local_F8) / 360.0f * (float)0x10000));
    script_2ABB(arg0 + 10.0f);
    local_F8 = local_F8 + 0.01f;
    local_FC = local_FC - 1;
    SCNF_WaitFrames_0000(1);
    script_2B1D();
  }
  return arg0;
}

static float script_28FB(float arg0) {
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    script_160E();
    script_2952(0);
    script_2A28(arg0, 0);
    script_2ABB(arg0);
    SCNF_WaitFrames_0000(1);
    script_2B1D();
  }
  return arg0;
}

static void script_2952(int32_t arg0) {
  {
    Vector3i local_F4;
    float local_F0;
    local_F0 = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR);
    Script_mtwkGetSetRotation(SHEN_NAME('BIKS'), &local_F4, 0);
    if (!(local_F0 > 42.0f)) {
      (&local_F4)->z = (int32_t)(local_F0 / 360.0f * (float)0x10000);
    } else {
      if (!(local_F0 < 46.0f)) {
        local_F0 = local_F0 - 4.0f;
        if (!(local_F0 >= 84.0f)) {
          (&local_F4)->z = (int32_t)((84.0f - local_F0) / 360.0f * (float)0x10000);
        } else {
          (&local_F4)->z = 0;
        }
      }
    }
    Script_EVT_SetRotation(SHEN_NAME('BIKS'), 0x38000000, &local_F4, 0);
  }
  {
    Vector3i local_F4;
    Script_mtwkGetSetRotation(SHEN_NAME('DMB1'), &local_F4, 0);
    (&local_F4)->y = (&local_F4)->y + arg0 + 145;
    Script_EVT_SetRotation(SHEN_NAME('DMB1'), 0x38000000, &local_F4, 0);
  }
  return;
}

static void script_2A28(float arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  Script_mtwkGetSetRotation(SHEN_NAME('DMB1'), &local_E8, 0x40000000);
  (&local_E8)->y = (&local_E8)->y - arg1;
  arg0 = arg0 / 108.0f;
  local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
  local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
  Script_EVT_GetPosition(SHEN_NAME('DMB1'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  local_F4.x = (&local_F4)->x - arg0 * local_E4;
  (&local_F4)->z = (&local_F4)->z - arg0 * local_E0;
  Script_EVT_SetPosition(SHEN_NAME('DMB1'), 0x78000000, &local_F4);
  return;
}

static void script_2ABB(float arg0) {
  Vector3i local_F4;
  float local_F0;
  local_F0 = arg0 / 176.342407f;
  local_F4.x = 0x10000 - (int32_t)(local_F0 * 360.0f / 360.0f * (float)0x10000);
  (&local_F4)->y = (&local_F4)->z = 0;
  Script_charaPosRotUtils(SHEN_NAME('BIKS'), 23, 0, &local_F4, CHARA_PART_Add);
  Script_charaPosRotUtils(SHEN_NAME('BIKS'), 18, 0, &local_F4, CHARA_PART_Add);
  return;
}

static void script_2B1D(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Script_EVT_GetPosition(CHARA_AKIR, 33, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_GetPosition(SHEN_NAME('DMB2'), -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  local_F4.x = (&local_F4)->x - ((&local_E8)->x - (&local_DC)->x);
  (&local_F4)->y = (&local_F4)->y - ((&local_E8)->y - (&local_DC)->y);
  (&local_F4)->z = (&local_F4)->z - ((&local_E8)->z - (&local_DC)->z);
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F4);
  return;
}

static void script_2BAB(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Script_EVT_GetPosition(CHARA_AKIR, 17, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_GetPosition(SHEN_NAME('DMB3'), -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  local_F4.x = (&local_F4)->x - ((&local_E8)->x - (&local_DC)->x);
  (&local_F4)->y = (&local_F4)->y - ((&local_E8)->y - (&local_DC)->y);
  (&local_F4)->z = (&local_F4)->z - ((&local_E8)->z - (&local_DC)->z);
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F4);
  return;
}

static void script_2C39(void) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(SHEN_NAME('DMB2'), -1, &local_F4, 0);
    Script_SetMotnFlags(CHARA_AKIR, 17, &local_F4, 0x38000000);
  }
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(SHEN_NAME('DMB3'), -1, &local_F4, 0);
    Script_SetMotnFlags(CHARA_AKIR, 17, &local_F4, 0x38000000);
  }
  return;
}

static void script_2C7E(void) {
  SCNF_WaitFrames_0000(1);
  while (Script_GetCurrentFrameCount() < 45) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(5, 0, 255, 255, 255, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(10, 255, 255, 255, 255, 0, 255, 255, 255);
  Script_scenCreateTask(script_2DA3, 9, CHARA_AKIR, SHEN_NAME('BIKS'), 0x5454454D, SHEN_NAME('DKI1'), SHEN_NAME('DBI1'), SHEN_NAME('DHE1'), 20, 0x3F800000, 0x3D4CCCCD);
  Script_scenCreateTask(script_2DA3, 9, SHEN_NAME('DKI1'), SHEN_NAME('DBI1'), 0x31454844, SHEN_NAME('DKI2'), SHEN_NAME('DBI2'), SHEN_NAME('DHE2'), 15, 0x3F666666, 0x3D75C28F);
  return;
}

static int32_t script_2D56(void) {
  Script_filmCheckOrSetTransitionUtil(5, 0, 200, 200, 200, 200, 200, 200, 200);
  SCNF_WaitFrames_0000(5);
  Script_filmCheckOrSetTransitionUtil(10, 200, 200, 200, 200, 0, 200, 200, 200);
  SCNF_WaitFrames_0000(10);
  end:;
}

static int32_t script_2DA3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, float arg7, float arg8) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  Vector3i local_C4;
  Vector3i local_B8;
  float local_B4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0x40000000);
  Script_EVT_GetPosition(arg1, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_C4, 0x40000000);
  local_B4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
  Script_charaDisplay(arg3, CDM_Show);
  Script_scenToggleVisibleFor(arg3, 1);
  Script_charaDisplay(arg4, CDM_Show);
  Script_scenToggleVisibleFor(arg4, 1);
  Script_charaDisplay(arg5, CDM_Show);
  Script_scenToggleVisibleFor(arg5, 1);
  Script_ccowUtils(arg5, CCOW_DisableAll);
  Script_PlayCharaMotion(arg3, 20993, ((scn3_bits){.f = local_B4}).i, ((scn3_bits){.f = local_B4}).i, ((scn3_bits){.f = local_B4}).i, 0.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(arg3, 5, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_SetPosition(arg5, POS_XYZ, &local_DC);
  local_DC.x = 0.014f;
  (&local_DC)->y = 0.003f;
  (&local_DC)->z = 0.152f;
  local_B8.x = 16560;
  (&local_B8)->y = 0;
  (&local_B8)->z = 48261;
  Script_fixoAttachEx(arg5, arg3, 5, &local_DC, &local_B8);
  Script_motmToggleFlags(arg3, SCEN_Disable, 0x40);
  while (arg6) {
    if (!(arg7 < 0.0f)) {
      Script_charaSetRenderOpacity(arg3, arg7);
      Script_charaSetRenderOpacity(arg4, arg7);
      Script_charaSetRenderOpacity(arg5, arg7);
    } else {
      Script_charaSetRenderOpacity(arg3, 0.0f);
      Script_charaSetRenderOpacity(arg4, 0.0f);
      Script_charaSetRenderOpacity(arg5, 0.0f);
    }
    Script_PlayCharaMotion(arg3, 20993, ((scn3_bits){.f = local_B4}).i, ((scn3_bits){.f = local_B4}).i, ((scn3_bits){.f = local_B4}).i, 0.0f, 0);
    Script_EVT_SetPosition(arg3, 0x78000000, &local_F4);
    Script_EVT_SetRotation(arg3, 0x78000000, &local_D0, 0);
    Script_EVT_SetPosition(arg4, 0x78000000, &local_E8);
    Script_EVT_SetRotation(arg4, 0x78000000, &local_C4, 0);
    Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_D0, 0x40000000);
    Script_EVT_GetPosition(arg1, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg1, &local_C4, 0x40000000);
    local_B4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    arg6 = arg6 - 1;
    arg7 = arg7 - arg8;
    SCNF_WaitFrames_0000(1);
  }
  Script_charaDisplay(arg3, CDM_Hide);
  Script_scenToggleVisibleFor(arg3, 0);
  Script_charaDisplay(arg4, CDM_Hide);
  Script_scenToggleVisibleFor(arg4, 0);
  Script_fixoClearAttachment(arg5);
  Script_charaDisplay(arg5, CDM_Hide);
  return Script_scenToggleVisibleFor(arg5, 0);
}

static int32_t script_2FDD(int32_t arg0, int32_t arg1, int32_t arg2) {
  while (arg1) {
    Script_executeAicaCommand(4261, arg0, arg1);
    if (arg1 > arg2) {
      arg1 = arg1 - arg2;
    } else {
      arg1 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(262304, 0, 0);
  return (task_1D = 0) & 1;
}

static int32_t script_302E(void) {
  Script_seqEnterEvtCam(12, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(30);
  Script_scenCreateTask(script_2DA3, 9, CHARA_AKIR, SHEN_NAME('BIKS'), 0x5454454D, SHEN_NAME('DKI1'), SHEN_NAME('DBI1'), SHEN_NAME('DHE1'), 20, 0x3F800000, 0x3D4CCCCD);
  Script_scenCreateTask(script_2DA3, 9, SHEN_NAME('DKI1'), SHEN_NAME('DBI1'), 0x31454844, SHEN_NAME('DKI2'), SHEN_NAME('DBI2'), SHEN_NAME('DHE2'), 15, 0x3F666666, 0x3D75C28F);
  SCNF_WaitFrames_0000(2);
  Script_seqEnterEvtCam(11, CHARA_AKIR, CHARA_AKIR);
  end:;
}
