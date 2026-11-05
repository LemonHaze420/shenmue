#include "act02_arar.h"

static void script_0030(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_0058(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_00AC(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_00FC(int32_t arg0) {
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

static void script_01A3(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_00FC(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_01C6(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_01F9(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_01C6(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_021C(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_01C6(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_027B(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_0290(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_0299(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_02A8(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L0002D4;
    }
  }
  L0002D4:;
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

static void script_0335(int32_t arg0) {
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

static void script_0385(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_03B3(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_03E1(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0415(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0449(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0467(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_04A1(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_04D5(void) {
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

static void script_05CD(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_05FD(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetCharaRefBoardPos(arg0, &data_5400);
  Script_charaSetRefboardFlagTo(arg0, 1);
  return;
}

static void script_0660(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_5070, 1);
  Script_handSetFloatsFor(arg0, 0, data_5070, 1);
  Script_SetCharaFlags(arg0, 4);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetCharaRefBoardPos(arg0, &data_5400);
  Script_charaSetRefboardFlagTo(arg0, 1);
  return;
}

static void script_0731(void) {
  int32_t unused_F4[3];
  script_0660(CHARA_AKIR);
  script_0660(CHARA_JIMY);
  script_0660(CHARA_KJHS);
  script_0660(CHARA_ETKO);
  script_05FD(CHARA_MNWA);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  return;
}

static int32_t script_078B(void) {
  for (;;) {
    task_4 = task_4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_07A4(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 2, "bgm013.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(9640, 0, 0);
  Script_executeAicaCommand(1184, 2, 107);
  Script_LoadSND(4, 0, 0, 0);
  return;
}

static int32_t script_0805(void) {
  Vector3 local_F4;
  if (-1) {
    Script_charaDisplay(CHARA_ETKO, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_ETKO, CDM_Show);
      goto L00082F;
    }
  }
  L00082F:;
  Script_scenToggleVisibleFor(CHARA_ETKO, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF1'), CDM_Show);
      goto L000863;
    }
  }
  L000863:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF2'), CDM_Show);
      goto L000897;
    }
  }
  L000897:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF3'), CDM_Show);
      goto L0008CB;
    }
  }
  L0008CB:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Show);
      goto L0008FF;
    }
  }
  L0008FF:;
  Script_scenToggleVisibleFor(SHEN_NAME('MAGA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Show);
      goto L000933;
    }
  }
  L000933:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PURS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PURS'), CDM_Show);
      goto L000967;
    }
  }
  L000967:;
  Script_scenToggleVisibleFor(SHEN_NAME('PURS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('COMP'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
      goto L00099B;
    }
  }
  L00099B:;
  Script_scenToggleVisibleFor(SHEN_NAME('COMP'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MONY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MONY'), CDM_Show);
      goto L0009CF;
    }
  }
  L0009CF:;
  Script_scenToggleVisibleFor(SHEN_NAME('MONY'), 0);
  for (;;) {
    {
      int32_t local_F0;
      local_F0 = task_4;
      switch (local_F0) {
        case 1:
          Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 691:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_4F8C, 0);
          break;
        case 831:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
        case 1379:
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_5154, 1);
          break;
        case 1524:
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_5070, 30);
          break;
        case 1560:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1388:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_5154, 1);
          break;
        case 1535:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_5070, 30);
          break;
        case 1570:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
        case 1709:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_5070, 1);
          break;
        case 1710:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_54F4, 6);
          break;
        case 1716:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_5154, 4);
          break;
        case 1818:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_54F4, 3);
          break;
        case 1824:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_4;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 1);
          break;
        case 305:
          Script_FACE(CHARA_AKIR, 7, 0, 10);
          break;
        case 431:
          Script_FACE(CHARA_AKIR, 2, 0, 1);
          break;
        case 633:
          Script_FACE(CHARA_AKIR, 10, 0, 10);
          break;
        case 976:
          Script_FACE(CHARA_AKIR, 0, 0, 1);
          break;
        case 1169:
          Script_FACE(CHARA_AKIR, 2, 0, 1);
          break;
        case 1212:
          Script_FACE(CHARA_AKIR, 4, 0, 6);
          break;
        case 1251:
          Script_FACE(CHARA_AKIR, 5, 0, 1);
          break;
        case 1824:
          Script_FACE(CHARA_AKIR, 0, 0, 1);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_4;
      switch (local_F0) {
        case 691:
          Script_clmdSetFlags(CHARA_AKIR, 2, 1);
          break;
        case 830:
          Script_clmdSetFlags(CHARA_AKIR, 2, 0);
          break;
      }
    }
    if (task_4 == 1) {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    } else if ((task_4 >= 1203) & (task_4 <= 1211)) {
      Script_EVT_GetPosition(CHARA_KJHS, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 8);
    } else {
      if (task_4 >= 1624) {
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        goto L000D9A;
      }
    }
    L000D9A:;
    {
      int32_t local_F0;
      local_F0 = task_4;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 321:
          Script_FACE(CHARA_JIMY, 10, 0, 1);
          break;
        case 431:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 624:
          Script_FACE(CHARA_JIMY, 2, 0, 8);
          break;
        case 831:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 976:
          Script_FACE(CHARA_JIMY, 5, 0, 1);
          break;
        case 1169:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 1251:
          Script_FACE(CHARA_JIMY, 1, 2, 1);
          break;
        case 1369:
          Script_FACE(CHARA_JIMY, 7, 0, 1);
          break;
        case 1709:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 1725:
          Script_FACE(CHARA_JIMY, 4, 1, 1);
          break;
        case 1817:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
        case 1824:
          Script_FACE(CHARA_JIMY, 4, 0, 1);
          break;
        case 2083:
          Script_FACE(CHARA_JIMY, 0, 0, 1);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_4;
      switch (local_F0) {
        case 1:
          Script_ResetHandMotion(CHARA_JIMY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_JIMY, 0, 0, 0);
          Script_CharHandlers(CHARA_JIMY, 2, 8, 16);
          break;
        case 1725:
          Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_JIMY, 1, data_5070, 1);
          Script_handSetFloatsFor(CHARA_JIMY, 0, data_5070, 1);
          break;
        case 1816:
          Script_ResetHandMotion(CHARA_JIMY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_JIMY, 0, 0, 0);
          break;
        case 1824:
          Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_JIMY, 1, data_5070, 1);
          Script_handSetFloatsFor(CHARA_JIMY, 0, data_5070, 1);
          break;
      }
    }
    if (task_4 == 1) {
      Script_CharHandlers(CHARA_KJHS, 2, 8, 16);
      Script_CharHandlers(CHARA_MNWA, 2, 8, 16);
    }
    {
      int32_t local_F0;
      local_F0 = task_4;
      switch (local_F0) {
        case 1824:
          Script_SetAnimatedCameraFov(StartTransition, 0x43340000, 800);
          break;
        case 2082:
          Script_SetAnimatedCameraFov(0, 0, 0);
          break;
      }
    }
    if (task_4 == 1381) {
      Script_executeAicaCommand(81832, 0, 0);
      Script_executeAicaCommand(1184, 2, 127);
    }
    if (task_4 == 2022) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1133(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  if (-1) {
    Script_charaDisplay(CHARA_ETKO, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_ETKO, CDM_Show);
      goto L001169;
    }
  }
  L001169:;
  Script_scenToggleVisibleFor(CHARA_ETKO, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF1'), CDM_Show);
      goto L00119D;
    }
  }
  L00119D:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF2'), CDM_Show);
      goto L0011D1;
    }
  }
  L0011D1:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF3'), CDM_Show);
      goto L001205;
    }
  }
  L001205:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Show);
      goto L001239;
    }
  }
  L001239:;
  Script_scenToggleVisibleFor(SHEN_NAME('MAGA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Show);
      goto L00126D;
    }
  }
  L00126D:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PURS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PURS'), CDM_Show);
      goto L0012A1;
    }
  }
  L0012A1:;
  Script_scenToggleVisibleFor(SHEN_NAME('PURS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('COMP'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
      goto L0012D5;
    }
  }
  L0012D5:;
  Script_scenToggleVisibleFor(SHEN_NAME('COMP'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MONY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MONY'), CDM_Show);
      goto L001309;
    }
  }
  L001309:;
  Script_scenToggleVisibleFor(SHEN_NAME('MONY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
      goto L00133D;
    }
  }
  L00133D:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKE2'), 0);
  if (-1) {
    Script_charaDisplay(CHARA_KJHS, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KJHS, CDM_Show);
      goto L001371;
    }
  }
  L001371:;
  Script_scenToggleVisibleFor(CHARA_KJHS, 0);
  if (-1) {
    Script_charaDisplay(CHARA_MNWA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MNWA, CDM_Show);
      goto L0013A5;
    }
  }
  L0013A5:;
  Script_scenToggleVisibleFor(CHARA_MNWA, 0);
  Script_charaDisplay(SHEN_NAME('TKET'), CDM_Show);
  Script_scenToggleActiveFor(SHEN_NAME('TKET'), 0);
  Script_ccowSetHeightMode(SHEN_NAME('TKET'), CCOW_KeepHeight);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_5070, 1);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_5070, 1);
  Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
  Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_JIMY, 1, data_5070, 1);
  Script_handSetFloatsFor(CHARA_JIMY, 0, data_5070, 1);
  for (;;) {
    if (Script_shenSeqUtils(-1)) {
      {
        int32_t local_E4;
        local_E4 = task_4;
        switch (local_E4) {
          case 1:
            Script_FACE(CHARA_AKIR, 0, 0, 1);
            break;
          case 299:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 348:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 404:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 799:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 829:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 869:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 929:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 1094:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 1326:
            Script_FACE(CHARA_AKIR, 5, 2, 8);
            break;
          case 1349:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 1404:
            Script_FACE(CHARA_AKIR, 5, 5, 8);
            break;
          case 1449:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 1659:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
          case 1699:
            Script_FACE(CHARA_AKIR, 2, 0, 8);
            break;
          case 1938:
            Script_FACE(CHARA_AKIR, 5, 0, 8);
            break;
        }
      }
      {
        int32_t local_E4;
        local_E4 = task_4;
        switch (local_E4) {
          case 799:
            Script_handSetFloatsFor(CHARA_AKIR, 1, data_45C0, 8);
            break;
          case 1289:
            Script_handSetFloatsFor(CHARA_AKIR, 1, data_5070, 8);
            Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_5D04);
            Script_handSetFloatsFor(CHARA_AKIR, 0, data_56BC, 8);
            break;
          case 1809:
            Script_handSetFloatsFor(CHARA_AKIR, 0, data_5070, 8);
            break;
        }
      }
      {
        int32_t local_E4;
        local_E4 = task_4;
        switch (local_E4) {
          case 1:
            Script_FACE(CHARA_JIMY, 4, 0, 1);
            break;
          case 5:
            Script_FACE(CHARA_JIMY, 7, 0, 8);
            break;
          case 59:
            Script_FACE(CHARA_JIMY, 6, 0, 8);
            break;
          case 829:
            Script_FACE(CHARA_JIMY, 4, 2, 8);
            break;
          case 1209:
            Script_FACE(CHARA_JIMY, 4, 1, 8);
            break;
          case 1449:
            Script_FACE(CHARA_JIMY, 4, 0, 8);
            break;
          case 1528:
            Script_FACE(CHARA_JIMY, 4, 1, 8);
            break;
          case 1608:
            Script_FACE(CHARA_JIMY, 4, 0, 8);
            break;
          case 1809:
            Script_FACE(CHARA_JIMY, 4, 1, 8);
            break;
        }
      }
      {
        int32_t local_E4;
        local_E4 = task_4;
        switch (local_E4) {
          case 404:
            local_F4.x = 0.121f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = 0.076f;
            local_E8.x = 0;
            (&local_E8)->y = 0;
            (&local_E8)->z = 0x4000;
            Script_fixoAttachEx(SHEN_NAME('TKET'), CHARA_JIMY, 12, &local_F4, &local_E8);
            Script_handSetFloatsFor(CHARA_JIMY, 0, data_5410, 1);
            break;
          case 473:
            local_F4.x = 0.121f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -0.076f;
            local_E8.x = 0;
            (&local_E8)->y = 0;
            (&local_E8)->z = 0x4000;
            Script_handSetFloatsFor(CHARA_JIMY, 0, data_5070, 8);
            Script_fixoAttachEx(SHEN_NAME('TKET'), CHARA_AKIR, 18, &local_F4, &local_E8);
            Script_handSetFloatsFor(CHARA_AKIR, 1, data_5410, 1);
            break;
          case 568:
            Script_charaDisplay(SHEN_NAME('TKET'), CDM_Hide);
            Script_handSetFloatsFor(CHARA_AKIR, 1, data_5070, 8);
            break;
        }
      }
      if (task_4 == 1960) {
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(45, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1948(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int16_t local_CE;
  local_CE = 1;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  if (-1) {
    Script_charaDisplay(CHARA_ETKO, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_ETKO, CDM_Show);
      goto L001984;
    }
  }
  L001984:;
  Script_scenToggleVisibleFor(CHARA_ETKO, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF1'), CDM_Show);
      goto L0019B8;
    }
  }
  L0019B8:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF2'), CDM_Show);
      goto L0019EC;
    }
  }
  L0019EC:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF3'), CDM_Show);
      goto L001A20;
    }
  }
  L001A20:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Show);
      goto L001A54;
    }
  }
  L001A54:;
  Script_scenToggleVisibleFor(SHEN_NAME('MAGA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Show);
      goto L001A88;
    }
  }
  L001A88:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PURS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PURS'), CDM_Show);
      goto L001ABC;
    }
  }
  L001ABC:;
  Script_scenToggleVisibleFor(SHEN_NAME('PURS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('COMP'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
      goto L001AF0;
    }
  }
  L001AF0:;
  Script_scenToggleVisibleFor(SHEN_NAME('COMP'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MONY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MONY'), CDM_Show);
      goto L001B24;
    }
  }
  L001B24:;
  Script_scenToggleVisibleFor(SHEN_NAME('MONY'), 0);
  if (-1) {
    Script_charaDisplay(CHARA_KJHS, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KJHS, CDM_Show);
      goto L001B58;
    }
  }
  L001B58:;
  Script_scenToggleVisibleFor(CHARA_KJHS, 0);
  if (-1) {
    Script_charaDisplay(CHARA_MNWA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MNWA, CDM_Show);
      goto L001B8C;
    }
  }
  L001B8C:;
  Script_scenToggleVisibleFor(CHARA_MNWA, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TKET'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TKET'), CDM_Show);
      goto L001BC0;
    }
  }
  L001BC0:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKET'), 0);
  Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
  Script_scenToggleActiveFor(SHEN_NAME('TKET'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('TKE2'), 0);
  Script_ccowSetHeightMode(SHEN_NAME('TKET'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('TKE2'), CCOW_KeepHeight);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
  Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_CE) {
        case 1:
          Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_5154, 10);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_5154, 10);
          Script_handSetFloatsFor(CHARA_JIMY, 0, data_5070, 10);
          Script_handSetFloatsFor(CHARA_JIMY, 1, data_5070, 10);
          break;
        case 541:
          Script_SetNewHandMotion(CHARA_AKIR, 0, 621, 0, 0, 0, 0, 1.0f, 0);
          break;
      }
      switch (local_CE) {
        case 1:
          local_F4.x = 1.89f;
          (&local_F4)->y = 0.75f;
          (&local_F4)->z = -4.7f;
          Script_EVT_SetPosition(SHEN_NAME('TKE2'), POS_XYZ, &local_F4);
          local_D0.x = 0;
          (&local_D0)->y = 0;
          (&local_D0)->z = 0;
          Script_EVT_SetRotation(SHEN_NAME('TKE2'), 0x38000000, &local_D0, 0);
          Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
          break;
        case 473:
          local_D0.x = -7462;
          (&local_D0)->y = -8372;
          (&local_D0)->z = 5095;
          Script_SetHandRootRotation(CHARA_JIMY, 1, 0x15, &local_D0);
          break;
        case 504:
          local_F4.x = 0.102f;
          (&local_F4)->y = 0.013f;
          (&local_F4)->z = 0.093f;
          local_D0.x = 4182;
          (&local_D0)->y = 58618;
          (&local_D0)->z = 10919;
          Script_fixoAttachEx(SHEN_NAME('TKE2'), CHARA_JIMY, 18, &local_F4, &local_D0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_5070, 0);
          break;
        case 506:
          local_D0.x = -6005;
          (&local_D0)->y = 5641;
          (&local_D0)->z = 2184;
          Script_SetHandRootRotation(CHARA_JIMY, 0, 0x15, &local_D0);
          local_D0.x = 2910;
          (&local_D0)->y = -7278;
          (&local_D0)->z = 0;
          Script_SetHandRootRotation(CHARA_JIMY, 1, 0x15, &local_D0);
          local_F4.x = 0.129f;
          (&local_F4)->y = 0.013f;
          (&local_F4)->z = 0.093f;
          local_D0.x = 52590;
          (&local_D0)->y = 60980;
          (&local_D0)->z = 1996;
          Script_fixoAttachEx(SHEN_NAME('TKE2'), CHARA_JIMY, 18, &local_F4, &local_D0);
          break;
        case 540:
          Script_fixoClearAttachment(SHEN_NAME('TKE2'));
          Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Hide);
          Script_EVT_GetPosition(CHARA_AKIR, 12, &local_F4, 0);
          local_F4.x = (&local_F4)->x + 0.07f;
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 1);
          Script_charaDisplay(SHEN_NAME('TKET'), CDM_Show);
          break;
        case 615:
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_5154, 10);
          break;
      }
      switch (local_CE) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 140:
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 241:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 300:
          Script_FACE(CHARA_AKIR, 2, 5, 5);
          break;
        case 340:
          Script_FACE(CHARA_AKIR, 2, 4, 5);
          break;
        case 431:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 506:
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 615:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 891:
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 966:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
          break;
      }
      if ((local_CE >= 140) & (local_CE <= 240)) {
        Script_EVT_GetPosition(CHARA_JIMY, 5, &local_E8, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else if (local_CE == 241) {
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      } else if ((local_CE >= 506) & (local_CE <= 600)) {
        Script_EVT_GetPosition(CHARA_AKIR, 12, &local_E8, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else {
        switch (local_CE) {
          case 601:
            Script_EVT_GetPosition(CHARA_JIMY, 5, &local_E8, 0);
            (&local_E8)->y = (&local_E8)->y + 1.0f;
            Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
            break;
          case 941:
            Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
            break;
        }
      }
      if ((local_CE >= 431) & (local_CE <= 470)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else if (local_CE == 471) {
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
      } else if ((local_CE >= 615) & (local_CE <= 695)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else if (local_CE == 696) {
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
      } else if ((local_CE >= 796) & (local_CE <= 890)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else {
        if (local_CE == 891) {
          Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
          goto L002215;
        }
      }
      L002215:;
      switch (local_CE) {
        case 1:
          Script_FACE_LCG(CHARA_JIMY, FACE_BlinkClose, 0, 0);
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
        case 230:
          Script_FACE_LCG(CHARA_JIMY, FACE_BlinkAutomatic, 0, 0);
          break;
        case 321:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 431:
          Script_FACE(CHARA_JIMY, 3, 0, 5);
          break;
        case 506:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 615:
          Script_FACE(CHARA_JIMY, 3, 0, 5);
          break;
        case 696:
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
        case 891:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 966:
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
      }
      if (Script_GetControllerInput(1) == 0x200) {
        Script_shenSeqUtils(-6);
      }
      local_CE = local_CE + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_2353(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int16_t local_CE;
  local_CE = 1;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  if (-1) {
    Script_charaDisplay(CHARA_ETKO, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_ETKO, CDM_Show);
      goto L00238F;
    }
  }
  L00238F:;
  Script_scenToggleVisibleFor(CHARA_ETKO, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF1'), CDM_Show);
      goto L0023C3;
    }
  }
  L0023C3:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF2'), CDM_Show);
      goto L0023F7;
    }
  }
  L0023F7:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF3'), CDM_Show);
      goto L00242B;
    }
  }
  L00242B:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Show);
      goto L00245F;
    }
  }
  L00245F:;
  Script_scenToggleVisibleFor(SHEN_NAME('MAGA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Show);
      goto L002493;
    }
  }
  L002493:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PURS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PURS'), CDM_Show);
      goto L0024C7;
    }
  }
  L0024C7:;
  Script_scenToggleVisibleFor(SHEN_NAME('PURS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('COMP'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
      goto L0024FB;
    }
  }
  L0024FB:;
  Script_scenToggleVisibleFor(SHEN_NAME('COMP'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MONY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MONY'), CDM_Show);
      goto L00252F;
    }
  }
  L00252F:;
  Script_scenToggleVisibleFor(SHEN_NAME('MONY'), 0);
  if (-1) {
    Script_charaDisplay(CHARA_KJHS, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KJHS, CDM_Show);
      goto L002563;
    }
  }
  L002563:;
  Script_scenToggleVisibleFor(CHARA_KJHS, 0);
  if (-1) {
    Script_charaDisplay(CHARA_MNWA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MNWA, CDM_Show);
      goto L002597;
    }
  }
  L002597:;
  Script_scenToggleVisibleFor(CHARA_MNWA, 0);
  Script_charaDisplay(SHEN_NAME('TKET'), CDM_Show);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
      goto L0025D6;
    }
  }
  L0025D6:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKE2'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('TKET'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('TKE2'), 0);
  Script_ccowSetHeightMode(SHEN_NAME('TKET'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('TKE2'), CCOW_KeepHeight);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
  Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_CE) {
        case 1:
          Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_5154, 10);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_5154, 10);
          Script_handSetFloatsFor(CHARA_JIMY, 0, data_5070, 10);
          Script_handSetFloatsFor(CHARA_JIMY, 1, data_5070, 10);
          break;
        case 541:
          Script_SetNewHandMotion(CHARA_AKIR, 0, 621, 0, 0, 0, 0, 1.0f, 0);
          break;
      }
      switch (local_CE) {
        case 1:
          local_F4.x = 1.89f;
          (&local_F4)->y = 0.75f;
          (&local_F4)->z = -4.7f;
          Script_EVT_SetPosition(SHEN_NAME('TKET'), POS_XYZ, &local_F4);
          local_D0.x = 0;
          (&local_D0)->y = 0;
          (&local_D0)->z = 0;
          Script_EVT_SetRotation(SHEN_NAME('TKET'), 0x38000000, &local_D0, 0);
          Script_charaDisplay(SHEN_NAME('TKET'), CDM_Show);
          break;
        case 473:
          local_D0.x = -7462;
          (&local_D0)->y = -8372;
          (&local_D0)->z = 5095;
          Script_SetHandRootRotation(CHARA_JIMY, 1, 0x15, &local_D0);
          break;
        case 504:
          local_F4.x = 0.102f;
          (&local_F4)->y = 0.013f;
          (&local_F4)->z = 0.093f;
          local_D0.x = 4182;
          (&local_D0)->y = 58618;
          (&local_D0)->z = 10919;
          Script_fixoAttachEx(SHEN_NAME('TKET'), CHARA_JIMY, 18, &local_F4, &local_D0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_5070, 0);
          break;
        case 506:
          local_D0.x = -6005;
          (&local_D0)->y = 5641;
          (&local_D0)->z = 2184;
          Script_SetHandRootRotation(CHARA_JIMY, 0, 0x15, &local_D0);
          local_D0.x = 2910;
          (&local_D0)->y = -7278;
          (&local_D0)->z = 0;
          Script_SetHandRootRotation(CHARA_JIMY, 1, 0x15, &local_D0);
          local_D0.x = -4731;
          (&local_D0)->y = 5095;
          (&local_D0)->z = -545;
          Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_D0);
          local_F4.x = 0.129f;
          (&local_F4)->y = -0.013f;
          (&local_F4)->z = 0.052f;
          local_D0.x = 52590;
          (&local_D0)->y = 60980;
          (&local_D0)->z = 1996;
          Script_fixoAttachEx(SHEN_NAME('TKET'), CHARA_JIMY, 18, &local_F4, &local_D0);
          break;
        case 540:
          Script_fixoClearAttachment(SHEN_NAME('TKET'));
          Script_EVT_GetPosition(CHARA_AKIR, 12, &local_F4, 0);
          local_F4.x = (&local_F4)->x + 0.07f;
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 1);
          local_F4.x = 0.135f;
          (&local_F4)->y = -0.022f;
          (&local_F4)->z = 0.119f;
          local_D0.x = 57525;
          (&local_D0)->y = 41313;
          (&local_D0)->z = 18196;
          Script_fixoAttachEx(SHEN_NAME('TKE2'), CHARA_AKIR, 12, &local_F4, &local_D0);
          Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
          break;
        case 541:
          local_D0.x = 0;
          (&local_D0)->y = 0;
          (&local_D0)->z = 0;
          Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_D0);
          local_F4.x = 0.135f;
          (&local_F4)->y = -0.022f;
          (&local_F4)->z = 0.119f;
          local_D0.x = 57525;
          (&local_D0)->y = 41313;
          (&local_D0)->z = 18196;
          Script_charaDisplay(SHEN_NAME('TKET'), CDM_Hide);
          break;
        case 615:
          local_F4.x = 0.14f;
          (&local_F4)->y = 0.018f;
          (&local_F4)->z = 0.111f;
          local_D0.x = 46420;
          (&local_D0)->y = 43499;
          (&local_D0)->z = 30941;
          Script_fixoAttachEx(SHEN_NAME('TKE2'), CHARA_AKIR, 12, &local_F4, &local_D0);
          Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
          break;
        case 696:
          local_F4.x = 0.149f;
          (&local_F4)->y = 0.033f;
          (&local_F4)->z = 0.091f;
          local_D0.x = 29225;
          (&local_D0)->y = 45423;
          (&local_D0)->z = 43527;
          Script_fixoAttachEx(SHEN_NAME('TKE2'), CHARA_AKIR, 12, &local_F4, &local_D0);
          Script_charaDisplay(SHEN_NAME('TKE2'), CDM_Show);
          local_D0.x = -8918;
          (&local_D0)->y = -909;
          (&local_D0)->z = -3821;
          Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_D0);
          break;
      }
      switch (local_CE) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 140:
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 241:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 300:
          Script_FACE(CHARA_AKIR, 2, 5, 5);
          break;
        case 321:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkClose, 16, 8);
          break;
        case 340:
          Script_FACE(CHARA_AKIR, 2, 4, 5);
          break;
        case 431:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 506:
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 615:
          Script_FACE(CHARA_AKIR, 0, 0, 5);
          break;
        case 891:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkClose, 16, 4);
          Script_FACE(CHARA_AKIR, 2, 0, 5);
          break;
        case 966:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
          Script_FACE(CHARA_AKIR, 10, 0, 5);
          break;
      }
      if ((local_CE >= 140) & (local_CE <= 240)) {
        Script_EVT_GetPosition(CHARA_JIMY, 5, &local_E8, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else if (local_CE == 241) {
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      } else if (local_CE == 321) {
        local_E8.x = 2.39f;
        (&local_E8)->y = -0.98f;
        (&local_E8)->z = -3.4f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else if ((local_CE >= 506) & (local_CE <= 695)) {
        Script_EVT_GetPosition(CHARA_JIMY, 5, &local_E8, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else if (local_CE == 696) {
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      } else if ((local_CE >= 796) & (local_CE <= 890)) {
        Script_EVT_GetPosition(CHARA_JIMY, 5, &local_E8, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
      } else {
        switch (local_CE) {
          case 891:
            Script_EVT_GetPosition(CHARA_AKIR, 12, &local_E8, 0);
            Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 8);
            break;
          case 1061:
            Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
            break;
        }
      }
      if ((local_CE >= 431) & (local_CE <= 470)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else if (local_CE == 471) {
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
      } else if ((local_CE >= 615) & (local_CE <= 695)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else if (local_CE == 696) {
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
      } else if ((local_CE >= 796) & (local_CE <= 890)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
        Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_DC, 8);
      } else {
        if (local_CE == 891) {
          Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookReset);
          goto L002EAF;
        }
      }
      L002EAF:;
      switch (local_CE) {
        case 1:
          Script_FACE_LCG(CHARA_JIMY, FACE_BlinkClose, 0, 0);
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
        case 230:
          Script_FACE_LCG(CHARA_JIMY, FACE_BlinkAutomatic, 0, 0);
          break;
        case 321:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 431:
          Script_FACE(CHARA_JIMY, 3, 0, 5);
          break;
        case 506:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 615:
          Script_FACE(CHARA_JIMY, 3, 0, 5);
          break;
        case 696:
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
        case 891:
          Script_FACE(CHARA_JIMY, 0, 0, 5);
          break;
        case 966:
          Script_FACE(CHARA_JIMY, 6, 0, 5);
          break;
      }
      if (Script_GetControllerInput(1) == 0x200) {
        Script_shenSeqUtils(-6);
      }
      local_CE = local_CE + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2FED(void) {
  Vector3i local_F4;
  if (-1) {
    Script_charaDisplay(CHARA_JIMY, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_JIMY, CDM_Show);
      goto L003017;
    }
  }
  L003017:;
  Script_scenToggleVisibleFor(CHARA_JIMY, 0);
  if (-1) {
    Script_charaDisplay(CHARA_KJHS, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KJHS, CDM_Show);
      goto L00304B;
    }
  }
  L00304B:;
  Script_scenToggleVisibleFor(CHARA_KJHS, 0);
  if (-1) {
    Script_charaDisplay(CHARA_MNWA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MNWA, CDM_Show);
      goto L00307F;
    }
  }
  L00307F:;
  Script_scenToggleVisibleFor(CHARA_MNWA, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF3'), CDM_Show);
      goto L0030B3;
    }
  }
  L0030B3:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TRAY'), CDM_Show);
      goto L0030E7;
    }
  }
  L0030E7:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MONY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MONY'), CDM_Show);
      goto L00311B;
    }
  }
  L00311B:;
  Script_scenToggleVisibleFor(SHEN_NAME('MONY'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PURS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PURS'), CDM_Show);
      goto L00314F;
    }
  }
  L00314F:;
  Script_scenToggleVisibleFor(SHEN_NAME('PURS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('COMP'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
      goto L003183;
    }
  }
  L003183:;
  Script_scenToggleVisibleFor(SHEN_NAME('COMP'), 0);
  Script_ccowSetHeightMode(SHEN_NAME('INF1'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('INF2'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('MAGA'), CCOW_KeepHeight);
  for (;;) {
    {
      int32_t local_F0;
      local_F0 = task_4;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 189:
          Script_FACE(CHARA_AKIR, 3, 0, 27);
          break;
        case 308:
          Script_FACE(CHARA_AKIR, 0, 0, 15);
          break;
        case 452:
          Script_FACE(CHARA_AKIR, 2, 0, 20);
          break;
        case 739:
          Script_FACE(CHARA_AKIR, 0, 0, 20);
          break;
        case 1047:
          Script_FACE(CHARA_AKIR, 1, 0, 10);
          break;
        case 1421:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 1430:
          Script_FACE(CHARA_AKIR, 3, 0, 10);
          break;
        case 1550:
          Script_FACE(CHARA_AKIR, 0, 0, 20);
          break;
        case 1631:
          Script_FACE(CHARA_AKIR, 3, 0, 20);
          break;
        case 1740:
          Script_FACE(CHARA_AKIR, 7, 0, 10);
          break;
        case 1848:
          Script_FACE(CHARA_AKIR, 2, 0, 20);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_4;
      switch (local_F0) {
        case 1:
          Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1978:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_5070, 0);
          break;
        case 1995:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_4;
      switch (local_F0) {
        case 1:
          (&local_F4)->z = 0;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 152, 0, &local_F4, CHARA_PART_Set);
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 162, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = -8865;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 153, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = 8865;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 163, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = -0x1600;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 154, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = 0x1600;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 164, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = -4109;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 155, 0, &local_F4, CHARA_PART_Set);
          (&local_F4)->z = 4109;
          Script_charaPosRotUtils(SHEN_NAME('MAGA'), 165, 0, &local_F4, CHARA_PART_Set);
          Script_ReadModelFormat7(CHARA_ETKO, 0, data_5D1C, 0);
          Script_ReadModelFormat7(CHARA_ETKO, 1, data_5D44, 0);
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_57A0, 0);
          Script_handSetFloatsFor(CHARA_ETKO, 0, data_57A0, 0);
          Script_SetHandRootRotation(CHARA_ETKO, 1, 0x15, &data_5D10);
          break;
        case 133:
          Script_ResetHandMotion(CHARA_ETKO, 0, 0, 0);
          break;
        case 216:
          Script_ResetHandMotion(CHARA_ETKO, 0, 1, 0);
          break;
        case 1093:
          Script_ResetHandMotion(CHARA_ETKO, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_ETKO, 0, data_57A0, 0);
          break;
        case 1195:
          Script_ResetHandMotion(CHARA_ETKO, 0, 0, 0);
          Script_ResetHandMotion(CHARA_ETKO, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_57A0, 0);
          Script_SetHandRootRotation(CHARA_ETKO, 1, 0x15, &data_5D10);
          break;
        case 1200:
          Script_CharHandlers(CHARA_ETKO, 1, 1, 16);
          break;
        case 1341:
          Script_ResetHandMotion(CHARA_ETKO, 0, 1, 0);
          break;
      }
    }
    if (task_4 > 2099) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_3646(void) {
  if (-1) {
    Script_charaDisplay(CHARA_JIMY, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_JIMY, CDM_Show);
      goto L003670;
    }
  }
  L003670:;
  Script_scenToggleVisibleFor(CHARA_JIMY, 0);
  if (-1) {
    Script_charaDisplay(CHARA_KJHS, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KJHS, CDM_Show);
      goto L0036A4;
    }
  }
  L0036A4:;
  Script_scenToggleVisibleFor(CHARA_KJHS, 0);
  if (-1) {
    Script_charaDisplay(CHARA_MNWA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MNWA, CDM_Show);
      goto L0036D8;
    }
  }
  L0036D8:;
  Script_scenToggleVisibleFor(CHARA_MNWA, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF1'), CDM_Show);
      goto L00370C;
    }
  }
  L00370C:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('INF2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('INF2'), CDM_Show);
      goto L003740;
    }
  }
  L003740:;
  Script_scenToggleVisibleFor(SHEN_NAME('INF2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGA'), CDM_Show);
      goto L003774;
    }
  }
  L003774:;
  Script_scenToggleVisibleFor(SHEN_NAME('MAGA'), 0);
  Script_ccowSetHeightMode(SHEN_NAME('INF3'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('TRAY'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('MONY'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('PURS'), CCOW_KeepHeight);
  for (;;) {
    {
      int32_t local_FC;
      local_FC = task_4;
      switch (local_FC) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 805:
          Script_FACE(CHARA_AKIR, 7, 0, 6);
          break;
        case 847:
          Script_FACE(CHARA_AKIR, 2, 0, 15);
          break;
        case 983:
          Script_FACE(CHARA_AKIR, 7, 0, 17);
          break;
        case 1424:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 1486:
          Script_FACE(CHARA_AKIR, 7, 0, 20);
          break;
        case 1701:
          Script_FACE(CHARA_AKIR, 3, 0, 20);
          break;
      }
    }
    {
      int32_t local_FC;
      local_FC = task_4;
      switch (local_FC) {
        case 750:
          Script_clmdSetFlags(CHARA_AKIR, 2, 1);
          break;
        case 1100:
          Script_clmdSetFlags(CHARA_AKIR, 2, 0);
          break;
      }
    }
    if ((task_4 >= 1041) & (task_4 <= 1298)) {
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_5DD4);
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_55D8, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_4F8C, 0);
    } else {
      Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
      Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
      Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    }
    {
      int32_t local_FC;
      local_FC = task_4;
      switch (local_FC) {
        case 1086:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          break;
        case 1094:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
        case 1125:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          break;
        case 1235:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
        case 1252:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          break;
        case 1299:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
      }
    }
    {
      int32_t local_FC;
      local_FC = task_4;
      switch (local_FC) {
        case 1091:
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          break;
        case 1165:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1175:
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          break;
        case 1223:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1256:
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          break;
        case 1282:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
      }
    }
    {
      int32_t local_FC;
      local_FC = task_4;
      switch (local_FC) {
        case 2:
          Script_ReadModelFormat7(CHARA_ETKO, 0, data_5D84, 0);
          Script_ReadModelFormat7(CHARA_ETKO, 1, data_5DAC, 0);
          Script_charaDisplay(SHEN_NAME('COMP'), CDM_Show);
          Script_fixoAttachEx(SHEN_NAME('COMP'), CHARA_ETKO, 12, &data_5D6C, &data_5D78);
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_5C14, 0);
          Script_handSetFloatsFor(CHARA_ETKO, 0, data_5968, 0);
          break;
        case 647:
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_5884, 8);
          break;
        case 727:
          Script_SetHandRootRotation(CHARA_ETKO, 1, 0x15, &data_5DE0);
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_5B30, 8);
          break;
        case 1040:
          Script_ResetHandMotion(CHARA_ETKO, 0, 0, 0);
          break;
        case 1070:
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_5A4C, 16);
          break;
        case 1089:
          Script_ResetHandMotion(CHARA_ETKO, 0, 1, 0);
          break;
        case 1298:
          Script_handSetFloatsFor(CHARA_ETKO, 0, data_5968, 0);
          break;
        case 1445:
          Script_handSetFloatsFor(CHARA_ETKO, 1, data_5C14, 8);
          break;
        case 1590:
          Script_ResetHandMotion(CHARA_ETKO, 0, 1, 0);
          Script_ResetHandMotion(CHARA_ETKO, 0, 0, 0);
          break;
      }
    }
    if ((task_4 > 1040) & (task_4 < 1220)) {
      int32_t unused_FC;
      Vector3 local_F0;
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F0, 0);
      Script_FaceLookAtTargetUtils(CHARA_ETKO, FACE_LookAtPosition, &local_F0, 4);
    } else {
      if (task_4 == 1220) {
        Script_FaceLookAtTargetUtils(CHARA_ETKO, FACE_LookReset);
        goto L003CA6;
      }
    }
    L003CA6:;
    if (task_4 > 1968) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_5DFC, 2, data_5E0D, 4, data_5E25, 5, data_5E2B, 6, data_5E31, 7, data_5E3C, 8, data_5E49, 9, data_5E54, 10, data_5E61, 11, data_5E67, 12, data_5E6D, 13, data_5E78, 14, data_5E85, 15, data_5E90, 16, data_5E9D, 17, data_5EA3, 18, data_5EA9, 19, data_5EB5, 20, data_5EC0, 21, data_5ECD, 22, data_5ED3, 23, data_5ED9, 24, data_5EE5, 25, data_5EF0, 26, data_5EFD)))
