#include "act80_tour.h"

static void script_0030(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  float local_A0;
  float local_9C;
  int32_t local_98;
  int16_t local_96;
  uint8_t unused_94[2];
  int32_t local_90;
  int32_t local_8C;
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4) * 5 + 1);
  *(int32_t *)&local_A4 = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4) * 5 + 2);
  local_98 = 0x425C0000;
  *(int32_t *)&local_A0 = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4) * 5 + 3);
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4) * 5 + 4);
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
  local_96 = 0;
  local_B8 = 0.0f;
  local_B4 = 0.0f;
  local_B0 = 0;
  {
    int32_t local_88;
    Vector3i local_FF7C;
    Script_mtwkGetSetRotation(arg1, &local_FF7C, 0);
    if ((arg4 == 1) | (arg4 == 3)) {
      local_88 = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4) * 5);
      local_8C = 18;
      local_90 = 18;
      local_AC = 1.0f;
    } else {
      local_88 = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4) * 5) | -0x80000000;
      local_8C = 12;
      local_90 = 12;
      local_AC = -1.0f;
    }
    local_88 = local_88 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_A8 - 3.0f) << 16) & 0x7FFF0000);
    if (task_124 == arg0) {
      if (task_144) {
        Script_seqEnterEvtCam(arg4 * 4 + 800, arg1, arg1);
      }
    }
    script_38CF(arg0, 0);
    local_C4.x = (&task_12C)->x;
    (&local_C4)->y = task_12C.y;
    (&local_C4)->z = task_12C.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMoveTarget(arg0, &task_12C, *(&task_138 + 1), local_88);
    while (Script_GetCharaMoveStatus(arg0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    local_88 = local_88 & 0xFFFF;
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        break;
      }
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      local_BC = Script_GetCharaMotionRemainingFramesScaled(arg0);
      Script_ReadMOTNPositionAtFrame(arg0, local_88, local_BC, &local_E8);
      local_E8.x = (&local_E8)->x * local_AC;
      if (!(local_BC >= local_A8)) {
        local_96 = 0;
      }
      if (!(local_BC < local_A8) & !(local_BC > local_A4)) {
        if (!(local_BC < local_A8) & !(local_BC >= local_A8 + 1.0f)) {
          int32_t local_FF78;
          local_FF78 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_FF78 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF78 * 9 + 1) * 4 + 2);
          if (local_FF78 != -1) {
            Script_Execute3DSoundCommand_Wrapper(local_FF78, 4, &local_C4, 5.0f, 20.0f, 0);
          }
        }
        Script_EVT_GetPosition(arg0, local_90, &local_F4, ENTITY_XFORM_MOTION_SPACE);
        if (local_96 == 1) {
          local_B4 = (&local_E8)->x;
          local_C0 = (&local_F4)->x;
        }
        local_B8 = (&local_E8)->x + ((&local_F4)->x - local_C0);
        local_B8 = local_B8 - local_B4;
        local_B8 = local_B8 * 1.28f;
        {
          int32_t local_FF78;
          local_FF78 = arg4;
          switch (local_FF78) {
            case 0:
              if (!(local_B8 < 0.0f)) {
                local_B8 = 0.0f;
              } else {
                if (!(local_B8 > -0.75f)) {
                  local_B8 = -0.75f;
                }
              }
              break;
            case 1:
              if (!(local_B8 > 0.0f)) {
                local_B8 = 0.0f;
              } else {
                if (!(local_B8 < 0.75f)) {
                  local_B8 = 0.75f;
                }
              }
              break;
            case 2:
              if (!(local_B8 < 0.0f)) {
                local_B8 = 0.0f;
              } else {
                if (!(local_B8 > -0.75f)) {
                  local_B8 = -0.75f;
                }
              }
              break;
            case 3:
              if (!(local_B8 > 0.0f)) {
                local_B8 = 0.0f;
              } else {
                if (!(local_B8 < 0.75f)) {
                  local_B8 = 0.75f;
                }
              }
              break;
          }
        }
      }
      if (((scn3_bits){.f = local_BC}).i == local_98) {
        if (task_124 == arg0) {
          if (task_144) {
            Script_seqEnterEvtCam(arg4 * 4 + 801, arg1, arg1);
          }
        }
      }
      if (!(local_BC <= local_A4) & !(local_BC >= local_A0)) {
        local_96 = 0;
      }
      if (!(local_BC < local_A0) & !(local_BC > local_9C)) {
        int32_t local_FF78;
        Script_EVT_GetPosition(arg0, local_8C, &local_F4, ENTITY_XFORM_MOTION_SPACE);
        local_B8 = (&local_E8)->x + ((&local_F4)->x - local_C0);
        local_B8 = local_B8 - local_B4;
        local_B8 = local_B8 * 1.28f;
        local_FF78 = arg4;
        switch (local_FF78) {
          case 0:
            if (!(local_B8 > -0.75f)) {
              local_B8 = -0.75f;
            } else {
              if (!(local_B8 < 0.0f)) {
                local_B8 = 0.0f;
              }
            }
            break;
          case 1:
            if (!(local_B8 < 0.75f)) {
              local_B8 = 0.75f;
            } else {
              if (!(local_B8 > 0.0f)) {
                local_B8 = 0.0f;
              }
            }
            break;
          case 2:
            if (!(local_B8 > -0.75f)) {
              local_B8 = -0.75f;
            } else {
              if (!(local_B8 < 0.0f)) {
                local_B8 = 0.0f;
              }
            }
            break;
          case 3:
            if (!(local_B8 < 0.75f)) {
              local_B8 = 0.75f;
            } else {
              if (!(local_B8 > 0.0f)) {
                local_B8 = 0.0f;
              }
            }
            break;
        }
        if (!(local_BC < local_A0) & !(local_BC >= local_A0 + 1.0f)) {
          int32_t local_FF74;
          local_FF74 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_FF74 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_FF74 * 9 + 1) * 4 + 3);
          if (local_FF74 != -1) {
            Script_Execute3DSoundCommand_Wrapper(local_FF74, 4, &local_C4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (!(local_9C <= 0.0f)) {
        if (!(local_BC <= local_9C)) {
          local_B8 = local_B8 - 0.01f;
          if (!(local_B8 > 0.0f)) {
            local_B8 = 0.0f;
          }
        }
      }
      {
        int32_t local_FF78;
        local_FF78 = arg4;
        switch (local_FF78) {
          case 0:
            local_F4.x = (&local_D0)->x;
            (&local_F4)->y = (&local_D0)->y;
            (&local_F4)->z = (&local_D0)->z;
            if (arg5) {
              local_F4.x = (&local_D0)->x - local_B8;
              Script_charaPosRotUtils(arg1, 7, &local_F4, 0, CHARA_PART_Set);
            } else {
              local_F4.x = (&local_D0)->x + ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF7C)->y);
              (&local_F4)->z = (&local_D0)->z + local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF7C)->y);
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
              local_F4.x = (&local_D0)->x - ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF7C)->y);
              (&local_F4)->z = (&local_D0)->z - local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF7C)->y);
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
              local_F4.x = (&local_DC)->x - ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF7C)->y);
              (&local_F4)->z = (&local_DC)->z - local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF7C)->y);
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
              local_F4.x = (&local_DC)->x + ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF7C)->y);
              (&local_F4)->z = (&local_DC)->z + local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF7C)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
        }
      }
      local_96 = local_96 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_38CF(arg0, 1);
  if (task_124 == arg0) {
    if (task_144) {
      Script_SetCameraSettings(5, 6, 1, 0);
      Script_convSetCamInactiveFlag(0);
      SCNF_WaitFrames_0000(1);
      Script_SetGameStateTo(AUTH_None);
    }
  }
  {
    int32_t local_88;
    int32_t unused_FF7C[3];
    int32_t local_FF78;
    local_88 = task_48[arg2];
    local_FF78 = arg3;
    if ((local_FF78 == 9) | (local_FF78 == 10) | (local_FF78 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_DC, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_D0, 0, CHARA_PART_Set);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_DC);
      }
      if (task_124 == arg0) {
        local_90 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_90, 1);
        local_90 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_90, 1);
      }
    } else if ((local_FF78 == 18) | (local_FF78 == 19) | (local_FF78 == 21) | (local_FF78 == 22)) {
      local_90 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_90, 1);
      {
        int32_t local_FF74;
        local_FF74 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 1);
        local_FF74 = local_FF74 | 8;
        Script_ScenDataUtil(task_0, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
      }
      {
        int32_t local_FF74;
        local_FF74 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 1);
        local_FF74 = local_FF74 | 0x10;
        Script_ScenDataUtil(task_0, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
      }
      {
        int32_t local_FF74;
        local_FF74 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 1);
        local_FF74 = local_FF74 | 2;
        Script_ScenDataUtil(task_0, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
      }
    } else {
      if ((local_FF78 == 20) | (local_FF78 == 23)) {
        if ((arg4 == 0) | (arg4 == 1)) {
          local_90 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_90, 1);
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 0x10;
            Script_ScenDataUtil(task_0, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 4;
            Script_ScenDataUtil(task_0, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
        } else {
          local_90 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_90, 1);
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 8;
            Script_ScenDataUtil(task_0, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 4;
            Script_ScenDataUtil(task_0, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
        }
        goto L000C74;
      }
    }
  }
  L000C74:;
  return;
}

static void script_0C75(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_124 == arg0) {
    local_FC = task_48[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 6 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_0CE1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  uint8_t unused_90[3];
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
  local_AC.x = (&task_12C)->x;
  (&local_AC)->y = task_12C.y;
  (&local_AC)->z = task_12C.z;
  local_DC.x = (&local_C4)->x;
  (&local_DC)->y = (&local_C4)->y;
  (&local_DC)->z = (&local_C4)->z;
  local_D0.x = (&local_B8)->x;
  (&local_D0)->y = (&local_B8)->y;
  (&local_D0)->z = (&local_B8)->z;
  local_9C = 0xC000;
  local_93 = 0;
  local_98 = 0x200;
  Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg1, &local_A0, 0);
  {
    int32_t local_80;
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
    local_80 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
    }
  }
  local_88 = 0;
  local_8C = 0;
  local_84 = 0;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
      if (Script_CheckTargetWithinAngleRange(&local_F4, 1.5f, (&local_A0)->y, 0x10000, &local_E8)) {
        if (local_8C) {
          task_74[arg2] = arg0;
          local_98 = -local_98;
          local_84 = 0;
        }
        local_8C = 0;
      } else {
        if (local_8C == 0) {
          int32_t local_80;
          local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_80 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
          if (local_80 != -1) {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
          local_93 = 0;
          local_8C = 1;
          local_98 = -local_98;
          local_84 = 0;
          task_74[arg2] = 0;
        }
      }
    } else {
      if (local_8C == 0) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_80 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_74[arg2] = 0;
      }
    }
    if (task_124 == arg0) {
      local_88 = task_48[arg2];
      local_88 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_88 * 6 + 2) & 0xFFFF;
      local_88 = local_88 + ((local_9C - 0x4000) >> 12);
      if (local_84 != local_88) {
        Script_scenToggleCharaFlag(0, local_88, local_8C);
        local_84 = local_88;
      }
      Script_PrintDebugString(30, 20, "%d - %d", local_88, local_8C);
    }
    local_9C = local_9C - local_98;
    Script_PrintDebugString(30, 21, "%04X : %d", local_9C, local_8C);
    if (local_9C >= 0xC000) {
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_74[arg2] = 0;
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
          goto L00117D;
        }
      }
    }
    L00117D:;
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

static void script_11CD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5);
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
          local_FC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5);
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
          local_FC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          break;
      }
      break;
  }
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 2);
  local_BC = 0.0f;
  script_38CF(arg0, 0);
  local_CC.x = (&task_12C)->x;
  (&local_CC)->y = task_12C.y;
  (&local_CC)->z = task_12C.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMoveTarget(arg0, &task_12C, *(&task_138 + 1), local_FC);
  while (Script_GetCharaMoveStatus(arg0) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_FC = local_FC & 0xFFFF;
  Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
  Script_mtwkGetSetRotation(arg1, &local_A8, 0);
  local_F4 = (&local_A8)->y;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
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
          Script_Execute3DSoundCommand_Wrapper(local_A0, 4, &local_CC, 5.0f, 20.0f, 0);
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
  script_38CF(arg0, 1);
  local_FC = task_48[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 6 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 6 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 6 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 6 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 6 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 6 + 1, local_A0);
  }
  return;
}

