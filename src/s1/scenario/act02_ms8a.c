#include "act02_ms8a.h"

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

static void script_0125(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_0153(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
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

static void script_0275(void) {
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

static void script_036D(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static int32_t script_039D(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Script_GetDataFromFightProgram(arg0, arg1);
  local_FC = 0;
  while (local_FC < arg2) {
    local_FC = local_FC + 1;
    Script_SetFightProgramStateBytes(arg0, local_FC, 0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_03D7(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 0);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  end:;
}

static void script_040B(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t unused_E0[3];
  int32_t local_DC;
  script_0030();
  script_1C42();
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 2);
  }
  Script_shenSeqUtils(arg1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_scenCreateTask(script_1B31, 0);
        Script_SetMotnFlag(CHARA_TAIJ, -1);
        Script_motmToggleFlags(CHARA_TAIJ, SCEN_Disable, 0x40);
        Script_SetMotnFlag(CHARA_CHAI, -1);
        Script_motmToggleFlags(CHARA_CHAI, SCEN_Disable, 0x40);
        Script_FACE(CHARA_TAIJ, 3, 0, 10);
        Script_FACE_LCG(CHARA_TAIJ, FACE_BlinkOpen, 1, 0);
        Script_FACE_LCG(CHARA_CHAI, FACE_BlinkOpen, 1, 0);
        break;
      case 34:
        Script_FACE(CHARA_TAIJ, 5, 0, 5);
        break;
      case 56:
        Script_executeAicaCommand(3496, 0, 0);
        Script_executeAicaCommand(1184, 2, 102);
        break;
      case 67:
        Script_scenCreateTask(script_039D, 3, CHARA_CHAI, 1, 6);
        break;
      case 87:
        Script_FACE(CHARA_CHAI, 1, 2, 8);
        break;
      case 117:
        Script_FACE(CHARA_CHAI, 9, 0, 12);
        break;
      case 140:
        Script_FACE(CHARA_AKIR, 7, 0, 16);
        Script_FACE(CHARA_KISY, 2, 0, 16);
        break;
      case 150:
        Script_FACE(CHARA_AKIR, 2, 0, 8);
        break;
      case 157:
        Script_FACE(CHARA_CHAI, 10, 0, 20);
        break;
      case 159:
        Script_scenCreateTask(script_03D7, 2, CHARA_CHAI, 6);
        break;
      case 166:
        Script_FaceLookAtTargetUtils(CHARA_CHAI, FACE_LookReset);
        break;
    }
    if ((local_FC > 75) & (local_FC < 166)) {
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_EC, 0);
      Script_FaceLookAtTargetUtils(CHARA_CHAI, FACE_LookAtPosition, &local_EC, 3);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_1E18();
  return;
}

static void script_066D(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_E0[7];
  int32_t local_DC;
  script_0030();
  script_1C42();
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 2);
  }
  Script_shenSeqUtils(arg1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
        Script_ResetHandMotion(CHARA_CHAI, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_CHAI, 0, data_29B0, 1);
        Script_FACE(CHARA_AKIR, 7, 0, 1);
        Script_FACE(CHARA_CHAI, 1, 0, 1);
        if (-1) {
          Script_charaDisplay(SHEN_NAME('HDBN'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('HDBN'), CDM_Show);
            goto L000722;
          }
        }
        L000722:;
        Script_scenToggleVisibleFor(SHEN_NAME('HDBN'), 0);
        Script_SetMotnFlag(CHARA_TAIJ, 0);
        Script_scenCreateTask(script_039D, 3, CHARA_CHAI, 1, 4);
        Script_FaceEx(CHARA_KISY, FACE_ExpressionDisabled, 0, 1);
        Script_FaceEx(CHARA_TAIJ, FACE_ExpressionDisabled, 0, 1);
        break;
      case 10:
        Script_scenCreateTask(script_03D7, 2, CHARA_CHAI, 4);
        break;
      case 42:
        Script_FACE(CHARA_AKIR, 10, 0, 6);
        break;
      case 43:
        Script_scenCreateTask(script_039D, 3, CHARA_CHAI, 1, 5);
        break;
      case 49:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
        Script_FaceEx(CHARA_CHAI, FACE_ExpressionDisabled, 0, 1);
        Script_ResetHandMotion(CHARA_CHAI, 0, 1, 0);
        Script_ResetHandMotion(CHARA_CHAI, 0, 0, 0);
        break;
      case 84:
        Script_scenCreateTask(script_03D7, 2, CHARA_CHAI, 5);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_1E18();
  return;
}

static void script_087C(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  int32_t local_DC;
  script_0030();
  script_1C42();
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 2);
  }
  Script_shenSeqUtils(arg1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
        Script_FaceEx(CHARA_KISY, FACE_ExpressionDisabled, 0, 1);
        Script_FaceEx(CHARA_TAIJ, FACE_ExpressionDisabled, 0, 1);
        Script_FaceEx(CHARA_CHAI, FACE_ExpressionDisabled, 0, 1);
        Script_scenCreateTask(script_039D, 3, CHARA_CHAI, 1, 5);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 140.0f);
        if (0) {
          Script_charaDisplay(SHEN_NAME('KAGA'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('KAGA'), CDM_Show);
            goto L000942;
          }
        }
        L000942:;
        Script_scenToggleVisibleFor(SHEN_NAME('KAGA'), 1);
        if (-1) {
          Script_charaDisplay(SHEN_NAME('HDBN'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('HDBN'), CDM_Show);
            goto L000976;
          }
        }
        L000976:;
        Script_scenToggleVisibleFor(SHEN_NAME('HDBN'), 0);
        local_EC.x = 0.159f;
        (&local_EC)->y = 0.009f;
        (&local_EC)->z = 0.021f;
        local_E0.x = 15291;
        (&local_E0)->y = 63897;
        (&local_E0)->z = 65353;
        Script_fixoAttachEx(SHEN_NAME('KAGA'), CHARA_CHAI, 12, &local_EC, &local_E0);
        break;
      case 18:
        Script_scenCreateTask(script_03D7, 2, CHARA_CHAI, 5);
        break;
      case 32:
        Script_scenCreateTask(script_039D, 3, CHARA_CHAI, 1, 5);
        break;
      case 46:
        Script_scenCreateTask(script_03D7, 2, CHARA_CHAI, 5);
        break;
      case 58:
        Script_scenCreateTask(script_039D, 3, CHARA_CHAI, 1, 5);
        break;
      case 70:
        Script_FaceEx(CHARA_CHAI, FACE_ExpressionEnabled, 0, 1);
        Script_FACE(CHARA_CHAI, 1, 2, 1);
        Script_ResetHandMotion(CHARA_CHAI, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_CHAI, 0, data_29B0, 1);
        Script_lightSetSlotEnabled(3, 0);
        break;
      case 76:
        Script_scenCreateTask(script_03D7, 2, CHARA_CHAI, 5);
        break;
      case 117:
        Script_FACE(CHARA_CHAI, 4, 0, 16);
        break;
      case 129:
        Script_FACE(CHARA_CHAI, 2, 0, 12);
        break;
      case 141:
        Script_FACE(CHARA_CHAI, 1, 0, 20);
        break;
      case 160:
        Script_FACE(CHARA_CHAI, 7, 0, 13);
        break;
      case 171:
        Script_FACE(CHARA_CHAI, 10, 0, 16);
        break;
      case 188:
        Script_FACE(CHARA_CHAI, 1, 0, 10);
        break;
      case 189:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        break;
      case 192:
        Script_FACE(CHARA_AKIR, 10, 0, 10);
        break;
      case 202:
        Script_FACE(CHARA_AKIR, 2, 0, 14);
        break;
      case 205:
        Script_FaceEx(CHARA_CHAI, FACE_ExpressionDisabled, 0, 1);
        Script_SetMotnFlag(CHARA_AKIR, -1);
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
        break;
      case 210:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 213:
        Script_FACE(CHARA_AKIR, 2, 0, 20);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_1E18();
  return;
}

static void script_0C86(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_E0[7];
  int32_t local_DC;
  script_0030();
  script_1C42();
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 2);
  }
  Script_shenSeqUtils(arg1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_ResetHandMotion(CHARA_CHAI, 0, 0, 0);
        if (0) {
          Script_charaDisplay(SHEN_NAME('HDBN'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('HDBN'), CDM_Show);
            goto L000D01;
          }
        }
        L000D01:;
        Script_scenToggleVisibleFor(SHEN_NAME('HDBN'), 1);
        Script_fixoClearAttachment(SHEN_NAME('KAGA'));
        Script_SetMotnFlag(CHARA_AKIR, -1);
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
        Script_FACE(CHARA_KISY, 2, 0, 1);
        break;
      case 27:
        Script_ResetHandMotion(CHARA_TAIJ, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_TAIJ, 1, data_29B0, 1);
        Script_SetMotnFlag(CHARA_TAIJ, -1);
        Script_motmToggleFlags(CHARA_TAIJ, SCEN_Disable, 0x40);
        break;
      case 42:
        Script_handSetFloatsFor(CHARA_TAIJ, 1, data_2B78, 10);
        break;
      case 70:
        Script_ResetHandMotion(CHARA_TAIJ, 0, 1, 0);
        break;
      case 72:
        Script_FACE(CHARA_KISY, 7, 1, 8);
        break;
      case 77:
        Script_FACE(CHARA_KISY, 0, 0, 18);
        break;
      case 93:
        Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2A94, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2A94, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2A94, 1);
        break;
      case 94:
        Script_handSetFloatsFor(CHARA_KISY, 1, data_2B78, 11);
        break;
      case 152:
        Script_FACE(CHARA_AKIR, 0, 0, 1);
        break;
      case 159:
        Script_FACE(CHARA_AKIR, 7, 1, 4);
        break;
      case 161:
        Script_FACE(CHARA_AKIR, 0, 0, 5);
        break;
      case 172:
        Script_FACE(CHARA_AKIR, 2, 0, 8);
        break;
      case 160:
        Script_scenCreateTask(script_240F, 0);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_1E18();
  return;
}

