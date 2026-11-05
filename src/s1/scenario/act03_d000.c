#include "act03_d000.h"

__attribute__((scn_string_bindings(0, data_2A818)))
static int32_t script_0030(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  float local_F4[3];
  int32_t local_E8[3];
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  float local_D8;
  float local_D4;
  int32_t local_D0;
  int8_t local_CF;
  int8_t local_CE;
  int8_t local_CD;
  int8_t local_CC;
  int8_t local_CB;
  int8_t local_CA;
  script_0932(arg0, arg1, arg2, arg3, arg4, arg5);
  local_E8[0] = local_E8[2] = 0;
  while (1) {
    while (*(&task_78 + arg5) & 4) {
      SCNF_WaitFrames_0000(1);
    }
    arg2 = Script_KaidanUtils(CAM_STAIR_QUERY_GUIDE_ENTRY, arg0, task_6C, (const void *)task_70, (const void *)task_74, task_8C);
    if (arg2 >= 0) {
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_2);
      Script_ccowUtils(arg0, CCOW_DisableHeightAdjustment);
      if (*(&task_78 + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      }
      (void)((local_CA = 0) & 1);
      local_E4 = arg2 * 6;
      *(int32_t *)&local_F4[1] = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, local_E4 + 1);
      *(int32_t *)&local_F4[0] = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, local_E4);
      *(int32_t *)&local_F4[2] = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, local_E4 + 2);
      local_DC = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, local_E4 + 4);
      local_E8[1] = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, local_E4 + 3);
      local_E0 = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, local_E4 + 5);
      local_D8 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1]);
      local_D4 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_E8[1]);
      script_20A3(arg0, arg5, arg2);
      *(&task_84 + arg5) = arg2 + 1;
      *((int8_t *)&task_B0 + arg5) = 3;
      script_2D34(arg0, arg5, local_DC);
      Script_CharHandlers(arg0, 2, 0, 16);
      (void)((local_CD = -((local_CB = 0) != 0)) & 1);
      if (local_DC & 0xF00 & 0x400) {
        script_0A25(1, arg2);
        if (local_DC & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      if (local_DC & 0x100) {
        (void)((local_CA = -1) & 1);
      }
      if (*(&task_78 + arg5) & 0x400) {
        Script_MotStepUtil(arg0, 0);
      }
      if ((local_DC & 0xF) <= 4) {
        Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
        if ((*(&task_78 + arg5) & 8) == 0) {
          if (local_DC & 0x1000000) {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          } else {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          }
        }
        if (local_DC & 0x2000) {
          task_B5 = 0;
        } else {
          uint8_t unused_C8[2];
          int32_t local_C4;
          local_C4 = *(&task_7C + arg5);
          switch (local_C4) {
            case 0:
              task_B5 = 0;
              break;
            case 1:
              task_B5 = 1;
              break;
            case 3:
              task_B5 = 1;
              break;
            case 2:
              if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                task_B5 = 1;
              } else {
                task_B5 = 0;
              }
              break;
          }
        }
        if (task_B5) {
          local_F4[0] = *local_F4 + local_D8 * -0.1f;
          local_F4[2] = local_F4[2] + local_D4 * -0.1f;
        } else {
          local_F4[0] = *local_F4 + local_D8 * 0.16f;
          local_F4[2] = local_F4[2] + local_D4 * 0.16f;
        }
        local_CF = script_1B07(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
        if (local_CA & 4) {
          Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_70);
        }
        (void)((local_CC = -1) & 1);
        Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
        Script_ccowUnknown(arg0, 1, 3.0f);
      } else {
        if ((local_DC & 0xF) <= 12) {
          if ((*(&task_78 + arg5) & 8) == 0) {
            Script_scenCreateTask(script_24C5, 2, arg0, 0);
          }
          if (local_DC & 0x2000) {
            task_B5 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_7C + arg5);
            switch (local_C4) {
              case 0:
                task_B5 = 0;
                break;
              case 1:
                task_B5 = 1;
                break;
              case 3:
                task_B5 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_B5 = 1;
                } else {
                  task_B5 = 0;
                }
                break;
            }
          }
          if (task_B5) {
            local_F4[0] = *local_F4 + local_D8 * -0.25f;
            local_F4[2] = local_F4[2] + local_D4 * -0.25f;
          } else {
            local_F4[0] = *local_F4 + local_D8 * 0.15f;
            local_F4[2] = local_F4[2] + local_D4 * 0.15f;
          }
          local_CF = local_E0 - script_1D96(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
          if (local_CA & 4) {
            Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_70);
          }
          Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
          Script_ccowUnknown(arg0, 1, 3.0f);
          (void)((local_CC = 0) & 0x80);
          goto L000525;
        }
      }
      L000525:;
      if ((local_E0 == 1) & ((local_DC & 0xF) <= 4)) {
        if (*((int8_t *)&task_B3 + arg5) == 1) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
        }
        if (task_B5) {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 10.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        } else {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 20.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        local_D0 = 1;
        if (local_CC & 0x10) {
          Script_scenCreateTask(script_2ADB, 2, arg0, arg5);
        } else {
          Script_scenCreateTask(script_2C03, 2, arg0, arg5);
        }
        while ((local_CF >= 0) & (local_CF <= local_E0)) {
          local_CE = task_B5;
          if (local_D0) {
            local_D0 = 0;
          } else if (local_DC & 0x2000) {
            task_B5 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_7C + arg5);
            switch (local_C4) {
              case 0:
                task_B5 = 0;
                break;
              case 1:
                task_B5 = 1;
                break;
              case 3:
                task_B5 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_B5 = 1;
                } else {
                  task_B5 = 0;
                }
                break;
            }
          }
          if (local_CB & 8) {
          } else {
            if (*(&task_78 + arg5) & 0x100) {
              if (*(&task_78 + arg5) & 2) {
                if ((Script_GetControllerInput(0) & 0x2000) > 0) {
                  (void)((local_CD = -1) & 4);
                }
              }
            }
            if (local_CC & 0x10) {
              local_CF = local_CF + script_0E0C(arg0, local_CF, local_E0, arg5, local_DC, arg2, local_CE - task_B5);
            } else {
              local_CF = local_CF - script_12FC(arg0, local_CF, arg5, local_DC, arg2, local_CE - task_B5);
            }
            if ((local_CF > 0) & (local_CF < local_E0)) {
              if (!(local_CD & 0x20)) goto L000767;
            }
          }
          L000767:;
        }
        if (local_D0) {
          if (((*(&task_78 + arg5) & 0x10) == 0) & ((local_DC & 0xF) < 2)) {
            if (*((int8_t *)&task_B3 + arg5) == 1) {
              Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
            }
            if (task_B5) {
              Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              SCNF_WaitFrames_0000(1);
              for (;;) {
                if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 10.0f) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
            } else {
              Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              SCNF_WaitFrames_0000(1);
              for (;;) {
                if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 20.0f) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
            }
          }
        }
      }
      if ((task_B6 == 1) | (task_B6 == 2)) {
        task_B6 = 3;
      }
      if ((*(&task_78 + arg5) & 2) > 0) {
        if (task_B5) {
          Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
        } else {
          Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
        }
        if (task_BC & 0x10) {
          Script_sprintf("Kaidan : ");
          Script_ryoControl(RYO_RestoreControl);
          (void)((task_BC = 0) & 4);
        }
        script_2E09(arg0, arg5, local_DC);
      }
      Script_ccowUnknown(arg0, 2, 0.07f);
      Script_ccowUtils(arg0, CCOW_EnableHeightAdjustment);
      if (*((int8_t *)&task_B1 + arg5) != 2) {
        *((int8_t *)&task_B1 + arg5) = 0;
      }
      *((int8_t *)&task_B2 + arg5) = 0;
      *((int8_t *)&task_B0 + arg5) = 0;
      *(&task_84 + arg5) = -1;
      if (*(&task_78 + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      }
    } else {
      (void)((task_BC = 0) & 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0932(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  *(&task_78 + arg5) = arg4;
  *(&task_80 + arg5) = arg0;
  *(&task_84 + arg5) = -1;
  *(&task_7C + arg5) = arg1;
  *((int8_t *)&task_B1 + arg5) = 0;
  *((int8_t *)&task_B2 + arg5) = 0;
  Script_sprintf("========== initializing kaidan\012");
  if (arg3) {
    (void)((task_B7 = 0) & 1);
    task_8C = 0x4000;
    task_B4 = 1;
    task_70 = arg3;
    (void)((task_BC = 0) & 1);
    if (arg2) {
      Script_sprintf("please ENDOFKDATA use can't start shuki kaidan\012");
      task_6C = arg2;
    } else {
      Script_CycleManagerUtils(CYCLE_CMD_SET_GLOBAL_CYCLE_PTR, task_70, &task_84);
      task_6C = local_F8 = 0;
      while ((local_FC = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, local_F8)) != 0x47C35000) {
        task_6C = task_6C + 1;
        local_F8 = local_F8 + 6;
      }
    }
  } else {
    task_B4 = task_B4 + 1;
  }
  return;
}

static void script_0A25(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F4 = 0;
  local_FC = 0;
  while ((local_F8 = Script_ScenDataUtil((void *)task_74, SCND_ReadU32, local_FC)) != 3) {
    local_F0 = local_F8;
    if (local_F0 == 0) {
      local_FC = local_FC + 3;
    } else if (local_F0 == 1) {
      if (arg0) {
        if (Script_ScenDataUtil((void *)task_74, SCND_ReadU32, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_74, SCND_ReadU32, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_90[local_F4];
            break;
          } else {
            Script_PreserveAccumulator_012C(0, "Kaidan Function Error 1\012");
          }
        }
      }
      local_FC = local_FC + 3;
    } else if (local_F0 == 2) {
      if (arg0 == 0) {
        if (Script_ScenDataUtil((void *)task_74, SCND_ReadU32, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_74, SCND_ReadU32, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_90[local_F4];
            break;
          } else {
            Script_PreserveAccumulator_012C(0, "Kaidan Function Error 2\012");
          }
        }
      }
      local_FC = local_FC + 3;
    } else {
      Script_PreserveAccumulator_012C(0, "Kaidan Data Error\012");
    }
  }
  if (local_F4) {
    Script_scenCreateTask(local_F4, 0);
  }
  return;
}

static void script_0B64(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int8_t local_FF;
  int32_t local_F8;
  if (arg2 & 0x2000) {
    task_B5 = 0;
  } else {
    local_F8 = *(&task_7C + arg1);
    switch (local_F8) {
      case 0:
        task_B5 = 0;
        break;
      case 1:
        task_B5 = 1;
        break;
      case 3:
        task_B5 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_B5 = 1;
        } else {
          task_B5 = 0;
        }
        break;
    }
  }
  if (task_B5) {
    if (arg3 > 1) {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, 9.0f, 1.0f, 0);
      local_FF = 2;
      if (arg2 & 0x100000) {
        script_2739(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 8.0f, 1.0f, 1.0f, 0);
      local_FF = arg3;
      SCNF_WaitFrames_0000(1);
      while (Script_IsCharaMotionFinished(arg0) == 0) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else {
    if (arg3 > 1) {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, 0.0f, 29.0f, 5.0f, 1.0f, 0);
      local_FF = 1;
    } else {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, 0.0f, 18.0f, 5.0f, 1.0f, 0);
      local_FF = arg3;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)local_FF;
  return;
}

static void script_0D13(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  if (arg2 & 0x2000) {
    task_B5 = 0;
  } else {
    local_F8 = *(&task_7C + arg1);
    switch (local_F8) {
      case 0:
        task_B5 = 0;
        break;
      case 1:
        task_B5 = 1;
        break;
      case 3:
        task_B5 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_B5 = 1;
        } else {
          task_B5 = 0;
        }
        break;
    }
  }
  if (task_B5) {
    local_FF = 0;
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_SONOBA_LP, 0.0f, 29.0f, 18.0f, 1.0f, 0);
    local_FF = 0;
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)local_FF;
  return;
}

static int8_t script_0E0C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int8_t local_FF;
  uint8_t unused_F0[15];
  task_A0 = 0;
  if (task_B5) {
    local_FF = 4;
    if (arg1 < arg2 - 3) {
      Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
      if (arg6) {
        if (*((int8_t *)&task_B3 + arg3) == 1) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg4 & 0x100000) {
        script_2814(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
    } else {
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg4 & 0xF00 & 0x800) {
        script_0A25(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else if (arg2 <= arg1 + 2) {
        if (*((int8_t *)&task_B3 + arg3) == 0) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 8.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else {
        if (arg6) {
          if (*((int8_t *)&task_B3 + arg3) == 1) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, 5.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          while (Script_IsCharaMotionFinished(arg0) == 0) {
            SCNF_WaitFrames_0000(1);
          }
        } else {
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 5.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
        if (*((int8_t *)&task_B3 + arg3) == 0) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 8.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 < arg2 - 1) {
      if (arg6) {
        if (*((int8_t *)&task_B3 + arg3) == 1) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg4 & 0x100000) {
        script_265E(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg6) {
        if (*((int8_t *)&task_B3 + arg3) == 1) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg2 != arg1) {
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 13.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else {
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 3.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
      if (arg4 & 0xF00 & 0x800) {
        script_0A25(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 2);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg2 == arg1) {
          if (*((int8_t *)&task_B3 + arg3) == 0) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        } else {
          if (*((int8_t *)&task_B3 + arg3) == 1) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 25.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 3;
      }
    }
  }
  return local_FF;
}

static int8_t script_12FC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  if (task_B5) {
    local_FF = 4;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&task_B3 + arg2) == 1) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (arg3 & 0x100000) {
        script_29E5(arg0, arg2);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      if (*((int8_t *)&task_B1 + arg2) != 2) {
        *((int8_t *)&task_B1 + arg2) = 0;
      }
      switch (arg1) {
        case 1:
          if (arg5) {
            if (*((int8_t *)&task_B3 + arg2) == 1) {
              Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
            }
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 5.0f, 0.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            while (Script_IsCharaMotionFinished(arg0) == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            SCNF_WaitFrames_0000(1);
            for (;;) {
              if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 5.0f) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
          }
          break;
        case 2:
          if (arg5) {
            if (*((int8_t *)&task_B3 + arg2) == 1) {
              Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
            }
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 7.0f, 0.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            while (Script_IsCharaMotionFinished(arg0) == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            SCNF_WaitFrames_0000(1);
            for (;;) {
              if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 7.0f) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
          }
          break;
        case 3:
          if (arg5) {
            if (*((int8_t *)&task_B3 + arg2) == 1) {
              Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
            }
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 9.0f, 0.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            while (Script_IsCharaMotionFinished(arg0) == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            SCNF_WaitFrames_0000(1);
            for (;;) {
              if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 9.0f) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
          }
          break;
      }
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 2;
      }
      script_2DB9(arg0, arg2, arg3);
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg1 <= 2) {
          if (*((int8_t *)&task_B3 + arg2) == 1) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        } else {
          if (*((int8_t *)&task_B3 + arg2) == 0) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 8.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&task_B3 + arg2) == 1) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (arg3 & 0x100000) {
        script_28EF(arg0, arg2);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      if (*((int8_t *)&task_B1 + arg2) != 2) {
        *((int8_t *)&task_B1 + arg2) = 0;
      }
      if (arg1) {
        if (arg5) {
          if (*((int8_t *)&task_B3 + arg2) == 1) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_SONOBA_LP, 0.0f, 15.0f, 0.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          while (Script_IsCharaMotionFinished(arg0) == 0) {
            SCNF_WaitFrames_0000(1);
          }
        } else {
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 15.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 2;
      }
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
        script_2DB9(arg0, arg2, arg3);
      } else {
        if (arg1) {
          if (*((int8_t *)&task_B3 + arg2) == 1) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        } else {
          if (*((int8_t *)&task_B3 + arg2) == 0) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 10.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        script_2DB9(arg0, arg2, arg3);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 20.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 3;
      }
    }
  }
  return local_FF;
}

static void script_18C8(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_E8[3];
  if (arg5 & 0x2000) {
    task_B5 = 0;
  } else {
    int32_t unused_D4[5];
    int32_t local_D0;
    local_D0 = *(&task_7C + arg4);
    switch (local_D0) {
      case 0:
        task_B5 = 0;
        break;
      case 1:
        task_B5 = 1;
        break;
      case 3:
        task_B5 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_B5 = 1;
        } else {
          task_B5 = 0;
        }
        break;
    }
  }
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_charaToggleMotEnabled(arg0, 0);
  if (arg5 & 0x100) {
    int32_t unused_E4;
    float local_E0;
    float local_DC;
    float local_D8;
    local_E0 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
    local_DC = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
    local_D8 = (&local_F4)->x - arg2;
    {
      float local_D4;
      local_D4 = (&local_F4)->z - arg3;
      local_D8 = local_D8 * local_DC - local_D4 * local_E0;
    }
    {
      float local_D4;
      local_D4 = 0.0f;
      arg1 = 0x10000 - arg1;
      local_E0 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
      local_DC = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
      local_E8[0] = local_D8 * local_DC - local_D4 * local_E0;
      local_E8[2] = local_D8 * local_E0 + local_D4 * local_DC;
    }
    arg2 = *local_E8 + arg2;
    arg3 = local_E8[2] + arg3;
  }
  if (task_B5) {
    float local_E4;
    local_E4 = 3.0f;
    arg2 = arg2 - (&local_F4)->x;
    arg2 = arg2 / local_E4;
    arg3 = arg3 - (&local_F4)->z;
    arg3 = arg3 / local_E4;
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, 0.0f, 10.0f, 7.0f, 1.0f, 0);
  } else {
    float local_E4;
    local_E4 = 6.0f;
    arg2 = arg2 - (&local_F4)->x;
    arg2 = arg2 / local_E4;
    arg3 = arg3 - (&local_F4)->z;
    arg3 = arg3 / local_E4;
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, 0.0f, 8.0f, 2.0f, 1.0f, 0);
  }
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    local_F4.x = (&local_F4)->x + arg2;
    (&local_F4)->z = (&local_F4)->z + arg3;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    SCNF_WaitFrames_0000(1);
  }
  Script_charaToggleMotEnabled(arg0, 1);
  return;
}

static int32_t script_1B07(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  float local_CC;
  float local_C8;
  int32_t local_C4;
  float local_C0;
  float local_BC;
  int32_t local_B8;
  task_A0 = 0;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  if (arg5 & 0x100) {
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
    local_C0 = (&local_F4)->x - arg2;
    local_BC = (&local_F4)->z - arg3;
    local_C0 = local_C0 * local_C8 - local_BC * local_CC;
    local_BC = 0.0f;
    local_C4 = 0x10000 - arg1;
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_C4);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_C4);
    local_E8[0] = local_C0 * local_C8 - local_BC * local_CC;
    local_E8[2] = local_C0 * local_CC + local_BC * local_C8;
    arg2 = *local_E8 + arg2;
    arg3 = local_E8[2] + arg3;
  }
  if (task_B5) {
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
    if (Script_motmUtilsEx(13, arg0, 0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_A0 = -0x80000000;
      *((int8_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2CB, 0, -1.0f, 2);
      local_B8 = 2;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2CB, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_A0 = -0x80000000;
      *((int8_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C6, 0, -1.0f, 2);
      local_B8 = 1;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C6, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(0, data_2A8B7)))
static int32_t script_1D96(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  float local_CC;
  float local_C8;
  int32_t local_C4;
  float local_C0;
  float local_BC;
  int32_t local_B8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (arg5 & 0x100) {
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
    local_C0 = (&local_F4)->x - arg2;
    local_BC = (&local_F4)->z - arg3;
    local_C0 = local_C0 * local_C8 - local_BC * local_CC;
    local_BC = 0.0f;
    local_C4 = 0x10000 - arg1;
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_C4);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_C4);
    local_E8[0] = local_C0 * local_C8 - local_BC * local_CC;
    local_E8[2] = local_C0 * local_CC + local_BC * local_C8;
    arg2 = *local_E8 + arg2;
    arg3 = local_E8[2] + arg3;
  }
  task_A0 = 0;
  if (task_B5) {
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
    if (Script_motmUtilsEx(13, arg0, 0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_motmUtilsEx(14, arg0, 0x3F800000);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_A0 = -0x80000000;
      *((int8_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_A0 = -0x80000000;
      *((int8_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(arg0, CCOW_EnableAll);
  script_2D34(arg0, arg4, arg5);
  if (*(&task_78 + arg4) & 2) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_BC = -1) & 4);
    }
    Script_ccowUtils(arg0, CCOW_DisableCharacters);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(1, data_2A8DD)))
static void script_20A3(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  (void)((local_F7 = 0) & 4);
  arg2 = arg2 & -2;
  local_F8 = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, arg2 * 6 + 4);
  local_FC = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, (arg2 + 1) * 6 + 4);
  local_FC = local_FC | local_F8;
  if (local_FC & -0x80000000) {
    while (local_FC & -0x80000000) {
      if (local_F7 & 0x80) {
        if (task_C0) {
        } else {
          Script_sprintf("wait ... shuki shuki shuki\012");
        }
      } else {
        if (*(&task_78 + arg1) & 2) {
          if (Script_ryoControl(PCCTL_QueryShouldRun)) {
            Script_sprintf("Kaidan : ");
            Script_ryoControl(PCCTL_DisablePlayerControl);
            (void)((task_BC = -1) & 4);
          }
          Script_ccowUtils(arg0, CCOW_DisableCharacters);
        }
        Script_charaToggleMotEnabled(arg0, 0);
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        } else {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        }
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        Script_charaToggleMotEnabled(arg0, 1);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_MOVE_B_45, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        (void)((local_F7 = -1) & 4);
      }
      task_C0 = (task_C0 + 1) & 0xF;
      local_F8 = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, arg2 * 6 + 4);
      local_FC = Script_ScenDataUtil((void *)task_70, SCND_ReadU32, (arg2 + 1) * 6 + 4);
      local_FC = local_FC | local_F8;
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static void script_22D1(int32_t arg0) {
  if (~task_B7) {
    (void)((task_B7 = -1) & 1);
    task_A4 = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
    task_A8 = Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, -1.0f);
    task_AC = Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, -1.0f);
  }
  Script_SetCameraSettings(6, 4);
  Script_convSetCamInactiveFlag(0);
  if (arg0) {
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 3.1f);
    Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, 0.0f);
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, 1.0f);
  } else {
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 2.0f);
    Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, 2.0f);
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, 1.0f);
  }
  return;
}

static void script_2370(float arg0) {
  int32_t local_FC;
  float local_F8;
  local_FC = 0;
  while (local_FC < 10) {
    int32_t local_F4;
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L0023A3;
    }
    L0023A3:;
    local_F8 = task_AC + (arg0 - task_AC) * (float)local_FC / 10.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, arg0);
    }
  }
  return;
}

static void script_23FF(float arg0) {
  int32_t local_FC;
  float local_F8;
  local_FC = 0;
  while (local_FC < 50) {
    int32_t local_F4;
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L002432;
    }
    L002432:;
    local_F8 = task_A4 + (arg0 - task_A4) * (float)local_FC / 50.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, arg0);
    }
  }
  return;
}

static void script_248E(void) {
  Script_SetCameraSettings(6, 7);
  Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, task_A4);
  Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, task_A8);
  Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, task_AC);
  Script_KaidanUtils(CAM_STAIR_RESET_RETRACTION_LIMIT);
  (void)((task_B7 = 0) & 2);
  return;
}

static int32_t script_24C5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  int8_t local_E6;
  task_88 = arg0;
  task_B6 = 1;
  local_E7 = -1;
  (void)((local_E6 = 0) & 2);
  Script_SetGameStateTo(AUTH_None);
  while (task_88 == arg0) {
    if (Script_GetAuthorState_And_SetCamPos(&local_F4, &local_E8) != 17) {
      if (task_B6 != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_B6;
        if (local_E0 == 1) {
          (void)((local_E6 = -1) & 2);
          script_22D1(arg1);
        } else if (local_E0 == 2) {
          script_248E();
        } else if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_88 = 0;
        } else {
          if (local_E0 == 4) goto L002572;
        }
        L002572:;
        local_E7 = task_B6;
      }
    } else {
      if (task_B6 != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_B6;
        if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_88 = 0;
        } else if ((local_E0 == 1) | (local_E0 == 2)) {
          Script_SetGameStateTo(AUTH_FirstPerson);
        } else {
          if (local_E0 == 4) goto L0025D2;
        }
        L0025D2:;
        local_E7 = 4;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if (task_B6 != 0) {
    if (local_E6 & 0x40) {
      script_248E();
    }
  }
  end:;
}

static void script_25FC(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  local_F8 = -1;
  local_FF = 0;
  while (local_FF < task_B4) {
    if (arg0 == *(&task_80 + local_FF)) {
      local_F8 = *(&task_84 + local_FF);
    }
    local_FF = local_FF + 1;
  }
  (void)local_F8;
  return;
}

static void script_2643(int32_t arg0) {
  task_74 = arg0;
  return;
}

static void script_264D(int32_t arg0) {
  task_8C = arg0;
  return;
}

static void script_2657(void) {
  (void)task_8C;
  return;
}

__attribute__((scn_string_bindings(0, data_2A8E7)))
static void script_265E(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
  local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = (&local_F4)->x - local_E4 * 0.6f;
  (&local_F4)->y = (&local_F4)->y + 0.45f;
  (&local_F4)->z = (&local_F4)->z - local_E0 * 0.6f;
  local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
  if (!(local_DC < (&local_F4)->y - 0.1f)) {
    (&local_F4)->y = local_DC;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  } else {
    Script_sprintf("KAIDAN : Warning coli!\012");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2A8FF)))
static void script_2739(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
  local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = (&local_F4)->x - local_E4 * 0.480000019f;
  (&local_F4)->y = (&local_F4)->y + 0.359999985f;
  (&local_F4)->z = (&local_F4)->z - local_E0 * 0.480000019f;
  local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
  if (!(local_DC < (&local_F4)->y - 0.1f)) {
    (&local_F4)->y = local_DC;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  } else {
    Script_sprintf("KAIDAN : Warning coli!\012");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2A917)))
static void script_2814(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
  local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = (&local_F4)->x - local_E4 * 1.2f;
  (&local_F4)->y = (&local_F4)->y + 0.9f;
  (&local_F4)->z = (&local_F4)->z - local_E0 * 1.2f;
  local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
  if (!(local_DC < (&local_F4)->y - 0.1f)) {
    (&local_F4)->y = local_DC;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  } else {
    Script_sprintf("KAIDAN : Warning coli!\012");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2A92F)))
static void script_28EF(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&task_B2 + arg1) == 0) {
    *((int8_t *)&task_B2 + arg1) = 1;
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
    local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    local_F4.x = (&local_F4)->x - local_E4 * 0.6f;
    (&local_F4)->y = (&local_F4)->y - 0.45f;
    (&local_F4)->z = (&local_F4)->z - local_E0 * 0.6f;
    local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
    if (!(local_DC < (&local_F4)->y - 0.1f)) {
      (&local_F4)->y = local_DC;
      Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    } else {
      Script_sprintf("KAIDAN : Warning coli!\012");
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2A947)))
static void script_29E5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&task_B2 + arg1) == 0) {
    *((int8_t *)&task_B2 + arg1) = 1;
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
    local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    local_F4.x = (&local_F4)->x - local_E4 * 1.2f;
    (&local_F4)->y = (&local_F4)->y - 0.9f;
    (&local_F4)->z = (&local_F4)->z - local_E0 * 1.2f;
    local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
    if (!(local_DC < (&local_F4)->y - 0.1f)) {
      (&local_F4)->y = local_DC;
      Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    } else {
      Script_sprintf("KAIDAN : Warning coli!\012");
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2A95F)))
static int32_t script_2ADB(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&task_B1 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&task_B1 + arg1) = 1;
    if (task_B5) {
      local_FC = local_F8 = -0.001f;
    } else {
      local_FC = local_F8 = 0.0f;
    }
    while (*((int8_t *)&task_B1 + arg1) == 1) {
      if (task_B5) {
        if (!(local_F8 <= -0.16f)) {
          local_FC = local_F8 - 0.001f;
        } else {
          local_FC = -0.001f;
        }
      } else if (!(local_F8 >= 0.0f)) {
        local_FC = local_F8 + 0.001f;
      } else {
        local_FC = 0.0f;
      }
      Script_KaidanSetY(arg0, local_FC);
      local_F8 = local_FC;
      SCNF_WaitFrames_0000(1);
    }
    if (*((int8_t *)&task_B1 + arg1) != 2) {
      for (;;) {
        if (local_FC >= 0.0f) {
          break;
        }
        if (*((int8_t *)&task_B1 + arg1) == 2) {
          *((int8_t *)&task_B1 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC + 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&task_B1 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_2A977)))
static int32_t script_2C03(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&task_B1 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&task_B1 + arg1) = 1;
    if (task_B5) {
      local_FC = local_F8 = 0.16f;
    } else {
      local_FC = local_F8 = 0.08f;
    }
    while (*((int8_t *)&task_B1 + arg1) == 1) {
      if (task_B5) {
        if (!(local_F8 >= 0.16f)) {
          local_FC = local_F8 + 0.004f;
        } else {
          local_FC = 0.16f;
        }
      } else if (!(local_F8 <= 0.08f)) {
        local_FC = local_F8 - 0.004f;
      } else {
        local_FC = 0.08f;
      }
      Script_KaidanSetY(arg0, local_FC);
      local_F8 = local_FC;
      SCNF_WaitFrames_0000(1);
    }
    if (*((int8_t *)&task_B1 + arg1) != 2) {
      for (;;) {
        if (local_FC <= 0.0f) {
          break;
        }
        if (*((int8_t *)&task_B1 + arg1) == 2) {
          *((int8_t *)&task_B1 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC - 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&task_B1 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

static void script_2D34(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((*(&task_78 + arg1) & 2) > 0) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf(data_2A98F);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_BC = -1) & 4);
    }
    Script_ccowUtils(arg0, CCOW_DisableCharacters);
  }
  if (*(&task_78 + arg1) & 0x20) {
    Script_ccowUtils(arg0, CCOW_DisableMap);
  } else {
    if ((arg2 & 0x100) | (arg2 & 0x4000) | ((arg2 & 0x8000) > 0)) {
      Script_ccowUtils(arg0, CCOW_DisableMap);
      goto L002DB8;
    }
  }
  L002DB8:;
  return;
}

static void script_2DB9(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (*(&task_78 + arg1) & 0x20) {
    Script_ccowUtils(arg0, CCOW_EnableMap);
  } else if ((arg2 & 0x8000) > 0) {
  } else {
    if ((arg2 & 0x100) | ((arg2 & 0x4000) > 0)) {
      Script_ccowUtils(arg0, CCOW_EnableMap);
      goto L002E08;
    }
  }
  L002E08:;
  return;
}

static void script_2E09(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_ccowUtils(arg0, CCOW_EnableCharacters);
  if (*(&task_78 + arg1) & 0x20) {
    Script_ccowUtils(arg0, CCOW_EnableMap);
  } else {
    if ((arg2 & 0x100) | (arg2 & 0x4000) | ((arg2 & 0x8000) > 0)) {
      Script_ccowUtils(arg0, CCOW_EnableMap);
      goto L002E57;
    }
  }
  L002E57:;
  return;
}

static void script_2E58(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_31EE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_387C(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_3885(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_3894(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L0038C0;
    }
  }
  L0038C0:;
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

static void script_3921(int32_t arg0) {
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

static void script_3971(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_399F(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_39CD(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3A01(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3A35(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_3A53(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_3A8D(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_3AC1(void) {
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

static void script_3BB9(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_3BE9(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = 0;
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg0 < 69000) {
    local_FF = 1;
  }
  (void)local_FF;
  return;
}

static int32_t script_3C15(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int32_t local_F8;
  (void)((local_FF = 0) & 4);
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg1 >= arg0) {
    (void)((local_FF = -1) & 4);
  }
  return local_FF & 0x80;
}

static int8_t script_3C48(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  int8_t local_F3;
  int32_t local_EC;
  int32_t local_E8;
  local_F8 = 0;
  local_F4 = 0;
  local_FF = 0;
  local_EC = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_E8 = arg0;
  if ((local_E8 == 0) | (local_E8 == 1)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_SPEND_BUDGET_BONUS_ACTIVE)) {
      local_F4 = 320;
    }
  } else {
    if (local_E8 == 2) {
      local_F4 = 160;
      goto L003CB6;
    }
  }
  L003CB6:;
  local_F8 = local_F8 + local_F4;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_F8 = local_F8 + 69000;
    (void)((local_F3 = 0) & 1);
  } else if ((local_EC >= 2202) & (local_EC <= 2204)) {
    (void)((local_F3 = 0) & 1);
    local_F8 = local_F4 = 0;
    local_FF = 4;
  } else {
    (void)((local_F3 = -(script_3C15(local_F8, arg1) != 0)) & 0x10);
  }
  if (~local_F3) {
    if (local_F4 & -(local_F8 == local_F4)) {
      local_FF = 2;
    } else if (local_F8 > local_F4) {
      local_FF = 3;
    } else {
      if (local_FF == 0) {
        local_FF = 1;
      }
    }
    if (arg2) {
      script_3D8A(local_FF, arg0);
    }
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_2B550, 8, data_2B594, 9, data_2B59D, 10, data_2B5A6, 11, data_2B5AF, 12, data_2B5B8, 13, data_2B5C1)))
static void script_3D8A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (arg0 == 1) {
  } else {
    Script_LoadVoiceFile(1, "SY11");
    local_F4 = arg0;
    switch (local_F4) {
      case 2:
        if (arg1 == 2) {
          local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
          local_F4 = local_F8;
          switch (local_F4) {
            case 0:
              local_FC = Script_ShowUIMessageFromID("SY11A006");
              break;
            case 1:
              local_FC = Script_ShowUIMessageFromID("SY11A007");
              break;
            case 2:
              local_FC = Script_ShowUIMessageFromID("SY11A008");
              break;
          }
        } else {
          local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 4);
          local_F4 = local_F8;
          switch (local_F4) {
            case 0:
              local_FC = Script_ShowUIMessageFromID("SY11A002");
              break;
            case 1:
              local_FC = Script_ShowUIMessageFromID("SY11A003");
              break;
            case 2:
              local_FC = Script_ShowUIMessageFromID("SY11A004");
              break;
            case 3:
              local_FC = Script_ShowUIMessageFromID("SY11A005");
              break;
          }
        }
        break;
      case 3:
        local_FC = Script_ShowUIMessageFromID("SY11A001");
        break;
      case 4:
        local_F4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        if (local_F4 == 1) {
          local_FC = Script_ShowUIMessageFromID("SY11A009");
        } else if (local_F4 == 2) {
          local_FC = Script_ShowUIMessageFromID("SY11A010");
        } else {
          local_FC = Script_ShowUIMessageFromID("SY11A011");
        }
        break;
      case 5:
        local_FC = Script_ShowUIMessageFromID("SY11A009");
        break;
    }
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadVoiceFile(0, "SY11");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2B5C6, 1, data_2B5CB, 2, data_2B5D4, 3, data_2B5D9, 4, data_2B5DE, 5, data_2B5E7, 6, data_2B5F0, 7, data_2B5F9)))
static int8_t script_3F53(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FE = 1;
  local_FF = 0;
  local_F4 = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_FE = 0;
    if (arg1) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_FF = 1;
      }
      Script_LoadVoiceFile(1, "SY11");
      local_F8 = Script_ShowUIMessageFromID("SY11A001");
      while (Script_convGetIsActive((const void *)local_F8)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "SY11");
    }
  }
  if ((local_F4 >= 2202) & (local_F4 <= 2204)) {
    local_FE = 0;
    if (arg1) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_FF = 1;
      }
      Script_LoadVoiceFile(1, "SY11");
      local_F0 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_F0 == 1) {
        local_F8 = Script_ShowUIMessageFromID("SY11A009");
      } else if (local_F0 == 2) {
        local_F8 = Script_ShowUIMessageFromID("SY11A010");
      } else {
        local_F8 = Script_ShowUIMessageFromID("SY11A011");
      }
      while (Script_convGetIsActive((const void *)local_F8)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "SY11");
    }
  }
  if (arg1) {
    if (local_FF == 1) {
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_ryoControl(RYO_RestoreControl);
    }
  }
  return local_FE;
}

static int32_t script_4129(int32_t arg0, const char *arg1) {
  task_12A = 0;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
      break;
    }
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/stream/", arg1) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  }
  task_12A = 1;
}

static void script_419E(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static int32_t script_41BF(int32_t arg0, const char *arg1, int32_t arg2) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, arg0, arg1, arg2);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static int32_t script_4204(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (task_128 == 0) {
    Script_executeAicaCommand(arg0, 0, 0);
  }
  return Script_srchControl(0);
}

static void script_4232(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_41BF, 3, arg0, arg1, arg2);
  return;
}

static void script_4246(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_4204, 2, arg0, arg1);
  return;
}

static int32_t script_4258(const char *arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (task_128 == 0) {
    Script_StartOrFreeSEQ(0);
    Script_ShowUIMessageFromID(arg0);
  }
  return Script_srchControl(0);
}

static void script_4289(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_4258, 2, arg0, arg1);
  return;
}

static int32_t script_429B(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (task_128 == 0) {
    Script_fixoAttachEx(arg0, arg1, arg2, arg3, arg4);
  }
  return Script_srchControl(0);
}

static void script_42CE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_429B, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_42E8(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_fixoClearAttachment(arg0);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_srchControl(0);
}

static void script_4310(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_42E8, 2, arg0, arg1);
  return;
}

static void script_4322(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  if (arg1) {
    Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
    Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  }
  return;
}

static int32_t script_4360(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (task_128 == 0) {
    if (arg4 & -0x80000000) {
      Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
    } else {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
    }
    if (!(arg3 >= 0.1f)) {
      arg3 = -1.0f;
    }
    Script_PlayCharaMotion(arg0, arg1, arg2, ((scn3_bits){.f = arg3}).i, arg2, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_43D1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  if (task_128 == 0) {
    if (arg3 & -0x80000000) {
      Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
    } else {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
    }
    Script_PlayCharaMotion(arg0, arg1, arg2, -1.0f, -1.0f, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static void script_4433(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_43D1, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static void script_444B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_4360, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_4465(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_128 == 0) {
    Script_SetNewHandMotion(arg0, arg1, arg2, 0, 0, 0, 0, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static void script_44A3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_4465, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_44B9(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_128 == 0) {
    Script_SetHandRootRotation(arg0, arg1, 0x15, arg2);
  }
  return Script_srchControl(0);
}

static void script_44EA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_44B9, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_4500(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  if (task_128 == 0) {
    Script_FACE(arg0, arg1, arg2, arg3);
  }
  return Script_srchControl(0);
}

static void script_4531(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_4500, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_4549(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3) {
  int32_t local_FC;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_128 == 0) {
    local_FC = Script_CEWPRelated(arg0);
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
    Script_MoveObjectToObject(arg0, local_FC);
  }
  return Script_srchControl(0);
}

static void script_4596(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_4549, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_45AC(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  int32_t local_FC;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_128 == 0) {
    local_FC = Script_CEWPRelated(arg0);
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
    Script_MoveObjectToObject(arg0, local_FC);
  }
  return Script_srchControl(0);
}

static void script_45FB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_45AC, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_4611(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_128 == 0) {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return Script_srchControl(0);
}

static void script_463F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_4611, 4, arg0, arg1, arg2, arg3);
  return;
}

static void script_4655(int32_t arg0) {
  Vector4 local_F0;
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.2f;
  (&local_F0)->z = 0.2f;
  (&local_F0)->w = 0.2f;
  Script_SetCharaRefBoardPos(arg0, &local_F0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  return;
}

static void script_4694(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    Script_PrintDebugString(2, 4, "%03d", local_FC);
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_46C0(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 100.0f);
  if (~(arg2 & 2)) {
    if (local_FF > 74) {
      task_11C = 5;
      task_118 = (char *)data_2B615;
      arg1 = (int32_t)task_118;
    }
  }
  local_F8 = task_11C;
  switch (local_F8) {
    case 0:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 0, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 0) + 1);
      break;
    case 1:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 1, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 1) + 1);
      break;
    case 2:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 2, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 2) + 1);
      break;
    case 3:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 3, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 3) + 1);
      break;
    case 4:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 4, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 4) + 1);
      break;
    case 5:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 5, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 5) + 1);
      break;
  }
  script_4129(arg0, (const char *)arg1);
  script_4655(arg0);
  return;
}

static int32_t script_47E6(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 == 0) {
    Script_charaPosRotUtils(arg0, 152, &data_2B61C, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg0, 153, &data_2B628, 0, CHARA_PART_Set);
  } else {
    Script_charaPosRotUtils(arg0, 152, &data_2B628, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg0, 153, &data_2B61C, 0, CHARA_PART_Set);
  }
  return Script_srchControl(0);
}

static void script_4858(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_47E6, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_486C(int32_t arg0) {
  SCNF_WaitFrames_0000(73);
  SCNF_SetCashValue(-100, 1);
  arg0 = arg0 - 100;
  Script_SetStats(STATS_CASH, arg0);
  end:;
}

static int32_t script_4892(int32_t arg0) {
  SCNF_SetCashValue(100, 1);
  arg0 = arg0 + 100;
  Script_SetStats(STATS_CASH, arg0);
  end:;
}

static int32_t script_48B0(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  SCNF_StartIconSystemMenu(arg0);
  Script_GetCassetteFlags(arg0, 1, 0, 0);
  return Script_srchControl(0);
}

static void script_48DE(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_48B0, 2, arg0, arg1);
  return;
}

__attribute__((scn_string_bindings(0, data_2B634)))
static void script_48F0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int32_t local_E4;
  local_FF = 0;
  local_F8 = Script_shenGetStats(STATS_CASH);
  Script_ReturnTimeDate(&local_E8);
  if (Script_scenUtils(SCENARIO_CMD_QUERY_FOCUS_OR_TIME_LIMIT) == 0) {
    if (Script_IsPlayerAllowed(0)) {
      if (local_F8 > 99) {
        script_4CB9(arg0);
        Script_scenCreateTask(script_486C, 1, local_F8);
        Script_scenCreateTask(script_4129, 2, arg2, "VEND");
        script_5282(arg0, arg1);
        script_4D94(arg0, arg1, arg2, arg3);
        local_F4 = script_4F3E(arg0, arg1, arg2, arg3, 1);
        local_E4 = local_F4;
        switch (local_E4) {
          case 13:
            if (task_11C == 5) {
              script_5350(arg0, arg1, 1);
            } else {
              script_5641(arg0, arg1, 1);
            }
            break;
          case 16:
            local_F8 = Script_shenGetStats(STATS_CASH);
            Script_scenCreateTask(script_4892, 1, local_F8);
            break;
        }
        script_4E35(arg0, arg1, arg2, arg3);
      } else {
        if (Script_ryoControl(PCCTL_QueryShouldRun)) {
          local_FF = 1;
          Script_ryoControl(PCCTL_DisablePlayerControl);
        }
        script_3D8A(2, 0);
        if (local_FF == 1) {
          Script_ryoControl(RYO_RestoreControl);
        }
      }
    }
  }
  return;
}

static void script_4A21(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_4B5C(arg0, arg1, arg2, arg3);
  script_4F3E(arg0, arg1, arg2, arg3, 0);
  script_4E35(arg0, arg1, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_2B639)))
static void script_4A53(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_shenGetStats(STATS_CASH);
  if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
    if (local_FC > 99) {
      script_4CB9(arg0);
      Script_scenCreateTask(script_486C, 1, local_FC);
      Script_scenCreateTask(script_4129, 2, arg2, "VEND");
      script_5282(arg0, arg1);
      script_4D94(arg0, arg1, arg2, arg3);
      local_F8 = script_4F3E(arg0, arg1, arg2, arg3, 1);
      local_F4 = local_F8;
      switch (local_F4) {
        case 13:
          script_5641(arg0, arg1, 1);
          break;
        case 16:
          local_FC = Script_shenGetStats(STATS_CASH);
          Script_scenCreateTask(script_4892, 1, local_FC);
          break;
      }
      script_4E35(arg0, arg1, arg2, arg3);
    }
  }
  return;
}

static void script_4B22(void) {
  task_114 = 0x314E4143;
  task_11C = 3;
  task_118 = (char *)data_2B63E;
  task_128 = 0;
  task_129 = 0;
  task_12A = 0;
  task_130 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_2B643, 1, data_2B64A, 2, data_2B657)))
static void script_4B5C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Vector3 local_E8;
  script_4B22();
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, arg0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  task_120 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_120) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  }
  Script_LoadVoiceFile(1, "01JUCE");
  task_130 = Script_LoadHandMot(3, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
  Script_scenCreateTask(script_4129, 2, arg2, "VEND");
  while (task_12A != 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_facemotUtils(arg3, 1, 0);
  Script_ResetHandMotion(arg3, 1, 1, 0);
  Script_charaDisplay(arg1, CDM_Hide);
  Script_ccowUtils(arg1, CCOW_DisableAll);
  Script_EVT_GetPosition(arg1, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_F4, 0x40000000);
  Script_EVT_SetPosition(arg2, 0x78000000, &local_E8);
  Script_EVT_SetRotation(arg2, 0x78000000, &local_F4, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  script_4655(arg3);
  Script_InitMOTMGlobalState(arg0, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_2B65C, 2, data_2B67D)))
static void script_4CB9(int32_t arg0) {
  script_4B22();
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  task_120 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_120) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  }
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_LoadVoiceFile(1, "01JUCE");
  Script_sprintf("HandMot read by current!\012");
  task_130 = Script_LoadHandMot(3, SHEN_NAME('D000'), "HMOT_CAN.BIN");
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  return;
}

static void script_4D94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  while (task_12A != 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_facemotUtils(arg3, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_ResetHandMotion(arg3, 1, 1, 0);
  script_4655(arg3);
  Script_charaDisplay(arg1, CDM_Hide);
  Script_ccowUtils(arg1, CCOW_DisableAll);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_E8, 0x40000000);
  Script_EVT_SetPosition(arg2, 0x78000000, &local_F4);
  Script_EVT_SetRotation(arg2, 0x78000000, &local_E8, 0);
  Script_InitMOTMGlobalState(arg0, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_2B68A, 1, data_2B691)))
static void script_4E35(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Script_charaSetRefboardFlagTo(arg3, 0);
  Script_ResetHandMotion(arg3, 0, 1, 0);
  Script_faceUpdateFor(arg3, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  if (task_130 != 0) {
    Script_FreeTaskData(task_130);
  }
  Script_LoadVoiceFile(0, "01JUCE");
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetGameStateTo(AUTH_None);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
  Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  if (task_120) {
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(RYO_RestoreControl);
  }
  SCNF_WaitFrames_0000(1);
  script_419E("VEND");
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 2);
  Script_SetCPAMFlags(arg0, 1);
  task_129 = 14;
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_4F3E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t unused_E8[6];
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int8_t local_D7;
  uint8_t unused_D4[3];
  int32_t local_D0;
  local_DC = 0;
  local_D8 = 0;
  local_E4 = 1;
  local_E0 = -1;
  local_D7 = 0;
  Script_sicnUtils(0, 37, 1);
  local_D8 = Script_shenGetStats(STATS_CASH);
  local_D8 = local_D8 + 100;
  Script_SetStats(STATS_CASH, local_D8);
  local_D7 = script_3C48(0, 100, 0);
  local_D8 = Script_shenGetStats(STATS_CASH);
  local_D8 = local_D8 - 100;
  Script_SetStats(STATS_CASH, local_D8);
  if (local_D7 > 0) {
    Script_shenClearFreeState(0);
  }
  for (;;) {
    if (local_DC > 1) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
        if (local_E4 < 3) {
          Script_sprintf("vendmachine exit is EV_STOP\012");
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
          local_E4 = 16;
        }
      } else {
        if (local_E0 >= 1) {
          if (local_D7 > 0) {
            script_3C48(0, 100, 1);
            Script_sprintf("vendmachine exit is MONEY_LOCK -> %d,%d\012", local_D7, local_DC);
            local_E4 = 16;
          }
        }
      }
    }
    if (local_E0 != local_E4) {
      local_E0 = local_E4;
      task_129 = local_E4;
      local_D0 = local_E4;
      if (local_D0 == 1) {
        script_44EA(arg3, 1, (int32_t)data_2B698, -1);
        script_44A3(arg3, 1, 542, -1);
      } else if (local_D0 == 3) {
        task_129 = 1;
        Script_shenClearFreeState(0);
        Script_sicnUtils(1, 0, 0);
      } else if (local_D0 == 16) {
        Script_sicnUtils(1, 0, 0);
      } else if ((local_D0 == 2) | (local_D0 == 4)) {
        task_129 = 1;
      } else {
        switch (local_D0) {
          case 5:
            script_4246(0x5E05A9, -1);
            break;
          case 6:
            script_4246(0x6005A9, 5);
            break;
          case 8:
            task_118 = (char *)data_2B6EA;
            task_11C = 0;
            if (arg4 & 1) {
              script_46C0(task_114, (int32_t)task_118, arg4);
            }
            break;
          case 9:
            task_118 = (char *)data_2B6EF;
            task_11C = 1;
            if (arg4 & 1) {
              script_46C0(task_114, (int32_t)task_118, arg4);
            }
            break;
          case 10:
            task_118 = (char *)data_2B6F4;
            task_11C = 2;
            if (arg4 & 1) {
              script_46C0(task_114, (int32_t)task_118, arg4);
            }
            break;
          case 11:
            task_118 = (char *)data_2B6F9;
            task_11C = 3;
            if (arg4 & 1) {
              script_46C0(task_114, (int32_t)task_118, arg4);
            }
            break;
          case 12:
            task_118 = (char *)data_2B6FE;
            task_11C = 4;
            if (arg4 & 1) {
              script_46C0(task_114, (int32_t)task_118, arg4);
            }
            break;
        }
      }
    }
    local_E4 = Script_UpdateCharaSelectionScene(arg0, arg2, arg3, CHARA_SELECT_IDLE);
    if ((local_E0 == 13) | (local_E0 == 14) | (local_E0 == 15) | (local_E0 == 16)) {
      break;
    }
    local_DC = local_DC + 1;
    if (local_D7 < 1) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_charaDisplay(arg1, CDM_Show);
  Script_charaDisplay(arg2, CDM_Hide);
  Script_ccowUtils(arg1, CCOW_EnableAll);
  Script_charaDisplay(arg3, CDM_Hide);
  return local_E0;
}

static void script_5282(int32_t arg0, int32_t arg1) {
  Script_shenClearFreeState(0);
  script_4596(arg0, arg1, (int32_t)&data_2B704, -1);
  script_45FB(arg0, arg1, (int32_t)data_2B710, -1);
  script_44EA(arg0, 1, (int32_t)data_2B71C, -1);
  script_44EA(arg0, 0, (int32_t)data_2B71C, -1);
  script_44A3(arg0, 1, 541, -1);
  script_4433(arg0, 712, 0, -0x80000000, -1);
  script_4246(0x5F05A9, 125);
  script_463F(4013, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(3, data_2B785, 4, data_2B790)))
static void script_5350(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg2 == 0) {
    script_4129(task_114, task_118);
  }
  script_42CE(task_114, arg0, 18, (int32_t)&data_2B728, (int32_t)&data_2B734, 82);
  script_4858(task_114, 1, 0);
  script_4596(arg0, arg1, (int32_t)&data_2B740, -1);
  script_45FB(arg0, arg1, (int32_t)data_2B74C, -1);
  script_44EA(arg0, 1, (int32_t)&data_2B758, -1);
  script_44A3(arg0, 1, 545, -1);
  script_44A3(arg0, 0, 546, -1);
  script_4433(arg0, 45059, 0, -0x80000000, -1);
  script_4246(0x6505A9, 80);
  script_463F(4019, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    script_4289((int32_t)"01JUCEA011", -1);
  } else if (local_FC == 1) {
    script_4289((int32_t)"01JUCEA012", -1);
  } else {
    script_4289((int32_t)"01JUCEA013", -1);
  }
  SCNF_WaitFrames_0000(90);
  script_4531(arg0, 10, 5, 16, 180);
  script_4531(arg0, 9, 5, 16, 251);
  script_4531(arg0, 4, 5, 16, 351);
  script_4531(arg0, 1, 4, 16, 391);
  script_4531(arg0, 9, 5, 16, 426);
  script_4531(arg0, 10, 1, 16, 521);
  script_4531(arg0, 1, 2, 16, 545);
  script_4531(arg0, 0, 0, 16, 596);
  script_4246(0x6105A9, 108);
  script_4858(task_114, 0, 108);
  script_444B(arg0, 45058, 0, 0x441C0000, 0, -1);
  script_4246(0x3E05A9, 210);
  script_4246(0x3F05A9, 330);
  script_4246(0x3E05A9, 470);
  script_4246(0x3F05A9, 500);
  script_4289((int32_t)"01JUCEA002", 550);
  script_4289((int32_t)"01JUCEA003", 400);
  script_463F(4015, arg1, arg1, -1);
  script_463F(4016, arg1, arg1, 575);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_16);
  script_4433(arg0, 45061, 0x42100000, 0, -1);
  script_4310(task_114, 30);
  script_48DE(66, 32);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  script_419E(task_118);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_2B7FC, 1, data_2B807)))
static void script_5641(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 == 0) {
    script_4129(task_114, task_118);
  }
  if (task_11C == 4) {
    script_42CE(task_114, arg0, 18, (int32_t)&data_2B7B4, (int32_t)&data_2B7C0, 82);
    script_4596(arg0, arg1, (int32_t)&data_2B7CC, -1);
    script_45FB(arg0, arg1, (int32_t)data_2B7D8, -1);
    script_44EA(arg0, 1, (int32_t)data_2B7F0, -1);
    script_44A3(arg0, 1, 544, -1);
  } else {
    script_42CE(task_114, arg0, 18, (int32_t)&data_2B79C, (int32_t)&data_2B7A8, 82);
    script_4596(arg0, arg1, (int32_t)&data_2B7CC, -1);
    script_45FB(arg0, arg1, (int32_t)data_2B7D8, -1);
    script_44EA(arg0, 1, (int32_t)&data_2B7E4, -1);
    script_44A3(arg0, 1, 545, -1);
  }
  script_4858(task_114, 1, 0);
  script_44A3(arg0, 0, 546, -1);
  script_4433(arg0, 45059, 0, -0x80000000, -1);
  script_4246(0x6505A9, 80);
  script_4246(0x6105A9, 290);
  script_463F(4014, arg1, arg1, -1);
  script_4858(task_114, 0, 290);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  script_4531(arg0, 10, 5, 16, 180);
  script_4531(arg0, 9, 5, 16, 251);
  script_4531(arg0, 4, 5, 16, 351);
  script_4531(arg0, 1, 4, 16, 391);
  script_4531(arg0, 9, 5, 16, 426);
  script_4531(arg0, 10, 1, 16, 521);
  script_4531(arg0, 1, 2, 16, 545);
  script_4531(arg0, 0, 0, 16, 596);
  if (task_11C == 4) {
    script_4433(arg0, 45060, 0, 0, -1);
  } else {
    script_4433(arg0, 45058, 0, 0, -1);
  }
  script_4246(0x3E05A9, 210);
  script_4246(0x3F05A9, 330);
  script_4246(0x3E05A9, 470);
  script_4246(0x3F05A9, 500);
  script_4246(0x6405A9, 690);
  script_4289((int32_t)"01JUCEA002", 550);
  script_4289((int32_t)"01JUCEA003", 400);
  script_463F(4015, arg1, arg1, -1);
  script_463F(4016, arg1, arg1, 575);
  script_4310(task_114, 690);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  script_419E(task_118);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_592B(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int8_t local_F9;
  int8_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  float local_90[8][3];
  Vector3 local_84;
  float local_FF24[8][3];
  int32_t local_FEC4[8][3];
  Vector3i local_FEB8;
  (void)((local_FF = 0) & 1);
  (void)((local_FE = 0) & 1);
  (void)((local_FD = 0) & 1);
  (void)((local_FC = 0) & 1);
  (void)((local_FB = 0) & 1);
  (void)((local_FA = 0) & 1);
  local_F9 = 0;
  local_F8 = 0;
  local_F4 = 0;
  while (local_F4 < 8) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, *(&data_2B530 + local_F4))) {
      Script_EVT_GetPosition(*(&data_2B530 + local_F4), -1, (Vector3 *)local_90[local_F4], ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(*(&data_2B530 + local_F4), (Vector3i *)local_FEC4[local_F4], 0x40000000);
      local_FF24[local_F4][0] = local_90[local_F4][0] + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FEC4[local_F4][1]) * 0.3f;
      local_FF24[local_F4][1] = local_90[local_F4][1];
      local_FF24[local_F4][2] = local_90[local_F4][2] + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FEC4[local_F4][1]) * 0.3f;
    }
    local_F4 = local_F4 + 1;
  }
  for (;;) {
    local_F9 = -1;
    (void)((local_FB = 0) & 2);
    Script_EVT_GetPosition(arg0, -1, &local_84, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_FEB8, 0x42000000);
    local_84.x = (&local_84)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FEB8)->y) * 0.3f;
    (&local_84)->z = (&local_84)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FEB8)->y) * 0.3f;
    local_F0 = 0;
    while (local_F0 < 8) {
      if (Script_CheckTargetWithinAngleRange((const Vector3 *)local_FF24[local_F0], 1.4f, local_FEC4[local_F0][1] + 0x8000, 12743, &local_84) != 0) {
        if (Script_CheckTargetWithinAngleRange(&local_84, 1.4f, (&local_FEB8)->y, 5461, (const Vector3 *)local_FF24[local_F0]) != 0) {
          local_F9 = local_F0;
          break;
        }
      }
      local_F0 = local_F0 + 1;
    }
    if (Script_shenGetStats(STATS_CASH) < 100) {
      local_F8 = 1;
    } else {
      local_F8 = Script_ryoControl(PCCTL_GetTatehiza);
    }
    if (local_FA == -1) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
        (void)((local_FA = 0) & 0x80);
      }
    }
    if ((local_F9 != -1) & (local_FA == 0)) {
      if (Script_GetControllerInput(1) & 0x400) {
        if (local_FB == 0) {
          (void)((local_FB = -1) & 0x80);
        }
        if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100)) {
          (void)((local_FA = -1) & 0x80);
        }
      } else {
        (void)((local_FB = 0) & 0x80);
      }
      if (local_FA != -1) {
        if (local_FD == 0) {
          Script_uiUtils(0, 25, 0);
          (void)((local_FD = -1) & 0x80);
        }
      } else {
        if (local_FD == -1) {
          Script_uiUtils(0, 0, 0);
        }
        (void)((local_FD = 0) & 0x80);
      }
      if (local_FE == 0) {
        Script_sicnUtils(0, 1, 1);
        (void)((local_FE = -1) & 0x80);
      }
      if (local_F8 == 0) {
        if (-(local_FB == -1) & -(local_FC == 0) & -(local_FA != -1)) {
          (void)((local_FC = -1) & 0x80);
        }
        if (local_FC == 0) {
          if (local_FF == 0) {
            SCNF_RequestIconNotification(0x100, 0, 0, 1);
            (void)((local_FF = -1) & 0x80);
          }
        } else {
          if (local_FF == -1) {
            SCNF_RequestIconNotification(0, 0, 0, 2);
          }
          (void)((local_FF = 0) & 0x80);
        }
      } else {
        if (local_FF == -1) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
        }
        (void)((local_FF = 0) & 0x80);
        (void)((local_FC = 0) & 0x80);
      }
    } else {
      if (local_FE == -1) {
        Script_sicnUtils(1, 0, 0);
        (void)((local_FE = 0) & 0x80);
      }
      if (local_FD == -1) {
        Script_uiUtils(0, 0, 0);
        (void)((local_FD = 0) & 0x80);
      }
      if (local_FF == -1) {
        SCNF_RequestIconNotification(0, 0, 0, 2);
        (void)((local_FF = 0) & 0x80);
      }
    }
    if ((local_FB == -1) & (local_FC == -1)) {
      script_4A53(arg0, *(&data_2B530 + local_F9), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
      (void)((local_FB = 0) & 0x80);
      (void)((local_FC = 0) & 0x80);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_5D8F(int32_t arg0) {
  script_4B22();
  task_124 = Script_scenCreateTask(script_592B, 1, arg0);
  return;
}

static void script_5DAA(void) {
  Script_srchControl((void *)task_124);
  task_124 = 0;
  return;
}

static void script_5DBB(void) {
  (void)task_129;
  return;
}

static void script_5DC2(void) {
  Script_ExitCharaSelector();
  task_128 = 1;
  return;
}

static void script_5DCF(void) {
  Script_ambSoundUtils(1, 1, &data_2B820, 80.0f, 100.0f, 1.0f, 0, &data_2B8BC);
  Script_ambSoundUtils(2, 1, &data_2B82C, 30.0f, 150.0f, 1.0f, 2, data_2B944);
  Script_ambSoundUtils(3, 0, &data_2B838, 20.0f, 90.0f, 0.8f, 0, data_2B9CC);
  Script_ambSoundUtils(4, 0, &data_2B844, 3.0f, 10.0f, 0.8f, 0, data_2BA54);
  Script_ambSoundUtils(5, 1, &data_2B850, 15.0f, 120.0f, 0.8f, 0, &data_2BADC);
  Script_ambSoundUtils(6, 0, &data_2B85C, 1.5f, 3.0f, 0.8f, 0, data_2BB64);
  Script_ambSoundUtils(7, 1, &data_2B868, 30.0f, 70.0f, 0.9f, 2, &data_2BBEC);
  Script_ambSoundUtils(8, 1, &data_2B874, 30.0f, 150.0f, 1.0f, 0, data_2BC74);
  Script_ambSoundUtils(9, 0, &data_2B880, 3.0f, 60.0f, 0.8f, 0, data_2BCFC);
  Script_ambSoundUtils(10, 1, &data_2B88C, 28.0f, 70.0f, 1.0f, 0, &data_2BD84);
  Script_ambSoundUtils(11, 0, &data_2B898, 5.0f, 60.0f, 1.0f, 0, &data_2BE0C);
  Script_ambSoundUtils(12, 1, &data_2B8A4, 30.0f, 170.0f, 0.8f, 0, &data_2BE94);
  Script_ambSoundUtils(15, 1, &data_2B8B0, 100.0f, 170.0f, 1.0f, 3, &data_2BF1C);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_5FB0(void) {
  Script_ambSoundUtils(1, 1, &data_2BFA4, 80.0f, 100.0f, 1.0f, 0, &data_2C058);
  Script_ambSoundUtils(2, 0, &data_2BFB0, 2.5f, 25.0f, 1.0f, 0, data_2C0E0);
  Script_ambSoundUtils(3, 0, &data_2BFBC, 2.5f, 25.0f, 1.0f, 0, data_2C168);
  Script_ambSoundUtils(4, 0, &data_2BFC8, 3.0f, 10.0f, 1.0f, 0, data_2C1F0);
  Script_ambSoundUtils(5, 1, &data_2BFD4, 15.0f, 120.0f, 1.0f, 0, &data_2C278);
  Script_ambSoundUtils(6, 0, &data_2BFE0, 1.5f, 3.0f, 0.8f, 0, data_2C300);
  Script_ambSoundUtils(7, 1, &data_2BFEC, 30.0f, 70.0f, 1.0f, 2, &data_2C388);
  Script_ambSoundUtils(8, 0, &data_2BFF8, 2.5f, 25.0f, 1.0f, 0, data_2C410);
  Script_ambSoundUtils(9, 0, &data_2C004, 5.0f, 40.0f, 1.0f, 0, data_2C498);
  Script_ambSoundUtils(10, 1, &data_2C010, 28.0f, 70.0f, 1.0f, 0, &data_2C520);
  Script_ambSoundUtils(11, 0, &data_2C01C, 5.0f, 40.0f, 1.0f, 0, &data_2C5A8);
  Script_ambSoundUtils(12, 0, &data_2C028, 2.5f, 25.0f, 1.0f, 0, data_2C630);
  Script_ambSoundUtils(13, 0, &data_2C034, 2.5f, 25.0f, 1.0f, 0, data_2C6B8);
  Script_ambSoundUtils(14, 0, &data_2C040, 2.5f, 25.0f, 1.0f, 0, data_2C740);
  Script_ambSoundUtils(15, 0, &data_2C04C, 2.5f, 25.0f, 1.0f, 0, data_2C7C8);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_61D9(void) {
  Script_ambSoundUtils(1, 1, &data_2C850, 80.0f, 100.0f, 1.0f, 0, &data_2C904);
  Script_ambSoundUtils(2, 0, &data_2C85C, 3.5f, 30.0f, 1.0f, 0, data_2C98C);
  Script_ambSoundUtils(3, 0, &data_2C868, 20.0f, 90.0f, 1.0f, 0, data_2CA14);
  Script_ambSoundUtils(4, 0, &data_2C874, 3.0f, 10.0f, 1.0f, 0, data_2CA9C);
  Script_ambSoundUtils(5, 1, &data_2C880, 15.0f, 120.0f, 1.0f, 0, &data_2CB24);
  Script_ambSoundUtils(6, 0, &data_2C88C, 1.5f, 3.0f, 1.0f, 0, data_2CBAC);
  Script_ambSoundUtils(7, 1, &data_2C898, 30.0f, 70.0f, 1.0f, 2, &data_2CC34);
  Script_ambSoundUtils(8, 1, &data_2C8A4, 30.0f, 150.0f, 1.0f, 0, data_2CCBC);
  Script_ambSoundUtils(9, 0, &data_2C8B0, 3.0f, 60.0f, 1.0f, 0, data_2CD44);
  Script_ambSoundUtils(9, 0, &data_2C8B0, 3.0f, 60.0f, 1.0f, 0, data_2CD44);
  Script_ambSoundUtils(10, 1, &data_2C8BC, 28.0f, 70.0f, 1.0f, 0, &data_2CDCC);
  Script_ambSoundUtils(11, 0, &data_2C8C8, 5.0f, 60.0f, 1.0f, 0, &data_2CE54);
  Script_ambSoundUtils(12, 0, &data_2C8D4, 3.5f, 30.0f, 1.0f, 0, data_2CEDC);
  Script_ambSoundUtils(13, 0, &data_2C8E0, 3.5f, 30.0f, 1.0f, 0, data_2CF64);
  Script_ambSoundUtils(14, 0, &data_2C8EC, 10.0f, 30.0f, 1.0f, 0, data_2CFEC);
  Script_ambSoundUtils(15, 1, &data_2C8F8, 100.0f, 170.0f, 1.0f, 3, &data_2D074);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_6426(void) {
  Script_ambSoundUtils(1, 1, &data_2D0FC, 80.0f, 100.0f, 1.0f, 0, &data_2D1B0);
  Script_ambSoundUtils(2, 1, &data_2D108, 30.0f, 150.0f, 1.0f, 2, data_2D238);
  Script_ambSoundUtils(3, 0, &data_2D114, 20.0f, 90.0f, 1.0f, 0, data_2D2C0);
  Script_ambSoundUtils(4, 0, &data_2D120, 3.0f, 10.0f, 1.0f, 0, data_2D348);
  Script_ambSoundUtils(5, 1, &data_2D12C, 15.0f, 120.0f, 1.0f, 0, &data_2D3D0);
  Script_ambSoundUtils(6, 0, &data_2D138, 1.5f, 3.0f, 1.0f, 0, data_2D458);
  Script_ambSoundUtils(7, 1, &data_2D144, 30.0f, 70.0f, 1.0f, 2, &data_2D4E0);
  Script_ambSoundUtils(8, 1, &data_2D150, 30.0f, 150.0f, 1.0f, 0, data_2D568);
  Script_ambSoundUtils(9, 0, &data_2D15C, 3.0f, 60.0f, 1.0f, 0, data_2D5F0);
  Script_ambSoundUtils(10, 1, &data_2D168, 28.0f, 70.0f, 1.0f, 0, &data_2D678);
  Script_ambSoundUtils(11, 0, &data_2D174, 5.0f, 60.0f, 1.0f, 0, &data_2D700);
  Script_ambSoundUtils(12, 0, &data_2D180, 3.5f, 30.0f, 1.0f, 0, data_2D788);
  Script_ambSoundUtils(13, 0, &data_2D18C, 3.5f, 30.0f, 1.0f, 0, data_2D810);
  Script_ambSoundUtils(14, 0, &data_2D198, 10.0f, 30.0f, 1.0f, 0, data_2D898);
  Script_ambSoundUtils(15, 1, &data_2D1A4, 100.0f, 170.0f, 1.0f, 3, &data_2D920);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_664F(int32_t arg0, int32_t arg1) {
  if (arg0 == 1) {
    Script_Ambient1SoundUtils(2, arg1);
  } else {
    Script_Ambient1SoundUtils(3, arg1);
  }
  return;
}

static void script_6670(void) {
  if (task_C8.hours < 6) {
    (void)((task_10F = -1) & 0x40);
    script_664F(1, 4);
  }
  return;
}

static void script_6696(void) {
  if (task_10F == 0) {
    script_6670();
  }
  return;
}

static void script_66A8(void) {
  if (task_112 == 0) {
    script_664F(1, 9);
  } else {
    script_664F(0, 9);
  }
  return;
}

static void script_66CE(int32_t arg0) {
  int8_t local_FF;
  local_FF = 16;
  while (local_FF) {
    script_664F(arg0, local_FF);
    local_FF = local_FF - 1;
  }
  if (arg0 == 0) {
    script_66A8();
  }
  return;
}

static void script_6702(void) {
  if (-(task_C8.month == 12) & -(task_C8.day >= 15) & -(task_C8.day <= 25)) {
    if (task_C8.day >= 24) {
      script_5FB0();
    } else if (task_C8.day >= 19) {
      script_61D9();
    } else {
      script_6426();
    }
  } else if ((task_C8.month == 1) & (task_C8.day <= 5)) {
    script_6426();
  } else {
    script_5DCF();
  }
  script_66A8();
  script_6696();
  return;
}

static void script_678D(int32_t arg0) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_set_val_to_1(3, 0);
  {
    int32_t local_FC;
    local_FC = arg0;
    switch (local_FC) {
      case 2:
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
        Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2DB70);
        Script_charaSetRefboardFlagTo(SHEN_NAME('TBK1'), 1);
        Script_SetCharaRefBoardPos(SHEN_NAME('TBK1'), &data_2DB70);
        Script_charaSetRefboardFlagTo(SHEN_NAME('TBK2'), 1);
        Script_SetCharaRefBoardPos(SHEN_NAME('TBK2'), &data_2DB70);
        Script_executeAicaCommand(10408, 0, 0);
        Script_executeAicaCommand(1184, 2, 112);
        Script_SetCameraFlags(0);
        Script_mapmSetCamFlags(0, 0);
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_scenCreateTask(script_6E9E, 0);
        Script_shenSeqUtils(arg0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_EVT_SetPosition(SHEN_NAME('TBK2'), 0x78000000, &data_2DB80);
        Script_SetCameraFlags(1);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
        Script_charaSetRefboardFlagTo(SHEN_NAME('TBK1'), 0);
        Script_charaSetRefboardFlagTo(SHEN_NAME('TBK2'), 0);
        break;
      case 3:
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_SetFogColour(&data_2DB8C);
        Script_mapmSetCamFlags(0, 0);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 170.0f);
        Script_scenCreateTask(script_7033, 0);
        Script_shenSeqUtils(arg0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 4:
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_SetFogColour(&data_2DB8C);
        Script_mapmSetCamFlags(0, 0);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 183.0f);
        Script_scenCreateTask(script_7068, 0);
        Script_shenSeqUtils(arg0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 5:
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 96);
        Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_SetFogColour(&data_2DB8C);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 183.0f);
        Script_scenCreateTask(script_70F9, 0);
        Script_shenSeqUtils(arg0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 6:
        Script_SetFogColour(&data_2DB8C);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 183.0f);
        Script_scenCreateTask(script_71A6, 0);
        Script_shenSeqUtils(arg0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 7:
        Script_SetFogColour(&data_2DB8C);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 183.0f);
        Script_scenCreateTask(script_722D, 0);
        Script_shenSeqUtils(arg0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_executeAicaCommand(2720, 2, 15);
        Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
  }
  Script_mapmSetCamFlags(0, 1);
  Script_set_val_to_1(2, 0);
  {
    int32_t local_FC;
    local_FC = arg0;
    if ((local_FC == 5) | (local_FC == 6) | (local_FC == 7)) {
      script_6DE5();
    } else {
      script_6DD0();
    }
  }
  return;
}

static void script_6B4C(void) {
  script_6D53(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_clmdSetFlags(CHARA_AKIR, 2, 0);
  return;
}

static void script_6BA0(void) {
  script_6CE7(SHEN_NAME('LIG1'));
  script_6CE7(SHEN_NAME('LIG2'));
  script_6CE7(SHEN_NAME('LIG3'));
  script_6CE7(SHEN_NAME('LIG4'));
  script_6CE7(SHEN_NAME('LIG5'));
  script_6CE7(SHEN_NAME('LIG6'));
  script_6CE7(SHEN_NAME('LIG7'));
  Script_EVT_SetPosition(SHEN_NAME('LIG3'), POS_XYZ, &data_2DB9C);
  Script_EVT_SetRotation(SHEN_NAME('LIG3'), 0x38000000, &data_2DBE4, 0);
  Script_EVT_SetPosition(SHEN_NAME('LIG4'), POS_XYZ, &data_2DBA8);
  Script_EVT_SetRotation(SHEN_NAME('LIG4'), 0x38000000, &data_2DBF0, 0);
  Script_EVT_SetPosition(SHEN_NAME('LIG5'), POS_XYZ, &data_2DBB4);
  Script_EVT_SetRotation(SHEN_NAME('LIG5'), 0x38000000, &data_2DBFC, 0);
  Script_EVT_SetPosition(SHEN_NAME('LIG6'), POS_XYZ, &data_2DBC0);
  Script_EVT_SetRotation(SHEN_NAME('LIG6'), 0x38000000, &data_2DC08, 0);
  Script_EVT_SetPosition(SHEN_NAME('LIG7'), POS_XYZ, &data_2DBCC);
  Script_EVT_SetRotation(SHEN_NAME('LIG7'), 0x38000000, &data_2DC14, 0);
  Script_EVT_SetPosition(SHEN_NAME('LIG8'), POS_XYZ, &data_2DBD8);
  Script_EVT_SetRotation(SHEN_NAME('LIG8'), 0x38000000, &data_2DC20, 0);
  return;
}

static void script_6CE7(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  return;
}

static void script_6D02(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 10, 16);
  Script_SetCharaFlags(arg0, 4);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_6D53(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 8, 16);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaFlags(arg0, 4);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_6DD0(void) {
  script_6E31(CHARA_AKIR);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_6DE5(void) {
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_6DEE(void) {
  return;
}

static void script_6DF1(int32_t arg0) {
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_CharHandlers(arg0, -1, 0, 0);
  return;
}

static void script_6E31(int32_t arg0) {
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetMotnFlag(arg0, 0);
  return;
}

static int32_t script_6E9E(void) {
  int16_t local_FE;
  uint8_t unused_E4[26];
  local_FE = 0;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_EVT_SetPosition(SHEN_NAME('TBK1'), 0x78000000, &data_2DC68);
          Script_EVT_SetRotation(SHEN_NAME('TBK1'), 0x78000000, &data_2DC74, 0);
          Script_EVT_SetPosition(SHEN_NAME('TBK2'), 0x78000000, &data_2DC44);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2DA8C, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2DA8C, 0);
          Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_2DC98);
          break;
        case 120:
          Script_EVT_SetPosition(SHEN_NAME('TBK1'), 0x78000000, &data_2DC44);
          Script_EVT_SetPosition(SHEN_NAME('TBK2'), 0x78000000, &data_2DC80);
          Script_EVT_SetRotation(SHEN_NAME('TBK2'), 0x78000000, &data_2DC8C, 0);
          break;
        case 300:
          Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_2DCA4);
          break;
        case 433:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2D9A8, 10);
          break;
        case 860:
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
          break;
      }
      local_FE = local_FE + 1;
    } else {
      Script_SetMotnFlag(CHARA_AKIR, 0);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_7033(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      local_FE = local_FE + 1;
    } else {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_7068(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_charaDisplay(SHEN_NAME('LIG1'), CDM_Show);
          Script_charaDisplay(SHEN_NAME('LIG2'), CDM_Show);
          Script_charaDisplay(SHEN_NAME('LIG3'), CDM_Show);
          break;
        case 80:
          Script_charaDisplay(SHEN_NAME('LIG3'), CDM_Hide);
          break;
        case 150:
          Script_charaDisplay(SHEN_NAME('LIG2'), CDM_Hide);
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

static int32_t script_70F9(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_charaDisplay(SHEN_NAME('LIG4'), CDM_Show);
          Script_charaDisplay(SHEN_NAME('LIG7'), CDM_Show);
          Script_charaSetRenderOpacity(SHEN_NAME('LIG7'), 1.0f);
          Script_scenCreateTask(script_7572, 4, 30, 18, 0x3F19999A, 0x3F800000);
          Script_scenCreateTask(script_73E6, 3, SHEN_NAME('LIG7'), 60, 0x3F333333);
          break;
        case 45:
          Script_charaDisplay(SHEN_NAME('LIG4'), CDM_Hide);
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

static int32_t script_71A6(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if (local_FE == 0) {
        Script_charaSetRenderOpacity(SHEN_NAME('LIG5'), 0.0f);
        Script_LightUtils(4, 5, 2.3f);
        Script_scenCreateTask(script_737D, 3, SHEN_NAME('LIG6'), 50, 0x3F36DC5D);
        Script_scenCreateTask(script_7572, 4, 50, 3, 0x3F7C56D6, 0x400CCCCD);
      }
      local_FE = local_FE + 1;
    } else {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_722D(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_lightSetSlotEnabled(9, 0);
          Script_charaDisplay(SHEN_NAME('LIG3'), CDM_Show);
          Script_scenCreateTask(script_737D, 3, SHEN_NAME('LIG8'), 50, 0x3F200000);
          Script_scenCreateTask(script_7572, 4, 50, 10, 0x3FF00000, 0x40400000);
          break;
        case 60:
          Script_lightSetSlotEnabled(9, 1);
          Script_charaDisplay(SHEN_NAME('LIG3'), CDM_Hide);
          break;
        case 62:
          Script_lightSetSlotEnabled(9, 0);
          Script_charaDisplay(SHEN_NAME('LIG3'), CDM_Show);
          break;
        case 65:
          Script_lightSetSlotEnabled(9, 1);
          Script_charaDisplay(SHEN_NAME('LIG3'), CDM_Hide);
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

static void script_730E(int32_t arg0, int32_t arg1) {
  int16_t local_FE;
  float local_F8;
  float local_F4;
  local_FE = 0;
  local_F8 = 1.0f / (float)arg1;
  local_F4 = 1.0f;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if (local_FE <= arg1) {
        Script_charaSetRenderOpacity(arg0, local_F4);
        local_F4 = local_F4 - local_F8;
      } else {
        Script_srchControl(0);
      }
    } else {
      Script_srchControl(0);
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_737D(int32_t arg0, int32_t arg1, float arg2) {
  int16_t local_FE;
  float local_F8;
  float local_F4;
  local_FE = 0;
  local_F4 = arg2;
  local_F8 = local_F4 / (float)arg1;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if (local_FE <= arg1) {
        Script_charaSetRenderOpacity(arg0, local_F4);
        local_F4 = local_F4 - local_F8;
      } else {
        Script_srchControl(0);
      }
    } else {
      Script_srchControl(0);
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_73E6(int32_t arg0, int32_t arg1, float arg2) {
  int16_t local_FE;
  float local_F8;
  float local_F4;
  float local_F0;
  local_FE = 0;
  local_F0 = 1.0f;
  local_F8 = arg2;
  local_F4 = local_F8 / (float)arg1;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if (local_FE <= arg1) {
        Script_charaSetRenderOpacity(arg0, local_F0);
        local_F0 = local_F0 - local_F4;
      } else {
        Script_srchControl(0);
      }
    } else {
      Script_srchControl(0);
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_7458(int32_t arg0) {
  float local_FC;
  float local_F8;
  int16_t local_F6;
  local_F8 = 1.8f / (float)arg0;
  local_FC = 0.5f;
  local_F6 = 0;
  while (1) {
    if (local_F6 <= arg0) {
      Script_LightUtils(4, 5, local_FC);
      local_FC = local_FC + local_F8;
    } else {
      Script_srchControl(0);
    }
    local_F6 = local_F6 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_74B6(int32_t arg0) {
  float local_FC;
  float local_F8;
  int16_t local_F6;
  local_F8 = 2.2f / (float)arg0;
  local_FC = 0.5f;
  local_F6 = 0;
  while (1) {
    if (local_F6 <= arg0) {
      Script_LightUtils(3, 5, local_FC);
      local_FC = local_FC + local_F8;
    } else {
      Script_srchControl(0);
    }
    local_F6 = local_F6 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_7514(int32_t arg0) {
  float local_FC;
  float local_F8;
  int16_t local_F6;
  local_F8 = 1.75f / (float)arg0;
  local_FC = 0.25f;
  local_F6 = 0;
  while (1) {
    if (local_F6 <= arg0) {
      Script_LightUtils(18, 5, local_FC);
      local_FC = local_FC + local_F8;
    } else {
      Script_srchControl(0);
    }
    local_F6 = local_F6 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_7572(int32_t arg0, int32_t arg1, float arg2, float arg3) {
  float local_FC;
  float local_F8;
  float local_F4;
  float local_F0;
  int16_t local_EE;
  local_F4 = arg2;
  local_F0 = arg3;
  local_F8 = (local_F0 - local_F4) / (float)arg0;
  local_FC = arg2;
  local_EE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if (local_EE <= arg0) {
        Script_LightUtils(arg1, 5, local_FC);
        local_FC = local_FC + local_F8;
      } else {
        Script_srchControl(0);
      }
    } else {
      Script_srchControl(0);
    }
    local_EE = local_EE + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_75EB(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_7613(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_7667(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_76B7(int32_t arg0) {
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

static void script_775E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_76B7(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_7781(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_77B4(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_7781(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_77D7(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_7781(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_7836(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

__attribute__((scn_string_bindings(0, data_2DCC0)))
static void script_784B(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_2DCD4)))
static void script_785A(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_7869(void) {
  return;
}

static void script_786C(void) {
  return;
}

static void script_786F(void) {
  return;
}

static void script_7872(void) {
  return;
}

static void script_7875(void) {
  (void)((task_140 = 0) & 0x20);
  return;
}

static int32_t script_7882(void) {
  return task_134 = Script_freeTalkGetCurrentChara();
}

static void script_788E(void) {
  *(&task_134 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_789D(void) {
  return *(&task_134 + 2) = Script_freeTalkGetAndResetState();
}

static int32_t script_78AC(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_78CA(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_78E9(int32_t arg0, int32_t arg1) {
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

static void script_7961(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_2A4 != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_18C[arg2], arg3, arg4);
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

static int32_t script_7F13(int32_t arg0) {
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

static void script_7FE4(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_10D07(arg0, arg2, 0);
  return;
}

static void script_7FF4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_1049C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_B780(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_10D07(arg0, arg2, 0);
  return;
}

static void script_80F8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_1049C(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_18C[arg2]);
    script_B780(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_10D07(arg0, arg2, 0);
  return;
}

static void script_829A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_1049C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 4) == 0) {
      script_9C8D(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_2A4) {
        script_D060(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_10D07(arg0, arg2, 2);
  return;
}

static void script_83FB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_1049C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 4) == 0) {
      script_9C8D(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_2A4) {
        script_D060(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_10D07(arg0, arg2, 2);
  return;
}

static void script_855C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_1049C(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_18C[arg2]);
      script_9C8D(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_2A4) {
        float local_F4;
        float local_F0;
        local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_1049C(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_18C[arg2]);
        script_D060(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_10D07(arg0, arg2, 2);
  return;
}

static void script_87FA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 6);
    script_1049C(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_10D07(arg0, arg2, 0);
  return;
}

static void script_88C8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_1049C(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_AC0C(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_10D07(arg0, arg2, 1);
  return;
}

static void script_892F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_1049C(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_AC0C(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_10D07(arg0, arg2, 1);
  return;
}

static void script_8996(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_1049C(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_AC0C(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_10D07(arg0, arg2, 1);
  return;
}

static void script_89DD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_1049C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_1049C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_B0C6(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_10D07(arg0, arg2, 2);
  return;
}

static void script_8B13(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  if (arg0 == task_2A4) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_FF7F = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_1049C(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_10B65(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_2AC)->x;
      (&local_9C)->y = task_2AC.y;
      (&local_9C)->z = task_2AC.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, CHARA_PART_Get);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, CHARA_PART_Get);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_2A4) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_2AC, *(&task_2B8 + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_2AC, *(&task_2B8 + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_2A4) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_2A4) {
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
      script_10B65(arg0, 1);
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_2A4) {
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
    script_10D07(arg0, arg2, 0);
  } else {
    script_80F8(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_93D4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_1049C(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_1049C(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_D781(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_10D07(arg0, arg2, 0);
  return;
}

static int32_t script_94F2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_F4 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 6);
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
        local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_1049C(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L0096AE;
            }
          }
          L0096AE:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_2B8 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_2AC)->x, (&local_E4)->z - task_2AC.z);
        script_DF0B(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadFloatBits, Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_2ECF8, SCND_ReadFloatBits, Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_1049C(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_18C[arg2]);
      script_DFE3(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadFloatBits, Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_2ECF8, SCND_ReadFloatBits, Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_1049C(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_18C[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_E45A(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadFloatBits, Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_2ECF8, SCND_ReadFloatBits, Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_1049C(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_18C[arg2]);
          script_DFE3(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L009BA9;
      }
    }
  }
  L009BA9:;
  script_10D07(arg0, arg2, 0);
  end:;
}

static void script_9BB7(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_7F13(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2F816)))
static void script_9C8D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_2A4) {
        local_FF78 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_7961(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_10B65(arg0, 0);
    local_C0.x = (&task_2AC)->x;
    (&local_C0)->y = task_2AC.y;
    (&local_C0)->z = task_2AC.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_2AC, *(&task_2B8 + 1), local_FF78);
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
    if (task_2A4 == arg0) {
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
          script_9BB7(arg0, task_1DC[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_2A4) {
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
                goto L00A215;
              }
            }
            L00A215:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L00A253;
                }
              }
              L00A253:;
              goto L00A256;
            }
          }
        }
      }
      L00A256:;
      if (task_2A4 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_2A4) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_2A4 != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_2A4 == arg0) {
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
                goto L00A422;
              }
            }
            L00A422:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L00A460;
                }
              }
              L00A460:;
              goto L00A463;
            }
          }
          L00A463:;
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
              goto L00A4BC;
            }
          }
        }
      }
      L00A4BC:;
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
      if (arg0 == task_2A4) {
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
  script_10B65(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_18C[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_2A4 == arg0) {
        local_80 = (Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_2A4 == arg0) {
        local_80 = (Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_144, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_144, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_144, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_2A4 == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_144, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_144, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_144, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_144, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L00AB9F;
      }
    }
  }
  L00AB9F:;
  return;
}

static void script_ABA0(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_2A4 == arg0) {
    local_FC = task_18C[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_AC0C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_2AC)->x;
  (&local_AC)->y = task_2AC.y;
  (&local_AC)->z = task_2AC.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
    local_80 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_2A4) {
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
        task_1B4[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
        local_80 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_2A4) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_1B4[arg2] = 0;
      }
    }
    local_88 = task_18C[arg2];
    local_88 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_18C[arg2];
      local_88 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_1B4[arg2] = 0;
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
          goto L00B076;
        }
      }
    }
    L00B076:;
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

__attribute__((scn_string_bindings(0, data_2F832)))
static void script_B0C6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_10B65(arg0, 0);
  local_CC.x = (&task_2AC)->x;
  (&local_CC)->y = task_2AC.y;
  (&local_CC)->z = task_2AC.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2AC, *(&task_2B8 + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_2A4) {
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
  script_10B65(arg0, 1);
  local_FC = task_18C[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_144, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_144, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_144, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2F84E)))
static void script_B780(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      if (arg0 == task_2A4) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_2A4) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_1DC[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_1DC[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_2A4) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_2A4) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_2A4) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_1DC[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_1DC[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L00C373;
      }
    }
  }
  L00C373:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 7);
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
  script_7961(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_10B65(arg0, 0);
  local_D0.x = (&task_2AC)->x;
  (&local_D0)->y = task_2AC.y;
  (&local_D0)->z = task_2AC.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2AC, *(&task_2B8 + 1), local_A4);
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
    if (arg0 == task_2A4) {
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
      if (arg0 == task_2A4) {
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
              script_9BB7(arg0, task_1DC[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_2A4) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_2A4) {
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
        local_FF60 = script_78E9((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L00C978;
          }
        }
        L00C978:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_78E9(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_78E9(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_2A4) {
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
          local_FF74 = (float)script_78AC(script_78E9(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_2A4) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_2A4) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_18C[arg2];
        if (arg0 == task_2A4) {
          if (task_2A4 == arg0) {
            local_A0 = (Script_ScenDataUtil(task_144, SCND_ReadU32, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_2A4) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L00CE24;
            }
          }
        }
      }
      L00CE24:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_2A4) {
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
      if (arg0 != task_2A4) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_10B65(arg0, 1);
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
  if (arg0 == task_2A4) {
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
      goto L00D05F;
    }
  }
  L00D05F:;
  return;
}

__attribute__((scn_string_bindings(0, data_2F86A)))
static void script_D060(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_10B65(arg0, 0);
    local_C4.x = (&task_2AC)->x;
    (&local_C4)->y = task_2AC.y;
    (&local_C4)->z = task_2AC.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_2AC, *(&task_2B8 + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
          local_84 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_2A4) {
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
                goto L00D384;
              }
            }
            L00D384:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L00D3C0;
                }
              }
              L00D3C0:;
              goto L00D3C3;
            }
          }
        }
      }
      L00D3C3:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L00D408;
          }
        }
      }
      L00D408:;
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
  script_10B65(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_18C[arg2];
    local_9C = Script_ScenDataUtil(task_144, SCND_ReadU32, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_144, SCND_ReadU32, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_144, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_144, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_144, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2F886)))
static void script_D781(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_2E2D8, SCND_ReadU32, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_2E2D8, SCND_ReadFloatBits, Script_ScenDataUtil(data_2DCE8, SCND_ReadU8, arg3 * 40 + task_1DC[arg2] * 4 + arg4) * 9 + 4);
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
  script_10B65(arg0, 0);
  local_B0.x = (&task_2AC)->x;
  (&local_B0)->y = task_2AC.y;
  (&local_B0)->z = task_2AC.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2AC, *(&task_2B8 + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_2A4) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_2A4) {
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
  script_10B65(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, CHARA_PART_Set);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, CHARA_PART_Set);
  return;
}

static void script_DF0B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2AC, *(&task_2B8 + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_2F8A2)))
static void script_DFE3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_D4 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadU32, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadU32, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadU32, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadU32, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L00E204;
    }
  }
  L00E204:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_2ECF8, SCND_ReadFloatBits, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_2ECF8, SCND_ReadFloatBits, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_2AC)->x;
  (&local_F4)->y = task_2AC.y;
  (&local_F4)->z = task_2AC.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2AC, *(&task_2B8 + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_2A4) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_2A4) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_2F8BE)))
static void script_E45A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  task_2D0 = 0;
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
        goto L00E766;
      }
    }
  }
  L00E766:;
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
  local_B8.x = (&task_2AC)->x;
  (&local_B8)->y = task_2AC.y;
  (&local_B8)->z = task_2AC.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2AC, *(&task_2B8 + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_2A4) {
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
      local_FF40 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_2A4) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_2A4) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    local_FF6C = 0;
    L00F231:;
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
      if (!(local_FF6C >= 20)) goto L00F231;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_2A4) {
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

static void script_F3ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    if (arg0 != task_2A4) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_7F13(arg5));
    }
  }
  return;
}

static void script_F837(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_144 = arg1;
  task_148 = arg2;
  task_14C = arg3;
  task_150 = arg4;
  task_154.mode = 0;
  task_154.id = 0;
  task_154.entry_idx = 0;
  task_2A4 = arg0;
  (void)((task_2C8 = -1) & 1);
  task_2C4 = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L00F8A1:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_22C[local_F8] = local_FC;
  task_164[local_F8] = -1;
  task_18C[local_F8] = -1;
  task_1B4[local_F8] = 0;
  task_1DC[local_F8] = 0;
  task_204[local_F8] = 0;
  task_254[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L00F8A1;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_144, SCND_ReadU32, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_144, SCND_WriteU32, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_154, task_164, task_18C, (shen_name *)task_1B4, task_1DC, arg0, arg5, data_2DCE8, data_2E2D8, (shen_name *)task_22C);
  Script_scenCreateTask(script_100F3, 0);
  return;
}

static void script_F9C3(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_144 = arg0;
  task_148 = arg1;
  task_14C = arg2;
  task_150 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_2C4 = 1;
  return;
}

static void script_F9F7(void) {
  int32_t local_FC;
  local_FC = 0;
  L00F9FF:;
  task_164[local_FC] = -1;
  task_18C[local_FC] = -1;
  task_1B4[local_FC] = 0;
  task_1DC[local_FC] = 0;
  task_204[local_FC] = 0;
  task_254[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00F9FF;
  return;
}

static void script_FA60(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_2A4 == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_2D8, 0);
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

static void script_FAFD(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_2A4 == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_2D8, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_FB37(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_144, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_FB9A(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_144, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_FBFD(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_144, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_FC34(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_144, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_FC6B(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_18C[local_F0] == arg0) {
      local_EC = task_22C[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[local_F0] * 13 + 1) & 0x20) {
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

static void script_FEAD(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_144, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_FFB5(arg0);
  script_FC6B(arg0);
  return;
}

static void script_FEF6(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_144, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_FFC3(arg0);
  script_FC6B(arg0);
  return;
}

static void script_FF3F(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_144, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_144, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_FFB5(int32_t arg0) {
  script_FF3F(arg0, 0);
  return;
}

static void script_FFC3(int32_t arg0) {
  script_FF3F(arg0, 1);
  return;
}

static void script_FFD1(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_144, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_144, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_10098(void) {
  script_FFD1(1);
  return;
}

static void script_100A4(void) {
  script_FFD1(0);
  return;
}

static void script_100B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_144, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_100F3(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_2C4 == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_164[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_154)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
        script_10EC3(1, local_FC, local_F4, local_F8);
        if (script_11077(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_10EC3(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_1128D(local_F4, local_F8);
    } else {
      task_2C4 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_10204(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_2A4, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_2A4, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_2F949, 1, data_2F962)))
static void script_1029C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_27C[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_27C[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_150, SCND_ReadU32, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_150, SCND_ReadU32, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_150, SCND_ReadU32, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_150, SCND_ReadU32, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_150, SCND_ReadU32, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_150, SCND_ReadU32, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_150, SCND_ReadU32, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_14C, SCND_ReadScriptOffset, Script_ScenDataUtil(task_150, SCND_ReadU32, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_27C[arg0] = -1;
    }
  } else {
    task_27C[arg0] = -1;
  }
  return;
}

static void script_1046B(int32_t arg0) {
  if (task_27C[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_27C[arg0], 1);
    task_27C[arg0] = -1;
  }
  return;
}

static int32_t script_1049C(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_2A4 == arg0) {
    local_F4.x = (&task_2D8)->x;
    (&local_F4)->y = task_2D8.y;
    (&local_F4)->z = task_2D8.z;
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
        if ((Script_ScenDataUtil(task_144, SCND_ReadU32, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_144, SCND_ReadU32, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_144, SCND_ReadU32, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_144, SCND_ReadU32, arg6 * 13 + 1) & 0x80)) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L010687;
        }
        L010687:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L01072A;
          }
        }
        L01072A:;
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
            goto L0108D6;
          }
        }
      }
    }
    L0108D6:;
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
          task_2AC.x = (&local_F4)->x;
          task_2AC.y = (&local_F4)->y;
          task_2AC.z = (&local_F4)->z;
          task_2B8 = (&local_C4)->x;
          *(&task_2B8 + 1) = (&local_C4)->y;
          *(&task_2B8 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_2AC.x = (&local_F4)->x;
          task_2AC.y = (&local_F4)->y;
          task_2AC.z = (&local_F4)->z;
          task_2B8 = (&local_C4)->x;
          *(&task_2B8 + 1) = (&local_C4)->y;
          *(&task_2B8 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_2AC, 0);
          task_2B8 = (&local_C4)->x;
          *(&task_2B8 + 1) = (&local_C4)->y;
          *(&task_2B8 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_10B65(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if (task_2A4 == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, CCOW_EnableMap);
      } else {
        Script_ccowUtils(arg0, CCOW_DisableMap);
      }
    }
  }
  return;
}

static void script_10B97(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_1049C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_2A4 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_144, SCND_ReadU32, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_144, SCND_ReadU32, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_144, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_144, SCND_ReadU32, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_144, SCND_ReadU32, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_144, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2F97B)))
static void script_10D07(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_2A4 == arg0) {
    task_154.mode = 0;
    task_154.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_2A4, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_2C8 & 0x40) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_18C[arg1];
        local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_144, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_18C[arg1];
      local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_144, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_164[arg1] = 1;
  task_254[arg1] = 0;
  task_204[arg1] = 0;
  task_1B4[arg1] = 0;
  if (task_27C[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_27C[arg1], 1);
    task_27C[arg1] = -1;
  }
  return;
}

static void script_10EC3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_10204(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_10204(arg2, arg3, local_FC);
          goto L010F87;
        }
      }
      L010F87:;
    } else {
      if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_10204(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_2A4, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L011022;
          }
        }
      }
    }
  }
  L011022:;
  if (task_254[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_2D4) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_2D4) & task_2D4) {
    Script_sicnUtils(2, 4, 0);
  }
  task_2D4 = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_2F98A)))
static int32_t script_11077(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg1] * 13 + 1) & 4) == 0)) {
      if (task_254[arg1] == 0) {
        task_164[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_1B4[arg1] != task_2A4) & task_2D4) {
        if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg1] * 13 + 1) & 2) == 0) {
          task_164[arg1] = 3;
          task_1B4[arg1] = task_2A4;
          task_1DC[arg1] = 0;
          task_154.mode = 1;
          task_154.entry_idx = task_18C[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_2A4, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_164[arg1] = 6;
              task_1B4[arg1] = task_2A4;
              task_1DC[arg1] = 0;
              task_154.mode = 0;
              task_154.entry_idx = task_18C[arg1];
              local_FC = 0;
            }
            goto L01122B;
          }
        }
        L01122B:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_2A4, -1, &task_2D8, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_154.id = task_2A4;
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

static void script_1128D(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L011295:;
  local_F8 = task_164[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L0112B5;
      L0112B5:;
      script_11357(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L0112D5;
      L0112D5:;
      script_113B8(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L0112F3;
      L0112F3:;
      script_1155D(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L011315;
      L011315:;
      script_115F5(local_FC, arg1);
      break;
    case 2:
      script_116D7(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L011295;
  return;
}

static void script_11357(int32_t arg0, int32_t arg1) {
  if (task_254[arg0] == 0) {
    task_254[arg0] = Script_scenCreateTask(script_94F2, 4, task_1B4[arg0], task_22C[arg0], arg0, arg1);
    task_204[arg0] = task_1B4[arg0];
    script_1029C(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2F999, 1, data_2F9AB)))
static void script_113B8(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_1B4[arg0])) {
    if (task_254[arg0] == 0) {
      if (task_1B4[arg0] == task_2A4) {
        task_154.mode = 1;
        task_154.entry_idx = task_18C[arg0];
        goto L01140D;
      }
      L01140D:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
      script_10B97(task_1B4[arg0], task_22C[arg0], task_18C[arg0], local_F8);
      task_254[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_2ED98, SCND_ReadScriptOffset, local_F8), 4, task_1B4[arg0], task_22C[arg0], arg0, local_F8);
      task_204[arg0] = task_1B4[arg0];
      if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_1029C(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_27C[arg0] = -1;
      }
    }
  } else {
    if (task_164[arg0] != 5) {
      task_164[arg0] = 1;
      task_254[arg0] = 0;
      task_204[arg0] = 0;
      task_1B4[arg0] = 0;
    }
  }
  return;
}

static void script_1155D(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_254[arg0] == 0) & (arg2 >= 0)) {
    task_1B4[arg0] = task_2A4;
    task_154.mode = 2;
    task_154.entry_idx = task_18C[arg0];
    task_254[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_2ED98, SCND_ReadScriptOffset, arg2), 4, task_1B4[arg0], task_22C[arg0], arg0, arg2);
    task_204[arg0] = task_1B4[arg0];
    script_1029C(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2F9BE, 1, data_2F9D0)))
static void script_115F5(int32_t arg0, int32_t arg1) {
  if ((task_254[arg0] == 0) & (arg1 >= 0)) {
    script_10B97(task_1B4[arg0], task_22C[arg0], task_18C[arg0], arg1);
    task_254[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_2ED98, SCND_ReadScriptOffset, arg1), 4, task_1B4[arg0], task_22C[arg0], arg0, arg1);
    task_204[arg0] = task_1B4[arg0];
    if ((Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_1029C(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_27C[arg0] = -1;
    }
  }
  return;
}

static void script_116D7(int32_t arg0) {
  int32_t local_FC;
  task_254[arg0] = 0;
  task_204[arg0] = 0;
  task_164[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[arg0], 2);
  if (local_FC != -1) {
    task_1B4[arg0] = 0;
  }
  return;
}

static void script_1172E(int32_t arg0, int32_t arg1) {
  task_2E8 = arg0;
  script_11D2C(arg1);
  task_2FC = 9999;
  task_300 = 9999;
  task_304 = 0;
  task_308 = 0;
  task_311 = 0;
  task_314 = 0;
  script_11A66();
  task_318 = 0;
  task_2EC = 0x52554B59;
  task_2F0 = 0x4C554B59;
  return;
}

static void script_1178D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  script_1172E(arg0, arg1);
  task_30C = Script_scenCreateTask(script_117B4, 4, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_117B4(float arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  float local_D8;
  int8_t local_D7;
  local_F4.x = arg0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = arg2;
  local_D8 = arg3 + 180.0f;
  local_D7 = 0;
  task_312 = 0;
  task_313 = 0;
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0x42000000);
    if (Script_CheckTargetWithinAngleRange(&local_F4, 2.0f, (int32_t)(local_D8 / 360.0f * (float)0x10000), 7281, &local_DC) & Script_CheckTargetWithinAngleRange(&local_DC, 2.0f, (&local_E8)->y, 7281, &local_F4)) {
      if (Script_IsPlayerAllowed(0) & -(task_311 == 0)) {
        if (local_D7 == 0) {
          Script_uiUtils(0, 9, 0);
          Script_sicnUtils(0, 5, 1);
          local_D7 = 1;
        }
        if (Script_GetControllerInput(1) == 0x400) {
          if ((local_D7 == 1) & (MMTK_ScriptMemoToggle(2, 0) == 0)) {
            Script_uiUtils(0, 0, 0);
            Script_sicnUtils(1, 0, 0);
            local_D7 = 0;
          }
          task_312 = 1;
          script_11E98();
          task_312 = 0;
        }
      } else {
        if ((local_D7 == 1) & (MMTK_ScriptMemoToggle(2, 0) == 0)) {
          Script_uiUtils(0, 0, 0);
          Script_sicnUtils(1, 0, 0);
          local_D7 = 0;
        }
      }
    } else {
      if ((local_D7 == 1) & (MMTK_ScriptMemoToggle(2, 0) == 0)) {
        Script_uiUtils(0, 0, 0);
        Script_sicnUtils(1, 0, 0);
        local_D7 = 0;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_11952(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    task_310 = 0;
    for (;;) {
      if ((Script_GetControllerInput(1) == 0x400) | (Script_GetControllerInput(1) == 0x200)) {
        task_310 = 1;
        Script_TELMUtils(TELM_CMD_RequestHangUp, task_2E8);
        break;
      }
      local_FC = local_FC + 1;
      if (local_FC == 300) {
        task_310 = 1;
        Script_TELMUtils(TELM_CMD_RequestHangUp, task_2E8);
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  if (task_308) {
    int32_t local_FC;
    local_FC = 0;
    for (;;) {
      if ((task_308 == 0) | (local_FC < -200)) {
        break;
      }
      local_FC = local_FC - 5;
      Script_SetSequenceAudioVolume(0, (void *)task_308, local_FC);
      SCNF_WaitFrames_0000(1);
    }
    if (task_308) {
      Script_StartOrFreeSEQ((void *)task_308);
    }
  }
  return Script_srchControl(0);
}

static void script_11A19(const char *arg0, int32_t arg1) {
  task_31C[arg1] = Script_TELMUtils(TELM_CMD_RegisterPhoneNumber, task_2E8, arg0);
  if (task_2F4 == arg1) {
    task_2FC = task_31C[arg1];
  }
  if (task_2F8 == arg1) {
    task_300 = task_31C[arg1];
  }
  return;
}

static void script_11A66(void) {
  script_11A19((const char *)&data_3205C, 0);
  script_11A19((const char *)&data_32060, 1);
  script_11A19((const char *)&data_32064, 2);
  script_11A19((const char *)&data_32068, 7);
  script_11A19("61564", 3);
  script_11A19("37742", 4);
  script_11A19("37749", 5);
  script_11A19("22550", 6);
  script_11A19("046861564", 8);
  script_11A19("046837742", 9);
  script_11A19("046837749", 10);
  script_11A19("046822550", 11);
  script_11A19("2?????", 12);
  script_11A19("5?????", 13);
  script_11A19("7?????", 14);
  script_11A19("8?????", 15);
  script_11A19("3?????", 16);
  script_11A19("4?????", 17);
  script_11A19("6?????", 18);
  script_11A19("9?????", 19);
  script_11A19("0?????????", 20);
  script_11A19("1?????", 21);
  return;
}

static int32_t script_11B9D(int32_t arg0) {
  if (arg0 > 0) {
    if ((arg0 == task_2FC) | (arg0 == task_300)) {
      Script_scenCreateTask(script_11952, 0);
      for (;;) {
        Script_executeAicaCommand(394667, 0, 0);
        SCNF_WaitFrames_0000(15);
        Script_executeAicaCommand(460203, 0, 0);
        SCNF_WaitFrames_0000(15);
        if (task_310) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      arg0 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_2E8);
    }
  }
  return arg0;
}

static void script_11C16(void) {
  if (task_311 == 0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 185, 1);
    }
    task_311 = script_11C78();
    if (task_311 == 0) {
      task_311 = task_311 + 1;
      if (task_312) {
        task_313 = 1;
      }
    }
  }
  return;
}

static int8_t script_11C78(void) {
  int32_t local_FC;
  int8_t local_FB;
  local_FC = Script_shenGetStats(STATS_CASH);
  local_FB = 0;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_StepUtils(GET_STEP_LOWER, 0) <= 5)) {
    local_FB = -1;
  }
  if (local_FC < 10) {
    local_FB = -15;
  }
  return local_FB;
}

__attribute__((scn_string_bindings(0, data_320F6, 3, data_32110, 4, data_3211A)))
static void script_11CC3(int32_t arg0) {
  int32_t local_FC;
  Script_LoadVoiceFile(1, "E1066");
  local_FC = arg0;
  if (local_FC == -1) {
    Script_ShowUIMessageFromID("E1066A012");
  } else if (local_FC == -10) {
    Script_ShowUIMessageFromID("E1066A013");
  } else {
    Script_ShowUIMessageFromID("E1066A006");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadVoiceFile(0, "E1066");
  return;
}

static void script_11D2C(int32_t arg0) {
  int32_t local_FC;
  task_2F4 = arg0;
  local_FC = arg0;
  if (local_FC == 4) {
    task_2F8 = 9;
  } else {
    task_2F8 = 9999;
  }
  return;
}

static void script_11D57(void) {
  Script_convSetCamInactiveFlag(0);
  if ((task_2F4 == 9998) | (task_2F4 == 9997)) {
    SCNF_SetCashValue(10, 1);
  }
  return;
}

static int32_t script_11D7E(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours <= 4) {
    arg0 = -1;
  } else {
    if (((&local_F4)->hours >= 23) & ((&local_F4)->minutes >= 30)) {
      arg0 = -1;
      goto L011DBC;
    }
  }
  L011DBC:;
  return arg0;
}

static int8_t script_11DC0(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  local_FF = 0;
  local_FE = 1;
  for (;;) {
    Script_executeAicaCommand(263595, 0, 0);
    {
      int8_t local_FD;
      local_FD = 0;
      for (;;) {
        if (Script_GetControllerInput(1) == 0x200) {
          local_FE = 0;
          break;
        }
        local_FD = local_FD + 1;
        if (local_FD > 45) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_executeAicaCommand(329131, 0, 0);
    if (local_FE == 0) {
      break;
    }
    {
      int8_t local_FD;
      local_FD = 0;
      for (;;) {
        if (Script_GetControllerInput(1) == 0x200) {
          local_FE = 0;
          break;
        }
        local_FD = local_FD + 1;
        if (local_FD > 45) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    if (local_FE == 0) {
      break;
    }
    local_FF = local_FF + 1;
    if (local_FF == arg0) {
      break;
    }
  }
  return local_FE;
}

static void script_11E98(void) {
  script_11C16();
  script_11EA5();
  return;
}

static void script_11EA5(void) {
  int32_t unused_F4[3];
  Vector4 local_E4;
  int32_t local_E0;
  int32_t local_DC;
  float local_D8;
  int32_t unused_D4;
  int8_t local_D3;
  uint8_t unused_D0[3];
  if (task_311 == 1) {
    Script_scenToggleActiveFor(CHARA_AKIR, 0);
    Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
    Script_scenToggleVisibleFor(task_2E8, 1);
    if (0) {
      Script_charaDisplay(task_2F0, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_2F0, CDM_Show);
        goto L011F29;
      }
    }
    L011F29:;
    Script_scenToggleVisibleFor(task_2F0, 1);
    if (0) {
      Script_charaDisplay(task_2EC, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_2EC, CDM_Show);
        goto L011F57;
      }
    }
    L011F57:;
    Script_scenToggleVisibleFor(task_2EC, 1);
    Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, task_2F0, task_2EC);
    local_D8 = Script_charaSetClipAngle(task_2E8, -1.0f);
    Script_charaSetClipAngle(task_2E8, 1.0f);
    local_E4.x = 0.8f;
    (&local_E4)->y = 0.8f;
    (&local_E4)->z = 0.8f;
    (&local_E4)->w = 0.8f;
    Script_charaSetRefboardFlagTo(task_2EC, 1);
    Script_charaSetRefboardFlagTo(task_2F0, 1);
    Script_SetCharaRefBoardPos(task_2EC, &local_E4);
    Script_SetCharaRefBoardPos(task_2F0, &local_E4);
    SCNF_WaitFrames_0000(1);
    script_1D854();
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
    script_12EC1();
    Script_charaDisplay(CHARA_AKIR, CDM_Show);
    script_1237A();
    local_E0 = 0;
    local_DC = 0;
    task_37A = 0;
    for (;;) {
      if (MMTK_ScriptMemoToggle(2, 0) == 0) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          task_37A = 1;
          Script_sicnUtils(1, 0, 0);
          Script_uiUtils(0, 0, 0);
          break;
        } else if (Script_GetControllerInput(1) == 0x400) {
          local_D3 = script_11C78();
          if (local_D3) {
            script_11CC3(local_D3);
            Script_sicnUtils(1, 0, 0);
            Script_uiUtils(0, 0, 0);
            break;
          } else {
            Script_sicnUtils(1, 0, 0);
            script_1237A();
            local_E0 = 0;
            local_DC = 0;
          }
        } else if ((Script_GetControllerInput(1) == 0x200) | (Script_GetControllerInput(1) == 0x2000) | (local_E0 > 1800)) {
          Script_sicnUtils(1, 0, 0);
          Script_uiUtils(0, 0, 0);
          break;
        } else {
          if (Script_GetControllerInput(1) == 4) {
            MMTK_ScriptMemoToggle(0, 0);
            goto L01211B;
          }
        }
        L01211B:;
        local_E0 = local_E0 + 1;
      } else {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          MMTK_ScriptMemoToggle(1, 0);
          goto L01213F;
        }
      }
      L01213F:;
      SCNF_WaitFrames_0000(1);
    }
    if ((task_2F4 == 9998) | (task_2F4 == 4)) {
      script_1CF16();
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      Script_convSetCamInactiveFlag(0);
      Script_SetCPAMFlags(CHARA_AKIR, 1);
      Script_charaSetRefboardFlagTo(task_2EC, 0);
      Script_charaSetRefboardFlagTo(task_2F0, 0);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_SetGameStateTo(AUTH_None);
      Script_SetCameraFieldOfView(10922);
      Script_scenToggleActiveFor(CHARA_AKIR, 1);
    }
    script_1D8C7();
    Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
    Script_ryoControl(RYO_RestoreControl);
    Script_charaSetRefboardFlagTo(task_2EC, 0);
    Script_charaSetRefboardFlagTo(task_2F0, 0);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
    Script_charaSetClipAngle(task_2E8, local_D8);
    Script_scenToggleVisibleFor(task_2E8, 0);
    if (-1) {
      Script_charaDisplay(task_2EC, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_2EC, CDM_Show);
        goto L01223A;
      }
    }
    L01223A:;
    Script_scenToggleVisibleFor(task_2EC, 0);
    if (-1) {
      Script_charaDisplay(task_2F0, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_2F0, CDM_Show);
        goto L012268;
      }
    }
    L012268:;
    Script_scenToggleVisibleFor(task_2F0, 0);
    task_311 = 0;
    task_313 = 0;
  } else {
    if ((task_311 == -1) | (task_311 == -10) | (task_311 == -15)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
      local_E4.x = 0.8f;
      (&local_E4)->y = 0.8f;
      (&local_E4)->z = 0.8f;
      (&local_E4)->w = 0.8f;
      Script_SetCharaRefBoardPos(CHARA_AKIR, &local_E4);
      script_1D854();
      script_11CC3(task_311);
      script_1D8C7();
      Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_ryoControl(RYO_RestoreControl);
      Script_convSetCamInactiveFlag(1);
      Script_SetGameStateTo(AUTH_None);
      task_311 = 0;
      task_313 = 0;
      goto L012379;
    }
  }
  L012379:;
  return;
}

__attribute__((scn_string_bindings(1, data_3212C, 7, data_32164, 8, data_3216A, 13, data_32198, 14, data_3219E, 15, data_321A4, 16, data_321AA, 18, data_321BA, 21, data_321D8, 23, data_321E8, 25, data_321FC, 28, data_32216)))
static void script_1237A(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int8_t local_EB;
  int8_t local_EA;
  int8_t local_E9;
  int8_t local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_E5;
  int8_t local_E4;
  Vector4 local_D4;
  local_D4.x = 0.8f;
  (&local_D4)->y = 0.8f;
  (&local_D4)->z = 0.8f;
  (&local_D4)->w = 0.8f;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_D4);
  local_EC = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  Script_TELMUtils(TELM_CMD_LoadHandMotionAndSetup, task_2E8);
  local_E4 = 0;
  task_37B = 0;
  for (;;) {
    task_378 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_2E8);
    task_378 = script_11B9D(task_378);
    Script_ReturnTimeDate(&local_F4);
    if (MMTK_ScriptMemoToggle(2, 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
      Script_sprintf("メモ帳消去\012");
      MMTK_ScriptMemoToggle(1, 0);
    }
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) & -(task_378 != -4) & -(task_378 != 0) & -(local_E4 == 0)) {
      local_E4 = 1;
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_2E8);
      task_378 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_2E8);
    }
    if ((task_378 == *task_31C) | (task_378 == task_31C[1])) {
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_2E8);
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_12F7F();
      if (task_378 == *task_31C) {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
        if (local_D0 == 0) {
          task_308 = Script_ShowUIMessageFromID("E1032A001");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
        } else if (local_D0 == 1) {
          task_308 = Script_ShowUIMessageFromID("E1032A002");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
        } else {
          task_308 = Script_ShowUIMessageFromID("E1032A003");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
        }
      } else {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
        if (local_D0 == 0) {
          task_308 = Script_ShowUIMessageFromID("E1032A004");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
        } else {
          task_308 = Script_ShowUIMessageFromID("E1032A005");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
        }
      }
      Script_LoadVoiceFile(0, "E1032");
      script_11D57();
    }
    if ((task_378 == task_31C[7]) | (task_378 == task_31C[2]) | (task_378 == task_31C[3]) | (task_378 == task_31C[8]) | (task_378 == task_31C[4]) | (task_378 == task_31C[9]) | (task_378 == task_31C[5]) | (task_378 == task_31C[10]) | (task_378 == task_31C[6]) | (task_378 == task_31C[11])) {
      local_EB = 2;
      local_EA = 1;
      if (task_378 == task_31C[7]) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_OPERATOR_ADDRESS_LOOKUP_REFUSED)) {
          int32_t local_D0;
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_2E8);
          SCNF_WaitFrames_0000(1);
          Script_LoadVoiceFile(1, "E1032");
          local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
          if (local_D0 == 0) {
            task_308 = Script_ShowUIMessageFromID("E1032A061");
            while (Script_convGetIsActive((const void *)task_308)) {
              SCNF_WaitFrames_0000(1);
            }
            task_308 = 0;
          } else if (local_D0 == 1) {
            task_308 = Script_ShowUIMessageFromID("E1032A062");
            while (Script_convGetIsActive((const void *)task_308)) {
              SCNF_WaitFrames_0000(1);
            }
            task_308 = 0;
          } else if (local_D0 == 2) {
            task_308 = Script_ShowUIMessageFromID("E1032A063");
            while (Script_convGetIsActive((const void *)task_308)) {
              SCNF_WaitFrames_0000(1);
            }
            task_308 = 0;
          } else {
            task_308 = Script_ShowUIMessageFromID("E1032A064");
            while (Script_convGetIsActive((const void *)task_308)) {
              SCNF_WaitFrames_0000(1);
            }
            task_308 = 0;
          }
          Script_LoadVoiceFile(0, "E1032");
          script_11D57();
          local_EA = 0;
        }
      }
      if (task_378 == task_31C[2]) {
        local_EB = 1;
      }
      if ((task_378 == task_31C[4]) | (task_378 == task_31C[9])) {
        local_E9 = script_11D7E(local_E9);
        if (local_E9 == -1) {
          task_378 = -10;
          local_EA = 0;
        }
      }
      if ((task_378 == task_31C[3]) | (task_378 == task_31C[8])) {
        local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0);
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) | (local_F0 >= 16)) {
          local_E6 = script_11D7E(local_E6);
          if (local_E6 == -1) {
            task_378 = -10;
            local_EA = 0;
          }
        } else {
          task_378 = -10;
          local_EA = 0;
        }
      }
      if ((task_378 == task_31C[5]) | (task_378 == task_31C[10])) {
        local_E8 = script_1582B();
        switch (local_E8) {
          case -1:
            task_378 = -10;
            local_EA = 0;
            break;
          case -2:
            task_378 = -11;
            local_EA = 0;
            break;
        }
      }
      if ((task_378 == task_31C[6]) | (task_378 == task_31C[11])) {
        local_E7 = script_16B52();
        local_E7 = script_11D7E(local_E7);
        if (local_E7 == -1) {
          task_378 = -10;
          local_EA = 0;
        }
      }
      if (local_EA) {
        if (script_11DC0(local_EB)) {
          Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
          Script_shenClearFreeState(0);
          Script_LoadVoiceFile(1, "E1032");
          script_12F7F();
          script_1CD7C();
          local_E5 = 0;
          if (task_378 == task_31C[7]) {
            script_12FA4();
          }
          if (task_378 == task_31C[2]) {
            script_136EB();
          }
          if ((task_378 == task_31C[5]) | (task_378 == task_31C[10])) {
            script_159E1(local_E8);
          }
          if ((task_378 == task_31C[6]) | (task_378 == task_31C[11])) {
            script_170A1(local_E7);
          }
          if ((task_378 == task_31C[3]) | (task_378 == task_31C[8])) {
            local_E5 = script_1BA27();
          }
          if ((task_378 == task_31C[4]) | (task_378 == task_31C[9])) {
            script_1AF5F();
          }
          script_1CF60();
          if (local_E5) {
            script_1BFAE();
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED, 1);
          }
          Script_LoadVoiceFile(0, "E1032");
          script_1CF02();
          Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
          Script_shenClearFreeState(1);
          task_378 = 0;
        } else {
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_2E8);
          script_11D57();
        }
      }
    }
    if (task_378 == -10) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_12F7F();
      if (script_11DC0(6)) {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
        if (local_D0 == 0) {
        } else if (local_D0 == 1) {
          task_308 = Script_ShowUIMessageFromID("E1032A031");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
        } else if (local_D0 == 2) {
          task_308 = Script_ShowUIMessageFromID("E1032A029");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
        } else if (local_D0 == 3) {
          task_308 = Script_ShowUIMessageFromID("E1032A032");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
        } else {
          task_308 = Script_ShowUIMessageFromID("E1032A033");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
        }
      }
      Script_LoadVoiceFile(0, "E1032");
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_2E8);
      script_11D57();
    }
    if (task_378 == -11) {
      int32_t local_D0;
      Script_scenCreateTask(script_11952, 0);
      script_12F7F();
      for (;;) {
        Script_executeAicaCommand(394667, 0, 0);
        SCNF_WaitFrames_0000(15);
        Script_executeAicaCommand(460203, 0, 0);
        SCNF_WaitFrames_0000(15);
        if (task_310) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_D0 == 0) {
      } else if (local_D0 == 1) {
        task_308 = Script_ShowUIMessageFromID("E1032A028");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_D0 == 2) {
        task_308 = Script_ShowUIMessageFromID("E1032A029");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032A030");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
      task_378 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_2E8);
      script_11D57();
    }
    if ((task_378 == task_31C[12]) | (task_378 == task_31C[13]) | (task_378 == task_31C[14]) | (task_378 == task_31C[15]) | (task_378 == task_31C[16]) | (task_378 == task_31C[17]) | (task_378 == task_31C[18]) | (task_378 == task_31C[19]) | (task_378 == task_31C[20]) | (task_378 == task_31C[21])) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_12F7F();
      Script_scenCreateTask(script_11952, 0);
      for (;;) {
        task_308 = Script_ShowUIMessageFromID("E1032G001");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        if (task_310) {
          break;
        }
        task_308 = Script_ShowUIMessageFromID("E1032G002");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        if (task_310) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "E1032");
      task_378 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_2E8);
      script_11D57();
    }
    if (task_378 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(2015, task_2E8, task_2E8);
  script_12EC1();
  Script_sicnUtils(0, 5, 1);
  Script_uiUtils(0, 9, 0);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_12EC1(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(task_2E8, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(task_2E8, &local_E8, 0);
  local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.75f;
  (&local_F4)->y = -0.3f;
  (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.75f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  (&local_DC)->y = (&local_E8)->y;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  Script_InitializeOsag(CHARA_AKIR);
  return;
}

static void script_12F7F(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_GetStrayCatSceneVisitCountSlot, 1);
  local_FC = local_FC + 1;
  Script_PlayerStatsUtils(PS_SetStrayCatSceneVisitCountSlot, 1, local_FC);
  return;
}

__attribute__((scn_string_bindings(0, data_3221C, 5, data_3224E, 6, data_32258, 7, data_32262, 8, data_3226C, 14, data_322A8, 16, data_322BC, 17, data_322C6, 18, data_322D0, 19, data_322DA, 21, data_322EE, 22, data_322F8, 23, data_32302, 24, data_3230C, 25, data_32316)))
static void script_12FA4(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0)) {
    int32_t local_F8;
    task_304 = 81;
    Script_scenCreateTask(script_1D128, 0);
    task_308 = Script_ShowUIMessageFromID("E1032I001");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032A052");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032I007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032I008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    local_FC = SCNF_RequestActionSelectorFromParams_0008(27, 28, 29, 30, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
    task_304 = 82;
    local_F8 = local_FC;
    switch (local_F8) {
      case 27:
        task_308 = Script_ShowUIMessageFromID("E1032A097");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        break;
      case 28:
        task_308 = Script_ShowUIMessageFromID("E1032A055");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I012");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032A056");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I013");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032A057");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE, 1);
        break;
      case 29:
        task_308 = Script_ShowUIMessageFromID("E1032A059");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        break;
      case 30:
        task_308 = Script_ShowUIMessageFromID("E1032A060");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        break;
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_OPERATOR_ADDRESS_LOOKUP_REFUSED, 1);
  } else {
    task_304 = 80;
    Script_scenCreateTask(script_1D128, 0);
    task_308 = Script_ShowUIMessageFromID("E1032I001");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F8 == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032A007");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032A006");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
      if (local_F8 == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032I002");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_F8 == 1) {
        task_308 = Script_ShowUIMessageFromID("E1032I003");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_F8 == 2) {
        task_308 = Script_ShowUIMessageFromID("E1032I004");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_F8 == 3) {
        task_308 = Script_ShowUIMessageFromID("E1032I005");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032I006");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F8 == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032A008");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032A009");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(5, data_323AC, 6, data_323B6, 7, data_323C0, 8, data_323CA, 9, data_323D4, 10, data_323DE, 11, data_323E8, 12, data_323F2, 13, data_323FC, 14, data_32406, 15, data_32410, 16, data_3241A, 17, data_32424, 18, data_3242E, 19, data_32438, 20, data_32442, 21, data_3244C, 22, data_32456, 23, data_32460, 24, data_3246A, 25, data_32474, 26, data_3247E, 27, data_32488, 28, data_32492, 29, data_3249C, 30, data_324A6, 31, data_324B0, 32, data_324BA, 33, data_324C4, 34, data_324CE, 35, data_324D8, 36, data_324E2, 37, data_324EC, 38, data_324F6, 39, data_32500, 40, data_3250A, 41, data_32514, 42, data_3251E, 43, data_32528, 44, data_32532, 45, data_3253C, 46, data_32546, 47, data_32550, 48, data_3255A, 49, data_32564, 50, data_3256E, 51, data_32578, 52, data_32582, 53, data_3258C, 54, data_32596, 55, data_325A0, 56, data_325AA, 57, data_325B4, 58, data_325BE, 59, data_325C8, 60, data_325D2, 61, data_325DC, 62, data_325E6, 63, data_325F0, 64, data_325FA, 65, data_32604, 66, data_3260E, 67, data_32618, 68, data_32622, 69, data_3262C, 70, data_32636, 71, data_32640, 72, data_3264A, 73, data_32654, 74, data_3265E, 75, data_32668, 76, data_32672, 77, data_3267C, 78, data_32686, 79, data_32690, 80, data_3269A, 81, data_326A4, 82, data_326AE, 83, data_326B8, 84, data_326C2, 85, data_326CC, 86, data_326D6, 87, data_326E0, 88, data_326EA, 89, data_326F4, 90, data_326FE, 91, data_32708, 92, data_32712, 93, data_3271C, 94, data_32726, 95, data_32730, 96, data_3273A, 97, data_32744, 98, data_3274E, 99, data_32758, 100, data_32762, 101, data_3276C, 102, data_32776, 103, data_32780, 104, data_3278A, 105, data_32794, 106, data_3279E, 107, data_327A8, 108, data_327B2, 109, data_327BC, 110, data_327C6, 111, data_327D0, 112, data_327DA, 113, data_327E4, 114, data_327EE, 115, data_327F8, 116, data_32802, 117, data_3280C, 118, data_32816, 119, data_32820, 120, data_3282A, 121, data_32834, 122, data_3283E, 123, data_32848, 124, data_32852, 125, data_3285C, 126, data_32866, 127, data_32870, 128, data_3287A, 129, data_32884, 130, data_3288E, 131, data_32898, 132, data_328A2, 133, data_328AC, 134, data_328B6, 135, data_328C0, 136, data_328CA, 137, data_328D4, 138, data_328DE, 139, data_328E8, 140, data_328F2, 141, data_328FC, 142, data_32906, 143, data_32910, 144, data_3291A, 145, data_32924, 146, data_3292E, 147, data_32938, 148, data_32942, 149, data_3294C, 150, data_32956, 151, data_32960, 152, data_3296A, 153, data_32974, 154, data_3297E, 155, data_32988, 156, data_32992, 157, data_3299C, 158, data_329A6, 159, data_329B0, 160, data_329BA, 161, data_329C4, 162, data_329CE)))
static void script_136EB(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  task_304 = 70;
  Script_scenCreateTask(script_1D128, 0);
  task_308 = Script_ShowUIMessageFromID("E1032K001");
  while (Script_convGetIsActive((const void *)task_308)) {
    SCNF_WaitFrames_0000(1);
  }
  task_308 = 0;
  task_308 = Script_ShowUIMessageFromID("E1032K002");
  while (Script_convGetIsActive((const void *)task_308)) {
    SCNF_WaitFrames_0000(1);
  }
  task_308 = 0;
  task_308 = Script_ShowUIMessageFromID("E1032K019");
  while (Script_convGetIsActive((const void *)task_308)) {
    SCNF_WaitFrames_0000(1);
  }
  task_308 = 0;
  task_308 = Script_ShowUIMessageFromID("E1032K020");
  while (Script_convGetIsActive((const void *)task_308)) {
    SCNF_WaitFrames_0000(1);
  }
  task_308 = 0;
  task_308 = Script_ShowUIMessageFromID("E1032K022");
  while (Script_convGetIsActive((const void *)task_308)) {
    SCNF_WaitFrames_0000(1);
  }
  task_308 = 0;
  Script_ReturnTimeDate(&local_F4);
  if (((&local_F4)->month == 1) | ((&local_F4)->month == 3) | ((&local_F4)->month == 5) | ((&local_F4)->month == 7) | ((&local_F4)->month == 8) | ((&local_F4)->month == 10) | ((&local_F4)->month == 12)) {
    if ((&local_F4)->day >= 31) {
      if ((&local_F4)->month == 12) {
        local_F4.year = (&local_F4)->year + 1;
        (&local_F4)->month = 1;
      } else {
        (&local_F4)->month = (&local_F4)->month + 1;
      }
      (&local_F4)->day = 1;
    } else {
      (&local_F4)->day = (&local_F4)->day + 1;
    }
  } else if ((&local_F4)->month == 2) {
    if ((&local_F4)->day >= 28) {
      (&local_F4)->month = 3;
      (&local_F4)->day = 1;
    } else {
      (&local_F4)->day = (&local_F4)->day + 1;
    }
  } else if ((&local_F4)->day >= 30) {
    (&local_F4)->month = (&local_F4)->month + 1;
    (&local_F4)->day = 1;
  } else {
    (&local_F4)->day = (&local_F4)->day + 1;
  }
  local_F0 = Script_WeatherUtils_0(WeatherDataUtil_LookupFCASDateCode, (&local_F4)->year, (&local_F4)->month, (&local_F4)->day);
  local_EC = local_F0;
  if (local_EC == 1) {
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 2) {
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 3) {
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 4) {
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 5) {
    task_308 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 6) {
    task_308 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 7) {
    task_308 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 8) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 9) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 10) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 11) {
    task_308 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 12) {
    task_308 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 13) {
    task_308 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 14) {
    task_308 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 15) {
    task_308 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 16) {
    task_308 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 17) {
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 18) {
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 19) {
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 20) {
    task_308 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 21) {
    task_308 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 22) {
    task_308 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 23) {
    task_308 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 24) {
    task_308 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 25) {
    task_308 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 26) {
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 27) {
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 28) {
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 29) {
    task_308 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 30) {
    task_308 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 31) {
    task_308 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 32) {
    task_308 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 33) {
    task_308 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 34) {
    task_308 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 35) {
    task_308 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 36) {
    task_308 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 37) {
    task_308 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 38) {
    task_308 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 39) {
    task_308 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 40) {
    task_308 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 41) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 42) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 43) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 44) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 45) {
    task_308 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 46) {
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 47) {
    task_308 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 48) {
    task_308 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 54) {
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 50) {
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 51) {
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 52) {
    task_308 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 53) {
    task_308 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 54) {
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 55) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 56) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 57) {
    task_308 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 58) {
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 59) {
    task_308 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 60) {
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 61) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 62) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 63) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 64) {
    task_308 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 65) {
    task_308 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 66) {
    task_308 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else if (local_EC == 67) {
    task_308 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else {
    task_308 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  }
  return;
}

static int8_t script_15772(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int8_t local_E7;
  uint8_t unused_E4[3];
  Script_ReturnTimeDate(&local_F4);
  if (arg0 == arg2) {
    if (arg1 < arg3) {
      local_E7 = 1;
    } else {
      local_E7 = 0;
    }
  } else if (arg0 < arg2) {
    local_E7 = 1;
  } else {
    local_E7 = 0;
  }
  local_F0 = arg0 * 100 + arg1;
  local_EC = arg2 * 100 + arg3;
  local_E8 = (&local_F4)->hours * 100 + (&local_F4)->minutes;
  if (local_E7) {
    if ((local_E8 >= local_F0) & (local_E8 < local_EC)) {
      local_E7 = 1;
    } else {
      local_E7 = 0;
    }
  } else if ((local_E8 >= local_F0) | (local_E8 < local_EC)) {
    local_E7 = 1;
  } else {
    local_E7 = 0;
  }
  return local_E7;
}

static int8_t script_1582B(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  int8_t local_ED;
  int8_t local_EC;
  int32_t local_E8;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (script_15772(8, 30, 15, 10)) {
    local_EE = 0;
  } else if (script_15772(15, 10, 20, 30)) {
    local_EE = 1;
  } else {
    local_EE = 2;
  }
  if ((&local_F4)->day & 1) {
    local_ED = 0;
  } else {
    local_ED = 1;
  }
  local_EF = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 950) == 0) {
      local_EF = 7;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 950, 1);
    } else {
      local_EF = -2;
    }
  } else {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG)) {
      if ((local_EE == 2) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NAOYUKI_MOTORCYCLE_THANKS_CALL) == 0)) {
        local_EF = 8;
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NAOYUKI_MOTORCYCLE_THANKS_CALL, 1);
      }
      goto L015920;
    }
  }
  L015920:;
  if (local_EF == 0) {
    local_EF = script_11D7E(local_EF);
  }
  if (local_EF == 0) {
    local_E8 = local_EE;
    if (local_E8 == 0) {
      if (local_ED == 0) {
        local_EF = 5;
      } else {
        local_EF = 6;
      }
    } else if (local_E8 == 1) {
      if (local_ED == 0) {
        local_EF = -1;
      } else {
        local_EF = -2;
      }
    } else {
      local_EC = (&local_F4)->day & 3;
      local_E8 = local_EC;
      if (local_E8 == 1) {
        local_EF = 1;
      } else if (local_E8 == 2) {
        if (local_F0 >= 11) {
          local_EF = 2;
        } else {
          local_EF = 3;
        }
      } else {
        local_EF = 4;
      }
    }
  }
  return local_EF;
}

__attribute__((scn_string_bindings(0, data_329D8, 8, data_32A28, 9, data_32A32, 14, data_32A64, 17, data_32A82, 18, data_32A8C, 19, data_32A96, 21, data_32AAA, 25, data_32AD2, 27, data_32AE6, 33, data_32B22, 40, data_32B68, 41, data_32B72, 42, data_32B7C, 43, data_32B86, 44, data_32B90, 48, data_32BB8, 49, data_32BC2, 52, data_32BE0, 53, data_32BEA, 57, data_32C12, 59, data_32C26, 61, data_32C3A, 62, data_32C44, 70, data_32C94, 75, data_32CC6, 76, data_32CD0, 77, data_32CDA, 82, data_32D0C, 83, data_32D16, 84, data_32D20, 85, data_32D2A, 86, data_32D34, 87, data_32D3E, 88, data_32D48, 92, data_32D70, 93, data_32D7A)))
static void script_159E1(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 1:
      task_304 = 170;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032L001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A098");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L002");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A099");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L003");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L005");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A100");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 2:
      task_304 = 180;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032L001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L006");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L007");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L008");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L009");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A102");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L010");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 3:
      task_304 = 190;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032L011");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L012");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L013");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L014");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L015");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 191;
      task_308 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L016");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A105");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L017");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A106");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 4:
      task_304 = 200;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032L018");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A107");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A108");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L020");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L021");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A109");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L022");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A110");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L023");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 5:
      task_304 = 210;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032M001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A112");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M003");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A113");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A114");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M005");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 6:
      task_304 = 220;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032M006");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A115");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M018");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A122");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A123");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M020");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A114");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M010");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 7:
      task_304 = 230;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032N001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A124");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A125");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032N002");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A126");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032N003");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 231;
      task_308 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A128");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032N004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A129");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032N005");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 8:
      task_304 = 240;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032L011");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A117");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L024");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L025");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L026");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A118");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032L023");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 9:
      task_308 = Script_ShowUIMessageFromID("E1032M001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A112");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M015");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A119");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M016");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A118");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032M017");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
  }
  return;
}

static int8_t script_16B52(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  uint8_t unused_EC[2];
  int32_t local_E8;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_EF = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    local_EF = -1;
  } else if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_DNOZ_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DKTY_TUBA_CUTSCENE_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_ROUTE_ACTIVE) == 0)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01BNK_BANK_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 0))) {
    if (script_15772(8, 30, 21, 0)) {
      local_EF = 1;
    } else {
      local_EF = 2;
    }
  } else if ((local_F0 == 13) | (local_F0 == 174)) {
    if (script_15772(8, 30, 22, 0)) {
      local_EF = 11;
    } else {
      local_EF = 10;
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SORRY_ABOUT_OTHER_DAY_CALL) == 0)) {
    local_EF = 3;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_JUTADN_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_YBHN_EVENT_DONE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 946) == 0)) {
    local_EF = 6;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_YBHN_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_ROUTE_ACTIVE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 537) == 0)) {
    local_EF = 12;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_ROUTE_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SPRUNG_ON_YOU_CALL) == 0)) {
    local_EF = 7;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SHRINE_KITTEN_CALL) == 0) & -(local_F0 >= 162)) {
    local_EF = 13;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KITTEN_RECOVERED_CALL) == 0) & -(local_F0 >= 201)) {
    local_EF = 14;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FLOWERS_THANKS_CALL) == 0)) {
    local_EF = 4;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_STAY_AWAY_FROM_HARBOR_CALL) == 0)) {
    local_EF = 16;
  } else {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_AFTER_RESCUE_CALL) == 0)) {
      local_EF = 5;
      goto L016ECB;
    }
  }
  L016ECB:;
  if (local_EF == 0) {
    if ((local_F0 <= 13) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TALK_TO_INE_CALL) == 0)) {
      local_EF = 17;
    } else if (-(local_F0 >= 21) & -(local_F0 <= 102) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_PARENTS_IN_VANCOUVER_CALL) == 0)) {
      local_EF = 18;
    } else if (-(local_F0 >= 61) & -(local_F0 <= 102) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_CALL_NAOYUKI_AND_ICHIRO_CALL) == 0)) {
      local_EF = 19;
    } else if (-(local_F0 >= 111) & -(local_F0 <= 193) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_LAPIS_FORTUNETELLER_CALL) == 0)) {
      local_EF = 20;
    } else if (-(local_F0 >= 141) & -(local_F0 <= 193) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FIRST_MEETING_CALL) == 0)) {
      local_EF = 22;
    } else if (-(local_F0 >= 171) & -(local_F0 <= 193) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_NURSERY_TEACHER_CALL) == 0)) {
      local_EF = 23;
    } else if (-(local_F0 >= 211) & -(local_F0 <= 218) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FEELS_WATCHED_CALL) == 0)) {
      local_EF = 24;
    } else if (-(local_F0 >= 201) & -(local_F0 <= 241) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TRIP_INVITATION_CALL) == 0)) {
      local_EF = 25;
    } else {
      if (-(local_F0 >= 215) & -(local_F0 <= 241) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_RELAY_RACE_PHOTO_CALL) == 0)) {
        local_EF = 21;
        goto L01702F;
      }
    }
  }
  L01702F:;
  if (local_EF == 0) {
    local_EE = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
    local_E8 = local_EE;
    switch (local_E8) {
      case 0:
        local_EF = 8;
        break;
      case 1:
        local_EF = 9;
        break;
      case 2:
        local_EF = 10;
        break;
      case 3:
        local_EF = 26;
        break;
    }
  }
  return local_EF;
}

__attribute__((scn_string_bindings(0, data_32D8E, 1, data_32D98, 2, data_32DA2, 4, data_32DB6, 5, data_32DC0, 6, data_32DCA, 7, data_32DD4, 8, data_32DDE, 9, data_32DE8, 10, data_32DF2, 11, data_32DFC, 12, data_32E06, 13, data_32E10, 15, data_32E24, 16, data_32E2E, 17, data_32E38, 18, data_32E42, 19, data_32E4C, 20, data_32E56, 21, data_32E60, 22, data_32E6A, 23, data_32E74, 25, data_32E88, 27, data_32E9C, 28, data_32EA6, 31, data_32EC4, 32, data_32ECE, 33, data_32ED8, 38, data_32F0A, 39, data_32F14, 40, data_32F1E, 41, data_32F28, 43, data_32F3C, 44, data_32F46, 45, data_32F50, 56, data_32FBE, 57, data_32FC8, 58, data_32FD2, 59, data_32FDC, 60, data_32FE6, 61, data_32FF0, 64, data_3300E, 65, data_33018, 66, data_33022, 67, data_3302C, 68, data_33036, 69, data_33040, 70, data_3304A, 71, data_33054, 72, data_3305E, 74, data_33072, 76, data_33086, 78, data_3309A, 79, data_330A4, 80, data_330AE, 81, data_330B8, 82, data_330C2, 84, data_330D6, 86, data_330EA, 87, data_330F4, 88, data_330FE, 89, data_33108, 90, data_33112, 91, data_3311C, 92, data_33126, 93, data_33130, 94, data_3313A, 96, data_3314E, 99, data_3316C, 100, data_33176, 101, data_33180, 102, data_3318A, 103, data_33194, 104, data_3319E, 106, data_331B2, 107, data_331BC, 111, data_331E4, 113, data_331F8, 115, data_3320C, 116, data_33216, 117, data_33220, 118, data_3322A, 119, data_33234, 120, data_3323E, 125, data_33270, 127, data_33284, 128, data_3328E, 129, data_33298, 132, data_332B6, 133, data_332C0, 134, data_332CA, 135, data_332D4, 136, data_332DE, 137, data_332E8, 138, data_332F2, 139, data_332FC, 140, data_33306, 141, data_33310, 142, data_3331A, 143, data_33324, 144, data_3332E, 150, data_3336A, 158, data_333BA, 159, data_333C4, 160, data_333CE, 161, data_333D8, 163, data_333EC, 172, data_33446, 173, data_33450, 174, data_3345A, 175, data_33464, 182, data_334AA, 185, data_334C8, 186, data_334D2, 187, data_334DC, 198, data_3354A, 200, data_3355E, 201, data_33568, 208, data_335AE, 209, data_335B8, 216, data_335FE, 224, data_3364E, 225, data_33658, 226, data_33662, 232, data_3369E, 234, data_336B2, 235, data_336BC, 237, data_336D0, 239, data_336E4, 244, data_33716, 245, data_33720, 256, data_3378E, 258, data_337A2, 260, data_337B6, 261, data_337C0, 265, data_337E8, 269, data_33810, 271, data_33824, 273, data_33838, 274, data_33842, 275, data_3384C, 276, data_33856, 277, data_33860, 278, data_3386A, 280, data_3387E, 282, data_33892, 285, data_338B0, 288, data_338CE, 289, data_338D8, 290, data_338E2, 295, data_33914, 297, data_33928, 299, data_3393C, 300, data_33946, 301, data_33950, 302, data_3395A, 303, data_33964, 304, data_3396E, 308, data_33996, 311, data_339B4, 313, data_339C8, 315, data_339DC, 316, data_339E6, 317, data_339F0, 318, data_339FA, 331, data_33A7C)))
static void script_170A1(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F4 = arg0;
  switch (local_F4) {
    case 1:
      task_304 = 90;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032O001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A136");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O002");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A137");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O007");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A138");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O005");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A139");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O006");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 2:
      task_304 = 90;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032O001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A136");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O002");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A140");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O007");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A138");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O005");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A139");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O006");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 3:
      task_304 = 90;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P002");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P003");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A141");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A142");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P005");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SORRY_ABOUT_OTHER_DAY_CALL, 1);
      break;
    case 4:
      task_304 = 100;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P006");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A143");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P007");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A144");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 101;
      task_308 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P009");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FLOWERS_THANKS_CALL, 1);
      break;
    case 5:
      task_304 = 110;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A145");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P011");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A146");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P012");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A147");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P013");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A148");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P014");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      SCNF_WaitFrames_0000(60);
      task_304 = 111;
      task_308 = Script_ShowUIMessageFromID("E1032A149");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P015");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_AFTER_RESCUE_CALL, 1);
      break;
    case 6:
      task_304 = 120;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A150");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A151");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 946, 1);
      break;
    case 7:
      task_304 = 130;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      SCNF_WaitFrames_0000(60);
      task_304 = 131;
      task_308 = Script_ShowUIMessageFromID("E1032A153");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P022");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P023");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P024");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SPRUNG_ON_YOU_CALL, 1);
      break;
    case 8:
      task_304 = 140;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P025");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A155");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A156");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 141;
      task_308 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P027");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 9:
      task_304 = 150;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P028");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A158");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P030");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 151;
      task_308 = Script_ShowUIMessageFromID("E1032A159");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P066");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 10:
      task_304 = 160;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P032");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A160");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P033");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      SCNF_WaitFrames_0000(60);
      task_304 = 161;
      if ((local_F8 == 13) | (local_F8 == 174)) {
        task_308 = Script_ShowUIMessageFromID("E1032P034");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
        local_F4 = local_FF;
        switch (local_F4) {
          case 0:
            task_308 = Script_ShowUIMessageFromID("E1032P080");
            while (Script_convGetIsActive((const void *)task_308)) {
              SCNF_WaitFrames_0000(1);
            }
            task_308 = 0;
            break;
          case 1:
            task_308 = Script_ShowUIMessageFromID("E1032P081");
            while (Script_convGetIsActive((const void *)task_308)) {
              SCNF_WaitFrames_0000(1);
            }
            task_308 = 0;
            break;
        }
      }
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P035");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A141");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P036");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 11:
      task_304 = 162;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P025");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A160");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P033");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      SCNF_WaitFrames_0000(60);
      task_304 = 163;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F4 = local_FF;
      switch (local_F4) {
        case 0:
          task_308 = Script_ShowUIMessageFromID("E1032P082");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
        case 1:
          task_308 = Script_ShowUIMessageFromID("E1032P083");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
        case 2:
          task_308 = Script_ShowUIMessageFromID("E1032P084");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032P085");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
      }
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P086");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 12:
      task_304 = 370;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A165");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P037");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P120");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A173");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 371;
      task_308 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P027");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 537, 1);
      break;
    case 13:
      task_304 = 380;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A170");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P044");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A200");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A172");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P045");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A173");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P046");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P047");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 381;
      task_308 = Script_ShowUIMessageFromID("E1032A201");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P049");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A176");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P050");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A177");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P051");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SHRINE_KITTEN_CALL, 1);
      break;
    case 14:
      task_304 = 390;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A178");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P052");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A179");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A180");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A181");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 391;
      task_308 = Script_ShowUIMessageFromID("E1032P053");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P087");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P055");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A182");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P051");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KITTEN_RECOVERED_CALL, 1);
      break;
    case 15:
      task_304 = 400;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A183");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P038");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A184");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P056");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P088");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P058");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A220");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A185");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P059");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KEEP_KITTEN_CALL, 1);
      break;
    case 16:
      task_304 = 410;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P060");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A186");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P061");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A187");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P062");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 411;
      task_308 = Script_ShowUIMessageFromID("E1032A188");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P063");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      SCNF_WaitFrames_0000(40);
      task_308 = Script_ShowUIMessageFromID("E1032P064");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P065");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A189");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P066");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A190");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_STAY_AWAY_FROM_HARBOR_CALL, 1);
      break;
    case 17:
      task_304 = 270;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P067");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A191");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A192");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P069");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A193");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TALK_TO_INE_CALL, 1);
      break;
    case 24:
      task_304 = 280;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P070");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A194");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P071");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A195");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P072");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P073");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A151");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P074");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P075");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P089");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P090");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 281;
      task_308 = Script_ShowUIMessageFromID("E1032A196");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P079");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A197");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FEELS_WATCHED_CALL, 1);
      break;
    case 21:
      task_304 = 290;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P091");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P092");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A203");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P093");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      SCNF_WaitFrames_0000(60);
      task_308 = Script_ShowUIMessageFromID("E1032P094");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P095");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_RELAY_RACE_PHOTO_CALL, 1);
      break;
    case 26:
      task_304 = 300;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P096");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P097");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 301;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F4 = local_FF;
      switch (local_F4) {
        case 0:
          task_308 = Script_ShowUIMessageFromID("E1032P098");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
        case 1:
          task_308 = Script_ShowUIMessageFromID("E1032P099");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
        case 2:
          task_308 = Script_ShowUIMessageFromID("E1032P100");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
      }
      task_308 = Script_ShowUIMessageFromID("E1032A206");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 18:
      task_304 = 310;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P101");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A207");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P102");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A208");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P103");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 311;
      task_308 = Script_ShowUIMessageFromID("E1032A209");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P104");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P105");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A210");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P106");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P107");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P108");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_PARENTS_IN_VANCOUVER_CALL, 1);
      break;
    case 19:
      task_304 = 320;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P109");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P110");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A211");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P111");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P112");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 321;
      task_308 = Script_ShowUIMessageFromID("E1032A212");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P113");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A158");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P114");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P115");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_CALL_NAOYUKI_AND_ICHIRO_CALL, 1);
      break;
    case 25:
      task_304 = 330;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P117");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A214");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P118");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P119");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P120");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 331;
      task_308 = Script_ShowUIMessageFromID("E1032A215");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P121");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P123");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A217");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TRIP_INVITATION_CALL, 1);
      break;
    case 20:
      task_304 = 340;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P124");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P125");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A218");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P126");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P127");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P128");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_LAPIS_FORTUNETELLER_CALL, 1);
      break;
    case 23:
      task_304 = 350;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032P109");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P117");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A214");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P129");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P130");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P131");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A219");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P132");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      SCNF_WaitFrames_0000(60);
      task_304 = 351;
      task_308 = Script_ShowUIMessageFromID("E1032A153");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P133");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A220");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P134");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_NURSERY_TEACHER_CALL, 1);
      break;
    case 22:
      task_304 = 360;
      Script_scenCreateTask(script_1D128, 0);
      task_308 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A221");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P135");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P136");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A222");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P137");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P138");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A223");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P139");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_304 = 361;
      task_308 = Script_ShowUIMessageFromID("E1032A224");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      SCNF_WaitFrames_0000(60);
      task_308 = Script_ShowUIMessageFromID("E1032P140");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A225");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A226");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FIRST_MEETING_CALL, 1);
      break;
  }
  return;
}

__attribute__((scn_string_bindings(5, data_33AB8, 7, data_33ACC, 8, data_33AD6, 21, data_33B58, 22, data_33B62, 30, data_33BB2, 32, data_33BC6, 33, data_33BD0, 48, data_33C66, 49, data_33C70)))
static void script_1AF5F(void) {
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    task_304 = 250;
    Script_scenCreateTask(script_1D128, 0);
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032E001");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_FC == 1) {
        task_308 = Script_ShowUIMessageFromID("E1032E002");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032E003");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
    task_308 = Script_ShowUIMessageFromID("E1032A010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032E004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      switch (local_FC) {
        case 0:
          task_308 = Script_ShowUIMessageFromID("E1032A011");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032E005");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032A012");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
        case 1:
          task_308 = Script_ShowUIMessageFromID("E1032A013");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032E006");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032A014");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
        case 2:
          task_308 = Script_ShowUIMessageFromID("E1032A015");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032E007");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032A016");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
      }
    }
    task_308 = Script_ShowUIMessageFromID("E1032E008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_304 = 251;
    task_308 = Script_ShowUIMessageFromID("E1032E009");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032A017");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032E010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032E011");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_FC == 1) {
        task_308 = Script_ShowUIMessageFromID("E1032E012");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032E013");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032A018");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_FC == 1) {
        task_308 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032A020");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
    task_308 = Script_ShowUIMessageFromID("E1032A021");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  } else {
    task_304 = 260;
    Script_scenCreateTask(script_1D128, 0);
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032F001");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_FC == 1) {
        task_308 = Script_ShowUIMessageFromID("E1032F002");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032F003");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
    task_308 = Script_ShowUIMessageFromID("E1032A022");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032F004");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      switch (local_FC) {
        case 0:
          task_308 = Script_ShowUIMessageFromID("E1032A011");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032F005");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032A012");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
        case 1:
          task_308 = Script_ShowUIMessageFromID("E1032A013");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032F006");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032A023");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
        case 2:
          task_308 = Script_ShowUIMessageFromID("E1032A024");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032F007");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032A025");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          task_308 = Script_ShowUIMessageFromID("E1032A026");
          while (Script_convGetIsActive((const void *)task_308)) {
            SCNF_WaitFrames_0000(1);
          }
          task_308 = 0;
          break;
      }
    }
    task_304 = 261;
    task_308 = Script_ShowUIMessageFromID("E1032F008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032F009");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032F010");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_FC == 1) {
        task_308 = Script_ShowUIMessageFromID("E1032F011");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032F012");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032F013");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_FC == 1) {
        task_308 = Script_ShowUIMessageFromID("E1032F014");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032F015");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_FC == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032A018");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
    task_308 = Script_ShowUIMessageFromID("E1032A027");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  }
  return;
}

static int8_t script_1BA27(void) {
  int8_t local_FF;
  local_FF = 0;
  task_304 = 55;
  Script_scenCreateTask(script_1D128, 0);
  local_FF = script_1BA4F();
  return local_FF;
}

static int8_t script_1BA4F(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  local_F7 = 0;
  task_308 = Script_ShowUIMessageFromID("E1032A034");
  while (Script_convGetIsActive((const void *)task_308)) {
    SCNF_WaitFrames_0000(1);
  }
  task_308 = 0;
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    local_F8 = 0;
  } else {
    local_F8 = 2;
  }
  script_1BB6A(local_F8);
  local_FC = script_1BCBC();
  script_1BCD7(local_FC);
  if (script_1BC53(local_F8, local_FC)) {
    if (local_F8 == 0) {
      local_F8 = 2;
    } else {
      local_F8 = 0;
    }
    task_304 = 56;
    Script_mapmSetCamFlags(0, 0);
    Script_charaVisUtils(CHARA_AKIR, 1);
    script_1BB6A(local_F8);
    local_FC = script_1BCBC();
    script_1BCD7(local_FC);
    Script_mapmSetCamFlags(0, 1);
    Script_charaVisUtils(CHARA_AKIR, 0);
    if (script_1BC53(local_F8, local_FC)) {
      local_F7 = script_1C0AD();
    } else {
      script_1BDC0(local_FC);
    }
  } else {
    script_1BDC0(local_FC);
  }
  return local_F7;
}

static void script_1BB6A(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      task_308 = Script_ShowUIMessageFromID("E1032H005");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 1:
      task_308 = Script_ShowUIMessageFromID("E1032H004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 2:
      task_308 = Script_ShowUIMessageFromID("E1032H009");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 3:
      task_308 = Script_ShowUIMessageFromID("E1032H006");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
  }
  return;
}

static int32_t script_1BC53(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = -1;
  local_F8 = arg1;
  switch (local_F8) {
    case 5:
      local_FC = 1;
      break;
    case 6:
      local_FC = 2;
      break;
    case 7:
      local_FC = 3;
      break;
    case 8:
      local_FC = 0;
      break;
  }
  if (arg0 == local_FC) {
    (void)1;
  } else {
    (void)0;
  }
  end:;
}

static int32_t script_1BCBC(void) {
  return SCNF_RequestActionSelectorFromParams_0008(7, 8, 6, 5, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
}

static void script_1BCD7(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 5:
      task_308 = Script_ShowUIMessageFromID("E1032A042");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 6:
      task_308 = Script_ShowUIMessageFromID("E1032A040");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 7:
      task_308 = Script_ShowUIMessageFromID("E1032A043");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
    case 8:
      task_308 = Script_ShowUIMessageFromID("E1032A041");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      break;
  }
  return;
}

__attribute__((scn_string_bindings(1, data_33CE8, 2, data_33CF2, 3, data_33CFC, 4, data_33D06)))
static void script_1BDC0(int32_t arg0) {
  int32_t local_FC;
  task_304 = 58;
  script_1BFE5(1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 714)) {
    if ((arg0 == 5) | (arg0 == 6) | (arg0 == 7) | (arg0 == 8)) {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_F8 == 0) {
      } else if (local_F8 == 1) {
        task_308 = Script_ShowUIMessageFromID("E1032A045");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else if (local_F8 == 2) {
        task_308 = Script_ShowUIMessageFromID("E1032A046");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032A047");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    } else {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_F8 == 0) {
      } else if (local_F8 == 1) {
        task_308 = Script_ShowUIMessageFromID("E1032A046");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032A047");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
    }
  } else {
    task_308 = Script_ShowUIMessageFromID("E1032A044");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 9) {
    local_FC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 9);
    local_FC = local_FC + 1;
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 9, local_FC);
  }
  return;
}

static void script_1BFAE(void) {
  task_308 = Script_ShowUIMessageFromID("E1032A051");
  while (Script_convGetIsActive((const void *)task_308)) {
    SCNF_WaitFrames_0000(1);
  }
  task_308 = 0;
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_1BFE5(int32_t arg0) {
  Script_executeAicaCommand(525739, 0, 0);
  SCNF_WaitFrames_0000(10);
  if (arg0) {
    task_308 = Script_ShowUIMessageFromID("E1032A039");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
  }
  Script_executeAicaCommand(394667, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(460203, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(394667, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(460203, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(394667, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(460203, 0, 0);
  SCNF_WaitFrames_0000(15);
  return;
}

__attribute__((scn_string_bindings(0, data_33D2E, 1, data_33D38, 2, data_33D42, 3, data_33D4C, 4, data_33D56, 5, data_33D60, 6, data_33D6A, 7, data_33D74, 8, data_33D7E, 10, data_33D92, 11, data_33D9C, 15, data_33DC4, 16, data_33DCE, 17, data_33DD8, 18, data_33DE2, 19, data_33DEC, 20, data_33DF6, 21, data_33E00, 22, data_33E0A, 23, data_33E14, 24, data_33E1E, 25, data_33E28, 26, data_33E32, 27, data_33E3C, 28, data_33E46, 29, data_33E50, 33, data_33E78, 34, data_33E82, 35, data_33E8C, 36, data_33E96, 37, data_33EA0, 38, data_33EAA, 39, data_33EB4, 40, data_33EBE, 41, data_33EC8, 42, data_33ED2, 43, data_33EDC, 44, data_33EE6, 45, data_33EF0, 46, data_33EFA, 47, data_33F04, 48, data_33F0E, 49, data_33F18, 52, data_33F36, 53, data_33F40, 54, data_33F4A, 59, data_33F7C)))
static int8_t script_1C0AD(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  int32_t local_F0;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F7 = 0;
  task_304 = 57;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (local_FC >= 17) {
    task_308 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032A120");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032H010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032A066");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032H011");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    SCNF_WaitFrames_0000(60);
    if (((local_F8 >= 170) & (local_F8 <= 193)) | ((local_F8 >= 220) & (local_F8 <= 241))) {
      local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F0 == 0) {
        task_308 = Script_ShowUIMessageFromID("E1032J001");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032A131");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032J010");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032J013");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      } else {
        task_308 = Script_ShowUIMessageFromID("E1032J001");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032A134");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032J015");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
        task_308 = Script_ShowUIMessageFromID("E1032A135");
        while (Script_convGetIsActive((const void *)task_308)) {
          SCNF_WaitFrames_0000(1);
        }
        task_308 = 0;
      }
      script_1BFE5(0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 546)) {
      task_308 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J018");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J020");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 546, 0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED)) {
      task_308 = Script_ShowUIMessageFromID("E1032J016");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A161");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J017");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A162");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
    } else {
      task_308 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A131");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J010");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A132");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J011");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J012");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) {
    task_308 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032A120");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032H010");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032A066");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032H011");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    SCNF_WaitFrames_0000(60);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 546)) {
      task_304 = 65;
      task_308 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J018");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J020");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 546, 0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED)) {
      task_304 = 66;
      task_308 = Script_ShowUIMessageFromID("E1032J016");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A161");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J017");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A162");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE)) {
      task_308 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A067");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J002");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
    } else {
      task_308 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J003");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A069");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032J004");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
      task_308 = Script_ShowUIMessageFromID("E1032A070");
      while (Script_convGetIsActive((const void *)task_308)) {
        SCNF_WaitFrames_0000(1);
      }
      task_308 = 0;
    }
  } else {
    local_F7 = 1;
    task_308 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032A048");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032H008");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032A049");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    task_308 = Script_ShowUIMessageFromID("E1032A050");
    while (Script_convGetIsActive((const void *)task_308)) {
      SCNF_WaitFrames_0000(1);
    }
    task_308 = 0;
    script_1BFE5(1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  }
  return local_F7;
}

static void script_1CD7C(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  script_75EB(CHARA_AKIR, 38.009102f, 0.114f, 9.338f);
  script_7613(CHARA_AKIR, 0.0f, 244.4f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_SPEAK_TEL_LP_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_31E94, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 0, 0, 0, 1);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_31E94, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 1, 0, 0, 1);
  Script_scenCreateTask(script_1D0A5, 0);
  SCNF_WaitFrames_0000(1);
  Script_TELMUtils(TELM_CMD_Cleanup, task_2E8);
  Script_TELMUtils(TELM_CMD_SetTargetTaskAndMode, task_2E8, 0x52494B41, 3);
  local_F4.x = -0.05f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.02f;
  local_E8.x = 51336;
  (&local_E8)->y = 546;
  (&local_E8)->z = 55523;
  Script_TELMUtils(TELM_CMD_SetCustomReceiverOffset, task_2E8, &local_F4);
  Script_TELMUtils(TELM_CMD_SetCustomReceiverEuler, task_2E8, &local_E8);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_executeAicaCommand(591275, 0, 0);
  return;
}

static void script_1CF02(void) {
  task_304 = -1;
  Script_TELMUtils(TELM_CMD_Cleanup, task_2E8);
  return;
}

static void script_1CF16(void) {
  script_75EB(CHARA_AKIR, 37.51f, 0.07f, 9.08f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_1CF60(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  task_379 = 1;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 4);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_OKU_TEL_F, -1.0f, 55.0f, -1.0f, 2.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_31F78, 5);
  Script_TELMUtils(TELM_CMD_ResetPeerState, task_2E8);
  Script_executeAicaCommand(67243, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_OKU_TEL_F, 55.0f, -1.0f, -1.0f, 2.0f, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_TELMUtils(TELM_CMD_SetCustomReceiverOffset, task_2E8, &local_F4);
  Script_TELMUtils(TELM_CMD_SetCustomReceiverEuler, task_2E8, &local_E8);
  return;
}

static int32_t script_1D0A5(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = -0.092468f;
  (&local_F4)->y = 1.467227f;
  (&local_F4)->z = -0.12347f;
  local_E8.x = -546;
  (&local_E8)->y = 728;
  (&local_E8)->z = -4551;
  task_379 = 0;
  for (;;) {
    if (task_379) {
      break;
    }
    Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E8);
    Script_SetMotnFlags(CHARA_AKIR, 11, &local_F4, 0x39000000);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_1D128(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_304;
    switch (local_FC) {
      case -1:
        goto L01D846;
        break;
      case 0:
        break;
      case 51:
        Script_seqEnterEvtCam(2004, 0, AUTH_None);
        break;
      case 53:
        Script_seqEnterEvtCam(2004, 0, AUTH_None);
        break;
      case 55:
        Script_seqEnterEvtCam(2006, task_2E8, task_2E8);
        break;
      case 56:
        Script_seqEnterEvtCam(2007, task_2E8, task_2E8);
        break;
      case 57:
        Script_seqEnterEvtCam(2008, task_2E8, task_2E8);
        break;
      case 58:
        Script_seqEnterEvtCam(2010, task_2E8, task_2E8);
        break;
      case 65:
        Script_seqEnterEvtCam(2016, 0, AUTH_None);
        break;
      case 66:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 70:
        local_FC = task_314;
        switch (local_FC) {
          case 0:
            Script_seqEnterEvtCam(2016, 0, AUTH_None);
            break;
          case 1:
            Script_seqEnterEvtCam(2017, 0, AUTH_None);
            break;
          case 2:
            Script_seqEnterEvtCam(2019, 0, AUTH_None);
            break;
          case 3:
            Script_seqEnterEvtCam(2020, 0, AUTH_None);
            break;
          case 4:
            Script_seqEnterEvtCam(2021, 0, AUTH_None);
            break;
        }
        task_314 = task_314 + 1;
        if (task_314 > 4) {
          task_314 = 0;
        }
        break;
      case 80:
        Script_seqEnterEvtCam(2017, 0, AUTH_None);
        break;
      case 81:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 82:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 90:
        Script_seqEnterEvtCam(2021, 0, AUTH_None);
        break;
      case 100:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 101:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 110:
        Script_seqEnterEvtCam(2021, 0, AUTH_None);
        break;
      case 111:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 120:
        Script_seqEnterEvtCam(2018, 0, AUTH_None);
        break;
      case 130:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 131:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 140:
        Script_seqEnterEvtCam(2016, 0, AUTH_None);
        break;
      case 141:
        Script_seqEnterEvtCam(2017, 0, AUTH_None);
        break;
      case 150:
        Script_seqEnterEvtCam(2020, 0, AUTH_None);
        break;
      case 151:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 160:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 161:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 162:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 163:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 170:
        Script_seqEnterEvtCam(2016, 0, AUTH_None);
        break;
      case 180:
        Script_seqEnterEvtCam(2021, 0, AUTH_None);
        break;
      case 190:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 191:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 200:
        Script_seqEnterEvtCam(2020, 0, AUTH_None);
        break;
      case 210:
        Script_seqEnterEvtCam(2017, 0, AUTH_None);
        break;
      case 220:
        Script_seqEnterEvtCam(2021, 0, AUTH_None);
        break;
      case 230:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 231:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 240:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 250:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 251:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 260:
        Script_seqEnterEvtCam(2016, 0, AUTH_None);
        break;
      case 261:
        Script_seqEnterEvtCam(2017, 0, AUTH_None);
        break;
      case 270:
        Script_seqEnterEvtCam(2016, 0, AUTH_None);
        break;
      case 280:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 281:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 290:
        Script_seqEnterEvtCam(2021, 0, AUTH_None);
        break;
      case 300:
        Script_seqEnterEvtCam(2016, 0, AUTH_None);
        break;
      case 301:
        Script_seqEnterEvtCam(2017, 0, AUTH_None);
        break;
      case 310:
        Script_seqEnterEvtCam(2020, 0, AUTH_None);
        break;
      case 311:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 320:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 321:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 330:
        Script_seqEnterEvtCam(2016, 0, AUTH_None);
        break;
      case 331:
        Script_seqEnterEvtCam(2017, 0, AUTH_None);
        break;
      case 340:
        Script_seqEnterEvtCam(2021, 0, AUTH_None);
        break;
      case 350:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 351:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 360:
        Script_seqEnterEvtCam(2021, 0, AUTH_None);
        break;
      case 361:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 370:
        Script_seqEnterEvtCam(2020, 0, AUTH_None);
        break;
      case 371:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 380:
        Script_seqEnterEvtCam(2021, 0, AUTH_None);
        break;
      case 381:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 390:
        Script_seqEnterEvtCam(2016, 0, AUTH_None);
        break;
      case 391:
        Script_seqEnterEvtCam(2017, 0, AUTH_None);
        break;
      case 400:
        Script_seqEnterEvtCam(2018, 0, AUTH_None);
        break;
      case 410:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        break;
      case 411:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
    }
    if (task_304) {
      task_304 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  L01D846:;
  task_304 = 0;
  return Script_srchControl(0);
}

static void script_1D854(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  SCNF_WaitFrames_0000(1);
  task_2E4 = Script_motiLoad(3, SHEN_NAME('D000'), "M_TPIN.BIN");
  SCNF_WaitFrames_0000(1);
  if (task_311 < 0) {
    script_1D927();
  } else {
    script_12EC1();
    if (task_313) {
      Script_seqEnterEvtCam(2015, task_2E8, task_2E8);
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static void script_1D8C7(void) {
  if (task_311 < 0) {
    script_1D9C5();
  }
  Script_ReallocateUNFO(task_2E4);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  SCNF_WaitFrames_0000(10);
  if (task_311 < 0) {
    script_75EB(CHARA_AKIR, 37.51f, 0.07f, 9.08f);
    Script_InitializeOsag(CHARA_AKIR);
  }
  return;
}

static void script_1D927(void) {
  Script_seqEnterEvtCam(2004, 0, AUTH_None);
  script_75EB(CHARA_AKIR, 38.009102f, 0.114f, 9.338f);
  script_7613(CHARA_AKIR, 0.0f, 244.4f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_OKU_TEL_F, 55.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1D9C5(void) {
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_OKU_TEL_F, 55.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x20);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x20);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_1DA61(int32_t arg0) {
  int32_t local_FC;
  if (arg0 != -1) {
    task_37C = arg0;
  }
  local_FC = task_37C;
  if (local_FC == 0) {
    Script_gachaponUtils(0, data_34138);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_341E1);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_3431C);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_34451);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_344F8);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_34665);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_3485E);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_34931);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_349A2);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_349EF);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_34A28);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_34B17);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_34B7A);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_34BCF);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_34C16);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_34C4F);
  } else {
    task_37C = -1;
  }
  return;
}

static int16_t script_1DC02(int8_t arg0, int8_t arg1) {
  int16_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_FE = -1;
  if (arg0 == -1) {
    local_FD = task_37C;
  } else {
    local_FD = arg0;
  }
  local_F8 = local_FD;
  switch (local_F8) {
    case 0:
      local_FE = *(int16_t *)((int8_t *)&data_33FDC + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_33FF4 + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_34018 + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_34044 + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_34058 + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_34080 + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_340A4 + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_340BC + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_340CC + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_340D4 + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_340DC + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_34100 + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_34110 + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_3411C + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_34128 + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_34130 + (arg1 << 1));
      break;
  }
  return local_FE;
}

static void script_1DDED(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_33FBC[*(int32_t *)arg0] - data_33FCC[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_33FBC[*(int32_t *)arg0] - data_33FCC[*(int32_t *)arg0]);
  return;
}

static int32_t script_1DE3C(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_33FBC[arg0] - data_33FCC[arg0]) {
    (void)1;
  } else if (arg0 == 15) {
    (void)3;
  } else {
    (void)2;
  }
  end:;
}

static void script_1DE8F(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
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
        goto L01DF00;
      }
      L01DF00:;
      break;
    case 3:
      local_EC = arg2;
      if (local_EC == 6) {
        (void)((local_F3 = -1) & 1);
        goto L01DF27;
      }
      L01DF27:;
      break;
    case 4:
      local_EC = arg2;
      if ((local_EC == 7) | (local_EC == 15) | (local_EC == 16) | (local_EC == 17) | (local_EC == 18)) {
        (void)((local_F3 = -1) & 1);
        goto L01DF66;
      }
      L01DF66:;
      break;
    case 5:
      (void)((local_F3 = -1) & 1);
      break;
    case 6:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 2) | (local_EC == 3)) {
        (void)((local_F3 = -1) & 1);
        goto L01DFB4;
      }
      L01DFB4:;
      break;
    case 8:
      local_EC = arg2;
      if (local_EC == 3) {
        (void)((local_F3 = -1) & 1);
        goto L01DFDB;
      }
      L01DFDB:;
      break;
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
  if (local_F3 & 8) {
    arg2 = arg2 + data_33FBC[arg1];
    local_F4.x = (&local_F4)->x * arg3;
    (&local_F4)->y = (&local_F4)->y * arg3;
    (&local_F4)->z = (&local_F4)->z * arg3;
  } else {
    local_F4.x = (&local_F4)->x * arg3 * 0.33333f;
    (&local_F4)->y = (&local_F4)->y * arg3 * 0.33333f;
    (&local_F4)->z = (&local_F4)->z * arg3 * 0.33333f;
  }
  if (local_F2 & 4) {
    script_1DA61(arg1);
    Script_gachaponUtils(5, arg0, arg2);
  } else {
    script_1DA61(arg1);
    Script_gachaponUtils(1, arg0, arg2);
  }
  if ((((scn3_bits){.f = (&local_F4)->x}).i != 0x3F800000) & (((scn3_bits){.f = (&local_F4)->x}).i != 0)) {
    Script_hmdlApplyScale(arg0, &local_F4);
  }
  return;
}

static void script_1E0C6(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  local_F8 = 9;
  local_FC = 0;
  while (local_FC < 6) {
    local_F4 = 1 << local_FC;
    if (local_F8 & local_F4) {
      local_D8 = *(&data_34C88 + local_FC);
      local_E8.x = *(float *)((const char *)&data_34CD0 + local_FC * 20);
      (&local_E8)->y = *(float *)((const char *)&data_34CD0 + (local_FC * 20 + 4));
      (&local_E8)->z = *(float *)((const char *)&data_34CD0 + (local_FC * 20 + 8));
      local_DC.x = 0;
      (&local_DC)->y = *(const uint32_t *)((const char *)&data_34CD0 + (local_FC * 20 + 12));
      (&local_DC)->z = 0;
      Script_EVT_SetPosition(local_D8, POS_XYZ, &local_E8);
      Script_EVT_SetRotation(local_D8, 0x38000000, &local_DC, 0);
      Script_charaDisplay(local_D8, CDM_Show);
      Script_SetOrGetWOCCValue(local_D8, *((int8_t *)&data_34CD0 + (local_FC * 20 + 16)));
      Script_MoveObjectToObject(*(&data_34CB8 + local_FC), *(&data_34C88 + local_FC));
      Script_EVT_SetPosition(*(&data_34CB8 + local_FC), POS_XYZ, &data_34D48);
      Script_EVT_SetRotation(*(&data_34CB8 + local_FC), 0x38000000, &data_34D54, 0);
      Script_charaDisplay(*(&data_34CB8 + local_FC), CDM_Show);
      goto L01E200;
    }
    L01E200:;
    local_FC = local_FC + 1;
  }
  task_380 = 0;
  (void)((task_388 = 0) & 2);
  (void)((task_389 = 0) & 2);
  (void)((task_38A = 0) & 2);
  return;
}

static int32_t script_1E232(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3 local_DC;
  int8_t local_DB;
  uint8_t unused_D8[3];
  float local_D4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, 0);
  local_D4 = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &local_DC);
  if (!(local_D4 >= 1.5f)) {
    (void)((local_DB = -1) & 0x80);
  } else {
    (void)((local_DB = 0) & 0x80);
  }
  if (task_388 & 1) {
    (void)((local_DB = 0) & 0x80);
  }
  return local_DB & 8;
}

static int32_t script_1E29C(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_E5;
  uint8_t unused_E4;
  int32_t local_E0;
  float local_DC;
  float local_D8;
  (void)((local_E7 = 0) & 0x80);
  (void)((local_E6 = -((local_E5 = 0) != 0)) & 0x10);
  local_F4.x = 0.5f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 1.5f;
  local_E8.x = -0.5f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = -0.15f;
  local_E0 = 0;
  while (local_E0 < 6) {
    int32_t local_D4;
    local_D4 = 9;
    if (local_D4 & (1 << local_E0)) {
      int32_t local_D0;
      local_D0 = *(&data_34C88 + local_E0);
      if (local_D0 != arg0) {
        Vector3 local_C4;
        Script_MoveObjectToObject(local_D0, arg0);
        Script_EVT_GetPosition(local_D0, -1, &local_C4, 0);
        Script_MoveObjectToObject(local_D0, -1);
        {
          float local_C0;
          local_C0 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_C4)->x, (&local_C4)->z) / 2.0f;
          if (!(local_C0 >= 0.5f)) {
            if (!((&local_C4)->x <= (float)0)) {
              if (~local_E5) {
                (void)((local_E5 = -1) & 1);
                local_D8 = local_C0;
              } else {
                if (!((&local_C4)->x >= local_D8)) {
                  local_D8 = local_C0;
                }
              }
            } else if (~local_E6) {
              (void)((local_E6 = -1) & 1);
              local_DC = (float)0 - local_C0;
            } else {
              if (!((&local_C4)->x <= local_DC)) {
                local_DC = (float)0 - local_C0;
              }
            }
          }
        }
      }
    }
    local_E0 = local_E0 + 1;
  }
  if (local_E5 & 0x20) {
    local_F4.x = local_D8;
  }
  if (local_E6 & 0x40) {
    local_E8.x = local_DC;
  }
  script_211B9(arg0, &local_F4);
  script_211B9(arg0, &local_E8);
  if (Script_SetTaskDataValue(CHARA_AKIR, &local_F4, &local_E8, 0)) {
    Vector3 local_CC;
    Vector3 local_C0;
    Vector3i local_B4;
    Script_EVT_GetPosition(arg0, -1, &local_C0, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_CC, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_B4, 0x42000000);
    local_CC.x = (&local_CC)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_B4)->y) * 0.3f;
    (&local_CC)->y = (&local_CC)->y;
    (&local_CC)->z = (&local_CC)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_B4)->y) * 0.3f;
    if (Script_CheckTargetWithinAngleRange(&local_CC, 1.5f, (&local_B4)->y, 5461, &local_C0)) {
      (void)((local_E7 = -1) & 0x10);
    }
  }
  return local_E7 & 0x80;
}

static int32_t script_1E4CD(void) {
  int16_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F8 = 0;
  local_FE = 0;
  while (local_FE < 6) {
    local_F4 = 9;
    if (local_F4 & (1 << local_FE)) {
      local_F0 = *(&data_34C88 + local_FE);
      if (script_1E232(local_F0)) {
        Script_scenToggleVisibleFor(local_F0, 1);
        Script_scenToggleVisibleFor(*(&data_34CB8 + local_FE), 1);
      } else {
        Script_scenToggleVisibleFor(local_F0, 0);
        Script_scenToggleVisibleFor(*(&data_34CB8 + local_FE), 0);
      }
      if (script_1E29C(local_F0)) {
        local_F8 = local_F0;
        task_380 = local_F0;
      }
    }
    local_FE = local_FE + 1;
  }
  task_380 = local_F8;
  return local_F8;
}

static int32_t script_1E57D(void) {
  int32_t local_FC;
  int8_t local_FB;
  local_FC = 0;
  (void)((local_FB = 0) & 2);
  if (Script_IsPlayerAllowed(0)) {
    if (script_1E4CD()) {
      (void)((local_FB = -1) & 2);
      if (Script_GetControllerInput(1) == 0x400) {
        local_FC = 1;
      }
    }
  }
  if (local_FB & 8) {
    if (~task_389) {
      (void)((task_389 = -1) & 2);
      Script_sicnUtils(0, 5, 1);
      Script_uiUtils(0, 6, 0);
    }
  } else {
    if (task_389 & 2) {
      (void)((task_389 = 0) & 2);
      Script_sicnUtils(1, 0, 0);
      Script_uiUtils(0, 0, 0);
    }
  }
  return local_FC;
}

__attribute__((scn_string_bindings(0, data_34D60, 2, data_34D73, 3, data_34D83)))
static int32_t script_1E620(void) {
  Script_LoadVoiceFile(1, "E1066");
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 7, "e1gachap.snd", 0x4000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/D000/", "HI_GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_34D8B, 1, data_34D90, 2, data_34D96)))
static void script_1E6B2(void) {
  Script_LoadSND(0, 7, "FREE", 0);
  Script_LoadVoiceFile(0, "E1066");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "HI_GACH");
  return;
}

__attribute__((scn_string_bindings(0, data_34D9E)))
static void script_1E6DF(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Vector3 local_E0;
  Vector3i local_D4;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "HI_GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = 9;
  local_FC = 0;
  while (local_FC < 6) {
    local_F4 = 1 << local_FC;
    if (local_F8 & local_F4) {
      local_F0 = *(&data_34C88 + local_FC);
      local_EC = *(&data_34CA0 + local_FC);
      Script_EVT_GetPosition(local_F0, -1, &local_E0, 0);
      Script_mtwkGetSetRotation(local_F0, &local_D4, 0);
      if (arg0) {
        Script_EVT_SetPosition(local_EC, POS_XYZ, &local_E0);
        Script_EVT_SetRotation(local_EC, 0x38000000, &local_D4, 0);
        Script_charaDisplay(local_EC, CDM_Show);
        Script_charaDisplay(local_F0, CDM_Hide);
      } else {
        Script_charaDisplay(local_F0, CDM_Show);
        Script_charaDisplay(local_EC, CDM_Hide);
      }
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_1E7B1(void) {
  Vector3i local_F4;
  (void)((task_388 = -1) & 8);
  Script_scenCreateTask(script_1E620, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_15);
  Script_sicnUtils(1, 0, 0);
  (void)((task_389 = 0) & 8);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  SCNF_WaitFrames_0000(1);
  Script_uiUtils(0, 6, 0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_34DA8);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_scenToggleVisibleFor(SHEN_NAME('DAMY'), 1);
  Script_charaDisplay(SHEN_NAME('DAMY'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPG'), 1);
  Script_charaDisplay(SHEN_NAME('GCPG'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPB'), 1);
  Script_charaDisplay(SHEN_NAME('GCPB'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPR'), 1);
  Script_charaDisplay(SHEN_NAME('GCPR'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPY'), 1);
  Script_charaDisplay(SHEN_NAME('GCPY'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('GABS'), 1);
  Script_charaDisplay(SHEN_NAME('GABS'), CDM_Hide);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  return;
}

static void script_1E958(void) {
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  script_1E6B2();
  Script_SetGameStateTo(AUTH_None);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_uiUtils(0, 0, 0);
  (void)((task_388 = 0) & 8);
  Script_scenToggleVisibleFor(SHEN_NAME('DAMY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPG'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPB'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GABS'), 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_shenClearFreeState(1);
  (void)((task_388 = 0) & 8);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_1EA32(void) {
  int32_t local_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  script_1F2AC(CHARA_AKIR, 1, 8, 1);
  script_1F2AC(CHARA_AKIR, 0, 8, 1);
  local_F4[0] = local_F4[1] = local_F4[2] = 0;
  local_E8 = 0;
  while (local_E8 < 6) {
    if (task_380 == *(&data_34C88 + local_E8)) {
      local_EC = *((int8_t *)&data_34CD0 + (local_E8 * 20 + 17));
      break;
    }
    local_E8 = local_E8 + 1;
  }
  local_F0 = Script_PlayerStatsUtils(PS_GetIndexedU16Table3Slot, local_EC);
  Script_PlayerStatsUtils(PS_SetIndexedU16Table3Slot, local_EC, local_F0 + 1);
  Script_InitializeOsag(CHARA_AKIR);
  return;
}

static void script_1EAF6(void) {
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_gachaponUtils(2, 0x4849454B);
  return;
}

static void script_1EB25(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  int32_t local_CC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  (&local_F4)->y = (&local_F4)->y + 0.6f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = -0.05f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.65f;
  script_211B9(arg0, &local_F4);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, (&local_D0)->y, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  local_F4.x = -0.0511f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.650771f;
  script_211B9(task_380, &local_F4);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, 0);
  local_DC[0] = (&local_F4)->x - (&local_E8)->x;
  local_DC[1] = 0.0f;
  local_DC[2] = (&local_F4)->z - (&local_E8)->z;
  local_CC = 10;
  while (local_CC) {
    local_E8.x = (&local_E8)->x + *local_DC / 10.0f;
    (&local_E8)->z = (&local_E8)->z + local_DC[2] / 10.0f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_E8);
    local_CC = local_CC - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  return;
}

static void script_1ECAE(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (arg1) {
    local_F4.x = -0.109743f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.860585f;
  } else {
    local_F4.x = -0.0511f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.650771f;
  }
  script_211B9(task_380, &local_F4);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_mtwkGetSetRotation(task_380, &local_E8, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static int32_t script_1ED2F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Vector3i local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  local_DC.x = arg1;
  (&local_DC)->y = arg2;
  (&local_DC)->z = arg3;
  Script_handSetFor(CHARA_AKIR, arg0, &local_F4);
  local_D8 = 0;
  while (local_D8 < arg4) {
    local_E8.x = (&local_F4)->x + (arg1 - (&local_F4)->x) * local_D8 / arg4;
    (&local_E8)->y = (&local_F4)->y + (arg2 - (&local_F4)->y) * local_D8 / arg4;
    (&local_E8)->z = (&local_F4)->z + (arg3 - (&local_F4)->z) * local_D8 / arg4;
    Script_SetHandRootRotation(CHARA_AKIR, arg0, 0x15, &local_E8);
    local_D8 = local_D8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetHandRootRotation(CHARA_AKIR, arg0, 0x15, &local_DC);
  end:;
}

static int32_t script_1EDE9(void) {
  float local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int8_t local_F9;
  int8_t local_F8;
  int8_t local_F7;
  int8_t local_F6;
  (void)((local_FB = -((local_FA = -((local_F9 = 0) != 0)) != 0)) & 1);
  (void)((local_F8 = -((local_F7 = 0) != 0)) & 0x10);
  (void)((local_F6 = 0) & 1);
  for (;;) {
    local_FC = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR);
    if (-!(local_FC < 100.0f) & ~local_FB) {
      (void)((local_FB = -1) & 2);
      Script_scenCreateTask(script_1EF24, 1, local_FC - 100.0f);
    }
    if (-!(local_FC < 170.0f) & ~local_FA) {
      (void)((local_FA = -1) & 2);
      Script_scenCreateTask(script_1EFF4, 0);
    }
    if (-!(local_FC < 215.0f) & ~local_F9) {
      (void)((local_F9 = -1) & 2);
      Script_scenCreateTask(script_1F0BD, 0);
    }
    if (-!(local_FC < 280.0f) & ~local_F8) {
      (void)((local_F8 = -1) & 2);
      Script_scenCreateTask(script_1F167, 0);
    }
    if (-!(local_FC < 300.0f) & ~local_F7) {
      (void)((local_F7 = -1) & 2);
      Script_scenCreateTask(script_1F1B1, 0);
    }
    if (-!(local_FC < 310.0f) & ~local_F6) {
      (void)((local_F6 = -1) & 2);
      Script_scenCreateTask(script_1F49D, 0);
    }
    if (local_F6 & 0x40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_1EF24(float arg0) {
  int32_t local_F4[3];
  int32_t local_F0;
  if (!(40.0f - arg0 <= 0.0f)) {
    local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, 40.0f - arg0);
    script_1F2AC(CHARA_AKIR, 1, 0, local_F0);
    local_F4[0] = 0;
    local_F4[1] = -5643;
    local_F4[2] = 728;
    script_1ED2F(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], local_F0);
  }
  script_1F2AC(CHARA_AKIR, 1, 1, 16);
  local_F4[1] = -4915;
  script_1ED2F(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  local_F4[1] = -5643;
  script_1ED2F(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  Script_scenCreateTask(script_200F6, 0);
  end:;
}

static int32_t script_1EFF4(void) {
  int32_t local_F4[3];
  script_1F2AC(CHARA_AKIR, 1, 2, 40);
  local_F4[0] = -7827;
  local_F4[1] = 4187;
  local_F4[2] = 4187;
  script_1ED2F(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  local_F4[0] = -7827;
  local_F4[1] = 3458;
  local_F4[2] = 4187;
  script_1ED2F(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  Script_scenCreateTask(script_1F9AB, 1, 20);
  local_F4[0] = 6735;
  local_F4[1] = 2821;
  local_F4[2] = 1456;
  script_1ED2F(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 16);
  SCNF_WaitFrames_0000(4);
  end:;
}

static int32_t script_1F0BD(void) {
  int32_t local_F4[3];
  script_1F2AC(CHARA_AKIR, 1, 3, 40);
  local_F4[0] = -7827;
  local_F4[1] = 3458;
  local_F4[2] = 4187;
  script_1ED2F(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 24);
  Script_scenCreateTask(script_1F9AB, 1, 14);
  local_F4[0] = 6735;
  local_F4[1] = 2821;
  local_F4[2] = 1456;
  script_1ED2F(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 16);
  Script_scenCreateTask(script_201B6, 0);
  SCNF_WaitFrames_0000(4);
  Script_scenCreateTask(script_204D5, 0);
  end:;
}

static int32_t script_1F167(void) {
  int32_t local_F4[3];
  script_1F2AC(CHARA_AKIR, 1, 4, 20);
  local_F4[0] = 2548;
  local_F4[1] = -3822;
  local_F4[2] = -2184;
  script_1ED2F(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 20);
  end:;
}

static int32_t script_1F1B1(void) {
  int32_t local_F4[3];
  script_1F2AC(CHARA_AKIR, 1, 5, 20);
  local_F4[0] = -364;
  local_F4[1] = -2730;
  local_F4[2] = -182;
  script_1ED2F(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 20);
  local_F4[0] = -546;
  local_F4[1] = -1274;
  local_F4[2] = 2730;
  Script_scenCreateTask(script_1ED2F, 5, 1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 40);
  local_F4[0] = -7645;
  local_F4[1] = 3822;
  local_F4[2] = 910;
  local_F4[0] = -2419;
  local_F4[1] = 4176;
  local_F4[2] = 5642;
  Script_scenCreateTask(script_1ED2F, 5, 0, *(int32_t *)local_F4, local_F4[1], local_F4[2], 30);
  script_1F2AC(CHARA_AKIR, 1, 6, 10);
  script_1F2AC(CHARA_AKIR, 0, 6, 10);
  end:;
}

static void script_1F2AC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_FF20[56];
  int32_t local_FF1C;
  int32_t local_FF18;
  int32_t local_FF14;
  local_FF18 = -1;
  local_FF14 = arg2;
  switch (local_FF14) {
    case 0:
      local_FF18 = (int32_t)data_34DB8;
      break;
    case 1:
      local_FF18 = (int32_t)data_34E9C;
      break;
    case 2:
      local_FF18 = (int32_t)data_34F80;
      break;
    case 3:
      local_FF18 = (int32_t)data_35064;
      break;
    case 4:
      local_FF18 = (int32_t)data_35148;
      break;
    case 5:
      local_FF18 = (int32_t)data_3522C;
      break;
    case 6:
      local_FF18 = (int32_t)data_35310;
      break;
    case 7:
      local_FF18 = (int32_t)data_353F4;
      break;
    case 8:
      local_FF18 = (int32_t)data_354D8;
      break;
    case 9:
      {
        int32_t local_FF10;
        local_FF10 = 0;
        while (local_FF10 < 57) {
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_3522C + (local_FF10 << 2));
          local_FF10 = local_FF10 + 1;
        }
        *(&local_FF1C + 3) = -5643;
        *(&local_FF1C + 4) = 0x2000;
        *(&local_FF1C + 14) = -6917;
        *(&local_FF1C + 17) = -4551;
        local_FF18 = (int32_t)&local_FF1C;
        break;
      }
    case 10:
      {
        int32_t local_FF10;
        local_FF10 = 0;
        while (local_FF10 < 57) {
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_3522C + (local_FF10 << 2));
          local_FF10 = local_FF10 + 1;
        }
        *(&local_FF1C + 3) = -5643;
        *(&local_FF1C + 4) = 1820;
        *(&local_FF1C + 14) = -6917;
        *(&local_FF1C + 17) = -13107;
        local_FF18 = (int32_t)&local_FF1C;
        break;
      }
  }
  if ((local_FF18 != -1) & (local_FF18 != 0)) {
    Script_handSetFloatsFor(arg0, arg1, (const int32_t *)local_FF18, arg3);
  }
  return;
}

static int32_t script_1F49D(void) {
  Script_seqEnterEvtCam(7008, task_380, task_380);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.126f;
    (&local_F4)->y = 0.035f;
    (&local_F4)->z = -0.056f;
    local_E8.x = 0x4000;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_fixoAttachEx(SHEN_NAME('GABS'), CHARA_AKIR, 18, &local_F4, &local_E8);
    Script_fixoAttachEx(task_384, CHARA_AKIR, 18, &local_F4, &local_E8);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 340.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.0f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.0f;
    local_E8.x = 0x4000;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0x4000;
    Script_fixoAttachEx(SHEN_NAME('KEIH'), task_384, 18, &local_F4, &local_E8);
  }
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Show);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 360.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(132265, 0, 0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.09f;
    (&local_F4)->y = 0.075f;
    (&local_F4)->z = 0.008f;
    local_E8.x = 13111;
    (&local_E8)->y = 817;
    (&local_E8)->z = 50684;
    Script_fixoAttachEx(task_384, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 410.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('GABS'));
  Script_scenCreateTask(script_1F92B, 1, SHEN_NAME('GABS'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 430.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_1F2AC(CHARA_AKIR, 1, 5, 20);
  {
    int32_t unused_F4[3];
    int32_t local_E8[3];
    local_E8[0] = 909;
    local_E8[1] = -1657;
    local_E8[2] = 1431;
    Script_scenCreateTask(script_1ED2F, 5, 1, *(int32_t *)local_E8, local_E8[1], local_E8[2], 20);
  }
  {
    int32_t unused_F4[3];
    int32_t local_E8[3];
    local_E8[0] = -38;
    local_E8[1] = 476;
    local_E8[2] = 9888;
    Script_scenCreateTask(script_1ED2F, 5, 0, *(int32_t *)local_E8, local_E8[1], local_E8[2], 20);
  }
  Script_scenCreateTask(script_1F839, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 450.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(7009, task_380, task_380);
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.11f;
    (&local_F4)->y = 0.055f;
    (&local_F4)->z = -0.065f;
    local_E8.x = 29664;
    (&local_E8)->y = 55344;
    (&local_E8)->z = 35490;
    Script_fixoAttachEx(SHEN_NAME('KEIH'), CHARA_AKIR, 18, &local_F4, &local_E8);
  }
  if (task_38B & 0x40) {
    int32_t unused_E8[6];
    Vector4 local_D8;
    local_D8.x = (&local_D8)->y = (&local_D8)->z = (&local_D8)->w = 0.35f;
    Script_charaSetRefboardFlagTo(SHEN_NAME('KEIH'), 1);
    Script_SetCharaRefBoardPos(SHEN_NAME('KEIH'), &local_D8);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 503.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(task_384);
  Script_scenCreateTask(script_1F92B, 1, task_384);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  end:;
}

static int32_t script_1F839(void) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F4.x = *(float *)&data_355C8;
  (&local_F4)->y = *(float *)(&data_355C8 + 1);
  (&local_F4)->z = *(float *)(&data_355C8 + 2);
  local_F0 = 20;
  while (local_F0) {
    local_F4.x = (&local_F4)->x + ((&data_355BC)->x - *(float *)&data_355C8) / 20.0f;
    (&local_F4)->y = (&local_F4)->y + ((&data_355BC)->y - *(float *)(&data_355C8 + 1)) / 20.0f;
    (&local_F4)->z = (&local_F4)->z + ((&data_355BC)->z - *(float *)(&data_355C8 + 2)) / 20.0f;
    Script_fixoClearAttachment(task_384);
    Script_fixoAttachEx(task_384, CHARA_AKIR, 12, &local_F4, &data_355D4);
    local_F0 = local_F0 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoAttachEx(task_384, CHARA_AKIR, 12, &data_355BC, &data_355D4);
  return Script_srchControl(0);
}

static int32_t script_1F92B(int32_t arg0) {
  Vector3 local_F4;
  float local_F0;
  float local_EC;
  int32_t unused_E8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  local_EC = 0.0108888894f;
  local_F0 = 0.0f;
  for (;;) {
    if ((&local_F4)->y <= 0.0f) {
      break;
    }
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    local_F0 = local_F0 + local_EC;
    (&local_F4)->y = (&local_F4)->y - local_F0;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(328873, 0, 0);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_scenToggleVisibleFor(arg0, 0);
  return Script_srchControl(0);
}

static int32_t script_1F9AB(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  {
    Vector3i local_EC;
    Vector3 local_E0;
    float local_DC;
    local_EC.x = 0;
    (&local_EC)->y = 0;
    (&local_EC)->z = -0x8000 / arg0;
    local_FC = 0;
    while (local_FC < 6) {
      if (task_380 == *(&data_34C88 + local_FC)) {
        local_F8 = *(&data_34CA0 + local_FC);
        break;
      }
      local_FC = local_FC + 1;
    }
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, CHARA_PART_Get);
    local_DC = (&local_E0)->y;
    local_FC = 0;
    L01FA2B:;
    if (local_FC == arg0 * 6 / 10) {
      Script_executeAicaCommand(66729, 0, 0);
    }
    Script_charaPosRotUtils(local_F8, 152, 0, &local_EC, CHARA_PART_Add);
    (&local_E0)->y = (&local_E0)->y + (Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.003f) - 0.0015f);
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, CHARA_PART_Set);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (!(local_FC >= arg0)) goto L01FA2B;
    (&local_E0)->y = local_DC;
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, CHARA_PART_Set);
  }
  {
    Vector3i local_EC;
    local_EC.x = (&local_EC)->y = (&local_EC)->z = 0;
    Script_charaPosRotUtils(local_F8, 152, 0, &local_EC, CHARA_PART_Set);
  }
  end:;
}

static void script_1FADD(int32_t arg0) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_D0[3];
  Vector3i local_C4;
  Script_MoveObjectToObject(SHEN_NAME('KEIH'), task_380);
  (&local_E8)->y = (&local_E8)->z = 0;
  local_E8.x = 0;
  Script_EVT_SetRotation(SHEN_NAME('KEIH'), 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(SHEN_NAME('KEIH'), -1);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4849454B);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_355E0);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_355EC, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_gachaponUtils(3, 0x4849454B, &data_35610);
  SCNF_WaitFrames_0000(1);
  local_E0 = Script_scenCreateTask(script_1FD49, 0);
  Script_RotateAllAxisWrapper(SHEN_NAME('YKUR'), 1, &local_E8);
  Script_sicnUtils(0, 5, 1);
  Script_shenClearFreeState(1);
  local_E4 = 0;
  while (1) {
    local_DC = Script_GetControllerInput(0);
    if (local_DC & 0x6000) {
      if (local_E4 > -20) {
        local_E4 = local_E4 - 1;
        local_E8.x = (&local_E8)->x - 546;
      }
    } else {
      if (local_DC & 0x9000) {
        if (local_E4 < 20) {
          local_E4 = local_E4 + 1;
          local_E8.x = (&local_E8)->x + 546;
        }
        goto L01FC2A;
      }
    }
    L01FC2A:;
    Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &local_E8);
    Script_fixoClearAttachment(SHEN_NAME('KEIH'));
    Script_fixoSetAttachmentOffset(SHEN_NAME('KEIH'), 0, &data_355E0, &data_355EC);
    local_C4.x = 0;
    (&local_C4)->y = -(&local_E8)->x;
    (&local_C4)->z = 0;
    Script_fixoAttachEx(SHEN_NAME('KEIH'), SHEN_NAME('YKUR'), 18, &data_355F8, &local_C4);
    SCNF_WaitFrames_0000(1);
    if (local_DC & 0x400) {
      if (~task_38A) {
        break;
      }
    }
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
      break;
    }
  }
  Script_sicnUtils(1, 0, 0);
  Script_shenClearFreeState(0);
  Script_gachaponUtils(4, 0x4849454B);
  Script_srchControl((void *)local_E0);
  script_20B0F(arg0);
  Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &data_35604);
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Hide);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 0, 1, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  return;
}

static int32_t script_1FD49(void) {
  Vector3 local_F4;
  int32_t local_E8[3];
  Vector3 local_DC;
  Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
  local_E8[0] = local_E8[1] = local_E8[2] = 0;
  for (;;) {
    local_E8[0] = *(int32_t *)local_E8 + 364;
    local_E8[1] = local_E8[1] + 546;
    local_E8[2] = local_E8[2] + 728;
    local_DC.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, *(int32_t *)local_E8 & 0xFFFF) * 0.005f;
    (&local_DC)->y = (&local_F4)->y + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1] & 0xFFFF) * 0.005f;
    (&local_DC)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[2] & 0xFFFF) * 0.005f;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_DC);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1FE21(void) {
  return;
}

__attribute__((scn_string_bindings(5, data_3564E)))
static int32_t script_1FE24(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  while (task_38A & 0x20) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_38A = -1) & 0x10);
  local_F8 = arg0;
  switch (local_F8) {
    case 0:
      local_FC = script_21167(3, "E1066A001", "E1066A002", "E1066A003");
      break;
    case 1:
      local_FC = script_21167(2, "E1066A004", "E1066A005");
      break;
    case 2:
      local_FC = script_21167(3, "E1066A006", "E1066A007", "E1066A008");
      break;
    case 3:
      local_FC = script_21167(3, "E1066A009", "E1066A010", "E1066A011");
      break;
    case 4:
      local_FC = script_21167(2, "E1066A026", "E1066A027");
      break;
  }
  if (local_FC) {
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)((task_38A = 0) & 4);
  return Script_srchControl(0);
}

static int32_t script_1FF3A(int16_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  while (task_38A & 0x20) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_38A = -1) & 4);
  local_F8 = task_37C;
  local_F4 = script_1DC02(local_F8, arg0);
  local_EC = script_1DE3C(local_F8, arg0);
  local_F0 = Script_GetOwnedItemCount(local_F4, 0);
  local_FC = 0;
  if (local_F0) {
    local_FC = script_21167(3, "E1066A019", "E1066A020", "E1066A021");
  } else {
    local_E8 = local_EC;
    if (local_E8 == 0) {
      local_FC = script_21167(5, "E1066A014", "E1066A015", "E1066A016", "E1066A017", "E1066A018");
    } else if (local_E8 == 1) {
      local_FC = script_21167(3, "E1066A022", "E1066A025", "E1066A024");
    } else {
      if ((local_E8 == 2) | (local_E8 == 3)) {
        local_FC = Script_ShowUIMessageFromID("E1066A023");
        goto L02003D;
      }
    }
  }
  L02003D:;
  if (local_FC) {
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return (task_38A = 0) & 0x10;
}

static int32_t script_20063(void) {
  int32_t local_FC;
  local_FC = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(-100, 1);
  Script_SetStats(STATS_CASH, local_FC - 100);
  return Script_srchControl(0);
}

static void script_2008C(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.132f;
  (&local_F4)->y = 0.041f;
  (&local_F4)->z = 0.018f;
  local_E8.x = 19114;
  (&local_E8)->y = 14563;
  (&local_E8)->z = 0;
  Script_scenToggleVisibleFor(0x6E496F63, 1);
  Script_charaDisplay(0x6E496F63, CDM_Show);
  Script_fixoAttachEx(0x6E496F63, CHARA_AKIR, 18, &local_F4, &local_E8);
  return;
}

static int32_t script_200F6(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  float local_E4;
  float local_E0;
  Script_fixoClearAttachment(0x6E496F63);
  Script_MoveObjectToObject(0x6E496F63, task_380);
  Script_EVT_GetPosition(0x6E496F63, -1, &local_F4, 0);
  local_E4 = (&local_F4)->y - 0.05f;
  local_E0 = 0.002f;
  for (;;) {
    if ((&local_F4)->y < local_E4) {
      break;
    }
    local_F4.x = (&local_F4)->x + 0.001f;
    (&local_F4)->y = (&local_F4)->y - local_E0;
    local_F4.x = (&local_F4)->x + 0.002f;
    local_E0 = local_E0 + 0.002f;
    Script_EVT_SetPosition(0x6E496F63, POS_XYZ, &local_F4);
    SCNF_WaitFrames_0000(1);
  }
  Script_scenToggleVisibleFor(0x6E496F63, 0);
  Script_charaDisplay(0x6E496F63, CDM_Hide);
  Script_executeAicaCommand(1193, 0, 0);
  end:;
}

static int32_t script_201B6(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t unused_CC[3];
  float local_C8;
  int32_t unused_C4;
  float local_C0;
  int32_t local_BC;
  int32_t local_B8;
  Script_executeAicaCommand(197801, 0, 0);
  local_D8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 4);
  local_B8 = local_D8;
  if (local_B8 == 0) {
    task_384 = 0x47504347;
  } else if (local_B8 == 1) {
    task_384 = 0x42504347;
  } else if (local_B8 == 2) {
    task_384 = 0x52504347;
  } else {
    task_384 = 0x59504347;
  }
  Script_EVT_GetPosition(task_380, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(task_380, &local_E8, 0);
  local_F4.x = (&local_F4)->x + (Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.2f + ((float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y)) * -0.1f);
  (&local_F4)->y = (&local_F4)->y + 0.5f;
  (&local_F4)->z = (&local_F4)->z + (Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.2f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * -0.1f);
  Script_EVT_SetPosition(task_384, POS_XYZ, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('GABS'), POS_XYZ, &local_F4);
  Script_charaDisplay(task_384, CDM_Show);
  Script_charaDisplay(SHEN_NAME('GABS'), CDM_Show);
  local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
  local_DC.x = 728;
  (&local_DC)->y = 546;
  (&local_DC)->y = 364;
  local_C8 = (&local_F4)->y - 0.08f;
  local_C0 = 0.001f;
  for (;;) {
    if (local_C8 > (&local_F4)->y) {
      break;
    }
    (&local_F4)->y = (&local_F4)->y - local_C0;
    local_C0 = local_C0 + 0.001f;
    local_E8.x = (&local_E8)->x + (&local_DC)->x;
    (&local_E8)->y = (&local_E8)->y + (&local_DC)->y;
    (&local_E8)->z = (&local_E8)->z + (&local_DC)->z;
    Script_EVT_SetPosition(task_384, POS_XYZ, &local_F4);
    Script_EVT_SetPosition(SHEN_NAME('GABS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_384, 0x38000000, &local_E8, 0);
    Script_EVT_SetRotation(SHEN_NAME('GABS'), 0x38000000, &local_E8, 0);
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 910;
  (&local_E8)->y = 1274;
  (&local_E8)->z = 0x8000;
  Script_mtwkGetSetRotation(task_384, &local_DC, 0);
  *(float *)&local_DC.x = (float)((&local_E8)->x - (&local_DC)->x) / 10.0f;
  *(float *)&(&local_DC)->y = (float)((&local_E8)->y - (&local_DC)->y) / 10.0f;
  *(float *)&(&local_DC)->z = (float)((&local_E8)->z - (&local_DC)->z) / 10.0f;
  local_BC = 10;
  while (local_BC) {
    local_E8.x = (&local_E8)->x + (&local_DC)->x;
    (&local_E8)->y = (&local_E8)->y + (&local_DC)->y;
    (&local_E8)->z = (&local_E8)->z + (&local_DC)->z;
    script_211F1(task_380, &local_E8);
    Script_EVT_SetRotation(task_384, 0x38000000, &local_E8, 0);
    Script_EVT_SetRotation(SHEN_NAME('GABS'), 0x38000000, &local_E8, 0);
    local_BC = local_BC - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(263337, 0, 0);
  end:;
}

static int32_t script_204D5(void) {
  Vector3 local_F4;
  Script_MoveObjectToObject(CHARA_AKIR, task_380);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  {
    int32_t local_F0;
    local_F0 = 15;
    while (local_F0) {
      (&local_F4)->z = (&local_F4)->z + 0.002f;
      Script_MoveObjectToObject(CHARA_AKIR, task_380);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_MoveObjectToObject(CHARA_AKIR, -1);
      local_F0 = local_F0 - 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  {
    int32_t local_F0;
    local_F0 = 15;
    while (local_F0) {
      (&local_F4)->z = (&local_F4)->z - 0.002f;
      Script_MoveObjectToObject(CHARA_AKIR, task_380);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_MoveObjectToObject(CHARA_AKIR, -1);
      local_F0 = local_F0 - 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static void script_205B2(void) {
  Script_SetMotnFlags(CHARA_AKIR, 17, &data_35718, 0x39000000);
  return;
}

static int32_t script_205CA(void) {
  for (;;) {
    script_205B2();
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_205DD(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 1, 1, 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, 0);
  script_1F2AC(SHEN_NAME('YKUR'), 1, 9, 1);
  SCNF_WaitFrames_0000(1);
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, SHEN_NAME('KEIH'));
  script_1F2AC(SHEN_NAME('YKUR'), 1, 10, 1);
  L020651:;
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4849454B);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_35724);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_35730, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  SCNF_WaitFrames_0000(1);
  if (!Script_GetHandControlBit3(SHEN_NAME('YKUR'), 1)) goto L020651;
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, 0);
  end:;
}

static void script_206B8(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  local_EC = 0;
  local_F0 = task_380;
  script_1E7B1();
  Script_sicnUtils(0, 1, 1);
  for (;;) {
    if (script_20793(local_F0, local_EC)) {
      Script_shenClearFreeState(0);
      Script_sicnUtils(2, 1, 0);
      script_209DF(local_F0, local_EC);
    } else {
      Script_shenClearFreeState(0);
      Script_sicnUtils(2, 1, 0);
      if (local_EC) {
        script_1E6DF(0);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
        Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetGameStateTo(AUTH_None);
        SCNF_WaitFrames_0000(30);
      }
      L02076E:;
      SCNF_WaitFrames_0000(1);
      if (!~task_38A) goto L02076E;
      break;
    }
    local_EC = local_EC + 1;
  }
  script_1E958();
  return;
}

static int32_t script_20793(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  int32_t local_E8;
  int32_t local_E4;
  local_F8 = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    (void)0;
  } else {
    script_1ECAE(arg0, arg1);
    Script_EVT_GetPosition(arg0, -1, &local_EC, 0);
    (&local_EC)->y = (&local_EC)->y + 0.6f;
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 0);
    if (arg1 == 0) {
      Script_scenCreateTask(script_1FE24, 1, 0);
      Script_seqEnterEvtCam(7000, arg0, arg0);
    } else {
      local_F8 = Script_scenCreateTask(script_205CA, 0);
      Script_scenCreateTask(script_1FE24, 1, 4);
      Script_seqEnterEvtCam(7010, arg0, arg0);
    }
    L020837:;
    SCNF_WaitFrames_0000(1);
    if (!~task_38A) goto L020837;
    if (arg1 == 0) {
      local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 40, 41, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    } else {
      local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 4, 34, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    }
    Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
    if ((local_FC == 40) | (local_FC == 4)) {
      local_E8 = script_3C48(0, 100, 1);
      local_E4 = local_E8;
      if (local_E4 == 0) {
        if (arg1 == 0) {
          Script_scenCreateTask(script_1FE24, 1, 1);
        }
        local_FC = 1;
      } else if (local_E4 == 1) {
        Script_scenCreateTask(script_1FE24, 1, 2);
        SCNF_SetCashValue(0, 1);
        local_FC = 0;
      } else {
        local_FC = 0;
      }
    } else {
      Script_scenCreateTask(script_1FE24, 1, 3);
      local_FC = 0;
    }
    if (local_F8) {
      Script_srchControl((void *)local_F8);
    }
    (void)local_FC;
  }
  end:;
}

static void script_2094C(int32_t arg0) {
  int32_t local_FC;
  if (arg0 == 0) {
    local_FC = -0x40800000;
  } else {
    local_FC = 0x42FA0000;
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  }
  script_1F2AC(CHARA_AKIR, 1, 7, 1);
  script_1F2AC(CHARA_AKIR, 0, 8, 1);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, -1.0f, -1.0f, local_FC, 0.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.25f);
  script_2008C();
  Script_scenCreateTask(script_1EDE9, 0);
  return;
}

static void script_209DF(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_shenClearFreeState(0);
  script_1EA32();
  local_FC = script_20C25();
  if (arg1 == 0) {
    Script_seqEnterEvtCam(7006, arg0, arg0);
    script_1E6DF(1);
  } else {
    Script_seqEnterEvtCam(7011, arg0, arg0);
  }
  script_2094C(arg1);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_20063, 0);
  Script_scenCreateTask(script_205DD, 0);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_1FF3A, 1, local_FC);
  Script_shenClearFreeState(1);
  script_1FADD(local_FC);
  script_1EAF6();
  script_20A95();
  return;
}

static void script_20A95(void) {
  Vector3 local_F4;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, -1.0f, -1.0f, 135.0f, 0.0f, 0);
  script_205B2();
  Script_EVT_GetPosition(task_380, -1, &local_F4, 0);
  (&local_F4)->y = (&local_F4)->y + 0.6f;
  Script_seqEnterEvtCam(7005, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  return;
}

static void script_20B0F(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  int32_t unused_CC;
  int32_t local_C8;
  int32_t local_C4;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_D0, 0x40000000);
  Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  local_DC[0] = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D0)->y) * 0.3f / 30.0f;
  local_DC[1] = 0.0f;
  local_DC[2] = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D0)->y) * 0.3f / 30.0f;
  local_C4 = 0;
  while (local_C4 < 30) {
    local_F4.x = (&local_E8)->x + *local_DC * (float)local_C4;
    (&local_F4)->y = (&local_E8)->y + local_DC[1] * (float)local_C4;
    (&local_F4)->z = (&local_E8)->z + local_DC[2] * (float)local_C4;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), 0x78000000, &local_F4);
    local_C4 = local_C4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  local_C8 = script_1DC02(-1, arg0);
  Script_ProcessSubtitle("JINGURU01", 0);
  Script_GetCassetteFlags(local_C8, 1, 0, 0);
  SCNF_StartIconSystemMenu(local_C8);
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  return;
}

static int32_t script_20C25(void) {
  float local_FC;
  float local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t unused_E8;
  GameDateTime local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  local_F0 = Script_SetOrGetWOCCValue(task_380, -1);
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
  local_EC = 2;
  if (!(local_FC >= 0.333333313f)) {
    local_EC = 0;
    if (Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3) == 0) {
      local_EC = 1;
    }
  } else {
    local_FC = local_FC - 0.333333313f;
    if (!(local_FC >= 0.05f)) {
      if (data_33FCC[local_F0] != 0) {
        local_EC = 3;
      }
    } else {
      local_FC = local_FC - 0.05f;
      if (!(local_FC >= 0.0f)) {
        local_EC = 5;
      } else {
        (void)local_FC;
        if (!(local_FC >= 0.0f)) {
          local_EC = 6;
        } else {
          (void)local_FC;
          if (!(local_FC >= 0.0f)) {
            local_EC = 7;
          } else {
            (void)local_FC;
            if (!(local_FC >= 0.0f)) {
              local_EC = 8;
            } else {
              (void)local_FC;
              if (!(local_FC >= 0.0f)) {
                local_EC = 9;
              }
            }
          }
        }
      }
    }
  }
  if (-(local_EC == 5) & (-(local_F0 == 1) | -(local_F0 == 2))) {
    local_EC = 2;
  }
  if ((-(local_EC == 7) | -(local_EC == 6)) & -(local_F0 != 2)) {
    local_EC = 2;
  }
  if ((-(local_EC == 9) | -(local_EC == 8)) & -(local_F0 != 1)) {
    local_EC = 2;
  }
  local_DC = local_EC;
  switch (local_DC) {
    case 0:
      local_F0 = 0;
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_33FBC[local_F0]);
      break;
    case 1:
      local_F0 = 14;
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_33FBC[local_F0]);
      break;
    case 2:
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_33FBC[local_F0] - data_33FCC[local_F0]);
      break;
    case 3:
      local_D8 = data_33FBC[local_F0] - data_33FCC[local_F0] - 1;
      local_D4 = 0;
      while (local_D4 < data_33FCC[local_F0]) {
        if (Script_GetOwnedItemCount(script_1DC02(local_F0, local_D8 + local_D4), 0) == 0) {
          if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.666666687f)) {
            break;
          }
        }
        local_D4 = local_D4 + 1;
      }
      if (local_D4 < data_33FCC[local_F0]) {
        local_F4 = data_33FBC[local_F0] - local_D4 - 1;
      } else {
        local_D8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_33FCC[local_F0]) + 1;
        local_F4 = data_33FBC[local_F0] - local_D8;
      }
      break;
    case 4:
      local_F0 = 15;
      local_D8 = 0;
      while (local_D8 < data_33FBC[local_F0]) {
        if (Script_GetOwnedItemCount(script_1DC02(local_F0, local_D8), 0) == 0) {
          if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.333333343f)) {
            break;
          }
        }
        local_D8 = local_D8 + 1;
      }
      if (local_D8 < data_33FBC[local_F0]) {
        local_F4 = local_D8;
      } else {
        local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_33FBC[local_F0]);
      }
      break;
    case 5:
      local_DC = local_F0;
      switch (local_DC) {
        case 3:
          local_F0 = 8;
          local_DC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 2);
          switch (local_DC) {
            case 0:
              local_F4 = 1;
              break;
            case 1:
              local_F4 = 0;
              break;
          }
          break;
        case 4:
          local_F0 = 7;
          local_DC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 5);
          switch (local_DC) {
            case 0:
              local_F4 = 2;
              break;
            case 1:
              local_F4 = 3;
              break;
            case 2:
              local_F4 = 4;
              break;
            case 3:
              local_F4 = 5;
              break;
            case 4:
              local_F0 = 8;
              local_F4 = 2;
              break;
          }
          break;
        case 5:
          local_F0 = 8;
          local_F4 = 3;
          break;
        case 6:
          local_F0 = 9;
          local_DC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 4);
          switch (local_DC) {
            case 0:
              local_F4 = 0;
              break;
            case 1:
              local_F4 = 1;
              break;
            case 2:
              local_F4 = 2;
              break;
            case 3:
              local_F4 = 3;
              break;
          }
          break;
      }
      break;
    case 6:
      local_F0 = 7;
      local_F4 = 1;
      break;
    case 7:
      local_F0 = 7;
      local_F4 = 0;
      break;
    case 8:
      local_F0 = 7;
      local_F4 = 6;
      break;
    case 9:
      local_F0 = 7;
      local_F4 = 7;
      break;
  }
  local_F8 = 1.0f;
  Script_ReturnTimeDate(&local_E0);
  if ((&local_E0)->hours >= 17) {
    local_F8 = -1.0f;
  } else {
    local_F8 = 1.0f;
  }
  if (!(local_F8 >= 0.0f)) {
    (void)((task_38B = -1) & 2);
  } else {
    (void)((task_38B = 0) & 2);
  }
  script_1DE8F(SHEN_NAME('KEIH'), local_F0, local_F4, local_F8);
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Hide);
  return local_F4;
}

static int32_t script_21167(int32_t arg0, const char *arg1, const char *arg2, ...) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, arg0);
  local_F4 = local_FC;
  if (local_F4 == 1) {
    local_F8 = Script_ShowUIMessageFromID(arg2);
  } else if (local_F4 == 2) {
    {
      va_list arguments0;
      va_start(arguments0, arg2);
      local_F8 = Script_ShowUIMessageFromID(va_arg(arguments0, const char *));
      va_end(arguments0);
    }
  } else {
    local_F8 = Script_ShowUIMessageFromID(arg1);
  }
  return local_F8;
}

static void script_211B9(int32_t arg0, Vector3 *arg1) {
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), arg0);
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, arg1);
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), -1);
  Script_EVT_GetPosition(SHEN_NAME('DAMY'), -1, arg1, 0);
  return;
}

static void script_211F1(int32_t arg0, Vector3i *arg1) {
  int32_t unused_F4[3];
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), arg0);
  Script_EVT_SetRotation(SHEN_NAME('DAMY'), 0x38000000, arg1, 0);
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), -1);
  Script_mtwkGetSetRotation(SHEN_NAME('DAMY'), arg1, 0);
  return;
}

static void script_21229(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 733) == 0)) {
    DSWK_AssignDoorEntryConfig(26, 1, -1, -1, -1, -1, -1, -1);
  }
  return;
}

static void script_2126A(void) {
  if (-(task_F8 == 301) & -(task_C8.hours < 19) & -(task_C8.hours >= 17) & -(Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_FIELD_B8_BY_ID, 0x49524948) == 3)) {
    DSWK_AssignDoorEntryConfig(0, 1, -1, -1, -1, -1, -1, -1);
  } else {
    DSWK_AssignDoorEntryConfig(0, 2, 2, -1, -1, -1, -1, -1);
  }
  return;
}

static void script_212CA(void) {
  if (-(Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_FIELD_B8_BY_ID, 0x4F544148) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_HATO_STOPPER_CLEARED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE) == 0) & -(task_C8.hours >= 19)) {
    DSWK_AssignDoorEntryConfig(30, 1, -1, -1, -1, -1, -1, -1);
  } else if (-(Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_FIELD_B8_BY_ID, 0x4F544148) == 0) & -(script_76B7(12) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_CHALLENGE_COMPLETE)) {
    DSWK_AssignDoorEntryConfig(30, 1, -1, -1, -1, -1, -1, -1);
  } else {
    DSWK_AssignDoorEntryConfig(30, 3, 2, -1, 0x2302AB, -1, -1, -1);
  }
  return;
}

static void script_21375(void) {
  if (-(task_F8 >= 501) & -(task_F8 <= 502) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_INITIAL_VISIT_COMPLETE) == 0)) {
    DSWK_AssignDoorEntryConfig(46, 1, -1, -1, -1, -1, -1, -1);
  } else {
    DSWK_AssignDoorEntryConfig(46, 3, 4, -1, 0x2302AB, -1, -1, -1);
  }
  return;
}

static void script_213C6(void) {
  script_21229();
  script_2126A();
  script_212CA();
  script_21375();
  DSWK_AssignDoorEntryConfig(28, 5, 4, -1, 0x2402AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(59, 3, 4, -1, 0x2302AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(1, 1, -1, -1, -1, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(53, 4, 4, -1, 0x2402AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(49, 3, 2, -1, 0x2302AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(50, 5, 2, -1, 0x2402AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(34, 3, 2, -1, 0x2302AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(56, 5, 4, -1, 0x2402AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(55, 5, 4, -1, 0x2402AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(58, 4, 4, -1, 0x2402AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(60, 4, 4, -1, 0x2402AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(35, 5, 4, -1, 0x2402AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(39, 3, 2, -1, 0x2302AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(54, 3, 2, -1, 0x2302AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(63, 3, 4, -1, 0x2302AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(61, 1, -1, -1, -1, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(51, 1, -1, -1, -1, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(45, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(47, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(44, 7, 1, 6, 0x2102AB, 0x2902AB, 0x2802AB, 0x594B554A);
  DSWK_AssignDoorEntryConfig(15, 7, 1, 5, 0x2102AB, 0x2902AB, 0x2802AB, 0x49525559);
  DSWK_AssignDoorEntryConfig(13, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(11, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(16, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(14, 7, 1, 2, 0x2102AB, 0x2902AB, 0x2802AB, 0x524F4F44);
  DSWK_AssignDoorEntryConfig(12, 7, 1, 1, 0x2102AB, 0x2902AB, 0x2802AB, 0x524F4F44);
  DSWK_AssignDoorEntryConfig(3, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(6, 7, 1, 1, 0x2102AB, 0x2902AB, 0x2802AB, 0x4F544148);
  DSWK_AssignDoorEntryConfig(4, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(5, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(10, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(7, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(9, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(8, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(21, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(18, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(23, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(19, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(22, 7, 1, 0, 0x2102AB, 0x2902AB, 0x2802AB, 0x4E4D4F4B);
  DSWK_AssignDoorEntryConfig(17, 7, 1, 0, 0x2102AB, 0x2902AB, 0x2802AB, 0x4F544D4E);
  DSWK_AssignDoorEntryConfig(24, 7, 1, 0, 0x2102AB, 0x2902AB, 0x2802AB, 0x4F4A4153);
  DSWK_AssignDoorEntryConfig(20, 7, 1, 1, 0x2102AB, 0x2902AB, 0x2802AB, 0x49524948);
  return;
}

static int32_t script_217EB(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3i local_EC;
  int8_t local_EB;
  int8_t local_EA;
  int8_t local_E9;
  int32_t local_E4;
  local_E4 = arg0;
  switch (local_E4) {
    case 0:
      local_FC = 0x4B4F4B54;
      local_F8 = 0x4D4F4B54;
      break;
    case 1:
      local_FC = 0x4C4F4B54;
      local_F8 = 0x4E4F4B54;
      break;
  }
  if (arg1 == 1) {
    local_EB = 1;
    local_EA = 0;
    (void)((local_E9 = -1) & 0x10);
    Script_charaPosRotUtils(local_F8, 152, 0, &local_EC, CHARA_PART_Get);
    (&local_EC)->y = (&local_EC)->y + 0x8000;
    Script_charaPosRotUtils(local_FC, 152, 0, &local_EC, CHARA_PART_Set);
  } else {
    local_EB = 0;
    local_EA = 1;
    (void)((local_E9 = 0) & 0x10);
    Script_charaPosRotUtils(local_FC, 152, 0, &local_EC, CHARA_PART_Get);
    (&local_EC)->y = (&local_EC)->y + 0x8000;
    Script_charaPosRotUtils(local_F8, 152, 0, &local_EC, CHARA_PART_Set);
  }
  switch (local_EB) {
    case 0:
      Script_charaDisplay(local_FC, CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(local_FC, CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(local_FC, local_EB);
  switch (local_EA) {
    case 0:
      Script_charaDisplay(local_F8, CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(local_F8, CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(local_F8, local_EA);
  return local_E9 & 2;
}

static void script_21934(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = arg1;
  switch (local_FC) {
    case 7:
      script_217EB(arg0, 0);
      break;
    case 6:
      script_217EB(arg0, 1);
      break;
    case 4:
      script_217EB(arg0, 0);
      break;
    case 3:
      script_217EB(arg0, 1);
      break;
    case 2:
      script_217EB(arg0, 0);
      break;
    case 1:
      script_217EB(arg0, 1);
      break;
  }
  return;
}

static int32_t script_219C7(void) {
  Vector3i local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int8_t local_F1;
  int8_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  local_F4.x = 0;
  (&local_F4)->y = 0;
  (&local_F4)->z = 0;
  local_F3 = 0;
  local_F2 = 0;
  if ((Script_ScenDataUtil(task_144, SCND_ReadU32, 755) & 2) == 0) {
    (void)((local_F1 = -(script_217EB(0, 1) != 0)) & 8);
  } else {
    (void)((local_F1 = -(script_217EB(0, 0) != 0)) & 8);
  }
  if ((Script_ScenDataUtil(task_144, SCND_ReadU32, 664) & 2) == 0) {
    (void)((local_F0 = -(script_217EB(1, 1) != 0)) & 8);
  } else {
    (void)((local_F0 = -(script_217EB(1, 0) != 0)) & 8);
  }
  if (script_27831(7, 0, 19, 0)) {
    (void)((local_EE = 0) & 2);
  } else {
    (void)((local_EE = -1) & 2);
  }
  (void)((local_EF = 0) & 2);
  while (local_EE == 0) {
    if ((Script_ScenDataUtil(task_144, SCND_ReadU32, 755) & 2) == 0) {
      if (local_F1 == 0) {
        (void)((local_F1 = -(script_217EB(0, 1) != 0)) & 8);
        local_F3 = 8;
      }
      if (local_F3) {
        script_21934(0, local_F3);
        local_F3 = local_F3 - 1;
      }
      Script_charaPosRotUtils(SHEN_NAME('TKOK'), 152, 0, &data_35748, CHARA_PART_Add);
    } else {
      if (local_F1 == -1) {
        (void)((local_F1 = -(script_217EB(0, 0) != 0)) & 8);
        local_F3 = 40;
      }
      if (local_F3) {
        (&local_F4)->y = (&data_35748)->y + (41 - local_F3) * 22;
        Script_charaPosRotUtils(SHEN_NAME('TKOM'), 152, 0, &local_F4, CHARA_PART_Add);
        local_F3 = local_F3 - 1;
      }
    }
    if (((Script_ScenDataUtil(task_144, SCND_ReadU32, 664) & 2) == 0) & (local_EF == 0)) {
      if (local_F0 == 0) {
        (void)((local_F0 = -(script_217EB(1, 1) != 0)) & 8);
        local_F2 = 8;
      }
      if (local_F2) {
        script_21934(1, local_F2);
        local_F2 = local_F2 - 1;
      }
      Script_charaPosRotUtils(SHEN_NAME('TKOL'), 152, 0, &data_35748, CHARA_PART_Add);
    } else {
      if (local_F0 == -1) {
        (void)((local_F0 = -(script_217EB(1, 0) != 0)) & 8);
        local_F2 = 40;
      }
      if (local_F2) {
        (&local_F4)->y = (&data_35748)->y + (41 - local_F2) * 22;
        Script_charaPosRotUtils(SHEN_NAME('TKON'), 152, 0, &local_F4, CHARA_PART_Add);
        local_F2 = local_F2 - 1;
      }
    }
    if (task_C8.hours >= 19) {
      if (local_EF == 0) {
        (void)((local_EF = -1) & 8);
      } else {
        if (-(local_F1 == 0) & -(local_F0 == 0) & -(local_F2 <= 0)) {
          (void)((local_EE = -1) & 8);
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaDisplay(SHEN_NAME('TKOM'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('TKON'), CDM_Show);
  Script_scenToggleVisibleFor(SHEN_NAME('TKOM'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TKON'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TKOK'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TKOK'), CDM_Show);
      goto L021CFC;
    }
  }
  L021CFC:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKOK'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TKOL'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TKOL'), CDM_Show);
      goto L021D30;
    }
  }
  L021D30:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKOL'), 0);
  return Script_srchControl(0);
}

static void script_21D43(void) {
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 527) == 0)) | (task_F8 >= 2302)) {
    Script_scenToggleCharaFlag(0, 200, 0);
    (void)((task_112 = 0) & 8);
  } else if (script_27831(19, 0, 5, 0)) {
    Script_scenToggleCharaFlag(0, 200, 1);
    Script_charaDisplay(SHEN_NAME('HDCA'), CDM_Show);
    (void)((task_112 = -1) & 8);
  } else {
    Script_scenToggleCharaFlag(0, 200, 0);
    (void)((task_112 = 0) & 8);
  }
  if (task_112 == 0) {
    script_22557();
  } else {
    script_22563();
  }
  return;
}

static int32_t script_21DE3(float arg0, float arg1) {
  int32_t unused_F4[3];
  float local_F0;
  int32_t unused_E4[3];
  float local_E0;
  float local_DC;
  float local_D8;
  int32_t unused_CC[3];
  float local_C8;
  int8_t local_C7;
  int8_t local_C6;
  int8_t local_C5;
  uint8_t unused_C4;
  local_F0 = -17.1f;
  local_E0 = 41.0f;
  *(&local_F0 + 1) = -19.0f;
  *(&local_E0 + 1) = 38.8f;
  *(&local_F0 + 2) = -14.8f;
  *(&local_E0 + 2) = 35.5f;
  *(&local_F0 + 3) = -13.0f;
  *(&local_E0 + 3) = 37.8f;
  local_C7 = 0;
  for (;;) {
    if (local_C7 == 3) {
      local_C6 = 0;
    } else {
      local_C6 = local_C7 + 1;
    }
    local_DC = (*(&local_E0 + local_C7) - *(&local_E0 + local_C6)) / (*(&local_F0 + local_C7) - *(&local_F0 + local_C6));
    local_D8 = *(&local_E0 + local_C7) - local_DC * *(&local_F0 + local_C7);
    *(&local_C8 + local_C7) = local_DC * arg0 + local_D8 - arg1;
    local_C7 = local_C7 + 1;
    if (local_C7 >= 4) {
      break;
    }
  }
  if (-!(*&local_C8 <= (float)0) & -!(*(&local_C8 + 1) >= (float)0) & -!(*(&local_C8 + 2) >= (float)0) & -!(*(&local_C8 + 3) <= (float)0)) {
    (void)((local_C5 = -1) & 1);
  } else {
    (void)((local_C5 = 0) & 1);
  }
  return local_C5 & 0x20;
}

static int32_t script_21F1A(void) {
  int32_t local_FC;
  int32_t local_F8;
  task_38C = 0;
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_35988);
  Script_InvestigationUtils(ShenmueUtils_SetSpecialAssetList, data_35914);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  for (;;) {
    if (task_38C == 0) {
      local_FC = Script_srchUtils(-1);
    }
    local_F8 = local_FC;
    if ((local_F8 == 0x304C4554) | (local_F8 == 133)) {
      Script_srchUtils(SRCH_SCRIPT_Reset);
      script_11E98();
    } else if (local_F8 == 0x314B4254) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_22290();
      Script_ryoControl(RYO_RestoreControl);
    } else if ((local_F8 == 0x30484347) | (local_F8 == 0x31484347) | (local_F8 == 0x32484347) | (local_F8 == 0x34484347) | (local_F8 == 0x35484347) | (local_F8 == 0x33484347)) {
      task_380 = local_FC;
      script_206B8();
    } else {
      if (local_F8 == 134) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        SCNF_PrepareCassetteUI();
        Script_ryoControl(RYO_RestoreControl);
        goto L022027;
      }
    }
    L022027:;
    local_FC = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
    if ((local_FC >= 0) & (local_FC <= 6)) {
      script_220C4(local_FC);
    }
    if ((local_FC == 7) | (local_FC == 8)) {
      script_22408();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2206B(int32_t arg0) {
  SCNF_RequestIconNotification(0, 0, 0, 2);
  (void)((task_10A = 0) & 8);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &task_EC, 0);
  script_220C4(arg0);
  return;
}

static void script_220C4(int32_t arg0) {
  int32_t local_FC;
  if (task_38C == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    script_2A74D();
    task_38C = 1;
    local_FC = arg0;
    switch (local_FC) {
      case 0:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, data_35754);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_35FE4, 60);
        break;
      case 1:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_3578C);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_35FF0, 60);
        break;
      case 2:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_357C4);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_35FFC, 60);
        break;
      case 3:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_35834);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_36008, 60);
        break;
      case 4:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_3586C);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_36014, 60);
        break;
      case 5:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_358A4);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_36020, 60);
        break;
      case 6:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_358DC);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_3602C, 60);
        break;
    }
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
      Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_ryoControl(RYO_RestoreControl);
    task_38C = 0;
  }
  return;
}

static void script_2228D(void) {
  return;
}

static void script_22290(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, SHEN_NAME('TBK1'));
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
      break;
    }
  }
  for (;;) {
    Script_InvestigationUtils(ShenmueUtils_UpdateInterpolatedSpeed);
    if (Script_GetControllerInput(1) == 0x200) {
      script_22315();
      Script_srchUtils(SRCH_SCRIPT_Reset);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  return;
}

static void script_22315(void) {
  int32_t local_FC;
  Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
  local_FC = 0;
  for (;;) {
    if (local_FC == 0) {
      Script_SetGameStateTo(AUTH_Scripted);
      Script_charaDisplay(CHARA_AKIR, CDM_Hide);
    }
    if (local_FC == 4) {
      Script_charaDisplay(SHEN_NAME('TBK1'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
    }
    if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
  }
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_SetGameStateTo(AUTH_None);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  return;
}

static void script_223B3(void) {
  SCNF_RequestIconNotification(0, 0, 0, 2);
  (void)((task_10A = 0) & 2);
  script_22408();
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &task_EC, 0);
  return;
}

static void script_22408(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_357FC);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_36038, 60);
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
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static int8_t script_224A8(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 28) {
    if (Script_GetOwnedItemCount(local_FF, 1)) {
      break;
    }
    local_FF = local_FF + 1;
  }
  if (local_FF < 28) {
    local_FF = 1;
  } else {
    local_FF = 0;
  }
  return local_FF;
}

static void script_224EF(void) {
  SCNF_RequestIconNotification(0, 0, 0, 2);
  (void)((task_10A = 0) & 2);
  if (script_224A8()) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_EC, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_PrepareCassetteUI();
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}

static void script_22557(void) {
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
  return;
}

static void script_22563(void) {
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, -1);
  return;
}

static void script_2256F(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = 0;
  script_387C();
  script_227DC();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_36044, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_36050, CHARA_PART_Set);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 160.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 80.0f);
  Script_SetShadowRange(160.0f);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 0:
        Script_executeAicaCommand(0x1D04A9, 0, 0);
        break;
      case 68:
        Script_scenCreateTask(script_22BE1, 0);
        break;
      case 100:
        Script_scenCreateTask(script_22904, 1, 0);
        break;
      case 125:
        Script_executeAicaCommand(0x1E04A9, 0, 0);
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        local_F8 = Script_scenCreateTask(script_22B91, 1, SHEN_NAME('BUS_'));
        break;
      case 260:
        Script_srchControl((void *)local_F8);
        goto L0226A0;
        break;
    }
    local_FC = local_FC + 1;
    script_22BFE();
    SCNF_WaitFrames_0000(1);
  }
  L0226A0:;
  return;
}

static void script_226A1(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_387C();
  script_227DC();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &data_36068, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &data_36074, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &data_36080, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &data_3608C, CHARA_PART_Set);
  local_FC = 0;
  if (arg1) {
    Script_executeAicaCommand(0x1B04A9, 0, 0);
  }
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 132:
        Script_scenCreateTask(script_22904, 1, 2);
        break;
      case 165:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        local_F8 = Script_scenCreateTask(script_22B91, 1, SHEN_NAME('BUS_'));
        break;
      case 216:
        goto L0227A5;
        break;
    }
    local_FC = local_FC + 1;
    script_22BFE();
    SCNF_WaitFrames_0000(1);
  }
  L0227A5:;
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenSeqUtils(-6);
  script_22894();
  return;
}

static void script_227DC(void) {
  Vector3 local_F4;
  int32_t unused_E4[4];
  GameDateTime local_DC;
  (void)1;
  Script_SetVector4ByOffset(&data_360A4);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_360A4);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  script_3894(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_SetCharaFlags(SHEN_NAME('BUS_'), 2);
  Script_SetCharaFlags(SHEN_NAME('BUS_'), 100, 1, &data_36098);
  Script_ReturnTimeDate(&local_DC);
  local_F4.x = 0.0f;
  (&local_F4)->y = -100.0f;
  (&local_F4)->z = 0.0f;
  if ((&local_DC)->hours * 100 + (&local_DC)->minutes >= 1730) {
    (&local_F4)->y = 0.0f;
  }
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, CHARA_PART_Set);
  return;
}

static void script_22894(void) {
  Vector3 local_F4;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  local_F4.x = 0.0f;
  (&local_F4)->y = -100.0f;
  (&local_F4)->z = 0.0f;
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, CHARA_PART_Set);
  script_3921(CHARA_AKIR);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  return;
}

static int32_t script_22904(int32_t arg0) {
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

static int32_t script_22B91(int32_t arg0) {
  while (Script_shenSeqUtils(-1)) {
    Script_charaPosRotUtils(arg0, 163, 0, &data_360B4, CHARA_PART_Add);
    Script_charaPosRotUtils(arg0, 173, 0, &data_360B4, CHARA_PART_Add);
    Script_charaPosRotUtils(arg0, 182, 0, &data_360C0, CHARA_PART_Add);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_22BE1(void) {
  SCNF_SetCashValue(-160, 1);
  SCNF_WaitFrames_0000(200);
  return Script_srchControl(0);
}

static void script_22BFE(void) {
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
  return;
}

__attribute__((scn_string_bindings(0, data_360CC, 2, data_360E2, 4, data_360F8, 5, data_36103)))
static void script_22CA2(void) {
  int8_t local_FF;
  int32_t local_F8;
  if (Script_shenGetStats(STATS_CASH) < 160) {
    (void)((local_FF = -1) & 1);
  } else {
    (void)((local_FF = 0) & 1);
  }
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_EC, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 28680, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
  if (local_F8 == 0) {
    if (local_FF == -1) {
      Script_ShowUIMessageFromID("SA1076A001");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1076A013");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else if (local_F8 == 1) {
    if (local_FF == -1) {
      Script_ShowUIMessageFromID("SA1076A002");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1076A014");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else if (local_FF == -1) {
    Script_ShowUIMessageFromID("SA1076A003");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("SA1076A015");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  SCNF_SetCashValue(0, 1);
  return;
}

__attribute__((scn_string_bindings(0, data_3610E)))
static void script_22E05(void) {
  int32_t local_FC;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_EC, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MIRU_TOKEI, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("SA1076A015");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("SA1076A016");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("SA1076A017");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_36154)))
static void script_22EBC(void) {
  int32_t unused_DC[9];
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int8_t local_CF;
  uint8_t unused_CC[3];
  local_D8 = Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_SPECIAL_DWORD120, 0x5F535542);
  local_D4 = 300;
  local_D0 = 450;
  if (local_D8 != 0) {
    SCNF_RequestIconNotification(0x100, 0, 8, 1);
    (void)((task_10A = -1) & 4);
    for (;;) {
      if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(1) == 0x400)) {
        (void)((local_CF = 0) & 8);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        if (script_27831(22, 25, 5, 0)) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          (void)((task_10A = 0) & 8);
          script_22E05();
          Script_ryoControl(RYO_RestoreControl);
          break;
        }
        if (Script_shenGetStats(STATS_CASH) < 320) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          (void)((task_10A = 0) & 8);
          script_22CA2();
          Script_ryoControl(RYO_RestoreControl);
          break;
        }
        Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
        SCNF_RequestIconNotification(0, 0, 0, 2);
        (void)((task_10A = 0) & 8);
        Script_CycleManagerUtils(CYCLE_CMD_SET_MOBJ_FIELD_108_BOOL, 0x5F535542, 1);
        if (!((&task_D4)->x * -1.7769f + 81.1959f - task_D4.z <= (float)0)) {
          Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &data_36130, 54631, MN_AKI_AKI_TATI_IWA_L_R, 0);
        } else if (local_D8 >= 0) {
          (void)((local_CF = -1) & 0x40);
        } else {
          Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &data_3613C, 60227, MN_AKI_AKI_TATI_IWA_L_R, 0);
        }
        if (local_CF == 0) {
          while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
            if (local_D4 == 0) {
              Script_GetCharaPos(CHARA_AKIR, 1);
              break;
            }
            local_D4 = local_D4 - 1;
            SCNF_WaitFrames_0000(1);
          }
        }
        for (;;) {
          if (((local_D8 = Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_SPECIAL_DWORD120, 0x5F535542)) >= 0) | (local_D0 == 0)) {
            Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
            Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &data_36148, 60227, MN_AKI_AKI_TATI_IWA_L_R, 0);
            while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
              if (local_D4 == 0) {
                Script_GetCharaPos(CHARA_AKIR, 1);
                break;
              }
              local_D4 = local_D4 - 1;
              SCNF_WaitFrames_0000(1);
            }
            for (;;) {
              if ((float)Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_PRIMARY_DOOR_OPEN_PERCENT, 0x5F535542) >= 90.0f) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
            (void)((task_10C = -1) & 2);
            script_231AD(16);
            script_2A627("BUSS");
            break;
          }
          local_D0 = local_D0 - 1;
          SCNF_WaitFrames_0000(1);
        }
        break;
      } else {
        if (((local_D8 = Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_SPECIAL_DWORD120, 0x5F535542)) == 0) | (Script_IsPlayerAllowed(0) == 0)) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          (void)((task_10A = 0) & 4);
          break;
          goto L023197;
        }
      }
      L023197:;
      SCNF_WaitFrames_0000(1);
    }
    (void)((task_10C = 0) & 4);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_36159, 1, data_36166, 2, data_36176, 4, data_36188, 5, data_3618D, 6, data_3619A, 7, data_3619F, 8, data_361AC, 9, data_361BC, 11, data_361CE, 12, data_361D3, 13, data_361E0)))
static void script_231AD(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_E4[4];
  int32_t local_E0;
  (void)((task_10C = -1) & 4);
  if (arg0 != 17) {
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_DC;
    local_DC = arg0;
    switch (local_DC) {
      case 16:
        script_2A5C6();
        Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
        Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
        script_2952B();
        Script_LoadSND(0, 4, "a1_busno.snd", 0);
        script_2A603("/scene/03/D000/", "BUSS");
        local_E0 = script_2A543(0x5F535542);
        Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
        Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
        Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
        if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f) >= 1.0f)) {
          Script_AuthorUtils(0, 16, "seqdata1.bin", "BUSS");
        } else {
          Script_AuthorUtils(0, 16, "seqdata4.bin", "BUSS");
        }
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
        script_2256F(arg0);
        break;
      case 17:
        Script_LoadSND(0, 4, "a1_busno.snd", 0);
        script_2A603("/scene/03/D000/", "BUSS");
        local_E0 = script_2A543(0x5F535542);
        SCNF_WaitFrames_0000(10);
        Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
        if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f) >= 1.0f)) {
          Script_AuthorUtils(0, 17, "seqdata2.bin", "BUSS");
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
          script_226A1(arg0, 1);
        } else {
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
          Script_AuthorUtils(0, 17, "seqdata5.bin", "BUSS");
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
          script_226A1(arg0, 0);
        }
        break;
    }
  }
  {
    int32_t local_DC;
    local_DC = arg0;
    switch (local_DC) {
      case 16:
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_PlayerStatsUtils(PS_SetStatU8_0, Script_PlayerStatsUtils(PS_GetStatU8_0) + 1);
        SCNF_WaitFrames_0000(75);
        if (task_C8.minutes < 40) {
          task_C8.minutes = task_C8.minutes + 20;
        } else {
          task_C8.minutes = task_C8.minutes - 40;
          task_C8.hours = task_C8.hours + 1;
        }
        Script_SetNewDateTime(&task_C8);
        Script_shenClearFreeState(0);
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('MFSY'), 4);
        SCNF_WaitFrames_0000(1);
        break;
      case 17:
        Script_shenSeqUtils(-6);
        Script_AuthorUtils(AUTH_FREE, 17);
        script_22894();
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
        local_F4.x = 43.9f;
        (&local_F4)->y = 0.1f;
        (&local_F4)->z = 7.9f;
        Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F4);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetGameStateTo(AUTH_None);
        Script_InitializeOsag(CHARA_AKIR);
        SCNF_WaitFrames_0000(1);
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
        while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        script_2A5AF(0x5F535542, local_E0);
        if ((task_C8.hours >= 19) | (task_C8.hours <= 6)) {
          Script_SetCharaFlags(CHARA_AKIR, 0);
        }
        break;
    }
  }
  (void)((task_10C = 0) & 8);
  return;
}

static void script_23569(int32_t arg0, int32_t arg1) {
  task_390 = 0;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_MoveObjectToObject(arg0, arg1);
  script_2A64B(arg0, -0.35f, 0.0f, -0.7f, 0, 0x4000, 0);
  Script_scenCreateTask(script_2377F, 2, arg1, 0);
  Script_PlayCharaMotion(arg0, 28696, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_2A7EC, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(arg0, 28695, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_23610(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_237F0, 2, arg1, 56);
  Script_PlayCharaMotion(arg0, 28694, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_2A7EC, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_MoveObjectToObject(arg0, -1);
  task_390 = 1;
  return;
}

static void script_23674(int32_t arg0, int32_t arg1) {
  task_390 = 0;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_MoveObjectToObject(arg0, arg1);
  script_2A64B(arg0, -0.345f, 0.0f, -0.65f, 0, 0x4000, 0);
  Script_scenCreateTask(script_2377F, 2, arg1, 15);
  Script_PlayCharaMotion(arg0, 28696, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_2A7EC, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(arg0, 28695, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_2371B(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_237F0, 2, arg1, 26);
  Script_PlayCharaMotion(arg0, 28694, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_2A7EC, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_MoveObjectToObject(arg0, -1);
  task_390 = 1;
  return;
}

static int32_t script_2377F(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  int8_t local_F3;
  local_F3 = 0;
  SCNF_WaitFrames_0000(arg1);
  Script_executeAicaCommand(0x1802AB, 0, 0);
  for (;;) {
    Script_mtwkGetSetRotation(arg0, &local_F4, 0);
    (&local_F4)->y = (&local_F4)->y + 200;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    script_23AEB(arg0);
    local_F3 = local_F3 + 1;
    if (local_F3 == 40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_237F0(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  int8_t local_F3;
  local_F3 = 0;
  SCNF_WaitFrames_0000(arg1);
  for (;;) {
    Script_mtwkGetSetRotation(arg0, &local_F4, 0);
    (&local_F4)->y = (&local_F4)->y - 200;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    script_23AEB(arg0);
    local_F3 = local_F3 + 1;
    if (local_F3 == 40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1902AB, 0, 0);
  end:;
}

static void script_23861(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  task_390 = 0;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_AddCharacterHandler(3, "YKB_M", SHEN_NAME('DDMY'));
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(SHEN_NAME('DDMY'), CDM_Hide);
  Script_ccowUtils(SHEN_NAME('DDMY'), CCOW_DisableAll);
  Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg1, &local_E8, 0);
  Script_EVT_SetPosition(SHEN_NAME('DDMY'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('DDMY'), 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, 0x594D4444);
  script_2A64B(arg0, 0.33f, 0.0f, -0.65f, 0, 0xC000, 0);
  Script_scenCreateTask(script_23A3D, 1, arg1);
  Script_scenCreateTask(script_23A3D, 1, SHEN_NAME('DDMY'));
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, 28696, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_2A7EC, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, 28695, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_239A2(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_23A90, 1, arg1);
  Script_scenCreateTask(script_23A90, 1, SHEN_NAME('DDMY'));
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, 28694, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_2A7EC, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
  Script_MoveObjectToObject(arg0, -1);
  Script_SetMapControlTaskFlag(SHEN_NAME('DDMY'), 1);
  SCNF_WaitFrames_0000(1);
  task_390 = 1;
  return;
}

static int32_t script_23A3D(int32_t arg0) {
  Vector3i local_F4;
  int8_t local_F3;
  local_F3 = 0;
  for (;;) {
    Script_mtwkGetSetRotation(arg0, &local_F4, 0);
    (&local_F4)->y = (&local_F4)->y - 182;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    local_F3 = local_F3 + 1;
    if (local_F3 == 40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_23A90(int32_t arg0) {
  Vector3i local_F4;
  int8_t local_F3;
  local_F3 = 0;
  SCNF_WaitFrames_0000(56);
  for (;;) {
    Script_mtwkGetSetRotation(arg0, &local_F4, 0);
    (&local_F4)->y = (&local_F4)->y + 182;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    local_F3 = local_F3 + 1;
    if (local_F3 == 40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_23AEB(int32_t arg0) {
  float local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  Script_mtwkGetSetRotation(arg0, &local_E4, 0);
  (&local_E4)->y = (&local_E4)->y - 1274;
  local_FC = 0.75f;
  task_394 = (&local_F0)->x - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, -(&local_E4)->y) * local_FC;
  *(&task_394 + 1) = 1.08f;
  *(&task_394 + 2) = (&local_F0)->z - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, -(&local_E4)->y) * local_FC;
  return;
}

static int32_t script_23B69(int32_t arg0) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  while ((local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0)) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCPAMFlags(arg0, 0);
  return local_FC;
}

static void script_23BCE(int32_t arg0, int32_t arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg1, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_23BEE(float arg0, float arg1, float arg2, int32_t arg3) {
  Vector3 local_F4;
  local_F4.x = arg0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = arg2;
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, (int32_t)((float)arg3 / 360.0f * (float)0x10000), MN_AKI_AKI_BASE, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_23C48(float arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
  Vector3 local_F4;
  local_F4.x = arg0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = arg2;
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, (int32_t)((float)arg3 / 360.0f * (float)0x10000), arg4, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_23CA1(int32_t arg0) {
  Script_PlayCharaMotion(arg0, 28693, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_2A7EC, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(arg0, 28692, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchControl(0);
  return;
}

static void script_23D0C(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      script_23BEE(-105.1f, 0.0f, 94.15f, -121);
      break;
    case 1:
      script_23BEE(-124.83f, -1.75f, 63.95f, -102);
      break;
  }
  Script_SetGameStateTo(AUTH_None);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ryoControl(RYO_RestoreControl);
  Script_srchControl(0);
  return;
}

static void script_23D77(void) {
  SCNF_WaitFrames_0000(10);
  while (~Script_ryoControl(PCCTL_QueryShouldRun)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static int32_t script_23D9F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  while (arg3 > 0) {
    arg3 = arg3 - 1;
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg1)) {
    Script_EVT_GetPosition(arg1, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(arg0, 16, &local_F4, arg2);
    Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 30);
  }
  return Script_srchControl(0);
}

static int32_t script_23E07(const char *arg0, int32_t arg1) {
  while (arg1 > 0) {
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID(arg0);
  return Script_srchControl(0);
}

static int32_t script_23E33(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_362E2, 13, data_3636D)))
static void script_23E63(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  local_FC = script_23B69(CHARA_ONO_);
  local_F8 = Script_motmGetCharaFlags(CHARA_ONO_, MOTM_PoseNone, SCEN_Read);
  Script_LoadVoiceFile(1, "SA1053");
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ccowUtils(CHARA_ONO_, CCOW_DisableAll);
  Script_SetCPAMFlags(CHARA_ONO_, 0);
  Script_clmdSetFlags(CHARA_ONO_, 2, 1);
  Script_scenToggleActiveFor(CHARA_ONO_, 0);
  Script_motmGetCharaFlags(CHARA_ONO_, MOTM_PoseNone, SCEN_Write);
  script_2A64B(CHARA_ONO_, -117.44f, 0.1f, 86.21f, 0, 6726, 0);
  script_23BEE(-118.6f, 0.05f, 85.1f, 237);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_ONO_, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_361E8);
  Script_SetCharaRefBoardPos(CHARA_ONO_, &data_361E8);
  Script_scenCreateTask(script_2467A, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 28686, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
  script_29945(CHARA_AKIR);
  Script_scenCreateTask(script_23E33, 2, 0x3005A9, 8);
  Script_scenCreateTask(script_23E33, 2, 0x3005A9, 16);
  Script_PlayCharaMotion(CHARA_AKIR, 28685, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
  script_29945(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, 28685, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
  script_29945(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, 28684, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
  Script_ShowUIMessageFromID("SA1053A001");
  script_29945(CHARA_AKIR);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 28686, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
  script_29945(CHARA_AKIR);
  Script_scenCreateTask(script_23E33, 2, 0x3005A9, 8);
  Script_scenCreateTask(script_23E33, 2, 0x3005A9, 16);
  Script_PlayCharaMotion(CHARA_AKIR, 28685, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
  script_29945(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, 28685, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
  Script_ShowUIMessageFromID("SA1053A002");
  script_29945(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, 28684, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
  script_29945(CHARA_AKIR);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_ShowUIMessageFromID("SA1053B007");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.5f);
  Script_EVT_GetPosition(CHARA_ONO_, 9, &local_EC, ENTITY_XFORM_WORLD_SPACE);
  (&local_EC)->y = (&local_EC)->y + 0.1f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 2);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 5);
  Script_scenCreateTask(script_23D9F, 4, CHARA_ONO_, CHARA_AKIR, 2, 30);
  Script_scenCreateTask(script_23E07, 2, "SA1053B001", 80);
  Script_scenCreateTask(script_23E33, 2, 0x1D02AB, 10);
  script_23861(CHARA_ONO_, arg0);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(4122, 0, AUTH_None);
  Script_ShowUIMessageFromID("SA1053A003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("SA1053B002");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("SA1053B003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(4124, 0, AUTH_None);
  Script_ShowUIMessageFromID("SA1053A004");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_GetPosition(CHARA_AKIR, 25, &local_EC, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_ONO_, 11, &local_EC, 2);
  Script_ShowUIMessageFromID("SA1053B004");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(60);
  Script_seqEnterEvtCam(4123, 0, AUTH_None);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_EC, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_ONO_, 6, &local_EC, 2);
  Script_ShowUIMessageFromID("SA1053B005");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("SA1053B006");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("SA1053A005");
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  }
  SCNF_WaitFrames_0000(40);
  Script_EVT_GetPosition(CHARA_ONO_, 9, &local_EC, ENTITY_XFORM_WORLD_SPACE);
  (&local_EC)->y = (&local_EC)->y + 0.2f;
  Script_SetLookPointDetails(CHARA_AKIR, 6, &local_EC, 2);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_ESHAKU, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.6f);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_29945(CHARA_AKIR);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  }
  Script_LoadVoiceFile(0, "SA1053");
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ASKED_FOR_MOTORCYCLE, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 733, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_BUS_TOO_SLOW_TO_HARBOR, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_BORROW_ITOS_MOTORCYCLE, 1);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_scenUtils(SCENARIO_CMD_SET_CHARA_RADIUS_SQUARED, 0x5F4F4E4F, 0.0f);
  Script_scenCreateTask(script_23E33, 2, 0x1C02AB, 56);
  script_239A2(CHARA_ONO_, arg0);
  if (task_390 == 1) {
    Script_motmGetCharaFlags(CHARA_ONO_, local_F8, SCEN_Write);
    Script_charaSetRefboardFlagTo(CHARA_ONO_, 2);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x5F4F4E4F)) {
      Script_SetCharaLookAtPosition(CHARA_ONO_, -13, 0, 0);
      Script_FaceLookAtTargetUtils(CHARA_ONO_, FACE_LookReset);
    }
    Script_SetCPAMFlags(CHARA_ONO_, 1);
    Script_scenToggleActiveFor(CHARA_ONO_, 1);
    Script_clmdSetFlags(CHARA_ONO_, 2, 0);
    script_23BCE(local_FC, 0x5F4F4E4F);
    task_390 = 0;
  }
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  return;
}

static int32_t script_2467A(void) {
  Script_seqEnterEvtCam(4120, 0, AUTH_None);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetCurrentFrameCount() >= 420) {
      break;
    }
  }
  Script_seqEnterEvtCam(4121, 0, AUTH_None);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(2, data_3638A, 4, data_363A0)))
static void script_246B2(int32_t arg0) {
  int8_t local_FF;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_MACHINE_PURCHASED_ONCE) == 0) {
    uint8_t unused_FC[3];
    int32_t local_F8;
    script_24892(arg0);
    local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
    local_F8 = local_FF;
    switch (local_F8) {
      case 0:
        Script_ShowUIMessageFromID("SA1080A003");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 1:
        Script_ShowUIMessageFromID("SA1080A002");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
  } else {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_MACHINE_PURCHASED_ONCE)) {
      script_24892(arg0);
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (task_C8.hours < 10) {
        uint8_t unused_FC[3];
        int32_t local_F8;
        local_F8 = local_FF;
        switch (local_F8) {
          case 0:
            Script_ShowUIMessageFromID("SA1080A004");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            break;
          case 1:
            Script_ShowUIMessageFromID("SA1080A005");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            break;
        }
      } else {
        if (task_C8.hours >= 21) {
          uint8_t unused_FC[3];
          int32_t local_F8;
          local_F8 = local_FF;
          switch (local_F8) {
            case 0:
              Script_ShowUIMessageFromID("SA1080A004");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              break;
            case 1:
              Script_ShowUIMessageFromID("SA1080A006");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              break;
          }
          goto L024844;
        }
      }
      L024844:;
      goto L024847;
    }
  }
  L024847:;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x80);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_24892(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_LoadVoiceFile(1, "SA1080");
  Script_MoveObjectToObject(CHARA_AKIR, arg0);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  if (!((&local_F4)->x <= 0.0f)) {
    Script_MoveObjectToObject(CHARA_AKIR, -1);
    script_23C48(-40.033798f, 0.08f, 74.684502f, 18, 1554);
    Script_scenCreateTask(script_23E33, 2, 0x4205A9, 56);
    Script_scenCreateTask(script_23E33, 2, 0x4205A9, 78);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.25f);
  } else {
    Script_MoveObjectToObject(CHARA_AKIR, -1);
    script_23C48(-40.9137f, 0.08f, 75.033096f, 18, -0x7FFFF9EE);
    Script_scenCreateTask(script_23E33, 2, 0x4205A9, 56);
    Script_scenCreateTask(script_23E33, 2, 0x4205A9, 78);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.25f);
  }
  SCNF_WaitFrames_0000(120);
  return;
}

__attribute__((scn_string_bindings(0, data_363BD, 1, data_363C4, 3, data_363DA, 6, data_363FB)))
static void script_249BE(int32_t arg0) {
  float local_FC;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_LoadVoiceFile(1, "SA1081");
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  script_23BEE(6.4521f, 0.07f, 84.816498f, 320);
  Script_PlayCharaMotion(CHARA_AKIR, 28677, -1.0f, -1.0f, 40.0f, 1.2f, 0);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 58.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x4205A9, 0, 0);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 82.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x4205A9, 0, 0);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 150.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  if (task_F8 <= 202) {
    if (!(local_FC >= 1.5f)) {
      Script_ShowUIMessageFromID("SA1081A002");
    } else {
      Script_ShowUIMessageFromID("SA1081A003");
    }
  } else if (!(local_FC >= 1.5f)) {
    Script_ShowUIMessageFromID("SA1081A002");
  } else if ((task_C8.hours > 4) & (task_C8.hours < 10)) {
    Script_ShowUIMessageFromID("SA1081A005");
  } else {
    Script_ShowUIMessageFromID("SA1081A006");
  }
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadVoiceFile(0, "SA1081");
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_36402, 2, data_36414, 3, data_3641F, 4, data_3642A, 6, data_36440, 8, data_36456, 9, data_36461, 10, data_3646C)))
static void script_24BB0(int32_t arg0) {
  float local_FC;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_LoadVoiceFile(1, "SA1088");
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  script_23BEE(-19.15f, 0.0724f, 72.97f, 10);
  Script_PlayCharaMotion(CHARA_AKIR, 28677, -1.0f, -1.0f, 40.0f, 1.2f, 0);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 58.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x4205A9, 0, 0);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 82.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x4205A9, 0, 0);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 150.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  if (task_F8 <= 401) {
    if (!(local_FC >= 1.0f)) {
      Script_ShowUIMessageFromID("SA1088A001");
    } else if (!(local_FC >= 2.0f)) {
      Script_ShowUIMessageFromID("SA1088A002");
    } else {
      Script_ShowUIMessageFromID("SA1088A003");
    }
  } else if (task_C8.hours >= 5) {
    if (!(local_FC >= 1.0f)) {
      Script_ShowUIMessageFromID("SA1088A002");
    } else if (!(local_FC >= 2.0f)) {
      Script_ShowUIMessageFromID("SA1088A004");
    } else {
      Script_ShowUIMessageFromID("SA1088A003");
    }
  } else if (!(local_FC >= 1.0f)) {
    Script_ShowUIMessageFromID("SA1088A005");
  } else if (!(local_FC >= 2.0f)) {
    Script_ShowUIMessageFromID("SA1088A002");
  } else {
    Script_ShowUIMessageFromID("SA1088A003");
  }
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadVoiceFile(0, "SA1088");
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_24DF6(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (Script_IsPlayerAllowed(0) == 0) {
  } else {
    local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
    if (local_FC != -1) {
      local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_18C[local_FC], 2);
      local_F8 = Script_ScenDataUtil(task_14C, SCND_ReadU32, Script_ScenDataUtil(task_150, SCND_ReadU32, local_F8 * 9 + 1) * 4 + 1);
      local_F8 = script_10204(local_FC, local_F8, 0);
      if (local_F8) {
        if (Script_GetControllerInput(1) == 0x400) {
          local_F4 = task_18C[local_FC];
          switch (local_F4) {
            case 26:
              if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
                if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 733) == 0) {
                  script_1029C(local_FC);
                  script_23E63(task_22C[local_FC]);
                  script_1046B(local_FC);
                }
              }
              break;
            case 51:
              if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[local_FC] * 13 + 1) & 2) {
                script_249BE(task_22C[local_FC]);
              }
              break;
            case 1:
              if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[local_FC] * 13 + 1) & 2) {
                script_246B2(task_22C[local_FC]);
              }
              break;
            case 61:
              if (Script_ScenDataUtil(task_144, SCND_ReadU32, task_18C[local_FC] * 13 + 1) & 2) {
                script_24BB0(task_22C[local_FC]);
              }
              break;
          }
        }
      }
    }
  }
  return;
}

static int32_t script_24FB1(void) {
  Script_scenCreateTask(script_0030, 6, CHARA_AKIR, 2, 0, &data_36474, 2, 0);
  script_2643((int32_t)data_36628);
  SCNF_WaitFrames_0000(1);
  if (1) {
    task_78 = *&task_78 | 0x20;
  } else {
    task_78 = *&task_78 & -33;
  }
  if (1) {
    task_78 = *&task_78 | 0x800;
  } else {
    task_78 = *&task_78 & -2049;
  }
  end:;
}

static void script_25029(void) {
  if (Script_GetCurrentEntryPoint() != 99) {
    Script_scenCreateTask(script_26037, 0);
  }
  Script_scenCreateTask(script_24FB1, 0);
  script_27C43();
  return;
}

static void script_25051(void) {
  if (task_106 & 0x80) {
    Script_GetCurrentDrawMode(1, 0);
  }
  Script_sprintf("DOBUITA EVENT START START START!\012");
  return;
}

static void script_25071(int32_t arg0) {
  if (task_106 & 0x80) {
    if (arg0 != 1) {
      script_26D79();
    }
    Script_SetCharaFlags(CHARA_AKIR, 0);
  } else {
    script_26D55();
    Script_SetCharaFlags(CHARA_AKIR, 4);
  }
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 3);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_mapmSetCamFlags(0, 1);
  Script_sprintf("DOBUITA EVENT END!\012");
  return;
}

static int8_t script_25108(int8_t arg0) {
  Vector3 local_F4;
  float local_F0;
  float local_EC;
  int32_t local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int32_t local_E0;
  local_E7 = 0;
  local_E6 = 0;
  local_E0 = arg0;
  if (local_E0 == 0) {
    local_F4.x = 11.7795f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 46.8381f;
    local_F0 = 2.0f;
    local_EC = 8.0f;
    local_E6 = 0;
    local_E8 = 1;
  } else if (local_E0 == 1) {
    local_F4.x = 41.61f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 22.6551f;
    local_F0 = 4.5f;
    local_EC = 10.0f;
    local_E6 = 1;
    local_E8 = 2;
  } else {
    Script_sprintf("training check error!!!!\012");
    local_F0 = 0.0f;
    local_E8 = local_E7;
  }
  if (*((int8_t *)&*(int32_t *)&task_102 + local_E6) == 1) {
    if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &task_D4) <= local_EC)) {
      *((int8_t *)&*(int32_t *)&task_102 + local_E6) = 0;
    }
  }
  if (script_27831(8, 0, 22, 0)) {
    if (Script_IsPlayerAllowed(0)) {
      if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_F4, 10.0f) == 0) {
        if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &task_D4) >= local_F0)) {
          if (*((int8_t *)&*(int32_t *)&task_102 + local_E6) == 0) {
            *((int8_t *)&*(int32_t *)&task_102 + local_E6) = 1;
            local_E7 = local_E8;
            Script_sprintf("Daily practice %d: Check: in range, setting training flag to true\012", local_E6);
          } else {
            if (task_104) {
              local_E7 = local_E8;
              Script_sprintf("Daily practice %d: Check: in range, training flag already true ?? Failed before due to NPC's so check again\012", local_E6);
            }
          }
        }
      }
    }
  }
  return local_E7;
}

static int8_t script_25279(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_F0;
  int32_t local_E8[2];
  int8_t local_E7;
  uint8_t unused_E4[3];
  int32_t local_E0;
  local_E7 = 0;
  local_E0 = arg0;
  switch (local_E0) {
    case 8:
      local_F4.x = 32.141899f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 22.096001f;
      local_E8[1] = 10922;
      break;
    case 9:
      local_F4.x = 15.3191f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 43.8f;
      local_E8[1] = 0xC000;
      break;
    case 10:
      local_F4.x = -25.4825f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 44.4692f;
      local_E8[1] = 65353;
      break;
    case 11:
      local_F4.x = -62.534199f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 90.357498f;
      local_E8[1] = 0;
      break;
    case 12:
      local_F4.x = 6.5485f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 91.922798f;
      local_E8[1] = 58254;
      break;
    case 13:
      local_F4.x = -110.483002f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 76.7537f;
      local_E8[1] = 7645;
      break;
    case 14:
      local_F4.x = -106.825996f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 102.427597f;
      local_E8[1] = 9648;
      break;
    case 15:
      local_F4.x = 42.545502f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 5.1289f;
      local_E8[1] = 27306;
      break;
  }
  if (Script_IsPlayerAllowed(0)) {
    if (Script_CheckTargetWithinAngleRange(&local_F4, 1.5f, local_E8[1], 12743, &task_D4)) {
      if (Script_CheckTargetWithinAngleRange(&task_D4, 1.4f, task_EC.y, 7281, &local_F4)) {
        local_E7 = arg0;
        if (task_10A == 0) {
          SCNF_RequestIconNotification(0x100, 0, 0, 1);
          (void)((task_10A = -1) & 4);
        }
      } else {
        if (task_10A == -1) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          (void)((task_10A = 0) & 4);
        }
      }
    }
  } else {
    if (task_10A == -1) {
      SCNF_RequestIconNotification(0, 0, 0, 2);
      (void)((task_10A = 0) & 4);
    }
  }
  return local_E7;
}

static int8_t script_254E2(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_F0;
  int32_t local_E8[2];
  int8_t local_E7;
  uint8_t unused_E4[3];
  local_F4.x = -18.045f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 38.519501f;
  local_E8[1] = 6553;
  local_E7 = 0;
  if (task_112 & -(Script_IsPlayerAllowed(0) != 0)) {
    if (Script_CheckTargetWithinAngleRange(&local_F4, 1.5f, local_E8[1], 12743, &task_D4)) {
      if (Script_CheckTargetWithinAngleRange(&task_D4, 1.4f, task_EC.y, 7281, &local_F4)) {
        local_E7 = arg0;
        if (task_10A == 0) {
          SCNF_RequestIconNotification(0x100, 0, 0, 1);
          (void)((task_10A = -1) & 1);
        }
      } else {
        if (task_10A == -1) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          (void)((task_10A = 0) & 1);
        }
      }
    }
  } else {
    if (task_10A == -1) {
      SCNF_RequestIconNotification(0, 0, 0, 2);
      (void)((task_10A = 0) & 1);
    }
  }
  return local_E7;
}

static int8_t script_255D1(int32_t arg0) {
  Vector3 local_F4;
  int8_t local_F3;
  int32_t local_EC;
  local_F3 = 0;
  local_EC = arg0;
  if (local_EC == 3) {
    local_F4.x = 17.95f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 25.969f;
  } else if (local_EC == 4) {
    local_F4.x = -97.890099f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 96.826103f;
  } else if (local_EC == 5) {
    local_F4.x = -70.8f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 56.6f;
  } else if (local_EC == 6) {
    local_F4.x = -108.824799f;
    (&local_F4)->y = 0.072f;
    (&local_F4)->z = 74.606903f;
  } else if (local_EC == 7) {
    local_F4.x = -15.99f;
    (&local_F4)->y = 0.07201f;
    (&local_F4)->z = 72.260063f;
  } else {
    Script_sprintf("VEND_MACHINE_CHECKER_ERROR!!!\012");
  }
  if (Script_IsPlayerAllowed(0)) {
    if (Script_CheckTargetWithinAngleRange(&task_D4, 1.4f, task_EC.y, 7281, &local_F4)) {
      local_F3 = arg0;
      if (task_10A == 0) {
        Script_sicnUtils(0, 5, 1);
        (void)((task_10A = -1) & 0x80);
      }
      if (task_3A0 == 0) {
        Script_uiUtils(0, 25, 0);
        task_3A0 = 1;
      }
    } else {
      if (task_10A == -1) {
        Script_sicnUtils(1, 0, 0);
        (void)((task_10A = 0) & 0x80);
      }
      if (task_3A0 != 0) {
        Script_uiUtils(0, 0, 0);
        task_3A0 = 0;
      }
    }
  } else {
    if (task_10A == -1) {
      Script_sicnUtils(1, 0, 0);
      (void)((task_10A = 0) & 0x80);
    }
  }
  return local_F3;
}

static int8_t script_2578D(void) {
  int8_t local_FF;
  local_FF = 0;
  if (script_1E57D()) {
    local_FF = 18;
  }
  return local_FF;
}

static void script_257A7(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  GameDateTime local_F0;
  int8_t local_EF;
  int8_t local_EE;
  uint8_t unused_EC[2];
  if (Script_IsPlayerAllowed(0)) {
    if (Script_GetControllerInput(1) == 0x400) {
      local_FC = arg0;
      if ((local_FC == 3) | (local_FC == 4) | (local_FC == 5) | (local_FC == 6) | (local_FC == 7)) {
        if (script_3F53(100, 1)) {
          if (task_10A == -1) {
            Script_sicnUtils(1, 0, 0);
            (void)((task_10A = 0) & 0x80);
          }
          script_25051();
          script_48F0(CHARA_AKIR, SHEN_NAME('VM_0'), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
          script_25071(0);
        }
      } else if ((local_FC == 8) | (local_FC == 9) | (local_FC == 10) | (local_FC == 11) | (local_FC == 12) | (local_FC == 13) | (local_FC == 14)) {
        script_25051();
        script_2206B(arg0 - 8);
        script_25071(0);
      } else {
        switch (local_FC) {
          case 15:
            script_25051();
            script_223B3();
            script_25071(0);
            break;
          case 16:
            script_25051();
            script_224EF();
            script_25071(1);
            break;
        }
      }
    }
    local_FC = arg0;
  }
  local_FC = arg0;
  if ((local_FC == 1) | (local_FC == 2)) {
    Script_scenSetAssignCVWK(CHARA_AKIR, 0, 3);
    if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1) == 0) {
      Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 1);
    }
    script_28ACC(arg0 - 1);
    if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1)) {
      Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 0);
    }
    Script_scenSetAssignCVWK(CHARA_AKIR, 0, 0);
    Script_uiUtils(0, 0, 0);
  } else {
    switch (local_FC) {
      case 18:
        script_25051();
        script_206B8();
        script_25071(0);
        break;
      case 20:
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_JIMSE_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_POST_GMCT_STAGE_ACTIVE) == 0)) {
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_POST_GMCT_STAGE_ACTIVE, 1);
          script_278B0(3, SHEN_NAME('ARAR'), 1, 0);
        } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_HANA_STAGE_ACTIVE) == 0)) {
          if ((script_76B7(24) > 0) | (script_7781(30) > 240)) {
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_HANA_STAGE_ACTIVE, 1);
            Script_ReturnTimeDate(&local_F0);
            if ((&local_F0)->month == 12) {
              local_EE = 0;
            } else {
              local_EE = (&local_F0)->month;
            }
            local_EF = (local_EE << 5) | (&local_F0)->day;
            Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 24, local_EF);
            script_278B0(3, SHEN_NAME('ARAR'), 0, 0);
          } else {
            script_278B0(3, SHEN_NAME('DAZA'), 0, 0);
          }
        } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_PRE_0172_FUKU_INE_STAGE_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_CASSETTE_REWARD_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_REWARD_ROUTE_COMPLETE) == 0)) {
          script_278B0(3, SHEN_NAME('ARAR'), 3, 0);
        } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_REWARD_ROUTE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED, 1);
          script_278B0(3, SHEN_NAME('ARAR'), 4, 0);
        } else {
          script_278B0(3, SHEN_NAME('DAZA'), 0, 0);
        }
        break;
      case 22:
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_CHALLENGE_COMPLETE) == 0)) {
          script_278B0(3, SHEN_NAME('DHQB'), 0, 0);
        } else {
          script_278B0(3, SHEN_NAME('DBHB'), 0, 0);
        }
        break;
      case 21:
        script_278B0(3, SHEN_NAME('DSKI'), 0, 0);
        break;
      case 23:
        script_278B0(3, SHEN_NAME('DTKY'), 0, 0);
        break;
      case 19:
        script_278B0(3, SHEN_NAME('DKTY'), 0, 0);
        break;
      case 24:
        script_278B0(3, SHEN_NAME('DCHA'), 0, 0);
        break;
      case 26:
        script_278B0(3, SHEN_NAME('DBYO'), 0, 0);
        break;
      case 27:
        script_278B0(3, SHEN_NAME('DJAZ'), 0, 0);
        break;
      case 28:
        script_278B0(3, SHEN_NAME('DSBA'), 0, 0);
        break;
      case 29:
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_E1049_ROUTE_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) == 0)) {
          script_278B0(3, CHARA_TOKI, 10, 0);
        } else if (-(task_F8 > 1600) & -(task_F8 < 1902) & -(Script_GetSceneID() == 2) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PHOENIX_LEGEND) == 0)) {
          script_278B0(3, CHARA_TOKI, 1, 0);
        } else {
          script_278B0(3, SHEN_NAME('DRSA'), 0, 0);
        }
        break;
      case 25:
        script_278B0(3, SHEN_NAME('TATQ'), 0, 0);
        break;
      case 30:
        script_278B0(3, SHEN_NAME('DMAJ'), 0, 0);
        break;
      case 31:
        script_278B0(3, SHEN_NAME('DPIZ'), 0, 0);
        break;
      case 32:
        script_278B0(3, SHEN_NAME('DRME'), 0, 0);
        break;
      case 33:
        script_278B0(3, SHEN_NAME('DSLI'), 0, 0);
        break;
      case 34:
        script_278B0(3, SHEN_NAME('DRHT'), 0, 0);
        break;
      case 35:
        script_278B0(3, SHEN_NAME('DSUS'), 0, 0);
        break;
      case 36:
        script_278B0(3, SHEN_NAME('DURN'), 0, 0);
        break;
      case 37:
        script_278B0(3, SHEN_NAME('DYKZ'), 0, 0);
        break;
      case 38:
        script_278B0(3, SHEN_NAME('DKPA'), 0, 0);
        break;
      case 39:
        script_278B0(3, SHEN_NAME('DSLT'), 0, 0);
        break;
    }
  }
  return;
}

static int32_t script_25DF1(int32_t arg0, int32_t arg1) {
  if (arg0) {
    (void)arg0;
  } else {
    (void)arg1;
  }
  end:;
}

static int32_t script_25E03(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  for (;;) {
    local_FC = script_25DF1(arg0, arg1);
    switch (local_FC) {
      case 1:
        arg0 = script_25108(0);
        break;
      case 2:
        arg0 = script_25108(1);
        break;
      case 3:
        arg0 = script_255D1(3);
        break;
      case 4:
        arg0 = script_255D1(4);
        break;
      case 5:
        arg0 = script_255D1(5);
        break;
      case 6:
        arg0 = script_255D1(6);
        break;
      case 7:
        arg0 = script_255D1(7);
        break;
      case 8:
        arg0 = script_25279(8);
        break;
      case 9:
        arg0 = script_25279(9);
        break;
      case 10:
        arg0 = script_25279(10);
        break;
      case 11:
        arg0 = script_25279(11);
        break;
      case 12:
        arg0 = script_25279(12);
        break;
      case 13:
        arg0 = script_25279(13);
        break;
      case 14:
        arg0 = script_25279(14);
        break;
      case 15:
        arg0 = script_25279(15);
        break;
      case 16:
        arg0 = script_254E2(16);
        break;
      case 18:
        arg0 = 0;
        break;
    }
    if (arg0 != -1) {
      break;
    } else {
      if (arg1 >= 17) {
        arg1 = 1;
      } else {
        arg1 = arg1 + 1;
      }
      arg0 = 0;
    }
  }
  if (arg0 == 0) {
    arg0 = script_2578D();
  }
  return arg0;
}

static void script_26011(void) {
  int32_t local_FC;
  local_FC = Script_UpdateTaskCollisionCache(0, 1, &task_D4);
  if (local_FC != -1) {
    Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, local_FC);
  }
  return;
}

static int32_t script_26037(void) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  local_FE = 1;
  local_FF = 0;
  local_FF = script_25E03(local_FF, local_FE);
  for (;;) {
    script_26011();
    local_FF = script_25E03(local_FF, local_FE);
    if (local_FE >= 17) {
      local_FE = 1;
    } else {
      local_FE = local_FE + 1;
    }
    if ((&task_154)->mode == 1) {
      local_F8 = task_154.entry_idx;
      switch (local_F8) {
        case 35:
          local_FF = 19;
          break;
        case 25:
          local_FF = 20;
          break;
        case 30:
          local_FF = 22;
          break;
        case 58:
          local_FF = 23;
          break;
        case 28:
          local_FF = 24;
          break;
        case 60:
          local_FF = 21;
          break;
        case 63:
          local_FF = 26;
          break;
        case 0:
          local_FF = 27;
          break;
        case 55:
          local_FF = 28;
          break;
        case 1:
          local_FF = 29;
          break;
        case 46:
          local_FF = 25;
          break;
        case 39:
          local_FF = 30;
          break;
        case 49:
          local_FF = 31;
          break;
        case 50:
          local_FF = 32;
          break;
        case 54:
          local_FF = 33;
          break;
        case 51:
          local_FF = 34;
          break;
        case 56:
          local_FF = 35;
          break;
        case 59:
          local_FF = 36;
          break;
        case 61:
          local_FF = 37;
          break;
        case 34:
          local_FF = 38;
          break;
        case 53:
          local_FF = 39;
          break;
      }
    }
    if (task_108 & 2) {
      if (Script_IsPlayerAllowed(0)) {
        script_22EBC();
      }
    }
    if (local_FF) {
      script_257A7(local_FF);
    }
    if (*&task_84 != -1) {
      script_26272();
    }
    if (Script_IsPlayerAllowed(0)) {
      script_24DF6();
    }
    script_26564();
    script_265AD();
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_26272(void) {
  return;
}

__attribute__((scn_string_bindings(1, data_3679E, 2, data_367A9, 3, data_367B4)))
static void script_26275(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 9) {
    Script_ShowUIMessageFromID("SA1101A020");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_BUS_TOO_SLOW_TO_HARBOR) == 0) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_BUS_TOO_SLOW_TO_HARBOR, 1);
    }
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, 28680, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
    if (local_FC == 0) {
      Script_ShowUIMessageFromID("SA1076A001");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_FC == 1) {
      Script_ShowUIMessageFromID("SA1076A002");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1076A003");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    SCNF_SetCashValue(0, 1);
  }
  return;
}

static void script_2636F(int32_t arg0) {
  Vector3 local_F4;
  if (!((&task_D4)->x * 0.42f - task_D4.z <= 7.13f)) {
    if (task_109 != -1) {
      (void)((task_109 = -1) & 0x20);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 80.0f);
    }
  } else {
    if (task_109 != 0) {
      (void)((task_109 = 0) & 0x20);
      script_26D55();
    }
  }
  if (!((&task_D4)->x * 0.45f - task_D4.z <= 10.07f)) {
    if (task_111 == 0) {
      (void)((task_111 = -1) & 0x40);
      Script_uiUtils(0, 63, 0);
    }
    if (arg0 != 0) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
      Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
      Script_shenClearFreeState(0);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      script_26275(arg0);
      local_F4.x = 33.25f;
      (&local_F4)->z = 10.74f;
      SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_2DCB0, 0.0f, 0, 0x200000);
      Script_charaToggleMotEnabled(CHARA_AKIR, 0);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_IPPO, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      for (;;) {
        if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
          break;
        }
        Script_RotateNodeUtils(CHARA_AKIR, &data_2A7EC, 0);
        SCNF_WaitFrames_0000(1);
      }
      Script_charaToggleMotEnabled(CHARA_AKIR, -1);
      Script_ryoControl(RYO_RestoreControl);
      Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
      Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
      Script_shenClearFreeState(1);
    }
  } else {
    if (task_111 == -1) {
      (void)((task_111 = 0) & 4);
      Script_uiUtils(0, 0, 0);
    }
  }
  return;
}

static void script_26564(void) {
  if (Script_IsPlayerAllowed()) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
      script_2636F(9);
    } else {
      script_2636F(0);
    }
  }
  return;
}

static void script_2659E(void) {
  Script_LoadVoiceFile(1, "SA1076");
  return;
}

static void script_265AD(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  Vector3i local_D0;
  int8_t local_CF;
  int8_t local_CE;
  (void)((local_CE = -1) & 4);
  (&local_F4)->y = (&local_E8)->y = 0.0724f;
  local_DC.x = (&local_DC)->z = 0;
  local_D0.x = (&local_D0)->z = 0;
  local_CF = task_105 & 0xF;
  if (local_CE & 0x40) {
    if (!((&task_D4)->x <= -22.5f)) {
      if (!(task_D4.z >= 70.0f)) {
        (void)((local_CE = 0) & 1);
        if (local_CF != 0) {
          task_105 = 0;
          local_F4.x = 17.95f;
          (&local_F4)->z = 25.969f;
          (&local_DC)->y = 60711;
          local_E8.x = -70.8f;
          (&local_E8)->z = 56.6f;
          (&local_D0)->y = 10370;
          goto L026670;
        }
      }
    }
  }
  L026670:;
  if (local_CE & 0x40) {
    if (!((&task_D4)->x <= -90.1f)) {
      if (!(task_D4.z >= 70.1f)) {
        (void)((local_CE = 0) & 8);
        if (local_CF != 1) {
          task_105 = 1;
          local_F4.x = -70.8f;
          (&local_F4)->z = 56.6f;
          (&local_DC)->y = 10370;
          local_E8.x = 17.95f;
          (&local_E8)->z = 25.969f;
          (&local_D0)->y = 60711;
        }
      }
    }
  }
  if (local_CE & 0x40) {
    if (!((&task_D4)->x <= -70.1f)) {
      if (!(task_D4.z <= 70.0f)) {
        (void)((local_CE = 0) & 1);
        if (local_CF != 2) {
          task_105 = 2;
          local_F4.x = -15.99f;
          (&local_F4)->z = 72.260063f;
          (&local_DC)->y = 0;
          local_E8.x = -70.8f;
          (&local_E8)->z = 56.6f;
          (&local_D0)->y = 10370;
        }
      }
    }
  }
  if (local_CE & 0x40) {
    if (!((&task_D4)->x >= -70.0f)) {
      if (!(task_D4.z >= 85.0f)) {
        (void)((local_CE = 0) & 1);
        if (local_CF != 3) {
          task_105 = 3;
          local_F4.x = -108.824799f;
          (&local_F4)->z = 74.606903f;
          (&local_DC)->y = 41414;
          local_E8.x = -70.8f;
          (&local_E8)->z = 56.6f;
          (&local_D0)->y = 10370;
        }
      }
    }
  }
  if (local_CE & 0x40) {
    if (local_CF != 4) {
      task_105 = 4;
      local_F4.x = -97.890099f;
      (&local_F4)->z = 96.826103f;
      (&local_DC)->y = 42511;
      local_E8.x = -108.824799f;
      (&local_E8)->z = 74.606903f;
      (&local_D0)->y = 41414;
    }
  }
  if ((task_105 & 0x80) != 0) {
  } else if ((task_105 & 0x40) != 0) {
    Script_scenToggleVisibleFor(SHEN_NAME('VM_0'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('VM_1'), 0);
    task_105 = task_105 | 0x80;
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('VM_0'), 1);
    Script_scenToggleVisibleFor(SHEN_NAME('VM_1'), 1);
    Script_EVT_SetPosition(SHEN_NAME('VM_0'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('VM_0'), 0x38000000, &local_DC, 0);
    Script_EVT_SetPosition(SHEN_NAME('VM_1'), POS_XYZ, &local_E8);
    Script_EVT_SetRotation(SHEN_NAME('VM_1'), 0x38000000, &local_D0, 0);
    task_105 = task_105 | 0x40;
  }
  return;
}

static int32_t script_268F8(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  (void)((local_FF = -1) & 2);
  (void)((local_FE = -1) & 2);
  (void)((local_FD = -1) & 2);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_367C8, 85);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet1FromTaskOffset, &data_37A60, 14);
  for (;;) {
    if (task_10C == -1) {
      Script_WeatherUtils(WeatherScript_SetWccpClipSet0Enabled, 0);
      for (;;) {
        if (task_10C == 0) {
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Enabled, 1);
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    if ((task_101 == 2) | (task_101 == 3)) {
      if (local_FF == 0) {
        int8_t local_FC;
        local_FC = 0;
        while (local_FC < 85) {
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_FC, 1);
          local_FC = local_FC + 1;
        }
        (void)((local_FF = -1) & 2);
        (void)((local_FE = -1) & 2);
      }
      if (task_112 & 8) {
        if (local_FE == 0) {
          (void)((local_FE = -1) & 2);
          Script_WeatherUtils(WeatherScript_SetWccpClipSet1Entry, 5, 1);
        }
      } else {
        if (local_FE == -1) {
          (void)((local_FE = 0) & 2);
          Script_WeatherUtils(WeatherScript_SetWccpClipSet1Entry, 5, 0);
          goto L026A01;
        }
      }
      L026A01:;
      if (Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_PRIMARY_DOOR_OPEN_PERCENT, 0x5F535542) | Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_FIELD_148_OR_13C_PERCENT, 0x5F535542)) {
        if (local_FD == 0) {
          (void)((local_FD = -1) & 2);
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 5, 1);
        }
      } else {
        if (local_FD == -1) {
          (void)((local_FD = 0) & 2);
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 5, 0);
          goto L026A62;
        }
      }
      L026A62:;
    } else {
      if (local_FF == -1) {
        int8_t local_FC;
        local_FC = 0;
        while (local_FC < 85) {
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_FC, 0);
          local_FC = local_FC + 1;
        }
        (void)((local_FF = 0) & 2);
        (void)((local_FE = 0) & 2);
      }
    }
    SCNF_WaitFrames_0000(30);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_37D70, 2, data_37D85, 4, data_37D9A, 6, data_37DAF)))
static void script_26AB2(void) {
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    script_2A603("/scene/03/D000/", "MAP1");
  } else {
    script_2A603("/scene/03/D000/", "MAP0");
  }
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 5)) {
    script_2A603("/scene/03/D000/", "MAP3");
  } else {
    if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 4)) {
      script_2A603("/scene/03/D000/", "MAP2");
      goto L026B25;
    }
  }
  L026B25:;
  return;
}

static void script_26B26(void) {
  if (script_27831(8, 30, 18, 30)) {
    Script_mapmSetRenderFlag(20, 0);
    Script_mapmSetRenderFlag(17, 1);
  } else {
    Script_mapmSetRenderFlag(17, 0);
    Script_mapmSetRenderFlag(20, 1);
  }
  if (script_27831(19, 0, 23, 0)) {
    Script_mapmSetRenderFlag(18, 0);
    Script_mapmSetRenderFlag(21, 1);
  } else {
    Script_mapmSetRenderFlag(21, 0);
    Script_mapmSetRenderFlag(18, 1);
  }
  if (script_27831(19, 0, 23, 30)) {
    Script_mapmSetRenderFlag(19, 0);
    Script_mapmSetRenderFlag(22, 1);
  } else {
    Script_mapmSetRenderFlag(22, 0);
    Script_mapmSetRenderFlag(19, 1);
  }
  return;
}

static void script_26BD4(void) {
  if (script_27831(8, 30, 18, 30)) {
    Script_mapmSetRenderFlag(8, 1);
    Script_mapmSetRenderFlag(11, 0);
  } else {
    Script_mapmSetRenderFlag(8, 0);
    Script_mapmSetRenderFlag(11, 1);
  }
  if (script_27831(10, 0, 21, 0)) {
    Script_mapmSetRenderFlag(10, 1);
    Script_mapmSetRenderFlag(12, 0);
  } else {
    Script_mapmSetRenderFlag(10, 0);
    Script_mapmSetRenderFlag(12, 1);
  }
  if (script_27831(8, 30, 17, 0)) {
    Script_mapmSetRenderFlag(13, 0);
  } else {
    Script_mapmSetRenderFlag(13, 1);
  }
  if (script_27831(8, 30, 19, 0)) {
    Script_mapmSetRenderFlag(14, 0);
  } else {
    Script_mapmSetRenderFlag(14, 1);
  }
  return;
}

static void script_26C97(void) {
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(6, 1);
  return;
}

static void script_26CBE(void) {
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(6, 0);
  return;
}

static void script_26CE5(void) {
  script_26B26();
  script_26BD4();
  return;
}

static void script_26CF2(void) {
  script_26CE5();
  return;
}

static void script_26CFA(void) {
  int8_t local_FF;
  if (task_C8.minutes < 30) {
    local_FF = 0;
  } else {
    local_FF = 1;
  }
  if (task_100 != task_C8.hours + task_C8.hours + local_FF) {
    script_26CE5();
    script_6696();
  }
  task_100 = task_C8.hours + task_C8.hours + local_FF;
  return;
}

static void script_26D55(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 80.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 32.0f);
  return;
}

static void script_26D79(void) {
  script_26D55();
  Script_SetupLightFromTable(0);
  Script_SetCharaFlags(CHARA_AKIR, 0);
  Script_PREP_Related(-1, 1);
  Script_GetCurrentDrawMode(1, 1);
  return;
}

__attribute__((scn_string_bindings(1, data_37DCF, 2, data_37DDF, 4, data_37DF1, 5, data_37DF6, 6, data_37E03, 7, data_37E08, 8, data_37E15, 9, data_37E1A)))
static void script_26DA6(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_66CE(1);
  Script_executeAicaCommand(2720, 2, 15);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_MotStepUtil(CHARA_AKIR, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
  if (script_21DE3((&local_F4)->x, (&local_F4)->z)) {
    local_F4.x = -18.1f;
    (&local_F4)->z = 34.9f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  script_2A74D();
  Script_LoadSND(0, 2, "bgm109.snd", 0);
  script_2A603("/scene/03/D000/", "YORU");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 5, "seqdata3.bin", "YORU");
  Script_AuthorUtils(0, 6, "seqdata4.bin", "YORU");
  Script_AuthorUtils(0, 7, "seqdata5.bin", "YORU");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(1);
  SCNF_WaitFrames_0000(1);
  script_21D43();
  script_6BA0();
  script_26CBE();
  script_678D(5);
  if (Script_shenSeqUtils(-7) == 0) {
    script_678D(6);
    if (Script_shenSeqUtils(-7) == 0) {
      script_678D(7);
    } else {
      Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
    }
  } else {
    Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  Script_AuthorUtils(AUTH_FREE, 5);
  Script_AuthorUtils(AUTH_FREE, 6);
  Script_AuthorUtils(AUTH_FREE, 7);
  script_2A627("YORU");
  script_293E2();
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_2A70B();
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  script_26D79();
  script_28E89();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_MotStepUtil(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  script_66CE(0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_2708A(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  if (Script_GetCycleManagerState((uint32_t *)&local_F4, (uint32_t *)&local_E8)) {
    while (Script_IsPlayerAllowed(0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_EC, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MIRU_TOKEI, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    local_E4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
    if (local_E4 == 0) {
      Script_ShowUIMessageFromID("SA1091A004");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_E4 == 1) {
      Script_ShowUIMessageFromID("SA1091A005");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1091A006");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    script_2A70B();
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}

static int32_t script_271F0(void) {
  int8_t local_FF;
  if ((task_C8.hours >= 19) | (task_C8.hours <= 6)) {
    (void)((task_106 = -1) & 8);
    script_26D79();
    script_26CBE();
  } else {
    (void)((task_106 = 0) & 8);
    Script_shenSetLightingMode(1);
    script_26D55();
    if (0) {
      Script_charaDisplay(SHEN_NAME('WAGK'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('WAGK'), CDM_Show);
        goto L02725F;
      }
    }
    L02725F:;
    Script_scenToggleVisibleFor(SHEN_NAME('WAGK'), 1);
    script_26C97();
  }
  local_FF = 0;
  script_27803();
  script_26CF2();
  for (;;) {
    Script_ReturnTimeDate(&task_C8);
    script_2A69A();
    Script_EVT_GetPosition(CHARA_AKIR, -1, &task_D4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &task_E0, 0x40000000);
    Script_mtwkGetSetRotation(CHARA_AKIR, &task_EC, 0x42000000);
    task_101 = Script_WeatherUtils_0(WeatherDataUtil_GetWeatherClassForDateTime, (&task_C8)->year, task_C8.month, task_C8.day, task_C8.hours, task_C8.minutes);
    script_26CFA();
    if (task_108 & 2) {
      script_2708A();
    }
    local_FF = script_2740C(local_FF);
    if (-(task_106 == 0) & -(task_10D == 0) & -(task_C8.hours >= 19) & Script_IsPlayerAllowed(0)) {
      script_26DA6();
      script_28E91();
      if (Script_scenToggleVisibleFor(SHEN_NAME('WAGK'), -1)) {
        if (-1) {
          Script_charaDisplay(SHEN_NAME('WAGK'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('WAGK'), CDM_Show);
            goto L027376;
          }
        }
        L027376:;
        Script_scenToggleVisibleFor(SHEN_NAME('WAGK'), 0);
      }
      (void)((task_106 = -1) & 0x80);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_27399(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (script_27831(arg0, arg1, arg2, arg3)) {
    if (Script_ScenDataUtil(task_144, SCND_ReadU32, arg4 * 13 + 1) & 2) {
      script_FC34(arg4);
      script_FEAD(arg4);
    }
  } else {
    if ((Script_ScenDataUtil(task_144, SCND_ReadU32, arg4 * 13 + 1) & 2) == 0) {
      script_FBFD(arg4);
      script_FEF6(arg4);
    }
  }
  return;
}

static int32_t script_2740C(int32_t arg0) {
  int32_t local_FC;
  arg0 = arg0 + 1;
  local_FC = arg0;
  if (local_FC == 0) {
    if (script_27831(7, 0, 18, 30)) {
      script_FEAD(43);
      script_FEAD(57);
      script_FEAD(48);
      script_FEAD(2);
    } else {
      script_FEF6(43);
      script_FEF6(57);
      script_FEF6(48);
      script_FEF6(2);
    }
  } else if (local_FC == 1) {
    if (script_27831(10, 0, 21, 0)) {
      script_FEAD(31);
      script_FEAD(27);
      script_FEAD(38);
      script_FEAD(26);
    } else {
      script_FEF6(31);
      script_FEF6(27);
      script_FEF6(38);
      script_FEF6(26);
    }
  } else if (local_FC == 2) {
    if (task_F8 >= 500) {
      script_27399(14, 0, 0, 0, 46);
    }
  } else if (local_FC == 3) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 957) == 0) {
      script_27399(9, 0, 20, 0, 60);
    }
  } else if (local_FC == 4) {
    script_27399(10, 0, 22, 0, 35);
  } else if (local_FC == 5) {
    if (task_F8 >= 203) {
      script_27399(10, 0, 19, 0, 51);
    }
  } else if (local_FC == 6) {
    script_27399(10, 0, 18, 0, 58);
  } else if (local_FC == 7) {
    script_27399(11, 0, 21, 0, 55);
  } else if (local_FC == 8) {
    if (task_F8 >= 401) {
      script_27399(13, 0, 0, 0, 61);
    }
  } else if (local_FC == 9) {
    script_27399(10, 0, 21, 0, 28);
  } else if (local_FC == 10) {
    if (task_F8 >= 804) {
      script_27399(10, 0, 21, 0, 1);
    }
  } else if (local_FC == 11) {
    if (task_F8 < 1902) {
      script_27399(10, 0, 20, 0, 25);
    }
  } else if (local_FC == 12) {
    script_27399(11, 0, 21, 0, 56);
  } else if (local_FC == 13) {
    script_27399(10, 0, 0, 0, 64);
  } else if (local_FC == 14) {
    script_27399(10, 0, 0, 0, 59);
  } else if (local_FC == 15) {
    script_27399(17, 0, 3, 0, 63);
  } else if (local_FC == 16) {
    script_27399(19, 0, 5, 0, 30);
  } else if (local_FC == 17) {
    script_27399(19, 0, 4, 0, 0);
  } else if (local_FC == 18) {
    script_27399(19, 0, 4, 0, 34);
  } else if (local_FC == 19) {
    script_27399(17, 0, 4, 0, 49);
  } else if (local_FC == 20) {
    script_27399(17, 0, 4, 0, 50);
  } else if (local_FC == 21) {
    script_27399(18, 30, 4, 0, 54);
  } else if (local_FC == 22) {
    script_27399(10, 0, 24, 0, 53);
  } else if (local_FC == 23) {
    script_27399(12, 0, 4, 30, 39);
  } else if (local_FC == 24) {
    script_21229();
  } else if (local_FC == 25) {
    script_2126A();
  } else if (local_FC == 26) {
    script_212CA();
  } else if (local_FC == 27) {
    script_21375();
  } else {
    arg0 = 0;
  }
  return arg0;
}

static void script_27803(void) {
  int8_t local_FF;
  local_FF = 0;
  local_FF = script_2740C(local_FF);
  while (local_FF) {
    local_FF = script_2740C(local_FF);
  }
  return;
}

static int8_t script_27831(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int8_t local_FF;
  int8_t local_FE;
  local_FE = 0;
  if (arg2 < 6) {
    arg2 = arg2 + 24;
  }
  if (task_C8.hours < 6) {
    local_FF = task_C8.hours + 24;
  } else {
    local_FF = task_C8.hours;
  }
  if ((-(local_FF > arg0) | (-(local_FF == arg0) & -(task_C8.minutes >= arg1))) & (-(local_FF < arg2) | (-(local_FF == arg2) & -(task_C8.minutes < arg3)))) {
    local_FE = 1;
  }
  return local_FE;
}

static void script_278B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = arg3;
  switch (local_FC) {
    case 0:
      SCNF_WaitFrames_0000(100);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 1:
      Script_shenClearFreeState(0);
      Script_ryoControl(PCCTL_RequestNewMode, 3);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(PCCTL_RequestNewMode, 0);
      break;
    case 2:
      Script_shenClearFreeState(0);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  Script_shenRequestGameModuleTransition(arg0, arg1, arg2);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_27A2D(void) {
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_213C6();
  script_FEF6(46);
  script_FEF6(43);
  script_FEF6(57);
  script_FEF6(48);
  script_FEF6(58);
  script_FEF6(40);
  script_FEF6(41);
  script_FEF6(35);
  script_FEF6(60);
  script_FEF6(32);
  script_FEF6(33);
  script_FEF6(2);
  script_FEF6(51);
  script_FEF6(31);
  script_FEF6(27);
  script_FEF6(55);
  script_FEF6(38);
  script_FEF6(26);
  script_FEF6(1);
  script_FEF6(28);
  script_FEF6(25);
  script_FEF6(39);
  script_FEF6(59);
  script_FEF6(61);
  script_FEF6(64);
  script_FEF6(56);
  script_FEF6(63);
  script_FEF6(30);
  script_FEF6(0);
  script_FEF6(34);
  script_FEF6(50);
  script_FEF6(49);
  script_FEF6(54);
  script_FEF6(53);
  {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, 365);
    local_FC = local_FC & -65;
    local_FC = local_FC & -129;
    local_FC = local_FC | 0x80;
    Script_ScenDataUtil(task_144, SCND_WriteU32, 365, local_FC);
  }
  {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_144, SCND_ReadU32, 716);
    local_FC = local_FC & -65;
    local_FC = local_FC & -129;
    local_FC = local_FC | 0x80;
    Script_ScenDataUtil(task_144, SCND_WriteU32, 716, local_FC);
  }
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 957) == 0) & (Script_shenGetStats(STATS_CASH) >= 158000)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 957, 1);
  }
  if (Script_GetCurrentEntryPoint() != 99) {
    script_FB37();
    script_FC34(29);
    script_27803();
  }
  Script_scenCreateTask(script_219C7, 0);
  end:;
}

static void script_27C43(void) {
  Script_scenCreateTask(script_27A2D, 0);
  Script_scenCreateTask(script_268F8, 1, &data_367C8);
  Script_scenCreateTask(script_271F0, 0);
  return;
}

static int32_t script_27C6C(void) {
  int32_t local_FC;
  int32_t local_F8;
  script_784B();
  for (;;) {
    local_FC = script_789D();
    local_F8 = local_FC;
    if (local_F8 == 130) {
      local_F8 = script_7882();
      script_7875();
    } else if (local_F8 == 131) {
      script_7875();
    } else {
      if (local_FC != -1) {
        script_7875();
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_27CCB(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_37EB0, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_37E78, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_37E40, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_37E5C, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_37E94, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_27E03(int32_t arg0) {
  script_27E1B(arg0);
  return;
}

static void script_27E0F(int32_t arg0) {
  script_27E1B(arg0);
  return;
}

static void script_27E1B(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L027E54;
    }
  }
  L027E54:;
  Script_ryoSetChara(arg0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_27CCB();
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  {
    int32_t local_FC;
    local_FC = Script_GetSceneID();
    if (local_FC == 2) {
      Script_LoadVoiceFile(1, "FREE02");
    } else if (local_FC == 3) {
      Script_LoadVoiceFile(1, "FREE03");
    } else if (local_FC == 4) {
      Script_LoadVoiceFile(1, "FREE04");
    } else {
      Script_LoadVoiceFile(1, "FREE01");
    }
  }
  Script_CPCTRelated(arg0, 32, 89);
  Script_clmdSetFlags(arg0, 8, 1);
  return;
}

static void script_27F27(int32_t arg0) {
  script_27E0F(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_27F3E(int32_t arg0) {
  Vector3i local_F4;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0);
  (&local_F4)->y = ((&local_F4)->y - arg0 + 10922) & 0xFFFF;
  if ((&local_F4)->y < 21845) {
    (void)-1;
  } else {
    (void)0;
  }
  return;
}

static void script_27F7F(int32_t arg0) {
  Vector3i local_F4;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
  (&local_F4)->y = ((&local_F4)->y - arg0 + 10922) & 0xFFFF;
  if ((&local_F4)->y < 21845) {
    (void)-1;
  } else {
    (void)0;
  }
  return;
}

static void script_27FC3(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if ((local_FC == 0) | (local_FC == 5)) {
    Script_executeAicaCommand(5288, 0, 0);
  } else if (local_FC == 10) {
    Script_executeAicaCommand(70824, 0, 0);
  } else if (local_FC == 20) {
    Script_executeAicaCommand(136360, 0, 0);
  } else if (local_FC == 30) {
    Script_executeAicaCommand(201896, 0, 0);
  } else {
    Script_executeAicaCommand(267432, 0, 0);
  }
  return;
}

static void script_28041(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_27FC3(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_27FC3(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_27FC3(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_27FC3(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_27FC3(5);
  } else {
    script_27FC3(0);
  }
  return;
}

static int32_t script_280C0(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetFightingStats(-1, 2) == 0) {
      local_FC = local_FC + 1;
    }
  }
  if (arg0) {
    int32_t local_F8;
    local_F8 = Script_PlayerStatsUtils(PS_GetShugyoCourseActiveFramesSlot, 3);
    local_F8 = local_F8 + local_FC;
    Script_PlayerStatsUtils(PS_SetShugyoCourseActiveFramesSlot, 3, local_F8);
  } else {
    int32_t local_F8;
    local_F8 = Script_PlayerStatsUtils(PS_GetShugyoCourseActiveFramesSlot, 2);
    local_F8 = local_F8 + local_FC;
    Script_PlayerStatsUtils(PS_SetShugyoCourseActiveFramesSlot, 2, local_F8);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_37EE8, 3, data_37F02, 7, data_37F26)))
static int32_t script_28146(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = 0;
  local_FC = task_C8.hours * 100 + task_C8.minutes;
  local_F4 = task_F8;
  if (local_F4 == 1901) {
    if (local_FC > 1100) {
      Script_LoadVoiceFile(1, "E1040");
      if ((int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f) > 0) {
        Script_ShowUIMessageFromID("E1040A028");
      } else {
        Script_ShowUIMessageFromID("E1040A029");
      }
      local_F8 = 1;
    }
  } else {
    if ((local_F4 == 2202) | (local_F4 == 2203)) {
      Script_LoadVoiceFile(1, "E1040");
      local_F4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_F4 == 0) {
        Script_ShowUIMessageFromID("E1040A030");
      } else if (local_F4 == 1) {
        Script_ShowUIMessageFromID("E1040A031");
      } else {
        Script_ShowUIMessageFromID("E1040A032");
      }
      local_F8 = 1;
      goto L028227;
    }
  }
  L028227:;
  if (local_F8) {
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadVoiceFile(0, "E1040");
  }
  if ((local_FC >= 2200) | (local_FC <= 829)) {
    local_F8 = 1;
  }
  return local_F8;
}

static int32_t script_28269(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = 0;
  local_FC = task_C8.hours * 100 + task_C8.minutes;
  local_F4 = task_F8;
  switch (local_F4) {
    case 602:
      if (local_FC > 1430) {
        local_F8 = 1;
      }
      break;
    case 1901:
      if (local_FC > 1200) {
        local_F8 = 1;
      }
      break;
  }
  if (local_FC >= 2200) {
    local_F8 = 1;
  }
  if (~local_F8 & -(local_FC == 1900)) {
    local_F8 = 1;
  }
  return local_F8;
}

static void script_282F0(void) {
  int32_t local_FC;
  local_FC = 0;
  Script_wazaUtils(MOVE_UI_CMD_OPEN_MOVES_SCREEN, 0, 0, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetControllerInput(1) == 0x200) {
      break;
    }
    if (local_FC < 1200) {
      local_FC = local_FC + 1;
    } else {
      break;
    }
  }
  Script_wazaUtils(MOVE_UI_CMD_REQUEST_CLOSE_MOVES_SCREEN, 0, 0, 0);
  return;
}

static int32_t script_28349(void) {
  Script_seqEnterEvtCam(2900, CHARA_AKIR, CHARA_AKIR);
  Script_shenClearFreeState(0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsInEventCam() == 0) {
      break;
    }
  }
  Script_shenClearFreeState(1);
  end:;
}

static int32_t script_28383(int32_t arg0) {
  Script_EVT_SetPosition(arg0, 0x78000000, &data_37F2C);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_ccowUtils(arg0, CCOW_DisableAll);
}

static void script_283A6(void) {
  SCNF_WaitFrames_0000(2);
  while (Script_IsInEventCam() != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_283C6(int32_t arg0) {
  Script_EVT_SetPosition(arg0, 0x78000000, &data_37F38);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_ccowUtils(arg0, CCOW_DisableAll);
}

static int32_t script_283E9(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 5, "e1shugyo.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static void script_28434(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A001");
  } else {
    Script_ShowUIMessageFromID("E1040A003");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2847B(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1040A009");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1040A010");
      break;
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_284CE(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A012");
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1040A013");
  } else {
    Script_ShowUIMessageFromID("E1040A014");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2852A(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A018");
  } else {
    Script_ShowUIMessageFromID("E1040A020");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_28571(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A015");
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1040A016");
  } else {
    Script_ShowUIMessageFromID("E1040A017");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_37FC9)))
static void script_285CD(int8_t arg0) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  Script_LoadVoiceFile(1, "E1040");
  if (arg0) {
    Script_scenCreateTask(script_28383, 1, SHEN_NAME('LUV2'));
  } else {
    Script_scenCreateTask(script_283C6, 1, SHEN_NAME('LUV2'));
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  script_2A74D();
  Script_scenCreateTask(script_283E9, 0);
  Script_scenCreateTask(script_28349, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MIMAWASU_K, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_37FCF)))
static void script_286A3(void) {
  Script_vfToggleBattleState(scnPostResultsSetup);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_LoadVoiceFile(0, "E1040");
  while (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE, 0) != 7) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  return;
}

static void script_2871A(void) {
  int32_t local_FC;
  local_FC = 0;
  while (Script_GetFightingStats(-3, 2)) {
    if (local_FC > 1200) {
      Script_vfToggleBattleState(scnExitConfirmPending);
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_28757(void) {
  int32_t local_FC;
  local_FC = 0;
  while (Script_GetFightingStats(-4, 2)) {
    if (local_FC > 300) {
      Script_vfToggleBattleState(scnExitConfirmPending);
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_28794(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int8_t local_F9;
  int32_t local_F4;
  local_F4 = arg0;
  if (local_F4 == 4) {
    local_FB = 58;
    local_FA = 34;
    local_F9 = local_FB;
  } else {
    local_FB = 37;
    local_FA = 34;
    local_F9 = local_FA;
  }
  if (SCNF_RequestActionSelectorFromParams_0008(0xFFFF, local_FB, local_FA, 0xFFFF, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL) == local_F9) {
    local_FC = 1;
  } else {
    local_FC = -1;
  }
  return local_FC;
}

static void script_28805(int32_t arg0) {
  Script_uiUtils(0, 15, 0);
  Script_vfSetup(0, 20, 20, 0, 0);
  task_0 = 0x52494B41;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 5;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  task_0 = 0x3256554C;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 5;
  *(&task_0 + 5) = 21;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('LUV2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  if (arg0) {
    Script_vfStageAssignRingBoundary(&data_37FD8, 4.8f);
  }
  SCNF_WaitFrames_0000(1);
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  if (arg0) {
    Vector3i local_F4;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_37FE4);
    (&local_F4)->y = 0xA000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  } else {
    Vector3i local_F4;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_37FF0);
    (&local_F4)->y = 0x3500;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(2903, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_283A6();
  SCN_Enqueue_VFGI(2, 99, 1.0f, 4, -1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_scenCreateTask(script_280C0, 1, arg0);
  return;
}

static int32_t script_28A68(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_GetFightingStats(-1, 2) == 1) {
    local_FC = 1;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    local_FC = 2;
  }
  if (script_28269()) {
    local_FC = 2;
  }
  return local_FC;
}

static void script_28AB0(void) {
  script_2852A();
  return;
}

static void script_28AB8(void) {
  SCN_Enqueue_VFGI(2, 99, 1.0f, 99, -1);
  return;
}

static void script_28ACC(int8_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  uint8_t unused_FC[2];
  int32_t local_F8;
  local_FE = 0;
  local_FF = 0;
  if (~Script_IsPlayerAllowed(0)) {
    local_FE = 1;
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_NEAR_NPC_COUNT_CLAMPED) > 0) {
    Script_sprintf("training_npc_too_close:  set\012");
    task_104 = 1;
    local_FE = 1;
  } else {
    if (~arg0 & Script_scenCheckCharaExist(CHARA_YAMA)) {
      if (~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_YAMAGISHI_EVENT_COMPLETE)) {
        Script_sprintf("\012yamagisi ga iru!\012\012");
        local_FE = 1;
      }
    }
    if (~arg0 & Script_scenCheckCharaExist(SHEN_NAME('RKKI'))) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_RYU_EVENT_ACTIVE) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE)) {
        Script_sprintf("\012ryu ga iru!\012\012");
        local_FE = 1;
      }
    }
  }
  if (script_28146()) {
    local_FE = 1;
  }
  if (local_FE == 0) {
    script_25051();
    script_285CD(arg0);
    local_FF = 1;
  }
  for (;;) {
    int32_t local_F4;
    local_F8 = 0;
    script_2871A();
    script_28757();
    if (local_FE == 1) {
      break;
      local_FF = 14;
    }
    SCNF_WaitFrames_0000(1);
    local_F4 = local_FF;
    if (local_F4 == 1) {
      script_28434();
      local_FF = 2;
    } else if (local_F4 == 2) {
      script_2847B();
      local_FF = 4;
    } else if ((local_F4 == 3) | (local_F4 == 13) | (local_F4 == 14)) {
      break;
    } else if ((local_F4 == 4) | (local_F4 == 12)) {
      local_FF = local_FF + script_28794(local_FF);
    } else if (local_F4 == 5) {
      local_FE = 2;
      script_284CE();
      script_28805(arg0);
      local_FF = 6;
    } else if (local_F4 == 6) {
      local_FF = local_FF + script_28A68(arg0);
    } else if ((local_F4 == 7) | (local_F4 == 8)) {
      local_FF = local_FF - 7 + 9;
    } else {
      switch (local_F4) {
        case 9:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_28AB0();
            local_FF = 12;
          }
          break;
        case 10:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_28AB0();
            local_FF = 14;
          }
          break;
        case 11:
          script_28AB8();
          local_FF = 6;
          break;
      }
    }
  }
  if (local_FE != 1) {
    Script_sprintf("training_npc_too_close reset before training starts \012");
    task_104 = 0;
    script_286A3();
  }
  if (local_FE == 2) {
    int32_t local_F4;
    script_282F0();
    local_F4 = arg0;
    if (local_F4 == 0) {
      Script_PlayerStatsUtils(PS_SetShugyoCourseClearCountSlot, 2, Script_PlayerStatsUtils(PS_GetShugyoCourseClearCountSlot, 2) + 1);
    } else {
      Script_PlayerStatsUtils(PS_SetShugyoCourseClearCountSlot, 3, Script_PlayerStatsUtils(PS_GetShugyoCourseClearCountSlot, 3) + 1);
    }
  }
  if (local_FE != 1) {
    script_25071(0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
    Script_SetGameStateTo(AUTH_None);
    Script_SetCPAMFlags(CHARA_AKIR, 1);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_shenClearFreeState(1);
    Script_ryoControl(RYO_RestoreControl);
    SCNF_WaitFrames_0000(1);
    Script_uiUtils(0, 0, 0);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  }
  return;
}

static void script_28E00(void) {
  if (-(task_F8 >= 1700) & -(task_F8 <= 3000) & -(task_C8.hours >= 19)) {
    Script_CycleManagerUtils(CYCLE_CMD_MOBJ_PUSH_TEMP_STATE_41, 0x414D4A54);
    Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, 0x414D4A54, 0);
    Script_CycleManagerUtils(CYCLE_CMD_MOBJ_SET_TEMP_STATE_41_POS, 0x414D4A54, 19.682301f, 0.1f, 31.281099f, 0x1600);
    Script_CycleManagerUtils(CYCLE_CMD_MOBJ_SET_TEMP_STATE_41_WORD5, 0x414D4A54, 28697);
  }
  return;
}

static void script_28E68(void) {
  if (task_F8 >= 2302) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5F4D4F54, 1);
  }
  script_28E00();
  return;
}

static void script_28E89(void) {
  script_28E00();
  return;
}

static void script_28E91(void) {
  return;
}

static void script_28E94(void) {
  Script_scenSetMapEvTbl((uint32_t *)&data_38074, 6);
  return;
}

static void script_28EA2(void) {
  return;
}

static void script_28EA5(void) {
  return;
}

static void script_28EA8(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    if (Script_IsPlayerAllowed(0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_278B0(3, SHEN_NAME('JD00'), 0, 1);
    }
  }
  return;
}

static void script_28ED9(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    if (Script_IsPlayerAllowed(0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_278B0(3, SHEN_NAME('JD00'), 1, 1);
    }
  }
  return;
}

static void script_28F0A(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    if (Script_IsPlayerAllowed(0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_278B0(3, SHEN_NAME('DGCT'), 0, 2);
    }
  }
  return;
}

static void script_28F3B(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    if (Script_IsPlayerAllowed(0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_278B0(3, SHEN_NAME('DCBN'), 0, 2);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3808C, 2, data_380AB, 3, data_380BB, 6, data_380DB)))
static void script_28F6C(void) {
  int8_t local_FF;
  float local_F0[3];
  Vector3i local_E4;
  Script_timeSetUpdateEnabled(0);
  (void)((task_108 = 0) & 0x10);
  (void)((task_107 = 0) & 0x10);
  (void)((task_10A = 0) & 0x10);
  (void)((task_10B = 0) & 0x10);
  *(int8_t *)&task_102 = 0;
  *((int8_t *)&*(int32_t *)&task_102 + 1) = 0;
  task_104 = 0;
  (void)((task_109 = 0) & 1);
  (void)((task_10C = 0) & 1);
  (void)((task_10D = -1) & 1);
  (void)((task_10F = 0) & 1);
  task_105 = 5;
  (void)((task_112 = 0) & 1);
  if (Script_ryoSleepUtils(0)) {
    (void)((task_10E = -1) & 1);
  } else {
    (void)((task_10E = 0) & 1);
  }
  (void)((task_110 = 0) & 1);
  (void)((task_111 = 0) & 1);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 1.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 1.0f);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_uiUtils(0, 0, 0);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_ReturnTimeDate(&task_C8);
  if (task_C8.minutes < 30) {
    local_FF = 0;
  } else {
    local_FF = 1;
  }
  task_100 = task_C8.hours + task_C8.hours + local_FF;
  script_2A69A();
  if ((Script_GetCurrentEntryPoint() == 9) & (task_10E == 0)) {
    Script_scenSetAssignCVWK(CHARA_AKIR, 0, 1);
  }
  Script_SetMapFlags(0, 1);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 99);
  script_27E0F(CHARA_AKIR);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  script_26AB2();
  script_2A603("/scene/03/D000/", (const char *)&data_3809C);
  script_1E0C6();
  task_FC = Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MOBJ.BIN");
  script_2A603("/scene/03/D000/", "SHUKI");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  script_F837(CHARA_AKIR, (DSWKDoorEntry *)data_2F9E4, (int32_t *)&data_3074C, (DoorResourceDescriptor *)&data_30B74, (MapObjectRecord *)data_30DB4, 10, 0);
  script_28E94();
  script_25029();
  script_2659E();
  script_293AC();
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_PreserveAccumulator_01CC(17);
  (void)1;
  Script_motnUtils(MOTN_SetBudgetCapacity, 5);
  Script_scenCreateTask(script_27C6C, 0);
  Script_scenCreateTask(script_21F1A, 0);
  script_1178D(0x304C4554, 9998, 0x421A21CB, 0x3F7851EC, 0x41185604, 0x43803FB8);
  script_21D43();
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/03/D000", (int32_t)"SCRTEST.BIN", 0x30303044);
  (void)((task_108 = -1) & 1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  if ((Script_GetCurrentEntryPoint() == 0) & (task_10E == 0)) {
    script_6702();
    script_231AD(17);
    script_2A627("BUSS");
    script_293E2();
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 100);
  }
  script_28E68();
  if (task_10E == -1) {
    Script_ryoSleepUtils(1);
    if (Script_InitINSHForScene((Vector3 *)0x30303044, (Vector3i *)local_F0, (int32_t)&local_E4) == 1) {
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, (const Vector3 *)local_F0);
      Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E4, 0);
    } else {
      Script_EVT_GetPosition(CHARA_AKIR, -1, (Vector3 *)local_F0, 0);
      if (script_21DE3(*local_F0, local_F0[2])) {
        local_F0[0] = -18.1f;
        local_F0[1] = 0.0f;
        local_F0[2] = 34.9f;
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, (const Vector3 *)local_F0);
      }
    }
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((Script_GetCurrentEntryPoint() != 0) | (task_10E == -1)) {
    script_6702();
    Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  }
  if (task_112 == 0) {
    script_22557();
  }
  (void)((task_10D = 0) & 4);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ryoSleepUtils(2, 1);
  Script_shenClearFreeState(1);
  Script_timeSetUpdateEnabled(1);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

__attribute__((scn_string_bindings(0, data_380E0)))
static void script_293AC(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 733) == 0)) {
    Script_LoadVoiceFile(1, "SA1053");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_380E7, 2, data_380FE, 3, data_38105, 4, data_3810A, 5, data_38117, 6, data_3811C, 7, data_38121, 8, data_38126, 10, data_3813D, 11, data_38144, 12, data_38149, 13, data_38156, 14, data_3815B, 15, data_38160, 16, data_38165, 18, data_3817C, 19, data_38183, 20, data_38188, 21, data_38195, 22, data_3819A, 23, data_3819F, 24, data_381A4, 26, data_381BB, 27, data_381C2, 28, data_381C7, 29, data_381D4, 30, data_381D9, 31, data_381DE, 32, data_381E3, 34, data_381FA, 35, data_38201, 36, data_38206, 37, data_38213, 38, data_38218, 39, data_3821D)))
static void script_293E2(void) {
  if (-(task_C8.month == 12) & -(task_C8.day >= 15) & -(task_C8.day <= 25)) {
    if (task_C8.day >= 24) {
      Script_LoadSND_Wrapper("system1.snd", "amb022.snd", "SELECT", "FREE", "f1dobuit.snd", "FREE", "FREE", "FREE");
    } else if (task_C8.day >= 19) {
      Script_LoadSND_Wrapper("system1.snd", "amb051.snd", "SELECT", "FREE", "f1dobuit.snd", "FREE", "FREE", "FREE");
    } else {
      Script_LoadSND_Wrapper("system1.snd", "amb050.snd", "SELECT", "FREE", "f1dobuit.snd", "FREE", "FREE", "FREE");
    }
  } else if ((task_C8.month == 1) & (task_C8.day <= 5)) {
    Script_LoadSND_Wrapper("system1.snd", "amb052.snd", "SELECT", "FREE", "f1dobuit.snd", "FREE", "FREE", "FREE");
  } else {
    Script_LoadSND_Wrapper("system1.snd", "amb021.snd", "SELECT", "FREE", "f1dobuit.snd", "FREE", "FREE", "FREE");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_38222)))
static void script_2952B(void) {
  script_2A627("SHUKI");
  return;
}

__attribute__((scn_string_bindings(0, data_38228, 1, data_38238)))
static void script_2953A(void) {
  script_2A603("/scene/03/D000/", "SHUKI");
  return;
}

void script_2954E(void) {
  script_28F6C();
  return;
}

static void script_29556(int32_t arg0) {
  if (arg0 < 0) {
    (void)-arg0;
    return;
  } else {
    (void)arg0;
    return;
  }
}

static void script_29574(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static void script_29593(int32_t arg0) {
  if (arg0 != 0) {
    Script_sprintf("WAZA Warning : handle is not NULL, and create same handle !\012");
  }
  return;
}

static void script_295A8(int32_t arg0, int32_t arg1) {
  for (;;) {
    if (*(&task_3D0 + 3) & 2) {
      {
        Vector3 local_F4;
        Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_sprintf("POS POS_RYOU_0 = %f, %f, %f.\012", (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
      }
      {
        Vector3 local_F4;
        Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_sprintf("POS POS_KAIS_0 = %f, %f, %f.\012", (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
      }
      {
        int32_t unused_F4[3];
        Vector3i local_E8;
        Script_mtwkGetSetRotation(arg0, &local_E8, 0x40000000);
        Script_sprintf("define ANG_RYOU_0 DEG(%d.0)\012", (&local_E8)->y * 360 / 0x10000);
      }
      {
        int32_t unused_F4[3];
        Vector3i local_E8;
        Script_mtwkGetSetRotation(arg1, &local_E8, 0x40000000);
        Script_sprintf("define ANG_KAIS_0 DEG(%d.0)\012", (&local_E8)->y * 360 / 0x10000);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_2965E(int32_t arg0) {
  task_8F4 = arg0;
  *(&task_8F4 + 1) = 0;
  return (int32_t)&task_8F4;
}

static void script_29676(int32_t arg0) {
  Script_sprintf("%s\012", script_2965E(arg0));
  return;
}

static void script_2968C(int32_t arg0) {
  Script_sprintf("Konnna Error ha Detekonai hazu : %d\012", arg0);
  return;
}

static void script_2969A(void) {
  Vector3 local_F4;
  SCNF_WaitFrames_0000(60);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 0.1f;
  for (;;) {
    if (*(&task_3C0 + 1) & 0x4000) {
      local_F4.x = (&local_F4)->x - 0.1f;
    }
    if (*(&task_3C0 + 1) & 0x8000) {
      local_F4.x = (&local_F4)->x + 0.1f;
    }
    if (*(&task_3C0 + 1) & 0x1000) {
      (&local_F4)->z = (&local_F4)->z - 0.1f;
    }
    if (*(&task_3C0 + 1) & 0x2000) {
      (&local_F4)->z = (&local_F4)->z + 0.1f;
    }
    Script_TranslateRotateScale_Wrapper(1, 0x3F800000, &local_F4, &data_38348, &data_3833C);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_29764(void) {
  return;
}

static void script_29767(void) {
  task_3F0 = 0;
  while (1) {
    task_3F0 = task_3F0 + 1;
    task_3C0 = Script_GetInputData(0, 0);
    *(&task_3C0 + 1) = Script_GetInputData(1, 0);
    *(&task_3C0 + 2) = Script_GetInputData(2, 0);
    *(&task_3C0 + 3) = Script_GetInputData(3, 0);
    task_3D0 = Script_GetInputData(0, 1);
    *(&task_3D0 + 1) = Script_GetInputData(1, 1);
    *(&task_3D0 + 2) = Script_GetInputData(2, 1);
    *(&task_3D0 + 3) = Script_GetInputData(3, 1);
    task_3E0 = Script_GetInputData(0, 2);
    *(&task_3E0 + 1) = Script_GetInputData(1, 2);
    *(&task_3E0 + 2) = Script_GetInputData(2, 2);
    *(&task_3E0 + 3) = Script_GetInputData(3, 2);
    if (*(&task_3D0 + 1) & 2) {
      Script_facemotUtils(CHARA_AKIR, 0, 0);
      Script_facemotUtils(CHARA_KISY, 0, 0);
      Script_facemotUtils(CHARA_TAIJ, 0, 0);
      Script_facemotUtils(CHARA_AKIR, 1, 0);
      Script_facemotUtils(CHARA_KISY, 1, 0);
      Script_facemotUtils(CHARA_TAIJ, 1, 0);
      Script_sprintf("Entry Model!\012");
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_298B5(int32_t arg0) {
  task_3F4 = arg0;
  Script_seqEnterEvtCam(arg0, 0, AUTH_None);
  return;
}

static int32_t script_298C9(int32_t arg0, int32_t arg1, int32_t arg2) {
  SCNF_WaitFrames_0000(arg2);
  script_298B5(arg0);
  SCNF_WaitFrames_0000(1);
  Script_AllocSpline(arg1);
  end:;
}

static void script_298EB(int32_t arg0, int32_t arg1, int32_t arg2) {
  arg2 = arg2 / 2;
  if (arg2) {
    arg2 = arg2 - 1;
    Script_scenCreateTask(script_298C9, 3, arg0, arg1, arg2);
  } else {
    script_298B5(arg0);
    SCNF_WaitFrames_0000(1);
    Script_AllocSpline(arg1);
  }
  return;
}

static void script_29931(int32_t arg0, int32_t arg1, int32_t arg2) {
  task_3F4 = arg0;
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return;
}

static void script_29945(int32_t arg0) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
  }
  return;
}

static void script_2995F(void) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsInEventCam() == 0) {
      break;
    }
  }
  return;
}

static void script_2997A(int32_t arg0) {
  script_298B5(arg0);
  SCNF_WaitFrames_0000(2);
  for (;;) {
    script_2995F();
    script_298B5(arg0);
    SCNF_WaitFrames_0000(2);
  }
  return;
}

static void script_299A7(int32_t arg0, int32_t arg1) {
  while (arg0) {
    SCNF_WaitFrames_0000(1);
    arg0 = arg0 - 1;
  }
  Script_executeAicaCommand(arg1, 0, 0);
  return;
}

static void script_299CE(void) {
  task_8FC = 0;
  return;
}

static void script_299D8(void) {
  for (;;) {
    if (task_8FC == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_299F3(void) {
  task_8FC = 1;
  return;
}

static void script_299FD(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0;
  local_FC = 0;
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if ((Script_convGetIsActive(0) == 0) & (arg0 == 0)) {
      break;
    }
    if (arg0 > 0) {
      arg0 = arg0 - 1;
    }
    if (Script_convGetIsActive(0) != 0) {
      local_FC = local_FC + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_29A61(float arg0, float arg1, float arg2, float arg3) {
  int32_t unused_FC;
  int32_t local_F4[2];
  arg0 = arg2 - arg0;
  arg1 = arg3 - arg1;
  return local_F4[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, arg0, arg1);
}

static int32_t script_29A88(int32_t arg0, int32_t arg1) {
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

static int32_t script_29B00(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    task_900[arg1] = Script_wmptUtils(arg0, arg3, arg2);
    if (task_900[arg1] == 0) {
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      local_FF = local_FF + 1;
      if (local_FF > 10) {
        Script_sprintf("WZ ModoriMove Limit Over !!!!!!!!!!!\012");
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(arg0, 0x78000000, arg3);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_charaToggleMotEnabled(arg0, 1);
  end:;
}

static void script_29BAC(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_DC[3];
  int32_t local_D8;
  int32_t unused_C8[4];
  int32_t local_C0[2];
  int32_t local_BC;
  int8_t local_BB;
  uint8_t unused_B8[3];
  int32_t local_B4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0x40000000);
  if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, (const Vector3 *)arg2) >= 0.06f)) {
    task_900[arg1] = 0;
  } else {
    local_DC[0] = *(float *)arg2 - (&local_F4)->x;
    local_DC[2] = *(float *)(arg2 + 8) - (&local_F4)->z;
    local_C0[1] = script_29A61((&local_F4)->x, (&local_F4)->z, *(float *)arg2, *(float *)(arg2 + 8));
    local_D8 = script_29A88(local_C0[1], (&local_E8)->y);
    local_BB = 0;
    if ((local_D8 < -0x7000) | (local_D8 > 0x7000)) {
      local_BC = 839;
    } else if (local_D8 < -0x5000) {
      local_BC = 842;
    } else if (local_D8 < -0x3000) {
      local_BC = 841;
    } else if (local_D8 < -0x1000) {
      local_BC = 1274;
    } else if (local_D8 < 0x1000) {
      local_BC = 840;
    } else if (local_D8 < 0x3000) {
      local_BC = 863;
    } else if (local_D8 < 0x5000) {
      local_BC = 861;
    } else {
      local_BC = 1273;
    }
    Script_PlayCharaMotion(arg0, local_BC, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    if (local_BB) {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    } else {
      Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
    }
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
    Script_SetCharaMotionPlaybackRate(arg0, 2.0f);
    Script_charaToggleMotEnabled(arg0, 1);
    Script_motmToggleFlags(arg0, SCEN_Disable, 4);
    task_900[arg1] = 1000;
    local_B4 = Script_scenCreateTask(script_29B00, 4, arg0, arg1, local_BC, arg2);
  }
  return;
}

static void script_29D72(int32_t arg0) {
  for (;;) {
    if (task_900[arg0] == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_29D94(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  Vector3i local_D0;
  Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg0, &local_DC, 0);
  (&local_DC)->y = ((&local_DC)->y + arg2) & 0xFFFF;
  (&local_D0)->y = (&local_DC)->y;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_D0, 0);
  local_F4.x = *(float *)(arg1 + 8) * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y) + *(float *)arg1 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y);
  (&local_F4)->z = *(float *)(arg1 + 8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y) - *(float *)arg1 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y);
  local_F4.x = (&local_F4)->x + (&local_E8)->x;
  (&local_F4)->y = (&local_E8)->y;
  (&local_F4)->z = (&local_F4)->z + (&local_E8)->z;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_29E6F(void) {
  for (;;) {
    task_404 = task_404 + 1;
    task_408 = task_408 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_29E93(int32_t arg0) {
  arg0 = 0;
  *(int8_t *)task_428[arg0] = *(int8_t *)&task_41C;
  *(int8_t *)&task_428[arg0][1] = *((int8_t *)&task_41C + 1);
  *(int8_t *)&task_428[arg0][2] = *((int8_t *)&task_41C + 2);
  *(int8_t *)&task_428[arg0][3] = *((int8_t *)&task_41C + 3);
  *(int8_t *)&task_428[arg0][4] = *((int8_t *)&task_41C + 4);
  *(int8_t *)&task_428[arg0][5] = *((int8_t *)&task_41C + 5);
  *(int8_t *)&task_428[arg0][6] = *((int8_t *)&task_41C + 6);
  *(int8_t *)&task_428[arg0][7] = *((int8_t *)&task_41C + 7);
  *(int32_t *)&task_428[arg0][8] = *(int32_t *)(&task_41C + 8);
  return;
}

static void script_29F53(void) {
  (void)0;
  return;
}

static void script_29F59(void) {
  *(int8_t *)&task_428[0][2];
  return;
}

static void script_29F64(int32_t arg0) {
  int32_t unused_F8[2];
  for (;;) {
    if (arg0 == 9) {
      if ((task_400 != task_3FC) & (task_3FC % 3 == 0)) {
        task_418 = 1;
      }
      if ((task_404 != 0) & (task_404 % 450 == 0)) {
        task_418 = 1;
        task_419 = 1;
      }
    } else {
      if (((task_408 != 0) & (task_408 % 300 == 0)) | ((task_400 != task_3FC) & (task_3FC % 3 == 0))) {
        task_418 = 1;
      }
      if ((task_404 != 0) & (task_404 % 2700 == 0)) {
        task_418 = 1;
        task_41B = 1;
      } else {
        if ((task_404 != 0) & (task_404 % 1800 == 0)) {
          task_418 = 1;
          task_41A = 1;
          goto L02A02B;
        }
      }
    }
    L02A02B:;
    task_400 = task_3FC;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A03F(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_8EF = 1;
  local_F0 = 0;
  for (;;) {
    if (task_8EF == 0) {
      break;
    }
    Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(arg0, 12, &local_F4, 0);
    if (arg1 != -1) {
      local_F0 = local_F0 + 1;
      if (local_F0 > arg1) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A0A4(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_8F0 = 1;
  local_F0 = 0;
  for (;;) {
    if (task_8F0 == 0) {
      break;
    }
    Script_EVT_GetPosition(arg0, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 0);
    if (arg1 != -1) {
      local_F0 = local_F0 + 1;
      if (local_F0 > arg1) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A109(void) {
  int32_t local_FC;
  task_8EE = 0;
  local_FC = 0;
  for (;;) {
    *((int8_t *)task_8D8 + local_FC) = 0;
    *((int8_t *)task_8E4 + local_FC) = 0;
    local_FC = local_FC + 1;
    if (local_FC >= 10) {
      break;
    }
  }
  return;
}

static void script_2A145(void) {
  for (;;) {
    if (task_8EE == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A160(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  SCNF_WaitFrames_0000(arg3);
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 12, &local_F4, 0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_motmSetBlendFrames(arg0, arg2);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(arg2 + 20);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  return;
}

static void script_2A1D4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_F4[3];
  script_299F3();
  task_8EE = 1;
  script_2A160(arg0, arg1, arg2, arg3);
  task_8EE = 0;
  return;
}

static void script_2A1F9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  script_299F3();
  task_8EE = 1;
  script_2A160(arg0, arg1, arg2, arg3);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
  task_8EE = 0;
  return;
}

static void script_2A242(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  script_299F3();
  task_8EE = 1;
  script_2A160(arg0, arg1, arg2, arg3);
  local_F0 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  if (local_F0 == 0) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
    goto L02A2A0;
  }
  L02A2A0:;
  task_8EE = 0;
  return;
}

static void script_2A2A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  script_299F3();
  task_8EE = 1;
  SCNF_WaitFrames_0000(arg3);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
  Script_PlayCharaMotion(arg0, 20998, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(30);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  script_2A160(arg0, arg1, arg2, 0);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
  script_29945(arg0);
  Script_PlayCharaMotion(arg0, 21000, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_8EE = 0;
  return;
}

static void script_2A370(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  task_8EE = 1;
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  SCNF_WaitFrames_0000(arg2);
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  task_8EE = 0;
  script_299F3();
  return;
}

static void script_2A3C6(void) {
  task_928 = 1;
  task_92C = 0;
  for (;;) {
    if (task_928) {
      task_928 = task_928 - 1;
      if (task_928 == 0) {
        task_92C = 0;
        Script_LCDM_Utils(0, (const void *)task_92C);
        Script_sicnUtils(2, 0x800, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A41B(int32_t arg0) {
  task_92C = arg0;
  if (task_92C == 0) {
    task_928 = 0;
    Script_LCDM_Utils(1, 0);
    Script_sicnUtils(2, 0x800, 0);
  } else {
    task_928 = 600;
    Script_LCDM_Utils(1, (const void *)1);
    Script_sicnUtils(0, 0x800, 0);
  }
  Script_LCDM_Utils(0, (const void *)task_92C);
  return;
}

static void script_2A475(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    Script_sprintf("WazaDen: Load PackFile %s:%s\012", arg0, arg1);
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    Script_sprintf("WazaDen: Now Loading PackFile.\012");
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A4C5(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  if (local_FC == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    while (local_FC == 0) {
      SCNF_WaitFrames_0000(1);
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    }
    L02A522:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L02A522;
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  }
  (void)local_FC;
  return;
}

static int32_t script_2A543(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  if (local_FC == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    while (local_FC == 0) {
      SCNF_WaitFrames_0000(1);
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    }
    L02A597:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L02A597;
  }
  return local_FC;
}

static void script_2A5AF(int32_t arg0, int32_t arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  return;
}

static void script_2A5C6(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A5E0(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A603(const char *arg0, const char *arg1) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1)) {
  } else {
    script_2A5E0(arg0, arg1);
    script_2A5C6();
  }
  return;
}

static void script_2A627(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) == 0) {
  } else {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_2A64B(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  local_E8.x = arg4;
  (&local_E8)->y = arg5;
  (&local_E8)->z = arg6;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  return;
}

static void script_2A69A(void) {
  task_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  return;
}

static void script_2A6BA(int32_t arg0, int32_t arg1, Vector3 *arg2, Vector3i *arg3, const void *arg4) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_InterpolateAndScaleAngles(arg0, arg1, arg2, arg3, arg4);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  return;
}

static void script_2A6E2(int32_t arg0, const char *arg1) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, arg0, arg1, "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  return;
}

static void script_2A70B(void) {
  Script_seqEnterEvtCam(0, 0, AUTH_None);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_2A725(int32_t arg0) {
  if (arg0 == 1) {
    Script_WeatherUtils_ForFIXC(13, 0);
    Script_WeatherTaskUtil(LoadSPR);
  } else {
    Script_WeatherUtils_ForFIXC(13, 1);
  }
  return;
}

static void script_2A74D(void) {
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  script_2A5C6();
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  return;
}

static void script_2A76B(void) {
  int8_t local_FF;
  if (task_106 & 0x10) {
    (void)((local_FF = 0) & 2);
  } else {
    (void)((task_106 = -1) & 2);
    (void)((local_FF = -1) & 2);
  }
  (void)(local_FF & 0x80);
  return;
}

static void script_2A79E(void) {
  (void)((task_106 = 0) & 2);
  return;
}

static void script_2A7AB(int32_t arg0) {
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 1);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_faceUpdateFor(arg0, 0, 0);
  Script_faceUpdateFor(arg0, 1, 0);
  return;
}