static void script_17B9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  int32_t local_AC;
  int32_t local_A8;
  int32_t local_A4;
  int32_t local_A0;
  int32_t local_9C;
  int32_t local_98;
  int32_t local_94;
  int32_t local_90;
  int32_t local_8C;
  int32_t local_88;
  local_CE = 0;
  local_88 = arg3;
  if ((local_88 == 4) | (local_88 == 6)) {
    local_88 = arg4;
    switch (local_88) {
      case 1:
        local_CF = 1;
        local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5);
        local_C0 = 1.0f;
        local_A8 = 18;
        local_A4 = 18;
        local_98 = 15473;
        local_94 = 17294;
        break;
      case 0:
        local_CF = 0;
        local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
        local_C0 = 1.0f;
        local_A8 = 12;
        local_A4 = 18;
        local_98 = 17294;
        local_94 = 15473;
        break;
    }
  } else if ((local_88 == 1) | (local_88 == 7)) {
    local_88 = arg4;
    switch (local_88) {
      case 1:
        local_CF = 0;
        local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
        local_C0 = 1.0f;
        local_A8 = 12;
        local_A4 = 18;
        local_98 = 50062;
        local_94 = 48241;
        break;
      case 0:
        local_CF = 1;
        local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5);
        local_C0 = 1.0f;
        local_A8 = 18;
        local_A4 = 18;
        local_98 = 48241;
        local_94 = 50062;
        break;
    }
  } else if ((local_88 == 3) | (local_88 == 5)) {
    local_88 = arg4;
    switch (local_88) {
      case 1:
        local_CF = 1;
        local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
        local_C0 = -1.0f;
        local_A8 = 12;
        local_A4 = 12;
        local_98 = 50062;
        local_94 = 48241;
        break;
      case 0:
        local_CF = 0;
        local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5);
        local_C0 = -1.0f;
        local_A8 = 18;
        local_A4 = 12;
        local_98 = 48241;
        local_94 = 50062;
        break;
    }
  } else if ((local_88 == 2) | (local_88 == 8)) {
    local_88 = arg4;
    switch (local_88) {
      case 1:
        local_CF = 0;
        local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5);
        local_C0 = -1.0f;
        local_A8 = 18;
        local_A4 = 12;
        local_98 = 15473;
        local_94 = 17294;
        break;
      case 0:
        local_CF = 1;
        local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
        local_C0 = -1.0f;
        local_A8 = 12;
        local_A4 = 12;
        local_98 = 17294;
        local_94 = 15473;
        break;
    }
  } else {
    if ((local_88 == 13) | (local_88 == 14)) {
      local_88 = arg4;
      switch (local_88) {
        case 0:
          local_CF = 1;
          local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          local_C0 = 1.0f;
          local_A8 = 18;
          local_A4 = 18;
          local_98 = 48241;
          local_94 = 50062;
          break;
        case 1:
          local_CF = 0;
          local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          local_C0 = 1.0f;
          local_A8 = 12;
          local_A4 = 18;
          local_98 = 50062;
          local_94 = 48241;
          break;
        case 2:
          local_CF = 0;
          local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          local_C0 = -1.0f;
          local_A8 = 18;
          local_A4 = 12;
          local_98 = 15473;
          local_94 = 17294;
          break;
        case 3:
          local_CF = 1;
          local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          local_C0 = -1.0f;
          local_A8 = 12;
          local_A4 = 12;
          local_98 = 17294;
          local_94 = 15473;
          break;
      }
      local_CE = 1;
      goto L001CEA;
    }
  }
  L001CEA:;
  *(int32_t *)&local_BC = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 1);
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 2);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 3);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 4);
  local_C8 = 0.0f;
  if (local_CE) {
    {
      Vector3i local_FF7C;
      Script_charaPosRotUtils(arg1, 12, 0, &local_FF7C, CHARA_PART_Get);
      local_A0 = (&local_FF7C)->y;
    }
    {
      Vector3i local_FF7C;
      Script_charaPosRotUtils(arg1, 7, 0, &local_FF7C, CHARA_PART_Get);
      local_9C = (&local_FF7C)->y;
    }
    if ((arg4 == 2) | (arg4 == 3)) {
      local_90 = local_A0;
      Script_charaPosRotUtils(arg1, 12, &local_F4, 0, CHARA_PART_Get);
    } else {
      local_90 = local_9C;
      Script_charaPosRotUtils(arg1, 7, &local_F4, 0, CHARA_PART_Get);
    }
    {
      Vector3i local_FF7C;
      Vector3 local_FF70;
      Script_EVT_GetPosition(arg1, -1, &local_FF70, 0);
      Script_mtwkGetSetRotation(arg1, &local_FF7C, 0);
      local_8C = (&local_FF7C)->y;
      {
        float local_FF6C;
        float local_FF68;
        local_FF68 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_8C) - (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_8C);
        local_FF6C = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_8C) + (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_8C);
        local_F4.x = local_FF6C + (&local_FF70)->x;
        (&local_F4)->y = (&local_F4)->y + (&local_FF70)->y;
        (&local_F4)->z = local_FF68 + (&local_FF70)->z;
      }
    }
  } else {
    Vector3i local_FF7C;
    Script_mtwkGetSetRotation(arg1, &local_FF7C, 0);
    local_A0 = (&local_FF7C)->y;
    local_9C = (&local_FF7C)->y;
    local_90 = (&local_FF7C)->y;
    Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
    local_8C = 0;
  }
  local_AC = local_AC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_BC - 3.0f) << 16) & 0x7FFF0000);
  script_38CF(arg0, 0);
  local_D0.x = (&task_12C)->x;
  (&local_D0)->y = task_12C.y;
  (&local_D0)->z = task_12C.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMoveTarget(arg0, &task_12C, *(&task_138 + 1), local_AC);
  while (Script_GetCharaMoveStatus(arg0)) {
    SCNF_WaitFrames_0000(1);
  }
  local_AC = local_AC & 0xFFFF;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    local_C4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    Script_PrintDebugString(30, 20, "%f : %f : %f", local_C4, local_BC, local_B0);
    if (!(local_C4 < local_BC) & !(local_C4 > local_B8)) {
      if (local_CF == 1) {
        local_C8 = local_C8 - 0.165526f;
      } else {
        local_C8 = local_C8 - 0.145973f;
      }
      if (!(local_C4 < local_BC) & !(local_C4 >= local_BC + 1.0f)) {
        int32_t local_84;
        local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_84 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_84 * 9 + 1) * 4 + 2);
        if (local_84 != -1) {
          Script_Execute3DSoundCommand_Wrapper(local_84, 4, &local_D0, 5.0f, 20.0f, 0);
        }
      }
      Script_EVT_GetPosition(arg0, local_A8, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      local_90 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
      local_90 = local_90 - local_8C;
      local_90 = local_90 + local_98 + (int32_t)(local_C8 * local_C0 / 360.0f * (float)0x10000);
    }
    if (!(local_C4 < local_B4) & !(local_C4 > local_B0)) {
      if (local_CF == 0) {
        local_C8 = local_C8 - 0.11939f;
      } else {
        local_C8 = local_C8 + 2.381415f;
      }
      if (!(local_C4 < local_B0) & !(local_C4 >= local_B0 + 1.0f)) {
        int32_t local_84;
        local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_84 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_84 * 9 + 1) * 4 + 3);
        if (local_84 != -1) {
          Script_Execute3DSoundCommand_Wrapper(local_84, 4, &local_D0, 5.0f, 20.0f, 0);
        }
      }
      Script_EVT_GetPosition(arg0, local_A4, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      local_90 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
      local_90 = local_90 - local_8C;
      local_90 = local_90 + local_94 + (int32_t)(local_C8 * local_C0 / 360.0f * (float)0x10000);
    }
    if (local_CE) {
      Vector3i local_FF7C;
      local_FF7C.x = 0;
      (&local_FF7C)->z = 0;
      if ((arg4 == 2) | (arg4 == 3)) {
        (&local_FF7C)->y = local_90;
        Script_charaPosRotUtils(arg1, 12, 0, &local_FF7C, CHARA_PART_Set);
      } else {
        (&local_FF7C)->y = local_90;
        Script_charaPosRotUtils(arg1, 7, 0, &local_FF7C, CHARA_PART_Set);
      }
    } else {
      Vector3i local_FF7C;
      (&local_FF7C)->y = local_90;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF7C, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_38CF(arg0, 1);
  if (local_CE) {
    Vector3i local_FF7C;
    local_FF7C.x = 0;
    (&local_FF7C)->z = 0;
    (&local_FF7C)->y = local_A0;
    Script_charaPosRotUtils(arg1, 12, 0, &local_FF7C, CHARA_PART_Set);
    (&local_FF7C)->y = local_9C;
    Script_charaPosRotUtils(arg1, 7, 0, &local_FF7C, CHARA_PART_Set);
  } else {
    {
      Vector3i local_FF7C;
      (&local_FF7C)->y = local_A0;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF7C, 0);
    }
    {
      Vector3i local_FF7C;
      (&local_FF7C)->y = local_9C;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF7C, 0);
    }
  }
  return;
}

static void script_22C6(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5) {
  int32_t local_FC;
  task_0 = arg1;
  task_4 = arg2;
  task_8 = arg3;
  task_C = arg4;
  task_10.mode = 0;
  task_10.id = 0;
  task_10.entry_idx = 0;
  task_124 = arg0;
  task_148 = 0;
  {
    int32_t local_F8;
    local_F8 = 0;
    L002313:;
    if (local_F8 == 10) {
      local_FC = Script_AddCharacterHandler(1, 0, 0x40786F62);
    } else {
      local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
    }
    Script_scenToggleVisibleFor(local_FC, 0);
    task_CC[local_F8] = local_FC;
    task_1C[local_F8] = -1;
    task_48[local_F8] = -1;
    task_74[local_F8] = 0;
    task_A0[local_F8] = 0;
    task_F8[local_F8] = 0;
    local_F8 = local_F8 + 1;
    if (!(local_F8 >= 11)) goto L002313;
  }
  {
    int32_t local_F8;
    local_F8 = 0;
    for (;;) {
      if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 6) == -1) {
        break;
      }
      Script_ScenDataUtil(task_0, SCND_WriteU32, local_F8 * 6 + 1, 0);
      local_F8 = local_F8 + 1;
    }
  }
  task_144 = 0;
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_10, task_1C, task_48, (shen_name *)task_74, arg0, arg5, data_7A30, data_7AA4);
  Script_scenCreateTask(script_2786, 0);
  return;
}

static void script_242E(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_0 = arg0;
  task_4 = arg1;
  task_8 = arg2;
  task_C = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_148 = 1;
  return;
}

static void script_2462(void) {
  int32_t local_FC;
  local_FC = 0;
  L00246A:;
  task_1C[local_FC] = -1;
  local_FC = local_FC + 1;
  if (!(local_FC >= 11)) goto L00246A;
  return;
}

static void script_248A(void) {
  return;
}

static void script_248D(void) {
  return;
}

static void script_2490(int32_t arg0, int32_t arg1) {
  DSWK_DoorUtils(DOOR_CMD_ALLOC_ENTRY_SLOT, arg0, arg1);
  return;
}

static void script_249D(void) {
  return;
}

static void script_24A0(int32_t arg0, int32_t arg1) {
  DSWK_DoorUtils(DOOR_CMD_ALLOC_ENTRY_SLOT, arg0, arg1);
  return;
}

static void script_24AD(int32_t arg0) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F4);
  return;
}

static void script_24C6(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 6) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 6 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 6 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_2529(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 6) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_0, SCND_ReadU32, local_FC * 6 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_0, SCND_WriteU32, local_FC * 6 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_258C(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  return;
}

static void script_25C3(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  return;
}

static void script_25FA(int32_t arg0) {
  task_144 = arg0;
  return;
}

