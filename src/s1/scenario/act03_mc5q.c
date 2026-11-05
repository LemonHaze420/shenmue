#include "act03_mc5q.h"

static int32_t script_0030(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_0054(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  task_1C[arg1] = 0;
  task_24[arg1] = 0;
  task_2C[arg1] = 0;
  task_34[arg1] = arg2;
  task_3C[arg1] = arg3;
  task_44 = 0;
  Script_CharHandlers(arg0, 0, task_34[arg1], 16);
  Script_CharHandlers(arg0, 1, task_3C[arg1], 16);
  for (;;) {
    task_1C[arg1] = 0;
    while (task_1C[arg1] == 0) {
      SCNF_WaitFrames_0000(1);
    }
    task_44 = task_2C[arg1] | 0x40000000;
    Script_scenCreateTask(arg4, 3, arg0, arg1, task_1C[arg1] + task_24[arg1]);
  }
  end:;
}

static int32_t script_012B(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  local_FC = task_44;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_motmGetCharaFlags(arg4, MOTM_PoseNone, SCEN_Write);
  Script_motmToggleFlags(arg4, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (local_FC & -0x80000000) {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x80);
  }
  if (local_FC & 0x40000000) {
    Script_charaToggleMotEnabled(arg4, 1);
  }
  if (local_FC & 0x20000000) {
    Script_charaToggleMotEnabled(arg4, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_01C2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 0, task_34[arg1], 16);
  Script_CharHandlers(arg0, 1, task_3C[arg1], 16);
  return Script_srchControl(0);
}

static int32_t script_0224(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = task_44;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, arg1);
  }
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, 0);
  }
  return Script_srchControl(0);
}

static void script_0280(int32_t arg0, float arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = task_44;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & -0x80000000) {
    arg1 = (float)0 - arg1;
  }
  (&local_F0)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F0, 0);
  Script_srchControl(0);
  return;
}

static void script_02DD(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_02F9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5) {
  int32_t local_FC;
  task_60[arg1] = Script_scenCreateTask(script_0054, 5, arg0, arg1, arg2, arg3, arg5);
  task_48[arg1] = arg0;
  task_50[arg1] = 0;
  task_58[arg1] = 0;
  task_68 = 4;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_50[arg1]) {
      local_FC = task_58[arg1] * 12;
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 5);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 8);
        if (local_F8 > 0) {
          task_1C[arg1] = local_F8;
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 6);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      if (task_24[arg1] == 0) {
        Script_scenCreateTask(script_0669, 4, Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 1), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 2), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 3));
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 7);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 9);
        if (local_F8 > 0) {
          task_1C[arg1] = local_F8;
        }
      }
      script_0567();
      {
        int32_t local_F8;
        if (task_74 == Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC)) {
          task_70 = 1;
          local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 10);
        } else {
          if (task_24[arg1] == 0) {
            task_7C = task_7C + Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 4);
          }
          task_70 = 0;
          local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 11);
        }
        if (local_F8 > 0) {
          task_1C[arg1] = local_F8;
        }
      }
      script_0557(arg1);
    }
  }
  end:;
}

static void script_0520(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  task_50[arg0] = 1;
  task_58[arg0] = arg1;
  task_24[arg0] = arg2;
  task_2C[arg0] = arg3;
  return;
}

static void script_0557(int32_t arg0) {
  task_50[arg0] = 0;
  return;
}

