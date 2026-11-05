#include "act02_toki.h"

static int32_t script_0030(void) {
  for (;;) {
    Script_ClearAccumulator_00FE(1, 0x3F000000, 10, 2, 2);
    Script_ClearAccumulator_00FE(3, &data_15B8, 8, 9);
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

static void script_03F3(void) {
  script_0086();
  script_0FC4();
  Script_shenSeqUtils(0);
  Script_ResetHandMotion(CHARA_SINF, 1, 1, 0);
  Script_ResetHandMotion(CHARA_SINF, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_SINF, 1, data_253C, 0);
  Script_handSetFloatsFor(CHARA_SINF, 0, data_253C, 0);
  Script_SetHandRootRotation(CHARA_SINF, 1, 0x15, &data_264C);
  Script_SetHandRootRotation(CHARA_SINF, 0, 0x15, &data_2658);
  Script_charaDisplay(SHEN_NAME('HOSI'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SEZ1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SEZ2'), CDM_Show);
  Script_charaDisplay(CHARA_SINF, CDM_Show);
  Script_mapmSetRenderFlag(0, 0);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('DOR2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('GAKU'), CDM_Hide);
  Script_charaDisplay(CHARA_ASDA, CDM_Hide);
  while (Script_shenSeqUtils(-1)) {
    Script_charaSetRenderOpacity(CHARA_AKIR, 0.5f);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_04F4(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_F0;
  int32_t local_EC;
  task_4 = 0.0f;
  task_8 = 0.0f;
  task_C = 0.0f;
  task_14 = 0.0f;
  task_10 = 0.0f;
  script_0086();
  script_0FC4();
  task_0 = 0;
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    script_0A9C();
    local_EC = task_0;
    switch (local_EC) {
      case 1:
        Script_charaDisplay(SHEN_NAME('PNXM'), CDM_Hide);
        Script_ResetHandMotion(CHARA_ASDA, 1, 1, 0);
        Script_ResetHandMotion(CHARA_ASDA, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_ASDA, 1, data_1C3C, 0);
        Script_handSetFloatsFor(CHARA_ASDA, 0, data_1C3C, 0);
        break;
      case 477:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_2194);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_21A0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_1C3C, 0);
        break;
      case 490:
        Script_charaDisplay(SHEN_NAME('PNXM'), CDM_Show);
        break;
      case 590:
        Script_handSetFloatsFor(CHARA_ASDA, 1, data_2458, 15);
        Script_handSetFloatsFor(CHARA_ASDA, 0, data_2458, 15);
        break;
      case 610:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2458, 15);
        Script_SetHandRootRotation(CHARA_ASDA, 1, 0x15, &data_2368);
        Script_handSetFloatsFor(CHARA_ASDA, 1, data_2284, 50);
        Script_handSetFloatsFor(CHARA_ASDA, 0, data_2284, 50);
        break;
      case 629:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1C3C, 8);
        break;
      case 630:
        break;
      case 798:
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        break;
      case 864:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1C3C, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_1C3C, 0);
        break;
      case 1257:
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_ASDA, 0, 1, 0);
        Script_ResetHandMotion(CHARA_ASDA, 0, 0, 0);
        break;
      case 1800:
        Script_ResetHandMotion(CHARA_SINF, 1, 1, 0);
        Script_ResetHandMotion(CHARA_SINF, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SINF, 1, data_253C, 0);
        Script_handSetFloatsFor(CHARA_SINF, 0, data_253C, 0);
        Script_SetHandRootRotation(CHARA_SINF, 1, 0x15, &data_2694);
        Script_SetHandRootRotation(CHARA_SINF, 0, 0x15, &data_26A0);
        break;
      case 2640:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1C3C, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_1C3C, 0);
        Script_ResetHandMotion(CHARA_ASDA, 1, 1, 0);
        Script_ResetHandMotion(CHARA_ASDA, 1, 0, 0);
        Script_SetHandRootRotation(CHARA_ASDA, 1, 0x15, &data_2368);
        Script_handSetFloatsFor(CHARA_ASDA, 1, data_2284, 0);
        Script_handSetFloatsFor(CHARA_ASDA, 0, data_2284, 0);
        break;
      case 3300:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2458, 15);
        Script_handSetFloatsFor(CHARA_ASDA, 1, data_2458, 20);
        Script_handSetFloatsFor(CHARA_ASDA, 0, data_2458, 20);
        break;
      case 3317:
        Script_fixoAttachEx(SHEN_NAME('PNXM'), CHARA_AKIR, 18, &data_2664, &data_2670);
        break;
      case 3569:
        Script_fixoClearAttachment(SHEN_NAME('PNXM'));
        Script_charaDisplay(SHEN_NAME('PNXM'), CDM_Hide);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_ASDA, 0, 1, 0);
        Script_ResetHandMotion(CHARA_ASDA, 0, 0, 0);
        break;
    }
    switch (task_0) {
      case 3430:
        Script_EVT_GetPosition(SHEN_NAME('PNXM'), -1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 20);
        break;
      case 3490:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        break;
    }
    switch (task_0) {
      case 0:
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 101);
        break;
      case 870:
        Script_executeAicaCommand(2720, 2, 30);
        break;
      case 900:
        Script_executeAicaCommand(81832, 0, 0);
        Script_executeAicaCommand(1184, 2, 91);
        break;
    }
    if (task_0 == 3850) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    task_0 = task_0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0A9C(void) {
  int32_t local_FC;
  local_FC = task_0;
  switch (local_FC) {
    case 1:
      Script_charaDisplay(CHARA_SINF, CDM_Hide);
      Script_charaDisplay(SHEN_NAME('HOSI'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('SEZ1'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('SEZ2'), CDM_Hide);
      Script_hmdlApplyScale(SHEN_NAME('HOSI'), &data_2620);
      task_14 = 1.0f;
      Script_SetupLightFromTable(0);
      break;
    case 1800:
      Script_charaDisplay(SHEN_NAME('HOSI'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('SEZ1'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('SEZ2'), CDM_Show);
      Script_charaDisplay(CHARA_SINF, CDM_Show);
      Script_InitializeOsag(CHARA_SINF);
      Script_charaDisplay(CHARA_ASDA, CDM_Hide);
      task_4 = 0.0f;
      task_8 = 0.0f;
      task_C = 0.0f;
      task_10 = 1.0f;
      break;
    case 1900:
      Script_mapmSetRenderFlag(0, 0);
      Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('DOR2'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('GAKU'), CDM_Hide);
      Script_charaDisplay(CHARA_AKIR, CDM_Hide);
      Script_SetupLightFromTable(1);
      task_1C.x = *(float *)&data_263C;
      task_1C.y = *(float *)(&data_263C + 1);
      task_1C.z = *(float *)(&data_263C + 2);
      task_1C.w = *(float *)(&data_263C + 3);
      task_18 = 1.0f;
      break;
    case 2400:
      Script_clmdSetFlags(CHARA_SINF, 2, 1);
      break;
    case 2520:
      Script_mapmSetRenderFlag(0, 1);
      Script_charaDisplay(SHEN_NAME('DOR2'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('GAKU'), CDM_Show);
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      task_14 = 0.0f;
      Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 0);
      break;
    case 2640:
      Script_charaDisplay(SHEN_NAME('HOSI'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('SEZ1'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('SEZ2'), CDM_Hide);
      Script_charaDisplay(CHARA_SINF, CDM_Hide);
      if (-1) {
        Script_charaDisplay(CHARA_SINF, CDM_Hide);
      } else {
        if (0) {
          Script_charaDisplay(CHARA_SINF, CDM_Show);
          goto L000CBF;
        }
      }
      L000CBF:;
      Script_scenToggleVisibleFor(CHARA_SINF, 0);
      Script_charaDisplay(CHARA_ASDA, CDM_Show);
      Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
      Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 0);
      Script_SetupLightFromTable(0);
      break;
  }
  if ((task_0 > 1800) & (task_0 <= 1900)) {
    task_14 = task_14 - 0.01f;
    if (task_0 == 1900) {
      task_14 = 0.0f;
    }
  } else {
    if ((task_0 > 2520) & (task_0 <= 2580)) {
      task_14 = task_14 + 0.0166666675f;
      if (task_0 == 2580) {
        task_14 = 1.0f;
      }
      goto L000D69;
    }
  }
  L000D69:;
  if ((task_0 > 2500) & (task_0 <= 2530)) {
    task_1C.x = (&task_1C)->x + (*(float *)&data_262C - *(float *)&data_263C) / 30.0f;
    task_1C.y = task_1C.y + (*(float *)(&data_262C + 1) - *(float *)(&data_263C + 1)) / 30.0f;
    task_1C.z = task_1C.z + (*(float *)(&data_262C + 2) - *(float *)(&data_263C + 2)) / 30.0f;
    task_1C.w = task_1C.w + (*(float *)(&data_262C + 3) - *(float *)(&data_263C + 3)) / 30.0f;
    task_18 = task_18 - 0.0333333351f;
    Script_lightSetGlobalSceneLighting(&task_1C);
    Script_LightUtils(4, 4, task_18);
  }
  if ((task_0 > 1850) & (task_0 <= 1900)) {
    task_4 = task_4 + 0.016f;
  } else {
    if ((task_0 > 2070) & (task_0 <= 2130)) {
      task_4 = task_4 - 0.0133333337f;
      if (task_0 == 2130) {
        task_4 = 0.0f;
      }
      goto L000E8E;
    }
  }
  L000E8E:;
  if ((task_0 > 2020) & (task_0 <= 2050)) {
    task_8 = task_8 + 0.0100000007f;
  }
  if ((task_0 > 2070) & (task_0 <= 2130)) {
    task_C = task_C + 0.00500000035f;
  } else {
    if ((task_0 > 2520) & (task_0 <= 2580)) {
      task_8 = task_8 - 0.00500000035f;
      task_C = task_C - 0.00500000035f;
      if (task_0 == 2580) {
        task_8 = 0.0f;
        task_C = 0.0f;
      }
      goto L000F1E;
    }
  }
  L000F1E:;
  if ((task_0 > 2520) & (task_0 <= 2580)) {
    task_10 = task_10 - 0.0166666675f;
    if (task_0 == 2580) {
      task_10 = 0.0f;
    }
  }
  Script_SetClampedParameterValue(0, task_14);
  Script_charaSetRenderOpacity(SHEN_NAME('DOOR'), task_14);
  Script_charaSetRenderOpacity(SHEN_NAME('DOR2'), task_14);
  Script_charaSetRenderOpacity(SHEN_NAME('GAKU'), task_14);
  Script_charaSetRenderOpacity(CHARA_AKIR, task_14);
  Script_charaSetRenderOpacity(SHEN_NAME('HOSI'), task_4);
  Script_charaSetRenderOpacity(SHEN_NAME('SEZ1'), task_8);
  Script_charaSetRenderOpacity(SHEN_NAME('SEZ2'), task_C);
  Script_charaSetRenderOpacity(CHARA_SINF, task_10);
  return;
}

static void script_0FC4(void) {
  script_009E(CHARA_AKIR);
  script_009E(CHARA_ASDA);
  script_009E(CHARA_SINF);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_ASDA, 1, 0);
  Script_facemotUtils(CHARA_SINF, 1, 0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_ASDA, -1);
  Script_SetMotnFlag(CHARA_SINF, -1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_ASDA, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_SINF, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(CHARA_AKIR, 0);
  Script_SetCharaFlags(CHARA_ASDA, 0);
  Script_SetCharaFlags(CHARA_SINF, 0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_26CC);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_ASDA, &data_26CC);
  Script_charaSetRefboardFlagTo(CHARA_ASDA, 1);
  Script_charaSetRefboardFlagTo(CHARA_SINF, 0);
  Script_FACE_LCG(CHARA_SINF, FACE_BlinkOpen, 0, 0);
  Script_SetMaxInterpSpeed(0.4f);
  Script_ccowSetHeightMode(SHEN_NAME('PNXM'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('HOSI'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('SEZ1'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('SEZ2'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('GAKU'), CCOW_KeepHeight);
  return;
}

static void script_1117(void) {
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_ASDA, 0);
  Script_SetMotnFlag(CHARA_SINF, 0);
  script_012B(CHARA_AKIR);
  script_012B(CHARA_ASDA);
  script_012B(CHARA_SINF);
  Script_SetGameStateTo(AUTH_None);
  return;
}

void script_1168(void) {
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 46);
  Script_scenCreateTask(script_1184, 1, 0x52494B41);
  return;
}

static int32_t script_1184(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  local_FC = Script_GetCurrentEntryPoint();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(60);
  Script_sprintf("start\012");
  script_1271();
  Script_sprintf("end\012");
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (1) {
    local_F4 = local_FC;
    if ((local_F4 == 0) | (local_F4 == 1) | (local_F4 == 2)) {
      script_1457(0);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PHOENIX_LEGEND, 1);
      Script_shenRequestGameModuleTransition(Script_GetSceneID(), SHEN_NAME('D000'), 10);
      SCNF_WaitFrames_0000(1);
    } else {
      if (local_F4 == 99) {
        Script_sprintf("auth start\012");
        script_1356();
        goto L00125D;
      }
    }
    L00125D:;
    SCNF_WaitFrames_0000(60);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1271(void) {
  Vector4 local_F0;
  int32_t local_D8[6];
  Vector3i local_CC;
  float local_C8;
  (&local_F0)->y = 0.3f;
  (&local_F0)->z = 0.3f;
  (&local_F0)->w = 0.3f;
  (void)1;
  Script_SetVector4ByOffset(&local_F0);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 15.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
  local_D8[0] = 0x40C9999A;
  local_D8[1] = 0x3F970A3D;
  local_D8[2] = 0x3FB851EC;
  local_C8 = 0.0f;
  local_D8[3] = 0x3F800000;
  local_D8[4] = 0;
  local_D8[5] = 0;
  local_CC.x = (int32_t)(local_C8 / 360.0f * (float)0x10000);
  (&local_CC)->y = (int32_t)(local_C8 / 360.0f * (float)0x10000);
  (&local_CC)->z = (int32_t)(local_C8 / 360.0f * (float)0x10000);
  Script_EVT_ApplyRot((void *)local_D8, &local_CC);
  return;
}

static void script_1356(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (1) {
    Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    for (;;) {
      if (Script_GetControllerInput(0) == 0x400) {
        break;
      }
      if (Script_GetControllerInput(1) == 64) {
        local_FC = local_FC - 1;
      }
      if (Script_GetControllerInput(1) == 128) {
        local_FC = local_FC + 1;
      }
      if (local_FC > 2) {
        local_FC = 0;
      }
      if (local_FC < 0) {
        local_FC = 2;
      }
      local_F8 = local_FC;
      switch (local_F8) {
        case 0:
          Script_PrintDebugString(3, 25, "01HOK_4.c");
          break;
        case 1:
          Script_PrintDebugString(3, 25, "01HOK_2.c");
          break;
        case 2:
          Script_PrintDebugString(3, 25, "01HOK_3.c");
          break;
      }
      SCNF_WaitFrames_0000(1);
    }
    script_1457(local_FC);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_271F, 3, data_273A, 5, data_2755, 8, data_277B, 12, data_27A1)))
static void script_1457(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(60);
  local_F8 = arg0;
  switch (local_F8) {
    case 0:
      Script_AuthorUtils(0, 0, "SEQDATA0.BIN", "SCENE/02/TOKI");
      break;
    case 1:
      Script_AuthorUtils(0, 0, "SEQDATA1.BIN", "SCENE/02/TOKI");
      break;
    case 2:
      Script_AuthorUtils(0, 0, "SEQDATA2.BIN", "SCENE/02/TOKI");
      break;
  }
  Script_LoadSND(0, 4, "a1_hokky.snd", 0);
  Script_LoadSND(0, 2, "bgm050.snd", 0);
  Script_LoadVoiceFile(1, "01HOK");
  local_FC = Script_motiLoad(2, CHARA_TOKI, "M_01HOK.BIN");
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL25.SPR");
  Script_SetCurrentScrollEnabled(1);
  Script_scenCreateTask(script_017B, 0);
  script_04F4(arg0);
  Script_ReallocateUNFO(local_FC);
  script_1117();
  Script_LoadVoiceFile(0, "01HOK");
  Script_AuthorUtils(AUTH_FREE, 0);
  SCNF_WaitFrames_0000(60);
  SCNF_WaitFrames_0000(1);
  Script_executeAicaCommand(0x1200A0, 2, 0);
  return;
}