static void script_2604(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_48[local_F0] == arg0) {
      local_EC = task_CC[local_F0];
      if (Script_shenGetNode(local_EC, 8) & Script_shenGetNode(local_EC, 13)) {
        if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F0] * 6 + 1) & 0x20) {
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

static void script_2706(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  script_2604(arg0);
  return;
}

static void script_2746(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_0, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  script_2604(arg0);
  return;
}

static int32_t script_2786(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  Vector3 local_D8;
  Vector3i local_CC;
  int8_t local_CB;
  int8_t local_CA;
  int8_t local_C9;
  int32_t local_C4;
  int32_t local_C0;
  int32_t local_BC;
  int32_t local_B8;
  int32_t local_B4;
  int32_t local_AC[2];
  Vector3i local_A0;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F8 = 0;
  local_E4 = -1;
  local_EC = -1;
  local_CA = 0;
  local_C9 = 0;
  for (;;) {
    DSWK_DoorUtils(DOOR_CMD_TIMER_START);
    if (task_148 == 0) {
      local_E8 = local_EC;
      local_E4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_E4 != -1) & -(task_1C[local_E4] == 1) & Script_ryoControl(PCCTL_QueryShouldRun) & -(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1)) {
        local_F0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[local_E4], 2);
        local_EC = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 1) * 4 + 1);
        local_C0 = local_EC;
        if ((local_C0 == 17) | (local_C0 == 11) | (local_C0 == 12)) {
          local_CA = 0;
          if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_E4] * 6 + 1) & 2) == 0) {
            if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_E4] * 6 + 1) & 4) == 0) {
              task_1C[local_E4] = 4;
            } else {
              local_E4 = -1;
              local_EC = -1;
            }
          } else {
            local_E4 = -1;
            local_EC = -1;
          }
        } else {
          local_CA = 0;
          if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_E4] * 6 + 1) & 2) == 0) {
            if (((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_E4] * 6 + 1) & 4) == 0) & (Script_ryoControl(PCCTL_GetTatehiza) == 0)) {
              local_CB = 0;
              local_BC = script_3386(task_124, task_CC[local_E4], -1, 0.5f, 0.8f, local_EC);
              Script_mtwkGetSetRotation(task_124, (Vector3i *)local_AC, 0x42000000);
              Script_mtwkGetSetRotation(task_CC[local_E4], &local_A0, 0);
              if (local_BC == 1) {
                local_B8 = (local_AC[1] - (&local_A0)->y) & 0xFFFF;
                if ((local_B8 <= 21845) | (local_B8 >= 43691)) {
                  local_CB = 1;
                }
              } else if (local_BC == 0) {
                local_B8 = (local_AC[1] - (&local_A0)->y + 0x8000) & 0xFFFF;
                if ((local_B8 <= 21845) | (local_B8 >= 43691)) {
                  local_CB = 1;
                }
              } else {
                local_CB = 0;
              }
              if (local_CB) {
                local_CA = 1;
              }
            }
          }
          if (Script_GetControllerInput(1) & 0x400 & -((Script_GetControllerInput(2) & 0x80) == 0) & -(local_CA == 1)) {
            if (-(task_74[local_E4] != task_124) & local_CB) {
              if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_E4] * 6 + 1) & 2) == 0) {
                if ((Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_E4] * 6 + 1) & 4) == 0) {
                  task_1C[local_E4] = 3;
                  task_74[local_E4] = task_124;
                  task_10.mode = 1;
                  task_10.entry_idx = task_48[local_E4];
                  Script_ryoControl(PCCTL_RequestNewMode, 4);
                  Script_ryoControl(PCCTL_DisablePlayerControl);
                  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
                    SCNF_WaitFrames_0000(1);
                  }
                  Script_ryoControl(PCCTL_RequestNewMode, 0);
                } else {
                  local_E4 = -1;
                  local_EC = -1;
                }
              } else {
                local_E4 = -1;
                local_EC = -1;
              }
            }
          } else {
            local_E4 = -1;
            local_EC = -1;
          }
        }
      } else {
        local_CA = 0;
        local_EC = -1;
      }
      if (((local_CA ^ local_C9) & local_CA) != 0) {
        SCNF_RequestIconNotification(0x100, 0, 1, 1);
      }
      if (((local_CA ^ local_C9) & local_C9) != 0) {
        SCNF_RequestIconNotification(0, 0, 0, 2);
      }
      local_C9 = local_CA;
      local_F4 = 0;
      L002B88:;
      local_C0 = task_1C[local_F4];
      switch (local_C0) {
        case 5:
          if (script_3A05(task_74[local_F4], local_F4, 0) == 0) {
            if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_74[local_F4])) {
              local_BC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[local_F4], 2);
              local_B8 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_BC * 9 + 1) * 4 + 1);
              if (local_B8 == 17) {
              } else {
                script_38F7(task_74[local_F4], task_48[local_F4], local_B8);
              }
              task_F8[local_F4] = Script_scenCreateTask(Script_ScenDataUtil(&data_7B58, SCND_ReadScriptOffset, local_B8), 4, task_74[local_F4], task_CC[local_F4], local_F4, local_B8);
              task_A0[local_F4] = task_74[local_F4];
              script_323C(local_F4);
            } else {
              task_1C[local_F4] = 1;
              task_F8[local_F4] = 0;
              task_A0[local_F4] = 0;
              task_74[local_F4] = 0;
            }
          }
          break;
        case 4:
          task_74[local_E4] = task_124;
          task_10.mode = 2;
          task_10.entry_idx = task_48[local_E4];
          if (script_3A05(task_74[local_F4], local_F4, 0) == 0) {
            task_F8[local_F4] = Script_scenCreateTask(Script_ScenDataUtil(&data_7B58, SCND_ReadScriptOffset, local_EC), 4, task_74[local_F4], task_CC[local_F4], local_F4, local_EC);
            task_A0[local_F4] = task_74[local_F4];
            script_323C(local_F4);
          }
          break;
        case 3:
          if (local_EC >= 0) {
            if (script_3A05(task_74[local_F4], local_F4, 0) == 0) {
              script_38F7(task_74[local_F4], task_48[local_F4], local_EC);
              task_F8[local_F4] = Script_scenCreateTask(Script_ScenDataUtil(&data_7B58, SCND_ReadScriptOffset, local_EC), 4, task_74[local_F4], task_CC[local_F4], local_F4, local_EC);
              task_A0[local_F4] = task_74[local_F4];
              script_323C(local_F4);
            }
          }
          break;
        case 2:
          script_3A05(task_74[local_F4], local_F4, 1);
          local_F0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[local_F4], 2);
          local_FC = task_CC[local_F4];
          if (local_F0 != -1) {
            *(int32_t *)&local_D8.x = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 5);
            *(int32_t *)&(&local_D8)->y = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 6);
            *(int32_t *)&(&local_D8)->z = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 7);
            Script_EVT_SetPosition(local_FC, POS_XYZ, &local_D8);
            local_CC.x = 0;
            (&local_CC)->y = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 8);
            (&local_CC)->z = 0;
            Script_EVT_SetRotation(local_FC, 0x38000000, &local_CC, 0);
            *(int32_t *)&local_D8.x = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 2);
            *(int32_t *)&(&local_D8)->y = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 3);
            *(int32_t *)&(&local_D8)->z = Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 4);
            Script_hmdlApplyScale(local_FC, &local_D8);
            Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_8, SCND_ReadScriptOffset, Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 1) * 4), 0);
            Script_dynamicStuff(local_FC, (const char *)Script_ScenDataUtil(task_8, SCND_ReadScriptOffset, Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 1) * 4), 0);
            Script_charaDisplay(local_FC, CDM_Show);
            Script_charaVisUtils(local_FC, 0);
            local_C0 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_F0 * 9 + 1) * 4 + 1);
            if ((local_C0 == 20) | (local_C0 == 23)) {
              if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F4] * 6 + 1) & 0x10) {
                Script_charaPosRotUtils(local_FC, 7, &data_7BEC, 0, CHARA_PART_Add);
              } else {
                if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F4] * 6 + 1) & 8) {
                  Script_charaPosRotUtils(local_FC, 12, &data_7BF8, 0, CHARA_PART_Add);
                  goto L00304F;
                }
              }
              L00304F:;
            } else if ((local_C0 == 18) | (local_C0 == 19) | (local_C0 == 21) | (local_C0 == 22)) {
            } else if ((local_C0 == 25) | (local_C0 == 26)) {
              if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F4] * 6 + 1) & 0x10) {
                Script_mtwkGetSetRotation(local_FC, (Vector3i *)&local_B4, 0);
                *(&local_B4 + 1) = *(&local_B4 + 1) + 15958;
                Script_EVT_SetRotation(local_FC, 0x38000000, (const Vector3i *)&local_B4, 0);
              }
            } else {
              if ((local_C0 == 24) | (local_C0 == 27)) {
                if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F4] * 6 + 1) & 0x10) {
                  Script_mtwkGetSetRotation(local_FC, (Vector3i *)&local_B4, 0);
                  *(&local_B4 + 1) = *(&local_B4 + 1) - 15958;
                  Script_EVT_SetRotation(local_FC, 0x38000000, (const Vector3i *)&local_B4, 0);
                }
                goto L00312C;
              }
            }
            L00312C:;
            if (Script_shenGetNode(local_FC, 8) & Script_shenGetNode(local_FC, 13)) {
              if (Script_ScenDataUtil(task_0, SCND_ReadU32, task_48[local_F4] * 6 + 1) & 0x20) {
                local_B4 = 0;
                *(&local_B4 + 1) = 0;
                *(&local_B4 + 2) = 0;
                Script_charaPosRotUtils(local_FC, 13, (Vector3 *)&local_B4, 0, CHARA_PART_Set);
                *(&local_B4 + 1) = -0x3B860000;
                Script_charaPosRotUtils(local_FC, 8, (Vector3 *)&local_B4, 0, CHARA_PART_Set);
              } else {
                local_B4 = 0;
                *(&local_B4 + 1) = -0x3B860000;
                *(&local_B4 + 2) = 0;
                Script_charaPosRotUtils(local_FC, 13, (Vector3 *)&local_B4, 0, CHARA_PART_Set);
                *(&local_B4 + 1) = 0;
                Script_charaPosRotUtils(local_FC, 8, (Vector3 *)&local_B4, 0, CHARA_PART_Set);
              }
            }
            task_74[local_F4] = 0;
          } else {
            Script_charaDisplay(local_FC, CDM_Hide);
          }
          break;
      }
      local_F4 = local_F4 + 1;
      if (!(local_F4 >= 11)) goto L002B88;
    } else {
      task_148 = 0;
    }
    local_C4 = DSWK_DoorUtils(DOOR_CMD_TIMER_GET_DELTA_SCALED);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_323C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 1);
  if (local_F8 != -1) {
    local_FC = 0x40786F62;
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
    Script_charaVisUtils(local_FC, 0);
  }
  return;
}

