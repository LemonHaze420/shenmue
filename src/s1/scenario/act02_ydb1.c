#include "act02_ydb1.h"

static void script_0030(int32_t arg0, const char *arg1, const char *arg2) {
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
      break;
    }
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg2) == 0) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg1, arg2) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  }
  return;
}

static void script_0094(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_00B5(int32_t arg0) {
  L0000B7:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, arg0)) goto L0000B7;
  return;
}

static void script_00CC(int32_t arg0) {
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
      script_00B5(local_FC);
    }
  }
  (void)local_FC;
  return;
}

static void script_0152(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static int32_t script_0175(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static void script_0199(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_0175, 2, arg0, arg1);
  return;
}

static int32_t script_01AB(const char *arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  Script_ShowUIMessageFromID(arg0);
  return Script_srchControl(0);
}

static void script_01D2(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_01AB, 2, arg0, arg1);
  return;
}

static int32_t script_01E4(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = arg3->x;
  (&local_F4)->y = arg3->y;
  (&local_F4)->z = arg3->z;
  local_E8.x = arg4->x;
  (&local_E8)->y = arg4->y;
  (&local_E8)->z = arg4->z;
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  Script_fixoAttachEx(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_024F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_01E4, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_0269(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_fixoClearAttachment(arg0);
  return Script_srchControl(0);
}

static void script_0289(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_0269, 2, arg0, arg1);
  return;
}

static int32_t script_029B(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_ECB4);
  Script_EVT_SetRotation(arg0, 0x38000000, &data_ECC0, 0);
  return Script_srchControl(0);
}

static void script_02DF(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_029B, 3, arg0, arg1, arg2);
  return;
}

static void script_02F3(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_030F(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (arg4 & -0x80000000) {
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  } else {
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  }
  if (arg4 & 0x40000000) {
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  } else {
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
  }
  if (arg4 & 0x20000000) {
    Script_charaToggleMotEnabled(arg0, 0);
  } else {
    Script_charaToggleMotEnabled(arg0, 1);
  }
  Script_SetCharaMotionPlaybackRate(arg0, arg3);
  Script_PlayCharaMotion(arg0, arg1, arg2, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_03B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_030F, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_03CD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg1, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  return Script_srchControl(0);
}

static void script_0412(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_03CD, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_042A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetNewHandMotion(arg0, arg1, arg2, 0, 0, 0, 0, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_045E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_042A, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0474(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  Vector3i local_F4;
  local_F4.x = arg2->x;
  (&local_F4)->y = arg2->y;
  (&local_F4)->z = arg2->z;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetHandRootRotation(arg0, arg1, 0x15, arg2);
  return Script_srchControl(0);
}

static void script_04BC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0474, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_04D2(int32_t arg0, int32_t arg1, const int32_t *arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_handSetFloatsFor(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_04F8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_04D2, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_0510(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_0537(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_0510, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_054F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_GetDataFromFightProgram(arg0, arg1);
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return Script_srchControl(0);
}

static void script_058C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_054F, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_05A2(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  int32_t local_F0;
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_F0 = arg5;
  while (local_F0 > 0) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = (&local_F4)->y + arg2;
    Script_SetLookPointDetails(arg0, arg3 + 0x18000, &local_F4, 0);
    local_F0 = local_F0 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(arg0, -(arg3 + 0x18000), 0, 0);
  return Script_srchControl(0);
}

static void script_0626(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_05A2, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_0640(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  local_FC = arg1;
  while (local_FC > 0) {
    Script_FACE_LCG(arg0, FACE_BlinkClose, 2, 0);
    SCNF_WaitFrames_0000(2);
    Script_FACE_LCG(arg0, FACE_BlinkOpen, 2, 0);
    SCNF_WaitFrames_0000(Script_FloatUtils(ScriptMathOp_RandomIntBelow, 100) + 50);
    local_FC = local_FC - 1;
  }
  return Script_srchControl(0);
}

static void script_06A9(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_0640, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_06BD(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F4.x = arg2->x;
  (&local_F4)->y = arg2->y;
  (&local_F4)->z = arg2->z;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  local_F0 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_F0);
  return Script_srchControl(0);
}

static void script_0721(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_06BD, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0737(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F4.x = arg2;
  (&local_F4)->y = arg3;
  (&local_F4)->z = arg4;
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  local_F0 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  Script_MoveObjectToObject(arg0, local_F0);
  return Script_srchControl(0);
}

static void script_0792(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_0737, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_07AC(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  Vector3i local_F4;
  int32_t local_F0;
  local_F4.x = arg2->x;
  (&local_F4)->y = arg2->y;
  (&local_F4)->z = arg2->z;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  local_F0 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
  Script_MoveObjectToObject(arg0, local_F0);
  return Script_srchControl(0);
}

static void script_0812(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_07AC, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0828(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  Vector3i local_F4;
  int32_t local_F0;
  local_F4.x = 0;
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = 0;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  local_F0 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  Script_MoveObjectToObject(arg0, local_F0);
  return Script_srchControl(0);
}

static void script_0893(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0828, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_08A9(int32_t arg0, const Vector3i *arg1, int32_t arg2) {
  Vector3i local_F4;
  local_F4.x = arg1->x;
  (&local_F4)->y = arg1->y;
  (&local_F4)->z = arg1->z;
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_EVT_SetRotation(arg0, -0x48000000, arg1, 0);
  return Script_srchControl(0);
}

static void script_08F3(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_08A9, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_0907(int32_t arg0, float arg1, int32_t arg2) {
  Vector3i local_F4;
  local_F4.x = 0;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->z = 0;
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return Script_srchControl(0);
}

static void script_0956(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_0907, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_096A(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  int32_t local_F4[3];
  float local_F0;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_F4[1] = 0;
  local_F4[2] = 0;
  local_F0 = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    local_F4[0] = (int32_t)(local_F0 / 360.0f * (float)0x10000);
    script_08F3(arg0, (int32_t)local_F4, -1);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_09E3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_096A, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_09FB(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  int32_t local_F4[3];
  float local_F0;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_F4[0] = 0;
  local_F4[2] = 0;
  local_F0 = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    local_F4[1] = (int32_t)(local_F0 / 360.0f * (float)0x10000);
    script_08F3(arg0, (int32_t)local_F4, -1);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_0A74(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_09FB, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_0A8C(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  int32_t local_F4[3];
  float local_F0;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_F4[0] = 0;
  local_F4[1] = 0;
  local_F0 = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    local_F4[2] = (int32_t)(local_F0 / 360.0f * (float)0x10000);
    script_08F3(arg0, (int32_t)local_F4, -1);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_0B05(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_0A8C, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_0B1D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg1) {
    Script_charaDisplay(arg0, CDM_Show);
    Script_scenToggleActiveFor(arg0, arg2);
  } else {
    Script_charaDisplay(arg0, CDM_Hide);
    Script_scenToggleActiveFor(arg0, 0);
  }
  return Script_srchControl(0);
}

static void script_0B62(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0B1D, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0B78(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_0B9C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0B78, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0BB2(float arg0, float arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetCamShake(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_0BD7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0BB2, 4, arg0, arg1, arg2, arg3);
  return;
}

static void script_0BED(void) {
  Script_shenClearFreeState(0);
  Script_SetCameraFlags(0);
  task_240 = 0;
  task_244 = 0;
  task_248 = Script_SetMaxNPCsNear(16);
  task_24C = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_24C) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  }
  return;
}

static void script_0C48(int32_t arg0, int32_t arg1) {
  if (task_240 < 16) {
    task_0[task_240][0] = arg0;
    task_0[task_240][1] = arg1;
    task_0[task_240][3] = Script_ccowUtils(arg0, CCOW_HasCharacters);
    task_0[task_240][2] = Script_ccowUtils(arg0, CCOW_HasMap);
    task_0[task_240][4] = Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Read);
    if (arg1 & -0x80000000) {
      Script_facemotUtils(arg0, 0, 0);
    }
    if (arg1 & 0x40000000) {
      Script_facemotUtils(arg0, 1, 0);
    }
    Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 0);
    Script_ResetHandMotion(arg0, 1, 1, 0);
    Script_ResetHandMotion(arg0, 1, 0, 0);
    Script_FACE(arg0, 0, 0, 0);
    Script_FACE_LCG(arg0, FACE_BlinkOpen, 0, 0);
    Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
    Script_charaToggleMotEnabled(arg0, 1);
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetFightProgramStateBytes(arg0, 2, 1);
    Script_SetCPAMFlags(arg0, 0);
    Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
    Script_SetCharaFlags(arg0, 4);
    Script_scenToggleActiveFor(arg0, 1);
    task_240 = task_240 + 1;
  }
  return;
}

static void script_0DBA(int32_t arg0, int32_t arg1) {
  if (task_244 < 32) {
    task_140[task_244][0] = arg0;
    task_140[task_244][1] = arg1;
    Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
    Script_SetCharaFlags(arg0, 0);
    Script_scenToggleActiveFor(arg0, 1);
    task_244 = task_244 + 1;
  }
  return;
}

static void script_0E0E(void) {
  while (task_240 > 0) {
    script_0E6D();
  }
  while (task_244 > 0) {
    script_0F98();
  }
  if (task_24C) {
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    Script_SetGameStateTo(AUTH_None);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_ryoControl(RYO_RestoreControl);
  }
  Script_SetMaxNPCsNear(task_248);
  Script_shenClearFreeState(1);
  Script_SetCameraFlags(1);
  return;
}

static void script_0E6D(void) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_240 > 0) {
    task_240 = task_240 - 1;
    local_FC = task_0[task_240][0];
    local_F8 = task_0[task_240][1];
    if (task_0[task_240][3]) {
      Script_ccowUtils(local_FC, CCOW_EnableCharacters);
    }
    if (task_0[task_240][2]) {
      Script_ccowUtils(local_FC, CCOW_EnableMap);
    }
    Script_motmGetCharaFlags(local_FC, task_0[task_240][4], SCEN_Write);
    Script_motmToggleFlags(local_FC, SCEN_Enable, 0x40);
    Script_motmSetBlendFrames(local_FC, MOTM_BLEND_FRAMES_5);
    Script_PlayCharaMotion(local_FC, 0xFFFF, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetFightProgramStateBytes(local_FC, 5, 2);
    Script_SetCPAMFlags(local_FC, 1);
    Script_ResetHandMotion(local_FC, 0, 1, 0);
    Script_ResetHandMotion(local_FC, 0, 0, 0);
    Script_FaceEx(local_FC, FACE_ExpressionDisabled, 0, 0);
    if (local_F8 & 0x40000000) {
      if (local_FC != 0x52494B41) {
        Script_faceUpdateFor(local_FC, 1, 0);
      }
    }
    if (local_F8 & -0x80000000) {
      Script_faceUpdateFor(local_FC, 0, 0);
    }
  }
  return;
}

static void script_0F98(void) {
  int32_t local_FC;
  if (task_244 > 0) {
    task_244 = task_244 - 1;
    local_FC = task_140[task_244][0];
  }
  return;
}

static void script_0FBF(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_ED3C, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_ED04, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_ECCC, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_ECE8, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_ED20, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_10F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  int16_t local_EE;
  task_440 = arg3;
  task_444 = arg2;
  task_414 = 0;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_ED64);
  task_430 = arg1;
  task_428 = arg0;
  task_42C = 1;
  task_614 = 0;
  task_448 = 0;
  Script_scenToggleVisibleFor(task_440, 0);
  Script_scenToggleVisibleFor(task_444, 0);
  if (arg0 == 1) {
    Script_lightResetSlotToDefaults(28);
    Script_LightUtils(28, 0, 4);
    Script_LightUtils(28, 2, &data_F680);
    Script_LightUtils(28, 4, 1.7f);
    Script_LightUtils(28, 5, 2.8f);
    Script_LightUtils(28, 6, data_F690);
    Script_LightUtils(28, 10, 1820);
  }
  local_F0 = 0;
  for (;;) {
    local_EE = Script_ScenDataUtil(&data_ED64, SCND_ReadU16, local_F0 * 16);
    if (local_EE < 0) {
      break;
    }
    if ((Script_ScenDataUtil(&data_ED64, SCND_ReadU16, local_F0 * 16 + 1) == 2) | (Script_ScenDataUtil(&data_ED64, SCND_ReadU16, local_F0 * 16 + 1) == 9)) {
      if (Script_ScenDataUtil(&data_ED64, SCND_ReadU32, local_F0 * 8 + 6) != 0) {
        Script_EVT_GetPosition(Script_ScenDataUtil(&data_ED64, SCND_ReadU32, local_F0 * 8 + 1), -1, &local_F4, 0);
        if (!((&local_F4)->x < 9999.0f) & !((&local_F4)->z < 9999.0f)) {
          task_614 = task_614 + 1;
          task_484[task_614] = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, local_F0 * 8 + 1);
        }
      }
    }
    local_F0 = local_F0 + 1;
  }
  return;
}

static void script_1294(void) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  local_F0 = 0;
  for (;;) {
    local_F4 = Script_ScenDataUtil(data_F2A8, SCND_ReadU32, local_F0 * 5);
    local_F8 = Script_ScenDataUtil(data_F2A8, SCND_ReadU32, local_F0 * 5 + 1);
    if (local_F4 == -1) {
      break;
    }
    Script_EVT_GetPosition(local_F8, -1, &local_E4, 0);
    if (!((&local_E4)->x < 9999.0f) & !((&local_E4)->z < 9999.0f)) {
      task_614 = task_614 + 1;
      task_484[task_614] = local_F8;
    } else if (Script_ScenDataUtil(data_F2A8, SCND_ReadU32, local_F0 * 5 + 3) == 0) {
      Script_mtwkGetSetRotation(local_F8, &task_3F4, 0);
      Script_EVT_GetPosition(local_F8, -1, &task_3E8, 0);
      Script_fixoAttachEx(local_F8, local_F4, 0, &task_3E8, &task_3F4);
    } else {
      task_3F4.x = 0;
      task_3F4.y = 0;
      task_3F4.z = 0;
      task_3E8.x = 0.0f;
      task_3E8.y = 0.0f;
      task_3E8.z = 0.0f;
      Script_CalculateMatrixRotationFromParams(local_F8, local_F4, 0, &task_3E8, &task_3F4);
    }
    local_F0 = local_F0 + 1;
  }
  return;
}

static int32_t script_13B4(void) {
  int16_t local_FE;
  int32_t local_F8;
  task_40C = -1;
  task_40C = Script_srchUtils(-1);
  if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) >= 0) {
    if (task_428 == 1) {
      Script_lightSetSlotEnabled(28, 1);
      script_2607(CHARA_AKIR);
      task_434 = 1;
    }
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    if (task_430) {
      task_410 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
      local_FE = Script_ScenDataUtil(&data_ED64, SCND_ReadU16, task_410 * 16 + 1);
      if ((local_FE < 0) & (Script_ScenDataUtil(&data_ED64, SCND_ReadU32, task_410 * 8 + 7) > 0)) {
        Script_sprintf("Map Start!!!!!!!!!!!!!!!!!\012");
        script_27A6(task_410);
      } else {
        if (task_438 != task_410) {
          for (;;) {
            if (!(task_428 == 1)) goto L001480;
            L001480:;
            if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == Script_ScenDataUtil(&data_ED64, SCND_ReadU32, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1)) {
              break;
            }
            if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 0) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
          for (;;) {
            if ((MMTK_ScriptMemoToggle(2, 0) == 0) & (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE, 0) == 7)) {
              break;
            }
          }
          task_448 = 9;
          script_486B(Script_ScenDataUtil(&data_ED64, SCND_ReadU32, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1));
          task_438 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
        }
      }
    }
    task_420 = 1;
  } else {
    if (task_434 == 1) {
      Script_lightSetSlotEnabled(28, 0);
      task_434 = 0;
    }
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    task_420 = 0;
    if (task_430) {
      task_438 = -1;
    }
  }
  if (task_40C != -1) {
    task_418 = 0;
    task_41C = 0;
    task_424 = 0;
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_convSetCamInactiveFlag(1);
    local_F8 = script_25A2(task_40C);
    local_FE = Script_ScenDataUtil(&data_ED64, SCND_ReadU16, local_F8 * 16 + 1);
    Script_scenToggleVisibleFor(task_440, 1);
    Script_scenToggleVisibleFor(task_444, 1);
    if (local_FE < 0) {
      task_448 = 8;
      script_486B(task_40C);
    } else {
      script_160E(local_F8);
    }
    Script_ryoControl(RYO_RestoreControl);
    Script_scenToggleVisibleFor(task_440, 0);
    Script_scenToggleVisibleFor(task_444, 0);
  }
  return task_40C;
}

static void script_160E(int32_t arg0) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  int32_t local_EC;
  local_FE = Script_ScenDataUtil(&data_ED64, SCND_ReadU16, arg0 * 16 + 1);
  local_EC = local_FE;
  if (local_EC == 0) {
    task_448 = 8;
    script_486B(task_40C);
  } else {
    switch (local_EC) {
      case 0:
        task_448 = 8;
        script_486B(task_40C);
        break;
      case 2:
        script_21CD(arg0);
        break;
      case 3:
        script_176A(arg0, 3);
        break;
      case 4:
        script_176A(arg0, 10);
        break;
      case 5:
        script_176A(arg0, 20);
        break;
      case 6:
        script_176A(arg0, 30);
        break;
      case 7:
        script_176A(arg0, 40);
        break;
      case 8:
        script_176A(arg0, 50);
        break;
      case 9:
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, 0, 60);
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        script_21CD(arg0);
        Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
  }
  return;
}

static void script_176A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F0[2];
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t unused_D8[2];
  int32_t local_D4;
  int32_t local_D0;
  int32_t local_CC;
  int32_t unused_B4[6];
  int32_t local_B0;
  int32_t local_AC;
  int32_t local_A8;
  int32_t local_A4;
  int32_t local_A0;
  int32_t local_9C;
  int32_t local_98;
  int32_t local_94;
  int8_t local_93;
  uint8_t unused_92;
  int8_t local_91;
  uint8_t unused_90;
  Vector3 local_84;
  int32_t unused_80;
  float local_FF78[2];
  int32_t local_FF74;
  local_91 = 0;
  local_93 = 0;
  local_FC = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 1);
  local_CC = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 6);
  local_D0 = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 7);
  local_E4 = 0;
  if (Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4) != -1) {
    local_B0 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4);
    local_AC = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 1);
    local_A8 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 2);
    local_A4 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 3);
    local_E4 = 4;
    for (;;) {
      if ((Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4) == -1) & (Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 1) == -1)) {
        local_B0 = 0;
        local_AC = 0;
        local_A8 = 0;
        local_A4 = 0;
        break;
      }
      if (Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4) == -1) {
        local_E4 = local_E4 + 1;
        local_B0 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4);
        local_AC = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 1);
        local_A8 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 2);
        local_A4 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 3);
        local_E4 = local_E4 + 4;
      }
      if (local_FC == Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4)) {
        break;
      }
      local_E4 = local_E4 + 1;
    }
  }
  if (task_430) {
    task_448 = 4;
    script_486B(local_FC);
  }
  if (task_424 == 0) {
    if (local_D0 != 0) {
      task_44C = *(int32_t *)(data_F44C + (local_D0 - 1) * 56);
      *(&task_44C + 1) = *(int32_t *)(data_F44C + ((local_D0 - 1) * 56 + 4));
      *(&task_44C + 2) = *(int32_t *)(data_F44C + ((local_D0 - 1) * 56 + 8));
      *(&task_44C + 3) = *(int32_t *)(data_F44C + ((local_D0 - 1) * 56 + 12));
      *(&task_44C + 4) = *(int32_t *)(data_F44C + ((local_D0 - 1) * 56 + 16));
      *(&task_44C + 5) = *(int32_t *)(data_F44C + ((local_D0 - 1) * 56 + 20));
      *(&task_44C + 6) = *(int32_t *)(data_F44C + ((local_D0 - 1) * 56 + 24));
      *(&task_44C + 7) = *(int32_t *)(data_F44C + ((local_D0 - 1) * 2 + 1) * 28);
      *(&task_44C + 8) = *(int32_t *)(data_F44C + (((local_D0 - 1) * 2 + 1) * 28 + 4));
      *(&task_44C + 9) = *(int32_t *)(data_F44C + (((local_D0 - 1) * 2 + 1) * 28 + 8));
      *(&task_44C + 10) = *(int32_t *)(data_F44C + (((local_D0 - 1) * 2 + 1) * 28 + 12));
      *(&task_44C + 11) = *(int32_t *)(data_F44C + (((local_D0 - 1) * 2 + 1) * 28 + 16));
      *(&task_44C + 12) = *(int32_t *)(data_F44C + (((local_D0 - 1) * 2 + 1) * 28 + 20));
      *(&task_44C + 13) = *(int32_t *)(data_F44C + (((local_D0 - 1) * 2 + 1) * 28 + 24));
      Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &task_44C);
    }
    if (local_CC != 0) {
      arg1 = local_CC - 1;
    }
    Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_B0, local_AC);
    Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, local_FC, arg1);
    local_E0 = local_E0 + 1;
    for (;;) {
      if (task_428 == 1) {
        script_2607(local_FC);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
      local_E0 = local_E0 + 1;
      if (local_E0 > 30) {
        if (Script_GetControllerInput(1) == 0x400) {
          local_91 = 1;
        }
      }
    }
    local_D4 = local_FC;
    task_448 = 5;
    script_486B(local_FC);
    local_E4 = 0;
    for (;;) {
      local_EC = Script_ScenDataUtil(data_F2A8, SCND_ReadU32, local_E4 * 5);
      if (local_EC == -1) {
        break;
      }
      if (local_EC == task_40C) {
        local_E8 = Script_ScenDataUtil(data_F2A8, SCND_ReadU32, local_E4 * 5 + 1);
        local_F8 = Script_ScenDataUtil(data_F2A8, SCND_ReadU32, local_E4 * 5 + 2);
        local_FF74 = Script_ScenDataUtil(data_F2A8, SCND_ReadU32, local_E4 * 5 + 4);
        Script_fixoClearAttachment(local_E8);
        Script_MoveObjectToObject(local_E8, local_FC);
        Script_EVT_GetPosition(local_E8, -1, &task_3E8, 0);
        Script_mtwkGetSetRotation(local_E8, &task_3F4, 0);
        Script_MoveObjectToObject(local_E8, -1);
        local_E4 = -1;
        break;
      }
      local_E4 = local_E4 + 1;
    }
    local_E0 = 0;
    if (Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0) != -1) {
      local_A0 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0);
      local_9C = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0 + 1);
      local_98 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0 + 2);
      local_94 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0 + 3);
      local_E0 = 4;
      for (;;) {
        if ((Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0) == -1) & (Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0 + 1) == -1)) {
          local_A0 = 0;
          local_9C = 0;
          local_98 = 0;
          local_94 = 0;
          break;
        }
        if (Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0) == -1) {
          local_E0 = local_E0 + 1;
          local_A0 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0);
          local_9C = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0 + 1);
          local_98 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0 + 2);
          local_94 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0 + 3);
          local_E0 = local_E0 + 4;
        }
        if (local_E8 == Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E0)) {
          break;
        }
        local_E0 = local_E0 + 1;
      }
    }
    local_E0 = 0;
    for (;;) {
      if (task_614 == local_E0) {
        break;
      }
      local_E0 = local_E0 + 1;
      if (task_484[local_E0] == local_E8) {
        Script_sprintf("このアイテムもってる");
        local_E4 = 0;
      }
    }
    if (local_E4 == -1) {
      if (Script_scenCheckCharaExist(local_E8)) {
        Script_sprintf("tansu naka = %d\012", Script_charaDisplay(local_E8, CDM_Query));
        if (Script_charaDisplay(local_E8, CDM_Query) == 0) {
          local_E4 = 0;
        }
      }
    }
    if (task_41C == 1) {
      local_F8 = 0;
    }
    if ((local_E4 == -1) & (task_42C >= 0)) {
      for (;;) {
        if (task_428 == 1) {
          script_2607(local_FC);
        }
        Script_InvestigationUtils(ShenmueUtils_UpdateSpecialAssetInput);
        if ((Script_GetControllerInput(0) & 0x400) | (local_91 == 1)) {
          Script_EVT_GetPosition(local_E8, -1, &local_84, 0);
          Script_ScreenToWorldAssign(&local_84, (void *)local_FF78);
          if (-!(*local_FF78 <= 80.0f) & -!(*local_FF78 >= 560.0f) & -!(local_FF78[1] <= 60.0f) & -!(local_FF78[1] >= 420.0f)) {
            if (local_F8 > -1) {
              break;
            }
          }
        }
        if (Script_GetControllerInput(0) & 0x200 & -(task_42C == 1)) {
          local_F8 = -1;
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      if (local_F8 != -1) {
        Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_A0, local_9C);
        if (local_FF74 == 0) {
          Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, local_E8);
        } else {
          Script_InvestigationUtils(ShenmueUtils_BeginTargetModeByType, local_E8, local_FF74 - 1);
        }
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        if (task_428 == 1) {
          script_2607(local_FC);
        }
        task_448 = 1;
        script_486B(local_E8);
        if (task_42C >= 0) {
          for (;;) {
            if (task_428 == 1) {
              script_2607(local_FC);
            }
            if ((local_91 == 1) & (task_42C == 1)) {
              break;
            }
            if (Script_GetControllerInput(0) & 0x400 & -(task_42C == 1)) {
              break;
            }
            if (Script_GetControllerInput(0) & 0x200 & -(task_42C == 1)) {
              local_F8 = 0;
              break;
            }
            SCNF_WaitFrames_0000(1);
            if (task_418 == 0) {
              Script_InvestigationUtils(ShenmueUtils_UpdateInterpolatedSpeed);
            }
          }
          if (local_F8 == 0) {
            if (task_430) {
              task_448 = 3;
              script_486B(local_E8);
            }
            Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_98, local_94);
            Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
          } else {
            if (task_430) {
              task_448 = 2;
              script_486B(local_E8);
            }
            if (local_F8 & 0x8000000) {
              Script_GetCassetteFlags(local_F8, 1, 0, 1);
              SCNF_StartIconSystemMenu(local_F8 + 255);
            } else {
              Script_GetCassetteFlags(local_F8, 1, 0, 0);
              SCNF_StartIconSystemMenu(local_F8);
            }
            Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantB);
            task_614 = task_614 + 1;
            task_484[task_614] = local_E8;
            local_93 = 1;
          }
          for (;;) {
            if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      Script_fixoAttachEx(local_E8, local_FC, 0, &task_3E8, &task_3F4);
    } else {
      if (task_42C >= 0) {
        for (;;) {
          if (task_428 == 1) {
            script_2607(local_FC);
          }
          Script_InvestigationUtils(ShenmueUtils_UpdateSpecialAssetInput);
          if ((local_91 == 1) & (task_42C == 1)) {
            break;
          }
          if (Script_GetControllerInput(0) & 0x400 & -(task_42C == 1)) {
            break;
          }
          if (Script_GetControllerInput(0) & 0x200 & -(task_42C == 1)) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
    }
    if (task_42C >= 0) {
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_A8, local_A4);
      Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
      task_448 = 6;
      script_486B(local_D4);
      for (;;) {
        if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      task_448 = 7;
      script_486B(local_D4);
      if (!(task_428 == 1)) goto L00219F;
      L00219F:;
      if (local_93 == 1) {
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_SetGameStateTo(AUTH_None);
      }
    } else {
      task_42C = 1;
      Script_srchUtils(SRCH_SCRIPT_Reset);
      Script_SetGameStateTo(AUTH_None);
    }
  }
  return;
}

static void script_21CD(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int8_t local_E3;
  int8_t local_E2;
  local_E3 = 0;
  local_FC = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 1);
  local_E4 = 0;
  if (Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4) != -1) {
    local_F4 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4);
    local_F0 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 1);
    local_EC = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 2);
    local_E8 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 3);
    local_E4 = 4;
    for (;;) {
      if ((Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4) == -1) & (Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 1) == -1)) {
        local_F4 = 0;
        local_F0 = 0;
        local_EC = 0;
        local_E8 = 0;
        break;
      }
      if (Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4) == -1) {
        local_E4 = local_E4 + 1;
        local_F4 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4);
        local_F0 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 1);
        local_EC = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 2);
        local_E8 = Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4 + 3);
        local_E4 = local_E4 + 4;
      }
      if (local_FC == Script_ScenDataUtil(&data_F310, SCND_ReadU32, local_E4)) {
        break;
      }
      local_E4 = local_E4 + 1;
    }
  }
  if (Script_charaDisplay(local_FC, CDM_Query)) {
    Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_F4, local_F0);
    if (Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 7) == 0) {
      Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, local_FC);
    } else {
      Script_InvestigationUtils(ShenmueUtils_BeginTargetModeByType, local_FC, Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 7) - 1);
    }
    local_E2 = 0;
    for (;;) {
      SCNF_WaitFrames_0000(1);
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
      if (Script_GetControllerInput(1) == 0x400) {
        local_E2 = 1;
      }
    }
    if (task_428 == 1) {
      script_2607(local_FC);
    }
    task_448 = 1;
    script_486B(task_40C);
    local_F8 = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 6);
    for (;;) {
      if (task_428 == 1) {
        script_2607(local_FC);
      }
      SCNF_WaitFrames_0000(1);
      if (task_418 == 0) {
        Script_InvestigationUtils(ShenmueUtils_UpdateInterpolatedSpeed);
      }
      if ((local_E2 == 1) & (task_42C == 1)) {
        break;
      }
      if (Script_GetControllerInput(0) & 0x400 & -(task_42C == 1)) {
        break;
      }
      if (Script_GetControllerInput(0) & 0x200 & -(task_42C == 1)) {
        local_F8 = 0;
        break;
      }
    }
    if (task_41C == 1) {
      local_F8 = 0;
    }
    if (local_F8 == 0) {
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
    } else {
      if (task_430) {
        task_448 = 2;
        script_486B(task_40C);
      }
      if (local_F8 & 0x8000000) {
        Script_GetCassetteFlags(local_F8, 1, 0, 1);
        SCNF_StartIconSystemMenu(local_F8 + 255);
      } else {
        Script_GetCassetteFlags(local_F8, 1, 0, 0);
        SCNF_StartIconSystemMenu(local_F8);
      }
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantB);
      task_614 = task_614 + 1;
      task_484[task_614] = local_FC;
      local_E3 = 1;
    }
    for (;;) {
      SCNF_WaitFrames_0000(1);
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
    }
    if (!(task_428 == 1)) goto L00256A;
    L00256A:;
    if (local_E3 == 1) {
      Script_srchUtils(SRCH_SCRIPT_Reset);
      Script_SetGameStateTo(AUTH_None);
    }
    if (local_F8 == 0) {
      if (task_430) {
        task_448 = 3;
        script_486B(task_40C);
      }
    }
  }
  return;
}