static void script_0567(void) {
  SCNF_WaitFrames_0000(2);
  while (task_74 == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0587(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_0520(arg0, arg1, 0, arg3);
  script_0520(arg2, arg1, 1, arg3);
  while (task_50[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_05C1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_0520(arg0, arg1, 0, arg3);
  script_0520(arg2, arg1, 1, arg3);
  script_0520(arg2 + 1, arg1, 2, arg3);
  while (task_50[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_060D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_0520(arg0, arg1, 0, arg4);
  script_0520(arg2, arg1, 1, arg4);
  script_0520(arg3, arg1, 2, arg4);
  while (task_50[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0656(int32_t arg0) {
  task_68 = arg0;
  Script_SetQteOverlayTimeoutTuning(arg0, QTE_SetTimeoutStep);
  return;
}

static int32_t script_0669(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = arg1 + arg3;
  local_F8 = arg2 + arg3;
  Script_scenCreateTask(script_06CC, 2, arg1, arg3);
  Script_SetQteOverlayTimeoutTuning(local_FC, QTE_SetTimeoutMaximum);
  if (arg0 == 68) {
    SCNF_UIUtils(0x44, local_F8, 5, 0, 1, 0);
  } else {
    SCNF_UIUtils(arg0, local_F8, 2, 0, 1, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_06CC(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  task_78 = arg0;
  task_74 = 0;
  task_6C = 0;
  local_F8 = 0xFFFF;
  for (;;) {
    local_FC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_6C = task_6C + 1;
    if (local_FC != 0xFFFF) {
      if (local_FC != 0xA000) {
        local_F8 = local_FC;
      }
    }
    if (arg0 == -1) {
      if (local_F8 != 0xFFFF) {
        break;
      }
    } else {
      if (task_6C >= arg0 + arg1 + 4) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((local_F8 & 0xFFF) == 0) {
    local_F8 = 0xFFFF;
  }
  task_74 = local_F8 & 0xFFF;
  return Script_srchControl(0);
}

static int32_t script_078D(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_80[arg1] = arg0;
    task_88[arg1][0] = (&local_F4)->x;
    task_88[arg1][1] = (&local_F4)->y;
    task_88[arg1][2] = (&local_F4)->z;
  }
  task_A0[arg1] = 0.0f;
  for (;;) {
    Vector3 local_F4;
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_88[arg1][0] = (&local_F4)->x;
    task_88[arg1][1] = (&local_F4)->y;
    task_88[arg1][2] = (&local_F4)->z;
    task_A0[arg1] = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, task_88[arg1][0] - task_88[arg2][0], task_88[arg1][2] - task_88[arg2][2]);
  }
  end:;
}

static void script_087B(void) {
  task_A8 = 0.0f;
  task_AC = 0.0f;
  task_B0 = 0.0f;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetInputData(2, 0) & 4) {
      if (Script_GetInputData(0, 2) & 0x4000) {
        task_A8 = task_A8 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x8000) {
        task_A8 = task_A8 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x1000) {
        task_AC = task_AC - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x2000) {
        task_AC = task_AC + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x40) {
        task_B0 = task_B0 - 1.0f;
      }
      if (Script_GetInputData(0, 2) & 0x80) {
        task_B0 = task_B0 + 1.0f;
      }
    }
  }
  return;
}

static void script_0969(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_0992(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4) {
  float local_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  Vector3 local_D8;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4[0] = arg2;
  local_F4[1] = 0.0f;
  local_F4[2] = arg3;
  local_D8.x = arg2;
  (&local_D8)->z = arg3;
  Script_EVT_SetPosition(arg0, 0x28000000, &local_D8);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg4 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_0A21(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  int32_t local_FC;
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_FC);
  Script_InitializeOsag(arg0);
  return;
}

static void script_0A54(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_0A6E, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_0A6E(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  script_0992(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_0A99(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4.x = arg2;
  (&local_F4)->y = arg3;
  (&local_F4)->z = arg4;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg5 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_0B19(int32_t arg0, int32_t arg1, float arg2) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  return;
}

static void script_0B6D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0B83, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0B83(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  script_0B19(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_0BAA(int32_t arg0, float arg1) {
  Vector3i local_F4;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F4, 0);
  return;
}

static void script_0BD3(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return;
}

static void script_0C27(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_0C3B, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_0C3B(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_0BAA(arg0, arg1);
  return Script_srchControl(0);
}

static void script_0C60(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_0C78, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_0C78(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  script_0BD3(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_0CA1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_0CB9, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_0CB9(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  float local_FC;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_FC = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    script_0BD3(arg0, 0.0f, 0.0f, local_FC);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_0D0E(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    Script_EVT_GetPosition(task_48[local_E4], -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_48[local_E4], &local_E8, 0);
    task_B4[local_E4][0] = (&local_F4)->x;
    task_B4[local_E4][1] = (&local_F4)->y;
    task_B4[local_E4][2] = (&local_F4)->z;
    task_CC[local_E4][0] = (&local_E8)->x;
    task_CC[local_E4][1] = (&local_E8)->y;
    task_CC[local_E4][2] = (&local_E8)->z;
    task_E4[local_E4] = Script_GetCharaMotionRemainingFramesScaled(task_48[local_E4]);
    task_EC[local_E4] = Script_scenCharaGetMotAnimId(task_48[local_E4]);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_0DF3(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    local_F4.x = task_B4[local_E4][0];
    (&local_F4)->y = task_B4[local_E4][1];
    (&local_F4)->z = task_B4[local_E4][2];
    local_E8.x = task_CC[local_E4][0];
    (&local_E8)->y = task_CC[local_E4][1];
    (&local_E8)->z = task_CC[local_E4][2];
    Script_EVT_SetPosition(task_48[local_E4], POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_48[local_E4], 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(task_48[local_E4], task_EC[local_E4], task_E4[local_E4], -1.0f, -1.0f, 1.0f, 0);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_0EDC(int32_t arg0) {
  task_F8 = arg0;
  task_F4 = 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_F8, task_F4);
  return;
}

static void script_0EFA(void) {
  int32_t local_FC;
  if (task_F4 == 1) {
    local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
    Script_PlayerStatsUtils(PS_Set70MBCounter1, local_FC + 1);
  }
  if (task_F4 < 255) {
    task_F4 = task_F4 + 1;
  }
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_F8, task_F4);
  return;
}

static void script_0F42(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter0);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, local_FC + 1);
  return;
}

static void script_0F5D(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0F77(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    script_0F5D();
  }
  return;
}

static void script_0FB5(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_0FD6(int32_t arg0) {
  L000FD8:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, arg0)) goto L000FD8;
  return;
}

static void script_0FED(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  } else {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    while (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      while (local_FC == 0) {
        SCNF_WaitFrames_0000(1);
        local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      }
      script_0FD6(local_FC);
    }
  }
  (void)local_FC;
  return;
}

static void script_1073(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static void script_1096(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  Script_shenSeqUtils(1);
  task_FC = 0;
  local_F4 = 0;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E4 = local_FC;
    switch (local_E4) {
      case 1:
        Script_SetMaxInterpSpeed(0.0f);
        Script_SetMotnFlag(CHARA_AKIR, -1);
        Script_SetMotnFlag(CHARA_CHAI, -1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2DA0, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2CBC, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2DA0, 1);
        Script_handSetFloatsFor(CHARA_CHAI, 0, data_2E84, 1);
        Script_handSetFloatsFor(CHARA_CHAI, 1, data_2E84, 1);
        Script_FACE(CHARA_CHAI, 0, 0, 10);
        Script_FACE_LCG(CHARA_CHAI, FACE_BlinkClose, 0, 0);
        break;
      case 131:
        task_FC = 1;
        break;
    }
    if (local_FC < 131) {
      if (local_F4 == 0) {
        if (local_F8 == 0) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_2F68, 2);
          local_F8 = 1;
          local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 5) + 10;
        } else if (local_F8 == 1) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_2E84, 2);
          local_F8 = 2;
          local_F4 = 0;
        } else if (local_F8 == 2) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_2F68, 2);
          local_F8 = 3;
          local_F4 = 3;
        } else {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_2E84, 2);
          local_F8 = 0;
          local_F4 = 1;
        }
      } else {
        local_F4 = local_F4 - 1;
      }
    }
    if (local_FC == 56) {
      Script_FACE_LCG(CHARA_CHAI, FACE_BlinkOpen, 2, 0);
    }
    if (local_FC == 59) {
      Script_FACE(CHARA_CHAI, 5, 0, 10);
    }
    if (local_FC == 83) {
      Script_FACE(CHARA_AKIR, 5, 0, 7);
    }
    if (local_FC == 121) {
      Script_handSetFloatsFor(CHARA_CHAI, 0, data_2CBC, 20);
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_2CBC, 20);
    }
    switch (local_FC) {
      case 74:
        Script_SetFightProgramStateBytes(CHARA_CHAI, 2, 1);
        Script_GetDataFromFightProgram(CHARA_CHAI, 1);
        break;
      case 141:
        Script_GetDataFromFightProgram(CHARA_CHAI, 0);
        break;
    }
    if (local_FC == 75) {
      local_E8.x = 70.702499f;
      (&local_E8)->y = 0.8512f;
      (&local_E8)->z = 24.5648f;
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_E8, 5);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMaxInterpSpeed(0.5f);
  return;
}

static int32_t script_134E(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  SCNF_WaitFrames_0000(1);
  local_FC = arg2;
  switch (local_FC) {
    case 1:
      Script_scenCreateTask(script_012B, 6, 23301, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_01C2, 5, arg0, arg1, 0, -1, 30);
      break;
    case 2:
      Script_scenCreateTask(script_012B, 6, 23311, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 15);
      Script_scenCreateTask(script_0030, 2, 0x2976AB, 30);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 18, 10);
      Script_scenCreateTask(script_01C2, 5, arg0, arg1, 0, -1, 30);
      break;
    case 3:
      Script_scenCreateTask(script_012B, 6, 23297, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_012B, 6, 23317, 60, 0x42D20000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 161451, 23);
      Script_scenCreateTask(script_0030, 2, 0x2876AB, 38);
      Script_scenCreateTask(script_0030, 2, 161451, 115);
      break;
    case 4:
      Script_scenCreateTask(script_012B, 6, 23306, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_012B, 6, 23320, 60, 0x42D20000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 15);
      Script_scenCreateTask(script_0030, 2, 0x2976AB, 30);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 95);
      Script_scenCreateTask(script_0030, 2, 0x2976AB, 110);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 148);
      Script_scenCreateTask(script_0030, 2, 0x2976AB, 165);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 18, 10);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 95, 15);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 148, 15);
      Script_scenCreateTask(script_01C2, 5, arg0, arg1, 0, -1, 30);
      break;
    case 5:
      Script_scenCreateTask(script_012B, 6, 23302, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 816811, 26);
      Script_scenCreateTask(script_0224, 4, arg0, 6, 24, 8);
      break;
    case 6:
      Script_scenCreateTask(script_012B, 6, 23312, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 0x1976AB, 20);
      Script_scenCreateTask(script_0030, 2, 161451, 30);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 62);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 63, 10);
      break;
    case 7:
      Script_scenCreateTask(script_012B, 6, 23298, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_012B, 6, 23300, 141, 0x42CC0000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 161451, 25);
      Script_scenCreateTask(script_0030, 2, 0x2876AB, 60);
      Script_scenCreateTask(script_0030, 2, 30379, 169);
      Script_scenCreateTask(script_0030, 2, 161451, 181);
      Script_scenCreateTask(script_0030, 2, 30379, 195);
      break;
    case 8:
      Script_scenCreateTask(script_012B, 6, 23308, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_012B, 6, 23310, 81, 0x41F00000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 0x1976AB, 20);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 119);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 232);
      Script_scenCreateTask(script_0030, 2, 0x2976AB, 253);
      Script_scenCreateTask(script_0224, 4, arg0, 6, 25, 5);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 119, 12);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 236, 12);
      break;
    case 9:
      Script_scenCreateTask(script_012B, 6, 23303, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 816811, 30);
      Script_scenCreateTask(script_0030, 2, 0x1976AB, 35);
      Script_scenCreateTask(script_0030, 2, 161451, 39);
      Script_scenCreateTask(script_0030, 2, 816811, 54);
      Script_scenCreateTask(script_0224, 4, arg0, 6, 37, 7);
      Script_scenCreateTask(script_0224, 4, arg0, 7, 54, 8);
      break;
    case 10:
      Script_scenCreateTask(script_012B, 6, 23313, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 0x1976AB, 26);
      Script_scenCreateTask(script_0224, 4, arg0, 6, 27, 7);
      break;
    case 11:
      Script_scenCreateTask(script_012B, 6, 23299, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_012B, 6, 23318, 75, 0x425C0000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 161451, 21);
      Script_scenCreateTask(script_0030, 2, 0x2876AB, 37);
      Script_scenCreateTask(script_0030, 2, 161451, 120);
      break;
    case 12:
      Script_scenCreateTask(script_012B, 6, 23309, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_012B, 6, 23321, 75, 0x425C0000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 0x1976AB, 18);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 105);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 148);
      Script_scenCreateTask(script_0030, 2, 0x2976AB, 160);
      Script_scenCreateTask(script_0224, 4, arg0, 6, 18, 10);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 105, 10);
      Script_scenCreateTask(script_0224, 4, arg0, 3, 136, 10);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 148, 10);
      break;
    case 13:
      Script_scenCreateTask(script_012B, 6, 23304, -1, 0x41F00000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0224, 4, arg0, 4, 30, 8);
      Script_scenCreateTask(script_0224, 4, arg0, 6, 77, 8);
      break;
    case 14:
      Script_scenCreateTask(script_012B, 6, 23314, -1, 0x41F00000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 17);
      Script_scenCreateTask(script_0030, 2, 0x2976AB, 30);
      Script_scenCreateTask(script_0030, 2, 161451, 36);
      Script_scenCreateTask(script_0030, 2, 0x2876AB, 58);
      Script_scenCreateTask(script_0030, 2, 161451, 80);
      Script_scenCreateTask(script_0030, 2, 0x2876AB, 105);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 16, 12);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 75, 10);
      break;
    case 15:
      Script_scenCreateTask(script_012B, 6, 23300, -1, 0x41F00000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 0x2876AB, 60);
      Script_scenCreateTask(script_0030, 2, 30379, 88);
      Script_scenCreateTask(script_0030, 2, 161451, 100);
      Script_scenCreateTask(script_0030, 2, 30379, 114);
      break;
    case 16:
      Script_scenCreateTask(script_012B, 6, 23310, -1, 0x41F00000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 38);
      Script_scenCreateTask(script_0030, 2, 0x2176AB, 151);
      Script_scenCreateTask(script_0030, 2, 0x2976AB, 172);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 38, 12);
      Script_scenCreateTask(script_0224, 4, arg0, 1, 155, 12);
      break;
  }
  return Script_srchControl(0);
}

