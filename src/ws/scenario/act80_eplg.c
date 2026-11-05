#include "act80_eplg.h"

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

static int32_t script_0159(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
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

static int32_t script_0373(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  end:;
}

static void script_03A3(int32_t arg0) {
  (void)((task_2 = -1) & 8);
  Script_ryoSetChara(arg0);
  Script_SetShouldSleepFlag(0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoControl(RYO_RestoreControl);
  SCNF_WaitFrames_0000(1);
  Script_CPCTRelated(arg0, 32, 89);
  Script_clmdSetFlags(arg0, 8, 1);
  (void)((task_0 = 0) & 8);
  return;
}

static void script_0435(void) {
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  return;
}

static void script_046A(void) {
  int8_t local_FF;
  (void)((local_FF = 0) & 8);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_shenClearFreeState(0);
  if (Script_ryoControl(PCCTL_QueryShouldRun)) {
    (void)((local_FF = -1) & 8);
    Script_ryoControl(PCCTL_RequestNewMode, 3);
    Script_ryoControl(PCCTL_DisablePlayerControl);
  }
  Script_executeAicaCommand(2720, 1, 15);
  Script_executeAicaCommand(2720, 5, 15);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (local_FF & 0x80) {
    Script_ryoControl(PCCTL_RequestNewMode, 0);
  }
  return;
}

static void script_04F6(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  float local_F0;
  if (~task_2) {
  } else {
    Script_ReturnTimeDate(&local_F4);
    if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 0)) {
      Script_shenClearFreeState(0);
      Script_ryoControl(PCCTL_RequestNewMode, 1);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      Script_SetActionSelectorHUDElementVisibility(1);
      Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      Script_GetConversationDetails(CONV_DETAILS_MODE_CLOSE_CONVERSATION_UI);
      Script_sicnUtils(1, 0, 0);
      SCNF_RequestIconNotification(0, 0, 0, 2);
      Script_GetConversationDetails(CONV_DETAILS_MODE_RESET_STATE);
      Script_GetConversationDetails(CONV_DETAILS_MODE_RESET_FOR_CPCT);
      Script_GetConversationDetails(CONV_DETAILS_MODE_UNUSED_3);
      Script_GetConversationDetails(CONV_DETAILS_MODE_SYNC_CASH_OVERLAY);
      Script_SetGameStateTo(AUTH_Scripted);
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      local_F0 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (!(local_F0 >= 1.0f)) {
        Script_ShowUIMessageFromID("TS1003A001");
      } else if (!(local_F0 >= 2.0f)) {
        Script_ShowUIMessageFromID("TS1003A002");
      } else {
        Script_ShowUIMessageFromID("TS1003A003");
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_uiUtils(5, 60, 2);
      SCNF_WaitFrames_0000(210);
      Script_uiUtils(7, 60, 2);
      Script_executeAicaCommand(2720, 5, 30);
      SCNF_WaitFrames_0000(60);
      SCNF_WaitFrames_0000(5);
      Script_ryoControl(PCCTL_RequestNewMode, 0);
      MODU_Script_shenRequestGameModule(Display, 80, 0x4E4B4954, 0);
    }
  }
  return;
}

static void script_0661(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  if (arg0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, arg1)) goto L000747;
    Script_ReturnTimeDate(&local_F4);
    if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 0)) goto L000747;
    Script_sicnUtils(0, 0, 1);
    (void)((task_0 = -1) & 8);
    (void)((task_1 = -1) & 8);
    while (1) {
      if (task_0 == 0) {
        break;
      }
      local_F0 = Script_GetControllerInput(1);
      if (task_1 & 2) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
          if (local_F0 & 0x800) {
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, arg1, 1);
            Script_sprintf("save!\012");
            Script_CaptureTrialSaveState();
            break;
          }
        }
      }
      SCNF_WaitFrames_0000(1);
    }
    if (task_0 & 1) {
      Script_sicnUtils(1, 0, 0);
      (void)((task_0 = 0) & 2);
    }
  } else {
    Script_sicnUtils(0, 0, 0);
  }
  L000747:;
  return;
}

