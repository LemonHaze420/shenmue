#include "act03_mk80.h"

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
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_2398);
  Script_EVT_SetRotation(arg0, 0x38000000, &data_23A4, 0);
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

void script_0FBF(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(5);
  Script_ryoSetChara(CHARA_AKIR);
  Script_scenCreateTask(script_1015, 1, Script_GetCurrentEntryPoint());
  task_250 = 0;
  for (;;) {
    task_250 = task_250 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1015(int32_t arg0) {
  for (;;) {
    Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
    script_1041(arg0);
  }
  return Script_srchControl(0);
}

static void script_1041(int32_t arg0) {
  if (arg0) {
    script_190F(arg0);
    script_1A59(arg0);
    script_1A35(arg0);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 547, 1);
    Script_shenRequestGameModuleTransition(3, SHEN_NAME('MFSY'), 1);
    SCNF_WaitFrames_0000(1);
  } else {
    script_10C3(arg0);
    script_120D(arg0);
    script_11E9(arg0);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 549, 1);
    Script_shenRequestGameModuleTransition(3, SHEN_NAME('MFSY'), 1);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2578, 1, data_2584, 2, data_258F, 3, data_259C, 4, data_25A3)))
static void script_10C3(int32_t arg0) {
  task_254 = 0;
  script_0BED();
  script_0C48(CHARA_AKIR, -0x40000000);
  script_0C48(CHARA_KISY, -0x40000000);
  script_0DBA(SHEN_NAME('WBK1'), 0);
  script_0DBA(SHEN_NAME('WBK2'), 0);
  script_0DBA(SHEN_NAME('WBK3'), 0);
  script_02DF(CHARA_AKIR, 0x304D5544, -1);
  script_02DF(CHARA_KISY, 0x304D5544, -1);
  script_02DF(SHEN_NAME('DUMA'), 0x52494B41, -1);
  script_02DF(SHEN_NAME('DUMK'), 0x5953494B, -1);
  Script_LoadSND(0, 0, "system1.snd", 0);
  Script_LoadSND(0, 2, "ykm004.snd", 0);
  Script_LoadSND(0, 4, "e1hidmak.snd", 0);
  Script_LoadVoiceFile(1, "SA1108");
  task_258 = Script_motiLoad(3, SHEN_NAME('MK80'), "M_MK80.BIN");
  Script_SetMapFlags(0, 0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
  Script_shenSetLightingMode(2);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_SetCurrentScrollEnabled(0);
  return;
}

__attribute__((scn_string_bindings(0, data_25AE)))
static void script_11E9(int32_t arg0) {
  task_254 = 1;
  Script_LoadVoiceFile(0, "SA1108");
  Script_ReallocateUNFO(*&task_258);
  script_0E0E();
  return;
}

static void script_120D(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(5);
  local_F8 = 0x304D5544;
  local_FC = 0x52494B41;
  local_F4 = 0x5953494B;
  task_250 = 0;
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_executeAicaCommand(1192, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  script_024F(SHEN_NAME('WBK1'), local_FC, 18, (int32_t)&data_25B8, (int32_t)&data_25DC, -1);
  script_024F(SHEN_NAME('WBK2'), local_F4, 12, (int32_t)&data_25C4, (int32_t)&data_25E8, -1);
  script_024F(SHEN_NAME('WBK3'), local_F4, 12, (int32_t)&data_25D0, (int32_t)&data_25F4, -1);
  script_0289(SHEN_NAME('WBK1'), 1675);
  script_0289(SHEN_NAME('WBK2'), 1675);
  script_0289(SHEN_NAME('WBK3'), 1675);
  script_0B62(local_FC, 1, 1, -1);
  script_0B62(local_F4, 1, 1, -1);
  script_0B62(SHEN_NAME('WBK1'), 0, 0, -1);
  script_0B62(SHEN_NAME('WBK1'), 1, 0, 160);
  script_0B62(SHEN_NAME('WBK1'), 0, 0, 283);
  script_0B62(SHEN_NAME('WBK1'), 1, 0, 1416);
  script_0B62(SHEN_NAME('WBK1'), 0, 0, 1612);
  script_0B62(SHEN_NAME('WBK2'), 0, 0, -1);
  script_0B62(SHEN_NAME('WBK2'), 1, 0, 283);
  script_0B62(SHEN_NAME('WBK2'), 0, 0, 376);
  script_0B62(SHEN_NAME('WBK2'), 1, 0, 1360);
  script_0B62(SHEN_NAME('WBK2'), 0, 0, 1416);
  script_0B62(SHEN_NAME('WBK3'), 0, 0, -1);
  script_0B62(SHEN_NAME('WBK3'), 1, 0, 376);
  script_0B62(SHEN_NAME('WBK3'), 0, 0, 1360);
  script_0626(local_F4, SHEN_NAME('WBK1'), 0, 16, 240, 115);
  script_0626(local_F4, SHEN_NAME('WBK3'), 0, 16, 360, 150);
  script_0626(local_F4, local_FC, 0x3FC00000, 16, 1416, 85);
  script_06A9(local_FC, 12, -1);
  script_06A9(local_F4, 12, -1);
  script_0792(local_FC, local_F4, -0x3FD842D9, 0, -0x3FDEC0AD, -1);
  script_0893(local_FC, local_F4, 0x43610000, -1);
  script_0792(local_F4, local_F8, -0x3E7562EB, 0, -0x3EE01E99, -1);
  script_0893(local_F4, local_F8, 0x42700000, -1);
  script_03B3(local_FC, 27906, 0x42A00000, 0x3F800000, -0x80000000, -1);
  script_03B3(local_F4, 27908, 0x42A00000, 0x3F800000, -0x80000000, -1);
  script_04F8(local_FC, 1, (int32_t)data_23B0, 10, -1);
  script_04F8(local_FC, 0, (int32_t)data_23B0, 10, -1);
  script_04F8(local_F4, 1, (int32_t)data_23B0, 10, -1);
  script_04F8(local_F4, 0, (int32_t)data_23B0, 10, -1);
  script_0537(local_FC, 0, 0, 15, -1);
  script_0537(local_FC, 1, 0, 15, 30);
  script_0537(local_FC, 0, 0, 15, 60);
  script_0537(local_FC, 1, 0, 15, 200);
  script_0537(local_FC, 7, 0, 15, 215);
  script_0537(local_FC, 0, 0, 15, 225);
  script_0537(local_FC, 1, 0, 15, 1580);
  script_0537(local_FC, 7, 0, 15, 1590);
  script_0537(local_FC, 0, 0, 15, 1600);
  script_0537(local_F4, 0, 0, 15, -1);
  script_0537(local_F4, 2, 0, 15, 100);
  script_0537(local_F4, 0, 0, 15, 120);
  script_0537(local_F4, 7, 0, 15, 360);
  script_0537(local_F4, 0, 0, 15, 380);
  script_0537(local_F4, 2, 0, 15, 890);
  script_0537(local_F4, 7, 0, 15, 900);
  script_0537(local_F4, 0, 0, 15, 910);
  script_0537(local_F4, 7, 0, 15, 1090);
  script_0537(local_F4, 0, 0, 15, 1110);
  script_0537(local_F4, 2, 0, 15, 1520);
  script_0537(local_F4, 0, 0, 15, 1530);
  script_0B9C(6000, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_0B9C(6001, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 118);
  script_0B9C(6002, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 283);
  script_0B9C(6003, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 441);
  script_0B9C(6004, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 779);
  script_0B9C(6005, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 885);
  script_0B9C(6006, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 1066);
  script_0B9C(6007, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 1416);
  script_01D2((int32_t)"SA1108A001", 45);
  script_01D2((int32_t)"SA1108B001", 110);
  script_01D2((int32_t)"SA1108A002", 210);
  script_01D2((int32_t)"SA1108B002", 360);
  script_01D2((int32_t)"SA1108B003", 465);
  script_01D2((int32_t)"SA1108B004", 620);
  script_01D2((int32_t)"SA1108B005", 770);
  script_01D2((int32_t)"SA1108B006", 890);
  script_01D2((int32_t)"SA1108B007", 1090);
  script_01D2((int32_t)"SA1108B008", 1185);
  script_01D2((int32_t)"SA1108B009", 1285);
  script_01D2((int32_t)"SA1108A003", 1464);
  script_01D2((int32_t)"SA1108B010", 1520);
  script_01D2((int32_t)"SA1108A004", 1580);
  script_0199(0x1403AB, 25);
  script_0199(0x1403AB, 45);
  script_0199(0x1403AB, 60);
  script_0199(0x1403AB, 80);
  script_0199(0x1403AB, 98);
  script_0199(132265, 145);
  script_0199(1193, 275);
  script_0199(459945, 355);
  script_0199(394409, 1360);
  script_0199(1193, 1425);
  script_0199(66729, 1605);
  SCNF_WaitFrames_0000(1620);
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_269A, 1, data_26A6, 2, data_26B1, 3, data_26BE, 4, data_26C5)))
static void script_190F(int32_t arg0) {
  task_254 = 0;
  script_0BED();
  script_0C48(CHARA_AKIR, -0x40000000);
  script_0C48(CHARA_KISY, -0x40000000);
  script_0DBA(SHEN_NAME('MKC4'), 0);
  script_0DBA(SHEN_NAME('MKC5'), 0);
  script_0DBA(SHEN_NAME('MKI4'), 0);
  script_02DF(CHARA_AKIR, 0x304D5544, -1);
  script_02DF(CHARA_KISY, 0x304D5544, -1);
  script_02DF(SHEN_NAME('DUMA'), 0x52494B41, -1);
  script_02DF(SHEN_NAME('DUMK'), 0x5953494B, -1);
  Script_LoadSND(0, 0, "system1.snd", 0);
  Script_LoadSND(0, 2, "ykm004.snd", 0);
  Script_LoadSND(0, 4, "e1hidmak.snd", 0);
  Script_LoadVoiceFile(1, "SA1109");
  task_258 = Script_motiLoad(3, SHEN_NAME('MK80'), "M_MK80.BIN");
  Script_SetMapFlags(0, 0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
  Script_shenSetLightingMode(2);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_SetCurrentScrollEnabled(0);
  return;
}

__attribute__((scn_string_bindings(0, data_26D0)))
static void script_1A35(int32_t arg0) {
  task_254 = 1;
  Script_LoadVoiceFile(0, "SA1109");
  Script_ReallocateUNFO(*&task_258);
  script_0E0E();
  return;
}

static void script_1A59(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(5);
  local_F8 = 0x304D5544;
  local_FC = 0x52494B41;
  local_F4 = 0x5953494B;
  task_250 = 0;
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_executeAicaCommand(1192, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  script_024F(SHEN_NAME('MKC4'), local_FC, 18, (int32_t)&data_26D8, (int32_t)&data_26FC, -1);
  script_024F(SHEN_NAME('MKC5'), local_F4, 12, (int32_t)&data_26E4, (int32_t)&data_2708, -1);
  script_024F(SHEN_NAME('MKI4'), local_F4, 12, (int32_t)&data_26F0, (int32_t)&data_2714, -1);
  script_0289(SHEN_NAME('MKC4'), 2595);
  script_0289(SHEN_NAME('MKC5'), 2595);
  script_0289(SHEN_NAME('MKI4'), 2595);
  script_0B62(local_FC, 1, 1, -1);
  script_0B62(local_F4, 1, 1, -1);
  script_0B62(SHEN_NAME('MKC4'), 0, 0, -1);
  script_0B62(SHEN_NAME('MKC4'), 1, 0, 160);
  script_0B62(SHEN_NAME('MKC4'), 0, 0, 272);
  script_0B62(SHEN_NAME('MKC4'), 1, 0, 1787);
  script_0B62(SHEN_NAME('MKC4'), 0, 0, 2485);
  script_0B62(SHEN_NAME('MKC5'), 0, 0, -1);
  script_0B62(SHEN_NAME('MKC5'), 1, 0, 272);
  script_0B62(SHEN_NAME('MKC5'), 0, 0, 415);
  script_0B62(SHEN_NAME('MKC5'), 1, 0, 1661);
  script_0B62(SHEN_NAME('MKC5'), 0, 0, 1787);
  script_0B62(SHEN_NAME('MKI4'), 0, 0, -1);
  script_0B62(SHEN_NAME('MKI4'), 1, 0, 415);
  script_0B62(SHEN_NAME('MKI4'), 0, 0, 1661);
  script_0626(local_F4, SHEN_NAME('MKC5'), 0, 16, 272, 160);
  script_06A9(local_FC, 18, -1);
  script_06A9(local_F4, 18, -1);
  script_0792(local_FC, local_F4, -0x3FD416C6, 0, -0x3FDB6A16, -1);
  script_0893(local_FC, local_F4, 0x43610000, -1);
  script_0792(local_F4, local_F8, -0x3E7562EB, 0, -0x3EE01E99, -1);
  script_0893(local_F4, local_F8, 0x42700000, -1);
  script_03B3(local_FC, 27905, 0, 0x3F800000, -0x80000000, -1);
  script_03B3(local_F4, 27907, 0, 0x3F800000, -0x80000000, -1);
  script_04BC(local_FC, 1, (int32_t)data_2720, -1);
  script_04F8(local_FC, 1, (int32_t)data_23B0, 10, -1);
  script_04F8(local_FC, 1, (int32_t)data_2494, 10, 155);
  script_04F8(local_FC, 1, (int32_t)data_23B0, 10, 270);
  script_04F8(local_FC, 1, (int32_t)data_2494, 10, 1783);
  script_04F8(local_FC, 1, (int32_t)data_23B0, 10, 2485);
  script_04F8(local_FC, 0, (int32_t)data_23B0, 10, -1);
  script_04F8(local_F4, 1, (int32_t)data_23B0, 10, -1);
  script_04F8(local_F4, 0, (int32_t)data_23B0, 10, -1);
  script_04F8(local_F4, 0, (int32_t)data_2494, 10, 270);
  script_04F8(local_F4, 0, (int32_t)data_23B0, 10, 410);
  script_04F8(local_F4, 0, (int32_t)data_2494, 10, 1715);
  script_04F8(local_F4, 0, (int32_t)data_23B0, 10, 1783);
  script_0537(local_FC, 0, 0, 15, -1);
  script_0537(local_FC, 1, 0, 15, 30);
  script_0537(local_FC, 0, 0, 15, 60);
  script_0537(local_FC, 1, 0, 15, 200);
  script_0537(local_FC, 7, 0, 15, 215);
  script_0537(local_FC, 0, 0, 15, 225);
  script_0537(local_FC, 7, 0, 15, 1335);
  script_0537(local_FC, 0, 0, 15, 1348);
  script_0537(local_FC, 7, 0, 15, 1730);
  script_0537(local_FC, 0, 0, 15, 1745);
  script_0537(local_FC, 2, 0, 15, 2200);
  script_0537(local_FC, 0, 0, 15, 2210);
  script_0537(local_FC, 1, 0, 15, 2485);
  script_0537(local_FC, 7, 0, 15, 2490);
  script_0537(local_FC, 0, 0, 15, 2500);
  script_0537(local_F4, 0, 0, 15, -1);
  script_0537(local_F4, 2, 0, 15, 100);
  script_0537(local_F4, 0, 0, 15, 120);
  script_0537(local_F4, 7, 0, 15, 290);
  script_0537(local_F4, 7, 0, 15, 450);
  script_0537(local_F4, 0, 0, 15, 470);
  script_0537(local_F4, 2, 0, 15, 880);
  script_0537(local_F4, 7, 0, 15, 900);
  script_0537(local_F4, 0, 0, 15, 920);
  script_0537(local_F4, 7, 0, 15, 1395);
  script_0537(local_F4, 0, 0, 15, 1400);
  script_0537(local_F4, 1, 0, 15, 2300);
  script_0537(local_F4, 0, 0, 15, 2320);
  script_0B9C(6010, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_0B9C(6011, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 101);
  script_0B9C(6012, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 272);
  script_0B9C(6013, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 440);
  script_0B9C(6014, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 640);
  script_0B9C(6015, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 850);
  script_0B9C(6016, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 1060);
  script_0B9C(6017, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 1295);
  script_0B9C(6018, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 1335);
  script_0B9C(6019, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 1661);
  script_0B9C(6020, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 1800);
  script_0B9C(6021, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 2175);
  script_0B9C(6022, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 2275);
  script_01D2((int32_t)"SA1109A001", 25);
  script_01D2((int32_t)"SA1109B001", 90);
  script_01D2((int32_t)"SA1109A002", 190);
  script_01D2((int32_t)"SA1109B002", 280);
  script_01D2((int32_t)"SA1109B003", 440);
  script_01D2((int32_t)"SA1109B004", 510);
  script_01D2((int32_t)"SA1109B005", 650);
  script_01D2((int32_t)"SA1109B006", 755);
  script_01D2((int32_t)"SA1109B007", 870);
  script_01D2((int32_t)"SA1109B008", 990);
  script_01D2((int32_t)"SA1109B009", 1170);
  script_01D2((int32_t)"SA1109B010", 1290);
  script_01D2((int32_t)"SA1109A003", 1335);
  script_01D2((int32_t)"SA1109B011", 1385);
  script_01D2((int32_t)"SA1109B012", 1505);
  script_01D2((int32_t)"SA1109B013", 1600);
  script_01D2((int32_t)"SA1109A004", 1720);
  script_01D2((int32_t)"SA1109B014", 1790);
  script_01D2((int32_t)"SA1109B015", 1940);
  script_01D2((int32_t)"SA1109A005", 2190);
  script_01D2((int32_t)"SA1109B016", 2245);
  script_01D2((int32_t)"SA1109B017", 2295);
  script_01D2((int32_t)"SA1109A006", 2475);
  script_0199(0x1403AB, 30);
  script_0199(0x1403AB, 45);
  script_0199(0x1403AB, 60);
  script_0199(0x1403AB, 80);
  script_0199(0x1403AB, 100);
  script_0199(132265, 155);
  script_0199(1193, 265);
  script_0199(197801, 405);
  script_0199(263337, 1600);
  script_0199(328873, 1650);
  script_0199(1193, 1785);
  script_0199(66729, 2485);
  script_0199(0x1403AB, 2535);
  script_0199(0x1403AB, 2558);
  script_0199(0x1403AB, 2575);
  SCNF_WaitFrames_0000(2540);
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}