static int32_t script_3386(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg1, &local_D0, 0);
  local_C4.x = (&local_D0)->x;
  (&local_C4)->y = (&local_D0)->y;
  (&local_C4)->z = (&local_D0)->z;
  local_C0 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
  local_C0 = ((local_C0 - (&local_D0)->y) >> 14) & 3;
  {
    int32_t unused_B8;
    int32_t local_B4;
    local_B4 = arg5;
    if ((local_B4 == 0) | (local_B4 == 1) | (local_B4 == 3) | (local_B4 == 5) | (local_B4 == 7) | (local_B4 == 9) | (local_B4 == 11) | (local_B4 == 25) | (local_B4 == 27)) {
      local_B4 = local_C0;
      if ((local_B4 == 0) | (local_B4 == 3)) {
        arg4 = (float)0 - arg4;
        (&local_C4)->y = (&local_C4)->y + 0x8000;
      } else {
        if ((local_B4 == 1) | (local_B4 == 2)) goto L003483;
      }
      L003483:;
    } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26)) {
      local_B4 = local_C0;
      if ((local_B4 == 2) | (local_B4 == 1)) {
        arg3 = (float)0 - arg3;
      } else {
        if ((local_B4 == 3) | (local_B4 == 0)) {
          arg3 = (float)0 - arg3;
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
          goto L003514;
        }
      }
      L003514:;
    } else if ((local_B4 == 13) | (local_B4 == 14) | (local_B4 == 15) | (local_B4 == 16) | (local_B4 == 17) | (local_B4 == 18) | (local_B4 == 19) | (local_B4 == 20) | (local_B4 == 21) | (local_B4 == 22) | (local_B4 == 23)) {
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
      if ((local_B4 == 2) | (local_B4 == 1)) {
      } else {
        if ((local_B4 == 3) | (local_B4 == 0)) {
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
          goto L003633;
        }
      }
      L003633:;
    } else {
      local_B4 = local_C0;
      if ((local_B4 == 2) | (local_B4 == 1)) {
      } else {
        if ((local_B4 == 3) | (local_B4 == 0)) {
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
          goto L00367D;
        }
      }
    }
  }
  L00367D:;
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
        task_12C.x = (&local_F4)->x;
        task_12C.y = (&local_F4)->y;
        task_12C.z = (&local_F4)->z;
        task_138 = (&local_C4)->x;
        *(&task_138 + 1) = (&local_C4)->y;
        *(&task_138 + 2) = (&local_C4)->z;
        break;
      case 5:
        task_12C.x = (&local_F4)->x;
        task_12C.y = (&local_F4)->y;
        task_12C.z = (&local_F4)->z;
        task_138 = (&local_C4)->x;
        *(&task_138 + 1) = (&local_C4)->y;
        *(&task_138 + 2) = (&local_C4)->z;
        if ((local_C0 == 1) | (local_C0 == 2)) {
          local_C0 = 1;
        } else {
          local_C0 = 0;
        }
        break;
    }
  }
  return local_C0;
}

static void script_38CF(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if (arg1) {
      Script_ccowUtils(arg0, CCOW_EnableMap);
    } else {
      Script_ccowUtils(arg0, CCOW_DisableMap);
    }
  }
  return;
}

static void script_38F7(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_124 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
    local_F8 = arg2;
    if ((local_F8 == 25) | (local_F8 == 27) | (local_F8 == 24) | (local_F8 == 26) | (local_F8 == 18) | (local_F8 == 19) | (local_F8 == 20) | (local_F8 == 21) | (local_F8 == 22) | (local_F8 == 23)) {
      local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg1 * 6 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 0);
      local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, arg1 * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 0);
      goto L0039B0;
    }
    L0039B0:;
  } else {
    local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, arg1 * 6 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_FC, 0);
    local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, arg1 * 6 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_FC, 0);
  }
  return;
}

static int32_t script_3A05(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = 0;
  if (task_F8[arg1] != 0) {
    if (arg2) {
      if ((task_124 == task_A0[arg1]) & (task_124 != arg0)) {
        task_10.mode = 0;
        Script_PlayCharaMotion(task_124, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_ryoControl(RYO_RestoreControl);
        script_38CF(task_A0[arg1], 1);
      }
      Script_srchControl((void *)task_F8[arg1]);
      task_F8[arg1] = 0;
      task_A0[arg1] = 0;
      task_1C[arg1] = 1;
      local_FC = 0;
    } else {
      local_FC = 1;
    }
  }
  end:;
}

static void script_3AC1(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_124 == arg0) {
    task_10.mode = 0;
    if (arg2 == 0) {
      Script_PlayCharaMotion(task_124, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      local_F8 = task_48[arg1];
      local_FC = Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 6 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_0, SCND_ReadU32, local_F8 * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      goto L003B5E;
    }
    L003B5E:;
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
  }
  task_1C[arg1] = 1;
  task_F8[arg1] = 0;
  task_A0[arg1] = 0;
  task_74[arg1] = 0;
  Script_srchControl(0);
  return;
}

static void script_3BB0(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (task_124 == arg0) {
    task_10.mode = 0;
    Script_PlayCharaMotion(task_124, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
  }
  task_1C[arg1] = 1;
  task_F8[arg1] = 0;
  task_A0[arg1] = 0;
  task_74[arg1] = 0;
  Script_srchControl(0);
  return;
}

static void script_3C34(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_3AC1(arg0, arg2, 0);
  return;
}

static void script_3C44(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3386(arg0, arg1, -1, 0.0f, 0.5f, -1);
    if (local_FC == 0) {
      local_FC = script_3386(arg0, arg1, 5, 0.45f, 0.8f, arg3);
    } else {
      local_FC = script_3386(arg0, arg1, 5, 0.45f, 0.8f, arg3);
    }
    script_17B9(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_3AC1(arg0, arg2, 0);
  return;
}

static void script_3CD6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3386(arg0, arg1, 4, 0.5f, 0.8f, arg3);
    script_17B9(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_3AC1(arg0, arg2, 0);
  return;
}

static void script_3D23(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3386(arg0, arg1, 5, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_0030(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_3BB0(arg0, arg2, 0);
  return;
}

static void script_3D88(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3386(arg0, arg1, 5, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_0030(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_3BB0(arg0, arg2, 0);
  return;
}

static void script_3DED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3386(arg0, arg1, 4, 0.5f, 0.5f, arg3);
    script_0030(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_3BB0(arg0, arg2, 0);
  return;
}

static void script_3E3A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_3386(arg0, arg1, 0, 0.4f, 0.5f, arg3);
  }
  script_3AC1(arg0, arg2, 0);
  return;
}

static void script_3E6F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3386(arg0, arg1, 4, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_0CE1(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_3AC1(arg0, arg2, 1);
  return;
}

static void script_3ED4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3386(arg0, arg1, 4, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_0CE1(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_3AC1(arg0, arg2, 1);
  return;
}

static void script_3F39(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_3386(arg0, arg1, 4, 0.0f, 0.5f, arg3);
    script_0CE1(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_3AC1(arg0, arg2, 1);
  return;
}

static void script_3F7E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3386(arg0, arg1, -1, 0.4f, 0.5f, arg3);
    if (local_FC == 0) {
      local_FC = script_3386(arg0, arg1, 5, 0.5f, 0.8f, arg3);
    } else {
      local_FC = script_3386(arg0, arg1, 5, 0.5f, 0.8f, arg3);
    }
    script_11CD(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_3BB0(arg0, arg2, 0);
  return;
}

static void script_4013(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  Vector3i local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  Vector3 local_C0;
  Vector3 local_B4;
  float local_B0;
  float local_AC;
  float local_A8;
  float local_A4;
  int32_t local_A0;
  int32_t local_9C;
  int8_t local_9B;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_9B = script_3386(arg0, arg1, 5, 0.0f, 0.5f, arg3);
    script_38CF(arg0, 0);
    local_FC = Script_ScenDataUtil(data_7AA4, SCND_ReadU32, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4) * 5);
    local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_AC - 3.0f) << 16) & 0x7FFF0000);
    local_B4.x = (&task_12C)->x;
    (&local_B4)->y = task_12C.y;
    (&local_B4)->z = task_12C.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMoveTarget(arg0, &task_12C, *(&task_138 + 1), local_FC);
    while (Script_GetCharaMoveStatus(arg0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    *(int32_t *)&local_AC = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4) * 5 + 1);
    *(int32_t *)&local_A8 = Script_ScenDataUtil(data_7AA4, SCND_ReadFloatBits, Script_ScenDataUtil(data_7A30, SCND_ReadU8, arg3 * 4) * 5 + 2);
    Script_charaPosRotUtils(arg1, 12, &local_E4, 0, CHARA_PART_Get);
    Script_charaPosRotUtils(arg1, 7, &local_D8, 0, CHARA_PART_Get);
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
        break;
      }
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      local_B0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
      Script_ReadMOTNPositionAtFrame(arg0, local_FC, local_B0, &local_C0);
      if (!(local_B0 < local_AC) & !(local_B0 > local_A8)) {
        if (local_9B == 1) {
          local_A4 = local_A4 - 0.2f;
          if (((scn3_bits){.f = local_B0}).i == ((scn3_bits){.f = local_AC}).i) {
            uint8_t unused_98[3];
            int32_t local_94;
            local_94 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
            local_94 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_94 * 9 + 1) * 4 + 2);
            if (local_94 != -1) {
              Script_Execute3DSoundCommand_Wrapper(local_94, 4, &local_B4, 5.0f, 20.0f, 0);
            }
          }
          Script_EVT_GetPosition(arg0, 18, &local_CC, ENTITY_XFORM_MOTION_SPACE);
          (&local_CC)->z = (&local_CC)->z + (&local_C0)->z;
          (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D8)->x - (&local_CC)->x, (&local_D8)->z - (&local_CC)->z);
          (&local_F0)->y = (&local_F0)->y - (int32_t)((local_A4 + 90.0f) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
          Script_EVT_GetPosition(arg0, 12, &local_CC, ENTITY_XFORM_MOTION_SPACE);
          (&local_CC)->z = (&local_CC)->z + (&local_C0)->z;
          (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&local_CC)->x, (&local_E4)->z - (&local_CC)->z);
          (&local_F0)->y = (&local_F0)->y + (int32_t)((local_A4 + 90.0f) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
        } else {
          local_A4 = local_A4 - 0.2f;
          if (((scn3_bits){.f = local_B0}).i == ((scn3_bits){.f = local_AC}).i) {
            uint8_t unused_98[3];
            int32_t local_94;
            local_94 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
            local_94 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_94 * 9 + 1) * 4 + 2);
            if (local_94 != -1) {
              Script_Execute3DSoundCommand_Wrapper(local_94, 4, &local_B4, 5.0f, 20.0f, 0);
            }
          }
          Script_EVT_GetPosition(arg0, 18, &local_CC, ENTITY_XFORM_MOTION_SPACE);
          (&local_CC)->z = (&local_CC)->z + (&local_C0)->z;
          (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D8)->x - (&local_CC)->x, (&local_D8)->z - (&local_CC)->z);
          (&local_F0)->y = -((&local_F0)->y - (int32_t)((local_A4 + 90.0f) / 360.0f * (float)0x10000));
          Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
          Script_EVT_GetPosition(arg0, 12, &local_CC, ENTITY_XFORM_MOTION_SPACE);
          (&local_CC)->z = (&local_CC)->z + (&local_C0)->z;
          (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&local_CC)->x, (&local_E4)->z - (&local_CC)->z);
          (&local_F0)->y = -((&local_F0)->y + (int32_t)((local_A4 + 90.0f) / 360.0f * (float)0x10000));
          Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
        }
      }
      if (((scn3_bits){.f = local_B0}).i == ((scn3_bits){.f = local_A8 + 5.0f}).i) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    script_38CF(arg0, 1);
    local_A4 = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, (&local_F0)->y);
    local_A4 = local_A4 / 182.044449f;
    if (!(local_A4 <= 180.0f)) {
      local_A4 = 360.0f - local_A4;
    }
    if (!(local_A4 >= -180.0f)) {
      local_A4 = local_A4 + 360.0f;
    }
    local_A0 = 0x4000;
    local_9C = 0;
    local_A8 = 1.0f;
    local_FC = 0;
    {
      uint8_t unused_98[3];
      int32_t local_94;
      local_94 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
      local_94 = Script_ScenDataUtil(task_8, SCND_ReadU32, Script_ScenDataUtil(task_C, SCND_ReadU32, local_94 * 9 + 1) * 4 + 3);
      if (local_94 != -1) {
        Script_Execute3DSoundCommand_Wrapper(local_94, 4, &local_B4, 5.0f, 20.0f, 0);
      }
    }
    for (;;) {
      local_AC = local_A4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_A0) * local_A8;
      local_A8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_9C);
      if (local_FC == 0) {
        local_A0 = local_A0 + 0x200;
        if (local_A0 >= 0xC000) {
          local_FC = 1;
        }
      } else {
        local_A0 = local_A0 - 0x200;
        if (local_A0 <= 0x4000) {
          local_FC = 0;
        }
      }
      local_9C = local_9C + 128;
      if (local_9B == 1) {
        (&local_F0)->y = -(int32_t)(((float)0 - local_AC) / 360.0f * (float)0x10000);
        Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
        (&local_F0)->y = (int32_t)(((float)0 - local_AC) / 360.0f * (float)0x10000);
        Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
      } else {
        (&local_F0)->y = (int32_t)(((float)0 - local_AC) / 360.0f * (float)0x10000);
        Script_charaPosRotUtils(arg1, 7, 0, &local_F0, CHARA_PART_Set);
        (&local_F0)->y = -(int32_t)(((float)0 - local_AC) / 360.0f * (float)0x10000);
        Script_charaPosRotUtils(arg1, 12, 0, &local_F0, CHARA_PART_Set);
      }
      SCNF_WaitFrames_0000(1);
      if (local_9C >= 0x4000) {
        break;
      }
    }
  }
  script_3AC1(arg0, arg2, 0);
  return;
}