static int32_t script_25A2(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int16_t local_F6;
  local_FC = 0;
  local_F8 = 0;
  for (;;) {
    local_F6 = Script_ScenDataUtil(&data_ED64, SCND_ReadU16, local_F8 * 16);
    if (local_F6 < 0) {
      break;
    }
    if (Script_ScenDataUtil(&data_ED64, SCND_ReadU32, local_F8 * 8 + 1) == arg0) {
      local_FC = local_F8;
      break;
    }
    local_F8 = local_F8 + 1;
  }
  return local_FC;
}

static void script_2607(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  int32_t unused_C0;
  int32_t local_B8[2];
  int32_t local_B4;
  int16_t local_B2;
  uint8_t unused_B0[2];
  local_B4 = script_25A2(arg0);
  local_B2 = Script_ScenDataUtil(&data_ED64, SCND_ReadU16, local_B4 * 16 + 1);
  if (local_B2 < 0) {
    *(int32_t *)&local_F4.x = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, local_B4 * 8 + 3);
    *(int32_t *)&(&local_F4)->y = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, local_B4 * 8 + 4);
    *(int32_t *)&(&local_F4)->z = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, local_B4 * 8 + 5);
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  }
  Script_GetAuthorState_And_SetCamPos(0, &local_DC);
  Script_GetAuthorState_And_SetCamPos(&local_D0, 0);
  local_B8[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D0)->x - (&local_DC)->x, (&local_D0)->z - (&local_DC)->z);
  local_B8[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D0)->z - (&local_DC)->z, (&local_D0)->y - (&local_DC)->y);
  local_C4.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_B8[1]);
  (&local_C4)->y = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(int32_t *)local_B8);
  (&local_C4)->z = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_B8[1]);
  Script_lightSpotLightEnable(28, &local_D0, &local_C4);
  Script_LightUtils(28, 2, &data_F680);
  Script_LightUtils(28, 4, 0.85f);
  Script_LightUtils(28, 5, 7.0f);
  Script_LightUtils(28, 6, data_F690);
  Script_LightUtils(28, 10, 7500);
  return;
}

static void script_277E(void) {
  task_42C = 0;
  return;
}

static void script_2788(void) {
  task_42C = 1;
  return;
}

static void script_2792(void) {
  task_42C = -1;
  return;
}

static void script_279C(int32_t arg0) {
  task_428 = arg0;
  return;
}

static void script_27A6(int32_t arg0) {
  int32_t local_F4[3];
  int32_t unused_F0;
  int32_t local_E8[2];
  local_F4[0] = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 3);
  local_F4[1] = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 4);
  local_F4[2] = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 5);
  local_E8[1] = Script_ScenDataUtil(&data_ED64, SCND_ReadU16, arg0 * 16 + 4);
  task_618 = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, arg0 * 8 + 7) - 1;
  *(&task_618 + 1) = 0;
  *(&task_618 + 2) = 0;
  *(&task_618 + 3) = 0;
  *(&task_618 + 4) = 0;
  *(&task_618 + 5) = local_E8[1];
  *(&task_618 + 6) = 0;
  *(&task_618 + 7) = -1;
  *(&task_618 + 8) = 0;
  *(&task_618 + 9) = 0;
  *(&task_618 + 10) = 0;
  *(&task_618 + 11) = 0;
  *(&task_618 + 12) = 0;
  *(&task_618 + 13) = 0;
  Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &task_618);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, (void *)local_F4, 60);
  for (;;) {
    if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    Script_InvestigationUtils(ShenmueUtils_UpdateSpecialAssetInput);
    SCNF_WaitFrames_0000(1);
    if (Script_GetControllerInput(0) == 0x400) {
      break;
    }
    if (Script_GetControllerInput(0) == 0x200) {
      break;
    }
  }
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  for (;;) {
    if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  return;
}

__attribute__((scn_string_bindings(5, data_F7A6, 8, data_F7C4, 9, data_F7CE)))
static int32_t script_294C(void) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_EC[3];
  int32_t local_E0[3];
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t unused_CC[2];
  int32_t local_C8;
  int32_t local_C4;
  task_334 = 0;
  task_330 = 0;
  local_EC[0] = 0;
  local_EC[1] = 0x3CF5C28F;
  local_EC[2] = 0x3CA3D70A;
  local_E0[0] = 0;
  local_E0[1] = 23665;
  local_E0[2] = 0x4000;
  task_32C = 0;
  task_324 = 0;
  task_328 = 0;
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 1);
  task_394[0] = 0;
  task_394[1] = 0;
  task_394[2] = 0;
  task_2F8 = 0;
  local_C8 = 0;
  for (;;) {
    task_338[local_C8] = 0;
    local_C8 = local_C8 + 1;
    if (local_C8 == 20) {
      break;
    }
  }
  task_38C = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 836)) {
    Script_charaDisplay(SHEN_NAME('MATI'), CDM_Hide);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_BOOK_FOR_MASTER_CHEN)) {
    Script_charaDisplay(SHEN_NAME('BOOK'), CDM_Hide);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 844)) {
    Script_EVT_SetPosition(SHEN_NAME('IROU'), POS_XYZ, &data_F768);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 226)) {
    task_388 = 1;
  }
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  Script_InvestigationUtils(ShenmueUtils_SetSpecialAssetList, data_F60C);
  script_10F7(1, 1, 0x4C554B59, 0x52554B59);
  script_1294();
  for (;;) {
    if (task_2E4 & -(task_250 == 0)) {
      Script_GetCameraInputDetails(15);
    }
    task_2D0 = -1;
    local_DC = Script_cassetteUtils();
    local_D8 = (local_DC & -0x10000) >> 16;
    local_D4 = local_DC & 0xFFFF;
    if ((local_D8 == 56) & (local_D4 == 2)) {
      if ((Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == 7) & (task_2CC == 0)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
        } else {
          script_AFA5();
        }
      } else {
        task_3CC.x = 3.994f;
        task_3CC.y = 0.0f;
        task_3CC.z = 3.999f;
        if (script_6302() == 1) {
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
          } else {
            script_AFA5();
          }
        }
      }
    }
    if (task_250 == 0) {
      script_3D4B();
    }
    if ((local_D8 == 54) & (local_D4 == 2)) {
      int32_t local_C0;
      local_C0 = Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection);
      switch (local_C0) {
        case 0x31554F52:
          if (*task_304 == 0) {
            if (*task_304 != 2) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B56B(1);
              Script_ryoControl(RYO_RestoreControl);
              task_304[0] = 1;
              task_394[0] = 0;
              task_3A4 = 0;
            }
            goto L002BD2;
          }
          L002BD2:;
          break;
        case 0x32554F52:
          if (task_304[1] == 0) {
            if (task_304[1] != 2) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B56B(2);
              Script_ryoControl(RYO_RestoreControl);
              task_304[1] = 1;
              task_394[1] = 0;
              *(&task_3A4 + 1) = 0;
            }
            goto L002C33;
          }
          L002C33:;
          break;
        case 0x33554F52:
          if (task_304[2] == 0) {
            if (task_304[2] != 2) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B56B(3);
              Script_ryoControl(RYO_RestoreControl);
              task_304[2] = 1;
              task_394[2] = 0;
              *(&task_3A4 + 2) = 0;
            }
            goto L002C94;
          }
          L002C94:;
          break;
        case 0x34554F52:
          if (task_304[3] == 0) {
            if (task_304[3] != 2) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B56B(4);
              Script_ryoControl(RYO_RestoreControl);
              task_304[3] = 1;
              task_394[3] = 0;
              *(&task_3A4 + 3) = 0;
            }
            goto L002CF5;
          }
          L002CF5:;
          break;
      }
      if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) < 0) {
        Script_EVT_GetPosition(SHEN_NAME('ROU1'), -1, &task_3CC, 0);
        if (script_6302() == 1) {
          if (*task_304 == 0) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B56B(1);
            Script_ryoControl(RYO_RestoreControl);
            task_304[0] = 1;
            task_394[0] = 0;
            task_3A4 = 0;
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU2'), -1, &task_3CC, 0);
        if (script_6302() == 1) {
          if (task_304[1] == 0) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B56B(2);
            Script_ryoControl(RYO_RestoreControl);
            task_304[1] = 1;
            task_394[1] = 0;
            *(&task_3A4 + 1) = 0;
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU3'), -1, &task_3CC, 0);
        if (script_6302() == 1) {
          if (task_304[2] == 0) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B56B(3);
            Script_ryoControl(RYO_RestoreControl);
            task_304[2] = 1;
            task_394[2] = 0;
            *(&task_3A4 + 2) = 0;
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU4'), -1, &task_3CC, 0);
        if (script_6302() == 1) {
          if (task_304[3] == 0) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B56B(4);
            Script_ryoControl(RYO_RestoreControl);
            task_304[3] = 1;
            task_394[3] = 0;
            *(&task_3A4 + 3) = 0;
          }
        }
      }
    }
    if ((local_D8 == 55) & (local_D4 == 2)) {
      int32_t local_C0;
      local_C0 = Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection);
      switch (local_C0) {
        case 0x31554F52:
          if (*task_304 == 1) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B936(1);
            Script_ryoControl(RYO_RestoreControl);
            task_304[0] = 2;
            task_394[0] = 0;
            task_3A4 = 0;
          }
          break;
        case 0x32554F52:
          if (task_304[1] == 1) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B936(2);
            Script_ryoControl(RYO_RestoreControl);
            task_304[1] = 2;
            task_394[1] = 0;
            *(&task_3A4 + 1) = 0;
          }
          break;
        case 0x33554F52:
          if (task_304[2] == 1) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B936(3);
            Script_ryoControl(RYO_RestoreControl);
            task_304[2] = 2;
            task_394[2] = 0;
            *(&task_3A4 + 2) = 0;
          }
          break;
        case 0x34554F52:
          if (task_304[3] == 1) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B936(4);
            Script_ryoControl(RYO_RestoreControl);
            task_304[3] = 2;
            task_394[3] = 0;
            *(&task_3A4 + 3) = 0;
          }
          break;
      }
      if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) < 0) {
        Script_EVT_GetPosition(SHEN_NAME('ROU1'), -1, &task_3CC, 0);
        if (script_6302() == 1) {
          if ((local_D8 == 55) & (local_D4 == 2)) {
            if (*task_304 == 1) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B936(1);
              Script_ryoControl(RYO_RestoreControl);
              task_304[0] = 2;
              task_394[0] = 0;
              task_3A4 = 0;
            }
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU2'), -1, &task_3CC, 0);
        if (script_6302() == 1) {
          if ((local_D8 == 55) & (local_D4 == 2)) {
            if (task_304[1] == 1) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B936(2);
              Script_ryoControl(RYO_RestoreControl);
              task_304[1] = 2;
              task_394[1] = 0;
              *(&task_3A4 + 1) = 0;
            }
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU3'), -1, &task_3CC, 0);
        if (script_6302() == 1) {
          if ((local_D8 == 55) & (local_D4 == 2)) {
            if (task_304[2] == 1) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B936(3);
              Script_ryoControl(RYO_RestoreControl);
              task_304[2] = 2;
              task_394[2] = 0;
              *(&task_3A4 + 2) = 0;
            }
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU4'), -1, &task_3CC, 0);
        if (script_6302() == 1) {
          if ((local_D8 == 55) & (local_D4 == 2)) {
            if (task_304[3] == 1) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B936(4);
              Script_ryoControl(RYO_RestoreControl);
              task_304[3] = 2;
              task_394[3] = 0;
              *(&task_3A4 + 3) = 0;
            }
          }
        }
      }
    }
    local_FC = script_13B4();
    if (local_FC == 0x5F4F4E4F) {
      if ((task_254 == 3) | ((task_254 == 2) & (task_2D4 == 1))) {
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_lightSetSlotEnabled(28, 0);
        Script_SetGameStateTo(AUTH_None);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
        Script_motiSetAngFlags(CHARA_AKIR, 0);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_F738);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_F744, 0);
        Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
        Script_PlayCharaMotion(CHARA_AKIR, 4164, 70.0f, -1, -1, 1.5f, 0);
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_WALL_DIFFERENT_COLOR) == 1) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SPACE_BEHIND_WALL) | (task_320 == 1)) {
          Script_ShowUIMessageFromID("E1012A272");
        } else {
          Script_ShowUIMessageFromID("E1012A014");
        }
        for (;;) {
          task_258 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
          if (task_258 == 120) {
            Script_executeAicaCommand(0x2505A9, 0, 0);
            Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
            Script_fixoAttachEx(CHARA_ONO_, CHARA_AKIR, 18, &data_F720, &data_F72C);
          }
          if (task_258 == 200) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_motiSetAngFlags(CHARA_AKIR, 1);
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
        Script_fixoClearAttachment(CHARA_ONO_);
        Script_fixoAttachEx(CHARA_ONO_, CHARA_AKIR, 18, &data_F750, &data_F75C);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MOVE_BACK_30, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(30);
        Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
        Script_ryoControl(RYO_RestoreControl);
        task_2CC = 1;
      } else {
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_SetGameStateTo(AUTH_None);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
        Script_motiSetAngFlags(CHARA_AKIR, 0);
        Script_lightSetSlotEnabled(28, 0);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_F738);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_F744, 0);
        Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
        Script_PlayCharaMotion(CHARA_AKIR, 4163, 0.0f, -1, -1, 1.5f, 0);
        Script_ShowUIMessageFromID("E1012A271");
        for (;;) {
          task_258 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
          if (task_258 == 47) {
            Script_executeAicaCommand(0x2505A9, 0, 0);
            Script_fixoAttachEx(CHARA_ONO_, CHARA_AKIR, 18, &data_F720, &data_F72C);
          }
          if (task_258 == 148) {
            Script_executeAicaCommand(0x1105A9, 0, 0);
            Script_fixoClearAttachment(CHARA_ONO_);
          }
          if (task_258 == 180) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_motiSetAngFlags(CHARA_AKIR, 1);
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MOVE_BACK_30, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(30);
        Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
        Script_ryoControl(RYO_RestoreControl);
        Script_SetGameStateTo(AUTH_None);
      }
    } else {
      if (((task_254 == 3) & (task_2CC == 0)) | (-(task_2CC == 0) & -(task_254 == 2) & -(task_2D4 == 1))) {
        if (-(Script_GetControllerInput(1) == 0x400) & Script_IsPlayerAllowed()) {
          if (script_63B5() == 1) {
            SCNF_RequestIconNotification(0, 0, 0, 2);
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x5F4F4E4F);
            local_FC = 0;
          }
        }
      }
    }
    if (-(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 0) & -(local_D8 == 51) & -(local_D4 == 2)) {
      if (task_2E4 == 1) {
        task_2E4 = 0;
        script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
        Script_lightSetSlotEnabled(15, 0);
        script_279C(0);
        SCNF_WaitFrames_0000(1);
        Script_uiUtils(0, 0, 0);
        Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 0);
      } else if (Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) > 0) {
        task_2E4 = 1;
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
        script_D9A7(CHARA_AKIR, SHEN_NAME('LIGH'));
        script_279C(1);
        Script_lightSetSlotEnabled(15, 1);
        Script_uiUtils(0, 26, 0);
        Script_CharHandlers(CHARA_AKIR, 1, 3, 16);
        Script_scenToggleCharaFlag(0, 100, 0);
        Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 1);
        task_2F8 = 1;
      } else {
        int32_t local_C0;
        Script_ryoControl(PCCTL_DisablePlayerControl);
        local_C4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f));
        local_C0 = local_C4;
        switch (local_C0) {
          case 0:
            Script_ShowUIMessageFromID("E1012A288");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1012A289");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(RYO_RestoreControl);
      }
    }
    if ((Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) > 27) & (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 35)) {
      task_328 = 1;
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES) == 0) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES) == 0)) {
        for (;;) {
          Script_lightSetSlotEnabled(28, 1);
          script_2607(CHARA_AKIR);
          if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == 0x414B554D) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_ShowUIMessageFromID("E1012A246");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_SetGameStateTo(AUTH_None);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES, 1);
        Script_ryoControl(RYO_RestoreControl);
      }
    } else {
      task_328 = 0;
      if ((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES) == 1) | -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES) == 1)) & -(task_2D4 == 0)) {
        Script_EVT_GetPosition(SHEN_NAME('MUKA'), -1, &task_3CC, 0);
        task_3CC.x = 6.078f;
        task_3CC.y = 0.0f;
        task_3CC.z = 0.699f;
        if (script_6302() == 1) {
          if (-(Script_GetControllerInput(1) == 0x400) & Script_IsPlayerAllowed()) {
            if (task_428 == 1) {
              Script_lightSetSlotEnabled(28, 0);
            }
            SCNF_RequestIconNotification(0, 0, 0, 2);
            task_2D4 = 1;
            Script_SetGameStateTo(AUTH_None);
            script_CA6B();
          }
          task_328 = 1;
        }
      }
    }
    if (task_320 == 0) {
      if (Script_ScenDataUtil(&data_ED64, SCND_ReadU32, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1) == 0x594D4144) {
        for (;;) {
          Script_lightSetSlotEnabled(28, 1);
          script_2607(CHARA_AKIR);
          if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == 0x594D4144) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_ShowUIMessageFromID("E1012A091");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_SetGameStateTo(AUTH_None);
        Script_ryoControl(RYO_RestoreControl);
        task_320 = 1;
      }
    }
    if ((Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) > 20) & (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 24)) {
      if (task_394[Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) - 21] == 0) {
        for (;;) {
          if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection)) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(PCCTL_DisablePlayerControl);
        {
          int32_t local_C0;
          local_C0 = Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection);
          switch (local_C0) {
            case 0x31554F52:
              task_394[0] = 1;
              local_C4 = *task_304;
              break;
            case 0x32554F52:
              task_394[1] = 1;
              local_C4 = task_304[1];
              break;
            case 0x33554F52:
              task_394[2] = 1;
              local_C4 = task_304[2];
              break;
            case 0x34554F52:
              task_394[3] = 1;
              local_C4 = task_304[3];
              break;
          }
        }
        {
          int32_t local_C0;
          local_C0 = Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection);
          if ((local_C0 == 0x31554F52) | (local_C0 == 0x32554F52) | (local_C0 == 0x33554F52) | (local_C0 == 0x34554F52)) {
            local_C0 = local_C4;
            switch (local_C0) {
              case 0:
                local_C4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f));
                local_C0 = local_C4;
                switch (local_C0) {
                  case 0:
                    Script_ShowUIMessageFromID("E1012A112");
                    break;
                  case 1:
                    Script_ShowUIMessageFromID("E1012A198");
                    break;
                }
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                break;
              case 1:
                local_C4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f));
                local_C0 = local_C4;
                switch (local_C0) {
                  case 0:
                    Script_ShowUIMessageFromID("E1012A135");
                    break;
                  case 1:
                    Script_ShowUIMessageFromID("E1012A066");
                    break;
                }
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                break;
              case 2:
                local_C4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f));
                local_C0 = local_C4;
                switch (local_C0) {
                  case 0:
                    Script_ShowUIMessageFromID("E1012A190");
                    break;
                  case 1:
                    Script_ShowUIMessageFromID("E1012A192");
                    break;
                }
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                break;
            }
            goto L003AE8;
          }
        }
        L003AE8:;
        Script_ryoControl(RYO_RestoreControl);
      }
    }
    (void)(task_2D4 == 0);
    task_318 = 0;
    if ((task_254 == 2) | ((task_254 == 3) & (task_250 == 0))) {
      if (local_FC == 0x594D4144) {
        if ((task_254 == 3) & (task_2CC == 0)) {
          script_4348(0);
        } else {
          Script_srchUtils(SRCH_SCRIPT_Reset);
          SCNF_WaitFrames_0000(1);
          task_318 = 1;
        }
      }
      if (-(Script_GetControllerInput(1) == 0x400) & -(task_2D4 == 1) & -(task_318 == 0)) {
        if ((-(task_254 == 2) | -(task_254 == 3)) & -(script_6438() == 1) & Script_IsPlayerAllowed()) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x594D4144);
          local_FC = 0;
        } else {
          if ((script_6438() == 1) & (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == -1)) {
            task_318 = 1;
            local_FC = 0;
          }
        }
      }
    }
    if (-(Script_GetControllerInput(1) == 0x400) & Script_IsPlayerAllowed()) {
      task_3CC.x = 0.0342f;
      task_3CC.y = 0.0f;
      task_3CC.z = 2.9097f;
      if (script_6302() == 1) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
          script_CED4();
        }
      }
    }
    if (-(local_FC == -1) & -(Script_GetControllerInput(1) == 0x400) & -(task_250 == 0) & -(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1) & Script_IsPlayerAllowed(0)) {
      int32_t local_C0;
      local_C0 = Script_mapEvUtils(0x4D450002, CHARA_AKIR);
      switch (local_C0) {
        case 0:
          script_4348(0);
          break;
        case 100:
          script_4348(100);
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_3CA6(void) {
  for (;;) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 0);
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 0);
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMLIGH, 0);
    if ((Script_GetCurrentEntryPoint() == 3) | (Script_GetCurrentEntryPoint() == 5)) {
      if (Script_GetInputData(3, 1) == 0x400) {
        SAVE_Script_UpdateForSaveData(3, 0, 300);
      }
      if (Script_GetInputData(3, 1) == 0x200) {
        SAVE_Script_UpdateForSaveData(3, 0, 3000);
      }
      Script_PrintDebugString(3, 24, "FUKUSAN  frame = %d\012", SAVE_Script_UpdateForSaveData(4, 0));
      Script_PrintDebugString(3, 25, "KAITYUU nokori = %d\012", Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0));
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_3D4B(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = 0;
  local_F8 = 0;
  if (Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) > 0) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMFLAS, 1);
  } else {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMFLAS, 0);
  }
  if (Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) < 13) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCBAT, 1);
  }
  local_FC = Script_ScenDataUtil(&data_ED64, SCND_ReadU32, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1);
  if (task_2CC == 0) {
    if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) >= 0) {
      local_F4 = local_FC;
      switch (local_F4) {
        case 0x31554F52:
          if (*task_304 == 1) {
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
          } else {
            if (*task_304 == 0) {
              Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
              goto L003E16;
            }
          }
          L003E16:;
          break;
        case 0x32554F52:
          if (task_304[1] == 1) {
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
          } else {
            if (task_304[1] == 0) {
              Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
              goto L003E5B;
            }
          }
          L003E5B:;
          break;
        case 0x33554F52:
          if (task_304[2] == 1) {
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
          } else {
            if (task_304[2] == 0) {
              Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
              goto L003EA0;
            }
          }
          L003EA0:;
          break;
        case 0x34554F52:
          if (task_304[3] == 1) {
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
          } else {
            if (task_304[3] == 0) {
              Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
              goto L003EE5;
            }
          }
          L003EE5:;
          break;
      }
    }
    Script_EVT_GetPosition(SHEN_NAME('ROU1'), -1, &task_3CC, 0);
    if ((script_6302() == 1) & (task_420 == 0)) {
      if (*task_304 == 1) {
        local_FC = 0x31554F52;
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
      } else {
        if ((*task_304 == 0) & (task_420 == 0)) {
          local_FC = 0x31554F52;
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
          goto L003F55;
        }
      }
    }
    L003F55:;
    Script_EVT_GetPosition(SHEN_NAME('ROU2'), -1, &task_3CC, 0);
    if ((script_6302() == 1) & (task_420 == 0)) {
      if (task_304[1] == 1) {
        local_FC = 0x32554F52;
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
      } else {
        if ((task_304[1] == 0) & (task_420 == 0)) {
          local_FC = 0x32554F52;
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
          goto L003FC8;
        }
      }
    }
    L003FC8:;
    Script_EVT_GetPosition(SHEN_NAME('ROU3'), -1, &task_3CC, 0);
    if ((script_6302() == 1) & (task_420 == 0)) {
      if (task_304[2] == 1) {
        local_FC = 0x33554F52;
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
      } else {
        if ((task_304[2] == 0) & (task_420 == 0)) {
          local_FC = 0x33554F52;
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
          goto L00403B;
        }
      }
    }
    L00403B:;
    Script_EVT_GetPosition(SHEN_NAME('ROU4'), -1, &task_3CC, 0);
    if (script_6302() == 1) {
      if ((task_304[3] == 1) & (task_420 == 0)) {
        local_FC = 0x34554F52;
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
      } else {
        if ((task_304[3] == 0) & (task_420 == 0)) {
          local_FC = 0x34554F52;
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
          goto L0040AE;
        }
      }
    }
    L0040AE:;
    task_3CC.x = 3.994f;
    task_3CC.y = 0.0f;
    task_3CC.z = 3.999f;
    if (script_6302() == 1) {
      if (~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB) & -(task_2CC == 0)) {
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMLIGH, 1);
      }
    }
  }
  if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == 7) {
    if (~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB) & -(task_2CC == 0)) {
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMLIGH, 1);
    }
  }
  if (Script_ScenDataUtil(&data_ED64, SCND_ReadU32, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1) == 0x306B6E64) {
    if (~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMLIGH, 1);
    }
  }
  if (task_2D4 == 1) {
    task_3CC.x = *(float *)&task_278;
    task_3CC.y = *(float *)(&task_278 + 1);
    task_3CC.z = *(float *)(&task_278 + 2);
    if (Script_ScenDataUtil(&data_ED64, SCND_ReadU32, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1) == 0x594D4144) {
      if ((task_254 == 3) & (task_2CC == 1)) {
        local_F8 = 1;
      }
      if (task_254 == 2) {
        local_F8 = 1;
      }
    }
    if (script_6438() == 1) {
      if ((task_254 == 3) & (task_2CC == 1)) {
        local_F8 = 1;
      }
      if (task_254 == 2) {
        local_F8 = 1;
      }
    }
    if (task_2CC == 0) {
      if (script_63B5() == 1) {
        if ((task_254 == 3) | (task_254 == 2)) {
          local_F8 = 1;
        }
      }
      if (Script_ScenDataUtil(&data_ED64, SCND_ReadU32, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1) == 0x5F4F4E4F) {
        if ((task_254 == 3) | (task_254 == 2)) {
          local_F8 = 1;
        }
      }
    }
  }
  if (-(task_2F0 == 0) & Script_IsPlayerAllowed(0)) {
    if ((task_3E4 == 0) & (local_F8 > 0)) {
      SCNF_RequestIconNotification(0x100, 0, 0, 1);
      task_3E4 = 1;
    }
    if (-(task_3E4 == 0) & (-(task_328 == 1) & (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES) == 1) | -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES) == 1)))) {
      SCNF_RequestIconNotification(0x100, 0, 0, 1);
      task_3E4 = 1;
    }
  } else {
    task_3E4 = 0;
    SCNF_RequestIconNotification(0, 0, 0, 2);
  }
  if ((local_F8 == 0) & (task_328 == 0)) {
    if (task_3E4 == 1) {
      task_3E4 = 0;
      SCNF_RequestIconNotification(0, 0, 0, 2);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_F820, 1, data_F82A, 5, data_F852, 6, data_F85C, 16, data_F8C0)))
