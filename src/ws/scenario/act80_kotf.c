#include "act80_kotf.h"

void script_0030(void) {
  script_0049();
  script_006E(CHARA_OISI);
  script_00C7();
  return;
}

static void script_0049(void) {
  Script_SetCurrentScrollEnabled(0);
  Script_SetFogFlags(0, data_0EFC);
  Script_SetFogFlags(4, 1);
  script_08AB(1);
  return;
}

static void script_006E(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 0);
  Script_SetClipperFlag(arg0);
  Script_SetCPAMFlags(arg0, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_LoadVoiceFile(1, "NET");
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
  Script_PREP_Related(-1, 1);
  return;
}

static void script_00C7(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  task_2 = 0;
  while (1) {
    local_F4 = task_2;
    switch (local_F4) {
      case 0:
        task_2 = 1;
        task_1 = 1;
        local_FC = 0;
        task_0 = 0;
        Script_seqEnterEvtCam(4000, 0, AUTH_None);
        Script_scenCreateTask(script_030B, 3, CHARA_OISI, &data_0A80, &data_0AF8);
        Script_scenCreateTask(script_0520, 2, CHARA_OISI, data_0DD8);
        Script_scenCreateTask(script_0748, 0);
        Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 1:
        break;
      case 2:
        task_2 = 3;
        task_1 = 1;
        local_FC = 0;
        task_0 = 0;
        local_F8 = 0;
        Script_SetNewCameraState(1);
        Script_scenCreateTask(script_030B, 3, CHARA_OISI, &data_0A80, &data_0AF8);
        Script_scenCreateTask(script_0520, 2, CHARA_OISI, data_0DD8);
        Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 3:
        while (1) {
          if ((Script_GetInputData(0, 4) - 128) | (Script_GetInputData(0, 5) - 128) | Script_GetInputData(0, 0)) {
            local_F8 = 0;
          } else {
            if (local_F8 / 30 >= 30) {
              task_2 = 0;
              task_1 = 0;
              while (local_FC > -199) {
                Script_SetSequenceAudioVolume(0, 0, local_FC = local_FC - 10);
                SCNF_WaitFrames_0000(1);
              }
              Script_StartOrFreeSEQ(0);
              Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
              while (Script_filmCheckOrSetTransitionUtil(-1)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_SetNewCameraState(0);
              script_09D2(1);
              SCNF_WaitFrames_0000(40);
              break;
              goto L0002DF;
            }
          }
          L0002DF:;
          script_08EE(1);
          local_F8 = local_F8 + 1;
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_030B(int32_t arg0, void *arg1, void *arg2) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  local_F8 = 0;
  while (task_1) {
    Script_ShowUIMessageFromID((const char *)Script_ScenDataUtil(arg1, SCND_ReadScriptOffset, local_FC));
    while (task_1) {
      if (Script_convGetIsActive(0) == 0) {
        if (Script_ScenDataUtil(arg2, SCND_ReadU32, local_F8) == -1) {
          local_F8 = local_F8 + 4;
          break;
        }
      }
      if (Script_ScenDataUtil(arg2, SCND_ReadU32, local_F8) != -1) {
        Script_FACE(arg0, Script_ScenDataUtil(arg2, SCND_ReadU32, local_F8), Script_ScenDataUtil(arg2, SCND_ReadU32, local_F8 + 1), Script_ScenDataUtil(arg2, SCND_ReadU32, local_F8 + 2));
        task_4 = 0;
        while (task_4 < Script_ScenDataUtil(arg2, SCND_ReadU32, local_F8 + 3)) {
          if (task_1 == 0) {
            break;
          }
          task_4 = task_4 + 1;
          SCNF_WaitFrames_0000(1);
        }
        local_F8 = local_F8 + 4;
      }
      SCNF_WaitFrames_0000(1);
    }
    task_4 = 0;
    while (task_4 < Script_ScenDataUtil(arg1, SCND_ReadU32, local_FC + 1)) {
      if (task_1 == 0) {
        break;
      }
      task_4 = task_4 + 1;
      SCNF_WaitFrames_0000(1);
    }
    local_FC = local_FC + 2;
    if (Script_ScenDataUtil(arg1, SCND_ReadU32, local_FC + 1) == -1) {
      Script_FACE(arg0, 0, -1, 100);
      task_4 = 0;
      while (task_4 < 100) {
        if (task_1 == 0) {
          break;
        }
        task_4 = task_4 + 1;
        SCNF_WaitFrames_0000(1);
      }
      switch (task_2) {
        case 3:
          local_FC = 0;
          local_F8 = 0;
          task_0 = 0;
          break;
        case 1:
          Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_shenRequestGameModuleTransition(80, SHEN_NAME('HGRF'), 0);
          break;
      }
    }
    task_0 = local_FC / 2;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_0520(int32_t arg0, void *arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3i local_CC;
  local_F4 = 0;
  local_F0 = 0;
  Script_EVT_GetPosition(arg0, -1, &local_E4, 0);
  Script_mtwkGetSetRotation(arg0, &local_CC, 0);
  while (task_1) {
    if (Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4) == task_0) {
      if (Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4 + 1) == local_F0) {
        local_FC = (&local_CC)->x - 0x8000 + (int32_t)((float)Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4 + 2) / 360.0f * (float)0x10000);
        local_F8 = (&local_CC)->y - 0x8000 + (int32_t)((float)Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4 + 3) / 360.0f * (float)0x10000);
        local_D8.x = (&local_E4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_F8) * 5.0f;
        (&local_D8)->y = (&local_E4)->y + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FC) * 5.0f + 1.5f;
        (&local_D8)->z = (&local_E4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_F8) * 5.0f;
        Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_D8, Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4 + 4));
        if (Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4 + 5) == 2) {
          Script_FACE_LCG(arg0, FACE_BlinkClose, Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4 + 4), 0);
        } else if (Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4 + 5) == 3) {
          Script_FACE_LCG(arg0, FACE_BlinkOpen, Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4 + 4), 0);
        } else {
          if (Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4 + 5) == 4) {
            Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 0, 0);
            goto L0006CD;
          }
        }
        L0006CD:;
        if (Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4 + 5) == 1) {
          (&local_D8)->y = (&local_D8)->y - 1.5f;
          Script_SetCharaLookAtPosition(arg0, 3, &local_D8, 0);
        }
        local_F4 = local_F4 + 6;
        if (Script_ScenDataUtil(arg1, SCND_ReadU32, local_F4) == -1) {
          local_F4 = 0;
        }
      }
      local_F0 = local_F0 + 1;
    } else {
      local_F0 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_0748(void) {
  int32_t local_FC;
  local_FC = task_0;
  while (task_1) {
    if (local_FC != task_0) {
      Script_seqEnterEvtCam(*(&data_0F10 + task_0), 0, AUTH_None);
      local_FC = task_0;
    }
    if (task_0 == 13) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0797(int32_t arg0) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0);
  while (1) {
    local_F8 = (&local_D0)->x - 0x8000 + (int32_t)((float)(Script_GetInputData(1, 5) - 128) / 360.0f * (float)0x10000);
    local_F4 = (&local_D0)->y - 0x8000 + (int32_t)((float)(Script_GetInputData(1, 4) - 128) / 360.0f * (float)0x10000);
    local_DC.x = (&local_E8)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_F4) * 5.0f;
    (&local_DC)->y = (&local_E8)->y + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_F8) * 5.0f + 1.5f;
    (&local_DC)->z = (&local_E8)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_F4) * 5.0f;
    Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_DC, 8);
    if (Script_GetInputData(1, 0) & 0x40) {
      (&local_DC)->y = (&local_DC)->y - 1.5f;
      Script_SetCharaLookAtPosition(arg0, 3, &local_DC, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_08AB(int32_t arg0) {
  Script_GetLightPosition(arg0, &task_8);
  task_14.x = (&task_8)->x;
  task_14.y = task_8.y;
  task_14.z = task_8.z;
  *(&task_20 + 1) = task_8.y;
  return;
}

static void script_08EE(int32_t arg0) {
  task_20 = (&task_8)->x + Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 4) - 128) / 300.0f;
  *(&task_20 + 2) = task_8.z + Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, Script_GetInputData(0, 5) - 128) / 300.0f;
  if (!(*&task_20 <= (&task_14)->x)) {
    task_14.x = (&task_14)->x + 0.01f;
  }
  if (!(*&task_20 >= (&task_14)->x)) {
    task_14.x = (&task_14)->x - 0.01f;
  }
  if (!(*(&task_20 + 2) <= task_14.z)) {
    task_14.z = task_14.z + 0.01f;
  }
  if (!(*(&task_20 + 2) >= task_14.z)) {
    task_14.z = task_14.z - 0.01f;
  }
  Script_LightUtils(arg0, 7, &task_14);
  return;
}

static void script_09D2(int32_t arg0) {
  Script_LightUtils(arg0, 7, &task_8);
  return;
}