static void script_46B2(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_46DA(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static int32_t script_472E(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1)) {
      Script_EVT_GetPosition(arg1, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(arg0, 16, &local_F4, arg2);
      Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 30);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_477E(int32_t arg0) {
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

static void script_4825(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_477E(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_4848(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_487B(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_4848(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_489E(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_4848(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_48FD(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  return (int32_t)local_F8;
}

static void script_4912(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E0 = arg3;
  switch (local_E0) {
    case 0:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.01f;
      local_E8.x = 0;
      (&local_E8)->y = 64443;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 1:
      local_F4.x = 0.163f;
      (&local_F4)->y = 0.015f;
      (&local_F4)->z = 0.014f;
      local_E8.x = 0;
      (&local_E8)->y = 0x8000;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 2:
      local_F4.x = 0.064f;
      (&local_F4)->y = -0.071f;
      (&local_F4)->z = -0.366f;
      local_E8.x = 22570;
      (&local_E8)->y = 47422;
      (&local_E8)->z = 50974;
      local_E4 = 18;
      break;
    case 3:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.036f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 4:
      local_F4.x = 0.563f;
      (&local_F4)->y = -0.18f;
      (&local_F4)->z = -0.033f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0x3000;
      local_E4 = 18;
      break;
    case 5:
      local_F4.x = 0.129f;
      (&local_F4)->y = -0.037f;
      (&local_F4)->z = -0.233f;
      local_E8.x = 18313;
      (&local_E8)->y = 46148;
      (&local_E8)->z = 56615;
      local_E4 = 18;
      break;
    case 6:
      local_F4.x = 0.143f;
      (&local_F4)->y = 0.024f;
      (&local_F4)->z = 0.01f;
      local_E8.x = 10920;
      (&local_E8)->y = 11829;
      (&local_E8)->z = 4550;
      local_E4 = 18;
      break;
    case 7:
      local_F4.x = -0.066f;
      (&local_F4)->y = -1.05f;
      (&local_F4)->z = -0.413f;
      local_E8.x = 5367;
      (&local_E8)->y = 0x2000;
      (&local_E8)->z = 2274;
      local_E4 = 18;
      break;
    case 8:
      local_F4.x = 0.135f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.384f;
      local_E8.x = 46789;
      (&local_E8)->y = 49970;
      (&local_E8)->z = 20929;
      local_E4 = 18;
      break;
    case 9:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
  }
  if (Script_motnUtils(MOTN_GetCharacterBudgetFlag, arg0)) {
    Script_motnUtils(MOTN_SetCharacterBudgetFlag, arg0, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_PlayCharaMotion(arg0, arg2, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, local_E4, &local_F4, &local_E8);
  Script_motmGetCharaFlags(arg0, MOTM_PoseHigh, SCEN_Write);
  if (Script_motnUtils(MOTN_GetCharacterBudgetFlag, arg0)) {
    Script_motnUtils(MOTN_SetCharacterBudgetFlag, arg0, 1);
  }
  return;
}

static void script_4CA8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E0 = arg3;
  switch (local_E0) {
    case 0:
      local_F4.x = 0.066f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.016f;
      local_E8.x = 0xC000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0xD000;
      local_E4 = 18;
      break;
    case 1:
      local_F4.x = 0.071f;
      (&local_F4)->y = -0.9f;
      (&local_F4)->z = -0.197f;
      local_E8.x = 1820;
      (&local_E8)->y = -64350;
      (&local_E8)->z = 364;
      local_E4 = 18;
      break;
    case 2:
      local_F4.x = 0.147f;
      (&local_F4)->y = -0.86f;
      (&local_F4)->z = -0.35f;
      local_E8.x = 3913;
      (&local_E8)->y = 0;
      (&local_E8)->z = 910;
      local_E4 = 18;
      break;
    case 3:
      local_F4.x = -0.29f;
      (&local_F4)->y = -1.095f;
      (&local_F4)->z = -0.123f;
      local_E8.x = 43146;
      (&local_E8)->y = 47880;
      (&local_E8)->z = 19113;
      local_E4 = 18;
      break;
    case 4:
      local_F4.x = 0.113f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.035f;
      local_E8.x = 182;
      (&local_E8)->y = 62441;
      (&local_E8)->z = 65171;
      local_E4 = 18;
      break;
    case 5:
      local_F4.x = 0.088f;
      (&local_F4)->y = 0.036f;
      (&local_F4)->z = -0.048f;
      local_E8.x = 5279;
      (&local_E8)->y = 37523;
      (&local_E8)->z = 5825;
      local_E4 = 18;
      break;
    case 6:
      local_F4.x = 0.103f;
      (&local_F4)->y = 0.012f;
      (&local_F4)->z = 0.021f;
      local_E8.x = 59892;
      (&local_E8)->y = 3845;
      (&local_E8)->z = 364;
      local_E4 = 12;
      break;
    case 7:
      local_F4.x = 0.1136f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 0.013f;
      local_E8.x = 44540;
      (&local_E8)->y = 43679;
      (&local_E8)->z = 13666;
      local_E4 = 18;
      break;
    case 8:
      local_F4.x = 0.131f;
      (&local_F4)->y = -0.045f;
      (&local_F4)->z = -0.375f;
      local_E8.x = 18204;
      (&local_E8)->y = 47058;
      (&local_E8)->z = 58166;
      local_E4 = 18;
      break;
    case 9:
      local_F4.x = 0.07f;
      (&local_F4)->y = 0.01f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0x4000;
      (&local_E8)->y = 0xC000;
      (&local_E8)->z = 0xA000;
      local_E4 = 18;
      break;
    case 10:
      local_F4.x = 0.049f;
      (&local_F4)->y = 0.001f;
      (&local_F4)->z = -0.011f;
      local_E8.x = 64169;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 11:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 12:
      local_F4.x = 0.114f;
      (&local_F4)->y = -0.018f;
      (&local_F4)->z = -0.055f;
      local_E8.x = 0x1000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 13:
      local_F4.x = 0.257f;
      (&local_F4)->y = -0.072f;
      (&local_F4)->z = 0.064f;
      local_E8.x = 0xF000;
      (&local_E8)->y = 0xA000;
      (&local_E8)->z = 0x2000;
      local_E4 = 18;
      break;
    case 14:
      local_F4.x = 0.061f;
      (&local_F4)->y = 0.015f;
      (&local_F4)->z = 0.026f;
      local_E8.x = 19842;
      (&local_E8)->y = 61986;
      (&local_E8)->z = 55977;
      local_E4 = 12;
      break;
    case 15:
      local_F4.x = 0.2f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 0.03f;
      local_E8.x = 0xC000;
      (&local_E8)->y = 30947;
      (&local_E8)->z = 0x4000;
      local_E4 = 12;
      break;
    case 16:
      local_F4.x = 0.089f;
      (&local_F4)->y = 0.016f;
      (&local_F4)->z = -0.017f;
      local_E8.x = 0x4000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 17:
      local_F4.x = 0.11f;
      (&local_F4)->y = 0.055f;
      (&local_F4)->z = -0.005f;
      local_E8.x = 47336;
      (&local_E8)->y = 18232;
      (&local_E8)->z = 59134;
      local_E4 = 18;
      break;
    case 18:
      local_F4.x = 0.132f;
      (&local_F4)->y = 0.002f;
      (&local_F4)->z = -0.019f;
      local_E8.x = 36416;
      (&local_E8)->y = 33701;
      (&local_E8)->z = 47303;
      local_E4 = 18;
      break;
    case 19:
      local_F4.x = 0.134f;
      (&local_F4)->y = -0.033f;
      (&local_F4)->z = -0.036f;
      local_E8.x = 0;
      (&local_E8)->y = 41859;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
  }
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_PlayCharaMotion(arg0, arg2, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, local_E4, &local_F4, &local_E8);
  Script_motmGetCharaFlags(arg0, MOTM_PoseHigh, SCEN_Write);
  return;
}

static void script_5336(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    task_170[local_FF] = -1;
    local_FF = local_FF + 1;
  }
  task_1B4 = -1;
  task_1B8 = -1;
  task_1BC = -1;
  task_19C = -1;
  (void)((task_1D6 = 0) & 2);
  (void)((task_1D7 = 0) & 2);
  (void)((task_1D8 = 0) & 2);
  (void)((task_1DB = 0) & 2);
  task_1D0 = 0x3F4CCCCD;
  (void)((task_1D9 = 0) & 2);
  task_1D5 = -1;
  script_54C9();
  (void)((task_1DA = 0) & 2);
  Script_scenUtils(SCENARIO_CMD_SET_RUNTIME_FLAG_80, 0);
  return;
}

static void script_53D7(void) {
  (void)((task_1D6 = -1) & 2);
  while (task_1D6 & 0x80) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_53FA(void) {
  return;
}

static void script_53FD(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = -10;
  SCNF_CopyScenario_000E(1, task_1AC, (const void *)local_F8, 8);
  return;
}

static int32_t script_541E(int32_t arg0) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_14C)) {
    if (Script_charaHasMotionwork(task_14C) != 0) {
      (void)-1;
      goto L005446;
    }
  }
  (void)0;
  L005446:;
}

static void script_5447(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  if (Script_CycleManagerUtils(CYCLE_CMD_SIMSET_GLOBALFLAG_CHECK, arg0) == 1) {
    (void)0;
  } else {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 4, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_F4)->y - (&local_E8)->y <= 0.5f)) {
      (&local_F4)->y = 0.0f;
      (&local_E8)->y = 0.0f;
      if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &local_E8) >= 1.5f)) {
        (void)-1;
        goto L0054C8;
      }
    }
    (void)0;
  }
  L0054C8:;
  return;
}

static void script_54C9(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    task_150[local_FF] = -1;
    local_FF = local_FF + 1;
  }
  return;
}

static void script_54F4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  Script_sprintf("FTLK: Get ID list(%s)\012", script_48FD(arg0));
  local_F8 = Script_scenUtils(SCENARIO_CMD_STORE_RUNTIME_OBJECT_TEXT_OFFSET, arg0);
  if (local_F8 == 0) {
    Script_sprintf("FTLK: Illegal address (cast)\012");
  } else {
    local_F7 = 0;
    for (;;) {
      if (local_F7 > 8) {
        Script_sprintf("FTLK: Too many person\012");
        break;
      }
      local_FC = Script_ScenDataUtil((void *)local_F8, SCND_ReadU32, local_F7);
      if (local_FC == 0) {
        break;
      }
      task_150[local_F7] = -1;
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC)) {
        if (Script_charaHasMotionwork(local_FC)) {
          task_150[local_F7] = local_FC;
          if (local_FC != 0x52494B41) {
            Script_FaceLookAtTargetUtils(local_FC, FACE_LookReset);
          }
        }
      }
      local_F7 = local_F7 + 1;
    }
    if (local_F7 == 0) {
      Script_sprintf("FTLK: No person\012");
    }
  }
  return;
}

static int8_t script_55C6(int32_t arg0) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    if (task_150[local_FF] == arg0) {
      break;
    }
    local_FF = local_FF + 1;
  }
  return local_FF;
}

static void script_55FB(void) {
  script_5336();
  Script_scenUtils(SCENARIO_CMD_CREATE_SEQ_CHARACTERS);
  Script_scenCreateTask(script_5646, 0);
  return;
}

static void script_5614(void) {
  script_5336();
  Script_scenUtils(SCENARIO_CMD_CREATE_SEQ_CHARACTERS);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_scenCreateTask(script_5646, 0);
  return;
}

static void script_5633(void) {
  script_5336();
  Script_scenUtils(SCENARIO_CMD_CREATE_SEQ_CHARACTERS);
  script_5646();
  return;
}