static void script_4348(int32_t arg0) {
  Vector3i local_F4;
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (arg0 == 0) {
    if ((arg0 == 2) & (task_2CC == 1)) {
      int32_t local_F0;
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
      Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_330 > 1) {
        task_330 = 0;
      }
      local_F0 = task_330;
      switch (local_F0) {
        case 0:
          Script_ShowUIMessageFromID("E1012A101");
          break;
        case 1:
          Script_ShowUIMessageFromID("E1012A103");
          break;
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
      task_330 = task_330 + 1;
      goto L00486A;
    }
    {
      int32_t local_F0;
      local_F0 = task_254;
      if (local_F0 == 3) {
        if (task_2CC == 0) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
          Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          if (task_330 > 2) {
            task_330 = 0;
          }
          local_F0 = task_330;
          switch (local_F0) {
            case 0:
              Script_ShowUIMessageFromID("E1012A167");
              break;
            case 1:
              Script_ShowUIMessageFromID("E1012A168");
              break;
            case 2:
              Script_ShowUIMessageFromID("E1012A169");
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        } else {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
          Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          if (task_330 > 1) {
            task_330 = 0;
          }
          local_F0 = task_330;
          switch (local_F0) {
            case 0:
              Script_ShowUIMessageFromID("E1012A101");
              break;
            case 1:
              Script_ShowUIMessageFromID("E1012A103");
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        task_330 = task_330 + 1;
      } else if (local_F0 == 12) {
      } else if (local_F0 == 6) {
      } else {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
        Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        if (task_330 > 13) {
          task_330 = 0;
        }
        local_F0 = task_330;
        switch (local_F0) {
          case 0:
            Script_ShowUIMessageFromID("E1012A092");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1012A093");
            break;
          case 2:
            Script_ShowUIMessageFromID("E1012A094");
            break;
          case 3:
            Script_ShowUIMessageFromID("E1012A095");
            break;
          case 4:
            Script_ShowUIMessageFromID("E1012A096");
            break;
          case 5:
            Script_ShowUIMessageFromID("E1012A097");
            break;
          case 6:
            Script_ShowUIMessageFromID("E1012A098");
            break;
          case 7:
            Script_ShowUIMessageFromID("E1012A099");
            break;
          case 8:
            Script_ShowUIMessageFromID("E1012A100");
            break;
          case 10:
            Script_ShowUIMessageFromID("E1012A103");
            break;
          case 11:
            Script_ShowUIMessageFromID("E1012A104");
            break;
          case 12:
            Script_ShowUIMessageFromID("E1012A105");
            break;
          case 13:
            Script_ShowUIMessageFromID("E1012A106");
            break;
          case 14:
            Script_ShowUIMessageFromID("E1012A107");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(RYO_RestoreControl);
        task_330 = task_330 + 1;
      }
    }
  } else {
    int32_t local_F0;
    local_F0 = task_254;
    if (local_F0 == 12) {
    } else if (local_F0 == 6) {
    } else {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
      Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_330 > 2) {
        task_330 = 0;
      }
      local_F0 = task_330;
      switch (local_F0) {
        case 0:
          Script_ShowUIMessageFromID("E1012A085");
          break;
        case 1:
          Script_ShowUIMessageFromID("E1012A084");
          break;
        case 2:
          Script_ShowUIMessageFromID("E1012A086");
          break;
      }
      task_330 = task_330 + 1;
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
    }
  }
  L00486A:;
  return;
}

__attribute__((scn_string_bindings(0, data_F940, 1, data_F94A, 2, data_F954, 12, data_F9B8, 13, data_F9C2, 14, data_F9CC, 15, data_F9D6, 20, data_FA08, 21, data_FA12, 22, data_FA1C, 23, data_FA26, 24, data_FA30, 25, data_FA3A, 26, data_FA44, 27, data_FA4E, 28, data_FA58, 29, data_FA62, 30, data_FA6C, 31, data_FA76, 32, data_FA80, 33, data_FA8A, 34, data_FA94, 35, data_FA9E, 36, data_FAA8, 37, data_FAB2, 38, data_FABC, 39, data_FAC6, 40, data_FAD0, 41, data_FADA, 42, data_FAE4, 43, data_FAEE, 44, data_FAF8, 45, data_FB02, 46, data_FB0C, 47, data_FB16, 48, data_FB20, 49, data_FB2A, 50, data_FB34, 51, data_FB3E, 52, data_FB48, 53, data_FB52, 54, data_FB5C, 55, data_FB66, 56, data_FB70, 57, data_FB7A, 58, data_FB84, 59, data_FB8E, 60, data_FB98, 61, data_FBA2, 62, data_FBAC, 63, data_FBB6, 64, data_FBC0, 65, data_FBCA, 66, data_FBD4, 67, data_FBDE, 68, data_FBE8, 69, data_FBF2, 70, data_FBFC, 71, data_FC06, 72, data_FC10, 73, data_FC1A, 74, data_FC24, 75, data_FC2E, 76, data_FC38, 85, data_FC92, 86, data_FC9C, 87, data_FCA6, 95, data_FCF6, 107, data_FD6E)))
static void script_486B(int32_t arg0) {
  int32_t unused_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  if (arg0 < 20) {
    if (task_338[arg0] == 1) {
      int32_t unused_D4[3];
      int32_t local_D0;
      local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      Script_ryoControl(PCCTL_DisablePlayerControl);
      local_D0 = local_E0;
      switch (local_D0) {
        case 0:
          Script_ShowUIMessageFromID("E1012A187");
          break;
        case 1:
          Script_ShowUIMessageFromID("E1012A188");
          break;
        case 2:
          Script_ShowUIMessageFromID("E1012A189");
          break;
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
    } else {
      int32_t unused_D4[3];
      int32_t local_D0;
      if (arg0 != 8) {
        if (task_448 == 8) {
          task_338[arg0] = 1;
        }
      }
      local_D0 = arg0;
      switch (local_D0) {
        case 3:
          if (task_448 == 8) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ShowUIMessageFromID("E1012A019");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
          break;
        case 4:
          if (task_448 == 8) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_ShowUIMessageFromID("E1012A057");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ShowUIMessageFromID("E1012A058");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
          break;
        case 5:
          if (task_448 == 8) {
            Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
            Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
            Script_motiSetAngFlags(CHARA_AKIR, 0);
            Script_PlayCharaMotion(CHARA_AKIR, 4126, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            for (;;) {
              if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
                break;
              }
              Script_RotateNodeUtils(CHARA_AKIR, &data_EC88, 0);
              SCNF_WaitFrames_0000(1);
            }
            Script_motiSetAngFlags(CHARA_AKIR, 1);
            Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
            Script_executeAicaCommand(198057, 0, 0);
            Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
            Script_PlayCharaMotion(CHARA_AKIR, 4125, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            for (;;) {
              if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
                break;
              }
              Script_RotateNodeUtils(CHARA_AKIR, &data_EC88, 0);
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
            Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
          }
          break;
        case 6:
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
          Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
          Script_motiSetAngFlags(CHARA_AKIR, 0);
          Script_PlayCharaMotion(CHARA_AKIR, 4126, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          for (;;) {
            if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
              break;
            }
            Script_RotateNodeUtils(CHARA_AKIR, &data_EC88, 0);
            SCNF_WaitFrames_0000(1);
          }
          Script_motiSetAngFlags(CHARA_AKIR, 1);
          Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
          Script_executeAicaCommand(198057, 0, 0);
          Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
          Script_PlayCharaMotion(CHARA_AKIR, 4125, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          for (;;) {
            if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
              break;
            }
            Script_RotateNodeUtils(CHARA_AKIR, &data_EC88, 0);
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
          Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
          break;
        case 7:
          if (task_448 == 8) {
            if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB) == 0) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              Script_ShowUIMessageFromID("E1012A236");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ryoControl(RYO_RestoreControl);
            }
          }
          break;
        case 8:
          if (task_448 == 8) {
            if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
              local_F0.x = 0.6872f;
              (&local_F0)->y = 0.0f;
              (&local_F0)->z = 3.3065f;
              local_E4.x = 0;
              (&local_E4)->y = 0x4000;
              (&local_E4)->z = 0;
              Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
              Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E4, 0);
              script_CED4();
            } else {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              local_D0 = *(&task_3A4 + 5);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A256");
                  *(&task_3A4 + 5) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A257");
                  *(&task_3A4 + 5) = 0;
                  break;
              }
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ryoControl(RYO_RestoreControl);
            }
          }
          break;
        case 12:
          if (task_448 == 8) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_ShowUIMessageFromID("E1012A061");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ShowUIMessageFromID("E1012A062");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ShowUIMessageFromID("E1012A110");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
          break;
        case 14:
          if (task_448 == 8) {
            local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
            Script_ryoControl(PCCTL_DisablePlayerControl);
            local_D0 = local_E0;
            switch (local_D0) {
              case 0:
                Script_ShowUIMessageFromID("E1012A187");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1012A188");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1012A189");
                break;
            }
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
          break;
        case 13:
          if ((task_448 == 8) & (task_2D4 == 0)) {
            if (task_334 == 0) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              local_F0.x = 6.8943f;
              (&local_F0)->y = 0.0f;
              (&local_F0)->z = 1.2971f;
              Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
              local_E4.x = 0;
              (&local_E4)->y = 47695;
              (&local_E4)->z = 0;
              Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E4, 0);
              Script_srchUtils(SRCH_SCRIPT_Reset);
              Script_ShowUIMessageFromID("E1012A040");
              Script_seqEnterEvtCam(4000, 0, AUTH_None);
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ShowUIMessageFromID("E1012A182");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_EVT_GetPosition(SHEN_NAME('FUDA'), -1, &local_F0, 0);
              Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F0, 1);
              Script_seqEnterEvtCam(4001, 0, AUTH_None);
              Script_ShowUIMessageFromID("E1012A183");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_lightSetSlotEnabled(28, 0);
              SCNF_WaitFrames_0000(15);
              Script_SetCameraFieldOfView(10922);
              Script_ryoControl(RYO_RestoreControl);
              Script_SetGameStateTo(AUTH_None);
              Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
              task_334 = 1;
            } else {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              Script_srchUtils(SRCH_SCRIPT_Reset);
              Script_seqEnterEvtCam(4000, 0, AUTH_None);
              if (task_334 == 1) {
                Script_ShowUIMessageFromID("E1012A184");
                task_334 = 2;
              } else {
                Script_ShowUIMessageFromID("E1012A185");
                task_334 = 1;
              }
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              SCNF_WaitFrames_0000(150);
              Script_ryoControl(RYO_RestoreControl);
              Script_SetCameraFieldOfView(10922);
              Script_SetGameStateTo(AUTH_None);
            }
          }
          break;
      }
    }
  } else {
    int32_t unused_D4[3];
    int32_t local_D0;
    local_D0 = arg0;
    switch (local_D0) {
      case 0x4F4B4148:
        if (task_448 == 8) {
          if ((Script_GetOwnedItemCount(8, 0) == 0) & (task_2CC == 0)) {
            Vector3i local_C4;
            script_9635();
            if (task_428 == 1) {
              Script_lightSetSlotEnabled(28, 0);
            }
            Script_lightSetSlotEnabled(15, 0);
            Script_srchUtils(SRCH_SCRIPT_Reset);
            Script_SetGameStateTo(AUTH_None);
            Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_F910);
            (&local_C4)->y = 0x8000;
            Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_C4, 0);
            Script_seqEnterEvtCam(2100, 0, AUTH_None);
            script_B22C(SHEN_NAME('FUTA'));
            script_B432();
            Script_GetCassetteFlags(8, 1, 0, 0);
            SCNF_StartIconSystemMenu(8);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_OLD_PHOTO_OF_DAD, 1);
            Script_charaDisplay(SHEN_NAME('SHAS'), CDM_Hide);
            Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_F91C);
            Script_fixoClearAttachment(SHEN_NAME('SHAS'));
            Script_seqEnterEvtCam(2101, 0, AUTH_None);
            script_B3C0(arg0);
            script_96C5();
            local_F0.x = 6.07f;
            (&local_F0)->y = 0.877f;
            (&local_F0)->z = 8.1409f;
            local_E4.x = 0;
            (&local_E4)->y = 1195;
            (&local_E4)->z = 0;
            Script_EVT_SetRotation(SHEN_NAME('FUTA'), 0x38000000, &local_E4, 0);
            Script_EVT_SetPosition(SHEN_NAME('FUTA'), POS_XYZ, &local_F0);
            Script_charaDisplay(CHARA_ONO_, CDM_Show);
          } else {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
            local_D0 = local_E0;
            switch (local_D0) {
              case 0:
                Script_ShowUIMessageFromID("E1012A187");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1012A188");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1012A189");
                break;
            }
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
        }
        break;
      case 0x31554F52:
        if (task_448 == 8) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          local_D0 = *task_304;
          switch (local_D0) {
            case 0:
              local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f));
              local_D0 = *&task_3A4;
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A194");
                  task_3A4 = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A198");
                  task_3A4 = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A187");
                  task_3A4 = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A188");
                  task_3A4 = 4;
                  break;
                case 4:
                  Script_ShowUIMessageFromID("E1012A189");
                  task_3A4 = 2;
                  break;
              }
              break;
            case 1:
              local_D0 = *&task_3A4;
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A195");
                  task_3A4 = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A187");
                  task_3A4 = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A188");
                  task_3A4 = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A189");
                  task_3A4 = 1;
                  break;
              }
              break;
            case 2:
              if ((*(int16_t *)task_2FC < 0x4B00) & (*(int16_t *)task_2FC > 9600)) {
                Script_ShowUIMessageFromID("E1012A197");
              }
              if ((*(int16_t *)task_2FC < 9600) & (*(int16_t *)task_2FC > 4800)) {
                Script_ShowUIMessageFromID("E1012A193");
              }
              if (*(int16_t *)task_2FC < 4800) {
                Script_ShowUIMessageFromID("E1012A191");
              }
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x32554F52:
        if (task_448 == 8) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          local_D0 = *task_304;
          switch (local_D0) {
            case 0:
              local_D0 = *(&task_3A4 + 1);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A194");
                  *(&task_3A4 + 1) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A198");
                  *(&task_3A4 + 1) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 1) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 1) = 4;
                  break;
                case 4:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 1) = 2;
                  break;
              }
              break;
            case 1:
              local_D0 = *(&task_3A4 + 1);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A195");
                  *(&task_3A4 + 1) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 1) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 1) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 1) = 1;
                  break;
              }
              break;
            case 2:
              if ((*(int16_t *)((int8_t *)task_2FC + 2) < 0x4B00) & (*(int16_t *)((int8_t *)task_2FC + 2) > 9600)) {
                Script_ShowUIMessageFromID("E1012A197");
              }
              if ((*(int16_t *)((int8_t *)task_2FC + 2) < 9600) & (*(int16_t *)((int8_t *)task_2FC + 2) > 4800)) {
                Script_ShowUIMessageFromID("E1012A193");
              }
              if (*(int16_t *)((int8_t *)task_2FC + 2) < 4800) {
                Script_ShowUIMessageFromID("E1012A191");
              }
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x33554F52:
        if (task_448 == 8) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          local_D0 = task_304[2];
          switch (local_D0) {
            case 0:
              local_D0 = *(&task_3A4 + 2);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A194");
                  *(&task_3A4 + 2) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A198");
                  *(&task_3A4 + 2) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 2) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 2) = 4;
                  break;
                case 4:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 2) = 2;
                  break;
              }
              break;
            case 1:
              local_D0 = *(&task_3A4 + 2);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A195");
                  *(&task_3A4 + 2) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 2) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 2) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 2) = 1;
                  break;
              }
              break;
            case 2:
              if ((*(int16_t *)((int8_t *)task_2FC + 4) < 0x4B00) & (*(int16_t *)((int8_t *)task_2FC + 4) > 9600)) {
                Script_ShowUIMessageFromID("E1012A197");
              }
              if ((*(int16_t *)((int8_t *)task_2FC + 4) < 9600) & (*(int16_t *)((int8_t *)task_2FC + 4) > 4800)) {
                Script_ShowUIMessageFromID("E1012A193");
              }
              if (*(int16_t *)((int8_t *)task_2FC + 4) < 4800) {
                Script_ShowUIMessageFromID("E1012A191");
              }
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x34554F52:
        if (task_448 == 8) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          local_D0 = task_304[3];
          switch (local_D0) {
            case 0:
              local_D0 = *(&task_3A4 + 3);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A194");
                  *(&task_3A4 + 3) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A198");
                  *(&task_3A4 + 3) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 3) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 3) = 4;
                  break;
                case 4:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 3) = 2;
                  break;
              }
              break;
            case 1:
              local_D0 = *(&task_3A4 + 3);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A195");
                  *(&task_3A4 + 3) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 3) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 3) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 3) = 1;
                  break;
              }
              break;
            case 2:
              if ((*(int16_t *)((int8_t *)task_2FC + 6) < 0x4B00) & (*(int16_t *)((int8_t *)task_2FC + 6) > 9600)) {
                Script_ShowUIMessageFromID("E1012A197");
              }
              if ((*(int16_t *)((int8_t *)task_2FC + 6) < 9600) & (*(int16_t *)((int8_t *)task_2FC + 6) > 4800)) {
                Script_ShowUIMessageFromID("E1012A193");
              }
              if (*(int16_t *)((int8_t *)task_2FC + 6) < 4800) {
                Script_ShowUIMessageFromID("E1012A191");
              }
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x42345842:
        if (task_448 == 8) {
          if ((task_38C == 0) & (task_2CC == 0)) {
            if (task_428 == 1) {
              Script_lightSetSlotEnabled(28, 0);
            }
            Script_srchUtils(SRCH_SCRIPT_Reset);
            Script_SetGameStateTo(AUTH_None);
            script_C30F();
            task_38C = 1;
          } else {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
            local_D0 = local_E0;
            switch (local_D0) {
              case 0:
                Script_ShowUIMessageFromID("E1012A187");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1012A188");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1012A189");
                break;
            }
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
        }
        break;
      case 0x574E4544:
        if (task_448 == 8) {
          if ((task_388 == 0) & (task_2CC == 0)) {
            script_BE52();
            task_388 = 1;
          } else {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
            local_D0 = local_E0;
            switch (local_D0) {
              case 0:
                Script_ShowUIMessageFromID("E1012A187");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1012A188");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1012A189");
                break;
            }
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
        }
        break;
      case 0x414B4153:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A138");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A139");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (task_448 == 3) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_ShowUIMessageFromID("E1012A140");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x53494E4B:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A131");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x59454B48:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A137");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x454E414B:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A141");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (task_448 == 2) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_ShowUIMessageFromID("E1012A142");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x414B554D:
        if (((task_448 == 8) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES) == 1)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES) == 1)) {
          if (Script_GetControllerInput(1) == 0x400) {
            if (task_428 == 1) {
              Script_lightSetSlotEnabled(28, 0);
            }
            SCNF_RequestIconNotification(0, 0, 0, 2);
            task_2D4 = 1;
            Script_srchUtils(SRCH_SCRIPT_Reset);
            Script_SetGameStateTo(AUTH_None);
            script_CA6B();
          }
        }
        break;
      case 0x41524954:
        if (task_448 == 8) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_ShowUIMessageFromID("E1012A268");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A295");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x32425554:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A144");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x33425554:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A144");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x494B414D:
        if (task_448 == 2) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_ShowUIMessageFromID("E1012A079");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A080");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_BOOK_FOR_MASTER_CHEN, 1);
        }
        break;
      case 0x324B4159:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A180");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A298");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (!(task_448 == 2)) goto L005F0F;
        L005F0F:;
        break;
      case 0x554B4159:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A290");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A293");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A294");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A295");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (!(task_448 == 2)) goto L005FAA;
        L005FAA:;
        break;
      case 0x30544E42:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A254");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x4954414D:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A153");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (task_448 == 2) {
          Script_GetCassetteFlags(55, 3, 0, 0);
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 836, 1);
        }
        break;
      case 0x4B4F4F42:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A299");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A300");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (task_448 == 2) {
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_BOOK_FOR_MASTER_CHEN, 1);
        }
        break;
      case 0x4C4C4142:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A274");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A275");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x33534E54:
        if (task_448 == 5) {
          if (Script_GetOwnedItemCount(0, 0) == 0) {
            Script_ShowUIMessageFromID("E1012A158");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
          }
        }
        break;
      case 0x5A414B4B:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A159");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A296");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A297");
        }
        if (!(task_448 == 2)) goto L0061A7;
        L0061A7:;
        break;
      case 0x4B53554B:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A164");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x554F5249:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A135");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (task_448 == 1) {
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 844, 1);
        }
        break;
      case 0x53415A53:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A150");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x30534E54:
        break;
      case 0x41445546:
        Script_ryoControl(PCCTL_DisablePlayerControl);
        local_D0 = *(&task_3A4 + 6);
        switch (local_D0) {
          case 0:
            Script_ShowUIMessageFromID("E1012A259");
            *(&task_3A4 + 6) = 1;
            break;
          case 1:
            Script_ShowUIMessageFromID("E1012A260");
            *(&task_3A4 + 6) = 0;
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(RYO_RestoreControl);
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_6302(void) {
  Vector3 local_F4;
  int32_t unused_EC[2];
  int32_t local_E4[2];
  float local_E0;
  int32_t local_DC;
  local_DC = 0;
  if (task_2F0 == 0) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    task_3CC.y = (&local_F4)->y;
    local_E0 = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &task_3CC);
    if (!(local_E0 >= 1.8f)) {
      local_E4[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&task_3CC)->x - (&local_F4)->x, task_3CC.z - (&local_F4)->z);
      Script_mtwkGetSetRotation(CHARA_AKIR, &task_290, 0x2000000);
      if ((local_E4[1] + 0x3000 > task_290.y) & (local_E4[1] - 0x3000 < task_290.y)) {
        local_DC = 1;
      }
    }
  }
  return local_DC;
}