void script_3D09(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Script_SetFogColour(&data_5DEC);
  Script_ryoSetChara(CHARA_AKIR);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 40);
  for (;;) {
    local_F0 = Script_GetCurrentEntryPoint();
    switch (local_F0) {
      case 0:
        Script_LoadVoiceFile(1, "A0165");
        local_F4 = Script_motiLoad(2, SHEN_NAME('ARAR'), "M_0165.BIN");
        Script_LoadSND(0, 2, "bgm073.snd", 0);
        Script_LoadSND(0, 4, "a1_asiar.snd", 0);
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 100);
        script_0731();
        local_FC = Script_scenCreateTask(script_0805, 0);
        task_4 = 0;
        local_F8 = Script_scenCreateTask(script_078B, 0);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_set_val_to_1(3, 0);
        Script_shenSeqUtils(0);
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
        Script_srchControl((void *)local_F8);
        Script_ReallocateUNFO(local_F4);
        Script_LoadVoiceFile(0, "A0165");
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('D000'), 2);
        break;
      case 1:
        Script_LoadVoiceFile(1, "A0174");
        local_F4 = Script_motiLoad(2, SHEN_NAME('ARAR'), "M_0174.BIN");
        Script_LoadHandMot(2, SHEN_NAME('ARAR'), "HMOT0151.BIN");
        Script_LoadSND(0, 2, "bgm063.snd", 0);
        Script_LoadSND(0, 4, "a1_hijym.snd", 0);
        Script_executeAicaCommand(18088, 0, 0);
        Script_executeAicaCommand(1184, 2, 107);
        script_0731();
        local_FC = Script_scenCreateTask(script_1133, 0);
        task_4 = 0;
        local_F8 = Script_scenCreateTask(script_078B, 0);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_set_val_to_1(3, 0);
        Script_shenSeqUtils(1);
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
        Script_srchControl((void *)local_F8);
        Script_ReallocateUNFO(local_F4);
        Script_LoadVoiceFile(0, "A0174");
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_POST_GMCT_STAGE_ACTIVE, 1);
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('D000'), 2);
        break;
      case 2:
        Script_LoadVoiceFile(1, "A0174");
        local_F4 = Script_motiLoad(2, SHEN_NAME('ARAR'), "M_0174.BIN");
        Script_LoadHandMot(2, SHEN_NAME('ARAR'), "HMOT0151.BIN");
        Script_LoadSND(0, 2, "bgm063.snd", 0);
        Script_LoadSND(0, 4, "a1_hijym.snd", 0);
        Script_executeAicaCommand(18088, 0, 0);
        Script_executeAicaCommand(1184, 2, 107);
        script_0731();
        local_FC = Script_scenCreateTask(script_2353, 0);
        task_4 = 0;
        local_F8 = Script_scenCreateTask(script_078B, 0);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_set_val_to_1(3, 0);
        Script_shenSeqUtils(2);
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_set_val_to_1(2, 0);
        Script_srchControl((void *)local_FC);
        Script_srchControl((void *)local_F8);
        Script_ReallocateUNFO(local_F4);
        Script_LoadVoiceFile(0, "A0174");
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('D000'), 2);
        break;
      case 3:
        Script_LoadVoiceFile(1, "01ETU");
        local_F4 = Script_motiLoad(2, SHEN_NAME('ARAR'), "M_01ETU.BIN");
        Script_LoadSND(0, 2, "bgm073.snd", 0);
        Script_LoadSND(0, 4, "a1_etuko.snd", 0);
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 100);
        script_0731();
        local_FC = Script_scenCreateTask(script_2FED, 0);
        task_4 = 0;
        local_F8 = Script_scenCreateTask(script_078B, 0);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_set_val_to_1(3, 0);
        Script_shenSeqUtils(3);
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
        Script_srchControl((void *)local_F8);
        Script_GetCassetteFlags(42, 1, 0, 0);
        SCNF_StartIconSystemMenu(42);
        Script_ReallocateUNFO(local_F4);
        Script_LoadVoiceFile(0, "01ETU");
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_REWARD_STAGE_ACTIVE, 1);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_CASSETTE_REWARD_COMPLETE, 1);
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('D000'), 2);
        break;
      case 4:
        Script_LoadVoiceFile(1, "01ETU");
        local_F4 = Script_motiLoad(2, SHEN_NAME('ARAR'), "M_01ETU.BIN");
        Script_LoadSND(0, 2, "bgm073.snd", 0);
        Script_LoadSND(0, 4, "a1_etuko.snd", 0);
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 100);
        script_0731();
        local_FC = Script_scenCreateTask(script_3646, 0);
        task_4 = 0;
        local_F8 = Script_scenCreateTask(script_078B, 0);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_set_val_to_1(3, 0);
        Script_shenSeqUtils(4);
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
        Script_srchControl((void *)local_F8);
        Script_ReallocateUNFO(local_F4);
        Script_LoadVoiceFile(0, "01ETU");
        {
          int32_t unused_EC;
          GameDateTime local_E4;
          int8_t local_E3;
          int8_t local_E2;
          Script_ReturnTimeDate(&local_E4);
          if ((&local_E4)->month == 12) {
            local_E2 = 0;
          } else {
            local_E2 = (&local_E4)->month;
          }
          local_E3 = (local_E2 << 5) | (&local_E4)->day;
          Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 24, local_E3);
        }
        {
          int32_t unused_EC;
          GameDateTime local_E4;
          int8_t local_E3;
          Script_ReturnTimeDate(&local_E4);
          local_E3 = ((&local_E4)->hours * 60 + (&local_E4)->minutes) / 6;
          Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 30, local_E3);
        }
        task_0 = Script_shenGetStats(STATS_CASH);
        task_0 = task_0 - 69000;
        SCNF_SetCashValue(-69000, 1);
        Script_SetStats(STATS_CASH, task_0);
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('D000'), 2);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(RYO_RestoreControl);
  return;
}