static void script_0748(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_0770(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_07C4(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_0814(int32_t arg0) {
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

static void script_08BB(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_0814(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_08DE(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_0911(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_08DE(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_0934(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_08DE(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_0993(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static int32_t script_09A8(void) {
  for (;;) {
    task_8 = task_8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_09C1(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  return;
}

static void script_09F0(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  return;
}

static void script_0A29(void) {
  Script_SetupLightFromTable(3);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_EIIU, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 4);
  Script_SetShadowFlags(CHARA_EIIU, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 4);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 4);
  Script_SetVector4ByOffset(&data_2A68);
  Script_refbSetActive(1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2A68);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_STSI, &data_2A68);
  Script_charaSetRefboardFlagTo(CHARA_STSI, 1);
  Script_SetCharaRefBoardPos(CHARA_EIIU, &data_2A68);
  Script_charaSetRefboardFlagTo(CHARA_EIIU, 1);
  return;
}

static void script_0AF4(void) {
  script_09F0(CHARA_AKIR);
  script_09F0(CHARA_EIIU);
  script_09C1(CHARA_STSI);
  Script_SetMotnFlag(CHARA_STSI, -1);
  Script_motmToggleFlags(CHARA_STSI, SCEN_Disable, 0x40);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  script_0A29();
  Script_handSetFloatsFor(CHARA_EIIU, 0, data_2984, 10);
  Script_handSetFloatsFor(CHARA_EIIU, 1, data_2984, 10);
  Script_mapmSetRenderFlag(1, 0);
  return;
}

static int32_t script_0B71(void) {
  for (;;) {
    {
      int32_t unused_F4[3];
      int32_t local_F0;
      local_F0 = task_8;
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
      local_F0 = task_8;
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
        case 272:
          Script_FACE_LCG(CHARA_EIIU, FACE_BlinkOpen, 1, 0);
          break;
        case 276:
          Script_FACE(CHARA_EIIU, 4, 0, 55);
          break;
        case 333:
          Script_FACE_LCG(CHARA_EIIU, FACE_BlinkAutomatic, 5, 0);
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
      local_F0 = task_8;
      if (local_F0 == 1391) {
        Script_handSetFloatsFor(CHARA_EIIU, 1, data_1EAC, 15);
        goto L000D92;
      }
    }
    L000D92:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0D9E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_EC[4];
  Vector3i local_E0;
  int32_t local_DC;
  script_0030();
  script_1061();
  if (arg0) {
    Script_scenCreateTask(script_0159, 1, 60);
  }
  Script_shenSeqUtils(arg1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_SetMaxInterpSpeed(0.0f);
        Script_SetMotnFlag(CHARA_EIIU, -1);
        Script_ResetHandMotion(CHARA_EIIU, 1, 1, 0);
        Script_ResetHandMotion(CHARA_EIIU, 1, 0, 0);
        local_E0.x = 910;
        (&local_E0)->y = -1456;
        (&local_E0)->z = 182;
        Script_SetHandRootRotation(CHARA_EIIU, 1, 0x15, &local_E0);
        Script_handSetFloatsFor(CHARA_EIIU, 1, data_2B5C, 1);
        Script_handSetFloatsFor(CHARA_EIIU, 0, data_2A78, 1);
        Script_FACE_LCG(CHARA_EIIU, FACE_BlinkClose, 1, 0);
        break;
      case 140:
        Script_FACE(CHARA_EIIU, 10, 0, 4);
        break;
      case 144:
        Script_FACE_LCG(CHARA_EIIU, FACE_BlinkOpen, 15, 0);
        break;
      case 160:
        Script_FACE(CHARA_EIIU, 4, 0, 18);
        break;
      case 166:
        Script_FACE(CHARA_EIIU, 10, 0, 8);
        break;
      case 170:
        Script_handSetFloatsFor(CHARA_EIIU, 0, data_2320, 10);
        break;
      case 182:
        Script_handSetFloatsFor(CHARA_EIIU, 1, data_223C, 40);
        break;
      case 210:
        Script_FACE(CHARA_EIIU, 0, 0, 30);
        break;
      case 226:
        Script_FACE(CHARA_EIIU, 7, 0, 8);
        break;
      case 228:
        Script_scenCreateTask(script_0373, 2, CHARA_EIIU, 2);
        break;
      case 234:
        Script_FACE(CHARA_EIIU, 0, 0, 12);
        break;
      case 340:
        Script_handSetFloatsFor(CHARA_EIIU, 0, data_2404, 8);
        break;
      case 368:
        Script_handSetFloatsFor(CHARA_EIIU, 0, data_1F90, 4);
        break;
      case 430:
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 500:
        Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1061(void) {
  int32_t unused_F0[4];
  script_0041(CHARA_EIIU);
  Script_facemotUtils(CHARA_EIIU, 1, 0);
  Script_ResetHandMotion(CHARA_EIIU, 0, 1, 0);
  Script_ResetHandMotion(CHARA_EIIU, 0, 0, 0);
  return;
}

static void script_109D(void) {
  script_00DB(CHARA_EIIU);
  Script_ResetHandMotion(CHARA_EIIU, 0, 1, 0);
  Script_ResetHandMotion(CHARA_EIIU, 0, 0, 0);
  return;
}

static void script_10CC(int32_t arg0, int32_t arg1) {
  if (-1) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L0010F0;
    }
  }
  L0010F0:;
  Script_scenToggleVisibleFor(arg0, 0);
  script_1D82(arg0, arg1);
  return;
}

static int32_t script_1105(int32_t arg0, int32_t arg1) {
  task_10 = script_1D12(arg0);
  Script_SetCPAMFlags(arg0, 0);
  SCNF_WaitFrames_0000(1);
  if (arg1 & 1) {
    if (arg1 == 5) {
      script_0748(arg0, 0.1219f, 0.0f, 0.0087f);
    } else {
      script_0748(arg0, 0.0f, 0.0f, 0.0f);
    }
    script_0770(arg0, 0.0f, 0.0f, 0.0f);
  } else {
    script_0748(arg0, 100.0f, 0.0f, 0.0f);
    script_0770(arg0, 0.0f, 0.0f, 0.0f);
  }
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  end:;
}

static void script_11AE(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  if (arg2) {
    Script_scenCreateTask(script_1105, 2, arg2, task_4 + 1);
  }
  Script_seqEnterEvtCam(task_4, 0, AUTH_None);
  Script_motmUtilsEx(0, arg0, 0);
  local_FD = 10;
  local_F8 = task_4;
  if (local_F8 == 1) {
    Script_InitializeOsag(CHARA_TOM_);
    Script_PlayCharaMotion(arg0, arg1, 7.0f, 21.0f, 0.0f, 0.2f, 0);
    local_FF = 10;
    local_FE = 10;
  } else if (local_F8 == 3) {
    Script_PlayCharaMotion(arg0, arg1, 7.0f, 21.0f, 0.0f, 0.4f, 0);
    local_FF = 10;
    local_FE = 10;
  } else if (local_F8 == 4) {
    Script_PlayCharaMotion(arg0, arg1, 7.0f, 21.0f, 0.0f, 0.5f, 0);
    local_FF = 10;
    local_FE = 10;
  } else if (local_F8 == 5) {
    Script_PlayCharaMotion(arg0, arg1, 7.0f, 21.0f, 0.0f, 0.4f, 0);
    local_FF = 10;
    local_FE = 10;
  } else if (local_F8 == 6) {
    Script_PlayCharaMotion(arg0, arg1, 7.0f, 31.0f, 0.0f, 0.1f, 0);
    local_FF = 14;
    local_FD = 6;
    local_FE = 10;
  } else if (local_F8 == 7) {
    Script_PlayCharaMotion(arg0, arg1, 4.0f, 21.0f, 0.0f, 0.5f, 0);
    local_FF = 10;
    local_FE = 10;
  } else if (local_F8 == 8) {
    Script_PlayCharaMotion(arg0, arg1, 7.0f, 21.0f, 0.0f, 0.2f, 0);
    local_FF = 10;
    local_FE = 10;
  } else if (local_F8 == 9) {
    Script_PlayCharaMotion(arg0, arg1, 3.0f, 21.0f, 0.0f, 0.5f, 0);
    local_FF = 15;
    local_FE = 15;
  } else if (local_F8 == 10) {
    Script_PlayCharaMotion(arg0, arg1, 3.0f, 21.0f, 0.0f, 0.5f, 0);
    local_FF = 35;
    local_FE = 25;
  } else {
    Script_PlayCharaMotion(arg0, arg1, 7.0f, 21.0f, 0.0f, 0.2f, 0);
    local_FF = 10;
    local_FE = 10;
  }
  Script_filmCheckOrSetTransitionUtil(local_FF, 255, 255, 217, 182, 80, 255, 172, 15);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (task_3) {
    for (;;) {
      if (Script_GetInputData(3, 0) == 0x400) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  if (local_FD) {
    SCNF_WaitFrames_0000(local_FD);
  }
  Script_filmCheckOrSetTransitionUtil(local_FE, 80, 255, 172, 15, 255, 255, 217, 182);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  task_4 = task_4 + 1;
  if (task_4 > 10) {
    task_4 = 1;
  }
  script_10CC(arg0, task_C);
  task_C = task_10;
  return;
}

static void script_14B3(void) {
  Script_SetCurrentScrollEnabled(0);
  Script_SetCameraFlags(0);
  if (-1) {
    Script_charaDisplay(CHARA_EIIU, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_EIIU, CDM_Show);
      goto L0014EA;
    }
  }
  L0014EA:;
  Script_scenToggleVisibleFor(CHARA_EIIU, 0);
  Script_shenSetLightingMode(1);
  Script_lightSetSlotEnabled(0, 1);
  Script_LightUtils(0, 0, 2);
  Script_LightUtils(0, 4, 1.0f);
  Script_LightUtils(0, 8, &data_2C40);
  Script_LightUtils(0, 2, &data_2C68);
  Script_lightSetGlobalSceneLighting(&data_2C58);
  Script_SetFogColour(&data_2C78);
  SCNF_WaitFrames_0000(1);
  Script_refbSetActive(1);
  task_4 = 1;
  script_1105(CHARA_TOM_, 1);
  SCNF_WaitFrames_0000(1);
  Script_SetMITKFlags(StartFadeIn, 17, 1);
  Script_SetFogFlags(2, -9802);
  task_C = task_10;
  script_11AE(CHARA_TOM_, 1071, CHARA_MIKI);
  script_11AE(CHARA_MIKI, 1076, CHARA_KURI);
  script_11AE(CHARA_KURI, 1071, CHARA_GJHF);
  script_11AE(CHARA_GJHF, 1076, CHARA_TSUC);
  script_11AE(CHARA_TSUC, 1071, CHARA_HIRA);
  script_11AE(CHARA_HIRA, 1071, CHARA_NITO);
  script_11AE(CHARA_NITO, 1071, CHARA_MEGM);
  Script_LightUtils(0, 8, &data_2C4C);
  script_11AE(CHARA_MEGM, 1076, CHARA_ENKI);
  Script_LightUtils(0, 8, &data_2C40);
  script_11AE(CHARA_ENKI, 1071, CHARA_HRSK);
  script_11AE(CHARA_HRSK, 1076, 0);
  Script_filmCheckOrSetTransitionUtil(0, 0, 255, 255, 255, 225, 255, 255, 255);
  Script_uiUtils(1, 14, 0);
  Script_SetMITKFlags(StartFadeOut, 0, 1);
  Script_SetFogFlags(2, -1);
  SCNF_WaitFrames_0000(30);
  Script_SetCameraFlags(1);
  return;
}

__attribute__((scn_string_bindings(3, data_2CAA, 4, data_2CB7, 5, data_2CC5)))
void script_16B9(void) {
  int32_t local_FC;
  int8_t local_FB;
  Script_SetQteOverlayTimeoutTuning(QTE_TimeoutBegin);
  Script_LoadVoiceFile(1, "ALLVOICE");
  Script_motiLoad(80, SHEN_NAME('EPLG'), "M_01YUK.BIN");
  Script_LoadSND(0, 5, "a1_ytai2.snd", 0);
  SCNF_WaitFrames_0000(1);
  Script_SetActionSelectorHUDElementVisibility(1);
  local_FB = Script_GetCurrentEntryPoint();
  if ((local_FB == 1) | (local_FB == 2)) {
    if (-1) {
      Script_charaDisplay(CHARA_AKIR, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_AKIR, CDM_Show);
        goto L00173C;
      }
    }
    L00173C:;
    Script_scenToggleVisibleFor(CHARA_AKIR, 0);
    Script_SetMapControlTaskFlag(CHARA_STSI, 1);
    SCNF_WaitFrames_0000(1);
    Script_mapmSetRenderFlag(0, 0);
    SCNF_WaitFrames_0000(1);
    Script_LoadSND_Wrapper(0, 0, 0, "bgmtk003.snd", 0, 0, 0, 0);
    Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/80/TIKN", (int32_t)"SCRTEST.BIN", 0x474C5045);
    Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
    if (local_FB == 2) {
      task_3 = 1;
    } else {
      task_3 = 0;
    }
    for (;;) {
      script_14B3();
    }
  } else {
    task_3 = 0;
    Script_scenCreateTask(script_199D, 1, CHARA_AKIR);
    Script_scenCreateTask(script_18BE, 0);
    local_FC = 0;
    for (;;) {
      if (local_FC == 10) {
        if (-1) {
          Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_AKIR, CDM_Show);
            goto L001821;
          }
        }
        L001821:;
        Script_scenToggleVisibleFor(CHARA_AKIR, 0);
      }
      if (local_FC == 350) {
        if (0) {
          Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(CHARA_AKIR, CDM_Show);
            goto L00185F;
          }
        }
        L00185F:;
        Script_scenToggleVisibleFor(CHARA_AKIR, 1);
      }
      if (local_FC == 680) {
        if (-1) {
          Script_charaDisplay(CHARA_STSI, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_STSI, CDM_Show);
            goto L00189D;
          }
        }
        L00189D:;
        Script_scenToggleVisibleFor(CHARA_STSI, 0);
      }
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static int32_t script_18BE(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/80/EPLG", "MODELS") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, -1);
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 6, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2984, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2984, 10);
  end:;
}

__attribute__((scn_string_bindings(0, data_2CE7, 1, data_2CF4, 2, data_2D02)))
static int32_t script_199D(int32_t arg0) {
  int32_t unused_FC;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(CHARA_EIIU, 0, 0);
  Script_FaceEx(CHARA_EIIU, FACE_ExpressionEnabled, 0, 2);
  Script_facemotUtils(CHARA_EIIU, 1, 0);
  Script_ResetHandMotion(CHARA_EIIU, 1, 1, 0);
  Script_ResetHandMotion(CHARA_EIIU, 1, 0, 0);
  Script_WeatherUtils_ForFIXC(10);
  script_1AF9();
  Script_LoadSND_Wrapper(0, 0, 0, "bgmtk003.snd", 0, 0, 0, 0);
  Script_executeAicaCommand(81832, 0, 0);
  script_1BCF();
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/80/TIKN", (int32_t)"SCRTEST.BIN", 0x474C5045);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  script_14B3();
  SCNF_WaitFrames_0000(1);
  Script_mapmSetRenderFlag(1, 1);
  if (0) {
    Script_charaDisplay(CHARA_EIIU, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_EIIU, CDM_Show);
      goto L001A81;
    }
  }
  L001A81:;
  Script_scenToggleVisibleFor(CHARA_EIIU, 1);
  Script_filmCheckOrSetTransitionUtil(60, 255, 255, 255, 255, 0, 255, 255, 255);
  script_1CEE();
  script_1CD5();
  Script_uiUtils(5, 60, 3);
  SCNF_WaitFrames_0000(210);
  SCNF_WaitFrames_0000(300);
  Script_uiUtils(7, 60, 3);
  SCNF_WaitFrames_0000(60);
  Script_SetQteOverlayTimeoutTuning(QTE_TimeoutReset);
  MODU_Script_shenRequestGameModule(SetToDisplayModule, 0, 0, 0);
  end:;
}

static void script_1AF9(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_mapmSetCamFlags(2, 0, 0);
  script_0AF4();
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_8 = 0;
  local_FC = Script_scenCreateTask(script_0B71, 0);
  local_F8 = Script_scenCreateTask(script_09A8, 0);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 4, 20);
  Script_filmCheckOrSetTransitionUtil(20, 0, 255, 255, 255, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_executeAicaCommand(65696, 0, 0);
  Script_set_val_to_1(2, 0);
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  return;
}

static void script_1BCF(void) {
  Script_mapmSetRenderFlag(0, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 0);
  Script_faceUpdateFor(CHARA_AKIR, 0, 0);
  Script_faceUpdateFor(CHARA_AKIR, 1, 0);
  if (-1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L001C2D;
    }
  }
  L001C2D:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 0);
  Script_SetMapControlTaskFlag(CHARA_STSI, 1);
  Script_SetMapControlTaskFlag(SHEN_NAME('DOOR'), 1);
  Script_SetMapControlTaskFlag(SHEN_NAME('SLTM'), 1);
  Script_SetMapControlTaskFlag(SHEN_NAME('DRAM'), 1);
  Script_SetMapControlTaskFlag(SHEN_NAME('DART'), 1);
  SCNF_WaitFrames_0000(1);
  Script_maprLoadRes(7, 0);
  Script_CopyAndModifyChid("KMA_L");
  Script_CopyAndModifyChid("DOOR");
  Script_CopyAndModifyChid("SLTM");
  Script_CopyAndModifyChid("DRAM");
  Script_CopyAndModifyChid("DATK30NG");
  return;
}

static int32_t script_1CB4(void) {
  SCNF_WaitFrames_0000(13);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 3, 96);
  end:;
}

static void script_1CD5(void) {
  Script_scenCreateTask(script_1CB4, 0);
  script_0D9E(0, 1);
  return;
}

static void script_1CEE(void) {
  Script_SetFogColour(&data_2D3C);
  Script_SetupLightFromTable(2);
  Script_refbSetActive(1);
  Script_SetVector4ByOffset(&data_2D2C);
  return;
}

static int32_t script_1D12(int32_t arg0) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
  while (local_FC == 0) {
    Script_sprintf("読み込み失敗\012");
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
  }
  L001D61:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L001D61;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return local_FC;
}

static void script_1D82(int32_t arg0, int32_t arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  return;
}