static int32_t script_63B5(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  local_E8.x = 0.3531f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 5.4435f;
  local_D8 = 0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x42000000);
  if (Script_CheckTargetWithinAngleRange(&local_E8, 1.5f, 0xC000, 12743, &local_F4)) {
    if (Script_CheckTargetWithinAngleRange(&local_F4, 1.4f, (&local_DC)->y, 7281, &local_E8)) {
      local_D8 = 1;
    }
  }
  return local_D8;
}

static int32_t script_6438(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  local_E8.x = 6.5205f;
  (&local_E8)->y = 0.2f;
  (&local_E8)->z = 0.0196f;
  local_D8 = 0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x42000000);
  if (Script_CheckTargetWithinAngleRange(&local_E8, 1.5f, 0x8000, 12743, &local_F4)) {
    if (Script_CheckTargetWithinAngleRange(&local_F4, 1.4f, (&local_DC)->y, 7281, &local_E8)) {
      local_D8 = 1;
    }
  }
  return local_D8;
}

static int32_t script_64BE(void) {
  for (;;) {
    Script_GetCameraInputDetails(15);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_64D3(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  task_650 = 0;
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  if (Script_GetOwnedItemCount(51, 0) == 0) {
    Script_scenCreateTask(script_809F, 0);
  }
  if (Script_GetCurrentEntryPoint() == 5) {
    local_F8 = 0;
    Script_GetCassetteFlags(55, 10, 0, 0);
    Script_GetCassetteFlags(54, 10, 0, 0);
    if (Script_GetOwnedItemCount(51, 0) == 0) {
      Script_GetCassetteFlags(51, 1, 0, 0);
    }
    Script_GetCassetteFlags(56, 1, 0, 0);
  } else {
    local_F8 = Script_GetCurrentEntryPoint();
  }
  SAVE_Script_UpdateForSaveData(0, 0);
  if (Script_GetCurrentEntryPoint() == 3) {
    SAVE_Script_UpdateForSaveData(3, 0, 300);
  } else {
    SAVE_Script_UpdateForSaveData(3, 0, (8 - local_F8 * 3) * 1800);
  }
  for (;;) {
    if (Script_GetOwnedItemCount(5, 0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_FC = 0;
  for (;;) {
    if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21) == 3) {
      break;
    }
    for (;;) {
      if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21) == 2) {
        if (task_2D4 == 1) {
          if (Script_mapEvUtils(0x4D450002, CHARA_AKIR) != 100) {
            local_FC = SAVE_Script_UpdateForSaveData(4, 0);
          }
        }
      } else {
        if (Script_mapEvUtils(0x4D450002, CHARA_AKIR) != 100) {
          local_FC = SAVE_Script_UpdateForSaveData(4, 0);
        }
      }
      SCNF_WaitFrames_0000(1);
      if (-(local_FC < 0) & Script_IsPlayerAllowed()) {
        if (Script_SetTaskDataValue(CHARA_AKIR, &data_FD98, &data_FDA4, 0)) {
          Script_mtwkGetSetRotation(CHARA_AKIR, &task_3F4, 0);
          if ((task_3F4.y > 0x9B00) & (task_3F4.y < 0xE500)) {
            if (task_2F0 == 0) {
              if ((task_420 == 0) & (task_250 == 0)) {
                break;
              }
            }
          }
        }
      }
      if (task_650 == 1) {
        script_7CED();
      }
    }
    local_F4 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21);
    if ((local_F4 == 0) | (local_F4 == 1)) {
      if (-(task_2D4 == 0) & (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES) == 0))) {
        (void)((task_250 = -1) & 0x80);
        Script_charaDisplay(CHARA_FUKU, CDM_Show);
        script_71A6();
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 2);
        SAVE_Script_UpdateForSaveData(3, 0, (8 - local_F8 * 3) * 1800);
        local_FC = 0;
        (void)((task_250 = 0) & 0x80);
        Script_sprintf(data_FDB0);
      } else {
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 2);
      }
    } else {
      switch (local_F4) {
        case 2:
          if (task_254 == 2) {
            if (-(task_2D4 == 1) & -(task_2CC == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_WALL_DIFFERENT_COLOR) == 0)) {
              (void)((task_250 = -1) & 0x80);
              Script_charaDisplay(CHARA_FUKU, CDM_Show);
              script_7796();
              Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 3);
              (void)((task_250 = 0) & 0x80);
              goto L0067C6;
            }
            L0067C6:;
          } else {
            Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 3);
          }
          break;
        case 3:
          goto L0067E8;
          break;
      }
    }
  }
  L0067E8:;
}

static void script_67E9(void) {
  Script_seqEnterEvtCam(5000, 0, AUTH_None);
  SCNF_WaitFrames_0000(125);
  Script_seqEnterEvtCam(5001, 0, AUTH_None);
  SCNF_WaitFrames_0000(220);
  Script_seqEnterEvtCam(5002, 0, AUTH_None);
  SCNF_WaitFrames_0000(60);
  Script_seqEnterEvtCam(5003, 0, AUTH_None);
  SCNF_WaitFrames_0000(120);
  Script_seqEnterEvtCam(5004, 0, AUTH_None);
  return;
}

__attribute__((scn_string_bindings(2, data_FE08, 3, data_FE12, 4, data_FE1C, 5, data_FE26, 6, data_FE30)))
static void script_6844(void) {
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
      Script_EVT_GetPosition(CHARA_AKIR, 4, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_FUKU, 12, &local_E8, 1);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
      Script_EVT_GetPosition(CHARA_FUKU, 4, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 12, &local_E8, 1);
    }
  }
  SCNF_WaitFrames_0000(1);
  {
    int32_t unused_F4[3];
    int32_t local_F0;
    local_F0 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21);
    if ((local_F0 == 0) | (local_F0 == 1)) {
      Script_ShowUIMessageFromID("E1012B065");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1012B266");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1012A230");
      task_32C = 1;
    } else {
      if (local_F0 == 2) {
        Script_ShowUIMessageFromID("E1012B059");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID("E1012B060");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID("E1012A250");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID("E1012A251");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID(data_FE3A);
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        goto L0069DD;
      }
    }
  }
  L0069DD:;
  Script_SetCharaLookAtPosition(CHARA_FUKU, -1, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_charaToggleMotEnabled(CHARA_FUKU, 1);
  SCNF_WaitFrames_0000(30);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TURN180_WAD_R, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(25);
  {
    int32_t unused_F0[4];
    Vector3i local_E4;
    (&local_E4)->y = 0x8000;
    Script_EVT_SetRotation(CHARA_FUKU, -0x70000000, &local_E4, 0);
  }
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_WALK_UP1_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(30);
  Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_FDD0);
  return;
}

__attribute__((scn_string_bindings(1, data_FE9E)))
static void script_6AA7(void) {
  int32_t unused_F4[3];
  int32_t local_E8[3];
  int32_t local_DC[3];
  Vector3 local_D0;
  local_E8[0] = 0x3F266666;
  local_E8[1] = 0;
  local_E8[2] = 0x40628F5C;
  local_DC[0] = 0;
  local_DC[1] = 48059;
  local_DC[2] = 0;
  Script_ccowUtils(CHARA_FUKU, CCOW_DisableAll);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_654 = Script_scenCreateTask(script_64BE, 0);
  script_0B9C(6100, CHARA_AKIR, CHARA_AKIR, -1);
  script_0B9C(6101, -1, -1, 135);
  script_0792(CHARA_AKIR, -1, 0x4091EB85, 0, 0x406C28F6, -1);
  script_0893(CHARA_AKIR, -1, 0x43610000, -1);
  script_0792(CHARA_AKIR, -1, 0x3FD851EC, 0, 0x4098A3D7, 135);
  script_0893(CHARA_AKIR, -1, 0x42200000, 135);
  script_0792(CHARA_FUKU, -1, 0x3F266666, 0, 0x40628F5C, -1);
  script_0893(CHARA_FUKU, -1, 0x43848000, -1);
  Script_ShowUIMessageFromID("E1012B038");
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_D0, 1);
  }
  SCNF_WaitFrames_0000(30);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ShowUIMessageFromID("E1012A242");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_FUKU, 4142, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_ShowUIMessageFromID("E1012B002");
  Script_FaceEx(CHARA_FUKU, FACE_ExpressionEnabled, 5, 4);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_D0, 1);
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012B003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A212");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_FUKU, 12, &local_D0, 1);
  }
  Script_ShowUIMessageFromID("E1012B004");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID(data_FED0);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_FUKU, -1, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCPAMFlags(CHARA_FUKU, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FUKU_TIPS, 1);
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_srchControl((void *)task_654);
  return;
}

__attribute__((scn_string_bindings(1, data_FF5E)))
static void script_6E68(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  script_9635();
  local_F4.x = 0.6543f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 3.5454f;
  local_E8.x = 0;
  (&local_E8)->y = 48059;
  (&local_E8)->z = 0;
  SCNF_RequestIconNotification(0, 0, 0, 2);
  task_654 = Script_scenCreateTask(script_64BE, 0);
  SCNF_WaitFrames_0000(1);
  Script_ccowUtils(CHARA_FUKU, CCOW_DisableAll);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_motmToggleFlags(CHARA_FUKU, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_E8, 0);
  SCNF_WaitFrames_0000(1);
  Script_ShowUIMessageFromID("E1012B067");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmToggleFlags(CHARA_FUKU, SCEN_Enable, 0x40);
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  Script_seqEnterEvtCam(6100, CHARA_AKIR, CHARA_AKIR);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_D0, 1);
  }
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_DC, 11832, MN_AKI_AKI_STAND_L, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
    Script_GetCameraInputDetails(15);
  }
  Script_ShowUIMessageFromID("E1012A242");
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(6101, 0, AUTH_None);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_FEE8);
  SCNF_WaitFrames_0000(1);
  Script_ShowUIMessageFromID("E1012B068");
  Script_FaceEx(CHARA_FUKU, FACE_ExpressionEnabled, 5, 4);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_D0, 1);
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_FUKU, 12, &local_D0, 1);
  }
  Script_ShowUIMessageFromID("E1012A243");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_FUKU, -1, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchControl((void *)task_654);
  return;
}

static void script_71A6(void) {
  int32_t local_FC;
  script_9635();
  script_D9A7(CHARA_AKIR, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 1);
  script_6AA7();
  task_660 = 0;
  for (;;) {
    if (task_420 == 0) {
      local_FC = task_660;
      switch (local_FC) {
        case 0:
          script_722A();
          task_660 = 1;
          break;
        case 1:
          script_6844();
          task_660 = 2;
          goto L007219;
          break;
      }
    }
  }
  L007219:;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES, 1);
  script_96C5();
  return;
}

__attribute__((scn_string_bindings(0, data_FFB4, 3, data_FFD2)))
static void script_722A(void) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_E4[3];
  task_654 = Script_scenCreateTask(script_64BE, 0);
  local_FC = Script_scenCreateTask(script_756B, 1, 60);
  Script_FaceLookAtTargetUtils(CHARA_FUKU, FACE_LookReset);
  script_0B9C(6103, -1, -1, 60);
  script_0B9C(6104, -1, -1, 130);
  script_0B9C(6105, -1, -1, 310);
  script_0B9C(6106, -1, -1, 480);
  script_0B9C(6107, -1, -1, 570);
  script_9904(CHARA_AKIR, 11, (int32_t)&data_FFA8, 130, 820);
  local_E4[0] = 0;
  local_E4[1] = 57708;
  local_E4[2] = 0;
  local_E4[1] = Script_EVT_SetRotation(CHARA_FUKU, 0x14000000, (const Vector3i *)local_E4, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(CHARA_FUKU)) {
      Script_EVT_SetRotation(CHARA_FUKU, MATRIX_XFORM_Y, (const Vector3i *)local_E4, 0);
      Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    }
  }
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_WALK_SLOW_K_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_FUKU, &data_FF7C, 0, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_FUKU, 0.05f) != -1) {
    Script_GetCameraInputDetails(15);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
      Script_EVT_GetPosition(CHARA_FUKU, 4, (Vector3 *)local_E4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 12, (const Vector3 *)local_E4, 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(CHARA_FUKU, MOTM_BLEND_FRAMES_30);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ShowUIMessageFromID("E1012B051");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_PlayCharaMotion(CHARA_FUKU, 4138, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(60);
  Script_ShowUIMessageFromID("E1012B052");
  SCNF_WaitFrames_0000(10);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_FF90);
  SCNF_WaitFrames_0000(1);
  local_F0.x = 4.0274f;
  (&local_F0)->y = -0.5f;
  (&local_F0)->z = -0.2809f;
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &data_FF9C, 4551, MN_AKI_AKI_STAND_LP, 0);
  L0074E4:;
  SCNF_WaitFrames_0000(1);
  if (!(Script_IsMoveTargetRequestActive(CHARA_AKIR) == 0)) goto L0074E4;
  Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F0, 1);
  SCNF_WaitFrames_0000(30);
  Script_ShowUIMessageFromID("E1012A245");
  SCNF_WaitFrames_0000(150);
  Script_ShowUIMessageFromID("E1012A246");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(110);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)task_654);
  return;
}

static int32_t script_756B(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  if (arg0 >= 0) {
    SCNF_WaitFrames_0000(arg0);
  }
  Script_lightSetSlotEnabled(15, 0);
  Script_fixoAttachEx(SHEN_NAME('LIGH'), CHARA_AKIR, 12, &data_FFDC, &data_FFE8);
  for (;;) {
    Script_EVT_GetPosition(SHEN_NAME('LIGH'), -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(SHEN_NAME('LIGH'), &local_DC, 0x40000000);
    local_F4.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->x);
    (&local_F4)->y = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y);
    (&local_F4)->z = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->z);
    local_F4.x = 1.0f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.0f;
    Script_lightSpotLightEnable(28, &local_E8, &local_F4);
    Script_LightUtils(28, 2, &data_F680);
    Script_LightUtils(28, 4, 2.1f);
    Script_LightUtils(28, 5, 2.8f);
    Script_LightUtils(28, 6, data_F690);
    Script_LightUtils(28, 10, 7281);
    Script_lightSetSlotEnabled(28, 1);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_767B(void) {
  Script_seqEnterEvtCam(3201, 0, AUTH_None);
  SCNF_WaitFrames_0000(225);
  Script_seqEnterEvtCam(3202, 0, AUTH_None);
  SCNF_WaitFrames_0000(275);
  Script_seqEnterEvtCam(3203, 0, AUTH_None);
  SCNF_WaitFrames_0000(175);
  Script_seqEnterEvtCam(3204, 0, AUTH_None);
  SCNF_WaitFrames_0000(275);
  return;
}

__attribute__((scn_string_bindings(0, data_FFF4)))
static int32_t script_76CE(void) {
  Vector3 local_F4;
  Script_seqEnterEvtCam(6100, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(135);
  Script_seqEnterEvtCam(6116, 0, AUTH_None);
  SCNF_WaitFrames_0000(350);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
  }
  Script_seqEnterEvtCam(6111, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1012B051");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID(data_FFFE);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  end:;
}

static void script_7796(void) {
  int32_t local_FC;
  script_9635();
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  SCNF_WaitFrames_0000(1);
  script_D9A7(CHARA_AKIR, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FUKU_TIPS)) {
    script_6E68();
  } else {
    Script_scenCreateTask(script_76CE, 0);
    script_6AA7();
  }
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  task_660 = 0;
  for (;;) {
    if (task_420 == 0) {
      local_FC = task_660;
      if (local_FC == 0) {
        script_7855();
        task_660 = 1;
        break;
        goto L00782D;
      }
    }
    L00782D:;
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_WALL_DIFFERENT_COLOR, 1);
  Script_SetCPAMFlags(CHARA_FUKU, 1);
  script_96C5();
  return;
}

__attribute__((scn_string_bindings(1, data_1006E, 6, data_100A0, 7, data_100AA, 8, data_100B4, 9, data_100BE)))
static void script_7855(void) {
  Vector3 local_F4;
  task_654 = Script_scenCreateTask(script_64BE, 0);
  Script_FaceLookAtTargetUtils(CHARA_FUKU, FACE_LookReset);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES)) {
    Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_10040);
    Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &data_1004C, 0);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
      Script_EVT_GetPosition(CHARA_FUKU, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
    }
    Script_seqEnterEvtCam(6111, 0, AUTH_None);
    Script_ShowUIMessageFromID("E1012B056");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1012A230");
    Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
    Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_10040);
    Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &data_1004C, 0);
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
    Script_ccowUtils(CHARA_FUKU, CCOW_DisableAll);
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_1001C);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10028, 0);
    Script_PlayCharaMotion(CHARA_FUKU, 4155, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(CHARA_AKIR, 4152, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(CHARA_FUKU, 1);
    Script_motmToggleFlags(CHARA_FUKU, SCEN_Disable, 8);
    SCNF_WaitFrames_0000(1);
    Script_seqEnterEvtCam(6115, 0, AUTH_None);
    SCNF_WaitFrames_0000(120);
    Script_seqEnterEvtCam(6112, 0, AUTH_None);
  } else {
    SCNF_WaitFrames_0000(100);
    Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_10040);
    Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &data_1004C, 0);
    SCNF_WaitFrames_0000(1);
    Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
    Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
    Script_ccowUtils(CHARA_FUKU, CCOW_DisableAll);
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_1001C);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10028, 0);
    Script_PlayCharaMotion(CHARA_FUKU, 4155, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(CHARA_AKIR, 4152, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(CHARA_FUKU, 1);
    SCNF_WaitFrames_0000(1);
    Script_seqEnterEvtCam(6115, 0, AUTH_None);
    Script_motmToggleFlags(CHARA_FUKU, SCEN_Disable, 8);
    SCNF_WaitFrames_0000(120);
  }
  Script_ShowUIMessageFromID("E1012B057");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A248");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012B058");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A249");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(6113, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1012B059");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
  }
  Script_ShowUIMessageFromID("E1012A250");
  Script_seqEnterEvtCam(6114, 0, AUTH_None);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012B060");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A251");
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012B061");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_FUKU) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_10058);
  Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  return;
}