static void script_0F62(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_E0[7];
  int32_t local_DC;
  script_0030();
  script_1C42();
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 2);
  }
  Script_shenSeqUtils(arg1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_FACE(CHARA_CHAI, 10, 0, 1);
        break;
      case 13:
        Script_scenCreateTask(script_039D, 3, CHARA_CHAI, 1, 6);
        break;
      case 28:
        Script_ResetHandMotion(CHARA_CHAI, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_CHAI, 1, data_29B0, 1);
        break;
      case 44:
        Script_scenCreateTask(script_03D7, 2, CHARA_CHAI, 6);
        break;
      case 55:
        Script_FACE(CHARA_CHAI, 4, 0, 16);
        break;
      case 68:
        Script_FACE(CHARA_CHAI, 1, 0, 8);
        break;
      case 87:
        Script_FACE(CHARA_CHAI, 2, 0, 14);
        break;
      case 95:
        Script_FACE(CHARA_CHAI, 10, 0, 16);
        break;
      case 102:
        Script_handSetFloatsFor(CHARA_CHAI, 1, data_253C, 16);
        break;
      case 108:
        Script_FACE(CHARA_CHAI, 1, 0, 20);
        break;
      case 140:
        Script_FACE(CHARA_CHAI, 2, 0, 12);
        break;
      case 150:
        Script_FACE(CHARA_CHAI, 8, 0, 12);
        break;
      case 162:
        Script_FACE(CHARA_CHAI, 10, 0, 6);
        break;
      case 165:
        Script_scenCreateTask(script_039D, 3, CHARA_CHAI, 1, 6);
        break;
      case 188:
        Script_scenCreateTask(script_03D7, 2, CHARA_CHAI, 6);
        break;
      case 250:
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_1E18();
  return;
}

