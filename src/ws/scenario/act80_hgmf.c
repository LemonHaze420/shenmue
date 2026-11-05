#include "act80_hgmf.h"

static int32_t script_0030(void) {
  for (;;) {
    Script_ClearAccumulator_00FE(1, 0x3F000000, 10, 2, 2);
    Script_ClearAccumulator_00FE(3, &data_0F04, 8, 9);
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
  Script_set_val_to_1(3, 0);
  return;
}

static void script_0097(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L0000C3;
    }
  }
  L0000C3:;
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

static void script_0131(int32_t arg0) {
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

static void script_0181(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01AF(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01DD(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0211(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0245(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0263(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_029D(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_02D1(void) {
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

static void script_03C9(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_03F9(int32_t arg0) {
  task_4 = arg0;
  task_8 = 0;
  task_0 = 0;
  task_C = 0;
  return;
}

static int32_t script_0418(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  SCNF_WaitFrames_0000(arg0);
  for (;;) {
    if (Script_shenSeqUtils(-1) == 0) {
      break;
    }
    local_FC = Script_GetInputData(0, 0) & 0xF7FF;
    if (local_FC & 0x200) {
      Script_shenSeqUtils(-6);
      task_0 = 1;
      break;
    }
    local_F8 = task_4;
    switch (local_F8) {
      case 0:
        if ((Script_GetInputData(0, 4) - 128) | (Script_GetInputData(0, 5) - 128) | local_FC) {
          task_8 = 0;
          task_4 = 1;
          Script_SetNewCameraState(1);
          script_05CA();
        }
        break;
      case 1:
        script_05FA();
        if ((Script_GetInputData(0, 4) - 128) | (Script_GetInputData(0, 5) - 128) | local_FC) {
          task_8 = 0;
        } else {
          if (task_8 / 30 >= 10) {
            task_4 = 0;
            Script_SetNewCameraState(0);
            script_05F0();
            goto L00050C;
          }
        }
        L00050C:;
        task_8 = task_8 + 1;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int8_t script_052C(void) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  local_FF = 0;
  local_F8 = 0;
  if (task_0 == 0) {
    for (;;) {
      local_F4 = Script_GetInputData(0, 0) & 0xF7FF;
      if (local_F4 & 0x200) {
        break;
      }
      if (local_F8 / 30 < 2) {
        if ((Script_GetInputData(0, 4) - 128) | (Script_GetInputData(0, 5) - 128) | local_F4) {
          local_FF = 1;
          break;
        }
      } else {
        break;
      }
      local_F8 = local_F8 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  return local_FF;
}

static void script_05B9(int32_t arg0, int32_t arg1) {
  task_C = arg0;
  task_10 = arg1;
  return;
}

static void script_05CA(void) {
  Script_SetupLightFromTable(task_C);
  task_44 = 0;
  *(&task_44 + 1) = 0;
  *(&task_44 + 2) = 0;
  return;
}

static void script_05F0(void) {
  Script_SetupLightFromTable(0);
  return;
}

static void script_05FA(void) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (task_C) {
    local_FC = (int32_t)(Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 4) - 128) * 0.46875f / 360.0f * (float)0x10000);
    if ((task_10 == 1) | (task_10 == 2)) {
      local_F8 = (int32_t)(Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 5) - 128) * 0.3125f / 360.0f * (float)0x10000);
    } else {
      local_F8 = (int32_t)(Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 5) - 128) * 0.15625f / 360.0f * (float)0x10000);
    }
    if (local_FC > *&task_44) {
      task_44 = *&task_44 + 182;
    }
    if (local_FC < *&task_44) {
      task_44 = *&task_44 - 182;
    }
    if (local_F8 > *(&task_44 + 1)) {
      *(&task_44 + 1) = *(&task_44 + 1) + 182;
    }
    if (local_F8 < *(&task_44 + 1)) {
      *(&task_44 + 1) = *(&task_44 + 1) - 182;
    }
    Script_GetAuthorState_And_SetCamPos(&task_2C, 0);
    Script_GetAuthorState_And_SetCamPos(0, &task_38);
    task_20.x = ((&task_2C)->x - (&task_38)->x) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *&task_44) - (task_2C.z - task_38.z) * ((float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, *&task_44));
    task_20.z = ((&task_2C)->x - (&task_38)->x) * ((float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, *&task_44)) + (task_2C.z - task_38.z) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *&task_44);
    local_F4 = (&task_2C)->x - (&task_38)->x;
    local_F0 = task_2C.z - task_38.z;
    task_20.y = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, local_F4, local_F0) * ((float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, *(&task_44 + 1)));
    task_20.z = task_20.y * ((float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, -*(&task_44 + 1))) + task_20.z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, -*(&task_44 + 1));
    task_20.x = task_20.y * ((float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, *(&task_44 + 1))) + (&task_20)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(&task_44 + 1));
    task_20.x = (&task_20)->x + (&task_38)->x;
    task_20.y = task_20.y + task_38.y;
    task_20.z = task_20.z + task_38.z;
    Script_LightUtils(1, 7, &task_20);
  }
  return;
}

static void script_0898(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return;
}

static void script_08F4(void) {
  script_0898(CHARA_MARK);
  task_50 = 0;
  return;
}

static int32_t script_090A(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_50;
    switch (local_FC) {
      case 1:
        Script_FACE(CHARA_MARK, 9, 0, 8);
        break;
      case 91:
        Script_FACE(CHARA_MARK, 1, 0, 8);
        break;
      case 151:
        Script_FACE(CHARA_MARK, 9, 0, 8);
        break;
      case 222:
        Script_FACE(CHARA_MARK, 0, 0, 8);
        break;
      case 318:
        Script_FACE(CHARA_MARK, 9, 0, 8);
        break;
      case 443:
        Script_FACE(CHARA_MARK, 0, 0, 8);
        break;
      case 595:
        Script_FACE(CHARA_MARK, 10, 0, 8);
        break;
      case 701:
        Script_FACE(CHARA_MARK, 7, 0, 8);
        break;
      case 741:
        Script_FACE(CHARA_MARK, 10, 0, 8);
        break;
      case 801:
        Script_FACE(CHARA_MARK, 7, 0, 8);
        break;
      case 831:
        Script_FACE(CHARA_MARK, 2, 0, 8);
        break;
      case 931:
        Script_FACE(CHARA_MARK, 9, 0, 8);
        break;
      case 0x400:
        Script_FACE(CHARA_MARK, 4, 0, 8);
        break;
      case 1114:
        Script_FACE(CHARA_MARK, 9, 0, 8);
        break;
      case 1161:
        Script_FACE(CHARA_MARK, 0, 0, 8);
        break;
      case 1190:
        Script_FACE(CHARA_MARK, 12, 0, 8);
        break;
      case 1397:
        Script_FACE(CHARA_MARK, 9, 0, 8);
        break;
      case 1541:
        Script_FACE(CHARA_MARK, 0, 0, 8);
        break;
      case 1736:
        Script_FACE(CHARA_MARK, 4, 0, 8);
        break;
      case 1797:
        Script_FACE(CHARA_MARK, 1, 0, 8);
        break;
      case 1986:
        Script_FACE(CHARA_MARK, 9, 0, 8);
        break;
      case 2021:
        Script_FACE(CHARA_MARK, 1, 0, 8);
        break;
      case 2151:
        Script_FACE(CHARA_MARK, 7, 0, 8);
        break;
      case 2206:
        Script_FACE(CHARA_MARK, 1, 0, 8);
        break;
      case 2326:
        Script_FACE(CHARA_MARK, 10, 0, 8);
        break;
      case 2401:
        Script_FACE(CHARA_MARK, 0, 0, 8);
        break;
      case 2621:
        Script_FACE(CHARA_MARK, 9, 0, 8);
        break;
      case 2731:
        Script_FACE(CHARA_MARK, 7, 0, 8);
        break;
      case 2771:
        Script_FACE(CHARA_MARK, 4, 0, 8);
        break;
      case 2806:
        Script_FACE(CHARA_MARK, 9, 0, 8);
        break;
    }
    if ((task_50 >= 516) & (task_50 <= 690)) {
      Script_GetAuthorState_And_SetCamPos(&task_54, 0);
      task_54.y = task_54.y + 0.5f;
      Script_FaceLookAtTargetUtils(CHARA_MARK, FACE_LookAtPosition, &task_54, 8);
    } else if (task_50 == 691) {
      Script_FaceLookAtTargetUtils(CHARA_MARK, FACE_LookReset);
    } else if ((task_50 >= 2151) & (task_50 <= 2200)) {
      Script_GetAuthorState_And_SetCamPos(&task_54, 0);
      Script_FaceLookAtTargetUtils(CHARA_MARK, FACE_LookAtPosition, &task_54, 8);
    } else if (task_50 == 2201) {
      Script_FaceLookAtTargetUtils(CHARA_MARK, FACE_LookReset);
    } else {
      if ((task_50 >= 2891) & (task_50 <= 3030)) {
        Script_GetAuthorState_And_SetCamPos(&task_54, 0);
        task_54.x = (&task_54)->x - 0.7f;
        task_54.y = task_54.y + 0.5f;
        Script_FaceLookAtTargetUtils(CHARA_MARK, FACE_LookAtPosition, &task_54, 8);
        goto L000D77;
      }
    }
    L000D77:;
    task_50 = task_50 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

void script_0D8E(void) {
  Script_LoadVoiceFile(1, "FACE3");
  script_0DB3();
  script_0DD5(CHARA_MARK);
  script_0DE4();
  return;
}

static void script_0DB3(void) {
  Script_uiUtils(0, 48, 0);
  script_03F9(0);
  script_05B9(1, 2);
  return;
}

static void script_0DD5(int32_t arg0) {
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_0DE4(void) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    script_08F4();
    local_F8 = Script_scenCreateTask(script_090A, 0);
    Script_shenSeqUtils(0);
    if (local_FE == 0) {
      Script_scenCreateTask(script_0418, 1, 60);
      Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenClearFreeState(1);
      local_FE = 1;
    } else {
      Script_scenCreateTask(script_0418, 1, 20);
    }
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(1);
    local_FF = script_052C();
    if (local_FF == 0) {
      break;
    }
    Script_srchControl((void *)local_F8);
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  MODU_Script_shenRequestGameModule(Display, 0, 0, 0);
  return;
}