static void script_7CED(void) {
  Vector3 local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A174");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_100D4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_1011C, 0);
  Script_charaDisplay(CHARA_FUKU, CDM_Show);
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_100EC);
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &data_10128, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1014C);
  Script_charaSetRefboardFlagTo(CHARA_FUKU, 1);
  Script_SetCharaRefBoardPos(CHARA_FUKU, &data_1014C);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Show);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_10110);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_10140, 0);
  Script_EVT_SetPosition(SHEN_NAME('YKUL'), POS_XYZ, &data_10104);
  Script_EVT_SetRotation(SHEN_NAME('YKUL'), 0x38000000, &data_10134, 0);
  Script_seqEnterEvtCam(9000, 0, AUTH_None);
  SCNF_WaitFrames_0000(1);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_FUKU, 12, &local_F4, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
  }
  Script_ShowUIMessageFromID("E1012B036");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_100E0);
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_100F8);
  Script_seqEnterEvtCam(9001, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1012A232");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_100EC);
  Script_seqEnterEvtCam(9002, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1012B037");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(9003, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1012A233");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  task_658 = 10;
  task_650 = 0;
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_250 = -1) & 0x20);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  task_2E4 = 0;
  SCNF_WaitFrames_0000(1);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  script_E05B();
  script_D28B(2, 0x4F4D4F4A, 0);
  Script_shenRequestGameModuleTransition(2, SHEN_NAME('JHD0'), 6);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_809F(void) {
  int32_t local_FC;
  local_FC = 0;
  while ((Script_GetOwnedItemCount(51, 0) == 0) & (task_658 < 2)) {
    if (local_FC > 600) {
      script_7CED();
    }
    if (Script_IsPlayerAllowed()) {
      local_FC = local_FC + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_80EE(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int8_t local_F1;
  uint8_t unused_EF[2];
  int8_t local_EE;
  int8_t local_ED;
  int8_t local_EC;
  int8_t local_EB;
  int8_t local_EA;
  int8_t local_E9;
  int8_t local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_DD[9];
  uint8_t unused_DC;
  int32_t local_D8;
  Script_ReturnTimeDate(&local_F4);
  local_EE = (&local_F4)->year;
  local_ED = (&local_F4)->month;
  local_EC = (&local_F4)->day;
  local_F3 = 0;
  local_D8 = local_ED;
  switch (local_D8) {
    case 12:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_10190[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 1:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_1019C[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 2:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_101A8[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 3:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_101B4[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
  }
  local_F3 = 0;
  local_EB = 0;
  while (local_F3 < 9) {
    if (local_EC == local_DD[local_F3]) {
      local_EB = 1;
    }
    local_F3 = local_F3 + 1;
  }
  *(int8_t *)task_664 = Script_PlayerStatsUtils(PS_GetStepSlot, 0);
  *((int8_t *)task_664 + 1) = Script_PlayerStatsUtils(PS_GetStepSlot, 1);
  *((int8_t *)task_664 + 2) = Script_PlayerStatsUtils(PS_GetStepSlot, 2);
  *((int8_t *)task_664 + 3) = Script_PlayerStatsUtils(PS_GetStepSlot, 3);
  *((int8_t *)task_664 + 4) = Script_PlayerStatsUtils(PS_GetStepSlot, 4);
  *((int8_t *)task_664 + 5) = Script_PlayerStatsUtils(PS_GetStepSlot, 5);
  *((int8_t *)task_664 + 6) = Script_PlayerStatsUtils(PS_GetStepSlot, 6);
  *((int8_t *)task_664 + 7) = Script_PlayerStatsUtils(PS_GetStepSlot, 7);
  *((int8_t *)task_664 + 8) = Script_PlayerStatsUtils(PS_GetStepSlot, 8);
  *((int8_t *)task_664 + 9) = Script_PlayerStatsUtils(PS_GetStepSlot, 9);
  *((int8_t *)task_664 + 10) = Script_PlayerStatsUtils(PS_GetStepSlot, 10);
  *((int8_t *)task_664 + 11) = Script_PlayerStatsUtils(PS_GetStepSlot, 11);
  *((int8_t *)task_664 + 12) = Script_PlayerStatsUtils(PS_GetStepSlot, 12);
  *((int8_t *)task_664 + 13) = Script_PlayerStatsUtils(PS_GetStepSlot, 13);
  *((int8_t *)task_664 + 14) = Script_PlayerStatsUtils(PS_GetStepSlot, 14);
  *((int8_t *)task_664 + 15) = Script_PlayerStatsUtils(PS_GetStepSlot, 15);
  if (*(int8_t *)task_664 == 0) {
    *(int8_t *)task_664 = 8;
    Script_PlayerStatsUtils(PS_SetStepSlot, 0, *(int8_t *)task_664);
    *((int8_t *)task_664 + 1) = 4;
    Script_PlayerStatsUtils(PS_SetStepSlot, 1, *((int8_t *)task_664 + 1));
    *((int8_t *)task_664 + 2) = 7;
    Script_PlayerStatsUtils(PS_SetStepSlot, 2, *((int8_t *)task_664 + 2));
    *((int8_t *)task_664 + 3) = 9;
    Script_PlayerStatsUtils(PS_SetStepSlot, 3, *((int8_t *)task_664 + 3));
    *((int8_t *)task_664 + 4) = 6;
    Script_PlayerStatsUtils(PS_SetStepSlot, 4, *((int8_t *)task_664 + 4));
    *((int8_t *)task_664 + 5) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 5, *((int8_t *)task_664 + 5));
    *((int8_t *)task_664 + 6) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 6, *((int8_t *)task_664 + 6));
    *((int8_t *)task_664 + 7) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 7, *((int8_t *)task_664 + 7));
    *((int8_t *)task_664 + 8) = 7;
    Script_PlayerStatsUtils(PS_SetStepSlot, 8, *((int8_t *)task_664 + 8));
    *((int8_t *)task_664 + 9) = 3;
    Script_PlayerStatsUtils(PS_SetStepSlot, 9, *((int8_t *)task_664 + 9));
    *((int8_t *)task_664 + 10) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 10, *((int8_t *)task_664 + 10));
    *((int8_t *)task_664 + 11) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 11, *((int8_t *)task_664 + 11));
    *((int8_t *)task_664 + 12) = 6;
    Script_PlayerStatsUtils(PS_SetStepSlot, 12, *((int8_t *)task_664 + 12));
    *((int8_t *)task_664 + 13) = 3;
    Script_PlayerStatsUtils(PS_SetStepSlot, 13, *((int8_t *)task_664 + 13));
    *((int8_t *)task_664 + 14) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 14, *((int8_t *)task_664 + 14));
    *((int8_t *)task_664 + 15) = 4;
    Script_PlayerStatsUtils(PS_SetStepSlot, 15, *((int8_t *)task_664 + 15));
  }
  local_F3 = 0;
  local_F2 = 0;
  local_F1 = 0;
  while (1) {
    if ((*(int8_t *)task_664 == Script_PlayerStatsUtils(PS_GetStepSlot, 0)) | (*((int8_t *)task_664 + 1) == Script_PlayerStatsUtils(PS_GetStepSlot, 1)) | (*((int8_t *)task_664 + 2) == Script_PlayerStatsUtils(PS_GetStepSlot, 2)) | (*((int8_t *)task_664 + 3) == Script_PlayerStatsUtils(PS_GetStepSlot, 3)) | (*((int8_t *)task_664 + 4) == Script_PlayerStatsUtils(PS_GetStepSlot, 4)) | (*((int8_t *)task_664 + 5) == Script_PlayerStatsUtils(PS_GetStepSlot, 5))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 6.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 6.0f);
      local_E8 = *((int8_t *)task_664 + local_EA);
      local_E7 = *((int8_t *)task_664 + local_E9);
      *((int8_t *)task_664 + local_EA) = local_E7;
      *((int8_t *)task_664 + local_E9) = local_E8;
    } else {
      local_F3 = 1;
    }
    if ((*((int8_t *)task_664 + 6) == Script_PlayerStatsUtils(PS_GetStepSlot, 6)) | (*((int8_t *)task_664 + 7) == Script_PlayerStatsUtils(PS_GetStepSlot, 7)) | (*((int8_t *)task_664 + 8) == Script_PlayerStatsUtils(PS_GetStepSlot, 8)) | (*((int8_t *)task_664 + 9) == Script_PlayerStatsUtils(PS_GetStepSlot, 9))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E8 = *((int8_t *)task_664 + (local_EA + 6));
      local_E7 = *((int8_t *)task_664 + (local_E9 + 6));
      *((int8_t *)task_664 + (local_EA + 6)) = local_E7;
      *((int8_t *)task_664 + (local_E9 + 6)) = local_E8;
    } else {
      local_F2 = 1;
    }
    if ((*((int8_t *)task_664 + 10) == Script_PlayerStatsUtils(PS_GetStepSlot, 10)) | (*((int8_t *)task_664 + 11) == Script_PlayerStatsUtils(PS_GetStepSlot, 11)) | (*((int8_t *)task_664 + 12) == Script_PlayerStatsUtils(PS_GetStepSlot, 12)) | (*((int8_t *)task_664 + 13) == Script_PlayerStatsUtils(PS_GetStepSlot, 13))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E8 = *((int8_t *)task_664 + (local_EA + 10));
      local_E7 = *((int8_t *)task_664 + (local_E9 + 10));
      *((int8_t *)task_664 + (local_EA + 10)) = local_E7;
      *((int8_t *)task_664 + (local_E9 + 10)) = local_E8;
    } else {
      local_F1 = 1;
    }
    local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    local_E8 = *((int8_t *)task_664 + (local_EA + 14));
    local_E7 = *((int8_t *)task_664 + (local_E9 + 14));
    *((int8_t *)task_664 + (local_EA + 14)) = local_E7;
    *((int8_t *)task_664 + (local_E9 + 14)) = local_E8;
    if (-(local_F3 == 1) & -(local_F2 == 1) & -(local_F1 == 1)) {
      break;
    }
  }
  local_F3 = 0;
  local_E6 = 1;
  local_E8 = *(int8_t *)task_664;
  while (local_F3 < 16) {
    local_E7 = *((int8_t *)task_664 + local_F3);
    if (local_E8 < local_E7) {
      local_E8 = local_E7;
      local_E6 = local_F3 + 1;
    }
    local_F3 = local_F3 + 1;
  }
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 11, local_E6);
  Script_PlayerStatsUtils(PS_SetStepSlot, 0, *(int8_t *)task_664);
  Script_PlayerStatsUtils(PS_SetStepSlot, 1, *((int8_t *)task_664 + 1));
  Script_PlayerStatsUtils(PS_SetStepSlot, 2, *((int8_t *)task_664 + 2));
  Script_PlayerStatsUtils(PS_SetStepSlot, 3, *((int8_t *)task_664 + 3));
  Script_PlayerStatsUtils(PS_SetStepSlot, 4, *((int8_t *)task_664 + 4));
  Script_PlayerStatsUtils(PS_SetStepSlot, 5, *((int8_t *)task_664 + 5));
  Script_PlayerStatsUtils(PS_SetStepSlot, 6, *((int8_t *)task_664 + 6));
  Script_PlayerStatsUtils(PS_SetStepSlot, 7, *((int8_t *)task_664 + 7));
  Script_PlayerStatsUtils(PS_SetStepSlot, 8, *((int8_t *)task_664 + 8));
  Script_PlayerStatsUtils(PS_SetStepSlot, 9, *((int8_t *)task_664 + 9));
  Script_PlayerStatsUtils(PS_SetStepSlot, 10, *((int8_t *)task_664 + 10));
  Script_PlayerStatsUtils(PS_SetStepSlot, 11, *((int8_t *)task_664 + 11));
  Script_PlayerStatsUtils(PS_SetStepSlot, 12, *((int8_t *)task_664 + 12));
  Script_PlayerStatsUtils(PS_SetStepSlot, 13, *((int8_t *)task_664 + 13));
  Script_PlayerStatsUtils(PS_SetStepSlot, 14, *((int8_t *)task_664 + 14));
  Script_PlayerStatsUtils(PS_SetStepSlot, 15, *((int8_t *)task_664 + 15));
  SCNF_WaitFrames_0000(1);
  Script_sprintf(" : %d %d %d %d %d %d : %d %d %d %d : %d %d %d %d : %d %d \012", *(int8_t *)task_664, *((int8_t *)task_664 + 1), *((int8_t *)task_664 + 2), *((int8_t *)task_664 + 3), *((int8_t *)task_664 + 4), *((int8_t *)task_664 + 5), *((int8_t *)task_664 + 6), *((int8_t *)task_664 + 7), *((int8_t *)task_664 + 8), *((int8_t *)task_664 + 9), *((int8_t *)task_664 + 10), *((int8_t *)task_664 + 11), *((int8_t *)task_664 + 12), *((int8_t *)task_664 + 13), *((int8_t *)task_664 + 14), *((int8_t *)task_664 + 15));
  Script_sprintf("lacky : %d\012", local_E6);
  return;
}

static void script_8966(void) {
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_lightSetSlotEnabled(2, 0);
  Script_lightSetSlotEnabled(3, 0);
  task_2F0 = 1;
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 14);
  Script_charaSetClipAngle(SHEN_NAME('KINK'), 15.0f);
  task_278 = 0x40D00000;
  *(&task_278 + 1) = 0;
  *(&task_278 + 2) = 0;
  task_2D8 = 45;
  task_2DC = 315;
  if (Script_GetOwnedItemCount(39, 0)) {
    Script_GetCassetteFlags_0(39, 1, 0);
    Script_GetCassetteFlags(39, 1, 0, 0);
  }
  if (Script_GetOwnedItemCount(51, 0)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 628, 1);
  }
  if (Script_GetOwnedItemCount(55, 0)) {
    Script_GetCassetteFlags_0(55, 1, 0);
    Script_GetCassetteFlags(55, 1, 0, 0);
  }
  if (Script_GetOwnedItemCount(0, 0)) {
    Script_GetCassetteFlags_0(0, 1, 0);
    Script_GetCassetteFlags(0, 1, 0, 0);
  }
  if (Script_GetOwnedItemCount(0, 0)) {
    Script_GetCassetteFlags_0(0, 1, 0);
    Script_GetCassetteFlags(0, 1, 0, 0);
  }
  if (Script_GetOwnedItemCount(55, 0) & Script_GetOwnedItemCount(54, 0)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 635, 1);
  }
  if (Script_GetOwnedItemCount(52, 0)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 638, 1);
  }
  return;
}

static int32_t script_8AA4(void) {
  int32_t unused_FF78[34];
  int32_t local_FF74;
  int32_t local_FF70;
  int32_t local_FF6C;
  int32_t local_FF68;
  int32_t local_FF64;
  int32_t local_FF60;
  Vector3 local_FF54;
  Vector3 local_FF48;
  int32_t local_FF44;
  int32_t unused_FF38[3];
  int32_t local_FF34;
  float local_FF30;
  int32_t unused_FF2C;
  GameDateTime local_FF24;
  local_FF44 = 0;
  Script_lightResetSlotToDefaults(15);
  Script_LightUtils(15, 0, 4);
  Script_LightUtils(15, 2, &data_10204);
  Script_LightUtils(15, 4, 1.0f);
  Script_LightUtils(15, 5, 7.0f);
  Script_LightUtils(15, 6, data_10214);
  Script_LightUtils(15, 10, 3640);
  Script_lightCreatePoint(6, &data_10270);
  Script_lightCreatePoint(7, &data_1029C);
  Script_lightCreatePoint(8, &data_102C8);
  Script_lightCreatePoint(9, &data_102F4);
  Script_lightSetSlotEnabled(6, 0);
  Script_lightSetSlotEnabled(7, 0);
  Script_lightSetSlotEnabled(8, 0);
  Script_lightSetSlotEnabled(9, 0);
  if (*task_304 == 2) {
    Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Show);
    Script_lightSetSlotEnabled(6, 1);
  }
  if (task_304[1] == 2) {
    Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Show);
    Script_lightSetSlotEnabled(7, 1);
  }
  if (task_304[2] == 2) {
    Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
    Script_lightSetSlotEnabled(8, 1);
  }
  if (task_304[3] == 2) {
    Script_charaDisplay(SHEN_NAME('FIR5'), CDM_Show);
    Script_lightSetSlotEnabled(9, 1);
  }
  Script_lightSetGlobalSceneLighting(&data_10240);
  task_2EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 22) * 1200;
  local_FF34 = 0x40E00000;
  local_FF54.x = 1.0f;
  (&local_FF54)->y = 1.0f;
  (&local_FF54)->z = 1.0f;
  local_FF70 = 0;
  for (;;) {
    (&local_FF54)->y = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, *(int16_t *)((int8_t *)task_2FC + (local_FF70 << 1))) / 240.0f;
    Script_hmdlApplyScale(*(&data_103B0 + local_FF70), &local_FF54);
    Script_EVT_GetPosition(*(&data_103B0 + local_FF70), -1, &local_FF48, 0);
    (&local_FF48)->y = (&local_FF48)->y + (&local_FF54)->y * 0.15f;
    Script_EVT_SetPosition(*(&data_103C0 + local_FF70), POS_XYZ, &local_FF48);
    if (task_304[local_FF70] == 0) {
      Script_charaDisplay(*(&data_103B0 + local_FF70), CDM_Hide);
    } else {
      if (task_304[local_FF70] == 1) {
        Script_charaDisplay(*(&data_103B0 + local_FF70), CDM_Show);
        goto L008CF0;
      }
    }
    L008CF0:;
    local_FF70 = local_FF70 + 1;
    if (local_FF70 == 4) {
      break;
    }
  }
  local_FF70 = 0;
  while (local_FF70 < 35) {
    *(&local_FF74 + local_FF70) = local_FF70 % 4;
    local_FF70 = local_FF70 + 1;
  }
  local_FF64 = 0;
  local_FF60 = 0;
  for (;;) {
    script_E632();
    Script_ReturnTimeDate(&local_FF24);
    if (((&local_FF24)->month == 4) & ((&local_FF24)->day == 15)) {
      (&local_FF24)->hours = 7;
      (&local_FF24)->minutes = 59;
      (&local_FF24)->seconds = 0;
      Script_SetNewDateTime(&local_FF24);
      Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
      SCNF_WaitFrames_0000(1);
    } else {
      if (((&local_FF24)->dayOfWeek == 1) & (local_FF60 == 0)) {
        script_80EE();
        local_FF60 = 1;
        goto L008DD0;
      }
    }
    L008DD0:;
    if ((*task_304 == 2) | (task_304[1] == 2) | (task_304[2] == 2) | (task_304[3] == 2) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890)) {
      task_2F0 = 0;
      if ((task_2E4 == 1) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890)) {
        Script_lightSetGlobalSceneLighting(&data_10260);
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
      } else {
        Script_lightSetGlobalSceneLighting(&data_10250);
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
      }
    } else {
      if (task_2E4 == 1) {
        Script_lightSetGlobalSceneLighting(&data_10250);
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
        task_2F0 = 0;
      } else {
        task_428 = 0;
        task_2F0 = 1;
        Script_lightSetGlobalSceneLighting(&data_10240);
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 1);
      }
      if (task_250 == -1) {
        Script_lightSetGlobalSceneLighting(&data_10260);
      }
    }
    {
      int32_t local_FF20;
      local_FF20 = local_FF64;
      switch (local_FF20) {
        case 0:
          local_FF6C = 0;
          local_FF68 = 17;
          local_FF64 = 1;
          break;
        case 1:
          local_FF6C = 17;
          local_FF68 = 35;
          local_FF64 = 0;
          break;
      }
    }
    local_FF70 = local_FF6C;
    while (local_FF70 < local_FF68) {
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR1'), *(&local_FF74 + local_FF70));
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR2'), *(&local_FF74 + local_FF70));
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR3'), *(&local_FF74 + local_FF70));
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR4'), *(&local_FF74 + local_FF70));
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR5'), *(&local_FF74 + local_FF70));
      *(&local_FF74 + local_FF70) = *(&local_FF74 + local_FF70) + 1;
      *(&local_FF74 + local_FF70) = *(&local_FF74 + local_FF70) % 6;
      local_FF70 = local_FF70 + 1;
    }
    SCNF_WaitFrames_0000(1);
    local_FF70 = 0;
    for (;;) {
      if (-(Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) < 13) & -(Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) > 0) & -(local_FF44 == 0) & -(task_2E4 == 1)) {
        if (Script_IsPlayerAllowed(0)) {
          if ((task_420 == 0) & (task_250 == 0)) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            local_FF44 = 1;
            Script_ShowUIMessageFromID("E1012A287");
            Script_ryoControl(RYO_RestoreControl);
          }
        }
      }
      if ((Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) < 1) & (task_2E4 == 1)) {
        if ((task_420 == 0) & (task_250 == 0)) {
          if (Script_IsPlayerAllowed(0)) {
            script_9568();
          }
        }
      }
      if (task_2F0 == 1) {
        if (Script_IsPlayerAllowed(0) & -(task_250 == 0)) {
          if (Script_SetTaskDataValue(CHARA_AKIR, &data_103DC, &data_103E8, 0)) {
            script_93DF();
          }
        }
      }
      if (task_304[local_FF70] == 2) {
        int32_t local_FF20;
        local_FF30 = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, *(int16_t *)((int8_t *)task_2FC + (local_FF70 << 1)) - script_92E0(local_FF70)) / 240.0f;
        (&local_FF54)->y = local_FF30;
        if (!(local_FF30 >= 0.0f)) {
          task_304[local_FF70] = 0;
          *(int16_t *)((int8_t *)task_2FC + (local_FF70 << 1)) = 240;
          Script_charaDisplay(*(&data_103C0 + local_FF70), CDM_Hide);
          Script_charaDisplay(*(&data_103B0 + local_FF70), CDM_Hide);
          Script_lightSetSlotEnabled(local_FF70 + 6, 0);
          (&local_FF54)->y = 1.0f;
          local_FF30 = 1.0f;
        }
        local_FF20 = local_FF70;
        switch (local_FF20) {
          case 0:
            Script_hmdlApplyScale(SHEN_NAME('RO11'), &local_FF54);
            Script_EVT_GetPosition(SHEN_NAME('RO11'), -1, &local_FF48, 0);
            (&local_FF48)->y = (&local_FF48)->y + local_FF30 * 0.15f;
            Script_EVT_SetPosition(SHEN_NAME('FIR1'), POS_XYZ, &local_FF48);
            break;
          case 1:
            Script_hmdlApplyScale(SHEN_NAME('RO21'), &local_FF54);
            Script_EVT_GetPosition(SHEN_NAME('RO21'), -1, &local_FF48, 0);
            (&local_FF48)->y = (&local_FF48)->y + local_FF30 * 0.15f;
            Script_EVT_SetPosition(SHEN_NAME('FIR2'), POS_XYZ, &local_FF48);
            break;
          case 2:
            Script_hmdlApplyScale(SHEN_NAME('RO31'), &local_FF54);
            Script_EVT_GetPosition(SHEN_NAME('RO31'), -1, &local_FF48, 0);
            (&local_FF48)->y = (&local_FF48)->y + local_FF30 * 0.15f;
            Script_EVT_SetPosition(SHEN_NAME('FIR3'), POS_XYZ, &local_FF48);
            break;
          case 3:
            Script_hmdlApplyScale(SHEN_NAME('RO41'), &local_FF54);
            Script_EVT_GetPosition(SHEN_NAME('RO41'), -1, &local_FF48, 0);
            (&local_FF48)->y = (&local_FF48)->y + local_FF30 * 0.15f;
            Script_EVT_SetPosition(SHEN_NAME('FIR5'), POS_XYZ, &local_FF48);
            break;
        }
      }
      local_FF70 = local_FF70 + 1;
      if (local_FF70 == 4) {
        break;
      }
    }
  }
  end:;
}

static int32_t script_92E0(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = -1;
  if (task_304[arg0] == 2) {
    if ((&local_F4)->hours - *(int8_t *)&task_674[arg0][4] < 0) {
      (&local_F4)->hours = (&local_F4)->hours + 24;
    }
    (&local_F4)->hours = (&local_F4)->hours - *(int8_t *)&task_674[arg0][4];
    (&local_F4)->minutes = (&local_F4)->minutes - *(int8_t *)&task_674[arg0][5];
    local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  } else {
    Script_ReturnTimeDate(&local_F4);
    *(int8_t *)task_674[arg0] = (&local_F4)->year;
    *(int8_t *)&task_674[arg0][1] = (&local_F4)->month;
    *(int8_t *)&task_674[arg0][3] = (&local_F4)->dayOfWeek;
    *(int8_t *)&task_674[arg0][4] = (&local_F4)->hours;
    *(int8_t *)&task_674[arg0][5] = (&local_F4)->minutes;
  }
  return local_F0;
}

__attribute__((scn_string_bindings(0, data_10424)))
static void script_93DF(void) {
  Vector3i local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ShowUIMessageFromID("E1012A083");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10400);
  (&local_F4)->y = 0;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  Script_scenToggleCharaFlag(0, 100, 1);
  if (task_2CC == 1) {
    task_2CC = 0;
    Script_fixoClearAttachment(CHARA_ONO_);
    Script_EVT_SetPosition(CHARA_ONO_, POS_XYZ, &data_1040C);
    Script_EVT_SetRotation(CHARA_ONO_, 0x38000000, &data_10418, 0);
  }
  Script_seqEnterEvtCam(10, 0, AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  task_2F8 = 0;
  return;
}

static void script_9568(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_2E4 = 0;
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  script_279C(0);
  Script_uiUtils(0, 0, 0);
  Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 0);
  Script_ShowUIMessageFromID(data_1042E);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if ((*task_304 == 2) | (task_304[1] == 2) | (task_304[2] == 2) | (task_304[3] == 2) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890)) {
    Script_ryoControl(RYO_RestoreControl);
  } else {
    script_93DF();
  }
  return;
}

static void script_9635(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  SCNF_RequestIconNotification(0, 0, 0, 2);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  (void)((task_250 = -1) & 4);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  if (task_2E4 == 1) {
    Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 0);
  }
  return;
}

static void script_96C5(void) {
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_lightSetSlotEnabled(28, 0);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_SetActionSelectorHUDElementVisibility(0);
  if (task_2E4 == 1) {
    Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 1);
    script_D9A7(CHARA_AKIR, SHEN_NAME('LIGH'));
    Script_lightSetSlotEnabled(15, 1);
  } else {
    script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
    Script_lightSetSlotEnabled(15, 0);
  }
  if (task_2CC == 1) {
    Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
  }
  if (SAVE_Script_UpdateForSaveData(4, 0) < 0) {
    SAVE_Script_UpdateForSaveData(3, 0, 10);
  }
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  SCNF_WaitFrames_0000(10);
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  (void)((task_250 = 0) & 4);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  return;
}