static void script_11D4(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_E0[7];
  int32_t local_DC;
  script_0030();
  script_1C42();
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 2);
  }
  Script_shenSeqUtils(arg1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_FACE(CHARA_AKIR, 8, 0, 1);
        Script_FACE(CHARA_CHAI, 1, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_253C, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_253C, 1);
        break;
      case 1:
        Script_FACE(CHARA_AKIR, 10, 0, 20);
        break;
      case 10:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2F08, 6);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2A94, 12);
        break;
      case 25:
        Script_FACE(CHARA_AKIR, 2, 0, 24);
        break;
      case 43:
        Script_FACE(CHARA_AKIR, 0, 0, 6);
        break;
      case 72:
        Script_executeAicaCommand(0x2D04A9, 0, 0);
        break;
      case 88:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_253C, 6);
        break;
      case 108:
        Script_FACE(CHARA_CHAI, 7, 0, 16);
        break;
      case 114:
        Script_FACE(CHARA_CHAI, 4, 0, 11);
        break;
      case 122:
        Script_FACE(CHARA_CHAI, 7, 0, 11);
        break;
      case 165:
        Script_FACE(CHARA_AKIR, 10, 0, 1);
        break;
      case 191:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2A94, 22);
        break;
      case 200:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 230:
        Script_FACE(CHARA_AKIR, 2, 0, 8);
        break;
      case 303:
        if (-1) {
          Script_charaDisplay(CHARA_KISY, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_KISY, CDM_Show);
            goto L001435;
          }
        }
        L001435:;
        Script_scenToggleVisibleFor(CHARA_KISY, 0);
        if (-1) {
          Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_TAIJ, CDM_Show);
            goto L001469;
          }
        }
        L001469:;
        Script_scenToggleVisibleFor(CHARA_TAIJ, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
        break;
      case 310:
        Script_scenCreateTask(script_039D, 3, CHARA_CHAI, 1, 6);
        break;
      case 336:
        Script_scenCreateTask(script_03D7, 2, CHARA_CHAI, 6);
        break;
      case 348:
        Script_FACE(CHARA_CHAI, 10, 0, 8);
        break;
      case 362:
        Script_FACE(CHARA_CHAI, 4, 0, 10);
        break;
      case 374:
        Script_FACE(CHARA_CHAI, 2, 0, 15);
        break;
      case 395:
        Script_FACE(CHARA_CHAI, 4, 0, 10);
        break;
      case 411:
        Script_FACE(CHARA_CHAI, 0, 0, 12);
        break;
      case 422:
        Script_scenCreateTask(script_039D, 3, CHARA_CHAI, 1, 6);
        Script_FACE(CHARA_CHAI, 7, 0, 6);
        break;
      case 437:
        Script_scenCreateTask(script_03D7, 2, CHARA_CHAI, 6);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        Script_FACE(CHARA_AKIR, 2, 0, 8);
        break;
      case 480:
        Script_executeAicaCommand(2720, 2, 60);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_1E18();
  return;
}