static void script_1BF1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_1C07, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_1C07(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_1C2B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_1C41, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_1C41(float arg0, float arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetCamShake(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_1C66(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_1C78, 2, arg0, arg1);
  return;
}

static int32_t script_1C78(const char *arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  Script_ShowUIMessageFromID(arg0);
  return Script_srchControl(0);
}

static void script_1C9F(int32_t arg0, int32_t arg1) {
  task_100 = arg0;
  task_0 = 0;
  return;
}

static void script_1CB0(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      SCNT_ShowSubtitle("オート０");
      break;
    case 2:
      SCNT_ShowSubtitle("開始０");
      break;
    case 3:
      SCNT_ShowSubtitle("成功０");
      break;
    case 4:
      SCNT_ShowSubtitle("失敗０");
      break;
    case 5:
      SCNT_ShowSubtitle("死亡０");
      break;
    case 6:
      SCNT_ShowSubtitle("開始１");
      break;
    case 7:
      SCNT_ShowSubtitle("成功１");
      break;
    case 8:
      SCNT_ShowSubtitle("失敗１");
      break;
    case 9:
      SCNT_ShowSubtitle("開始２");
      break;
    case 10:
      SCNT_ShowSubtitle("成功２");
      break;
    case 11:
      SCNT_ShowSubtitle("失敗２");
      break;
    case 12:
      SCNT_ShowSubtitle("開始３");
      break;
    case 13:
      SCNT_ShowSubtitle("成功３");
      break;
    case 14:
      SCNT_ShowSubtitle("失敗３");
      break;
    case 15:
      SCNT_ShowSubtitle("開始４");
      break;
    case 16:
      SCNT_ShowSubtitle("成功４");
      break;
    case 17:
      SCNT_ShowSubtitle("失敗４");
      break;
  }
  Script_srchControl(0);
  return;
}

static void script_1DF1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1E09, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_1E09(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_1E30(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_1E46, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_1E46(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg1) {
    Script_charaDisplay(arg0, CDM_Show);
  } else {
    Script_charaDisplay(arg0, CDM_Hide);
  }
  Script_scenToggleActiveFor(arg0, arg2);
  return Script_srchControl(0);
}

static void script_1E82(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Script_scenCreateTask(script_1E9E, 7, arg2, arg6, arg3, arg4, arg0, arg1, arg5);
  return;
}

static int32_t script_1E9E(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_charaToggleMotEnabled(arg4, 1);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (arg6) {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x80);
  }
  return Script_srchControl(0);
}

static void script_1EF4(void) {
  return;
}

static int32_t script_1EF7(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F4[2];
  GameDateTime local_EC;
  int32_t local_E8;
  SCNF_WaitFrames_0000(5);
  Script_SetQteOverlayTimeoutTuning(0, QTE_TimeoutSession);
  Script_uiUtils(0, 21, 0);
  script_0EDC(18);
  Script_filmCheckOrSetTransitionUtil(60, 255, 255, 255, 255, 0, 0, 0, 0);
  Script_ReturnTimeDate(&local_EC);
  for (;;) {
    Script_SetNewDateTime(&local_EC);
    Script_WeatherTaskUtil(FreeTaskData);
    Script_WeatherTaskUtil(InitTaskData);
    if (0) {
    } else {
      Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
      for (;;) {
        Script_PrintDebugString(15, 14, "PRESS A BUTTON");
        if (Script_GetInputData(0, 2) & 0x400) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    script_1EF4();
    Script_scenCreateTask(script_272B, 2, arg0, arg1);
    Script_executeAicaCommand(25768, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
    Script_executeAicaCommand(1184, 1, 96);
    L001FD3:;
    local_FC = Script_scenCreateTask(script_213F, 2, arg0, arg1);
    task_7C = 0;
    for (;;) {
      if (task_7C >= 10) {
        Script_srchControl((void *)local_FC);
        local_E8 = task_100;
        if (local_E8 == 17) {
          script_1C9F(5, -1);
          SCNF_WaitFrames_0000(200);
        } else {
          script_1C9F(5, -1);
          SCNF_WaitFrames_0000(250);
        }
        break;
      } else {
        if (task_7C == -1) {
          Script_srchControl((void *)local_FC);
          break;
          goto L002055;
        }
      }
      L002055:;
      SCNF_WaitFrames_0000(1);
    }
    Script_executeAicaCommand(2720, 2, 30);
    Script_executeAicaCommand(1184, 1, 127);
    if (task_7C == -1) {
      Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
      script_0F42();
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      goto L002133;
    } else {
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_sicnUtils(0, 1, 1);
      SCNF_WaitFrames_0000(90);
      Script_sicnUtils(2, 1, 0);
      Script_SetQteOverlayTimeoutTuning(1, QTE_TimeoutSession);
      script_0EFA();
      Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
      goto L002133;
    }
    goto L001FD3;
    L002133:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_3194, 2, data_31A8, 3, data_31B2, 5, data_31C6)))
static int32_t script_213F(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = task_48[1];
  local_F8 = 1;
  SCNF_WaitFrames_0000(10);
  while (task_FC == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  script_1DF1(arg0, 2, 0, 1, 1);
  script_1DF1(arg0, 1, 0, 1, 1);
  script_1C9F(2, -1);
  script_0587(arg1, 0, 1, 0x10000000);
  if (task_70 == 0) {
    script_1C9F(4, -1);
    script_1C66((int32_t)"E1062B003", 100);
    script_1DF1(arg0, 4, 0, 8, 23);
    script_1BF1(8702, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_1BF1(8707, SHEN_NAME('DUMC'), SHEN_NAME('DUMC'), 30);
    script_1BF1(8711, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), 60);
    script_1C2B(0x3E4CCCCD, 0x3E4CCCCD, 0x3F333333, 115);
    script_0A54(local_FC, arg0, 0x3E113C68, -0x3FB6CAF7, 0x43340000, -1);
    script_0A54(local_FC, arg0, 0x3FF5D495, -0x41167382, 0x42B40000, 60);
    task_7C = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    script_1C9F(3, -1);
    script_1BF1(8700, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_0A54(local_FC, arg0, 0x3E113C68, -0x3FB6CAF7, 0x43340000, -1);
  }
  SCNF_WaitFrames_0000(33);
  script_1C9F(6, -1);
  script_1BF1(8701, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
  script_0587(arg1, 1, 1, 0x10000000);
  if (task_70 == 0) {
    script_1C9F(8, -1);
    script_1C66((int32_t)"E1062B009", 150);
    script_1DF1(arg0, 4, 0, 8, 28);
    script_1BF1(8708, SHEN_NAME('DUMC'), SHEN_NAME('DUMC'), 30);
    script_1BF1(0x2200, SHEN_NAME('DUMC'), SHEN_NAME('DUMC'), 81);
    script_1BF1(8713, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), 141);
    script_0A54(local_FC, arg0, -0x3FD76998, -0x411BDA51, 0x42B40000, -1);
    script_0A54(local_FC, arg0, -0x427DAA50, -0x3FD6BEB6, 0x433E8ADB, 81);
    script_0C27(arg0, 0x42B40000, -1);
    task_7C = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    script_1C9F(7, -1);
    script_1C66((int32_t)"E1062B003", -1);
    script_1BF1(8701, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_0A54(local_FC, arg0, -0x3FE3714C, 0x3C14C448, 0x42B40000, -1);
    script_0C27(arg0, 0x42B40000, -1);
    SCNF_WaitFrames_0000(14);
  }
  SCNF_WaitFrames_0000(33);
  script_1C9F(9, -1);
  script_1BF1(8705, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
  script_0587(arg1, 2, 1, 0x10000000);
  if (task_70 == 0) {
    script_1C9F(11, -1);
    script_1C66((int32_t)"E1062B005", 180);
    script_1DF1(arg0, 4, 0, 8, 40);
    script_1BF1(8706, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_1BF1(8711, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), 75);
    script_1C2B(0x3E4CCCCD, 0x3E4CCCCD, 0x3F333333, 120);
    script_0A54(local_FC, arg0, 0x3EB6EEB7, -0x3FF147AE, 0x43340000, -1);
    script_0A54(local_FC, arg0, 0x3FF4AFF7, 0x3F0DCC64, 0x42B40000, 75);
    script_0C27(arg0, 0x42B40000, -1);
    script_0C27(arg0, 0x42B40000, 75);
    task_7C = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    script_1C9F(10, -1);
    script_1C66((int32_t)"E1062B008", -1);
    script_1BF1(8706, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_0A54(local_FC, arg0, 0x3E669AD4, -0x3FFF736D, 0x4339B333, -1);
    script_0C27(arg0, 0x42B40000, -1);
    SCNF_WaitFrames_0000(13);
  }
  SCNF_WaitFrames_0000(35);
  script_1C9F(15, -1);
  script_1BF1(8709, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
  script_0587(arg1, 3, 1, 0x10000000);
  if (task_70 == 0) {
    script_1C9F(17, -1);
    script_1C66((int32_t)"E1062B005", 180);
    script_1DF1(arg0, 7, 0, 16, 15);
    script_1DF1(arg0, 4, 0, 8, 60);
    script_1BF1(0x2200, SHEN_NAME('DUMC'), SHEN_NAME('DUMC'), -1);
    script_1BF1(8713, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), 60);
    script_0A54(local_FC, arg0, -0x427DAA50, -0x3FD6BEB6, 0x433E8ADB, -1);
    SCNF_WaitFrames_0000(60);
    task_7C = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    script_1C9F(16, -1);
    script_1BF1(8706, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_1BF1(8710, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), 30);
    script_1BF1(8706, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), 45);
    script_0A54(local_FC, arg0, 0x3ED60419, -0x3FB1CAC1, 0x43340000, -1);
    SCNF_WaitFrames_0000(200);
  }
  task_7C = -1;
  SCNF_WaitFrames_0000(1);
  end:;
}

static int32_t script_272B(int32_t arg0, int32_t arg1) {
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_charaToggleMotEnabled(CHARA_CHAI, 0);
  script_1DF1(arg0, 2, 0, 1, 1);
  script_1DF1(arg0, 4, 0, 1, 1);
  script_1096(1);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_CHAI, 1);
  return Script_srchControl(0);
}

void script_278B(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(5);
  script_27F7(0);
  script_2830(0);
  task_0 = 0;
  for (;;) {
    task_0 = task_0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_27DC(int32_t arg0) {
  for (;;) {
    if (task_C == 2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_31DB, 2, data_31E2)))
static void script_27F7(int32_t arg0) {
  task_C = 0;
  task_10 = Script_motiLoad(3, SHEN_NAME('MC5Q'), "M_MC5Q.BIN");
  Script_LoadVoiceFile(1, "A0184D");
  Script_LoadVoiceFile(1, "E1062");
  return;
}

static void script_2830(int32_t arg0) {
  task_C = 1;
  Script_scenCreateTask(script_2B0A, 5, CHARA_CHAI, 1, SHEN_NAME('DUMC'), 0, 0);
  Script_scenCreateTask(script_295A, 3, CHARA_AKIR, 0, SHEN_NAME('DUMA'));
  task_8 = Script_scenCreateTask(script_1EF7, 2, CHARA_AKIR, 0);
  Script_LoadSND(0, 0, "system1.snd", 0);
  Script_LoadSND(0, 5, "qtebat.snd", 0);
  Script_LoadSND(0, 2, "bgm067.snd", 0);
  Script_SetCameraFlags(0);
  Script_camSetRotationStepSize(0);
  task_4 = Script_SetMaxNPCsNear(32);
  script_27DC(arg0);
  return;
}

__attribute__((scn_string_bindings(0, data_320A, 1, data_3211)))
static void script_28D8(void) {
  task_C = 2;
  Script_SetMaxNPCsNear(task_4);
  Script_LoadVoiceFile(0, "A0184D");
  Script_LoadVoiceFile(0, "E1062");
  Script_PlayCharaMotion(CHARA_AKIR, 0xFFFF, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_CHAI, 0xFFFF, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ReallocateUNFO(*&task_10);
  Script_srchControl((void *)task_8);
  return;
}

static int32_t script_295A(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_078D, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_02F9, 6, arg0, arg1, 0, 2, &data_304C, script_134E);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 2, 4);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_FACE(arg0, 2, 0, 4);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, 4, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_MoveObjectToObject(arg2, arg0);
  Script_EVT_SetPosition(arg2, POS_XYZ, &data_3218);
  Script_EVT_SetRotation(arg2, 0x38000000, &data_3224, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(arg0, 1);
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  for (;;) {
    if (task_C == 2) {
      Script_ResetHandMotion(arg0, 0, 0, 0);
      Script_ResetHandMotion(arg0, 0, 1, 0);
      Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 0);
      Script_faceUpdateFor(arg0, 0, 0);
      Script_faceUpdateFor(arg0, 1, 0);
      Script_srchControl((void *)task_60[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2B0A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_078D, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_02F9, 6, arg0, arg1, arg3, arg4, &data_304C, script_134E);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 1, 4);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_FACE(arg0, 1, 0, 4);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, 4, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_MoveObjectToObject(arg2, arg0);
  Script_EVT_SetPosition(arg2, POS_XYZ, &data_3230);
  Script_EVT_SetRotation(arg2, 0x38000000, &data_323C, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_scenToggleActiveFor(arg0, 1);
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  for (;;) {
    if (task_C == 2) {
      Script_ResetHandMotion(arg0, 0, 0, 0);
      Script_ResetHandMotion(arg0, 0, 1, 0);
      Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 0);
      Script_faceUpdateFor(arg0, 0, 0);
      Script_faceUpdateFor(arg0, 1, 0);
      Script_srchControl((void *)task_60[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}