static void script_9853(int32_t arg0, float arg1) {
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= arg1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_9872(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_988E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F0 = 0;
  local_F4.x = *(float *)arg2;
  (&local_F4)->y = *(float *)(arg2 + 4);
  (&local_F4)->z = *(float *)(arg2 + 8);
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  while (local_F0 < arg4) {
    local_F0 = local_F0 + 1;
    Script_motiSetAngFlags(arg0, 1);
    Script_SetMotnFlags(arg0, arg1, &local_F4, 0x78000000);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_9904(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_988E, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static void script_991C(void) {
  Vector3i local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10444);
  (&local_F4)->y = 0x4000;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4162, 0.0f, -1, -1, 1.5f, 0);
  script_9853(0x52494B41, 90.0f);
  Script_executeAicaCommand(0x1105A9, 0, 0);
  Script_fixoClearAttachment(CHARA_ONO_);
  Script_EVT_SetPosition(CHARA_ONO_, POS_XYZ, &data_10450);
  Script_EVT_SetRotation(CHARA_ONO_, 0x38000000, &data_1045C, 0);
  script_9872(CHARA_AKIR);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_SetGameStateTo(AUTH_None);
  return;
}

__attribute__((scn_string_bindings(0, data_10480)))
static int32_t script_9AA4(void) {
  int32_t unused_F8[2];
  SCNF_RequestIconNotification(0, 0, 0, 2);
  (void)((task_250 = 0) & 8);
  task_254 = 2;
  SCNF_WaitFrames_0000(1);
  Script_SetGameStateTo(AUTH_None);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (-(task_318 == 1) & -(task_2F0 == 0) & Script_IsPlayerAllowed()) {
      break;
    }
  }
  if (task_2CC == 0) {
    script_9635();
    task_318 = 0;
    Script_ResetHandMotion(CHARA_AKIR, 1, 0, 411);
    task_258 = 0;
    task_284.x = 6.3f;
    task_284.y = 0.0f;
    task_284.z = 0.7f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_284);
    task_290.x = 0;
    task_290.y = 0;
    task_290.z = 0;
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_290, 0);
    script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
    Script_lightSetSlotEnabled(15, 0);
    Script_motiSetAngFlags(CHARA_AKIR, 0);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
    Script_PlayCharaMotion(CHARA_AKIR, 4105, 78.0f, -1, -1, 1.3f, 0);
    Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
    SCNF_WaitFrames_0000(1);
    Script_motiSetAngFlags(CHARA_AKIR, 1);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
    Script_SetGameStateTo(AUTH_EventCam);
    Script_scenCreateTask(script_AC4D, 0);
    Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
    Script_lightSpotLightEnable(28, &data_10468, &data_10474);
    Script_LightUtils(28, 2, &data_F680);
    Script_LightUtils(28, 4, 2.1f);
    Script_LightUtils(28, 5, 2.8f);
    Script_LightUtils(28, 6, data_F690);
    Script_LightUtils(28, 10, 4187);
    Script_lightSetSlotEnabled(28, 1);
    Script_clmdSetFlags(CHARA_AKIR, 4, 1);
    script_9853(0x52494B41, 203.0f);
    Script_SetNewHandMotion(CHARA_AKIR, 0, 402, 0, 0, 0, 0, 1.0f, 60);
    script_9853(0x52494B41, 225.0f);
    Script_executeAicaCommand(132521, 0, 0);
    Script_ShowUIMessageFromID("E1012A040");
    script_9853(0x52494B41, 305.0f);
    Script_executeAicaCommand(198057, 0, 0);
    Script_ShowUIMessageFromID("E1012A041");
    script_9853(0x52494B41, 350.0f);
    Script_ShowUIMessageFromID("E1012A042");
    script_9853(0x52494B41, 385.0f);
    Script_executeAicaCommand(132521, 0, 0);
    script_9853(0x52494B41, 460.0f);
    Script_clmdSetFlags(CHARA_AKIR, 4, 0);
    script_9853(0x52494B41, 470.0f);
    Script_SetNewHandMotion(CHARA_AKIR, 0, 411, 0, 0, 0, 0, 1.0f, 60);
    script_9872(CHARA_AKIR);
    Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SPACE_BEHIND_WALL, 1);
    script_96C5();
    Script_lightSetSlotEnabled(28, 0);
  }
  Script_scenCreateTask(script_9DC6, 0);
  end:;
}

static int32_t script_9DC6(void) {
  int32_t local_FC;
  int32_t unused_F4[2];
  SCNF_RequestIconNotification(0, 0, 0, 2);
  task_254 = 3;
  local_FC = 0;
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 26, 2);
  L009DED:;
  (void)((task_250 = 0) & 4);
  for (;;) {
    if (-(task_318 == 1) & -(task_2CC == 1) & -(task_2F0 == 0) & Script_IsPlayerAllowed()) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_9635();
  local_FC = local_FC + 1;
  task_258 = 0;
  task_284.x = 6.2196f;
  task_284.y = 0.0f;
  task_284.z = 0.7367f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_284);
  task_290.x = 0;
  task_290.y = 51154;
  task_290.z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_290, 0);
  task_25C = 11;
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_fixoAttachEx(CHARA_ONO_, CHARA_AKIR, 18, &data_104A0, &data_104AC);
  SCNF_WaitFrames_0000(1);
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  script_9F48();
  script_A040();
  local_FC = 2;
  task_290.x = 0;
  task_290.y = 0;
  task_290.z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_290, 0);
  if (!(local_FC >= 2)) goto L009DED;
  Script_scenCreateTask(script_A2FD, 0);
  return task_2CC = 0;
}

static void script_9F48(void) {
  Script_lightSpotLightEnable(28, &data_104B8, &data_104C4);
  Script_LightUtils(28, 2, &data_F680);
  Script_LightUtils(28, 4, 2.1f);
  Script_LightUtils(28, 5, 2.8f);
  Script_LightUtils(28, 6, data_F690);
  Script_LightUtils(28, 10, 4187);
  Script_lightSetSlotEnabled(28, 1);
  Script_scenCreateTask(script_AC77, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4108, 20.0f, -1, -1, 1.0f, 0);
  Script_ShowUIMessageFromID("E1012A043");
  SCNF_WaitFrames_0000(1);
  script_9853(0x52494B41, 45.0f);
  Script_executeAicaCommand(263593, 0, 0);
  script_9853(0x52494B41, 80.0f);
  Script_clmdSetFlags(CHARA_AKIR, 4, 1);
  script_9853(0x52494B41, 110.0f);
  Script_clmdSetFlags(CHARA_AKIR, 4, 0);
  script_9872(CHARA_AKIR);
  return;
}

static void script_A040(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  task_258 = 0;
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, &data_104DC);
  Script_charaDisplay(SHEN_NAME('KABE'), CDM_Show);
  Script_seqEnterEvtCam(1000, 0, AUTH_None);
  for (;;) {
    Script_charaDisplay(SHEN_NAME('KABE'), CDM_Show);
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_PlayCharaMotion(CHARA_AKIR, 4108, 0.0f, 56.0f, 0.0f, 1.0f, 0);
        break;
      case 1:
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
        SCNF_WaitFrames_0000(1);
        Script_seqEnterEvtCam(900, 0, AUTH_None);
        Script_PlayCharaMotion(CHARA_AKIR, 4108, 0.0f, 60.0f, 42.0f, 1.0f, 0);
        break;
      case 2:
        Script_seqEnterEvtCam(702, 0, AUTH_None);
        Script_GetDataFromFightProgram(CHARA_AKIR, 2);
        Script_PlayCharaMotion(CHARA_AKIR, 4115, 30.0f, 90.0f, 25.0f, 1.0f, 0);
        break;
      case 3:
        Script_GetDataFromFightProgram(CHARA_AKIR, 0);
        Script_charaDisplay(SHEN_NAME('KABE'), CDM_Hide);
        Script_PlayCharaMotion(CHARA_AKIR, 4115, 90.0f, -1.0f, 90.0f, 1.0f, 0);
        break;
      case 4:
        Script_charaDisplay(SHEN_NAME('KABE'), CDM_Hide);
        goto L00A2E8;
        break;
    }
    SCNF_WaitFrames_0000(1);
    if (local_FC == 0) {
      script_9853(0x52494B41, 30.0f);
      Script_ShowUIMessageFromID("E1012A045");
    }
    if (local_FC != 3) {
      script_9853(0x52494B41, 55.0f);
      Script_executeAicaCommand(329129, 0, 0);
    }
    if (local_FC == 2) {
      Script_charaDisplay(SHEN_NAME('KABE'), CDM_Hide);
      Script_scenCreateTask(script_E531, 1);
    }
    if (local_FC == 2) {
      script_9853(0x52494B41, 56.0f);
      Script_SetCamShake(0.23f, 0.23f, 0.9f);
      script_9853(0x52494B41, 75.0f);
      Script_scenCreateTask(script_ADA9, 0);
    }
    if (local_FC == 3) {
      script_9853(0x52494B41, 60.0f);
      Script_executeAicaCommand(0x1005A9, 0, 0);
      script_9853(0x52494B41, 100.0f);
      Script_fixoClearAttachment(CHARA_ONO_);
      Script_scenCreateTask(script_AEE5, 0);
      Script_seqEnterEvtCam(703, 0, AUTH_None);
      script_9853(0x52494B41, 110.0f);
      Script_executeAicaCommand(984489, 0, 0);
    }
    script_9872(CHARA_AKIR);
    local_FC = local_FC + 1;
  }
  L00A2E8:;
  script_9872(CHARA_AKIR);
  Script_lightSetSlotEnabled(28, 0);
  return;
}

static int32_t script_A2FD(void) {
  int32_t unused_F8[2];
  int32_t local_F4;
  int32_t unused_F0;
  SCNF_RequestIconNotification(0, 0, 0, 2);
  local_F4 = 0;
  (void)((task_250 = 0) & 8);
  task_254 = 4;
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, &data_104F4);
  Script_charaToggleMotEnabled(CHARA_AKIR, -1);
  Script_charaDisplay(SHEN_NAME('HKFT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('HKHK'), CDM_Show);
  (void)((task_250 = -1) & 8);
  Script_charaDisplay(SHEN_NAME('DAMY'), CDM_Hide);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 407);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 407);
  Script_scenCreateTask(script_ABA5, 0);
  script_A3A0();
  end:;
}

static void script_A3A0(void) {
  int32_t unused_F4[3];
  Vector3 local_E8;
  Vector3i local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  int32_t unused_B8[3];
  Vector3i local_AC;
  int32_t local_A8;
  SCNF_RequestIconNotification(0, 0, 0, 2);
  (void)((task_250 = -1) & 8);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_lightSpotLightEnable(28, &data_105D8, &data_105E4);
  Script_LightUtils(28, 2, &data_F680);
  Script_LightUtils(28, 4, 2.1f);
  Script_LightUtils(28, 5, 2.8f);
  Script_LightUtils(28, 6, data_F690);
  Script_LightUtils(28, 10, 4187);
  Script_lightSetSlotEnabled(28, 1);
  local_E8.x = 6.202f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.821f;
  local_DC.x = 0;
  (&local_DC)->y = 0;
  (&local_DC)->z = 0;
  Script_EVT_SetPosition(SHEN_NAME('HKHK'), POS_XYZ, &local_E8);
  Script_EVT_SetRotation(SHEN_NAME('HKHK'), 0x38000000, &local_DC, 0);
  local_E8.x = 6.349f;
  (&local_E8)->y = -0.021f;
  (&local_E8)->z = 1.037f;
  local_DC.x = 0;
  (&local_DC)->y = 21293;
  (&local_DC)->z = 0;
  Script_EVT_SetPosition(SHEN_NAME('HKFT'), POS_XYZ, &local_E8);
  Script_EVT_SetRotation(SHEN_NAME('HKFT'), 0x38000000, &local_DC, 0);
  Script_charaDisplay(SHEN_NAME('HKFT'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HKHK'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HOKY'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CH2K'), CDM_Show);
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  Script_charaDisplay(SHEN_NAME('LIGH'), CDM_Show);
  Script_EVT_SetPosition(SHEN_NAME('LIGH'), POS_XYZ, &data_10590);
  Script_EVT_SetRotation(SHEN_NAME('LIGH'), 0x38000000, &data_1059C, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_1050C);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10500, 0);
  Script_ShowUIMessageFromID("E1012A276");
  Script_PlayCharaMotion(CHARA_AKIR, 4146, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 407, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 0, 407, 0, 0, 0, 0, 1.0f, 0);
  Script_charaSetClipAngle(SHEN_NAME('CH2K'), 15.0f);
  task_258 = 0;
  for (;;) {
    Script_GetCameraInputDetails(15);
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_A8 = task_258;
    switch (local_A8) {
      case 110:
        Script_tmnmUtils(SHEN_NAME('KINK'), 4157, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_executeAicaCommand(591273, 0, 0);
        break;
      case 200:
        break;
      case 231:
        Script_SetNewHandMotion(CHARA_AKIR, 1, 413, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 312:
        Script_SetNewHandMotion(CHARA_AKIR, 1, 411, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 494:
        Script_SetNewHandMotion(CHARA_AKIR, 1, 407, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 291:
        Script_SetNewHandMotion(CHARA_AKIR, 0, 413, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 385:
        Script_SetNewHandMotion(CHARA_AKIR, 0, 411, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 495:
        Script_SetNewHandMotion(CHARA_AKIR, 0, 407, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 230:
        Script_tmnmUtils(SHEN_NAME('CH2K'), 4158, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 255:
        Script_ShowUIMessageFromID("E1012A278");
        Script_executeAicaCommand(0x3705A9, 0, 0);
        break;
      case 320:
        Script_executeAicaCommand(0x3705A9, 0, 0);
        break;
      case 390:
        break;
      case 405:
        Script_executeAicaCommand(0x2205A9, 0, 0);
        break;
      case 430:
        Script_executeAicaCommand(0x2405A9, 0, 0);
        break;
      case 470:
        break;
      case 500:
        Script_fixoAttachEx(SHEN_NAME('HOKY'), CHARA_AKIR, 18, &data_10578, &data_10584);
        Script_charaDisplay(SHEN_NAME('HKFT'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('HKHK'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('CH2K'), CDM_Hide);
        Script_ShowUIMessageFromID("E1012A279");
        break;
    }
    if (task_258 > 600) {
      Script_fixoClearAttachment(SHEN_NAME('LIGH'));
      Script_mtwkGetSetRotation(SHEN_NAME('LIGH'), &local_AC, 0);
      Script_EVT_GetPosition(SHEN_NAME('LIGH'), -1, &local_D0, 0);
      local_C4.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_AC)->x);
      (&local_C4)->y = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_AC)->y);
      (&local_C4)->z = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_AC)->z);
      Script_lightSpotLightEnable(28, &local_D0, &local_C4);
      Script_LightUtils(28, 2, &data_F680);
      Script_LightUtils(28, 4, 2.1f);
      Script_LightUtils(28, 5, 2.8f);
      Script_LightUtils(28, 6, data_F690);
      Script_LightUtils(28, 10, 7281);
      Script_lightSetSlotEnabled(28, 1);
      Script_fixoAttachEx(SHEN_NAME('LIGH'), CHARA_AKIR, 12, &data_105C0, &data_105CC);
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  task_258 = 0;
  Script_ShowUIMessageFromID("E1012A081");
  for (;;) {
    task_258 = task_258 + 1;
    Script_GetCameraInputDetails(15);
    if (task_258 == 60) {
      SCNF_StartIconSystemMenu(5);
    }
    if (task_258 == 120) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10518, 0);
  Script_fixoAttachEx(SHEN_NAME('HOKY'), CHARA_AKIR, 18, &data_10578, &data_10584);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 4145, 20.0f, 155.0f, -1, 1.0f, 0);
  task_258 = 0;
  for (;;) {
    Script_GetCameraInputDetails(15);
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    if (task_258 == 60) {
      Script_executeAicaCommand(0x3505A9, 0, 0);
      Script_fixoClearAttachment(SHEN_NAME('HOKY'));
      Script_charaDisplay(SHEN_NAME('HOKY'), CDM_Hide);
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetCassetteFlags(5, 1, 0, 0);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_D28B(2, 0x4F4D4F4A, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE, 1);
  Script_charaDisplay(SHEN_NAME('HOKY'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HKFT'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HKHK'), CDM_Hide);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  Script_fixoClearAttachment(SHEN_NAME('LIGH'));
  (void)((task_250 = 0) & 2);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 26, 4);
  Script_clmdSetFlags(CHARA_AKIR, 4, 0);
  task_254 = 6;
  (void)((task_250 = -1) & 2);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  task_2E4 = 0;
  SCNF_WaitFrames_0000(1);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  script_E05B();
  Script_shenRequestGameModuleTransition(2, SHEN_NAME('JHD0'), 6);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_ABA5(void) {
  Script_seqEnterEvtCam(810, 0, AUTH_None);
  SCNF_WaitFrames_0000(160);
  Script_seqEnterEvtCam(805, 0, AUTH_None);
  SCNF_WaitFrames_0000(90);
  Script_seqEnterEvtCam(806, 0, AUTH_None);
  SCNF_WaitFrames_0000(150);
  Script_seqEnterEvtCam(807, 0, AUTH_None);
  SCNF_WaitFrames_0000(80);
  Script_seqEnterEvtCam(808, 0, AUTH_None);
  SCNF_WaitFrames_0000(360);
  Script_seqEnterEvtCam(809, 0, AUTH_None);
  SCNF_WaitFrames_0000(190);
  Script_convSetCamInactiveFlag(0);
  end:;
}

static void script_AC25(void) {
  Script_seqEnterEvtCam(10, 0, AUTH_None);
  SCNF_WaitFrames_0000(120);
  Script_seqEnterEvtCam(11, 0, AUTH_None);
  SCNF_WaitFrames_0000(195);
  return;
}

static int32_t script_AC4D(void) {
  Script_seqEnterEvtCam(600, 0, AUTH_None);
  SCNF_WaitFrames_0000(120);
  Script_seqEnterEvtCam(601, 0, AUTH_None);
  SCNF_WaitFrames_0000(310);
  end:;
}

static int32_t script_AC77(void) {
  Script_seqEnterEvtCam(1000, 0, AUTH_None);
  end:;
}

static int32_t script_AC85(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  local_E4.x = 0.0f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 0.0f;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F0, 0);
  if (arg0 == 1) {
    (&local_F0)->y = (&local_F0)->y + 0.7f;
  }
  local_FC = Script_EffectUtils(0, &local_F0, &local_E4, 10, 0.08f, 0, 0);
  Script_EffectUtils(8, local_FC, 1.25f, 1.25f);
  local_D8.x = 0.00078f;
  (&local_D8)->y = 0.0f;
  (&local_D8)->z = 0.00507f;
  local_CC.x = 0.00468f;
  (&local_CC)->y = 0.0f;
  (&local_CC)->z = 0.0f;
  Script_EffectUtils(6, local_FC, &local_D8, &local_CC);
  Script_EffectUtils(9, local_FC, 46, 49, 46);
  Script_EffectUtils(12, local_FC, 26);
  Script_EffectUtils(10, local_FC, 0.2246f);
  Script_EffectUtils(11, local_FC, 125);
  Script_EffectUtils(8, local_FC, 1.1718f, 1.0546f);
  Script_EffectUtils(1, local_FC);
  SCNF_WaitFrames_0000(150);
  Script_EffectUtils(2, local_FC);
  return Script_EffectUtils(3, local_FC);
}

static int32_t script_ADA9(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  local_E4.x = 0.0f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 0.0f;
  Script_EVT_GetPosition(SHEN_NAME('KABE'), -1, &local_F0, 0);
  local_FC = Script_EffectUtils(0, &local_F0, &local_E4, 10, 0.08f, 0, 0);
  Script_EffectUtils(8, local_FC, 1.25f, 1.25f);
  local_D8.x = 0.00078f;
  (&local_D8)->y = 0.0f;
  (&local_D8)->z = 0.00507f;
  local_CC.x = 0.00468f;
  (&local_CC)->y = 0.0f;
  (&local_CC)->z = 0.0f;
  Script_EffectUtils(6, local_FC, &local_D8, &local_CC);
  Script_EffectUtils(9, local_FC, 46, 49, 46);
  Script_EffectUtils(12, local_FC, 26);
  Script_EffectUtils(10, local_FC, 0.2246f);
  Script_EffectUtils(11, local_FC, 125);
  Script_EffectUtils(8, local_FC, 1.1718f, 1.0546f);
  Script_EffectUtils(1, local_FC);
  SCNF_WaitFrames_0000(100);
  Script_EffectUtils(2, local_FC);
  return Script_EffectUtils(3, local_FC);
}

static void script_AEB0(void) {
  int32_t local_FC;
  local_FC = 0;
  L00AEB8:;
  Script_EVT_SetPosition(*(&data_10618 + local_FC), POS_XYZ, &data_10660);
  local_FC = local_FC + 1;
  if (!(local_FC >= 18)) goto L00AEB8;
  return;
}

__attribute__((scn_string_bindings(0, data_1066C)))
static int32_t script_AEE5(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Vector3 local_D8;
  Script_EVT_GetPosition(CHARA_ONO_, -1, &local_D8, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  local_E4 = 200;
  L00AF16:;
  Script_InterpolateAndScaleAngles(0, local_E4, &local_F4, &local_E8, "DYN2");
  local_F4.x = (&local_F4)->x + (&local_D8)->x;
  (&local_F4)->y = (&local_F4)->y * (float)10;
  (&local_F4)->z = (&local_D8)->z - (&local_F4)->z * (float)40;
  Script_EVT_SetPosition(CHARA_ONO_, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_ONO_, 0x38000000, &local_E8, 0);
  local_E4 = local_E4 + 2;
  SCNF_WaitFrames_0000(1);
  if (!(local_E4 > 289)) goto L00AF16;
  return Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
}

static void script_AFA5(void) {
  Vector3i local_F4;
  int32_t unused_DC[6];
  Vector3 local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  script_9635();
  Script_GetCassetteFlags_0(56, 1, 0);
  Script_mtwkGetSetRotation(0x306B6E64, &local_C4, 0);
  Script_EVT_GetPosition(0x306B6E64, -1, &local_D0, 0);
  Script_charaDisplay(0x306B6E64, CDM_Show);
  Script_charaDisplay(CHARA_ONO_, CDM_Hide);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10684);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10690, 0);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_seqEnterEvtCam(6300, 0, AUTH_None);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 812);
  Script_fixoAttachEx(0x306B6E64, CHARA_AKIR, 18, &data_1069C, &data_106A8);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4151, 163.0f, 186.0f, -1.0f, 1.0f, 0);
  task_258 = 0;
  for (;;) {
    if (task_258 == 24) {
      Script_fixoClearAttachment(0x306B6E64);
      break;
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  local_C0 = 0;
  for (;;) {
    local_BC = local_C0 % 30;
    if (local_BC < 20) {
      if (local_BC == 0) {
        Script_executeAicaCommand(0x2105A9, 0, 0);
      }
      local_F4.x = (&local_F4)->x + 455;
    } else {
      local_F4.x = (&local_F4)->x - 910;
    }
    local_C0 = local_C0 + 1;
    if (local_C0 == 120) {
      break;
    }
    Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_F4);
    SCNF_WaitFrames_0000(1);
  }
  task_2E8 = 1;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB, 1);
  Script_EVT_SetRotation(0x306B6E64, 0x38000000, &local_C4, 0);
  Script_EVT_SetPosition(0x306B6E64, POS_XYZ, &local_D0);
  Script_charaDisplay(CHARA_ONO_, CDM_Show);
  Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_ShowUIMessageFromID(data_106B4);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_96C5();
  return;
}

static void script_B22C(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F0[3];
  Vector3 local_E4;
  Script_lightSetSlotEnabled(15, 0);
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSpotLightEnable(28, &data_106F0, &data_106FC);
  Script_LightUtils(28, 2, &data_F680);
  Script_LightUtils(28, 4, 2.1f);
  Script_LightUtils(28, 5, 2.8f);
  Script_LightUtils(28, 6, data_F690);
  Script_LightUtils(28, 10, 4187);
  Script_lightSetSlotEnabled(28, 1);
  Script_charaDisplay(CHARA_ONO_, CDM_Hide);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_charaDisplay(SHEN_NAME('DOGI'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SHAS'), CDM_Show);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4100, 0.0f, -1, -1, 1.0f, 0);
  Script_EVT_GetPosition(SHEN_NAME('FUTA'), -1, &local_E4, 0);
  (&local_E4)->y = 0.45f;
  Script_EVT_SetPosition(SHEN_NAME('FUTA'), POS_XYZ, &local_E4);
  Script_tmnmUtils(SHEN_NAME('FUTA'), 4170, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  local_FC = 0;
  for (;;) {
    if (local_FC == 40) {
      Script_executeAicaCommand(0x2405A9, 0, 0);
    }
    if (local_FC == 86) {
      Script_executeAicaCommand(0x1105A9, 0, 0);
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  return;
}

static void script_B3C0(int32_t arg0) {
  int32_t unused_FC;
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_charaDisplay(SHEN_NAME('DOGI'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHAS'), CDM_Hide);
  Script_lightSetSlotEnabled(28, 0);
  Script_charaDisplay(CHARA_ONO_, CDM_Show);
  if (task_2E4 == 1) {
    Script_lightSetSlotEnabled(15, 1);
    script_D9A7(CHARA_AKIR, SHEN_NAME('LIGH'));
  }
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  return;
}

static void script_B432(void) {
  int32_t local_FC;
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 812);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10738);
  {
    Vector3i local_F0;
    (&local_F0)->y = 0x4000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 4129, 245.0f, 305.0f, -1.0f, 1.0f, 0);
  Script_fixoAttachEx(SHEN_NAME('SHAS'), CHARA_AKIR, 18, &data_10720, &data_1072C);
  Script_seqEnterEvtCam(2102, 0, AUTH_None);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  {
    Vector3i local_F0;
    (&local_F0)->y = 0x8000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
  }
  local_FC = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4130, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (local_FC == 30) {
      Script_seqEnterEvtCam(2103, 0, AUTH_None);
      Script_ShowUIMessageFromID("E1012A074");
      break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_B56B(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  script_9635();
  Script_GetCassetteFlags_0(54, 1, 0);
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(SHEN_NAME('HROU'), CDM_Show);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 812);
  {
    int32_t local_E4;
    local_E4 = arg0;
    switch (local_E4) {
      case 1:
        Script_seqEnterEvtCam(1204, 0, AUTH_None);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10750);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10780, 0);
        Script_PlayCharaMotion(CHARA_AKIR, 4161, 0.0f, 85.0f, -1, 1.0f, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU1'), -1, &local_E8, 0);
        Script_fixoAttachEx(SHEN_NAME('HROU'), CHARA_AKIR, 18, &data_107B0, &data_107E0);
        script_9904(CHARA_AKIR, 17, (int32_t)&data_10810, 20, 45);
        break;
      case 2:
        Script_seqEnterEvtCam(1205, 0, AUTH_None);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_1075C);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_1078C, 0);
        Script_PlayCharaMotion(CHARA_AKIR, 4159, 0.0f, 85.0f, -1, 1.0f, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU2'), -1, &local_E8, 0);
        Script_fixoAttachEx(SHEN_NAME('HROU'), CHARA_AKIR, 18, &data_107BC, &data_107EC);
        script_9904(CHARA_AKIR, 17, (int32_t)&data_1081C, 30, 35);
        break;
      case 3:
        Script_seqEnterEvtCam(1206, 0, AUTH_None);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10768);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10798, 0);
        Script_PlayCharaMotion(CHARA_AKIR, 4161, 0.0f, 85.0f, -1, 1.0f, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU3'), -1, &local_E8, 0);
        Script_fixoAttachEx(SHEN_NAME('HROU'), CHARA_AKIR, 18, &data_107C8, &data_107F8);
        script_9904(CHARA_AKIR, 17, (int32_t)&data_10828, 25, 40);
        break;
      case 4:
        Script_seqEnterEvtCam(1207, 0, AUTH_None);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10774);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_107A4, 0);
        Script_PlayCharaMotion(CHARA_AKIR, 4160, 0.0f, 85.0f, -1, 1.0f, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU4'), -1, &local_E8, 0);
        Script_fixoAttachEx(SHEN_NAME('HROU'), CHARA_AKIR, 18, &data_107D4, &data_10804);
        script_9904(CHARA_AKIR, 17, (int32_t)&data_10834, 25, 40);
        break;
    }
  }
  (&local_E8)->y = 0.0f;
  Script_SetLookPointDetails(CHARA_AKIR, 1, &local_E8, 1);
  Script_charaDisplay(SHEN_NAME('HROU'), CDM_Show);
  task_258 = 0;
  SCNF_WaitFrames_0000(50);
  script_96C5();
  Script_fixoClearAttachment(SHEN_NAME('HROU'));
  Script_charaDisplay(SHEN_NAME('HROU'), CDM_Hide);
  {
    int32_t local_E4;
    local_E4 = arg0;
    switch (local_E4) {
      case 1:
        Script_charaDisplay(SHEN_NAME('RO11'), CDM_Show);
        break;
      case 2:
        Script_charaDisplay(SHEN_NAME('RO21'), CDM_Show);
        break;
      case 3:
        Script_charaDisplay(SHEN_NAME('RO31'), CDM_Show);
        break;
      case 4:
        Script_charaDisplay(SHEN_NAME('RO41'), CDM_Show);
        break;
    }
  }
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->y = 0.0f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  script_E05B();
  return;
}

static void script_B936(int32_t arg0) {
  Vector3 local_F4;
  script_9635();
  Script_GetCassetteFlags_0(55, 1, 0);
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 811);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 812);
  {
    int32_t local_F0;
    local_F0 = arg0;
    switch (local_F0) {
      case 1:
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10888);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_108C4, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU1'), -1, &local_F4, 0);
        Script_seqEnterEvtCam(1200, 0, AUTH_None);
        Script_PlayCharaMotion(CHARA_AKIR, 4169, 0.0f, 215.0f, -1, 1.0f, 0);
        break;
      case 2:
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10894);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_108D0, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU2'), -1, &local_F4, 0);
        Script_seqEnterEvtCam(1201, 0, AUTH_None);
        if (*(int16_t *)((int8_t *)task_2FC + 4) > 239) {
          Script_PlayCharaMotion(CHARA_AKIR, 4165, 0.0f, 215.0f, -1, 1.0f, 0);
        } else {
          Script_PlayCharaMotion(CHARA_AKIR, 4166, 0.0f, 215.0f, -1, 1.0f, 0);
        }
        break;
      case 3:
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_108A0);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_108DC, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU3'), -1, &local_F4, 0);
        Script_seqEnterEvtCam(1202, 0, AUTH_None);
        if (*(int16_t *)((int8_t *)task_2FC + 6) > 239) {
          Script_PlayCharaMotion(CHARA_AKIR, 4169, 0.0f, 215.0f, -1, 1.0f, 0);
        } else {
          Script_PlayCharaMotion(CHARA_AKIR, 4167, 0.0f, 215.0f, -1, 1.0f, 0);
        }
        break;
      case 4:
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_108AC);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_108E8, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU4'), -1, &local_F4, 0);
        Script_seqEnterEvtCam(1203, 0, AUTH_None);
        Script_PlayCharaMotion(CHARA_AKIR, 4168, 0.0f, 215.0f, -1, 1.0f, 0);
        break;
    }
  }
  (&local_F4)->y = 0.0f;
  Script_charaDisplay(SHEN_NAME('MAT2'), CDM_Show);
  Script_CharHandlers(CHARA_AKIR, 0, 3, 16);
  Script_charaDisplay(SHEN_NAME('MATB'), CDM_Show);
  Script_fixoAttachEx(SHEN_NAME('MAT2'), CHARA_AKIR, 12, &data_10840, &data_1084C);
  Script_fixoAttachEx(SHEN_NAME('MATB'), CHARA_AKIR, 18, &data_10870, &data_1087C);
  Script_SetLookPointDetails(CHARA_AKIR, 1, &local_F4, 1);
  task_258 = 0;
  for (;;) {
    int32_t local_F0;
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_F0 = task_258;
    switch (local_F0) {
      case 34:
        Script_executeAicaCommand(0x1905A9, 0, 0);
        break;
      case 44:
        Script_executeAicaCommand(0x1D05A9, 0, 0);
        Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Show);
        Script_fixoAttachEx(SHEN_NAME('FIR4'), CHARA_AKIR, 18, &data_108B8, &data_108F4);
        local_F0 = arg0;
        switch (local_F0) {
          case 1:
            Script_lightSetSlotEnabled(6, 1);
            break;
          case 2:
            Script_lightSetSlotEnabled(7, 1);
            break;
          case 3:
            Script_lightSetSlotEnabled(8, 1);
            break;
          case 4:
            Script_lightSetSlotEnabled(9, 1);
            break;
        }
        break;
      case 46:
        Script_executeAicaCommand(0x3205A9, 0, 0);
        break;
      case 153:
        local_F0 = arg0;
        switch (local_F0) {
          case 1:
            Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Show);
            break;
          case 2:
            Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Show);
            break;
          case 3:
            Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
            break;
          case 4:
            Script_charaDisplay(SHEN_NAME('FIR5'), CDM_Show);
            break;
        }
        break;
    }
    if (-(*(int16_t *)((int8_t *)task_2FC + 2) == 240) & -(arg0 == 2) & -(task_258 > 100)) {
      Script_motiSetAngFlags(CHARA_AKIR, 1);
      Script_SetMotnFlags(CHARA_AKIR, 17, &data_10900, 0x78000000);
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_96C5();
  Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('MAT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('MATB'), CDM_Hide);
  Script_fixoClearAttachment(SHEN_NAME('FIR4'));
  Script_fixoClearAttachment(SHEN_NAME('MAT2'));
  Script_fixoClearAttachment(SHEN_NAME('MATB'));
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &task_3E8, 0);
  task_3E8.y = 0.0f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_3E8);
  script_E05B();
  script_92E0(arg0 - 1);
  return;
}