__attribute__((scn_string_bindings(13, data_7DEB, 14, data_7DFF, 15, data_7E13)))
static int32_t script_5646(void) {
  int32_t local_F8[2];
  int32_t local_F4;
  int32_t local_F0;
  uint8_t unused_EF;
  int8_t local_EE;
  uint8_t unused_EC[2];
  int32_t local_E8;
  task_1A8 = SCNF_CopyScenario_000E(0, 0x414E4353);
  task_1AC = SCNF_CopyScenario_000E(0, 0x464E4353);
  for (;;) {
    SCNF_CopyScenario_000E(2, task_1A8, &task_1A0, 8);
    local_F8[0] = *&task_1A0;
    local_F8[1] = -10;
    task_14C = *&task_1A0;
    local_F4 = *(&task_1A0 + 1);
    task_190 = task_14C;
    (void)((local_EE = -(script_541E(task_14C) != 0)) & 8);
    if ((local_F4 >= 140) & (local_F4 <= 149)) {
      Script_sprintf("FTLK: ATURN0%d start\012", local_F4 - 140);
      script_54F4(task_14C);
      script_60C9(local_F4);
      Script_sprintf("FTLK: ATURN0%d end\012", local_F4 - 140);
    } else if ((local_F4 >= 40) & (local_F4 <= 49)) {
      Script_sprintf("FTLK: TURN0%d start\012", local_F4 - 40);
      script_54F4(task_14C);
      script_5F54(local_F4);
      Script_sprintf("FTLK: TURN0%d end\012", local_F4 - 40);
    } else if ((local_F4 >= 50) & (local_F4 <= 59)) {
      Script_sprintf("FTLK: RETURN0%d start\012", local_F4 - 50);
      script_54F4(task_14C);
      script_61E6(local_F4);
      Script_sprintf("FTLK: RETURN0%d end\012", local_F4 - 50);
    } else if (((local_F4 >= 20) & (local_F4 <= 29)) | ((local_F4 >= 120) & (local_F4 <= 129))) {
      if (local_F4 < 120) {
        Script_sprintf("FTLK: POINT0%d start\012", local_F4 - 20);
      } else {
        Script_sprintf("FTLK: APOINT0%d start\012", local_F4 - 120);
      }
      script_54F4(task_14C);
      script_6898(task_14C, local_F4);
      if (local_F4 < 120) {
        Script_sprintf("FTLK: POINT0%d end\012", local_F4 - 20);
      } else {
        Script_sprintf("FTLK: APOINT0%d end\012", local_F4 - 120);
      }
    } else {
      local_E8 = local_F4;
      if (local_E8 == -1) {
        Script_sprintf("FTLK: START start\012");
        *(&task_190 + 1) = 101;
        script_54F4(task_14C);
        if (local_EE & -(~task_1DB != 0)) {
          Vector3i object_0;
          Script_mtwkGetSetRotation(task_14C, &object_0, 0);
          task_1C4 = (&object_0)->y;
          task_1C8 = Script_scenCharaGetMotAnimId(task_14C);
          task_1CC = task_14C;
        }
        (void)((task_1DB = -1) & 0x10);
        if (task_19C != -1) {
          Script_srchControl((void *)task_19C);
          Script_sprintf("FTLK: Wait finish canseled(%d)\012", task_19C);
          task_19C = -1;
        }
        task_19C = -1;
        if (~task_1D9 & ~task_1DA) {
          if (Script_scenUtils(SCENARIO_CMD_RETURN_ZERO)) {
            local_F0 = 0;
          } else {
            Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            local_F0 = 2;
          }
        }
        if (Script_scenUtils(SCENARIO_CMD_QUERY_ACTIVE_FLAG_0x400000)) {
          Script_sprintf("FTLK: Start by BS\012");
        }
        if (~Script_scenUtils(SCENARIO_CMD_QUERY_ACTIVE_FLAG_0x400000) & -(task_1B4 == -1) & local_EE & ~task_1DA) {
          script_67C3(CHARA_AKIR, task_14C, (const uint16_t *)&data_7C64);
          if (*task_170 == -1) {
            if (task_14C == 0x4952554B) {
              Script_sprintf("FTLK: AKIRA ->> %s\012", script_48FD(task_14C));
              task_170[0] = Script_scenCreateTask(script_472E, 3, CHARA_AKIR, task_14C, local_F0);
            } else if (Script_CycleManagerUtils(CYCLE_CMD_SIMSET_GLOBALFLAG_CHECK, task_14C) == 1) {
              Script_sprintf("FTLK: AKIRA ->> %s\012", script_48FD(task_14C));
              task_170[0] = Script_scenCreateTask(script_472E, 3, CHARA_AKIR, task_14C, local_F0);
            } else {
              Script_sprintf("FTLK: AKIRA -> %s\012", script_48FD(task_14C));
              if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_14C)) {
                Vector3 object_1;
                Script_EVT_GetPosition(task_14C, 4, &object_1, ENTITY_XFORM_WORLD_SPACE);
                Script_SetLookPointDetails(CHARA_AKIR, 16, &object_1, local_F0);
                Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &object_1, 30);
              }
            }
          }
        }
        SCNF_CopyScenario_000E(1, task_1AC, (const void *)local_F8, 8);
        script_5F0E();
        Script_sprintf("FTLK: START end\012");
      } else if (local_E8 == -2) {
        Script_sprintf("FTLK: END start\012");
        task_190 = -1;
        *(&task_190 + 1) = 102;
        if (local_EE & 0x40) {
          Script_SetRyoMotionDetails(task_14C);
        }
        while (task_1B8 != -1) {
          SCNF_WaitFrames_0000(1);
        }
        while (task_1D8 & -(task_1BC != -1)) {
          SCNF_WaitFrames_0000(1);
        }
        local_F8[1] = -2;
        SCNF_CopyScenario_000E(1, task_1AC, (const void *)local_F8, 8);
        (void)((task_1DA = 0) & 0x40);
        script_54F4(task_14C);
        script_5EEE();
        Script_sprintf("FTLK: END end\012");
      } else if (local_E8 == -8) {
        Script_sprintf("FTLK: FINISH start\012");
        if ((task_14C == 0x4155524D) | (task_14C == 0x444E4953)) {
          Script_PlayCharaMotion(task_14C, task_1C8, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        } else {
          if (task_1DB & 0x10) {
            script_63B6(task_14C);
            goto L005B4B;
          }
        }
        L005B4B:;
        local_F8[1] = -8;
        Script_sprintf("FTLK: FINISH send (%s)\012", script_48FD(task_14C));
        SCNF_CopyScenario_000E(1, task_1AC, (const void *)local_F8, 8);
        script_5F31();
        (void)((task_1DB = 0) & 1);
        *(&task_190 + 1) = 103;
        Script_sprintf("FTLK: FINISH end\012");
      } else if (local_E8 == -3) {
        *(&task_190 + 1) = 104;
      } else if (local_E8 == -4) {
        *(&task_190 + 1) = 105;
      } else if (local_E8 == -5) {
        *(&task_190 + 1) = 106;
      } else if (local_E8 == -6) {
        *(&task_190 + 1) = 107;
      } else if (local_E8 == -7) {
        *(&task_190 + 1) = 108;
      } else if (local_E8 == 13) {
        Script_sprintf("FTLK: POINT_END star\012");
        script_6EFF(task_14C);
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        }
        Script_sprintf("FTLK: POINT_END end\012");
        SCNF_CopyScenario_000E(1, task_1AC, (const void *)local_F8, 8);
      } else if (local_E8 == 14) {
        Script_sprintf("FTLK: APOINT_END start\012");
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_14C)) {
          Script_SetCharaLookAtPosition(task_14C, -13, 0, 0);
          Script_FaceLookAtTargetUtils(task_14C, FACE_LookReset);
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        }
        Script_sprintf("FTLK: APOINT_END end\012");
        SCNF_CopyScenario_000E(1, task_1AC, (const void *)local_F8, 8);
      } else if (local_E8 == 10) {
        Script_sprintf("FTLK: SHOW_MEMO start\012");
        script_7161();
        Script_sprintf("FTLK: SHOW_MEMO end\012");
        SCNF_CopyScenario_000E(1, task_1AC, (const void *)local_F8, 8);
      } else if (local_E8 == 15) {
        Script_sprintf("FTLK: SHOW_END start\012");
        script_729D();
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_14C)) {
          Vector3 object_2;
          Script_EVT_GetPosition(task_14C, 4, &object_2, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_AKIR, 16, &object_2, 2);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &object_2, 30);
        }
        Script_sprintf("FTLK: SHOW_END end\012");
        SCNF_CopyScenario_000E(1, task_1AC, (const void *)local_F8, 8);
      } else if (local_E8 == 60) {
        Script_sprintf("FTLK: WAIT start\012");
        SCNF_WaitFrames_0000(60);
        Script_sprintf("FTLK: WAIT end\012");
        SCNF_CopyScenario_000E(1, task_1AC, (const void *)local_F8, 8);
      } else {
        Script_sprintf("FTLK: EXTRA start\012", local_F4);
        *(&task_190 + 1) = 109;
        *(&task_190 + 2) = local_F4;
        script_53D7();
        *(&task_190 + 2) = -1;
        Script_sprintf("FTLK: EXTRA end\012", local_F4);
        SCNF_CopyScenario_000E(1, task_1AC, (const void *)local_F8, 8);
      }
    }
    *(&task_190 + 1) = -1;
  }
  end:;
}

static void script_5E22(void) {
  (void)((task_1D6 = 0) & 0x80);
  return;
}

static void script_5E30(void) {
  (void)*&task_190;
  return;
}

static void script_5E38(void) {
  (void)*(&task_190 + 1);
  return;
}

static void script_5E43(void) {
  (void)*(&task_190 + 2);
  return;
}

static int32_t script_5E4E(void) {
  int32_t unused_FC;
  for (;;) {
    if (~task_1DB | (Script_GetControllerInput(0) & 0xF244) | (Script_GetControllerInput(5) != 128) | (Script_GetControllerInput(6) != 128)) {
      script_6C18();
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
        Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      }
      Script_SetMotnFlag(CHARA_AKIR, 0);
      script_5F31();
      script_54C9();
      Script_sprintf("FTLK: Wait finish end\012");
      task_19C = -1;
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_5EEE(void) {
  task_19C = Script_scenCreateTask(script_5E4E, 0);
  Script_sprintf("FTLK: Wait finish start(%d)\012", task_19C);
  return;
}

static void script_5F0E(void) {
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  return;
}

static void script_5F31(void) {
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  return;
}

static void script_5F54(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  {
    int32_t unused_F4;
    int8_t local_F3;
    int8_t local_F2;
    int8_t local_F1;
    local_F3 = arg0 - 40;
    local_F2 = local_F3 / 3 + 1;
    local_F1 = local_F3 % 3;
    if (local_F1 >= local_F2) {
      local_F1 = local_F1 + 1;
    }
    local_FC = task_150[local_F2];
    local_F8 = task_150[local_F1];
  }
  if ((Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC) == 0) | (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_F8) == 0)) {
    script_53FD(task_14C);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC) != 0) {
      local_FC = local_F8;
    }
    Script_sprintf("FTLK: %s not exists\012", script_48FD(local_FC));
  } else {
    int32_t unused_F4;
    int8_t local_F3;
    local_F3 = script_55C6(local_FC);
    if (task_170[local_F3] != -1) {
      Script_srchControl((void *)task_170[local_F3]);
    }
    Script_sprintf("FTLK: %s ->> ", script_48FD(local_FC));
    Script_sprintf("%s\012", script_48FD(local_F8));
    task_170[local_F3] = Script_scenCreateTask(script_472E, 3, local_FC, local_F8, 2);
    script_6251(local_FC, local_F8);
    if ((local_FC != 0x4155524D) & (local_FC != 0x444E4953)) {
      Script_scenCreateTask(script_633F, 1, local_FC);
    } else {
      Script_PlayCharaMotion(local_FC, Script_CycleManagerUtils(CYCLE_CMD_GET_VALUE_BY_CYCLE_ID, local_FC), -1.0f, -1.0f, -1.0f, 1.0f, 0);
    }
    script_53FD(task_14C);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_7FE5, 1, data_7FF5)))
