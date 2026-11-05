#include "act80_hgsf.h"

static int32_t script_0030(void) {
  for (;;) {
    Script_ClearAccumulator_00FE(1, 0x3F000000, 10, 2, 2);
    Script_ClearAccumulator_00FE(3, &data_1270, 8, 9);
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

static int32_t script_0181(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
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
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  script_0086();
  script_1067();
  local_FC = 0;
  local_F0.x = 2.67f;
  (&local_F0)->y = 1.59f;
  (&local_F0)->z = 0.0f;
  Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 0);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_FC;
    switch (local_EC) {
      case 0:
        Script_FACE(SHEN_NAME('SYUE'), 0, 0, 32);
        break;
      case 462:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 464:
        local_F0.x = 1.38f;
        (&local_F0)->y = 1.58f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 470:
        Script_FACE(SHEN_NAME('SYUE'), 2, 0, 32);
        break;
      case 570:
        Script_FACE(SHEN_NAME('SYUE'), 1, 0, 32);
        break;
      case 670:
        Script_FACE(SHEN_NAME('SYUE'), 0, 0, 32);
        break;
      case 685:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 687:
        local_F0.x = 2.67f;
        (&local_F0)->y = 1.58f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 807:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 809:
        local_F0.x = 2.8f;
        (&local_F0)->y = 1.56f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 954:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 956:
        local_F0.x = 2.62f;
        (&local_F0)->y = 1.58f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 990:
        Script_FACE(SHEN_NAME('SYUE'), 2, 0, 32);
        break;
      case 1090:
        Script_FACE(SHEN_NAME('SYUE'), 0, 0, 32);
        break;
      case 1200:
        Script_FACE(SHEN_NAME('SYUE'), 5, 0, 32);
        break;
      case 1344:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 1346:
        local_F0.x = 1.32f;
        (&local_F0)->y = 1.5f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 1400:
        Script_FACE(SHEN_NAME('SYUE'), 2, 0, 32);
        break;
      case 1450:
        Script_FACE(SHEN_NAME('SYUE'), 0, 0, 32);
        break;
      case 1458:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 1460:
        local_F0.x = 2.04f;
        (&local_F0)->y = 1.58f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 1860:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 1862:
        local_F0.x = 2.15f;
        (&local_F0)->y = 1.61f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 1998:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 2000:
        local_F0.x = 2.6f;
        (&local_F0)->y = 1.58f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 2647:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 2798:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 2800:
        local_F0.x = 2.15f;
        (&local_F0)->y = 1.61f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 3447:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 3449:
        local_F0.x = 2.15f;
        (&local_F0)->y = 1.41f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 3450:
        Script_FACE(SHEN_NAME('SYUE'), 10, 0, 32);
        break;
      case 3597:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 3599:
        local_F0.x = 2.45f;
        (&local_F0)->y = 1.56f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 3600:
        Script_FACE(SHEN_NAME('SYUE'), 0, 0, 32);
        break;
      case 3912:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 3998:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 4000:
        local_F0.x = 2.95f;
        (&local_F0)->y = 1.56f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 4521:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 4523:
        local_F0.x = 2.95f;
        (&local_F0)->y = 1.56f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 4997:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 5162:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 5164:
        local_F0.x = 2.34f;
        (&local_F0)->y = 1.53f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 6097:
        Script_scenCreateTask(script_10B8, 0);
        break;
      case 6099:
        local_F0.x = 2.03f;
        (&local_F0)->y = 1.55f;
        (&local_F0)->z = -0.42f;
        Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
        break;
      case 6150:
        Script_FACE(SHEN_NAME('SYUE'), 10, 0, 32);
        break;
      case 6200:
        Script_FACE(SHEN_NAME('SYUE'), 0, 0, 32);
        break;
      case 6320:
        Script_FACE(SHEN_NAME('SYUE'), 1, 0, 32);
        break;
    }
    if ((local_FC >= 2649) & (local_FC <= 2799)) {
      Script_GetAuthorState_And_SetCamPos(&local_F0, 0);
      Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
    }
    if ((local_FC >= 3914) & (local_FC <= 3999)) {
      Script_GetAuthorState_And_SetCamPos(&local_F0, 0);
      Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
    }
    if ((local_FC >= 4999) & (local_FC <= 5163)) {
      Script_GetAuthorState_And_SetCamPos(&local_F0, 0);
      Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookAtPosition, &local_F0, 1);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1067(void) {
  script_0097(SHEN_NAME('SYUE'));
  Script_SetShadowFlags(SHEN_NAME('SYUE'), SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_scenToggleActiveFor(SHEN_NAME('SYUE'), 0);
  Script_clmdSetFlags(SHEN_NAME('SYUE'), 2, 1);
  return;
}

static void script_109D(void) {
  Script_FaceLookAtTargetUtils(SHEN_NAME('SYUE'), FACE_LookReset);
  script_0131(SHEN_NAME('SYUE'));
  return;
}

static int32_t script_10B8(void) {
  Script_FACE_LCG(SHEN_NAME('SYUE'), FACE_BlinkClose, 3, 0);
  SCNF_WaitFrames_0000(3);
  Script_FACE_LCG(SHEN_NAME('SYUE'), FACE_BlinkOpen, 3, 0);
  SCNF_WaitFrames_0000(3);
  return Script_srchControl(0);
}

void script_10F1(void) {
  Script_motiLoad(80, SHEN_NAME('HGSF'), "M_FACE4.BIN");
  Script_LoadVoiceFile(1, "FACE4");
  Script_AuthorUtils(0, 0, "SEQDATA1.BIN", "SCENE/80/HGSF");
  script_113A();
  script_118B(SHEN_NAME('SYUE'));
  script_119A();
  return;
}

static void script_113A(void) {
  Script_SetupLightFromTable(0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 10.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
  Script_PREP_Related(-1, 1);
  Script_SetFogEnabled(0);
  Script_SetCurrentScrollEnabled(0);
  Script_uiUtils(0, 47, 0);
  script_03F9(0);
  script_05B9(1, 3);
  return;
}

static void script_118B(int32_t arg0) {
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_119A(void) {
  int8_t local_FF;
  Script_scenCreateTask(script_0181, 0);
  Script_scenCreateTask(script_0418, 1, 60);
  Script_scenCreateTask(script_123B, 0);
  for (;;) {
    script_0898(0);
    SCNF_WaitFrames_0000(1);
    local_FF = script_052C();
    if (local_FF == 0) {
      break;
    }
    script_109D();
    Script_scenCreateTask(script_0418, 1, 20);
  }
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  MODU_Script_shenRequestGameModule(Display, 0, 0, 0);
  return;
}

static int32_t script_123B(void) {
  SCNF_WaitFrames_0000(60);
  Script_shenClearFreeState(1);
  return Script_srchControl(0);
}