static void script_BE52(void) {
  int32_t unused_FC;
  int32_t local_F4[2];
  int32_t local_F0;
  int32_t local_EC;
  Vector3 local_E0;
  int32_t local_DC;
  Vector3i local_D0;
  local_D0.x = 0;
  (&local_D0)->y = 0;
  script_9635();
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 411);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 411);
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_charaDisplay(CHARA_ONO_, CDM_Hide);
  local_F4[0] = 0;
  local_F4[1] = 0;
  local_F0 = 0;
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  task_388 = 2;
  local_EC = 0;
  Script_EVT_SetPosition(SHEN_NAME('LEAF'), POS_XYZ, &data_10918);
  Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, SHEN_NAME('DENW'));
  for (;;) {
    if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_DC = 0;
  for (;;) {
    local_DC = local_DC + 1;
    if (local_DC == 20) {
      break;
    }
    Script_InvestigationUtils(ShenmueUtils_UpdateInterpolatedSpeed);
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10948);
  {
    int32_t unused_B0[8];
    Vector3i local_A4;
    (&local_A4)->y = 0xC000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_A4, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_SetLookPointDetails(CHARA_AKIR, 12, &data_1096C, 1);
  (&local_D0)->z = -0x8000;
  Script_charaPosRotUtils(SHEN_NAME('DENW'), 152, &data_10978, &local_D0, CHARA_PART_Add);
  Script_ShowUIMessageFromID("E1012A119");
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_lightSetSlotEnabled(28, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_seqEnterEvtCam(6000, 0, AUTH_None);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_PlayCharaMotion(CHARA_AKIR, 4129, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Hide);
  Script_fixoAttachEx(SHEN_NAME('DENW'), CHARA_AKIR, 12, &data_10930, &data_1093C);
  Script_EVT_GetPosition(SHEN_NAME('LEAF'), -1, &local_E0, 0);
  Script_charaDisplay(SHEN_NAME('LEAF'), CDM_Show);
  task_258 = 0;
  for (;;) {
    int32_t unused_B0[8];
    int32_t local_AC;
    if ((task_258 > 35) & (task_258 < 135)) {
      (&local_E0)->y = (&local_E0)->y + (*(float *)(&data_10924 + 1) - (&data_10918)->y) / (float)100;
      local_E0.x = (&local_E0)->x + (*(float *)&data_10924 - (&data_10918)->x) / (float)100;
      (&local_E0)->z = (&local_E0)->z + (*(float *)(&data_10924 + 2) - (&data_10918)->z) / (float)100;
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_AC = task_258;
    switch (local_AC) {
      case 35:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 402);
        Script_scenCreateTask(script_E14A, 0);
        break;
      case 80:
        Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
        Script_seqEnterEvtCam(6001, 0, AUTH_None);
        break;
      case 150:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 411);
        Script_seqEnterEvtCam(6002, 0, AUTH_None);
        break;
      case 250:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 811);
        Script_fixoAttachEx(SHEN_NAME('LEAF'), CHARA_AKIR, 18, &data_10954, &data_10960);
        break;
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 226, 1);
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  {
    int32_t unused_B0[8];
    Vector3i local_A4;
    (&local_A4)->y = 0;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_A4, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 4130, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ShowUIMessageFromID("E1012A177");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A178");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_GetCassetteFlags(40, 1, 0, 0);
  SCNF_StartIconSystemMenu(40);
  (&local_D0)->z = 0x8000;
  Script_charaPosRotUtils(SHEN_NAME('DENW'), 152, &data_10978, &local_D0, CHARA_PART_Add);
  Script_fixoClearAttachment(SHEN_NAME('DENW'));
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_lightSetSlotEnabled(28, 1);
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, 0, 0);
  script_96C5();
  Script_charaDisplay(CHARA_ONO_, CDM_Show);
  Script_charaDisplay(SHEN_NAME('LEAF'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  Script_lightSetSlotEnabled(28, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  return;
}

static void script_C30F(void) {
  script_9635();
  Script_charaDisplay(CHARA_ONO_, CDM_Hide);
  Script_lightSpotLightEnable(28, &data_10A58, &data_10A64);
  Script_LightUtils(28, 2, &data_F680);
  Script_LightUtils(28, 4, 2.1f);
  Script_LightUtils(28, 5, 2.8f);
  Script_LightUtils(28, 6, data_F690);
  Script_LightUtils(28, 10, 4187);
  Script_lightSetSlotEnabled(28, 1);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_109A4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_109B0, 0);
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 411);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 411);
  Script_scenCreateTask(script_CA0B, 0);
  task_258 = 0;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4140, 0.0f, -1, -1, 1.0f, 0);
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  Script_charaDisplay(0x746E6564, CDM_Show);
  Script_EVT_SetPosition(0x746E6564, POS_XYZ, &data_10A40);
  Script_EVT_SetRotation(0x746E6564, 0x38000000, &data_10A4C, 0);
  for (;;) {
    Vector3 local_F4;
    int32_t local_F0;
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_F0 = task_258;
    switch (local_F0) {
      case 80:
        local_F4.x = 0.0f;
        (&local_F4)->z = 0.0f;
        (&local_F4)->y = 0.0f;
        Script_EVT_SetPosition(SHEN_NAME('BX4B'), POS_XYZ, &local_F4);
        Script_tmnmUtils(SHEN_NAME('BX4B'), 4171, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 92:
        Script_executeAicaCommand(0x2605A9, 0, 0);
        break;
      case 150:
        Script_executeAicaCommand(0x2205A9, 0, 0);
        break;
      case 195:
        Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_AKIR, 18, &data_109BC, &data_109C8);
        Script_executeAicaCommand(0x3305A9, 0, 0);
        break;
      case 307:
        Script_fixoClearAttachment(SHEN_NAME('PIPE'));
        Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_AKIR, 12, &data_109D4, &data_109E0);
        break;
      case 380:
        Script_executeAicaCommand(0x1505A9, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('PIPE'));
        Script_charaDisplay(SHEN_NAME('PIPE'), CDM_Hide);
        break;
      case 530:
        Script_fixoClearAttachment(SHEN_NAME('PIPE'));
        Script_charaDisplay(SHEN_NAME('PIPE'), CDM_Hide);
        Script_EVT_SetPosition(SHEN_NAME('PIPE'), POS_XYZ, &data_10A04);
        break;
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  task_258 = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4133, -1, -1, -1, 1.0f, 0);
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 0x8000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_seqEnterEvtCam(1110, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
  Script_EVT_SetPosition(SHEN_NAME('KAK1'), POS_XYZ, &data_10A10);
  Script_EVT_SetRotation(SHEN_NAME('KAK1'), 0x38000000, &data_10A1C, 0);
  Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Show);
  for (;;) {
    task_258 = task_258 + 1;
    if (task_258 == 150) {
      break;
    }
    if (task_258 == 50) {
      Script_ShowUIMessageFromID("E1012A208");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1012A209");
      SCNF_WaitFrames_0000(1);
      Script_seqEnterEvtCam(1111, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
    }
    SCNF_WaitFrames_0000(1);
  }
  task_258 = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4137, -1, -1, -1, 1.0f, 0);
  for (;;) {
    if (task_258 == 27) {
      Script_seqEnterEvtCam(1120, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
      Script_EVT_SetPosition(SHEN_NAME('KAK1'), POS_XYZ, &data_10A28);
      Script_EVT_SetRotation(SHEN_NAME('KAK1'), 0x38000000, &data_10A34, 0);
      Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  task_258 = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4136, -1, -1, -1, 1.0f, 0);
  Script_executeAicaCommand(0x3805A9, 0, 0);
  for (;;) {
    task_258 = task_258 + 1;
    if (task_258 == 100) {
      Script_executeAicaCommand(0x3905A9, 0, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_258 = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4139, -1, -1, -1, 1.0f, 0);
  Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_AKIR, 12, &data_109EC, &data_109F8);
  for (;;) {
    Vector3 local_F4;
    int32_t local_F0;
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_F0 = task_258;
    switch (local_F0) {
      case 20:
        Script_charaDisplay(SHEN_NAME('PIPE'), CDM_Show);
        break;
      case 62:
        Script_seqEnterEvtCam(1121, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
        break;
      case 142:
        Script_fixoClearAttachment(SHEN_NAME('PIPE'));
        break;
      case 162:
        Script_seqEnterEvtCam(1122, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
        break;
      case 182:
        local_F4.x = 0.0f;
        (&local_F4)->z = 0.0f;
        (&local_F4)->y = 0.0f;
        Script_EVT_SetPosition(SHEN_NAME('BX4B'), POS_XYZ, &local_F4);
        Script_executeAicaCommand(0x2205A9, 0, 0);
        Script_tmnmUtils(SHEN_NAME('BX4B'), 4172, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 230:
        Script_executeAicaCommand(0x2705A9, 0, 0);
        break;
      case 250:
        break;
      case 320:
        break;
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
  Script_CharHandlers(CHARA_AKIR, 1, 13, 16);
  Script_EVT_SetPosition(0x746E6564, POS_XYZ, &data_10A28);
  Script_EVT_SetRotation(0x746E6564, 0x38000000, &data_10A34, 0);
  Script_charaDisplay(0x746E6564, CDM_Hide);
  Script_lightSetSlotEnabled(28, 0);
  Script_tmnmReset(SHEN_NAME('BX4B'));
  Script_charaDisplay(CHARA_ONO_, CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = 2.126f;
    (&local_F4)->y = 0.489f;
    (&local_F4)->z = 0.922f;
    Script_EVT_SetPosition(SHEN_NAME('BX4B'), POS_XYZ, &local_F4);
  }
  Script_CreateNPCImageModule(SHEN_NAME('BX4B'), 0, 0);
  Script_CreateNPCImageModule(SHEN_NAME('BX4B'), "HAKO_4B", 0);
  script_96C5();
  return;
}

static int32_t script_CA0B(void) {
  Script_seqEnterEvtCam(1106, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
  SCNF_WaitFrames_0000(70);
  Script_seqEnterEvtCam(1107, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
  SCNF_WaitFrames_0000(210);
  Script_seqEnterEvtCam(1108, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
  SCNF_WaitFrames_0000(100);
  Script_seqEnterEvtCam(1109, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
  end:;
}

static void script_CA6B(void) {
  int32_t unused_F4[3];
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3 local_AC;
  Vector3 local_A0;
  Vector3 local_94;
  Vector3 local_88;
  Vector3 local_FF7C;
  Vector3 local_FF70;
  Vector3 local_FF64;
  script_9635();
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 26, 1);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10A8C);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10A98, 0);
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_seqEnterEvtCam(1112, 0, AUTH_None);
  SCNF_WaitFrames_0000(1);
  task_258 = 120;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4135, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_258 = 0;
  Script_EVT_GetPosition(SHEN_NAME('TANR'), -1, &local_D0, 0);
  Script_EVT_GetPosition(SHEN_NAME('BALL'), -1, &local_B8, 0);
  Script_EVT_GetPosition(SHEN_NAME('UBOX'), -1, &local_A0, 0);
  Script_EVT_GetPosition(SHEN_NAME('YAK2'), -1, &local_88, 0);
  Script_EVT_GetPosition(SHEN_NAME('FUDA'), -1, &local_FF70, 0);
  (&local_C4)->z = (&local_D0)->z;
  (&local_C4)->y = 0.0f;
  (&local_94)->z = (&local_A0)->z;
  (&local_94)->y = (&local_A0)->y;
  (&local_AC)->z = (&local_B8)->z;
  (&local_AC)->y = (&local_B8)->y;
  (&local_FF7C)->z = (&local_88)->z;
  (&local_FF7C)->y = (&local_88)->y;
  (&local_FF64)->y = (&local_FF70)->y;
  (&local_FF64)->z = (&local_FF70)->z;
  local_C4.x = 8.0f;
  Script_fixoAttachEx(SHEN_NAME('DAMY'), CHARA_AKIR, 18, &data_10AA4, &data_10AB0);
  task_258 = 0;
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    {
      int32_t unused_FF58[3];
      int32_t local_FF54;
      local_FF54 = task_258;
      if (local_FF54 == 90) {
        Script_seqEnterEvtCam(1116, 0, AUTH_None);
        goto L00CC93;
      }
    }
    L00CC93:;
    Script_motiSetAngFlags(CHARA_AKIR, 1);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
    if (task_258 == 85) {
      Script_EVT_GetPosition(SHEN_NAME('DAMY'), -1, &local_E8, 0);
    }
    if ((task_258 > 84) & (task_258 < 348)) {
      Script_EVT_GetPosition(SHEN_NAME('DAMY'), -1, &local_DC, 0);
      local_DC.x = (&local_DC)->x - (&local_E8)->x;
      if (!((&local_DC)->x + (&local_D0)->x >= (&local_C4)->x)) {
        local_C4.x = (&local_D0)->x + (&local_DC)->x;
        local_AC.x = (&local_B8)->x + (&local_DC)->x;
        local_94.x = (&local_A0)->x + (&local_DC)->x;
        local_FF7C.x = (&local_88)->x + (&local_DC)->x;
        local_FF64.x = (&local_FF70)->x + (&local_DC)->x;
      }
      Script_EVT_SetPosition(SHEN_NAME('TANR'), POS_XYZ, &local_C4);
      Script_EVT_SetPosition(SHEN_NAME('BALL'), POS_XYZ, &local_AC);
      Script_EVT_SetPosition(SHEN_NAME('UBOX'), POS_XYZ, &local_94);
      Script_EVT_SetPosition(SHEN_NAME('YAK2'), POS_XYZ, &local_FF7C);
      Script_EVT_SetPosition(SHEN_NAME('FUDA'), POS_XYZ, &local_FF64);
    }
    {
      int32_t unused_FF58[3];
      int32_t local_FF54;
      local_FF54 = task_258;
      switch (local_FF54) {
        case 120:
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          break;
        case 170:
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          break;
        case 220:
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          break;
        case 267:
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          break;
        case 320:
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
    task_258 = task_258 + 1;
  }
  Script_EVT_SetPosition(SHEN_NAME('MUKA'), POS_XYZ, &data_10AC8);
  Script_fixoClearAttachment(SHEN_NAME('DAMY'));
  local_E8.x = 3.15f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.68f;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SHELF_MOVED, 1);
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, &data_10ABC);
  script_96C5();
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_E8);
  if ((Script_GetCurrentEntryPoint() == 3) | (Script_GetCurrentEntryPoint() == 5)) {
    SAVE_Script_UpdateForSaveData(3, 0, 300);
  } else {
    SAVE_Script_UpdateForSaveData(3, 0, (8 - Script_GetCurrentEntryPoint() * 3) * 1800);
  }
  return;
}

static void script_CED4(void) {
  Vector3 local_F4;
  int32_t local_E8[3];
  int32_t unused_E4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  local_F4.x = 0.6872f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 3.3065f;
  local_E8[0] = 0;
  local_E8[1] = 0x4000;
  local_E8[2] = 0;
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_SetGameStateTo(AUTH_None);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, 0x4000, MN_AKI_AKI_STAND_L, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
    Script_GetCameraInputDetails(15);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 4147, 55.0f, 140.0f, -1, 1.5f, 0);
  script_9853(0x52494B41, 100.0f);
  Script_executeAicaCommand(0x2005A9, 0, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890) == 0) {
    Script_lightSetSlotEnabled(2, 1);
  } else {
    Script_charaDisplay(0x316B6E64, CDM_Hide);
    Script_charaDisplay(0x306B6E64, CDM_Show);
    Script_lightSetSlotEnabled(2, 0);
    Script_lightSetSlotEnabled(3, 0);
  }
  script_9872(CHARA_AKIR);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890) == 0) {
    Script_charaDisplay(0x306B6E64, CDM_Hide);
    Script_charaDisplay(0x316B6E64, CDM_Show);
    Script_lightSetSlotEnabled(2, 1);
    Script_lightSetSlotEnabled(3, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 890, 1);
    task_2F0 = 0;
  } else {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 890, 0);
  }
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_D07B(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 1;
  local_F8 = 0;
  for (;;) {
    if (local_FC % 6 == 0) {
      Script_lightSetSlotEnabled(2, 1);
    } else {
      Script_lightSetSlotEnabled(2, 0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (local_FC == 40) {
      break;
    }
  }
  return;
}

void script_D0C9(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ryoSleepUtils(2, 1);
  SCNT_Enqueue();
  Script_MotStepUtil(CHARA_AKIR, 260);
  task_2F4 = Script_ryoSleepUtils(0);
  Script_SetMapFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 15.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
  Script_SetCameraFieldOfView(10922);
  task_318 = 0;
  task_2B8 = 0;
  task_2D4 = 0;
  task_2D0 = 0;
  task_2CC = 0;
  task_2E8 = 0;
  Script_cpctStateUtils(1, 2);
  Script_SetDynamicMapTransform(1, &data_ED58, 0);
  Script_mapmSetCamFlags(1, 1, 0);
  Script_mapmSetCamFlags(1, 0, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE)) {
    Script_SetShouldSleepFlag(0);
    script_D28B(2, 0x4F4D4F4A, 0);
    Script_SetShouldSleepFlag(1);
  } else {
    Script_SetShouldSleepFlag(0);
    script_D28B(2, 0x4F4D4F4A, 1);
    Script_SetShouldSleepFlag(1);
  }
  Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('FIR5'), CDM_Hide);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 8);
  Script_ryoControl(PCCTL_SetCPCTFlag400, 1);
  Script_LoadVoiceFile(1, data_10AD4);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  script_8966();
  Script_scenCreateTask(script_64D3, 0);
  Script_scenCreateTask(script_D369, 1, CHARA_AKIR);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN) == 0) {
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 0);
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 22, 48);
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 26, 0);
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, 255);
  }
  if (task_2F4 != -1) {
    script_DE5D();
  }
  return;
}