static void script_60C9(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  if (Script_scenUtils(SCENARIO_CMD_RETURN_ZERO)) {
    Script_sprintf("自転車乗ってる\012");
    local_F8 = 0;
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    local_F8 = 2;
  }
  script_6C0C();
  local_FC = task_150[arg0 - 140 + 1];
  if ((local_FC != -1) & (*task_170 == -1)) {
    Script_sprintf("FTLK: AKIRA -> %s\012", script_48FD(local_FC));
    if (Script_CycleManagerUtils(CYCLE_CMD_SIMSET_GLOBALFLAG_CHECK, local_FC) == 1) {
      task_170[0] = Script_scenCreateTask(script_472E, 3, CHARA_AKIR, local_FC, local_F8);
    } else {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC)) {
        Script_EVT_GetPosition(local_FC, 4, &local_EC, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(CHARA_AKIR, 16, &local_EC, local_F8);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 30);
      }
    }
  } else {
    Script_sprintf("FTLK: AKIRA -X %s\012", script_48FD(local_FC));
  }
  script_53FD(task_14C);
  return;
}

static void script_61E6(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0 - 50 + 1;
  local_FC = task_150[local_F8];
  if (task_170[arg0 - 50 + 1] != -1) {
    script_6B7E(local_F8);
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC)) {
      Script_SetCharaLookAtPosition(local_FC, -13, 0, 0);
      Script_FaceLookAtTargetUtils(local_FC, FACE_LookReset);
    }
  }
  script_53FD(task_14C);
  return;
}

static void script_6251(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  Script_EVT_GetPosition(arg1, 4, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(arg0, 12, &local_E8, 2);
  SCNF_WaitFrames_0000(3);
  local_D0 = Script_motmUtilsEx(9, arg0, 0);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_DC, 0);
  local_D8 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E8)->x - (&local_F4)->x, (&local_E8)->z - (&local_F4)->z);
  local_D4 = (local_D8 - (&local_DC)->y) & 0xFFFF;
  if ((local_D4 < 18204) | (local_D4 > 47331)) {
    task_1C0 = (&local_DC)->y;
  } else if (local_D0 < 0) {
    local_D4 = 0x10000 - local_D4;
    task_1C0 = ((&local_DC)->y - local_D4 * 6 / 10) & 0xFFFF;
  } else {
    task_1C0 = (local_D4 * 6 / 10 + (&local_DC)->y) & 0xFFFF;
  }
  (void)0;
  return;
}

static int32_t script_633F(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_VALUE_BY_CYCLE_ID, arg0);
  if ((local_FC == -1) | (local_FC == 0)) {
    local_FC = Script_scenCharaGetMotAnimId(arg0);
  }
  Script_sprintf("FTLK: %s 's talk motion is %d\012", script_48FD(arg0), local_FC);
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_SIMSET_MODE01, arg0) == 0) {
    script_63FE(arg0, task_1C0, 1043, local_FC);
  } else {
    script_63FE(arg0, task_1C0, 32514, local_FC);
  }
  end:;
}

static void script_63B6(int32_t arg0) {
  if (arg0 != task_1CC) {
  } else if (Script_CycleManagerUtils(CYCLE_CMD_GET_SIMSET_MODE01, arg0) == 0) {
    script_63FE(arg0, task_1C4, 1043, task_1C8);
  } else {
    script_63FE(arg0, task_1C4, 32514, task_1C8);
  }
  return;
}

static void script_63FE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  float local_E0;
  int32_t local_DC;
  float local_D8;
  float local_D4;
  int32_t unused_D0;
  (void)((task_1D7 = -1) & 1);
  *(int32_t *)&local_D8 = task_1D0;
  task_1D0 = 0x3F4CCCCD;
  local_D4 = 1.0f / local_D8;
  if (~Script_scenCheckCharaExist(arg0)) {
    Script_sprintf("FTLK: Turn canseled 1\012");
    (void)((task_1D7 = 0) & 1);
  } else {
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    if ((&local_E8)->y == arg1) {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_18);
      Script_PlayCharaMotion(arg0, arg3, 0.0f, 0.0f, 0.0f, 1.0f, 0);
      Script_sprintf("FTLK: Set motion %d to %s 1\012", arg3, script_48FD(arg0));
    } else {
      local_F8 = Script_motmUtilsEx(1, arg0, 0);
      local_F4 = Script_motmUtilsEx(8, arg0, 0);
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
      local_FC = Script_motmUtilsEx(5, 0, arg2);
      local_E0 = (float)local_FC * local_D4 * 0.333333343f;
      local_DC = (int32_t)local_E0;
      Script_motmSetBlendFrames(arg0, local_DC);
      local_E0 = (float)local_FC * 0.166666672f;
      Script_mtwkGetSetRotation(arg0, &local_E8, 0);
      if (((arg1 - (&local_E8)->y) & 0xFFFF) > 0x8000) {
        Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
        local_E4 = script_674B((&local_E8)->y - 0x4000, arg1);
      } else {
        local_E4 = script_674B((&local_E8)->y + 0x4000, arg1);
      }
      SCNF_WaitFrames_0000((int32_t)((float)local_DC / 2.0f));
      if (~Script_scenCheckCharaExist(arg0)) {
        Script_sprintf("FTLK: Turn canseled 2\012");
        (void)((task_1D7 = 0) & 0x10);
        goto L00674A;
      }
      Script_motmUtilsEx(6, arg0, local_E4);
      Script_motmToggleFlags(arg0, SCEN_Disable, 4);
      Script_motmUtilsEx(0, arg0, 1);
      Script_PlayCharaMotion(arg0, arg2, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = (float)local_FC - local_E0}).i, ((scn3_bits){.f = local_E0}).i, local_D8, 0);
      if (Script_motmUtilsEx(7, 0, arg3)) {
        SCNF_WaitFrames_0000((int32_t)((float)local_FC * local_D4) - (int32_t)local_E0 * 2);
        if (~Script_scenCheckCharaExist(arg0)) {
          Script_sprintf("FTLK: Turn canseled 3\012");
          (void)((task_1D7 = 0) & 0x10);
          goto L00674A;
        }
        Script_PlayCharaMotion(arg0, arg3, 0.0f, 0.0f, 0.0f, local_D8, 0);
        Script_sprintf("FTLK: Set motion %d to %s 2\012", arg3, script_48FD(arg0));
        if (local_F4) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 4);
        }
        local_E8.x = 0;
        (&local_E8)->y = arg1;
        (&local_E8)->z = 0;
        Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
      } else {
        SCNF_WaitFrames_0000(local_DC + 1);
        if (~Script_scenCheckCharaExist(arg0)) {
          Script_sprintf("FTLK: Turn canseled 4\012");
          (void)((task_1D7 = 0) & 2);
          goto L00674A;
        }
        Script_motmUtilsEx(0, arg0, 1);
        Script_PlayCharaMotion(arg0, arg3, 0.0f, 0.0f, 0.0f, local_D8, 0);
        Script_sprintf("FTLK: Set motion %d to %s 3\012", arg3, script_48FD(arg0));
        if (local_F4) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 4);
        }
        local_E8.x = 0;
        (&local_E8)->y = arg1;
        (&local_E8)->z = 0;
        Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
        SCNF_WaitFrames_0000(local_DC);
        if (~Script_scenCheckCharaExist(arg0)) {
          Script_sprintf("FTLK: Turn canseled 5\012");
          (void)((task_1D7 = 0) & 1);
          goto L00674A;
        }
        Script_motmUtilsEx(0, arg0, local_F8);
      }
    }
    (void)((task_1D7 = 0) & 1);
  }
  L00674A:;
  return;
}

static int32_t script_674B(int32_t arg0, int32_t arg1) {
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

static void script_67C3(int32_t arg0, int32_t arg1, const uint16_t *arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t local_CC;
  int32_t local_C8;
  if (Script_scenUtils(SCENARIO_CMD_RETURN_ZERO)) {
    local_C8 = 0;
  } else {
    local_C8 = 2;
  }
  Script_EVT_GetPosition(arg1, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_GetPosition(arg0, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0x42000000);
  local_CC = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
  local_CC = (local_CC - (&local_D0)->y + 0x6000) & 0xFFFF;
  if (local_CC < 0xC000) {
    Script_SetCharaLookAtPosition(arg0, 13, &local_F4, local_C8);
  } else {
    local_DC.x = (&local_F4)->x;
    (&local_DC)->y = 0.0f;
    (&local_DC)->z = (&local_F4)->z;
    Script_SetCharaLookAtPosition(arg0, 12, &local_F4, local_C8);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_DC, arg2, 0.0f, 0, 0x200000);
  }
  return;
}

static void script_6898(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  int32_t local_D8;
  int32_t local_D4;
  (void)((task_1D8 = -1) & 0x10);
  {
    int32_t local_D0;
    local_D0 = script_55C6(arg0);
    if (local_D0 != 8) {
      script_6B7E(local_D0);
    }
  }
  {
    int32_t local_D0;
    if (arg1 < 120) {
      local_D0 = arg1 - 20;
    } else {
      local_D0 = arg1 - 120;
    }
    local_D4 = Script_scenUtils(SCENARIO_CMD_QUERY_NAMED_RUNTIME_OBJECT_EX, arg0, 6, local_D0);
  }
  if (local_D4 == 0) {
    Script_sprintf("FTLK: Illegal addres (point)\012");
    script_53FD(task_14C);
  } else {
    *(int32_t *)&local_E4 = Script_ScenDataUtil((void *)local_D4, SCND_ReadU32, 0);
    *(int32_t *)&local_E0 = Script_ScenDataUtil((void *)local_D4, SCND_ReadU32, 1);
    *(int32_t *)&local_DC = Script_ScenDataUtil((void *)local_D4, SCND_ReadU32, 2);
    Script_sprintf("FTLK: POINT TO (%f, %f, %f)\012", local_E4, local_E0, local_DC);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    local_D8 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, local_E4 - (&local_F4)->x, local_DC - (&local_F4)->z);
    local_D8 = script_674B((&local_E8)->y, local_D8) & 0xFFFF;
    if (local_D8 < 0x8000) {
      task_1C0 = (&local_E8)->y + local_D8 - 0x4000;
    } else {
      task_1C0 = (&local_E8)->y + local_D8 + 0x4000;
    }
    if ((local_D8 >= 21845) & (local_D8 <= 43690)) {
      Script_scenCreateTask(script_633F, 1, arg0);
      task_1D0 = 0x3FC00000;
      SCNF_WaitFrames_0000(1);
    }
    script_53FD(task_14C);
    while (task_1D7 & 1) {
      SCNF_WaitFrames_0000(1);
    }
    (void)((task_1D7 = 0) & 0x10);
    task_1D0 = 0x3F4CCCCD;
    if (arg1 < 120) {
      script_6EDB(arg0, 15, ((scn3_bits){.f = local_E4}).i, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = local_DC}).i, 2, 75, -1);
    } else {
      script_6CDD(arg0, ((scn3_bits){.f = local_E4}).i, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = local_DC}).i, 75);
    }
    SCNF_WaitFrames_0000(30);
    script_6C0C();
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
      Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    }
    if (task_1D9 & 4) {
      script_6CDD(CHARA_AKIR, ((scn3_bits){.f = local_E4}).i, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = local_DC}).i, 75);
    } else {
      script_6AD7(CHARA_AKIR, local_E4, local_E0, local_DC, (const uint16_t *)&data_7C64);
    }
  }
  return;
}

static void script_6AD7(int32_t arg0, float arg1, float arg2, float arg3, const uint16_t *arg4) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t local_D4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  if (Script_scenUtils(SCENARIO_CMD_RETURN_ZERO)) {
    local_D4 = 0;
  } else {
    local_D4 = 2;
  }
  Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg0, &local_DC, 0);
  local_D8 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
  local_D8 = (local_D8 - (&local_DC)->y + 15473) & 0xFFFF;
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, arg4, 0.0f, 0, 0);
  Script_SetCharaLookAtPosition(arg0, 12, &local_F4, local_D4);
  (void)((task_1D8 = 0) & 0x10);
  return;
}

static void script_6B7E(int32_t arg0) {
  if (task_170[arg0] == -1) {
  } else {
    Script_srchControl((void *)task_170[arg0]);
    SCNF_WaitFrames_0000(1);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_150[arg0])) {
      Script_SetCharaLookAtPosition(task_150[arg0], -13, 0, 0);
      Script_FaceLookAtTargetUtils(task_150[arg0], FACE_LookReset);
    }
    Script_sprintf("FTLK: %s finished looking\012", script_48FD(task_150[arg0]));
    task_170[arg0] = -1;
  }
  return;
}