static void script_163A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  int32_t local_DC;
  script_0030();
  script_1C42();
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_TAIJ, -1);
  Script_SetMotnFlag(CHARA_KISY, -1);
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 2);
  }
  Script_shenSeqUtils(arg1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_sprintf("\012");
        Script_ResetHandMotion(CHARA_TAIJ, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_TAIJ, 1, data_2FEC, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_29B0, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_29B0, 1);
        local_E0.x = 0;
        (&local_E0)->y = -546;
        (&local_E0)->z = 0;
        Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &local_E0);
        break;
      case 723:
        local_EC.x = 0.167f;
        (&local_EC)->y = 0.009f;
        (&local_EC)->z = -0.022f;
        local_E0.x = 50972;
        (&local_E0)->y = 1092;
        (&local_E0)->z = 63169;
        Script_fixoAttachEx(SHEN_NAME('KAGA'), CHARA_AKIR, 18, &local_EC, &local_E0);
        break;
      case 1185:
        local_E0.x = 0;
        (&local_E0)->y = -1820;
        (&local_E0)->z = 0;
        Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &local_E0);
        break;
      case 1750:
        local_E0.x = 0;
        (&local_E0)->y = 0;
        (&local_E0)->z = 0;
        Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &local_E0);
        break;
      case 2199:
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        break;
      case 2262:
        Script_fixoClearAttachment(SHEN_NAME('KAGA'));
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_1E18();
  return;
}

static void script_1897(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  int32_t local_DC;
  script_0030();
  script_1C42();
  Script_scenCreateTask(script_1B7C, 0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_TAIJ, -1);
  Script_SetMotnFlag(CHARA_KISY, -1);
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 2);
  }
  Script_shenSeqUtils(arg1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_executeAicaCommand(0x2E04A9, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_29B0, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2A94, 1);
        local_EC.x = 0.167f;
        (&local_EC)->y = 0.009f;
        (&local_EC)->z = -0.022f;
        local_E0.x = 50972;
        (&local_E0)->y = 1092;
        (&local_E0)->z = 63169;
        Script_fixoAttachEx(SHEN_NAME('KAGA'), CHARA_AKIR, 18, &local_EC, &local_E0);
        break;
      case 300:
        Script_FaceLookAtTargetUtils(CHARA_KISY, FACE_LookReset);
        break;
      case 562:
        Script_FaceLookAtTargetUtils(CHARA_KISY, FACE_LookReset);
        break;
      case 2745:
        Script_FaceLookAtTargetUtils(CHARA_KISY, FACE_LookReset);
        break;
      case 3630:
        Script_fixoClearAttachment(SHEN_NAME('KAGA'));
        Script_SetMotnFlag(CHARA_TAIJ, 0);
        break;
      case 3650:
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    }
    if ((local_FC > 0) & (local_FC < 300)) {
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_EC, 0);
      Script_FaceLookAtTargetUtils(CHARA_KISY, FACE_LookAtPosition, &local_EC, 3);
    } else if ((local_FC > 480) & (local_FC < 562)) {
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_EC, 0);
      Script_FaceLookAtTargetUtils(CHARA_KISY, FACE_LookAtPosition, &local_EC, 8);
    } else {
      if ((local_FC > 2528) & (local_FC < 2745)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_EC, 0);
        Script_FaceLookAtTargetUtils(CHARA_KISY, FACE_LookAtPosition, &local_EC, 2);
        goto L001AE6;
      }
    }
    L001AE6:;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_1E18();
  return;
}