static void script_D28B(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_D8[7];
  Script_ReturnTimeDate(&local_F4);
  local_D8[0] = 22;
  local_D8[1] = 30;
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DKTY_TUBA_CUTSCENE_COMPLETE) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE) == 0)) {
    if (arg2) {
      if (((&local_F4)->hours >= 0) & ((&local_F4)->hours <= 4)) {
        local_D8[0] = 3;
        local_D8[1] = 30;
        local_D8[2] = 4;
        local_D8[3] = 0;
      } else {
        local_D8[0] = 27;
        local_D8[1] = 30;
        local_D8[2] = 28;
        local_D8[3] = 0;
      }
    } else {
      local_D8[0] = 22;
      local_D8[1] = 30;
      local_D8[2] = 23;
      local_D8[3] = 30;
    }
    local_D8[4] = arg0;
    local_D8[5] = arg1;
    local_D8[6] = 4;
    Script_SetHomeArea((const void *)local_D8);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10B84, 3, data_10BA4)))
static int32_t script_D369(int32_t arg0) {
  int32_t local_E8[6];
  Vector3i local_DC;
  int32_t unused_D8;
  Vector3 local_CC;
  int32_t unused_BC[4];
  int32_t local_B8;
  int32_t local_B4;
  task_658 = 0;
  task_2E4 = 0;
  task_320 = 0;
  task_25C = 0;
  local_B4 = 0;
  (void)((task_250 = 0) & 4);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  Script_LoadHandMot(2, SHEN_NAME('YDB1'), "HMOT_DB1.BIN");
  Script_LoadHandMot(2, SHEN_NAME('YDB1'), "HMOT_STD.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_clmdSetFlags(arg0, 2, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 10, 0);
  script_0FBF();
  Script_charaSetClipAngle(SHEN_NAME('TANR'), 15.0f);
  Script_charaSetClipAngle(SHEN_NAME('UBOX'), 15.0f);
  Script_charaSetClipAngle(SHEN_NAME('BALL'), 15.0f);
  Script_charaSetClipAngle(SHEN_NAME('FUDA'), 15.0f);
  local_E8[0] = 0x40FE2824;
  local_E8[1] = 0x3F98BE0E;
  local_E8[2] = 0x3FB91340;
  local_E8[3] = 0x3F800000;
  local_E8[4] = 0;
  local_E8[5] = 0;
  local_DC.x = 0;
  (&local_DC)->y = 0;
  (&local_DC)->z = 0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_CC, 0);
  if (task_2F4 != -1) {
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    script_E205(CHARA_AKIR);
  } else {
    Script_ryoSleepUtils(1);
    script_E779();
    Script_hmdlApplyScale(SHEN_NAME('UBOX'), &data_10AE8);
    Script_charaSetClipAngle(SHEN_NAME('TANR'), 15.0f);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
      Script_charaDisplay(0x306B6E64, CDM_Show);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890)) {
        Script_charaDisplay(0x316B6E64, CDM_Show);
        Script_lightSetSlotEnabled(2, 1);
        Script_lightSetSlotEnabled(3, 1);
        Script_scenToggleCharaFlag(0, 100, 0);
      }
    }
    Script_scenCreateTask(script_8AA4, 0);
    Script_scenCreateTask(script_294C, 0);
    Script_scenCreateTask(script_3CA6, 0);
    script_DB26();
    SCNF_WaitFrames_0000(30);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Script_shenClearFreeState(1);
  }
  Script_CreateNPCImageModule(SHEN_NAME('DAMY'), 0, 0);
  if (task_2E4) {
    Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
    task_2E4 = 1;
    script_279C(1);
    script_D9A7(CHARA_AKIR, SHEN_NAME('LIGH'));
    Script_lightSetSlotEnabled(15, 1);
    SCNF_WaitFrames_0000(1);
    Script_uiUtils(0, 26, 0);
    Script_CharHandlers(CHARA_AKIR, 1, 3, 16);
    Script_scenToggleCharaFlag(0, 100, 0);
    Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 1);
  }
  if (task_2CC) {
    Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
    Script_fixoAttachEx(CHARA_ONO_, CHARA_AKIR, 18, &data_10B6C, &data_10B78);
  }
  task_25C = 0;
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  local_B8 = 0;
  while (task_2B8 == 0) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_10B00, &data_10B0C, 0) & Script_IsPlayerAllowed()) {
      if (local_B4 == 1) {
        Script_SetShouldSleepFlag(0);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
        Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
        Script_shenClearFreeState(0);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        (void)((task_250 = -1) & 1);
        Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
        script_DA9B(0x52494B41, SHEN_NAME('LIGH'));
        task_2E4 = 0;
        SCNF_WaitFrames_0000(1);
        Script_SetRyoMotionDetails(arg0);
        script_E05B();
        script_D28B(2, 0x4F4D4F4A, 0);
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('JHD0'), 6);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      local_B4 = 1;
    }
    if (task_2CC == 1) {
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_10B54, &data_10B60, 0) & Script_IsPlayerAllowed()) {
        script_991C();
        task_2CC = 0;
      }
    }
    if (task_2F0 == 1) {
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_10B18, &data_10B24, 0)) {
        if (-(local_B8 == 0) & -(task_2F8 == 0) & Script_IsPlayerAllowed()) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          SCNF_WaitFrames_0000(1);
          if (task_2E4 == 0) {
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_ShowUIMessageFromID("E1012A083");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            {
              Vector3i local_A8;
              (&local_A8)->y = 0x8000;
              Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_A8, 0);
            }
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TURN180_WAD_R, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(20);
            {
              Vector3i local_A8;
              (&local_A8)->y = 0;
              Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_A8, 0);
            }
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_UP1_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(30);
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            local_B8 = 1;
            if (Script_GetOwnedItemCount(51, 0) == 0) {
              task_658 = task_658 + 1;
              if (task_658 == 2) {
                task_650 = 1;
                task_658 = task_658 + 1;
              }
            }
          }
          Script_ryoControl(RYO_RestoreControl);
        }
      } else {
        local_B8 = 0;
      }
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_10B3C, &data_10B48, 0)) {
      Script_EVT_ApplyRot((void *)local_E8, &local_DC);
    } else {
      Script_EVT_ApplyRot(0, &local_DC);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_D9A7(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.124f;
  (&local_F4)->y = 0.056f;
  (&local_F4)->z = 0.019f;
  local_E8.x = 13378;
  (&local_E8)->y = 17293;
  (&local_E8)->z = 3639;
  Script_fixoAttachEx(arg1, arg0, 12, &local_F4, &local_E8);
  Script_charaDisplay(arg1, CDM_Show);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 3);
  Script_CharHandlers(arg0, 1, 0, 16);
  Script_CharHandlers(arg0, 0, 2, 16);
  task_2F0 = 0;
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_DA2E(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 3);
  SCNF_WaitFrames_0000(1);
  local_F4.x = 0.124f;
  (&local_F4)->y = 0.056f;
  (&local_F4)->z = 0.019f;
  local_E8.x = 13378;
  (&local_E8)->y = 17293;
  (&local_E8)->z = 3639;
  Script_fixoAttachEx(arg1, arg0, 12, &local_F4, &local_E8);
  Script_charaDisplay(arg1, CDM_Show);
  task_2F0 = 0;
  return;
}

static void script_DA9B(int32_t arg0, int32_t arg1) {
  int32_t unused_E8[6];
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  if (Script_charaDisplay(arg1, CDM_Query)) {
    Script_fixoClearAttachment(arg1);
  }
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_charaDisplay(arg1, CDM_Hide);
  return;
}

static void script_DADF(void) {
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
    SCNF_WaitFrames_0000(2);
  }
  Script_SetCurrentScrollEnabled(0);
  Script_SetFogFlags(2, 0);
  return;
}

static void script_DB26(void) {
  int32_t local_FC;
  local_FC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 26);
  switch (local_FC) {
    case 0:
      Script_scenCreateTask(script_9AA4, 0);
      break;
    case 1:
      Script_EVT_SetPosition(SHEN_NAME('YAK2'), POS_XYZ, &data_10BBC);
      Script_EVT_SetPosition(SHEN_NAME('UBOX'), POS_XYZ, &data_10BC8);
      Script_EVT_SetPosition(SHEN_NAME('BALL'), POS_XYZ, &data_10BD4);
      Script_EVT_SetPosition(SHEN_NAME('FUDA'), POS_XYZ, &data_10C1C);
      Script_EVT_SetPosition(SHEN_NAME('TANR'), POS_XYZ, &data_10BB0);
      task_2D4 = 1;
      Script_charaDisplay(SHEN_NAME('MUKA'), CDM_Hide);
      Script_scenCreateTask(script_9AA4, 0);
      Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, &data_10C28);
      break;
    case 2:
      Script_EVT_SetPosition(SHEN_NAME('YAK2'), POS_XYZ, &data_10BBC);
      Script_EVT_SetPosition(SHEN_NAME('UBOX'), POS_XYZ, &data_10BC8);
      Script_EVT_SetPosition(SHEN_NAME('BALL'), POS_XYZ, &data_10BD4);
      Script_EVT_SetPosition(SHEN_NAME('FUDA'), POS_XYZ, &data_10C1C);
      Script_EVT_SetPosition(SHEN_NAME('TANR'), POS_XYZ, &data_10BB0);
      task_2D4 = 1;
      Script_charaDisplay(SHEN_NAME('MUKA'), CDM_Hide);
      Script_scenCreateTask(script_9DC6, 0);
      Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, &data_10C28);
      break;
    case 3:
      Script_EVT_SetPosition(SHEN_NAME('YAK2'), POS_XYZ, &data_10BBC);
      Script_EVT_SetPosition(SHEN_NAME('UBOX'), POS_XYZ, &data_10BC8);
      Script_EVT_SetPosition(SHEN_NAME('BALL'), POS_XYZ, &data_10BD4);
      Script_EVT_SetPosition(SHEN_NAME('FUDA'), POS_XYZ, &data_10C1C);
      Script_EVT_SetPosition(SHEN_NAME('TANR'), POS_XYZ, &data_10BB0);
      task_2D4 = 1;
      Script_charaDisplay(SHEN_NAME('MUKA'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('KABE'), CDM_Hide);
      Script_scenCreateTask(script_E531, 1);
      Script_scenCreateTask(script_A2FD, 0);
      break;
    case 4:
      task_2D4 = 1;
      Script_charaDisplay(SHEN_NAME('MUKA'), CDM_Hide);
      task_254 = 12;
      Script_charaDisplay(SHEN_NAME('KABE'), CDM_Hide);
      Script_scenCreateTask(script_E531, 1);
      Script_EVT_SetRotation(SHEN_NAME('KINK'), 0x38000000, &data_10BE0, 0);
      Script_EVT_SetPosition(SHEN_NAME('TANR'), POS_XYZ, &data_10BB0);
      Script_EVT_SetPosition(SHEN_NAME('YAK2'), POS_XYZ, &data_10BBC);
      Script_EVT_SetPosition(SHEN_NAME('UBOX'), POS_XYZ, &data_10BC8);
      Script_EVT_SetPosition(SHEN_NAME('BALL'), POS_XYZ, &data_10BD4);
      Script_EVT_SetPosition(SHEN_NAME('FUDA'), POS_XYZ, &data_10C1C);
      Script_EVT_SetRotation(SHEN_NAME('KINK'), 0x38000000, &data_10BE0, 0);
      Script_EVT_SetPosition(SHEN_NAME('HKHK'), POS_XYZ, &data_10BEC);
      Script_EVT_SetRotation(SHEN_NAME('HKHK'), 0x38000000, &data_10BF8, 0);
      Script_EVT_SetPosition(SHEN_NAME('HKFT'), POS_XYZ, &data_10C04);
      Script_EVT_SetRotation(SHEN_NAME('HKFT'), 0x38000000, &data_10C10, 0);
      Script_EVT_SetPosition(CHARA_ONO_, POS_XYZ, &data_10C34);
      Script_EVT_SetRotation(CHARA_ONO_, 0x38000000, &data_10C40, 0);
      break;
  }
  return;
}

static void script_DE5D(void) {
  int32_t local_FC;
  local_FC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20);
  {
    int32_t local_F8;
    local_F8 = (local_FC & 0x30) >> 4;
    switch (local_F8) {
      case 0:
        *(int16_t *)task_2FC = 240;
        task_304[0] = 0;
        break;
      case 1:
        *(int16_t *)task_2FC = 240;
        task_304[0] = 1;
        break;
      case 2:
        *(int16_t *)task_2FC = 240;
        task_304[0] = 0;
        break;
      case 3:
        *(int16_t *)task_2FC = 240;
        task_304[0] = 1;
        break;
    }
  }
  {
    int32_t local_F8;
    local_F8 = (local_FC & 0xC) >> 2;
    switch (local_F8) {
      case 0:
        *(int16_t *)((int8_t *)task_2FC + 2) = 240;
        task_304[1] = 0;
        break;
      case 1:
        *(int16_t *)((int8_t *)task_2FC + 2) = 240;
        task_304[1] = 1;
        break;
      case 2:
        *(int16_t *)((int8_t *)task_2FC + 2) = 240;
        task_304[1] = 0;
        break;
      case 3:
        *(int16_t *)((int8_t *)task_2FC + 2) = 120;
        task_304[1] = 1;
        break;
    }
  }
  {
    int32_t local_F8;
    local_F8 = local_FC & 3;
    switch (local_F8) {
      case 0:
        *(int16_t *)((int8_t *)task_2FC + 4) = 240;
        task_304[2] = 0;
        break;
      case 1:
        *(int16_t *)((int8_t *)task_2FC + 4) = 240;
        task_304[2] = 1;
        break;
      case 2:
        *(int16_t *)((int8_t *)task_2FC + 4) = 240;
        task_304[2] = 0;
        break;
      case 3:
        *(int16_t *)((int8_t *)task_2FC + 4) = 60;
        task_304[2] = 1;
        break;
    }
  }
  {
    int32_t local_F8;
    local_F8 = (local_FC & 0xC0) >> 6;
    if (local_F8 == 1) {
      *(int16_t *)((int8_t *)task_2FC + 6) = 240;
      task_304[3] = 1;
    } else {
      if ((local_F8 == 0) | (local_F8 == 2) | (local_F8 == 3)) {
        *(int16_t *)((int8_t *)task_2FC + 6) = 240;
        task_304[3] = 0;
        goto L00E05A;
      }
    }
  }
  L00E05A:;
  return;
}

static void script_E05B(void) {
  int16_t local_FE;
  if ((*(int16_t *)task_2FC == 240) & (*task_304 == 1)) {
    local_FE = 48;
  } else {
    local_FE = 0;
  }
  if ((*(int16_t *)((int8_t *)task_2FC + 2) == 120) & (task_304[1] == 1)) {
    local_FE = local_FE | 0xC;
  } else {
    if (task_304[1] == 1) {
      local_FE = local_FE | 4;
      goto L00E0BF;
    }
  }
  L00E0BF:;
  if ((*(int16_t *)((int8_t *)task_2FC + 4) == 60) & (task_304[2] == 1)) {
    local_FE = local_FE | 3;
  } else {
    if (task_304[2] == 1) {
      local_FE = local_FE | 1;
      goto L00E0FE;
    }
  }
  L00E0FE:;
  if ((*(int16_t *)((int8_t *)task_2FC + 6) == 240) & (task_304[3] == 0)) {
    local_FE = local_FE | 0xC0;
  } else {
    if (task_304[3] == 1) {
      local_FE = local_FE | 0x40;
      goto L00E13F;
    }
  }
  L00E13F:;
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, local_FE);
  return;
}

__attribute__((scn_string_bindings(0, data_10C4C)))
static int32_t script_E14A(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t unused_D8[3];
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  local_E4 = 0;
  L00E16B:;
  Script_InterpolateAndScaleAngles(0, local_E4, &local_F4, &local_E8, "DYN3");
  local_F4.x = (&local_F4)->x * (float)10 + 7.0521f;
  (&local_F4)->y = (&local_F4)->y * (float)10 - 0.05f;
  (&local_F4)->z = (&local_F4)->z * (float)10 + 4.5825f;
  Script_EVT_SetPosition(SHEN_NAME('LEAF'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('LEAF'), 0x38000000, &local_E8, 0);
  local_E4 = local_E4 + 1;
  SCNF_WaitFrames_0000(1);
  if (!(local_E4 > 154)) goto L00E16B;
  return Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
}

static void script_E205(int32_t arg0) {
  (void)((task_250 = -1) & 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN) == 0) {
    Script_seqEnterEvtCam(10, 0, AUTH_None);
  } else {
    Script_seqEnterEvtCam(15, 0, AUTH_None);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN) == 0) {
    Script_PlayCharaMotion(arg0, 4103, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(arg0, 4103, 0.0f, 82.0f, -1, 1.0f, 0);
  }
  for (;;) {
    int32_t local_FC;
    task_258 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_FC = task_258;
    switch (local_FC) {
      case 5:
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN) == 0) {
          Script_ShowUIMessageFromID("E1012A001");
        } else {
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE) == 0) {
            Script_ShowUIMessageFromID("E1012A004");
          }
        }
        break;
      case 6:
        Script_executeAicaCommand(0x2805A9, 0, 0);
        break;
      case 24:
        Script_executeAicaCommand(0x2805A9, 0, 0);
        break;
      case 70:
        Script_executeAicaCommand(0x3605A9, 0, 0);
        Script_scenCreateTask(script_AC85, 0);
        break;
      case 110:
        Script_seqEnterEvtCam(11, 0, AUTH_None);
        break;
      case 225:
        Script_executeAicaCommand(0x1205A9, 0, 0);
        break;
      case 235:
        Script_executeAicaCommand(0x1305A9, 0, 0);
        break;
      case 180:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN, 1);
  (void)((task_250 = 0) & 0x10);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10C70);
  {
    Vector3i local_F4;
    (&local_F4)->y = 0x8000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(10);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ryoControl(RYO_RestoreControl);
  Script_hmdlApplyScale(SHEN_NAME('UBOX'), &data_10C64);
  Script_charaSetClipAngle(SHEN_NAME('TANR'), 15.0f);
  Script_charaSetClipAngle(SHEN_NAME('UBOX'), 15.0f);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
    Script_charaDisplay(0x306B6E64, CDM_Show);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890)) {
      Script_charaDisplay(0x316B6E64, CDM_Show);
      Script_lightSetSlotEnabled(2, 1);
      Script_lightSetSlotEnabled(3, 1);
      Script_scenToggleCharaFlag(0, 100, 0);
    }
  }
  Script_scenCreateTask(script_8AA4, 0);
  Script_scenCreateTask(script_294C, 0);
  Script_scenCreateTask(script_3CA6, 0);
  script_DB26();
  script_E8E9();
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(1, data_10CEB)))
static int32_t script_E531(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = 2;
  Script_sprintf("start\012");
  {
    int32_t local_E0;
    local_E0 = 0;
    L00E548:;
    Script_charaDisplay(*(&data_10C9C + local_E0), CDM_Show);
    local_E0 = local_E0 + 1;
    if (!(local_E0 >= 18)) goto L00E548;
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  L00E585:;
  {
    int32_t local_E0;
    local_E0 = 0;
    L00E58B:;
    Script_InterpolateAndScaleAngles(local_E0, local_E4, &local_F4, &local_E8, "DYN1");
    local_F4.x = (&local_F4)->x * (float)10;
    (&local_F4)->y = (&local_F4)->y * (float)10;
    (&local_F4)->z = (&local_F4)->z * (float)10;
    Script_EVT_SetPosition(*(&data_10C9C + local_E0), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(*(&data_10C9C + local_E0), 0x38000000, &local_E8, 0);
    local_E0 = local_E0 + 1;
    if (!(local_E0 >= 18)) goto L00E58B;
  }
  local_E4 = local_E4 + 2;
  SCNF_WaitFrames_0000(1);
  if (!(local_E4 > 65)) goto L00E585;
  return Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
}

static void script_E632(void) {
  Script_ryoSleepUtils(3, 1, 0, task_2CC);
  Script_ryoSleepUtils(3, 1, 1, *(int16_t *)task_2FC);
  Script_ryoSleepUtils(3, 1, 2, *(int16_t *)((int8_t *)task_2FC + 2));
  Script_ryoSleepUtils(3, 1, 3, *(int16_t *)((int8_t *)task_2FC + 4));
  Script_ryoSleepUtils(3, 1, 4, *(int16_t *)((int8_t *)task_2FC + 6));
  Script_ryoSleepUtils(3, 1, 5, *task_304);
  Script_ryoSleepUtils(3, 1, 6, task_304[1]);
  Script_ryoSleepUtils(3, 1, 7, task_304[2]);
  Script_ryoSleepUtils(3, 1, 8, task_304[3]);
  Script_ryoSleepUtils(3, 1, 9, *(int8_t *)&task_674[0][4]);
  Script_ryoSleepUtils(3, 1, 10, *(int8_t *)&task_674[0][5]);
  Script_ryoSleepUtils(3, 1, 11, *(int8_t *)&task_674[1][4]);
  Script_ryoSleepUtils(3, 1, 12, *(int8_t *)&task_674[1][5]);
  Script_ryoSleepUtils(3, 1, 13, *(int8_t *)&task_674[2][4]);
  Script_ryoSleepUtils(3, 1, 14, *(int8_t *)&task_674[2][5]);
  Script_ryoSleepUtils(3, 1, 15, *(int8_t *)&task_674[3][4]);
  Script_ryoSleepUtils(3, 1, 16, *(int8_t *)&task_674[3][5]);
  Script_ryoSleepUtils(3, 1, 17, task_2E4);
  Script_ryoSleepUtils(3, 1, 18, task_320);
  return;
}

static void script_E779(void) {
  task_2CC = Script_ryoSleepUtils(3, 0, 0);
  *(int16_t *)task_2FC = Script_ryoSleepUtils(3, 0, 1);
  *(int16_t *)((int8_t *)task_2FC + 2) = Script_ryoSleepUtils(3, 0, 2);
  *(int16_t *)((int8_t *)task_2FC + 4) = Script_ryoSleepUtils(3, 0, 3);
  *(int16_t *)((int8_t *)task_2FC + 6) = Script_ryoSleepUtils(3, 0, 4);
  task_304[0] = Script_ryoSleepUtils(3, 0, 5);
  task_304[1] = Script_ryoSleepUtils(3, 0, 6);
  task_304[2] = Script_ryoSleepUtils(3, 0, 7);
  task_304[3] = Script_ryoSleepUtils(3, 0, 8);
  *(int8_t *)&task_674[0][4] = Script_ryoSleepUtils(3, 0, 9);
  *(int8_t *)&task_674[0][5] = Script_ryoSleepUtils(3, 0, 10);
  *(int8_t *)&task_674[1][4] = Script_ryoSleepUtils(3, 0, 11);
  *(int8_t *)&task_674[1][5] = Script_ryoSleepUtils(3, 0, 12);
  *(int8_t *)&task_674[2][4] = Script_ryoSleepUtils(3, 0, 13);
  *(int8_t *)&task_674[2][5] = Script_ryoSleepUtils(3, 0, 14);
  *(int8_t *)&task_674[3][4] = Script_ryoSleepUtils(3, 0, 15);
  *(int8_t *)&task_674[3][5] = Script_ryoSleepUtils(3, 0, 16);
  task_2E4 = Script_ryoSleepUtils(3, 0, 17);
  task_320 = Script_ryoSleepUtils(3, 0, 18);
  return;
}

static void script_E8E9(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('HN01'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN02'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN03'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN04'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN05'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN06'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN07'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN08'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN09'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN10'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN11'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN12'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN13'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN14'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN15'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN16'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN17'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN18'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HOKY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('SHAS'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TUB2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TUB3'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HIK1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HIK2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HIK3'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YAKU'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YAK2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BNT0'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('KKEY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('DAMY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('MKDE'), 0);
  Script_scenToggleVisibleFor(CHARA_ONO_, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BALL'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BOOK'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BX4A'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BX8A'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('SAKA'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('DENW'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('LEAF'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TIRA'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('KNIS'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS0'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS3'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS4'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS5'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('PIPE'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('UBOX'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('KKAZ'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('KUSK'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('MAK2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('KAK1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ROU1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO11'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO14'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ROU2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO21'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO24'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ROU3'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO31'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO34'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ROU4'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO41'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO44'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('IROU'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HROU'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('MATB'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('FUDA'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('DOGI'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('SKDI'), 0);
  Script_scenToggleVisibleFor(0x316B6E64, 0);
  Script_scenToggleVisibleFor(0x306B6E64, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('LIGH'), 0);
  Script_scenToggleVisibleFor(0x746E6564, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HKFT'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HKHK'), 0);
  return;
}