static void script_6C0C(void) {
  script_6B7E(0);
  return;
}

static void script_6C18(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    script_6B7E(local_FF);
    local_FF = local_FF + 1;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_815A, 1, data_816E)))
static int32_t script_6C3F(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
    Script_sprintf("FTLK: %s not exist\012", script_48FD(arg0));
  } else {
    Script_SetCharaLookAtPosition(arg0, 10, &local_F4, 0);
    SCNF_WaitFrames_0000(arg4);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("FTLK: %s not exist\012", script_48FD(arg0));
    } else {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
        Script_SetCharaLookAtPosition(arg0, -13, 0, 0);
        Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
      }
    }
  }
  end:;
}

static void script_6CDD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_6C3F, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

__attribute__((scn_string_bindings(0, data_8182, 1, data_8196, 2, data_81A6)))
static int32_t script_6CF5(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5, int32_t arg6, int32_t arg7) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int8_t local_E7;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
    Script_sprintf("FTLK: %s not exist\012", script_48FD(arg0));
  } else {
    local_F4.x = arg2;
    (&local_F4)->y = arg3;
    (&local_F4)->z = arg4;
    if (Script_scenUtils(SCENARIO_CMD_RETURN_ZERO)) {
      Script_sprintf("自転車乗ってる\012");
      local_E8 = 0;
    } else {
      local_E8 = 2;
    }
    if (arg0 == 0x4B535248) {
      local_F0 = Script_HandleLookPointAnimationUpdate(arg0, arg1, &local_F4, arg5);
    } else {
      local_F0 = Script_HandleLookPoint(arg0, arg1, &local_F4, arg5);
    }
    if (local_F0) {
      local_EC = Script_CharHandlers(arg0, -3, 1, 0);
      local_F0 = 1;
    } else {
      local_EC = Script_CharHandlers(arg0, -3, 0, 0);
      local_F0 = 0;
    }
    Script_CharHandlers(arg0, local_F0, 4, 20);
    SCNF_WaitFrames_0000(arg6);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("FTLK: %s not exist\012", script_48FD(arg0));
    } else {
      script_6EFF(arg0);
      if (local_EC == -3) {
        Script_CharHandlers(arg0, -1, 0, 0);
      } else {
        Script_CharHandlers(arg0, local_F0, local_EC, 20);
      }
      if (arg7 != -1) {
        Script_PlayCharaMotion(arg0, arg7, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (task_1DB & 0x10) {
        if (arg0 == 0x52494B41) {
          if (*task_170 == -1) {
            task_170[0] = Script_scenCreateTask(script_472E, 3, CHARA_AKIR, task_14C, local_E8);
          }
        } else {
          local_E7 = script_55C6(arg0);
          if (task_170[local_E7] == -1) {
            task_170[local_E7] = Script_scenCreateTask(script_472E, 3, arg0, CHARA_AKIR, 2);
          }
        }
      }
      task_1BC = -1;
    }
  }
  end:;
}

static void script_6EDB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7) {
  task_1BC = Script_scenCreateTask(script_6CF5, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
  return;
}

static void script_6EFF(int32_t arg0) {
  Script_HandleLookPoint(arg0, -98312, 0, 2);
  return;
}

static void script_6F12(float arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = arg2;
  Script_convCtrlCamApply(1);
  Script_RunAuthorAndSetState(0, arg3, 0, 0, 0, &local_F4, 0, 0.0f, 0, 0);
  return;
}

static void script_6F4D(const Vector3 *arg0, float arg1) {
  Script_convCtrlCamApply(1);
  Script_RunAuthorAndSetState(0, arg1, 0, 0, 0, arg0, 0, 0.0f, 0, 0);
  return;
}

static void script_6F70(void) {
  Script_convCtrlCamApply(0);
  return;
}

static void script_6F7A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  task_1D4 = arg1;
  local_FC = arg1;
  if (local_FC == 1) {
    task_1B0 = arg0;
    Script_SetImageModuleTaskDataFlag(arg0, 0);
  } else {
    task_1D4 = 0;
  }
  return;
}

static int32_t script_6FAD(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  if (task_1D9 & 4) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_CHUGOSI_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_MID_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  if (task_1D4 == 1) {
    local_F4.x = 0.091f;
    (&local_F4)->y = 0.052f;
    (&local_F4)->z = 0.016f;
    local_E8.x = 0xA000;
    (&local_E8)->y = 0x5000;
    (&local_E8)->z = 0x5000;
    SCNF_WaitFrames_0000(30);
    if (0) {
      Script_charaDisplay(task_1B0, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_1B0, CDM_Show);
        goto L0070B1;
      }
    }
    L0070B1:;
    Script_scenToggleVisibleFor(task_1B0, 1);
    Script_fixoAttachEx(task_1B0, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  if (task_1D9 & 4) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_CHUGOSI_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  return task_1B4 = -1;
}

static void script_7161(void) {
  task_1B4 = Script_scenCreateTask(script_6FAD, 0);
  return;
}

static int32_t script_7175(void) {
  if (task_1B4 != -1) {
    Script_srchControl((void *)task_1B4);
    task_1B4 = -1;
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 4);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
  if (task_1D9 & 4) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_CHUGOSI_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_MID_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  if (task_1B0 != 0) {
    SCNF_WaitFrames_0000(25);
    Script_fixoClearAttachment(task_1B0);
    if (-1) {
      Script_charaDisplay(task_1B0, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_1B0, CDM_Show);
        goto L00724A;
      }
    }
    L00724A:;
    Script_scenToggleVisibleFor(task_1B0, 0);
  }
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return task_1B8 = -1;
}

static void script_729D(void) {
  task_1B8 = Script_scenCreateTask(script_7175, 0);
  return;
}

static int32_t script_72B1(int32_t arg0) {
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 8);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_7A04, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_738D();
  end:;
}

static int32_t script_7332(void) {
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_7A04, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 8);
  end:;
}

static void script_738D(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  for (;;) {
    Script_EVT_GetPosition(task_14C, 4, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_E8)->y - (&local_F4)->y <= 0.3f)) {
      script_742F();
      break;
    }
    if (~task_1D9) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_73E7(int32_t arg0) {
  if (task_1D9 & 4) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_scenCreateTask(script_72B1, 1, arg0);
    (void)((task_1D9 = -1) & 0x10);
  }
  return;
}

static void script_742F(void) {
  Script_scenCreateTask(script_7332, 0);
  (void)((task_1D9 = 0) & 0x10);
  return;
}

static void script_7447(int32_t arg0) {
  (void)((task_1DE = -1) & 0x10);
  (void)((task_1DF = 0) & 0x10);
  Script_ryoSetChara(arg0);
  Script_SetShouldSleepFlag(0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoControl(RYO_RestoreControl);
  SCNF_WaitFrames_0000(1);
  Script_CPCTRelated(arg0, 32, 89);
  Script_clmdSetFlags(arg0, 8, 1);
  (void)((task_1DC = 0) & 0x10);
  return;
}

static int32_t script_74E3(void) {
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (task_1DF & 1) {
  } else {
    Script_shenClearFreeState(1);
  }
  end:;
}

static void script_7525(void) {
  int8_t local_FF;
  (void)((local_FF = 0) & 0x10);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_shenClearFreeState(0);
  if (Script_ryoControl(PCCTL_QueryShouldRun)) {
    (void)((local_FF = -1) & 0x10);
    Script_ryoControl(PCCTL_RequestNewMode, 3);
    Script_ryoControl(PCCTL_DisablePlayerControl);
  }
  Script_executeAicaCommand(2720, 1, 15);
  Script_executeAicaCommand(2720, 5, 15);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (local_FF & 0x80) {
    Script_ryoControl(PCCTL_RequestNewMode, 0);
  }
  return;
}

static void script_75B1(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  float local_F0;
  if (~task_1DE) {
  } else {
    Script_ReturnTimeDate(&local_F4);
    if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 0)) {
      (void)((task_1DF = -1) & 8);
      Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
      Script_shenClearFreeState(0);
      Script_ryoControl(PCCTL_RequestNewMode, 1);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      Script_SetActionSelectorHUDElementVisibility(1);
      Script_GetConversationDetails(CONV_DETAILS_MODE_CLOSE_CONVERSATION_UI);
      Script_sicnUtils(1, 0, 0);
      SCNF_RequestIconNotification(0, 0, 0, 2);
      Script_GetConversationDetails(CONV_DETAILS_MODE_RESET_STATE);
      Script_GetConversationDetails(CONV_DETAILS_MODE_RESET_FOR_CPCT);
      Script_GetConversationDetails(CONV_DETAILS_MODE_UNUSED_3);
      Script_GetConversationDetails(CONV_DETAILS_MODE_SYNC_CASH_OVERLAY);
      Script_SetGameStateTo(AUTH_Scripted);
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      local_F0 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (!(local_F0 >= 1.0f)) {
        Script_ShowUIMessageFromID("TS1003A001");
      } else if (!(local_F0 >= 2.0f)) {
        Script_ShowUIMessageFromID("TS1003A002");
      } else {
        Script_ShowUIMessageFromID("TS1003A003");
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_uiUtils(5, 60, 2);
      SCNF_WaitFrames_0000(210);
      Script_uiUtils(7, 60, 2);
      Script_executeAicaCommand(2720, 5, 30);
      SCNF_WaitFrames_0000(60);
      SCNF_WaitFrames_0000(5);
      Script_ryoControl(PCCTL_RequestNewMode, 0);
      MODU_Script_shenRequestGameModule(Display, 80, 0x4E4B4954, 0);
    }
  }
  return;
}

static void script_772F(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  if (arg0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, arg1)) goto L007815;
    Script_ReturnTimeDate(&local_F4);
    if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 0)) goto L007815;
    Script_sicnUtils(0, 0, 1);
    (void)((task_1DC = -1) & 8);
    (void)((task_1DD = -1) & 8);
    while (1) {
      if (task_1DC == 0) {
        break;
      }
      local_F0 = Script_GetControllerInput(1);
      if (task_1DD & 0x40) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
          if (local_F0 & 0x800) {
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, arg1, 1);
            Script_sprintf("save!\012");
            Script_CaptureTrialSaveState();
            break;
          }
        }
      }
      SCNF_WaitFrames_0000(1);
    }
    if (task_1DC & 0x20) {
      Script_sicnUtils(1, 0, 0);
      (void)((task_1DC = 0) & 2);
    }
  } else {
    Script_sicnUtils(0, 0, 0);
  }
  L007815:;
  return;
}

void script_7816(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_uiUtils(1, 14, 0);
  Script_motiLoad(80, SHEN_NAME('TIKN'), "M_MOBJ.BIN");
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/80/TOUR", (int32_t)"SCRTEST.BIN", 0x494B5344);
  Script_LoadVoiceFile(1, "ALLVOICE");
  script_22C6(CHARA_AKIR, (DSWKDoorEntry *)&data_7C04, (int32_t *)data_7C20, (DoorResourceDescriptor *)&data_7C30, (MapObjectRecord *)&data_7C40, 10);
  Script_scenCreateTask(script_78B0, 1, CHARA_AKIR);
  Script_scenCreateTask(script_797D, 0);
  script_55FB();
  return;
}

static int32_t script_78B0(int32_t arg0) {
  Vector3 local_F4;
  Script_scenCreateTask(script_74E3, 0);
  script_7447(arg0);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, -0.4f, -5.7f, 6.0f, -5.7f, 6.0f, 0.7f, -0.4f, 0.7f, -0.1f, 3.0f);
  Script_ProcessFogFromTable(0);
  Script_SetupLightFromTable(0);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_24C6();
  for (;;) {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    if (!((&local_F4)->z <= -0.2f)) {
      script_7525();
      Script_shenRequestGameModuleTransition(80, SHEN_NAME('TIKN'), 2);
    }
    if ((&task_10)->mode == 0) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        script_75B1();
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_797D(void) {
  while (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x444E4953) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/80/TOUR", "MODELS") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_2529();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, -1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  return Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
}
