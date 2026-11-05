#include "act03_ma00.h"

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

static int32_t script_0385(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
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

static int32_t script_05FD(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_061B(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_063A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = arg1 - arg0;
  local_F8 = arg0 - arg1;
  if ((local_F8 & 0xFFFF) < (local_FC & 0xFFFF)) {
    if (arg0 > arg1) {
      local_F4 = local_F8 * -1;
    } else {
      local_F4 = (local_F8 & 0xFFFF) * -1;
    }
  } else if (arg0 > arg1) {
    local_F4 = local_FC & 0xFFFF;
  } else {
    local_F4 = local_FC & 0xFFFF;
  }
  return local_F4;
}

static void script_06B2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_160 != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_48[arg2], arg3, arg4);
    local_FC = arg1;
    switch (local_FC) {
      case 40731:
        Script_sprintf("Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012");
        break;
      case 40732:
        Script_sprintf("Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012");
        break;
      case 40705:
        Script_sprintf("Door Open Motion = DEB_HIKISIME_DOOR_L_F\012");
        break;
      case 40706:
        Script_sprintf("Door Open Motion = DEB_HIKISIME_DOOR_R_F\012");
        break;
      case 40733:
        Script_sprintf("Door Open Motion = DEB_OSISIME_DOOR_L_F\012");
        break;
      case 40707:
        Script_sprintf("Door Open Motion = DEB_OSISIME_DOOR_R_F\012");
        break;
      case 40734:
        Script_sprintf("Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012");
        break;
      case 40735:
        Script_sprintf("Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012");
        break;
      case 40708:
        Script_sprintf("Door Open Motion = GAK_HIKISIME_DOOR_L_F\012");
        break;
      case 40709:
        Script_sprintf("Door Open Motion = GAK_HIKISIME_DOOR_R_F\012");
        break;
      case 40736:
        Script_sprintf("Door Open Motion = GAK_OSISIME_DOOR_L_F\012");
        break;
      case 40710:
        Script_sprintf("Door Open Motion = GAK_OSISIME_DOOR_R_F\012");
        break;
      case 40737:
        Script_sprintf("Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012");
        break;
      case 40738:
        Script_sprintf("Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012");
        break;
      case 40711:
        Script_sprintf("Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012");
        break;
      case 40712:
        Script_sprintf("Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012");
        break;
      case 40739:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_L_F\012");
        break;
      case 40713:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_R_F\012");
        break;
      case 40740:
        Script_sprintf("Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012");
        break;
      case 40741:
        Script_sprintf("Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012");
        break;
      case 40714:
        Script_sprintf("Door Open Motion = KOD_HIKISIME_DOOR_L_F\012");
        break;
      case 40715:
        Script_sprintf("Door Open Motion = KOD_HIKISIME_DOOR_R_F\012");
        break;
      case 40742:
        Script_sprintf("Door Open Motion = KOD_OSISIME_DOOR_L_F\012");
        break;
      case 40716:
        Script_sprintf("Door Open Motion = KOD_OSISIME_DOOR_R_F\012");
        break;
      case 40743:
        Script_sprintf("Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012");
        break;
      case 40744:
        Script_sprintf("Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012");
        break;
      case 40717:
        Script_sprintf("Door Open Motion = OTH_HIKISIME_DOOR_L_F\012");
        break;
      case 40718:
        Script_sprintf("Door Open Motion = OTH_HIKISIME_DOOR_R_F\012");
        break;
      case 40745:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_L_F\012");
        break;
      case 40719:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_R_F\012");
        break;
      case 40746:
        Script_sprintf("Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012");
        break;
      case 40747:
        Script_sprintf("Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012");
        break;
      case 40720:
        Script_sprintf("Door Open Motion = PNW_HIKISIME_DOOR_L_F\012");
        break;
      case 40721:
        Script_sprintf("Door Open Motion = PNW_HIKISIME_DOOR_R_F\012");
        break;
      case 40748:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_L_F\012");
        break;
      case 40722:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_R_F\012");
        break;
      case 40749:
        Script_sprintf("Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012");
        break;
      case 40750:
        Script_sprintf("Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012");
        break;
      case 40723:
        Script_sprintf("Door Open Motion = SIN_HIKISIME_DOOR_L_F\012");
        break;
      case 40724:
        Script_sprintf("Door Open Motion = SIN_HIKISIME_DOOR_R_F\012");
        break;
      case 40751:
        Script_sprintf("Door Open Motion = SIN_OSISIME_DOOR_L_F\012");
        break;
      case 40725:
        Script_sprintf("Door Open Motion = SIN_OSISIME_DOOR_R_F\012");
        break;
      case 40757:
        Script_sprintf("Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012");
        break;
      case 40752:
        Script_sprintf("Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012");
        break;
      case 40726:
        Script_sprintf("Door Open Motion = SYP_HIKISIME_DOOR_L_F\012");
        break;
      case 40727:
        Script_sprintf("Door Open Motion = SYP_HIKISIME_DOOR_R_F\012");
        break;
      case 40753:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_L_F\012");
        break;
      case 40728:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_R_F\012");
        break;
      case 40758:
        Script_sprintf("Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012");
        break;
      case 40754:
        Script_sprintf("Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012");
        break;
      case 40729:
        Script_sprintf("Door Open Motion = YKI_HIKISIME_DOOR_L_F\012");
        break;
      case 40730:
        Script_sprintf("Door Open Motion = YKI_HIKISIME_DOOR_R_F\012");
        break;
      case 40755:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_L_F\012");
        break;
      case 40756:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_R_F\012");
        break;
      case 40759:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40760:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40761:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40762:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40763:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012");
        break;
    }
  }
  return;
}

static int32_t script_0C64(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0;
  if (local_F8 == 0) {
    local_FC = 594;
  } else if (local_F8 == 1) {
    local_FC = 595;
  } else if (local_F8 == 2) {
    local_FC = 596;
  } else if (local_F8 == 3) {
    local_FC = 598;
  } else if (local_F8 == 4) {
    local_FC = 599;
  } else if (local_F8 == 5) {
    local_FC = 600;
  } else if (local_F8 == 6) {
    local_FC = 1000;
  } else if (local_F8 == 7) {
    local_FC = 656;
  } else if (local_F8 == 8) {
    local_FC = 601;
  } else if (local_F8 == 9) {
    local_FC = 594;
  } else {
    local_FC = 594;
  }
  return local_FC;
}

static void script_0D35(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_9A58(arg0, arg2, 0);
  return;
}

static void script_0D45(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_91ED(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_44D1(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_9A58(arg0, arg2, 0);
  return;
}

static void script_0E49(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) {
        if ((local_FC == 0) | (local_FC == 1)) {
          local_F8 = 6;
        } else {
          local_F8 = 4;
        }
      } else if ((local_FC == 2) | (local_FC == 3)) {
        local_F8 = 6;
      } else {
        local_F8 = 4;
      }
    } else {
      local_F8 = 4;
    }
    local_FC = script_91ED(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_48[arg2]);
    script_44D1(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_9A58(arg0, arg2, 0);
  return;
}

static void script_0FEB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_91ED(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 4) == 0) {
      script_29DE(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_160) {
        script_5DB1(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_9A58(arg0, arg2, 2);
  return;
}

static void script_114C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_91ED(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 4) == 0) {
      script_29DE(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_160) {
        script_5DB1(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_9A58(arg0, arg2, 2);
  return;
}

static void script_12AD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) {
          if ((local_FC == 0) | (local_FC == 1)) {
            local_F8 = 6;
          } else {
            local_F8 = 4;
          }
        } else if ((local_FC == 2) | (local_FC == 3)) {
          local_F8 = 6;
        } else {
          local_F8 = 4;
        }
      } else {
        local_F8 = 4;
      }
      local_FC = script_91ED(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_48[arg2]);
      script_29DE(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_160) {
        float local_F4;
        float local_F0;
        local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_91ED(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_48[arg2]);
        script_5DB1(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_9A58(arg0, arg2, 2);
  return;
}

static void script_154B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    script_91ED(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_9A58(arg0, arg2, 0);
  return;
}

static void script_1619(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_91ED(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_395D(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_9A58(arg0, arg2, 1);
  return;
}

static void script_1680(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_91ED(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_395D(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_9A58(arg0, arg2, 1);
  return;
}

static void script_16E7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_91ED(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_395D(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_9A58(arg0, arg2, 1);
  return;
}

static void script_172E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_91ED(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_91ED(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_3E17(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_9A58(arg0, arg2, 2);
  return;
}

static void script_1864(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  Vector3i local_F0;
  Vector3i local_E4;
  Vector3i local_D8;
  Vector3 local_CC;
  Vector3 local_C0;
  Vector3 local_B4;
  Vector3 local_A8;
  Vector3 local_9C;
  float local_98;
  float local_94;
  float local_90;
  float local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t unused_80;
  int8_t local_FF7F;
  uint8_t unused_FF7C[3];
  float local_FF78;
  float local_FF74;
  if (arg0 == task_160) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_FF7F = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_91ED(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_98B6(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_168)->x;
      (&local_9C)->y = task_168.y;
      (&local_9C)->z = task_168.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, CHARA_PART_Get);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, CHARA_PART_Get);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_160) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_168, *(&task_174 + 1), local_FC, 0);
        for (;;) {
          if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
            break;
          }
          if (Script_IsMoveTargetRequestActive(arg0) == 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FC);
        for (;;) {
          if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
            break;
          }
          if (Script_GetCharaMoveStatus(arg0) == 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      for (;;) {
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
          break;
        }
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
        local_98 = Script_GetCharaMotionRemainingFramesScaled(arg0);
        Script_ReadMOTNPositionAtFrame(arg0, local_FC, local_98, &local_A8);
        if (!(local_98 < local_94) & !(local_98 > local_90)) {
          if (local_FF7F == 1) {
            if (!(local_98 < local_94) & !(local_98 >= local_94 + 1.0f)) {
              int32_t unused_FF70;
              int32_t local_FF6C;
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_160) {
                  Script_executeAicaCommand(local_FF6C, 0, 0);
                } else {
                  Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
                }
              }
            }
            Script_EVT_GetPosition(arg0, 12, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_C0)->x - (&local_B4)->x, (&local_C0)->z - (&local_B4)->z);
            (&local_F0)->y = (&local_F0)->y - (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000);
            local_F0.x = (&local_D8)->x;
            (&local_F0)->z = (&local_D8)->z;
            Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
            Script_EVT_GetPosition(arg0, 18, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_CC)->x - (&local_B4)->x, (&local_CC)->z - (&local_B4)->z);
            (&local_F0)->y = (&local_F0)->y + (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000);
            local_F0.x = (&local_E4)->x;
            (&local_F0)->z = (&local_E4)->z;
            Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
          } else {
            if (!(local_98 < local_94) & !(local_98 >= local_94 + 1.0f)) {
              int32_t unused_FF70;
              int32_t local_FF6C;
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_160) {
                  Script_executeAicaCommand(local_FF6C, 0, 0);
                } else {
                  Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
                }
              }
            }
            Script_EVT_GetPosition(arg0, 12, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_C0)->x - (&local_B4)->x, (&local_C0)->z - (&local_B4)->z);
            (&local_F0)->y = -((&local_F0)->y - (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000));
            local_F0.x = (&local_D8)->x;
            (&local_F0)->z = (&local_D8)->z;
            Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
            Script_EVT_GetPosition(arg0, 18, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_CC)->x - (&local_B4)->x, (&local_CC)->z - (&local_B4)->z);
            (&local_F0)->y = -((&local_F0)->y + (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000));
            local_F0.x = (&local_E4)->x;
            (&local_F0)->z = (&local_E4)->z;
            Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
          }
        }
        if (!(local_98 < local_90 + 10.0f)) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      script_98B6(arg0, 1);
      local_8C = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, (&local_F0)->y);
      local_8C = local_8C / 182.044449f;
      if (!(local_8C <= 180.0f)) {
        local_8C = 360.0f - local_8C;
      }
      if (!(local_8C >= -180.0f)) {
        local_8C = local_8C + 360.0f;
      }
      local_88 = 0x4000;
      local_84 = 0;
      local_90 = 1.0f;
      local_FC = 0;
      {
        int32_t unused_FF70;
        int32_t local_FF6C;
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF6C, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
          }
        }
      }
      for (;;) {
        local_94 = local_8C * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_88) * local_90;
        local_90 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84);
        if (local_FC == 0) {
          local_88 = local_88 + 0x200;
          if (local_88 >= 0xC000) {
            local_FC = 1;
          }
        } else {
          local_88 = local_88 - 0x200;
          if (local_88 <= 0x4000) {
            local_FC = 0;
          }
        }
        local_84 = local_84 + 128;
        if (local_FF7F == 1) {
          (&local_F0)->y = -(int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
          (&local_F0)->y = (int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
        } else {
          (&local_F0)->y = (int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
          (&local_F0)->y = -(int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
        }
        SCNF_WaitFrames_0000(1);
        if (local_84 >= 0x4000) {
          break;
        }
      }
    }
    script_9A58(arg0, arg2, 0);
  } else {
    script_0E49(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_2125(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_91ED(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_91ED(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_64D2(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_9A58(arg0, arg2, 0);
  return;
}

static int32_t script_2243(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3i local_D8;
  float local_D4;
  float local_D0;
  float local_CC;
  int32_t unused_C8;
  GameDateTime local_C0;
  int32_t local_BC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_F4 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6);
    if ((local_F4 == 6) | (local_F4 == 7)) {
      Script_ReturnTimeDate(&local_C0);
      if (((&local_C0)->hours >= 22) | ((&local_C0)->hours <= 4)) {
        local_F0 = 1;
        local_F4 = 2;
      } else {
        local_F0 = 0;
      }
    } else {
      local_F0 = 0;
    }
    local_BC = local_F4;
    if (local_BC == 2) {
      local_BC = arg3;
      if ((local_BC == 17) | (local_BC == 11) | (local_BC == 12)) {
      } else {
        local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_91ED(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
        local_BC = arg3;
        if ((local_BC == 2) | (local_BC == 4) | (local_BC == 6) | (local_BC == 8) | (local_BC == 10) | (local_BC == 12) | (local_BC == 24) | (local_BC == 26) | (local_BC == 31) | (local_BC == 33) | (local_BC == 35)) {
          local_D4 = -0.45f;
        } else if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 17) | (local_BC == 18) | (local_BC == 19) | (local_BC == 20) | (local_BC == 21) | (local_BC == 22) | (local_BC == 23) | (local_BC == 29)) {
          local_BC = local_FC;
          if ((local_BC == 0) | (local_BC == 1)) {
            local_D4 = -0.45f;
          } else {
            if ((local_BC == 2) | (local_BC == 3)) {
              local_D4 = 0.45f;
              goto L0023FF;
            }
          }
          L0023FF:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_174 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_168)->x, (&local_E4)->z - task_168.z);
        script_6C5C(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_27B54, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_27B54, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) {
            if ((local_FC == 0) | (local_FC == 1)) {
              local_F8 = 6;
            } else {
              local_F8 = 4;
            }
          } else if ((local_FC == 2) | (local_FC == 3)) {
            local_F8 = 6;
          } else {
            local_F8 = 4;
          }
        } else {
          local_F8 = 4;
        }
      } else {
        local_F8 = 5;
      }
      local_FC = script_91ED(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
      script_6D34(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 0x40) {
              if ((local_FC == 0) | (local_FC == 1)) {
                local_F8 = 6;
              } else {
                local_F8 = 4;
              }
            } else if ((local_FC == 2) | (local_FC == 3)) {
              local_F8 = 6;
            } else {
              local_F8 = 4;
            }
          } else {
            local_F8 = 4;
          }
        } else {
          local_F8 = 5;
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_27B54, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_27B54, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_91ED(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_71AB(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_27B54, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_27B54, SCND_ReadFloatBits, Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_91ED(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          script_6D34(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L0028FA;
      }
    }
  }
  L0028FA:;
  script_9A58(arg0, arg2, 0);
  end:;
}

static void script_2908(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0x42000000);
  if ((arg1 == 5) | (arg1 == 6)) {
    local_F4.x = (&local_F4)->x - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.4f;
    (&local_F4)->z = (&local_F4)->z - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.4f;
  } else {
    local_F4.x = (&local_F4)->x - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.3f;
    (&local_F4)->z = (&local_F4)->z - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.3f;
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_0C64(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_28672)))
static void script_29DE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  Vector3 local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  float local_B0;
  float local_AC;
  int32_t local_A8;
  float local_A4;
  int32_t local_A0;
  float local_9C;
  float local_98;
  float local_94;
  float local_90;
  float local_8C;
  float local_88;
  int16_t local_86;
  int16_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
  if (arg5) {
    Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Get);
    Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Get);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_D8, 0);
    Script_EVT_GetPosition(arg1, -1, &local_CC, 0);
  }
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = 0.0f;
  local_E4.x = 0.0f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 0.0f;
  local_BC = 0.0f;
  local_86 = 0;
  {
    int32_t local_FF78;
    int32_t local_FF74;
    Vector3i local_FF68;
    local_FF74 = 0;
    local_A0 = 0x3F800000;
    local_B4 = 0.0f;
    local_B0 = 0.0f;
    local_AC = 0.0f;
    local_A8 = 0;
    Script_mtwkGetSetRotation(arg1, &local_FF68, 0);
    local_84 = 0;
    if ((arg4 == 1) | (arg4 == 3)) {
      local_FF78 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_160) {
        local_FF78 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_06B2(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_98B6(arg0, 0);
    local_C0.x = (&task_168)->x;
    (&local_C0)->y = task_168.y;
    (&local_C0)->z = task_168.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FF78);
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        break;
      }
      if (Script_GetCharaMoveStatus(arg0) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_FF78 = local_FF78 & 0xFFFF;
    if (task_160 == arg0) {
      Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
      local_FF = 0;
    } else {
      local_FF = 1;
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_B8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    }
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (local_FF) {
        if (!(local_B8 <= local_90)) {
          local_FF = 0;
          script_2908(arg0, task_98[arg2]);
        }
      }
      Script_ReadMOTNPositionAtFrame(arg0, local_FF78, local_B8, &local_E4);
      local_E4.x = (&local_E4)->x * local_A4;
      if (!(local_B8 >= local_98)) {
        local_86 = 0;
      }
      if (!(local_B8 < local_98) & !(local_B8 > local_94)) {
        if (!(local_B8 < local_98) & !(local_B8 >= local_98 + 1.5f)) {
          if (local_84 == 0) {
            int32_t local_FF64;
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_160) {
                Script_executeAicaCommand(local_FF64, 0, 0);
              } else {
                Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
              }
            }
            local_84 = 1;
          }
        }
        Script_EVT_GetPosition(arg0, local_80, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (local_86 == 1) {
          local_B0 = (&local_E4)->x;
          local_BC = (&local_F0)->x;
        }
        local_B4 = (&local_E4)->x + ((&local_F0)->x - local_BC);
        local_B4 = local_B4 - local_B0;
        local_B4 = local_B4 * 1.28f;
        {
          int32_t local_FF64;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 < 0.0f)) {
              local_B4 = 0.0f;
            } else {
              if (!(local_B4 > -0.75f)) {
                local_B4 = -0.75f;
                goto L002F66;
              }
            }
            L002F66:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L002FA4;
                }
              }
              L002FA4:;
              goto L002FA7;
            }
          }
        }
      }
      L002FA7:;
      if (task_160 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_160) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_160 != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_160 == arg0) {
          int32_t local_FF64;
          local_B4 = (&local_E4)->x + ((&local_F0)->x - local_BC);
          local_B4 = local_B4 - local_B0;
          local_B4 = local_B4 * 1.28f;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 > -0.75f)) {
              local_B4 = -0.75f;
            } else {
              if (!(local_B4 < 0.0f)) {
                local_B4 = 0.0f;
                goto L003173;
              }
            }
            L003173:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L0031B1;
                }
              }
              L0031B1:;
              goto L0031B4;
            }
          }
          L0031B4:;
        } else {
          int32_t local_FF64;
          local_B4 = local_B4 - local_AC;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 < 0.0f)) {
              local_B4 = 0.0f;
            }
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              }
              goto L00320D;
            }
          }
        }
      }
      L00320D:;
      if (!(local_8C <= 0.0f) & !(local_B8 <= local_8C)) {
        local_B4 = local_B4 - 0.01f;
        if (!(local_B4 > 0.0f)) {
          local_B4 = 0.0f;
        }
      }
      {
        int32_t local_FF64;
        local_FF64 = arg4;
        switch (local_FF64) {
          case 0:
            local_F0.x = (&local_CC)->x;
            (&local_F0)->y = (&local_CC)->y;
            (&local_F0)->z = (&local_CC)->z;
            if (arg5) {
              local_F0.x = (&local_CC)->x - local_B4;
              Script_charaPosRotUtils(arg1, 7, &local_F0, 0, CHARA_PART_Set);
            } else {
              local_F0.x = (&local_CC)->x + ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_CC)->z + local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 1:
            local_F0.x = (&local_CC)->x;
            (&local_F0)->y = (&local_CC)->y;
            (&local_F0)->z = (&local_CC)->z;
            if (arg5) {
              local_F0.x = (&local_CC)->x + local_B4;
              Script_charaPosRotUtils(arg1, 7, &local_F0, 0, CHARA_PART_Set);
            } else {
              local_F0.x = (&local_CC)->x - ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_CC)->z - local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 2:
            local_F0.x = (&local_D8)->x;
            (&local_F0)->y = (&local_D8)->y;
            (&local_F0)->z = (&local_D8)->z;
            if (arg5) {
              local_F0.x = (&local_D8)->x + local_B4;
              Script_charaPosRotUtils(arg1, 12, &local_F0, 0, CHARA_PART_Set);
            } else {
              local_F0.x = (&local_D8)->x - ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_D8)->z - local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 3:
            local_F0.x = (&local_D8)->x;
            (&local_F0)->y = (&local_D8)->y;
            (&local_F0)->z = (&local_D8)->z;
            if (arg5) {
              local_F0.x = (&local_D8)->x - local_B4;
              Script_charaPosRotUtils(arg1, 12, &local_F0, 0, CHARA_PART_Set);
            } else {
              local_F0.x = (&local_D8)->x + ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_D8)->z + local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
        }
      }
      local_86 = local_86 + 1;
      if (arg0 == task_160) {
        local_B8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
      } else {
        if (!(local_B8 <= local_8C)) {
          break;
        }
        local_B8 = local_B8 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_98B6(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_48[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_160 == arg0) {
        local_80 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_160 == arg0) {
        local_80 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_160 == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_0, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L0038F0;
      }
    }
  }
  L0038F0:;
  return;
}

static void script_38F1(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_160 == arg0) {
    local_FC = task_48[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_395D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3 local_AC;
  Vector3i local_A0;
  int32_t local_9C;
  int32_t local_98;
  float local_94;
  int8_t local_93;
  int8_t local_92;
  uint8_t unused_90[2];
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  if (arg4 == 4) {
    Script_charaPosRotUtils(arg1, 12, &local_C4, 0, CHARA_PART_Get);
    Script_charaPosRotUtils(arg1, 7, &local_B8, 0, CHARA_PART_Get);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_C4, 0);
    Script_EVT_GetPosition(arg1, -1, &local_B8, 0);
  }
  local_AC.x = (&task_168)->x;
  (&local_AC)->y = task_168.y;
  (&local_AC)->z = task_168.z;
  local_DC.x = (&local_C4)->x;
  (&local_DC)->y = (&local_C4)->y;
  (&local_DC)->z = (&local_C4)->z;
  local_D0.x = (&local_B8)->x;
  (&local_D0)->y = (&local_B8)->y;
  (&local_D0)->z = (&local_B8)->z;
  local_9C = 0xC000;
  local_93 = 0;
  local_98 = 0x300;
  Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg1, &local_A0, 0);
  {
    int32_t local_80;
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
    local_80 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_160) {
        Script_executeAicaCommand(local_80, 0, 0);
      } else {
        Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
      }
    }
  }
  local_88 = 0;
  local_8C = 0;
  local_84 = 0;
  for (;;) {
    local_92 = 0;
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
      if (Script_CheckTargetWithinAngleRange(&local_F4, 1.5f, (&local_A0)->y, 0x10000, &local_E8)) {
        local_92 = 1;
      }
    }
    if (local_92) {
      if (local_8C) {
        task_70[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_80 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_70[arg2] = 0;
      }
    }
    local_88 = task_48[arg2];
    local_88 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_48[arg2];
      local_88 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_70[arg2] = 0;
        local_93 = 1;
      }
    }
    local_94 = (Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_9C) + 1.0f) * 0.45f;
    {
      int32_t local_80;
      local_80 = arg4;
      if (local_80 == 4) {
        local_DC.x = (&local_C4)->x + local_94;
        local_D0.x = (&local_B8)->x - local_94;
        Script_charaPosRotUtils(arg1, 12, &local_DC, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_D0, 0, CHARA_PART_Set);
      } else if ((local_80 == 0) | (local_80 == 1)) {
        local_DC.x = (&local_C4)->x - ((float)0 - local_94) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_A0)->y);
        (&local_DC)->z = (&local_C4)->z - local_94 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_A0)->y);
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_DC);
      } else {
        if ((local_80 == 2) | (local_80 == 3)) {
          local_D0.x = (&local_B8)->x + ((float)0 - local_94) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_A0)->y);
          (&local_D0)->z = (&local_B8)->z + local_94 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_A0)->y);
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D0);
          goto L003DC7;
        }
      }
    }
    L003DC7:;
    SCNF_WaitFrames_0000(1);
  }
  if (arg4 == 4) {
    Script_charaPosRotUtils(arg1, 12, &local_C4, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg1, 7, &local_B8, 0, CHARA_PART_Set);
  } else {
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_C4);
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_B8);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2868E)))
static void script_3E17(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  float local_C8;
  float local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  int8_t local_B7;
  uint8_t unused_B4[3];
  Vector3i local_A8;
  int32_t local_A4;
  local_A4 = arg3;
  switch (local_A4) {
    case 24:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
    case 27:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
      }
      break;
    case 25:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
      }
      break;
    case 26:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_98B6(arg0, 0);
  local_CC.x = (&task_168)->x;
  (&local_CC)->y = task_168.y;
  (&local_CC)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FC);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_FC = local_FC & 0xFFFF;
  Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
  Script_mtwkGetSetRotation(arg1, &local_A8, 0);
  local_F4 = (&local_A8)->y;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    local_C8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_C8 < local_C4) & !(local_C8 > local_C0)) {
      if (local_B7 == 1) {
        local_BC = local_BC - 0.3f;
      }
      if (!(local_C8 < local_C4) & !(local_C8 >= local_C4 + 1.0f)) {
        int32_t local_A0;
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_A0, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_A0, 4, &local_CC, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_F8, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      local_F4 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&local_D8)->x, (&local_E4)->z - (&local_D8)->z);
      local_F4 = local_F4 + local_F0 + (int32_t)(local_BC * local_B8 / 360.0f * (float)0x10000);
    }
    {
      Vector3i local_98;
      (&local_98)->y = local_F4;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_98, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_98B6(arg0, 1);
  local_FC = task_48[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_286AA)))
static void script_44D1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_DC[3];
  Vector3 local_D0;
  int8_t local_CF;
  int8_t local_CE;
  uint8_t unused_CC[2];
  float local_C8;
  float local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  float local_B0;
  float local_AC;
  float local_A8;
  int32_t local_A4;
  int32_t local_A0;
  int32_t local_9C;
  int32_t local_98;
  int32_t local_94;
  int32_t local_90;
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  float local_FF78;
  float local_FF74;
  int32_t local_FF70;
  Vector3i local_FF64;
  int32_t local_FF60;
  local_CE = 0;
  local_80 = 0;
  local_FF78 = 1.0f;
  local_FF74 = 0.0f;
  {
    int32_t local_FF5C;
    local_FF5C = arg3;
    if ((local_FF5C == 4) | (local_FF5C == 6) | (local_FF5C == 33) | (local_FF5C == 35)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 17294;
            local_8C = 15473;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 48241;
            local_8C = 50062;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_98[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_98[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 48241;
            local_8C = 50062;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 17294;
            local_8C = 15473;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_160) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = 1.0f;
              local_A0 = 12;
              local_9C = 18;
              local_90 = 17294;
              local_8C = 15473;
              break;
          }
        } else {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_98[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_98[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L0050C4;
      }
    }
  }
  L0050C4:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
  local_C8 = 0.0f;
  local_FF7C = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1)) {
    if (local_CE) {
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Script_charaPosRotUtils(arg1, 12, 0, &local_FF50, CHARA_PART_Get);
        local_98 = (&local_FF50)->y;
      }
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Script_charaPosRotUtils(arg1, 7, 0, &local_FF50, CHARA_PART_Get);
        local_94 = (&local_FF50)->y;
      }
      if ((arg4 == 2) | (arg4 == 3)) {
        local_88 = local_98;
        Script_charaPosRotUtils(arg1, 12, &local_F4, 0, CHARA_PART_Get);
      } else {
        local_88 = local_94;
        Script_charaPosRotUtils(arg1, 7, &local_F4, 0, CHARA_PART_Get);
      }
      local_FF70 = local_88;
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Vector3 local_FF44;
        Script_EVT_GetPosition(arg1, -1, &local_FF44, 0);
        Script_mtwkGetSetRotation(arg1, &local_FF50, 0);
        local_84 = (&local_FF50)->y;
        {
          float local_FF40;
          float local_FF3C;
          local_FF3C = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84) - (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_84);
          local_FF40 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_84) + (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84);
          local_F4.x = local_FF40 + (&local_FF44)->x;
          (&local_F4)->y = (&local_F4)->y + (&local_FF44)->y;
          (&local_F4)->z = local_FF3C + (&local_FF44)->z;
        }
      }
    } else {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      Script_mtwkGetSetRotation(arg1, &local_FF50, 0);
      local_98 = (&local_FF50)->y;
      local_94 = (&local_FF50)->y;
      local_88 = (&local_FF50)->y;
      local_FF70 = (&local_FF50)->y;
      Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
      local_84 = 0;
    }
  }
  script_06B2(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_98B6(arg0, 0);
  local_D0.x = (&task_168)->x;
  (&local_D0)->y = task_168.y;
  (&local_D0)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_A4);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = local_A4 & 0xFFFF;
  {
    int32_t local_FF5C;
    int8_t local_FF5B;
    if (arg0 == task_160) {
      (void)((local_FF5B = 0) & 0x80);
    } else {
      (void)((local_FF5B = -1) & 0x80);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_C4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    }
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (arg0 == task_160) {
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
        local_C4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
      }
      if (local_FF5B & 8) {
        if (!(local_C4 <= local_B0)) {
          (void)((local_FF5B = 0) & 1);
          {
            uint8_t unused_FF54[7];
            int32_t local_FF50;
            local_FF50 = 1;
            if (local_FF50) {
              script_2908(arg0, task_98[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_160) {
            if (local_CF == 1) {
              local_C8 = local_C8 - 0.165526f;
            } else {
              local_C8 = local_C8 - 0.145973f;
            }
          }
        }
        if (!(local_C4 < local_B8) & !(local_C4 >= local_B8 + 1.0f)) {
          uint8_t unused_FF54[7];
          int32_t local_FF50;
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_160) {
              Script_executeAicaCommand(local_FF50, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
            }
          }
        }
        Script_EVT_GetPosition(arg0, local_A0, &local_E8, ENTITY_XFORM_WORLD_SPACE);
        local_88 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
        local_88 = local_88 - local_84;
        local_88 = local_88 + local_90 + (int32_t)(local_C8 * local_C0 / 360.0f * (float)0x10000);
        if (local_CE) {
          if ((arg4 == 2) | (arg4 == 3)) {
            Script_charaPosRotUtils(arg1, 12, 0, &local_FF64, CHARA_PART_Get);
          } else {
            Script_charaPosRotUtils(arg1, 7, 0, &local_FF64, CHARA_PART_Get);
          }
        } else {
          Script_mtwkGetSetRotation(arg1, &local_FF64, 0);
        }
        if ((local_CF == 0) & !(local_C4 >= local_B8)) {
          local_88 = (&local_FF64)->y;
        }
        local_FF60 = script_063A((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L0056C9;
          }
        }
        L0056C9:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_063A(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_063A(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_160) {
        if (!(local_B4 > local_C4) & !(local_C4 >= local_B4 + 1.0f)) {
          uint8_t unused_FF54[7];
          Vector3i local_FF48;
          if (local_CE) {
            if ((arg4 == 2) | (arg4 == 3)) {
              Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, CHARA_PART_Get);
            } else {
              Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, CHARA_PART_Get);
            }
          } else {
            Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
          }
          local_FF74 = (float)script_05FD(script_063A(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
          if (!(local_FF74 < 90.0f)) {
            local_BC = 0.0f;
          } else {
            local_FF74 = local_FF74 + local_BC;
            if (!(local_FF74 <= 90.0f)) {
              local_BC = local_FF74 - 90.0f;
            }
          }
          local_88 = local_88 - (int32_t)(local_BC * local_C0 / 360.0f * (float)0x10000);
        }
      }
      if (!(local_C4 < local_B0) & !(local_C4 >= local_B0 + 1.0f)) {
        if (local_80 == 0) {
          if (arg0 != task_160) {
            uint8_t unused_FF54[7];
            Vector3i local_FF48;
            int32_t local_FF44;
            if (local_CE) {
              if ((arg4 == 2) | (arg4 == 3)) {
                Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, CHARA_PART_Get);
              } else {
                Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, CHARA_PART_Get);
              }
            } else {
              Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
            }
            if (local_CE) {
              if ((arg4 == 2) | (arg4 == 3)) {
                if (!((float)(((&local_FF48)->y - local_98) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
                  local_FF44 = local_98 + 0x10000 - (&local_FF48)->y;
                } else {
                  local_FF44 = (&local_FF48)->y - local_98;
                }
              } else if (!((float)(((&local_FF48)->y - local_94) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
                local_FF44 = local_94 + 0x10000 - (&local_FF48)->y;
              } else {
                local_FF44 = (&local_FF48)->y - local_94;
              }
            } else if (!((float)(((&local_FF48)->y - local_98) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
              local_FF44 = local_98 + 0x10000 - (&local_FF48)->y;
            } else {
              local_FF44 = (&local_FF48)->y - local_98;
            }
            local_FF7C = (int32_t)((float)(local_FF44 & 0xFFFF) * 360.0f / (float)0x10000 / (local_AC - local_B0) * local_C0 / 360.0f * (float)0x10000);
            local_80 = 1;
          }
        }
      }
      if (!(local_C4 < local_AC) & !(local_C4 >= local_AC + 1.0f)) {
        uint8_t unused_FF54[7];
        int32_t local_FF50;
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_48[arg2];
        if (arg0 == task_160) {
          if (task_160 == arg0) {
            local_A0 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_160) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L005B75;
            }
          }
        }
      }
      L005B75:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_160) {
          if (local_CF == 0) {
            local_C8 = local_C8 - 0.11939f;
          } else {
            local_C8 = local_C8 + 2.381415f;
          }
          Script_EVT_GetPosition(arg0, local_9C, &local_E8, ENTITY_XFORM_WORLD_SPACE);
          local_88 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
          local_88 = local_88 - local_84;
          local_88 = local_88 + local_8C + (int32_t)(local_C8 * local_C0 / 360.0f * (float)0x10000);
        } else {
          local_88 = local_88 + local_FF7C;
        }
      }
      if (local_CE) {
        uint8_t unused_FF54[7];
        Vector3i local_FF48;
        local_FF48.x = 0;
        (&local_FF48)->z = 0;
        if ((arg4 == 2) | (arg4 == 3)) {
          (&local_FF48)->y = local_88;
          Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, CHARA_PART_Set);
        } else {
          (&local_FF48)->y = local_88;
          Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, CHARA_PART_Set);
        }
      } else {
        uint8_t unused_FF54[7];
        Vector3i local_FF48;
        (&local_FF48)->y = local_88;
        Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF48, 0);
      }
      if (arg0 != task_160) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_98B6(arg0, 1);
  if (local_CE) {
    int32_t unused_FF5C;
    Vector3i local_FF50;
    local_FF50.x = 0;
    (&local_FF50)->z = 0;
    (&local_FF50)->y = local_98;
    Script_charaPosRotUtils(arg1, 12, 0, &local_FF50, CHARA_PART_Set);
    (&local_FF50)->y = local_94;
    Script_charaPosRotUtils(arg1, 7, 0, &local_FF50, CHARA_PART_Set);
  } else {
    {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      (&local_FF50)->y = local_98;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF50, 0);
    }
    {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      (&local_FF50)->y = local_94;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF50, 0);
    }
  }
  if (arg0 == task_160) {
    int32_t local_FF5C;
    local_FF5C = arg3;
    if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
      for (;;) {
        if (Script_CamCtrl(CamCtrlCmd_CheckPosRotCollision) != -1) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      DSWK_SetActorOpacityAndDrawCallback(arg1, 1.0f);
      goto L005DB0;
    }
  }
  L005DB0:;
  return;
}

__attribute__((scn_string_bindings(0, data_286C6)))
static void script_5DB1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  int32_t local_B0;
  float local_AC;
  float local_A8;
  float local_A4;
  int16_t local_A2;
  uint8_t unused_A0[2];
  int32_t local_9C;
  int32_t local_98;
  local_A8 = 30.0f;
  local_A4 = 60.0f;
  if (arg5) {
    Script_charaPosRotUtils(arg1, 12, &local_DC, 0, CHARA_PART_Get);
    Script_charaPosRotUtils(arg1, 7, &local_D0, 0, CHARA_PART_Get);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_DC, 0);
    Script_EVT_GetPosition(arg1, -1, &local_D0, 0);
  }
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  local_C0 = 0.0f;
  local_A2 = 0;
  local_B8 = 0.0f;
  local_B4 = 0.0f;
  local_B0 = 0;
  {
    int32_t local_94;
    Vector3i local_88;
    Script_mtwkGetSetRotation(arg1, &local_88, 0);
    if ((arg4 == 1) | (arg4 == 3)) {
      local_94 = 1395;
      local_98 = 18;
      local_9C = 18;
      local_AC = 1.0f;
    } else {
      local_94 = -0x7FFFFA8D;
      local_98 = 12;
      local_9C = 12;
      local_AC = -1.0f;
    }
    local_94 = local_94 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_A8 - 3.0f) << 16) & 0x7FFF0000);
    script_98B6(arg0, 0);
    local_C4.x = (&task_168)->x;
    (&local_C4)->y = task_168.y;
    (&local_C4)->z = task_168.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_94);
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        break;
      }
      if (Script_GetCharaMoveStatus(arg0) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_94 = local_94 & 0xFFFF;
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      local_BC = Script_GetCharaMotionRemainingFramesScaled(arg0);
      Script_ReadMOTNPositionAtFrame(arg0, local_94, local_BC, &local_E8);
      local_E8.x = (&local_E8)->x * local_AC;
      if (!(local_BC >= local_A8)) {
        local_A2 = 0;
      }
      if (!(local_BC < local_A8) & !(local_BC > local_A4)) {
        if (!(local_BC < local_A8) & !(local_BC >= local_A8 + 1.0f)) {
          int32_t local_84;
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_84 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_160) {
              Script_executeAicaCommand(local_84, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_84, 4, &local_C4, 5.0f, 20.0f, 0);
            }
          }
        }
        Script_EVT_GetPosition(arg0, local_9C, &local_F4, ENTITY_XFORM_MOTION_SPACE);
        if (local_A2 == 1) {
          local_B4 = (&local_E8)->x;
          local_C0 = (&local_F4)->x;
        }
        local_B8 = (&local_E8)->x + ((&local_F4)->x - local_C0);
        local_B8 = local_B8 - local_B4;
        local_B8 = local_B8 * 1.28f;
        {
          int32_t local_84;
          local_84 = arg4;
          if ((local_84 == 0) | (local_84 == 2)) {
            if (!(local_B8 < 0.75f)) {
              local_B8 = 0.75f;
            } else {
              if (!(local_B8 > 0.0f)) {
                local_B8 = 0.0f;
                goto L0060D5;
              }
            }
            L0060D5:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L006111;
                }
              }
              L006111:;
              goto L006114;
            }
          }
        }
      }
      L006114:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L006159;
          }
        }
      }
      L006159:;
      {
        int32_t local_84;
        local_84 = arg4;
        switch (local_84) {
          case 0:
            local_F4.x = (&local_D0)->x;
            (&local_F4)->y = (&local_D0)->y;
            (&local_F4)->z = (&local_D0)->z;
            if (arg5) {
              local_F4.x = (&local_D0)->x - local_B8;
              Script_charaPosRotUtils(arg1, 7, &local_F4, 0, CHARA_PART_Set);
            } else {
              local_F4.x = (&local_D0)->x + ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_D0)->z + local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 1:
            local_F4.x = (&local_D0)->x;
            (&local_F4)->y = (&local_D0)->y;
            (&local_F4)->z = (&local_D0)->z;
            if (arg5) {
              local_F4.x = (&local_D0)->x + local_B8;
              Script_charaPosRotUtils(arg1, 7, &local_F4, 0, CHARA_PART_Set);
            } else {
              local_F4.x = (&local_D0)->x - ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_D0)->z - local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 2:
            local_F4.x = (&local_DC)->x;
            (&local_F4)->y = (&local_DC)->y;
            (&local_F4)->z = (&local_DC)->z;
            if (arg5) {
              local_F4.x = (&local_DC)->x + local_B8;
              Script_charaPosRotUtils(arg1, 12, &local_F4, 0, CHARA_PART_Set);
            } else {
              local_F4.x = (&local_DC)->x - ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_DC)->z - local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 3:
            local_F4.x = (&local_DC)->x;
            (&local_F4)->y = (&local_DC)->y;
            (&local_F4)->z = (&local_DC)->z;
            if (arg5) {
              local_F4.x = (&local_DC)->x - local_B8;
              Script_charaPosRotUtils(arg1, 12, &local_F4, 0, CHARA_PART_Set);
            } else {
              local_F4.x = (&local_DC)->x + ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_DC)->z + local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
        }
      }
      local_A2 = local_A2 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_98B6(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_48[arg2];
    local_9C = Script_ScenDataUtil(task_0, SCND_ReadU32, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_286E2)))
static void script_64D2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3i local_F4;
  Vector3 local_E8;
  float local_E4;
  float local_E0;
  Vector3 local_D4;
  Vector3 local_C8;
  int32_t unused_BC[3];
  Vector3 local_B0;
  int8_t local_AF;
  uint8_t unused_AC[3];
  float local_A8;
  float local_A4;
  float local_A0;
  float local_9C;
  float local_98;
  float local_94;
  float local_90;
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  int32_t local_FF78;
  int32_t local_FF74;
  int32_t local_FF70;
  int32_t local_FF6C;
  int32_t local_FF68;
  local_FF68 = arg4;
  switch (local_FF68) {
    case 0:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_27134, SCND_ReadU32, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_27134, SCND_ReadFloatBits, Script_ScenDataUtil(data_26B44, SCND_ReadU8, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  local_A8 = 0.0f;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, CHARA_PART_Get);
  local_80 = (&local_F4)->y;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, CHARA_PART_Get);
  local_FF7C = (&local_F4)->y;
  if ((arg4 == 2) | (arg4 == 3)) {
    local_FF70 = local_80;
    Script_charaPosRotUtils(arg1, 12, &local_D4, 0, CHARA_PART_Get);
  } else {
    local_FF70 = local_FF7C;
    Script_charaPosRotUtils(arg1, 7, &local_D4, 0, CHARA_PART_Get);
  }
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg1, &local_F4, 0);
  local_FF6C = (&local_F4)->y;
  local_E0 = (&local_D4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF6C) - (&local_D4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF6C);
  local_E4 = (&local_D4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF6C) + (&local_D4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF6C);
  local_D4.x = local_E4 + (&local_E8)->x;
  (&local_D4)->y = (&local_D4)->y + (&local_E8)->y;
  (&local_D4)->z = local_E0 + (&local_E8)->z;
  local_8C = local_8C | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_9C - 3.0f) << 16) & 0x7FFF0000);
  script_98B6(arg0, 0);
  local_B0.x = (&task_168)->x;
  (&local_B0)->y = task_168.y;
  (&local_B0)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_8C);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_8C = local_8C & 0xFFFF;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    local_A4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_A4 < local_9C) & !(local_A4 > local_98)) {
      if (local_AF == 1) {
      } else {
        local_A8 = local_A8 - 0.208874f;
      }
      if (!(local_A4 < local_9C) & !(local_A4 >= local_9C + 1.0f)) {
        int32_t local_FF64;
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF64, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_B0, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_88, &local_C8, ENTITY_XFORM_WORLD_SPACE);
      local_FF70 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D4)->x - (&local_C8)->x, (&local_D4)->z - (&local_C8)->z);
      local_FF70 = local_FF70 - local_FF6C;
      local_FF70 = local_FF70 + local_FF78 + (int32_t)(local_A8 * local_A0 / 360.0f * (float)0x10000);
    }
    if (!(local_A4 < local_94) & !(local_A4 > local_90)) {
      if (local_AF == 0) {
      } else {
        local_A8 = local_A8 + 0.12844f;
      }
      if (!(local_A4 < local_90) & !(local_A4 >= local_90 + 1.0f)) {
        int32_t local_FF64;
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF64, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_B0, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_84, &local_C8, ENTITY_XFORM_WORLD_SPACE);
      local_FF70 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D4)->x - (&local_C8)->x, (&local_D4)->z - (&local_C8)->z);
      local_FF70 = local_FF70 - local_FF6C;
      local_FF70 = local_FF70 + local_FF74 + (int32_t)(local_A8 * local_A0 / 360.0f * (float)0x10000);
    }
    local_F4.x = 0;
    (&local_F4)->z = 0;
    if ((arg4 == 2) | (arg4 == 3)) {
      (&local_F4)->y = local_FF70;
      Script_charaPosRotUtils(arg1, 12, 0, &local_F4, CHARA_PART_Set);
    } else {
      (&local_F4)->y = local_FF70;
      Script_charaPosRotUtils(arg1, 7, 0, &local_F4, CHARA_PART_Set);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_98B6(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, CHARA_PART_Set);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, CHARA_PART_Set);
  return;
}

static void script_6C5C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), MN_AKI_AKI_BASE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = 0;
  for (;;) {
    if (local_F8 == 0) {
      local_F8 = 1;
      if (arg5 != 0) {
        local_FC = 11;
      } else {
        local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 7);
      }
      if (local_FC == 0) {
        SCNF_WaitFrames_0000(45);
      }
      Script_EnqueueSCNT(1, 0x47524F44, local_FC);
    } else {
      if (Script_EnqueueSCNT(2) == 0) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_286FE)))
static void script_6D34(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_F0;
  float local_EC;
  float local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t unused_D0;
  int32_t local_CC;
  local_DC = 0;
  local_D4 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_27B54, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_27B54, SCND_ReadU32, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_27B54, SCND_ReadU32, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_27B54, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_27B54, SCND_ReadU32, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_27B54, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_27B54, SCND_ReadU32, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_27B54, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006F55;
    }
  }
  L006F55:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_27B54, SCND_ReadFloatBits, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_27B54, SCND_ReadFloatBits, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_168)->x;
  (&local_F4)->y = task_168.y;
  (&local_F4)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_E0);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      if (local_DC == 1) {
        if (Script_EnqueueSCNT(2) == 0) {
          break;
        }
      }
    }
    local_F0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_F0 < local_EC) & !(local_F0 >= local_EC + 1.0f)) {
      int32_t local_C8;
      local_C8 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 7);
        if (local_D8 == 0) {
          SCNF_WaitFrames_0000(45);
        }
        Script_EnqueueSCNT(1, 0x47524F44, local_D8);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2871A)))
static void script_71AB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  float local_DC[3];
  Vector3 local_D0;
  float local_C4[3];
  Vector3 local_B8;
  int32_t unused_AC[3];
  int8_t local_AB;
  int8_t local_AA;
  uint8_t unused_A8[2];
  float local_A4;
  float local_A0;
  float local_9C;
  float local_98;
  float local_94;
  int32_t local_90;
  int32_t local_8C;
  int32_t unused_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  int32_t local_FF78;
  int32_t local_FF74;
  int32_t unused_FF70;
  int32_t local_FF6C;
  int32_t local_FF68;
  int32_t local_FF64;
  int32_t local_FF60;
  int32_t local_FF5C;
  int32_t local_FF58;
  int32_t unused_FF54;
  Vector3i local_FF48;
  local_AA = 0;
  local_FF5C = 0;
  local_FF58 = 0;
  task_18C = 0;
  local_90 = 1543;
  Script_EVT_GetPosition(arg1, -1, &local_D0, 0);
  Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
  local_DC[0] = 0.0f;
  local_DC[1] = 0.0f;
  local_DC[2] = 0.0f;
  {
    int32_t local_FF44;
    local_FF44 = arg3;
    if ((local_FF44 == 4) | (local_FF44 == 6) | (local_FF44 == 33) | (local_FF44 == 35)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 1;
          local_9C = 1.0f;
          local_FF74 = 0;
          break;
        case 0:
          local_AB = 0;
          local_90 = local_90 | -0x80000000;
          local_9C = 1.0f;
          local_FF74 = 1;
          break;
      }
    } else if ((local_FF44 == 1) | (local_FF44 == 7) | (local_FF44 == 30) | (local_FF44 == 36) | (local_FF44 == 37)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 0;
          local_90 = local_90 | -0x80000000;
          local_9C = 1.0f;
          local_FF74 = 1;
          break;
        case 0:
          local_AB = 1;
          local_9C = 1.0f;
          local_FF74 = 0;
          break;
      }
    } else if ((local_FF44 == 3) | (local_FF44 == 5) | (local_FF44 == 32) | (local_FF44 == 34)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 1;
          local_90 = local_90 | -0x80000000;
          local_9C = -1.0f;
          local_FF74 = 1;
          break;
        case 0:
          local_AB = 0;
          local_9C = -1.0f;
          local_FF74 = 0;
          break;
      }
    } else if ((local_FF44 == 2) | (local_FF44 == 8) | (local_FF44 == 31)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 0;
          local_9C = -1.0f;
          local_FF74 = 0;
          break;
        case 0:
          local_AB = 1;
          local_90 = local_90 | -0x80000000;
          local_9C = -1.0f;
          local_FF74 = 1;
          break;
      }
    } else {
      if ((local_FF44 == 13) | (local_FF44 == 14)) {
        local_FF44 = arg4;
        switch (local_FF44) {
          case 0:
            local_AB = 0;
            local_9C = -1.0f;
            local_FF74 = 0;
            break;
          case 1:
            local_AB = 1;
            local_90 = local_90 | -0x80000000;
            local_9C = -1.0f;
            local_FF74 = 1;
            break;
          case 2:
            local_AB = 1;
            local_9C = 1.0f;
            local_FF74 = 0;
            break;
          case 3:
            local_AB = 0;
            local_90 = local_90 | -0x80000000;
            local_9C = 1.0f;
            local_FF74 = 1;
            break;
        }
        local_AA = 1;
        goto L0074B7;
      }
    }
  }
  L0074B7:;
  if (local_AA) {
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, CHARA_PART_Get);
      local_84 = (&local_FF38)->y;
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, CHARA_PART_Get);
      local_80 = (&local_FF38)->y;
    }
    if ((arg4 == 2) | (arg4 == 3)) {
      local_FF7C = local_84;
      Script_charaPosRotUtils(arg1, 12, &local_F4, 0, CHARA_PART_Get);
    } else {
      local_FF7C = local_80;
      Script_charaPosRotUtils(arg1, 7, &local_F4, 0, CHARA_PART_Get);
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Vector3 local_FF2C;
      Script_EVT_GetPosition(arg1, -1, &local_FF2C, 0);
      Script_mtwkGetSetRotation(arg1, &local_FF38, 0);
      local_FF78 = (&local_FF38)->y;
      {
        float local_FF28;
        float local_FF24;
        local_FF24 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF78) - (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF78);
        local_FF28 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF78) + (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF78);
        local_F4.x = local_FF28 + (&local_FF2C)->x;
        (&local_F4)->y = (&local_F4)->y + (&local_FF2C)->y;
        (&local_F4)->z = local_FF24 + (&local_FF2C)->z;
      }
    }
  } else {
    int32_t unused_FF44;
    Vector3i local_FF38;
    Script_mtwkGetSetRotation(arg1, &local_FF38, 0);
    local_84 = (&local_FF38)->y;
    local_80 = (&local_FF38)->y;
    local_FF7C = (&local_FF38)->y;
    Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
    local_FF78 = 0;
  }
  local_98 = 33.0f;
  local_94 = 49.0f;
  local_B8.x = (&task_168)->x;
  (&local_B8)->y = task_168.y;
  (&local_B8)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_90);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x524F4F44, 1);
  while ((local_8C = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x524F4F44)) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_8C) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(SHEN_NAME('DOOR'), MOTM_PoseNone, SCEN_Write);
  Script_PlayCharaMotion(SHEN_NAME('DOOR'), MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  {
    int32_t local_FF44;
    local_FF44 = arg3;
    if ((local_FF44 == 0) | (local_FF44 == 1) | (local_FF44 == 3) | (local_FF44 == 5) | (local_FF44 == 7) | (local_FF44 == 9) | (local_FF44 == 11) | (local_FF44 == 25) | (local_FF44 == 27) | (local_FF44 == 30) | (local_FF44 == 32) | (local_FF44 == 34) | (local_FF44 == 36) | (local_FF44 == 37)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if ((local_FF44 == 2) | (local_FF44 == 4) | (local_FF44 == 6) | (local_FF44 == 8) | (local_FF44 == 10) | (local_FF44 == 12) | (local_FF44 == 24) | (local_FF44 == 26) | (local_FF44 == 31) | (local_FF44 == 33) | (local_FF44 == 35)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if ((local_FF44 == 13) | (local_FF44 == 14) | (local_FF44 == 15) | (local_FF44 == 16) | (local_FF44 == 17) | (local_FF44 == 18) | (local_FF44 == 19) | (local_FF44 == 20) | (local_FF44 == 21) | (local_FF44 == 22) | (local_FF44 == 23) | (local_FF44 == 29)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if (local_FF44 == 28) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
      }
    } else {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
      }
    }
  }
  local_FF60 = (&local_FF48)->y & 0xFFFF;
  local_C4[2] = local_DC[2] * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF60) - *local_DC * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF60);
  local_C4[0] = local_DC[2] * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF60) + *local_DC * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF60);
  local_D0.x = (&local_F4)->x + *local_C4;
  (&local_D0)->y = (&local_F4)->y;
  (&local_D0)->z = (&local_F4)->z + local_C4[2];
  Script_EVT_SetPosition(SHEN_NAME('DOOR'), POS_XYZ, &local_D0);
  Script_EVT_SetRotation(SHEN_NAME('DOOR'), 0x38000000, &local_FF48, 0);
  local_A4 = 0.0f;
  local_FF68 = 0;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      if (local_FF68 == 1) {
        if (Script_EnqueueSCNT(2) == 0) {
          break;
        }
      }
    }
    if (local_FF68 == 1) {
      if (local_FF5C == 0) {
        int32_t unused_FF44;
        int32_t local_FF40;
        SCNF_WaitFrames_0000(45);
        local_FF40 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF40, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
          }
        }
        local_FF5C = 1;
      } else {
        if (local_FF58 <= 20) {
          local_A4 = local_A4 - 0.05f;
          local_FF7C = local_FF7C + (int32_t)(local_A4 * local_9C / 360.0f * (float)0x10000);
          if (local_AA) {
            int32_t unused_FF44;
            Vector3i local_FF38;
            local_FF38.x = 0;
            (&local_FF38)->z = 0;
            if ((arg4 == 2) | (arg4 == 3)) {
              (&local_FF38)->y = local_FF7C;
              Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, CHARA_PART_Set);
            } else {
              (&local_FF38)->y = local_FF7C;
              Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, CHARA_PART_Set);
            }
          } else {
            int32_t unused_FF44;
            Vector3i local_FF38;
            (&local_FF38)->y = local_FF7C;
            Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
          }
          local_FF58 = local_FF58 + 1;
        }
      }
    }
    local_A0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_A0 < local_98) & !(local_A0 >= local_98 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    local_FF6C = 0;
    L007F82:;
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
    } else {
      local_A4 = local_A4 + 0.05f;
      local_FF7C = local_FF7C + (int32_t)(local_A4 * local_9C / 360.0f * (float)0x10000);
      if (local_AA) {
        int32_t unused_FF44;
        Vector3i local_FF38;
        local_FF38.x = 0;
        (&local_FF38)->z = 0;
        if ((arg4 == 2) | (arg4 == 3)) {
          (&local_FF38)->y = local_FF7C;
          Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, CHARA_PART_Set);
        } else {
          (&local_FF38)->y = local_FF7C;
          Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, CHARA_PART_Set);
        }
      } else {
        int32_t unused_FF44;
        Vector3i local_FF38;
        (&local_FF38)->y = local_FF7C;
        Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
      }
      SCNF_WaitFrames_0000(1);
      local_FF6C = local_FF6C + 1;
      if (!(local_FF6C >= 20)) goto L007F82;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
  }
  if (local_AA) {
    int32_t unused_FF44;
    Vector3i local_FF38;
    local_FF38.x = 0;
    (&local_FF38)->z = 0;
    (&local_FF38)->y = local_84;
    Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, CHARA_PART_Set);
    (&local_FF38)->y = local_80;
    Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, CHARA_PART_Set);
  } else {
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      (&local_FF38)->y = local_84;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      (&local_FF38)->y = local_80;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
    }
  }
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_8C);
  return;
}

static void script_813E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t unused_F8;
  float local_EC[3];
  Vector3 local_E0;
  float local_D4[3];
  Vector3 local_C8;
  int32_t unused_C4;
  int32_t local_BC[2];
  Vector3i local_B0;
  int32_t local_AC;
  Script_sprintf("open-move!!\012");
  local_BC[1] = arg4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    Script_EVT_GetPosition(arg1, -1, &local_E0, 0);
    Script_EVT_GetPosition(arg0, -1, &local_C8, 0);
    Script_mtwkGetSetRotation(arg0, &local_B0, 0);
    local_EC[0] = 0.0f;
    local_EC[1] = 0.0f;
    local_EC[2] = 0.0f;
    if (arg0 != task_160) {
      local_AC = arg2;
      if ((local_AC == 0) | (local_AC == 1) | (local_AC == 3) | (local_AC == 5) | (local_AC == 7) | (local_AC == 9) | (local_AC == 11) | (local_AC == 25) | (local_AC == 27) | (local_AC == 30) | (local_AC == 32) | (local_AC == 34) | (local_AC == 36) | (local_AC == 37)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else if ((local_AC == 2) | (local_AC == 4) | (local_AC == 6) | (local_AC == 8) | (local_AC == 10) | (local_AC == 12) | (local_AC == 24) | (local_AC == 26) | (local_AC == 31) | (local_AC == 33) | (local_AC == 35)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else if ((local_AC == 13) | (local_AC == 14) | (local_AC == 15) | (local_AC == 16) | (local_AC == 17) | (local_AC == 18) | (local_AC == 19) | (local_AC == 20) | (local_AC == 21) | (local_AC == 22) | (local_AC == 23) | (local_AC == 29) | (local_AC == 28)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      }
      local_FC = local_BC[1] & 0xFFFF;
      local_D4[2] = local_EC[2] * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FC) - *local_EC * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FC);
      local_D4[0] = local_EC[2] * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FC) + *local_EC * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FC);
      local_E0.x = (&local_E0)->x + *local_D4;
      (&local_E0)->z = (&local_E0)->z + local_D4[2];
      if ((arg3 == 0) | (arg3 == 3)) {
        local_BC[1] = local_BC[1] + 0x8000;
      }
      Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_0C64(arg5));
    }
  }
  return;
}

static void script_8588(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_0 = arg1;
  task_4 = arg2;
  task_8 = arg3;
  task_C = arg4;
  task_10.mode = 0;
  task_10.id = 0;
  task_10.entry_idx = 0;
  task_160 = arg0;
  (void)((task_184 = -1) & 1);
  task_180 = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L0085F2:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_E8[local_F8] = local_FC;
  task_20[local_F8] = -1;
  task_48[local_F8] = -1;
  task_70[local_F8] = 0;
  task_98[local_F8] = 0;
  task_C0[local_F8] = 0;
  task_110[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L0085F2;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_10, task_20, task_48, (shen_name *)task_70, task_98, arg0, arg5, data_26B44, data_27134, (shen_name *)task_E8);
  Script_scenCreateTask(script_8E44, 0);
  return;
}

static void script_8714(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_0 = arg0;
  task_4 = arg1;
  task_8 = arg2;
  task_C = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_180 = 1;
  return;
}

static void script_8748(void) {
  int32_t local_FC;
  local_FC = 0;
  L008750:;
  task_20[local_FC] = -1;
  task_48[local_FC] = -1;
  task_70[local_FC] = 0;
  task_98[local_FC] = 0;
  task_C0[local_FC] = 0;
  task_110[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L008750;
  return;
}

static void script_87B1(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_160 == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_194, 0);
    }
    local_F8 = DSWK_DoorUtils(DOOR_CMD_ALLOC_ENTRY_SLOT, arg0, arg1);
    if (local_F8 != -1) {
      for (;;) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_TYPE13, local_F8);
        if (local_FC != 2) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      if (local_FC != -2) {
        break;
      }
      local_F4 = local_F4 + 1;
      if (local_F4 >= 120) {
        Script_sprintf("無限ループになりそうなので抜ける\012");
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  (void)local_FC;
  return;
}

static void script_884E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_160 == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_194, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_8888(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_88EB(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_894E(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_8985(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_89BC(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_48[local_F0] == arg0) {
      local_EC = task_E8[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F0] * 13 + 1) & 0x100) {
            local_F4.x = 0.0f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(local_EC, 5, &local_F4, 0, CHARA_PART_Set);
          } else {
            local_F4.x = 0.0f;
            (&local_F4)->y = -1200.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(local_EC, 5, &local_F4, 0, CHARA_PART_Set);
          }
        }
      } else {
        if (Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F0] * 13 + 1) & 0x100) {
            local_F4.x = 0.0f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, CHARA_PART_Set);
          } else {
            local_F4.x = 0.0f;
            (&local_F4)->y = -1200.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, CHARA_PART_Set);
          }
        }
      }
      if (Script_shenGetNode(local_EC, 8) & Script_shenGetNode(local_EC, 13)) {
        if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F0] * 13 + 1) & 0x20) {
          local_F4.x = 0.0f;
          (&local_F4)->y = 0.0f;
          (&local_F4)->z = 0.0f;
          Script_charaPosRotUtils(local_EC, 13, &local_F4, 0, CHARA_PART_Set);
          (&local_F4)->y = -1000.0f;
          Script_charaPosRotUtils(local_EC, 8, &local_F4, 0, CHARA_PART_Set);
        } else {
          local_F4.x = 0.0f;
          (&local_F4)->y = -1000.0f;
          (&local_F4)->z = 0.0f;
          Script_charaPosRotUtils(local_EC, 13, &local_F4, 0, CHARA_PART_Set);
          (&local_F4)->y = 0.0f;
          Script_charaPosRotUtils(local_EC, 8, &local_F4, 0, CHARA_PART_Set);
        }
      }
    }
    local_F0 = local_F0 + 1;
    if (local_F0 == 10) {
      break;
    }
  }
  return;
}

static void script_8BFE(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_8D06(arg0);
  script_89BC(arg0);
  return;
}

static void script_8C47(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_8D14(arg0);
  script_89BC(arg0);
  return;
}

static void script_8C90(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_8D06(int32_t arg0) {
  script_8C90(arg0, 0);
  return;
}

static void script_8D14(int32_t arg0) {
  script_8C90(arg0, 1);
  return;
}

static void script_8D22(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_8DE9(void) {
  script_8D22(1);
  return;
}

static void script_8DF5(void) {
  script_8D22(0);
  return;
}

static void script_8E01(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_8E44(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_180 == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_20[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_10)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
        script_9C14(1, local_FC, local_F4, local_F8);
        if (script_9DC8(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_9C14(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_9FDE(local_F4, local_F8);
    } else {
      task_180 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_8F55(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_160, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_160, &local_DC, 0x42000000);
  local_E8.x = (&local_F4)->x;
  (&local_E8)->y = (&local_F4)->y;
  (&local_E8)->z = (&local_F4)->z;
  if (DSWK_DoorUtils(DOOR_CMD_CALC_VECTOR_FROM_ANGLE, arg0, &local_E8, &local_D8)) {
    local_D8 = ((&local_DC)->y - local_D8) & 0xFFFF;
    if ((local_D8 <= 21845) | (local_D8 >= 43691)) {
      if (DSWK_DoorUtils(DOOR_CMD_TEST_LINE_INTERSECTION, &local_F4, &local_E8) == 0) {
        arg2 = 1;
      }
    }
  }
  return arg2;
}

__attribute__((scn_string_bindings(0, data_287A5, 1, data_287BE)))
static void script_8FED(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_138[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_138[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_8, SCND_ReadScriptOffset, Script_ScenDataUtil(task_C, SCND_ReadU32, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_138[arg0] = -1;
    }
  } else {
    task_138[arg0] = -1;
  }
  return;
}

static void script_91BC(int32_t arg0) {
  if (task_138[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_138[arg0], 1);
    task_138[arg0] = -1;
  }
  return;
}

static int32_t script_91ED(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_160 == arg0) {
    local_F4.x = (&task_194)->x;
    (&local_F4)->y = task_194.y;
    (&local_F4)->z = task_194.z;
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  }
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg1, &local_D0, 0);
  local_C4.x = (&local_D0)->x;
  (&local_C4)->y = (&local_D0)->y;
  (&local_C4)->z = (&local_D0)->z;
  local_C0 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
  local_C0 = ((local_C0 - (&local_D0)->y) >> 14) & 3;
  if (arg2 != 99) {
    if (arg2 == 6) {
      if ((local_C0 == 0) | (local_C0 == 1)) {
        if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_0, SCND_ReadU32, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_0, SCND_ReadU32, arg6 * 13 + 1) & 0x80)) {
          local_C0 = local_C0 ^ 3;
        }
      }
      arg2 = 4;
    }
    {
      int32_t unused_B8;
      int32_t local_B4;
      local_B4 = arg5;
      if ((local_B4 == 0) | (local_B4 == 1) | (local_B4 == 3) | (local_B4 == 5) | (local_B4 == 7) | (local_B4 == 9) | (local_B4 == 11) | (local_B4 == 25) | (local_B4 == 27) | (local_B4 == 30) | (local_B4 == 32) | (local_B4 == 34) | (local_B4 == 36) | (local_B4 == 37)) {
        local_B4 = local_C0;
        if ((local_B4 == 0) | (local_B4 == 3)) {
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
        } else {
          if ((local_B4 == 1) | (local_B4 == 2)) goto L0093D8;
        }
        L0093D8:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00947B;
          }
        }
        L00947B:;
      } else if ((local_B4 == 13) | (local_B4 == 14) | (local_B4 == 15) | (local_B4 == 16) | (local_B4 == 17) | (local_B4 == 18) | (local_B4 == 19) | (local_B4 == 20) | (local_B4 == 21) | (local_B4 == 22) | (local_B4 == 23) | (local_B4 == 29)) {
        local_B4 = local_C0;
        switch (local_B4) {
          case 0:
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
          case 1:
            arg3 = (float)0 - arg3;
            break;
          case 2:
            break;
          case 3:
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
        }
      } else if (local_B4 == 28) {
        local_B4 = local_C0;
        switch (local_B4) {
          case 0:
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
          case 1:
            arg3 = (float)0 - arg3;
            break;
          case 2:
            break;
          case 3:
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
        }
      } else {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L009627;
          }
        }
      }
    }
    L009627:;
    local_BC = (&local_D0)->y & 0xFFFF;
    local_DC[2] = arg4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_BC) - arg3 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_BC);
    local_DC[0] = arg4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_BC) + arg3 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_BC);
    if (arg2 == -1) {
      int32_t unused_B4[2];
      Vector3 local_A8;
      int32_t local_A4;
      local_A8.x = (&local_E8)->x + *local_DC;
      (&local_A8)->y = (&local_E8)->y;
      (&local_A8)->z = (&local_E8)->z + local_DC[2];
      local_A4 = DSWK_DoorUtils(DOOR_CMD_TEST_LINE_INTERSECTION, &local_F4, &local_A8);
      if ((local_C0 == 1) | (local_C0 == 2)) {
        local_C0 = 1;
      } else {
        local_C0 = 0;
      }
      if (local_A4) {
        local_C0 = -1;
      }
    } else {
      int32_t unused_B8;
      int32_t local_B4;
      local_F4.x = (&local_E8)->x + *local_DC;
      (&local_F4)->y = (&local_E8)->y;
      (&local_F4)->z = (&local_E8)->z + local_DC[2];
      local_B4 = arg2;
      switch (local_B4) {
        case 0:
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
          Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
          break;
        case 2:
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
          Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 3:
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 4:
          task_168.x = (&local_F4)->x;
          task_168.y = (&local_F4)->y;
          task_168.z = (&local_F4)->z;
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_168.x = (&local_F4)->x;
          task_168.y = (&local_F4)->y;
          task_168.z = (&local_F4)->z;
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_168, 0);
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_98B6(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if (task_160 == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, CCOW_EnableMap);
      } else {
        Script_ccowUtils(arg0, CCOW_DisableMap);
      }
    }
  }
  return;
}

static void script_98E8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_91ED(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_160 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_287D7)))
static void script_9A58(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_160 == arg0) {
    task_10.mode = 0;
    task_10.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_160, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_184 & 4) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_48[arg1];
        local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_48[arg1];
      local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_20[arg1] = 1;
  task_110[arg1] = 0;
  task_C0[arg1] = 0;
  task_70[arg1] = 0;
  if (task_138[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_138[arg1], 1);
    task_138[arg1] = -1;
  }
  return;
}

static void script_9C14(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8F55(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_8F55(arg2, arg3, local_FC);
          goto L009CD8;
        }
      }
      L009CD8:;
    } else {
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8F55(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_160, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L009D73;
          }
        }
      }
    }
  }
  L009D73:;
  if (task_110[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_190) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_190) & task_190) {
    Script_sicnUtils(2, 4, 0);
  }
  task_190 = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_287E6)))
static int32_t script_9DC8(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg1] * 13 + 1) & 4) == 0)) {
      if (task_110[arg1] == 0) {
        task_20[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_70[arg1] != task_160) & task_190) {
        if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg1] * 13 + 1) & 2) == 0) {
          task_20[arg1] = 3;
          task_70[arg1] = task_160;
          task_98[arg1] = 0;
          task_10.mode = 1;
          task_10.entry_idx = task_48[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_160, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_20[arg1] = 6;
              task_70[arg1] = task_160;
              task_98[arg1] = 0;
              task_10.mode = 0;
              task_10.entry_idx = task_48[arg1];
              local_FC = 0;
            }
            goto L009F7C;
          }
        }
        L009F7C:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_160, -1, &task_194, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_10.id = task_160;
          while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(PCCTL_RequestNewMode, 0);
        }
      }
    }
  }
  end:;
}

static void script_9FDE(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L009FE6:;
  local_F8 = task_20[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L00A006;
      L00A006:;
      script_A0A8(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L00A026;
      L00A026:;
      script_A109(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L00A044;
      L00A044:;
      script_A2AE(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L00A066;
      L00A066:;
      script_A346(local_FC, arg1);
      break;
    case 2:
      script_A428(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L009FE6;
  return;
}

static void script_A0A8(int32_t arg0, int32_t arg1) {
  if (task_110[arg0] == 0) {
    task_110[arg0] = Script_scenCreateTask(script_2243, 4, task_70[arg0], task_E8[arg0], arg0, arg1);
    task_C0[arg0] = task_70[arg0];
    script_8FED(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_287F5, 1, data_28807)))
static void script_A109(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_70[arg0])) {
    if (task_110[arg0] == 0) {
      if (task_70[arg0] == task_160) {
        task_10.mode = 1;
        task_10.entry_idx = task_48[arg0];
        goto L00A15E;
      }
      L00A15E:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
      script_98E8(task_70[arg0], task_E8[arg0], task_48[arg0], local_F8);
      task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_27BF4, SCND_ReadScriptOffset, local_F8), 4, task_70[arg0], task_E8[arg0], arg0, local_F8);
      task_C0[arg0] = task_70[arg0];
      if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_8FED(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_138[arg0] = -1;
      }
    }
  } else {
    if (task_20[arg0] != 5) {
      task_20[arg0] = 1;
      task_110[arg0] = 0;
      task_C0[arg0] = 0;
      task_70[arg0] = 0;
    }
  }
  return;
}

static void script_A2AE(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_110[arg0] == 0) & (arg2 >= 0)) {
    task_70[arg0] = task_160;
    task_10.mode = 2;
    task_10.entry_idx = task_48[arg0];
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_27BF4, SCND_ReadScriptOffset, arg2), 4, task_70[arg0], task_E8[arg0], arg0, arg2);
    task_C0[arg0] = task_70[arg0];
    script_8FED(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2881A, 1, data_2882C)))
static void script_A346(int32_t arg0, int32_t arg1) {
  if ((task_110[arg0] == 0) & (arg1 >= 0)) {
    script_98E8(task_70[arg0], task_E8[arg0], task_48[arg0], arg1);
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_27BF4, SCND_ReadScriptOffset, arg1), 4, task_70[arg0], task_E8[arg0], arg0, arg1);
    task_C0[arg0] = task_70[arg0];
    if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_8FED(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_138[arg0] = -1;
    }
  }
  return;
}

static void script_A428(int32_t arg0) {
  int32_t local_FC;
  task_110[arg0] = 0;
  task_C0[arg0] = 0;
  task_20[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 2);
  if (local_FC != -1) {
    task_70[arg0] = 0;
  }
  return;
}

static void script_A47F(void) {
  Script_ambSoundUtils(1, 1, &data_29054, 135.0f, 280.0f, 1.0f, 0, &data_290E4);
  Script_ambSoundUtils(2, 1, &data_29060, 135.0f, 60.0f, 1.0f, 0, &data_2916C);
  Script_ambSoundUtils(3, 0, &data_2906C, 50.0f, 170.0f, 0.8f, 0, data_291F4);
  Script_ambSoundUtils(4, 0, &data_29078, 10.0f, 5.0f, 1.0f, 0, &data_2927C);
  Script_ambSoundUtils(5, 0, data_29084, 8.0f, 5.0f, 1.0f, 0, &data_29304);
  Script_ambSoundUtils(6, 1, &data_29090, 170.0f, 100.0f, 1.0f, 2, data_2938C);
  Script_ambSoundUtils(7, 1, &data_2909C, 170.0f, 100.0f, 1.0f, 0, &data_29414);
  Script_ambSoundUtils(8, 1, &data_290A8, 40.0f, 70.0f, 0.8f, 0, &data_2949C);
  Script_ambSoundUtils(9, 0, &data_290B4, 3.0f, 60.0f, 0.8f, 0, data_29524);
  Script_ambSoundUtils(10, 0, &data_290C0, 60.0f, 35.0f, 0.7f, 0, &data_295AC);
  Script_ambSoundUtils(13, 0, &data_290CC, 8.0f, 5.0f, 1.0f, 0, &data_29634);
  Script_ambSoundUtils(15, 1, data_290D8, 300.0f, 0.0f, 1.0f, 3, &data_296BC);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_A639(void) {
  Script_ambSoundUtils(1, 1, &data_29744, 135.0f, 280.0f, 1.0f, 0, &data_297A4);
  Script_ambSoundUtils(2, 1, &data_29750, 135.0f, 60.0f, 1.0f, 0, &data_2982C);
  Script_ambSoundUtils(3, 0, &data_2975C, 50.0f, 170.0f, 0.8f, 0, data_298B4);
  Script_ambSoundUtils(6, 1, &data_29768, 170.0f, 100.0f, 1.0f, 2, data_2993C);
  Script_ambSoundUtils(7, 1, &data_29774, 170.0f, 100.0f, 1.0f, 0, &data_299C4);
  Script_ambSoundUtils(8, 1, &data_29780, 40.0f, 70.0f, 0.8f, 0, &data_29A4C);
  Script_ambSoundUtils(10, 0, &data_2978C, 60.0f, 35.0f, 0.7f, 0, &data_29AD4);
  Script_ambSoundUtils(15, 1, data_29798, 300.0f, 0.0f, 1.0f, 3, &data_29B5C);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_A763(void) {
  Script_ambSoundUtils(1, 0, &data_29BE4, 1.0f, 10.0f, 1.0f, 0, &data_29BF0);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_A794(void) {
  Script_ambSoundUtils(1, 0, &data_29C78, 1.0f, 10.0f, 1.0f, 0, &data_29C84);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_A7C5(void) {
  Script_ambSoundUtils(1, 1, &data_29D0C, 135.0f, 280.0f, 1.0f, 0, &data_29D6C);
  Script_ambSoundUtils(2, 1, &data_29D18, 135.0f, 60.0f, 1.0f, 0, &data_29DF4);
  Script_ambSoundUtils(3, 0, &data_29D24, 50.0f, 170.0f, 0.8f, 0, data_29E7C);
  Script_ambSoundUtils(6, 1, &data_29D30, 170.0f, 100.0f, 1.0f, 2, data_29F04);
  Script_ambSoundUtils(7, 1, &data_29D3C, 170.0f, 100.0f, 1.0f, 0, &data_29F8C);
  Script_ambSoundUtils(8, 1, &data_29D48, 40.0f, 70.0f, 0.8f, 0, &data_2A014);
  Script_ambSoundUtils(15, 1, data_29D60, 300.0f, 0.0f, 1.0f, 3, &data_2A09C);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_A8CB(void) {
  script_A47F();
  return;
}

static float script_A8D3(int32_t arg0, float arg1, float arg2, float arg3, float arg4) {
  float local_FC;
  float local_F8;
  float local_F4;
  float local_F0;
  Vector3 local_E4;
  Script_EVT_GetPosition(arg0, -1, &local_E4, ENTITY_XFORM_WORLD_SPACE);
  local_F4 = arg3 - arg1;
  local_FC = (float)0 - (arg4 - arg2);
  local_F8 = (&local_E4)->x - arg1;
  local_F0 = (&local_E4)->z - arg2;
  return local_FC * local_F8 + local_F4 * local_F0;
}

static int32_t script_A91E(int32_t arg0, const Vector3 *arg1, const Vector3 *arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  local_FC = 0;
  if (!((float)((scn3_bits){.f = script_A8D3(arg0, arg1->x, arg1->z, arg2->x, arg2->z)}).i < 0.0f)) {
    if (!((float)((scn3_bits){.f = script_A8D3(arg0, arg2->x, arg2->z, *(float *)arg3, *(float *)(arg3 + 8))}).i < 0.0f)) {
      if (!((float)((scn3_bits){.f = script_A8D3(arg0, *(float *)arg3, *(float *)(arg3 + 8), *(float *)arg4, *(float *)(arg4 + 8))}).i < 0.0f)) {
        if (!((float)((scn3_bits){.f = script_A8D3(arg0, *(float *)arg4, *(float *)(arg4 + 8), arg1->x, arg1->z)}).i < 0.0f)) {
          local_FC = 1;
        }
      }
    }
  }
  return local_FC;
}

static void script_A9C0(int32_t arg0) {
  Script_seqEnterEvtCam(1000, arg0, arg0);
  return;
}

static void script_A9CE(int32_t arg0, float arg1, float arg2, int32_t arg3) {
  {
    Vector3 local_F4;
    local_F4.x = arg1;
    (&local_F4)->z = arg2;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = arg3;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  return;
}

static void script_AA05(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  (&local_E8)->y = arg4;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
  Script_SetCharaFlags(arg0, 9);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  return;
}

static void script_AA6F(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t local_DC[3];
  int32_t local_D0[3];
  int8_t local_CF;
  int8_t local_CE;
  int32_t local_C8;
  int32_t local_C4;
  local_C4 = task_2C1;
  switch (local_C4) {
    case 1:
      local_CF = Script_flmwHandleInteractionState(arg1);
      if (local_CF) {
        (void)((task_2C8 = -1) & 0x10);
        Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
        Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
        local_C4 = local_CF;
        switch (local_C4) {
          case 1:
            script_C87A();
            break;
          case 2:
            if (task_2C4 == 2) {
              local_C8 = Script_ForkliftUtils(arg1, FORK_CMD_GET_ATTACHED_LOAD_ID);
              local_CE = 0;
              if ((local_C8 >> 24) < 66) {
                local_F4.x = -104.49f;
                (&local_F4)->z = 63.75f;
                local_E8.x = -119.29f;
                (&local_E8)->z = 63.75f;
                local_DC[0] = -0x3D116B85;
                local_DC[2] = 0x42550000;
                local_D0[0] = -0x3D2F051F;
                local_D0[2] = 0x42550000;
                local_CE = script_A91E(arg1, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
              } else {
                local_F4.x = -0.79f;
                (&local_F4)->z = 143.52f;
                local_E8.x = 15.51f;
                (&local_E8)->z = 127.02f;
                local_DC[0] = 0x41EA7AE1;
                local_DC[2] = 0x430CD1EC;
                local_D0[0] = 0x4151C28F;
                local_D0[2] = 0x431D547B;
                local_CE = script_A91E(arg1, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
              }
              if (local_CE) {
                script_C91E();
              } else {
                script_C846();
              }
            } else {
              script_C846();
            }
            break;
          case 3:
            script_C8AE();
            break;
          case 4:
            script_C906();
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
          Script_flmwHandleInteractionState(arg1);
        }
        Script_ForkliftUtils(FLMW_MODE_SET_GLOBAL_FLAG);
        Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
        Script_convSetCamInactiveFlag(1);
        Script_SetGameStateTo(AUTH_None);
        (void)((task_2C8 = 0) & 0x10);
      }
      break;
    case 2:
      break;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2A190)))
static void script_AC68(void) {
  Vector3 local_F4;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_2ND") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_mapmSetCamFlags(1, 16, 0);
  Script_mapmSetCamFlags(2, 16, 0);
  Script_EVT_GetPosition(SHEN_NAME('SHT3'), -1, &local_F4, 0);
  (&local_F4)->y = 5.0f;
  Script_EVT_SetPosition(SHEN_NAME('SHT3'), POS_XYZ, &local_F4);
  return;
}

__attribute__((scn_string_bindings(0, data_2A1A6, 1, data_2A1B5)))
static void script_ACE9(void) {
  Vector3 local_F4;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_1ST") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_mapmSetCamFlags(1, 16, 0);
  Script_mapmSetCamFlags(2, 16, 0);
  Script_EVT_GetPosition(SHEN_NAME('SH18'), -1, &local_F4, 0);
  (&local_F4)->y = 5.0f;
  Script_EVT_SetPosition(SHEN_NAME('SH18'), POS_XYZ, &local_F4);
  return;
}

static int32_t script_AD6A(void) {
  Script_executeAicaCommand(0x1504A9, 0, 0);
  SCNF_WaitFrames_0000(90);
  Script_executeAicaCommand(0x1204A9, 0, 0);
  Script_executeAicaCommand(0x1404A9, 0, 0);
  end:;
}

static void script_AD9C(void) {
  if (task_2C1 == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
  } else {
    Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
    Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
  }
  Script_GetConversationDetails(CONV_DETAILS_MODE_CLOSE_CONVERSATION_UI, 0);
  Script_GetConversationDetails(CONV_DETAILS_MODE_SET_SHARED_ITEM_PREVIEW, -1);
  Script_scenCreateTask(script_AD6A, 0);
  SCNF_WaitFrames_0000(60);
  return;
}

static int32_t script_ADEA(int32_t arg0, int8_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = 0;
  local_F8 = -1;
  while (local_FC < 20) {
    if (arg1 == 0) {
      local_F4 = task_20C[local_FC][0];
    } else {
      local_F4 = task_20C[local_FC][1];
    }
    if (local_F4 == arg0) {
      local_F8 = local_FC;
      break;
    }
    local_FC = local_FC + 1;
  }
  return local_F8;
}

__attribute__((scn_string_bindings(0, data_2A1BC)))
static int32_t script_AE4E(int32_t arg0) {
  int32_t local_FC;
  {
    int32_t local_F8;
    local_F8 = 0;
    local_FC = 0;
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
    if (local_FC == 0) {
      Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      while (local_FC == 0) {
        if (local_F8 == 0) {
          Script_sprintf("MA00: Syuki Chara Read Failed:ID=%c%c%c%c\012", arg0, arg0 >> 8, arg0 >> 16, arg0 >> 24);
          local_F8 = 1;
        }
        SCNF_WaitFrames_0000(1);
        local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      }
      L00AEE0:;
      SCNF_WaitFrames_0000(1);
      if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L00AEE0;
    }
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  {
    int32_t local_F8;
    local_F8 = script_ADEA(0, 0);
    if (local_F8 >= 0) {
      task_20C[local_F8][0] = arg0;
      task_20C[local_F8][1] = local_FC;
    }
  }
  return local_FC;
}

static void script_AF37(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = 0;
  local_FC = script_ADEA(arg0, 0);
  if ((local_FC >= 0) & (arg0 != 0)) {
    local_F8 = task_20C[local_FC][1];
    if (local_F8 != 0) {
      Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_F8);
    }
    task_20C[local_FC][0] = 0;
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  }
  local_F8 = 0;
  return;
}

static void script_AFA0(int32_t arg0) {
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  Script_sprintf("Arbeit Character Lock:%c%c%c%c", arg0, arg0 >> 8, arg0 >> 16, arg0 >> 24);
  Script_sprintf("\012");
  return;
}

static void script_AFD1(void) {
  int32_t local_FC;
  local_FC = 0;
  while (local_FC < 20) {
    script_AF37(task_20C[local_FC][0], 0);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_B002(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_SetPosition(arg1, 0x78000000, &local_F4);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0x40000000);
  Script_EVT_SetRotation(arg1, 0x78000000, &local_E8, 0);
  Script_MoveObjectToObject(arg1, arg0);
  Script_scenToggleVisibleFor(arg1, 0);
  return;
}

static void script_B04E(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (local_FC < 5) {
    local_F8 = Script_ScenDataUtil(&data_2A17C, SCND_ReadU32, local_FC);
    if (Script_GetOwnedItemCount(local_F8, 0)) {
      Script_GetCassetteFlags_0(local_F8, 0, 0);
    }
    local_FC = local_FC + 1;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2A214, 2, data_2A22A, 4, data_2A242)))
static int32_t script_B094(void) {
  int32_t local_FC;
  local_FC = Script_MT5Loader_TRWV_TRWAVE(0, &data_2A208, "model/object", "SEA3.PVR", "model/object", "TRSEA4.MT6", 4.0f, 0);
  Script_MT5Loader_TRWV_TRWAVE(21, local_FC, 8, 8);
  Script_MT5Loader_TRWV_TRWAVE(16, local_FC, -1, 0);
  Script_MT5Loader_TRWV_TRWAVE(13, local_FC, 0.0901960805f, 0.196078435f);
  Script_MT5Loader_TRWV_TRWAVE(15, local_FC, "model/object", "TRWTBL1.BIN");
  Script_MT5Loader_TRWV_TRWAVE(12, local_FC, 0x4000);
  Script_MT5Loader_TRWV_TRWAVE(10, local_FC, 64.0f, 64.0f, 1, 1, 3, 1);
  Script_MT5Loader_TRWV_TRWAVE(24, local_FC, 0, 203, 255, 143);
  Script_MT5Loader_TRWV_TRWAVE(11, local_FC, -0.51f, -0.68f, 0.52f);
  return local_FC;
}

static void script_B159(int32_t arg0) {
  Script_MT5Loader_TRWV_TRWAVE(3, arg0);
  return;
}

static void script_B165(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_PlayCharaMotion(arg0, 61442, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4.x = -0.0113f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.477f;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  (&local_E8)->y = 0;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
  Script_MoveObjectToObject(arg0, -1);
  Script_flmwTriggerInteraction(arg1, arg0);
  return;
}

static void script_B1ED(int32_t arg0, int32_t arg1) {
  task_2C1 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_SetupClothData(arg0, 5);
  script_B165(arg0, arg1);
  Script_ForkliftUtils(arg1, FORK_CMD_ENTER_FORKLIFT_CAMERA_MODE);
  Script_uiUtils(0, 23, 0);
  return;
}

static int32_t script_B225(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t local_DC[3];
  int32_t local_D0[3];
  int32_t local_CC;
  int32_t local_C8;
  local_C8 = task_2C4;
  switch (local_C8) {
    case 1:
      local_F4.x = 19.47f;
      (&local_F4)->z = 94.04f;
      local_E8.x = 24.45f;
      (&local_E8)->z = 99.02f;
      local_DC[0] = 0x41B0147B;
      local_DC[2] = 0x42CAEB85;
      local_D0[0] = 0x41883D71;
      local_D0[2] = 0x42C0F0A4;
      local_CC = script_A91E(arg0, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
      local_F4.x = 19.47f;
      (&local_F4)->z = 94.04f;
      local_E8.x = 21.9f;
      (&local_E8)->z = 96.47f;
      local_DC[0] = 0x4191999A;
      local_DC[2] = 0x42C86148;
      local_D0[0] = 0x417C51EC;
      local_D0[2] = 0x42C375C3;
      local_CC = local_CC | script_A91E(arg0, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
      break;
    case 0:
      local_F4.x = -118.51f;
      (&local_F4)->z = 63.04f;
      local_E8.x = -118.51f;
      (&local_E8)->z = 54.18f;
      local_DC[0] = -0x3D19E666;
      local_DC[2] = 0x4258B852;
      local_D0[0] = -0x3D19E666;
      local_D0[2] = 0x427C28F6;
      local_CC = script_A91E(arg0, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
      local_F4.x = -118.51f;
      (&local_F4)->z = 63.04f;
      local_E8.x = -118.51f;
      (&local_E8)->z = 61.39f;
      local_DC[0] = -0x3D21147B;
      local_DC[2] = 0x42758F5C;
      local_D0[0] = -0x3D21147B;
      local_D0[2] = 0x427C28F6;
      local_CC = local_CC | script_A91E(arg0, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
      break;
    case 2:
      if ((arg0 >> 24) < 66) {
        local_F4.x = 20.45f;
        (&local_F4)->z = 148.74f;
        local_E8.x = 15.39f;
        (&local_E8)->z = 153.88f;
        local_DC[0] = 0x414CA3D7;
        local_DC[2] = 0x431747AE;
        local_D0[0] = 0x418F1EB8;
        local_D0[2] = 0x431247AE;
        local_CC = script_A91E(arg0, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
      } else {
        local_F4.x = -118.51f;
        (&local_F4)->z = 63.04f;
        local_E8.x = -118.51f;
        (&local_E8)->z = 54.18f;
        local_DC[0] = -0x3D19E666;
        local_DC[2] = 0x4258B852;
        local_D0[0] = -0x3D19E666;
        local_D0[2] = 0x427C28F6;
        local_CC = script_A91E(arg0, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
        local_F4.x = -118.51f;
        (&local_F4)->z = 63.04f;
        local_E8.x = -118.51f;
        (&local_E8)->z = 61.39f;
        local_DC[0] = -0x3D21147B;
        local_DC[2] = 0x42758F5C;
        local_D0[0] = -0x3D21147B;
        local_D0[2] = 0x427C28F6;
        local_CC = local_CC | script_A91E(arg0, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
      }
      break;
  }
  return local_CC;
}

static int32_t script_B554(void) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  float local_D0[3];
  Vector3i local_C4;
  Script_mtwkGetSetRotation(SHEN_NAME('FORK'), &local_C4, 0x40000000);
  if (((&local_C4)->y < 0x2000) | ((&local_C4)->y > 0xA000)) {
    (&local_C4)->y = 0xE000;
    Script_executeAicaCommand(853161, 0, 0);
    local_F4 = -0.08f;
  } else {
    (&local_C4)->y = 0x6000;
    local_F4 = 0.12f;
  }
  Script_EVT_SetRotation(SHEN_NAME('FORK'), 0x78000000, &local_C4, 0);
  Script_EVT_GetPosition(SHEN_NAME('FORK'), -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  (&local_E8)->y = (&local_DC)->y;
  local_D0[0] = 34.93f - (&local_DC)->x;
  local_D0[2] = 87.64f - (&local_DC)->z;
  local_FC = script_C58E(local_F4);
  local_F8 = 0.0f;
  for (;;) {
    if (local_F8 >= 30.0f) {
      break;
    }
    local_E8.x = (&local_DC)->x + *local_D0 * local_F8 / 30.0f;
    (&local_E8)->z = (&local_DC)->z + local_D0[2] * local_F8 / 30.0f;
    Script_EVT_SetPosition(SHEN_NAME('FORK'), 0x78000000, &local_E8);
    local_F8 = local_F8 + 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 34.93f;
  (&local_E8)->z = 87.64f;
  Script_EVT_SetPosition(SHEN_NAME('FORK'), 0x78000000, &local_E8);
  script_C5E3(local_FC);
  Script_executeAicaCommand(918697, 0, 0);
  end:;
}

static void script_B6BE(int32_t arg0, const Vector3 *arg1, const Vector3 *arg2, int32_t arg3, float *arg4, const Vector3i *arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int32_t local_CC;
  float local_C8;
  Vector4 local_B8;
  int32_t local_B4;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_GetPosition(arg0, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  if (Script_CheckIsInsideCullVolume(&local_E8, 0.5f) & -!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z) >= 100.0f)) {
    Script_scenToggleVisibleFor(arg0, 1);
  } else {
    Script_scenToggleVisibleFor(arg0, 0);
  }
  local_D8 = 0x52494B41;
  if (!((float)((scn3_bits){.f = script_A8D3(local_D8, arg1->x, arg1->z, arg2->x, arg2->z)}).i < 0.0f)) {
    if (!((float)((scn3_bits){.f = script_A8D3(local_D8, arg2->x, arg2->z, *(float *)arg3, *(float *)(arg3 + 8))}).i < 0.0f)) {
      if (!((float)((scn3_bits){.f = script_A8D3(local_D8, *(float *)arg3, *(float *)(arg3 + 8), arg1->x, arg1->z)}).i < 0.0f)) {
        (void)((task_2C8 = -1) & 0x40);
        local_B8.x = 0.4f;
        (&local_B8)->y = 0.4f;
        (&local_B8)->z = 0.4f;
        (&local_B8)->w = 0.4f;
        local_CC = Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_ATTACHED_LOAD_ID);
        Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
        Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
        Script_scenToggleVisibleFor(arg0, 1);
        Script_SetCPAMFlags(arg0, 0);
        SCNF_WaitFrames_0000(1);
        Script_ForkliftUtils(0x4B524F46, FORK_CMD_SET_INTERACTION_STATE, 0);
        script_A9C0(arg0);
        Script_charaSetRefboardFlagTo(arg0, 1);
        Script_SetCharaRefBoardPos(arg0, &local_B8);
        local_D0 = Script_PlayerStatsUtils(PS_GetCharaItemCount, arg0);
        Script_PlayerStatsUtils(PS_SetCharaItemCount, arg0, local_D0 + 1);
        local_B4 = arg0;
        switch (local_B4) {
          case 0x4242454B:
            if (arg6 == 0) {
              script_CB3A();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            } else {
              script_CB46();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            }
            break;
          case 0x4742454B:
            if (arg6 == 0) {
              script_CB52();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            } else {
              script_CB5E();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            }
            break;
          case 0x4942454B:
            Script_scenCreateTask(script_B554, 0);
            if (local_CC) {
              script_CB6A();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            } else {
              script_CB76();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            }
            break;
          case 0x4A42454B:
            if (local_CC) {
              script_CB82();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            } else {
              script_CB8E();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            }
            break;
          case 0x4B42454B:
            if (arg6 == 0) {
              script_CB9A();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            } else {
              script_CBA6();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            }
            break;
          case 0x4C42454B:
            if (local_CC) {
              script_CBB2();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            } else {
              script_CBBE();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            }
            break;
          case 0x594A4E53:
            if (local_CC) {
              script_CBCA();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            } else {
              script_CBD6();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            }
            break;
        }
        Script_SetGameStateTo(AUTH_None);
        Script_charaSetRefboardFlagTo(arg0, 0);
        if (arg0 != 0x4942454B) {
          Script_mtwkGetSetRotation(SHEN_NAME('FORK'), &local_DC, 0);
          if (((&local_DC)->y > arg5->x) & ((&local_DC)->y < arg5->y)) {
            local_C8 = -0.08f;
            Script_executeAicaCommand(853161, 0, 0);
          } else {
            local_C8 = 0.12f;
          }
          arg4[1] = 0.0f;
          local_D4 = script_C58E(local_C8);
          if (Script_forkAttachLoadActor(SHEN_NAME('FORK'), arg4, local_C8, 0, 1)) {
            while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FORK'))) {
              SCNF_WaitFrames_0000(1);
            }
          }
          script_C5E3(local_D4);
        }
        Script_SetCPAMFlags(arg0, 1);
        Script_executeAicaCommand(918697, 0, 0);
        Script_ForkliftUtils(0x4B524F46, FORK_CMD_SET_INTERACTION_STATE, 1);
        Script_ForkliftUtils(FLMW_MODE_SET_GLOBAL_FLAG);
        Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
        Script_convSetCamInactiveFlag(1);
        Script_SetGameStateTo(AUTH_None);
        (void)((task_2C8 = 0) & 1);
      }
    }
  }
  return;
}

static int32_t script_BB87(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t local_DC[3];
  int32_t local_D0[3];
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  int8_t local_BB;
  int8_t local_BA;
  int8_t local_B9;
  int8_t local_B8;
  int8_t local_B7;
  int32_t local_B0;
  script_A9CE(CHARA_KEBB, -130.4f, -12.0f, 0xC000);
  script_A9CE(CHARA_KEBL, -124.2f, 14.6f, 0xA000);
  script_A9CE(CHARA_SNJY, -124.4f, 71.0f, 0xE000);
  script_A9CE(CHARA_KEBK, -119.2f, 115.4f, 0xE000);
  script_A9CE(CHARA_KEBI, 41.38f, 85.92f, 19114);
  if (task_2C3 != 6) {
    script_A9CE(CHARA_KEBG, 12.7f, 136.3f, 0x4000);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KEBG, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_KEBG, CDM_Show);
        goto L00BC5C;
      }
    }
    L00BC5C:;
    Script_scenToggleVisibleFor(CHARA_KEBG, 0);
  }
  script_A9CE(CHARA_KEBJ, 142.5f, 19.9f, 0x6000);
  local_BA = 0;
  for (;;) {
    (&local_C4)->z = 0;
    if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
      local_B9 = 0;
    } else {
      local_B9 = 1;
    }
    local_F4.x = -125.49f;
    (&local_F4)->z = -15.33f;
    local_E8.x = -125.49f;
    (&local_E8)->z = -8.23f;
    local_DC[0] = -0x3CFD828F;
    local_DC[2] = -0x3EC451EC;
    local_D0[0] = -0x3D0CCCCD;
    local_D0[2] = -0x3EC63D71;
    local_C4.x = 0;
    (&local_C4)->y = 0x8000;
    script_B6BE(CHARA_KEBB, &local_F4, &local_E8, (int32_t)local_DC, (float *)local_D0, &local_C4, local_B9);
    local_F4.x = -120.34f;
    (&local_F4)->z = 14.5f;
    local_E8.x = -120.34f;
    (&local_E8)->z = 21.9f;
    local_DC[0] = -0x3D031EB8;
    local_DC[2] = 0x418F3333;
    local_D0[0] = -0x3D180A3D;
    local_D0[2] = 0x4193999A;
    local_C4.x = 0;
    (&local_C4)->y = 0x8000;
    script_B6BE(CHARA_KEBL, &local_F4, &local_E8, (int32_t)local_DC, (float *)local_D0, &local_C4, local_B9);
    local_F4.x = -120.06f;
    (&local_F4)->z = 64.94f;
    local_E8.x = -120.06f;
    (&local_E8)->z = 71.84f;
    local_DC[0] = -0x3D0347AE;
    local_DC[2] = 0x428947AE;
    local_D0[0] = -0x3D17999A;
    local_D0[2] = 0x4289F0A4;
    local_C4.x = 0;
    (&local_C4)->y = 0x8000;
    script_B6BE(CHARA_SNJY, &local_F4, &local_E8, (int32_t)local_DC, (float *)local_D0, &local_C4, local_B9);
    local_F4.x = -115.18f;
    (&local_F4)->z = 110.7f;
    local_E8.x = -109.78f;
    (&local_E8)->z = 110.7f;
    local_DC[0] = -0x3D18DC29;
    local_DC[2] = 0x42EEDC29;
    local_D0[0] = -0x3D1A7AE1;
    local_D0[2] = 0x42D5428F;
    local_C4.x = 0x4000;
    (&local_C4)->y = 54613;
    script_B6BE(CHARA_KEBK, &local_F4, &local_E8, (int32_t)local_DC, (float *)local_D0, &local_C4, local_B9);
    local_F4.x = -121.18f;
    (&local_F4)->z = 110.7f;
    local_E8.x = -114.28f;
    (&local_E8)->z = 110.7f;
    local_DC[0] = -0x3D1CA3D7;
    local_DC[2] = 0x42EF428F;
    local_C4.x = 10922;
    (&local_C4)->y = 0xC000;
    script_B6BE(CHARA_KEBK, &local_F4, &local_E8, (int32_t)local_DC, (float *)local_D0, &local_C4, local_B9);
    local_F4.x = 38.9f;
    (&local_F4)->z = 89.3f;
    local_E8.x = 33.7f;
    (&local_E8)->z = 84.4f;
    local_DC[0] = 0x42233333;
    local_DC[2] = 0x42A73333;
    script_B6BE(CHARA_KEBI, &local_F4, &local_E8, (int32_t)local_DC, (float *)local_D0, &local_C4, local_B9);
    (&local_C4)->z = 0;
    if (task_2C3 != 6) {
      local_F4.x = 4.16f;
      (&local_F4)->z = 138.53f;
      local_E8.x = 10.46f;
      (&local_E8)->z = 132.13f;
      local_DC[0] = 0x413428F6;
      local_DC[2] = 0x430C547B;
      local_D0[0] = 0x40AF5C29;
      local_D0[2] = 0x43050CCD;
      local_C4.x = 0x6000;
      (&local_C4)->y = 0xE000;
      script_B6BE(CHARA_KEBG, &local_F4, &local_E8, (int32_t)local_DC, (float *)local_D0, &local_C4, local_B9);
    }
    local_F4.x = 139.01f;
    (&local_F4)->z = 26.73f;
    local_E8.x = 138.81f;
    (&local_E8)->z = 19.53f;
    local_DC[0] = 0x43104F5C;
    local_DC[2] = 0x41B6A3D7;
    local_D0[0] = 0x43060A3D;
    local_D0[2] = 0x41BE51EC;
    local_C4.x = 0x8000;
    (&local_C4)->y = 65517;
    script_B6BE(CHARA_KEBJ, &local_F4, &local_E8, (int32_t)local_DC, (float *)local_D0, &local_C4, local_B9);
    local_BB = 0;
    local_C0 = 0x52494B41;
    local_B0 = task_2C4;
    if (local_B0 == 1) {
      local_F4.x = 19.1f;
      (&local_F4)->z = 108.21f;
      local_E8.x = 8.8f;
      (&local_E8)->z = 97.51f;
      local_DC[0] = 0x4180CCCD;
      local_DC[2] = 0x42B470A4;
      local_D0[0] = 0x41D33333;
      local_D0[2] = 0x42C90A3D;
      local_BB = script_A91E(local_C0, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
    } else {
      if ((local_B0 == 0) | (local_B0 == 2)) {
        local_F4.x = -104.49f;
        (&local_F4)->z = 63.75f;
        local_E8.x = -119.29f;
        (&local_E8)->z = 63.75f;
        local_DC[0] = -0x3D116B85;
        local_DC[2] = 0x42550000;
        local_D0[0] = -0x3D2F051F;
        local_D0[2] = 0x42550000;
        local_BB = script_A91E(local_C0, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
        goto L00C18A;
      }
    }
    L00C18A:;
    if (local_BB == 0) {
      local_F4.x = -0.79f;
      (&local_F4)->z = 143.52f;
      local_E8.x = 15.51f;
      (&local_E8)->z = 127.02f;
      local_DC[0] = 0x41EA7AE1;
      local_DC[2] = 0x430CD1EC;
      local_D0[0] = 0x4151C28F;
      local_D0[2] = 0x431D547B;
      local_BB = script_A91E(local_C0, &local_F4, &local_E8, (int32_t)local_DC, (int32_t)local_D0);
      if (local_BB) {
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 20, 0);
      } else {
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 20, 1);
      }
    }
    if (local_BB != local_BA) {
      local_BA = local_BB;
      if (local_BB) {
        if (Script_ForkliftUtils(0x4B524F46, FORK_CMD_ENGINE_BANK_ENABLE, -1) != 1) {
          Script_ForkliftUtils(0x4B524F46, FORK_CMD_ENGINE_BANK_ENABLE, 1);
        }
      } else {
        Script_ForkliftUtils(0x4B524F46, FORK_CMD_ENGINE_BANK_ENABLE, 0);
      }
    }
    local_B8 = 0;
    local_BC = Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_ATTACHED_LOAD_ID);
    if (local_BC != 0) {
      local_B8 = script_B225(local_BC);
      if (local_B8 != 0) {
        local_BA = 2;
        Script_ForkliftUtils(0x4B524F46, FORK_CMD_ENGINE_BANK_ENABLE, 2);
      }
    }
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_F4)->x <= 32.0f)) {
      Script_mapmSetRenderFlag(14, 1);
      Script_mapmSetRenderFlag(6, 0);
      Script_mapmSetRenderFlag(11, 0);
      Script_mapmSetRenderFlag(12, 0);
      if (!((&local_F4)->z >= 90.0f)) {
        Script_mapmSetRenderFlag(16, 0);
      } else {
        Script_mapmSetRenderFlag(16, 1);
      }
    } else {
      Script_mapmSetRenderFlag(14, 0);
      Script_mapmSetRenderFlag(6, 1);
      Script_mapmSetRenderFlag(11, 1);
      Script_mapmSetRenderFlag(12, 1);
      Script_mapmSetRenderFlag(16, 1);
    }
    local_B7 = 0;
    while (local_B7 < 20) {
      local_BC = Script_ScenDataUtil(&data_2A124, SCND_ReadU32, local_B7);
      Script_EVT_GetPosition(local_BC, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      if (!((&local_F4)->y <= -1.0f)) {
        Script_mtwkGetSetRotation(local_BC, &local_C4, 0x40000000);
        Script_CycleManagerUtils(CYCLE_CMD_QUEUE_INTERSECTION_ENTRY, ((scn3_bits){.f = (&local_F4)->x}).i, ((scn3_bits){.f = (&local_F4)->y}).i, ((scn3_bits){.f = (&local_F4)->z}).i, (&local_C4)->y, 0x3F428F5C, 0x3F428F5C, 0);
      }
      local_B7 = local_B7 + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_C3D7(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int8_t local_EB;
  local_FC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 22) * 10;
  local_F8 = Script_ScenDataUtil(&data_2A174, SCND_ReadU8, task_2C3 - 2);
  local_F4 = local_FC * task_2C6;
  local_EC = local_F4;
  {
    int8_t local_EA;
    if (local_F4 % 100) {
      local_EA = 128;
    } else {
      local_EA = 0;
    }
    local_EA = local_F4 / 100 + local_EA;
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, local_EA);
  }
  local_EB = task_2C6 * 10 / local_F8;
  local_F4 = 53;
  if (task_2C6 >= local_F8) {
    local_F4 = 52;
    if (local_FC < 600) {
      local_FC = local_FC + 50;
      local_F4 = 51;
    }
  } else {
    if (task_2C6 == 0) {
      local_F4 = 54;
      goto L00C491;
    }
  }
  L00C491:;
  local_FC = local_FC / 10;
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 22, local_FC);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 23, local_F4);
  {
    int8_t local_EA;
    local_EA = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 3);
    if (local_EB >= 8) {
      local_EA = local_EA + 1;
      if (local_F4 <= 2) {
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 3, local_EA);
      }
    }
  }
  local_F0 = Script_PlayerStatsUtils(PS_GetStatU16_0);
  local_F0 = local_F0 + task_2C6;
  if (local_F0 > 0x7D00) {
    local_F0 = 0x7D00;
  }
  Script_PlayerStatsUtils(PS_SetStatU16_0, local_F0);
  local_F0 = Script_PlayerStatsUtils(PS_GetStatU32_0);
  local_F0 = local_F0 + local_EC;
  if (local_F0 > 0x7F8DCF00) {
    local_F0 = 0x7F8DCF00;
  }
  Script_PlayerStatsUtils(PS_SetStatU32_0, local_F0);
  return;
}

static void script_C544(void) {
  SCNF_WaitFrames_0000(1);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_2A25C, 28);
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 27, 0);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet1FromTaskOffset, &data_2A87C, 4);
  return;
}

static void script_C576(int32_t arg0, int32_t arg1) {
  Script_FaceEx(arg0, arg1, 0, 4);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return;
}

static int8_t script_C58E(float arg0) {
  int8_t local_FF;
  int32_t local_F8;
  float local_F4;
  if (!(arg0 < 0.0f)) {
    local_F8 = 1026;
  } else {
    local_F8 = 1025;
  }
  local_FF = Script_ExecuteAicaCommand(1, local_F8);
  if (local_FF >= 0) {
    local_F4 = arg0 * 423.333313f;
    Script_executeAicaCommand(166, local_FF, Script_FloatUtils(ScriptMathOp_FloatToInt, local_F4));
  }
  return local_FF;
}

static void script_C5E3(int32_t arg0) {
  if (arg0 >= 0) {
    Script_ExecuteAicaCommand(0, arg0);
  }
  return;
}

static void script_C5F8(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int8_t local_BF;
  int8_t local_BE;
  int8_t local_BD;
  local_BF = 0;
  while (local_BF < 20) {
    local_C0 = Script_ScenDataUtil(&data_2A124, SCND_ReadU32, local_BF);
    Script_EVT_GetPosition(local_C0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    *(int32_t *)&local_DC.x = Script_ForkliftUtils(FLMW_MODE_RETURN_ARG, ((scn3_bits){.f = (&local_F4)->x}).i);
    (&local_DC)->y = (&local_F4)->y;
    *(int32_t *)&(&local_DC)->z = Script_ForkliftUtils(FLMW_MODE_RETURN_ARG, ((scn3_bits){.f = (&local_F4)->z}).i);
    Script_EVT_SetPosition(local_C0, 0x78000000, &local_DC);
    if (!((&local_F4)->y <= 1.49f)) {
      if (Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_ATTACHED_LOAD_ID) != local_C0) {
        if (script_B225(local_C0) == 0) {
          Script_mtwkGetSetRotation(local_C0, &local_D0, 0x40000000);
          local_BE = 0;
          local_BD = 0;
          while (local_BE < 20) {
            if (local_BF != local_BE) {
              Script_EVT_GetPosition(Script_ScenDataUtil(&data_2A124, SCND_ReadU32, local_BE), -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
              if (!((&local_E8)->y >= 0.011f)) {
                Script_mtwkGetSetRotation(Script_ScenDataUtil(&data_2A124, SCND_ReadU32, local_BE), &local_C4, 0x40000000);
                local_E8.x = Script_MathUtils(SCRIPT_MATH_ABS, (&local_E8)->x - (&local_DC)->x);
                (&local_E8)->z = Script_MathUtils(SCRIPT_MATH_ABS, (&local_E8)->z - (&local_DC)->z);
                if (-!((&local_E8)->x >= 0.05f) & -!((&local_E8)->z >= 0.05f) & -((&local_D0)->y == (&local_C4)->y)) {
                  local_BD = 1;
                }
              }
            }
            local_BE = local_BE + 1;
          }
          if (local_BD == 0) {
            (&local_DC)->y = 0.001f;
            Script_EVT_SetPosition(local_C0, 0x78000000, &local_DC);
          }
        }
      }
    }
    if (!((&local_F4)->y <= 1.52f)) {
      Script_EVT_SetPosition(local_C0, 0x78000000, &local_F4);
    }
    local_BF = local_BF + 1;
  }
  return;
}

static int32_t script_C7C2(void) {
  return Script_ShowUIMessageFromID("E1001DC025");
}

static int32_t script_C7CE(void) {
  return Script_ShowUIMessageFromID("E1001DC026");
}

static int32_t script_C7DA(void) {
  return Script_ShowUIMessageFromID("E1001DC027");
}

static void script_C7E6(void) {
  Script_ShowUIMessageFromID("E1001DC028");
  return;
}

static void script_C7F2(void) {
  Script_ShowUIMessageFromID("E1001DC029");
  return;
}

static void script_C7FE(void) {
  Script_ShowUIMessageFromID("E1001DA001");
  return;
}

static void script_C80A(void) {
  Script_ShowUIMessageFromID("E1001DA002");
  return;
}

static void script_C816(void) {
  Script_ShowUIMessageFromID("E1001EA001");
  return;
}

__attribute__((scn_string_bindings(0, data_2A9B4)))
static void script_C822(void) {
  Script_ShowUIMessageFromID("E1001EA006");
  return;
}

static void script_C82E(void) {
  Script_ShowUIMessageFromID("E1001EA009");
  return;
}

__attribute__((scn_string_bindings(0, data_2A9CA)))
static void script_C83A(void) {
  Script_ShowUIMessageFromID("E1001EA006");
  return;
}

static void script_C846(void) {
  float local_FC;
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
  if (!(local_FC >= 0.5f)) {
    Script_ShowUIMessageFromID("E1001EA023");
  } else {
    Script_ShowUIMessageFromID("E1001EA024");
  }
  return;
}

static void script_C87A(void) {
  float local_FC;
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
  if (!(local_FC >= 0.5f)) {
    Script_ShowUIMessageFromID("E1001EA021");
  } else {
    Script_ShowUIMessageFromID("E1001EA022");
  }
  return;
}

static void script_C8AE(void) {
  Script_ShowUIMessageFromID("E1001EA017");
  return;
}

static void script_C8BA(void) {
  Script_ShowUIMessageFromID("E1001EA014");
  return;
}

static void script_C8C6(void) {
  float local_FC;
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
  if (!(local_FC >= 0.5f)) {
    Script_ShowUIMessageFromID("E1001EA013");
  } else {
    Script_ShowUIMessageFromID("E1001EA015");
  }
  return;
}

static void script_C8FA(void) {
  Script_ShowUIMessageFromID("E1001EA016");
  return;
}

static void script_C906(void) {
  Script_ShowUIMessageFromID("E1001EA020");
  return;
}

static void script_C912(void) {
  Script_ShowUIMessageFromID("E1001EA005");
  return;
}

static void script_C91E(void) {
  Script_ShowUIMessageFromID("E1001EA010");
  return;
}

static void script_C92A(void) {
  Script_ShowUIMessageFromID("E1001EA004");
  return;
}

static void script_C936(void) {
  Script_ShowUIMessageFromID("E1001DC053");
  return;
}

static int32_t script_C942(void) {
  return Script_ShowUIMessageFromID("E1001DC054");
}

static void script_C94E(void) {
  Script_ShowUIMessageFromID("E1001DC055");
  return;
}

static void script_C95A(void) {
  Script_ShowUIMessageFromID("E1001DC004");
  return;
}

static int32_t script_C966(void) {
  return Script_ShowUIMessageFromID("E1001DC005");
}

static void script_C972(void) {
  Script_ShowUIMessageFromID("E1001DC006");
  return;
}

static void script_C97E(void) {
  Script_ShowUIMessageFromID("E1001DC007");
  return;
}

static int32_t script_C98A(void) {
  return Script_ShowUIMessageFromID("E1001DC008");
}

static void script_C996(void) {
  Script_ShowUIMessageFromID("E1001DC056");
  return;
}

static void script_C9A2(void) {
  Script_ShowUIMessageFromID("E1001DC010");
  return;
}

static int32_t script_C9AE(void) {
  return Script_ShowUIMessageFromID("E1001DC011");
}

static void script_C9BA(void) {
  Script_ShowUIMessageFromID("E1001DC057");
  return;
}

static void script_C9C6(void) {
  Script_ShowUIMessageFromID("E1001DC013");
  return;
}

static void script_C9D2(void) {
  Script_ShowUIMessageFromID("E1001DC058");
  return;
}

static void script_C9DE(void) {
  Script_ShowUIMessageFromID("E1001DC016");
  return;
}

static int32_t script_C9EA(void) {
  return Script_ShowUIMessageFromID("E1001DC017");
}

static int32_t script_C9F6(void) {
  return Script_ShowUIMessageFromID("E1001DC018");
}

static void script_CA02(void) {
  Script_ShowUIMessageFromID("E1001DC019");
  return;
}

static void script_CA0E(void) {
  Script_ShowUIMessageFromID("E1001DC020");
  return;
}

static void script_CA1A(void) {
  Script_ShowUIMessageFromID("E1001DC021");
  return;
}

static void script_CA26(void) {
  Script_ShowUIMessageFromID("E1001DC022");
  return;
}

static void script_CA32(void) {
  Script_ShowUIMessageFromID("E1001DC023");
  return;
}

static void script_CA3E(void) {
  Script_ShowUIMessageFromID("E1001DC059");
  return;
}

static void script_CA4A(void) {
  Script_ShowUIMessageFromID("E1001DC062");
  return;
}

static void script_CA56(void) {
  Script_ShowUIMessageFromID("E1001DC030");
  return;
}

static int32_t script_CA62(void) {
  return Script_ShowUIMessageFromID("E1001DC031");
}

static int32_t script_CA6E(void) {
  return Script_ShowUIMessageFromID("E1001DC032");
}

static int32_t script_CA7A(void) {
  return Script_ShowUIMessageFromID("E1001DC033");
}

static int32_t script_CA86(void) {
  return Script_ShowUIMessageFromID("E1001DC034");
}

static int32_t script_CA92(void) {
  return Script_ShowUIMessageFromID("E1001DC035");
}

static int32_t script_CA9E(void) {
  return Script_ShowUIMessageFromID("E1001DC036");
}

static int32_t script_CAAA(void) {
  return Script_ShowUIMessageFromID("E1001DC037");
}

static int32_t script_CAB6(void) {
  return Script_ShowUIMessageFromID("E1001DC060");
}

static int32_t script_CAC2(void) {
  return Script_ShowUIMessageFromID("E1001DC039");
}

static void script_CACE(void) {
  Script_ShowUIMessageFromID("E1001DC040");
  return;
}

static void script_CADA(void) {
  Script_ShowUIMessageFromID("E1001DC061");
  return;
}

static void script_CAE6(void) {
  Script_ShowUIMessageFromID("E1001DC051");
  return;
}

static int32_t script_CAF2(void) {
  return Script_ShowUIMessageFromID("E1001DC045");
}

static int32_t script_CAFE(void) {
  return Script_ShowUIMessageFromID("E1001DC046");
}

static int32_t script_CB0A(void) {
  return Script_ShowUIMessageFromID("E1001DC047");
}

static int32_t script_CB16(void) {
  return Script_ShowUIMessageFromID("E1001DC048");
}

static int32_t script_CB22(void) {
  return Script_ShowUIMessageFromID("E1001DC049");
}

static int32_t script_CB2E(void) {
  return Script_ShowUIMessageFromID("E1001DC050");
}

static void script_CB3A(void) {
  Script_ShowUIMessageFromID("E1001BB001");
  return;
}

static void script_CB46(void) {
  Script_ShowUIMessageFromID("E1001BB002");
  return;
}

static void script_CB52(void) {
  Script_ShowUIMessageFromID("E1001BC001");
  return;
}

static void script_CB5E(void) {
  Script_ShowUIMessageFromID("E1001BC002");
  return;
}

static void script_CB6A(void) {
  Script_ShowUIMessageFromID("E1001BD001");
  return;
}

static void script_CB76(void) {
  Script_ShowUIMessageFromID("E1001BD002");
  return;
}

static void script_CB82(void) {
  Script_ShowUIMessageFromID("E1001BE001");
  return;
}

static void script_CB8E(void) {
  Script_ShowUIMessageFromID("E1001BE002");
  return;
}

static void script_CB9A(void) {
  Script_ShowUIMessageFromID("E1001BF001");
  return;
}

static void script_CBA6(void) {
  Script_ShowUIMessageFromID("E1001BF002");
  return;
}

static void script_CBB2(void) {
  Script_ShowUIMessageFromID("E1001BG001");
  return;
}

static void script_CBBE(void) {
  Script_ShowUIMessageFromID("E1001BG002");
  return;
}

static void script_CBCA(void) {
  Script_ShowUIMessageFromID("E1001BH001");
  return;
}

static void script_CBD6(void) {
  Script_ShowUIMessageFromID(data_2ACCC);
  return;
}

static int32_t script_CBE2(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t unused_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  int32_t local_C0[3];
  int32_t local_B4[3];
  local_E8 = 0x52494B41;
  local_F0 = 0;
  Script_ReturnTimeDate(&local_F4);
  local_EC = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  Script_EVT_GetPosition(local_E8, -1, &local_D8, ENTITY_XFORM_WORLD_SPACE);
  (&local_CC)->y = 0.0f;
  if (task_2C3 == 2) {
    if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 857) == 0) & (task_2C6 > 1)) {
      if (((&local_F4)->hours >= 15) & ((&local_F4)->hours < 16)) {
        Vector3i local_A8;
        Script_mtwkGetSetRotation(local_E8, &local_A8, 0x40000000);
        if (((&local_A8)->y >= 1820) & ((&local_A8)->y <= 30947)) {
          if (!((float)Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_THROTTLE_DELTA) <= 0.2f)) {
            local_CC.x = -104.4f;
            (&local_CC)->z = 58.5f;
            if (Script_CheckTargetWithinAngleRange(&local_CC, 1.0f, 0, 0x8000, &local_D8)) {
              Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
              Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
              Script_StartOrFreeSEQ(0);
              SCNF_WaitFrames_0000(1);
              Script_GetConversationDetails(CONV_DETAILS_MODE_CLOSE_CONVERSATION_UI, 0);
              Script_GetConversationDetails(CONV_DETAILS_MODE_SET_SHARED_ITEM_PREVIEW, -1);
              script_2514C();
              Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 857, 1);
              Script_ForkliftUtils(FLMW_MODE_SET_GLOBAL_FLAG);
              Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
              Script_convSetCamInactiveFlag(1);
              Script_SetGameStateTo(AUTH_None);
            }
          }
        }
      }
    }
  }
  if (task_2C3 == 3) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PARTTIME_JOB_WAKE_ROUTE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_MAD_ANGELS_BEHIND_WAREHOUSE_17) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 954) == 0)) {
      if ((local_EC >= 930) & ((&local_F4)->hours < 16)) {
        if (Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_ATTACHED_LOAD_ID) != 0) {
          local_CC.x = -1.17f;
          (&local_CC)->z = 110.68f;
          if (Script_CheckTargetWithinAngleRange(&local_CC, 5.0f, 0, 0x8000, &local_D8)) {
            Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
            Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
            SCNF_WaitFrames_0000(1);
            Script_GetConversationDetails(CONV_DETAILS_MODE_CLOSE_CONVERSATION_UI, 0);
            Script_GetConversationDetails(CONV_DETAILS_MODE_SET_SHARED_ITEM_PREVIEW, -1);
            Script_ForkliftUtils(0x4B524F46, FORK_CMD_SAVE_LOAD_STATE);
            Script_ForkliftUtils(0x4B524F46, FORK_CMD_SNAPSHOT_FOLLOWER_LOADS);
            task_2C5 = 2;
            local_F0 = 1;
          }
        }
      }
    }
  }
  if (task_2C3 == 5) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LAN_DI_STILL_IN_HARBOR) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HARBOR_FOREIGNERS_INQUIRY) == 0)) {
      if ((local_EC >= 870) & (local_EC < 930)) {
        if (Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_ATTACHED_LOAD_ID) != 0) {
          local_D8.x = 29.9f;
          (&local_D8)->z = 48.52f;
          local_CC.x = 33.0f;
          (&local_CC)->z = 46.42f;
          local_C0[0] = 0x42273333;
          local_C0[2] = 0x4254E148;
          local_B4[0] = 0x421D999A;
          local_B4[2] = 0x4262147B;
          if (script_A91E(local_E8, &local_D8, &local_CC, (int32_t)local_C0, (int32_t)local_B4)) {
            Vector3i local_A8;
            Script_mtwkGetSetRotation(local_E8, &local_A8, 0x40000000);
            if (((&local_A8)->y >= 0x2000) & ((&local_A8)->y <= 0xA000)) {
              Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
              Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
              SCNF_WaitFrames_0000(1);
              Script_GetConversationDetails(CONV_DETAILS_MODE_CLOSE_CONVERSATION_UI, 0);
              Script_GetConversationDetails(CONV_DETAILS_MODE_SET_SHARED_ITEM_PREVIEW, -1);
              Script_ForkliftUtils(0x4B524F46, FORK_CMD_SAVE_LOAD_STATE);
              Script_ForkliftUtils(0x4B524F46, FORK_CMD_SNAPSHOT_FOLLOWER_LOADS);
              Script_SetTaskExecutionSuspended((void *)task_2B8, 1);
              SCNF_WaitFrames_0000(1);
              script_AFD1();
              Script_StartOrFreeSEQ(0);
              script_139A7();
              task_2C5 = 4;
              local_F0 = 1;
            }
          }
        }
      }
    }
  }
  if (task_2C3 == 6) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TWO_FOREIGNERS_IN_LONG_ZHA) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 0)) {
      if (((local_EC >= 930) & (local_EC < 990)) | (task_2C7 >= 9)) {
        if ((Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_ATTACHED_LOAD_ID) != 0) & (task_2C6 > 0)) {
          local_D8.x = -90.38f;
          (&local_D8)->z = 37.65f;
          local_CC.x = -93.77f;
          (&local_CC)->z = 33.45f;
          local_C0[0] = -0x3D4E0A3D;
          local_C0[2] = 0x41D60000;
          local_B4[0] = -0x3D535C29;
          local_B4[2] = 0x41E8147B;
          if (script_A91E(local_E8, &local_D8, &local_CC, (int32_t)local_C0, (int32_t)local_B4)) {
            Vector3i local_A8;
            Script_mtwkGetSetRotation(local_E8, &local_A8, 0x40000000);
            if (((&local_A8)->y >= 21845) & ((&local_A8)->y <= 52792)) {
              Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
              Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
              SCNF_WaitFrames_0000(1);
              Script_GetConversationDetails(CONV_DETAILS_MODE_CLOSE_CONVERSATION_UI, 0);
              Script_GetConversationDetails(CONV_DETAILS_MODE_SET_SHARED_ITEM_PREVIEW, -1);
              Script_SetActionSelectorHUDElementVisibility(1);
              Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 255, 0, 0, 0);
              Script_StartOrFreeSEQ(0);
              Script_shenClearFreeState(0);
              Script_sicnUtils(2, 1, 0);
              Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
              while (Script_filmCheckOrSetTransitionUtil(-1)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ForkliftUtils(0x4B524F46, FORK_CMD_ENTER_FORKLIFT_CAMERA_MODE);
              task_2C1 = 3;
              SCNF_WaitFrames_0000(1);
              Script_forkSetSoundBankDetails(SHEN_NAME('FORK'));
              script_23210();
              task_2C5 = 3;
              local_F0 = 1;
            }
          }
        }
      }
    }
  }
  return local_F0;
}

static void script_D109(int32_t arg0, float arg1, float arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_charaDisplay(arg0, CDM_Show);
  local_F4.x = arg1;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = arg2;
  Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  (&local_E8)->y = arg3;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
  return;
}

static void script_D151(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_EC[3];
  int8_t local_EB;
  int32_t local_E4;
  script_B002(SHEN_NAME('FOR2'), SHEN_NAME('ZE06'));
  Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
  local_FC = 0;
  for (;;) {
    local_F8 = Script_GetControllerInput(1);
    local_E4 = task_2C0;
    switch (local_E4) {
      case 0:
        Script_clmdSetFlags(arg0, 4, 1);
        Script_seqEnterEvtCam(2000, arg0, arg0);
        Script_shenClearFreeState(1);
        Script_filmCheckOrSetTransitionUtil(16, 255, 0, 0, 0, 0, 0, 0, 0);
        local_EC[0] = 0x41B15C29;
        local_EC[1] = 0;
        local_EC[2] = 0x42DA0A3D;
        local_EB = script_C58E(0.08f);
        if (Script_forkAttachLoadActor(SHEN_NAME('FOR2'), (const float *)local_EC, 0.08f, 0, 1)) {
          while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2'))) {
            SCNF_WaitFrames_0000(1);
          }
        }
        script_C5E3(local_EB);
        task_2C0 = 2;
        task_2C2 = 1;
        break;
      case 2:
        script_AA6F(arg0, SHEN_NAME('FORK'));
        break;
      case 3:
        Script_flmwFreeFollowerLoadIds(SHEN_NAME('FORK'));
        task_2C0 = 99;
        break;
    }
    if (task_2C0 == 99) {
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_ForkliftUtils(0x4B524F46, FORK_CMD_SUPPRESS_ACTION_HOTKEYS, 0);
  task_2C5 = 1;
  return;
}

static void script_D2A4(int32_t arg0) {
  int32_t local_FC;
  float local_F8;
  Vector3 local_EC;
  if (arg0 == 0) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
    (&local_EC)->y = 1.8f;
    Script_SetCharaLookAtPosition(CHARA_MARK, 6, &local_EC, 2);
  }
  local_F8 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
  if (!(local_F8 >= 0.5f)) {
    if (arg0) {
      local_FC = 2008;
    } else {
      local_FC = 2006;
    }
  } else if (arg0) {
    local_FC = 2009;
  } else {
    local_FC = 2007;
  }
  if (local_FC == 2007) {
    Script_seqEnterEvtCam(local_FC, CHARA_AKIR, CHARA_MARK);
  } else {
    Script_seqEnterEvtCam(local_FC, CHARA_MARK, CHARA_MARK);
  }
  (void)local_FC;
  return;
}

static void script_D35A(void) {
  Script_SetCharaLookAtPosition(CHARA_MARK, -1, 0, 0);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_D379(void) {
  Script_ForkliftUtils(0x4B524F46, FORK_CMD_RESET_DRIVE_INPUTS);
  script_A9CE(SHEN_NAME('FORK'), 22.5f, 111.9f, 56433);
  return;
}

static int32_t script_D3A3(int32_t arg0, int32_t arg1) {
  int32_t unused_F8[2];
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int32_t local_CC;
  int32_t local_C8;
  Vector3 local_BC;
  Vector3 local_B0;
  int32_t local_A4[3];
  int32_t local_98[3];
  int32_t unused_94;
  GameDateTime local_8C;
  int8_t local_8B;
  int8_t local_8A;
  int8_t local_89;
  uint8_t unused_88;
  int32_t local_84;
  int32_t local_80;
  Script_ReturnTimeDate(&local_8C);
  if ((&local_8C)->hours < 12) {
    local_8B = 12;
  } else {
    if ((&local_8C)->hours < 17) {
      local_8B = 17;
      goto L00D3D8;
    }
  }
  L00D3D8:;
  local_EC = 0;
  local_E8 = 0;
  local_DC = 0;
  for (;;) {
    local_D8 = Script_GetControllerInput(1) & -193;
    local_D4 = Script_GetControllerInput(0) & -193;
    local_D0 = Script_GetControllerInput(4);
    local_CC = Script_GetControllerInput(3);
    local_C8 = Script_GetControllerInput(5);
    Script_ReturnTimeDate(&local_8C);
    if (-((&local_8C)->hours >= local_8B) & -(task_2C2 != 8) & -(task_2C8 == 0)) {
      Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
      Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
      Script_sicnUtils(1, 0, 0);
      Script_shenClearFreeState(0);
      script_AD9C();
      if (task_2C2 < 7) {
        script_C576(arg1, 1);
        Script_seqEnterEvtCam(1001, arg1, arg1);
        script_CA4A();
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      } else {
        script_C576(arg0, 1);
        Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
        Script_seqEnterEvtCam(1001, arg0, arg0);
        Script_ForkliftUtils(0x4B524F46, FORK_CMD_HIDE_HELPER_ACTORS);
        script_C83A();
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        SCNF_WaitFrames_0000(40);
      }
      task_2C2 = 8;
    }
    if (task_2C2 <= 6) {
      Script_ForkliftUtils(0x4B524F46, FORK_CMD_SUPPRESS_ACTION_HOTKEYS, 1);
      if (local_D8 & 0x200) {
        Script_ForkliftUtils(0x4B524F46, FORK_CMD_ENTER_FORKLIFT_CAMERA_MODE);
      }
    } else {
      Script_ForkliftUtils(0x4B524F46, FORK_CMD_SUPPRESS_ACTION_HOTKEYS, 0);
    }
    if ((task_2C2 == 2) | (task_2C2 == 3) | (task_2C2 == 4) | (task_2C2 == 5)) {
      if ((-((local_D8 & -10759) == 0) & -(local_D0 == 0) & -(local_CC == 0) & -(local_C8 - 128 == 0)) | (local_D0 & local_CC)) {
        local_EC = local_EC + 1;
        local_E8 = local_EC / 30;
        if (local_EC % 30 == 0) {
          Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
          Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
          script_D2A4(0);
          Script_shenClearFreeState(0);
          if (local_E8 == 10) {
            Script_scenCreateTask(script_CAF2, 0);
            SCNF_WaitFrames_0000(60);
          } else if (local_E8 == 20) {
            Script_scenCreateTask(script_CAFE, 0);
            SCNF_WaitFrames_0000(60);
          } else {
            if (local_E8 >= 30) {
              if (local_E8 % 10 == 0) {
                switch (task_2C2) {
                  case 2:
                    Script_scenCreateTask(script_CB0A, 0);
                    SCNF_WaitFrames_0000(60);
                    break;
                  case 3:
                    Script_scenCreateTask(script_CB16, 0);
                    SCNF_WaitFrames_0000(60);
                    break;
                  case 4:
                    Script_scenCreateTask(script_CB22, 0);
                    SCNF_WaitFrames_0000(60);
                    break;
                  case 5:
                    Script_scenCreateTask(script_CB2E, 0);
                    SCNF_WaitFrames_0000(60);
                    break;
                }
              }
              goto L00D6A3;
            }
          }
          L00D6A3:;
          Script_shenClearFreeState(1);
          script_D35A();
          Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
          Script_convSetCamInactiveFlag(1);
          Script_SetGameStateTo(AUTH_None);
        }
      } else {
        local_E8 = 0;
        local_EC = 0;
      }
    }
    if (task_2C1 == 1) {
      int32_t local_FF7C;
      local_FF7C = task_2C2;
      switch (local_FF7C) {
        case 1:
          Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
          Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
          Script_shenClearFreeState(0);
          script_EAF8(2001, arg0, arg0);
          Script_clmdSetFlags(arg0, 4, 0);
          script_C936();
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_scenCreateTask(script_C942, 0);
          SCNF_WaitFrames_0000(60);
          local_F0 = 0;
          local_F4 = 0;
          while (local_F0 < 15) {
            local_F4 = *&local_F4 + 273;
            Script_ForkliftUtils(0x32524F46, FORK_CMD_GET_SET_STEER_OFFSET, *&local_F4);
            local_F0 = local_F0 + 1;
            SCNF_WaitFrames_0000(1);
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          script_EB7D();
          Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
          Script_convSetCamInactiveFlag(1);
          Script_SetGameStateTo(AUTH_None);
          Script_shenClearFreeState(1);
          task_2C2 = 2;
          local_E4 = 0;
          local_8A = 0;
          break;
        case 2:
          local_F4 = Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_SET_STEER_OFFSET, 0);
          if ((*&local_F4 >= 3640) | (*&local_F4 <= -3640)) {
            script_EAF8(2003, arg1, arg1);
            SCNF_WaitFrames_0000(1);
            Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
            Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
            Script_shenClearFreeState(0);
            script_C94E();
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            script_C95A();
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            script_EB7D();
            script_D379();
            Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
            Script_convSetCamInactiveFlag(1);
            Script_SetGameStateTo(AUTH_None);
            Script_shenClearFreeState(1);
            task_2C2 = 3;
            local_E4 = 0;
            local_E0 = 0;
            local_8A = 0;
          } else if ((local_D8 & 0x15F9) | local_D0 | local_CC) {
            if (local_8A <= 6) {
              local_8A = local_8A + 1;
            }
            script_EBAB(1113, local_8A);
            local_E4 = 0;
          } else {
            if ((local_C8 - 128) | (local_D4 & 0xC000)) {
              if (local_E4 > 30) {
                if (local_8A < 6) {
                  local_8A = local_8A + 1;
                }
                script_EBAB(64, local_8A);
                local_E4 = 0;
              } else {
                local_E4 = local_E4 + 1;
              }
              goto L00D947;
            }
          }
          L00D947:;
          break;
        case 3:
          if ((local_CC != 0) & ((local_D4 & -13831) == 0)) {
            local_E0 = local_E0 + local_CC;
            if (local_E4 > 25) {
              if (local_E0 > 5000) {
                SCNF_WaitFrames_0000(1);
                Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
                Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
                Script_shenClearFreeState(0);
                script_EAF8(2005, arg0, arg1);
                script_C972();
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                script_C97E();
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                script_EB7D();
                script_D379();
                Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
                Script_convSetCamInactiveFlag(1);
                Script_SetGameStateTo(AUTH_None);
                Script_shenClearFreeState(1);
                task_2C2 = 4;
                local_E4 = 0;
                local_E0 = 0;
                local_8A = 0;
              } else {
                if (local_8A <= 8) {
                  local_8A = local_8A + 1;
                }
                script_EBAB(32, local_8A);
                local_E0 = 0;
                local_E4 = 0;
              }
            } else {
              if (!((float)Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_THROTTLE_DELTA) <= 0.0f)) {
                local_E4 = local_E4 + 1;
              }
            }
          } else {
            local_E0 = 0;
          }
          if ((local_D4 & 0xD5F9) | local_D0 | (local_C8 != 128)) {
            if (local_8A <= 8) {
              local_8A = local_8A + 1;
            }
            script_EBAB(572, local_8A);
            if (local_D4 & -5121) {
              local_E0 = 0;
              local_E4 = 0;
            }
          }
          break;
        case 4:
          if ((local_D0 != 0) & ((local_D4 & -13831) == 0)) {
            local_E0 = local_E0 + local_D0;
            if (local_E4 > 30) {
              if (local_E0 > 6000) {
                SCNF_WaitFrames_0000(1);
                Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
                Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
                Script_shenClearFreeState(0);
                script_EAF8(2004, arg1, arg1);
                script_C996();
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                script_C9A2();
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                script_EB7D();
                script_D379();
                Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
                Script_convSetCamInactiveFlag(1);
                Script_SetGameStateTo(AUTH_None);
                Script_shenClearFreeState(1);
                task_2C2 = 5;
                local_E4 = script_F16E(0x4B524F46);
                local_E0 = local_E4;
                local_8A = 0;
              } else {
                if (local_8A <= 8) {
                  local_8A = local_8A + 1;
                }
                script_EBAB(128, local_8A);
                local_E0 = 0;
                local_E4 = 0;
              }
            } else {
              if (!((float)Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_THROTTLE_DELTA) >= 0.0f)) {
                local_E4 = local_E4 + 1;
              }
            }
          } else {
            local_E0 = 0;
          }
          if ((local_D4 & 0xD5F9) | local_CC | (local_C8 != 128)) {
            if (local_8A <= 8) {
              local_8A = local_8A + 1;
            }
            script_EBAB(2197, local_8A);
            if (local_D4 & -5121) {
              local_E0 = 0;
              local_E4 = 0;
            }
          }
          break;
        case 5:
          local_E4 = script_F16E(0x4B524F46);
          if ((local_E4 != -1) & (local_E4 != local_E0)) {
            Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
            Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
            Script_shenClearFreeState(0);
            script_EAF8(2010, arg1, arg1);
            script_C9BA();
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_seqEnterEvtCam(2011, arg1, arg1);
            script_C9C6();
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_seqEnterEvtCam(2012, arg1, arg1);
            script_C9D2();
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_scenCreateTask(script_C9EA, 0);
            SCNF_WaitFrames_0000(0);
            script_EB7D();
            script_D379();
            Script_ForkliftUtils(0x32524F46, FORK_CMD_RESET_DRIVE_INPUTS);
            task_2C2 = 6;
            Script_seqEnterEvtCam(2014, arg0, arg0);
          } else {
            if ((local_D4 & 0xD1F9) | local_D0 | local_CC | (local_C8 != 128)) {
              if (local_8A <= 8) {
                local_8A = local_8A + 1;
              }
              script_EBAB(269, local_8A);
              for (;;) {
                local_E0 = script_F16E(0x4B524F46);
                if (local_E0 != -1) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
              goto L00DDB4;
            }
          }
          L00DDB4:;
          break;
        case 6:
          local_BC.x = 28.63f;
          (&local_BC)->y = 0.0f;
          (&local_BC)->z = 102.87f;
          local_89 = script_C58E(0.08f);
          if (Script_forkAttachLoadActor(SHEN_NAME('FOR2'), (const float *)&local_BC, 0.08f, 0, 1)) {
            while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2'))) {
              SCNF_WaitFrames_0000(1);
            }
          }
          script_C5E3(local_89);
          Script_seqEnterEvtCam(2015, 0, AUTH_None);
          Script_scenCreateTask(script_C9F6, 0);
          SCNF_WaitFrames_0000(0);
          local_BC.x = 23.33f;
          (&local_BC)->y = 1.54f;
          (&local_BC)->z = 105.94f;
          Script_SetCharaLookAtPosition(arg0, 98405, &local_BC, 0);
          Script_executeAicaCommand(0x1304A9, 0, 0);
          Script_forkSetForkHeightTarget(SHEN_NAME('FOR2'), 0);
          while (Script_forkUpdateForkHeightLerp(SHEN_NAME('FOR2'))) {
            SCNF_WaitFrames_0000(1);
          }
          Script_flmwAttachLoadActor(SHEN_NAME('FOR2'), SHEN_NAME('BOX1'));
          Script_forkSetForkHeightTarget(SHEN_NAME('FOR2'), 1);
          while (Script_forkUpdateForkHeightLerp(SHEN_NAME('FOR2'))) {
            SCNF_WaitFrames_0000(1);
          }
          Script_executeAicaCommand(394409, 0, 0);
          Script_executeAicaCommand(853161, 0, 0);
          local_BC.x = 28.25f;
          (&local_BC)->y = 0.0f;
          (&local_BC)->z = 109.88f;
          local_89 = script_C58E(0.08f);
          if (Script_forkAttachLoadActor(SHEN_NAME('FOR2'), (const float *)&local_BC, -0.08f, 0, 0)) {
            while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2'))) {
              SCNF_WaitFrames_0000(1);
            }
          }
          script_C5E3(local_89);
          Script_executeAicaCommand(918697, 0, 0);
          local_BC.x = 17.64f;
          (&local_BC)->y = 0.0f;
          (&local_BC)->z = 100.51f;
          local_89 = script_C58E(0.08f);
          if (Script_forkAttachLoadActor(SHEN_NAME('FOR2'), (const float *)&local_BC, 0.08f, 0, 0)) {
            while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2'))) {
              SCNF_WaitFrames_0000(1);
            }
          }
          script_C5E3(local_89);
          local_BC.x = 19.33f;
          (&local_BC)->y = 0.0f;
          (&local_BC)->z = 96.8f;
          local_89 = script_C58E(0.08f);
          if (Script_forkAttachLoadActor(SHEN_NAME('FOR2'), (const float *)&local_BC, 0.08f, 0, 1)) {
            while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2'))) {
              SCNF_WaitFrames_0000(1);
            }
          }
          script_C5E3(local_89);
          Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
          Script_seqEnterEvtCam(2016, arg0, arg0);
          Script_executeAicaCommand(0x1104A9, 0, 0);
          Script_forkSetForkHeightTarget(SHEN_NAME('FOR2'), 0);
          while (Script_forkUpdateForkHeightLerp(SHEN_NAME('FOR2'))) {
            SCNF_WaitFrames_0000(1);
          }
          Script_flmwFinalizeLoadDetach(SHEN_NAME('FOR2'));
          Script_executeAicaCommand(263337, 0, 0);
          Script_executeAicaCommand(656553, 0, 0);
          SCNF_WaitFrames_0000(30);
          Script_executeAicaCommand(853161, 0, 0);
          local_BC.x = 12.58f;
          (&local_BC)->y = 0.0f;
          (&local_BC)->z = 98.89f;
          local_89 = script_C58E(0.08f);
          if (Script_forkAttachLoadActor(SHEN_NAME('FOR2'), (const float *)&local_BC, -0.08f, 0, 0)) {
            while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2'))) {
              SCNF_WaitFrames_0000(1);
            }
          }
          script_C5E3(local_89);
          Script_executeAicaCommand(918697, 0, 0);
          local_BC.x = 21.03f;
          (&local_BC)->y = 0.0f;
          (&local_BC)->z = 103.41f;
          local_89 = script_C58E(0.08f);
          if (Script_forkAttachLoadActor(SHEN_NAME('FOR2'), (const float *)&local_BC, 0.08f, 0, 0)) {
            while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2'))) {
              SCNF_WaitFrames_0000(1);
            }
          }
          script_C5E3(local_89);
          Script_seqEnterEvtCam(2017, arg0, arg0);
          local_BC.x = 21.23f;
          (&local_BC)->y = 0.0f;
          (&local_BC)->z = 109.3f;
          local_89 = script_C58E(0.08f);
          if (Script_forkAttachLoadActor(SHEN_NAME('FOR2'), (const float *)&local_BC, 0.08f, 0, 1)) {
            while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2'))) {
              SCNF_WaitFrames_0000(1);
            }
          }
          script_C5E3(local_89);
          script_EAF8(-1, 0, 0);
          script_CA02();
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          script_CA0E();
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_seqEnterEvtCam(2018, arg1, arg1);
          script_CA1A();
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          script_CA26();
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          script_CA32();
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_seqEnterEvtCam(2019, arg1, arg1);
          script_C7FE();
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          script_CA3E();
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          script_C80A();
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          script_EB7D();
          SCNF_WaitFrames_0000(15);
          local_BC.x = 16.93f;
          (&local_BC)->y = 0.0f;
          (&local_BC)->z = 112.3f;
          local_89 = script_C58E(0.08f);
          if (Script_forkAttachLoadActor(SHEN_NAME('FOR2'), (const float *)&local_BC, 0.08f, 0, 1)) {
            while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2'))) {
              SCNF_WaitFrames_0000(1);
            }
          }
          script_C5E3(local_89);
          Script_PlayCharaMotion(arg1, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_forkSetSoundBankDetails(SHEN_NAME('FOR2'));
          local_BC.x = 1000.0f;
          (void)(&local_BC)->y;
          (void)0;
          (&local_BC)->z = (&local_BC)->x;
          Script_EVT_SetPosition(SHEN_NAME('FOR2'), 0x78000000, &local_BC);
          Script_EVT_SetPosition(arg1, 0x78000000, &local_BC);
          Script_charaDisplay(SHEN_NAME('FOR2'), CDM_Hide);
          script_AF37(arg1, 0);
          Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
          Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 1);
          Script_ForkliftUtils(FLMW_MODE_SET_GLOBAL_FLAG);
          Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
          Script_convSetCamInactiveFlag(0);
          Script_SetGameStateTo(AUTH_None);
          Script_shenClearFreeState(1);
          Script_sicnUtils(0, 1, 1);
          Script_scenCreateTask(script_F1DA, 2, arg0, 1);
          Script_scenCreateTask(script_F1DA, 2, arg1, 3);
          task_2B0 = 0;
          Script_flmwSetInputMask(SHEN_NAME('FORK'), 0);
          task_2C2 = 7;
          script_C576(arg0, 0);
          if (-1) {
            Script_charaDisplay(SHEN_NAME('FOR2'), CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(SHEN_NAME('FOR2'), CDM_Show);
              goto L00E484;
            }
          }
          L00E484:;
          Script_scenToggleVisibleFor(SHEN_NAME('FOR2'), 0);
          if (-1) {
            Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Show);
              goto L00E4B8;
            }
          }
          L00E4B8:;
          Script_scenToggleVisibleFor(SHEN_NAME('ZE06'), 0);
          break;
      }
    }
    {
      int32_t local_FF7C;
      local_FF7C = task_2C2;
      switch (local_FF7C) {
        case 7:
          local_F0 = 0;
          local_DC = 0;
          while (local_F0 < 16) {
            local_84 = Script_ScenDataUtil(&data_2A124, SCND_ReadU32, local_F0);
            local_BC.x = 19.47f;
            (&local_BC)->z = 94.04f;
            local_B0.x = 24.45f;
            (&local_B0)->z = 99.02f;
            local_A4[0] = 0x41B0147B;
            local_A4[2] = 0x42CAEB85;
            local_98[0] = 0x41883D71;
            local_98[2] = 0x42C0F0A4;
            local_E8 = script_A91E(local_84, &local_BC, &local_B0, (int32_t)local_A4, (int32_t)local_98);
            local_BC.x = 19.47f;
            (&local_BC)->z = 94.04f;
            local_B0.x = 21.9f;
            (&local_B0)->z = 96.47f;
            local_A4[0] = 0x4191999A;
            local_A4[2] = 0x42C86148;
            local_98[0] = 0x417C51EC;
            local_98[2] = 0x42C375C3;
            local_E8 = local_E8 | script_A91E(local_84, &local_BC, &local_B0, (int32_t)local_A4, (int32_t)local_98);
            if (local_E8 & -(Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_ATTACHED_LOAD_ID) != local_84)) {
              Script_SetCharaFlags(local_84, 2);
            } else {
              Script_SetCharaFlags(local_84, 9);
            }
            local_DC = local_DC + local_E8;
            local_F0 = local_F0 + 1;
          }
          if (local_DC >= 16) {
            if (Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_ATTACHED_LOAD_ID) == 0) {
              Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
              Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
              Script_sicnUtils(1, 0, 0);
              Script_shenClearFreeState(0);
              script_C576(arg0, 1);
              SCNF_WaitFrames_0000(1);
              Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
              Script_seqEnterEvtCam(1000, arg0, arg0);
              script_C912();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              SCNF_WaitFrames_0000(60);
              task_2C2 = 8;
            }
          }
          break;
        case 8:
          if (local_DC > 0) {
            local_DC = local_DC - 1;
          }
          local_80 = Script_PlayerStatsUtils(PS_GetStatU16_0);
          local_80 = local_80 + local_DC;
          if (local_80 > 0x7D00) {
            local_80 = 0x7D00;
          }
          Script_PlayerStatsUtils(PS_SetStatU16_0, local_80);
          local_80 = Script_PlayerStatsUtils(PS_GetStatU32_0);
          local_80 = local_80 + 2000;
          if (local_80 > 0x7F8DCF00) {
            local_80 = 0x7F8DCF00;
          }
          Script_PlayerStatsUtils(PS_SetStatU32_0, local_80);
          Script_ForkliftUtils(0x4B524F46, FORK_CMD_SNAPSHOT_FOLLOWER_LOADS);
          task_2C0 = 3;
          Script_srchControl(0);
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_2ACF8)))
static void script_E755(void) {
  int32_t unused_F4[3];
  Script_ForkliftUtils(FLMW_MODE_SET_GLOBAL_FLAG);
  Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  task_2C0 = 0;
  task_2C2 = 0;
  Script_LoadVoiceFile(1, "E1001D");
  Script_SetAnimatedSequenceCollisionCheckDisabled(0);
  task_2AC = Script_scenCreateTask(script_D3A3, 2, CHARA_AKIR, CHARA_MARK);
  Script_scenToggleVisibleFor(SHEN_NAME('FOR2'), 1);
  Script_charaDisplay(SHEN_NAME('FOR2'), CDM_Show);
  Script_SetCPAMFlags(CHARA_MARK, 0);
  script_A9CE(CHARA_AKIR, 22.5f, 111.9f, 56433);
  script_A9CE(SHEN_NAME('FORK'), 22.5f, 111.9f, 56433);
  script_A9CE(SHEN_NAME('FOR2'), 13.37f, 117.72f, 56433);
  script_AA05(SHEN_NAME('BOX1'), 29.1f, 0.001f, 102.1f, 0x6000);
  script_AA05(SHEN_NAME('BOX2'), 31.23f, 0.001f, 104.7f, 0x6000);
  script_AA05(SHEN_NAME('BOX3'), 37.5f, 0.001f, 106.9f, 0x6000);
  script_AA05(SHEN_NAME('BOX4'), 35.8f, 0.001f, 108.3f, 0x6000);
  script_AA05(SHEN_NAME('BOX5'), 35.9f, 0.001f, 105.2f, 0x6000);
  script_AA05(SHEN_NAME('BOX6'), 34.2f, 0.001f, 106.7f, 0x6000);
  script_AA05(SHEN_NAME('BOX7'), 37.5f, 1.5f, 106.9f, 0x6000);
  script_AA05(SHEN_NAME('BOX8'), 35.8f, 1.5f, 108.3f, 0x6000);
  script_AA05(SHEN_NAME('BOX9'), 35.9f, 1.5f, 105.2f, 0x6000);
  script_AA05(SHEN_NAME('BOXA'), 32.7f, 0.001f, 108.3f, 0x6000);
  script_AA05(SHEN_NAME('BOXB'), 34.1f, 0.001f, 109.9f, 0x6000);
  script_AA05(SHEN_NAME('BOXC'), 32.8f, 0.001f, 111.3f, 0x6000);
  script_AA05(SHEN_NAME('BOXD'), 31.4f, 0.001f, 109.6f, 0x6000);
  script_AA05(SHEN_NAME('BOXE'), 34.1f, 1.5f, 109.9f, 0x6000);
  script_AA05(SHEN_NAME('BOXF'), 32.8f, 1.5f, 111.3f, 0x6000);
  script_AA05(SHEN_NAME('BOXG'), 31.4f, 1.5f, 109.6f, 0x6000);
  script_AFA0(0x31334143);
  script_AFA0(0x31344143);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_CharHandlers(CHARA_MARK, -1, 0, 0);
  Script_ForkliftUtils(0x4B524F46, FORK_CMD_SUPPRESS_ACTION_HOTKEYS, 1);
  Script_flmwSetFollowerLoadIdList(SHEN_NAME('FORK'), 0x31584F42, 0x32584F42, 0x33584F42, 0x34584F42, 0x35584F42, 0x36584F42, 0x37584F42, 0x38584F42, 0x39584F42, 0x41584F42, 0x42584F42, 0x43584F42, 0x44584F42, 0x45584F42, 0x46584F42, 0x47584F42, -1);
  Script_convSetCamInactiveFlag(1);
  script_B1ED(CHARA_AKIR, SHEN_NAME('FORK'));
  script_B165(CHARA_MARK, SHEN_NAME('FOR2'));
  Script_scenCreateTask(script_F1DA, 2, CHARA_AKIR, 2);
  Script_scenCreateTask(script_F1DA, 2, CHARA_MARK, 2);
  return;
}

static void script_EAF8(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = 1.7f;
    Script_SetCharaLookAtPosition(CHARA_MARK, 6, &local_F4, 0);
  }
  SCNF_WaitFrames_0000(15);
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_MARK, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = 1.8f;
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_F4, 0);
  }
  Script_clmdSetFlags(CHARA_AKIR, 4, 1);
  if (arg0 >= 0) {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return;
}

static void script_EB7D(void) {
  Script_SetCharaLookAtPosition(CHARA_MARK, -1, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_clmdSetFlags(CHARA_AKIR, 4, 0);
  return;
}

static void script_EBAB(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int8_t local_EB;
  local_FC = Script_GetControllerInput(1);
  local_F8 = Script_GetControllerInput(0);
  local_F4 = Script_GetControllerInput(4);
  local_F0 = Script_GetControllerInput(3);
  local_EC = Script_GetControllerInput(5);
  local_EB = 0;
  if (arg0 == 64) {
    Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
    Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
    Script_shenClearFreeState(0);
    script_D2A4(0);
    Script_scenCreateTask(script_C966, 0);
    SCNF_WaitFrames_0000(90);
    local_EB = 1;
  }
  if ((local_F0 != 0) & (arg0 == 32)) {
    Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
    Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
    Script_shenClearFreeState(0);
    script_D2A4(0);
    Script_scenCreateTask(script_C98A, 0);
    SCNF_WaitFrames_0000(80);
    local_EB = 1;
  }
  if ((local_F4 != 0) & (arg0 == 128)) {
    Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
    Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
    Script_shenClearFreeState(0);
    script_D2A4(0);
    Script_scenCreateTask(script_C9AE, 0);
    SCNF_WaitFrames_0000(90);
    local_EB = 1;
  }
  if (local_FC & 0x1000) {
    Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
    Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
    Script_shenClearFreeState(0);
    script_D2A4(0);
    script_C7F2();
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    local_EB = 1;
  } else if (local_FC & 0x400 & -((arg0 & 0x10) != 0)) {
    Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
    Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
    Script_shenClearFreeState(0);
    script_D2A4(0);
    script_C7E6();
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    local_EB = 1;
  } else if ((-(local_EC != 128) | (local_F8 & 0xC000)) & -((arg0 & 4) != 0)) {
    SCNF_WaitFrames_0000(4);
    Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
    Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
    Script_shenClearFreeState(0);
    script_D2A4(0);
    Script_scenCreateTask(script_C7CE, 0);
    SCNF_WaitFrames_0000(90);
    local_EB = 1;
  } else if ((local_F0 != 0) & ((arg0 & 1) != 0)) {
    if (!((float)Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_THROTTLE_DELTA) <= 0.0f)) {
      SCNF_WaitFrames_0000(10);
      Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
      Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
      Script_shenClearFreeState(0);
      script_D2A4(0);
      Script_scenCreateTask(script_C7C2, 0);
      SCNF_WaitFrames_0000(90);
      local_EB = 1;
    }
  } else {
    if ((local_F4 != 0) & ((arg0 & 8) != 0)) {
      if (!((float)Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_THROTTLE_DELTA) >= 0.0f)) {
        SCNF_WaitFrames_0000(10);
        Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
        Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
        Script_shenClearFreeState(0);
        script_D2A4(0);
        Script_scenCreateTask(script_C7DA, 0);
        SCNF_WaitFrames_0000(80);
        local_EB = 1;
      }
      goto L00EEAE;
    }
  }
  L00EEAE:;
  if (local_EB) {
    if (arg0 & 0x20) {
      if (arg1 == 4) {
        Script_scenCreateTask(script_CA62, 0);
        SCNF_WaitFrames_0000(60);
      } else if (arg1 == 5) {
        Script_scenCreateTask(script_CA92, 0);
        SCNF_WaitFrames_0000(60);
      } else if (arg1 == 6) {
        Script_scenCreateTask(script_CA9E, 0);
        SCNF_WaitFrames_0000(60);
      } else {
        if (arg1 >= 7) {
          if (arg0 & 0x200) {
            Script_scenCreateTask(script_CAAA, 0);
            SCNF_WaitFrames_0000(60);
          } else {
            Script_scenCreateTask(script_CA9E, 0);
            SCNF_WaitFrames_0000(60);
          }
          goto L00EF59;
        }
      }
      L00EF59:;
    } else if (arg0 & 0x40) {
      if (arg1 == 3) {
        Script_scenCreateTask(script_CA62, 0);
        SCNF_WaitFrames_0000(60);
      } else if (arg1 == 4) {
        Script_scenCreateTask(script_CA6E, 0);
        SCNF_WaitFrames_0000(60);
      } else if (arg1 == 5) {
        Script_scenCreateTask(script_CA7A, 0);
        SCNF_WaitFrames_0000(60);
      } else {
        if (arg1 >= 6) {
          if (arg0 & 0x400) {
            Script_scenCreateTask(script_CA86, 0);
            SCNF_WaitFrames_0000(120);
          } else {
            Script_scenCreateTask(script_CA7A, 0);
            SCNF_WaitFrames_0000(60);
          }
          goto L00F001;
        }
      }
      L00F001:;
    } else if (arg0 & 0x80) {
      if (arg1 == 4) {
        Script_scenCreateTask(script_CA62, 0);
        SCNF_WaitFrames_0000(60);
      } else if (arg1 == 5) {
        Script_scenCreateTask(script_CA6E, 0);
        SCNF_WaitFrames_0000(60);
      } else if (arg1 == 6) {
        Script_scenCreateTask(script_CA7A, 0);
        SCNF_WaitFrames_0000(70);
      } else {
        if (arg1 >= 7) {
          if (arg0 & 0x800) {
            Script_scenCreateTask(script_CAB6, 0);
            SCNF_WaitFrames_0000(120);
          } else {
            Script_scenCreateTask(script_CA7A, 0);
            SCNF_WaitFrames_0000(60);
          }
          goto L00F0AA;
        }
      }
      L00F0AA:;
    } else {
      if (arg0 & 0x100) {
        if (arg1 == 3) {
          Script_scenCreateTask(script_CA62, 0);
          SCNF_WaitFrames_0000(60);
        } else if (arg1 == 4) {
          Script_scenCreateTask(script_CA92, 0);
          SCNF_WaitFrames_0000(60);
        } else if (arg1 == 5) {
          Script_scenCreateTask(script_CA9E, 0);
          SCNF_WaitFrames_0000(60);
        } else {
          if (arg1 >= 6) {
            Script_scenCreateTask(script_CAC2, 0);
            SCNF_WaitFrames_0000(70);
            goto L00F133;
          }
        }
        L00F133:;
        goto L00F136;
      }
    }
    L00F136:;
    Script_ForkliftUtils(0x4B524F46, FORK_CMD_ENTER_FORKLIFT_CAMERA_MODE);
    script_D379();
    script_D35A();
    Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
    Script_shenClearFreeState(1);
  }
  return;
}

static int8_t script_F16E(int32_t arg0) {
  float local_FC;
  int32_t local_F8;
  int8_t local_F7;
  local_F7 = -1;
  *(int32_t *)&local_FC = Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_SET_FORK_HEIGHT, -1.0f);
  local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, local_FC * (float)1000);
  switch (local_F8) {
    case 0:
      local_F7 = 0;
      break;
    case 1500:
      local_F7 = 1;
      break;
    case 1800:
      local_F7 = 2;
      break;
  }
  return local_F7;
}

static int32_t script_F1DA(int32_t arg0, int32_t arg1) {
  switch (arg1) {
    case 0:
      {
        Vector4 local_F0;
        local_F0.x = 0.0f;
        (&local_F0)->y = 0.0f;
        (&local_F0)->z = 0.0f;
        (&local_F0)->w = 0.0f;
        Script_charaSetRefboardFlagTo(arg0, 1);
        while (1) {
          if (Script_scenCheckCharaExist(arg0) == 0) {
            break;
          }
          if (!((&local_F0)->x >= 0.5f)) {
            local_F0.x = (&local_F0)->x + 0.02f;
            (&local_F0)->y = (&local_F0)->y + 0.02f;
            (&local_F0)->z = (&local_F0)->z + 0.02f;
            (&local_F0)->w = (&local_F0)->w + 0.02f;
            Script_SetCharaRefBoardPos(arg0, &local_F0);
          } else {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        break;
      }
    case 1:
      {
        Vector4 local_F0;
        local_F0.x = 0.5f;
        (&local_F0)->y = 0.5f;
        (&local_F0)->z = 0.5f;
        (&local_F0)->w = 0.5f;
        while (1) {
          if (Script_scenCheckCharaExist(arg0) == 0) {
            break;
          }
          if (!((&local_F0)->x <= 0.0f)) {
            local_F0.x = (&local_F0)->x - 0.02f;
            (&local_F0)->y = (&local_F0)->y - 0.02f;
            (&local_F0)->z = (&local_F0)->z - 0.02f;
            (&local_F0)->w = (&local_F0)->w - 0.02f;
            Script_SetCharaRefBoardPos(arg0, &local_F0);
          } else {
            Script_charaSetRefboardFlagTo(arg0, 2);
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        break;
      }
    case 2:
      {
        Vector4 local_F0;
        if (Script_scenCheckCharaExist(arg0) == 0) {
          break;
        }
        local_F0.x = 0.5f;
        (&local_F0)->y = 0.5f;
        (&local_F0)->z = 0.5f;
        (&local_F0)->w = 0.5f;
        Script_charaSetRefboardFlagTo(arg0, 1);
        Script_SetCharaRefBoardPos(arg0, &local_F0);
        break;
      }
    case 3:
      {
        Vector4 local_F0;
        if (Script_scenCheckCharaExist(arg0) == 0) {
          break;
        }
        local_F0.x = 0.0f;
        (&local_F0)->y = 0.0f;
        (&local_F0)->z = 0.0f;
        (&local_F0)->w = 0.0f;
        Script_SetCharaRefBoardPos(arg0, &local_F0);
        Script_charaSetRefboardFlagTo(arg0, 2);
        break;
      }
  }
  end:;
}

static void script_F406(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F4[2];
  GameDateTime local_EC;
  int32_t unused_E0[3];
  Vector3 local_D4;
  int32_t local_D0;
  for (;;) {
    local_D0 = task_2C0;
    switch (local_D0) {
      case 0:
        Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
        Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
        Script_ReturnTimeDate(&local_EC);
        if ((&local_EC)->hours < 12) {
          script_1184C();
          Script_GetCassetteFlags(Script_ScenDataUtil(&data_2A17C, SCND_ReadU32, task_2C3 - 2), 1, 0, 0);
          Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
          for (;;) {
            if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        } else if ((&local_EC)->hours < 17) {
          script_B1ED(arg0, SHEN_NAME('FORK'));
          Script_convSetCamInactiveFlag(1);
          local_D4.x = 1000.0f;
          (&local_D4)->y = 0.0f;
          (&local_D4)->z = (&local_D4)->x;
          Script_EVT_SetPosition(CHARA_MARK, POS_XYZ, &local_D4);
          SCNF_WaitFrames_0000(1);
          script_A9C0(arg0);
          Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
          for (;;) {
            if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
          Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          local_D0 = task_2C4;
          switch (local_D0) {
            case 1:
              if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_MAD_ANGELS_BEHIND_WAREHOUSE_17) == 0) {
                script_C8BA();
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
              }
              break;
            case 0:
              if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HARBOR_FOREIGNERS_INQUIRY) == 0) {
                script_C8C6();
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
              }
              break;
            case 2:
              script_C8FA();
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              break;
          }
          Script_SetGameStateTo(AUTH_None);
        } else {
          script_B1ED(arg0, SHEN_NAME('FORK'));
          Script_convSetCamInactiveFlag(1);
          Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
          for (;;) {
            if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
          Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        script_AF37(0x4B52414D, 0);
        SCNF_WaitFrames_0000(1);
        if (task_2C3 == 6) {
          script_AFA0(0x4B52414D);
        }
        script_C576(arg0, 0);
        Script_shenClearFreeState(1);
        Script_sicnUtils(0, 1, 1);
        Script_ForkliftUtils(FLMW_MODE_SET_GLOBAL_FLAG);
        Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
        Script_convSetCamInactiveFlag(1);
        Script_SetGameStateTo(AUTH_None);
        Script_convSetCamInactiveFlag(1);
        Script_SetGameStateTo(AUTH_None);
        task_2C0 = 2;
        break;
      case 2:
        script_AA6F(arg0, SHEN_NAME('FORK'));
        break;
      case 3:
        Script_flmwFreeFollowerLoadIds(SHEN_NAME('FORK'));
        task_2B0 = 0;
        Script_flmwSetInputMask(SHEN_NAME('FORK'), 0);
        task_2C0 = 99;
        break;
    }
    if (task_2C0 == 99) {
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_F72A(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t unused_E0[5];
  int8_t local_DF;
  int8_t local_DE;
  int8_t local_DD;
  int8_t local_DC;
  int8_t local_DB;
  uint8_t unused_D8[3];
  int32_t local_D4;
  Vector3 local_C8;
  Vector3 local_BC;
  int32_t local_B0[3];
  int32_t local_A4[3];
  local_DB = 0;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours >= 17) {
    local_DB = 1;
    for (;;) {
      Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
      Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
      if (task_2C0 == 2) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    if (script_CBE2()) {
      break;
    }
    local_DF = 0;
    local_DE = 0;
    local_DD = 0;
    while (local_DF < 20) {
      int32_t local_A0;
      local_D4 = Script_ScenDataUtil(&data_2A124, SCND_ReadU32, local_DF);
      local_DC = 0;
      local_A0 = task_2C4;
      switch (local_A0) {
        case 1:
          local_C8.x = 19.47f;
          (&local_C8)->z = 94.04f;
          local_BC.x = 24.45f;
          (&local_BC)->z = 99.02f;
          local_B0[0] = 0x41B0147B;
          local_B0[2] = 0x42CAEB85;
          local_A4[0] = 0x41883D71;
          local_A4[2] = 0x42C0F0A4;
          local_DC = script_A91E(local_D4, &local_C8, &local_BC, (int32_t)local_B0, (int32_t)local_A4);
          local_C8.x = 19.47f;
          (&local_C8)->z = 94.04f;
          local_BC.x = 21.9f;
          (&local_BC)->z = 96.47f;
          local_B0[0] = 0x4191999A;
          local_B0[2] = 0x42C86148;
          local_A4[0] = 0x417C51EC;
          local_A4[2] = 0x42C375C3;
          local_DC = local_DC | script_A91E(local_D4, &local_C8, &local_BC, (int32_t)local_B0, (int32_t)local_A4);
          if (local_DC) {
            local_DE = local_DE + 1;
          }
          break;
        case 0:
          local_C8.x = -118.51f;
          (&local_C8)->z = 63.04f;
          local_BC.x = -118.51f;
          (&local_BC)->z = 54.18f;
          local_B0[0] = -0x3D19E666;
          local_B0[2] = 0x4258B852;
          local_A4[0] = -0x3D19E666;
          local_A4[2] = 0x427C28F6;
          local_DC = script_A91E(local_D4, &local_C8, &local_BC, (int32_t)local_B0, (int32_t)local_A4);
          local_C8.x = -118.51f;
          (&local_C8)->z = 63.04f;
          local_BC.x = -118.51f;
          (&local_BC)->z = 61.39f;
          local_B0[0] = -0x3D21147B;
          local_B0[2] = 0x42758F5C;
          local_A4[0] = -0x3D21147B;
          local_A4[2] = 0x427C28F6;
          local_DC = local_DC | script_A91E(local_D4, &local_C8, &local_BC, (int32_t)local_B0, (int32_t)local_A4);
          if (local_DC) {
            local_DE = local_DE + 1;
          }
          break;
        case 2:
          if (local_DF < 10) {
            local_C8.x = 20.45f;
            (&local_C8)->z = 148.74f;
            local_BC.x = 15.39f;
            (&local_BC)->z = 153.88f;
            local_B0[0] = 0x414CA3D7;
            local_B0[2] = 0x431747AE;
            local_A4[0] = 0x418F1EB8;
            local_A4[2] = 0x431247AE;
            local_DC = script_A91E(local_D4, &local_C8, &local_BC, (int32_t)local_B0, (int32_t)local_A4);
            local_DE = local_DE + local_DC;
          } else {
            local_C8.x = -118.51f;
            (&local_C8)->z = 63.04f;
            local_BC.x = -118.51f;
            (&local_BC)->z = 54.18f;
            local_B0[0] = -0x3D19E666;
            local_B0[2] = 0x4258B852;
            local_A4[0] = -0x3D19E666;
            local_A4[2] = 0x427C28F6;
            local_DC = script_A91E(local_D4, &local_C8, &local_BC, (int32_t)local_B0, (int32_t)local_A4);
            local_C8.x = -118.51f;
            (&local_C8)->z = 63.04f;
            local_BC.x = -118.51f;
            (&local_BC)->z = 61.39f;
            local_B0[0] = -0x3D21147B;
            local_B0[2] = 0x42758F5C;
            local_A4[0] = -0x3D21147B;
            local_A4[2] = 0x427C28F6;
            local_DC = local_DC | script_A91E(local_D4, &local_C8, &local_BC, (int32_t)local_B0, (int32_t)local_A4);
            if (local_DC) {
              local_DD = local_DD + 1;
            }
          }
          break;
      }
      if (local_DC & -(Script_ForkliftUtils(0x4B524F46, FORK_CMD_GET_ATTACHED_LOAD_ID) != local_D4)) {
        Script_SetCharaFlags(local_D4, 2);
      } else {
        Script_SetCharaFlags(local_D4, 9);
      }
      local_DF = local_DF + 1;
    }
    if (task_2C4 == 2) {
      task_2C7 = local_DE;
      local_DE = local_DE + local_DD;
    }
    task_2C6 = local_DE;
    if (((&local_F4)->hours >= 17) & (task_2C8 == 0)) {
      int32_t local_A0;
      Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
      Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
      Script_sicnUtils(1, 0, 0);
      Script_shenClearFreeState(0);
      if ((&local_F4)->hours * 100 + (&local_F4)->minutes < 1730) {
        script_AD9C();
      } else {
        Script_GetConversationDetails(CONV_DETAILS_MODE_CLOSE_CONVERSATION_UI, 0);
        Script_GetConversationDetails(CONV_DETAILS_MODE_SET_SHARED_ITEM_PREVIEW, -1);
      }
      script_C576(arg0, 1);
      script_A9C0(arg0);
      Script_ForkliftUtils(0x4B524F46, FORK_CMD_HIDE_HELPER_ACTORS);
      SCNF_WaitFrames_0000(30);
      if (local_DB == 0) {
        script_C822();
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      } else {
        script_C92A();
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      }
      SCNF_WaitFrames_0000(60);
      local_DE = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20);
      local_DE = local_DE + 1;
      Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, local_DE);
      script_C3D7();
      local_A0 = task_2C3;
      if (local_A0 == 2) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PARTTIME_JOB_WAKE_ROUTE_COMPLETE)) {
          task_2C3 = task_2C3 + 1;
          Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, 0);
        }
        goto L00FCA4;
      }
      L00FCA4:;
      script_B04E();
      Script_ForkliftUtils(0x4B524F46, FORK_CMD_SNAPSHOT_FOLLOWER_LOADS);
      break;
    }
    if (-((&local_F4)->hours >= 12) & -((&local_F4)->hours < 13) & -(task_2C8 == 0)) {
      Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
      Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
      Script_sicnUtils(1, 0, 0);
      Script_shenClearFreeState(0);
      script_AD9C();
      script_C576(arg0, 1);
      script_A9C0(arg0);
      Script_ForkliftUtils(0x4B524F46, FORK_CMD_HIDE_HELPER_ACTORS);
      script_C816();
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(30);
      Script_ForkliftUtils(0x4B524F46, FORK_CMD_SNAPSHOT_FOLLOWER_LOADS);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_2C0 = 3;
}

static void script_FD64(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  Script_ForkliftUtils(FLMW_MODE_SET_GLOBAL_FLAG);
  Script_flmwSetInputMask(SHEN_NAME('FORK'), task_2B0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  task_2C0 = 0;
  task_2C1 = 0;
  Script_SetAnimatedSequenceCollisionCheckDisabled(1);
  task_2AC = Script_scenCreateTask(script_F72A, 2, CHARA_AKIR, 0x4B52414D);
  Script_SetCPAMFlags(CHARA_MARK, 0);
  Script_charaDisplay(SHEN_NAME('FORK'), CDM_Show);
  Script_ccowUtils(SHEN_NAME('FORK'), CCOW_EnableAll);
  local_EC = task_2C3;
  switch (local_EC) {
    case 2:
      script_AFA0(0x555A5953);
      script_AFA0(0x5F454F4A);
      script_AFA0(0x4F48494D);
      script_A9CE(CHARA_MARK, -106.44f, -5.43f, 25376);
      script_A9CE(SHEN_NAME('FORK'), -117.68f, -13.09f, 51973);
      script_A9CE(CHARA_AKIR, -117.68f, -13.09f, 51973);
      script_AA05(SHEN_NAME('BOX1'), -101.43f, 0.001f, -8.51f, 0x6000);
      script_AA05(SHEN_NAME('BOX2'), -102.87f, 0.001f, -10.1f, 0x6000);
      script_AA05(SHEN_NAME('BOX3'), -104.05f, 0.001f, -11.63f, 0xE000);
      script_AA05(SHEN_NAME('BOX4'), -102.85f, 0.001f, -6.95f, 0x6000);
      script_AA05(SHEN_NAME('BOX5'), -104.25f, 0.001f, -8.45f, 0x6000);
      script_AA05(SHEN_NAME('BOX6'), -105.65f, 0.001f, -9.95f, 24758);
      script_AA05(SHEN_NAME('BOX7'), -105.85f, 0.001f, -13.15f, 38229);
      script_AA05(SHEN_NAME('BOX8'), -107.45f, 0.001f, -11.75f, 22755);
      script_AA05(SHEN_NAME('BOX9'), -109.2f, 0.001f, -15.52f, 33678);
      script_AA05(SHEN_NAME('BOXA'), -108.85f, 0.001f, -13.55f, 36408);
      script_AA05(SHEN_NAME('BOXB'), -101.43f, 1.5f, -8.51f, 0x6000);
      script_AA05(SHEN_NAME('BOXC'), -102.87f, 1.5f, -10.1f, 0x6000);
      script_AA05(SHEN_NAME('BOXD'), -104.05f, 1.5f, -11.63f, 0xE000);
      script_AA05(SHEN_NAME('BOXE'), -102.85f, 1.5f, -6.95f, 0x6000);
      script_AA05(SHEN_NAME('BOXF'), -104.25f, 1.5f, -8.45f, 0x6000);
      script_AA05(SHEN_NAME('BOXG'), -105.65f, 1.5f, -9.95f, 24758);
      script_AA05(SHEN_NAME('BOXH'), -105.85f, 1.5f, -13.15f, 38229);
      script_AA05(SHEN_NAME('BOXI'), -107.45f, 1.5f, -11.75f, 22755);
      script_AA05(SHEN_NAME('BOXJ'), -109.2f, 1.5f, -15.52f, 33678);
      script_AA05(SHEN_NAME('BOXK'), -108.85f, 1.5f, -13.55f, 36408);
      break;
    case 3:
      script_AFA0(0x4F524F47);
      script_AFA0(0x4B535448);
      script_AA05(SHEN_NAME('BOX1'), -84.55f, 0.001f, 73.474998f, 0x2000);
      script_AA05(SHEN_NAME('BOX2'), -86.1f, 0.001f, 75.0f, 0x2000);
      script_AA05(SHEN_NAME('BOX3'), -87.55f, 0.001f, 76.574997f, 0x2000);
      script_AA05(SHEN_NAME('BOX4'), -89.025002f, 0.001f, 78.05f, 0x2000);
      script_AA05(SHEN_NAME('BOX5'), -90.574997f, 0.001f, 79.525002f, 0x2000);
      script_AA05(SHEN_NAME('BOX6'), -86.224998f, 0.001f, 71.85f, 0x2000);
      script_AA05(SHEN_NAME('BOX7'), -87.675003f, 0.001f, 73.45f, 0x2000);
      script_AA05(SHEN_NAME('BOX8'), -89.125f, 0.001f, 75.0f, 0x2000);
      script_AA05(SHEN_NAME('BOX9'), -90.6f, 0.001f, 76.5f, 0x2000);
      script_AA05(SHEN_NAME('BOXA'), -92.05f, 0.001f, 78.074997f, 0x2000);
      script_AA05(SHEN_NAME('BOXB'), -84.55f, 1.5f, 73.474998f, 0x2000);
      script_AA05(SHEN_NAME('BOXC'), -86.1f, 1.5f, 75.0f, 0x2000);
      script_AA05(SHEN_NAME('BOXD'), -87.55f, 1.5f, 76.574997f, 0x2000);
      script_AA05(SHEN_NAME('BOXE'), -89.025002f, 1.5f, 78.05f, 0x2000);
      script_AA05(SHEN_NAME('BOXF'), -90.574997f, 1.5f, 79.525002f, 0x2000);
      script_AA05(SHEN_NAME('BOXG'), -86.224998f, 1.5f, 71.85f, 0x2000);
      script_AA05(SHEN_NAME('BOXH'), -87.675003f, 1.5f, 73.45f, 0x2000);
      script_AA05(SHEN_NAME('BOXI'), -89.125f, 1.5f, 75.0f, 0x2000);
      script_AA05(SHEN_NAME('BOXJ'), -90.6f, 1.5f, 76.5f, 0x2000);
      script_AA05(SHEN_NAME('BOXK'), -92.05f, 1.5f, 78.074997f, 0x2000);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_MAD_ANGELS_BEHIND_WAREHOUSE_17)) {
        if (Script_ForkliftUtils(0x4B524F46, FORK_CMD_RESTORE_LOAD_STATE)) {
          script_A9CE(SHEN_NAME('FORK'), 19.16f, 104.84f, 8374);
        } else {
          script_A9CE(CHARA_MARK, -92.15f, 70.15f, 3123);
          script_A9CE(SHEN_NAME('FORK'), -92.1f, 65.99f, 40049);
          script_A9CE(CHARA_AKIR, -92.1f, 65.99f, 40049);
        }
      } else {
        script_A9CE(CHARA_MARK, -92.15f, 70.15f, 3123);
        script_A9CE(SHEN_NAME('FORK'), -92.1f, 65.99f, 40049);
        script_A9CE(CHARA_AKIR, -92.1f, 65.99f, 40049);
      }
      break;
    case 4:
      script_AFA0(0x33344143);
      script_AFA0(0x4B474B53);
      script_AFA0(0x59485359);
      script_AFA0(0x41534B54);
      script_A9CE(CHARA_MARK, -5.54f, 143.32f, 39776);
      script_A9CE(SHEN_NAME('FORK'), -7.95f, 144.93f, 7918);
      script_A9CE(CHARA_AKIR, -7.95f, 144.93f, 7918);
      script_AA05(SHEN_NAME('BOX1'), -15.5f, 0.001f, 136.95f, 0x6000);
      script_AA05(SHEN_NAME('BOX2'), -13.97f, 0.001f, 135.6f, 0x6000);
      script_AA05(SHEN_NAME('BOX3'), -12.55f, 0.001f, 134.14f, 0x6000);
      script_AA05(SHEN_NAME('BOX4'), -11.15f, 0.001f, 132.74f, 0x6000);
      script_AA05(SHEN_NAME('BOX5'), -13.85f, 0.001f, 138.54f, 0x6000);
      script_AA05(SHEN_NAME('BOX6'), -12.45f, 0.001f, 137.04f, 0x6000);
      script_AA05(SHEN_NAME('BOX7'), -11.15f, 0.001f, 135.64f, 0x6000);
      script_AA05(SHEN_NAME('BOX8'), -9.75f, 0.001f, 134.24f, 0x6000);
      script_AA05(SHEN_NAME('BOX9'), -12.5f, 0.001f, 139.95f, 0x6000);
      script_AA05(SHEN_NAME('BOXA'), -11.05f, 0.001f, 138.54f, 0x6000);
      script_AA05(SHEN_NAME('BOXB'), -15.5f, 1.5f, 136.95f, 0x6000);
      script_AA05(SHEN_NAME('BOXC'), -13.97f, 1.5f, 135.6f, 0x6000);
      script_AA05(SHEN_NAME('BOXD'), -12.55f, 1.5f, 134.14f, 0x6000);
      script_AA05(SHEN_NAME('BOXE'), -11.15f, 1.5f, 132.74f, 0x6000);
      script_AA05(SHEN_NAME('BOXF'), -13.85f, 1.5f, 138.54f, 0x6000);
      script_AA05(SHEN_NAME('BOXG'), -12.45f, 1.5f, 137.04f, 0x6000);
      script_AA05(SHEN_NAME('BOXH'), -11.15f, 1.5f, 135.64f, 0x6000);
      script_AA05(SHEN_NAME('BOXI'), -9.75f, 1.5f, 134.24f, 0x6000);
      script_AA05(SHEN_NAME('BOXJ'), -12.5f, 1.5f, 139.95f, 0x6000);
      script_AA05(SHEN_NAME('BOXK'), -11.05f, 1.5f, 138.54f, 0x6000);
      break;
    case 5:
      script_AFA0(0x4542454B);
      script_AFA0(0x4E524F4D);
      script_AFA0(0x49454448);
      script_AFA0(0x49594554);
      script_AFA0(0x484D4F54);
      script_AFA0(0x494B4D4E);
      script_AFA0(0x31444C46);
      script_AFA0(0x32444C46);
      script_AFA0(0x49594159);
      script_AFA0(0x32344143);
      script_AA05(SHEN_NAME('BOX1'), 101.275002f, 0.001f, 37.95f, 0);
      script_AA05(SHEN_NAME('BOX2'), 103.55f, 0.001f, 37.95f, 0);
      script_AA05(SHEN_NAME('BOX3'), 105.724998f, 0.001f, 37.95f, 0);
      script_AA05(SHEN_NAME('BOX4'), 107.9f, 0.001f, 37.924999f, 0);
      script_AA05(SHEN_NAME('BOX5'), 109.875f, 0.001f, 40.112499f, 0);
      script_AA05(SHEN_NAME('BOX6'), 101.175003f, 0.001f, 35.912498f, 0);
      script_AA05(SHEN_NAME('BOX7'), 103.55f, 0.001f, 35.75f, 0);
      script_AA05(SHEN_NAME('BOX8'), 105.724998f, 0.001f, 35.6875f, 0);
      script_AA05(SHEN_NAME('BOX9'), 107.8f, 0.001f, 35.75f, 0);
      script_AA05(SHEN_NAME('BOXA'), 109.95f, 0.001f, 37.8f, 0);
      script_AA05(SHEN_NAME('BOXB'), 101.275002f, 1.5f, 37.95f, 0);
      script_AA05(SHEN_NAME('BOXC'), 103.55f, 1.5f, 37.95f, 0);
      script_AA05(SHEN_NAME('BOXD'), 105.724998f, 1.5f, 37.95f, 0);
      script_AA05(SHEN_NAME('BOXE'), 107.9f, 1.5f, 37.924999f, 0);
      script_AA05(SHEN_NAME('BOXF'), 109.875f, 1.5f, 40.112499f, 0);
      script_AA05(SHEN_NAME('BOXG'), 101.175003f, 1.5f, 35.912498f, 0);
      script_AA05(SHEN_NAME('BOXH'), 103.55f, 1.5f, 35.75f, 0);
      script_AA05(SHEN_NAME('BOXI'), 105.724998f, 1.5f, 35.6875f, 0);
      script_AA05(SHEN_NAME('BOXJ'), 107.8f, 1.5f, 35.75f, 0);
      script_AA05(SHEN_NAME('BOXK'), 109.95f, 1.5f, 37.8f, 0);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HARBOR_FOREIGNERS_INQUIRY)) {
        if (Script_ForkliftUtils(0x4B524F46, FORK_CMD_RESTORE_LOAD_STATE) == 0) {
          script_A9CE(CHARA_MARK, 102.26f, 31.19f, 2639);
          script_A9CE(SHEN_NAME('FORK'), 101.86f, 26.26f, 35862);
          script_A9CE(CHARA_AKIR, 101.86f, 26.26f, 35862);
        }
      } else {
        script_A9CE(CHARA_MARK, 102.26f, 31.19f, 2639);
        script_A9CE(SHEN_NAME('FORK'), 101.86f, 26.26f, 35862);
        script_A9CE(CHARA_AKIR, 101.86f, 26.26f, 35862);
      }
      break;
    case 6:
      script_AFA0(0x36444C46);
      script_AFA0(0x41444C46);
      script_AFA0(0x41524D54);
      script_AFA0(0x4F4B5952);
      script_AFA0(0x41445349);
      script_AFA0(0x4152554D);
      script_AFA0(0x41474E53);
      script_AFA0(0x4954494D);
      script_AFA0(0x4154534D);
      script_AFA0(0x555A5953);
      script_AFA0(0x5F454F4A);
      script_AFA0(0x4F48494D);
      script_A9CE(CHARA_MARK, -111.0f, -10.09f, 50972);
      script_A9CE(SHEN_NAME('FORK'), -117.68f, -13.09f, 51973);
      script_A9CE(CHARA_AKIR, -117.68f, -13.09f, 51973);
      script_AA05(SHEN_NAME('BOX1'), -101.375f, 0.001f, -8.5f, 0x6000);
      script_AA05(SHEN_NAME('BOX2'), -102.75f, 0.001f, -10.0938f, 0x6000);
      script_AA05(SHEN_NAME('BOX3'), -104.25f, 0.001f, -11.5375f, 0xE000);
      script_AA05(SHEN_NAME('BOX4'), -102.75f, 0.001f, -6.9453f, 0x6000);
      script_AA05(SHEN_NAME('BOX5'), -104.25f, 0.001f, -8.4375f, 0x6000);
      script_AA05(SHEN_NAME('BOX6'), -105.625f, 0.001f, -9.9375f, 24758);
      script_AA05(SHEN_NAME('BOX7'), -101.375f, 1.5f, -8.5f, 0x6000);
      script_AA05(SHEN_NAME('BOX8'), -102.75f, 1.5f, -10.0938f, 0x6000);
      script_AA05(SHEN_NAME('BOX9'), -104.25f, 1.5f, -11.5375f, 0xE000);
      script_AA05(SHEN_NAME('BOXA'), -102.75f, 1.5f, -6.9453f, 0x6000);
      script_AA05(SHEN_NAME('BOXB'), 18.65f, 0.001f, 136.55f, 0x2000);
      script_AA05(SHEN_NAME('BOXC'), 16.95f, 0.001f, 138.25f, 0x2000);
      script_AA05(SHEN_NAME('BOXD'), 20.137501f, 0.001f, 137.8f, 0x2000);
      script_AA05(SHEN_NAME('BOXE'), 18.35f, 0.001f, 139.75f, 0x2000);
      script_AA05(SHEN_NAME('BOXF'), 19.95f, 0.001f, 141.05f, 0x2000);
      script_AA05(SHEN_NAME('BOXG'), 21.45f, 0.001f, 139.45f, 0x2000);
      script_AA05(SHEN_NAME('BOXH'), 18.65f, 1.5f, 136.55f, 0x2000);
      script_AA05(SHEN_NAME('BOXI'), 16.95f, 1.5f, 138.25f, 0x2000);
      script_AA05(SHEN_NAME('BOXJ'), 20.137501f, 1.5f, 137.8f, 0x2000);
      script_AA05(SHEN_NAME('BOXK'), 21.45f, 1.5f, 139.45f, 0x2000);
      break;
  }
  local_F0 = 0;
  while (local_F0 < 20) {
    Script_charaVisUtils(Script_ScenDataUtil(&data_2A124, SCND_ReadU32, local_F0), 1);
    local_F0 = local_F0 + 1;
  }
  Script_charaVisUtils(CHARA_AKIR, 1);
  Script_charaVisUtils(SHEN_NAME('FORK'), 1);
  Script_charaVisUtils(SHEN_NAME('ZE05'), 1);
  Script_flmwSetFollowerLoadIdList(SHEN_NAME('FORK'), 0x31584F42, 0x32584F42, 0x33584F42, 0x34584F42, 0x35584F42, 0x36584F42, 0x37584F42, 0x38584F42, 0x39584F42, 0x41584F42, 0x42584F42, 0x43584F42, 0x44584F42, 0x45584F42, 0x46584F42, 0x47584F42, 0x48584F42, 0x49584F42, 0x4A584F42, 0x4B584F42, -1);
  return;
}

__attribute__((scn_string_bindings(0, data_2ACFF, 2, data_2AD1A, 4, data_2AD30)))
static int32_t script_10E94(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ForkliftUtils(FLMW_MODE_CLEAR_GLOBAL_FLAGS);
  Script_flmwSetInputMask(SHEN_NAME('FORK'), -1);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_ClearAccumulatorIfArgumentZero(0, 1);
  SCNF_WaitFrames_0000(2);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_B002(SHEN_NAME('FORK'), SHEN_NAME('ZE05'));
  local_F4 = 0;
  while (local_F4 < 20) {
    task_20C[local_F4][0] = 0;
    local_F4 = local_F4 + 1;
  }
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/03/MFSY", (int32_t)"CYCLEMAN.BIN", 0x5953464D);
  script_AE4E(0x4B52414D);
  if (task_2C3 == 1) {
    local_F8 = Script_PlayerStatsUtils(PS_GetStatU32_1);
    local_F8 = local_F8 + 1;
    Script_PlayerStatsUtils(PS_SetStatU32_1, local_F8);
    Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  }
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  {
    int32_t unused_F0;
    GameDateTime local_E8;
    int32_t local_E4;
    Script_ReturnTimeDate(&local_E8);
    script_C544();
    if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 2)) {
      Script_mapmSetRenderFlag(1, 0);
      Script_mapmSetRenderFlag(5, 0);
      Script_mapmSetRenderFlag(3, 1);
      Script_mapmSetRenderFlag(7, 1);
    } else {
      if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
        Script_mapmSetRenderFlag(1, 0);
        Script_mapmSetRenderFlag(5, 0);
        Script_mapmSetRenderFlag(3, 1);
        Script_mapmSetRenderFlag(7, 1);
        Script_mapmSetRenderFlag(10, 1);
        goto L011008;
      }
    }
    L011008:;
    Script_handSetFloatsFor(SHEN_NAME('HNDL'), 0, data_26094, 8);
    Script_handSetFloatsFor(SHEN_NAME('HNDR'), 1, data_26094, 8);
    task_2B4 = script_B094();
    Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('TOMC'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('TOMC'), CDM_Show);
        goto L011068;
      }
    }
    L011068:;
    Script_scenToggleVisibleFor(SHEN_NAME('TOMC'), 0);
    if ((task_2C3 >= 2) & ((&local_E8)->hours < 12)) {
      local_F8 = Script_PlayerStatsUtils(PS_GetStatU32_1);
      local_F8 = local_F8 + 1;
      Script_PlayerStatsUtils(PS_SetStatU32_1, local_F8);
      script_13816();
      if (task_2C3 >= 2) {
        (&local_E8)->hours = 8;
        (&local_E8)->minutes = 45;
        (&local_E8)->seconds = 0;
        Script_SetNewDateTime(&local_E8);
        Script_mapmSetRenderFlag(15, 0);
        Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
        Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
        Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
        Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
        script_1658D();
        Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
        Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 1);
        Script_mapmSetRenderFlag(15, 1);
        Script_Ambient1SoundUtils(0, 23976);
      }
    }
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    SCNF_WaitFrames_0000(1);
    if (task_2C3 != 6) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_BOX") == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_BOX2") == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    if ((&local_E8)->hours * 60 + (&local_E8)->minutes < 870) {
      Script_ForkliftUtils(0x4B524F46, FORK_CMD_CLEAR_SAVED_LOAD_STATE);
    }
    (void)((task_2C8 = 0) & 0x10);
    Script_CycleManagerUtils(CYCLE_CMD_SET_MOVEMENT_GLOBALS, 1.5f, 0.7f, 0.877f);
    local_E4 = task_2C3;
    if (local_E4 == 1) {
      local_FC = (&local_E8)->hours * 60 + (&local_E8)->minutes + 15;
      (&local_E8)->hours = local_FC / 60;
      (&local_E8)->minutes = local_FC % 60;
      Script_SetNewDateTime(&local_E8);
      Script_ForkliftUtils(FLMW_MODE_CLEAR_FOLLOWER_SNAPSHOT);
      script_E755();
      task_2C4 = 1;
    } else if ((local_E4 == 2) | (local_E4 == 4) | (local_E4 == 5)) {
      script_FD64();
      task_2C4 = 0;
    } else {
      switch (local_E4) {
        case 3:
          script_FD64();
          task_2C4 = 1;
          break;
        case 6:
          script_FD64();
          task_2C4 = 2;
          break;
      }
    }
    Script_scenToggleVisibleFor(SHEN_NAME('DOR1'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('DOR8'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('SHT2'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('SHT3'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('SHT4'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('SH18'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('AZIT'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('JIH0'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('JIH2'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('JIH3'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('JIH4'), 0);
    if (task_2C3 != 1) {
      if ((&local_E8)->hours < 12) {
        Script_ForkliftUtils(FLMW_MODE_CLEAR_FOLLOWER_SNAPSHOT);
      } else {
        Script_ForkliftUtils(0x4B524F46, FORK_CMD_RESTORE_FOLLOWER_LOADS);
        script_C5F8();
      }
    }
  }
  script_AFA0(0x46424A47);
  script_AFA0(0x5F4D4F54);
  Script_charaDisplay(SHEN_NAME('TOMC'), CDM_Hide);
  script_AFA0(0x49594159);
  script_AFA0(0x42474F44);
  {
    int32_t unused_E8[3];
    int32_t local_E4;
    local_E4 = task_2C4;
    if (local_E4 == 1) {
      int32_t local_E0;
      script_AC68();
      local_E0 = Script_ScenDataUtil(task_0, SCND_ReadU32, 118);
      local_E0 = local_E0 | 0x20000;
      Script_ScenDataUtil(task_0, SCND_WriteU32, 118, local_E0);
      script_AFA0(0x4F4E5341);
      script_AFA0(0x414B414E);
      script_AFA0(0x43474D59);
      script_AFA0(0x41474F4B);
      script_AFA0(0x4F4B5952);
      script_AFA0(0x41574E4D);
    } else {
      if ((local_E4 == 0) | (local_E4 == 2)) {
        int32_t local_E0;
        script_ACE9();
        local_E0 = Script_ScenDataUtil(task_0, SCND_ReadU32, 92);
        local_E0 = local_E0 | 0x20000;
        Script_ScenDataUtil(task_0, SCND_WriteU32, 92, local_E0);
        goto L011472;
      }
    }
  }
  L011472:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOOD'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('FOD_'), 0);
  Script_scenToggleVisibleFor(0x31786264, 0);
  Script_scenToggleVisibleFor(0x32786264, 0);
  Script_SetShadowRange(30.0f);
  script_AE4E(0x4242454B);
  script_AE4E(0x4C42454B);
  script_AE4E(0x594A4E53);
  script_AE4E(0x4B42454B);
  script_AE4E(0x4942454B);
  script_AE4E(0x4742454B);
  script_AE4E(0x4A42454B);
  Script_CPCTRelated(arg0, 32, 89);
  Script_ccowUtils(SHEN_NAME('FORK'), CCOW_EnableAll);
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_2C5 = 0;
  task_2B0 = 0;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  task_2B8 = Script_scenCreateTask(script_BB87, 0);
  if (task_2C3 == 1) {
    Script_sicnUtils(0, 1, 1);
    script_D151(arg0);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOR2'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('FOR2'), CDM_Show);
        goto L011594;
      }
    }
    L011594:;
    Script_scenToggleVisibleFor(SHEN_NAME('FOR2'), 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Show);
        goto L0115C8;
      }
    }
    L0115C8:;
    Script_scenToggleVisibleFor(SHEN_NAME('ZE06'), 0);
    script_F406(arg0);
  }
  Script_sicnUtils(1, 0, 0);
  Script_shenClearFreeState(0);
  if (~(-(task_2C5 == 3) | -(task_2C5 == 4))) {
    Script_executeAicaCommand(2720, 1, 30);
    Script_executeAicaCommand(2720, 2, 30);
    Script_executeAicaCommand(2720, 3, 30);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_ryoSleepUtils(2, 1);
  Script_SetTaskExecutionSuspended((void *)task_2B8, 1);
  SCNF_WaitFrames_0000(1);
  script_AFD1();
  Script_forkSetSoundBankDetails(SHEN_NAME('FORK'));
  Script_SetupClothData(CHARA_AKIR, 0);
  Script_srchControl((void *)task_2AC);
  {
    int32_t unused_F0;
    GameDateTime local_E8;
    int32_t local_E4;
    Script_ReturnTimeDate(&local_E8);
    local_E4 = task_2C5;
    switch (local_E4) {
      case 0:
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 29, task_2C3);
        break;
      case 1:
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 3, 0);
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 20);
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 22, 30);
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, 0);
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 23, 53);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_PARTTIME_JOB_WAKE_ROUTE_ACTIVE, 1);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 954, 1);
        if ((&local_E8)->hours >= 13) {
          (&local_E8)->hours = 17;
        } else {
          (&local_E8)->hours = 12;
        }
        task_2C3 = task_2C3 + 1;
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 29, task_2C3);
        (&local_E8)->minutes = 0;
        (&local_E8)->seconds = 0;
        Script_SetNewDateTime(&local_E8);
        Script_timeSetUpdateEnabled(1);
        break;
      case 2:
        Script_cdttUtil(Mode6);
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('M3FB'), 0);
        SCNF_WaitFrames_0000(1);
        break;
      case 3:
        script_C3D7();
        script_B04E();
        Script_cdttUtil(8);
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('MFSY'), 41);
        SCNF_WaitFrames_0000(1);
        break;
      case 4:
        Script_cdttUtil(Mode6);
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('MB9Q'), 0);
        SCNF_WaitFrames_0000(1);
        break;
      case 5:
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB, 1);
        break;
    }
    if ((&local_E8)->hours >= 17) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 885, 1);
    }
    script_B159(task_2B4);
    Script_srchControl((void *)task_2B8);
    if ((&local_E8)->hours <= 12) {
      Script_shenRequestGameModuleTransition(3, SHEN_NAME('MFSY'), 33);
    } else {
      Script_shenRequestGameModuleTransition(3, SHEN_NAME('MFSY'), Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23));
    }
  }
  end:;
}

static void script_1184C(void) {
  Script_shenClearFreeState(0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  script_118CB();
  script_11934();
  script_11CB5();
  script_11AF1();
  script_11917();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  script_B1ED(CHARA_AKIR, SHEN_NAME('FORK'));
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_shenClearFreeState(1);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  return;
}

__attribute__((scn_string_bindings(0, data_2AD60, 1, data_2AD67, 2, data_2AD76)))
static void script_118CB(void) {
  Script_LoadVoiceFile(1, "E1001C");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "ASSA") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2AD7B, 1, data_2AD82)))
static void script_11917(void) {
  Script_LoadVoiceFile(0, "E1001C");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "ASSA");
  return;
}

static void script_11934(void) {
  int32_t local_FC;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  if (task_2C3 == 6) {
    Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  }
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOR2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOR2'), CDM_Show);
      goto L011989;
    }
  }
  L011989:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOR2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Show);
      goto L0119BD;
    }
  }
  L0119BD:;
  Script_scenToggleVisibleFor(SHEN_NAME('ZE06'), 1);
  local_FC = task_2C3;
  switch (local_FC) {
    case 2:
      task_2CC = 0x315A4341;
      break;
    case 4:
      task_2CC = 0x325A4341;
      break;
    case 3:
      task_2CC = 0x335A4341;
      break;
    case 5:
      task_2CC = 0x345A4341;
      break;
    case 6:
      task_2CC = 0x355A4341;
      break;
  }
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  Script_charaDisplay(task_2CC, CDM_Show);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ccowUtils(CHARA_MARK, CCOW_DisableAll);
  Script_ccowUtils(SHEN_NAME('FORK'), CCOW_DisableAll);
  Script_ccowUtils(task_2CC, CCOW_DisableAll);
  Script_ccowUtils(SHEN_NAME('FOR2'), CCOW_DisableAll);
  Script_MoveObjectToObject(SHEN_NAME('ZE06'), 0x32524F46);
  Script_charaToggleMotEnabled(CHARA_MARK, 1);
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  Script_clmdSetFlags(CHARA_AKIR, 4, 1);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 4);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 4);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_MARK, 1);
  return;
}

static void script_11AF1(void) {
  int32_t local_FC;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 205.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_EVT_SetPosition(CHARA_MARK, POS_XYZ, &data_2AD48);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ccowUtils(CHARA_MARK, CCOW_EnableAll);
  Script_ccowUtils(SHEN_NAME('FORK'), CCOW_EnableAll);
  Script_ccowUtils(task_2CC, CCOW_EnableAll);
  Script_ccowUtils(SHEN_NAME('FOR2'), CCOW_EnableAll);
  local_FC = task_2C3;
  switch (local_FC) {
    case 2:
      script_A9CE(SHEN_NAME('FORK'), -117.68f, -13.09f, 51973);
      break;
    case 4:
      script_A9CE(SHEN_NAME('FORK'), -7.95f, 144.93f, 7918);
      break;
    case 3:
      script_A9CE(SHEN_NAME('FORK'), -92.1f, 65.99f, 40049);
      break;
    case 5:
      script_A9CE(SHEN_NAME('FORK'), 101.86f, 26.26f, 35862);
      break;
    case 6:
      script_A9CE(SHEN_NAME('FORK'), -117.68f, -13.09f, 51973);
      break;
  }
  Script_MoveObjectToObject(SHEN_NAME('ZE06'), -1);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FOR2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FOR2'), CDM_Show);
      goto L011C4C;
    }
  }
  L011C4C:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOR2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Show);
      goto L011C80;
    }
  }
  L011C80:;
  Script_scenToggleVisibleFor(SHEN_NAME('ZE06'), 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_MARK, 2);
  return;
}

static void script_11CB5(void) {
  Script_charaDisplay(task_2CC, CDM_Show);
  Script_MoveObjectToObject(CHARA_MARK, 0x52494B41);
  Script_MoveObjectToObject(SHEN_NAME('FORK'), 0x52494B41);
  Script_MoveObjectToObject(task_2CC, 0x52494B41);
  Script_MoveObjectToObject(SHEN_NAME('FOR2'), 0x52494B41);
  Script_MoveObjectToObject(SHEN_NAME('ADUM'), 0x52494B41);
  Script_EVT_SetPosition(CHARA_MARK, POS_XYZ, &data_2AD88);
  {
    int32_t unused_FC;
    Vector3i local_F0;
    (&local_F0)->y = 0x4000;
    Script_EVT_SetRotation(CHARA_MARK, MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_EVT_SetPosition(SHEN_NAME('FORK'), POS_XYZ, &data_2AD94);
  Script_EVT_SetRotation(SHEN_NAME('FORK'), 0x38000000, &data_2AD54, 0);
  Script_EVT_SetPosition(SHEN_NAME('FOR2'), POS_XYZ, &data_2ADA0);
  {
    int32_t unused_FC;
    Vector3i local_F0;
    (&local_F0)->y = 33496;
    Script_EVT_SetRotation(SHEN_NAME('FOR2'), MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_EVT_SetPosition(SHEN_NAME('ADUM'), POS_XYZ, &data_2ADAC);
  Script_EVT_SetRotation(SHEN_NAME('ADUM'), 0x38000000, &data_2AD54, 0);
  Script_MoveObjectToObject(CHARA_MARK, -1);
  Script_MoveObjectToObject(SHEN_NAME('FORK'), -1);
  Script_MoveObjectToObject(task_2CC, -1);
  Script_MoveObjectToObject(SHEN_NAME('FOR2'), -1);
  Script_MoveObjectToObject(SHEN_NAME('ADUM'), -1);
  script_11EC4();
  return;
}

static int32_t script_11DEF(void) {
  int32_t local_FC;
  local_FC = task_2C3;
  switch (local_FC) {
    case 2:
      Script_seqEnterEvtCam(3010, CHARA_AKIR, CHARA_AKIR);
      break;
    case 4:
      Script_seqEnterEvtCam(3020, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      break;
    case 3:
      Script_seqEnterEvtCam(3030, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      break;
    case 5:
      Script_seqEnterEvtCam(3040, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      SCNF_WaitFrames_0000(160);
      Script_seqEnterEvtCam(3041, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      break;
    case 6:
      Script_seqEnterEvtCam(3050, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      break;
  }
  return Script_srchControl(0);
}

static int32_t script_11EAA(void) {
  Script_seqEnterEvtCam(3014, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
  return Script_srchControl(0);
}

static void script_11EC4(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  (void)((task_2D0 = 0) & 1);
  (void)((task_2D4 = -1) & 1);
  local_F4 = task_2C3;
  switch (local_F4) {
    case 2:
      local_FC = 1;
      break;
    case 4:
      local_FC = 3;
      break;
    case 3:
      local_FC = 5;
      break;
    case 5:
      local_FC = 7;
      break;
    case 6:
      local_FC = 9;
      break;
  }
  script_12138();
  script_1256B();
  Script_scenCreateTask(script_11DEF, 0);
  Script_scenCreateTask(script_126C3, 1, local_FC);
  local_FC = local_FC + 1;
  SCNF_WaitFrames_0000(1);
  script_12604();
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_12313, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (task_2D4 == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_126C3, 1, local_FC);
  Script_fixoClearAttachment(task_2CC);
  script_1259B();
  Script_PlayCharaMotion(CHARA_AKIR, 4098, 180.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_MARK, 4100, 180.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_SetCharaMotionPlaybackRate(CHARA_MARK, 1.0f);
  Script_scenCreateTask(script_12347, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (task_2D4 == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 4097, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  local_F8 = Script_scenCreateTask(script_123AD, 0);
  SCNF_WaitFrames_0000(90);
  Script_scenCreateTask(script_11EAA, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4099, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(20);
  (void)((task_2D0 = -1) & 0x10);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 62.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(task_2CC);
  Script_charaDisplay(task_2CC, CDM_Hide);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 100.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_F8);
  Script_forkSetSoundBankDetails(SHEN_NAME('FOR2'));
  return;
}

static void script_12138(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = task_2C3;
  switch (local_F8) {
    case 2:
      Script_PlayCharaMotion(CHARA_AKIR, 4098, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_PlayCharaMotion(CHARA_MARK, 4100, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.616f);
      Script_SetCharaMotionPlaybackRate(CHARA_MARK, 0.616f);
      break;
    case 4:
      local_FC = 0x42920000;
      Script_PlayCharaMotion(CHARA_AKIR, 4098, local_FC, -1.0f, -1.0f, 1.0f, 0);
      Script_PlayCharaMotion(CHARA_MARK, 4100, local_FC, -1.0f, -1.0f, 1.0f, 0);
      break;
    case 3:
      local_FC = 0x42A40000;
      Script_PlayCharaMotion(CHARA_AKIR, 4098, local_FC, -1.0f, -1.0f, 1.0f, 0);
      Script_PlayCharaMotion(CHARA_MARK, 4100, local_FC, -1.0f, -1.0f, 1.0f, 0);
      break;
    case 5:
      local_FC = 0x42C80000;
      Script_PlayCharaMotion(CHARA_AKIR, 4098, local_FC, -1.0f, -1.0f, 1.0f, 0);
      Script_PlayCharaMotion(CHARA_MARK, 4100, local_FC, -1.0f, -1.0f, 1.0f, 0);
      break;
    case 6:
      Script_PlayCharaMotion(CHARA_AKIR, 4098, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_PlayCharaMotion(CHARA_MARK, 4100, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.319f);
      Script_SetCharaMotionPlaybackRate(CHARA_MARK, 0.319f);
      break;
  }
  return;
}

static int32_t script_12313(void) {
  Script_filmCheckOrSetTransitionUtil(16, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_12347(void) {
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 270.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(task_2CC);
  script_1266D();
  Script_SetCharaLookAtPosition(CHARA_AKIR, -2, 0, 0);
  SCNF_WaitFrames_0000(10);
  Script_CharHandlers(CHARA_MARK, 0, 8, 16);
  return Script_srchControl(0);
}

static int32_t script_123AD(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_MoveObjectToObject(CHARA_MARK, 0x32524F46);
  local_F0 = 0;
  for (;;) {
    Vector3 local_E0;
    local_F0 = local_F0 + 1;
    if (local_F0 == 1) {
      Script_PlayCharaMotion(CHARA_MARK, MN_A_WALK_S_R_01, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(CHARA_MARK, 0.5f);
    }
    local_E0.x = -0.02f;
    (&local_E0)->z = 0.02f;
    Script_EVT_SetPosition(CHARA_MARK, -0x58000000, &local_E0);
    SCNF_WaitFrames_0000(1);
    if (local_F0 > 30) {
      break;
    }
  }
  Script_MoveObjectToObject(CHARA_MARK, -1);
  Script_MoveObjectToObject(CHARA_MARK, 0x32524F46);
  Script_EVT_SetPosition(CHARA_MARK, POS_XYZ, &data_2ADB8);
  {
    Vector3i local_E0;
    (&local_E0)->y = (int32_t)&data_2AD54;
    Script_EVT_SetRotation(CHARA_MARK, MATRIX_XFORM_Y, &local_E0, 0);
  }
  Script_MoveObjectToObject(CHARA_MARK, -1);
  Script_flmwTriggerInteraction(SHEN_NAME('FOR2'), CHARA_MARK);
  Script_PlayCharaMotion(CHARA_MARK, 4098, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_MARK, 0.0f);
  Script_EVT_GetPosition(SHEN_NAME('ADUM'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  for (;;) {
    if (task_2D0 & 0x40) {
      (void)((task_2D0 = 0) & 0x10);
      local_EC = script_C58E(0.06f);
      if (Script_forkAttachLoadActor(SHEN_NAME('FOR2'), (const float *)&local_F4, 0.06f, 0, 0)) {
        while (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2'))) {
          SCNF_WaitFrames_0000(1);
        }
      }
      for (;;) {
        SCNF_WaitFrames_0000(1);
        if (Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FOR2')) == 0) {
          break;
        }
      }
      script_C5E3(local_EC);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1256B(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_MARK, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 1.3f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 20, &local_F4, 0);
  return;
}

static void script_1259B(void) {
  Script_fixoSetAttachmentOffset(task_2CC, 0, &data_2AD48, &data_2AE04);
  Script_ReadModelFormat7(CHARA_MARK, 0, data_2ADDC, 0);
  Script_motmGetCharaFlags(CHARA_MARK, MOTM_PoseNone, SCEN_Write);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 0x40);
  Script_fixoAttachEx(task_2CC, CHARA_MARK, 18, &data_2ADC4, &data_2ADD0);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Enable, 0x40);
  return;
}

static void script_12604(void) {
  Script_fixoSetAttachmentOffset(task_2CC, 0, &data_2AD48, &data_2AE5C);
  Script_ReadModelFormat7(CHARA_MARK, 0, data_2AE28, 0);
  Script_motmGetCharaFlags(CHARA_MARK, MOTM_PoseNone, SCEN_Write);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 0x40);
  Script_fixoAttachEx(task_2CC, CHARA_MARK, 18, &data_2AE10, &data_2AE1C);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Enable, 0x40);
  return;
}

static void script_1266D(void) {
  Script_fixoSetAttachmentOffset(task_2CC, 0, &data_2AD48, &data_2AE80);
  Script_motmGetCharaFlags(CHARA_AKIR, MOTM_PoseNone, SCEN_Write);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_fixoAttachEx(task_2CC, CHARA_AKIR, 18, &data_2AE68, &data_2AE74);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  return;
}

__attribute__((scn_string_bindings(9, data_2AEEF, 10, data_2AEFA, 13, data_2AF1B, 15, data_2AF31, 16, data_2AF3C, 19, data_2AF5D, 21, data_2AF73, 22, data_2AF7E, 25, data_2AF9F, 27, data_2AFB5, 28, data_2AFC0, 30, data_2AFD6)))
static int32_t script_126C3(int32_t arg0) {
  int32_t local_FC;
  (void)((task_2D4 = 0) & 1);
  local_FC = arg0;
  switch (local_FC) {
    case 1:
      Script_ShowUIMessageFromID("E1001CD026");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CA005");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CD002");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CA002");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 2:
      Script_seqEnterEvtCam(3013, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      Script_ShowUIMessageFromID("E1001CD003");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CD004");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_seqEnterEvtCam(3011, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      Script_ShowUIMessageFromID("E1001CD005");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CA003");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_seqEnterEvtCam(3002, CHARA_AKIR, CHARA_AKIR);
      Script_ShowUIMessageFromID("E1001CD007");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 3:
      Script_ShowUIMessageFromID("E1001CD016");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CA016");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 4:
      Script_ShowUIMessageFromID("E1001CD009");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_seqEnterEvtCam(3021, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      Script_ShowUIMessageFromID("E1001CD010");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CA008");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_seqEnterEvtCam(3022, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      Script_ShowUIMessageFromID("E1001CD011");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 5:
      Script_ShowUIMessageFromID("E1001CD016");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CA016");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 6:
      Script_ShowUIMessageFromID("E1001CD013");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CD014");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_seqEnterEvtCam(3031, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      Script_ShowUIMessageFromID("E1001CA008");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CD015");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 7:
      Script_ShowUIMessageFromID("E1001CD016");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CA012");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 8:
      Script_ShowUIMessageFromID("E1001CD017");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CD018");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_seqEnterEvtCam(3042, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      Script_ShowUIMessageFromID("E1001CA008");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CD019");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 9:
      Script_ShowUIMessageFromID("E1001CD016");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CA012");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CD020");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CA012");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CD021");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CD022");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 10:
      Script_ShowUIMessageFromID("E1001CD023");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_seqEnterEvtCam(3051, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      Script_ShowUIMessageFromID("E1001CA013");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1001CD024");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_seqEnterEvtCam(3052, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      Script_ShowUIMessageFromID("E1001CD025");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_seqEnterEvtCam(3053, SHEN_NAME('FORK'), SHEN_NAME('FORK'));
      Script_ShowUIMessageFromID("E1001CA014");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  (void)((task_2D4 = -1) & 1);
  return Script_srchControl(0);
}

static void script_12C7C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_0290();
  script_13238();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_2B030, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_2B03C, CHARA_PART_Set);
  local_FC = 0;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 0:
        Script_executeAicaCommand(0x1B04A9, 0, 0);
        break;
      case 58:
        Script_scenCreateTask(script_13755, 0);
        break;
      case 146:
        Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_2B048, CHARA_PART_Set);
        Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_2B048, CHARA_PART_Set);
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        break;
      case 147:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        local_F8 = Script_scenCreateTask(script_136FF, 1, SHEN_NAME('BUS_'));
        break;
      case 272:
        goto L012DB2;
        break;
    }
    local_FC = local_FC + 1;
    script_13772();
    SCNF_WaitFrames_0000(1);
  }
  L012DB2:;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
  return;
}

static void script_12DC9(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_0290();
  script_13238();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_2B054, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_2B060, CHARA_PART_Set);
  local_FC = 0;
  local_F8 = Script_scenCreateTask(script_136FF, 1, SHEN_NAME('BUS_'));
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 0:
        Script_executeAicaCommand(0x1B04A9, 0, 0);
        break;
      case 58:
        Script_scenCreateTask(script_13755, 0);
        break;
      case 146:
        Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_2B06C, CHARA_PART_Set);
        Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_2B06C, CHARA_PART_Set);
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        break;
      case 147:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        local_F8 = Script_scenCreateTask(script_136FF, 1, SHEN_NAME('BUS_'));
        break;
      case 272:
        goto L012F14;
        break;
    }
    script_13772();
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L012F14:;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
  return;
}

static void script_12F2B(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_0290();
  script_13238();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &data_2B078, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &data_2B084, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &data_2B090, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &data_2B09C, CHARA_PART_Set);
  local_FC = 0;
  Script_executeAicaCommand(0x1B04A9, 0, 0);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 145:
        Script_scenCreateTask(script_13472, 1, 2);
        break;
      case 200:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        local_F8 = Script_scenCreateTask(script_136FF, 1, SHEN_NAME('BUS_'));
        break;
      case 250:
        goto L013029;
        break;
    }
    script_13772();
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L013029:;
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenSeqUtils(-6);
  script_1336B();
  return;
}

static void script_13060(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_0290();
  script_13238();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &data_2B0A8, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &data_2B0B4, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &data_2B0C0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &data_2B0CC, CHARA_PART_Set);
  local_FC = 0;
  Script_executeAicaCommand(0x1B04A9, 0, 0);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 145:
        Script_scenCreateTask(script_13472, 1, 2);
        break;
      case 200:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        local_F8 = Script_scenCreateTask(script_136FF, 1, SHEN_NAME('BUS_'));
        break;
      case 250:
        goto L01315E;
        break;
    }
    script_13772();
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L01315E:;
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenSeqUtils(-6);
  script_1336B();
  return;
}

static void script_13195(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_0290();
  script_13238();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_2B0D8, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_2B0E4, CHARA_PART_Set);
  local_FC = 0;
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_executeAicaCommand(0x1B04A9, 0, 0);
        break;
      case 250:
        goto L01322A;
        break;
    }
    script_13772();
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L01322A:;
  Script_executeAicaCommand(0x1C04A9, 0, 0);
  return;
}

static void script_13238(void) {
  Vector3i local_F4;
  Vector3 local_E8;
  int32_t unused_D8[4];
  GameDateTime local_D0;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2B0F0);
  Script_scenCreateTask(script_13772, 0);
  script_02A8(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  local_F4.x = 0;
  (&local_F4)->y = 0;
  (&local_F4)->z = 0;
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &local_F4, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &local_F4, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &local_F4, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &local_F4, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &local_F4, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &local_F4, CHARA_PART_Set);
  Script_SetMaxNPCsNear(32);
  Script_mapmSetCamFlags(0, 0);
  Script_ReturnTimeDate(&local_D0);
  if ((&local_D0)->hours < 19) {
    local_E8.x = 0.0f;
    (&local_E8)->y = -100.0f;
    (&local_E8)->z = 0.0f;
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_E8, 0, CHARA_PART_Set);
  }
  return;
}

static void script_1336B(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3i local_DC;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, CHARA_PART_Set);
  script_0335(CHARA_AKIR);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 4);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->y = 0.0f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_mapmSetCamFlags(0, 1);
  Script_SetMaxNPCsNear(6);
  if (Script_shenSeqUtils(-7)) {
    local_F4.x = -123.0f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 141.0f;
    local_DC.x = 0;
    (&local_DC)->y = 48902;
    (&local_DC)->z = 0;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_DC, 0);
  }
  return;
}

static int32_t script_13472(int32_t arg0) {
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
    if (Script_shenSeqUtils(-1)) {
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
    }
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_136FF(int32_t arg0) {
  while (Script_shenSeqUtils(-1)) {
    Script_charaPosRotUtils(arg0, 163, 0, &data_2B100, CHARA_PART_Add);
    Script_charaPosRotUtils(arg0, 173, 0, &data_2B100, CHARA_PART_Add);
    Script_charaPosRotUtils(arg0, 182, 0, &data_2B10C, CHARA_PART_Add);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_13755(void) {
  SCNF_SetCashValue(-160, 1);
  SCNF_WaitFrames_0000(200);
  return Script_srchControl(0);
}

static int32_t script_13772(void) {
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
  end:;
}

__attribute__((scn_string_bindings(1, data_2B125, 2, data_2B133, 4, data_2B145, 6, data_2B157, 7, data_2B15C, 8, data_2B161)))
static void script_13816(void) {
  int32_t local_FC;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  local_FC = script_AE4E(0x5F535542);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_LoadSND(0, 4, "a1_busno.snd", 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "scene/03/MFSY", "YBUS") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
    Script_AuthorUtils(0, 0, "seqdata9.bin", "YBUS");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    script_12F2B(0);
  } else {
    Script_AuthorUtils(0, 0, "seqdataB.bin", "YBUS");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    script_13060(0);
  }
  Script_SetGameStateTo(AUTH_None);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "YBUS");
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_FC);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5F535542, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  SCNF_WaitFrames_0000(1);
  Script_LoadSND(0, 4, "e1forkli.snd", 0);
  return;
}

__attribute__((scn_string_bindings(0, data_2B16E, 1, data_2B177, 2, data_2B186, 3, data_2B18E, 4, data_2B193, 5, data_2B198, 6, data_2B19D, 7, data_2B1A2, 10, data_2B1BF, 11, data_2B1C5, 13, data_2B1DA, 14, data_2B1E2, 15, data_2B1EA, 16, data_2B1EB, 17, data_2B1F3)))
static void script_139A7(void) {
  Vector3 local_F4;
  Script_sicnUtils(2, 1, 0);
  Script_executeAicaCommand(2720, 2, 20);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1300A0, 4, 0);
  script_AE4E(0x594E4F54);
  script_AE4E(0x4F524F47);
  script_AE4E(0x48544D53);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  task_2C1 = 3;
  SCNF_WaitFrames_0000(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "PK_SHUKI");
  Script_ReallocateUNFO(task_2BC);
  Script_forkSetSoundBankDetails(SHEN_NAME('FORK'));
  Script_SetupClothData(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  local_F4.x = 1000.0f;
  (&local_F4)->z = 1000.0f;
  Script_EVT_SetPosition(SHEN_NAME('FORK'), 0x28000000, &local_F4);
  Script_ccowUtils(SHEN_NAME('FORK'), CCOW_DisableAll);
  Script_ccowUtils(CHARA_TONY, CCOW_DisableAll);
  Script_ccowUtils(CHARA_GORO, CCOW_DisableAll);
  Script_motmGetCharaFlags(CHARA_AKIR, MOTM_PoseNone, SCEN_Write);
  Script_motmGetCharaFlags(CHARA_TONY, MOTM_PoseNone, SCEN_Write);
  Script_motmGetCharaFlags(CHARA_GORO, MOTM_PoseNone, SCEN_Write);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_9QTE") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(0, 6, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm067.snd", 0);
  Script_LoadSND(0, 4, "a1_qrenq.snd", 0);
  Script_LoadVoiceFile(1, "019RN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_9QTE");
  Script_AuthorUtils(0, 1, "seqdata2.bin", "PK_9QTE");
  Script_AuthorUtils(0, 0, "PK_9QTE", "");
  script_13C2B(1);
  Script_AuthorUtils(AUTH_FREE, 1);
  script_13E74(1);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "PK_9QTE");
  Script_LoadVoiceFile(0, "019RN");
  return;
}

static void script_13C2B(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_0290();
  script_13F6C();
  local_F8 = Script_shenSeqUtils(-8, 0);
  Script_shenSeqUtils(-9, 0, 0);
  if (arg0) {
    Script_scenCreateTask(script_0385, 0);
  }
  Script_shenSeqUtils(1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 1:
        Script_executeAicaCommand(25768, 0, 0);
        Script_executeAicaCommand(1184, 2, 127);
        Script_SetMaxInterpSpeed(0.0f);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        Script_FaceEx(CHARA_GORO, FACE_ExpressionDisabled, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_SetupLightFromTable(0);
        break;
      case 280:
        Script_executeAicaCommand(0x1100A0, 4, 0);
        Script_ReadModelFormat7(CHARA_AKIR, 1, data_2B1FC, 10);
        break;
      case 520:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
        break;
      case 551:
        Script_FaceEx(CHARA_GORO, FACE_ExpressionEnabled, 0, 1);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 90.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 90.0f);
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        Script_scenToggleActiveFor(CHARA_GORO, 0);
        break;
      case 580:
        Script_CharHandlers(CHARA_AKIR, 1, 8, 16);
        break;
      case 626:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
        Script_FaceEx(CHARA_GORO, FACE_ExpressionDisabled, 0, 1);
        Script_scenToggleActiveFor(CHARA_AKIR, 1);
        Script_scenToggleActiveFor(CHARA_GORO, 1);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMaxInterpSpeed(0.5f);
  Script_executeAicaCommand(0x1100A0, 4, 0);
  Script_shenSeqUtils(-9, 0, local_F8);
  return;
}

static int32_t script_13E18(void) {
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1300A0, 4, 0);
  end:;
}

static void script_13E74(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_shenSeqUtils(-8, 0);
  Script_shenSeqUtils(-9, 0, 0);
  Script_shenSeqUtils(0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 1:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 200.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 200.0f);
        Script_SetMaxInterpSpeed(0.0f);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_265EC, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_265EC, 1);
        Script_SetupLightFromTable(0);
        break;
      case 150:
        Script_scenCreateTask(script_13E18, 0);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMaxInterpSpeed(0.5f);
  Script_shenSeqUtils(-9, 0, local_F8);
  return;
}

static void script_13F6C(void) {
  Vector4 local_F0;
  local_F0.x = 0.4f;
  (&local_F0)->y = 0.4f;
  (&local_F0)->z = 0.4f;
  (&local_F0)->w = 0.4f;
  Script_SetFogColour(&data_2B224);
  Script_SetFogFlags(0, data_2B234);
  Script_SetCharaFlags(CHARA_TONY, 4);
  Script_SetCharaFlags(CHARA_GORO, 4);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_TONY, 4);
  Script_SetCharaFlags(CHARA_GORO, 4);
  Script_mapmSetRenderFlag(0, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_TONY, 1);
  Script_charaSetRefboardFlagTo(CHARA_GORO, 1);
  Script_SetCharaRefBoardPos(CHARA_GORO, &local_F0);
  Script_SetCharaRefBoardPos(CHARA_TONY, &local_F0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_F0);
  return;
}

static void script_1403D(int32_t arg0) {
  int32_t local_FC;
  if (arg0 != -1) {
    task_388 = arg0;
  }
  local_FC = task_388;
  if (local_FC == 0) {
    Script_gachaponUtils(0, data_30400);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_304A9);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_305E4);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_30719);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_307C0);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_3092D);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_30B26);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_30BF9);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_30C6A);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_30CB7);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_30CF0);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_30DDF);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_30E42);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_30E97);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_30EDE);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_30F17);
  } else {
    task_388 = -1;
  }
  return;
}

static int16_t script_141DE(int8_t arg0, int8_t arg1) {
  int16_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_FE = -1;
  if (arg0 == -1) {
    local_FD = task_388;
  } else {
    local_FD = arg0;
  }
  local_F8 = local_FD;
  switch (local_F8) {
    case 0:
      local_FE = *(int16_t *)((int8_t *)&data_302A4 + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_302BC + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_302E0 + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_3030C + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_30320 + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_30348 + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_3036C + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_30384 + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_30394 + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_3039C + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_303A4 + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_303C8 + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_303D8 + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_303E4 + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_303F0 + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_303F8 + (arg1 << 1));
      break;
  }
  return local_FE;
}

static void script_143C9(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_30284[*(int32_t *)arg0] - data_30294[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_30284[*(int32_t *)arg0] - data_30294[*(int32_t *)arg0]);
  return;
}

static void script_14418(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_30284[arg0] - data_30294[arg0]) {
    (void)1;
  } else if (arg0 == 15) {
    (void)3;
  } else {
    (void)2;
  }
  return;
}

static void script_1446B(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
  Vector3 local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int32_t local_EC;
  (void)((local_F3 = 0) & 1);
  if (!(arg3 >= 0.0f)) {
    (void)((local_F2 = -1) & 1);
    arg3 = arg3 * -1.0f;
  } else {
    (void)((local_F2 = 0) & 1);
  }
  local_EC = arg1;
  switch (local_EC) {
    case 1:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 12) | (local_EC == 13)) {
        (void)((local_F3 = -1) & 1);
        goto L0144DC;
      }
      L0144DC:;
      break;
    case 3:
      local_EC = arg2;
      if (local_EC == 6) {
        (void)((local_F3 = -1) & 1);
        goto L014503;
      }
      L014503:;
      break;
    case 4:
      local_EC = arg2;
      if ((local_EC == 7) | (local_EC == 15) | (local_EC == 16) | (local_EC == 17) | (local_EC == 18)) {
        (void)((local_F3 = -1) & 1);
        goto L014542;
      }
      L014542:;
      break;
    case 5:
      (void)((local_F3 = -1) & 1);
      break;
    case 6:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 2) | (local_EC == 3)) {
        (void)((local_F3 = -1) & 1);
        goto L014590;
      }
      L014590:;
      break;
    case 8:
      local_EC = arg2;
      if (local_EC == 3) {
        (void)((local_F3 = -1) & 1);
        goto L0145B7;
      }
      L0145B7:;
      break;
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
  if (local_F3 & 8) {
    arg2 = arg2 + data_30284[arg1];
    local_F4.x = (&local_F4)->x * arg3;
    (&local_F4)->y = (&local_F4)->y * arg3;
    (&local_F4)->z = (&local_F4)->z * arg3;
  } else {
    local_F4.x = (&local_F4)->x * arg3 * 0.33333f;
    (&local_F4)->y = (&local_F4)->y * arg3 * 0.33333f;
    (&local_F4)->z = (&local_F4)->z * arg3 * 0.33333f;
  }
  if (local_F2 & 4) {
    script_1403D(arg1);
    Script_gachaponUtils(5, arg0, arg2);
  } else {
    script_1403D(arg1);
    Script_gachaponUtils(1, arg0, arg2);
  }
  if ((((scn3_bits){.f = (&local_F4)->x}).i != 0x3F800000) & (((scn3_bits){.f = (&local_F4)->x}).i != 0)) {
    Script_hmdlApplyScale(arg0, &local_F4);
  }
  return;
}

static void script_146A2(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  Script_MT5Loader_TRWV_TRWAVE(2, task_2B4);
  Script_executeAicaCommand(81832, 0, 0);
  Script_executeAicaCommand(1184, 2, 111);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_FaceLookAtTargetUtils(CHARA_MARK, FACE_LookReset);
  Script_charaSetRefboardFlagTo(CHARA_MARK, 1);
  Script_scenCreateTask(script_17A4E, 1, 30);
  local_FC = Script_scenCreateTask(script_1486C, 0);
  local_F8 = Script_scenCreateTask(script_14AAB, 0);
  local_F4 = Script_scenCreateTask(script_14C1E, 0);
  local_F0 = Script_scenCreateTask(script_1510B, 0);
  local_EC = Script_scenCreateTask(script_14F4A, 0);
  if ((Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 29) == 2) & (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20) == 0)) {
    task_394 = 0;
    local_E8 = 1130;
  } else {
    local_E4 = Script_PlayerStatsUtils(PS_GetStatU32_1);
    if (local_E4 % 2) {
      task_394 = 1;
    } else {
      task_394 = 2;
    }
    local_E8 = 910;
  }
  task_38C = 0;
  for (;;) {
    task_38C = task_38C + 1;
    if (task_38C == local_E8) {
      Script_scenCreateTask(script_17A1A, 1, 20);
    }
    if (task_38C == local_E8 + 20) {
      Script_PlayCharaMotion(CHARA_MARK, 4101, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      script_14EB7();
      Script_srchControl((void *)local_FC);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_F4);
      Script_srchControl((void *)local_F0);
      Script_srchControl((void *)local_EC);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_MARK, 2);
  Script_MT5Loader_TRWV_TRWAVE(1, task_2B4);
  return;
}

static int32_t script_1486C(void) {
  for (;;) {
    switch (task_394) {
      case 0:
        {
          int32_t local_FC;
          local_FC = task_38C;
          switch (local_FC) {
            case 1:
              Script_seqEnterEvtCam(4008, 0, AUTH_None);
              break;
            case 160:
              Script_seqEnterEvtCam(4009, 0, AUTH_None);
              break;
            case 240:
              Script_seqEnterEvtCam(4010, 0, AUTH_None);
              break;
            case 325:
              Script_seqEnterEvtCam(4011, 0, AUTH_None);
              break;
            case 520:
              Script_seqEnterEvtCam(4012, 0, AUTH_None);
              break;
            case 600:
              Script_seqEnterEvtCam(4013, 0, AUTH_None);
              break;
            case 770:
              Script_seqEnterEvtCam(4014, 0, AUTH_None);
              break;
            case 900:
              Script_seqEnterEvtCam(4015, 0, AUTH_None);
              break;
            case 990:
              Script_seqEnterEvtCam(4016, 0, AUTH_None);
              break;
          }
          break;
        }
      case 1:
        {
          int32_t local_FC;
          local_FC = task_38C;
          switch (local_FC) {
            case 1:
              Script_seqEnterEvtCam(4008, 0, AUTH_None);
              break;
            case 160:
              Script_seqEnterEvtCam(4009, 0, AUTH_None);
              break;
            case 240:
              Script_seqEnterEvtCam(4010, 0, AUTH_None);
              break;
            case 325:
              Script_seqEnterEvtCam(4011, 0, AUTH_None);
              break;
            case 480:
              Script_seqEnterEvtCam(4018, 0, AUTH_None);
              break;
            case 710:
              Script_seqEnterEvtCam(4015, 0, AUTH_None);
              break;
            case 770:
              Script_seqEnterEvtCam(4016, 0, AUTH_None);
              break;
          }
          break;
        }
      case 2:
        {
          int32_t local_FC;
          local_FC = task_38C;
          switch (local_FC) {
            case 1:
              Script_seqEnterEvtCam(4019, 0, AUTH_None);
              break;
            case 240:
              Script_seqEnterEvtCam(4020, 0, AUTH_None);
              break;
            case 325:
              Script_seqEnterEvtCam(4021, 0, AUTH_None);
              break;
            case 480:
              Script_seqEnterEvtCam(4022, 0, AUTH_None);
              break;
            case 770:
              Script_seqEnterEvtCam(4023, 0, AUTH_None);
              break;
          }
          break;
        }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_31034, 1, data_3103E, 2, data_31048, 3, data_31052, 4, data_3105C, 6, data_31070, 7, data_3107A, 8, data_31084, 9, data_3108E, 10, data_31098, 11, data_310A2, 12, data_310AC, 13, data_310B6, 14, data_310C0)))
static int32_t script_14AAB(void) {
  for (;;) {
    if (task_394 == 0) {
      int32_t local_FC;
      local_FC = task_38C;
      switch (local_FC) {
        case 260:
          Script_ShowUIMessageFromID("E1078B001");
          break;
        case 320:
          Script_ShowUIMessageFromID("E1078C001");
          break;
        case 380:
          Script_ShowUIMessageFromID("E1078B002");
          break;
        case 520:
          Script_ShowUIMessageFromID("E1078C002");
          break;
        case 600:
          Script_ShowUIMessageFromID("E1078B003");
          break;
        case 770:
          Script_ShowUIMessageFromID("E1078B004");
          break;
        case 900:
          Script_ShowUIMessageFromID("E1078C002");
          break;
        case 990:
          Script_ShowUIMessageFromID("E1078B005");
          break;
      }
    } else {
      int32_t local_FC;
      local_FC = task_38C;
      switch (local_FC) {
        case 260:
          Script_ShowUIMessageFromID("E1078B001");
          break;
        case 320:
          Script_ShowUIMessageFromID("E1078C001");
          break;
        case 380:
          Script_ShowUIMessageFromID("E1078B002");
          break;
        case 520:
          Script_ShowUIMessageFromID("E1078C002");
          break;
        case 570:
          Script_ShowUIMessageFromID("E1078B003");
          break;
        case 710:
          Script_ShowUIMessageFromID("E1078C002");
          break;
        case 770:
          Script_ShowUIMessageFromID("E1078B005");
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_14C1E(void) {
  int32_t local_FC;
  Script_motmGetCharaFlags(CHARA_MARK, MOTM_PoseNone, SCEN_Write);
  Script_PlayCharaMotion(CHARA_MARK, 4099, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 0x40);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Enable, 4);
  Script_charaToggleMotEnabled(CHARA_MARK, 1);
  local_FC = 0;
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_MARK)) {
      local_FC = local_FC + 1;
    }
    if (local_FC >= 3) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 4);
  Script_PlayCharaMotion(CHARA_MARK, 4100, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_MARK) <= 70.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(CHARA_MARK, MOTM_BLEND_FRAMES_30);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(CHARA_MARK, 4102, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_MARK) <= 160.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_MARK, 4114, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Enable, 4);
  for (;;) {
    if (task_394 == 0) {
      if (task_38C == 1095) {
        break;
      }
    } else {
      if (task_38C == 875) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 4);
  Script_motmSetBlendFrames(CHARA_MARK, MOTM_BLEND_FRAMES_5);
  Script_charaToggleMotEnabled(CHARA_MARK, 0);
  script_0058(CHARA_MARK, 0.0f, -180.0f, 0.0f);
  Script_PlayCharaMotion(CHARA_MARK, 4115, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_MARK, 2.0f);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_MARK)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_0058(CHARA_MARK, 0.0f, 90.0f, 0.0f);
  Script_PlayCharaMotion(CHARA_MARK, 4116, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_MARK)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_MARK, &data_25F84, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(CHARA_MARK, MOTM_PoseLow, SCEN_Write);
  Script_SetCharaMotionPlaybackRate(CHARA_MARK, 1.0f);
  end:;
}

static void script_14EB7(void) {
  Script_PlayCharaMotion(SHEN_NAME('TYNK'), 61442, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_HTSI, 61442, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_TAKE, 61442, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_YOHI, 61442, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static int32_t script_14F4A(void) {
  Script_motmSetBlendFrames(SHEN_NAME('TYNK'), MOTM_BLEND_FRAMES_20);
  Script_motmSetBlendFrames(CHARA_HTSI, MOTM_BLEND_FRAMES_20);
  Script_motmSetBlendFrames(CHARA_TAKE, MOTM_BLEND_FRAMES_20);
  Script_motmSetBlendFrames(CHARA_YOHI, MOTM_BLEND_FRAMES_20);
  for (;;) {
    if (task_394 == 0) {
      if ((task_38C == 320) | (task_38C == 520) | (task_38C == 900)) {
        Script_PlayCharaMotion(SHEN_NAME('TYNK'), 4111, 70.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_HTSI, 4107, 70.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_TAKE, 4109, 70.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_YOHI, 4110, 70.0f, -1.0f, -1.0f, 1.0f, 0);
      }
    } else {
      if ((task_38C == 320) | (task_38C == 520) | (task_38C == 710)) {
        Script_PlayCharaMotion(SHEN_NAME('TYNK'), 4111, 70.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_HTSI, 4107, 70.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_TAKE, 4109, 70.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_YOHI, 4110, 70.0f, -1.0f, -1.0f, 1.0f, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(SHEN_NAME('TYNK'), MOTM_BLEND_FRAMES_5);
  Script_motmSetBlendFrames(CHARA_HTSI, MOTM_BLEND_FRAMES_5);
  Script_motmSetBlendFrames(CHARA_TAKE, MOTM_BLEND_FRAMES_5);
  Script_motmSetBlendFrames(CHARA_YOHI, MOTM_BLEND_FRAMES_5);
  end:;
}

static int32_t script_1510B(void) {
  int32_t local_FC;
  int32_t local_F0[3];
  local_FC = 0;
  while (local_FC < 5) {
    local_F0[0] = *(const uint32_t *)((const char *)&data_310CC + local_FC * 12);
    local_F0[1] = *(const uint32_t *)((const char *)&data_310CC + (local_FC * 12 + 4));
    local_F0[2] = *(const uint32_t *)((const char *)&data_310CC + (local_FC * 12 + 8));
    Script_forkAttachLoadActor(*(int32_t *)&task_2EC[local_FC][0], (const float *)local_F0, 0.05f, 0, 1);
    local_FC = local_FC + 1;
  }
  for (;;) {
    local_FC = 0;
    while (local_FC < 5) {
      Script_forkAdjustCharacterMovementBasedOnDistance(*(int32_t *)&task_2EC[local_FC][0]);
      local_FC = local_FC + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_151B2(void) {
  int32_t local_FC;
  Script_seqEnterEvtCam(4017, 0, AUTH_None);
  Script_PlayCharaMotion(CHARA_MARK, 4112, 130.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 0x40);
  local_FC = 0;
  for (;;) {
    if (local_FC == 2) {
      Script_ShowUIMessageFromID("E1078B006");
    }
    if (local_FC == 38) {
      Script_ShowUIMessageFromID("E1078B007");
    }
    if (local_FC > 40) {
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_MARK, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(CHARA_MARK, 4108, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmSetBlendFrames(CHARA_MARK, MOTM_BLEND_FRAMES_20);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Enable, 4);
  Script_seqEnterEvtCam(4007, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_1529A(int8_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  task_390 = arg0;
  Script_executeAicaCommand(147368, 0, 0);
  Script_executeAicaCommand(1184, 2, 111);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_FaceLookAtTargetUtils(CHARA_MARK, FACE_LookReset);
  Script_SetCharaLookAtPosition(CHARA_MARK, -20, 0, 0);
  Script_charaSetRefboardFlagTo(CHARA_MARK, 1);
  Script_ForkliftUtils(0x304B5246, FORK_CMD_ENTER_FORKLIFT_CAMERA_MODE);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  script_15B2E();
  Script_ForkliftUtils(0x304B5246, FORK_CMD_RESET_DRIVE_INPUTS);
  Script_ForkliftUtils(0x304B5246, FORK_CMD_SET_ENGINE_AUDIO_VOLUME, 116);
  Script_ForkliftUtils(0x304B5246, FORK_CMD_SOUND_CONTROL, 4);
  Script_ForkliftUtils(0x304B5246, FORK_CMD_SOUND_CONTROL, 3, 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_30F50, 30);
  Script_seqEnterEvtCam(4018, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
  Script_scenCreateTask(script_17A4E, 1, 30);
  script_0030(SHEN_NAME('FRK0'), -101.0f, 0.0f, 105.0f);
  script_0058(SHEN_NAME('FRK0'), 0.0f, 35.0f, 0.0f);
  Script_MoveObjectToObject(CHARA_MARK, 0x304B5246);
  local_FC = Script_scenCreateTask(script_1588C, 0);
  local_F8 = Script_scenCreateTask(script_15743, 0);
  local_F4 = Script_scenCreateTask(script_1559A, 0);
  Script_scenCreateTask(script_159FA, 0);
  task_38C = 0;
  if (task_390 == 1) {
    local_F0 = 560;
    local_EC = 380;
  } else {
    local_F0 = 500;
    local_EC = 320;
  }
  for (;;) {
    if (task_38C == local_EC) {
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
      Script_SetCharaMotionPlaybackRate(CHARA_MARK, 0.0f);
      SCNF_WaitFrames_0000(120);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
      Script_SetCharaMotionPlaybackRate(CHARA_MARK, 1.0f);
    }
    task_38C = task_38C + 1;
    if (task_38C == local_F0) {
      Script_scenCreateTask(script_17A1A, 1, 30);
    }
    if (task_38C == local_F0 + 30) {
      Script_srchControl((void *)local_FC);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_F4);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_MoveObjectToObject(CHARA_MARK, -1);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 61442, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_MARK, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 0x40);
  SCNF_WaitFrames_0000(1);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  Script_motmSetBlendFrames(CHARA_MARK, MOTM_BLEND_FRAMES_5);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_MARK, 2);
  return;
}

static int32_t script_1559A(void) {
  for (;;) {
    if (task_390 == 1) {
      int32_t local_FC;
      local_FC = task_38C;
      switch (local_FC) {
        case 1:
          Script_seqEnterEvtCam(4024, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
        case 220:
          Script_seqEnterEvtCam(4025, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
        case 320:
          Script_seqEnterEvtCam(4026, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
        case 390:
          Script_seqEnterEvtCam(4027, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
      }
    } else if ((task_390 == 2) | (task_390 == 3)) {
      int32_t local_FC;
      local_FC = task_38C;
      switch (local_FC) {
        case 1:
          Script_seqEnterEvtCam(4028, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
        case 200:
          Script_seqEnterEvtCam(4029, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
        case 300:
          Script_seqEnterEvtCam(4030, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
        case 340:
          Script_seqEnterEvtCam(4031, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
      }
    } else {
      int32_t local_FC;
      local_FC = task_38C;
      switch (local_FC) {
        case 1:
          Script_seqEnterEvtCam(4032, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
        case 200:
          Script_seqEnterEvtCam(4033, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
        case 300:
          Script_seqEnterEvtCam(4030, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
        case 340:
          Script_seqEnterEvtCam(4034, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(2, data_31130, 5, data_3114E, 6, data_31158, 9, data_31176, 10, data_31180)))
static int32_t script_15743(void) {
  for (;;) {
    if (task_390 == 1) {
      int32_t local_FC;
      local_FC = task_38C;
      switch (local_FC) {
        case 120:
          Script_ShowUIMessageFromID("E1078B009");
          break;
        case 220:
          Script_ShowUIMessageFromID("E1078B010");
          break;
        case 350:
          Script_ShowUIMessageFromID("E1078A001");
          break;
        case 390:
          Script_ShowUIMessageFromID("E1078B011");
          break;
      }
    } else if ((task_390 == 2) | (task_390 == 3)) {
      int32_t local_FC;
      local_FC = task_38C;
      switch (local_FC) {
        case 100:
          Script_ShowUIMessageFromID("E1078B012");
          break;
        case 200:
          Script_ShowUIMessageFromID("E1078B013");
          break;
        case 300:
          Script_ShowUIMessageFromID("E1078A001");
          break;
        case 340:
          Script_ShowUIMessageFromID("E1078B014");
          break;
      }
    } else {
      int32_t local_FC;
      local_FC = task_38C;
      switch (local_FC) {
        case 80:
          Script_ShowUIMessageFromID("E1078B015");
          break;
        case 200:
          Script_ShowUIMessageFromID("E1078B013");
          break;
        case 300:
          Script_ShowUIMessageFromID("E1078A001");
          break;
        case 340:
          Script_ShowUIMessageFromID("E1078B016");
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1588C(void) {
  if (task_390 == 1) {
    script_0030(CHARA_MARK, 3.2453f, 0.0f, 1.88413f);
    script_0058(CHARA_MARK, 0.0f, 45.0f, 0.0f);
    Script_charaToggleMotEnabled(CHARA_AKIR, 0);
    Script_PlayCharaMotion(CHARA_AKIR, 4097, 50.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(CHARA_MARK, 1);
    Script_PlayCharaMotion(CHARA_MARK, 4105, 50.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_MARK, 1.0f);
    for (;;) {
      if (Script_IsCharaMotionFinished(CHARA_MARK)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  } else {
    script_0030(CHARA_MARK, 3.39491f, 0.0f, 1.88462f);
    script_0058(CHARA_MARK, 0.0f, 45.0f, 0.0f);
    Script_charaToggleMotEnabled(CHARA_AKIR, 0);
    Script_PlayCharaMotion(CHARA_AKIR, 4098, 100.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(CHARA_MARK, 1);
    Script_PlayCharaMotion(CHARA_MARK, 4106, 100.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_MARK, 1.0f);
    for (;;) {
      if (Script_IsCharaMotionFinished(CHARA_MARK)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static int32_t script_159FA(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E0 = 0;
  script_1403D(13);
  Script_gachaponUtils(1, 0x43524C46, task_390 - 1);
  Script_hmdlApplyScale(SHEN_NAME('FLRC'), &data_311AC);
  Script_charaDisplay(SHEN_NAME('FLRC'), CDM_Hide);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(SHEN_NAME('FLRC'), CHARA_AKIR, 18, &data_31194, &data_311A0);
  if (task_390 == 1) {
    local_EC = 319;
    local_E8 = 380;
    local_E4 = 540;
  } else {
    local_EC = 299;
    local_E8 = 320;
    local_E4 = 500;
  }
  for (;;) {
    if (task_38C == local_EC) {
      Script_charaDisplay(SHEN_NAME('FLRC'), CDM_Show);
    }
    if (task_38C == local_E8) {
      if (local_E0 == 0) {
        Script_sprintf("item has been given");
        local_F0 = script_141DE(-1, task_390 - 1);
        Script_ProcessSubtitle("JINGURU01", 0);
        Script_GetCassetteFlags(local_F0, 1, 0, 0);
        SCNF_StartIconSystemMenu(local_F0);
        local_E0 = 1;
      }
    }
    if (task_38C == local_E4) {
      Script_gachaponUtils(2, 0x43524C46);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_15B2E(void) {
  Script_atckUtils(SetFlag, 1, 0);
  Script_atckUtils(SetFlag, 0, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TYNK'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TYNK'), CDM_Show);
      goto L015B6E;
    }
  }
  L015B6E:;
  Script_scenToggleVisibleFor(SHEN_NAME('TYNK'), 0);
  if (-1) {
    Script_charaDisplay(CHARA_HTSI, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_HTSI, CDM_Show);
      goto L015BA2;
    }
  }
  L015BA2:;
  Script_scenToggleVisibleFor(CHARA_HTSI, 0);
  if (-1) {
    Script_charaDisplay(CHARA_TAKE, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_TAKE, CDM_Show);
      goto L015BD6;
    }
  }
  L015BD6:;
  Script_scenToggleVisibleFor(CHARA_TAKE, 0);
  if (-1) {
    Script_charaDisplay(CHARA_YOHI, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_YOHI, CDM_Show);
      goto L015C0A;
    }
  }
  L015C0A:;
  Script_scenToggleVisibleFor(CHARA_YOHI, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FRK1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FRK1'), CDM_Show);
      goto L015C3E;
    }
  }
  L015C3E:;
  Script_scenToggleVisibleFor(SHEN_NAME('FRK1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FRK2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FRK2'), CDM_Show);
      goto L015C72;
    }
  }
  L015C72:;
  Script_scenToggleVisibleFor(SHEN_NAME('FRK2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FRK3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FRK3'), CDM_Show);
      goto L015CA6;
    }
  }
  L015CA6:;
  Script_scenToggleVisibleFor(SHEN_NAME('FRK3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FRK4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FRK4'), CDM_Show);
      goto L015CDA;
    }
  }
  L015CDA:;
  Script_scenToggleVisibleFor(SHEN_NAME('FRK4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('ZEK1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('ZEK1'), CDM_Show);
      goto L015D0E;
    }
  }
  L015D0E:;
  Script_scenToggleVisibleFor(SHEN_NAME('ZEK1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('ZEK2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('ZEK2'), CDM_Show);
      goto L015D42;
    }
  }
  L015D42:;
  Script_scenToggleVisibleFor(SHEN_NAME('ZEK2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('ZEK3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('ZEK3'), CDM_Show);
      goto L015D76;
    }
  }
  L015D76:;
  Script_scenToggleVisibleFor(SHEN_NAME('ZEK3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('ZEK4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('ZEK4'), CDM_Show);
      goto L015DAA;
    }
  }
  L015DAA:;
  Script_scenToggleVisibleFor(SHEN_NAME('ZEK4'), 0);
  return;
}

static void script_15DB7(void) {
  task_380 = task_2C1;
  task_2C1 = 2;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 205.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 150.0f);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_MARK, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FORK'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FORK'), CDM_Show);
      goto L015E1E;
    }
  }
  L015E1E:;
  Script_scenToggleVisibleFor(SHEN_NAME('FORK'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('ZE05'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('ZE05'), CDM_Show);
      goto L015E52;
    }
  }
  L015E52:;
  Script_scenToggleVisibleFor(SHEN_NAME('ZE05'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Show);
      goto L015E86;
    }
  }
  L015E86:;
  Script_scenToggleVisibleFor(SHEN_NAME('ZE06'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DOR1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DOR1'), CDM_Show);
      goto L015EBA;
    }
  }
  L015EBA:;
  Script_scenToggleVisibleFor(SHEN_NAME('DOR1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DOR8'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DOR8'), CDM_Show);
      goto L015EEE;
    }
  }
  L015EEE:;
  Script_scenToggleVisibleFor(SHEN_NAME('DOR8'), 0);
  return;
}

__attribute__((scn_string_bindings(0, data_311D6)))
static void script_15EFB(void) {
  task_2C1 = task_380;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 205.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_MARK, 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FORK'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FORK'), CDM_Show);
      goto L015F5B;
    }
  }
  L015F5B:;
  Script_scenToggleVisibleFor(SHEN_NAME('FORK'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('ZE05'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('ZE05'), CDM_Show);
      goto L015F8F;
    }
  }
  L015F8F:;
  Script_scenToggleVisibleFor(SHEN_NAME('ZE05'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('ZE06'), CDM_Show);
      goto L015FC3;
    }
  }
  L015FC3:;
  Script_scenToggleVisibleFor(SHEN_NAME('ZE06'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('DOR1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('DOR1'), CDM_Show);
      goto L015FF7;
    }
  }
  L015FF7:;
  Script_scenToggleVisibleFor(SHEN_NAME('DOR1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('DOR8'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('DOR8'), CDM_Show);
      goto L01602B;
    }
  }
  L01602B:;
  Script_scenToggleVisibleFor(SHEN_NAME('DOR8'), 1);
  Script_LoadSND(0, 2, "bgm062.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 114);
  return;
}

static void script_1605E(void) {
  int32_t local_FC;
  local_FC = 0;
  while (local_FC < 5) {
    *(int32_t *)&task_2EC[local_FC][0] = *(&data_311E4 + local_FC);
    *(int32_t *)&task_2EC[local_FC][4] = *(&data_311F8 + local_FC);
    *(int32_t *)&task_2EC[local_FC][8] = 0;
    *(int32_t *)&task_2EC[local_FC][12] = 0;
    *(int8_t *)&task_2EC[local_FC][16] = 0;
    *(int8_t *)&task_2EC[local_FC][17] = 0;
    *(int8_t *)&task_2EC[local_FC][18] = -1;
    *(int8_t *)&task_2EC[local_FC][19] = 0;
    local_FC = local_FC + 1;
  }
  task_35C[0] = task_35C[1] = task_35C[2] = 0;
  task_36C[0] = task_36C[1] = task_36C[2] = 0.0f;
  task_368 = 0;
  task_37C = 0;
  task_37D = -1;
  task_378 = 0;
  task_37E = -1;
  (void)((task_384 = 0) & 0x80);
  (void)((task_383 = 0) & 0x80);
  task_381 = 0;
  task_382 = 0;
  task_37F = 0;
  task_2D8 = 0;
  *(&task_2D8 + 1) = 0;
  *(&task_2D8 + 2) = 0;
  *((int8_t *)&task_2D8 + 12) = 1;
  *((int8_t *)&task_2D8 + 13) = 3;
  *((int8_t *)&task_2D8 + 14) = 5;
  *((int8_t *)&task_2D8 + 15) = 5;
  task_2E8 = 0;
  return;
}

static void script_161CF(void) {
  int32_t unused_F8[2];
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t unused_E4;
  Script_GetCassetteFlags(48, 1, 0, 0);
  script_15DB7();
  script_0030(SHEN_NAME('FRK0'), -42.0f, 0.0f, 105.0f);
  script_0030(SHEN_NAME('FRK1'), -46.0f, 0.0f, 107.0f);
  script_0030(SHEN_NAME('FRK2'), -50.0f, 0.0f, 105.0f);
  script_0030(SHEN_NAME('FRK3'), -54.0f, 0.0f, 107.0f);
  script_0030(SHEN_NAME('FRK4'), -58.0f, 0.0f, 105.0f);
  script_0058(SHEN_NAME('FRK0'), 0.0f, 90.0f, 0.0f);
  script_0058(SHEN_NAME('FRK1'), 0.0f, 90.0f, 0.0f);
  script_0058(SHEN_NAME('FRK2'), 0.0f, 90.0f, 0.0f);
  script_0058(SHEN_NAME('FRK3'), 0.0f, 90.0f, 0.0f);
  script_0058(SHEN_NAME('FRK4'), 0.0f, 90.0f, 0.0f);
  script_0030(CHARA_MARK, -65.0f, 0.0f, 106.7f);
  script_0058(CHARA_MARK, 0.0f, -90.0f, 0.0f);
  script_B002(SHEN_NAME('FRK0'), SHEN_NAME('ZEK5'));
  script_B002(SHEN_NAME('FRK1'), SHEN_NAME('ZEK4'));
  script_B002(SHEN_NAME('FRK2'), SHEN_NAME('ZEK3'));
  script_B002(SHEN_NAME('FRK3'), SHEN_NAME('ZEK2'));
  script_B002(SHEN_NAME('FRK4'), SHEN_NAME('ZEK1'));
  script_1605E();
  local_F0 = 0;
  while (local_F0 < 5) {
    local_EC = *(int32_t *)&task_2EC[local_F0][0];
    local_E8 = *(int32_t *)&task_2EC[local_F0][4];
    Script_ForkliftUtils(local_EC, FORK_CMD_SET_INTERACTION_PROFILE, 1);
    script_1644B(local_EC, local_E8);
    if (local_E8 != 0x52494B41) {
      script_16ADD(local_F0, (int32_t)&local_F4);
      Script_forkAttachLoadActor(local_EC, (const float *)&local_F4, *(float *)(data_3120C + (local_F0 << 2)), 10, 0);
    }
    local_F0 = local_F0 + 1;
  }
  Script_ForkliftUtils(0x304B5246, FORK_CMD_SOUND_CONTROL, 0);
  return;
}

static void script_163D8(void) {
  int32_t local_FC;
  local_FC = 0;
  while (local_FC < 5) {
    Script_forkSetSoundBankDetails(*(int32_t *)&task_2EC[local_FC][0]);
    Script_ForkliftUtils(*(int32_t *)&task_2EC[local_FC][0], FORK_CMD_SET_INTERACTION_PROFILE, 0);
    Script_ForkliftUtils(*(int32_t *)&task_2EC[local_FC][0], FORK_CMD_RESET_DRIVE_INPUTS);
    local_FC = local_FC + 1;
  }
  Script_ForkliftUtils(0x304B5246, FORK_CMD_SOUND_CONTROL, 1);
  script_15EFB();
  script_179E3();
  Script_GetCassetteFlags_0(48, 0, 0);
  return;
}

static void script_1644B(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_ccowUtils(arg1, CCOW_DisableAll);
  Script_MoveObjectToObject(arg1, arg0);
  local_F4.x = -0.0113f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.477f;
  Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
  (&local_E8)->y = 0;
  Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_E8, 0);
  if (arg1 == 0x52494B41) {
    Script_SetupClothData(arg1, 5);
  }
  Script_MoveObjectToObject(arg1, -1);
  Script_PlayCharaMotion(arg1, 61442, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_flmwTriggerInteraction(arg0, arg1);
  return;
}

static void script_164E8(void) {
  script_0030(SHEN_NAME('FRK0'), -51.0f, 0.0f, 105.0f);
  script_0030(SHEN_NAME('FRK1'), -54.0f, 0.0f, 107.0f);
  script_0030(SHEN_NAME('FRK2'), -57.0f, 0.0f, 105.0f);
  script_0030(SHEN_NAME('FRK3'), -60.0f, 0.0f, 107.0f);
  script_0030(SHEN_NAME('FRK4'), -63.0f, 0.0f, 105.0f);
  script_0030(CHARA_MARK, -66.0f, 0.0f, 109.5f);
  script_0058(CHARA_MARK, 0.0f, 0.0f, 0.0f);
  return;
}

__attribute__((scn_string_bindings(0, data_31220, 2, data_31234, 4, data_31245)))
static void script_1658D(void) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t unused_F4;
  int32_t local_F0;
  int32_t unused_EC;
  Script_shenClearFreeState(0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "FLRC") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadVoiceFile(1, "E1078");
  Script_LoadSND(0, 2, "bgm076.snd", 0);
  script_161CF();
  Script_executeAicaCommand(66729, 0, 0);
  script_146A2();
  script_164E8();
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 111);
  Script_ForkliftUtils(0x304B5246, FORK_CMD_SET_ENGINE_AUDIO_VOLUME, 127);
  script_17527();
  Script_scenCreateTask(script_17A4E, 1, 30);
  Script_seqEnterEvtCam(4001, SHEN_NAME('FRK0'), SHEN_NAME('FRK0'));
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsInEventCam() == 0) {
      break;
    }
  }
  SCNF_WaitFrames_0000(60);
  script_151B2();
  Script_executeAicaCommand(132265, 0, 0);
  Script_SetGameStateTo(AUTH_None);
  Script_ForkliftUtils(0x304B5246, FORK_CMD_ENTER_FORKLIFT_CAMERA_MODE);
  Script_convSetCamInactiveFlag(1);
  Script_atckUtils(Enable, 1);
  local_F8 = 10800;
  for (;;) {
    Script_ForkliftUtils(0x304B5246, FORK_CMD_SUPPRESS_ACTION_HOTKEYS, 1);
    Script_EVT_GetPosition(SHEN_NAME('FRK0'), -1, &task_350, 0);
    Script_flmwHandleInteractionState(SHEN_NAME('FRK0'));
    script_17564();
    script_168FC(1);
    script_168FC(2);
    script_168FC(3);
    script_168FC(4);
    Script_ForkliftUtils(0x304B5246, FORK_CMD_UPDATE_PROXIMITY_STATE);
    Script_ForkliftUtils(0x304B5246, FORK_CMD_SOUND_CONTROL, 2);
    script_1774F();
    local_F0 = script_16BC8();
    script_16E4D();
    script_17393();
    script_17171();
    script_172CE();
    if (script_1742A(local_F0)) {
      break;
    }
    local_F8 = local_F8 - 1;
    if (local_F8 <= 0) {
      *(int8_t *)&task_2EC[0][16] = 5;
      Script_atckUtils(Activate, 0, 0);
      Script_scenCreateTask(script_174EB, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetConversationDetails(CONV_DETAILS_MODE_SET_SHARED_ITEM_PREVIEW, -1);
  Script_atckUtils(Enable, 0);
  SCNF_WaitFrames_0000(1);
  local_F8 = 0;
  for (;;) {
    if (*(int8_t *)&task_2EC[0][16] > 1) {
      Script_ForkliftUtils(0x304B5246, FORK_CMD_SUPPRESS_ACTION_HOTKEYS, 1);
      Script_flmwHandleInteractionState(SHEN_NAME('FRK0'));
      script_168FC(1);
      script_168FC(2);
      script_168FC(3);
      script_168FC(4);
      Script_ForkliftUtils(0x304B5246, FORK_CMD_UPDATE_PROXIMITY_STATE);
    } else {
      script_17743();
      script_168FC(1);
      script_168FC(2);
      script_168FC(3);
      script_168FC(4);
    }
    Script_ForkliftUtils(0x304B5246, FORK_CMD_SOUND_CONTROL, 2);
    local_F8 = local_F8 + 1;
    if (local_F8 == 90) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_scenCreateTask(script_17A1A, 1, 30);
    }
    if (local_F8 == 120) {
      Script_executeAicaCommand(0x1300A0, 4, 0);
      Script_GetConversationDetails(CONV_DETAILS_MODE_SET_SHARED_ITEM_PREVIEW, -1);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_383 = -1) & 2);
  SCNF_WaitFrames_0000(1);
  script_17990();
  script_1529A(*(int8_t *)&task_2EC[0][16]);
  script_17598();
  script_163D8();
  script_17556();
  Script_LoadVoiceFile(0, "E1078");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, data_3124B);
  return;
}

static void script_168FC(int32_t arg0) {
  int32_t unused_F8[2];
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  local_EC = *(int32_t *)&task_2EC[arg0][0];
  local_F0 = Script_forkAdjustCharacterMovementBasedOnDistance(local_EC);
  if (local_F0 == 0) {
    script_16A46(arg0);
    script_16ADD(arg0, (int32_t)&local_F4);
    Script_forkAttachLoadActor(local_EC, (const float *)&local_F4, 0.0f, 11, 0);
  }
  local_E8 = script_1698E(arg0);
  if (*(int32_t *)&task_2EC[arg0][8] != local_E8) {
    *(int32_t *)&task_2EC[arg0][8] = local_E8;
    script_16ADD(arg0, (int32_t)&local_F4);
    Script_forkAttachLoadActor(local_EC, (const float *)&local_F4, 0.0f, 11, 0);
  }
  return;
}

static int32_t script_1698E(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FC = *(int32_t *)&task_2EC[arg0][0];
  local_F8 = *(int32_t *)&task_2EC[arg0][8];
  local_F0 = arg0;
  switch (local_F0) {
    case 1:
      local_F4 = Script_ForkliftUtils(local_FC, FORK_CMD_CALC_ANGLE_BETWEEN_POINTS, local_F8, 421, &data_2B244);
      break;
    case 2:
      local_F4 = Script_ForkliftUtils(local_FC, FORK_CMD_CALC_ANGLE_BETWEEN_POINTS, local_F8, 398, &data_2BF6C);
      break;
    case 3:
      local_F4 = Script_ForkliftUtils(local_FC, FORK_CMD_CALC_ANGLE_BETWEEN_POINTS, local_F8, 425, &data_2CBDC);
      break;
    case 4:
      local_F4 = Script_ForkliftUtils(local_FC, FORK_CMD_CALC_ANGLE_BETWEEN_POINTS, local_F8, 391, &data_2D924);
      break;
  }
  end:;
}

static void script_16A46(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0;
  switch (local_F8) {
    case 1:
      local_FC = 421;
      break;
    case 2:
      local_FC = 398;
      break;
    case 3:
      local_FC = 425;
      break;
    case 4:
      local_FC = 391;
      break;
  }
  *(int32_t *)&task_2EC[arg0][8] = *(int32_t *)&task_2EC[arg0][8] + 1;
  if (*(int32_t *)&task_2EC[arg0][8] >= local_FC) {
    *(int32_t *)&task_2EC[arg0][8] = 0;
  }
  return;
}

static void script_16ADD(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = *(int32_t *)&task_2EC[arg0][8];
  local_F8 = arg0;
  switch (local_F8) {
    case 1:
      *(int32_t *)arg1 = *(const uint32_t *)((const char *)&data_2B244 + local_FC * 8);
      *(int32_t *)(arg1 + 8) = *(const uint32_t *)((const char *)&data_2B244 + (local_FC * 8 + 4));
      break;
    case 2:
      *(int32_t *)arg1 = *(const uint32_t *)((const char *)&data_2BF6C + local_FC * 8);
      *(int32_t *)(arg1 + 8) = *(const uint32_t *)((const char *)&data_2BF6C + (local_FC * 8 + 4));
      break;
    case 3:
      *(int32_t *)arg1 = *(const uint32_t *)((const char *)&data_2CBDC + local_FC * 8);
      *(int32_t *)(arg1 + 8) = *(const uint32_t *)((const char *)&data_2CBDC + (local_FC * 8 + 4));
      break;
    case 4:
      *(int32_t *)arg1 = *(const uint32_t *)((const char *)&data_2D924 + local_FC * 8);
      *(int32_t *)(arg1 + 8) = *(const uint32_t *)((const char *)&data_2D924 + (local_FC * 8 + 4));
      break;
  }
  *(int32_t *)(arg1 + 4) = 0;
  return;
}

static int32_t script_16BC8(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_E5;
  int8_t local_E4;
  int8_t local_E3;
  local_E8 = 0;
  local_F0 = 0;
  while (local_F0 < 5) {
    if (*(int8_t *)&task_2EC[local_F0][19] == 0) {
      local_EC = *(int32_t *)&task_2EC[local_F0][0];
      local_E6 = *(int8_t *)&task_2EC[local_F0][18];
      local_E7 = Script_ForkliftUtils(local_EC, FORK_CMD_GET_REGION_INDEX);
      local_E5 = 0;
      local_E3 = 0;
      if (local_F0 == 0) {
        if (-(local_E7 == 0) & -(local_E6 == 8) & -(task_37E == 8)) {
          local_E3 = 1;
        }
      } else {
        if ((local_E7 == 0) & (local_E6 == 8)) {
          local_E3 = 1;
        }
      }
      if (local_E3) {
        if ((local_F0 == 0) & (*(int8_t *)&task_2EC[local_F0][17] > 0)) {
          Script_EVT_GetPosition(local_EC, -1, &local_F4, 0);
          local_E4 = *(int8_t *)&task_2EC[local_F0][17] - 1;
          *(int32_t *)(task_36C + local_E4) = Script_ForkliftUtils(0x304B5246, FORK_CMD_CALC_START_LINE_CROSSING_TIME, (const float *)&task_350, (const float *)&local_F4);
          Script_atckUtils(SetSlot, task_35C[local_E4], task_36C[local_E4], *(int8_t *)&task_2EC[local_F0][17] - 1);
          task_37F = task_37F + 1;
        }
        if (*(int8_t *)&task_2EC[local_F0][17] == 3) {
          *(int8_t *)&task_2EC[local_F0][19] = 1;
          local_E8 = 1;
        } else {
          *(int8_t *)&task_2EC[local_F0][17] = *(int8_t *)&task_2EC[local_F0][17] + 1;
          if (local_F0 == 0) {
            *((int8_t *)&task_2D8 + 12) = *(int8_t *)&task_2EC[local_F0][17];
          }
          local_E5 = 1;
        }
      }
      if (local_E7 != -1) {
        if ((local_E7 == local_E6 + 1) | local_E5 | (*(int8_t *)&task_2EC[local_F0][17] == 0)) {
          *(int8_t *)&task_2EC[local_F0][18] = local_E7;
        }
      } else {
        if (*(int8_t *)&task_2EC[local_F0][17] == 0) {
          *(int8_t *)&task_2EC[local_F0][18] = 7;
        }
      }
      if (*(int8_t *)&task_2EC[local_F0][19]) {
        *(int32_t *)&task_2EC[local_F0][12] = script_17001(local_EC, 4, 0);
      } else {
        *(int32_t *)&task_2EC[local_F0][12] = script_17001(local_EC, *(int8_t *)&task_2EC[local_F0][17], *(int8_t *)&task_2EC[local_F0][18]);
      }
      if (local_F0 == 0) {
        task_37E = local_E7;
      }
    }
    local_F0 = local_F0 + 1;
  }
  return local_E8;
}

static void script_16E4D(void) {
  int8_t local_FF;
  int8_t local_FE;
  uint8_t unused_FC[2];
  float local_F8;
  int32_t unused_D8[8];
  float local_D4;
  *(int32_t *)&local_D4 = Script_ForkliftUtils(0x304B5246, FORK_CMD_DISTANCE_TO_POINT_LIST_SQ, &data_2E55C);
  *(int32_t *)(&local_D4 + 1) = Script_ForkliftUtils(0x304B5246, FORK_CMD_DISTANCE_TO_POINT_LIST_SQ, &data_2E854);
  *(int32_t *)(&local_D4 + 2) = Script_ForkliftUtils(0x304B5246, FORK_CMD_DISTANCE_TO_POINT_LIST_SQ, &data_2EC84);
  *(int32_t *)(&local_D4 + 3) = Script_ForkliftUtils(0x304B5246, FORK_CMD_DISTANCE_TO_POINT_LIST_SQ, &data_2F104);
  *(int32_t *)(&local_D4 + 4) = Script_ForkliftUtils(0x304B5246, FORK_CMD_DISTANCE_TO_POINT_LIST_SQ, &data_2F47C);
  *(int32_t *)(&local_D4 + 5) = Script_ForkliftUtils(0x304B5246, FORK_CMD_DISTANCE_TO_POINT_LIST_SQ, &data_2F6B4);
  *(int32_t *)(&local_D4 + 6) = Script_ForkliftUtils(0x304B5246, FORK_CMD_DISTANCE_TO_POINT_LIST_SQ, &data_2FA64);
  *(int32_t *)(&local_D4 + 7) = Script_ForkliftUtils(0x304B5246, FORK_CMD_DISTANCE_TO_POINT_LIST_SQ, &data_2FDF4);
  *(int32_t *)(&local_D4 + 8) = Script_ForkliftUtils(0x304B5246, FORK_CMD_DISTANCE_TO_POINT_LIST_SQ, &data_30224);
  local_FE = task_37D;
  local_FF = 0;
  local_F8 = 1000000.0f;
  while (local_FF < 9) {
    if (!(local_F8 <= *(&local_D4 + local_FF))) {
      task_37D = local_FF;
      local_F8 = *(&local_D4 + local_FF);
    }
    local_FF = local_FF + 1;
  }
  if ((task_37D == 0) & (local_FE == 8)) {
    if (task_37C < *(int8_t *)&task_2EC[0][17]) {
      task_37C = task_37C + 1;
    }
  } else {
    if ((task_37D == 8) & (local_FE == 0)) {
      if (-(task_37C == *(int8_t *)&task_2EC[0][17]) & -(task_37C > 0) & -(*(int8_t *)&task_2EC[0][18] < 3)) {
        task_37C = task_37C - 1;
      }
      goto L016FE8;
    }
  }
  L016FE8:;
  task_378 = script_17001(0x304B5246, task_37C, task_37D);
  return;
}

static int32_t script_17001(int32_t arg0, int8_t arg1, int8_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  local_F8 = arg2;
  switch (local_F8) {
    case -1:
      local_FC = Script_ForkliftUtils(arg0, FORK_CMD_FIND_NEAREST_POINT_INDEX, arg1, arg2, &data_2E55C);
      break;
    case 0:
      local_FC = Script_ForkliftUtils(arg0, FORK_CMD_FIND_NEAREST_POINT_INDEX, arg1, arg2, &data_2E55C);
      break;
    case 1:
      local_FC = Script_ForkliftUtils(arg0, FORK_CMD_FIND_NEAREST_POINT_INDEX, arg1, arg2, &data_2E854);
      break;
    case 2:
      local_FC = Script_ForkliftUtils(arg0, FORK_CMD_FIND_NEAREST_POINT_INDEX, arg1, arg2, &data_2EC84);
      break;
    case 3:
      local_FC = Script_ForkliftUtils(arg0, FORK_CMD_FIND_NEAREST_POINT_INDEX, arg1, arg2, &data_2F104);
      break;
    case 4:
      local_FC = Script_ForkliftUtils(arg0, FORK_CMD_FIND_NEAREST_POINT_INDEX, arg1, arg2, &data_2F47C);
      break;
    case 5:
      local_FC = Script_ForkliftUtils(arg0, FORK_CMD_FIND_NEAREST_POINT_INDEX, arg1, arg2, &data_2F6B4);
      break;
    case 6:
      local_FC = Script_ForkliftUtils(arg0, FORK_CMD_FIND_NEAREST_POINT_INDEX, arg1, arg2, &data_2FA64);
      break;
    case 7:
      local_FC = Script_ForkliftUtils(arg0, FORK_CMD_FIND_NEAREST_POINT_INDEX, arg1, arg2, &data_2FDF4);
      break;
    case 8:
      local_FC = Script_ForkliftUtils(arg0, FORK_CMD_FIND_NEAREST_POINT_INDEX, arg1, arg2, &data_30224);
      break;
  }
  return local_FC;
}

static void script_17171(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int8_t local_F3;
  int8_t local_F2;
  local_FC = 0;
  local_F2 = 0;
  while (local_FC < 5) {
    if (*(int8_t *)&task_2EC[local_FC][19] == 2) {
      local_F2 = local_F2 + 1;
    }
    local_FC = local_FC + 1;
  }
  local_F4 = *(int32_t *)&task_2EC[0][12];
  if ((local_F4 > task_378) & (*(int8_t *)&task_2EC[0][19] == 0)) {
    *(int32_t *)&task_2EC[0][12] = task_378;
  }
  local_FC = 0;
  while (local_FC < 5) {
    if (*(int8_t *)&task_2EC[local_FC][19] != 2) {
      local_F8 = 0;
      local_F3 = 1;
      while (local_F8 < 5) {
        if ((local_FC != local_F8) & (*(int8_t *)&task_2EC[local_F8][19] != 2)) {
          if (*(int32_t *)&task_2EC[local_FC][12] < *(int32_t *)&task_2EC[local_F8][12]) {
            local_F3 = local_F3 + 1;
          }
        }
        local_F8 = local_F8 + 1;
      }
      *(int8_t *)&task_2EC[local_FC][16] = local_F2 + local_F3;
      if (local_FC == 0) {
        *((int8_t *)&task_2D8 + 14) = *(int8_t *)&task_2EC[local_FC][16];
      }
      if (*(int8_t *)&task_2EC[local_FC][19] == 1) {
        *(int8_t *)&task_2EC[local_FC][19] = 2;
      }
    }
    local_FC = local_FC + 1;
  }
  *(int32_t *)&task_2EC[0][12] = local_F4;
  return;
}

static void script_172CE(void) {
  int32_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  if (*(int8_t *)&task_2EC[0][16] == 5) {
    local_FC = 1;
    while (local_FC < 5) {
      if (*(int8_t *)&task_2EC[local_FC][16] == 4) {
        break;
      }
      local_FC = local_FC + 1;
    }
    if (local_FC != 5) {
      local_FB = *(int8_t *)&task_2EC[0][18] + (*(int8_t *)&task_2EC[0][17] - 1) * 9;
      local_FA = *(int8_t *)&task_2EC[local_FC][18] + (*(int8_t *)&task_2EC[local_FC][17] - 1) * 9;
      if (local_FA - local_FB >= 2) {
        Script_ForkliftUtils(*(int32_t *)&task_2EC[local_FC][0], FORK_CMD_SET_GOAL_SPEED_STEP, -0.0025f);
      } else {
        Script_ForkliftUtils(*(int32_t *)&task_2EC[local_FC][0], FORK_CMD_SET_GOAL_SPEED_STEP, 0.03f);
      }
    }
  }
  return;
}

static void script_17393(void) {
  uint8_t unused_FF;
  int8_t local_FE;
  uint8_t unused_F0[14];
  int32_t local_EC;
  local_FE = *(int8_t *)&task_2EC[0][17];
  if (local_FE == 0) {
    local_FE = 1;
  }
  task_35C[local_FE - 1] = task_35C[local_FE - 1] + 1;
  task_368 = task_368 + 1;
  local_EC = local_FE;
  switch (local_EC) {
    case 1:
      task_2D8 = *task_35C;
      break;
    case 2:
      *(&task_2D8 + 1) = task_35C[1];
      break;
    case 3:
      *(&task_2D8 + 2) = task_35C[2];
      break;
  }
  return;
}

static int32_t script_1742A(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = 0;
  if (arg0 == 0) {
    (void)local_F8;
  } else {
    local_FC = 1;
    if (*(int8_t *)&task_2EC[0][19]) {
      Script_ShowUIMessageFromID("E1078B008");
      if (task_384 == 0) {
        Script_SetCharaLookAtPosition(CHARA_MARK, -20, 0, 0);
        Script_scenCreateTask(script_178B0, 0);
        script_176B0();
        (void)((task_384 = -1) & 1);
      }
      local_F8 = 1;
    } else {
      while (local_FC < 5) {
        if (*(int8_t *)&task_2EC[local_FC][19]) {
          Script_ForkliftUtils(*(int32_t *)&task_2EC[local_FC][0], FORK_CMD_SET_GOAL_SPEED_STEP, -0.0025f);
        }
        local_FC = local_FC + 1;
      }
      if (task_384 == 0) {
        (void)((task_384 = -1) & 2);
      }
    }
    (void)local_F8;
  }
  end:;
}

static int32_t script_174EB(void) {
  if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
    Script_ShowUIMessageFromID("E1078A002");
  } else {
    Script_ShowUIMessageFromID("E1078A003");
  }
  Script_srchControl(0);
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_17527(void) {
  Script_atckUtils(0, 0, 0);
  Script_atckUtils(SetLayout, &task_2D8, 0);
  Script_atckUtils(SetStyle, 1, 0);
  Script_atckUtils(SetGauge, (const float *)&task_2E8);
  return;
}

static void script_17556(void) {
  Script_atckUtils(2, 0, 0);
  return;
}

static void script_17564(void) {
  float local_FC;
  *(int32_t *)&local_FC = Script_ForkliftUtils(0x304B5246, FORK_CMD_GET_THROTTLE_DELTA);
  if (!(local_FC >= 0.0f)) {
    local_FC = local_FC * -1.0f;
  }
  *(float *)&task_2E8 = local_FC * (float)108;
  return;
}

static void script_17598(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  int32_t local_EC;
  int32_t local_E8;
  float local_E4;
  int16_t local_E2;
  int8_t local_E1;
  uint8_t unused_E0;
  if (task_37F == 3) {
    Script_atckUtils(UpdateTime, 1, 0);
  }
  if (task_37F) {
    local_EC = 0;
    local_E8 = *task_35C;
    local_E4 = *task_36C;
    local_FC = 1;
    while (local_FC < task_37F) {
      if ((task_35C[local_FC] < local_E8) | ((task_35C[local_FC] == local_E8) & !(task_36C[local_FC] >= local_E4))) {
        local_E8 = task_35C[local_FC];
        local_E4 = task_36C[local_FC];
        local_EC = local_FC;
      }
      local_FC = local_FC + 1;
    }
    Script_atckUtils(UpdateTime, 0, local_EC);
  }
  local_F8 = Script_PlayerStatsUtils(PS_GetStatU16_1);
  local_F8 = local_F8 + 1;
  Script_PlayerStatsUtils(PS_SetStatU16_1, local_F8);
  local_F4 = Script_PlayerStatsUtils(PS_GetStatU32_2);
  local_F4 = local_F4 + task_368;
  Script_PlayerStatsUtils(PS_SetStatU32_2, local_F4);
  local_E1 = *(int8_t *)&task_2EC[0][16] - 1;
  local_E2 = Script_PlayerStatsUtils(PS_GetIndexedU16Table0Slot, local_E1);
  local_E2 = local_E2 + 1;
  Script_PlayerStatsUtils(PS_SetIndexedU16Table0Slot, local_E1, local_E2);
  return;
}

static void script_176B0(void) {
  SCNF_WaitFrames_0000(1);
  script_0030(SHEN_NAME('FRK0'), -64.64f, 0.0f, 106.2f);
  if (!((float)Script_ForkliftUtils(0x304B5246, FORK_CMD_GET_THROTTLE_DELTA) >= 0.0f)) {
    script_0058(SHEN_NAME('FRK0'), 0.0f, 270.0f, 0.0f);
    Script_forkAttachLoadActor(SHEN_NAME('FRK0'), data_3127C, -0.13f, 0, 1);
  } else {
    script_0058(SHEN_NAME('FRK0'), 0.0f, 90.0f, 0.0f);
    Script_forkAttachLoadActor(SHEN_NAME('FRK0'), data_3127C, 0.2315f, 0, 1);
  }
  return;
}

static void script_17743(void) {
  Script_forkAdjustCharacterMovementBasedOnDistance(SHEN_NAME('FRK0'));
  return;
}

static void script_1774F(void) {
  int8_t local_FF;
  Vector3 local_F0;
  Script_EVT_GetPosition(SHEN_NAME('FRK0'), -1, &local_F0, 0);
  (&local_F0)->y = 1.5f;
  if (task_382 == 1) {
    Script_SetCharaLookAtPosition(CHARA_MARK, 20, &local_F0, 0);
  } else {
    Script_SetCharaLookAtPosition(CHARA_MARK, 20, &local_F0, 2);
  }
  local_FF = *(int8_t *)&task_2EC[0][17];
  if (task_381 > local_FF) {
    goto L0178AF;
  } else if (task_381 == local_FF) {
    if (local_FF == 3) {
      if (Script_SetTaskDataValue(SHEN_NAME('FRK0'), &data_312A0, &data_312AC, 0) & -(*(int8_t *)&task_2EC[0][18] >= 7)) {
        Script_PlayCharaMotion(CHARA_MARK, 4103, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        task_382 = 2;
        task_381 = task_381 + 1;
      }
    }
  } else {
    if (Script_SetTaskDataValue(SHEN_NAME('FRK0'), &data_31288, &data_31294, 0)) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.5f)) {
        Script_PlayCharaMotion(CHARA_MARK, 4108, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        task_382 = 0;
      } else {
        Script_PlayCharaMotion(CHARA_MARK, 4101, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        task_382 = 1;
      }
      task_381 = task_381 + 1;
    }
  }
  L0178AF:;
  return;
}

static int32_t script_178B0(void) {
  int32_t local_FC;
  local_FC = 0;
  script_0058(CHARA_MARK, 0.0f, 270.0f, 0.0f);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 0x40);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 4);
  Script_PlayCharaMotion(CHARA_MARK, 4113, 100.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Enable, 0x80);
  Script_SetCharaMotionPlaybackRate(CHARA_MARK, 1.3f);
  for (;;) {
    if (local_FC == 0) {
      Script_seqEnterEvtCam(4035, SHEN_NAME('MGDM'), SHEN_NAME('MGDM'));
    }
    if (Script_IsCharaMotionFinished(CHARA_MARK) | task_383) {
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_MARK, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_MARK, MOTM_BLEND_FRAMES_5);
  return Script_srchControl(0);
}

static void script_17990(void) {
  Script_PlayCharaMotion(CHARA_MARK, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Disable, 0x40);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_MARK, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_MARK, MOTM_BLEND_FRAMES_5);
  return;
}

static void script_179E3(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours < 10) {
    (&local_F4)->hours = 10;
    (&local_F4)->minutes = 0;
    (&local_F4)->seconds = 0;
    Script_SetNewDateTime(&local_F4);
  }
  return;
}

static int32_t script_17A1A(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_17A4E(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_17A82(void) {
  int16_t local_FE;
  local_FE = 0;
  while (local_FE < 32) {
    task_3EC[local_FE] = -1;
    local_FE = local_FE + 1;
  }
  return;
}

static int16_t script_17AAD(int32_t arg0) {
  int16_t local_FE;
  int16_t local_FC;
  local_FE = 0;
  while (local_FE < 32) {
    if (task_3EC[local_FE] == -1) {
      task_3EC[local_FE] = arg0;
      local_FC = local_FE;
      break;
    }
    local_FE = local_FE + 1;
  }
  if (local_FE == 32) {
    local_FC = -1;
  } else {
    local_FC = local_FE;
  }
  return local_FC;
}

static void script_17B0D(int32_t arg0, int32_t arg1) {
  int16_t local_FE;
  int16_t local_FC;
  if (task_3EC[arg1] == -1) {
    task_3EC[arg1] = arg0;
    local_FC = arg1;
    local_FE = arg1;
  } else {
    local_FE = 0;
    while (local_FE < 32) {
      if (task_3EC[local_FE] == -1) {
        task_3EC[local_FE] = arg0;
        local_FC = local_FE;
        break;
      }
      local_FE = local_FE + 1;
    }
  }
  if (local_FE == 32) {
    local_FC = -1;
  }
  (void)local_FC;
  return;
}

static void script_17B91(int32_t arg0) {
  int16_t local_FE;
  int16_t local_FC;
  local_FE = 0;
  while (local_FE < 32) {
    if (task_3EC[local_FE] == arg0) {
      if (task_3EC[local_FE] != 0) {
        Script_srchControl((void *)task_3EC[local_FE]);
        task_3EC[local_FE] = -1;
        break;
        goto L017BE5;
      }
    }
    L017BE5:;
    local_FE = local_FE + 1;
  }
  if (local_FE == 32) {
    local_FC = -1;
  } else {
    local_FC = local_FE;
  }
  (void)local_FC;
  return;
}

static void script_17C0D(int32_t arg0) {
  if (task_3EC[arg0] != -1) {
    if (task_3EC[arg0] != 0) {
      Script_srchControl((void *)task_3EC[arg0]);
      task_3EC[arg0] = -1;
      (void)1;
    } else {
      (void)0;
    }
  } else {
    (void)0;
  }
  return;
}

static void script_17C5C(void) {
  int16_t local_FE;
  local_FE = 0;
  while (local_FE < 32) {
    if (task_3EC[local_FE] != -1) {
      if (task_3EC[local_FE] != 0) {
        Script_srchControl((void *)task_3EC[local_FE]);
        goto L017CA0;
      }
    }
    L017CA0:;
    local_FE = local_FE + 1;
  }
  return;
}

static void script_17CAD(void) {
  int16_t local_FE;
  int16_t local_FC;
  local_FE = 0;
  local_FC = 0;
  while (local_FE < 32) {
    if (task_3EC[local_FE] != -1) {
      local_FC = local_FC + 1;
    }
    local_FE = local_FE + 1;
  }
  (void)local_FC;
  return;
}

static void script_17CEE(int32_t arg0) {
  int16_t local_FE;
  local_FE = 0;
  while (local_FE < 32) {
    if (task_3EC[local_FE] == arg0) {
      Script_SetTaskExecutionSuspended((void *)task_3EC[local_FE], 1);
    }
    local_FE = local_FE + 1;
  }
  return;
}

static void script_17D2D(int32_t arg0) {
  int16_t local_FE;
  if (task_3EC[arg0] != -1) {
    script_17CEE(task_3EC[arg0]);
    local_FE = 1;
  } else {
    local_FE = 0;
  }
  (void)local_FE;
  return;
}

static void script_17D64(int32_t arg0) {
  int16_t local_FE;
  local_FE = 0;
  while (local_FE < 32) {
    if (task_3EC[local_FE] == arg0) {
      Script_SetTaskExecutionSuspended((void *)task_3EC[local_FE], 0);
    }
    local_FE = local_FE + 1;
  }
  return;
}

static void script_17DA3(int32_t arg0) {
  int16_t local_FE;
  if (task_3EC[arg0] != -1) {
    script_17D64(task_3EC[arg0]);
    local_FE = 1;
  } else {
    local_FE = 0;
  }
  (void)local_FE;
  return;
}

static float script_17DDA(float arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_E4;
  local_F4.x = arg0;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = arg1;
  local_E8.x = arg2;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = arg3;
  local_E4 = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &local_E8);
  return local_E4;
}

static int32_t script_17E1F(float arg0, float arg1, float arg2, float arg3) {
  float local_FC;
  float local_F8;
  int32_t local_F4;
  local_FC = arg2 - arg0;
  local_F8 = arg3 - arg1;
  local_F4 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, local_FC, local_F8);
  return local_F4;
}

static int32_t script_17E46(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
  Vector3i local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg0, &local_F4, 0);
  local_E4 = script_17E1F((&local_E8)->x, (&local_E8)->z, arg1, arg2);
  local_E0 = local_E4 - (&local_F4)->y;
  if (local_E0 < 0) {
    local_E0 = -local_E0;
  }
  if (local_E0 <= arg3) {
    (&local_F4)->y = local_E4;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    local_DC = 1;
  } else {
    if (arg4 == 1) {
      (&local_F4)->y = (&local_F4)->y - arg3;
    } else {
      (&local_F4)->y = (&local_F4)->y + arg3;
    }
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    local_DC = 0;
  }
  return local_DC;
}

static int32_t script_17F05(int32_t arg0, float arg1, float arg2, int32_t arg3) {
  Vector3i local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg0, &local_F4, 0);
  local_E4 = script_17E1F((&local_E8)->x, (&local_E8)->z, arg1, arg2);
  local_E0 = local_E4 - (&local_F4)->y;
  local_E4 = 1;
  if (local_E0 < 0) {
    local_E0 = -local_E0;
    local_E4 = 0;
  }
  if (local_E0 <= arg3) {
    (&local_F4)->y = local_E4;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    local_DC = 1;
  } else {
    if (local_E4 == 1) {
      (&local_F4)->y = (&local_F4)->y - arg3;
    } else {
      (&local_F4)->y = (&local_F4)->y + arg3;
    }
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    local_DC = 0;
  }
  return local_DC;
}

static void script_17FD0(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  local_F0 = script_17E1F((&local_F4)->x, (&local_F4)->z, arg1, arg2);
  local_EC = local_F0 / arg4;
  while (1) {
    if (script_17E46(arg0, arg1, arg2, local_EC, arg3)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_18029(int32_t arg0, float arg1, float arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  local_F0 = script_17E1F((&local_F4)->x, (&local_F4)->z, arg1, arg2);
  local_EC = local_F0 / arg3;
  while (1) {
    if (script_17F05(arg0, arg1, arg2, local_EC)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_18080(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  float local_F0;
  while (1) {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    local_F0 = script_17DDA((&local_F4)->x, (&local_F4)->z, arg1, arg2);
    if (!(local_F0 > arg3)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_180C5(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = 0;
  while (1) {
    if (*(int32_t *)(arg1 + (local_FC << 2)) == -999) {
      break;
    }
    script_17FD0(arg0, *(float *)(arg1 + (local_FC << 2)), *(float *)(arg1 + ((local_FC + 1) << 2)), *(int32_t *)(arg1 + ((local_FC + 2) << 2)), *(int32_t *)(arg1 + ((local_FC + 3) << 2)));
    script_18080(arg0, *(float *)(arg1 + (local_FC << 2)), *(float *)(arg1 + ((local_FC + 1) << 2)), *(float *)(arg1 + ((local_FC + 4) << 2)));
    local_FC = local_FC + 5;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1815C(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4, float arg5) {
  script_17FD0(arg0, arg1, arg2, arg3, arg4);
  script_18080(arg0, arg1, arg2, arg5);
  return;
}

static void script_1817F(int32_t arg0, float arg1, float arg2, int32_t arg3, float arg4) {
  script_18029(arg0, arg1, arg2, arg3);
  script_18080(arg0, arg1, arg2, arg4);
  return;
}

static void script_181A0(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_181BC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_SetFightProgramStateBytes(arg0, arg4, arg5);
  Script_GetDataFromFightProgram(arg0, arg3);
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return Script_srchControl(0);
}

static void script_18204(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_181BC, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static void script_1821E(int32_t arg0, float arg1) {
  float local_FC;
  SCNF_WaitFrames_0000(1);
  local_FC = Script_GetCharaMotionRemainingFramesScaled(arg0);
  for (;;) {
    if (local_FC >= arg1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
    local_FC = Script_GetCharaMotionRemainingFramesScaled(arg0);
  }
  return;
}

static void script_18253(int32_t arg0, float arg1, int32_t arg2) {
  script_1821E(arg0, arg1);
  Script_executeAicaCommand(arg2, 0, 0);
  return;
}

static void script_1826B(int32_t arg0, int32_t arg1) {
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, arg1, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static int32_t script_18297(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t unused_E8;
  local_FC = 0;
  local_F8 = 0;
  local_F0 = -1;
  task_46C = 0;
  while (1) {
    local_FC = 0;
    while (*(int32_t *)(arg0 + (local_FC << 2)) != -999) {
      local_F4 = *(int32_t *)(arg0 + (local_FC << 2));
      local_EC = *(int32_t *)(arg0 + ((local_FC + 1) << 2));
      local_F0 = *(int32_t *)(arg0 + ((local_FC + 2) << 2));
      local_FC = local_FC + 4;
      if (local_EC == local_F8) {
        script_18331(local_F0, local_F4);
      }
    }
    local_F8 = local_F8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_18331(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_CamCtrl(CamCtrlCmd_Reset);
  local_FC = arg0;
  if (local_FC == 0) {
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
  } else if ((local_FC == 1) | (local_FC == 2)) {
    Script_seqEnterEvtCam(arg1, 0, AUTH_None);
  } else {
    switch (local_FC) {
      case 3:
        Script_seqEnterEvtCam(arg1, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
        break;
      case 4:
        Script_seqEnterEvtCam(arg1, SHEN_NAME('PIL2'), SHEN_NAME('PIL2'));
        break;
      case 5:
        Script_seqEnterEvtCam(arg1, SHEN_NAME('PIL3'), SHEN_NAME('PIL3'));
        break;
      case 6:
        Script_seqEnterEvtCam(arg1, SHEN_NAME('PIL2'), CHARA_BOB_);
        break;
      case 7:
        Script_seqEnterEvtCam(arg1, SHEN_NAME('CAR1'), SHEN_NAME('CAR1'));
        break;
      case 98:
        Script_CamCtrl(CamCtrlCmd_SetPosBlendFactors, 100, 5, 100);
        Script_CamCtrl(CamCtrlCmd_SetTargetBlendFactors, 100, 5, 100);
        Script_seqEnterEvtCam(arg1, CHARA_AKIR, CHARA_AKIR);
        break;
      case 99:
        Script_CamCtrl(CamCtrlCmd_SetLocalOffset, -1.2f, -0.55f, 3.0f);
        Script_CamCtrl(CamCtrlCmd_SetYawAdjust, 0, -5460, 0);
        Script_convSetCamInactiveFlag(1);
        Script_SetGameStateTo(AUTH_None);
        break;
      case 97:
        Script_CamCtrl(CamCtrlCmd_SetLocalOffset, -0.6f, -1.0f, 7.0f);
        Script_CamCtrl(CamCtrlCmd_SetYawAdjust, 0, -25860, 0);
        Script_convSetCamInactiveFlag(1);
        Script_SetGameStateTo(AUTH_None);
        break;
      case 96:
        Script_CamCtrl(CamCtrlCmd_SetLocalOffset, -0.8f, -0.25f, 3.0f);
        Script_CamCtrl(CamCtrlCmd_SetYawAdjust, 0, -69524, 0);
        Script_convSetCamInactiveFlag(0);
        Script_SetGameStateTo(AUTH_None);
        break;
      case 95:
        Script_seqEnterEvtCam(arg1, SHEN_NAME('FOK0'), SHEN_NAME('PIL1'));
        break;
      case 94:
        Script_seqEnterEvtCam(arg1, SHEN_NAME('FOK1'), SHEN_NAME('PIL1'));
        break;
    }
  }
  return;
}

static int32_t script_18529(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_charaDisplay(arg0, CDM_Hide);
  local_DC.x = 0.1f;
  (&local_DC)->y = 0.1f;
  (&local_DC)->z = 0.1f;
  while (1) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_mtwkGetSetRotation(arg1, &local_E8, 0);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    Script_hmdlApplyScale(arg0, &local_DC);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_185A8(int32_t arg0, float arg1, float arg2, int32_t arg3, float arg4) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  Script_EVT_GetPosition(arg0, -1, &local_EC, 0);
  Script_mtwkGetSetRotation(arg0, &local_E0, 0);
  local_FC = script_17E1F((&local_EC)->x, (&local_EC)->z, arg1, arg2);
  local_F8 = local_FC - (&local_E0)->y;
  if (local_F8 < 0) {
    local_FC = 1;
  } else {
    local_FC = 0;
  }
  script_17FD0(arg0, arg1, arg2, local_FC, arg3);
  script_18080(arg0, arg1, arg2, arg4);
  return;
}

static void script_1861F(int32_t arg0, float arg1, float arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  Script_EVT_GetPosition(arg0, -1, &local_EC, 0);
  Script_mtwkGetSetRotation(arg0, &local_E0, 0);
  local_FC = script_17E1F((&local_EC)->x, (&local_EC)->z, arg1, arg2);
  local_F8 = local_FC - (&local_E0)->y;
  if (local_F8 < 0) {
    local_FC = 1;
  } else {
    local_FC = 0;
  }
  script_17FD0(arg0, arg1, arg2, local_FC, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_312CC)))
static int32_t script_18687(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_E4 = 2;
  local_E0 = 2;
  while (1) {
    local_DC = 0;
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_CTS");
    while (local_DC < 1) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "HASI");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_312C8 + local_DC), &local_F4, &local_E8, CHARA_PART_Set);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (arg2 == 0) {
      break;
    } else {
      if (local_E4 >= 140) {
        Script_srchControl(0);
      }
    }
    if (local_E4 == 100) {
      Script_SetCamShake(0.3f, 0.3f, 0.5f);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_312D8)))
static int32_t script_1879A(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_D4[2];
  int32_t local_D0;
  local_D0 = 152;
  *(&local_D0 + 1) = 153;
  *(&local_D0 + 2) = 154;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_E4 = 2;
  local_E0 = 2;
  while (1) {
    local_DC = 0;
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_CTS");
    while (local_DC < 3) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "HAKO");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&local_D0 + local_DC), &local_F4, &local_E8, CHARA_PART_Set);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (arg2 == 0) {
      break;
    } else {
      if (local_E4 >= 150) {
        Script_srchControl(0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_312E4)))
static int32_t script_188A8(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_98[17];
  int32_t local_94;
  local_94 = 165;
  *(&local_94 + 1) = 164;
  *(&local_94 + 2) = 163;
  *(&local_94 + 3) = 162;
  *(&local_94 + 4) = 161;
  *(&local_94 + 5) = 160;
  *(&local_94 + 6) = 159;
  *(&local_94 + 7) = 158;
  *(&local_94 + 8) = 157;
  *(&local_94 + 9) = 156;
  *(&local_94 + 10) = 155;
  *(&local_94 + 11) = 154;
  *(&local_94 + 12) = 153;
  *(&local_94 + 13) = 152;
  *(&local_94 + 14) = 169;
  *(&local_94 + 15) = 168;
  *(&local_94 + 16) = 167;
  *(&local_94 + 17) = 166;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_E4 = 2;
  local_E0 = 2;
  while (1) {
    local_DC = 0;
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_CTS");
    while (local_DC < 18) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "IKAN");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&local_94 + local_DC), &local_F4, &local_E8, CHARA_PART_Set);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (arg2 == 0) {
      break;
    } else {
      if (local_E4 >= 180) {
        Script_srchControl(0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_312F0)))
static int32_t script_18A4C(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_C4[6];
  int32_t local_C0;
  local_C0 = 158;
  *(&local_C0 + 1) = 152;
  *(&local_C0 + 2) = 153;
  *(&local_C0 + 3) = 154;
  *(&local_C0 + 4) = 155;
  *(&local_C0 + 5) = 156;
  *(&local_C0 + 6) = 157;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_E4 = 2;
  local_E0 = 2;
  while (1) {
    local_DC = 0;
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_CTS");
    while (local_DC < 7) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "PIPE");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&local_C0 + local_DC), &local_F4, &local_E8, CHARA_PART_Set);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (arg2 == 0) {
      break;
    } else {
      if (local_E4 >= 180) {
        Script_srchControl(0);
      }
    }
    if (!(local_E4 == 50)) goto L018B7F;
    L018B7F:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_312FC)))
static int32_t script_18B8B(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_C4[6];
  int32_t local_C0;
  local_C0 = 158;
  *(&local_C0 + 1) = 152;
  *(&local_C0 + 2) = 153;
  *(&local_C0 + 3) = 154;
  *(&local_C0 + 4) = 155;
  *(&local_C0 + 5) = 156;
  *(&local_C0 + 6) = 157;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_E4 = 2;
  local_E0 = 2;
  while (1) {
    local_DC = 0;
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_CTS");
    while (local_DC < 7) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "IPIP");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&local_C0 + local_DC), &local_F4, &local_E8, CHARA_PART_Set);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (arg2 == 0) {
      break;
    } else {
      if (local_E4 >= 204) {
        Script_srchControl(0);
      }
    }
    if (!(local_E4 == 50)) goto L018CBE;
    L018CBE:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_31308)))
static int32_t script_18CCA(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_C8[5];
  int32_t local_C4;
  local_C4 = 157;
  *(&local_C4 + 1) = 155;
  *(&local_C4 + 2) = 154;
  *(&local_C4 + 3) = 152;
  *(&local_C4 + 4) = 156;
  *(&local_C4 + 5) = 153;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_E4 = 2;
  local_E0 = 2;
  while (1) {
    local_DC = 0;
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_CTS");
    while (local_DC < 6) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "DRAM");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&local_C4 + local_DC), &local_F4, &local_E8, CHARA_PART_Set);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (arg2 == 0) {
      break;
    } else {
      if (local_E4 >= 188) {
        Script_srchControl(0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_31314)))
static int32_t script_18DF6(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_D0[3];
  int32_t local_CC;
  local_CC = 152;
  *(&local_CC + 1) = 153;
  *(&local_CC + 2) = 154;
  *(&local_CC + 3) = 155;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_E4 = 2;
  local_E0 = 2;
  while (1) {
    local_DC = 0;
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_CTS");
    while (local_DC < 4) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "TRAC");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&local_CC + local_DC), &local_F4, &local_E8, CHARA_PART_Set);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (arg2 == 0) {
      break;
    } else {
      if (local_E4 >= 250) {
        Script_srchControl(0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_18F0E(int32_t arg0, int32_t arg1, float arg2, const Vector3 *arg3) {
  if (!(arg2 < (float)0)) {
    script_18F3A(arg0, arg1, arg2, arg3);
  } else {
    script_18F87(arg0, arg1, arg3);
  }
  return;
}

static void script_18F3A(int32_t arg0, int32_t arg1, float arg2, const Vector3 *arg3) {
  Script_PlayCharaMotion(arg0, arg1, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(arg0) <= arg2)) {
      break;
    }
    Script_RotateNodeUtils(arg0, arg3, 1);
  }
  return;
}

static void script_18F87(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  Script_PlayCharaMotion(arg0, arg1, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, arg2, 1);
  }
  return;
}

static void script_18FCF(int32_t arg0, int32_t arg1) {
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, arg1, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static int32_t script_18FFB(int32_t arg0, int32_t arg1, float arg2) {
  float local_FC;
  local_FC = Script_GetCharaMotionRemainingFramesScaled(arg0);
  for (;;) {
    if (local_FC >= arg2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
    local_FC = Script_GetCharaMotionRemainingFramesScaled(arg0);
  }
  Script_executeAicaCommand(arg1, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_19038(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  SCNF_WaitFrames_0000(arg0);
  Script_EVT_GetPosition(arg1, -1, &local_F0, 0);
  local_E4.x = 0.0f;
  (&local_E4)->y = 0.01f;
  (&local_E4)->z = 0.0f;
  local_D8.x = 0.0f;
  (&local_D8)->y = 0.0f;
  (&local_D8)->z = 0.0f;
  local_FC = Script_EffectUtils(0, &local_F0, &data_31320, 100, 0.01f, 0, 3);
  Script_EffectUtils(12, local_FC, 32);
  Script_EffectUtils(11, local_FC, 255);
  Script_EffectUtils(9, local_FC, 142, 125, 110);
  Script_EffectUtils(10, local_FC, 0.262745112f);
  Script_EffectUtils(8, local_FC, 2.54901958f, 1.254902f);
  Script_EffectUtils(7, local_FC, 0.00196078443f);
  Script_EffectUtils(6, local_FC, &local_E4, &local_D8);
  Script_EffectUtils(15, local_FC, 55.863998f, (float)arg2 * 10.0f / 255.0f);
  Script_EffectUtils(14, local_FC, 0.752941191f, 0.125490203f);
  Script_EffectUtils(1, local_FC);
  Script_EffectUtils(7, local_FC, 0.1f);
  SCNF_WaitFrames_0000(8);
  Script_EffectUtils(7, local_FC, 0.00196078443f);
  SCNF_WaitFrames_0000(150);
  Script_EffectUtils(3, local_FC);
  return Script_srchControl(0);
}

static float script_19178(int32_t arg0, int32_t arg1) {
  float local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
  local_FC = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F0, &local_E4);
  return local_FC;
}

static void script_191A5(int32_t arg0, int32_t arg1, float arg2) {
  float local_FC;
  local_FC = script_19178(arg0, arg1);
  for (;;) {
    if (arg2 >= local_FC) {
      break;
    }
    SCNF_WaitFrames_0000(1);
    local_FC = script_19178(arg0, arg1);
  }
  return;
}

static int32_t script_191DC(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_19200(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  SCNF_WaitFrames_0000(arg1);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1023A001");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1023A002");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1023A003");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1023A004");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1023A005");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1023A006");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1023A007");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1023B001");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1023C001");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1023C004");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1023C003");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1023D004");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1023D002");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1023D003");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1023E001");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1023E002");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1023F005");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1023F002");
      break;
    case 18:
      break;
    case 19:
      break;
    case 20:
      Script_ShowUIMessageFromID("E1023G001");
      break;
    case 21:
      Script_ShowUIMessageFromID("E1023G002");
      break;
    case 22:
      Script_ShowUIMessageFromID("E1023H003");
      break;
    case 23:
      Script_ShowUIMessageFromID("E1023H002");
      break;
    case 24:
      Script_ShowUIMessageFromID("E1023I001");
      break;
  }
  return Script_srchControl(0);
}

static void script_19412(int32_t arg0) {
  task_470 = arg0;
  task_474 = 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_470, task_474);
  return;
}

static void script_19430(void) {
  int32_t local_FC;
  if (task_474 == 1) {
    local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
    Script_PlayerStatsUtils(PS_Set70MBCounter1, local_FC + 1);
  }
  task_474 = task_474 + 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_470, task_474);
  return;
}

static void script_1946D(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter0);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, local_FC + 1);
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_470, task_474);
  return;
}

static void script_19495(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_3E8 != -1) {
    script_17C0D(0);
  }
  task_3E8 = arg0;
  local_F8 = arg0;
  if (local_F8 == 0) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31414);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 1) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31428);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 2) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_3143C);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 3) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31450);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 4) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31464);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 5) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31478);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 6) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_3148C);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 7) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_314A0);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 8) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_314B4);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 9) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_314C8);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 10) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31500);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 11) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_314EC);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 12) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31514);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 13) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31528);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 14) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_3152C);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 15) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31550);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 16) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_3161C);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 17) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31630);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 18) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31644);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 19) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31658);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 20) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_3166C);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 21) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31680);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 22) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31694);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 23) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_316A8);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 24) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_316BC);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 25) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_316D0);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 26) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_316D4);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 27) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_316E8);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 28) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_316FC);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 29) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31700);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 30) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31714);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 31) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31728);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 32) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_3175C);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 33) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31770);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 34) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31784);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 35) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31788);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 36) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_3179C);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 37) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31564);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 38) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31588);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 39) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_3158C);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 40) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_315A0);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 41) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_315A4);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 42) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_315A8);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 43) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_315BC);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 44) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_315E4);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 45) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_315D0);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 46) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31608);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 56) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_317B0);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 57) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_317C4);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 58) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_317E8);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 59) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_317FC);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 47) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31820);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 48) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31834);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 49) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31848);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 50) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_3185C);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 51) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31860);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 52) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31874);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 53) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_31888);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 54) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_3189C);
    script_17B0D(local_FC, 0);
  } else if (local_F8 == 55) {
    local_FC = Script_scenCreateTask(script_18297, 1, &data_318B0);
    script_17B0D(local_FC, 0);
  } else {
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
  }
  return;
}

static void script_19F15(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_scenCreateTask(script_0385, 0);
  local_F8 = arg0;
  if (local_F8 == 0) {
    Script_scenCreateTask(script_1A2DB, 0);
    local_FC = Script_scenCreateTask(script_1A276, 0);
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_FC);
    goto L019F75;
  }
  L019F75:;
  return;
}

static void script_19F76(void) {
  script_1A00B(CHARA_TONY);
  script_1A00B(CHARA_SMTH);
  script_19FF8(SHEN_NAME('FORK'));
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_318C4);
  Script_charaSetRefboardFlagTo(CHARA_TONY, 1);
  Script_SetCharaRefBoardPos(CHARA_TONY, &data_318C4);
  Script_charaSetRefboardFlagTo(CHARA_SMTH, 1);
  Script_SetCharaRefBoardPos(CHARA_SMTH, &data_318C4);
  Script_SetShadowRange(100.0f);
  return;
}

static void script_19FF8(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  return;
}

static void script_1A00B(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 10, 16);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_SetCharaFlags(arg0, 4);
  Script_SetCPAMFlags(arg0, 0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_charaToggleMotEnabled(arg0, 0);
  return;
}

static void script_1A076(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_facemotUtils(arg0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 8, 16);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaFlags(arg0, 4);
  Script_clmdSetFlags(arg0, 2, 0);
  Script_SetCPAMFlags(arg0, 0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_charaToggleMotEnabled(arg0, 0);
  return;
}

static void script_1A133(void) {
  Script_charaPosRotUtils(SHEN_NAME('FORK'), 3, &data_31904, 0, CHARA_PART_Set);
  script_24202(CHARA_AKIR);
  script_241A6(CHARA_TONY);
  script_241A6(CHARA_SMTH);
  script_241A3(0x4B524F46);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_TONY, 2);
  Script_charaSetRefboardFlagTo(CHARA_SMTH, 2);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_1A1A5(void) {
  return;
}

static void script_1A1A8(int32_t arg0) {
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_motmGetCharaFlags(arg0, MOTM_PoseUnknown3, SCEN_Write);
  return;
}

static void script_1A1FB(int32_t arg0) {
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_FACE(arg0, 0, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseUnknown3, SCEN_Write);
  return;
}

static int32_t script_1A276(void) {
  int16_t local_FE;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 228:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
      }
      local_FE = local_FE + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1A2DB(void) {
  int16_t local_FE;
  Vector3i local_F0;
  local_F0.x = -4551;
  (&local_F0)->y = 0;
  (&local_F0)->z = 0;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if ((local_FE >= 0) & (local_FE <= 70)) {
        Script_charaPosRotUtils(SHEN_NAME('FORK'), 8, 0, &local_F0, CHARA_PART_Add);
        Script_charaPosRotUtils(SHEN_NAME('FORK'), 23, 0, &local_F0, CHARA_PART_Add);
        Script_charaPosRotUtils(SHEN_NAME('FORK'), 18, 0, &local_F0, CHARA_PART_Add);
      } else {
        if ((local_FE >= 270) & (local_FE <= 600)) {
          Script_charaPosRotUtils(SHEN_NAME('FORK'), 8, 0, &local_F0, CHARA_PART_Add);
          Script_charaPosRotUtils(SHEN_NAME('FORK'), 23, 0, &local_F0, CHARA_PART_Add);
          Script_charaPosRotUtils(SHEN_NAME('FORK'), 18, 0, &local_F0, CHARA_PART_Add);
          goto L01A39D;
        }
      }
      L01A39D:;
      if ((local_FE >= 575) & (local_FE < 600)) {
        local_F0.x = (&local_F0)->x + 182;
      }
      if (local_FE >= 600) {
        Script_ForkliftUtils(0x4B524F46, FORK_CMD_RESET_DRIVE_INPUTS);
        Script_srchControl(0);
      }
      local_FE = local_FE + 1;
    } else {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1A3F3(void) {
  int16_t local_FE;
  float local_F8;
  Vector3 local_EC;
  float local_E0[3];
  local_F8 = 0.0257142857f;
  local_EC.x = 0.0f;
  (&local_EC)->z = 0.0f;
  local_E0[0] = 0.0f;
  local_E0[1] = 1.85f;
  local_E0[2] = -1.35f;
  local_FE = 0;
  (&local_EC)->y = 1.8f;
  Script_charaPosRotUtils(SHEN_NAME('FORK'), 3, &local_EC, 0, CHARA_PART_Set);
  (&local_EC)->y = (float)0 - local_F8;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if ((local_FE >= 0) & (local_FE <= 70)) {
        local_E0[1] = local_E0[1] - local_F8;
        Script_charaPosRotUtils(SHEN_NAME('FORK'), 3, &local_EC, 0, CHARA_PART_Add);
        local_FE = local_FE + 1;
      }
    } else {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1A4BA(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_94[26];
  int32_t local_90;
  script_0290();
  script_1A892();
  if (arg0) {
    Script_scenCreateTask(script_0385, 0);
  }
  Script_shenSeqUtils(0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_90 = local_FC;
    switch (local_90) {
      case 1:
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 120);
        Script_SetMaxInterpSpeed(0.0f);
        Script_SetMotnFlag(CHARA_SMTH, -1);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_FaceEx(CHARA_SMTH, FACE_ExpressionEnabled, 0, 1);
        Script_ResetHandMotion(CHARA_SMTH, 1, 0, 0);
        Script_ResetHandMotion(CHARA_SMTH, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_3191C, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_265EC, 1);
        Script_handSetFloatsFor(CHARA_SMTH, 0, data_265EC, 1);
        Script_handSetFloatsFor(CHARA_SMTH, 1, data_265EC, 1);
        break;
      case 110:
        Script_CharHandlers(CHARA_SMTH, 0, 0, 16);
        Script_handSetFloatsFor(CHARA_SMTH, 1, data_3191C, 1);
        break;
      case 220:
        Script_CharHandlers(CHARA_SMTH, 0, 8, 16);
        Script_handSetFloatsFor(CHARA_SMTH, 1, data_265EC, 1);
        break;
      case 120:
        Script_ReadModelFormat7(CHARA_SMTH, 1, data_31A30, 10);
        break;
      case 219:
        Script_CharHandlers(CHARA_SMTH, 1, 8, 16);
        break;
      case 320:
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_31A18);
        Script_CharHandlers(CHARA_SMTH, 1, 1, 16);
        break;
      case 340:
        Script_CharHandlers(CHARA_SMTH, 1, 8, 16);
        break;
      case 500:
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_31A24);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_3191C, 20);
        break;
    }
    if (local_FC == 2578) {
      break;
    }
    if (local_FC == 1) {
      Script_FACE(CHARA_SMTH, 7, 0, 10);
    }
    if (local_FC == 108) {
      Script_FACE(CHARA_AKIR, 2, 0, 10);
    }
    if (local_FC == 270) {
      Script_FACE(CHARA_SMTH, 7, 0, 10);
    }
    if (local_FC == 331) {
      Script_FACE(CHARA_SMTH, 11, 0, 10);
    }
    if (local_FC == 502) {
      Script_FACE(CHARA_AKIR, 5, 0, 11);
    }
    if (local_FC == 551) {
      Script_FACE(CHARA_AKIR, 2, 0, 15);
    }
    if (local_FC == 699) {
      Script_FACE(CHARA_AKIR, 2, 0, 10);
    }
    if (local_FC == 711) {
      Script_FACE(CHARA_SMTH, 3, 0, 10);
    }
    if (local_FC == 784) {
      Script_FACE(CHARA_SMTH, 3, 0, 10);
    }
    if (local_FC == 813) {
      Script_FACE(CHARA_SMTH, 4, 0, 10);
    }
    if (local_FC == 853) {
      Script_FACE(CHARA_AKIR, 2, 0, 13);
    }
    if (local_FC == 1309) {
      Script_FACE(CHARA_AKIR, 5, 0, 10);
    }
    if (local_FC == 1441) {
      Script_FACE(CHARA_AKIR, 2, 0, 16);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 2, 40);
  script_1A93F();
  return;
}

static void script_1A892(void) {
  Vector4 local_F0;
  local_F0.x = 1.0f;
  (&local_F0)->y = 0.3f;
  (&local_F0)->z = 0.3f;
  (&local_F0)->w = 0.3f;
  Script_LODSnapshotUtils(DISP_CMD_SaveDistancesAndCaptureLodState);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 150.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 150.0f);
  script_02A8(CHARA_AKIR);
  script_02A8(CHARA_SMTH);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_SMTH, 4);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_TONY, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_F0);
  Script_SetCharaRefBoardPos(CHARA_SMTH, &local_F0);
  return;
}

static void script_1A93F(void) {
  int32_t unused_F4[3];
  script_0335(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_0335(CHARA_SMTH);
  Script_SetGameStateTo(AUTH_None);
  Script_SetMaxInterpSpeed(0.5f);
  Script_LODSnapshotUtils(DISP_CMD_RestoreDistancesAndApplyLodState);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_SMTH, 2);
  return;
}

static void script_1A9A9(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t local_E8;
  script_0290();
  script_1AE54();
  if (arg0) {
    Script_scenCreateTask(script_0385, 0);
  }
  Script_shenSeqUtils(1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E8 = local_FC;
    switch (local_E8) {
      case 1:
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 120);
        Script_SetMaxInterpSpeed(0.0f);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_265EC, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_3191C, 1);
        Script_FaceEx(CHARA_TONY, FACE_ExpressionEnabled, 0, 1);
        Script_ResetHandMotion(CHARA_TONY, 1, 0, 0);
        Script_ResetHandMotion(CHARA_TONY, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_TONY, 0, data_265EC, 1);
        Script_handSetFloatsFor(CHARA_TONY, 1, data_265EC, 1);
        break;
      case 110:
        Script_CharHandlers(CHARA_TONY, 1, 0, 16);
        Script_handSetFloatsFor(CHARA_TONY, 0, data_3191C, 1);
        break;
      case 120:
        Script_ReadModelFormat7(CHARA_TONY, 0, data_31A30, 10);
        break;
      case 219:
        Script_CharHandlers(CHARA_TONY, 0, 8, 16);
        Script_handSetFloatsFor(CHARA_TONY, 1, data_265EC, 1);
        break;
      case 220:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 150.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 150.0f);
        Script_CharHandlers(CHARA_TONY, 1, 8, 16);
        Script_handSetFloatsFor(CHARA_TONY, 0, data_265EC, 1);
        break;
      case 320:
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_31A0C);
        Script_CharHandlers(CHARA_TONY, 0, 1, 16);
        break;
      case 340:
        Script_CharHandlers(CHARA_TONY, 0, 8, 16);
        break;
      case 490:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_3191C, 20);
        break;
      case 500:
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_31A24);
        local_EC.x = 34.726898f;
        (&local_EC)->y = 1.5662f;
        (&local_EC)->z = 85.914597f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 1);
        break;
      case 845:
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_FaceEx(CHARA_TONY, FACE_ExpressionDisabled, 0, 1);
        Script_ResetHandMotion(CHARA_TONY, 0, 0, 0);
        Script_ResetHandMotion(CHARA_TONY, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        break;
      case 885:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 150.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 150.0f);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
        break;
      case 1292:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_FaceEx(CHARA_TONY, FACE_ExpressionEnabled, 0, 1);
        Script_ResetHandMotion(CHARA_TONY, 1, 0, 0);
        Script_ResetHandMotion(CHARA_TONY, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_3191C, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_3191C, 0);
        Script_handSetFloatsFor(CHARA_TONY, 0, data_265EC, 1);
        Script_handSetFloatsFor(CHARA_TONY, 1, data_265EC, 1);
        Script_FACE(CHARA_TONY, 4, 0, 0);
        break;
      case 1460:
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_FaceEx(CHARA_TONY, FACE_ExpressionDisabled, 0, 1);
        Script_ResetHandMotion(CHARA_TONY, 0, 0, 0);
        Script_ResetHandMotion(CHARA_TONY, 0, 1, 0);
        break;
    }
    if (local_FC == 2578) {
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 2, 40);
  script_1AF0E();
  return;
}

static void script_1AE54(void) {
  Vector4 local_F0;
  local_F0.x = 1.0f;
  (&local_F0)->y = 0.3f;
  (&local_F0)->z = 0.3f;
  (&local_F0)->w = 0.3f;
  Script_LODSnapshotUtils(DISP_CMD_SaveDistancesAndCaptureLodState);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 5.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 5.0f);
  script_02A8(CHARA_AKIR);
  script_02A8(CHARA_TONY);
  Script_facemotUtils(CHARA_TONY, 1, 0);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_TONY, 4);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_TONY, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_F0);
  Script_SetCharaRefBoardPos(CHARA_TONY, &local_F0);
  return;
}

static void script_1AF0E(void) {
  int32_t unused_F4[3];
  script_0335(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_0335(CHARA_TONY);
  Script_SetGameStateTo(AUTH_None);
  Script_SetMaxInterpSpeed(0.5f);
  Script_LODSnapshotUtils(DISP_CMD_RestoreDistancesAndApplyLodState);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_TONY, 2);
  return;
}

__attribute__((scn_string_bindings(0, data_31A58, 1, data_31A5D, 2, data_31A62)))
static void script_1AF78(void) {
  int32_t local_FC;
  while (1) {
    script_22ABE();
    Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 255, 0, 0, 0);
    Script_LoadSND(0, 5, "FREE", 0);
    Script_LoadSND(0, 4, "FREE", 0);
    Script_LoadSND(0, 3, "FREE", 0);
    Script_LoadSND(0, 3, "f1yokosu.snd", 0);
    Script_LoadSND(0, 4, "e1tonysm.snd", 0);
    Script_LoadSND(0, 5, "qtebat.snd", 0);
    Script_executeAicaCommand(2984, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
    Script_motmSetBlendFrames(CHARA_TONY, MOTM_BLEND_FRAMES_5);
    Script_motmSetBlendFrames(CHARA_SMTH, MOTM_BLEND_FRAMES_5);
    Script_motmSetBlendFrames(CHARA_PAUR, MOTM_BLEND_FRAMES_5);
    Script_motmSetBlendFrames(CHARA_BOB_, MOTM_BLEND_FRAMES_5);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
    script_18DF6(SHEN_NAME('CAR4'), 0, 0);
    script_1B2C5();
    script_1B894();
    script_1BFF5();
    script_1C55F();
    script_1D2D2();
    script_1DE56();
    local_FC = task_398;
    switch (local_FC) {
      case 1:
        script_1EBC9();
        script_20D30();
        break;
      case 2:
        script_1E583();
        script_1F5CB();
        script_1FE1C();
        break;
    }
    script_21C3D();
    Script_executeAicaCommand(2720, 2, 30);
    if ((task_398 == 0) | (task_3B1 > task_3E4 + 3)) {
      script_229B7();
    } else {
      script_1946D();
      break;
    }
  }
  Script_HATO_Funcs(FreeTask);
  Script_executeAicaCommand(0x1300A0, 4, 0);
  Script_FaceEx(CHARA_TONY, FACE_ExpressionDisabled, 0, 2);
  Script_faceUpdateFor(CHARA_TONY, 0, 0);
  Script_faceUpdateFor(CHARA_TONY, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_FaceEx(CHARA_SMTH, FACE_ExpressionDisabled, 0, 2);
  Script_faceUpdateFor(CHARA_SMTH, 0, 0);
  Script_faceUpdateFor(CHARA_SMTH, 1, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_1B1A5(void) {
  Script_FACE(CHARA_AKIR, 8, 0, 15);
  SCNF_WaitFrames_0000(450);
  Script_FACE(CHARA_AKIR, 2, 4, 15);
  SCNF_WaitFrames_0000(450);
  Script_FACE(CHARA_AKIR, 0, -1, 15);
  SCNF_WaitFrames_0000(450);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  return Script_srchControl(0);
}

static int32_t script_1B205(void) {
  Script_FACE(CHARA_AKIR, 2, 2, 15);
  SCNF_WaitFrames_0000(450);
  Script_FACE(CHARA_AKIR, 4, 4, 10);
  SCNF_WaitFrames_0000(300);
  Script_FACE(CHARA_AKIR, 0, -1, 15);
  SCNF_WaitFrames_0000(450);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  return Script_srchControl(0);
}

static int32_t script_1B265(void) {
  Script_FACE(CHARA_AKIR, 8, 0, 30);
  SCNF_WaitFrames_0000(900);
  Script_FACE(CHARA_AKIR, 2, 2, 10);
  SCNF_WaitFrames_0000(300);
  Script_FACE(CHARA_AKIR, 0, -1, 15);
  SCNF_WaitFrames_0000(450);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  return Script_srchControl(0);
}

static void script_1B2C5(void) {
  int32_t local_FC;
  script_23678();
  if (0) {
    Script_charaDisplay(SHEN_NAME('DORA'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('DORA'), CDM_Show);
      goto L01B2F4;
    }
  }
  L01B2F4:;
  Script_scenToggleVisibleFor(SHEN_NAME('DORA'), 1);
  Script_charaDisplay(CHARA_PAUR, CDM_Hide);
  Script_charaDisplay(CHARA_BOB_, CDM_Hide);
  script_19495(-10);
  {
    int32_t unused_F8;
    Vector3 local_EC;
    local_EC.x = -92.669601f;
    (&local_EC)->z = 47.646198f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_EC);
  }
  {
    int32_t unused_F8;
    Vector3i local_EC;
    (&local_EC)->y = 56957;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_EC, 0);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_3B1 = 0;
  script_18CCA(SHEN_NAME('DORA'), 0, 0);
  Script_charaDisplay(SHEN_NAME('DORA'), CDM_Show);
  script_19495(0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_scenCreateTask(script_18CCA, 3, SHEN_NAME('DORA'), 65, -1);
  Script_scenCreateTask(script_191DC, 2, 66729, 66);
  Script_scenCreateTask(script_1B665, 1, CHARA_SMTH);
  Script_scenCreateTask(script_1B7A0, 1, CHARA_TONY);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_185A8(CHARA_AKIR, -86.1091f, 40.705601f, 120, 0.5f);
  script_1861F(CHARA_AKIR, -78.14653f, 44.11f, 30);
  local_FC = script_227FE(64, 20);
  script_1B4B1(local_FC);
  Script_charaDisplay(SHEN_NAME('DORA'), CDM_Hide);
  return;
}

static void script_1B4B1(int32_t arg0) {
  if (arg0 & 0x40) {
    script_19495(1);
    SCNF_WaitFrames_0000(24 - task_39C);
    Script_scenCreateTask(script_19200, 2, 6, 3);
    script_18FCF(CHARA_AKIR, 28692);
    Script_scenCreateTask(script_191DC, 2, 0x2076AB, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
    script_181A0(CHARA_AKIR);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  } else {
    script_19495(2);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    {
      Vector3 local_F4;
      local_F4.x = -80.118599f;
      (&local_F4)->z = 43.081299f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
    }
    {
      Vector3i local_F4;
      (&local_F4)->y = 44101;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
    }
    Script_scenCreateTask(script_19200, 2, 2, 1);
    script_18FCF(CHARA_AKIR, 28691);
    Script_scenCreateTask(script_191DC, 2, 0x2276AB, 0);
    script_1821E(CHARA_AKIR, 55.0f);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_18FCF(CHARA_AKIR, 28692);
    script_181A0(CHARA_AKIR);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    task_3B1 = task_3B1 + 1;
  }
  return;
}

static int32_t script_1B665(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetCharaMotionPlaybackRate(arg0, 0.9f);
  {
    Vector3 local_F4;
    local_F4.x = -85.660202f;
    (&local_F4)->z = 42.334f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 59063;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  script_185A8(arg0, -78.143799f, 43.642601f, 20, 1.0f);
  {
    Vector3i local_F4;
    (&local_F4)->y = 41380;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(arg0, 28739, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_181A0(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_185A8(arg0, -72.008598f, 49.326f, 20, 1.0f);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static int32_t script_1B7A0(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  {
    Vector3 local_F4;
    local_F4.x = -88.214996f;
    (&local_F4)->z = 42.582401f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 56335;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  script_185A8(arg0, -81.0187f, 39.636902f, 40, 2.0f);
  script_185A8(arg0, -77.444603f, 42.1143f, 50, 1.0f);
  script_185A8(arg0, -69.266602f, 49.984001f, 30, 1.0f);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_1B894(void) {
  int32_t local_FC;
  script_23678();
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASK'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASK'), CDM_Show);
      goto L01B8C3;
    }
  }
  L01B8C3:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASK'), 1);
  script_1BB49();
  script_19495(3);
  Script_scenCreateTask(script_1BDCB, 1, CHARA_TONY);
  Script_scenCreateTask(script_1BEB0, 1, CHARA_SMTH);
  script_185A8(CHARA_AKIR, -70.154503f, 48.93f, 20, 5.0f);
  local_FC = script_227FE(0x200, 20);
  script_1B935(local_FC);
  return;
}

static void script_1B935(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (arg0 & 0x200) {
    SCNF_WaitFrames_0000(5);
    local_F4.x = -71.6185f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 47.1315f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F4);
    script_19495(4);
    Script_charaDisplay(CHARA_SMTH, CDM_Hide);
    (&local_E8)->y = 50958;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
    local_F4.x = 1.3f;
    (&local_F4)->y = 10.0f;
    (&local_F4)->z = 1.3f;
    Script_scenCreateTask(script_19200, 2, 1, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2976AB, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2A76AB, 0x42080000);
    script_18F0E(CHARA_AKIR, 28696, 34.0f, &local_F4);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_185A8(CHARA_AKIR, -57.514099f, 45.6017f, 30, 1.0f);
  } else {
    script_185A8(CHARA_AKIR, -69.3573f, 49.904499f, 10, 0.3f);
    script_19495(5);
    Script_charaDisplay(CHARA_SMTH, CDM_Hide);
    SCNF_WaitFrames_0000(6);
    Script_PlayCharaMotion(CHARA_AKIR, 28691, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 4, 1);
    Script_scenCreateTask(script_191DC, 2, 0x2276AB, 0);
    script_1821E(CHARA_AKIR, 55.0f);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_seqEnterEvtCam(5904, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
    script_185A8(CHARA_AKIR, -57.514099f, 45.6017f, 40, 1.0f);
    task_3B1 = task_3B1 + 1;
  }
  return;
}

static void script_1BB49(void) {
  {
    Vector3 local_F4;
    local_F4.x = -66.966499f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 56.713501f;
    Script_EVT_SetPosition(SHEN_NAME('BO_1'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 24556;
    Script_EVT_SetRotation(SHEN_NAME('BO_1'), MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(SHEN_NAME('BO_1'), CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = -66.048203f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 55.771801f;
    Script_EVT_SetPosition(SHEN_NAME('BO_2'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 57435;
    Script_EVT_SetRotation(SHEN_NAME('BO_2'), MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(SHEN_NAME('BO_2'), CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = -67.945099f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 55.667801f;
    Script_EVT_SetPosition(SHEN_NAME('BO_3'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 57253;
    Script_EVT_SetRotation(SHEN_NAME('BO_3'), MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(SHEN_NAME('BO_3'), CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = -66.966499f;
    (&local_F4)->y = 1.2f;
    (&local_F4)->z = 56.713501f;
    Script_EVT_SetPosition(SHEN_NAME('BO_4'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 24556;
    Script_EVT_SetRotation(SHEN_NAME('BO_4'), MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(SHEN_NAME('BO_4'), CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = -67.945099f;
    (&local_F4)->y = 1.2f;
    (&local_F4)->z = 55.667801f;
    Script_EVT_SetPosition(SHEN_NAME('BO_5'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 57253;
    Script_EVT_SetRotation(SHEN_NAME('BO_5'), MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(SHEN_NAME('BO_5'), CDM_Show);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -65.103203f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 54.7948f;
    Script_EVT_SetPosition(SHEN_NAME('BO_6'), POS_XYZ, &local_F4);
    (&local_E8)->y = 57253;
    Script_EVT_SetRotation(SHEN_NAME('BO_6'), MATRIX_XFORM_Y, &local_E8, 0);
    Script_charaDisplay(SHEN_NAME('BO_6'), CDM_Show);
    script_188A8(SHEN_NAME('IKN0'), 0, 0);
    Script_charaDisplay(SHEN_NAME('IKN0'), CDM_Show);
    local_F4.x = 0.7f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 1.3f;
  }
  Script_scenCreateTask(script_188A8, 3, SHEN_NAME('IKN0'), 18, 1);
  Script_scenCreateTask(script_19038, 3, 45, SHEN_NAME('IKN0'), 31);
  Script_scenCreateTask(script_191DC, 2, 0x1304A9, 18);
  return;
}

static int32_t script_1BDCB(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = -67.632698f;
    (&local_F4)->z = 51.295601f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 41547;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(arg0, CDM_Show);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetCharaMotionPlaybackRate(arg0, 1.1f);
  script_185A8(arg0, -64.498001f, 53.092999f, 50, 1.0f);
  script_185A8(arg0, -60.1194f, 46.826199f, 30, 1.0f);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static int32_t script_1BEB0(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = -70.985001f;
    (&local_F4)->z = 51.488201f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 41446;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(arg0, CDM_Show);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetCharaMotionPlaybackRate(arg0, 1.6f);
  script_185A8(arg0, -66.770897f, 53.1898f, 30, 0.4f);
  script_185A8(arg0, -63.453701f, 53.094002f, 3, 0.5f);
  Script_PlayCharaMotion(arg0, 28740, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_181A0(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_185A8(arg0, -59.655701f, 48.553101f, 30, 3.0f);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_1BFF5(void) {
  int32_t local_FC;
  script_23678();
  if (0) {
    Script_charaDisplay(SHEN_NAME('PIP0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('PIP0'), CDM_Show);
      goto L01C024;
    }
  }
  L01C024:;
  Script_scenToggleVisibleFor(SHEN_NAME('PIP0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('PIP1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('PIP1'), CDM_Show);
      goto L01C058;
    }
  }
  L01C058:;
  Script_scenToggleVisibleFor(SHEN_NAME('PIP1'), 1);
  script_18A4C(SHEN_NAME('PIP0'), 0, 0);
  Script_charaDisplay(SHEN_NAME('PIP0'), CDM_Show);
  script_18A4C(SHEN_NAME('PIP1'), 0, 0);
  Script_charaDisplay(SHEN_NAME('PIP1'), CDM_Show);
  script_19495(6);
  Script_scenCreateTask(script_1C369, 1, CHARA_TONY);
  Script_scenCreateTask(script_1C434, 1, CHARA_SMTH);
  Script_scenCreateTask(script_19200, 2, 23, 10);
  script_185A8(CHARA_AKIR, -56.385899f, 44.450699f, 20, 1.0f);
  script_1861F(CHARA_AKIR, -51.5863f, 44.705002f, 10);
  local_FC = script_227FE(0x200, 15);
  script_1C122(local_FC);
  return;
}

static void script_1C122(int32_t arg0) {
  script_185A8(CHARA_AKIR, -51.167198f, 45.553001f, 5, 0.5f);
  {
    Vector3i local_F4;
    (&local_F4)->y = 41299;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_scenCreateTask(script_18A4C, 3, SHEN_NAME('PIP0'), 10, 1);
  Script_scenCreateTask(script_19038, 3, 30, SHEN_NAME('PIP0'), 32);
  Script_scenCreateTask(script_191DC, 2, 0x1404A9, 13);
  Script_scenCreateTask(script_18B8B, 3, SHEN_NAME('PIP1'), 16, 1);
  Script_scenCreateTask(script_19038, 3, 36, SHEN_NAME('PIP1'), 32);
  if (arg0 & 0x200) {
    Vector3 local_F4;
    local_F4.x = -52.234402f;
    (&local_F4)->z = 44.190498f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
    script_19495(7);
    Script_PlayCharaMotion(CHARA_AKIR, 28738, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 6, 3);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.5f);
    script_1821E(CHARA_AKIR, 53.0f);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.2f);
    script_18FCF(CHARA_AKIR, 28692);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.6f);
    SCNF_WaitFrames_0000(20);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  } else {
    script_19495(8);
    Script_PlayCharaMotion(CHARA_AKIR, 28736, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 2, 5);
    Script_scenCreateTask(script_19200, 2, 4, 30);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
    script_1821E(CHARA_AKIR, 50.0f);
    Script_SetCamShake(0.5f, 0.5f, 0.7f);
    Script_SetCamShake(0.5f, 0.5f, 0.7f);
    script_1821E(CHARA_AKIR, 90.0f);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
    task_3B1 = task_3B1 + 1;
  }
  return;
}

static int32_t script_1C369(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = -49.1749f;
    (&local_F4)->z = 45.040401f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 42166;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(arg0, CDM_Show);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
  script_185A8(arg0, -45.346401f, 48.936901f, 30, 1.0f);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static int32_t script_1C434(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = -52.4132f;
    (&local_F4)->z = 45.327099f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 41446;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(arg0, CDM_Show);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetCharaMotionPlaybackRate(arg0, 0.9f);
  script_185A8(arg0, -49.506199f, 48.933899f, 30, 0.5f);
  Script_PlayCharaMotion(arg0, 28740, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_181A0(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_185A8(arg0, -46.436001f, 50.780899f, 30, 1.0f);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_1C55F(void) {
  script_23678();
  if (0) {
    Script_charaDisplay(CHARA_BOB_, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_BOB_, CDM_Show);
      goto L01C58E;
    }
  }
  L01C58E:;
  Script_scenToggleVisibleFor(CHARA_BOB_, 1);
  if (0) {
    Script_charaDisplay(CHARA_PAUR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_PAUR, CDM_Show);
      goto L01C5C2;
    }
  }
  L01C5C2:;
  Script_scenToggleVisibleFor(CHARA_PAUR, 1);
  task_3B0 = 0;
  script_1CD2E();
  {
    int32_t local_FC;
    local_FC = script_227FE(64, 12);
    script_1C6D5(local_FC);
  }
  script_19495(40);
  {
    int32_t local_FC;
    local_FC = script_227FE(0x100, 15);
    script_1C992(local_FC);
  }
  script_185A8(CHARA_AKIR, -38.39f, 58.072399f, 30, 5.0f);
  return;
}

static void script_1C634(int32_t arg0) {
  script_181A0(arg0);
  script_18FCF(arg0, 785);
  Script_srchControl(0);
  return;
}

static int32_t script_1C652(int32_t arg0) {
  Vector3i local_F4;
  script_1821E(arg0, 50.0f);
  script_18FCF(arg0, 28752);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  (&local_F4)->y = 57965;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  return Script_srchControl(0);
}

static int32_t script_1C6A4(int32_t arg0) {
  script_181A0(arg0);
  Script_PlayCharaMotion(arg0, 28711, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_1C6D5(int32_t arg0) {
  Vector3i local_F4;
  (&local_F4)->y = 41096;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  if (arg0 & 0x40) {
    script_19495(38);
    SCNF_WaitFrames_0000(17 - task_39C);
    script_18FCF(CHARA_AKIR, 28692);
    Script_scenCreateTask(script_191DC, 2, 0x2076AB, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
    script_1821E(CHARA_AKIR, 4.0f);
    Script_motmToggleFlags(CHARA_PAUR, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(CHARA_PAUR, 28749, 15.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_PAUR, 0x2276AB, 0x40A00000);
    Script_scenCreateTask(script_1C6A4, 1, CHARA_PAUR);
    task_3B0 = 1;
  } else {
    script_19495(39);
    SCNF_WaitFrames_0000(17 - task_39C);
    Script_motmToggleFlags(CHARA_PAUR, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(CHARA_PAUR, 28749, 15.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_PAUR, 0x2276AB, 0x40A00000);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(CHARA_AKIR, 28750, 10.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.4f);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2576AB, 0x41700000);
    Script_scenCreateTask(script_1C652, 1, CHARA_PAUR);
    script_1821E(CHARA_AKIR, 106.0f);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
    task_3B1 = task_3B1 + 1;
  }
  return;
}

static int32_t script_1C8CA(int32_t arg0) {
  Vector3i local_F4;
  script_1821E(arg0, 20.0f);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, MN_AKIRA_KAMAE_TURN, 10.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetCharaMotionPlaybackRate(arg0, 1.85f);
  script_1821E(arg0, 30.0f);
  (&local_F4)->y = 41567;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  script_18FCF(arg0, 785);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  end:;
}

static int32_t script_1C961(int32_t arg0) {
  script_181A0(arg0);
  Script_PlayCharaMotion(arg0, 28754, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_1C992(int32_t arg0, ...) {
  {
    va_list arguments0;
    va_start(arguments0, arg0);
    if (va_arg(arguments0, int32_t) & 0x100) {
      script_19495(41);
      script_181A0(CHARA_AKIR);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
      {
        Vector3 local_F4;
        local_F4.x = -43.695999f;
        (&local_F4)->z = 51.237099f;
        Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
      }
      Script_seqEnterEvtCam(5816, 0, AUTH_None);
      script_18FCF(CHARA_AKIR, 1007);
      Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2576AB, 0x41200000);
      script_1821E(CHARA_AKIR, 5.0f);
      Script_PlayCharaMotion(CHARA_BOB_, 28751, 25.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_scenCreateTask(script_18FFB, 3, CHARA_BOB_, 0x2776AB, 0x42200000);
      Script_scenCreateTask(script_1C961, 1, CHARA_BOB_);
      {
        Vector3i local_F4;
        (&local_F4)->y = 60074;
        Script_EVT_SetRotation(CHARA_BOB_, MATRIX_XFORM_Y, &local_F4, 0);
      }
      Script_SetCharaMotionPlaybackRate(CHARA_BOB_, 0.9f);
      script_1821E(CHARA_AKIR, 47.0f);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    } else {
      script_19495(42);
      {
        Vector3i local_F4;
        (&local_F4)->y = 1194;
        Script_EVT_SetRotation(CHARA_BOB_, MATRIX_XFORM_Y, &local_F4, 0);
      }
      {
        Vector3 local_F4;
        local_F4.x = -42.235401f;
        (&local_F4)->z = 52.941002f;
        Script_EVT_SetPosition(CHARA_BOB_, 0x28000000, &local_F4);
      }
      Script_PlayCharaMotion(CHARA_BOB_, 28753, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_scenCreateTask(script_18FFB, 3, CHARA_BOB_, 0x2276AB, 0x40A00000);
      Script_scenCreateTask(script_1C8CA, 1, CHARA_BOB_);
      {
        Vector3 local_F4;
        local_F4.x = -43.695999f;
        (&local_F4)->z = 51.237099f;
        Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
      }
      Script_PlayCharaMotion(CHARA_AKIR, 28685, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.2f);
      Script_scenCreateTask(script_18FFB, 3, CHARA_BOB_, 0x2576AB, 0x41A00000);
      script_1821E(CHARA_AKIR, 18.0f);
      Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
      Script_PlayCharaMotion(CHARA_AKIR, 28686, 90.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.2f);
      Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2776AB, 0x42DC0000);
      Script_seqEnterEvtCam(5913, 0, AUTH_None);
      script_1821E(CHARA_AKIR, 115.0f);
      Script_SetCamShake(0.3f, 0.3f, 0.4f);
      script_1821E(CHARA_AKIR, 128.0f);
      Script_PlayCharaMotion(CHARA_AKIR, 28698, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
      script_181A0(CHARA_AKIR);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
      task_3B1 = task_3B1 + 1;
    }
    va_end(arguments0);
  }
  return;
}

static void script_1CCFE(int32_t arg0) {
  script_181A0(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchControl(0);
  return;
}

static void script_1CD2E(void) {
  if (0) {
    Script_charaDisplay(CHARA_BOB_, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_BOB_, CDM_Show);
      goto L01CD58;
    }
  }
  L01CD58:;
  Script_scenToggleVisibleFor(CHARA_BOB_, 1);
  if (0) {
    Script_charaDisplay(CHARA_PAUR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_PAUR, CDM_Show);
      goto L01CD8C;
    }
  }
  L01CD8C:;
  Script_scenToggleVisibleFor(CHARA_PAUR, 1);
  Script_PlayCharaMotion(CHARA_BOB_, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = -45.686199f;
    (&local_E8)->z = 53.844299f;
    Script_EVT_SetPosition(CHARA_BOB_, 0x28000000, &local_E8);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 59468;
    Script_EVT_SetRotation(CHARA_BOB_, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(CHARA_BOB_, CDM_Show);
  Script_motmSetBlendFrames(CHARA_BOB_, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_charaToggleMotEnabled(CHARA_BOB_, 1);
  Script_motmToggleFlags(CHARA_BOB_, SCEN_Enable, 4);
  Script_PlayCharaMotion(CHARA_BOB_, 28746, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    Vector3 local_F4;
    local_F4.x = -46.746399f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 52.957199f;
    Script_PlayCharaMotion(CHARA_PAUR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_EVT_SetPosition(CHARA_PAUR, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 59468;
    Script_EVT_SetRotation(CHARA_PAUR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(CHARA_PAUR, CDM_Show);
  Script_motmSetBlendFrames(CHARA_PAUR, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_charaToggleMotEnabled(CHARA_PAUR, 1);
  Script_motmToggleFlags(CHARA_PAUR, SCEN_Enable, 4);
  Script_PlayCharaMotion(CHARA_PAUR, 28746, 15.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = -44.112202f;
    (&local_E8)->z = 52.9832f;
    Script_EVT_SetPosition(CHARA_SMTH, 0x28000000, &local_E8);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 41071;
    Script_EVT_SetRotation(CHARA_SMTH, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(CHARA_SMTH, CDM_Show);
  Script_charaToggleMotEnabled(CHARA_SMTH, 1);
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = -43.833099f;
    (&local_E8)->z = 51.477699f;
    Script_EVT_SetPosition(CHARA_TONY, 0x28000000, &local_E8);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 41071;
    Script_EVT_SetRotation(CHARA_TONY, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(CHARA_TONY, CDM_Show);
  Script_charaToggleMotEnabled(CHARA_TONY, 1);
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = -50.428699f;
    (&local_E8)->z = 45.9664f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_E8);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 40538;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_18FCF(CHARA_TONY, 28715);
  Script_PlayCharaMotion(CHARA_PAUR, 28742, 9.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmSetBlendFrames(CHARA_BOB_, MOTM_BLEND_FRAMES_5);
  Script_motmSetBlendFrames(CHARA_PAUR, MOTM_BLEND_FRAMES_5);
  script_19495(37);
  Script_scenCreateTask(script_19200, 2, 8, 0);
  Script_scenCreateTask(script_19200, 2, 11, 50);
  Script_motmToggleFlags(CHARA_BOB_, SCEN_Disable, 4);
  script_1821E(CHARA_TONY, 11.0f);
  script_1821E(CHARA_TONY, 18.0f);
  Script_motmToggleFlags(CHARA_PAUR, SCEN_Disable, 4);
  Script_PlayCharaMotion(CHARA_PAUR, 28742, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_PAUR, 1.3f);
  script_1821E(CHARA_TONY, 40.0f);
  script_181A0(CHARA_TONY);
  Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(2);
  SCNF_WaitFrames_0000(2);
  SCNF_WaitFrames_0000(15);
  {
    Vector3 local_F4;
    local_F4.x = -45.190601f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 52.2318f;
    script_18FCF(CHARA_PAUR, 785);
    Script_SetCharaMotionPlaybackRate(CHARA_PAUR, 1.0f);
    Script_EVT_SetPosition(CHARA_PAUR, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 8809;
    Script_EVT_SetRotation(CHARA_PAUR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  script_18FCF(CHARA_BOB_, 786);
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = -41.808498f;
    (&local_E8)->z = 52.487701f;
    Script_EVT_SetPosition(CHARA_BOB_, 0x28000000, &local_E8);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 13026;
    Script_EVT_SetRotation(CHARA_BOB_, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(CHARA_BOB_, CDM_Show);
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = -50.428699f;
    (&local_E8)->z = 45.9664f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_E8);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 40538;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  SCNF_WaitFrames_0000(16);
  Script_charaDisplay(CHARA_TONY, CDM_Hide);
  Script_charaDisplay(CHARA_SMTH, CDM_Hide);
  return;
}

static void script_1D2D2(void) {
  int32_t local_FC;
  script_23678();
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Show);
      goto L01D301;
    }
  }
  L01D301:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR3'), CDM_Show);
      goto L01D335;
    }
  }
  L01D335:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Show);
      goto L01D369;
    }
  }
  L01D369:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR4'), 1);
  if (0) {
    Script_charaDisplay(CHARA_BOB_, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_BOB_, CDM_Show);
      goto L01D39D;
    }
  }
  L01D39D:;
  Script_scenToggleVisibleFor(CHARA_BOB_, 1);
  if (0) {
    Script_charaDisplay(CHARA_PAUR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_PAUR, CDM_Show);
      goto L01D3D1;
    }
  }
  L01D3D1:;
  Script_scenToggleVisibleFor(CHARA_PAUR, 1);
  Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Hide);
  {
    Vector3 local_F0;
    local_F0.x = -4.975f;
    (&local_F0)->z = 98.071602f;
    Script_EVT_SetPosition(SHEN_NAME('CAR1'), 0x28000000, &local_F0);
  }
  {
    Vector3i local_F0;
    (&local_F0)->y = 62172;
    Script_EVT_SetRotation(SHEN_NAME('CAR1'), MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  {
    Vector3 local_F0;
    local_F0.x = -20.3923f;
    (&local_F0)->z = 75.295998f;
    Script_EVT_SetPosition(SHEN_NAME('CAR0'), 0x28000000, &local_F0);
  }
  {
    Vector3i local_F0;
    (&local_F0)->y = 8505;
    Script_EVT_SetRotation(SHEN_NAME('CAR0'), MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Show);
  Script_scenCreateTask(script_1D736, 0);
  Script_scenCreateTask(script_1D81D, 0);
  script_19495(9);
  Script_scenCreateTask(script_1B1A5, 0);
  script_185A8(CHARA_AKIR, -31.7929f, 64.3694f, 30, 0.5f);
  {
    Vector3i local_F0;
    (&local_F0)->y = 41145;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
  }
  if (-1) {
    Script_charaDisplay(CHARA_BOB_, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_BOB_, CDM_Show);
      goto L01D50F;
    }
  }
  L01D50F:;
  Script_scenToggleVisibleFor(CHARA_BOB_, 0);
  if (-1) {
    Script_charaDisplay(CHARA_PAUR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_PAUR, CDM_Show);
      goto L01D543;
    }
  }
  L01D543:;
  Script_scenToggleVisibleFor(CHARA_PAUR, 0);
  local_FC = script_22782(50);
  script_1D56C(local_FC);
  script_234B8();
  return;
}

static void script_1D56C(int32_t arg0) {
  Vector3i local_F4;
  if (arg0 & 4) {
    script_19495(10);
    script_185A8(CHARA_AKIR, -23.547701f, 73.756302f, 60 - task_39C, 0.2f);
    task_3E0 = Script_scenCreateTask(script_1DCE0, 1, 5);
    Script_seqEnterEvtCam(5920, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
    script_185A8(CHARA_AKIR, -19.9063f, 77.6745f, 10, 0.2f);
    script_185A8(CHARA_AKIR, -11.4318f, 82.251503f, 90, 0.5f);
  } else if (arg0 & 0x40) {
    script_19495(11);
    script_185A8(CHARA_AKIR, -19.947701f, 72.765404f, 70 - task_39C, 0.2f);
    task_3E0 = Script_scenCreateTask(script_1DCE0, 1, 0);
    task_3A4 = 0.8f;
    script_1D909();
  } else {
    script_19495(12);
    task_3E0 = Script_scenCreateTask(script_1DCE0, 1, 60);
    Script_scenCreateTask(script_19200, 2, 4, 2);
    Script_PlayCharaMotion(CHARA_AKIR, 28680, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_1821E(CHARA_AKIR, 55.0f);
    Script_seqEnterEvtCam(5905, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_185A8(CHARA_AKIR, -19.947701f, 72.765404f, 20, 0.2f);
    (&local_F4)->y = 42427;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
    script_1D909();
    task_3B1 = task_3B1 + 1;
  }
  return;
}

static int32_t script_1D736(void) {
  Script_charaDisplay(CHARA_TONY, CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = -33.3255f;
    (&local_F4)->z = 63.071899f;
    Script_EVT_SetPosition(CHARA_TONY, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 40758;
    Script_EVT_SetRotation(CHARA_TONY, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_22976, 3, CHARA_TONY, CHARA_SMTH, 200);
  script_185A8(CHARA_TONY, -31.0882f, 64.648201f, 60, 0.5f);
  script_185A8(CHARA_TONY, -20.8517f, 72.106499f, 60, 0.5f);
  script_185A8(CHARA_TONY, -10.4764f, 81.306f, 60, 0.5f);
  return Script_srchControl(0);
}

static int32_t script_1D81D(void) {
  Script_charaDisplay(CHARA_SMTH, CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = -33.132599f;
    (&local_F4)->z = 64.160301f;
    Script_EVT_SetPosition(CHARA_SMTH, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 41695;
    Script_EVT_SetRotation(CHARA_SMTH, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_185A8(CHARA_SMTH, -30.731701f, 66.161903f, 60, 0.5f);
  script_185A8(CHARA_SMTH, -22.466499f, 75.09f, 60, 0.5f);
  script_185A8(CHARA_SMTH, -12.0063f, 81.1399f, 60, 0.5f);
  script_185A8(CHARA_SMTH, -10.1969f, 82.790703f, 30, 0.5f);
  return Script_srchControl(0);
}

static void script_1D909(void) {
  int32_t local_FC;
  script_19495(13);
  Script_scenCreateTask(script_191DC, 2, 0x1004A9, 10);
  script_185A8(CHARA_AKIR, -19.4228f, 73.76825f, 10, 0.5f);
  script_1861F(CHARA_AKIR, -17.1252f, 75.748596f, 30);
  local_FC = script_227FE(64, 15);
  script_1DA80(local_FC);
  return;
}

static int32_t script_1D975(void) {
  while (1) {
    task_3A4 = task_3A4 + 0.03f;
    if (!(task_3A4 < 0.5f)) {
      break;
    }
  }
  return Script_srchControl(0);
}

static int32_t script_1D9A5(void) {
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
      goto L01D9CF;
    }
  }
  L01D9CF:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR2'), 1);
  {
    Vector3 local_F4;
    local_F4.x = -11.2462f;
    (&local_F4)->z = 83.03f;
    Script_EVT_SetPosition(SHEN_NAME('CAR2'), 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 5349;
    Script_EVT_SetRotation(SHEN_NAME('CAR2'), MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
  task_3A4 = 0.5f;
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('CAR2'), &data_31A8C, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('CAR2'), task_3A4 / (float)2) == -1) {
      break;
    }
  }
  return Script_srchControl(0);
}

static void script_1DA80(int32_t arg0) {
  Vector3 local_F4;
  task_3A4 = 0.5f;
  Script_scenCreateTask(script_19200, 2, 7, 10);
  if (arg0 & 0x40) {
    script_19495(14);
    Script_PlayCharaMotion(CHARA_AKIR, 28700, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 3, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.5f);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2076AB, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2776AB, 0x41A00000);
    script_181A0(CHARA_AKIR);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  } else {
    script_19495(15);
    Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Hide);
    Script_scenCreateTask(script_1D9A5, 0);
    if (0) {
      Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
        goto L01DBA2;
      }
    }
    L01DBA2:;
    Script_scenToggleVisibleFor(SHEN_NAME('CAR2'), 1);
    Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
    Script_scenCreateTask(script_191DC, 2, 197801, 0);
    Script_scenCreateTask(script_191DC, 2, 1193, 0);
    local_F4.x = -14.7804f;
    (&local_F4)->z = 78.658897f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
    Script_PlayCharaMotion(CHARA_AKIR, 28679, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_1821E(CHARA_AKIR, 5.0f);
    task_3A4 = 0.0f;
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2276AB, 0);
    Script_scenCreateTask(script_19200, 2, 0, 30);
    script_181A0(CHARA_AKIR);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_191DC, 2, 132265, 0);
    Script_scenCreateTask(script_1DDC8, 1, 0);
    Script_scenCreateTask(script_1D975, 0);
    task_3B1 = task_3B1 + 1;
  }
  script_185A8(CHARA_AKIR, -8.2868f, 82.634003f, 90, 0.5f);
  return;
}

static int32_t script_1DCE0(int32_t arg0) {
  {
    int32_t unused_E8[6];
    Vector3 local_DC;
    local_DC.x = -4.975f;
    (&local_DC)->z = 98.071602f;
    Script_EVT_SetPosition(SHEN_NAME('CAR1'), 0x28000000, &local_DC);
  }
  {
    int32_t unused_E8[6];
    Vector3i local_DC;
    (&local_DC)->y = 62172;
    Script_EVT_SetRotation(SHEN_NAME('CAR1'), MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Show);
  task_3A4 = 0.5f;
  SCNF_WaitFrames_0000(arg0);
  Script_scenCreateTask(script_191DC, 2, 132265, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('CAR1'), &data_31AA0, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('CAR1'), task_3A4) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 197801, 0);
  Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Hide);
  script_23642(SHEN_NAME('CAR1'));
  return Script_srchControl(0);
}

static int32_t script_1DDC8(int32_t arg0) {
  int32_t unused_E8[6];
  task_3A4 = 0.5f;
  SCNF_WaitFrames_0000(arg0);
  Script_scenCreateTask(script_191DC, 2, 132265, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('CAR2'), &data_31B04, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('CAR2'), task_3A4) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 197801, 0);
  Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Hide);
  return Script_srchControl(0);
}

static void script_1DE56(void) {
  int32_t local_FC;
  {
    int32_t unused_F8;
    Vector3 local_EC;
    local_EC.x = -12.166f;
    (&local_EC)->z = 80.3293f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_EC);
  }
  {
    int32_t unused_F8;
    Vector3i local_EC;
    (&local_EC)->y = 42964;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_EC, 0);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    int32_t local_F8;
    local_F8 = Script_scenCreateTask(script_1E21F, 0);
    task_3B6 = script_17AAD(local_F8);
  }
  {
    int32_t local_F8;
    local_F8 = Script_scenCreateTask(script_1E30B, 0);
    task_3B7 = script_17AAD(local_F8);
  }
  Script_scenCreateTask(script_1E414, 0);
  script_19495(43);
  script_185A8(CHARA_AKIR, -6.7278f, 85.604897f, 30, 0.5f);
  local_FC = script_22782(40);
  script_1DF4E(local_FC);
  return;
}

static void script_1DF4E(int32_t arg0) {
  if (arg0 & 0x40) {
    Script_PlayerStatsUtils(PS_SetCharaItemCount, 0x52444550, Script_PlayerStatsUtils(PS_GetCharaItemCount, 0x52444550) + 1);
    script_19495(45);
    Script_scenCreateTask(script_1B205, 0);
    script_185A8(CHARA_AKIR, 6.777f, 91.012604f, 120 - task_39C, 6.0f);
    task_398 = 1;
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
    Script_charaDisplay(CHARA_SMTH, CDM_Hide);
    Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else if (arg0 & 4) {
    script_19495(44);
    script_185A8(CHARA_AKIR, -3.7238f, 109.243698f, 120 - task_39C, 5.0f);
    task_398 = 2;
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
    Script_charaDisplay(CHARA_TONY, CDM_Hide);
    Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    script_19495(46);
    Script_PlayCharaMotion(CHARA_AKIR, 28682, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_1821E(CHARA_AKIR, 40.0f);
    {
      Vector3 local_F4;
      local_F4.x = 6.777f;
      (&local_F4)->y = 1.75f;
      (&local_F4)->z = 91.012604f;
      Script_SetCharaLookAtPosition(CHARA_AKIR, 9, &local_F4, 0);
    }
    Script_charaDisplay(CHARA_TONY, CDM_Hide);
    Script_charaDisplay(CHARA_SMTH, CDM_Hide);
    Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    {
      int32_t unused_F4[3];
      Vector3 local_E8;
      local_E8.x = 1000.0f;
      (&local_E8)->z = 0.0f;
      Script_EVT_SetPosition(CHARA_TONY, 0x28000000, &local_E8);
    }
    {
      int32_t unused_F4[3];
      Vector3 local_E8;
      local_E8.x = 1000.0f;
      (&local_E8)->z = 0.0f;
      Script_EVT_SetPosition(CHARA_SMTH, 0x28000000, &local_E8);
    }
    script_1821E(CHARA_AKIR, 60.0f);
    Script_SetCharaLookAtPosition(CHARA_AKIR, -9, 0, 0);
    script_1821E(CHARA_AKIR, 65.0f);
    {
      Vector3 local_F4;
      local_F4.x = -3.7238f;
      (&local_F4)->y = 1.75f;
      (&local_F4)->z = 109.243698f;
      Script_SetCharaLookAtPosition(CHARA_AKIR, 9, &local_F4, 0);
    }
    script_1821E(CHARA_AKIR, 85.0f);
    Script_scenCreateTask(script_1B265, 0);
    Script_SetCharaLookAtPosition(CHARA_AKIR, -9, 0, 0);
    Script_scenCreateTask(script_19200, 2, 4, 0);
    script_1821E(CHARA_AKIR, 200.0f);
    task_398 = 0;
  }
  return;
}

static int32_t script_1E21F(void) {
  Script_charaDisplay(CHARA_TONY, CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = -4.1196f;
    (&local_F4)->z = 85.228401f;
    Script_EVT_SetPosition(CHARA_TONY, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 40777;
    Script_EVT_SetRotation(CHARA_TONY, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_185A8(CHARA_TONY, -1.5166f, 87.377502f, 30, 0.5f);
  script_185A8(CHARA_TONY, 2.3035f, 88.929001f, 60, 0.5f);
  script_185A8(CHARA_TONY, 5.8944f, 88.8293f, 60, 0.5f);
  script_185A8(CHARA_TONY, 9.5837f, 85.741997f, 60, 0.5f);
  return Script_srchControl(0);
}

static int32_t script_1E30B(void) {
  Script_charaDisplay(CHARA_SMTH, CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = -5.1564f;
    (&local_F4)->z = 86.139603f;
    Script_EVT_SetPosition(CHARA_SMTH, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 40777;
    Script_EVT_SetRotation(CHARA_SMTH, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_185A8(CHARA_SMTH, -3.2992f, 88.299202f, 30, 0.5f);
  script_185A8(CHARA_SMTH, -1.7358f, 93.442001f, 60, 0.5f);
  script_185A8(CHARA_SMTH, -4.1569f, 103.184502f, 60, 0.5f);
  script_185A8(CHARA_SMTH, -3.2902f, 107.771698f, 60, 0.5f);
  script_185A8(CHARA_SMTH, -1.9621f, 110.805702f, 30, 0.5f);
  return Script_srchControl(0);
}

static int32_t script_1E414(void) {
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Show);
      goto L01E43E;
    }
  }
  L01E43E:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR0'), 1);
  {
    Vector3 local_F4;
    local_F4.x = 2.7531f;
    (&local_F4)->z = 113.134102f;
    Script_EVT_SetPosition(SHEN_NAME('CAR0'), 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 6687;
    Script_EVT_SetRotation(SHEN_NAME('CAR0'), MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Show);
  Script_scenCreateTask(script_191DC, 2, 132265, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('CAR0'), &data_31B38, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('CAR0'), 0.4f) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 197801, 0);
  Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Hide);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Show);
      goto L01E533;
    }
  }
  L01E533:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR0'), 0);
  return Script_srchControl(0);
}

static int32_t script_1E546(int32_t arg0) {
  script_185A8(arg0, -3.7238f, 109.243698f, 20, 2.0f);
  script_185A8(arg0, 1.7247f, 114.233498f, 20, 4.0f);
  return Script_srchControl(0);
}

static void script_1E583(void) {
  int32_t local_FC;
  script_23678();
  if (0) {
    Script_charaDisplay(CHARA_DICK, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_DICK, CDM_Show);
      goto L01E5B2;
    }
  }
  L01E5B2:;
  Script_scenToggleVisibleFor(CHARA_DICK, 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASK'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASK'), CDM_Show);
      goto L01E5E6;
    }
  }
  L01E5E6:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASK'), 1);
  script_17C0D(task_3B7);
  script_17C0D(task_3B6);
  script_1E82B();
  Script_charaDisplay(CHARA_SMTH, CDM_Hide);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_1E546, 1, CHARA_AKIR);
  local_FC = script_227FE(0x100, 25);
  SCNF_WaitFrames_0000(29 - task_39C);
  script_1E68F(local_FC);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_DICK, CDM_Hide);
  return;
}

static void script_1E68F(int32_t arg0) {
  if (arg0 & 0x100) {
    script_19495(17);
    script_18080(CHARA_AKIR, 1.7247f, 114.233498f, 2.0f);
    Script_PlayCharaMotion(CHARA_AKIR, 28696, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.2f);
    Script_scenCreateTask(script_19200, 2, 6, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2976AB, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2A76AB, 0x42080000);
    script_1821E(CHARA_AKIR, 36.0f);
  } else {
    script_19495(18);
    script_18080(CHARA_AKIR, 1.7247f, 114.233498f, 1.0f);
    Script_PlayCharaMotion(CHARA_AKIR, 28690, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 2, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2776AB, 0x41A00000);
    script_1821E(CHARA_AKIR, 20.0f);
    Script_SetCamShake(0.3f, 0.3f, 0.4f);
    script_1821E(CHARA_AKIR, 36.0f);
    task_3B1 = task_3B1 + 1;
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  SCNF_WaitFrames_0000(5);
  return;
}

static void script_1E82B(void) {
  Vector3i local_F4;
  script_18687(SHEN_NAME('HASK'), 0, 0);
  {
    Vector3 local_E8;
    local_E8.x = 3.2294f;
    (&local_E8)->z = 112.521797f;
    Script_EVT_SetPosition(SHEN_NAME('HASK'), 0x28000000, &local_E8);
  }
  local_F4.x = 0;
  (&local_F4)->y = 43544;
  (&local_F4)->z = 182;
  Script_EVT_SetRotation(SHEN_NAME('HASK'), 0x38000000, &local_F4, 0);
  Script_charaDisplay(SHEN_NAME('HASK'), CDM_Show);
  if (0) {
    Script_charaDisplay(CHARA_DICK, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_DICK, CDM_Show);
      goto L01E8C3;
    }
  }
  L01E8C3:;
  Script_scenToggleVisibleFor(CHARA_DICK, 1);
  {
    Vector3 local_E8;
    local_E8.x = 0.9379f;
    (&local_E8)->z = 110.905701f;
    Script_EVT_SetPosition(CHARA_DICK, 0x28000000, &local_E8);
  }
  {
    Vector3i local_E8;
    (&local_E8)->y = 41547;
    Script_EVT_SetRotation(CHARA_DICK, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_motmSetBlendFrames(CHARA_DICK, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_charaDisplay(CHARA_DICK, CDM_Show);
  Script_scenCreateTask(script_1EA81, 0);
  {
    Vector3 local_E8;
    local_E8.x = -1.921f;
    (&local_E8)->z = 104.438301f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_E8);
  }
  {
    Vector3i local_E8;
    (&local_E8)->y = 41547;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_DICK, 28714, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_DICK, 1);
  Script_SetCharaMotionPlaybackRate(CHARA_DICK, 1.3f);
  Script_scenCreateTask(script_1EA38, 1, CHARA_DICK);
  script_19495(16);
  script_1821E(CHARA_DICK, 190.0f);
  Script_scenCreateTask(script_18687, 3, SHEN_NAME('HASK'), 0, 1);
  Script_scenCreateTask(script_191DC, 2, 787625, 47);
  SCNF_WaitFrames_0000(10);
  return;
}

static int32_t script_1EA38(int32_t arg0) {
  Vector3i local_F4;
  script_181A0(arg0);
  (&local_F4)->y = 8508;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static int32_t script_1EA81(void) {
  {
    Vector3 local_F4;
    local_F4.x = -1.9621f;
    (&local_F4)->z = 110.805702f;
    Script_EVT_SetPosition(CHARA_SMTH, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 41547;
    Script_EVT_SetRotation(CHARA_SMTH, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(CHARA_SMTH, CDM_Show);
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_SMTH, 1);
  script_185A8(CHARA_SMTH, 7.9141f, 119.691704f, 10, 14.0f);
  Script_PlayCharaMotion(CHARA_SMTH, 28715, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_19200, 2, 22, 5);
  Script_scenCreateTask(script_19200, 2, 24, 45);
  script_181A0(CHARA_SMTH);
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_185A8(CHARA_SMTH, 7.9141f, 119.691704f, 30, 2.0f);
  script_185A8(CHARA_SMTH, 19.9744f, 115.309097f, 240, 4.0f);
  return Script_srchControl(0);
}

static void script_1EBC9(void) {
  script_23678();
  if (0) {
    Script_charaDisplay(CHARA_PEDR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_PEDR, CDM_Show);
      goto L01EBF8;
    }
  }
  L01EBF8:;
  Script_scenToggleVisibleFor(CHARA_PEDR, 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BOX0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX0'), CDM_Show);
      goto L01EC2C;
    }
  }
  L01EC2C:;
  Script_scenToggleVisibleFor(SHEN_NAME('BOX0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Show);
      goto L01EC60;
    }
  }
  L01EC60:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Show);
      goto L01EC94;
    }
  }
  L01EC94:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
      goto L01ECC8;
    }
  }
  L01ECC8:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR3'), CDM_Show);
      goto L01ECFC;
    }
  }
  L01ECFC:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Show);
      goto L01ED30;
    }
  }
  L01ED30:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR4'), 1);
  script_17C0D(task_3B7);
  script_17C0D(task_3B6);
  task_3B0 = 0;
  script_1F07E();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    int32_t local_FC;
    local_FC = script_227FE(4, 20);
    script_1EDCE(local_FC);
  }
  if (task_3B0) {
    int32_t local_FC;
    local_FC = script_227FE(0x100, 20);
    script_1EEDF(local_FC);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  return;
}

static void script_1EDCE(int32_t arg0) {
  if (arg0 & 4) {
    Vector3 local_F4;
    script_19495(32);
    local_F4.x = 9.16f;
    (&local_F4)->z = 86.2f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
    Script_PlayCharaMotion(CHARA_AKIR, 28692, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 1, 0);
    task_3B0 = 1;
  } else {
    Vector3 local_F4;
    script_19495(33);
    local_F4.x = 10.1879f;
    (&local_F4)->z = 85.674301f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
    Script_PlayCharaMotion(CHARA_AKIR, 28690, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 3, 0);
    script_1821E(CHARA_AKIR, 36.0f);
    task_3B1 = task_3B1 + 1;
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  return;
}

static void script_1EEDF(int32_t arg0) {
  script_181A0(CHARA_AKIR);
  if (arg0 & 0x100) {
    Vector3 local_F4;
    script_19495(35);
    local_F4.x = 12.5879f;
    (&local_F4)->z = 85.474297f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
    Script_PlayCharaMotion(CHARA_AKIR, 28696, 5.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(CHARA_AKIR, 0);
    Script_scenCreateTask(script_19200, 2, 1, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2976AB, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2A76AB, 0x42080000);
    SCNF_WaitFrames_0000(5);
    Script_charaToggleMotEnabled(CHARA_AKIR, 1);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.2f);
    script_1821E(CHARA_AKIR, 36.0f);
  } else {
    Vector3 local_F4;
    script_19495(36);
    local_F4.x = 12.5879f;
    (&local_F4)->z = 85.474297f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
    Script_PlayCharaMotion(CHARA_AKIR, 28690, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 4, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2776AB, 0x41A00000);
    script_1821E(CHARA_AKIR, 36.0f);
    task_3B1 = task_3B1 + 1;
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_1F07E(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (0) {
    Script_charaDisplay(CHARA_PEDR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_PEDR, CDM_Show);
      goto L01F0A8;
    }
  }
  L01F0A8:;
  Script_scenToggleVisibleFor(CHARA_PEDR, 1);
  Script_charaDisplay(CHARA_PEDR, CDM_Show);
  Script_motmSetBlendFrames(CHARA_PEDR, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_PlayCharaMotion(CHARA_PEDR, 28706, 0.0f, 5.0f, 0.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  script_1879A(SHEN_NAME('BOX0'), 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  {
    Vector3 local_DC;
    local_DC.x = 20.66f;
    (&local_DC)->z = 70.37f;
    Script_EVT_SetPosition(SHEN_NAME('CAR4'), 0x28000000, &local_DC);
  }
  {
    Vector3i local_DC;
    (&local_DC)->y = 24656;
    Script_EVT_SetRotation(SHEN_NAME('CAR4'), MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Show);
  {
    Vector3 local_DC;
    local_DC.x = 20.5599f;
    (&local_DC)->z = 68.094398f;
    Script_EVT_SetPosition(SHEN_NAME('CAR2'), 0x28000000, &local_DC);
  }
  {
    Vector3i local_DC;
    (&local_DC)->y = 24656;
    Script_EVT_SetRotation(SHEN_NAME('CAR2'), MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
  Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  Script_charaDisplay(CHARA_KOGA, CDM_Hide);
  Script_charaDisplay(CHARA_MITA, CDM_Hide);
  Script_charaDisplay(CHARA_FLD7, CDM_Hide);
  Script_charaDisplay(CHARA_FLDF, CDM_Hide);
  local_F4.x = 13.46f;
  (&local_F4)->y = 4.2f;
  (&local_F4)->z = 87.3f;
  Script_EVT_SetPosition(CHARA_PEDR, POS_XYZ, &local_F4);
  local_E8.x = 906;
  (&local_E8)->y = 25546;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_PEDR, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_PEDR, 28706, 0.0f, 5.0f, 0.0f, 1.0f, 0);
  {
    Vector3 local_DC;
    local_DC.x = 6.6156f;
    (&local_DC)->z = 84.477997f;
    Script_EVT_SetPosition(CHARA_TONY, 0x28000000, &local_DC);
  }
  {
    Vector3i local_DC;
    (&local_DC)->y = 54494;
    Script_EVT_SetRotation(CHARA_TONY, MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_charaDisplay(CHARA_TONY, CDM_Show);
  Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_TONY, 1);
  local_F4.x = 13.6261f;
  (&local_F4)->y = 4.2f;
  (&local_F4)->z = 87.422897f;
  Script_EVT_SetPosition(SHEN_NAME('BOX0'), POS_XYZ, &local_F4);
  {
    Vector3i local_DC;
    (&local_DC)->y = 24963;
    Script_EVT_SetRotation(SHEN_NAME('BOX0'), MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_charaDisplay(SHEN_NAME('BOX0'), CDM_Show);
  script_19495(31);
  Script_PlayCharaMotion(CHARA_TONY, 28724, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_19200, 2, 9, 3);
  Script_PlayCharaMotion(CHARA_PEDR, 28706, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_19200, 2, 16, 40);
  Script_charaToggleMotEnabled(CHARA_PEDR, 1);
  Script_SetCharaMotionPlaybackRate(CHARA_PEDR, 1.5f);
  script_181A0(CHARA_TONY);
  Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_1821E(CHARA_PEDR, 145.0f);
  {
    Vector3 local_DC;
    local_DC.x = -1.23f;
    (&local_DC)->z = 98.865799f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_DC);
  }
  {
    Vector3i local_DC;
    (&local_DC)->y = 58068;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  script_1821E(CHARA_PEDR, 180.0f);
  Script_scenCreateTask(script_1F51D, 0);
  script_1821E(CHARA_PEDR, 212.0f);
  Script_scenCreateTask(script_19200, 2, 17, 0);
  Script_scenCreateTask(script_1879A, 3, SHEN_NAME('BOX0'), 0, 1);
  Script_scenCreateTask(script_191DC, 2, 918697, 5);
  Script_scenCreateTask(script_191DC, 2, 853161, 35);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  {
    Vector3i local_DC;
    (&local_DC)->y = 58068;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  }
  SCNF_WaitFrames_0000(10);
  return;
}

static int32_t script_1F51D(void) {
  Vector3 local_F4;
  local_F4.x = 20.66f;
  (&local_F4)->z = 70.37f;
  Script_EVT_SetPosition(SHEN_NAME('CAR4'), 0x28000000, &local_F4);
  Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Show);
  Script_scenCreateTask(script_191DC, 2, 132265, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('CAR4'), &data_31B5C, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('CAR4'), 0.35f) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 197801, 0);
  Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Hide);
  return Script_srchControl(0);
}

static void script_1F5CB(void) {
  int32_t local_FC;
  int32_t unused_F0[3];
  script_23678();
  if (0) {
    Script_charaDisplay(CHARA_FLDF, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_FLDF, CDM_Show);
      goto L01F5FA;
    }
  }
  L01F5FA:;
  Script_scenToggleVisibleFor(CHARA_FLDF, 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAB0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAB0'), CDM_Show);
      goto L01F62E;
    }
  }
  L01F62E:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAB0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK3'), CDM_Show);
      goto L01F662;
    }
  }
  L01F662:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK4'), CDM_Show);
      goto L01F696;
    }
  }
  L01F696:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK4'), 1);
  script_19495(19);
  script_1F9E4();
  local_FC = script_227FE(64, 20);
  script_1F6DD(local_FC);
  SCNF_WaitFrames_0000(10);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  return;
}

static void script_1F6DD(int32_t arg0) {
  if (arg0 & 0x40) {
    script_19495(20);
    Script_scenCreateTask(script_1FC20, 1, 10);
    Script_scenCreateTask(script_1FD4A, 1, 16);
    Script_scenCreateTask(script_191DC, 2, 984233, 10);
    {
      Vector3 local_F4;
      local_F4.x = 9.2471f;
      (&local_F4)->z = 116.7183f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
    }
    {
      Vector3i local_F4;
      (&local_F4)->y = 42313;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
    }
    Script_PlayCharaMotion(CHARA_AKIR, 28687, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 20, 0);
    Script_scenCreateTask(script_19200, 2, 1, 10);
    Script_scenCreateTask(script_19200, 2, 6, 30);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2176AB, 0x41F00000);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2276AB, 0);
    script_181A0(CHARA_AKIR);
  } else {
    Vector3 local_F4;
    script_19495(21);
    SCNF_WaitFrames_0000(24 - task_39C);
    task_3A0 = task_3A0 * (float)2;
    local_F4.x = 10.1277f;
    (&local_F4)->z = 118.968002f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
    Script_PlayCharaMotion(CHARA_AKIR, 28683, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 2, 0);
    Script_scenCreateTask(script_19200, 2, 21, 50);
    Script_scenCreateTask(script_19200, 2, 5, 80);
    Script_scenCreateTask(script_191DC, 2, 591017, 0);
    Script_scenCreateTask(script_191DC, 2, 656553, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 722089, 0x41700000);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2776AB, 0x42480000);
    Script_PlayCharaMotion(CHARA_FLDF, 28708, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(CHARA_FLDF, 0);
    script_1821E(CHARA_AKIR, 15.0f);
    Script_srchControl((void *)task_3D0);
    script_1821E(CHARA_AKIR, 45.0f);
    Script_SetCamShake(0.5f, 0.5f, 0.5f);
    script_181A0(CHARA_AKIR);
    task_3B1 = task_3B1 + 1;
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_seqEnterEvtCam(5901, 0, AUTH_None);
  script_185A8(CHARA_AKIR, 19.4109f, 114.9673f, 60, 0.5f);
  if (arg0 & 0x40) {
    Script_srchControl((void *)task_3D0);
  }
  Script_MoveObjectToObject(CHARA_FLDF, -1);
  Script_PlayCharaMotion(CHARA_FLDF, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_1F9E4(void) {
  int32_t unused_E8[6];
  Vector3i local_DC;
  if (-1) {
    Script_charaDisplay(CHARA_DICK, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_DICK, CDM_Show);
      goto L01FA0E;
    }
  }
  L01FA0E:;
  Script_scenToggleVisibleFor(CHARA_DICK, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  task_3D0 = Script_scenCreateTask(script_1FAA3, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  (&local_DC)->y = 45655;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static int32_t script_1FAA3(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  local_DC.x = 23.4f;
  (&local_DC)->z = 109.43f;
  Script_EVT_SetPosition(SHEN_NAME('CAB0'), 0x28000000, &local_DC);
  Script_charaDisplay(SHEN_NAME('CAB0'), CDM_Show);
  task_3A0 = 0.3f;
  if (0) {
    Script_charaDisplay(CHARA_FLDF, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_FLDF, CDM_Show);
      goto L01FB07;
    }
  }
  L01FB07:;
  Script_scenToggleVisibleFor(CHARA_FLDF, 1);
  Script_ccowUtils(CHARA_FLDF, CCOW_DisableAll);
  Script_MoveObjectToObject(CHARA_FLDF, 0x30424143);
  local_F4.x = 0.012f;
  (&local_F4)->y = 0.332f;
  (&local_F4)->z = 0.457f;
  Script_EVT_SetPosition(CHARA_FLDF, POS_XYZ, &local_F4);
  local_E8.x = 64625;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_FLDF, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_FLDF, 28703, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_FLDF, CDM_Show);
  Script_scenCreateTask(script_191DC, 2, 525481, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('CAB0'), &data_31B80, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('CAB0'), task_3A0) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 591017, 0);
  end:;
}

static int32_t script_1FC20(int32_t arg0) {
  {
    Vector3i local_F4;
    (&local_F4)->y = 58100;
    Script_EVT_SetRotation(SHEN_NAME('FOK3'), MATRIX_XFORM_Y, &local_F4, 0);
  }
  {
    Vector3 local_F4;
    local_F4.x = 18.99f;
    (&local_F4)->z = 110.7603f;
    Script_EVT_SetPosition(SHEN_NAME('FOK3'), 0x28000000, &local_F4);
  }
  Script_charaDisplay(SHEN_NAME('FOK3'), CDM_Show);
  SCNF_WaitFrames_0000(arg0);
  Script_scenCreateTask(script_191DC, 2, 263337, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('FOK3'), &data_31BD4, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('FOK3'), 0.17f) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 328873, 0);
  Script_charaDisplay(SHEN_NAME('FOK3'), CDM_Hide);
  {
    Vector3 local_F4;
    local_F4.x = 1000.0f;
    (&local_F4)->z = 0.0f;
    Script_EVT_SetPosition(SHEN_NAME('FOK3'), 0x28000000, &local_F4);
  }
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FOK3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FOK3'), CDM_Show);
      goto L01FD37;
    }
  }
  L01FD37:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK3'), 0);
  return Script_srchControl(0);
}

static int32_t script_1FD4A(int32_t arg0) {
  {
    Vector3i local_F4;
    (&local_F4)->y = 25151;
    Script_EVT_SetRotation(SHEN_NAME('FOK4'), MATRIX_XFORM_Y, &local_F4, 0);
  }
  {
    Vector3 local_F4;
    local_F4.x = 25.5233f;
    (&local_F4)->z = 111.298103f;
    Script_EVT_SetPosition(SHEN_NAME('FOK4'), 0x28000000, &local_F4);
  }
  Script_charaDisplay(SHEN_NAME('FOK4'), CDM_Show);
  SCNF_WaitFrames_0000(arg0);
  Script_scenCreateTask(script_191DC, 2, 263337, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('FOK4'), &data_31C08, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('FOK4'), 0.17f) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 328873, 0);
  Script_charaDisplay(SHEN_NAME('FOK4'), CDM_Hide);
  return Script_srchControl(0);
}

static void script_1FE1C(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  if (0) {
    Script_charaDisplay(CHARA_MITA, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_MITA, CDM_Show);
      goto L01FE46;
    }
  }
  L01FE46:;
  Script_scenToggleVisibleFor(CHARA_MITA, 1);
  if (0) {
    Script_charaDisplay(CHARA_KOGA, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KOGA, CDM_Show);
      goto L01FE7A;
    }
  }
  L01FE7A:;
  Script_scenToggleVisibleFor(CHARA_KOGA, 1);
  if (0) {
    Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_NMKI, CDM_Show);
      goto L01FEAE;
    }
  }
  L01FEAE:;
  Script_scenToggleVisibleFor(CHARA_NMKI, 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Show);
      goto L01FEE2;
    }
  }
  L01FEE2:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Show);
      goto L01FF16;
    }
  }
  L01FF16:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK2'), CDM_Show);
      goto L01FF4A;
    }
  }
  L01FF4A:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK2'), 1);
  Script_scenCreateTask(script_191DC, 2, 591017, 0);
  script_205AC();
  script_19495(22);
  local_F0 = script_227FE(4, 15);
  script_20267(local_F0);
  if (task_3B0 == 1) {
    script_19495(25);
    task_3B0 = 0;
    local_F0 = script_227FE(64, 15);
    script_2038A(local_F0);
  } else {
    task_3B0 = 1;
  }
  if (task_3B0 == 1) {
    script_19495(28);
    task_3B0 = 0;
    local_F0 = script_227FE(4, 12);
    script_20490(local_F0);
  }
  SCNF_WaitFrames_0000(30);
  Script_srchControl((void *)task_3D4);
  Script_scenCreateTask(script_191DC, 2, 328873, 0);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Hide);
  local_E4.x = 1000.0f;
  (&local_E4)->z = 0.0f;
  Script_EVT_SetPosition(SHEN_NAME('FOK0'), 0x28000000, &local_E4);
  Script_MoveObjectToObject(CHARA_MITA, -1);
  Script_PlayCharaMotion(CHARA_MITA, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_MITA, 0x78000000, &local_F4);
  Script_charaDisplay(CHARA_MITA, CDM_Hide);
  SCNF_WaitFrames_0000(1);
  if (-1) {
    Script_charaDisplay(CHARA_MITA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MITA, CDM_Show);
      goto L0200DD;
    }
  }
  L0200DD:;
  Script_scenToggleVisibleFor(CHARA_MITA, 0);
  Script_srchControl((void *)task_3D8);
  Script_scenCreateTask(script_191DC, 2, 328873, 0);
  Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Hide);
  Script_MoveObjectToObject(CHARA_KOGA, -1);
  Script_PlayCharaMotion(CHARA_KOGA, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_MITA, 0x78000000, &local_F4);
  Script_charaDisplay(CHARA_KOGA, CDM_Hide);
  SCNF_WaitFrames_0000(1);
  if (-1) {
    Script_charaDisplay(CHARA_KOGA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KOGA, CDM_Show);
      goto L020185;
    }
  }
  L020185:;
  Script_scenToggleVisibleFor(CHARA_KOGA, 0);
  Script_srchControl((void *)task_3DC);
  Script_scenCreateTask(script_191DC, 2, 328873, 0);
  Script_charaDisplay(SHEN_NAME('FOK2'), CDM_Hide);
  Script_MoveObjectToObject(CHARA_NMKI, -1);
  Script_PlayCharaMotion(CHARA_NMKI, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_MITA, 0x78000000, &local_F4);
  Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  SCNF_WaitFrames_0000(1);
  if (-1) {
    Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_NMKI, CDM_Show);
      goto L02022D;
    }
  }
  L02022D:;
  Script_scenToggleVisibleFor(CHARA_NMKI, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  return;
}

static void script_20267(int32_t arg0) {
  if (arg0 & 4) {
    script_19495(23);
    Script_PlayCharaMotion(CHARA_AKIR, 28692, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 1, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2076AB, 0);
    script_1821E(CHARA_AKIR, 10.0f);
    task_3B0 = 1;
  } else {
    script_19495(24);
    script_191A5(CHARA_AKIR, SHEN_NAME('FOK0'), 4.0f);
    task_3A8 = 0;
    Script_PlayCharaMotion(CHARA_AKIR, 28691, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 3, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2276AB, 0);
    script_1821E(CHARA_AKIR, 55.0f);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    task_3B1 = task_3B1 + 1;
  }
  return;
}

static void script_2038A(int32_t arg0) {
  if (arg0 & 0x40) {
    script_19495(26);
    script_181A0(CHARA_AKIR);
    script_18FCF(CHARA_AKIR, 28692);
    Script_scenCreateTask(script_19200, 2, 6, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2076AB, 0);
    script_181A0(CHARA_AKIR);
    task_3B0 = 1;
  } else {
    script_19495(27);
    script_181A0(CHARA_AKIR);
    Script_PlayCharaMotion(CHARA_AKIR, 28691, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 4, 2);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2276AB, 0);
    script_1821E(CHARA_AKIR, 55.0f);
    task_3B1 = task_3B1 + 1;
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_20490(int32_t arg0) {
  task_3AC = 0;
  if (arg0 & 4) {
    script_19495(29);
    Script_PlayCharaMotion(CHARA_AKIR, 28692, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 6, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2076AB, 0);
    script_181A0(CHARA_AKIR);
  } else {
    script_19495(30);
    Script_PlayCharaMotion(CHARA_AKIR, 28691, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_19200, 2, 2, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2276AB, 0);
    script_1821E(CHARA_AKIR, 55.0f);
    task_3B1 = task_3B1 + 1;
  }
  task_3AC = 0x3FB33333;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_seqEnterEvtCam(5012, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
  return;
}

static void script_205AC(void) {
  task_3B0 = 0;
  {
    int32_t unused_E8[6];
    Vector3 local_DC;
    local_DC.x = 26.25f;
    (&local_DC)->z = 102.41f;
    Script_EVT_SetPosition(SHEN_NAME('FOK0'), 0x28000000, &local_DC);
  }
  {
    int32_t unused_E8[6];
    Vector3i local_DC;
    (&local_DC)->y = 26431;
    Script_EVT_SetRotation(SHEN_NAME('FOK0'), MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Show);
  if (0) {
    Script_charaDisplay(CHARA_MITA, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_MITA, CDM_Show);
      goto L020629;
    }
  }
  L020629:;
  Script_scenToggleVisibleFor(CHARA_MITA, 1);
  Script_MoveObjectToObject(CHARA_MITA, 0x304B4F46);
  {
    Vector3 local_F4;
    local_F4.x = -0.053f;
    (&local_F4)->y = 0.452f;
    (&local_F4)->z = 0.5f;
    Script_EVT_SetPosition(CHARA_MITA, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(CHARA_MITA, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_MITA, 28705, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_MITA, CDM_Show);
  task_3D4 = Script_scenCreateTask(script_2099A, 0);
  {
    int32_t unused_E8[6];
    Vector3 local_DC;
    local_DC.x = 25.7138f;
    (&local_DC)->z = 112.8965f;
    Script_EVT_SetPosition(SHEN_NAME('FOK1'), 0x28000000, &local_DC);
  }
  {
    int32_t unused_E8[6];
    Vector3i local_DC;
    (&local_DC)->y = 910;
    Script_EVT_SetRotation(SHEN_NAME('FOK1'), MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Show);
  if (0) {
    Script_charaDisplay(CHARA_KOGA, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KOGA, CDM_Show);
      goto L020750;
    }
  }
  L020750:;
  Script_scenToggleVisibleFor(CHARA_KOGA, 1);
  Script_MoveObjectToObject(CHARA_KOGA, 0x314B4F46);
  {
    Vector3 local_F4;
    local_F4.x = -0.053f;
    (&local_F4)->y = 0.452f;
    (&local_F4)->z = 0.5f;
    Script_EVT_SetPosition(CHARA_KOGA, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(CHARA_KOGA, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_KOGA, 28705, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_KOGA, CDM_Show);
  task_3D8 = Script_scenCreateTask(script_20ADA, 0);
  {
    int32_t unused_E8[6];
    Vector3 local_DC;
    local_DC.x = 29.413799f;
    (&local_DC)->z = 104.196602f;
    Script_EVT_SetPosition(SHEN_NAME('FOK2'), 0x28000000, &local_DC);
  }
  {
    int32_t unused_E8[6];
    Vector3i local_DC;
    (&local_DC)->y = 25495;
    Script_EVT_SetRotation(SHEN_NAME('FOK2'), MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_charaDisplay(SHEN_NAME('FOK2'), CDM_Show);
  if (0) {
    Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_NMKI, CDM_Show);
      goto L020877;
    }
  }
  L020877:;
  Script_scenToggleVisibleFor(CHARA_NMKI, 1);
  Script_MoveObjectToObject(CHARA_NMKI, 0x324B4F46);
  {
    Vector3 local_F4;
    local_F4.x = -0.053f;
    (&local_F4)->y = 0.452f;
    (&local_F4)->z = 0.5f;
    Script_EVT_SetPosition(CHARA_NMKI, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(CHARA_NMKI, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_NMKI, 28705, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_NMKI, CDM_Show);
  task_3DC = Script_scenCreateTask(script_20BF8, 0);
  {
    int32_t unused_E8[6];
    Vector3 local_DC;
    local_DC.x = 20.616699f;
    (&local_DC)->z = 114.278099f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_DC);
  }
  {
    int32_t unused_E8[6];
    Vector3i local_DC;
    (&local_DC)->y = 57936;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  return;
}

static int32_t script_2099A(void) {
  {
    int32_t unused_E8[6];
    Vector3 local_DC;
    local_DC.x = 30.813299f;
    (&local_DC)->z = 114.991501f;
    Script_EVT_SetPosition(SHEN_NAME('FOK0'), 0x28000000, &local_DC);
  }
  {
    int32_t unused_E8[6];
    Vector3i local_DC;
    (&local_DC)->y = 8378;
    Script_EVT_SetRotation(SHEN_NAME('FOK0'), MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Show);
  Script_scenCreateTask(script_191DC, 2, 263337, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('FOK0'), &data_31C2C, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('FOK0'), 0.22f) == -1) {
      break;
    }
  }
  Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Hide);
  {
    int32_t unused_E8[6];
    Vector3 local_DC;
    local_DC.x = 1000.0f;
    (&local_DC)->z = 0.0f;
    Script_EVT_SetPosition(SHEN_NAME('FOK0'), 0x28000000, &local_DC);
  }
  Script_MoveObjectToObject(CHARA_MITA, -1);
  Script_PlayCharaMotion(CHARA_MITA, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_MITA, CDM_Hide);
  if (-1) {
    Script_charaDisplay(CHARA_MITA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MITA, CDM_Show);
      goto L020ACD;
    }
  }
  L020ACD:;
  return Script_scenToggleVisibleFor(CHARA_MITA, 0);
}

static int32_t script_20ADA(void) {
  {
    Vector3 local_F4;
    local_F4.x = 32.413799f;
    (&local_F4)->z = 96.7967f;
    Script_EVT_SetPosition(SHEN_NAME('FOK1'), 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 27316;
    Script_EVT_SetRotation(SHEN_NAME('FOK1'), MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Show);
  Script_scenCreateTask(script_191DC, 2, 263337, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('FOK1'), &data_31C90, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('FOK1'), 0.183f) == -1) {
      break;
    }
  }
  Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Hide);
  Script_MoveObjectToObject(CHARA_KOGA, -1);
  Script_PlayCharaMotion(CHARA_KOGA, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_KOGA, CDM_Hide);
  if (-1) {
    Script_charaDisplay(CHARA_KOGA, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KOGA, CDM_Show);
      goto L020BEB;
    }
  }
  L020BEB:;
  return Script_scenToggleVisibleFor(CHARA_KOGA, 0);
}

static int32_t script_20BF8(void) {
  {
    Vector3 local_F4;
    local_F4.x = 36.39f;
    (&local_F4)->z = 107.44f;
    Script_EVT_SetPosition(SHEN_NAME('FOK2'), 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 8527;
    Script_EVT_SetRotation(SHEN_NAME('FOK2'), MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(SHEN_NAME('FOK2'), CDM_Show);
  SCNF_WaitFrames_0000(70);
  Script_scenCreateTask(script_191DC, 2, 263337, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('FOK2'), &data_31CC4, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('FOK2'), 0.17f) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 328873, 0);
  Script_charaDisplay(SHEN_NAME('FOK2'), CDM_Hide);
  Script_MoveObjectToObject(CHARA_NMKI, -1);
  Script_PlayCharaMotion(CHARA_NMKI, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  if (-1) {
    Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_NMKI, CDM_Show);
      goto L020D23;
    }
  }
  L020D23:;
  return Script_scenToggleVisibleFor(CHARA_NMKI, 0);
}

static void script_20D30(void) {
  script_20E1A();
  Script_scenCreateTask(script_2158B, 0);
  script_19495(47);
  SCNF_WaitFrames_0000(8);
  {
    int32_t local_FC;
    local_FC = script_227FE(4, 15);
    script_212D2(local_FC);
  }
  if (task_3B2) {
    int32_t local_FC;
    script_19495(50);
    local_FC = script_227FE(64, 20);
    script_213EF(local_FC);
  }
  if (task_3B3 | (task_3B2 == 0)) {
    int32_t local_FC;
    script_19495(53);
    Script_scenCreateTask(script_21689, 0);
    Script_scenCreateTask(script_2195F, 0);
    local_FC = script_227FE(4, 20);
    script_21492(local_FC);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_185A8(CHARA_AKIR, 22.783199f, 76.523201f, 5, 0.5f);
  return;
}

static void script_20E1A(void) {
  task_3B2 = 0;
  task_3B3 = 0;
  task_3B4 = 0;
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  {
    Vector3 local_F4;
    local_F4.x = 10.1554f;
    (&local_F4)->z = 84.277f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 56690;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(SHEN_NAME('BOX0'), CDM_Hide);
  script_20EC6();
  Script_scenCreateTask(script_210DD, 0);
  Script_charaDisplay(CHARA_SMTH, CDM_Hide);
  Script_charaDisplay(CHARA_PEDR, CDM_Hide);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_20EC6(void) {
  if (0) {
    Script_charaDisplay(CHARA_FLDG, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_FLDG, CDM_Show);
      goto L020EF0;
    }
  }
  L020EF0:;
  Script_scenToggleVisibleFor(CHARA_FLDG, 1);
  Script_motmSetBlendFrames(CHARA_FLDG, MOTM_BLEND_FRAMES_IMMEDIATE);
  if (0) {
    Script_charaDisplay(CHARA_KOGA, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KOGA, CDM_Show);
      goto L020F30;
    }
  }
  L020F30:;
  Script_scenToggleVisibleFor(CHARA_KOGA, 1);
  Script_motmSetBlendFrames(CHARA_KOGA, MOTM_BLEND_FRAMES_IMMEDIATE);
  {
    Vector3 local_F4;
    local_F4.x = 15.6808f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 78.800499f;
    Script_EVT_SetPosition(CHARA_FLDG, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 0;
    Script_EVT_SetRotation(CHARA_FLDG, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_FLDG, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (0) {
    Script_charaDisplay(CHARA_KOGA, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KOGA, CDM_Show);
      goto L020FDB;
    }
  }
  L020FDB:;
  Script_scenToggleVisibleFor(CHARA_KOGA, 1);
  {
    Vector3 local_F4;
    local_F4.x = 15.5808f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 77.600502f;
    Script_EVT_SetPosition(CHARA_KOGA, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 33685;
    Script_EVT_SetRotation(CHARA_KOGA, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_KOGA, MN_AKI_AKI_PORI_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_FLDG, CDM_Show);
  Script_charaDisplay(CHARA_KOGA, CDM_Show);
  Script_PlayCharaMotion(CHARA_FLDG, MN_AKI_AKI_THINK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KOGA, MN_AKI_AKI_PORI_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = 16.180799f;
    (&local_E8)->z = 77.300499f;
    Script_EVT_SetPosition(SHEN_NAME('BO_3'), 0x28000000, &local_E8);
  }
  Script_charaDisplay(SHEN_NAME('BO_3'), CDM_Show);
  return;
}

static int32_t script_210DD(void) {
  if (0) {
    Script_charaDisplay(CHARA_MITA, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_MITA, CDM_Show);
      goto L021107;
    }
  }
  L021107:;
  Script_scenToggleVisibleFor(CHARA_MITA, 1);
  Script_ccowUtils(CHARA_MITA, CCOW_DisableAll);
  Script_motmSetBlendFrames(CHARA_MITA, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_PlayCharaMotion(CHARA_MITA, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    Vector3 local_F4;
    local_F4.x = 20.497801f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 76.616699f;
    Script_EVT_SetPosition(CHARA_MITA, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 24673;
    Script_EVT_SetRotation(CHARA_MITA, MATRIX_XFORM_Y, &local_E8, 0);
  }
  if (0) {
    Script_charaDisplay(CHARA_FLD7, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_FLD7, CDM_Show);
      goto L0211BD;
    }
  }
  L0211BD:;
  Script_scenToggleVisibleFor(CHARA_FLD7, 1);
  Script_ccowUtils(CHARA_FLD7, CCOW_DisableAll);
  Script_motmSetBlendFrames(CHARA_FLD7, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_PlayCharaMotion(CHARA_FLD7, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    Vector3 local_F4;
    local_F4.x = 20.3347f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 77.770699f;
    Script_EVT_SetPosition(CHARA_FLD7, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 24584;
    Script_EVT_SetRotation(CHARA_FLD7, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_charaDisplay(CHARA_MITA, CDM_Show);
  Script_charaDisplay(CHARA_FLD7, CDM_Show);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  Script_PlayCharaMotion(CHARA_MITA, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_MITA, 1);
  SCNF_WaitFrames_0000(10);
  Script_PlayCharaMotion(CHARA_FLD7, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_FLD7, 1);
  end:;
}

static void script_212D2(int32_t arg0) {
  SCNF_WaitFrames_0000(19 - task_39C);
  Script_PlayCharaMotion(CHARA_NMKI, MN_AKI_AKI_KIZUKU_FRONT, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (arg0 & 4) {
    script_19495(48);
    task_3B2 = 1;
    Script_PlayCharaMotion(CHARA_AKIR, 28688, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_1821E(CHARA_AKIR, 10.0f);
  } else {
    script_19495(49);
    task_3B2 = 0;
    task_3B1 = task_3B1 + 1;
    Script_PlayCharaMotion(CHARA_AKIR, 28691, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_1821E(CHARA_AKIR, 50.0f);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.8f);
  }
  Script_PlayCharaMotion(CHARA_FLDG, MN_AKI_AKI_THINK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_213EF(int32_t arg0) {
  if (arg0 & 0x40) {
    script_19495(51);
    task_3B3 = 1;
    script_1821E(CHARA_AKIR, 66.0f);
  } else {
    script_19495(52);
    task_3B3 = 0;
    task_3B1 = task_3B1 + 1;
    script_1821E(CHARA_AKIR, 34.0f);
    script_18FCF(CHARA_AKIR, 28691);
    script_1821E(CHARA_AKIR, 10.0f);
    Script_scenCreateTask(script_21689, 0);
    Script_scenCreateTask(script_2195F, 0);
    script_1821E(CHARA_AKIR, 55.0f);
  }
  return;
}

static void script_21492(int32_t arg0) {
  Vector3i local_F4;
  SCNF_WaitFrames_0000(24 - task_39C);
  if (arg0 & 4) {
    script_19495(54);
    task_3B4 = 1;
    if (task_3B2 & task_3B3) {
      script_181A0(CHARA_AKIR);
    }
    Script_PlayCharaMotion(CHARA_AKIR, 28692, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_1821E(CHARA_AKIR, 20.0f);
  } else {
    script_19495(55);
    task_3B4 = 0;
    task_3B1 = task_3B1 + 1;
    if (task_3B2) {
      script_1821E(CHARA_AKIR, 55.0f);
    }
    Script_PlayCharaMotion(CHARA_AKIR, 28691, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_1821E(CHARA_AKIR, 45.0f);
    (&local_F4)->y = 54315;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  }
  return;
}

static int32_t script_2158B(void) {
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
      goto L0215B5;
    }
  }
  L0215B5:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR2'), 1);
  {
    Vector3 local_F4;
    local_F4.x = 20.5599f;
    (&local_F4)->z = 68.094398f;
    Script_EVT_SetPosition(SHEN_NAME('CAR2'), 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 24656;
    Script_EVT_SetRotation(SHEN_NAME('CAR2'), MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
  Script_scenCreateTask(script_191DC, 2, 132265, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('CAR2'), &data_31CF8, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('CAR2'), 0.4f) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 197801, 0);
  Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Hide);
  return Script_srchControl(0);
}

static int32_t script_21689(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  local_F4.x = 25.622299f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 79.058098f;
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Show);
      goto L0216D1;
    }
  }
  L0216D1:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK0'), 1);
  Script_EVT_SetPosition(SHEN_NAME('FOK0'), POS_XYZ, &local_F4);
  (&local_DC)->y = 7534;
  Script_EVT_SetRotation(SHEN_NAME('FOK0'), MATRIX_XFORM_Y, &local_DC, 0);
  Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Show);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BO_1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BO_1'), CDM_Show);
      goto L02173C;
    }
  }
  L02173C:;
  Script_scenToggleVisibleFor(SHEN_NAME('BO_1'), 1);
  Script_MoveObjectToObject(SHEN_NAME('BO_1'), 0x304B4F46);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.1f;
  (&local_F4)->z = -1.4f;
  Script_EVT_SetPosition(SHEN_NAME('BO_1'), POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('BO_1'), 0x38000000, &local_E8, 0);
  Script_charaDisplay(SHEN_NAME('BO_1'), CDM_Show);
  if (0) {
    Script_charaDisplay(CHARA_FLDF, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_FLDF, CDM_Show);
      goto L0217E1;
    }
  }
  L0217E1:;
  Script_scenToggleVisibleFor(CHARA_FLDF, 1);
  Script_MoveObjectToObject(CHARA_FLDF, 0x304B4F46);
  local_F4.x = -0.053f;
  (&local_F4)->y = 0.452f;
  (&local_F4)->z = 0.5f;
  Script_EVT_SetPosition(CHARA_FLDF, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_FLDF, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_FLDF, 28705, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_FLDF, CDM_Show);
  Script_scenCreateTask(script_191DC, 2, 263337, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('FOK0'), &data_31D1C, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('FOK0'), 0.2f) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 328873, 0);
  Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Hide);
  Script_MoveObjectToObject(SHEN_NAME('BO_1'), -1);
  Script_charaDisplay(SHEN_NAME('BO_1'), CDM_Hide);
  Script_MoveObjectToObject(CHARA_FLDF, -1);
  Script_charaDisplay(CHARA_FLDF, CDM_Hide);
  if (-1) {
    Script_charaDisplay(CHARA_FLDF, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_FLDF, CDM_Show);
      goto L02194C;
    }
  }
  L02194C:;
  Script_scenToggleVisibleFor(CHARA_FLDF, 0);
  return Script_srchControl(0);
}

static int32_t script_2195F(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  local_F4.x = 25.622299f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 79.058098f;
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Show);
      goto L0219A7;
    }
  }
  L0219A7:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK1'), 1);
  Script_EVT_SetPosition(SHEN_NAME('FOK1'), POS_XYZ, &local_F4);
  (&local_DC)->y = 7534;
  Script_EVT_SetRotation(SHEN_NAME('FOK1'), MATRIX_XFORM_Y, &local_DC, 0);
  Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Show);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BO_2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BO_2'), CDM_Show);
      goto L021A12;
    }
  }
  L021A12:;
  Script_scenToggleVisibleFor(SHEN_NAME('BO_2'), 1);
  Script_MoveObjectToObject(SHEN_NAME('BO_2'), 0x314B4F46);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.1f;
  (&local_F4)->z = -1.4f;
  Script_EVT_SetPosition(SHEN_NAME('BO_2'), POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('BO_2'), 0x38000000, &local_E8, 0);
  Script_charaDisplay(SHEN_NAME('BO_2'), CDM_Show);
  if (0) {
    Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_NMKI, CDM_Show);
      goto L021AB7;
    }
  }
  L021AB7:;
  Script_scenToggleVisibleFor(CHARA_NMKI, 1);
  Script_MoveObjectToObject(CHARA_NMKI, 0x314B4F46);
  local_F4.x = -0.053f;
  (&local_F4)->y = 0.452f;
  (&local_F4)->z = 0.5f;
  Script_EVT_SetPosition(CHARA_NMKI, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_NMKI, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_NMKI, 28705, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_NMKI, CDM_Show);
  SCNF_WaitFrames_0000(20);
  Script_scenCreateTask(script_191DC, 2, 263337, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('FOK1'), &data_31D50, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('FOK1'), 0.2f) == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 328873, 0);
  Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Hide);
  Script_MoveObjectToObject(SHEN_NAME('BO_2'), -1);
  Script_charaDisplay(SHEN_NAME('BO_2'), CDM_Hide);
  Script_MoveObjectToObject(CHARA_NMKI, -1);
  Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  if (0) {
    Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_NMKI, CDM_Show);
      goto L021C2A;
    }
  }
  L021C2A:;
  Script_scenToggleVisibleFor(CHARA_NMKI, 1);
  return Script_srchControl(0);
}

static void script_21C3D(void) {
  int32_t local_FC;
  local_FC = task_398;
  switch (local_FC) {
    case 0:
      break;
    case 1:
      script_21C75();
      break;
    case 2:
      script_22158();
      break;
  }
  return;
}

static void script_21C75(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_mapmSetCamFlags(2, 2, 0);
  script_19495(-1);
  Script_charaDisplay(CHARA_SMTH, CDM_Hide);
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (task_3B1 <= task_3E4 + 3) {
    Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Hide);
    Script_scenCreateTask(script_21F7B, 0);
    script_19495(56);
    {
      Vector3 local_E8;
      local_E8.x = 21.1077f;
      (&local_E8)->z = 76.904099f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_E8);
    }
    {
      Vector3i local_E8;
      (&local_E8)->y = 43237;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaDisplay(CHARA_AKIR, CDM_Show);
    script_185A8(CHARA_AKIR, 28.7757f, 82.175201f, 60, 0.5f);
    Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 255, 0, 0, 0);
    Script_executeAicaCommand(2720, 2, 30);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_22F12();
    Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
    Script_ReturnTimeDate(&local_F4);
    (&local_F4)->hours = 17;
    (&local_F4)->minutes = 0;
    (&local_F4)->seconds = 0;
    Script_SetNewDateTime(&local_F4);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY, 1);
  } else {
    Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Show);
    {
      Vector3 local_E8;
      local_E8.x = 29.57f;
      (&local_E8)->z = 83.5f;
      Script_EVT_SetPosition(SHEN_NAME('CAR4'), 0x28000000, &local_E8);
    }
    {
      Vector3i local_E8;
      (&local_E8)->y = 3827;
      Script_EVT_SetRotation(SHEN_NAME('CAR4'), MATRIX_XFORM_Y, &local_E8, 0);
    }
    script_19495(57);
    Script_scenCreateTask(script_19200, 2, 10, 0);
    {
      Vector3 local_E8;
      local_E8.x = 21.1077f;
      (&local_E8)->z = 76.904099f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_E8);
    }
    {
      Vector3i local_E8;
      (&local_E8)->y = 43237;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaDisplay(CHARA_AKIR, CDM_Show);
    script_185A8(CHARA_AKIR, 28.7757f, 82.175201f, 60, 4.0f);
    Script_PlayCharaMotion(CHARA_AKIR, 28735, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2276AB, 0);
    script_181A0(CHARA_AKIR);
  }
  return;
}

static int32_t script_21F7B(void) {
  {
    Vector3 local_F4;
    local_F4.x = 28.7124f;
    (&local_F4)->z = 81.958f;
    Script_EVT_SetPosition(CHARA_TONY, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 42157;
    Script_EVT_SetRotation(CHARA_TONY, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_185A8(CHARA_TONY, 34.557598f, 85.642403f, 60, 2.0f);
  script_185A8(CHARA_TONY, 37.307098f, 84.943001f, 30, 0.5f);
  Script_charaDisplay(CHARA_TONY, CDM_Hide);
  Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_2204F(void) {
  Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = 30.87f;
    (&local_F4)->z = 85.2f;
    Script_EVT_SetPosition(SHEN_NAME('CAR4'), 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 3631;
    Script_EVT_SetRotation(SHEN_NAME('CAR4'), MATRIX_XFORM_Y, &local_F4, 0);
  }
  {
    Vector3 local_F4;
    local_F4.x = 21.1077f;
    (&local_F4)->z = 76.904099f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 43237;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  script_185A8(CHARA_AKIR, 28.7757f, 82.175201f, 60, 0.5f);
  Script_PlayCharaMotion(CHARA_AKIR, 28735, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_181A0(CHARA_AKIR);
  return;
}

static void script_22158(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Vector3 local_E8;
  Script_mapmSetCamFlags(2, 2, 0);
  script_19495(-1);
  if (task_3B1 <= task_3E4 + 3) {
    Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Hide);
    Script_scenCreateTask(script_224D8, 0);
    {
      Vector3 local_DC;
      local_DC.x = 32.810398f;
      (&local_DC)->z = 100.635002f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_DC);
    }
    {
      Vector3i local_DC;
      (&local_DC)->y = 57686;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaDisplay(CHARA_AKIR, CDM_Show);
    script_19495(58);
    script_185A8(CHARA_AKIR, 35.221298f, 96.5578f, 1, 1.0f);
    Script_executeAicaCommand(2720, 2, 30);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_22E18();
    Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY, 1);
    Script_ReturnTimeDate(&local_F4);
    (&local_F4)->hours = 17;
    (&local_F4)->minutes = 0;
    (&local_F4)->seconds = 0;
    Script_SetNewDateTime(&local_F4);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY, 1);
  } else {
    Script_scenCreateTask(script_224D8, 0);
    script_18DF6(SHEN_NAME('CAR4'), 0, 0);
    {
      Vector3 local_DC;
      local_DC.x = 50.716599f;
      (&local_DC)->z = 115.024498f;
      Script_EVT_SetPosition(SHEN_NAME('CAR4'), 0x28000000, &local_DC);
    }
    {
      Vector3i local_DC;
      (&local_DC)->y = 6489;
      Script_EVT_SetRotation(SHEN_NAME('CAR4'), MATRIX_XFORM_Y, &local_DC, 0);
    }
    Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Show);
    Script_scenCreateTask(script_18DF6, 3, SHEN_NAME('CAR4'), 0, 1);
    Script_scenCreateTask(script_191DC, 2, 1193, 10);
    Script_scenCreateTask(script_191DC, 2, 0x1904A9, 60);
    local_E8.x = 35.661301f;
    (&local_E8)->y = 0.0f;
    (&local_E8)->z = 93.094803f;
    Script_EVT_SetPosition(SHEN_NAME('BO_4'), POS_XYZ, &local_E8);
    Script_charaDisplay(SHEN_NAME('BO_4'), CDM_Hide);
    Script_scenCreateTask(script_19200, 2, 23, 0);
    {
      Vector3 local_DC;
      local_DC.x = 32.810398f;
      (&local_DC)->z = 100.635002f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_DC);
    }
    {
      Vector3i local_DC;
      (&local_DC)->y = 57686;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaDisplay(CHARA_AKIR, CDM_Show);
    Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Hide);
    Script_charaDisplay(SHEN_NAME('BO_1'), CDM_Hide);
    Script_charaDisplay(CHARA_FLDF, CDM_Hide);
    script_19495(59);
    script_185A8(CHARA_AKIR, 35.221298f, 96.5578f, 10, 1.5f);
    Script_PlayCharaMotion(CHARA_AKIR, 28735, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_18FFB, 3, CHARA_AKIR, 0x2276AB, 0);
    script_181A0(CHARA_AKIR);
  }
  SCNF_WaitFrames_0000(60);
  return;
}

static int32_t script_224D8(void) {
  {
    Vector3 local_F4;
    local_F4.x = 35.765999f;
    (&local_F4)->z = 93.320801f;
    Script_EVT_SetPosition(CHARA_SMTH, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 62698;
    Script_EVT_SetRotation(CHARA_SMTH, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_SMTH, CDM_Show);
  Script_SetCharaMotionPlaybackRate(CHARA_SMTH, 0.7f);
  script_185A8(CHARA_SMTH, 36.408401f, 82.690804f, 5, 0.5f);
  Script_charaDisplay(CHARA_SMTH, CDM_Hide);
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_225A8(void) {
  {
    Vector3 local_F4;
    local_F4.x = 35.765999f;
    (&local_F4)->z = 93.320801f;
    Script_EVT_SetPosition(CHARA_SMTH, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 62698;
    Script_EVT_SetRotation(CHARA_SMTH, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_SMTH, CDM_Show);
  script_185A8(CHARA_SMTH, 37.268501f, 84.065498f, 5, 0.5f);
  Script_charaDisplay(CHARA_SMTH, CDM_Hide);
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchControl(0);
  return;
}

static void script_2266A(void) {
  int8_t local_FF;
  {
    uint8_t unused_FC[3];
    Vector3 local_F0;
    local_F0.x = 45.169899f;
    (&local_F0)->z = 98.299599f;
    Script_EVT_SetPosition(SHEN_NAME('CAR4'), 0x28000000, &local_F0);
  }
  Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Show);
  Script_scenCreateTask(script_191DC, 2, 132265, 0);
  {
    uint8_t unused_FC[3];
    Vector3i local_F0;
    (&local_F0)->y = 0x2000;
    Script_EVT_SetRotation(SHEN_NAME('CAR4'), MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('CAR4'), &data_31D84, 0, 0);
  while (1) {
    SCNF_WaitFrames_0000(1);
    local_FF = Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('CAR4'), 0.54f);
    if (local_FF == -1) {
      break;
    }
  }
  Script_scenCreateTask(script_191DC, 2, 0x1004A9, 0);
  Script_scenCreateTask(script_191DC, 2, 197801, 5);
  Script_scenCreateTask(script_191DC, 2, 1193, 5);
  {
    uint8_t unused_FC[3];
    Vector3i local_F0;
    (&local_F0)->y = 14507;
    Script_EVT_SetRotation(SHEN_NAME('CAR4'), MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_srchControl(0);
  return;
}

static int32_t script_2276D(int32_t arg0) {
  return SCNF_UIUtils(0x44, arg0, 5, 0, 1, 0);
}

static int32_t script_22782(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_2276D, 1, arg0);
  task_39C = 0;
  while (1) {
    local_F8 = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_39C = task_39C + 1;
    if (local_F8 != 0xFFFF) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  return local_F8;
}

static int32_t script_227E9(int32_t arg0, int32_t arg1) {
  return SCNF_UIUtils(arg0, arg1, 2, 0, 1, 0);
}

static int32_t script_227FE(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_227E9, 2, arg0, arg1);
  task_39C = 0;
  while (1) {
    local_F8 = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_39C = task_39C + 1;
    if (local_F8 != 0xFFFF) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  return local_F8;
}

static void script_22867(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_227E9, 2, arg0, arg1);
  task_39C = 0;
  while (1) {
    local_F8 = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_39C = task_39C + 1;
    if (local_F8 != 0xFFFF) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  while (task_39C <= arg1 + 3) {
    task_39C = task_39C + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  (void)local_F8;
  return;
}

__attribute__((scn_string_bindings(0, data_31DA8)))
static void script_228F3(void) {
  Script_SetMapFlags(0, 1);
  Script_SetCameraFlags(0);
  script_17A82();
  Script_LoadVoiceFile(1, "E1023");
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  script_22958();
  script_18DF6(SHEN_NAME('CAR4'), 0, 0);
  return;
}

static void script_22958(void) {
  task_3C4 = Script_scenCreateTask(script_18529, 2, SHEN_NAME('PIL1'), CHARA_AKIR);
  return;
}

static int32_t script_22976(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
  (&local_F4)->y = 1.75f;
  Script_SetCharaLookAtPosition(arg0, 9, &local_F4, 0);
  SCNF_WaitFrames_0000(arg2);
  Script_SetCharaLookAtPosition(arg0, -9, 0, 0);
  return Script_srchControl(0);
}

static void script_229B7(void) {
  task_3E4 = task_3E4 + 1;
  if (task_3E4 >= 3) {
    task_3E4 = 3;
  }
  script_19430();
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_HATO_Funcs(StopTask);
  script_23678();
  Script_shenClearFreeState(1);
  Script_uiUtils(0, 35, 0);
  Script_sicnUtils(0, 1, 0);
  {
    Vector3 local_F4;
    local_F4.x = -92.669601f;
    (&local_F4)->z = 47.646198f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 56957;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  SCNF_WaitFrames_0000(90);
  Script_shenClearFreeState(0);
  Script_sicnUtils(2, 1, 0);
  script_2380F();
  Script_SetNewDateTime(&task_3B8);
  Script_WeatherTaskUtil(FreeTaskData);
  Script_WeatherTaskUtil(InitTaskData);
  return;
}

__attribute__((scn_string_bindings(0, data_31DAE, 1, data_31DB5, 2, data_31DBC, 3, data_31DBD)))
static void script_22ABE(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_CTS");
  Script_AuthorUtils(0, 0, "PK_CTS", "");
  script_23678();
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(11, 0);
  Script_mapmSetRenderFlag(12, 0);
  Script_mapmSetRenderFlag(13, 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 4, "a1_cts.snd", 0);
  if (0) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L022B7D;
    }
  }
  L022B7D:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 1);
  if (0) {
    Script_charaDisplay(CHARA_SMTH, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_SMTH, CDM_Show);
      goto L022BB1;
    }
  }
  L022BB1:;
  Script_scenToggleVisibleFor(CHARA_SMTH, 1);
  if (0) {
    Script_charaDisplay(CHARA_TONY, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_TONY, CDM_Show);
      goto L022BE5;
    }
  }
  L022BE5:;
  Script_scenToggleVisibleFor(CHARA_TONY, 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BOX1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX1'), CDM_Show);
      goto L022C19;
    }
  }
  L022C19:;
  Script_scenToggleVisibleFor(SHEN_NAME('BOX1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FORK'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FORK'), CDM_Show);
      goto L022C4D;
    }
  }
  L022C4D:;
  Script_scenToggleVisibleFor(SHEN_NAME('FORK'), 1);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(CHARA_TONY, CDM_Show);
  Script_charaDisplay(CHARA_SMTH, CDM_Show);
  Script_executeAicaCommand(68520, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  script_19F76();
  script_19F15(0);
  Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 255, 0, 0, 0);
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_TONY, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_SMTH, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('BOX1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('BOX1'), CDM_Show);
      goto L022D8F;
    }
  }
  L022D8F:;
  Script_scenToggleVisibleFor(SHEN_NAME('BOX1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FORK'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FORK'), CDM_Show);
      goto L022DC3;
    }
  }
  L022DC3:;
  Script_scenToggleVisibleFor(SHEN_NAME('FORK'), 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_sicnUtils(2, 1, 0);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(8, 1);
  Script_mapmSetRenderFlag(11, 1);
  Script_mapmSetRenderFlag(12, 1);
  Script_mapmSetRenderFlag(13, 1);
  script_2380F();
  return;
}

__attribute__((scn_string_bindings(0, data_31DCD, 1, data_31DD2, 2, data_31DD7, 3, data_31DE2, 4, data_31DEE, 5, data_31DF5, 6, data_31E04, 7, data_31E0C, 8, data_31E14, 9, data_31E1C)))
static void script_22E18(void) {
  script_23678();
  Script_mapmSetCamFlags(2, 2, 1);
  Script_srchControl((void *)*&task_3C4);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm102.snd", 0);
  Script_LoadSND(0, 4, "a1_ctsa.snd", 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "PK_CTS");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_CTS2") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_CTS2");
  Script_AuthorUtils(0, 0, "PK_CTS2", "");
  script_1A4BA(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_sicnUtils(2, 1, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_31E1D, 1, data_31E22, 2, data_31E27, 3, data_31E32, 4, data_31E3E, 5, data_31E45, 6, data_31E54, 7, data_31E5C, 8, data_31E64, 9, data_31E6C)))
static void script_22F12(void) {
  script_23678();
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(11, 0);
  Script_mapmSetRenderFlag(12, 0);
  Script_mapmSetRenderFlag(13, 0);
  Script_mapmSetRenderFlag(15, 0);
  Script_mapmSetCamFlags(2, 2, 1);
  Script_srchControl((void *)*&task_3C4);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm102.snd", 0);
  Script_LoadSND(0, 4, "a1_ctsa.snd", 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "PK_CTS");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_CTS2") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_CTS2");
  Script_AuthorUtils(0, 1, "PK_CTS2", "");
  script_1A9A9(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 1);
  Script_mapmSetRenderFlag(8, 1);
  Script_mapmSetRenderFlag(11, 1);
  Script_mapmSetRenderFlag(12, 1);
  Script_mapmSetRenderFlag(13, 1);
  Script_mapmSetRenderFlag(15, 1);
  return;
}

static void script_23091(int32_t arg0) {
  int32_t local_FC;
  local_FC = script_AE4E(arg0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_230B5(void) {
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  script_23584(0x48544D53);
  Script_motmGetCharaFlags(CHARA_SMTH, MOTM_PoseNone, SCEN_Write);
  if (-1) {
    Script_charaDisplay(CHARA_SMTH, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_SMTH, CDM_Show);
      goto L023121;
    }
  }
  L023121:;
  Script_scenToggleVisibleFor(CHARA_SMTH, 0);
  Script_SetCPAMFlags(CHARA_SMTH, 0);
  Script_facemotUtils(CHARA_SMTH, 0, 0);
  Script_facemotUtils(CHARA_SMTH, 1, 0);
  Script_FaceEx(CHARA_SMTH, FACE_ExpressionEnabled, 0, 1);
  script_23091(CHARA_TONY);
  Script_facemotUtils(CHARA_TONY, 0, 0);
  Script_facemotUtils(CHARA_TONY, 1, 0);
  Script_FaceEx(CHARA_TONY, FACE_ExpressionEnabled, 0, 1);
  script_23091(CHARA_BOB_);
  script_23091(CHARA_KOGA);
  script_23091(CHARA_MITA);
  script_23091(CHARA_DICK);
  script_23091(CHARA_PAUR);
  script_23091(CHARA_PEDR);
  script_23091(CHARA_NMKI);
  script_23091(CHARA_FLD7);
  script_23091(CHARA_FLDF);
  script_23091(CHARA_FLDG);
  return;
}

__attribute__((scn_string_bindings(0, data_31E6D, 1, data_31E74, 2, data_31E7B, 3, data_31E84, 4, data_31E8D, 5, data_31E9C, 6, data_31EA3, 8, data_31EB3, 9, data_31EBB)))
static void script_23210(void) {
  Vector3 local_F4;
  int32_t unused_F0;
  Script_SetTaskExecutionSuspended((void *)task_2B8, 1);
  SCNF_WaitFrames_0000(1);
  script_AFD1();
  SCNF_WaitFrames_0000(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_1ST");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "PK_1ST");
  script_230B5();
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_SHUKI");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "PK_SHUKI");
  Script_ReallocateUNFO(task_2BC);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_CTS") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_228F3();
  script_19412(15);
  Script_HATO_Funcs(Alloc);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ccowUtils(SHEN_NAME('FORK'), CCOW_DisableAll);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DOR1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DOR1'), CDM_Show);
      goto L02336F;
    }
  }
  L02336F:;
  Script_scenToggleVisibleFor(SHEN_NAME('DOR1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DOR2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DOR2'), CDM_Show);
      goto L0233A3;
    }
  }
  L0233A3:;
  Script_scenToggleVisibleFor(SHEN_NAME('DOR2'), 0);
  Script_ryoControl(PCCTL_SetCPCTFlag800Inverted, 0);
  Script_GetConversationDetails(CONV_DETAILS_MODE_DISABLE_PROMPT, 0);
  local_F4.x = 0.0f;
  (&local_F4)->y = -5.0f;
  (&local_F4)->z = 0.0f;
  Script_charaPosRotUtils(SHEN_NAME('CAR0'), 196, &local_F4, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('CAR1'), 196, &local_F4, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('CAR2'), 196, &local_F4, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('CAR3'), 196, &local_F4, 0, CHARA_PART_Set);
  Script_uiUtils(0, 21, 0);
  Script_ReturnTimeDate(&task_3B8);
  Script_SetQteOverlayTimeoutTuning(0, QTE_TimeoutSession);
  Script_SetQteOverlayTimeoutTuning(0, QTE_DisableTimeoutBoost);
  task_3E4 = 0;
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "rim006.snd", 0);
  script_23EC6();
  script_1AF78();
  Script_sicnUtils(2, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "PK_CTS2");
  Script_LoadVoiceFile(0, "E1023");
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 205.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  return;
}

static void script_234B8(void) {
  SCNF_WaitFrames_0000(1);
  Script_HATO_Funcs(InitTask, &data_31EC4, 5.0f);
  SCNF_WaitFrames_0000(1);
  Script_HATO_Funcs(StartTask, 4, 10);
  return;
}

static void script_234E7(void) {
  script_AF37(0x594E4F54, 0);
  script_AF37(0x52554150, 0);
  script_AF37(0x5F424F42, 0);
  script_AF37(0x52444550, 0);
  script_AF37(0x4B434944, 0);
  script_AF37(0x494B4D4E, 0);
  script_AF37(0x41474F4B, 0);
  script_AF37(0x4154494D, 0);
  script_AF37(0x37444C46, 0);
  script_AF37(0x46444C46, 0);
  script_AF37(0x47444C46, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_31ED0)))
static void script_23584(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = 0;
  local_FC = 0;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  if (local_FC == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    while (local_FC == 0) {
      if (local_F8 == 0) {
        Script_sprintf("MA00: Syuki Chara Read Failed:ID=%c%c%c%c\012", arg0, arg0 >> 8, arg0 >> 16, arg0 >> 24);
        local_F8 = 1;
      }
      SCNF_WaitFrames_0000(1);
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    }
    L023621:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L023621;
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  (void)local_FC;
  return;
}

static void script_23642(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Hide);
  if (-1) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L02366E;
    }
  }
  L02366E:;
  Script_scenToggleVisibleFor(arg0, 0);
  return;
}

static void script_23678(void) {
  script_23642(SHEN_NAME('FOK0'));
  script_2368C();
  return;
}

static void script_2368C(void) {
  script_23642(CHARA_DICK);
  script_23642(CHARA_PEDR);
  script_23642(CHARA_BOB_);
  script_23642(CHARA_PAUR);
  script_23642(CHARA_NMKI);
  script_23642(CHARA_KOGA);
  script_23642(CHARA_MITA);
  script_23642(CHARA_FLD7);
  script_23642(CHARA_FLDF);
  script_23642(CHARA_FLDG);
  script_23642(SHEN_NAME('CAR0'));
  script_23642(SHEN_NAME('CAR1'));
  script_23642(SHEN_NAME('CAR2'));
  script_23642(SHEN_NAME('CAR3'));
  script_23642(SHEN_NAME('CAR4'));
  script_23642(SHEN_NAME('CAB0'));
  script_23642(SHEN_NAME('FOK1'));
  script_23642(SHEN_NAME('FOK2'));
  script_23642(SHEN_NAME('FOK3'));
  script_23642(SHEN_NAME('FOK4'));
  script_23642(SHEN_NAME('BOX0'));
  script_23642(SHEN_NAME('BO_1'));
  script_23642(SHEN_NAME('BO_2'));
  script_23642(SHEN_NAME('BO_3'));
  script_23642(SHEN_NAME('BO_4'));
  script_23642(SHEN_NAME('BO_5'));
  script_23642(SHEN_NAME('BO_6'));
  script_23642(SHEN_NAME('IKN0'));
  script_23642(SHEN_NAME('PIP0'));
  script_23642(SHEN_NAME('PIP1'));
  script_23642(SHEN_NAME('DORA'));
  script_23642(SHEN_NAME('HASK'));
  return;
}

static void script_2380F(void) {
  if (0) {
    Script_charaDisplay(CHARA_DICK, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_DICK, CDM_Show);
      goto L023839;
    }
  }
  L023839:;
  Script_scenToggleVisibleFor(CHARA_DICK, 1);
  if (0) {
    Script_charaDisplay(CHARA_PEDR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_PEDR, CDM_Show);
      goto L02386D;
    }
  }
  L02386D:;
  Script_scenToggleVisibleFor(CHARA_PEDR, 1);
  if (0) {
    Script_charaDisplay(CHARA_BOB_, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_BOB_, CDM_Show);
      goto L0238A1;
    }
  }
  L0238A1:;
  Script_scenToggleVisibleFor(CHARA_BOB_, 1);
  if (0) {
    Script_charaDisplay(CHARA_PAUR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_PAUR, CDM_Show);
      goto L0238D5;
    }
  }
  L0238D5:;
  Script_scenToggleVisibleFor(CHARA_PAUR, 1);
  if (0) {
    Script_charaDisplay(CHARA_NMKI, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_NMKI, CDM_Show);
      goto L023909;
    }
  }
  L023909:;
  Script_scenToggleVisibleFor(CHARA_NMKI, 1);
  if (0) {
    Script_charaDisplay(CHARA_KOGA, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KOGA, CDM_Show);
      goto L02393D;
    }
  }
  L02393D:;
  Script_scenToggleVisibleFor(CHARA_KOGA, 1);
  if (0) {
    Script_charaDisplay(CHARA_MITA, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_MITA, CDM_Show);
      goto L023971;
    }
  }
  L023971:;
  Script_scenToggleVisibleFor(CHARA_MITA, 1);
  if (0) {
    Script_charaDisplay(CHARA_FLD7, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_FLD7, CDM_Show);
      goto L0239A5;
    }
  }
  L0239A5:;
  Script_scenToggleVisibleFor(CHARA_FLD7, 1);
  if (0) {
    Script_charaDisplay(CHARA_FLDF, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_FLDF, CDM_Show);
      goto L0239D9;
    }
  }
  L0239D9:;
  Script_scenToggleVisibleFor(CHARA_FLDF, 1);
  if (0) {
    Script_charaDisplay(CHARA_FLDG, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_FLDG, CDM_Show);
      goto L023A0D;
    }
  }
  L023A0D:;
  Script_scenToggleVisibleFor(CHARA_FLDG, 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR0'), CDM_Show);
      goto L023A41;
    }
  }
  L023A41:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Show);
      goto L023A75;
    }
  }
  L023A75:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
      goto L023AA9;
    }
  }
  L023AA9:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR3'), CDM_Show);
      goto L023ADD;
    }
  }
  L023ADD:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Show);
      goto L023B11;
    }
  }
  L023B11:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAR4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('CAB0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('CAB0'), CDM_Show);
      goto L023B45;
    }
  }
  L023B45:;
  Script_scenToggleVisibleFor(SHEN_NAME('CAB0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK0'), CDM_Show);
      goto L023B79;
    }
  }
  L023B79:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK1'), CDM_Show);
      goto L023BAD;
    }
  }
  L023BAD:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK2'), CDM_Show);
      goto L023BE1;
    }
  }
  L023BE1:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK3'), CDM_Show);
      goto L023C15;
    }
  }
  L023C15:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FOK4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FOK4'), CDM_Show);
      goto L023C49;
    }
  }
  L023C49:;
  Script_scenToggleVisibleFor(SHEN_NAME('FOK4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BOX0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BOX0'), CDM_Show);
      goto L023C7D;
    }
  }
  L023C7D:;
  Script_scenToggleVisibleFor(SHEN_NAME('BOX0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BO_1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BO_1'), CDM_Show);
      goto L023CB1;
    }
  }
  L023CB1:;
  Script_scenToggleVisibleFor(SHEN_NAME('BO_1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BO_2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BO_2'), CDM_Show);
      goto L023CE5;
    }
  }
  L023CE5:;
  Script_scenToggleVisibleFor(SHEN_NAME('BO_2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BO_3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BO_3'), CDM_Show);
      goto L023D19;
    }
  }
  L023D19:;
  Script_scenToggleVisibleFor(SHEN_NAME('BO_3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BO_4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BO_4'), CDM_Show);
      goto L023D4D;
    }
  }
  L023D4D:;
  Script_scenToggleVisibleFor(SHEN_NAME('BO_4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BO_5'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BO_5'), CDM_Show);
      goto L023D81;
    }
  }
  L023D81:;
  Script_scenToggleVisibleFor(SHEN_NAME('BO_5'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BO_6'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BO_6'), CDM_Show);
      goto L023DB5;
    }
  }
  L023DB5:;
  Script_scenToggleVisibleFor(SHEN_NAME('BO_6'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('IKN0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('IKN0'), CDM_Show);
      goto L023DE9;
    }
  }
  L023DE9:;
  Script_scenToggleVisibleFor(SHEN_NAME('IKN0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('PIP0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('PIP0'), CDM_Show);
      goto L023E1D;
    }
  }
  L023E1D:;
  Script_scenToggleVisibleFor(SHEN_NAME('PIP0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('PIP1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('PIP1'), CDM_Show);
      goto L023E51;
    }
  }
  L023E51:;
  Script_scenToggleVisibleFor(SHEN_NAME('PIP1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('DORA'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('DORA'), CDM_Show);
      goto L023E85;
    }
  }
  L023E85:;
  Script_scenToggleVisibleFor(SHEN_NAME('DORA'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASK'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASK'), CDM_Show);
      goto L023EB9;
    }
  }
  L023EB9:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASK'), 1);
  return;
}

static void script_23EC6(void) {
  Script_ambSoundUtils(4, 0, &data_31EFC, 10.0f, 5.0f, 1.0f, 0, &data_31F08);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_23EF7(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 0) {
    Script_scenCreateTask(script_2427D, 0);
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    goto L023F30;
  }
  L023F30:;
  return;
}

static void script_23F31(void) {
  script_24080(CHARA_AKIR);
  script_2401D(CHARA_PAUR);
  script_2401D(CHARA_BOB_);
  script_2401D(CHARA_RBRT);
  script_2400A(SHEN_NAME('FORK'));
  script_2400A(SHEN_NAME('FNUM'));
  script_2400A(SHEN_NAME('DOOR'));
  script_2400A(SHEN_NAME('SHAT'));
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_31F90);
  Script_charaSetRefboardFlagTo(CHARA_PAUR, 1);
  Script_SetCharaRefBoardPos(CHARA_PAUR, &data_31F90);
  Script_charaSetRefboardFlagTo(CHARA_BOB_, 1);
  Script_SetCharaRefBoardPos(CHARA_BOB_, &data_31F90);
  Script_charaSetRefboardFlagTo(CHARA_RBRT, 1);
  Script_SetCharaRefBoardPos(CHARA_RBRT, &data_31F90);
  Script_SetShadowRange(1000000.0f);
  return;
}

static void script_2400A(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  return;
}

static void script_2401D(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 10, 16);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_scenToggleActiveFor(arg0, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  return;
}

static void script_24080(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_facemotUtils(arg0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 8, 16);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_scenToggleActiveFor(arg0, 1);
  Script_SetCharaFlags(arg0, 4);
  Script_clmdSetFlags(arg0, 2, 0);
  Script_SetCPAMFlags(arg0, 0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  return;
}

static void script_24134(void) {
  script_24202(CHARA_AKIR);
  script_241A6(CHARA_PAUR);
  script_241A6(CHARA_BOB_);
  script_241A6(CHARA_RBRT);
  script_241A3(0x4B524F46);
  script_241A3(0x4D554E46);
  script_241A3(0x5F584F42);
  script_241A3(0x524F4F44);
  script_241A3(0x54414853);
  return;
}

static void script_241A3(int32_t arg0) {
  return;
}

static void script_241A6(int32_t arg0) {
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_motmGetCharaFlags(arg0, MOTM_PoseUnknown3, SCEN_Write);
  return;
}

static void script_24202(int32_t arg0) {
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_FACE(arg0, 0, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseUnknown3, SCEN_Write);
  return;
}

static int32_t script_2427D(void) {
  int16_t local_FE;
  Vector3i local_F0;
  local_F0.x = -1092;
  (&local_F0)->y = 0;
  (&local_F0)->z = 0;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if ((local_FE > 0) & (local_FE < 101)) {
        Script_charaPosRotUtils(SHEN_NAME('FORK'), 8, 0, &local_F0, CHARA_PART_Add);
        Script_charaPosRotUtils(SHEN_NAME('FORK'), 23, 0, &local_F0, CHARA_PART_Add);
        Script_charaPosRotUtils(SHEN_NAME('FORK'), 18, 0, &local_F0, CHARA_PART_Add);
      }
      if ((local_FE >= 81) & (local_FE < 101)) {
        local_F0.x = (&local_F0)->x + 54;
      }
      if (local_FE == 101) {
        Script_executeAicaCommand(0x1100A0, 4, 0);
      }
      switch (local_FE) {
        case 0:
          Script_CharHandlers(CHARA_RBRT, 1, 8, 16);
          Script_CharHandlers(CHARA_RBRT, 0, 8, 16);
          Script_CharHandlers(CHARA_BOB_, 1, 8, 16);
          Script_CharHandlers(CHARA_BOB_, 0, 8, 16);
          Script_CharHandlers(CHARA_PAUR, 1, 8, 16);
          Script_CharHandlers(CHARA_PAUR, 0, 8, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 8, 16);
          Script_CharHandlers(CHARA_AKIR, 0, 8, 16);
          break;
        case 350:
          Script_CharHandlers(CHARA_RBRT, 1, 4, 16);
          Script_CharHandlers(CHARA_RBRT, 0, 4, 16);
          break;
        case 395:
          Script_CharHandlers(CHARA_RBRT, 1, 8, 16);
          Script_CharHandlers(CHARA_RBRT, 0, 8, 16);
          break;
        case 1130:
          Script_CharHandlers(CHARA_RBRT, 1, 4, 16);
          Script_CharHandlers(CHARA_RBRT, 0, 4, 16);
          break;
        case 1255:
          Script_CharHandlers(CHARA_RBRT, 1, 8, 16);
          Script_CharHandlers(CHARA_RBRT, 0, 8, 16);
          break;
        case 1445:
          Script_CharHandlers(CHARA_RBRT, 1, 4, 16);
          Script_CharHandlers(CHARA_RBRT, 0, 4, 16);
          break;
        case 1560:
          Script_CharHandlers(CHARA_RBRT, 1, 8, 16);
          Script_CharHandlers(CHARA_RBRT, 0, 8, 16);
          break;
        case 1610:
          Script_CharHandlers(CHARA_BOB_, 1, 0, 16);
          Script_CharHandlers(CHARA_BOB_, 0, 0, 16);
          break;
        case 1655:
          Script_CharHandlers(CHARA_RBRT, 1, 0, 16);
          Script_CharHandlers(CHARA_RBRT, 0, 0, 16);
          break;
        case 1665:
          Script_CharHandlers(CHARA_PAUR, 1, 0, 16);
          Script_CharHandlers(CHARA_PAUR, 0, 0, 16);
          break;
        case 1710:
          Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
          break;
      }
      local_FE = local_FE + 1;
    } else {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2458D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_245A7, 6, arg1, arg5, arg2, arg3, arg0, arg4);
  return;
}

static int32_t script_245A7(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_motmGetCharaFlags(arg4, MOTM_PoseLow, SCEN_Write);
  Script_charaToggleMotEnabled(arg4, 1);
  Script_motmToggleFlags(arg4, SCEN_Enable, 0x40);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (arg5 == 1) {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x80);
  } else {
    Script_motmToggleFlags(arg4, SCEN_Disable, 0x80);
  }
  if (arg5 == 2) {
    Script_motmToggleFlags(arg4, SCEN_Disable, 0x40);
  }
  return Script_srchControl(0);
}

static void script_2463D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_24657, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_24657(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  int32_t local_FC;
  Vector3i local_F0;
  Vector3 local_E4;
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_E4.x = arg2;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_E4);
  local_F0.x = 0;
  (&local_F0)->y = (int32_t)(arg4 / 360.0f * (float)0x10000);
  (&local_F0)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F0, 0);
  Script_InitializeOsag(arg0);
  return Script_srchControl(0);
}

static void script_246E6(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_246FA, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_246FA(int32_t arg0, float arg1, int32_t arg2) {
  Vector3i local_F4;
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  local_F4.x = 0;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->z = 0;
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return Script_srchControl(0);
}

static void script_24749(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_2475F, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_2475F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, arg2, &local_F4, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 8);
  return Script_srchControl(0);
}

static void script_247A2(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_247B6, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_247B6(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_SetCharaLookAtPosition(arg0, -arg1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return Script_srchControl(0);
}

static void script_247E8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_247FE, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_247FE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg0 == -1) {
    Script_SetGameStateTo(AUTH_None);
  } else {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return Script_srchControl(0);
}

static void script_24834(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_24846, 2, arg0, arg1);
  return;
}

static int32_t script_24846(const char *arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  Script_ShowUIMessageFromID(arg0);
  return Script_srchControl(0);
}

static void script_2486D(void) {
  Script_charaVisUtils(CHARA_PAUR, 1);
  Script_charaVisUtils(CHARA_BOB_, 1);
  Script_charaVisUtils(CHARA_RBRT, 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_charaSetRefboardFlagTo(CHARA_PAUR, 1);
  Script_charaSetRefboardFlagTo(CHARA_BOB_, 1);
  Script_charaSetRefboardFlagTo(CHARA_RBRT, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_EVT_SetPosition(SHEN_NAME('TBAS'), POS_XYZ, &data_31FB8);
  Script_EVT_SetRotation(SHEN_NAME('TBAS'), 0x38000000, &data_31FC4, 0);
  Script_SetCPAMFlags(CHARA_PAUR, 0);
  Script_SetCPAMFlags(CHARA_BOB_, 0);
  Script_SetCPAMFlags(CHARA_RBRT, 0);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ccowUtils(CHARA_PAUR, CCOW_DisableAll);
  Script_ccowUtils(CHARA_BOB_, CCOW_DisableAll);
  Script_ccowUtils(CHARA_RBRT, CCOW_DisableAll);
  Script_motmSetBlendFrames(CHARA_PAUR, MOTM_BLEND_FRAMES_10);
  script_247E8(3500, SHEN_NAME('TBAS'), SHEN_NAME('TBAS'), -1);
  script_247E8(3501, SHEN_NAME('TBAS'), SHEN_NAME('TBAS'), 120);
  script_247E8(3502, SHEN_NAME('TBAS'), SHEN_NAME('TBAS'), 170);
  script_2463D(CHARA_AKIR, 0x53414254, -0x3D1C6666, 0x42606666, 0x43660000, -1);
  script_2463D(CHARA_PAUR, 0x53414254, -0x3D223333, 0x42653333, 0x437A0000, -1);
  script_2463D(CHARA_BOB_, 0x53414254, -0x3CAE999A, 0x426B999A, 0x436B0000, -1);
  script_2463D(CHARA_RBRT, 0x53414254, -0x3CB06666, 0x42693333, 0x43820000, -1);
  script_2463D(CHARA_BOB_, 0x53414254, -0x3D253333, 0x426B999A, 0x436B0000, 120);
  script_2463D(CHARA_RBRT, 0x53414254, -0x3D28CCCD, 0x42693333, 0x43820000, 120);
  script_2458D(CHARA_AKIR, 655, 0, 0x3F800000, 2, -1);
  script_2458D(CHARA_AKIR, 89, 0, 0x3F800000, -1, 30);
  script_2458D(CHARA_PAUR, 29526, 0x42480000, 0x3F800000, 2, -1);
  script_2458D(CHARA_PAUR, 29524, 0, 0x3F800000, -1, 160);
  script_2458D(CHARA_PAUR, 29524, 0, 0x3F800000, 2, 170);
  script_2458D(CHARA_RBRT, 29524, 0, 0x3F19999A, 2, 120);
  script_2458D(CHARA_RBRT, 29524, 0, 0x3F800000, 2, 170);
  script_2458D(CHARA_BOB_, 29523, 0x42200000, 0x3F800000, 2, 120);
  script_2458D(CHARA_BOB_, 29524, 0x41200000, 0x3F800000, 2, 170);
  script_24834((int32_t)"E1071B001", -1);
  script_24834((int32_t)"E1071A001", 40);
  script_24834((int32_t)"E1071B002", 120);
  script_24834((int32_t)"E1071C001", 150);
  script_24834((int32_t)"E1071A002", 180);
  script_24834((int32_t)"E1071A003", 260);
  SCNF_WaitFrames_0000(300);
  Script_SetCPAMFlags(CHARA_PAUR, 1);
  Script_SetCPAMFlags(CHARA_BOB_, 1);
  Script_SetCPAMFlags(CHARA_RBRT, 1);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_charaVisUtils(CHARA_PAUR, 0);
  Script_charaVisUtils(CHARA_BOB_, 0);
  Script_charaVisUtils(CHARA_RBRT, 0);
  return;
}

static void script_24C08(void) {
  int32_t local_FC;
  Script_FACE(CHARA_AKIR, 4, 5, 15);
  Script_charaSetRefboardFlagTo(CHARA_PAUR, 1);
  Script_charaSetRefboardFlagTo(CHARA_BOB_, 1);
  Script_charaSetRefboardFlagTo(CHARA_RBRT, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_EVT_SetPosition(SHEN_NAME('TBAS'), POS_XYZ, &data_3200C);
  Script_EVT_SetRotation(SHEN_NAME('TBAS'), 0x38000000, &data_32018, 0);
  Script_SetCPAMFlags(CHARA_PAUR, 0);
  Script_SetCPAMFlags(CHARA_BOB_, 0);
  Script_SetCPAMFlags(CHARA_RBRT, 0);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_charaVisUtils(CHARA_PAUR, 1);
  Script_charaVisUtils(CHARA_BOB_, 1);
  Script_charaVisUtils(CHARA_RBRT, 1);
  Script_ccowUtils(CHARA_PAUR, CCOW_DisableAll);
  Script_ccowUtils(CHARA_BOB_, CCOW_DisableAll);
  Script_ccowUtils(CHARA_RBRT, CCOW_DisableAll);
  Script_motmSetBlendFrames(CHARA_PAUR, MOTM_BLEND_FRAMES_8);
  Script_motmSetBlendFrames(CHARA_BOB_, MOTM_BLEND_FRAMES_8);
  Script_motmSetBlendFrames(CHARA_RBRT, MOTM_BLEND_FRAMES_8);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_2458D(CHARA_AKIR, 29522, 0, 0, 2, 1);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  script_247E8(3503, SHEN_NAME('TBAS'), SHEN_NAME('TBAS'), -1);
  script_247E8(3504, SHEN_NAME('TBAS'), SHEN_NAME('TBAS'), 90);
  script_247E8(3505, SHEN_NAME('TBAS'), SHEN_NAME('TBAS'), 180);
  script_2463D(CHARA_AKIR, 0x53414254, -0x3D23D1EC, 0x426A51EC, 0x43610000, -1);
  script_2463D(CHARA_PAUR, 0x53414254, -0x3D286666, 0x426F3333, 0x42700000, -1);
  script_2463D(CHARA_BOB_, 0x53414254, -0x3D263333, 0x4272CCCD, 0x41F00000, -1);
  script_2463D(CHARA_RBRT, 0x53414254, -0x3D243333, 0x42673333, 0x43070000, -1);
  script_246E6(CHARA_RBRT, 0x42DC0000, 223);
  script_246E6(CHARA_PAUR, -0x3CE00000, 295);
  script_246E6(CHARA_BOB_, -0x3D240000, 316);
  script_24834((int32_t)"E1071A004", -1);
  script_24834((int32_t)"E1071B003", 60);
  script_24834((int32_t)"E1071B004", 110);
  script_24834((int32_t)"E1071B005", 220);
  script_24834((int32_t)"E1071C002", 290);
  script_24834((int32_t)"E1071A005", 500);
  script_2458D(CHARA_AKIR, 29522, 0x43960000, 0x3F800000, -1, 340);
  script_2458D(CHARA_RBRT, 29521, 0, 0x3F800000, 2, -1);
  script_2458D(CHARA_PAUR, 89, 0, 0x3F800000, 2, -1);
  script_2458D(CHARA_BOB_, 29525, 0, 0x3F800000, 2, -1);
  script_2458D(CHARA_RBRT, 1076, 0, 0x3F800000, 1, 210);
  script_2458D(CHARA_RBRT, 655, 0, 0x3F800000, -1, 223);
  script_2458D(CHARA_PAUR, 1076, 0, 0x3F800000, -1, 280);
  script_2458D(CHARA_PAUR, 655, 0, 0x3F800000, -1, 295);
  script_2458D(CHARA_BOB_, 1076, 0, 0x3F800000, -1, 300);
  script_2458D(CHARA_BOB_, 655, 0, 0x3F800000, -1, 316);
  SCNF_WaitFrames_0000(90);
  local_FC = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(-local_FC, 1);
  Script_SetStats(STATS_CASH, 0);
  SCNF_WaitFrames_0000(440);
  Script_FACE(CHARA_AKIR, 0, -1, 15);
  Script_SetCPAMFlags(CHARA_PAUR, 1);
  Script_SetCPAMFlags(CHARA_BOB_, 1);
  Script_SetCPAMFlags(CHARA_RBRT, 1);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_charaVisUtils(CHARA_PAUR, 0);
  Script_charaVisUtils(CHARA_BOB_, 0);
  Script_charaVisUtils(CHARA_RBRT, 0);
  return;
}

static void script_2506B(int32_t arg0, int32_t arg1) {
  script_AF37(arg0, arg1);
  return;
}

__attribute__((scn_string_bindings(0, data_32060, 1, data_32066, 2, data_3206D, 3, data_32074, 4, data_3207B, 5, data_3207C)))
static void script_25079(void) {
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_LoadVoiceFile(1, "01TOU");
  script_23F31();
  Script_charaDisplay(SHEN_NAME('SHAT'), CDM_Hide);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 3, "PK_TOU") == 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "PK_TOU");
  }
  Script_executeAicaCommand(9640, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  Script_AuthorUtils(0, 0, "PK_TOU", "");
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_23EF7(0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_LoadVoiceFile(0, "01TOU");
  script_24134();
  return;
}

__attribute__((scn_string_bindings(0, data_32082, 1, data_32087, 2, data_3208C, 3, data_32091, 4, data_32096, 9, data_320C9, 10, data_320CF, 11, data_320DE, 12, data_320E5, 13, data_320EC, 14, data_320FB, 15, data_32103, 16, data_3210B, 17, data_32111, 18, data_32116, 19, data_3211B, 20, data_32120, 21, data_32125, 22, data_3212A, 23, data_32135)))
static void script_2514C(void) {
  int32_t unused_FC;
  int32_t local_F0[3];
  int32_t local_EC;
  int32_t local_DC[4];
  float local_D8;
  Script_filmCheckOrSetTransitionUtil(16, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_sicnUtils(2, 0xFFF, 0);
  script_B159(task_2B4);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_forkSetSoundBankDetails(SHEN_NAME('FORK'));
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  task_2C1 = 3;
  Script_SetupClothData(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  task_478 = script_AE4E(0x52554150);
  *(&task_478 + 1) = script_AE4E(0x5F424F42);
  *(&task_478 + 2) = script_AE4E(0x54524252);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  SCNF_WaitFrames_0000(3);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  Script_LoadSND(0, 6, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm013.snd", 0);
  Script_LoadSND(0, 4, "a1_tou.snd", 0);
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  Script_ccowUtils(SHEN_NAME('FORK'), CCOW_DisableAll);
  local_EC = Script_motiLoad(3, SHEN_NAME('MA00'), "M_ZAKO.BIN");
  Script_LoadVoiceFile(1, "E1071");
  Script_charaSetRefboardFlagTo(CHARA_PAUR, 1);
  Script_charaSetRefboardFlagTo(CHARA_BOB_, 1);
  Script_charaSetRefboardFlagTo(CHARA_RBRT, 1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_TOU") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_charaVisUtils(CHARA_PAUR, 1);
  Script_charaVisUtils(CHARA_BOB_, 1);
  Script_charaVisUtils(CHARA_RBRT, 1);
  script_25079();
  if (Script_shenSeqUtils(-7) != 0) {
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  local_DC[0] = Script_scenSetAssignCVWK(CHARA_AKIR, 1);
  local_DC[1] = Script_scenSetAssignCVWK(CHARA_RBRT, 1);
  local_DC[2] = Script_scenSetAssignCVWK(CHARA_PAUR, 1);
  local_DC[3] = Script_scenSetAssignCVWK(CHARA_BOB_, 1);
  Script_scenSetAssignCVWK(CHARA_AKIR, 0, 1);
  Script_scenSetAssignCVWK(CHARA_RBRT, 0, 1);
  Script_scenSetAssignCVWK(CHARA_PAUR, 0, 1);
  Script_scenSetAssignCVWK(CHARA_BOB_, 0, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_BATTLE_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_RBRT, MN_AKI_AKI_BATTLE_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_PAUR, MN_AKI_AKI_BATTLE_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_BOB_, MN_AKI_AKI_BATTLE_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_A9CE(CHARA_AKIR, -109.9f, 58.3f, 18204);
  script_D109(CHARA_PAUR, -110.7f, 60.36f, 61531);
  script_D109(CHARA_BOB_, -112.45f, 58.36f, 49261);
  script_D109(CHARA_RBRT, -112.3f, 59.53f, 53703);
  script_A9CE(SHEN_NAME('FORK'), -106.49f, 57.16f, 0x4000);
  Script_seqEnterEvtCam(3503, SHEN_NAME('TBAS'), SHEN_NAME('TBAS'));
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "PK_TOU");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_TOU2") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_vfSetup(0, 20, 20, 0, 0);
  local_F0[0] = -0x3D18CCCD;
  local_F0[1] = 0;
  local_F0[2] = 0x4274CCCD;
  task_1A0 = 0x52494B41;
  *(&task_1A0 + 1) = 0;
  *(&task_1A0 + 2) = 0;
  *(&task_1A0 + 3) = 0;
  *(&task_1A0 + 4) = 200;
  *(&task_1A0 + 5) = 0;
  *(&task_1A0 + 6) = 0;
  *(&task_1A0 + 7) = 0;
  *(&task_1A0 + 8) = 0;
  *(&task_1A0 + 9) = 0;
  *(&task_1A0 + 10) = 0;
  Script_vfUtils(&task_1A0);
  task_1A0 = 0x52554150;
  *(&task_1A0 + 1) = 0;
  *(&task_1A0 + 2) = 1;
  *(&task_1A0 + 3) = 1;
  *(&task_1A0 + 4) = 80;
  *(&task_1A0 + 5) = 23;
  *(&task_1A0 + 6) = 0;
  *(&task_1A0 + 7) = 0;
  *(&task_1A0 + 8) = 0;
  *(&task_1A0 + 9) = 0;
  *(&task_1A0 + 10) = 0;
  Script_vfUtils(&task_1A0);
  Script_vfConfigureFormationGroup(CHARA_PAUR, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(CHARA_PAUR, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(CHARA_PAUR, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_1A0 = 0x5F424F42;
  *(&task_1A0 + 1) = 0;
  *(&task_1A0 + 2) = 1;
  *(&task_1A0 + 3) = 1;
  *(&task_1A0 + 4) = 80;
  *(&task_1A0 + 5) = 24;
  *(&task_1A0 + 6) = 0;
  *(&task_1A0 + 7) = 0;
  *(&task_1A0 + 8) = 0;
  *(&task_1A0 + 9) = 0;
  *(&task_1A0 + 10) = 0;
  Script_vfUtils(&task_1A0);
  Script_vfConfigureFormationGroup(CHARA_BOB_, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(CHARA_BOB_, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(CHARA_BOB_, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_1A0 = 0x54524252;
  *(&task_1A0 + 1) = 0;
  *(&task_1A0 + 2) = 1;
  *(&task_1A0 + 3) = 1;
  *(&task_1A0 + 4) = 80;
  *(&task_1A0 + 5) = 25;
  *(&task_1A0 + 6) = 0;
  *(&task_1A0 + 7) = 0;
  *(&task_1A0 + 8) = 0;
  *(&task_1A0 + 9) = 0;
  *(&task_1A0 + 10) = 0;
  Script_vfUtils(&task_1A0);
  Script_vfConfigureFormationGroup(CHARA_RBRT, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(CHARA_RBRT, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(CHARA_RBRT, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  local_D8 = Script_LODUtils(LOD_UTILS_GET_DISTANCE_0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  if (Script_shenSeqUtils(-7) != 0) {
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  SCN_Enqueue_VFGI(2, 99, 1.0f, 1, -1);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, local_D8);
  if (Script_GetFightingStats(0x52494B41, 0) != 3) {
    script_24C08();
    task_1A0 = -6;
    *(&task_1A0 + 1) = 0;
    *(&task_1A0 + 2) = 0;
    *(&task_1A0 + 3) = 0;
    *(&task_1A0 + 4) = 0;
    *(&task_1A0 + 5) = 7;
    *(&task_1A0 + 6) = 0;
    *(&task_1A0 + 7) = 0;
    *(&task_1A0 + 8) = 0;
    *(&task_1A0 + 9) = 0;
    *(&task_1A0 + 10) = 0;
    Script_vfUtils(&task_1A0);
  } else {
    script_2486D();
    task_1A0 = -6;
    *(&task_1A0 + 1) = 0;
    *(&task_1A0 + 2) = 0;
    *(&task_1A0 + 3) = 0;
    *(&task_1A0 + 4) = 0;
    *(&task_1A0 + 5) = 7;
    *(&task_1A0 + 6) = 0;
    *(&task_1A0 + 7) = 0;
    *(&task_1A0 + 8) = 0;
    *(&task_1A0 + 9) = 0;
    *(&task_1A0 + 10) = 0;
    Script_vfUtils(&task_1A0);
  }
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenSetAssignCVWK(CHARA_AKIR, 0, *(int32_t *)local_DC);
  Script_scenSetAssignCVWK(CHARA_RBRT, 0, local_DC[1]);
  Script_scenSetAssignCVWK(CHARA_PAUR, 0, local_DC[2]);
  Script_scenSetAssignCVWK(CHARA_BOB_, 0, local_DC[3]);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_PAUR, 2);
  Script_charaSetRefboardFlagTo(CHARA_BOB_, 2);
  Script_charaSetRefboardFlagTo(CHARA_RBRT, 2);
  script_2506B(0x52554150, *&task_478);
  script_2506B(0x5F424F42, *(&task_478 + 1));
  script_2506B(0x54524252, *(&task_478 + 2));
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "PK_TOU2");
  Script_LoadVoiceFile(0, "E1071");
  Script_ReallocateUNFO(local_EC);
  Script_LoadSND(0, 6, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm062.snd", 0);
  Script_LoadSND(0, 4, "e1forkli.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 114);
  Script_ccowUtils(SHEN_NAME('FORK'), CCOW_EnableAll);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 1);
  Script_timeSetUpdateEnabled(1);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_SetActionSelectorHUDElementVisibility(0);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  script_B1ED(CHARA_AKIR, SHEN_NAME('FORK'));
  script_A9CE(SHEN_NAME('FORK'), -103.01f, 58.52f, 16029);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 857, 1);
  Script_sicnUtils(0, 1, 1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  task_2B4 = script_B094();
  Script_filmCheckOrSetTransitionUtil(16, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_32142, 2, data_32158, 4, data_32170, 5, data_3217F, 8, data_3219A)))
void script_25C6D(void) {
  int32_t local_FC;
  int32_t unused_F8;
  GameDateTime local_F0;
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "ALLMAP") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_OTHER") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00", "PK_SHUKI") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  task_2BC = Script_motiLoad(3, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 4);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  Script_facemotUtils(SHEN_NAME('HNDL'), 1, 0);
  Script_ResetHandMotion(SHEN_NAME('HNDL'), 1, 0, 0);
  script_A7C5();
  Script_LoadVoiceFile(1, "E1001B");
  Script_LoadVoiceFile(1, "E1001D");
  Script_LoadVoiceFile(1, "E1001E");
  Script_ryoSleepUtils(2, 0);
  script_8588(CHARA_AKIR, (DSWKDoorEntry *)data_28840, (int32_t *)&data_28B4C, (DoorResourceDescriptor *)&data_28C28, (MapObjectRecord *)data_28C88, 10, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 205.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_SetCameraFieldOfView(10922);
  Script_SetMapFlags(0, 1);
  local_FC = Script_GetCurrentEntryPoint();
  if (local_FC >= 10) {
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 29, local_FC - 10);
    Script_ReturnTimeDate(&local_F0);
    if (Script_GetControllerInput(0)) {
      (&local_F0)->hours = 14;
    } else {
      (&local_F0)->hours = 9;
    }
    (&local_F0)->minutes = 0;
    (&local_F0)->seconds = 0;
    Script_SetNewDateTime(&local_F0);
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, 0);
    Script_ForkliftUtils(FLMW_MODE_CLEAR_FOLLOWER_SNAPSHOT);
  }
  task_2C3 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 29);
  switch (task_2C3) {
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_MARKS_BROTHER_KILLED)) {
        task_2C3 = task_2C3 + 1;
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 29, task_2C3);
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, 0);
      }
      break;
    case 4:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LAN_DI_STILL_IN_HARBOR)) {
        task_2C3 = task_2C3 + 1;
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 29, task_2C3);
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, 0);
      }
      break;
    case 5:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LONG_ZHA)) {
        task_2C3 = task_2C3 + 1;
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 29, task_2C3);
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, 0);
      }
      break;
  }
  if (task_2C3 == 0) {
    task_2C3 = 1;
  }
  if (task_2C3 >= 7) {
    task_2C3 = 6;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LUNCH_WITH_TOM)) {
    Script_shenRequestGameModuleTransition(3, SHEN_NAME('MFSY'), 1);
  }
  Script_scenCreateTask(script_10E94, 1, CHARA_AKIR);
  return;
}