static int32_t script_1B31(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 2, "tsm001.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static int32_t script_1B7C(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 2, "tsm005.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(4520, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  return Script_LoadSND(4, 0, 0, 0);
}

static void script_1BDD(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_2A94, 1);
  Script_handSetFloatsFor(arg0, 0, data_2A94, 1);
  return;
}

static void script_1C18(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_1C42(void) {
  script_0048(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_30E8);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  script_0048(CHARA_KISY);
  Script_facemotUtils(CHARA_KISY, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
  Script_SetMotnFlag(CHARA_KISY, -1);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_30E8);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  script_0048(CHARA_TAIJ);
  Script_facemotUtils(CHARA_TAIJ, 1, 0);
  Script_ResetHandMotion(CHARA_TAIJ, 0, 1, 0);
  Script_ResetHandMotion(CHARA_TAIJ, 0, 0, 0);
  Script_SetMotnFlag(CHARA_TAIJ, -1);
  Script_SetShadowFlags(CHARA_TAIJ, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_SetCharaRefBoardPos(CHARA_TAIJ, &data_30E8);
  Script_charaSetRefboardFlagTo(CHARA_TAIJ, 1);
  script_0048(CHARA_CHAI);
  Script_facemotUtils(CHARA_CHAI, 1, 0);
  Script_ResetHandMotion(CHARA_CHAI, 0, 1, 0);
  Script_ResetHandMotion(CHARA_CHAI, 0, 0, 0);
  Script_SetMotnFlag(CHARA_CHAI, -1);
  Script_SetShadowFlags(CHARA_CHAI, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_SetCharaRefBoardPos(CHARA_CHAI, &data_30E8);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 1);
  Script_SetCharaRefBoardPos(SHEN_NAME('KAGA'), &data_30E8);
  Script_charaSetRefboardFlagTo(SHEN_NAME('KAGA'), 1);
  return;
}

static void script_1E18(void) {
  script_00D5(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  script_00D5(CHARA_KISY);
  Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
  Script_SetMotnFlag(CHARA_KISY, 0);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 2);
  script_00D5(CHARA_TAIJ);
  Script_ResetHandMotion(CHARA_TAIJ, 0, 1, 0);
  Script_ResetHandMotion(CHARA_TAIJ, 0, 0, 0);
  Script_SetMotnFlag(CHARA_TAIJ, 0);
  Script_SetShadowFlags(CHARA_TAIJ, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_charaSetRefboardFlagTo(CHARA_TAIJ, 2);
  script_00D5(CHARA_CHAI);
  Script_ResetHandMotion(CHARA_CHAI, 0, 1, 0);
  Script_ResetHandMotion(CHARA_CHAI, 0, 0, 0);
  Script_SetMotnFlag(CHARA_CHAI, 0);
  Script_SetShadowFlags(CHARA_CHAI, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 2);
  Script_charaSetRefboardFlagTo(SHEN_NAME('KAGA'), 0);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_1F75(const char *arg0, const char *arg1) {
  Script_sprintf("MS8A: Load PackFile Start %s:%s\012", arg0, arg1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    Script_sprintf("MS8A: Load PackFile Failed %s:%s\012", arg0, arg1);
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

void script_1FC9(void) {
  Script_scenCreateTask(script_1FD7, 0);
  return;
}

__attribute__((scn_string_bindings(1, data_3149, 3, data_3154, 4, data_3159, 5, data_315A, 6, data_315F, 7, data_3160, 8, data_3165, 9, data_3166, 10, data_316B, 11, data_316C, 12, data_3171, 13, data_3172, 14, data_3177, 15, data_3178, 16, data_317D, 17, data_317E, 18, data_3183, 19, data_3184)))
static int32_t script_1FD7(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_F3;
  uint8_t unused_F0[3];
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  script_1F75("scene/02/MS8A", "MS8A");
  Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 76);
  script_23ED();
  (void)((local_F3 = 0) & 0x20);
  L00208E:;
  if (1) {
    Script_SetQteOverlayTimeoutTuning(0, QTE_TimeoutSession);
    Script_uiUtils(0, 21, 0);
    Script_LoadVoiceFile(1, "A0159");
    Script_AuthorUtils(0, 0, "MS8A", "");
    Script_AuthorUtils(0, 1, "MS8A", "");
    Script_AuthorUtils(0, 2, "MS8A", "");
    Script_AuthorUtils(0, 3, "MS8A", "");
    Script_AuthorUtils(0, 4, "MS8A", "");
    Script_AuthorUtils(0, 5, "MS8A", "");
    Script_AuthorUtils(0, 6, "MS8A", "");
    Script_AuthorUtils(0, 7, "MS8A", "");
    Script_shenSeqUtils(-9, 1, 0);
    Script_shenSeqUtils(-9, 2, 0);
    Script_shenSeqUtils(-9, 3, 0);
    Script_shenSeqUtils(-9, 4, 0);
    Script_shenSeqUtils(-9, 5, 0);
    Script_shenSeqUtils(-9, 6, 1);
    Script_shenSeqUtils(-9, 7, 1);
    script_23ED();
    Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
    script_163A(0, 6);
    script_040B(0, 0);
    script_066D(0, 1);
    (void)((task_4 = 0) & 0x20);
    Script_ReturnTimeDate(&local_F4);
    for (;;) {
      Script_SetNewDateTime(&local_F4);
      if (task_4 & 2) {
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
      }
      task_0 = 0;
      script_087C(0, 2);
      script_0C86(0, 3);
      Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, 10, Script_PlayerStatsUtils(PS_Get70MBTimeCapSlot, 10) + 1);
      if (task_0 & 0x40) {
        script_11D4(0, 5);
        script_1897(0, 7);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE, 1);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 0);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A0160_FUKU_EVENT_DONE, 1);
        Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
        Script_PlayerStatsUtils(PS_Set70MBCounter0, Script_PlayerStatsUtils(PS_Get70MBCounter0) + 1);
        Script_executeAicaCommand(0x1200A0, 2, 0);
        Script_ReturnTimeDate(&local_F4);
        if ((((&local_F4)->hours > 7) & ((&local_F4)->hours < 23)) | (((&local_F4)->hours == 23) & ((&local_F4)->minutes < 30))) {
          (&local_F4)->hours = 23;
          (&local_F4)->minutes = 30;
          (&local_F4)->seconds = 0;
          Script_SetNewDateTime(&local_F4);
        }
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 31);
        SCNF_WaitFrames_0000(1);
      } else {
        if (local_F3 == 0) {
          Script_PlayerStatsUtils(PS_Set70MBCounter1, Script_PlayerStatsUtils(PS_Get70MBCounter1) + 1);
          (void)((local_F3 = -1) & 1);
        }
        script_0F62(0, 4);
        Script_uiUtils(0, 35, 0);
        Script_sicnUtils(0, 1, 1);
        SCNF_WaitFrames_0000(90);
        Script_sicnUtils(2, 1, 0);
        Script_SetQteOverlayTimeoutTuning(1, QTE_TimeoutSession);
      }
      (void)((task_4 = -1) & 1);
    }
    Script_AuthorUtils(AUTH_FREE, 0);
    Script_AuthorUtils(AUTH_FREE, 1);
    Script_AuthorUtils(AUTH_FREE, 2);
    Script_AuthorUtils(AUTH_FREE, 3);
    Script_AuthorUtils(AUTH_FREE, 4);
    Script_AuthorUtils(AUTH_FREE, 5);
    Script_AuthorUtils(AUTH_FREE, 6);
    Script_AuthorUtils(AUTH_FREE, 7);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MS8A");
    Script_LoadVoiceFile(0, data_3189);
    Script_executeAicaCommand(0x1200A0, 2, 0);
    goto L00208E;
  }
  end:;
}

static void script_23ED(void) {
  Script_ProcessFogFromTable(0);
  Script_SetupLightFromTable(0);
  SCNF_WaitFrames_0000(1);
  Script_PREP_Related(-1, 1);
  return;
}

static int32_t script_240F(void) {
  return task_0 = SCNF_UIUtils(0x40, 20, 2, 0, 1, 0);
}
