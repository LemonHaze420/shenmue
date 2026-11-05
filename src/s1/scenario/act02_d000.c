#include "act02_d000.h"

__attribute__((scn_string_bindings(0, data_373C4)))
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

__attribute__((scn_string_bindings(0, data_37463)))
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

__attribute__((scn_string_bindings(1, data_37489)))
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

__attribute__((scn_string_bindings(0, data_37493)))
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

__attribute__((scn_string_bindings(0, data_374AB)))
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

__attribute__((scn_string_bindings(0, data_374C3)))
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

__attribute__((scn_string_bindings(0, data_374DB)))
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

__attribute__((scn_string_bindings(0, data_374F3)))
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

__attribute__((scn_string_bindings(0, data_3750B)))
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

__attribute__((scn_string_bindings(0, data_37523)))
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

__attribute__((scn_string_bindings(0, data_3753B)))
static void script_2D34(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((*(&task_78 + arg1) & 2) > 0) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
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

__attribute__((scn_string_bindings(0, data_380FC, 8, data_38140, 9, data_38149, 10, data_38152, 11, data_3815B, 12, data_38164, 13, data_3816D)))
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

__attribute__((scn_string_bindings(0, data_38172, 1, data_38177, 2, data_38180, 3, data_38185, 4, data_3818A, 5, data_38193, 6, data_3819C, 7, data_381A5)))
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
  task_12E = 0;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
      break;
    }
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/stream/", arg1) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  }
  task_12E = 1;
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
  if (task_12C == 0) {
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
  if (task_12C == 0) {
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
  if (task_12C == 0) {
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
  if (task_12C == 0) {
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
  if (task_12C == 0) {
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
  if (task_12C == 0) {
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
  if (task_12C == 0) {
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
  if (task_12C == 0) {
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
  if (task_12C == 0) {
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
  if (task_12C == 0) {
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
  if (task_12C == 0) {
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
      task_120 = 5;
      task_11C = (char *)data_381C1;
      arg1 = (int32_t)task_11C;
    }
  }
  local_F8 = task_120;
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
    Script_charaPosRotUtils(arg0, 152, &data_381C8, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg0, 153, &data_381D4, 0, CHARA_PART_Set);
  } else {
    Script_charaPosRotUtils(arg0, 152, &data_381D4, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg0, 153, &data_381C8, 0, CHARA_PART_Set);
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

__attribute__((scn_string_bindings(0, data_381E0)))
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
            if (task_120 == 5) {
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

__attribute__((scn_string_bindings(0, data_381E5)))
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
  task_118 = 0x314E4143;
  task_120 = 3;
  task_11C = (char *)data_381EA;
  task_12C = 0;
  task_12D = 0;
  task_12E = 0;
  task_134 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_381EF, 1, data_381F6, 2, data_38203)))
static void script_4B5C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Vector3 local_E8;
  script_4B22();
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, arg0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  task_124 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_124) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  }
  Script_LoadVoiceFile(1, "01JUCE");
  task_134 = Script_LoadHandMot(2, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
  Script_scenCreateTask(script_4129, 2, arg2, "VEND");
  while (task_12E != 1) {
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

__attribute__((scn_string_bindings(0, data_38208, 2, data_38229)))
static void script_4CB9(int32_t arg0) {
  script_4B22();
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  task_124 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_124) {
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
  task_134 = Script_LoadHandMot(2, SHEN_NAME('D000'), "HMOT_CAN.BIN");
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  return;
}

static void script_4D94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  while (task_12E != 1) {
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

__attribute__((scn_string_bindings(0, data_38236, 1, data_3823D)))
static void script_4E35(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Script_charaSetRefboardFlagTo(arg3, 0);
  Script_ResetHandMotion(arg3, 0, 1, 0);
  Script_faceUpdateFor(arg3, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  if (task_134 != 0) {
    Script_FreeTaskData(task_134);
  }
  Script_LoadVoiceFile(0, "01JUCE");
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetGameStateTo(AUTH_None);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
  Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  if (task_124) {
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(RYO_RestoreControl);
  }
  SCNF_WaitFrames_0000(1);
  script_419E("VEND");
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 2);
  Script_SetCPAMFlags(arg0, 1);
  task_12D = 14;
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
      task_12D = local_E4;
      local_D0 = local_E4;
      if (local_D0 == 1) {
        script_44EA(arg3, 1, (int32_t)data_38244, -1);
        script_44A3(arg3, 1, 542, -1);
      } else if (local_D0 == 3) {
        task_12D = 1;
        Script_shenClearFreeState(0);
        Script_sicnUtils(1, 0, 0);
      } else if (local_D0 == 16) {
        Script_sicnUtils(1, 0, 0);
      } else if ((local_D0 == 2) | (local_D0 == 4)) {
        task_12D = 1;
      } else {
        switch (local_D0) {
          case 5:
            script_4246(0x5E05A9, -1);
            break;
          case 6:
            script_4246(0x6005A9, 5);
            break;
          case 8:
            task_11C = (char *)data_38296;
            task_120 = 0;
            if (arg4 & 1) {
              script_46C0(task_118, (int32_t)task_11C, arg4);
            }
            break;
          case 9:
            task_11C = (char *)data_3829B;
            task_120 = 1;
            if (arg4 & 1) {
              script_46C0(task_118, (int32_t)task_11C, arg4);
            }
            break;
          case 10:
            task_11C = (char *)data_382A0;
            task_120 = 2;
            if (arg4 & 1) {
              script_46C0(task_118, (int32_t)task_11C, arg4);
            }
            break;
          case 11:
            task_11C = (char *)data_382A5;
            task_120 = 3;
            if (arg4 & 1) {
              script_46C0(task_118, (int32_t)task_11C, arg4);
            }
            break;
          case 12:
            task_11C = (char *)data_382AA;
            task_120 = 4;
            if (arg4 & 1) {
              script_46C0(task_118, (int32_t)task_11C, arg4);
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
  script_4596(arg0, arg1, (int32_t)&data_382B0, -1);
  script_45FB(arg0, arg1, (int32_t)data_382BC, -1);
  script_44EA(arg0, 1, (int32_t)data_382C8, -1);
  script_44EA(arg0, 0, (int32_t)data_382C8, -1);
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

__attribute__((scn_string_bindings(3, data_38331, 4, data_3833C)))
static void script_5350(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg2 == 0) {
    script_4129(task_118, task_11C);
  }
  script_42CE(task_118, arg0, 18, (int32_t)&data_382D4, (int32_t)&data_382E0, 82);
  script_4858(task_118, 1, 0);
  script_4596(arg0, arg1, (int32_t)&data_382EC, -1);
  script_45FB(arg0, arg1, (int32_t)data_382F8, -1);
  script_44EA(arg0, 1, (int32_t)&data_38304, -1);
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
  script_4858(task_118, 0, 108);
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
  script_4310(task_118, 30);
  script_48DE(66, 32);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  script_419E(task_11C);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_383A8, 1, data_383B3)))
static void script_5641(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 == 0) {
    script_4129(task_118, task_11C);
  }
  if (task_120 == 4) {
    script_42CE(task_118, arg0, 18, (int32_t)&data_38360, (int32_t)&data_3836C, 82);
    script_4596(arg0, arg1, (int32_t)&data_38378, -1);
    script_45FB(arg0, arg1, (int32_t)data_38384, -1);
    script_44EA(arg0, 1, (int32_t)data_3839C, -1);
    script_44A3(arg0, 1, 544, -1);
  } else {
    script_42CE(task_118, arg0, 18, (int32_t)&data_38348, (int32_t)&data_38354, 82);
    script_4596(arg0, arg1, (int32_t)&data_38378, -1);
    script_45FB(arg0, arg1, (int32_t)data_38384, -1);
    script_44EA(arg0, 1, (int32_t)&data_38390, -1);
    script_44A3(arg0, 1, 545, -1);
  }
  script_4858(task_118, 1, 0);
  script_44A3(arg0, 0, 546, -1);
  script_4433(arg0, 45059, 0, -0x80000000, -1);
  script_4246(0x6505A9, 80);
  script_4246(0x6105A9, 290);
  script_463F(4014, arg1, arg1, -1);
  script_4858(task_118, 0, 290);
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
  if (task_120 == 4) {
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
  script_4310(task_118, 690);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  script_419E(task_11C);
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
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, *(&data_380DC + local_F4))) {
      Script_EVT_GetPosition(*(&data_380DC + local_F4), -1, (Vector3 *)local_90[local_F4], ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(*(&data_380DC + local_F4), (Vector3i *)local_FEC4[local_F4], 0x40000000);
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
      script_4A53(arg0, *(&data_380DC + local_F9), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
      (void)((local_FB = 0) & 0x80);
      (void)((local_FC = 0) & 0x80);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_5D8F(int32_t arg0) {
  script_4B22();
  task_128 = Script_scenCreateTask(script_592B, 1, arg0);
  return;
}

static void script_5DAA(void) {
  Script_srchControl((void *)task_128);
  task_128 = 0;
  return;
}

static void script_5DBB(void) {
  (void)task_12D;
  return;
}

static void script_5DC2(void) {
  Script_ExitCharaSelector();
  task_12C = 1;
  return;
}

static void script_5DCF(void) {
  Script_ambSoundUtils(1, 1, &data_383CC, 80.0f, 100.0f, 1.0f, 0, &data_38468);
  Script_ambSoundUtils(2, 1, &data_383D8, 30.0f, 150.0f, 1.0f, 2, data_384F0);
  Script_ambSoundUtils(3, 0, &data_383E4, 20.0f, 90.0f, 0.8f, 0, data_38578);
  Script_ambSoundUtils(4, 0, &data_383F0, 3.0f, 10.0f, 0.8f, 0, data_38600);
  Script_ambSoundUtils(5, 1, &data_383FC, 15.0f, 120.0f, 0.8f, 0, &data_38688);
  Script_ambSoundUtils(6, 0, &data_38408, 1.5f, 3.0f, 0.8f, 0, data_38710);
  Script_ambSoundUtils(7, 1, &data_38414, 30.0f, 70.0f, 0.9f, 2, &data_38798);
  Script_ambSoundUtils(8, 1, &data_38420, 30.0f, 150.0f, 1.0f, 0, data_38820);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01BNK_BANK_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_RETURN_PROMPT_SHOWN) == 0)) {
    Script_ambSoundUtils(9, 0, &data_3842C, 3.0f, 60.0f, 0.8f, 0, data_38B50);
  } else {
    Script_ambSoundUtils(9, 0, &data_3842C, 3.0f, 60.0f, 0.8f, 0, data_388A8);
  }
  Script_ambSoundUtils(10, 1, &data_38438, 28.0f, 70.0f, 1.0f, 0, &data_38930);
  Script_ambSoundUtils(11, 0, &data_38444, 5.0f, 60.0f, 1.0f, 0, &data_389B8);
  Script_ambSoundUtils(12, 1, &data_38450, 30.0f, 170.0f, 0.8f, 0, &data_38A40);
  Script_ambSoundUtils(15, 1, &data_3845C, 100.0f, 170.0f, 1.0f, 3, &data_38AC8);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_5FF3(void) {
  Script_ambSoundUtils(1, 1, &data_38BD8, 80.0f, 100.0f, 1.0f, 0, &data_38C8C);
  Script_ambSoundUtils(2, 0, &data_38BE4, 2.5f, 25.0f, 1.0f, 0, data_38D14);
  Script_ambSoundUtils(3, 0, &data_38BF0, 2.5f, 25.0f, 1.0f, 0, data_38D9C);
  Script_ambSoundUtils(4, 0, &data_38BFC, 3.0f, 10.0f, 1.0f, 0, data_38E24);
  Script_ambSoundUtils(5, 1, &data_38C08, 15.0f, 120.0f, 1.0f, 0, &data_38EAC);
  Script_ambSoundUtils(6, 0, &data_38C14, 1.5f, 3.0f, 0.8f, 0, data_38F34);
  Script_ambSoundUtils(7, 1, &data_38C20, 30.0f, 70.0f, 1.0f, 2, &data_38FBC);
  Script_ambSoundUtils(8, 0, &data_38C2C, 2.5f, 25.0f, 1.0f, 0, data_39044);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01BNK_BANK_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_RETURN_PROMPT_SHOWN) == 0)) {
    Script_ambSoundUtils(9, 0, &data_38C38, 5.0f, 40.0f, 1.0f, 0, data_39484);
  } else {
    Script_ambSoundUtils(9, 0, &data_38C38, 5.0f, 40.0f, 1.0f, 0, data_390CC);
  }
  Script_ambSoundUtils(10, 1, &data_38C44, 28.0f, 70.0f, 1.0f, 0, &data_39154);
  Script_ambSoundUtils(11, 0, &data_38C50, 5.0f, 40.0f, 1.0f, 0, &data_391DC);
  Script_ambSoundUtils(12, 0, &data_38C5C, 2.5f, 25.0f, 1.0f, 0, data_39264);
  Script_ambSoundUtils(13, 0, &data_38C68, 2.5f, 25.0f, 1.0f, 0, data_392EC);
  Script_ambSoundUtils(14, 0, &data_38C74, 2.5f, 25.0f, 1.0f, 0, data_39374);
  Script_ambSoundUtils(15, 0, &data_38C80, 2.5f, 25.0f, 1.0f, 0, data_393FC);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_625F(void) {
  Script_ambSoundUtils(1, 1, &data_3950C, 80.0f, 100.0f, 1.0f, 0, &data_395C0);
  Script_ambSoundUtils(2, 0, &data_39518, 3.5f, 30.0f, 1.0f, 0, data_39648);
  Script_ambSoundUtils(3, 0, &data_39524, 20.0f, 90.0f, 1.0f, 0, data_396D0);
  Script_ambSoundUtils(4, 0, &data_39530, 3.0f, 10.0f, 1.0f, 0, data_39758);
  Script_ambSoundUtils(5, 1, &data_3953C, 15.0f, 120.0f, 1.0f, 0, &data_397E0);
  Script_ambSoundUtils(6, 0, &data_39548, 1.5f, 3.0f, 1.0f, 0, data_39868);
  Script_ambSoundUtils(7, 1, &data_39554, 30.0f, 70.0f, 1.0f, 2, &data_398F0);
  Script_ambSoundUtils(8, 1, &data_39560, 30.0f, 150.0f, 1.0f, 0, data_39978);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01BNK_BANK_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_RETURN_PROMPT_SHOWN) == 0)) {
    Script_ambSoundUtils(9, 0, &data_3956C, 3.0f, 60.0f, 1.0f, 0, data_39DB8);
  } else {
    Script_ambSoundUtils(9, 0, &data_3956C, 3.0f, 60.0f, 1.0f, 0, data_39A00);
  }
  Script_ambSoundUtils(9, 0, &data_3956C, 3.0f, 60.0f, 1.0f, 0, data_39A00);
  Script_ambSoundUtils(10, 1, &data_39578, 28.0f, 70.0f, 1.0f, 0, &data_39A88);
  Script_ambSoundUtils(11, 0, &data_39584, 5.0f, 60.0f, 1.0f, 0, &data_39B10);
  Script_ambSoundUtils(12, 0, &data_39590, 3.5f, 30.0f, 1.0f, 0, data_39B98);
  Script_ambSoundUtils(13, 0, &data_3959C, 3.5f, 30.0f, 1.0f, 0, data_39C20);
  Script_ambSoundUtils(14, 0, &data_395A8, 10.0f, 30.0f, 1.0f, 0, data_39CA8);
  Script_ambSoundUtils(15, 1, &data_395B4, 100.0f, 170.0f, 1.0f, 3, &data_39D30);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_64EF(void) {
  Script_ambSoundUtils(1, 1, &data_39E40, 80.0f, 100.0f, 1.0f, 0, &data_39EF4);
  Script_ambSoundUtils(2, 1, &data_39E4C, 30.0f, 150.0f, 1.0f, 2, data_39F7C);
  Script_ambSoundUtils(3, 0, &data_39E58, 20.0f, 90.0f, 1.0f, 0, data_3A004);
  Script_ambSoundUtils(4, 0, &data_39E64, 3.0f, 10.0f, 1.0f, 0, data_3A08C);
  Script_ambSoundUtils(5, 1, &data_39E70, 15.0f, 120.0f, 1.0f, 0, &data_3A114);
  Script_ambSoundUtils(6, 0, &data_39E7C, 1.5f, 3.0f, 1.0f, 0, data_3A19C);
  Script_ambSoundUtils(7, 1, &data_39E88, 30.0f, 70.0f, 1.0f, 2, &data_3A224);
  Script_ambSoundUtils(8, 1, &data_39E94, 30.0f, 150.0f, 1.0f, 0, data_3A2AC);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01BNK_BANK_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_RETURN_PROMPT_SHOWN) == 0)) {
    Script_ambSoundUtils(9, 0, &data_39EA0, 3.0f, 60.0f, 1.0f, 0, data_3A334);
  } else {
    Script_ambSoundUtils(9, 0, &data_39EA0, 3.0f, 60.0f, 1.0f, 0, data_3A334);
  }
  Script_ambSoundUtils(10, 1, &data_39EAC, 28.0f, 70.0f, 1.0f, 0, &data_3A3BC);
  Script_ambSoundUtils(11, 0, &data_39EB8, 5.0f, 60.0f, 1.0f, 0, &data_3A444);
  Script_ambSoundUtils(12, 0, &data_39EC4, 3.5f, 30.0f, 1.0f, 0, data_3A4CC);
  Script_ambSoundUtils(13, 0, &data_39ED0, 3.5f, 30.0f, 1.0f, 0, data_3A554);
  Script_ambSoundUtils(14, 0, &data_39EDC, 10.0f, 30.0f, 1.0f, 0, data_3A5DC);
  Script_ambSoundUtils(15, 1, &data_39EE8, 100.0f, 170.0f, 1.0f, 3, &data_3A664);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_675B(int32_t arg0, int32_t arg1) {
  if (arg0 == 1) {
    Script_Ambient1SoundUtils(2, arg1);
  } else {
    Script_Ambient1SoundUtils(3, arg1);
  }
  return;
}

static void script_677C(void) {
  if (task_C8.hours < 6) {
    (void)((task_113 = -1) & 0x40);
    script_675B(1, 4);
  }
  return;
}

static void script_67A2(void) {
  if (task_113 == 0) {
    script_677C();
  }
  return;
}

static void script_67B4(void) {
  if (task_116 == 0) {
    script_675B(1, 9);
  } else {
    script_675B(0, 9);
  }
  return;
}

static void script_67DA(int32_t arg0) {
  int8_t local_FF;
  local_FF = 16;
  while (local_FF) {
    script_675B(arg0, local_FF);
    local_FF = local_FF - 1;
  }
  if (arg0 == 0) {
    script_67B4();
  }
  return;
}

static void script_680E(void) {
  if (-(task_C8.month == 12) & -(task_C8.day >= 15) & -(task_C8.day <= 25)) {
    if (task_C8.day >= 24) {
      script_5FF3();
    } else if (task_C8.day >= 19) {
      script_625F();
    } else {
      script_64EF();
    }
  } else if ((task_C8.month == 1) & (task_C8.day <= 5)) {
    script_64EF();
  } else {
    script_5DCF();
  }
  script_67B4();
  script_67A2();
  return;
}

static void script_6899(int32_t arg0) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_set_val_to_1(3, 0);
  {
    int32_t local_FC;
    local_FC = arg0;
    switch (local_FC) {
      case 2:
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
        Script_SetCharaRefBoardPos(CHARA_AKIR, &data_3A93C);
        Script_charaSetRefboardFlagTo(SHEN_NAME('TBK1'), 1);
        Script_SetCharaRefBoardPos(SHEN_NAME('TBK1'), &data_3A93C);
        Script_charaSetRefboardFlagTo(SHEN_NAME('TBK2'), 1);
        Script_SetCharaRefBoardPos(SHEN_NAME('TBK2'), &data_3A93C);
        Script_executeAicaCommand(10408, 0, 0);
        Script_executeAicaCommand(1184, 2, 112);
        Script_SetCameraFlags(0);
        Script_mapmSetCamFlags(0, 0);
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_scenCreateTask(script_6FAA, 0);
        Script_shenSeqUtils(arg0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_EVT_SetPosition(SHEN_NAME('TBK2'), 0x78000000, &data_3A94C);
        Script_SetCameraFlags(1);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
        Script_charaSetRefboardFlagTo(SHEN_NAME('TBK1'), 0);
        Script_charaSetRefboardFlagTo(SHEN_NAME('TBK2'), 0);
        break;
      case 3:
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_SetFogColour(&data_3A958);
        Script_mapmSetCamFlags(0, 0);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 170.0f);
        Script_scenCreateTask(script_713F, 0);
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
        Script_SetFogColour(&data_3A958);
        Script_mapmSetCamFlags(0, 0);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 183.0f);
        Script_scenCreateTask(script_7174, 0);
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
        Script_SetFogColour(&data_3A958);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 183.0f);
        Script_scenCreateTask(script_7205, 0);
        Script_shenSeqUtils(arg0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 6:
        Script_SetFogColour(&data_3A958);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 183.0f);
        Script_scenCreateTask(script_72B2, 0);
        Script_shenSeqUtils(arg0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 7:
        Script_SetFogColour(&data_3A958);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 183.0f);
        Script_scenCreateTask(script_7339, 0);
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
      script_6EF1();
    } else {
      script_6EDC();
    }
  }
  return;
}

static void script_6C58(void) {
  script_6E5F(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_clmdSetFlags(CHARA_AKIR, 2, 0);
  return;
}

static void script_6CAC(void) {
  script_6DF3(SHEN_NAME('LIG1'));
  script_6DF3(SHEN_NAME('LIG2'));
  script_6DF3(SHEN_NAME('LIG3'));
  script_6DF3(SHEN_NAME('LIG4'));
  script_6DF3(SHEN_NAME('LIG5'));
  script_6DF3(SHEN_NAME('LIG6'));
  script_6DF3(SHEN_NAME('LIG7'));
  Script_EVT_SetPosition(SHEN_NAME('LIG3'), POS_XYZ, &data_3A968);
  Script_EVT_SetRotation(SHEN_NAME('LIG3'), 0x38000000, &data_3A9B0, 0);
  Script_EVT_SetPosition(SHEN_NAME('LIG4'), POS_XYZ, &data_3A974);
  Script_EVT_SetRotation(SHEN_NAME('LIG4'), 0x38000000, &data_3A9BC, 0);
  Script_EVT_SetPosition(SHEN_NAME('LIG5'), POS_XYZ, &data_3A980);
  Script_EVT_SetRotation(SHEN_NAME('LIG5'), 0x38000000, &data_3A9C8, 0);
  Script_EVT_SetPosition(SHEN_NAME('LIG6'), POS_XYZ, &data_3A98C);
  Script_EVT_SetRotation(SHEN_NAME('LIG6'), 0x38000000, &data_3A9D4, 0);
  Script_EVT_SetPosition(SHEN_NAME('LIG7'), POS_XYZ, &data_3A998);
  Script_EVT_SetRotation(SHEN_NAME('LIG7'), 0x38000000, &data_3A9E0, 0);
  Script_EVT_SetPosition(SHEN_NAME('LIG8'), POS_XYZ, &data_3A9A4);
  Script_EVT_SetRotation(SHEN_NAME('LIG8'), 0x38000000, &data_3A9EC, 0);
  return;
}

static void script_6DF3(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  return;
}

static void script_6E0E(int32_t arg0) {
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

static void script_6E5F(int32_t arg0) {
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

static void script_6EDC(void) {
  script_6F3D(CHARA_AKIR);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_6EF1(void) {
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_6EFA(void) {
  return;
}

static void script_6EFD(int32_t arg0) {
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_CharHandlers(arg0, -1, 0, 0);
  return;
}

static void script_6F3D(int32_t arg0) {
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

static int32_t script_6FAA(void) {
  int16_t local_FE;
  uint8_t unused_E4[26];
  local_FE = 0;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_EVT_SetPosition(SHEN_NAME('TBK1'), 0x78000000, &data_3AA34);
          Script_EVT_SetRotation(SHEN_NAME('TBK1'), 0x78000000, &data_3AA40, 0);
          Script_EVT_SetPosition(SHEN_NAME('TBK2'), 0x78000000, &data_3AA10);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_3A858, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_3A858, 0);
          Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_3AA64);
          break;
        case 120:
          Script_EVT_SetPosition(SHEN_NAME('TBK1'), 0x78000000, &data_3AA10);
          Script_EVT_SetPosition(SHEN_NAME('TBK2'), 0x78000000, &data_3AA4C);
          Script_EVT_SetRotation(SHEN_NAME('TBK2'), 0x78000000, &data_3AA58, 0);
          break;
        case 300:
          Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_3AA70);
          break;
        case 433:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_3A774, 10);
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

static int32_t script_713F(void) {
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

static int32_t script_7174(void) {
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

static int32_t script_7205(void) {
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
          Script_scenCreateTask(script_767E, 4, 30, 18, 0x3F19999A, 0x3F800000);
          Script_scenCreateTask(script_74F2, 3, SHEN_NAME('LIG7'), 60, 0x3F333333);
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

static int32_t script_72B2(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if (local_FE == 0) {
        Script_charaSetRenderOpacity(SHEN_NAME('LIG5'), 0.0f);
        Script_LightUtils(4, 5, 2.3f);
        Script_scenCreateTask(script_7489, 3, SHEN_NAME('LIG6'), 50, 0x3F36DC5D);
        Script_scenCreateTask(script_767E, 4, 50, 3, 0x3F7C56D6, 0x400CCCCD);
      }
      local_FE = local_FE + 1;
    } else {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_7339(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_lightSetSlotEnabled(9, 0);
          Script_charaDisplay(SHEN_NAME('LIG3'), CDM_Show);
          Script_scenCreateTask(script_7489, 3, SHEN_NAME('LIG8'), 50, 0x3F200000);
          Script_scenCreateTask(script_767E, 4, 50, 10, 0x3FF00000, 0x40400000);
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

static void script_741A(int32_t arg0, int32_t arg1) {
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

static int32_t script_7489(int32_t arg0, int32_t arg1, float arg2) {
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

static int32_t script_74F2(int32_t arg0, int32_t arg1, float arg2) {
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

static void script_7564(int32_t arg0) {
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

static void script_75C2(int32_t arg0) {
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

static void script_7620(int32_t arg0) {
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

static int32_t script_767E(int32_t arg0, int32_t arg1, float arg2, float arg3) {
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

static void script_76F7(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_771F(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_7773(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_77C3(int32_t arg0) {
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

static void script_786A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_77C3(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_788D(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_78C0(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_788D(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_78E3(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_788D(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_7942(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

__attribute__((scn_string_bindings(0, data_3AA8C)))
static void script_7957(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_3AAA0)))
static void script_7966(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_7975(void) {
  return;
}

static void script_7978(void) {
  return;
}

static void script_797B(void) {
  return;
}

static void script_797E(void) {
  return;
}

static void script_7981(void) {
  (void)((task_144 = 0) & 0x20);
  return;
}

static int32_t script_798E(void) {
  return task_138 = Script_freeTalkGetCurrentChara();
}

static void script_799A(void) {
  *(&task_138 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_79A9(void) {
  return *(&task_138 + 2) = Script_freeTalkGetAndResetState();
}

static int32_t script_79B8(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_79D6(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_79F5(int32_t arg0, int32_t arg1) {
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

static void script_7A6D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_2A8 != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_190[arg2], arg3, arg4);
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

static int32_t script_801F(int32_t arg0) {
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

static void script_80F0(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_10E13(arg0, arg2, 0);
  return;
}

static void script_8100(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_105A8(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_B88C(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_10E13(arg0, arg2, 0);
  return;
}

static void script_8204(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_105A8(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_190[arg2]);
    script_B88C(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_10E13(arg0, arg2, 0);
  return;
}

static void script_83A6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_105A8(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 4) == 0) {
      script_9D99(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_2A8) {
        script_D16C(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_10E13(arg0, arg2, 2);
  return;
}

static void script_8507(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_105A8(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 4) == 0) {
      script_9D99(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_2A8) {
        script_D16C(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_10E13(arg0, arg2, 2);
  return;
}

static void script_8668(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_105A8(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_190[arg2]);
      script_9D99(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_2A8) {
        float local_F4;
        float local_F0;
        local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_105A8(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_190[arg2]);
        script_D16C(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_10E13(arg0, arg2, 2);
  return;
}

static void script_8906(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 6);
    script_105A8(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_10E13(arg0, arg2, 0);
  return;
}

static void script_89D4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_105A8(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_AD18(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_10E13(arg0, arg2, 1);
  return;
}

static void script_8A3B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_105A8(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_AD18(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_10E13(arg0, arg2, 1);
  return;
}

static void script_8AA2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_105A8(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_AD18(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_10E13(arg0, arg2, 1);
  return;
}

static void script_8AE9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_105A8(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_105A8(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_B1D2(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_10E13(arg0, arg2, 2);
  return;
}

static void script_8C1F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  if (arg0 == task_2A8) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_FF7F = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_105A8(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_10C71(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_2B0)->x;
      (&local_9C)->y = task_2B0.y;
      (&local_9C)->z = task_2B0.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, CHARA_PART_Get);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, CHARA_PART_Get);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_2A8) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_2B0, *(&task_2BC + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_2B0, *(&task_2BC + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_2A8) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_2A8) {
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
      script_10C71(arg0, 1);
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_2A8) {
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
    script_10E13(arg0, arg2, 0);
  } else {
    script_8204(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_94E0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_105A8(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_105A8(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_D88D(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_10E13(arg0, arg2, 0);
  return;
}

static int32_t script_95FE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_F4 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 6);
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
        local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_105A8(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L0097BA;
            }
          }
          L0097BA:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_2BC + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_2B0)->x, (&local_E4)->z - task_2B0.z);
        script_E017(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadFloatBits, Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_3BAC4, SCND_ReadFloatBits, Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_105A8(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_190[arg2]);
      script_E0EF(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadFloatBits, Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_3BAC4, SCND_ReadFloatBits, Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_105A8(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_190[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_E566(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadFloatBits, Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_3BAC4, SCND_ReadFloatBits, Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_105A8(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_190[arg2]);
          script_E0EF(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L009CB5;
      }
    }
  }
  L009CB5:;
  script_10E13(arg0, arg2, 0);
  end:;
}

static void script_9CC3(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_801F(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3C5E2)))
static void script_9D99(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_2A8) {
        local_FF78 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_7A6D(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_10C71(arg0, 0);
    local_C0.x = (&task_2B0)->x;
    (&local_C0)->y = task_2B0.y;
    (&local_C0)->z = task_2B0.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_2B0, *(&task_2BC + 1), local_FF78);
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
    if (task_2A8 == arg0) {
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
          script_9CC3(arg0, task_1E0[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_2A8) {
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
                goto L00A321;
              }
            }
            L00A321:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L00A35F;
                }
              }
              L00A35F:;
              goto L00A362;
            }
          }
        }
      }
      L00A362:;
      if (task_2A8 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_2A8) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_2A8 != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_2A8 == arg0) {
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
                goto L00A52E;
              }
            }
            L00A52E:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L00A56C;
                }
              }
              L00A56C:;
              goto L00A56F;
            }
          }
          L00A56F:;
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
              goto L00A5C8;
            }
          }
        }
      }
      L00A5C8:;
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
      if (arg0 == task_2A8) {
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
  script_10C71(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_190[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_2A8 == arg0) {
        local_80 = (Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_2A8 == arg0) {
        local_80 = (Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_148, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_148, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_148, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_2A8 == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_148, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_148, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_148, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_148, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L00ACAB;
      }
    }
  }
  L00ACAB:;
  return;
}

static void script_ACAC(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_2A8 == arg0) {
    local_FC = task_190[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_AD18(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_2B0)->x;
  (&local_AC)->y = task_2B0.y;
  (&local_AC)->z = task_2B0.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
    local_80 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_2A8) {
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
        task_1B8[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
        local_80 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_2A8) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_1B8[arg2] = 0;
      }
    }
    local_88 = task_190[arg2];
    local_88 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_190[arg2];
      local_88 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_1B8[arg2] = 0;
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
          goto L00B182;
        }
      }
    }
    L00B182:;
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

__attribute__((scn_string_bindings(0, data_3C5FE)))
static void script_B1D2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_10C71(arg0, 0);
  local_CC.x = (&task_2B0)->x;
  (&local_CC)->y = task_2B0.y;
  (&local_CC)->z = task_2B0.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2B0, *(&task_2BC + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_2A8) {
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
  script_10C71(arg0, 1);
  local_FC = task_190[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_148, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_148, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_148, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3C61A)))
static void script_B88C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      if (arg0 == task_2A8) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_2A8) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_1E0[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_1E0[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_2A8) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_2A8) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_2A8) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_1E0[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_1E0[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L00C47F;
      }
    }
  }
  L00C47F:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 7);
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
  script_7A6D(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_10C71(arg0, 0);
  local_D0.x = (&task_2B0)->x;
  (&local_D0)->y = task_2B0.y;
  (&local_D0)->z = task_2B0.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2B0, *(&task_2BC + 1), local_A4);
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
    if (arg0 == task_2A8) {
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
      if (arg0 == task_2A8) {
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
              script_9CC3(arg0, task_1E0[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_2A8) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_2A8) {
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
        local_FF60 = script_79F5((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L00CA84;
          }
        }
        L00CA84:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_79F5(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_79F5(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_2A8) {
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
          local_FF74 = (float)script_79B8(script_79F5(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_2A8) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_2A8) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_190[arg2];
        if (arg0 == task_2A8) {
          if (task_2A8 == arg0) {
            local_A0 = (Script_ScenDataUtil(task_148, SCND_ReadU32, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_2A8) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L00CF30;
            }
          }
        }
      }
      L00CF30:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_2A8) {
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
      if (arg0 != task_2A8) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_10C71(arg0, 1);
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
  if (arg0 == task_2A8) {
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
      goto L00D16B;
    }
  }
  L00D16B:;
  return;
}

__attribute__((scn_string_bindings(0, data_3C636)))
static void script_D16C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_10C71(arg0, 0);
    local_C4.x = (&task_2B0)->x;
    (&local_C4)->y = task_2B0.y;
    (&local_C4)->z = task_2B0.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_2B0, *(&task_2BC + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
          local_84 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_2A8) {
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
                goto L00D490;
              }
            }
            L00D490:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L00D4CC;
                }
              }
              L00D4CC:;
              goto L00D4CF;
            }
          }
        }
      }
      L00D4CF:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L00D514;
          }
        }
      }
      L00D514:;
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
  script_10C71(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_190[arg2];
    local_9C = Script_ScenDataUtil(task_148, SCND_ReadU32, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_148, SCND_ReadU32, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_148, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_148, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_148, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3C652)))
static void script_D88D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_3B0A4, SCND_ReadU32, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_3B0A4, SCND_ReadFloatBits, Script_ScenDataUtil(data_3AAB4, SCND_ReadU8, arg3 * 40 + task_1E0[arg2] * 4 + arg4) * 9 + 4);
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
  script_10C71(arg0, 0);
  local_B0.x = (&task_2B0)->x;
  (&local_B0)->y = task_2B0.y;
  (&local_B0)->z = task_2B0.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2B0, *(&task_2BC + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_2A8) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_2A8) {
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
  script_10C71(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, CHARA_PART_Set);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, CHARA_PART_Set);
  return;
}

static void script_E017(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2B0, *(&task_2BC + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_3C66E)))
static void script_E0EF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_D4 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadU32, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadU32, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadU32, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadU32, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L00E310;
    }
  }
  L00E310:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_3BAC4, SCND_ReadFloatBits, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_3BAC4, SCND_ReadFloatBits, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_2B0)->x;
  (&local_F4)->y = task_2B0.y;
  (&local_F4)->z = task_2B0.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2B0, *(&task_2BC + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_2A8) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_2A8) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_3C68A)))
static void script_E566(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  task_2D4 = 0;
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
        goto L00E872;
      }
    }
  }
  L00E872:;
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
  local_B8.x = (&task_2B0)->x;
  (&local_B8)->y = task_2B0.y;
  (&local_B8)->z = task_2B0.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_2B0, *(&task_2BC + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_2A8) {
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
      local_FF40 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_2A8) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_2A8) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    local_FF6C = 0;
    L00F33D:;
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
      if (!(local_FF6C >= 20)) goto L00F33D;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_2A8) {
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

static void script_F4F9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    if (arg0 != task_2A8) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_801F(arg5));
    }
  }
  return;
}

static void script_F943(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_148 = arg1;
  task_14C = arg2;
  task_150 = arg3;
  task_154 = arg4;
  task_158.mode = 0;
  task_158.id = 0;
  task_158.entry_idx = 0;
  task_2A8 = arg0;
  (void)((task_2CC = -1) & 1);
  task_2C8 = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L00F9AD:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_230[local_F8] = local_FC;
  task_168[local_F8] = -1;
  task_190[local_F8] = -1;
  task_1B8[local_F8] = 0;
  task_1E0[local_F8] = 0;
  task_208[local_F8] = 0;
  task_258[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L00F9AD;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_148, SCND_ReadU32, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_148, SCND_WriteU32, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_158, task_168, task_190, (shen_name *)task_1B8, task_1E0, arg0, arg5, data_3AAB4, data_3B0A4, (shen_name *)task_230);
  Script_scenCreateTask(script_101FF, 0);
  return;
}

static void script_FACF(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_148 = arg0;
  task_14C = arg1;
  task_150 = arg2;
  task_154 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_2C8 = 1;
  return;
}

static void script_FB03(void) {
  int32_t local_FC;
  local_FC = 0;
  L00FB0B:;
  task_168[local_FC] = -1;
  task_190[local_FC] = -1;
  task_1B8[local_FC] = 0;
  task_1E0[local_FC] = 0;
  task_208[local_FC] = 0;
  task_258[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00FB0B;
  return;
}

static void script_FB6C(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_2A8 == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_2DC, 0);
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

static void script_FC09(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_2A8 == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_2DC, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_FC43(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_148, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_FCA6(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_148, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_FD09(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_148, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_FD40(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_148, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_FD77(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_190[local_F0] == arg0) {
      local_EC = task_230[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[local_F0] * 13 + 1) & 0x20) {
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

static void script_FFB9(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_148, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_100C1(arg0);
  script_FD77(arg0);
  return;
}

static void script_10002(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_148, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_100CF(arg0);
  script_FD77(arg0);
  return;
}

static void script_1004B(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_148, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_148, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_100C1(int32_t arg0) {
  script_1004B(arg0, 0);
  return;
}

static void script_100CF(int32_t arg0) {
  script_1004B(arg0, 1);
  return;
}

static void script_100DD(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_148, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_148, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_101A4(void) {
  script_100DD(1);
  return;
}

static void script_101B0(void) {
  script_100DD(0);
  return;
}

static void script_101BC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_148, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_101FF(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_2C8 == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_168[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_158)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
        script_10FD0(1, local_FC, local_F4, local_F8);
        if (script_11184(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_10FD0(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_1139A(local_F4, local_F8);
    } else {
      task_2C8 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_10310(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_2A8, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_2A8, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_3C715, 1, data_3C72E)))
static void script_103A8(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_280[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_280[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_154, SCND_ReadU32, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_154, SCND_ReadU32, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_154, SCND_ReadU32, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_154, SCND_ReadU32, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_154, SCND_ReadU32, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_154, SCND_ReadU32, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_154, SCND_ReadU32, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_150, SCND_ReadScriptOffset, Script_ScenDataUtil(task_154, SCND_ReadU32, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_280[arg0] = -1;
    }
  } else {
    task_280[arg0] = -1;
  }
  return;
}

static void script_10577(int32_t arg0) {
  if (task_280[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_280[arg0], 1);
    task_280[arg0] = -1;
  }
  return;
}

static int32_t script_105A8(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_2A8 == arg0) {
    local_F4.x = (&task_2DC)->x;
    (&local_F4)->y = task_2DC.y;
    (&local_F4)->z = task_2DC.z;
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
        if ((Script_ScenDataUtil(task_148, SCND_ReadU32, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_148, SCND_ReadU32, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_148, SCND_ReadU32, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_148, SCND_ReadU32, arg6 * 13 + 1) & 0x80)) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L010793;
        }
        L010793:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L010836;
          }
        }
        L010836:;
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
            goto L0109E2;
          }
        }
      }
    }
    L0109E2:;
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
          task_2B0.x = (&local_F4)->x;
          task_2B0.y = (&local_F4)->y;
          task_2B0.z = (&local_F4)->z;
          task_2BC = (&local_C4)->x;
          *(&task_2BC + 1) = (&local_C4)->y;
          *(&task_2BC + 2) = (&local_C4)->z;
          break;
        case 5:
          task_2B0.x = (&local_F4)->x;
          task_2B0.y = (&local_F4)->y;
          task_2B0.z = (&local_F4)->z;
          task_2BC = (&local_C4)->x;
          *(&task_2BC + 1) = (&local_C4)->y;
          *(&task_2BC + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_2B0, 0);
          task_2BC = (&local_C4)->x;
          *(&task_2BC + 1) = (&local_C4)->y;
          *(&task_2BC + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_10C71(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if (task_2A8 == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, CCOW_EnableMap);
      } else {
        Script_ccowUtils(arg0, CCOW_DisableMap);
      }
    }
  }
  return;
}

static void script_10CA3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_105A8(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_2A8 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_148, SCND_ReadU32, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_148, SCND_ReadU32, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_148, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_148, SCND_ReadU32, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_148, SCND_ReadU32, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_148, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3C747)))
static void script_10E13(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_2A8 == arg0) {
    task_158.mode = 0;
    task_158.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_2A8, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_2CC & 0x80) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_190[arg1];
        local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_148, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_190[arg1];
      local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_148, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_168[arg1] = 1;
  task_258[arg1] = 0;
  task_208[arg1] = 0;
  task_1B8[arg1] = 0;
  if (task_280[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_280[arg1], 1);
    task_280[arg1] = -1;
  }
  return;
}

static void script_10FD0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_10310(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_10310(arg2, arg3, local_FC);
          goto L011094;
        }
      }
      L011094:;
    } else {
      if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_10310(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_2A8, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L01112F;
          }
        }
      }
    }
  }
  L01112F:;
  if (task_258[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_2D8) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_2D8) & task_2D8) {
    Script_sicnUtils(2, 4, 0);
  }
  task_2D8 = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_3C756)))
static int32_t script_11184(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg1] * 13 + 1) & 4) == 0)) {
      if (task_258[arg1] == 0) {
        task_168[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_1B8[arg1] != task_2A8) & task_2D8) {
        if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg1] * 13 + 1) & 2) == 0) {
          task_168[arg1] = 3;
          task_1B8[arg1] = task_2A8;
          task_1E0[arg1] = 0;
          task_158.mode = 1;
          task_158.entry_idx = task_190[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_2A8, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_168[arg1] = 6;
              task_1B8[arg1] = task_2A8;
              task_1E0[arg1] = 0;
              task_158.mode = 0;
              task_158.entry_idx = task_190[arg1];
              local_FC = 0;
            }
            goto L011338;
          }
        }
        L011338:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_2A8, -1, &task_2DC, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_158.id = task_2A8;
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

static void script_1139A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L0113A2:;
  local_F8 = task_168[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L0113C2;
      L0113C2:;
      script_11464(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L0113E2;
      L0113E2:;
      script_114C5(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L011400;
      L011400:;
      script_1166A(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L011422;
      L011422:;
      script_11702(local_FC, arg1);
      break;
    case 2:
      script_117E4(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L0113A2;
  return;
}

static void script_11464(int32_t arg0, int32_t arg1) {
  if (task_258[arg0] == 0) {
    task_258[arg0] = Script_scenCreateTask(script_95FE, 4, task_1B8[arg0], task_230[arg0], arg0, arg1);
    task_208[arg0] = task_1B8[arg0];
    script_103A8(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3C765, 1, data_3C777)))
static void script_114C5(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_1B8[arg0])) {
    if (task_258[arg0] == 0) {
      if (task_1B8[arg0] == task_2A8) {
        task_158.mode = 1;
        task_158.entry_idx = task_190[arg0];
        goto L01151A;
      }
      L01151A:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
      script_10CA3(task_1B8[arg0], task_230[arg0], task_190[arg0], local_F8);
      task_258[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_3BB64, SCND_ReadScriptOffset, local_F8), 4, task_1B8[arg0], task_230[arg0], arg0, local_F8);
      task_208[arg0] = task_1B8[arg0];
      if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_103A8(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_280[arg0] = -1;
      }
    }
  } else {
    if (task_168[arg0] != 5) {
      task_168[arg0] = 1;
      task_258[arg0] = 0;
      task_208[arg0] = 0;
      task_1B8[arg0] = 0;
    }
  }
  return;
}

static void script_1166A(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_258[arg0] == 0) & (arg2 >= 0)) {
    task_1B8[arg0] = task_2A8;
    task_158.mode = 2;
    task_158.entry_idx = task_190[arg0];
    task_258[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_3BB64, SCND_ReadScriptOffset, arg2), 4, task_1B8[arg0], task_230[arg0], arg0, arg2);
    task_208[arg0] = task_1B8[arg0];
    script_103A8(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_3C78A, 1, data_3C79C)))
static void script_11702(int32_t arg0, int32_t arg1) {
  if ((task_258[arg0] == 0) & (arg1 >= 0)) {
    script_10CA3(task_1B8[arg0], task_230[arg0], task_190[arg0], arg1);
    task_258[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_3BB64, SCND_ReadScriptOffset, arg1), 4, task_1B8[arg0], task_230[arg0], arg0, arg1);
    task_208[arg0] = task_1B8[arg0];
    if ((Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_103A8(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_280[arg0] = -1;
    }
  }
  return;
}

static void script_117E4(int32_t arg0) {
  int32_t local_FC;
  task_258[arg0] = 0;
  task_208[arg0] = 0;
  task_168[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[arg0], 2);
  if (local_FC != -1) {
    task_1B8[arg0] = 0;
  }
  return;
}

static void script_1183B(int32_t arg0, int32_t arg1) {
  task_2EC = arg0;
  script_11E39(arg1);
  task_300 = 9999;
  task_304 = 9999;
  task_308 = 0;
  task_30C = 0;
  task_315 = 0;
  task_318 = 0;
  script_11B73();
  task_31C = 0;
  task_2F0 = 0x52554B59;
  task_2F4 = 0x4C554B59;
  return;
}

static void script_1189A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  script_1183B(arg0, arg1);
  task_310 = Script_scenCreateTask(script_118C1, 4, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_118C1(float arg0, float arg1, float arg2, float arg3) {
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
  task_316 = 0;
  task_317 = 0;
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0x42000000);
    if (Script_CheckTargetWithinAngleRange(&local_F4, 2.0f, (int32_t)(local_D8 / 360.0f * (float)0x10000), 7281, &local_DC) & Script_CheckTargetWithinAngleRange(&local_DC, 2.0f, (&local_E8)->y, 7281, &local_F4)) {
      if (Script_IsPlayerAllowed(0) & -(task_315 == 0)) {
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
          task_316 = 1;
          script_11FA5();
          task_316 = 0;
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

static int32_t script_11A5F(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    task_314 = 0;
    for (;;) {
      if ((Script_GetControllerInput(1) == 0x400) | (Script_GetControllerInput(1) == 0x200)) {
        task_314 = 1;
        Script_TELMUtils(TELM_CMD_RequestHangUp, task_2EC);
        break;
      }
      local_FC = local_FC + 1;
      if (local_FC == 300) {
        task_314 = 1;
        Script_TELMUtils(TELM_CMD_RequestHangUp, task_2EC);
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  if (task_30C) {
    int32_t local_FC;
    local_FC = 0;
    for (;;) {
      if ((task_30C == 0) | (local_FC < -200)) {
        break;
      }
      local_FC = local_FC - 5;
      Script_SetSequenceAudioVolume(0, (void *)task_30C, local_FC);
      SCNF_WaitFrames_0000(1);
    }
    if (task_30C) {
      Script_StartOrFreeSEQ((void *)task_30C);
    }
  }
  return Script_srchControl(0);
}

static void script_11B26(const char *arg0, int32_t arg1) {
  task_320[arg1] = Script_TELMUtils(TELM_CMD_RegisterPhoneNumber, task_2EC, arg0);
  if (task_2F8 == arg1) {
    task_300 = task_320[arg1];
  }
  if (task_2FC == arg1) {
    task_304 = task_320[arg1];
  }
  return;
}

static void script_11B73(void) {
  script_11B26((const char *)&data_3EE28, 0);
  script_11B26((const char *)&data_3EE2C, 1);
  script_11B26((const char *)&data_3EE30, 2);
  script_11B26((const char *)&data_3EE34, 7);
  script_11B26("61564", 3);
  script_11B26("37742", 4);
  script_11B26("37749", 5);
  script_11B26("22550", 6);
  script_11B26("046861564", 8);
  script_11B26("046837742", 9);
  script_11B26("046837749", 10);
  script_11B26("046822550", 11);
  script_11B26("2?????", 12);
  script_11B26("5?????", 13);
  script_11B26("7?????", 14);
  script_11B26("8?????", 15);
  script_11B26("3?????", 16);
  script_11B26("4?????", 17);
  script_11B26("6?????", 18);
  script_11B26("9?????", 19);
  script_11B26("0?????????", 20);
  script_11B26("1?????", 21);
  return;
}

static int32_t script_11CAA(int32_t arg0) {
  if (arg0 > 0) {
    if ((arg0 == task_300) | (arg0 == task_304)) {
      Script_scenCreateTask(script_11A5F, 0);
      for (;;) {
        Script_executeAicaCommand(394667, 0, 0);
        SCNF_WaitFrames_0000(15);
        Script_executeAicaCommand(460203, 0, 0);
        SCNF_WaitFrames_0000(15);
        if (task_314) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      arg0 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_2EC);
    }
  }
  return arg0;
}

static void script_11D23(void) {
  if (task_315 == 0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 185, 1);
    }
    task_315 = script_11D85();
    if (task_315 == 0) {
      task_315 = task_315 + 1;
      if (task_316) {
        task_317 = 1;
      }
    }
  }
  return;
}

static int8_t script_11D85(void) {
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

__attribute__((scn_string_bindings(0, data_3EEC2, 3, data_3EEDC, 4, data_3EEE6)))
static void script_11DD0(int32_t arg0) {
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

static void script_11E39(int32_t arg0) {
  int32_t local_FC;
  task_2F8 = arg0;
  local_FC = arg0;
  if (local_FC == 4) {
    task_2FC = 9;
  } else {
    task_2FC = 9999;
  }
  return;
}

static void script_11E64(void) {
  Script_convSetCamInactiveFlag(0);
  if ((task_2F8 == 9998) | (task_2F8 == 9997)) {
    SCNF_SetCashValue(10, 1);
  }
  return;
}

static int32_t script_11E8B(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours <= 4) {
    arg0 = -1;
  } else {
    if (((&local_F4)->hours >= 23) & ((&local_F4)->minutes >= 30)) {
      arg0 = -1;
      goto L011EC9;
    }
  }
  L011EC9:;
  return arg0;
}

static int8_t script_11ECD(int32_t arg0) {
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

static void script_11FA5(void) {
  script_11D23();
  script_11FB2();
  return;
}

static void script_11FB2(void) {
  int32_t unused_F4[3];
  Vector4 local_E4;
  int32_t local_E0;
  int32_t local_DC;
  float local_D8;
  int32_t unused_D4;
  int8_t local_D3;
  uint8_t unused_D0[3];
  if (task_315 == 1) {
    Script_scenToggleActiveFor(CHARA_AKIR, 0);
    Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
    Script_scenToggleVisibleFor(task_2EC, 1);
    if (0) {
      Script_charaDisplay(task_2F4, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_2F4, CDM_Show);
        goto L012036;
      }
    }
    L012036:;
    Script_scenToggleVisibleFor(task_2F4, 1);
    if (0) {
      Script_charaDisplay(task_2F0, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_2F0, CDM_Show);
        goto L012064;
      }
    }
    L012064:;
    Script_scenToggleVisibleFor(task_2F0, 1);
    Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, task_2F4, task_2F0);
    local_D8 = Script_charaSetClipAngle(task_2EC, -1.0f);
    Script_charaSetClipAngle(task_2EC, 1.0f);
    local_E4.x = 0.8f;
    (&local_E4)->y = 0.8f;
    (&local_E4)->z = 0.8f;
    (&local_E4)->w = 0.8f;
    Script_charaSetRefboardFlagTo(task_2F0, 1);
    Script_charaSetRefboardFlagTo(task_2F4, 1);
    Script_SetCharaRefBoardPos(task_2F0, &local_E4);
    Script_SetCharaRefBoardPos(task_2F4, &local_E4);
    SCNF_WaitFrames_0000(1);
    script_1D961();
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
    script_12FCE();
    Script_charaDisplay(CHARA_AKIR, CDM_Show);
    script_12487();
    local_E0 = 0;
    local_DC = 0;
    task_37E = 0;
    for (;;) {
      if (MMTK_ScriptMemoToggle(2, 0) == 0) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          task_37E = 1;
          Script_sicnUtils(1, 0, 0);
          Script_uiUtils(0, 0, 0);
          break;
        } else if (Script_GetControllerInput(1) == 0x400) {
          local_D3 = script_11D85();
          if (local_D3) {
            script_11DD0(local_D3);
            Script_sicnUtils(1, 0, 0);
            Script_uiUtils(0, 0, 0);
            break;
          } else {
            Script_sicnUtils(1, 0, 0);
            script_12487();
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
            goto L012228;
          }
        }
        L012228:;
        local_E0 = local_E0 + 1;
      } else {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          MMTK_ScriptMemoToggle(1, 0);
          goto L01224C;
        }
      }
      L01224C:;
      SCNF_WaitFrames_0000(1);
    }
    if ((task_2F8 == 9998) | (task_2F8 == 4)) {
      script_1D023();
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      Script_convSetCamInactiveFlag(0);
      Script_SetCPAMFlags(CHARA_AKIR, 1);
      Script_charaSetRefboardFlagTo(task_2F0, 0);
      Script_charaSetRefboardFlagTo(task_2F4, 0);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_SetGameStateTo(AUTH_None);
      Script_SetCameraFieldOfView(10922);
      Script_scenToggleActiveFor(CHARA_AKIR, 1);
    }
    script_1D9D4();
    Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
    Script_ryoControl(RYO_RestoreControl);
    Script_charaSetRefboardFlagTo(task_2F0, 0);
    Script_charaSetRefboardFlagTo(task_2F4, 0);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
    Script_charaSetClipAngle(task_2EC, local_D8);
    Script_scenToggleVisibleFor(task_2EC, 0);
    if (-1) {
      Script_charaDisplay(task_2F0, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_2F0, CDM_Show);
        goto L012347;
      }
    }
    L012347:;
    Script_scenToggleVisibleFor(task_2F0, 0);
    if (-1) {
      Script_charaDisplay(task_2F4, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_2F4, CDM_Show);
        goto L012375;
      }
    }
    L012375:;
    Script_scenToggleVisibleFor(task_2F4, 0);
    task_315 = 0;
    task_317 = 0;
  } else {
    if ((task_315 == -1) | (task_315 == -10) | (task_315 == -15)) {
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
      script_1D961();
      script_11DD0(task_315);
      script_1D9D4();
      Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_ryoControl(RYO_RestoreControl);
      Script_convSetCamInactiveFlag(1);
      Script_SetGameStateTo(AUTH_None);
      task_315 = 0;
      task_317 = 0;
      goto L012486;
    }
  }
  L012486:;
  return;
}

__attribute__((scn_string_bindings(1, data_3EEF8, 7, data_3EF30, 8, data_3EF36, 13, data_3EF64, 14, data_3EF6A, 15, data_3EF70, 16, data_3EF76, 18, data_3EF86, 21, data_3EFA4, 23, data_3EFB4, 25, data_3EFC8, 28, data_3EFE2)))
static void script_12487(void) {
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
  Script_TELMUtils(TELM_CMD_LoadHandMotionAndSetup, task_2EC);
  local_E4 = 0;
  task_37F = 0;
  for (;;) {
    task_37C = Script_TELMUtils(TELM_CMD_GetActiveResult, task_2EC);
    task_37C = script_11CAA(task_37C);
    Script_ReturnTimeDate(&local_F4);
    if (MMTK_ScriptMemoToggle(2, 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
      Script_sprintf("メモ帳消去\012");
      MMTK_ScriptMemoToggle(1, 0);
    }
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) & -(task_37C != -4) & -(task_37C != 0) & -(local_E4 == 0)) {
      local_E4 = 1;
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_2EC);
      task_37C = Script_TELMUtils(TELM_CMD_GetActiveResult, task_2EC);
    }
    if ((task_37C == *task_320) | (task_37C == task_320[1])) {
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_2EC);
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_1308C();
      if (task_37C == *task_320) {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
        if (local_D0 == 0) {
          task_30C = Script_ShowUIMessageFromID("E1032A001");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
        } else if (local_D0 == 1) {
          task_30C = Script_ShowUIMessageFromID("E1032A002");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
        } else {
          task_30C = Script_ShowUIMessageFromID("E1032A003");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
        }
      } else {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
        if (local_D0 == 0) {
          task_30C = Script_ShowUIMessageFromID("E1032A004");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
        } else {
          task_30C = Script_ShowUIMessageFromID("E1032A005");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
        }
      }
      Script_LoadVoiceFile(0, "E1032");
      script_11E64();
    }
    if ((task_37C == task_320[7]) | (task_37C == task_320[2]) | (task_37C == task_320[3]) | (task_37C == task_320[8]) | (task_37C == task_320[4]) | (task_37C == task_320[9]) | (task_37C == task_320[5]) | (task_37C == task_320[10]) | (task_37C == task_320[6]) | (task_37C == task_320[11])) {
      local_EB = 2;
      local_EA = 1;
      if (task_37C == task_320[7]) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_OPERATOR_ADDRESS_LOOKUP_REFUSED)) {
          int32_t local_D0;
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_2EC);
          SCNF_WaitFrames_0000(1);
          Script_LoadVoiceFile(1, "E1032");
          local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
          if (local_D0 == 0) {
            task_30C = Script_ShowUIMessageFromID("E1032A061");
            while (Script_convGetIsActive((const void *)task_30C)) {
              SCNF_WaitFrames_0000(1);
            }
            task_30C = 0;
          } else if (local_D0 == 1) {
            task_30C = Script_ShowUIMessageFromID("E1032A062");
            while (Script_convGetIsActive((const void *)task_30C)) {
              SCNF_WaitFrames_0000(1);
            }
            task_30C = 0;
          } else if (local_D0 == 2) {
            task_30C = Script_ShowUIMessageFromID("E1032A063");
            while (Script_convGetIsActive((const void *)task_30C)) {
              SCNF_WaitFrames_0000(1);
            }
            task_30C = 0;
          } else {
            task_30C = Script_ShowUIMessageFromID("E1032A064");
            while (Script_convGetIsActive((const void *)task_30C)) {
              SCNF_WaitFrames_0000(1);
            }
            task_30C = 0;
          }
          Script_LoadVoiceFile(0, "E1032");
          script_11E64();
          local_EA = 0;
        }
      }
      if (task_37C == task_320[2]) {
        local_EB = 1;
      }
      if ((task_37C == task_320[4]) | (task_37C == task_320[9])) {
        local_E9 = script_11E8B(local_E9);
        if (local_E9 == -1) {
          task_37C = -10;
          local_EA = 0;
        }
      }
      if ((task_37C == task_320[3]) | (task_37C == task_320[8])) {
        local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0);
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) | (local_F0 >= 16)) {
          local_E6 = script_11E8B(local_E6);
          if (local_E6 == -1) {
            task_37C = -10;
            local_EA = 0;
          }
        } else {
          task_37C = -10;
          local_EA = 0;
        }
      }
      if ((task_37C == task_320[5]) | (task_37C == task_320[10])) {
        local_E8 = script_15938();
        switch (local_E8) {
          case -1:
            task_37C = -10;
            local_EA = 0;
            break;
          case -2:
            task_37C = -11;
            local_EA = 0;
            break;
        }
      }
      if ((task_37C == task_320[6]) | (task_37C == task_320[11])) {
        local_E7 = script_16C5F();
        local_E7 = script_11E8B(local_E7);
        if (local_E7 == -1) {
          task_37C = -10;
          local_EA = 0;
        }
      }
      if (local_EA) {
        if (script_11ECD(local_EB)) {
          Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
          Script_shenClearFreeState(0);
          Script_LoadVoiceFile(1, "E1032");
          script_1308C();
          script_1CE89();
          local_E5 = 0;
          if (task_37C == task_320[7]) {
            script_130B1();
          }
          if (task_37C == task_320[2]) {
            script_137F8();
          }
          if ((task_37C == task_320[5]) | (task_37C == task_320[10])) {
            script_15AEE(local_E8);
          }
          if ((task_37C == task_320[6]) | (task_37C == task_320[11])) {
            script_171AE(local_E7);
          }
          if ((task_37C == task_320[3]) | (task_37C == task_320[8])) {
            local_E5 = script_1BB34();
          }
          if ((task_37C == task_320[4]) | (task_37C == task_320[9])) {
            script_1B06C();
          }
          script_1D06D();
          if (local_E5) {
            script_1C0BB();
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED, 1);
          }
          Script_LoadVoiceFile(0, "E1032");
          script_1D00F();
          Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
          Script_shenClearFreeState(1);
          task_37C = 0;
        } else {
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_2EC);
          script_11E64();
        }
      }
    }
    if (task_37C == -10) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_1308C();
      if (script_11ECD(6)) {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
        if (local_D0 == 0) {
        } else if (local_D0 == 1) {
          task_30C = Script_ShowUIMessageFromID("E1032A031");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
        } else if (local_D0 == 2) {
          task_30C = Script_ShowUIMessageFromID("E1032A029");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
        } else if (local_D0 == 3) {
          task_30C = Script_ShowUIMessageFromID("E1032A032");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
        } else {
          task_30C = Script_ShowUIMessageFromID("E1032A033");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
        }
      }
      Script_LoadVoiceFile(0, "E1032");
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_2EC);
      script_11E64();
    }
    if (task_37C == -11) {
      int32_t local_D0;
      Script_scenCreateTask(script_11A5F, 0);
      script_1308C();
      for (;;) {
        Script_executeAicaCommand(394667, 0, 0);
        SCNF_WaitFrames_0000(15);
        Script_executeAicaCommand(460203, 0, 0);
        SCNF_WaitFrames_0000(15);
        if (task_314) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_D0 == 0) {
      } else if (local_D0 == 1) {
        task_30C = Script_ShowUIMessageFromID("E1032A028");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_D0 == 2) {
        task_30C = Script_ShowUIMessageFromID("E1032A029");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032A030");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
      task_37C = Script_TELMUtils(TELM_CMD_GetActiveResult, task_2EC);
      script_11E64();
    }
    if ((task_37C == task_320[12]) | (task_37C == task_320[13]) | (task_37C == task_320[14]) | (task_37C == task_320[15]) | (task_37C == task_320[16]) | (task_37C == task_320[17]) | (task_37C == task_320[18]) | (task_37C == task_320[19]) | (task_37C == task_320[20]) | (task_37C == task_320[21])) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_1308C();
      Script_scenCreateTask(script_11A5F, 0);
      for (;;) {
        task_30C = Script_ShowUIMessageFromID("E1032G001");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        if (task_314) {
          break;
        }
        task_30C = Script_ShowUIMessageFromID("E1032G002");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        if (task_314) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "E1032");
      task_37C = Script_TELMUtils(TELM_CMD_GetActiveResult, task_2EC);
      script_11E64();
    }
    if (task_37C == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(2015, task_2EC, task_2EC);
  script_12FCE();
  Script_sicnUtils(0, 5, 1);
  Script_uiUtils(0, 9, 0);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_12FCE(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(task_2EC, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(task_2EC, &local_E8, 0);
  local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.75f;
  (&local_F4)->y = -0.3f;
  (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.75f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  (&local_DC)->y = (&local_E8)->y;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  Script_InitializeOsag(CHARA_AKIR);
  return;
}

static void script_1308C(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_GetStrayCatSceneVisitCountSlot, 1);
  local_FC = local_FC + 1;
  Script_PlayerStatsUtils(PS_SetStrayCatSceneVisitCountSlot, 1, local_FC);
  return;
}

__attribute__((scn_string_bindings(0, data_3EFE8, 5, data_3F01A, 6, data_3F024, 7, data_3F02E, 8, data_3F038, 14, data_3F074, 16, data_3F088, 17, data_3F092, 18, data_3F09C, 19, data_3F0A6, 21, data_3F0BA, 22, data_3F0C4, 23, data_3F0CE, 24, data_3F0D8, 25, data_3F0E2)))
static void script_130B1(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0)) {
    int32_t local_F8;
    task_308 = 81;
    Script_scenCreateTask(script_1D235, 0);
    task_30C = Script_ShowUIMessageFromID("E1032I001");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032A052");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032I007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032I008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    local_FC = SCNF_RequestActionSelectorFromParams_0008(27, 28, 29, 30, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
    task_308 = 82;
    local_F8 = local_FC;
    switch (local_F8) {
      case 27:
        task_30C = Script_ShowUIMessageFromID("E1032A097");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        break;
      case 28:
        task_30C = Script_ShowUIMessageFromID("E1032A055");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I012");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032A056");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I013");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032A057");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE, 1);
        break;
      case 29:
        task_30C = Script_ShowUIMessageFromID("E1032A059");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        break;
      case 30:
        task_30C = Script_ShowUIMessageFromID("E1032A060");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        break;
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_OPERATOR_ADDRESS_LOOKUP_REFUSED, 1);
  } else {
    task_308 = 80;
    Script_scenCreateTask(script_1D235, 0);
    task_30C = Script_ShowUIMessageFromID("E1032I001");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F8 == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032A007");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032A006");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
      if (local_F8 == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032I002");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_F8 == 1) {
        task_30C = Script_ShowUIMessageFromID("E1032I003");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_F8 == 2) {
        task_30C = Script_ShowUIMessageFromID("E1032I004");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_F8 == 3) {
        task_30C = Script_ShowUIMessageFromID("E1032I005");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032I006");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F8 == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032A008");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032A009");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(5, data_3F178, 6, data_3F182, 7, data_3F18C, 8, data_3F196, 9, data_3F1A0, 10, data_3F1AA, 11, data_3F1B4, 12, data_3F1BE, 13, data_3F1C8, 14, data_3F1D2, 15, data_3F1DC, 16, data_3F1E6, 17, data_3F1F0, 18, data_3F1FA, 19, data_3F204, 20, data_3F20E, 21, data_3F218, 22, data_3F222, 23, data_3F22C, 24, data_3F236, 25, data_3F240, 26, data_3F24A, 27, data_3F254, 28, data_3F25E, 29, data_3F268, 30, data_3F272, 31, data_3F27C, 32, data_3F286, 33, data_3F290, 34, data_3F29A, 35, data_3F2A4, 36, data_3F2AE, 37, data_3F2B8, 38, data_3F2C2, 39, data_3F2CC, 40, data_3F2D6, 41, data_3F2E0, 42, data_3F2EA, 43, data_3F2F4, 44, data_3F2FE, 45, data_3F308, 46, data_3F312, 47, data_3F31C, 48, data_3F326, 49, data_3F330, 50, data_3F33A, 51, data_3F344, 52, data_3F34E, 53, data_3F358, 54, data_3F362, 55, data_3F36C, 56, data_3F376, 57, data_3F380, 58, data_3F38A, 59, data_3F394, 60, data_3F39E, 61, data_3F3A8, 62, data_3F3B2, 63, data_3F3BC, 64, data_3F3C6, 65, data_3F3D0, 66, data_3F3DA, 67, data_3F3E4, 68, data_3F3EE, 69, data_3F3F8, 70, data_3F402, 71, data_3F40C, 72, data_3F416, 73, data_3F420, 74, data_3F42A, 75, data_3F434, 76, data_3F43E, 77, data_3F448, 78, data_3F452, 79, data_3F45C, 80, data_3F466, 81, data_3F470, 82, data_3F47A, 83, data_3F484, 84, data_3F48E, 85, data_3F498, 86, data_3F4A2, 87, data_3F4AC, 88, data_3F4B6, 89, data_3F4C0, 90, data_3F4CA, 91, data_3F4D4, 92, data_3F4DE, 93, data_3F4E8, 94, data_3F4F2, 95, data_3F4FC, 96, data_3F506, 97, data_3F510, 98, data_3F51A, 99, data_3F524, 100, data_3F52E, 101, data_3F538, 102, data_3F542, 103, data_3F54C, 104, data_3F556, 105, data_3F560, 106, data_3F56A, 107, data_3F574, 108, data_3F57E, 109, data_3F588, 110, data_3F592, 111, data_3F59C, 112, data_3F5A6, 113, data_3F5B0, 114, data_3F5BA, 115, data_3F5C4, 116, data_3F5CE, 117, data_3F5D8, 118, data_3F5E2, 119, data_3F5EC, 120, data_3F5F6, 121, data_3F600, 122, data_3F60A, 123, data_3F614, 124, data_3F61E, 125, data_3F628, 126, data_3F632, 127, data_3F63C, 128, data_3F646, 129, data_3F650, 130, data_3F65A, 131, data_3F664, 132, data_3F66E, 133, data_3F678, 134, data_3F682, 135, data_3F68C, 136, data_3F696, 137, data_3F6A0, 138, data_3F6AA, 139, data_3F6B4, 140, data_3F6BE, 141, data_3F6C8, 142, data_3F6D2, 143, data_3F6DC, 144, data_3F6E6, 145, data_3F6F0, 146, data_3F6FA, 147, data_3F704, 148, data_3F70E, 149, data_3F718, 150, data_3F722, 151, data_3F72C, 152, data_3F736, 153, data_3F740, 154, data_3F74A, 155, data_3F754, 156, data_3F75E, 157, data_3F768, 158, data_3F772, 159, data_3F77C, 160, data_3F786, 161, data_3F790, 162, data_3F79A)))
static void script_137F8(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  task_308 = 70;
  Script_scenCreateTask(script_1D235, 0);
  task_30C = Script_ShowUIMessageFromID("E1032K001");
  while (Script_convGetIsActive((const void *)task_30C)) {
    SCNF_WaitFrames_0000(1);
  }
  task_30C = 0;
  task_30C = Script_ShowUIMessageFromID("E1032K002");
  while (Script_convGetIsActive((const void *)task_30C)) {
    SCNF_WaitFrames_0000(1);
  }
  task_30C = 0;
  task_30C = Script_ShowUIMessageFromID("E1032K019");
  while (Script_convGetIsActive((const void *)task_30C)) {
    SCNF_WaitFrames_0000(1);
  }
  task_30C = 0;
  task_30C = Script_ShowUIMessageFromID("E1032K020");
  while (Script_convGetIsActive((const void *)task_30C)) {
    SCNF_WaitFrames_0000(1);
  }
  task_30C = 0;
  task_30C = Script_ShowUIMessageFromID("E1032K022");
  while (Script_convGetIsActive((const void *)task_30C)) {
    SCNF_WaitFrames_0000(1);
  }
  task_30C = 0;
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
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 2) {
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 3) {
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 4) {
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 5) {
    task_30C = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 6) {
    task_30C = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 7) {
    task_30C = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 8) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 9) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 10) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 11) {
    task_30C = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 12) {
    task_30C = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 13) {
    task_30C = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 14) {
    task_30C = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 15) {
    task_30C = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 16) {
    task_30C = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 17) {
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 18) {
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 19) {
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 20) {
    task_30C = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 21) {
    task_30C = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 22) {
    task_30C = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 23) {
    task_30C = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 24) {
    task_30C = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 25) {
    task_30C = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 26) {
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 27) {
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 28) {
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 29) {
    task_30C = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 30) {
    task_30C = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 31) {
    task_30C = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 32) {
    task_30C = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 33) {
    task_30C = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 34) {
    task_30C = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 35) {
    task_30C = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 36) {
    task_30C = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 37) {
    task_30C = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 38) {
    task_30C = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 39) {
    task_30C = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 40) {
    task_30C = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 41) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 42) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 43) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 44) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 45) {
    task_30C = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 46) {
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 47) {
    task_30C = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 48) {
    task_30C = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 54) {
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 50) {
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 51) {
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 52) {
    task_30C = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 53) {
    task_30C = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 54) {
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 55) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 56) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 57) {
    task_30C = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 58) {
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 59) {
    task_30C = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 60) {
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 61) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 62) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 63) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 64) {
    task_30C = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 65) {
    task_30C = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 66) {
    task_30C = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else if (local_EC == 67) {
    task_30C = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else {
    task_30C = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  }
  return;
}

static int8_t script_1587F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static int8_t script_15938(void) {
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
  if (script_1587F(8, 30, 15, 10)) {
    local_EE = 0;
  } else if (script_1587F(15, 10, 20, 30)) {
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
      goto L015A2D;
    }
  }
  L015A2D:;
  if (local_EF == 0) {
    local_EF = script_11E8B(local_EF);
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

__attribute__((scn_string_bindings(0, data_3F7A4, 8, data_3F7F4, 9, data_3F7FE, 14, data_3F830, 17, data_3F84E, 18, data_3F858, 19, data_3F862, 21, data_3F876, 25, data_3F89E, 27, data_3F8B2, 33, data_3F8EE, 40, data_3F934, 41, data_3F93E, 42, data_3F948, 43, data_3F952, 44, data_3F95C, 48, data_3F984, 49, data_3F98E, 52, data_3F9AC, 53, data_3F9B6, 57, data_3F9DE, 59, data_3F9F2, 61, data_3FA06, 62, data_3FA10, 70, data_3FA60, 75, data_3FA92, 76, data_3FA9C, 77, data_3FAA6, 82, data_3FAD8, 83, data_3FAE2, 84, data_3FAEC, 85, data_3FAF6, 86, data_3FB00, 87, data_3FB0A, 88, data_3FB14, 92, data_3FB3C, 93, data_3FB46)))
static void script_15AEE(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 1:
      task_308 = 170;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032L001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A098");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L002");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A099");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L003");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L005");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A100");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 2:
      task_308 = 180;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032L001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L006");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L007");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L008");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L009");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A102");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L010");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 3:
      task_308 = 190;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032L011");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L012");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L013");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L014");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L015");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 191;
      task_30C = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L016");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A105");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L017");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A106");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 4:
      task_308 = 200;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032L018");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A107");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A108");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L020");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L021");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A109");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L022");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A110");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L023");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 5:
      task_308 = 210;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032M001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A112");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M003");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A113");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A114");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M005");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 6:
      task_308 = 220;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032M006");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A115");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M018");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A122");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A123");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M020");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A114");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M010");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 7:
      task_308 = 230;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032N001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A124");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A125");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032N002");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A126");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032N003");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 231;
      task_30C = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A128");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032N004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A129");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032N005");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 8:
      task_308 = 240;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032L011");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A117");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L024");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L025");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L026");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A118");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032L023");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 9:
      task_30C = Script_ShowUIMessageFromID("E1032M001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A112");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M015");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A119");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M016");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A118");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032M017");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
  }
  return;
}

static int8_t script_16C5F(void) {
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
    if (script_1587F(8, 30, 21, 0)) {
      local_EF = 1;
    } else {
      local_EF = 2;
    }
  } else if ((local_F0 == 13) | (local_F0 == 174)) {
    if (script_1587F(8, 30, 22, 0)) {
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
      goto L016FD8;
    }
  }
  L016FD8:;
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
        goto L01713C;
      }
    }
  }
  L01713C:;
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

__attribute__((scn_string_bindings(0, data_3FB5A, 1, data_3FB64, 2, data_3FB6E, 4, data_3FB82, 5, data_3FB8C, 6, data_3FB96, 7, data_3FBA0, 8, data_3FBAA, 9, data_3FBB4, 10, data_3FBBE, 11, data_3FBC8, 12, data_3FBD2, 13, data_3FBDC, 15, data_3FBF0, 16, data_3FBFA, 17, data_3FC04, 18, data_3FC0E, 19, data_3FC18, 20, data_3FC22, 21, data_3FC2C, 22, data_3FC36, 23, data_3FC40, 25, data_3FC54, 27, data_3FC68, 28, data_3FC72, 31, data_3FC90, 32, data_3FC9A, 33, data_3FCA4, 38, data_3FCD6, 39, data_3FCE0, 40, data_3FCEA, 41, data_3FCF4, 43, data_3FD08, 44, data_3FD12, 45, data_3FD1C, 56, data_3FD8A, 57, data_3FD94, 58, data_3FD9E, 59, data_3FDA8, 60, data_3FDB2, 61, data_3FDBC, 64, data_3FDDA, 65, data_3FDE4, 66, data_3FDEE, 67, data_3FDF8, 68, data_3FE02, 69, data_3FE0C, 70, data_3FE16, 71, data_3FE20, 72, data_3FE2A, 74, data_3FE3E, 76, data_3FE52, 78, data_3FE66, 79, data_3FE70, 80, data_3FE7A, 81, data_3FE84, 82, data_3FE8E, 84, data_3FEA2, 86, data_3FEB6, 87, data_3FEC0, 88, data_3FECA, 89, data_3FED4, 90, data_3FEDE, 91, data_3FEE8, 92, data_3FEF2, 93, data_3FEFC, 94, data_3FF06, 96, data_3FF1A, 99, data_3FF38, 100, data_3FF42, 101, data_3FF4C, 102, data_3FF56, 103, data_3FF60, 104, data_3FF6A, 106, data_3FF7E, 107, data_3FF88, 111, data_3FFB0, 113, data_3FFC4, 115, data_3FFD8, 116, data_3FFE2, 117, data_3FFEC, 118, data_3FFF6, 119, data_40000, 120, data_4000A, 125, data_4003C, 127, data_40050, 128, data_4005A, 129, data_40064, 132, data_40082, 133, data_4008C, 134, data_40096, 135, data_400A0, 136, data_400AA, 137, data_400B4, 138, data_400BE, 139, data_400C8, 140, data_400D2, 141, data_400DC, 142, data_400E6, 143, data_400F0, 144, data_400FA, 150, data_40136, 158, data_40186, 159, data_40190, 160, data_4019A, 161, data_401A4, 163, data_401B8, 172, data_40212, 173, data_4021C, 174, data_40226, 175, data_40230, 182, data_40276, 185, data_40294, 186, data_4029E, 187, data_402A8, 198, data_40316, 200, data_4032A, 201, data_40334, 208, data_4037A, 209, data_40384, 216, data_403CA, 224, data_4041A, 225, data_40424, 226, data_4042E, 232, data_4046A, 234, data_4047E, 235, data_40488, 237, data_4049C, 239, data_404B0, 244, data_404E2, 245, data_404EC, 256, data_4055A, 258, data_4056E, 260, data_40582, 261, data_4058C, 265, data_405B4, 269, data_405DC, 271, data_405F0, 273, data_40604, 274, data_4060E, 275, data_40618, 276, data_40622, 277, data_4062C, 278, data_40636, 280, data_4064A, 282, data_4065E, 285, data_4067C, 288, data_4069A, 289, data_406A4, 290, data_406AE, 295, data_406E0, 297, data_406F4, 299, data_40708, 300, data_40712, 301, data_4071C, 302, data_40726, 303, data_40730, 304, data_4073A, 308, data_40762, 311, data_40780, 313, data_40794, 315, data_407A8, 316, data_407B2, 317, data_407BC, 318, data_407C6, 331, data_40848)))
static void script_171AE(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F4 = arg0;
  switch (local_F4) {
    case 1:
      task_308 = 90;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032O001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A136");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O002");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A137");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O007");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A138");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O005");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A139");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O006");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 2:
      task_308 = 90;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032O001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A136");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O002");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A140");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O007");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A138");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O005");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A139");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O006");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 3:
      task_308 = 90;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P002");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P003");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A141");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A142");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P005");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SORRY_ABOUT_OTHER_DAY_CALL, 1);
      break;
    case 4:
      task_308 = 100;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P006");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A143");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P007");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A144");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 101;
      task_30C = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P009");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FLOWERS_THANKS_CALL, 1);
      break;
    case 5:
      task_308 = 110;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A145");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P011");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A146");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P012");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A147");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P013");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A148");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P014");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      SCNF_WaitFrames_0000(60);
      task_308 = 111;
      task_30C = Script_ShowUIMessageFromID("E1032A149");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P015");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_AFTER_RESCUE_CALL, 1);
      break;
    case 6:
      task_308 = 120;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A150");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A151");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 946, 1);
      break;
    case 7:
      task_308 = 130;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      SCNF_WaitFrames_0000(60);
      task_308 = 131;
      task_30C = Script_ShowUIMessageFromID("E1032A153");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P022");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P023");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P024");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SPRUNG_ON_YOU_CALL, 1);
      break;
    case 8:
      task_308 = 140;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P025");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A155");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A156");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 141;
      task_30C = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P027");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 9:
      task_308 = 150;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P028");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A158");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P030");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 151;
      task_30C = Script_ShowUIMessageFromID("E1032A159");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P066");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 10:
      task_308 = 160;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P032");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A160");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P033");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      SCNF_WaitFrames_0000(60);
      task_308 = 161;
      if ((local_F8 == 13) | (local_F8 == 174)) {
        task_30C = Script_ShowUIMessageFromID("E1032P034");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
        local_F4 = local_FF;
        switch (local_F4) {
          case 0:
            task_30C = Script_ShowUIMessageFromID("E1032P080");
            while (Script_convGetIsActive((const void *)task_30C)) {
              SCNF_WaitFrames_0000(1);
            }
            task_30C = 0;
            break;
          case 1:
            task_30C = Script_ShowUIMessageFromID("E1032P081");
            while (Script_convGetIsActive((const void *)task_30C)) {
              SCNF_WaitFrames_0000(1);
            }
            task_30C = 0;
            break;
        }
      }
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P035");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A141");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P036");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 11:
      task_308 = 162;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P025");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A160");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P033");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      SCNF_WaitFrames_0000(60);
      task_308 = 163;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F4 = local_FF;
      switch (local_F4) {
        case 0:
          task_30C = Script_ShowUIMessageFromID("E1032P082");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
        case 1:
          task_30C = Script_ShowUIMessageFromID("E1032P083");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
        case 2:
          task_30C = Script_ShowUIMessageFromID("E1032P084");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032P085");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
      }
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P086");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 12:
      task_308 = 370;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A165");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P037");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P120");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A173");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 371;
      task_30C = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P027");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 537, 1);
      break;
    case 13:
      task_308 = 380;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A170");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P044");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A200");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A172");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P045");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A173");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P046");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P047");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 381;
      task_30C = Script_ShowUIMessageFromID("E1032A201");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P049");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A176");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P050");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A177");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P051");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SHRINE_KITTEN_CALL, 1);
      break;
    case 14:
      task_308 = 390;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A178");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P052");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A179");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A180");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A181");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 391;
      task_30C = Script_ShowUIMessageFromID("E1032P053");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P087");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P055");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A182");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P051");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KITTEN_RECOVERED_CALL, 1);
      break;
    case 15:
      task_308 = 400;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A183");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P038");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A184");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P056");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P088");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P058");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A220");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A185");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P059");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KEEP_KITTEN_CALL, 1);
      break;
    case 16:
      task_308 = 410;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P060");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A186");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P061");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A187");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P062");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 411;
      task_30C = Script_ShowUIMessageFromID("E1032A188");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P063");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      SCNF_WaitFrames_0000(40);
      task_30C = Script_ShowUIMessageFromID("E1032P064");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P065");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A189");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P066");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A190");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_STAY_AWAY_FROM_HARBOR_CALL, 1);
      break;
    case 17:
      task_308 = 270;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P067");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A191");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A192");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P069");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A193");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TALK_TO_INE_CALL, 1);
      break;
    case 24:
      task_308 = 280;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P070");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A194");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P071");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A195");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P072");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P073");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A151");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P074");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P075");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P089");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P090");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 281;
      task_30C = Script_ShowUIMessageFromID("E1032A196");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P079");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A197");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FEELS_WATCHED_CALL, 1);
      break;
    case 21:
      task_308 = 290;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P091");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P092");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A203");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P093");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      SCNF_WaitFrames_0000(60);
      task_30C = Script_ShowUIMessageFromID("E1032P094");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P095");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_RELAY_RACE_PHOTO_CALL, 1);
      break;
    case 26:
      task_308 = 300;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P096");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P097");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 301;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F4 = local_FF;
      switch (local_F4) {
        case 0:
          task_30C = Script_ShowUIMessageFromID("E1032P098");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
        case 1:
          task_30C = Script_ShowUIMessageFromID("E1032P099");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
        case 2:
          task_30C = Script_ShowUIMessageFromID("E1032P100");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
      }
      task_30C = Script_ShowUIMessageFromID("E1032A206");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 18:
      task_308 = 310;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P101");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A207");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P102");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A208");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P103");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 311;
      task_30C = Script_ShowUIMessageFromID("E1032A209");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P104");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P105");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A210");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P106");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P107");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P108");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_PARENTS_IN_VANCOUVER_CALL, 1);
      break;
    case 19:
      task_308 = 320;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P109");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P110");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A211");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P111");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P112");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 321;
      task_30C = Script_ShowUIMessageFromID("E1032A212");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P113");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A158");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P114");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P115");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_CALL_NAOYUKI_AND_ICHIRO_CALL, 1);
      break;
    case 25:
      task_308 = 330;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P117");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A214");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P118");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P119");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P120");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 331;
      task_30C = Script_ShowUIMessageFromID("E1032A215");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P121");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P123");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A217");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TRIP_INVITATION_CALL, 1);
      break;
    case 20:
      task_308 = 340;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P124");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P125");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A218");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P126");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P127");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P128");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_LAPIS_FORTUNETELLER_CALL, 1);
      break;
    case 23:
      task_308 = 350;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032P109");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P117");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A214");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P129");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P130");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P131");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A219");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P132");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      SCNF_WaitFrames_0000(60);
      task_308 = 351;
      task_30C = Script_ShowUIMessageFromID("E1032A153");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P133");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A220");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P134");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_NURSERY_TEACHER_CALL, 1);
      break;
    case 22:
      task_308 = 360;
      Script_scenCreateTask(script_1D235, 0);
      task_30C = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A221");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P135");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P136");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A222");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P137");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P138");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A223");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P139");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_308 = 361;
      task_30C = Script_ShowUIMessageFromID("E1032A224");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      SCNF_WaitFrames_0000(60);
      task_30C = Script_ShowUIMessageFromID("E1032P140");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A225");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A226");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FIRST_MEETING_CALL, 1);
      break;
  }
  return;
}

__attribute__((scn_string_bindings(5, data_40884, 7, data_40898, 8, data_408A2, 21, data_40924, 22, data_4092E, 30, data_4097E, 32, data_40992, 33, data_4099C, 48, data_40A32, 49, data_40A3C)))
static void script_1B06C(void) {
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    task_308 = 250;
    Script_scenCreateTask(script_1D235, 0);
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032E001");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_FC == 1) {
        task_30C = Script_ShowUIMessageFromID("E1032E002");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032E003");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
    task_30C = Script_ShowUIMessageFromID("E1032A010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032E004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      switch (local_FC) {
        case 0:
          task_30C = Script_ShowUIMessageFromID("E1032A011");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032E005");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032A012");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
        case 1:
          task_30C = Script_ShowUIMessageFromID("E1032A013");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032E006");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032A014");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
        case 2:
          task_30C = Script_ShowUIMessageFromID("E1032A015");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032E007");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032A016");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
      }
    }
    task_30C = Script_ShowUIMessageFromID("E1032E008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_308 = 251;
    task_30C = Script_ShowUIMessageFromID("E1032E009");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032A017");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032E010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032E011");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_FC == 1) {
        task_30C = Script_ShowUIMessageFromID("E1032E012");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032E013");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032A018");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_FC == 1) {
        task_30C = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032A020");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
    task_30C = Script_ShowUIMessageFromID("E1032A021");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  } else {
    task_308 = 260;
    Script_scenCreateTask(script_1D235, 0);
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032F001");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_FC == 1) {
        task_30C = Script_ShowUIMessageFromID("E1032F002");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032F003");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
    task_30C = Script_ShowUIMessageFromID("E1032A022");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032F004");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      switch (local_FC) {
        case 0:
          task_30C = Script_ShowUIMessageFromID("E1032A011");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032F005");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032A012");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
        case 1:
          task_30C = Script_ShowUIMessageFromID("E1032A013");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032F006");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032A023");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
        case 2:
          task_30C = Script_ShowUIMessageFromID("E1032A024");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032F007");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032A025");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          task_30C = Script_ShowUIMessageFromID("E1032A026");
          while (Script_convGetIsActive((const void *)task_30C)) {
            SCNF_WaitFrames_0000(1);
          }
          task_30C = 0;
          break;
      }
    }
    task_308 = 261;
    task_30C = Script_ShowUIMessageFromID("E1032F008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032F009");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032F010");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_FC == 1) {
        task_30C = Script_ShowUIMessageFromID("E1032F011");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032F012");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032F013");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_FC == 1) {
        task_30C = Script_ShowUIMessageFromID("E1032F014");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032F015");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_FC == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032A018");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
    task_30C = Script_ShowUIMessageFromID("E1032A027");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  }
  return;
}

static int8_t script_1BB34(void) {
  int8_t local_FF;
  local_FF = 0;
  task_308 = 55;
  Script_scenCreateTask(script_1D235, 0);
  local_FF = script_1BB5C();
  return local_FF;
}

static int8_t script_1BB5C(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  local_F7 = 0;
  task_30C = Script_ShowUIMessageFromID("E1032A034");
  while (Script_convGetIsActive((const void *)task_30C)) {
    SCNF_WaitFrames_0000(1);
  }
  task_30C = 0;
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    local_F8 = 0;
  } else {
    local_F8 = 2;
  }
  script_1BC77(local_F8);
  local_FC = script_1BDC9();
  script_1BDE4(local_FC);
  if (script_1BD60(local_F8, local_FC)) {
    if (local_F8 == 0) {
      local_F8 = 2;
    } else {
      local_F8 = 0;
    }
    task_308 = 56;
    Script_mapmSetCamFlags(0, 0);
    Script_charaVisUtils(CHARA_AKIR, 1);
    script_1BC77(local_F8);
    local_FC = script_1BDC9();
    script_1BDE4(local_FC);
    Script_mapmSetCamFlags(0, 1);
    Script_charaVisUtils(CHARA_AKIR, 0);
    if (script_1BD60(local_F8, local_FC)) {
      local_F7 = script_1C1BA();
    } else {
      script_1BECD(local_FC);
    }
  } else {
    script_1BECD(local_FC);
  }
  return local_F7;
}

static void script_1BC77(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      task_30C = Script_ShowUIMessageFromID("E1032H005");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 1:
      task_30C = Script_ShowUIMessageFromID("E1032H004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 2:
      task_30C = Script_ShowUIMessageFromID("E1032H009");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 3:
      task_30C = Script_ShowUIMessageFromID("E1032H006");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
  }
  return;
}

static int32_t script_1BD60(int32_t arg0, int32_t arg1) {
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

static int32_t script_1BDC9(void) {
  return SCNF_RequestActionSelectorFromParams_0008(7, 8, 6, 5, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
}

static void script_1BDE4(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 5:
      task_30C = Script_ShowUIMessageFromID("E1032A042");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 6:
      task_30C = Script_ShowUIMessageFromID("E1032A040");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 7:
      task_30C = Script_ShowUIMessageFromID("E1032A043");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
    case 8:
      task_30C = Script_ShowUIMessageFromID("E1032A041");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      break;
  }
  return;
}

__attribute__((scn_string_bindings(1, data_40AB4, 2, data_40ABE, 3, data_40AC8, 4, data_40AD2)))
static void script_1BECD(int32_t arg0) {
  int32_t local_FC;
  task_308 = 58;
  script_1C0F2(1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 714)) {
    if ((arg0 == 5) | (arg0 == 6) | (arg0 == 7) | (arg0 == 8)) {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_F8 == 0) {
      } else if (local_F8 == 1) {
        task_30C = Script_ShowUIMessageFromID("E1032A045");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else if (local_F8 == 2) {
        task_30C = Script_ShowUIMessageFromID("E1032A046");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032A047");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    } else {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_F8 == 0) {
      } else if (local_F8 == 1) {
        task_30C = Script_ShowUIMessageFromID("E1032A046");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032A047");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
    }
  } else {
    task_30C = Script_ShowUIMessageFromID("E1032A044");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 9) {
    local_FC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 9);
    local_FC = local_FC + 1;
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 9, local_FC);
  }
  return;
}

static void script_1C0BB(void) {
  task_30C = Script_ShowUIMessageFromID("E1032A051");
  while (Script_convGetIsActive((const void *)task_30C)) {
    SCNF_WaitFrames_0000(1);
  }
  task_30C = 0;
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_1C0F2(int32_t arg0) {
  Script_executeAicaCommand(525739, 0, 0);
  SCNF_WaitFrames_0000(10);
  if (arg0) {
    task_30C = Script_ShowUIMessageFromID("E1032A039");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
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

__attribute__((scn_string_bindings(0, data_40AFA, 1, data_40B04, 2, data_40B0E, 3, data_40B18, 4, data_40B22, 5, data_40B2C, 6, data_40B36, 7, data_40B40, 8, data_40B4A, 10, data_40B5E, 11, data_40B68, 15, data_40B90, 16, data_40B9A, 17, data_40BA4, 18, data_40BAE, 19, data_40BB8, 20, data_40BC2, 21, data_40BCC, 22, data_40BD6, 23, data_40BE0, 24, data_40BEA, 25, data_40BF4, 26, data_40BFE, 27, data_40C08, 28, data_40C12, 29, data_40C1C, 33, data_40C44, 34, data_40C4E, 35, data_40C58, 36, data_40C62, 37, data_40C6C, 38, data_40C76, 39, data_40C80, 40, data_40C8A, 41, data_40C94, 42, data_40C9E, 43, data_40CA8, 44, data_40CB2, 45, data_40CBC, 46, data_40CC6, 47, data_40CD0, 48, data_40CDA, 49, data_40CE4, 52, data_40D02, 53, data_40D0C, 54, data_40D16, 59, data_40D48)))
static int8_t script_1C1BA(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  int32_t local_F0;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F7 = 0;
  task_308 = 57;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (local_FC >= 17) {
    task_30C = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032A120");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032H010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032A066");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032H011");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    SCNF_WaitFrames_0000(60);
    if (((local_F8 >= 170) & (local_F8 <= 193)) | ((local_F8 >= 220) & (local_F8 <= 241))) {
      local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F0 == 0) {
        task_30C = Script_ShowUIMessageFromID("E1032J001");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032A131");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032J010");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032J013");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      } else {
        task_30C = Script_ShowUIMessageFromID("E1032J001");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032A134");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032J015");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
        task_30C = Script_ShowUIMessageFromID("E1032A135");
        while (Script_convGetIsActive((const void *)task_30C)) {
          SCNF_WaitFrames_0000(1);
        }
        task_30C = 0;
      }
      script_1C0F2(0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 546)) {
      task_30C = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J018");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J020");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 546, 0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED)) {
      task_30C = Script_ShowUIMessageFromID("E1032J016");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A161");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J017");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A162");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
    } else {
      task_30C = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A131");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J010");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A132");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J011");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J012");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) {
    task_30C = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032A120");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032H010");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032A066");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032H011");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    SCNF_WaitFrames_0000(60);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 546)) {
      task_308 = 65;
      task_30C = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J018");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J020");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 546, 0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED)) {
      task_308 = 66;
      task_30C = Script_ShowUIMessageFromID("E1032J016");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A161");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J017");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A162");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE)) {
      task_30C = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A067");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J002");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
    } else {
      task_30C = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J003");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A069");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032J004");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
      task_30C = Script_ShowUIMessageFromID("E1032A070");
      while (Script_convGetIsActive((const void *)task_30C)) {
        SCNF_WaitFrames_0000(1);
      }
      task_30C = 0;
    }
  } else {
    local_F7 = 1;
    task_30C = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032A048");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032H008");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032A049");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    task_30C = Script_ShowUIMessageFromID("E1032A050");
    while (Script_convGetIsActive((const void *)task_30C)) {
      SCNF_WaitFrames_0000(1);
    }
    task_30C = 0;
    script_1C0F2(1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  }
  return local_F7;
}

static void script_1CE89(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  script_76F7(CHARA_AKIR, 38.009102f, 0.114f, 9.338f);
  script_771F(CHARA_AKIR, 0.0f, 244.4f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_SPEAK_TEL_LP_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_3EC60, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 0, 0, 0, 1);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_3EC60, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 1, 0, 0, 1);
  Script_scenCreateTask(script_1D1B2, 0);
  SCNF_WaitFrames_0000(1);
  Script_TELMUtils(TELM_CMD_Cleanup, task_2EC);
  Script_TELMUtils(TELM_CMD_SetTargetTaskAndMode, task_2EC, 0x52494B41, 3);
  local_F4.x = -0.05f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.02f;
  local_E8.x = 51336;
  (&local_E8)->y = 546;
  (&local_E8)->z = 55523;
  Script_TELMUtils(TELM_CMD_SetCustomReceiverOffset, task_2EC, &local_F4);
  Script_TELMUtils(TELM_CMD_SetCustomReceiverEuler, task_2EC, &local_E8);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_executeAicaCommand(591275, 0, 0);
  return;
}

static void script_1D00F(void) {
  task_308 = -1;
  Script_TELMUtils(TELM_CMD_Cleanup, task_2EC);
  return;
}

static void script_1D023(void) {
  script_76F7(CHARA_AKIR, 37.51f, 0.07f, 9.08f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_1D06D(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  task_37D = 1;
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
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_3ED44, 5);
  Script_TELMUtils(TELM_CMD_ResetPeerState, task_2EC);
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
  Script_TELMUtils(TELM_CMD_SetCustomReceiverOffset, task_2EC, &local_F4);
  Script_TELMUtils(TELM_CMD_SetCustomReceiverEuler, task_2EC, &local_E8);
  return;
}

static int32_t script_1D1B2(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = -0.092468f;
  (&local_F4)->y = 1.467227f;
  (&local_F4)->z = -0.12347f;
  local_E8.x = -546;
  (&local_E8)->y = 728;
  (&local_E8)->z = -4551;
  task_37D = 0;
  for (;;) {
    if (task_37D) {
      break;
    }
    Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E8);
    Script_SetMotnFlags(CHARA_AKIR, 11, &local_F4, 0x39000000);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_1D235(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_308;
    switch (local_FC) {
      case -1:
        goto L01D953;
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
        Script_seqEnterEvtCam(2006, task_2EC, task_2EC);
        break;
      case 56:
        Script_seqEnterEvtCam(2007, task_2EC, task_2EC);
        break;
      case 57:
        Script_seqEnterEvtCam(2008, task_2EC, task_2EC);
        break;
      case 58:
        Script_seqEnterEvtCam(2010, task_2EC, task_2EC);
        break;
      case 65:
        Script_seqEnterEvtCam(2016, 0, AUTH_None);
        break;
      case 66:
        Script_seqEnterEvtCam(2019, 0, AUTH_None);
        break;
      case 70:
        local_FC = task_318;
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
        task_318 = task_318 + 1;
        if (task_318 > 4) {
          task_318 = 0;
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
    if (task_308) {
      task_308 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  L01D953:;
  task_308 = 0;
  return Script_srchControl(0);
}

static void script_1D961(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  SCNF_WaitFrames_0000(1);
  task_2E8 = Script_motiLoad(2, SHEN_NAME('D000'), "M_TPIN.BIN");
  SCNF_WaitFrames_0000(1);
  if (task_315 < 0) {
    script_1DA34();
  } else {
    script_12FCE();
    if (task_317) {
      Script_seqEnterEvtCam(2015, task_2EC, task_2EC);
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static void script_1D9D4(void) {
  if (task_315 < 0) {
    script_1DAD2();
  }
  Script_ReallocateUNFO(task_2E8);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  SCNF_WaitFrames_0000(10);
  if (task_315 < 0) {
    script_76F7(CHARA_AKIR, 37.51f, 0.07f, 9.08f);
    Script_InitializeOsag(CHARA_AKIR);
  }
  return;
}

static void script_1DA34(void) {
  Script_seqEnterEvtCam(2004, 0, AUTH_None);
  script_76F7(CHARA_AKIR, 38.009102f, 0.114f, 9.338f);
  script_771F(CHARA_AKIR, 0.0f, 244.4f, 0.0f);
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

static void script_1DAD2(void) {
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

static void script_1DB6E(int32_t arg0) {
  int32_t local_FC;
  if (arg0 != -1) {
    task_380 = arg0;
  }
  local_FC = task_380;
  if (local_FC == 0) {
    Script_gachaponUtils(0, data_40F04);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_40FAD);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_410E8);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_4121D);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_412C4);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_41431);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_4162A);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_416FD);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_4176E);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_417BB);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_417F4);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_418E3);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_41946);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_4199B);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_419E2);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_41A1B);
  } else {
    task_380 = -1;
  }
  return;
}

static int16_t script_1DD0F(int8_t arg0, int8_t arg1) {
  int16_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_FE = -1;
  if (arg0 == -1) {
    local_FD = task_380;
  } else {
    local_FD = arg0;
  }
  local_F8 = local_FD;
  switch (local_F8) {
    case 0:
      local_FE = *(int16_t *)((int8_t *)&data_40DA8 + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_40DC0 + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_40DE4 + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_40E10 + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_40E24 + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_40E4C + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_40E70 + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_40E88 + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_40E98 + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_40EA0 + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_40EA8 + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_40ECC + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_40EDC + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_40EE8 + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_40EF4 + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_40EFC + (arg1 << 1));
      break;
  }
  return local_FE;
}

static void script_1DEFA(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_40D88[*(int32_t *)arg0] - data_40D98[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_40D88[*(int32_t *)arg0] - data_40D98[*(int32_t *)arg0]);
  return;
}

static int32_t script_1DF49(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_40D88[arg0] - data_40D98[arg0]) {
    (void)1;
  } else if (arg0 == 15) {
    (void)3;
  } else {
    (void)2;
  }
  end:;
}

static void script_1DF9C(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
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
        goto L01E00D;
      }
      L01E00D:;
      break;
    case 3:
      local_EC = arg2;
      if (local_EC == 6) {
        (void)((local_F3 = -1) & 1);
        goto L01E034;
      }
      L01E034:;
      break;
    case 4:
      local_EC = arg2;
      if ((local_EC == 7) | (local_EC == 15) | (local_EC == 16) | (local_EC == 17) | (local_EC == 18)) {
        (void)((local_F3 = -1) & 1);
        goto L01E073;
      }
      L01E073:;
      break;
    case 5:
      (void)((local_F3 = -1) & 1);
      break;
    case 6:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 2) | (local_EC == 3)) {
        (void)((local_F3 = -1) & 1);
        goto L01E0C1;
      }
      L01E0C1:;
      break;
    case 8:
      local_EC = arg2;
      if (local_EC == 3) {
        (void)((local_F3 = -1) & 1);
        goto L01E0E8;
      }
      L01E0E8:;
      break;
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
  if (local_F3 & 8) {
    arg2 = arg2 + data_40D88[arg1];
    local_F4.x = (&local_F4)->x * arg3;
    (&local_F4)->y = (&local_F4)->y * arg3;
    (&local_F4)->z = (&local_F4)->z * arg3;
  } else {
    local_F4.x = (&local_F4)->x * arg3 * 0.33333f;
    (&local_F4)->y = (&local_F4)->y * arg3 * 0.33333f;
    (&local_F4)->z = (&local_F4)->z * arg3 * 0.33333f;
  }
  if (local_F2 & 4) {
    script_1DB6E(arg1);
    Script_gachaponUtils(5, arg0, arg2);
  } else {
    script_1DB6E(arg1);
    Script_gachaponUtils(1, arg0, arg2);
  }
  if ((((scn3_bits){.f = (&local_F4)->x}).i != 0x3F800000) & (((scn3_bits){.f = (&local_F4)->x}).i != 0)) {
    Script_hmdlApplyScale(arg0, &local_F4);
  }
  return;
}

static void script_1E1D3(void) {
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
      local_D8 = *(&data_41A54 + local_FC);
      local_E8.x = *(float *)((const char *)&data_41A9C + local_FC * 20);
      (&local_E8)->y = *(float *)((const char *)&data_41A9C + (local_FC * 20 + 4));
      (&local_E8)->z = *(float *)((const char *)&data_41A9C + (local_FC * 20 + 8));
      local_DC.x = 0;
      (&local_DC)->y = *(const uint32_t *)((const char *)&data_41A9C + (local_FC * 20 + 12));
      (&local_DC)->z = 0;
      Script_EVT_SetPosition(local_D8, POS_XYZ, &local_E8);
      Script_EVT_SetRotation(local_D8, 0x38000000, &local_DC, 0);
      Script_charaDisplay(local_D8, CDM_Show);
      Script_SetOrGetWOCCValue(local_D8, *((int8_t *)&data_41A9C + (local_FC * 20 + 16)));
      Script_MoveObjectToObject(*(&data_41A84 + local_FC), *(&data_41A54 + local_FC));
      Script_EVT_SetPosition(*(&data_41A84 + local_FC), POS_XYZ, &data_41B14);
      Script_EVT_SetRotation(*(&data_41A84 + local_FC), 0x38000000, &data_41B20, 0);
      Script_charaDisplay(*(&data_41A84 + local_FC), CDM_Show);
      goto L01E30D;
    }
    L01E30D:;
    local_FC = local_FC + 1;
  }
  task_384 = 0;
  (void)((task_38C = 0) & 2);
  (void)((task_38D = 0) & 2);
  (void)((task_38E = 0) & 2);
  return;
}

static int32_t script_1E33F(int32_t arg0) {
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
  if (task_38C & 2) {
    (void)((local_DB = 0) & 0x80);
  }
  return local_DB & 8;
}

static int32_t script_1E3A9(int32_t arg0) {
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
      local_D0 = *(&data_41A54 + local_E0);
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
  script_212C7(arg0, &local_F4);
  script_212C7(arg0, &local_E8);
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

static int32_t script_1E5DA(void) {
  int16_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F8 = 0;
  local_FE = 0;
  while (local_FE < 6) {
    local_F4 = 9;
    if (local_F4 & (1 << local_FE)) {
      local_F0 = *(&data_41A54 + local_FE);
      if (script_1E33F(local_F0)) {
        Script_scenToggleVisibleFor(local_F0, 1);
        Script_scenToggleVisibleFor(*(&data_41A84 + local_FE), 1);
      } else {
        Script_scenToggleVisibleFor(local_F0, 0);
        Script_scenToggleVisibleFor(*(&data_41A84 + local_FE), 0);
      }
      if (script_1E3A9(local_F0)) {
        local_F8 = local_F0;
        task_384 = local_F0;
      }
    }
    local_FE = local_FE + 1;
  }
  task_384 = local_F8;
  return local_F8;
}

static int32_t script_1E68A(void) {
  int32_t local_FC;
  int8_t local_FB;
  local_FC = 0;
  (void)((local_FB = 0) & 2);
  if (Script_IsPlayerAllowed(0)) {
    if (script_1E5DA()) {
      (void)((local_FB = -1) & 2);
      if (Script_GetControllerInput(1) == 0x400) {
        local_FC = 1;
      }
    }
  }
  if (local_FB & 8) {
    if (~task_38D) {
      (void)((task_38D = -1) & 2);
      Script_sicnUtils(0, 5, 1);
      Script_uiUtils(0, 6, 0);
    }
  } else {
    if (task_38D & 4) {
      (void)((task_38D = 0) & 2);
      Script_sicnUtils(1, 0, 0);
      Script_uiUtils(0, 0, 0);
    }
  }
  return local_FC;
}

__attribute__((scn_string_bindings(0, data_41B2C, 2, data_41B3F, 3, data_41B4F)))
static int32_t script_1E72D(void) {
  Script_LoadVoiceFile(1, "E1066");
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 7, "e1gachap.snd", 0x4000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/D000/", "HI_GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_41B57, 1, data_41B5C, 2, data_41B62)))
static void script_1E7BF(void) {
  Script_LoadSND(0, 7, "FREE", 0);
  Script_LoadVoiceFile(0, "E1066");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "HI_GACH");
  return;
}

__attribute__((scn_string_bindings(0, data_41B6A)))
static void script_1E7EC(int32_t arg0) {
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
      local_F0 = *(&data_41A54 + local_FC);
      local_EC = *(&data_41A6C + local_FC);
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

static void script_1E8BE(void) {
  Vector3i local_F4;
  (void)((task_38C = -1) & 8);
  Script_scenCreateTask(script_1E72D, 0);
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
  (void)((task_38D = 0) & 8);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  SCNF_WaitFrames_0000(1);
  Script_uiUtils(0, 6, 0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_41B74);
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

static void script_1EA65(void) {
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  script_1E7BF();
  Script_SetGameStateTo(AUTH_None);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_uiUtils(0, 0, 0);
  (void)((task_38C = 0) & 8);
  Script_scenToggleVisibleFor(SHEN_NAME('DAMY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPG'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPB'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GABS'), 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_shenClearFreeState(1);
  (void)((task_38C = 0) & 8);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_1EB3F(void) {
  int32_t local_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  script_1F3B9(CHARA_AKIR, 1, 8, 1);
  script_1F3B9(CHARA_AKIR, 0, 8, 1);
  local_F4[0] = local_F4[1] = local_F4[2] = 0;
  local_E8 = 0;
  while (local_E8 < 6) {
    if (task_384 == *(&data_41A54 + local_E8)) {
      local_EC = *((int8_t *)&data_41A9C + (local_E8 * 20 + 17));
      break;
    }
    local_E8 = local_E8 + 1;
  }
  local_F0 = Script_PlayerStatsUtils(PS_GetIndexedU16Table3Slot, local_EC);
  Script_PlayerStatsUtils(PS_SetIndexedU16Table3Slot, local_EC, local_F0 + 1);
  Script_InitializeOsag(CHARA_AKIR);
  return;
}

static void script_1EC03(void) {
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_gachaponUtils(2, 0x4849454B);
  return;
}

static void script_1EC32(int32_t arg0) {
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
  script_212C7(arg0, &local_F4);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, (&local_D0)->y, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  local_F4.x = -0.0511f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.650771f;
  script_212C7(task_384, &local_F4);
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

static void script_1EDBB(int32_t arg0, int32_t arg1) {
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
  script_212C7(task_384, &local_F4);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_mtwkGetSetRotation(task_384, &local_E8, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static int32_t script_1EE3C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
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

static int32_t script_1EEF6(void) {
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
      Script_scenCreateTask(script_1F031, 1, local_FC - 100.0f);
    }
    if (-!(local_FC < 170.0f) & ~local_FA) {
      (void)((local_FA = -1) & 2);
      Script_scenCreateTask(script_1F101, 0);
    }
    if (-!(local_FC < 215.0f) & ~local_F9) {
      (void)((local_F9 = -1) & 2);
      Script_scenCreateTask(script_1F1CA, 0);
    }
    if (-!(local_FC < 280.0f) & ~local_F8) {
      (void)((local_F8 = -1) & 2);
      Script_scenCreateTask(script_1F274, 0);
    }
    if (-!(local_FC < 300.0f) & ~local_F7) {
      (void)((local_F7 = -1) & 2);
      Script_scenCreateTask(script_1F2BE, 0);
    }
    if (-!(local_FC < 310.0f) & ~local_F6) {
      (void)((local_F6 = -1) & 2);
      Script_scenCreateTask(script_1F5AA, 0);
    }
    if (local_F6 & 0x40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_1F031(float arg0) {
  int32_t local_F4[3];
  int32_t local_F0;
  if (!(40.0f - arg0 <= 0.0f)) {
    local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, 40.0f - arg0);
    script_1F3B9(CHARA_AKIR, 1, 0, local_F0);
    local_F4[0] = 0;
    local_F4[1] = -5643;
    local_F4[2] = 728;
    script_1EE3C(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], local_F0);
  }
  script_1F3B9(CHARA_AKIR, 1, 1, 16);
  local_F4[1] = -4915;
  script_1EE3C(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  local_F4[1] = -5643;
  script_1EE3C(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  Script_scenCreateTask(script_20204, 0);
  end:;
}

static int32_t script_1F101(void) {
  int32_t local_F4[3];
  script_1F3B9(CHARA_AKIR, 1, 2, 40);
  local_F4[0] = -7827;
  local_F4[1] = 4187;
  local_F4[2] = 4187;
  script_1EE3C(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  local_F4[0] = -7827;
  local_F4[1] = 3458;
  local_F4[2] = 4187;
  script_1EE3C(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  Script_scenCreateTask(script_1FAB9, 1, 20);
  local_F4[0] = 6735;
  local_F4[1] = 2821;
  local_F4[2] = 1456;
  script_1EE3C(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 16);
  SCNF_WaitFrames_0000(4);
  end:;
}

static int32_t script_1F1CA(void) {
  int32_t local_F4[3];
  script_1F3B9(CHARA_AKIR, 1, 3, 40);
  local_F4[0] = -7827;
  local_F4[1] = 3458;
  local_F4[2] = 4187;
  script_1EE3C(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 24);
  Script_scenCreateTask(script_1FAB9, 1, 14);
  local_F4[0] = 6735;
  local_F4[1] = 2821;
  local_F4[2] = 1456;
  script_1EE3C(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 16);
  Script_scenCreateTask(script_202C4, 0);
  SCNF_WaitFrames_0000(4);
  Script_scenCreateTask(script_205E3, 0);
  end:;
}

static int32_t script_1F274(void) {
  int32_t local_F4[3];
  script_1F3B9(CHARA_AKIR, 1, 4, 20);
  local_F4[0] = 2548;
  local_F4[1] = -3822;
  local_F4[2] = -2184;
  script_1EE3C(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 20);
  end:;
}

static int32_t script_1F2BE(void) {
  int32_t local_F4[3];
  script_1F3B9(CHARA_AKIR, 1, 5, 20);
  local_F4[0] = -364;
  local_F4[1] = -2730;
  local_F4[2] = -182;
  script_1EE3C(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 20);
  local_F4[0] = -546;
  local_F4[1] = -1274;
  local_F4[2] = 2730;
  Script_scenCreateTask(script_1EE3C, 5, 1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 40);
  local_F4[0] = -7645;
  local_F4[1] = 3822;
  local_F4[2] = 910;
  local_F4[0] = -2419;
  local_F4[1] = 4176;
  local_F4[2] = 5642;
  Script_scenCreateTask(script_1EE3C, 5, 0, *(int32_t *)local_F4, local_F4[1], local_F4[2], 30);
  script_1F3B9(CHARA_AKIR, 1, 6, 10);
  script_1F3B9(CHARA_AKIR, 0, 6, 10);
  end:;
}

static void script_1F3B9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_FF20[56];
  int32_t local_FF1C;
  int32_t local_FF18;
  int32_t local_FF14;
  local_FF18 = -1;
  local_FF14 = arg2;
  switch (local_FF14) {
    case 0:
      local_FF18 = (int32_t)data_41B84;
      break;
    case 1:
      local_FF18 = (int32_t)data_41C68;
      break;
    case 2:
      local_FF18 = (int32_t)data_41D4C;
      break;
    case 3:
      local_FF18 = (int32_t)data_41E30;
      break;
    case 4:
      local_FF18 = (int32_t)data_41F14;
      break;
    case 5:
      local_FF18 = (int32_t)data_41FF8;
      break;
    case 6:
      local_FF18 = (int32_t)data_420DC;
      break;
    case 7:
      local_FF18 = (int32_t)data_421C0;
      break;
    case 8:
      local_FF18 = (int32_t)data_422A4;
      break;
    case 9:
      {
        int32_t local_FF10;
        local_FF10 = 0;
        while (local_FF10 < 57) {
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_41FF8 + (local_FF10 << 2));
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
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_41FF8 + (local_FF10 << 2));
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

static int32_t script_1F5AA(void) {
  Script_seqEnterEvtCam(7008, task_384, task_384);
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
    Script_fixoAttachEx(task_388, CHARA_AKIR, 18, &local_F4, &local_E8);
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
    Script_fixoAttachEx(SHEN_NAME('KEIH'), task_388, 18, &local_F4, &local_E8);
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
    Script_fixoAttachEx(task_388, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 410.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('GABS'));
  Script_scenCreateTask(script_1FA39, 1, SHEN_NAME('GABS'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 430.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_1F3B9(CHARA_AKIR, 1, 5, 20);
  {
    int32_t unused_F4[3];
    int32_t local_E8[3];
    local_E8[0] = 909;
    local_E8[1] = -1657;
    local_E8[2] = 1431;
    Script_scenCreateTask(script_1EE3C, 5, 1, *(int32_t *)local_E8, local_E8[1], local_E8[2], 20);
  }
  {
    int32_t unused_F4[3];
    int32_t local_E8[3];
    local_E8[0] = -38;
    local_E8[1] = 476;
    local_E8[2] = 9888;
    Script_scenCreateTask(script_1EE3C, 5, 0, *(int32_t *)local_E8, local_E8[1], local_E8[2], 20);
  }
  Script_scenCreateTask(script_1F947, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 450.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(7009, task_384, task_384);
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
  if (task_38F & 0x80) {
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
  Script_fixoClearAttachment(task_388);
  Script_scenCreateTask(script_1FA39, 1, task_388);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  end:;
}

static int32_t script_1F947(void) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F4.x = *(float *)&data_42394;
  (&local_F4)->y = *(float *)(&data_42394 + 1);
  (&local_F4)->z = *(float *)(&data_42394 + 2);
  local_F0 = 20;
  while (local_F0) {
    local_F4.x = (&local_F4)->x + ((&data_42388)->x - *(float *)&data_42394) / 20.0f;
    (&local_F4)->y = (&local_F4)->y + ((&data_42388)->y - *(float *)(&data_42394 + 1)) / 20.0f;
    (&local_F4)->z = (&local_F4)->z + ((&data_42388)->z - *(float *)(&data_42394 + 2)) / 20.0f;
    Script_fixoClearAttachment(task_388);
    Script_fixoAttachEx(task_388, CHARA_AKIR, 12, &local_F4, &data_423A0);
    local_F0 = local_F0 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoAttachEx(task_388, CHARA_AKIR, 12, &data_42388, &data_423A0);
  return Script_srchControl(0);
}

static int32_t script_1FA39(int32_t arg0) {
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

static int32_t script_1FAB9(int32_t arg0) {
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
      if (task_384 == *(&data_41A54 + local_FC)) {
        local_F8 = *(&data_41A6C + local_FC);
        break;
      }
      local_FC = local_FC + 1;
    }
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, CHARA_PART_Get);
    local_DC = (&local_E0)->y;
    local_FC = 0;
    L01FB39:;
    if (local_FC == arg0 * 6 / 10) {
      Script_executeAicaCommand(66729, 0, 0);
    }
    Script_charaPosRotUtils(local_F8, 152, 0, &local_EC, CHARA_PART_Add);
    (&local_E0)->y = (&local_E0)->y + (Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.003f) - 0.0015f);
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, CHARA_PART_Set);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (!(local_FC >= arg0)) goto L01FB39;
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

static void script_1FBEB(int32_t arg0) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_D0[3];
  Vector3i local_C4;
  Script_MoveObjectToObject(SHEN_NAME('KEIH'), task_384);
  (&local_E8)->y = (&local_E8)->z = 0;
  local_E8.x = 0;
  Script_EVT_SetRotation(SHEN_NAME('KEIH'), 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(SHEN_NAME('KEIH'), -1);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4849454B);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_423AC);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_423B8, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_gachaponUtils(3, 0x4849454B, &data_423DC);
  SCNF_WaitFrames_0000(1);
  local_E0 = Script_scenCreateTask(script_1FE57, 0);
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
        goto L01FD38;
      }
    }
    L01FD38:;
    Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &local_E8);
    Script_fixoClearAttachment(SHEN_NAME('KEIH'));
    Script_fixoSetAttachmentOffset(SHEN_NAME('KEIH'), 0, &data_423AC, &data_423B8);
    local_C4.x = 0;
    (&local_C4)->y = -(&local_E8)->x;
    (&local_C4)->z = 0;
    Script_fixoAttachEx(SHEN_NAME('KEIH'), SHEN_NAME('YKUR'), 18, &data_423C4, &local_C4);
    SCNF_WaitFrames_0000(1);
    if (local_DC & 0x400) {
      if (~task_38E) {
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
  script_20C1D(arg0);
  Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &data_423D0);
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Hide);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 0, 1, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  return;
}

static int32_t script_1FE57(void) {
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

static void script_1FF2F(void) {
  return;
}

__attribute__((scn_string_bindings(5, data_4241A)))
static int32_t script_1FF32(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  while (task_38E & 0x40) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_38E = -1) & 0x10);
  local_F8 = arg0;
  switch (local_F8) {
    case 0:
      local_FC = script_21275(3, "E1066A001", "E1066A002", "E1066A003");
      break;
    case 1:
      local_FC = script_21275(2, "E1066A004", "E1066A005");
      break;
    case 2:
      local_FC = script_21275(3, "E1066A006", "E1066A007", "E1066A008");
      break;
    case 3:
      local_FC = script_21275(3, "E1066A009", "E1066A010", "E1066A011");
      break;
    case 4:
      local_FC = script_21275(2, "E1066A026", "E1066A027");
      break;
  }
  if (local_FC) {
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)((task_38E = 0) & 4);
  return Script_srchControl(0);
}

static int32_t script_20048(int16_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  while (task_38E & 0x40) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_38E = -1) & 4);
  local_F8 = task_380;
  local_F4 = script_1DD0F(local_F8, arg0);
  local_EC = script_1DF49(local_F8, arg0);
  local_F0 = Script_GetOwnedItemCount(local_F4, 0);
  local_FC = 0;
  if (local_F0) {
    local_FC = script_21275(3, "E1066A019", "E1066A020", "E1066A021");
  } else {
    local_E8 = local_EC;
    if (local_E8 == 0) {
      local_FC = script_21275(5, "E1066A014", "E1066A015", "E1066A016", "E1066A017", "E1066A018");
    } else if (local_E8 == 1) {
      local_FC = script_21275(3, "E1066A022", "E1066A025", "E1066A024");
    } else {
      if ((local_E8 == 2) | (local_E8 == 3)) {
        local_FC = Script_ShowUIMessageFromID("E1066A023");
        goto L02014B;
      }
    }
  }
  L02014B:;
  if (local_FC) {
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return (task_38E = 0) & 1;
}

static int32_t script_20171(void) {
  int32_t local_FC;
  local_FC = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(-100, 1);
  Script_SetStats(STATS_CASH, local_FC - 100);
  return Script_srchControl(0);
}

static void script_2019A(void) {
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

static int32_t script_20204(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  float local_E4;
  float local_E0;
  Script_fixoClearAttachment(0x6E496F63);
  Script_MoveObjectToObject(0x6E496F63, task_384);
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

static int32_t script_202C4(void) {
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
    task_388 = 0x47504347;
  } else if (local_B8 == 1) {
    task_388 = 0x42504347;
  } else if (local_B8 == 2) {
    task_388 = 0x52504347;
  } else {
    task_388 = 0x59504347;
  }
  Script_EVT_GetPosition(task_384, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(task_384, &local_E8, 0);
  local_F4.x = (&local_F4)->x + (Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.2f + ((float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y)) * -0.1f);
  (&local_F4)->y = (&local_F4)->y + 0.5f;
  (&local_F4)->z = (&local_F4)->z + (Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.2f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * -0.1f);
  Script_EVT_SetPosition(task_388, POS_XYZ, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('GABS'), POS_XYZ, &local_F4);
  Script_charaDisplay(task_388, CDM_Show);
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
    Script_EVT_SetPosition(task_388, POS_XYZ, &local_F4);
    Script_EVT_SetPosition(SHEN_NAME('GABS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_388, 0x38000000, &local_E8, 0);
    Script_EVT_SetRotation(SHEN_NAME('GABS'), 0x38000000, &local_E8, 0);
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 910;
  (&local_E8)->y = 1274;
  (&local_E8)->z = 0x8000;
  Script_mtwkGetSetRotation(task_388, &local_DC, 0);
  *(float *)&local_DC.x = (float)((&local_E8)->x - (&local_DC)->x) / 10.0f;
  *(float *)&(&local_DC)->y = (float)((&local_E8)->y - (&local_DC)->y) / 10.0f;
  *(float *)&(&local_DC)->z = (float)((&local_E8)->z - (&local_DC)->z) / 10.0f;
  local_BC = 10;
  while (local_BC) {
    local_E8.x = (&local_E8)->x + (&local_DC)->x;
    (&local_E8)->y = (&local_E8)->y + (&local_DC)->y;
    (&local_E8)->z = (&local_E8)->z + (&local_DC)->z;
    script_212FF(task_384, &local_E8);
    Script_EVT_SetRotation(task_388, 0x38000000, &local_E8, 0);
    Script_EVT_SetRotation(SHEN_NAME('GABS'), 0x38000000, &local_E8, 0);
    local_BC = local_BC - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(263337, 0, 0);
  end:;
}

static int32_t script_205E3(void) {
  Vector3 local_F4;
  Script_MoveObjectToObject(CHARA_AKIR, task_384);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  {
    int32_t local_F0;
    local_F0 = 15;
    while (local_F0) {
      (&local_F4)->z = (&local_F4)->z + 0.002f;
      Script_MoveObjectToObject(CHARA_AKIR, task_384);
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
      Script_MoveObjectToObject(CHARA_AKIR, task_384);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_MoveObjectToObject(CHARA_AKIR, -1);
      local_F0 = local_F0 - 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static void script_206C0(void) {
  Script_SetMotnFlags(CHARA_AKIR, 17, &data_424E4, 0x39000000);
  return;
}

static int32_t script_206D8(void) {
  for (;;) {
    script_206C0();
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_206EB(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 1, 1, 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, 0);
  script_1F3B9(SHEN_NAME('YKUR'), 1, 9, 1);
  SCNF_WaitFrames_0000(1);
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, SHEN_NAME('KEIH'));
  script_1F3B9(SHEN_NAME('YKUR'), 1, 10, 1);
  L02075F:;
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4849454B);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_424F0);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_424FC, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  SCNF_WaitFrames_0000(1);
  if (!Script_GetHandControlBit3(SHEN_NAME('YKUR'), 1)) goto L02075F;
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, 0);
  end:;
}

static void script_207C6(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  local_EC = 0;
  local_F0 = task_384;
  script_1E8BE();
  Script_sicnUtils(0, 1, 1);
  for (;;) {
    if (script_208A1(local_F0, local_EC)) {
      Script_shenClearFreeState(0);
      Script_sicnUtils(2, 1, 0);
      script_20AED(local_F0, local_EC);
    } else {
      Script_shenClearFreeState(0);
      Script_sicnUtils(2, 1, 0);
      if (local_EC) {
        script_1E7EC(0);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
        Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetGameStateTo(AUTH_None);
        SCNF_WaitFrames_0000(30);
      }
      L02087C:;
      SCNF_WaitFrames_0000(1);
      if (!~task_38E) goto L02087C;
      break;
    }
    local_EC = local_EC + 1;
  }
  script_1EA65();
  return;
}

static int32_t script_208A1(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  int32_t local_E8;
  int32_t local_E4;
  local_F8 = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    (void)0;
  } else {
    script_1EDBB(arg0, arg1);
    Script_EVT_GetPosition(arg0, -1, &local_EC, 0);
    (&local_EC)->y = (&local_EC)->y + 0.6f;
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 0);
    if (arg1 == 0) {
      Script_scenCreateTask(script_1FF32, 1, 0);
      Script_seqEnterEvtCam(7000, arg0, arg0);
    } else {
      local_F8 = Script_scenCreateTask(script_206D8, 0);
      Script_scenCreateTask(script_1FF32, 1, 4);
      Script_seqEnterEvtCam(7010, arg0, arg0);
    }
    L020945:;
    SCNF_WaitFrames_0000(1);
    if (!~task_38E) goto L020945;
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
          Script_scenCreateTask(script_1FF32, 1, 1);
        }
        local_FC = 1;
      } else if (local_E4 == 1) {
        Script_scenCreateTask(script_1FF32, 1, 2);
        SCNF_SetCashValue(0, 1);
        local_FC = 0;
      } else {
        local_FC = 0;
      }
    } else {
      Script_scenCreateTask(script_1FF32, 1, 3);
      local_FC = 0;
    }
    if (local_F8) {
      Script_srchControl((void *)local_F8);
    }
    (void)local_FC;
  }
  end:;
}

static void script_20A5A(int32_t arg0) {
  int32_t local_FC;
  if (arg0 == 0) {
    local_FC = -0x40800000;
  } else {
    local_FC = 0x42FA0000;
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  }
  script_1F3B9(CHARA_AKIR, 1, 7, 1);
  script_1F3B9(CHARA_AKIR, 0, 8, 1);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, -1.0f, -1.0f, local_FC, 0.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.25f);
  script_2019A();
  Script_scenCreateTask(script_1EEF6, 0);
  return;
}

static void script_20AED(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_shenClearFreeState(0);
  script_1EB3F();
  local_FC = script_20D33();
  if (arg1 == 0) {
    Script_seqEnterEvtCam(7006, arg0, arg0);
    script_1E7EC(1);
  } else {
    Script_seqEnterEvtCam(7011, arg0, arg0);
  }
  script_20A5A(arg1);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_20171, 0);
  Script_scenCreateTask(script_206EB, 0);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_20048, 1, local_FC);
  Script_shenClearFreeState(1);
  script_1FBEB(local_FC);
  script_1EC03();
  script_20BA3();
  return;
}

static void script_20BA3(void) {
  Vector3 local_F4;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, -1.0f, -1.0f, 135.0f, 0.0f, 0);
  script_206C0();
  Script_EVT_GetPosition(task_384, -1, &local_F4, 0);
  (&local_F4)->y = (&local_F4)->y + 0.6f;
  Script_seqEnterEvtCam(7005, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  return;
}

static void script_20C1D(int32_t arg0) {
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
  local_C8 = script_1DD0F(-1, arg0);
  Script_ProcessSubtitle("JINGURU01", 0);
  Script_GetCassetteFlags(local_C8, 1, 0, 0);
  SCNF_StartIconSystemMenu(local_C8);
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  return;
}

static int32_t script_20D33(void) {
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
  local_F0 = Script_SetOrGetWOCCValue(task_384, -1);
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
      if (data_40D98[local_F0] != 0) {
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
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_40D88[local_F0]);
      break;
    case 1:
      local_F0 = 14;
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_40D88[local_F0]);
      break;
    case 2:
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_40D88[local_F0] - data_40D98[local_F0]);
      break;
    case 3:
      local_D8 = data_40D88[local_F0] - data_40D98[local_F0] - 1;
      local_D4 = 0;
      while (local_D4 < data_40D98[local_F0]) {
        if (Script_GetOwnedItemCount(script_1DD0F(local_F0, local_D8 + local_D4), 0) == 0) {
          if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.666666687f)) {
            break;
          }
        }
        local_D4 = local_D4 + 1;
      }
      if (local_D4 < data_40D98[local_F0]) {
        local_F4 = data_40D88[local_F0] - local_D4 - 1;
      } else {
        local_D8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_40D98[local_F0]) + 1;
        local_F4 = data_40D88[local_F0] - local_D8;
      }
      break;
    case 4:
      local_F0 = 15;
      local_D8 = 0;
      while (local_D8 < data_40D88[local_F0]) {
        if (Script_GetOwnedItemCount(script_1DD0F(local_F0, local_D8), 0) == 0) {
          if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.333333343f)) {
            break;
          }
        }
        local_D8 = local_D8 + 1;
      }
      if (local_D8 < data_40D88[local_F0]) {
        local_F4 = local_D8;
      } else {
        local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_40D88[local_F0]);
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
    (void)((task_38F = -1) & 2);
  } else {
    (void)((task_38F = 0) & 2);
  }
  script_1DF9C(SHEN_NAME('KEIH'), local_F0, local_F4, local_F8);
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Hide);
  return local_F4;
}

static int32_t script_21275(int32_t arg0, const char *arg1, const char *arg2, ...) {
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

static void script_212C7(int32_t arg0, Vector3 *arg1) {
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), arg0);
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, arg1);
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), -1);
  Script_EVT_GetPosition(SHEN_NAME('DAMY'), -1, arg1, 0);
  return;
}

static void script_212FF(int32_t arg0, Vector3i *arg1) {
  int32_t unused_F4[3];
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), arg0);
  Script_EVT_SetRotation(SHEN_NAME('DAMY'), 0x38000000, arg1, 0);
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), -1);
  Script_mtwkGetSetRotation(SHEN_NAME('DAMY'), arg1, 0);
  return;
}

static void script_21337(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 733) == 0)) {
    DSWK_AssignDoorEntryConfig(26, 1, -1, -1, -1, -1, -1, -1);
  }
  return;
}

static void script_21378(void) {
  if (-(task_F8 == 301) & -(task_C8.hours < 19) & -(task_C8.hours >= 17) & -(Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_FIELD_B8_BY_ID, 0x49524948) == 3)) {
    DSWK_AssignDoorEntryConfig(0, 1, -1, -1, -1, -1, -1, -1);
  } else {
    DSWK_AssignDoorEntryConfig(0, 2, 2, -1, -1, -1, -1, -1);
  }
  return;
}

static void script_213D8(void) {
  if (-(Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_FIELD_B8_BY_ID, 0x4F544148) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_HATO_STOPPER_CLEARED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE) == 0) & -(task_C8.hours >= 19)) {
    DSWK_AssignDoorEntryConfig(30, 1, -1, -1, -1, -1, -1, -1);
  } else if (-(Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_FIELD_B8_BY_ID, 0x4F544148) == 0) & -(script_77C3(12) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_CHALLENGE_COMPLETE)) {
    DSWK_AssignDoorEntryConfig(30, 1, -1, -1, -1, -1, -1, -1);
  } else {
    DSWK_AssignDoorEntryConfig(30, 3, 2, -1, 0x2302AB, -1, -1, -1);
  }
  return;
}

static void script_21483(void) {
  if (-(task_F8 >= 501) & -(task_F8 <= 502) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_INITIAL_VISIT_COMPLETE) == 0)) {
    DSWK_AssignDoorEntryConfig(46, 1, -1, -1, -1, -1, -1, -1);
  } else {
    DSWK_AssignDoorEntryConfig(46, 3, 4, -1, 0x2302AB, -1, -1, -1);
  }
  return;
}

static void script_214D4(void) {
  script_21337();
  script_21378();
  script_213D8();
  script_21483();
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

static int32_t script_218F9(int32_t arg0, int32_t arg1) {
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

static void script_21A42(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = arg1;
  switch (local_FC) {
    case 7:
      script_218F9(arg0, 0);
      break;
    case 6:
      script_218F9(arg0, 1);
      break;
    case 4:
      script_218F9(arg0, 0);
      break;
    case 3:
      script_218F9(arg0, 1);
      break;
    case 2:
      script_218F9(arg0, 0);
      break;
    case 1:
      script_218F9(arg0, 1);
      break;
  }
  return;
}

static int32_t script_21AD5(void) {
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
  if ((Script_ScenDataUtil(task_148, SCND_ReadU32, 755) & 2) == 0) {
    (void)((local_F1 = -(script_218F9(0, 1) != 0)) & 8);
  } else {
    (void)((local_F1 = -(script_218F9(0, 0) != 0)) & 8);
  }
  if ((Script_ScenDataUtil(task_148, SCND_ReadU32, 664) & 2) == 0) {
    (void)((local_F0 = -(script_218F9(1, 1) != 0)) & 8);
  } else {
    (void)((local_F0 = -(script_218F9(1, 0) != 0)) & 8);
  }
  if (script_2773C(7, 0, 19, 0)) {
    (void)((local_EE = 0) & 2);
  } else {
    (void)((local_EE = -1) & 2);
  }
  (void)((local_EF = 0) & 2);
  while (local_EE == 0) {
    if ((Script_ScenDataUtil(task_148, SCND_ReadU32, 755) & 2) == 0) {
      if (local_F1 == 0) {
        (void)((local_F1 = -(script_218F9(0, 1) != 0)) & 8);
        local_F3 = 8;
      }
      if (local_F3) {
        script_21A42(0, local_F3);
        local_F3 = local_F3 - 1;
      }
      Script_charaPosRotUtils(SHEN_NAME('TKOK'), 152, 0, &data_42514, CHARA_PART_Add);
    } else {
      if (local_F1 == -1) {
        (void)((local_F1 = -(script_218F9(0, 0) != 0)) & 8);
        local_F3 = 40;
      }
      if (local_F3) {
        (&local_F4)->y = (&data_42514)->y + (41 - local_F3) * 22;
        Script_charaPosRotUtils(SHEN_NAME('TKOM'), 152, 0, &local_F4, CHARA_PART_Add);
        local_F3 = local_F3 - 1;
      }
    }
    if (((Script_ScenDataUtil(task_148, SCND_ReadU32, 664) & 2) == 0) & (local_EF == 0)) {
      if (local_F0 == 0) {
        (void)((local_F0 = -(script_218F9(1, 1) != 0)) & 8);
        local_F2 = 8;
      }
      if (local_F2) {
        script_21A42(1, local_F2);
        local_F2 = local_F2 - 1;
      }
      Script_charaPosRotUtils(SHEN_NAME('TKOL'), 152, 0, &data_42514, CHARA_PART_Add);
    } else {
      if (local_F0 == -1) {
        (void)((local_F0 = -(script_218F9(1, 0) != 0)) & 8);
        local_F2 = 40;
      }
      if (local_F2) {
        (&local_F4)->y = (&data_42514)->y + (41 - local_F2) * 22;
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
      goto L021E0A;
    }
  }
  L021E0A:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKOK'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TKOL'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TKOL'), CDM_Show);
      goto L021E3E;
    }
  }
  L021E3E:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKOL'), 0);
  return Script_srchControl(0);
}

static void script_21E51(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01BNK_BANK_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_RETURN_PROMPT_SHOWN) == 0)) {
    Script_scenToggleCharaFlag(0, 200, 1);
    Script_charaDisplay(SHEN_NAME('HDCA'), CDM_Show);
    (void)((task_116 = -1) & 8);
  } else if (script_2773C(19, 0, 5, 0)) {
    Script_scenToggleCharaFlag(0, 200, 1);
    Script_charaDisplay(SHEN_NAME('HDCA'), CDM_Show);
    (void)((task_116 = -1) & 8);
  } else {
    Script_scenToggleCharaFlag(0, 200, 0);
    (void)((task_116 = 0) & 8);
  }
  if (task_116 == 0) {
    script_22668();
  } else {
    script_22674();
  }
  return;
}

static int32_t script_21EF4(float arg0, float arg1) {
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

static int32_t script_2202B(void) {
  int32_t local_FC;
  int32_t local_F8;
  task_390 = 0;
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_42754);
  Script_InvestigationUtils(ShenmueUtils_SetSpecialAssetList, data_426E0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  for (;;) {
    if (task_390 == 0) {
      local_FC = Script_srchUtils(-1);
    }
    local_F8 = local_FC;
    if ((local_F8 == 0x304C4554) | (local_F8 == 133)) {
      Script_srchUtils(SRCH_SCRIPT_Reset);
      script_11FA5();
    } else if (local_F8 == 0x314B4254) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_223A1();
      Script_ryoControl(RYO_RestoreControl);
    } else if ((local_F8 == 0x30484347) | (local_F8 == 0x31484347) | (local_F8 == 0x32484347) | (local_F8 == 0x34484347) | (local_F8 == 0x35484347) | (local_F8 == 0x33484347)) {
      task_384 = local_FC;
      script_207C6();
    } else {
      if (local_F8 == 134) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        SCNF_PrepareCassetteUI();
        Script_ryoControl(RYO_RestoreControl);
        goto L022138;
      }
    }
    L022138:;
    local_FC = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
    if ((local_FC >= 0) & (local_FC <= 6)) {
      script_221D5(local_FC);
    }
    if ((local_FC == 7) | (local_FC == 8)) {
      script_22519();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2217C(int32_t arg0) {
  SCNF_RequestIconNotification(0, 0, 0, 2);
  (void)((task_10E = 0) & 8);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &task_EC, 0);
  script_221D5(arg0);
  return;
}

static void script_221D5(int32_t arg0) {
  int32_t local_FC;
  if (task_390 == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    script_37135();
    task_390 = 1;
    local_FC = arg0;
    switch (local_FC) {
      case 0:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, data_42520);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_42DB0, 60);
        break;
      case 1:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_42558);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_42DBC, 60);
        break;
      case 2:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_42590);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_42DC8, 60);
        break;
      case 3:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_42600);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_42DD4, 60);
        break;
      case 4:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_42638);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_42DE0, 60);
        break;
      case 5:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_42670);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_42DEC, 60);
        break;
      case 6:
        Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_426A8);
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_42DF8, 60);
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
    task_390 = 0;
  }
  return;
}

static void script_2239E(void) {
  return;
}

static void script_223A1(void) {
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
      script_22426();
      Script_srchUtils(SRCH_SCRIPT_Reset);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  return;
}

static void script_22426(void) {
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

static void script_224C4(void) {
  SCNF_RequestIconNotification(0, 0, 0, 2);
  (void)((task_10E = 0) & 2);
  script_22519();
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &task_EC, 0);
  return;
}

static void script_22519(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &data_425C8);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_42E04, 60);
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

static int8_t script_225B9(void) {
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

static void script_22600(void) {
  SCNF_RequestIconNotification(0, 0, 0, 2);
  (void)((task_10E = 0) & 2);
  if (script_225B9()) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_EC, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_PrepareCassetteUI();
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}

static void script_22668(void) {
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
  return;
}

static void script_22674(void) {
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, -1);
  return;
}

static void script_22680(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = 0;
  script_387C();
  script_228ED();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_42E10, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_42E1C, CHARA_PART_Set);
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
        Script_scenCreateTask(script_22CF2, 0);
        break;
      case 100:
        Script_scenCreateTask(script_22A15, 1, 0);
        break;
      case 125:
        Script_executeAicaCommand(0x1E04A9, 0, 0);
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        local_F8 = Script_scenCreateTask(script_22CA2, 1, SHEN_NAME('BUS_'));
        break;
      case 260:
        Script_srchControl((void *)local_F8);
        goto L0227B1;
        break;
    }
    local_FC = local_FC + 1;
    script_22D0F();
    SCNF_WaitFrames_0000(1);
  }
  L0227B1:;
  return;
}

static void script_227B2(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_387C();
  script_228ED();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &data_42E34, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &data_42E40, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &data_42E4C, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &data_42E58, CHARA_PART_Set);
  local_FC = 0;
  if (arg1) {
    Script_executeAicaCommand(0x1B04A9, 0, 0);
  }
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 132:
        Script_scenCreateTask(script_22A15, 1, 2);
        break;
      case 165:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        local_F8 = Script_scenCreateTask(script_22CA2, 1, SHEN_NAME('BUS_'));
        break;
      case 216:
        goto L0228B6;
        break;
    }
    local_FC = local_FC + 1;
    script_22D0F();
    SCNF_WaitFrames_0000(1);
  }
  L0228B6:;
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenSeqUtils(-6);
  script_229A5();
  return;
}

static void script_228ED(void) {
  Vector3 local_F4;
  int32_t unused_E4[4];
  GameDateTime local_DC;
  (void)1;
  Script_SetVector4ByOffset(&data_42E70);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_42E70);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  script_3894(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_SetCharaFlags(SHEN_NAME('BUS_'), 2);
  Script_SetCharaFlags(SHEN_NAME('BUS_'), 100, 1, &data_42E64);
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

static void script_229A5(void) {
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

static int32_t script_22A15(int32_t arg0) {
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

static int32_t script_22CA2(int32_t arg0) {
  while (Script_shenSeqUtils(-1)) {
    Script_charaPosRotUtils(arg0, 163, 0, &data_42E80, CHARA_PART_Add);
    Script_charaPosRotUtils(arg0, 173, 0, &data_42E80, CHARA_PART_Add);
    Script_charaPosRotUtils(arg0, 182, 0, &data_42E8C, CHARA_PART_Add);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_22CF2(void) {
  SCNF_SetCashValue(-160, 1);
  SCNF_WaitFrames_0000(200);
  return Script_srchControl(0);
}

static void script_22D0F(void) {
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

__attribute__((scn_string_bindings(0, data_42E98, 2, data_42EAE, 4, data_42EC4, 5, data_42ECF)))
static void script_22DB3(void) {
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

__attribute__((scn_string_bindings(0, data_42EDA)))
static void script_22F16(void) {
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

__attribute__((scn_string_bindings(0, data_42F20)))
static void script_22FCD(void) {
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
    (void)((task_10E = -1) & 4);
    for (;;) {
      if (Script_IsPlayerAllowed(0) & -(Script_GetControllerInput(1) == 0x400)) {
        (void)((local_CF = 0) & 8);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        if (script_2773C(22, 25, 5, 0)) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          (void)((task_10E = 0) & 8);
          script_22F16();
          Script_ryoControl(RYO_RestoreControl);
          break;
        }
        if (Script_shenGetStats(STATS_CASH) < 320) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          (void)((task_10E = 0) & 8);
          script_22DB3();
          Script_ryoControl(RYO_RestoreControl);
          break;
        }
        Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
        SCNF_RequestIconNotification(0, 0, 0, 2);
        (void)((task_10E = 0) & 8);
        Script_CycleManagerUtils(CYCLE_CMD_SET_MOBJ_FIELD_108_BOOL, 0x5F535542, 1);
        if (!((&task_D4)->x * -1.7769f + 81.1959f - task_D4.z <= (float)0)) {
          Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &data_42EFC, 54631, MN_AKI_AKI_TATI_IWA_L_R, 0);
        } else if (local_D8 >= 0) {
          (void)((local_CF = -1) & 0x40);
        } else {
          Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &data_42F08, 60227, MN_AKI_AKI_TATI_IWA_L_R, 0);
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
            Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &data_42F14, 60227, MN_AKI_AKI_TATI_IWA_L_R, 0);
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
            (void)((task_110 = -1) & 2);
            script_232BE(16);
            script_3700F("BUSS");
            break;
          }
          local_D0 = local_D0 - 1;
          SCNF_WaitFrames_0000(1);
        }
        break;
      } else {
        if (((local_D8 = Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_SPECIAL_DWORD120, 0x5F535542)) == 0) | (Script_IsPlayerAllowed(0) == 0)) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          (void)((task_10E = 0) & 4);
          break;
          goto L0232A8;
        }
      }
      L0232A8:;
      SCNF_WaitFrames_0000(1);
    }
    (void)((task_110 = 0) & 4);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_42F25, 1, data_42F32, 2, data_42F42, 4, data_42F54, 5, data_42F59, 6, data_42F66, 7, data_42F6B, 8, data_42F78, 9, data_42F88, 11, data_42F9A, 12, data_42F9F)))
static void script_232BE(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_E4[4];
  int32_t local_E0;
  (void)((task_110 = -1) & 4);
  if (arg0 != 17) {
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_DC;
    local_DC = arg0;
    switch (local_DC) {
      case 16:
        script_36FAE();
        Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
        Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
        script_29E86();
        Script_LoadSND(0, 4, "a1_busno.snd", 0);
        script_36FEB("/scene/02/D000/", "BUSS");
        local_E0 = script_36F2B(0x5F535542);
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
        script_22680(arg0);
        break;
      case 17:
        Script_LoadSND(0, 4, "a1_busno.snd", 0);
        script_36FEB("/scene/02/D000/", "BUSS");
        local_E0 = script_36F2B(0x5F535542);
        SCNF_WaitFrames_0000(10);
        Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
        if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f) >= 1.0f)) {
          Script_AuthorUtils(0, 17, "seqdata2.bin", "BUSS");
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
          script_227B2(arg0, 1);
        } else {
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
          Script_AuthorUtils(0, 17, "seqdata5.bin", data_42FAC);
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
          script_227B2(arg0, 0);
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
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('MFSY'), 4);
        SCNF_WaitFrames_0000(1);
        break;
      case 17:
        Script_shenSeqUtils(-6);
        Script_AuthorUtils(AUTH_FREE, 17);
        script_229A5();
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
        script_36F97(0x5F535542, local_E0);
        if ((task_C8.hours >= 19) | (task_C8.hours <= 6)) {
          Script_SetCharaFlags(CHARA_AKIR, 0);
        }
        break;
    }
  }
  (void)((task_110 = 0) & 8);
  return;
}

static void script_2367A(int32_t arg0, int32_t arg1) {
  task_394 = 0;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_MoveObjectToObject(arg0, arg1);
  script_37033(arg0, -0.35f, 0.0f, -0.7f, 0, 0x4000, 0);
  Script_scenCreateTask(script_23890, 2, arg1, 0);
  Script_PlayCharaMotion(arg0, 28696, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_37398, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(arg0, 28695, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_23721(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_23901, 2, arg1, 56);
  Script_PlayCharaMotion(arg0, 28694, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_37398, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_MoveObjectToObject(arg0, -1);
  task_394 = 1;
  return;
}

static void script_23785(int32_t arg0, int32_t arg1) {
  task_394 = 0;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_MoveObjectToObject(arg0, arg1);
  script_37033(arg0, -0.345f, 0.0f, -0.65f, 0, 0x4000, 0);
  Script_scenCreateTask(script_23890, 2, arg1, 15);
  Script_PlayCharaMotion(arg0, 28696, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_37398, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(arg0, 28695, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_2382C(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_23901, 2, arg1, 26);
  Script_PlayCharaMotion(arg0, 28694, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_37398, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_MoveObjectToObject(arg0, -1);
  task_394 = 1;
  return;
}

static int32_t script_23890(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  int8_t local_F3;
  local_F3 = 0;
  SCNF_WaitFrames_0000(arg1);
  Script_executeAicaCommand(0x1802AB, 0, 0);
  for (;;) {
    Script_mtwkGetSetRotation(arg0, &local_F4, 0);
    (&local_F4)->y = (&local_F4)->y + 200;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    script_23BFC(arg0);
    local_F3 = local_F3 + 1;
    if (local_F3 == 40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_23901(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  int8_t local_F3;
  local_F3 = 0;
  SCNF_WaitFrames_0000(arg1);
  for (;;) {
    Script_mtwkGetSetRotation(arg0, &local_F4, 0);
    (&local_F4)->y = (&local_F4)->y - 200;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    script_23BFC(arg0);
    local_F3 = local_F3 + 1;
    if (local_F3 == 40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1902AB, 0, 0);
  end:;
}

static void script_23972(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  task_394 = 0;
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
  script_37033(arg0, 0.33f, 0.0f, -0.65f, 0, 0xC000, 0);
  Script_scenCreateTask(script_23B4E, 1, arg1);
  Script_scenCreateTask(script_23B4E, 1, SHEN_NAME('DDMY'));
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, 28696, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_37398, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, 28695, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_23AB3(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_23BA1, 1, arg1);
  Script_scenCreateTask(script_23BA1, 1, SHEN_NAME('DDMY'));
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, 28694, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_37398, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
  Script_MoveObjectToObject(arg0, -1);
  Script_SetMapControlTaskFlag(SHEN_NAME('DDMY'), 1);
  SCNF_WaitFrames_0000(1);
  task_394 = 1;
  return;
}

static int32_t script_23B4E(int32_t arg0) {
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

static int32_t script_23BA1(int32_t arg0) {
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

static void script_23BFC(int32_t arg0) {
  float local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  Script_mtwkGetSetRotation(arg0, &local_E4, 0);
  (&local_E4)->y = (&local_E4)->y - 1274;
  local_FC = 0.75f;
  task_398 = (&local_F0)->x - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, -(&local_E4)->y) * local_FC;
  *(&task_398 + 1) = 1.08f;
  *(&task_398 + 2) = (&local_F0)->z - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, -(&local_E4)->y) * local_FC;
  return;
}

static void script_23C7A(int32_t arg0) {
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
  (void)local_FC;
  return;
}

static void script_23CDF(int32_t arg0, int32_t arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg1, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_23CFF(float arg0, float arg1, float arg2, int32_t arg3) {
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

static void script_23D59(float arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
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

static void script_23DB2(int32_t arg0) {
  Script_PlayCharaMotion(arg0, 28693, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_37398, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(arg0, 28692, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchControl(0);
  return;
}

static void script_23E1D(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      script_23CFF(-105.1f, 0.0f, 94.15f, -121);
      break;
    case 1:
      script_23CFF(-124.83f, -1.75f, 63.95f, -102);
      break;
  }
  Script_SetGameStateTo(AUTH_None);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ryoControl(RYO_RestoreControl);
  Script_srchControl(0);
  return;
}

static void script_23E88(void) {
  SCNF_WaitFrames_0000(10);
  while (~Script_ryoControl(PCCTL_QueryShouldRun)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_23EB0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  Script_srchControl(0);
  return;
}

static void script_23F18(const char *arg0, int32_t arg1) {
  while (arg1 > 0) {
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID(arg0);
  Script_srchControl(0);
  return;
}

static int32_t script_23F44(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(2, data_430C4, 4, data_430DA)))
static void script_23F74(int32_t arg0) {
  int8_t local_FF;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_MACHINE_PURCHASED_ONCE) == 0) {
    uint8_t unused_FC[3];
    int32_t local_F8;
    script_24154(arg0);
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
      script_24154(arg0);
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
          goto L024106;
        }
      }
      L024106:;
      goto L024109;
    }
  }
  L024109:;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x80);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_24154(int32_t arg0) {
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
    script_23D59(-40.033798f, 0.08f, 74.684502f, 18, 1554);
    Script_scenCreateTask(script_23F44, 2, 0x4205A9, 56);
    Script_scenCreateTask(script_23F44, 2, 0x4205A9, 78);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.25f);
  } else {
    Script_MoveObjectToObject(CHARA_AKIR, -1);
    script_23D59(-40.9137f, 0.08f, 75.033096f, 18, -0x7FFFF9EE);
    Script_scenCreateTask(script_23F44, 2, 0x4205A9, 56);
    Script_scenCreateTask(script_23F44, 2, 0x4205A9, 78);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.25f);
  }
  SCNF_WaitFrames_0000(120);
  return;
}

__attribute__((scn_string_bindings(0, data_430F7, 1, data_430FE, 3, data_43114, 6, data_43135)))
static void script_24280(int32_t arg0) {
  float local_FC;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_LoadVoiceFile(1, "SA1081");
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  script_23CFF(6.4521f, 0.07f, 84.816498f, 320);
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

__attribute__((scn_string_bindings(0, data_4313C, 2, data_4314E, 3, data_43159, 4, data_43164, 6, data_4317A, 8, data_43190, 9, data_4319B, 10, data_431A6)))
static void script_24472(int32_t arg0) {
  float local_FC;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_LoadVoiceFile(1, "SA1088");
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  script_23CFF(-19.15f, 0.0724f, 72.97f, 10);
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

static void script_246B8(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (Script_IsPlayerAllowed(0) == 0) {
  } else {
    local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
    if (local_FC != -1) {
      local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_190[local_FC], 2);
      local_F8 = Script_ScenDataUtil(task_150, SCND_ReadU32, Script_ScenDataUtil(task_154, SCND_ReadU32, local_F8 * 9 + 1) * 4 + 1);
      local_F8 = script_10310(local_FC, local_F8, 0);
      if (local_F8) {
        if (Script_GetControllerInput(1) == 0x400) {
          local_F4 = task_190[local_FC];
          switch (local_F4) {
            case 51:
              if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[local_FC] * 13 + 1) & 2) {
                script_24280(task_230[local_FC]);
              }
              break;
            case 1:
              if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[local_FC] * 13 + 1) & 2) {
                script_23F74(task_230[local_FC]);
              }
              break;
            case 61:
              if (Script_ScenDataUtil(task_148, SCND_ReadU32, task_190[local_FC] * 13 + 1) & 2) {
                script_24472(task_230[local_FC]);
              }
              break;
          }
        }
      }
    }
  }
  return;
}

static int32_t script_24817(void) {
  Script_scenCreateTask(script_0030, 6, CHARA_AKIR, 2, 0, &data_431B0, 2, 0);
  script_2643((int32_t)data_43364);
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

static void script_2488F(void) {
  if (Script_GetCurrentEntryPoint() != 99) {
    Script_scenCreateTask(script_25AE5, 0);
  }
  Script_scenCreateTask(script_24817, 0);
  script_27B54();
  return;
}

static void script_248B7(void) {
  if (task_10A & 1) {
    Script_GetCurrentDrawMode(1, 0);
  }
  Script_sprintf("DOBUITA EVENT START START START!\012");
  return;
}

static void script_248D6(int32_t arg0) {
  if (task_10A & 1) {
    if (arg0 != 1) {
      script_26C84();
    }
    Script_SetCharaFlags(CHARA_AKIR, 0);
  } else {
    script_26C60();
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

static int8_t script_2496C(int8_t arg0) {
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
  if (*((int8_t *)&*(int32_t *)&task_106 + local_E6) == 1) {
    if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &task_D4) <= local_EC)) {
      *((int8_t *)&*(int32_t *)&task_106 + local_E6) = 0;
    }
  }
  if (script_2773C(8, 0, 22, 0)) {
    if (Script_IsPlayerAllowed(0)) {
      if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_F4, 10.0f) == 0) {
        if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &task_D4) >= local_F0)) {
          if (*((int8_t *)&*(int32_t *)&task_106 + local_E6) == 0) {
            *((int8_t *)&*(int32_t *)&task_106 + local_E6) = 1;
            local_E7 = local_E8;
            Script_sprintf("Daily practice %d: Check: in range, setting training flag to true\012", local_E6);
          } else {
            if (task_108) {
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

static int8_t script_24ADD(int32_t arg0) {
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
        if (task_10E == 0) {
          SCNF_RequestIconNotification(0x100, 0, 0, 1);
          (void)((task_10E = -1) & 4);
        }
      } else {
        if (task_10E == -1) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          (void)((task_10E = 0) & 4);
        }
      }
    }
  } else {
    if (task_10E == -1) {
      SCNF_RequestIconNotification(0, 0, 0, 2);
      (void)((task_10E = 0) & 4);
    }
  }
  return local_E7;
}

static int8_t script_24D46(int32_t arg0) {
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
  if (task_116 & -(Script_IsPlayerAllowed(0) != 0)) {
    if (Script_CheckTargetWithinAngleRange(&local_F4, 1.5f, local_E8[1], 12743, &task_D4)) {
      if (Script_CheckTargetWithinAngleRange(&task_D4, 1.4f, task_EC.y, 7281, &local_F4)) {
        local_E7 = arg0;
        if (task_10E == 0) {
          SCNF_RequestIconNotification(0x100, 0, 0, 1);
          (void)((task_10E = -1) & 1);
        }
      } else {
        if (task_10E == -1) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          (void)((task_10E = 0) & 1);
        }
      }
    }
  } else {
    if (task_10E == -1) {
      SCNF_RequestIconNotification(0, 0, 0, 2);
      (void)((task_10E = 0) & 1);
    }
  }
  return local_E7;
}

static int8_t script_24E35(int32_t arg0) {
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
      if (task_10E == 0) {
        Script_sicnUtils(0, 5, 1);
        (void)((task_10E = -1) & 0x80);
      }
      if (task_3A4 == 0) {
        Script_uiUtils(0, 25, 0);
        task_3A4 = 1;
      }
    } else {
      if (task_10E == -1) {
        Script_sicnUtils(1, 0, 0);
        (void)((task_10E = 0) & 0x80);
      }
      if (task_3A4 != 0) {
        Script_uiUtils(0, 0, 0);
        task_3A4 = 0;
      }
    }
  } else {
    if (task_10E == -1) {
      Script_sicnUtils(1, 0, 0);
      (void)((task_10E = 0) & 0x80);
    }
  }
  return local_F3;
}

static int8_t script_24FF1(void) {
  int8_t local_FF;
  local_FF = 0;
  if (script_1E68A()) {
    local_FF = 20;
  }
  return local_FF;
}

static int8_t script_2500B(void) {
  Vector3 local_F4;
  int8_t local_F3;
  local_F3 = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_JIMSE_EVENT_DONE) == 0) & script_2773C(8, 30, 18, 0) & Script_IsPlayerAllowed(0)) {
    local_F4.x = -71.79f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 79.86f;
    if (Script_CheckTargetWithinAngleRange(&local_F4, 1.5f, 0, 0x8000, &task_D4)) {
      local_F3 = 17;
      if (task_10E == 0) {
        SCNF_RequestIconNotification(0x100, 0, 0, 1);
        (void)((task_10E = -1) & 4);
      }
    } else {
      if (task_10E == -1) {
        SCNF_RequestIconNotification(0, 0, 0, 2);
        (void)((task_10E = 0) & 4);
      }
    }
  } else {
    local_F3 = -1;
  }
  return local_F3;
}

static int8_t script_250DA(void) {
  Vector3 local_F4;
  int8_t local_F3;
  local_F3 = 0;
  if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 199) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_HANA_STAGE_ACTIVE) == 0) & ~(-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DKTY_TUBA_CUTSCENE_COMPLETE) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_PENDING) == 0)) & -(task_C8.hours >= 19)) {
    local_F4.x = -115.48f;
    (&local_F4)->y = 0.1f;
    (&local_F4)->z = 98.703003f;
    if (Script_CheckTargetWithinAngleRange(&local_F4, 3.0f, 0, 0x8000, &task_D4)) {
      local_F3 = 18;
    }
  } else {
    local_F3 = -1;
  }
  return local_F3;
}

static void script_25175(int32_t arg0) {
  if (Script_IsPlayerAllowed(0)) {
    if (Script_GetControllerInput(1) == 0x400) {
      int32_t local_FC;
      local_FC = arg0;
      if ((local_FC == 3) | (local_FC == 4) | (local_FC == 5) | (local_FC == 6) | (local_FC == 7)) {
        if (script_3F53(100, 1)) {
          if (task_10E == -1) {
            Script_sicnUtils(1, 0, 0);
            (void)((task_10E = 0) & 1);
          }
          script_248B7();
          script_48F0(CHARA_AKIR, SHEN_NAME('VM_0'), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
          script_248D6(0);
        }
      } else if ((local_FC == 8) | (local_FC == 9) | (local_FC == 10) | (local_FC == 11) | (local_FC == 12) | (local_FC == 13) | (local_FC == 14)) {
        script_248B7();
        script_2217C(arg0 - 8);
        script_248D6(0);
      } else {
        switch (local_FC) {
          case 15:
            script_248B7();
            script_224C4();
            script_248D6(0);
            break;
          case 16:
            script_248B7();
            script_22600();
            script_248D6(1);
            break;
          case 17:
            script_248B7();
            Script_scenCreateTask(script_2646B, 0);
            SCNF_RequestIconNotification(0, 0, 0, 2);
            Script_srchControl((void *)task_100);
            Script_SetQteOverlayTimeoutTuning(0, QTE_TimeoutSession);
            Script_SetQteOverlayTimeoutTuning(0, QTE_DisableTimeoutBoost);
            script_33AB9();
            Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
            script_248D6(0);
            break;
        }
      }
    }
    {
      int32_t local_FC;
      local_FC = arg0;
      if (local_FC == 18) {
        float local_F8;
        float local_F4;
        float local_F0;
        local_F8 = -2.4f;
        local_F4 = -178.651001f;
        local_F0 = local_F8 * (&task_D4)->x + local_F4;
        if (-!(task_D4.z >= local_F0) & -(task_E0.y < 57435) & -(task_E0.y > 35589)) {
          script_365F6();
        }
        goto L025353;
      }
    }
  }
  L025353:;
  {
    int32_t local_FC;
    local_FC = arg0;
    if ((local_FC == 1) | (local_FC == 2)) {
      Script_scenSetAssignCVWK(CHARA_AKIR, 0, 3);
      if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1) == 0) {
        Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 1);
      }
      script_289DD(arg0 - 1);
      if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1)) {
        Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 0);
      }
      Script_scenSetAssignCVWK(CHARA_AKIR, 0, 0);
      Script_uiUtils(0, 0, 0);
    } else {
      switch (local_FC) {
        case 20:
          script_248B7();
          script_207C6();
          script_248D6(0);
          break;
        case 22:
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_JIMSE_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_POST_GMCT_STAGE_ACTIVE) == 0)) {
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_POST_GMCT_STAGE_ACTIVE, 1);
            script_277BB(2, SHEN_NAME('ARAR'), 1, 0);
          } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_HANA_STAGE_ACTIVE) == 0)) {
            if ((script_77C3(24) > 0) | (script_788D(30) > 240)) {
              int32_t unused_F8;
              GameDateTime local_F0;
              int8_t local_EF;
              int8_t local_EE;
              Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_HANA_STAGE_ACTIVE, 1);
              Script_ReturnTimeDate(&local_F0);
              if ((&local_F0)->month == 12) {
                local_EE = 0;
              } else {
                local_EE = (&local_F0)->month;
              }
              local_EF = (local_EE << 5) | (&local_F0)->day;
              Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 24, local_EF);
              script_277BB(2, SHEN_NAME('ARAR'), 0, 0);
            } else {
              script_277BB(2, SHEN_NAME('DAZA'), 0, 0);
            }
          } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_PRE_0172_FUKU_INE_STAGE_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_CASSETTE_REWARD_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_REWARD_ROUTE_COMPLETE) == 0)) {
            script_277BB(2, SHEN_NAME('ARAR'), 3, 0);
          } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_REWARD_ROUTE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED, 1);
            script_277BB(2, SHEN_NAME('ARAR'), 4, 0);
          } else {
            script_277BB(2, SHEN_NAME('DAZA'), 0, 0);
          }
          break;
        case 24:
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_CHALLENGE_COMPLETE) == 0)) {
            script_277BB(2, SHEN_NAME('DHQB'), 0, 0);
          } else {
            script_277BB(2, SHEN_NAME('DBHB'), 0, 0);
          }
          break;
        case 23:
          script_277BB(2, SHEN_NAME('DSKI'), 0, 0);
          break;
        case 25:
          script_277BB(2, SHEN_NAME('DTKY'), 0, 0);
          break;
        case 21:
          script_277BB(2, SHEN_NAME('DKTY'), 0, 0);
          break;
        case 26:
          script_277BB(2, SHEN_NAME('DCHA'), 0, 0);
          break;
        case 28:
          script_277BB(2, SHEN_NAME('DBYO'), 0, 0);
          break;
        case 29:
          script_277BB(2, SHEN_NAME('DJAZ'), 0, 0);
          break;
        case 30:
          script_277BB(2, SHEN_NAME('DSBA'), 0, 0);
          break;
        case 31:
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_E1049_ROUTE_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) == 0)) {
            script_277BB(2, CHARA_TOKI, 10, 0);
          } else if (-(task_F8 > 1600) & -(task_F8 < 1902) & -(Script_GetSceneID() == 2) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PHOENIX_LEGEND) == 0)) {
            script_277BB(2, CHARA_TOKI, 1, 0);
          } else {
            script_277BB(2, SHEN_NAME('DRSA'), 0, 0);
          }
          break;
        case 27:
          script_277BB(2, SHEN_NAME('TATQ'), 0, 0);
          break;
        case 32:
          script_277BB(2, SHEN_NAME('DMAJ'), 0, 0);
          break;
        case 33:
          script_277BB(2, SHEN_NAME('DPIZ'), 0, 0);
          break;
        case 34:
          script_277BB(2, SHEN_NAME('DRME'), 0, 0);
          break;
        case 35:
          script_277BB(2, SHEN_NAME('DSLI'), 0, 0);
          break;
        case 36:
          script_277BB(2, SHEN_NAME('DRHT'), 0, 0);
          break;
        case 37:
          script_277BB(2, SHEN_NAME('DSUS'), 0, 0);
          break;
        case 38:
          script_277BB(2, SHEN_NAME('DURN'), 0, 0);
          break;
        case 39:
          script_277BB(2, SHEN_NAME('DYKZ'), 0, 0);
          break;
        case 40:
          script_277BB(2, SHEN_NAME('DKPA'), 0, 0);
          break;
        case 41:
          script_277BB(2, SHEN_NAME('DSLT'), 0, 0);
          break;
      }
    }
  }
  return;
}

static int32_t script_25873(int32_t arg0, int32_t arg1) {
  if (arg0) {
    (void)arg0;
  } else {
    (void)arg1;
  }
  end:;
}

static int32_t script_25885(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  for (;;) {
    local_FC = script_25873(arg0, arg1);
    switch (local_FC) {
      case 1:
        arg0 = script_2496C(0);
        break;
      case 2:
        arg0 = script_2496C(1);
        break;
      case 3:
        arg0 = script_24E35(3);
        break;
      case 4:
        arg0 = script_24E35(4);
        break;
      case 5:
        arg0 = script_24E35(5);
        break;
      case 6:
        arg0 = script_24E35(6);
        break;
      case 7:
        arg0 = script_24E35(7);
        break;
      case 8:
        arg0 = script_24ADD(8);
        break;
      case 9:
        arg0 = script_24ADD(9);
        break;
      case 10:
        arg0 = script_24ADD(10);
        break;
      case 11:
        arg0 = script_24ADD(11);
        break;
      case 12:
        arg0 = script_24ADD(12);
        break;
      case 13:
        arg0 = script_24ADD(13);
        break;
      case 14:
        arg0 = script_24ADD(14);
        break;
      case 15:
        arg0 = script_24ADD(15);
        break;
      case 16:
        arg0 = script_24D46(16);
        break;
      case 17:
        arg0 = script_2500B();
        break;
      case 18:
        arg0 = script_250DA();
        break;
      case 20:
        arg0 = 0;
        break;
    }
    if (arg0 != -1) {
      break;
    } else {
      if (arg1 >= 19) {
        arg1 = 1;
      } else {
        arg1 = arg1 + 1;
      }
      arg0 = 0;
    }
  }
  if (arg0 == 0) {
    arg0 = script_24FF1();
  }
  return arg0;
}

static void script_25ABF(void) {
  int32_t local_FC;
  local_FC = Script_UpdateTaskCollisionCache(0, 1, &task_D4);
  if (local_FC != -1) {
    Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, local_FC);
  }
  return;
}

static int32_t script_25AE5(void) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  local_FE = 1;
  local_FF = 0;
  local_FF = script_25885(local_FF, local_FE);
  for (;;) {
    script_25ABF();
    local_FF = script_25885(local_FF, local_FE);
    if (local_FE >= 19) {
      local_FE = 1;
    } else {
      local_FE = local_FE + 1;
    }
    if ((&task_158)->mode == 1) {
      local_F8 = task_158.entry_idx;
      switch (local_F8) {
        case 35:
          local_FF = 21;
          break;
        case 25:
          local_FF = 22;
          break;
        case 30:
          local_FF = 24;
          break;
        case 58:
          local_FF = 25;
          break;
        case 28:
          local_FF = 26;
          break;
        case 60:
          local_FF = 23;
          break;
        case 63:
          local_FF = 28;
          break;
        case 0:
          local_FF = 29;
          break;
        case 55:
          local_FF = 30;
          break;
        case 1:
          local_FF = 31;
          break;
        case 46:
          local_FF = 27;
          break;
        case 39:
          local_FF = 32;
          break;
        case 49:
          local_FF = 33;
          break;
        case 50:
          local_FF = 34;
          break;
        case 54:
          local_FF = 35;
          break;
        case 51:
          local_FF = 36;
          break;
        case 56:
          local_FF = 37;
          break;
        case 59:
          local_FF = 38;
          break;
        case 61:
          local_FF = 39;
          break;
        case 34:
          local_FF = 40;
          break;
        case 53:
          local_FF = 41;
          break;
      }
    }
    if (task_10C & 4) {
      if (Script_IsPlayerAllowed(0)) {
        script_22FCD();
      }
    }
    if (local_FF) {
      script_25175(local_FF);
    }
    if (*&task_84 != -1) {
      script_25D20();
    }
    if (Script_IsPlayerAllowed(0)) {
      script_246B8();
    }
    script_26399();
    script_264B8();
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_25D20(void) {
  return;
}

__attribute__((scn_string_bindings(21, data_435B6, 22, data_435C1, 23, data_435CC)))
static void script_25D23(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 2) {
    local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
    if (local_FC == 0) {
      Script_ShowUIMessageFromID("SA1089A001");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_FC == 1) {
      Script_ShowUIMessageFromID("SA1089A002");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1089A003");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else if (local_FC == 3) {
    local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
    if (local_FC == 0) {
      Script_ShowUIMessageFromID("SA1095A001");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_FC == 1) {
      Script_ShowUIMessageFromID("SA1095A002");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1095A003");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else if (local_FC == 4) {
    local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
    if (local_FC == 0) {
      Script_ShowUIMessageFromID("SA1119A001");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_FC == 1) {
      Script_ShowUIMessageFromID("SA1119A002");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1119A003");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else if (local_FC == 5) {
    local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
    if (local_FC == 0) {
      Script_ShowUIMessageFromID("SA1120A001");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_FC == 1) {
      Script_ShowUIMessageFromID("SA1120A002");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1120A003");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else if (local_FC == 6) {
    local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
    if (local_FC == 0) {
      Script_ShowUIMessageFromID("SA1121A001");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_FC == 1) {
      Script_ShowUIMessageFromID("SA1121A002");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1121A003");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else if (local_FC == 7) {
    local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
    if (local_FC == 0) {
      Script_ShowUIMessageFromID("SA1122A004");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_FC == 1) {
      Script_ShowUIMessageFromID("SA1122A005");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1122A006");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else if (local_FC == 8) {
    local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
    if (local_FC == 0) {
      Script_ShowUIMessageFromID("SA1122A001");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_FC == 1) {
      Script_ShowUIMessageFromID("SA1122A002");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("SA1122A003");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
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

static void script_261A4(int32_t arg0) {
  Vector3 local_F4;
  if (!((&task_D4)->x * 0.42f - task_D4.z <= 7.13f)) {
    if (task_10D != -1) {
      (void)((task_10D = -1) & 0x20);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 80.0f);
    }
  } else {
    if (task_10D != 0) {
      (void)((task_10D = 0) & 0x20);
      script_26C60();
    }
  }
  if (!((&task_D4)->x * 0.45f - task_D4.z <= 10.07f)) {
    if (task_115 == 0) {
      (void)((task_115 = -1) & 0x40);
      Script_uiUtils(0, 63, 0);
    }
    if (arg0 != 0) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
      Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
      Script_shenClearFreeState(0);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      script_25D23(arg0);
      local_F4.x = 33.25f;
      (&local_F4)->z = 10.74f;
      SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_3AA7C, 0.0f, 0, 0x200000);
      Script_charaToggleMotEnabled(CHARA_AKIR, 0);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_IPPO, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      for (;;) {
        if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
          break;
        }
        Script_RotateNodeUtils(CHARA_AKIR, &data_37398, 0);
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
    if (task_115 == -1) {
      (void)((task_115 = 0) & 4);
      Script_uiUtils(0, 0, 0);
    }
  }
  return;
}

static void script_26399(void) {
  if (Script_IsPlayerAllowed()) {
    if ((task_F8 >= 1301) & (task_F8 <= 1504)) {
      script_261A4(2);
    } else if (task_F8 == 1706) {
      script_261A4(4);
    } else if (task_F8 == 1801) {
      script_261A4(5);
    } else if (task_F8 == 1901) {
      script_261A4(6);
    } else if (task_F8 == 1902) {
      script_261A4(7);
    } else if (task_F8 == 1903) {
      script_261A4(8);
    } else if ((task_F8 >= 1604) & (task_F8 <= 1903)) {
      script_261A4(3);
    } else {
      script_261A4(0);
    }
  }
  return;
}

static void script_2645C(void) {
  Script_LoadVoiceFile(1, "SA1076");
  return;
}

static int32_t script_2646B(void) {
  int32_t local_FC;
  SCNF_WaitFrames_0000(10);
  while (1) {
    local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
    if (task_190[local_FC] == 25) {
      Script_charaDisplay(task_230[local_FC], CDM_Hide);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_264B8(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  Vector3i local_D0;
  int8_t local_CF;
  int8_t local_CE;
  (void)((local_CE = -1) & 0x10);
  (&local_F4)->y = (&local_E8)->y = 0.0724f;
  local_DC.x = (&local_DC)->z = 0;
  local_D0.x = (&local_D0)->z = 0;
  local_CF = task_109 & 0xF;
  if (local_CE & 0x40) {
    if (!((&task_D4)->x <= -22.5f)) {
      if (!(task_D4.z >= 70.0f)) {
        (void)((local_CE = 0) & 1);
        if (local_CF != 0) {
          task_109 = 0;
          local_F4.x = 17.95f;
          (&local_F4)->z = 25.969f;
          (&local_DC)->y = 60711;
          local_E8.x = -70.8f;
          (&local_E8)->z = 56.6f;
          (&local_D0)->y = 10370;
          goto L02657B;
        }
      }
    }
  }
  L02657B:;
  if (local_CE & 0x40) {
    if (!((&task_D4)->x <= -90.1f)) {
      if (!(task_D4.z >= 70.1f)) {
        (void)((local_CE = 0) & 8);
        if (local_CF != 1) {
          task_109 = 1;
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
          task_109 = 2;
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
          task_109 = 3;
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
      task_109 = 4;
      local_F4.x = -97.890099f;
      (&local_F4)->z = 96.826103f;
      (&local_DC)->y = 42511;
      local_E8.x = -108.824799f;
      (&local_E8)->z = 74.606903f;
      (&local_D0)->y = 41414;
    }
  }
  if ((task_109 & 0x80) != 0) {
  } else if ((task_109 & 0x40) != 0) {
    Script_scenToggleVisibleFor(SHEN_NAME('VM_0'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('VM_1'), 0);
    task_109 = task_109 | 0x80;
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('VM_0'), 1);
    Script_scenToggleVisibleFor(SHEN_NAME('VM_1'), 1);
    Script_EVT_SetPosition(SHEN_NAME('VM_0'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('VM_0'), 0x38000000, &local_DC, 0);
    Script_EVT_SetPosition(SHEN_NAME('VM_1'), POS_XYZ, &local_E8);
    Script_EVT_SetRotation(SHEN_NAME('VM_1'), 0x38000000, &local_D0, 0);
    task_109 = task_109 | 0x40;
  }
  return;
}

static int32_t script_26803(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  (void)((local_FF = -1) & 2);
  (void)((local_FE = -1) & 2);
  (void)((local_FD = -1) & 2);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_435E0, 85);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet1FromTaskOffset, &data_44878, 14);
  for (;;) {
    if (task_110 == -1) {
      Script_WeatherUtils(WeatherScript_SetWccpClipSet0Enabled, 0);
      for (;;) {
        if (task_110 == 0) {
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Enabled, 1);
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    if ((task_105 == 2) | (task_105 == 3)) {
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
      if (task_116 & 0x10) {
        if (local_FE == 0) {
          (void)((local_FE = -1) & 2);
          Script_WeatherUtils(WeatherScript_SetWccpClipSet1Entry, 5, 1);
        }
      } else {
        if (local_FE == -1) {
          (void)((local_FE = 0) & 2);
          Script_WeatherUtils(WeatherScript_SetWccpClipSet1Entry, 5, 0);
          goto L02690C;
        }
      }
      L02690C:;
      if (Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_PRIMARY_DOOR_OPEN_PERCENT, 0x5F535542) | Script_CycleManagerUtils(CYCLE_CMD_GET_MOBJ_FIELD_148_OR_13C_PERCENT, 0x5F535542)) {
        if (local_FD == 0) {
          (void)((local_FD = -1) & 2);
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 5, 1);
        }
      } else {
        if (local_FD == -1) {
          (void)((local_FD = 0) & 2);
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 5, 0);
          goto L02696D;
        }
      }
      L02696D:;
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

__attribute__((scn_string_bindings(0, data_44B88, 2, data_44B9D, 4, data_44BB2, 6, data_44BC7)))
static void script_269BD(void) {
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    script_36FEB("/scene/02/D000/", "MAP1");
  } else {
    script_36FEB("/scene/02/D000/", "MAP0");
  }
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 5)) {
    script_36FEB("/scene/02/D000/", "MAP3");
  } else {
    if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 4)) {
      script_36FEB("/scene/02/D000/", "MAP2");
      goto L026A30;
    }
  }
  L026A30:;
  return;
}

static void script_26A31(void) {
  if (script_2773C(8, 30, 18, 30)) {
    Script_mapmSetRenderFlag(20, 0);
    Script_mapmSetRenderFlag(17, 1);
  } else {
    Script_mapmSetRenderFlag(17, 0);
    Script_mapmSetRenderFlag(20, 1);
  }
  if (script_2773C(19, 0, 23, 0)) {
    Script_mapmSetRenderFlag(18, 0);
    Script_mapmSetRenderFlag(21, 1);
  } else {
    Script_mapmSetRenderFlag(21, 0);
    Script_mapmSetRenderFlag(18, 1);
  }
  if (script_2773C(19, 0, 23, 30)) {
    Script_mapmSetRenderFlag(19, 0);
    Script_mapmSetRenderFlag(22, 1);
  } else {
    Script_mapmSetRenderFlag(22, 0);
    Script_mapmSetRenderFlag(19, 1);
  }
  return;
}

static void script_26ADF(void) {
  if (script_2773C(8, 30, 18, 30)) {
    Script_mapmSetRenderFlag(8, 1);
    Script_mapmSetRenderFlag(11, 0);
  } else {
    Script_mapmSetRenderFlag(8, 0);
    Script_mapmSetRenderFlag(11, 1);
  }
  if (script_2773C(10, 0, 21, 0)) {
    Script_mapmSetRenderFlag(10, 1);
    Script_mapmSetRenderFlag(12, 0);
  } else {
    Script_mapmSetRenderFlag(10, 0);
    Script_mapmSetRenderFlag(12, 1);
  }
  if (script_2773C(8, 30, 17, 0)) {
    Script_mapmSetRenderFlag(13, 0);
  } else {
    Script_mapmSetRenderFlag(13, 1);
  }
  if (script_2773C(8, 30, 19, 0)) {
    Script_mapmSetRenderFlag(14, 0);
  } else {
    Script_mapmSetRenderFlag(14, 1);
  }
  return;
}

static void script_26BA2(void) {
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(6, 1);
  return;
}

static void script_26BC9(void) {
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(6, 0);
  return;
}

static void script_26BF0(void) {
  script_26A31();
  script_26ADF();
  return;
}

static void script_26BFD(void) {
  script_26BF0();
  return;
}

static void script_26C05(void) {
  int8_t local_FF;
  if (task_C8.minutes < 30) {
    local_FF = 0;
  } else {
    local_FF = 1;
  }
  if (task_104 != task_C8.hours + task_C8.hours + local_FF) {
    script_26BF0();
    script_67A2();
  }
  task_104 = task_C8.hours + task_C8.hours + local_FF;
  return;
}

static void script_26C60(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 80.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 32.0f);
  return;
}

static void script_26C84(void) {
  script_26C60();
  Script_SetupLightFromTable(0);
  Script_SetCharaFlags(CHARA_AKIR, 0);
  Script_PREP_Related(-1, 1);
  Script_GetCurrentDrawMode(1, 1);
  return;
}

__attribute__((scn_string_bindings(1, data_44BE7, 2, data_44BF7, 4, data_44C09, 5, data_44C0E, 6, data_44C1B, 7, data_44C20, 8, data_44C2D, 9, data_44C32)))
static void script_26CB1(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_67DA(1);
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
  if (script_21EF4((&local_F4)->x, (&local_F4)->z)) {
    local_F4.x = -18.1f;
    (&local_F4)->z = 34.9f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  script_37135();
  Script_LoadSND(0, 2, "bgm109.snd", 0);
  script_36FEB("/scene/02/D000/", "YORU");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 5, "seqdata3.bin", "YORU");
  Script_AuthorUtils(0, 6, "seqdata4.bin", "YORU");
  Script_AuthorUtils(0, 7, "seqdata5.bin", "YORU");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(1);
  SCNF_WaitFrames_0000(1);
  script_21E51();
  script_6CAC();
  script_26BC9();
  script_6899(5);
  if (Script_shenSeqUtils(-7) == 0) {
    script_6899(6);
    if (Script_shenSeqUtils(-7) == 0) {
      script_6899(7);
    } else {
      Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
    }
  } else {
    Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  Script_AuthorUtils(AUTH_FREE, 5);
  Script_AuthorUtils(AUTH_FREE, 6);
  Script_AuthorUtils(AUTH_FREE, 7);
  script_3700F("YORU");
  script_29D3D();
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_370F3();
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  script_26C84();
  script_28DDE();
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
  script_67DA(0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_26F95(void) {
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
    script_370F3();
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

static int32_t script_270FB(void) {
  int8_t local_FF;
  if ((task_C8.hours >= 19) | (task_C8.hours <= 6)) {
    (void)((task_10A = -1) & 8);
    script_26C84();
    script_26BC9();
  } else {
    (void)((task_10A = 0) & 8);
    Script_shenSetLightingMode(1);
    script_26C60();
    if (0) {
      Script_charaDisplay(SHEN_NAME('WAGK'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('WAGK'), CDM_Show);
        goto L02716A;
      }
    }
    L02716A:;
    Script_scenToggleVisibleFor(SHEN_NAME('WAGK'), 1);
    script_26BA2();
  }
  local_FF = 0;
  script_2770E();
  script_26BFD();
  for (;;) {
    Script_ReturnTimeDate(&task_C8);
    script_37082();
    Script_EVT_GetPosition(CHARA_AKIR, -1, &task_D4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &task_E0, 0x40000000);
    Script_mtwkGetSetRotation(CHARA_AKIR, &task_EC, 0x42000000);
    task_105 = Script_WeatherUtils_0(WeatherDataUtil_GetWeatherClassForDateTime, (&task_C8)->year, task_C8.month, task_C8.day, task_C8.hours, task_C8.minutes);
    script_26C05();
    if (task_10C & 4) {
      script_26F95();
    }
    local_FF = script_27317(local_FF);
    if (-(task_10A == 0) & -(task_111 == 0) & -(task_C8.hours >= 19) & Script_IsPlayerAllowed(0)) {
      script_26CB1();
      script_28DE6();
      if (Script_scenToggleVisibleFor(SHEN_NAME('WAGK'), -1)) {
        if (-1) {
          Script_charaDisplay(SHEN_NAME('WAGK'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('WAGK'), CDM_Show);
            goto L027281;
          }
        }
        L027281:;
        Script_scenToggleVisibleFor(SHEN_NAME('WAGK'), 0);
      }
      (void)((task_10A = -1) & 0x80);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_272A4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (script_2773C(arg0, arg1, arg2, arg3)) {
    if (Script_ScenDataUtil(task_148, SCND_ReadU32, arg4 * 13 + 1) & 2) {
      script_FD40(arg4);
      script_FFB9(arg4);
    }
  } else {
    if ((Script_ScenDataUtil(task_148, SCND_ReadU32, arg4 * 13 + 1) & 2) == 0) {
      script_FD09(arg4);
      script_10002(arg4);
    }
  }
  return;
}

static int32_t script_27317(int32_t arg0) {
  int32_t local_FC;
  arg0 = arg0 + 1;
  local_FC = arg0;
  if (local_FC == 0) {
    if (script_2773C(7, 0, 18, 30)) {
      script_FFB9(43);
      script_FFB9(57);
      script_FFB9(48);
      script_FFB9(2);
    } else {
      script_10002(43);
      script_10002(57);
      script_10002(48);
      script_10002(2);
    }
  } else if (local_FC == 1) {
    if (script_2773C(10, 0, 21, 0)) {
      script_FFB9(31);
      script_FFB9(27);
      script_FFB9(38);
      script_FFB9(26);
    } else {
      script_10002(31);
      script_10002(27);
      script_10002(38);
      script_10002(26);
    }
  } else if (local_FC == 2) {
    if (task_F8 >= 500) {
      script_272A4(14, 0, 0, 0, 46);
    }
  } else if (local_FC == 3) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 957) == 0) {
      script_272A4(9, 0, 20, 0, 60);
    }
  } else if (local_FC == 4) {
    script_272A4(10, 0, 22, 0, 35);
  } else if (local_FC == 5) {
    if (task_F8 >= 203) {
      script_272A4(10, 0, 19, 0, 51);
    }
  } else if (local_FC == 6) {
    script_272A4(10, 0, 18, 0, 58);
  } else if (local_FC == 7) {
    script_272A4(11, 0, 21, 0, 55);
  } else if (local_FC == 8) {
    if (task_F8 >= 401) {
      script_272A4(13, 0, 0, 0, 61);
    }
  } else if (local_FC == 9) {
    script_272A4(10, 0, 21, 0, 28);
  } else if (local_FC == 10) {
    if (task_F8 >= 804) {
      script_272A4(10, 0, 21, 0, 1);
    }
  } else if (local_FC == 11) {
    if (task_F8 < 1902) {
      script_272A4(10, 0, 20, 0, 25);
    }
  } else if (local_FC == 12) {
    script_272A4(11, 0, 21, 0, 56);
  } else if (local_FC == 13) {
    script_272A4(10, 0, 0, 0, 64);
  } else if (local_FC == 14) {
    script_272A4(10, 0, 0, 0, 59);
  } else if (local_FC == 15) {
    script_272A4(17, 0, 3, 0, 63);
  } else if (local_FC == 16) {
    script_272A4(19, 0, 5, 0, 30);
  } else if (local_FC == 17) {
    script_272A4(19, 0, 4, 0, 0);
  } else if (local_FC == 18) {
    script_272A4(19, 0, 4, 0, 34);
  } else if (local_FC == 19) {
    script_272A4(17, 0, 4, 0, 49);
  } else if (local_FC == 20) {
    script_272A4(17, 0, 4, 0, 50);
  } else if (local_FC == 21) {
    script_272A4(18, 30, 4, 0, 54);
  } else if (local_FC == 22) {
    script_272A4(10, 0, 24, 0, 53);
  } else if (local_FC == 23) {
    script_272A4(12, 0, 4, 30, 39);
  } else if (local_FC == 24) {
    script_21337();
  } else if (local_FC == 25) {
    script_21378();
  } else if (local_FC == 26) {
    script_213D8();
  } else if (local_FC == 27) {
    script_21483();
  } else {
    arg0 = 0;
  }
  return arg0;
}

static void script_2770E(void) {
  int8_t local_FF;
  local_FF = 0;
  local_FF = script_27317(local_FF);
  while (local_FF) {
    local_FF = script_27317(local_FF);
  }
  return;
}

static int8_t script_2773C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_277BB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static int32_t script_27938(void) {
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_214D4();
  script_10002(46);
  script_10002(43);
  script_10002(57);
  script_10002(48);
  script_10002(58);
  script_10002(40);
  script_10002(41);
  script_10002(35);
  script_10002(60);
  script_10002(32);
  script_10002(33);
  script_10002(2);
  script_10002(51);
  script_10002(31);
  script_10002(27);
  script_10002(55);
  script_10002(38);
  script_10002(26);
  script_10002(1);
  script_10002(28);
  script_10002(25);
  script_10002(39);
  script_10002(59);
  script_10002(61);
  script_10002(64);
  script_10002(56);
  script_10002(63);
  script_10002(30);
  script_10002(0);
  script_10002(34);
  script_10002(50);
  script_10002(49);
  script_10002(54);
  script_10002(53);
  {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, 365);
    local_FC = local_FC & -65;
    local_FC = local_FC & -129;
    local_FC = local_FC | 0x80;
    Script_ScenDataUtil(task_148, SCND_WriteU32, 365, local_FC);
  }
  {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_148, SCND_ReadU32, 716);
    local_FC = local_FC & -65;
    local_FC = local_FC & -129;
    local_FC = local_FC | 0x80;
    Script_ScenDataUtil(task_148, SCND_WriteU32, 716, local_FC);
  }
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 957) == 0) & (Script_shenGetStats(STATS_CASH) >= 158000)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 957, 1);
  }
  if (Script_GetCurrentEntryPoint() != 99) {
    script_FC43();
    script_FD40(29);
    script_2770E();
  }
  return task_100 = Script_scenCreateTask(script_21AD5, 0);
}

static void script_27B54(void) {
  Script_scenCreateTask(script_27938, 0);
  Script_scenCreateTask(script_26803, 1, &data_435E0);
  Script_scenCreateTask(script_270FB, 0);
  return;
}

static int32_t script_27B7D(void) {
  int32_t local_FC;
  int32_t local_F8;
  script_7957();
  for (;;) {
    local_FC = script_79A9();
    local_F8 = local_FC;
    if (local_F8 == 130) {
      local_F8 = script_798E();
      script_7981();
    } else if (local_F8 == 131) {
      script_7981();
    } else {
      if (local_FC != -1) {
        script_7981();
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_27BDC(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_44CC8, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_44C90, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_44C58, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_44C74, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_44CAC, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_27D14(int32_t arg0) {
  script_27D2C(arg0);
  return;
}

static void script_27D20(int32_t arg0) {
  script_27D2C(arg0);
  return;
}

static void script_27D2C(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L027D65;
    }
  }
  L027D65:;
  Script_ryoSetChara(arg0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_27BDC();
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

static void script_27E38(int32_t arg0) {
  script_27D20(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_27E4F(int32_t arg0) {
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

static void script_27E90(int32_t arg0) {
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

static void script_27ED4(int32_t arg0) {
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

static void script_27F52(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_27ED4(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_27ED4(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_27ED4(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_27ED4(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_27ED4(5);
  } else {
    script_27ED4(0);
  }
  return;
}

static int32_t script_27FD1(int32_t arg0) {
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

__attribute__((scn_string_bindings(0, data_44D00, 3, data_44D1A, 7, data_44D3E)))
static int32_t script_28057(void) {
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
      goto L028138;
    }
  }
  L028138:;
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

static int32_t script_2817A(void) {
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

static void script_28201(void) {
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

static int32_t script_2825A(void) {
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

static int32_t script_28294(int32_t arg0) {
  Script_EVT_SetPosition(arg0, 0x78000000, &data_44D44);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_ccowUtils(arg0, CCOW_DisableAll);
}

static void script_282B7(void) {
  SCNF_WaitFrames_0000(2);
  while (Script_IsInEventCam() != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_282D7(int32_t arg0) {
  Script_EVT_SetPosition(arg0, 0x78000000, &data_44D50);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_ccowUtils(arg0, CCOW_DisableAll);
}

static int32_t script_282FA(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 5, "e1shugyo.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static void script_28345(void) {
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

static void script_2838C(void) {
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

static void script_283DF(void) {
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

static void script_2843B(void) {
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

static void script_28482(void) {
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

__attribute__((scn_string_bindings(0, data_44DE1)))
static void script_284DE(int8_t arg0) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  Script_LoadVoiceFile(1, "E1040");
  if (arg0) {
    Script_scenCreateTask(script_28294, 1, SHEN_NAME('LUV2'));
  } else {
    Script_scenCreateTask(script_282D7, 1, SHEN_NAME('LUV2'));
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  script_37135();
  Script_scenCreateTask(script_282FA, 0);
  Script_scenCreateTask(script_2825A, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MIMAWASU_K, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_44DE7)))
static void script_285B4(void) {
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

static void script_2862B(void) {
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

static void script_28668(void) {
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

static int32_t script_286A5(int32_t arg0) {
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

static void script_28716(int32_t arg0) {
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
    Script_vfStageAssignRingBoundary(&data_44DF0, 4.8f);
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
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_44DFC);
    (&local_F4)->y = 0xA000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  } else {
    Vector3i local_F4;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_44E08);
    (&local_F4)->y = 0x3500;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(2903, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_282B7();
  SCN_Enqueue_VFGI(2, 99, 1.0f, 4, -1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_scenCreateTask(script_27FD1, 1, arg0);
  return;
}

static int32_t script_28979(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_GetFightingStats(-1, 2) == 1) {
    local_FC = 1;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    local_FC = 2;
  }
  if (script_2817A()) {
    local_FC = 2;
  }
  return local_FC;
}

static void script_289C1(void) {
  script_2843B();
  return;
}

static void script_289C9(void) {
  SCN_Enqueue_VFGI(2, 99, 1.0f, 99, -1);
  return;
}

static void script_289DD(int8_t arg0) {
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
    task_108 = 1;
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
  if (script_28057()) {
    local_FE = 1;
  }
  if (local_FE == 0) {
    script_248B7();
    script_284DE(arg0);
    local_FF = 1;
  }
  for (;;) {
    int32_t local_F4;
    local_F8 = 0;
    script_2862B();
    script_28668();
    if (local_FE == 1) {
      break;
      local_FF = 14;
    }
    SCNF_WaitFrames_0000(1);
    local_F4 = local_FF;
    if (local_F4 == 1) {
      script_28345();
      local_FF = 2;
    } else if (local_F4 == 2) {
      script_2838C();
      local_FF = 4;
    } else if ((local_F4 == 3) | (local_F4 == 13) | (local_F4 == 14)) {
      break;
    } else if ((local_F4 == 4) | (local_F4 == 12)) {
      local_FF = local_FF + script_286A5(local_FF);
    } else if (local_F4 == 5) {
      local_FE = 2;
      script_283DF();
      script_28716(arg0);
      local_FF = 6;
    } else if (local_F4 == 6) {
      local_FF = local_FF + script_28979(arg0);
    } else if ((local_F4 == 7) | (local_F4 == 8)) {
      local_FF = local_FF - 7 + 9;
    } else {
      switch (local_F4) {
        case 9:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_289C1();
            local_FF = 12;
          }
          break;
        case 10:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_289C1();
            local_FF = 14;
          }
          break;
        case 11:
          script_289C9();
          local_FF = 6;
          break;
      }
    }
  }
  if (local_FE != 1) {
    Script_sprintf("training_npc_too_close reset before training starts \012");
    task_108 = 0;
    script_285B4();
  }
  if (local_FE == 2) {
    int32_t local_F4;
    script_28201();
    local_F4 = arg0;
    if (local_F4 == 0) {
      Script_PlayerStatsUtils(PS_SetShugyoCourseClearCountSlot, 2, Script_PlayerStatsUtils(PS_GetShugyoCourseClearCountSlot, 2) + 1);
    } else {
      Script_PlayerStatsUtils(PS_SetShugyoCourseClearCountSlot, 3, Script_PlayerStatsUtils(PS_GetShugyoCourseClearCountSlot, 3) + 1);
    }
  }
  if (local_FE != 1) {
    script_248D6(0);
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

static void script_28D11(void) {
  if (-(task_F8 >= 1700) & -(task_F8 <= 3000) & -(task_C8.hours >= 19)) {
    Script_CycleManagerUtils(CYCLE_CMD_MOBJ_PUSH_TEMP_STATE_41, 0x414D4A54);
    Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, 0x414D4A54, 0);
    Script_CycleManagerUtils(CYCLE_CMD_MOBJ_SET_TEMP_STATE_41_POS, 0x414D4A54, 19.682301f, 0.1f, 31.281099f, 0x1600);
    Script_CycleManagerUtils(CYCLE_CMD_MOBJ_SET_TEMP_STATE_41_WORD5, 0x414D4A54, 28697);
  }
  return;
}

static void script_28D79(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_AKIR_INE_DUO_SCENE_COMPLETE) == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4953494F, 1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_AKIR_INE_DUO_SCENE_COMPLETE) == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4F47414D, 1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_ROUTE_ENABLED) == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x41534948, 1);
  }
  script_28D11();
  return;
}

static void script_28DDE(void) {
  script_28D11();
  return;
}

static void script_28DE6(void) {
  return;
}

static void script_28DE9(void) {
  Script_scenSetMapEvTbl((uint32_t *)&data_44E8C, 6);
  return;
}

static void script_28DF7(void) {
  return;
}

static void script_28DFA(void) {
  return;
}

static void script_28DFD(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    if (Script_IsPlayerAllowed(0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_277BB(2, SHEN_NAME('JD00'), 0, 1);
    }
  }
  return;
}

static void script_28E2E(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    if (Script_IsPlayerAllowed(0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_277BB(2, SHEN_NAME('JD00'), 1, 1);
    }
  }
  return;
}

static void script_28E5F(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    if (Script_IsPlayerAllowed(0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_RETURN_PROMPT_SHOWN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0)) {
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED, 1);
        script_277BB(2, SHEN_NAME('GMCT'), 1, 2);
      } else {
        script_277BB(2, SHEN_NAME('DGCT'), 0, 2);
      }
    }
  }
  return;
}

static void script_28ECC(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    if (Script_IsPlayerAllowed(0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_277BB(2, SHEN_NAME('DCBN'), 0, 2);
    }
  }
  return;
}

static void script_28EFD(void) {
  int32_t local_FC;
  script_292D5();
  local_FC = Script_scenCreateTask(script_28FA3, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 140.0f);
  script_29324();
  Script_srchControl((void *)local_FC);
  return;
}

static void script_28F50(void) {
  int32_t local_FC;
  script_292D5();
  local_FC = Script_scenCreateTask(script_2919E, 0);
  Script_shenSeqUtils(1);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 140.0f);
  script_29324();
  Script_srchControl((void *)local_FC);
  return;
}

static int32_t script_28FA3(void) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  local_D8 = 0;
  local_E8.x = -2730;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_mapmSetRenderFlag(20, 0);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('CAR_'), 8, 0, &local_E8, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('CAR_'), 13, 0, &local_E8, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('CAR_'), 18, 0, &local_E8, CHARA_PART_Add);
    if (local_D8 == 1) {
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_3762C, 10);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_3762C, 10);
    }
    if (local_D8 == 2) {
      Script_FACE(CHARA_AKIR, 5, 0, 10);
      Script_FACE(CHARA_JIMY, 4, 0, 10);
    }
    if (local_D8 == 208) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
      Script_mapmSetCamFlags(1, 2, 0);
    }
    if (local_D8 == 273) {
      Script_mapmSetCamFlags(1, 2, 1);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 80.0f);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 80.0f);
    }
    if (local_D8 == 307) {
      Script_handSetFloatsFor(CHARA_JIMY, 0, data_3762C, 10);
      Script_handSetFloatsFor(CHARA_JIMY, 1, data_3762C, 10);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_37AA0, 10);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_37AA0, 10);
      local_DC.x = 0;
      (&local_DC)->y = -5095;
      (&local_DC)->z = -1636;
      Script_SetHandRootRotation(CHARA_JIMY, 1, 0x15, &local_DC);
    }
    if (local_D8 == 355) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_3762C, 10);
    }
    if (local_D8 == 600) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    }
    local_D8 = local_D8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2919E(void) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = 0;
  local_E8.x = -2730;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_charaPosRotUtils(SHEN_NAME('CAR_'), 8, 0, &local_E8, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('CAR_'), 13, 0, &local_E8, CHARA_PART_Set);
  Script_FACE(CHARA_AKIR, 10, 0, 1);
  for (;;) {
    if ((local_E4 > 49) & (local_E4 < 93)) {
      (&local_E8)->y = -91;
    }
    if ((local_E4 > 93) & (local_E4 < 137)) {
      (&local_E8)->y = 91;
    }
    Script_charaPosRotUtils(SHEN_NAME('CAR_'), 8, 0, &local_E8, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('CAR_'), 13, 0, &local_E8, CHARA_PART_Add);
    (&local_E8)->y = 0;
    Script_charaPosRotUtils(SHEN_NAME('CAR_'), 18, 0, &local_E8, CHARA_PART_Add);
    if (local_E4 == 125) {
      Script_mapmSetCamFlags(1, 2, 0);
    }
    if (local_E4 == 155) {
      Script_mapmSetCamFlags(1, 2, 1);
    }
    if (local_E4 == 190) {
      Script_FACE(CHARA_AKIR, 5, 0, 10);
    }
    if (local_E4 == 220) {
      Script_FACE(CHARA_AKIR, 2, 0, 70);
    }
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_292D5(void) {
  script_2934A(CHARA_AKIR);
  script_2934A(CHARA_JIMY);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_charaDisplay(SHEN_NAME('CAR_'), CDM_Show);
  Script_SetShadowRange(10000.0f);
  Script_SetFogEnabled(0);
  Script_SetVector4ByOffset(&data_44EA4);
  (void)1;
  return;
}

static void script_29324(void) {
  script_29410(CHARA_AKIR);
  script_29410(CHARA_JIMY);
  Script_charaDisplay(SHEN_NAME('CAR_'), CDM_Hide);
  return;
}

static void script_2934A(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_37AA0, 10);
  Script_handSetFloatsFor(arg0, 1, data_37AA0, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_44EB4);
  return;
}

static void script_293F0(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_29410(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static void script_29456(void) {
  int32_t local_FC;
  script_296C8();
  local_FC = Script_scenCreateTask(script_29493, 0);
  Script_shenSeqUtils(2);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_29708();
  Script_srchControl((void *)local_FC);
  return;
}

static int32_t script_29493(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (local_FC == 0) {
      Script_FACE(CHARA_AKIR, 0, 0, 10);
    }
    if (local_FC == 201) {
      Script_FACE(CHARA_AKIR, 2, 0, 19);
    }
    if (local_FC == 461) {
      Script_FACE(CHARA_AKIR, 7, 0, 9);
    }
    if (local_FC == 491) {
      Script_FACE(CHARA_AKIR, 5, 0, 9);
    }
    if (local_FC == 0) {
      Script_FACE(CHARA_JIMY, 2, 0, 0);
    }
    if (local_FC == 321) {
      Script_FACE(CHARA_JIMY, 7, 0, 9);
    }
    if (local_FC == 366) {
      Script_FACE(CHARA_JIMY, 11, 0, 9);
    }
    if (local_FC == 441) {
      Script_FACE(CHARA_JIMY, 5, 0, 9);
    }
    if (local_FC == 0) {
      Vector3i local_F0;
      local_F0.x = 1820;
      (&local_F0)->y = 0;
      (&local_F0)->z = 2912;
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_F0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_378D8, 10);
    }
    if (local_FC == 140) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_3762C, 10);
    }
    if (local_FC == 198) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_37AA0, 10);
    }
    if (local_FC == 235) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_3762C, 10);
    }
    if (local_FC == 296) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_37AA0, 10);
    }
    if (local_FC == 355) {
      {
        Vector3i local_F0;
        local_F0.x = -182;
        (&local_F0)->y = -2002;
        (&local_F0)->z = -3094;
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_F0);
      }
      {
        Vector3i local_F0;
        local_F0.x = 4187;
        (&local_F0)->y = -4187;
        (&local_F0)->z = 546;
        Script_SetHandRootRotation(CHARA_JIMY, 1, 0x15, &local_F0);
      }
    }
    if (local_FC == 400) {
      Vector3i local_F0;
      local_F0.x = -182;
      (&local_F0)->y = 364;
      (&local_F0)->z = 3458;
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_F0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_296C8(void) {
  script_29723(CHARA_AKIR);
  script_29723(CHARA_JIMY);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_JIMY, -1);
  Script_SetVector4ByOffset(&data_44EC4);
  (void)1;
  return;
}

static void script_29708(void) {
  script_29828(CHARA_AKIR);
  script_29828(CHARA_JIMY);
  return;
}

static void script_29723(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_37AA0, 10);
  Script_handSetFloatsFor(arg0, 1, data_37AA0, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaFlags(arg0, 4);
  Script_scenToggleActiveFor(arg0, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_44ED4);
  return;
}

static void script_297DC(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_29805(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  return;
}

static void script_29828(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_44EE4, 2, data_44F03, 3, data_44F13, 4, data_44F19, 5, data_44F27, 6, data_44F33)))
static void script_2986E(void) {
  int8_t local_FF;
  float local_F0[3];
  Vector3i local_E4;
  Script_timeSetUpdateEnabled(0);
  (void)((task_10C = 0) & 4);
  (void)((task_10B = 0) & 4);
  (void)((task_10E = 0) & 4);
  (void)((task_10F = 0) & 4);
  *(int8_t *)&task_106 = 0;
  *((int8_t *)&*(int32_t *)&task_106 + 1) = 0;
  task_108 = 0;
  (void)((task_10D = 0) & 1);
  (void)((task_110 = 0) & 1);
  (void)((task_111 = -1) & 1);
  (void)((task_113 = 0) & 1);
  task_109 = 5;
  (void)((task_116 = 0) & 1);
  if (Script_ryoSleepUtils(0)) {
    (void)((task_112 = -1) & 1);
  } else {
    (void)((task_112 = 0) & 1);
  }
  (void)((task_114 = 0) & 1);
  (void)((task_115 = 0) & 1);
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
  task_104 = task_C8.hours + task_C8.hours + local_FF;
  script_37082();
  if ((Script_GetCurrentEntryPoint() == 9) & (task_112 == 0)) {
    Script_scenSetAssignCVWK(CHARA_AKIR, 0, 1);
  }
  Script_SetMapFlags(0, 1);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 99);
  script_27D20(CHARA_AKIR);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  script_269BD();
  script_36FEB("/scene/02/D000/", (const char *)&data_44EF4);
  script_1E1D3();
  task_FC = Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MOBJ.BIN");
  script_36FEB("/scene/02/D000/", "SHUKI");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  script_F943(CHARA_AKIR, (DSWKDoorEntry *)data_3C7B0, (int32_t *)&data_3D518, (DoorResourceDescriptor *)&data_3D940, (MapObjectRecord *)data_3DB80, 10, 0);
  script_28DE9();
  script_2488F();
  script_2645C();
  script_29CFA();
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_PreserveAccumulator_01CC(17);
  (void)1;
  Script_motnUtils(MOTN_SetBudgetCapacity, 5);
  Script_scenCreateTask(script_27B7D, 0);
  Script_scenCreateTask(script_2202B, 0);
  script_1189A(0x304C4554, 9998, 0x421A21CB, 0x3F7851EC, 0x41185604, 0x43803FB8);
  script_21E51();
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/02/D000", (int32_t)"SCRTEST.BIN", 0x30303044);
  (void)((task_10C = -1) & 1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  if ((Script_GetCurrentEntryPoint() == 0) & (task_112 == 0)) {
    script_680E();
    script_232BE(17);
    script_3700F("BUSS");
    script_29D3D();
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 100);
  }
  script_28D79();
  if (task_112 == -1) {
    Script_ryoSleepUtils(1);
    if (Script_InitINSHForScene((Vector3 *)0x30303044, (Vector3i *)local_F0, (int32_t)&local_E4) == 1) {
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, (const Vector3 *)local_F0);
      Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E4, 0);
    } else {
      Script_EVT_GetPosition(CHARA_AKIR, -1, (Vector3 *)local_F0, 0);
      if (script_21EF4(*local_F0, local_F0[2])) {
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
  if ((Script_GetCurrentEntryPoint() != 0) | (task_112 == -1)) {
    script_680E();
    Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  }
  if (task_116 == 0) {
    script_22668();
  }
  if (script_37344() & -(task_112 == 0)) {
    Script_timeSetUpdateEnabled(1);
    script_371D2();
    script_37283(30);
    Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
    Script_ryoSleepUtils(2, 1);
    script_37222();
    (void)((task_111 = 0) & 1);
  } else {
    (void)((task_111 = 0) & 1);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
    Script_ryoSleepUtils(2, 1);
    Script_shenClearFreeState(1);
    Script_timeSetUpdateEnabled(1);
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_44F38, 1, data_44F3F, 2, data_44F46)))
static void script_29CFA(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_JIMSE_EVENT_DONE) == 0)) {
    Script_LoadVoiceFile(1, "01JIMB");
    Script_LoadVoiceFile(1, "E1004a");
    Script_LoadVoiceFile(1, "01JIM");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_44F4C, 2, data_44F63, 3, data_44F6A, 4, data_44F6F, 5, data_44F7C, 6, data_44F81, 7, data_44F86, 8, data_44F8B, 10, data_44FA2, 11, data_44FA9, 12, data_44FAE, 13, data_44FBB, 14, data_44FC0, 15, data_44FC5, 16, data_44FCA, 18, data_44FE1, 19, data_44FE8, 20, data_44FED, 21, data_44FFA, 22, data_44FFF, 23, data_45004, 24, data_45009, 26, data_45020, 27, data_45027, 28, data_4502C, 29, data_45039, 30, data_4503E, 31, data_45043, 32, data_45048, 34, data_4505F, 35, data_45066, 36, data_4506B, 37, data_45078, 38, data_4507D, 39, data_45082)))
static void script_29D3D(void) {
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

__attribute__((scn_string_bindings(0, data_45087)))
static void script_29E86(void) {
  script_3700F("SHUKI");
  return;
}

__attribute__((scn_string_bindings(0, data_4508D, 1, data_4509D)))
static void script_29E95(void) {
  script_36FEB("/scene/02/D000/", "SHUKI");
  return;
}

void script_29EA9(void) {
  script_2986E();
  return;
}

static void script_29EB1(int32_t arg0) {
  if (arg0 < 0) {
    (void)-arg0;
    return;
  } else {
    (void)arg0;
    return;
  }
}

static void script_29ECF(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static void script_29EEE(int32_t arg0) {
  if (arg0 != 0) {
    Script_sprintf("WAZA Warning : handle is not NULL, and create same handle !\012");
  }
  return;
}

static void script_29F03(int32_t arg0, int32_t arg1) {
  for (;;) {
    if (*(&task_3D4 + 3) & 2) {
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

static int32_t script_29FB9(int32_t arg0) {
  task_8F8 = arg0;
  *(&task_8F8 + 1) = 0;
  return (int32_t)&task_8F8;
}

static void script_29FD1(int32_t arg0) {
  Script_sprintf("%s\012", script_29FB9(arg0));
  return;
}

static void script_29FE7(int32_t arg0) {
  Script_sprintf("Konnna Error ha Detekonai hazu : %d\012", arg0);
  return;
}

static void script_29FF5(void) {
  Vector3 local_F4;
  SCNF_WaitFrames_0000(60);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 0.1f;
  for (;;) {
    if (*(&task_3C4 + 1) & 0x4000) {
      local_F4.x = (&local_F4)->x - 0.1f;
    }
    if (*(&task_3C4 + 1) & 0x8000) {
      local_F4.x = (&local_F4)->x + 0.1f;
    }
    if (*(&task_3C4 + 1) & 0x1000) {
      (&local_F4)->z = (&local_F4)->z - 0.1f;
    }
    if (*(&task_3C4 + 1) & 0x2000) {
      (&local_F4)->z = (&local_F4)->z + 0.1f;
    }
    Script_TranslateRotateScale_Wrapper(1, 0x3F800000, &local_F4, &data_451AC, &data_451A0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A0BF(void) {
  return;
}

static void script_2A0C2(void) {
  task_3F4 = 0;
  while (1) {
    task_3F4 = task_3F4 + 1;
    task_3C4 = Script_GetInputData(0, 0);
    *(&task_3C4 + 1) = Script_GetInputData(1, 0);
    *(&task_3C4 + 2) = Script_GetInputData(2, 0);
    *(&task_3C4 + 3) = Script_GetInputData(3, 0);
    task_3D4 = Script_GetInputData(0, 1);
    *(&task_3D4 + 1) = Script_GetInputData(1, 1);
    *(&task_3D4 + 2) = Script_GetInputData(2, 1);
    *(&task_3D4 + 3) = Script_GetInputData(3, 1);
    task_3E4 = Script_GetInputData(0, 2);
    *(&task_3E4 + 1) = Script_GetInputData(1, 2);
    *(&task_3E4 + 2) = Script_GetInputData(2, 2);
    *(&task_3E4 + 3) = Script_GetInputData(3, 2);
    if (*(&task_3D4 + 1) & 2) {
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

static void script_2A210(int32_t arg0) {
  task_3F8 = arg0;
  Script_seqEnterEvtCam(arg0, 0, AUTH_None);
  return;
}

static int32_t script_2A224(int32_t arg0, int32_t arg1, int32_t arg2) {
  SCNF_WaitFrames_0000(arg2);
  script_2A210(arg0);
  SCNF_WaitFrames_0000(1);
  Script_AllocSpline(arg1);
  end:;
}

static void script_2A246(int32_t arg0, int32_t arg1, int32_t arg2) {
  arg2 = arg2 / 2;
  if (arg2) {
    arg2 = arg2 - 1;
    Script_scenCreateTask(script_2A224, 3, arg0, arg1, arg2);
  } else {
    script_2A210(arg0);
    SCNF_WaitFrames_0000(1);
    Script_AllocSpline(arg1);
  }
  return;
}

static void script_2A28C(int32_t arg0, int32_t arg1, int32_t arg2) {
  task_3F8 = arg0;
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return;
}

static void script_2A2A0(int32_t arg0) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
  }
  return;
}

static void script_2A2BA(void) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsInEventCam() == 0) {
      break;
    }
  }
  return;
}

static void script_2A2D5(int32_t arg0) {
  script_2A210(arg0);
  SCNF_WaitFrames_0000(2);
  for (;;) {
    script_2A2BA();
    script_2A210(arg0);
    SCNF_WaitFrames_0000(2);
  }
  return;
}

static void script_2A302(int32_t arg0, int32_t arg1) {
  while (arg0) {
    SCNF_WaitFrames_0000(1);
    arg0 = arg0 - 1;
  }
  Script_executeAicaCommand(arg1, 0, 0);
  return;
}

static void script_2A329(void) {
  task_900 = 0;
  return;
}

static void script_2A333(void) {
  for (;;) {
    if (task_900 == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A34E(void) {
  task_900 = 1;
  return;
}

static void script_2A358(int32_t arg0) {
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

static int32_t script_2A3BC(float arg0, float arg1, float arg2, float arg3) {
  int32_t unused_FC;
  int32_t local_F4[2];
  arg0 = arg2 - arg0;
  arg1 = arg3 - arg1;
  return local_F4[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, arg0, arg1);
}

static int32_t script_2A3E3(int32_t arg0, int32_t arg1) {
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

static int32_t script_2A45B(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    task_904[arg1] = Script_wmptUtils(arg0, arg3, arg2);
    if (task_904[arg1] == 0) {
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

static void script_2A507(int32_t arg0, int32_t arg1, int32_t arg2) {
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
    task_904[arg1] = 0;
  } else {
    local_DC[0] = *(float *)arg2 - (&local_F4)->x;
    local_DC[2] = *(float *)(arg2 + 8) - (&local_F4)->z;
    local_C0[1] = script_2A3BC((&local_F4)->x, (&local_F4)->z, *(float *)arg2, *(float *)(arg2 + 8));
    local_D8 = script_2A3E3(local_C0[1], (&local_E8)->y);
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
    task_904[arg1] = 1000;
    local_B4 = Script_scenCreateTask(script_2A45B, 4, arg0, arg1, local_BC, arg2);
  }
  return;
}

static void script_2A6CD(int32_t arg0) {
  for (;;) {
    if (task_904[arg0] == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A6EF(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_2A7CA(void) {
  for (;;) {
    task_408 = task_408 + 1;
    task_40C = task_40C + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A7EE(int32_t arg0) {
  arg0 = 0;
  *(int8_t *)task_42C[arg0] = *(int8_t *)&task_420;
  *(int8_t *)&task_42C[arg0][1] = *((int8_t *)&task_420 + 1);
  *(int8_t *)&task_42C[arg0][2] = *((int8_t *)&task_420 + 2);
  *(int8_t *)&task_42C[arg0][3] = *((int8_t *)&task_420 + 3);
  *(int8_t *)&task_42C[arg0][4] = *((int8_t *)&task_420 + 4);
  *(int8_t *)&task_42C[arg0][5] = *((int8_t *)&task_420 + 5);
  *(int8_t *)&task_42C[arg0][6] = *((int8_t *)&task_420 + 6);
  *(int8_t *)&task_42C[arg0][7] = *((int8_t *)&task_420 + 7);
  *(int32_t *)&task_42C[arg0][8] = *(int32_t *)(&task_420 + 8);
  return;
}

static void script_2A8AE(void) {
  (void)0;
  return;
}

static void script_2A8B4(void) {
  *(int8_t *)&task_42C[0][2];
  return;
}

static void script_2A8BF(int32_t arg0) {
  int32_t unused_F8[2];
  for (;;) {
    if (arg0 == 9) {
      if ((task_404 != task_400) & (task_400 % 3 == 0)) {
        task_41C = 1;
      }
      if ((task_408 != 0) & (task_408 % 450 == 0)) {
        task_41C = 1;
        task_41D = 1;
      }
    } else {
      if (((task_40C != 0) & (task_40C % 300 == 0)) | ((task_404 != task_400) & (task_400 % 3 == 0))) {
        task_41C = 1;
      }
      if ((task_408 != 0) & (task_408 % 2700 == 0)) {
        task_41C = 1;
        task_41F = 1;
      } else {
        if ((task_408 != 0) & (task_408 % 1800 == 0)) {
          task_41C = 1;
          task_41E = 1;
          goto L02A986;
        }
      }
    }
    L02A986:;
    task_404 = task_400;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2A99A(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_8F3 = 1;
  local_F0 = 0;
  for (;;) {
    if (task_8F3 == 0) {
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

static void script_2A9FF(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_8F4 = 1;
  local_F0 = 0;
  for (;;) {
    if (task_8F4 == 0) {
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

static void script_2AA64(void) {
  int32_t local_FC;
  task_8F2 = 0;
  local_FC = 0;
  for (;;) {
    *((int8_t *)task_8DC + local_FC) = 0;
    *((int8_t *)task_8E8 + local_FC) = 0;
    local_FC = local_FC + 1;
    if (local_FC >= 10) {
      break;
    }
  }
  return;
}

static void script_2AAA0(void) {
  for (;;) {
    if (task_8F2 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2AABB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_2AB2F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_F4[3];
  script_2A34E();
  task_8F2 = 1;
  script_2AABB(arg0, arg1, arg2, arg3);
  task_8F2 = 0;
  return;
}

static void script_2AB54(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  script_2A34E();
  task_8F2 = 1;
  script_2AABB(arg0, arg1, arg2, arg3);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
  task_8F2 = 0;
  return;
}

static void script_2AB9D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  script_2A34E();
  task_8F2 = 1;
  script_2AABB(arg0, arg1, arg2, arg3);
  local_F0 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  if (local_F0 == 0) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
    goto L02ABFB;
  }
  L02ABFB:;
  task_8F2 = 0;
  return;
}

static void script_2AC03(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  script_2A34E();
  task_8F2 = 1;
  SCNF_WaitFrames_0000(arg3);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
  Script_PlayCharaMotion(arg0, 20998, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(30);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  script_2AABB(arg0, arg1, arg2, 0);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
  script_2A2A0(arg0);
  Script_PlayCharaMotion(arg0, 21000, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_8F2 = 0;
  return;
}

static void script_2ACCB(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  task_8F2 = 1;
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  SCNF_WaitFrames_0000(arg2);
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  task_8F2 = 0;
  script_2A34E();
  return;
}

static void script_2AD21(void) {
  task_92C = 1;
  task_930 = 0;
  for (;;) {
    if (task_92C) {
      task_92C = task_92C - 1;
      if (task_92C == 0) {
        task_930 = 0;
        Script_LCDM_Utils(0, (const void *)task_930);
        Script_sicnUtils(2, 0x800, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2AD76(int32_t arg0) {
  task_930 = arg0;
  if (task_930 == 0) {
    task_92C = 0;
    Script_LCDM_Utils(1, 0);
    Script_sicnUtils(2, 0x800, 0);
  } else {
    task_92C = 600;
    Script_LCDM_Utils(1, (const void *)1);
    Script_sicnUtils(0, 0x800, 0);
  }
  Script_LCDM_Utils(0, (const void *)task_930);
  return;
}

static void script_2ADD0(const char *arg0, const char *arg1) {
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

static void script_2AE20(int32_t arg0) {
  Script_FACE(arg0, 7, 5, 30);
  SCNF_WaitFrames_0000(30);
  Script_FACE(arg0, 0, -1, 12);
  return;
}

static void script_2AE45(int32_t arg0) {
  Script_FACE(arg0, 2, 0, 60);
  SCNF_WaitFrames_0000(60);
  Script_FACE(arg0, 0, -1, 12);
  return;
}

static int32_t script_2AE6A(int32_t arg0, int32_t arg1) {
  Script_FACE(arg0, 2, 2, 60);
  SCNF_WaitFrames_0000(arg1);
  Script_FACE(arg0, 0, -1, 12);
  end:;
}

static void script_2AE8F(void) {
  Script_ShowUIMessageFromID("E1004AA001");
  return;
}

static void script_2AE9B(void) {
  return;
}

static void script_2AE9E(void) {
  Script_ShowUIMessageFromID("E1004AB001");
  return;
}

static void script_2AEAA(void) {
  Script_ShowUIMessageFromID("E1004AA002");
  return;
}

static void script_2AEB6(void) {
  Script_ShowUIMessageFromID("E1004AB002");
  return;
}

static void script_2AEC2(void) {
  Script_ShowUIMessageFromID("E1004AA003");
  return;
}

static int32_t script_2AECE(void) {
  return Script_ShowUIMessageFromID("E1004AA004");
}

static int32_t script_2AEDA(void) {
  return Script_ShowUIMessageFromID("E1004AC001");
}

static void script_2AEE6(void) {
  Script_ShowUIMessageFromID("E1004AA005");
  return;
}

static int32_t script_2AEF2(void) {
  return Script_ShowUIMessageFromID("E1004AA006");
}

static int32_t script_2AEFE(void) {
  return Script_ShowUIMessageFromID("E1004AB003");
}

static void script_2AF0A(void) {
  Script_ShowUIMessageFromID("E1004A007");
  return;
}

__attribute__((scn_string_bindings(0, data_45330)))
static int32_t script_2AF16(void) {
  return Script_ShowUIMessageFromID("E1004AA007");
}

static int32_t script_2AF22(void) {
  return Script_ShowUIMessageFromID("E1004AA009");
}

static void script_2AF2E(void) {
  SCNT_ShowSubtitle("「よっと！」");
  return;
}

static void script_2AF36(void) {
  SCNT_ShowSubtitle("「おわっと！」");
  return;
}

static int32_t script_2AF3E(void) {
  return Script_ShowUIMessageFromID("E1004AD001");
}

static int32_t script_2AF4A(void) {
  return Script_ShowUIMessageFromID("E1004AB004");
}

static int32_t script_2AF56(void) {
  return Script_ShowUIMessageFromID("E1004AA010");
}

static int32_t script_2AF62(void) {
  return Script_ShowUIMessageFromID("E1004AA011");
}

static void script_2AF6E(void) {
  SCNT_ShowSubtitle("「クソ、ジミーの野郎」");
  return;
}

static void script_2AF76(void) {
  Script_ShowUIMessageFromID("E1004AA012");
  return;
}

static int32_t script_2AF82(void) {
  float local_FC;
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
  if (!(local_FC >= 0.33f)) {
    Script_ShowUIMessageFromID("E1004AA013");
  } else if (!(local_FC >= 0.66f)) {
    Script_ShowUIMessageFromID("E1004AA014");
  } else {
    Script_ShowUIMessageFromID("E1004AA015");
  }
  end:;
}

static int32_t script_2AFCE(void) {
  return Script_ShowUIMessageFromID("E1004AA016");
}

static int32_t script_2AFDA(void) {
  return Script_ShowUIMessageFromID("E1004AE002");
}

static int32_t script_2AFE6(void) {
  return Script_ShowUIMessageFromID("E1004AE001");
}

static int32_t script_2AFF2(void) {
  return Script_ShowUIMessageFromID("E1004AB005");
}

static void script_2AFFE(void) {
  SCNT_ShowSubtitle("「フン！」");
  return;
}

static int32_t script_2B006(void) {
  return Script_ShowUIMessageFromID("E1004AF001");
}

__attribute__((scn_string_bindings(0, data_45413)))
static int32_t script_2B012(void) {
  return Script_ShowUIMessageFromID("E1004AA017");
}

static int32_t script_2B01E(void) {
  return Script_ShowUIMessageFromID("E1004AG001");
}

static int32_t script_2B02A(void) {
  return Script_ShowUIMessageFromID("E1004AA034");
}

static int32_t script_2B036(void) {
  return Script_ShowUIMessageFromID("E1004AG002");
}

static void script_2B042(void) {
  Script_ShowUIMessageFromID("E1004AH001");
  return;
}

static void script_2B04E(void) {
  Script_ShowUIMessageFromID("E1004AH002");
  return;
}

static void script_2B05A(void) {
  Script_ShowUIMessageFromID("E1004AH003");
  return;
}

static void script_2B066(void) {
  Script_ShowUIMessageFromID("E1004AA029");
  return;
}

static void script_2B072(void) {
  Script_ShowUIMessageFromID("E1004AB006");
  return;
}

static void script_2B07E(void) {
  Script_ShowUIMessageFromID("E1004AA031");
  return;
}

static int32_t script_2B08A(void) {
  return Script_ShowUIMessageFromID("E1004AN002");
}

static int32_t script_2B096(void) {
  return Script_ShowUIMessageFromID("E1004AN001");
}

__attribute__((scn_string_bindings(0, data_45497)))
static void script_2B0A2(void) {
  Script_ShowUIMessageFromID("E1004AO002");
  return;
}

static void script_2B0AE(void) {
  Script_ShowUIMessageFromID("E1004AO004");
  return;
}

static int32_t script_2B0BA(void) {
  return Script_ShowUIMessageFromID("E1004AA027");
}

static int32_t script_2B0C6(void) {
  return Script_ShowUIMessageFromID("E1004AL002");
}

__attribute__((scn_string_bindings(0, data_454C3)))
static int32_t script_2B0D2(void) {
  return Script_ShowUIMessageFromID("E1004AA037");
}

static int32_t script_2B0DE(void) {
  return Script_ShowUIMessageFromID("E1004AA018");
}

static int32_t script_2B0EA(void) {
  return Script_ShowUIMessageFromID("E1004AL001");
}

static int32_t script_2B0F6(void) {
  return Script_ShowUIMessageFromID("E1004AA028");
}

static int32_t script_2B102(void) {
  return Script_ShowUIMessageFromID("E1004AM001");
}

static int32_t script_2B10E(void) {
  return Script_ShowUIMessageFromID("E1004AI002");
}

static int32_t script_2B11A(void) {
  return Script_ShowUIMessageFromID("E1004AI001");
}

static void script_2B126(void) {
  Script_ShowUIMessageFromID("E1004AI003");
  return;
}

static void script_2B132(void) {
  Script_ShowUIMessageFromID("E1004AA022");
  return;
}

static int32_t script_2B13E(void) {
  return Script_ShowUIMessageFromID("E1004AA023");
}

static int32_t script_2B14A(void) {
  return Script_ShowUIMessageFromID("E1004AL003");
}

static int32_t script_2B156(void) {
  return Script_ShowUIMessageFromID("E1004AJ001");
}

static int32_t script_2B162(void) {
  return Script_ShowUIMessageFromID("E1004AJ002");
}

static void script_2B16E(void) {
  Script_ShowUIMessageFromID("E1004AA040");
  return;
}

static void script_2B17A(void) {
  Script_ShowUIMessageFromID("E1004AB009");
  return;
}

static void script_2B186(int32_t arg0) {
  Script_ScenDataUtil(&task_990, SCND_WriteU8, arg0 / 8, Script_ScenDataUtil(&task_990, SCND_ReadU8, arg0 / 8) | (1 << (arg0 % 8)));
  return;
}

static void script_2B1B1(int32_t arg0) {
  Script_ScenDataUtil(&task_990, SCND_WriteU8, arg0 / 8, Script_ScenDataUtil(&task_990, SCND_ReadU8, arg0 / 8) & ((1 << (arg0 % 8)) ^ -1));
  return;
}

static int32_t script_2B1DF(int32_t arg0) {
  return Script_ScenDataUtil(&task_990, SCND_ReadU8, arg0 / 8) & (1 << (arg0 % 8));
}

static void script_2B1FC(float arg0, float arg1, float arg2, float arg3, float arg4, float arg5) {
  float local_FC;
  float local_F8;
  float local_F4;
  float local_F0;
  local_F4 = arg4 - arg2;
  local_FC = (float)0 - (arg5 - arg3);
  local_F8 = arg0 - arg2;
  local_F0 = arg1 - arg3;
  (void)(local_FC * local_F8 + local_F4 * local_F0);
  return;
}

static int32_t script_2B233(int32_t arg0, float arg1, float arg2, float arg3, float arg4) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  script_2B1FC((&local_F4)->x, (&local_F4)->z, arg1, arg2, arg3, arg4);
  end:;
}

static void script_2B25A(int32_t arg0, float arg1, float arg2, float arg3, float arg4) {
  Vector3 local_F4;
  Vector3 local_E8;
  local_F4.x = arg1;
  (&local_F4)->z = arg2;
  local_E8.x = arg3;
  (&local_E8)->z = arg4;
  (void)(Script_SetTaskDataValue(arg0, &local_F4, &local_E8, 0) & -(script_2B233(arg0, arg1, arg2, arg3, arg4) <= 0));
  return;
}

static void script_2B29F(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2B2BB(int32_t arg0, float arg1) {
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= arg1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_2B2DF(int32_t arg0, float arg1) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  return -!((&local_F4)->x > arg1);
}

static void script_2B2F5(int32_t arg0, float arg1) {
  while (1) {
    if (script_2B2DF(arg0, arg1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2B31A(int32_t arg0, float arg1) {
  Vector3 local_F4;
  while (1) {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    if (!((&local_F4)->x < arg1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_2B347(int32_t arg0, float arg1) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  return -!((&local_F4)->z > arg1);
}

static int32_t script_2B360(int32_t arg0, float arg1) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  return -!((&local_F4)->x < arg1);
}

static void script_2B376(int32_t arg0, float arg1) {
  while (1) {
    if (script_2B347(arg0, arg1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_2B39B(int32_t arg0, float arg1) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  return -!((&local_F4)->z < arg1);
}

static void script_2B3B4(int32_t arg0, float arg1) {
  while (1) {
    if (script_2B39B(arg0, arg1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_2B3D9(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
  Vector3i local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg0, &local_F4, 0);
  local_E4 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, arg1 - (&local_E8)->x, arg2 - (&local_E8)->z);
  local_E0 = local_E4 - (&local_F4)->y;
  if (local_E0 < 0) {
    local_E0 = -local_E0;
  }
  if (local_E0 >= 0x8000) {
    local_E0 = local_E0 - 0x10000;
    if (local_E0 < 0) {
      local_E0 = -local_E0;
    }
  }
  if (local_E0 <= arg3) {
    (&local_F4)->y = local_E4;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    (void)1;
  } else {
    if (arg4 == 0) {
      (&local_F4)->y = (&local_F4)->y - arg3;
    } else {
      (&local_F4)->y = (&local_F4)->y + arg3;
    }
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    (void)0;
  }
  end:;
}

static int32_t script_2B4BA(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
  while (1) {
    if (script_2B3D9(arg0, arg1, arg2, arg3, arg4)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2B4E5(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
  Vector3 local_F4;
  int32_t local_F0;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  local_F0 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, arg1 - (&local_F4)->x, arg2 - (&local_F4)->z);
  script_2B4BA(arg0, arg1, arg2, arg3, arg4);
  if ((local_F0 >= 0xE000) | (local_F0 < 0x2000)) {
    script_2B376(arg0, arg2);
  } else if (local_F0 < 0x6000) {
    script_2B2F5(arg0, arg1);
  } else if (local_F0 < 0xA000) {
    script_2B3B4(arg0, arg2);
  } else {
    script_2B31A(arg0, arg1);
  }
  return;
}

static void script_2B57D(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_E4;
  float local_E0;
  arg3 = arg3 * arg3;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  while (1) {
    Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
    local_E4 = (&local_E8)->x - (&local_F4)->x;
    local_E0 = (&local_E8)->z - (&local_F4)->z;
    if (!(local_E4 * local_E4 + local_E0 * local_E0 < arg3)) {
      break;
    }
    script_2B3D9(arg0, arg1, arg2, arg4, arg5);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2B5F4(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  int32_t local_E4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  local_E4 = 0;
  while (1) {
    if (local_E4 >= arg3) {
      break;
    }
    local_E4 = local_E4 + 1;
    script_2B3D9(arg0, arg1, arg2, arg4, arg5);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2B640(int32_t arg0) {
  while (script_2B1DF(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2B65F(int32_t arg0) {
  while (script_2B1DF(arg0) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_2B67E(int32_t arg0, int32_t arg1, int32_t arg2) {
  return SCNF_UIUtils(arg0, arg1, 2, arg2, 1, 0);
}

static int32_t script_2B693(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_2B1B1(1);
  local_FC = 0;
  local_F4 = Script_scenCreateTask(script_2B67E, 3, arg0, arg1, arg2);
  while (1) {
    local_F8 = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    if (local_F8 != 0xFFFF) {
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_F4);
  local_F8 = -(arg0 == (local_F8 & arg0));
  Script_sprintf("QTE [%d]\012", local_FC);
  while (local_FC < arg1 + arg2 + 3) {
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (local_F8) {
    script_2B186(0);
  } else {
    script_2B1B1(0);
  }
  script_2B186(1);
  Script_sprintf("check end\012");
  return local_F8;
}

static void script_2B76B(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_2B1B1(1);
  local_FC = 0;
  local_F4 = Script_scenCreateTask(script_2B67E, 3, arg0, arg1, arg2);
  while (1) {
    local_F8 = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    if (local_F8 != 0xFFFF) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_F4);
  local_F8 = -(arg0 == (local_F8 & arg0));
  if (local_F8) {
    script_2B186(0);
  } else {
    script_2B1B1(0);
  }
  script_2B186(1);
  (void)local_F8;
  return;
}

static void script_2B803(int32_t arg0, int32_t arg1, float arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_E4;
  while (1) {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
    local_E4 = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &local_E8);
    if (!(local_E4 >= arg2)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2B84A(int32_t arg0) {
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_2B879(int32_t arg0) {
  Script_motmToggleFlags(arg0, SCEN_Disable, 4);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  Script_RotateNodeUtils(arg0, &data_4522C, 0);
  return;
}

static int32_t script_2B8B1(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_scenToggleVisibleFor(arg0, 1);
  for (;;) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_mtwkGetSetRotation(arg1, &local_E8, 0);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2B90A(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4, int8_t arg5) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (arg5) {
    Script_ccowUtils(arg0, CCOW_DisableAll);
  }
  local_F4.x = arg1;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = arg2;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  (&local_E8)->y = arg3;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
  if (arg4 >= 0) {
    Script_charaToggleMotEnabled(arg0, 1);
    Script_PlayCharaMotion(arg0, arg4, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  script_2C0F3(arg0);
  return;
}

static void script_2B990(int32_t arg0) {
  int32_t unused_D8[10];
  int32_t local_D4;
  local_D4 = arg0;
  switch (local_D4) {
    case 1:
      script_2B90A(SHEN_NAME('HAKO'), -73.63f, 63.37f, 2730, -1, 0);
      script_2B90A(CHARA_ONO_, -118.3f, 85.4f, 10012, 8239, 0);
      Script_CharHandlers(CHARA_ONO_, 2, 0, 16);
      if (0) {
        Script_charaDisplay(CHARA_YOSE, CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(CHARA_YOSE, CDM_Show);
          goto L02BA14;
        }
      }
      L02BA14:;
      Script_scenToggleVisibleFor(CHARA_YOSE, 1);
      script_2B90A(CHARA_YOSE, -71.4f, 64.01f, 0, 8210, 0);
      break;
    case 2:
      if (0) {
        Script_charaDisplay(CHARA_SUMI, CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(CHARA_SUMI, CDM_Show);
          goto L02BA71;
        }
      }
      L02BA71:;
      Script_scenToggleVisibleFor(CHARA_SUMI, 1);
      script_2B90A(CHARA_SUMI, -21.53f, 36.55f, 41870, 8221, 1);
      break;
  }
  return;
}

static void script_2BAA1(void) {
  {
    int32_t local_FC;
    local_FC = 0x41534948;
    script_2C0F3(local_FC);
    Script_motmToggleFlags(local_FC, SCEN_Disable, 0x40);
    script_2B90A(CHARA_HISA, 7.16f, 27.63f, 16748, -1, 1);
    Script_motmToggleFlags(local_FC, SCEN_Enable, 4);
    Script_charaToggleMotEnabled(local_FC, 1);
    Script_PlayCharaMotion(local_FC, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(local_FC, 0.7f);
    Script_RequestFightAnimation(local_FC, 8222, 0.0f, 1, 0);
  }
  {
    int32_t local_FC;
    local_FC = 0x49525559;
    Script_motmToggleFlags(local_FC, SCEN_Disable, 0x40);
    script_2C0F3(local_FC);
    script_2B90A(CHARA_YURI, 7.16f, 28.14f, 16748, -1, 1);
    Script_motmToggleFlags(local_FC, SCEN_Enable, 4);
    Script_charaToggleMotEnabled(local_FC, 1);
    Script_PlayCharaMotion(local_FC, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(local_FC, 0.7f);
    Script_RequestFightAnimation(local_FC, 8223, 0.0f, 1, 0);
  }
  return;
}

static void script_2BBAC(int32_t arg0) {
  Vector3 local_F4;
  for (;;) {
    Script_GetAuthorState_And_SetCamPos(&local_F4, 0);
    Script_SetCharaLookAtPosition(arg0, 1, &local_F4, 0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_4557D)))
static void script_2BBCF(int32_t arg0, int32_t arg1, void *arg2, int32_t arg3, const void *arg4) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = 0;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "CTERMO1");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  while (local_E4 < arg3) {
    Script_InterpolateAndScaleAngles(local_E4, arg1, &local_F4, &local_E8, arg4);
    local_F4.x = (&local_F4)->x * 10.0f;
    (&local_F4)->y = (&local_F4)->y * 10.0f;
    (&local_F4)->z = (&local_F4)->z * 10.0f;
    Script_charaPosRotUtils(arg0, Script_ScenDataUtil(arg2, SCND_ReadU32, local_E4), &local_F4, &local_E8, CHARA_PART_Set);
    local_E4 = local_E4 + 1;
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_45585)))
static void script_2BC6C(int32_t arg0, int32_t arg1, void *arg2, int32_t arg3, const void *arg4) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = 0;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "CTERMO1");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  while (local_E4 < arg3) {
    Script_InterpolateAndScaleAngles(local_E4, arg1, &local_F4, &local_E8, arg4);
    local_F4.x = (&local_F4)->x * 10.0f;
    (&local_F4)->y = (&local_F4)->y * 9.4f;
    (&local_F4)->z = (&local_F4)->z * 10.0f;
    Script_charaPosRotUtils(arg0, Script_ScenDataUtil(arg2, SCND_ReadU32, local_E4), &local_F4, &local_E8, CHARA_PART_Set);
    local_E4 = local_E4 + 1;
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_455B8, 1, data_455BD)))
static int32_t script_2BD09(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_2C0F3(arg0);
  {
    Vector3 local_EC;
    local_EC.x = -118.98f;
    (&local_EC)->z = 84.14f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_EC);
  }
  {
    Vector3i local_EC;
    (&local_EC)->y = 50065;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_EC, 0);
  }
  local_FC = 20;
  local_F8 = 20;
  script_2BBCF(arg0, local_FC / 10, (void *)data_45590, 10, "BIKE");
  while (1) {
    if (script_2B1DF(12)) {
      script_2BBCF(arg0, local_FC / 10, (void *)data_45590, 10, "BIKE");
      local_FC = local_FC + local_F8;
      if (local_FC > 900) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_455E0, 1, data_455E5)))
static int32_t script_2BDC5(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_2C0F3(arg0);
  {
    Vector3 local_EC;
    local_EC.x = -104.45f;
    (&local_EC)->z = 68.48f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_EC);
  }
  {
    Vector3i local_EC;
    (&local_EC)->y = 50966;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_EC, 0);
  }
  local_FC = 20;
  local_F8 = 20;
  script_2BBCF(arg0, local_FC / 10, (void *)&data_455C4, 7, "KANB");
  while (1) {
    if (script_2B1DF(11)) {
      script_2BBCF(arg0, local_FC / 10, (void *)&data_455C4, 7, "KANB");
      local_FC = local_FC + local_F8;
      if (local_FC > 900) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_45674, 1, data_45679)))
static int32_t script_2BE81(int32_t arg0) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  int32_t local_F8;
  int32_t local_F4;
  local_FF = 0;
  script_2C0F3(arg0);
  {
    Vector3 local_E8;
    local_E8.x = -73.65f;
    (&local_E8)->z = 63.37f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E8);
  }
  {
    Vector3i local_E8;
    (&local_E8)->y = 0xB000;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
  }
  local_F8 = 20;
  local_F4 = 20;
  script_2BBCF(arg0, local_F8 / 10, (void *)data_455EC, 34, "APLE");
  while (1) {
    if (script_2B1DF(7)) {
      script_2BBCF(arg0, local_F8 / 10, (void *)data_455EC, 34, "APLE");
      if (local_FF == 0) {
        Script_executeAicaCommand(853161, 0, 0);
        local_FF = 1;
      }
      local_F8 = local_F8 + local_F4;
      if (local_F8 > 830) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2BF5F(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  local_F7 = 0;
  local_FC = 20;
  local_F8 = 20;
  while (local_FC < 450) {
    script_2BBCF(SHEN_NAME('FLIT'), local_FC / 10, (void *)&data_45680, 34, "APL2");
    if (local_F7 == 0) {
      Script_executeAicaCommand(853161, 0, 0);
      local_F7 = 1;
    }
    local_FC = local_FC + local_F8;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_45754, 1, data_45759)))
static int32_t script_2BFCB(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  {
    Vector3 local_EC;
    local_EC.x = -111.16f;
    (&local_EC)->z = 100.52f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_EC);
  }
  {
    Vector3i local_EC;
    (&local_EC)->y = 20388;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_EC, 0);
  }
  local_FC = 20;
  local_F8 = 20;
  while (task_99E & 4) {
    SCNF_WaitFrames_0000(1);
  }
  script_2BC6C(arg0, local_FC / 10, (void *)&data_45710, 17, "SOBA");
  script_2B640(10);
  local_FC = 290;
  if (script_2B1DF(0) == 0) {
    while (1) {
      script_2BC6C(arg0, local_FC / 10, (void *)&data_45710, 17, "SOBA");
      local_FC = local_FC + local_F8;
      if (local_FC >= 1500) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static int32_t script_2C0A6(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 2;
  local_F8 = 3;
  while (1) {
    script_2BBCF(arg0, local_FC, (void *)&data_45760, 4, "HAKO");
    local_FC = local_FC + local_F8;
    if (local_FC >= 260) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2C0F3(int32_t arg0) {
  Script_scenToggleVisibleFor(arg0, 1);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_2C107(int32_t arg0) {
  Script_scenToggleVisibleFor(arg0, 0);
  Script_charaDisplay(arg0, CDM_Hide);
  return;
}

static int32_t script_2C11B(int32_t arg0) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  while ((local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0)) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenToggleVisibleFor(arg0, 0);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_SetCPAMFlags(arg0, 0);
  return local_FC;
}

static void script_2C189(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  return;
}

static void script_2C1A9(const char *arg0, const char *arg1) {
  int32_t local_FC;
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1)) {
  } else {
    while (1) {
      local_FC = Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1);
      if (local_FC) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static void script_2C200(int32_t arg0, int32_t arg1, float arg2, float arg3) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  arg2 = ((&local_F4)->x - arg2) / (float)arg1;
  arg3 = ((&local_F4)->z - arg3) / (float)arg1;
  while (arg1) {
    local_F4.x = (&local_F4)->x - arg2;
    (&local_F4)->z = (&local_F4)->z - arg3;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2C272(void) {
  Script_WeatherTaskUtil(FreeTaskData);
  Script_WeatherTaskUtil(InitTaskData);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
  return;
}

static void script_2C299(void) {
  Script_charaVisUtils(CHARA_AKIR, 1);
  Script_charaVisUtils(CHARA_JIMY, 1);
  Script_charaVisUtils(CHARA_JIMY, 1);
  return;
}

static void script_2C2C0(void) {
  Script_charaVisUtils(CHARA_AKIR, 0);
  Script_charaVisUtils(CHARA_JIMY, 0);
  Script_charaVisUtils(CHARA_JIMY, 0);
  Script_SetFogEnabled(1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
  return;
}

static int32_t script_2C303(const char *arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_AddCharacterHandler(3, arg0, 0);
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  local_E8.x = (&local_E8)->z = 0;
  (&local_E8)->y = arg4;
  Script_EVT_SetPosition(local_E4, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(local_E4, 0x38000000, &local_E8, 0);
  Script_ccowSetHeightMode(local_E4, CCOW_KeepHeight);
  Script_charaDisplay(local_E4, CDM_Show);
  return local_E4;
}

static void script_2C371(int32_t arg0, int32_t arg1) {
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, 0, 0, 16);
  Script_PlayCharaMotion(arg0, arg1, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  return;
}

static void script_2C3AB(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_PlayCharaMotion(arg0, arg1, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  return;
}

static int32_t script_2C3D1(int32_t arg0) {
  int32_t unused_E8[6];
  int32_t local_E4;
  int32_t local_E0;
  script_2C0F3(arg0);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  script_2B879(arg0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  script_2B84A(arg0);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, 3, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_mapmSetCamFlags(0, 0);
  local_E0 = script_2C303("DR28_001", -95.490257f, 0.0274f, 84.369667f, 4551);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  SCNF_WaitFrames_0000(4);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_457B0, (int32_t)&data_45778, 4);
  Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.29f);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(1463, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
  SCNF_WaitFrames_0000(1);
  script_2B2F5(arg0, -92.5f);
  Script_SetMapControlTaskFlag(local_E0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  if (script_2B1DF(0) == 0) {
    script_2B2F5(arg0, -93.5f);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, 0, 0, 16);
    Script_PlayCharaMotion(arg0, 8195, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    Script_charaToggleMotEnabled(arg0, 1);
    script_2B29F(arg0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(arg0, 0);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_45814, (int32_t)&data_45778, 4);
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.29f);
  }
  script_2B2F5(arg0, -107.2f);
  script_2B640(1);
  if (script_2B1DF(0) == 0) {
    script_2B186(10);
    script_2C371(arg0, 8206);
    script_2B2BB(arg0, 30.0f);
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x20);
    Script_PlayCharaMotion(arg0, 8206, 25.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x20);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(arg0, 0);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_45858, (int32_t)&data_45778, 4);
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.29f);
  }
  while (Script_LineWalkUtils(LNWK_SCRIPT_GET_STATUS, arg0) != 5) {
    SCNF_WaitFrames_0000(1);
  }
  Script_mapmSetCamFlags(0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 35.0f);
  script_2B186(13);
  script_2B186(4);
  local_E4 = 2;
  while (local_E4 < 9) {
    Script_srchControl((void *)task_9A0[local_E4]);
    local_E4 = local_E4 + 1;
  }
  if (script_2B1DF(10) == 0) {
    Script_srchControl((void *)task_9A0[1]);
  }
  SCNF_WaitFrames_0000(2);
  end:;
}

static int32_t script_2C719(int32_t arg0) {
  Vector3i local_F4;
  int32_t unused_F0;
  Vector3 local_E4;
  Script_charaDisplay(arg0, CDM_Show);
  script_2B879(arg0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  script_2B84A(arg0);
  local_E4.x = -84.65f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 82.328201f;
  Script_EVT_SetPosition(arg0, 0x78000000, &local_E4);
  local_F4.x = (&local_F4)->z = 0;
  (&local_F4)->y = 18204;
  Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_45888, (int32_t)&data_45778, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.26f) >= 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  end:;
}

static void script_2C7FA(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_ryoControl(PCCTL_SetCPCTFlag400, 0);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  local_F4.x = 0.0f;
  (&local_F4)->y = (&local_F4)->x;
  (&local_F4)->z = (&local_F4)->x;
  local_E8.x = 13653;
  (&local_E8)->y = 30947;
  (&local_E8)->z = 45522;
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  if (Script_motnUtils(MOTN_GetCharacterBudgetFlag, arg0) == 1) {
    Script_motnUtils(MOTN_SetCharacterBudgetFlag, arg0, 0);
  }
  Script_fixoAttachEx(arg1, arg0, 18, &local_F4, &local_E8);
  Script_CharHandlers(arg0, 0, 1, 16);
  Script_motmGetCharaFlags(arg0, MOTM_PoseHigh, SCEN_Write);
  script_2C0F3(arg1);
  return;
}

static int32_t script_2C8B9(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  Vector3i local_D0;
  Script_ccowUtils(SHEN_NAME('DSOB'), CCOW_DisableAll);
  Script_motmToggleFlags(SHEN_NAME('DSOB'), SCEN_Enable, 4);
  Script_PlayCharaMotion(SHEN_NAME('DSOB'), 8237, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  (void)((task_9C5 = -1) & 4);
  (void)((task_9C4 = 0) & 4);
  (void)((task_9C6 = 0) & 4);
  Script_ccowSetHeightMode(SHEN_NAME('SEIR'), CCOW_KeepHeight);
  while (1) {
    if (task_9C4 & 0x20) {
      break;
    }
    if (task_9C5 == 0) {
      if (Script_IsInEventCam() == 0) {
        break;
      }
    }
    Script_EVT_GetPosition(CHARA_SOBA, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(CHARA_SOBA, &local_DC, 0);
    local_DC.x = (&local_DC)->z = 0;
    Script_EVT_GetPosition(SHEN_NAME('BICY'), -1, &local_E8, 0);
    Script_mtwkGetSetRotation(SHEN_NAME('BICY'), &local_D0, 0);
    (&local_F4)->y = -5.0f;
    (&local_E8)->y = -5.0f;
    Script_EVT_SetPosition(CHARA_SOBA, POS_XYZ, &local_F4);
    Script_EVT_SetPosition(SHEN_NAME('DCYC'), POS_XYZ, &local_E8);
    (&local_F4)->y = 0.0f;
    (&local_E8)->y = 0.0f;
    Script_EVT_SetPosition(SHEN_NAME('DSOB'), POS_XYZ, &local_F4);
    if (task_9C6 & 0x80) {
      (&local_DC)->y = (&local_DC)->y - 9102;
    }
    Script_EVT_SetRotation(SHEN_NAME('DSOB'), 0x38000000, &local_DC, 0);
    Script_EVT_SetRotation(SHEN_NAME('DCYC'), 0x38000000, &local_D0, 0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2CA32(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  SCNF_WaitFrames_0000(5);
  Script_ccowSetHeightMode(SHEN_NAME('SEIR'), CCOW_KeepHeight);
  Script_EVT_GetPosition(SHEN_NAME('SEIR'), -1, &local_F0, ENTITY_XFORM_WORLD_SPACE);
  Script_fixoClearAttachment(SHEN_NAME('SEIR'));
  local_FC = 20;
  while (local_FC) {
    Script_EVT_GetPosition(SHEN_NAME('SEIR'), -1, &local_F0, ENTITY_XFORM_WORLD_SPACE);
    local_F0.x = (&local_F0)->x + 0.1f;
    Script_EVT_SetPosition(SHEN_NAME('SEIR'), 0x78000000, &local_F0);
    Script_mtwkGetSetRotation(SHEN_NAME('SEIR'), &local_E4, 0x40000000);
    (&local_E4)->y = (&local_E4)->y + 182;
    Script_EVT_SetRotation(SHEN_NAME('SEIR'), 0x78000000, &local_E4, 0);
    local_FC = local_FC - 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2CAE5(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  (void)((task_999 = -1) & 8);
  script_2C0F3(CHARA_SOBA);
  Script_charaDisplay(CHARA_SOBA, CDM_Hide);
  script_2C0F3(SHEN_NAME('BICY'));
  script_2C0F3(SHEN_NAME('DSOB'));
  script_2C0F3(SHEN_NAME('DCYC'));
  script_2C0F3(SHEN_NAME('SEIR'));
  local_F4.x = (&local_F4)->z = 0.0f;
  (&local_F4)->y = -5.0f;
  Script_EVT_SetPosition(CHARA_SOBA, 0x78000000, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('BICY'), 0x78000000, &local_F4);
  Script_scenCreateTask(script_2C8B9, 0);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(SHEN_NAME('DSOB'), SCEN_Enable, 0x40);
  Script_PlayCharaMotion(SHEN_NAME('DSOB'), 8237, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_vehiUtils(VehicleCommand_AttachRiderToVehicle, CHARA_SOBA, 0x59434942, 0);
  Script_vehiUtils(VehicleCommand_Unknown8_SetFlag, CHARA_SOBA, 0);
  Script_vehiUtils(VehicleCommand_StartPathFollow, CHARA_SOBA, &data_458FC, 5, 1);
  Script_vehiUtils(VehicleCommand_SetMaxSpeed, CHARA_SOBA, 13.0f);
  local_E8.x = (&local_E8)->z = 0;
  (&local_E8)->y = 46603;
  Script_EVT_SetRotation(CHARA_SOBA, 0x78000000, &local_E8, 0);
  Script_EVT_SetRotation(SHEN_NAME('DSOB'), 0x78000000, &local_E8, 0);
  Script_EVT_SetRotation(SHEN_NAME('DCYC'), 0x78000000, &local_E8, 0);
  Script_EVT_SetRotation(SHEN_NAME('BICY'), 0x78000000, &local_E8, 0);
  script_2B640(9);
  Script_vehiUtils(VehicleCommand_SetControlProfile, CHARA_SOBA, 1, 2.0f);
  Script_executeAicaCommand(328873, 0, 0);
  script_2C7FA(SHEN_NAME('DSOB'), SHEN_NAME('SEIR'));
  script_2B640(1);
  script_2B2F5(CHARA_AKIR, -107.3f);
  Script_vehiUtils(VehicleCommand_SetControlProfile, CHARA_SOBA, 3, 0.96f);
  Script_executeAicaCommand(394409, 0, 0);
  script_2B640(10);
  if (script_2B1DF(0) == 0) {
    (void)((task_999 = 0) & 8);
    if (task_9C7 & 1) {
      SCNF_WaitFrames_0000(3);
    } else {
      SCNF_WaitFrames_0000(3);
    }
    (void)((task_9C6 = -1) & 8);
    Script_PlayCharaMotion(SHEN_NAME('DSOB'), 8238, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_2CA32, 0);
    while (Script_vehiUtils(VehicleCommand_GetState, CHARA_SOBA) != 4) {
      SCNF_WaitFrames_0000(1);
    }
  }
  while (Script_vehiUtils(VehicleCommand_GetState, CHARA_SOBA) != 4) {
    SCNF_WaitFrames_0000(1);
  }
  script_2C107(CHARA_SOBA);
  return (task_9C5 = 0) & 8;
}

static int32_t script_2CD9A(int32_t arg0) {
  script_2C0F3(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    Vector3 local_F4;
    local_F4.x = -96.9f;
    (&local_F4)->z = 87.1f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 54613;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaToggleMotEnabled(arg0, 1);
  script_2B2F5(CHARA_AKIR, -93.0f);
  if (script_2B1DF(0)) {
    script_2C3AB(arg0, 8242, 0x42400000);
    (void)((task_9C7 = -1) & 0x20);
  } else {
    script_2C3AB(arg0, 8240, 0x423C0000);
    (void)((task_9C7 = 0) & 0x20);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_45924)))
static int32_t script_2CE5F(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F0[2];
  int8_t local_EF;
  int8_t local_EE;
  int8_t local_ED;
  int8_t local_EC;
  int8_t local_EB;
  int8_t local_EA;
  local_EB = local_EA = 0;
  task_995 = 0;
  task_9A0[0] = Script_scenCreateTask(script_2C3D1, 1, CHARA_AKIR);
  task_9A0[1] = Script_scenCreateTask(script_2BFCB, 1, SHEN_NAME('SEIR'));
  task_9A0[2] = Script_scenCreateTask(script_2C719, 1, CHARA_TERY);
  task_9A0[4] = Script_scenCreateTask(script_2CAE5, 0);
  task_9A0[7] = Script_scenCreateTask(script_2B8B1, 2, SHEN_NAME('PIL2'), SHEN_NAME('DSOB'));
  task_9A0[3] = Script_scenCreateTask(script_2CD9A, 1, CHARA_HANA);
  task_9A0[8] = Script_scenCreateTask(script_2B8B1, 2, SHEN_NAME('PIL3'), CHARA_HANA);
  task_9A0[5] = Script_scenCreateTask(script_2B8B1, 2, SHEN_NAME('PIL0'), CHARA_AKIR);
  task_9A0[6] = Script_scenCreateTask(script_2B8B1, 2, SHEN_NAME('PIL1'), CHARA_TERY);
  local_EF = 0;
  local_EE = -1;
  local_ED = 0;
  local_EC = -1;
  while (local_EF != -1) {
    if (local_EF != local_EE) {
      local_F8 = 0;
      local_EE = local_EF;
    } else {
      local_F8 = local_F8 + 1;
    }
    {
      uint8_t unused_E8[2];
      int32_t local_E4;
      local_E4 = local_EF;
      switch (local_E4) {
        case 0:
          local_EF = 5;
          break;
        case 5:
          if (script_2B2DF(CHARA_AKIR, -84.5f)) {
            script_2B186(7);
            local_EF = 6;
          }
          break;
        case 6:
          script_2B1B1(1);
          script_2B186(7);
          local_EF = 7;
          local_ED = 2;
          break;
        case 7:
          local_EF = 8;
          Script_scenCreateTask(script_2B693, 3, 64, 15, 0);
          break;
        case 8:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_EB = local_EB + 1;
            } else {
              local_EA = local_EA + 1;
            }
            local_EF = 9;
          }
          break;
        case 9:
          if (script_2B2DF(CHARA_AKIR, -97.5f)) {
            script_2B186(9);
            if (script_2B2DF(CHARA_AKIR, -103.6f)) {
              local_EF = 10;
            }
          }
          break;
        case 10:
          script_2B1B1(1);
          script_2B186(9);
          local_EF = 11;
          local_ED = 3;
          break;
        case 11:
          Script_scenCreateTask(script_2B693, 3, 4, 12, 0);
          local_EF = 12;
          break;
        case 12:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_EB = local_EB + 1;
            } else {
              local_EA = local_EA + 1;
            }
            local_EF = 13;
          }
          break;
        case 13:
          local_EF = 14;
          break;
        case 14:
          task_995 = local_EA;
          script_2B186(3);
          script_2B186(2);
          break;
      }
    }
    if (local_ED != local_EC) {
      local_FC = 0;
      local_EC = local_ED;
    } else {
      local_FC = local_FC + 1;
    }
    {
      uint8_t unused_E8[2];
      int32_t local_E4;
      local_E4 = local_ED;
      switch (local_E4) {
        case 0:
          local_E4 = local_FC;
          if (local_E4 == 0) {
            Script_executeAicaCommand(2984, 0, 0);
            Script_executeAicaCommand(1184, 2, 127);
            Script_executeAicaCommand(2464, 2, 20);
            goto L02D1DC;
          }
          L02D1DC:;
          break;
        case 2:
          local_E4 = local_FC;
          switch (local_E4) {
            case 20:
              if (script_2B1DF(0)) {
                Script_seqEnterEvtCam(1471, 0, AUTH_None);
              } else {
                Script_seqEnterEvtCam(1458, SHEN_NAME('PIL3'), SHEN_NAME('PIL3'));
              }
              break;
            case 31:
              if (script_2B1DF(0)) {
                Script_scenCreateTask(script_2B0F6, 0);
              }
              break;
            case 35:
              if (script_2B1DF(0) == 0) {
                Script_executeAicaCommand(0x1004A9, 0, 0);
              }
              break;
            case 40:
              Script_scenCreateTask(script_2B102, 0);
              break;
            case 42:
              if (script_2B1DF(0)) {
                Script_executeAicaCommand(0x1104A9, 0, 0);
              }
              break;
            case 57:
              if (script_2B1DF(0)) {
                Script_seqEnterEvtCam(1451, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              } else {
                Script_ShowUIMessageFromID("E1004AA017");
              }
              break;
            case 117:
              if (script_2B1DF(0) == 0) {
                Script_seqEnterEvtCam(1451, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
          }
          break;
        case 3:
          local_E4 = local_FC;
          switch (local_E4) {
            case 18:
              if (script_2B1DF(0)) {
                Script_seqEnterEvtCam(1465, 0, AUTH_None);
              } else {
                Script_seqEnterEvtCam(1467, 0, AUTH_None);
              }
              break;
            case 24:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2AF16, 0);
              }
              break;
            case 25:
              if (script_2B1DF(0) == 0) {
                Script_executeAicaCommand(0x1204A9, 0, 0);
              }
              break;
            case 27:
              if (script_2B1DF(0)) {
                Script_scenCreateTask(script_2B11A, 0);
                Script_executeAicaCommand(0x1204A9, 0, 0);
              }
              break;
            case 33:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2B10E, 0);
              }
              break;
            case 50:
              if (script_2B1DF(0) == 0) {
                Script_executeAicaCommand(0x1304A9, 0, 0);
              }
              break;
            case 55:
              if (script_2B1DF(0)) {
                Script_convSetCamInactiveFlag(0);
                Script_SetGameStateTo(AUTH_None);
              }
              break;
            case 65:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2AF56, 0);
              }
              break;
            case 73:
              if (script_2B1DF(0) == 0) {
                Script_convSetCamInactiveFlag(0);
                Script_SetGameStateTo(AUTH_None);
              }
              break;
          }
          break;
      }
    }
    if (script_2B1DF(13)) {
      local_EF = -1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2D4C0(void) {
  int32_t local_FC;
  local_FC = 0;
  while (local_FC < 500) {
    Script_sprintf("check count = %d\012", local_FC);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2D4F2(void) {
  int32_t local_FC;
  local_FC = 40;
  while (local_FC) {
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC - 1;
  }
  return;
}

static int32_t script_2D515(int32_t arg0) {
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L02D539;
    }
  }
  L02D539:;
  Script_scenToggleVisibleFor(arg0, 1);
  script_2C107(arg0);
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = -92.004501f;
    (&local_E8)->z = 61.267799f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E8);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 34454;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B640(21);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.5f);
  script_2B640(1);
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = -92.004501f;
    (&local_E8)->z = 61.267799f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E8);
  }
  if (script_2B1DF(0)) {
    script_2C0F3(arg0);
  } else {
    script_2C107(arg0);
  }
  script_2B640(14);
  script_2B640(1);
  if (script_2B1DF(0)) {
    Vector3 local_F4;
    Script_PlayCharaMotion(arg0, 8230, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(20);
    Script_ShowUIMessageFromID("TE1004AG001");
    script_2B2BB(arg0, 30.0f);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetCharaLookAtPosition(arg0, 1, &local_F4, 0);
    script_2B2BB(arg0, 60.0f);
    Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
    script_2B29F(arg0);
  } else {
    Vector3 local_F4;
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    (&local_F4)->y = 0.1f;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
    Script_PlayCharaMotion(arg0, 8214, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_2B2BB(arg0, 5.0f);
    Script_scenCreateTask(script_2B006, 0);
    script_2B2BB(arg0, 15.0f);
    Script_executeAicaCommand(0x1404A9, 0, 0);
    script_2B2BB(arg0, 34.0f);
    Script_executeAicaCommand(0x1504A9, 0, 0);
    script_2B29F(arg0);
  }
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.6f);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  end:;
}

static int32_t script_2D7CD(int32_t arg0) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = 0;
  while (local_FC < 20) {
    (&local_F0)->y = -163;
    Script_EVT_SetRotation(arg0, -0x70000000, &local_F0, 0);
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
  }
  end:;
}

static int32_t script_2D80C(void) {
  while (1) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x594E4F54)) {
      Script_SetCPAMFlags(CHARA_TONY, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2D83B(int32_t arg0) {
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L02D85F;
    }
  }
  L02D85F:;
  Script_scenToggleVisibleFor(arg0, 1);
  script_2C107(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    Vector3i local_F4;
    (&local_F4)->y = 20979;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  {
    Vector3 local_F4;
    local_F4.x = -94.258797f;
    (&local_F4)->z = 65.330101f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  script_2B640(21);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B640(1);
  if (script_2B1DF(0)) {
    Script_PlayCharaMotion(arg0, 8220, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_charaToggleMotEnabled(arg0, 0);
    Script_PlayCharaMotion(arg0, 8213, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_2B2BB(arg0, 10.0f);
    Script_executeAicaCommand(0x1404A9, 0, 0);
    script_2B2BB(arg0, 80.0f);
    Script_scenCreateTask(script_2D7CD, 1, arg0);
    Script_charaToggleMotEnabled(arg0, 1);
    Script_SetCharaMotionPlaybackRate(arg0, 0.8f);
  }
  end:;
}

static int32_t script_2D9A9(int32_t arg0) {
  Vector3 local_F4;
  Script_charaToggleMotEnabled(arg0, 1);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  {
    int32_t unused_F0;
    int32_t local_EC;
    local_EC = script_2C303("DR01_009", -123.658234f, 0.027396f, 96.484764f, 18513);
    local_F4.x = -103.89f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 95.71f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    {
      Vector3i local_E0;
      (&local_E0)->y = 25211;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E0, 0);
    }
    script_2C0F3(CHARA_TERY);
    Script_charaToggleMotEnabled(CHARA_TERY, 1);
    {
      Vector3 local_E0;
      local_E0.x = -114.5f;
      (&local_E0)->z = 98.9f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_E0);
    }
    {
      Vector3i local_E0;
      (&local_E0)->y = 12196;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E0, 0);
    }
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_2C0F3(CHARA_TERY);
    script_2B376(arg0, 97.07f);
    script_2B4BA(arg0, -121.55f, 88.14f, 910, 0);
    script_2B376(arg0, 88.7f);
    script_2B4BA(arg0, -119.75f, 85.2f, 2730, 0);
    script_2C0F3(CHARA_TERY);
    script_2B640(1);
    Script_SetMapControlTaskFlag(local_EC, 1);
  }
  if (script_2B1DF(0)) {
    Script_executeAicaCommand(0x1104A9, 0, 0);
    script_2B31A(arg0, -125.0f);
    script_2B57D(arg0, -120.1f, 81.63f, 1.8f, 1820, 1);
  } else {
    int32_t unused_E0[5];
    Vector3i local_D4;
    script_2B31A(arg0, -120.3f);
    Script_scenCreateTask(script_2B0DE, 0);
    Script_PlayCharaMotion(arg0, 8193, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(25);
    Script_SetCharaMotionPlaybackRate(CHARA_ONO_, 0.0f);
    SCNF_WaitFrames_0000(25);
    Script_motmToggleFlags(CHARA_ONO_, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(CHARA_ONO_, 8227, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_ONO_, 1.2f);
    Script_motmSetBlendFrames(CHARA_ONO_, MOTM_BLEND_FRAMES_15);
    SCNF_WaitFrames_0000(10);
    (&local_D4)->y = 4551;
    Script_EVT_SetRotation(CHARA_ONO_, MATRIX_XFORM_Y, &local_D4, 0);
    Script_CharHandlers(CHARA_ONO_, 0, 8, 16);
    script_2B29F(arg0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  script_2B4E5(arg0, -111.33f, 72.31f, 910, 0);
  script_2B4BA(arg0, -109.99f, 71.53f, 2730, 0);
  script_2B640(1);
  if (script_2B1DF(0)) {
    int32_t unused_EC[2];
    Vector3 local_E0;
    Vector3i local_D4;
    int32_t local_D0;
    script_2B31A(arg0, -108.98f);
    Script_PlayCharaMotion(arg0, 8207, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(0x1104A9, 0, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.25f);
    script_2B29F(arg0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    Script_EVT_GetPosition(arg0, -1, &local_E0, 0);
    Script_mtwkGetSetRotation(arg0, &local_D4, 0);
    local_D0 = script_2FA07((&local_E0)->x, (&local_E0)->z, -94.62f, 64.94f);
    local_D0 = local_D0 - (&local_D4)->y;
    if (local_D0 < 0) {
      Script_sprintf("LEFT[%d]\012", local_D0);
      script_2B4E5(arg0, -94.62f, 64.94f, 910, 1);
    } else {
      Script_sprintf("RIGHT[%d]\012", local_D0);
      script_2B4E5(arg0, -94.62f, 64.94f, 910, 0);
    }
  } else {
    int32_t unused_F0;
    int32_t local_EC;
    local_EC = script_2C303("DR02_017", -124.66507f, 0.072396f, 88.421341f, 18513);
    script_2B31A(arg0, -109.48f);
    Script_PlayCharaMotion(arg0, 8206, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_2B2BB(arg0, 36.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_2B57D(arg0, -100.88f, 68.64f, 1.0f, 1820, 0);
    script_2B4E5(arg0, -94.62f, 64.94f, 910, 1);
    Script_SetMapControlTaskFlag(local_EC, 1);
  }
  script_2B4E5(arg0, -84.97f, 62.46f, 546, 0);
  Script_seqEnterEvtCam(1040, 0, AUTH_None);
  {
    int32_t unused_EC[2];
    Vector3 local_E0;
    local_E0.x = -87.12f;
    (&local_E0)->z = 63.04f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E0);
  }
  script_2C107(CHARA_TONY);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  script_2B4BA(arg0, -71.7f, 62.08f, 546, 0);
  script_2B640(1);
  {
    int32_t unused_F0;
    int32_t local_EC;
    local_EC = script_2C303("DR15_009", -74.390099f, 0.02505f, 68.947693f, -0x8000);
    if (script_2B1DF(0)) {
      script_2B31A(arg0, -79.06f);
      Script_executeAicaCommand(0x1604A9, 0, 0);
      Script_PlayCharaMotion(arg0, 8233, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
      Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
      script_2B2BB(arg0, 38.0f);
      Script_executeAicaCommand(0x1704A9, 0, 0);
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_6);
      script_2B4E5(arg0, -68.4f, 60.19f, 910, 1);
    } else {
      script_2B31A(arg0, -75.83f);
      Script_PlayCharaMotion(arg0, 8205, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_scenCreateTask(script_2BF5F, 0);
      SCNF_WaitFrames_0000(20);
      Script_executeAicaCommand(0x1504A9, 0, 0);
      script_2B2BB(arg0, 60.0f);
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      script_2B4E5(arg0, -68.4f, 60.19f, 2730, 1);
    }
    script_2B640(2);
    Script_SetMapControlTaskFlag(local_EC, 1);
  }
  script_2C107(CHARA_TERY);
  script_2C107(CHARA_NORK);
  script_2C107(CHARA_AKSK);
  script_2C107(CHARA_YMGC);
  script_2C107(CHARA_KURI);
  script_2C107(CHARA_MRUA);
  if (script_2B1DF(3) == 0) {
    Script_PlayCharaMotion(arg0, 8201, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(0x1804A9, 0, 0);
    script_2B2BB(arg0, 25.0f);
    Script_charaToggleMotEnabled(arg0, 0);
    Script_PlayCharaMotion(arg0, 8200, 15.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      Script_RotateNodeUtils(arg0, &data_4522C, 0);
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(1);
    script_2B29F(arg0);
  } else {
    int32_t unused_EC[2];
    Vector3 local_E0;
    Script_PlayCharaMotion(arg0, 8201, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(0x1804A9, 0, 0);
    script_2B2BB(arg0, 55.0f);
    SCNF_WaitFrames_0000(5);
    local_E0.x = -62.32f;
    (&local_E0)->y = 1.75f;
    (&local_E0)->z = 53.59f;
    Script_SetCharaLookAtPosition(arg0, 1, &local_E0, 2);
  }
  end:;
}

static int32_t script_2E1CD(int32_t arg0) {
  Vector3 local_F4;
  if (script_2B1DF(0)) {
    script_2B31A(arg0, -97.89f);
    Script_PlayCharaMotion(arg0, 8208, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(0x1104A9, 0, 0);
    script_2B2BB(arg0, 37.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    local_F4.x = -97.3181f;
    (&local_F4)->z = 65.080803f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  } else {
    script_2B31A(arg0, -98.54f);
    Script_PlayCharaMotion(arg0, 8196, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_2B2BB(arg0, 54.0f);
    Script_scenCreateTask(script_2B0D2, 0);
    script_2B29F(arg0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
  }
  script_2B186(19);
  return Script_srchControl(0);
}

static int32_t script_2E36D(int32_t arg0) {
  if (script_2B1DF(0)) {
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(arg0, 8208, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(0x1104A9, 0, 0);
    SCNF_WaitFrames_0000(5);
    Script_scenCreateTask(script_2B13E, 0);
    script_2B29F(arg0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_12);
  } else {
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(arg0, 8197, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_2B2BB(arg0, 60.0f);
    Script_scenCreateTask(script_2B0BA, 0);
    script_2B29F(arg0);
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  }
  script_2B186(20);
  return Script_srchControl(0);
}

static int32_t script_2E486(int32_t arg0) {
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = -121.89f;
    (&local_E8)->z = 94.66f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E8);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 3600;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
  }
  script_2C0F3(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  script_2B640(9);
  script_2C0F3(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_START000, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B29F(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B4E5(arg0, -122.51f, 86.21f, 910, 0);
  script_2B4E5(arg0, -110.18f, 72.35f, 910, 0);
  script_2C107(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  end:;
}

static int32_t script_2E56D(int32_t arg0) {
  Vector3 local_F4;
  script_2C0F3(arg0);
  local_F4.x = -109.84f;
  (&local_F4)->z = 72.73f;
  Script_EVT_SetPosition(CHARA_TERY, 0x28000000, &local_F4);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  script_2B4E5(arg0, -104.61f, 69.53f, 0x10000, 0);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, 8226, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B29F(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B4E5(arg0, -94.62f, 64.94f, 910, 0);
  script_2B4BA(arg0, -92.4f, 64.32f, 2730, 0);
  script_2B4E5(arg0, -84.63f, 62.45f, 910, 0);
  script_2B4E5(arg0, -70.11f, 63.25f, 910, 0);
  script_2C107(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  end:;
}

static int32_t script_2E6B7(int32_t arg0) {
  Vector3 local_F4;
  script_2C0F3(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  local_F4.x = -77.69f;
  (&local_F4)->z = 62.72f;
  Script_EVT_SetPosition(CHARA_TERY, 0x28000000, &local_F4);
  script_2B4BA(arg0, -73.42f, 62.46f, 0x10000, 0);
  script_2B31A(arg0, -75.47f);
  Script_PlayCharaMotion(arg0, 8225, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B29F(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B4E5(arg0, -62.53f, 54.16f, 1820, 1);
  Script_charaToggleMotEnabled(arg0, 0);
  script_2C107(arg0);
  end:;
}

static int32_t script_2E7A5(void) {
  int32_t unused_DC[9];
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int32_t local_CC;
  int32_t local_C8;
  int32_t local_C4;
  int32_t local_C0;
  int8_t local_BF;
  int8_t local_BE;
  int8_t local_BD;
  int8_t local_BC;
  int8_t local_BB;
  int8_t local_BA;
  int8_t local_B9;
  local_BB = task_995;
  local_BA = task_995;
  (void)((task_9C8 = 0) & 1);
  (void)((task_9C9 = -1) & 1);
  script_2B1B1(21);
  script_2B1B1(4);
  local_CC = 0;
  while (local_CC < 10) {
    *(&local_D8 + local_CC) = 0;
    local_CC = local_CC + 1;
  }
  local_D8 = Script_scenCreateTask(script_2D9A9, 1, CHARA_AKIR);
  *(&local_D8 + 1) = Script_scenCreateTask(script_2E486, 1, CHARA_TERY);
  *(&local_D8 + 2) = Script_scenCreateTask(script_2BE81, 1, SHEN_NAME('FLIT'));
  *(&local_D8 + 3) = 0;
  *(&local_D8 + 4) = Script_scenCreateTask(script_2BDC5, 1, SHEN_NAME('D201'));
  *(&local_D8 + 5) = Script_scenCreateTask(script_2BD09, 1, SHEN_NAME('BIKE'));
  *(&local_D8 + 6) = Script_scenCreateTask(script_2D515, 1, CHARA_HOND);
  *(&local_D8 + 7) = Script_scenCreateTask(script_2D83B, 1, CHARA_TONY);
  local_C0 = Script_scenCreateTask(script_2D80C, 0);
  *(&local_D8 + 8) = Script_scenCreateTask(script_2B8B1, 2, SHEN_NAME('PIL0'), CHARA_AKIR);
  *(&local_D8 + 9) = Script_scenCreateTask(script_2B8B1, 2, SHEN_NAME('PIL1'), CHARA_TERY);
  script_2B990(1);
  local_BF = 0;
  local_BE = -1;
  local_BD = 0;
  local_BC = -1;
  while (local_BF != -1) {
    if (local_BF != local_BE) {
      local_D0 = 0;
      local_BE = local_BF;
    } else {
      local_D0 = local_D0 + 1;
    }
    task_98C = task_98C + 1;
    {
      uint8_t unused_B8;
      int32_t local_B4;
      local_B4 = local_BF;
      switch (local_B4) {
        case 0:
          if (local_D0 >= 5) {
            local_BF = 4;
          }
          break;
        case 4:
          script_2B186(9);
          script_2B186(10);
          if (script_2B2DF(CHARA_AKIR, -114.15f)) {
            local_BF = 5;
          }
          break;
        case 5:
          local_BD = 1;
          task_98C = 0;
          script_2B1B1(1);
          local_BF = 6;
          break;
        case 6:
          if (script_2B2DF(CHARA_AKIR, -115.0f)) {
            local_BF = 7;
          }
          break;
        case 7:
          if (local_D0 >= 44) {
            Script_scenCreateTask(script_2B693, 3, 64, 15, 0);
            local_BF = 8;
          }
          break;
        case 8:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_BB = local_BB + 1;
            } else {
              local_BA = local_BA + 1;
            }
            local_BF = 9;
          }
          break;
        case 9:
          {
            int8_t local_B3;
            local_B3 = 0;
            if (script_2B1DF(0)) {
              if (script_2B360(CHARA_AKIR, -116.6f)) {
                local_B3 = 1;
              }
            } else {
              if (script_2B360(CHARA_AKIR, -119.8f)) {
                script_2B186(12);
              }
              if (script_2B360(CHARA_AKIR, -116.6f)) {
                local_B3 = 1;
              }
            }
            if (local_B3) {
              Script_srchControl((void *)*(&local_D8 + 1));
              local_BF = 10;
            }
            break;
          }
        case 10:
          local_BD = 2;
          script_2B1B1(1);
          *(&local_D8 + 1) = Script_scenCreateTask(script_2E56D, 1, CHARA_TERY);
          local_BF = 11;
          break;
        case 11:
          if (script_2B360(CHARA_TERY, -108.39f)) {
            local_BF = 13;
          }
          break;
        case 13:
          if (local_D0 >= 15) {
            Script_scenCreateTask(script_2B693, 3, 4, 15, 0);
            local_BF = 12;
          }
          break;
        case 12:
          if (script_2B360(CHARA_TERY, -103.51f)) {
            script_2B186(11);
            local_BF = 14;
          }
          break;
        case 14:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_BB = local_BB + 1;
            } else {
              local_BA = local_BA + 1;
            }
            local_BF = 15;
          }
          break;
        case 15:
          {
            int8_t local_B3;
            local_B3 = 0;
            if (script_2B1DF(0)) {
              if (script_2B360(CHARA_AKIR, -102.92f)) {
                local_B3 = 1;
              }
            } else {
              if (script_2B360(CHARA_AKIR, -102.92f)) {
                local_B3 = 1;
              }
            }
            if (local_B3) {
              local_BF = 16;
            }
            break;
          }
        case 16:
          local_BD = 3;
          script_2B1B1(1);
          script_2B1B1(19);
          script_2B1B1(20);
          script_2B1B1(14);
          script_2B1B1(16);
          script_2B1B1(15);
          script_2B1B1(17);
          local_BF = 17;
          break;
        case 17:
          if (script_2B360(CHARA_AKIR, -101.0f)) {
            local_BF = 18;
          }
          break;
        case 18:
          if (local_D0 == 0) {
            Script_scenCreateTask(script_2B693, 3, 64, 15, 0);
            local_BF = 19;
            script_2B186(21);
          }
          break;
        case 19:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_BB = local_BB + 1;
              script_2B186(15);
            } else {
              script_2B1B1(15);
              local_BA = local_BA + 1;
            }
            Script_scenCreateTask(script_2E1CD, 1, CHARA_AKIR);
            local_BF = 20;
          }
          break;
        case 20:
          if (script_2B1DF(19)) {
            if (script_2B1DF(0)) {
              local_BF = 21;
              script_2B1B1(1);
              script_2B1B1(0);
              script_2B186(14);
              script_2B186(15);
              script_2B1B1(17);
            } else {
              script_2B1B1(15);
              local_BF = 23;
            }
          }
          break;
        case 21:
          script_2B640(14);
          if (local_D0 == 0) {
            Script_scenCreateTask(script_2B693, 3, 4, 12, 0);
          }
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              script_2B186(17);
              local_BB = local_BB + 1;
            } else {
              script_2B1B1(17);
              local_BA = local_BA + 1;
            }
            Script_scenCreateTask(script_2E36D, 1, CHARA_AKIR);
            local_BF = 22;
          }
          break;
        case 22:
          if (script_2B1DF(20)) {
            local_BF = 23;
          }
          break;
        case 23:
          {
            int8_t local_B3;
            local_B3 = 0;
            if (script_2B360(CHARA_AKIR, -89.9f)) {
              local_B3 = 1;
            }
            if (local_B3) {
              Script_srchControl((void *)*(&local_D8 + 1));
              local_BF = 24;
              script_2B1B1(21);
            }
            break;
          }
        case 24:
          local_BD = 4;
          script_2B1B1(1);
          *(&local_D8 + 1) = Script_scenCreateTask(script_2E6B7, 1, CHARA_TERY);
          local_BF = 25;
          break;
        case 25:
          if (script_2B360(CHARA_TERY, -73.75f)) {
            script_2B186(7);
            local_BF = 26;
          }
          break;
        case 26:
          if (local_D0 >= 10) {
            Script_scenCreateTask(script_2B693, 3, 0x100, 15, 0);
            local_BF = 27;
          }
          break;
        case 27:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_BB = local_BB + 1;
            } else {
              local_BA = local_BA + 1;
            }
            local_BF = 28;
          }
          break;
        case 28:
          if (script_2B360(CHARA_AKIR, -73.8f)) {
            local_BF = 29;
          }
          break;
        case 29:
          if (script_2B360(CHARA_AKIR, -68.4f)) {
            local_BD = 5;
            script_2B186(2);
            if (local_BA <= (task_994 + 1) / 2 + 3) {
              script_2B186(3);
              local_BF = 30;
            } else {
              script_2B1B1(3);
              local_BF = 31;
            }
          }
          break;
        case 30:
          if (local_D0 >= 20) {
            local_BF = -1;
          }
          break;
        case 31:
          if (local_D0 >= 260) {
            Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
            Script_executeAicaCommand(2720, 2, 20);
            local_BF = 32;
          }
          break;
        case 32:
          if (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
            local_BF = 33;
          }
          break;
        case 33:
          if (local_D0 >= 20) {
            local_BF = -1;
          }
          break;
      }
    }
    if (local_BD != local_BC) {
      local_D4 = 0;
      local_BC = local_BD;
    } else {
      local_D4 = local_D4 + 1;
    }
    {
      uint8_t unused_B8;
      int32_t local_B4;
      local_B4 = task_98C;
      if (local_B4 == 320) {
        if (!(script_2B1DF(0) == 0)) goto L02F0E1;
        L02F0E1:;
        goto L02F0E4;
      }
    }
    L02F0E4:;
    {
      uint8_t unused_B8;
      int32_t local_B4;
      local_B4 = local_BD;
      switch (local_B4) {
        case 1:
          local_B4 = local_D4;
          switch (local_B4) {
            case 0:
              script_2C0F3(CHARA_ONO_);
              script_2C0F3(CHARA_TERY);
              break;
            case 1:
              Script_seqEnterEvtCam(1516, 0, AUTH_None);
              break;
            case 25:
              if (task_9C8 == 0) {
                (void)((task_9C8 = -1) & 2);
                Script_scenCreateTask(script_2FA2E, 0);
              }
              break;
            case 63:
              Script_seqEnterEvtCam(1503, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              break;
            case 68:
              if (script_2B1DF(0)) {
                Script_seqEnterEvtCam(1012, 0, AUTH_None);
                Script_scenCreateTask(script_2AECE, 0);
              }
              break;
            case 85:
              if (script_2B1DF(0) == 0) {
                Script_executeAicaCommand(1193, 0, 0);
              }
              break;
            case 90:
              if (script_2B1DF(0) == 0) {
                Script_seqEnterEvtCam(1015, 0, AUTH_None);
              }
              break;
            case 100:
              if (script_2B1DF(0) == 1) {
                Script_seqEnterEvtCam(1509, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
            case 110:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2AEDA, 0);
              }
              break;
            case 115:
              if (script_2B1DF(0)) {
                Script_scenCreateTask(script_2FA57, 0);
              } else {
                Script_executeAicaCommand(66729, 0, 0);
              }
              break;
            case 130:
              if (script_2B1DF(0) == 0) {
                Script_seqEnterEvtCam(1514, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
            case 135:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2AEF2, 0);
              }
              break;
            case 150:
              if (!(script_2B1DF(0) == 0)) goto L02F2F6;
              L02F2F6:;
              break;
          }
          break;
        case 2:
          local_B4 = local_D4;
          switch (local_B4) {
            case 5:
              Script_seqEnterEvtCam(1474, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
              (void)((task_9C9 = 0) & 2);
              break;
            case 12:
              Script_scenCreateTask(script_2FA51, 0);
              break;
            case 24:
              Script_scenCreateTask(script_2FA57, 0);
              break;
            case 37:
              Script_scenCreateTask(script_2AEFE, 0);
              break;
            case 38:
              Script_executeAicaCommand(132265, 0, 0);
              break;
            case 50:
              Script_executeAicaCommand(197801, 0, 0);
              break;
            case 52:
              if (script_2B1DF(0)) {
                Script_scenCreateTask(script_2AECE, 0);
              }
              break;
            case 58:
              if (script_2B1DF(0) == 0) {
                script_2C0F3(CHARA_TONY);
                script_2C107(SHEN_NAME('BIKE'));
                script_2C107(CHARA_ONO_);
                Script_seqEnterEvtCam(1517, 0, AUTH_None);
              }
              break;
            case 59:
              Script_executeAicaCommand(263337, 0, 0);
              break;
            case 65:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2AF16, 0);
              }
              break;
            case 70:
              if (script_2B1DF(0) == 1) {
                local_B9 = 1;
              } else {
                local_B9 = 0;
              }
              break;
            case 84:
              if (script_2B1DF(0)) {
                script_2C0F3(CHARA_TONY);
                script_2C107(SHEN_NAME('BIKE'));
                script_2C107(CHARA_ONO_);
                Script_seqEnterEvtCam(1507, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                Script_scenCreateTask(script_2FA54, 0);
              }
              break;
            case 94:
              if (script_2B1DF(0) == 0) {
                Script_seqEnterEvtCam(1507, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                Script_scenCreateTask(script_2FA54, 0);
              }
              break;
          }
          break;
        case 3:
          local_B4 = local_D4;
          switch (local_B4) {
            case 65:
              if (script_2B1DF(0)) {
                Script_seqEnterEvtCam(1473, 0, AUTH_None);
              }
              break;
            case 27:
              if (script_2B1DF(0) == 1) {
                Script_seqEnterEvtCam(1035, 0, AUTH_None);
              }
              break;
            case 90:
              if (script_2B1DF(15) == 1) {
                if (script_2B1DF(0) == 0) {
                  Script_seqEnterEvtCam(1036, 0, AUTH_None);
                } else {
                  Script_seqEnterEvtCam(1034, CHARA_AKIR, CHARA_AKIR);
                }
              }
              break;
            case 70:
              if (script_2B1DF(15) == 0) {
                Script_seqEnterEvtCam(1509, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
            case 93:
              if (script_2B1DF(17) == 0) {
                Script_seqEnterEvtCam(1525, 0, AUTH_None);
              }
              break;
            case 31:
              if (script_2B1DF(0)) {
                Script_scenCreateTask(script_2AECE, 0);
              }
              break;
            case 36:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2B156, 0);
              }
              break;
            case 71:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2B162, 0);
              }
              break;
          }
          break;
        case 4:
          local_B4 = local_D4;
          switch (local_B4) {
            case 0:
              Script_seqEnterEvtCam(1519, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              Script_srchControl((void *)local_C0);
              script_2C107(CHARA_TONY);
              script_2C107(CHARA_HOND);
              script_2C107(SHEN_NAME('D201'));
              script_2C107(CHARA_TERY);
              break;
            case 2:
              Script_scenCreateTask(script_2AF22, 0);
              break;
            case 35:
              Script_scenCreateTask(script_2AF4A, 0);
              break;
            case 45:
              Script_executeAicaCommand(722089, 0, 0);
              break;
            case 47:
              Script_executeAicaCommand(722089, 0, 0);
              break;
            case 58:
              if (script_2B1DF(0)) {
                Script_scenCreateTask(script_2AECE, 0);
              }
              break;
            case 60:
              if (script_2B1DF(0) == 0) {
                Script_seqEnterEvtCam(1041, 0, AUTH_None);
              } else {
                Script_seqEnterEvtCam(1522, 0, AUTH_None);
              }
              break;
            case 66:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2AF16, 0);
              }
              break;
            case 76:
              Script_scenCreateTask(script_2AF3E, 0);
              break;
            case 75:
              if (!script_2B1DF(0)) goto L02F7FB;
              L02F7FB:;
              break;
            case 91:
              Script_scenCreateTask(script_2AF62, 0);
              break;
            case 90:
              local_C4 = script_2C303("DR12_001", -68.2295f, 0.08239f, 53.425629f, 13034);
              if (script_2B1DF(0) == 0) {
                Script_seqEnterEvtCam(1520, 0, AUTH_None);
              }
              break;
            case 113:
              Script_SetMapControlTaskFlag(local_C4, 1);
              Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
              if (script_2B1DF(0)) {
                Script_seqEnterEvtCam(1524, 0, AUTH_None);
              }
              break;
            case 115:
              if (!(script_2B1DF(0) == 0)) goto L02F8B0;
              L02F8B0:;
              break;
          }
          break;
        case 5:
          local_B4 = local_D4;
          switch (local_B4) {
            case 15:
              if (!(script_2B1DF(3) == 0)) goto L02F8DF;
              L02F8DF:;
              break;
            case 100:
              if (script_2B1DF(3) == 0) {
                Script_seqEnterEvtCam(1050, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                Script_scenCreateTask(script_2AE6A, 2, CHARA_AKIR, 60);
              }
              break;
            case 101:
              if (script_2B1DF(3) == 0) {
                Script_scenCreateTask(script_2AF82, 0);
              }
              break;
          }
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
  local_CC = 0;
  local_C8 = 0;
  while (local_CC < 10) {
    if (*(&local_D8 + local_CC) == 0) {
    } else {
      Script_srchControl((void *)*(&local_D8 + local_CC));
      local_C8 = local_C8 + 1;
    }
    *(&local_D8 + local_CC) = 0;
    local_CC = local_CC + 1;
  }
  script_2B186(4);
  end:;
}

static int32_t script_2F9C7(void) {
  script_2B186(4);
  script_2B186(3);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2F9E7(void) {
  script_2B186(4);
  script_2B186(3);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2FA07(float arg0, float arg1, float arg2, float arg3) {
  float local_FC;
  float local_F8;
  int32_t local_F4;
  local_FC = arg2 - arg0;
  local_F8 = arg3 - arg1;
  local_F4 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, local_FC, local_F8);
  return local_F4;
}

static int32_t script_2FA2E(void) {
  int32_t local_FC;
  local_FC = 25;
  while (local_FC) {
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC - 1;
  }
  end:;
}

static int32_t script_2FA51(void) {
  end:;
}

static int32_t script_2FA54(void) {
  end:;
}

static int32_t script_2FA57(void) {
  int8_t local_FF;
  local_FF = 50;
  while (local_FF) {
    if (task_9C9 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
    local_FF = local_FF - 1;
  }
  if (local_FF) {
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
  }
  end:;
}

static int32_t script_2FA9A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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

static void script_2FAE2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_2FA9A, 6, arg0, arg2, arg3, arg1, arg4, arg5);
  return;
}

static int32_t script_2FAFC(int32_t arg0, float arg1, int32_t arg2) {
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= arg1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(arg2, 0, 0);
  end:;
}

static void script_2FB22(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_2FAFC, 3, arg0, arg1, arg2);
  return;
}

static void script_2FB36(void) {
  script_2C107(CHARA_AKSK);
  script_2C107(CHARA_TURU);
  script_2C107(SHEN_NAME('CYCL'));
  script_2C107(SHEN_NAME('D201'));
  script_2C107(SHEN_NAME('BIKE'));
  return;
}

static void script_2FB75(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case -1:
      break;
    case 0:
      script_2C107(CHARA_TURU);
      script_2C107(SHEN_NAME('BICY'));
      break;
    case 1:
      script_2C107(CHARA_AKSK);
      script_2C107(CHARA_MISM);
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
  }
  return;
}

static int32_t script_2FC02(void) {
  script_2C0F3(CHARA_TURU);
  script_2C0F3(SHEN_NAME('CYCL'));
  Script_PlayCharaMotion(CHARA_TURU, 8234, 0.0f, 127.0f, 1.0f, 1.0f, 0);
  Script_vehiUtils(VehicleCommand_AttachRiderToVehicle, CHARA_TURU, 0x4C435943, 0);
  Script_vehiUtils(VehicleCommand_Unknown8_SetFlag, CHARA_TURU, 0);
  Script_vehiUtils(VehicleCommand_StartPathFollow, CHARA_TURU, &data_45988, 3, 1);
  Script_vehiUtils(VehicleCommand_SetMaxSpeed, CHARA_TURU, 18.0f);
  Script_vehiUtils(VehicleCommand_SetSpeedMotion, CHARA_TURU, 0, 8234);
  Script_vehiUtils(VehicleCommand_SetSpeedMotion, CHARA_TURU, 1, 8235);
  script_2B640(10);
  Script_vehiUtils(VehicleCommand_SetControlProfile, CHARA_TURU, 2, 0.0f);
  script_2B640(11);
  return Script_vehiUtils(VehicleCommand_SetControlProfile, CHARA_TURU, 3, 0.94f);
}

static int32_t script_2FCDA(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t unused_D0[3];
  Vector3i local_C4;
  int32_t unused_B8[3];
  int32_t local_B4;
  script_2C0F3(arg0);
  Script_MoveObjectToObject(arg0, 0x43474D59);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  local_C4.x = 0;
  (&local_C4)->y = 0;
  (&local_C4)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
  script_2B640(13);
  local_B4 = Script_scenCreateTask(script_2C0A6, 1, arg0);
  while (1) {
    if (script_2B1DF(12)) {
      break;
    }
    Script_EVT_GetPosition(CHARA_YMGC, 18, &local_E8, 0);
    Script_EVT_GetPosition(CHARA_YMGC, 12, &local_DC, 0);
    local_C4.x = 0;
    (&local_C4)->y = 0;
    (&local_C4)->z = 0;
    if ((&local_C4)->z < -0x8000) {
      (&local_C4)->z = (&local_C4)->z + 0x10000;
    }
    Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
    local_F4.x = 0.0f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.0f;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2FDF6(int32_t arg0) {
  Vector3 local_F4;
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L02FE1A;
    }
  }
  L02FE1A:;
  Script_scenToggleVisibleFor(arg0, 1);
  script_2C107(arg0);
  Script_charaToggleMotEnabled(arg0, 1);
  {
    Vector3 local_E8;
    local_E8.x = -13.8478f;
    (&local_E8)->z = 30.5f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E8);
  }
  {
    Vector3i local_E8;
    (&local_E8)->y = 40049;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
  }
  script_2C107(arg0);
  script_2B640(13);
  script_2C0F3(arg0);
  Script_PlayCharaMotion(arg0, 8232, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  (void)((task_9CC = -1) & 2);
  while (task_9CC & 0x40) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = 1.6f;
    Script_SetLookPointDetails(arg0, 6, &local_F4, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
  end:;
}

static int32_t script_2FEFC(int32_t arg0) {
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L02FF20;
    }
  }
  L02FF20:;
  Script_scenToggleVisibleFor(arg0, 1);
  script_2C0F3(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_clmdSetFlags(arg0, 2, 0);
  {
    int32_t unused_F0[4];
    Vector3 local_E4;
    local_E4.x = -23.05f;
    (&local_E4)->z = 38.9f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 0.0f;
    (&local_F4)->y = 1.75f;
    (&local_F4)->z = 0.0f;
    Script_SetCharaLookAtPosition(arg0, 6, &local_F4, 0);
  }
  script_2B4BA(arg0, -24.57f, 40.05f, 0x10000, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B640(6);
  script_2C0F3(arg0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B640(1);
  if (script_2B1DF(0) == 0) {
    Vector3 local_F4;
    int32_t local_F0;
    (void)((task_9CA = -1) & 1);
    script_2C200(arg0, 5, -24.18627f, 39.859962f);
    Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
    Script_charaToggleMotEnabled(arg0, 1);
    Script_PlayCharaMotion(arg0, 8231, 6.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
    script_2B2BB(arg0, 61.0f);
    {
      Vector3i local_E4;
      (&local_E4)->y = 20261;
      Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E4, 0);
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.78f);
    local_F0 = 0;
    while (local_F0 >= 200) {
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
      (&local_F4)->y = 1.65f;
      Script_SetCharaLookAtPosition(arg0, 6, &local_F4, 2);
      SCNF_WaitFrames_0000(1);
      {
        local_F0 = local_F0 + 1;
      }
    }
  } else {
    (void)((task_9CA = 0) & 2);
    SCNF_WaitFrames_0000(14);
    Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
    Script_PlayCharaMotion(arg0, 8224, 60.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      Script_RotateNodeUtils(arg0, &data_4522C, 0);
      SCNF_WaitFrames_0000(1);
    }
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  }
  end:;
}

static int32_t script_3017F(int32_t arg0) {
  script_2C0F3(arg0);
  {
    Vector3 local_F4;
    local_F4.x = -35.27f;
    (&local_F4)->z = 42.4f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 18204;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B640(7);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B803(arg0, CHARA_TERY, 0.9f);
  script_2B186(16);
  Script_PlayCharaMotion(arg0, 8211, 28.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B29F(arg0);
  Script_PlayCharaMotion(arg0, 8212, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  end:;
}

static int32_t script_30277(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  {
    Vector3 local_F4;
    local_F4.x = -19.867701f;
    (&local_F4)->z = 36.049702f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 26958;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  script_2C107(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B640(24);
  script_2C0F3(arg0);
  {
    Vector3 local_F4;
    local_F4.x = -19.867701f;
    (&local_F4)->z = 36.049702f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 26958;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
  script_2B640(17);
  if (script_2B1DF(19)) {
    script_2B640(17);
    script_2B640(1);
    if (script_2B1DF(0)) {
      Script_PlayCharaMotion(arg0, 8220, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      script_2B29F(arg0);
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(arg0, 1.1f);
    } else {
      Script_charaToggleMotEnabled(arg0, 0);
      Script_PlayCharaMotion(arg0, 8216, 10.0f, -1.0f, -1.0f, 1.0f, 0);
      for (;;) {
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
        Script_RotateNodeUtils(arg0, &data_4522C, 0);
        SCNF_WaitFrames_0000(1);
      }
      Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      script_2B29F(arg0);
    }
  }
  script_2B1B1(24);
  end:;
}

static int32_t script_3045D(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = -21.0553f;
    (&local_F4)->z = 37.317299f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 6671;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  script_2C107(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(arg0, 8241, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B640(24);
  script_2C0F3(arg0);
  script_2B640(25);
  if (script_2B1DF(0)) {
    Script_ShowUIMessageFromID("E1004AO001");
  } else {
    Script_PlayCharaMotion(arg0, 8217, 2.1019477e-44f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(arg0, 0);
    script_2B2BB(arg0, 30.0f);
    script_2B29F(arg0);
  }
  script_2B1B1(25);
  end:;
}

static void script_30559(int32_t arg0) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  script_2C107(arg0);
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKSK, -1, &local_F4, 0);
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_459B4, 5, data_459E0, 6, data_459EB)))
static int32_t script_30594(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  local_F0 = script_2C303("DR15_011", -56.760239f, 0.0724f, 44.021336f, 6098);
  {
    Vector3 local_E4;
    local_E4.x = -68.4f;
    (&local_E4)->z = 60.19f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E4);
  }
  script_2B4BA(arg0, -62.38f, 53.68f, 0x10000, 0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_18);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B640(9);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B4E5(arg0, -62.38f, 53.68f, 2730, 0);
  script_2B4BA(arg0, -46.66f, 42.91f, 910, 0);
  script_2B31A(arg0, -54.97f);
  Script_SetMapControlTaskFlag(local_F0, 1);
  script_2B640(1);
  if (script_2B1DF(0)) {
    script_2B31A(arg0, -54.03f);
    Script_executeAicaCommand(0x1F04A9, 0, 0);
    script_2B57D(arg0, -52.5f, 45.17f, 1.4f, 1820, 1);
    script_2B4BA(arg0, -46.66f, 42.91f, 728, 0);
  } else {
    script_2B31A(arg0, -54.97f);
    Script_PlayCharaMotion(arg0, 8206, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(0x1804A9, 0, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    script_2B2BB(arg0, 36.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    script_2B57D(arg0, -52.45f, 45.76f, 1.8f, 1820, 1);
    script_2B4BA(arg0, -46.66f, 42.91f, 910, 0);
  }
  script_2B31A(arg0, -46.66f);
  script_2B4BA(arg0, -42.83f, 42.77f, 728, 0);
  script_2C107(CHARA_TURU);
  script_2C107(SHEN_NAME('BICY'));
  script_2B640(1);
  if (script_2B1DF(0)) {
    script_2B31A(arg0, -37.67f);
    Script_PlayCharaMotion(CHARA_AKSK, 8215, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(0x1104A9, 0, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKSK, 2.0f);
    script_2B57D(arg0, -37.27f, 42.3f, 1.4f, 910, 1);
    script_2B4BA(arg0, -25.92f, 40.36f, 910, 0);
  } else {
    Vector3i local_E4;
    script_2B31A(arg0, -37.83f);
    (&local_E4)->y = 7281;
    Script_EVT_SetRotation(CHARA_AKSK, -0x70000000, &local_E4, 0);
    Script_charaToggleMotEnabled(CHARA_AKSK, 1);
    Script_PlayCharaMotion(CHARA_AKSK, 8218, 65.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(arg0, 8195, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    Script_ccowSetHeightMode(CHARA_AKSK, CCOW_KeepHeight);
    Script_EVT_GetPosition(CHARA_AKSK, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = (&local_F4)->y + 0.05f;
    Script_EVT_SetPosition(CHARA_AKSK, 0x78000000, &local_F4);
    script_2B2BB(CHARA_AKSK, 122.0f);
    Script_scenCreateTask(script_2B14A, 0);
    script_2B29F(arg0);
    Script_ccowSetHeightMode(CHARA_AKSK, CCOW_FollowGround);
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, 8.0f, 0.8f, 0);
    Script_scenCreateTask(script_32EEA, 2, CHARA_AKIR, 0x3F4CCCCD);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    while (1) {
      if (script_2B4BA(arg0, -25.92f, 40.36f, 1820, 1)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_2B31A(arg0, -27.92f);
  script_2B640(1);
  if (script_2B1DF(0)) {
    script_2B31A(arg0, -27.62f);
    script_2B57D(arg0, -24.64f, 41.82f, 0.9f, 1820, 0);
    Script_executeAicaCommand(0x1104A9, 0, 0);
    script_2B186(19);
    script_2B4BA(arg0, -8.7349f, 30.3598f, 910, 1);
  } else {
    script_2B31A(arg0, -27.82f);
    Script_PlayCharaMotion(arg0, 8194, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(0x1804A9, 0, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
    script_2B29F(arg0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    Script_SetCharaMotionPlaybackRate(arg0, 0.7f);
  }
  (void)((task_9CB = 0) & 0x10);
  if (script_2B1DF(19)) {
    script_2B640(17);
    script_2B640(1);
    if (script_2B1DF(0)) {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
      Script_PlayCharaMotion(arg0, 8204, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
      Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
      Script_executeAicaCommand(0x1104A9, 0, 0);
      script_2B186(20);
    } else {
      script_2B4BA(arg0, -8.7349f, 30.3598f, 910, 1);
      Script_PlayCharaMotion(arg0, 8202, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
      script_2B2BB(arg0, 74.0f);
      Script_scenCreateTask(script_2B012, 0);
      script_2B29F(arg0);
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    }
  } else {
    if (script_2B1DF(19) == 0) {
      (void)((task_9CB = -1) & 0x10);
      {
        Vector3 local_E4;
        local_E4.x = -24.6513f;
        (&local_E4)->z = 40.043301f;
        Script_EVT_SetPosition(arg0, 0x28000000, &local_E4);
      }
      {
        Vector3i local_E4;
        (&local_E4)->y = 55655;
        Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E4, 0);
      }
      Script_mapmSetCamFlags(0, 0);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
      Script_seqEnterEvtCam(1503, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
      script_2B640(23);
      script_2B640(1);
      if (script_2B1DF(0)) {
        Vector3i local_E4;
        Script_PlayCharaMotion(arg0, 8207, 9.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        Script_executeAicaCommand(0x1104A9, 0, 0);
        script_2B2BB(arg0, 31.0f);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        (&local_E4)->y = 55655;
        Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E4, 0);
      } else {
        Vector3i local_E4;
        (&local_E4)->y = 52925;
        Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E4, 0);
        Script_ShowUIMessageFromID("E1004AA007");
        Script_PlayCharaMotion(arg0, 8203, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        SCNF_WaitFrames_0000(3);
        Script_executeAicaCommand(0x2004A9, 0, 0);
        SCNF_WaitFrames_0000(17);
        Script_ShowUIMessageFromID("E1004AO003");
        SCNF_WaitFrames_0000(10);
        Script_ShowUIMessageFromID("E1004AA026");
        script_2B29F(arg0);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        script_2B4BA(arg0, -8.7349f, 30.3598f, 910, 1);
      }
      goto L030E08;
    }
  }
  L030E08:;
  if (script_2B1DF(19)) {
    script_2FB36();
    if (script_2B1DF(20)) {
      script_2B640(18);
      script_2B640(1);
      if (script_2B1DF(0)) {
        SCNF_WaitFrames_0000(8);
        Script_ShowUIMessageFromID("E1004AA036");
        Script_executeAicaCommand(0x1F04A9, 0, 0);
        Script_mapmSetCamFlags(0, 0);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
        script_2B29F(arg0);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      } else {
        Script_PlayCharaMotion(arg0, 8203, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        SCNF_WaitFrames_0000(10);
        Script_ShowUIMessageFromID("E1004AA037");
        SCNF_WaitFrames_0000(10);
        Script_ShowUIMessageFromID("E1004AO002");
        script_2B29F(arg0);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
      }
    }
    script_2B4BA(arg0, -8.7349f, 30.3598f, 910, 1);
  }
  script_2B31A(arg0, -20.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
  Script_mapmSetCamFlags(0, 1);
  Script_seqEnterEvtCam(1503, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
  script_2C107(CHARA_KURI);
  script_2C107(CHARA_MRUA);
  script_2C107(CHARA_NORK);
  script_2C107(CHARA_SUMI);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  script_2B640(12);
  script_2B31A(arg0, -16.83f);
  script_2B640(2);
  if (script_2B1DF(0)) {
    script_2B31A(arg0, -13.83f);
    Script_executeAicaCommand(0x1604A9, 0, 0);
    Script_PlayCharaMotion(arg0, 8233, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
    script_2B2BB(arg0, 36.0f);
    Script_executeAicaCommand(0x1704A9, 0, 0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
  } else {
    script_2B31A(arg0, -12.83f);
    Script_PlayCharaMotion(arg0, 8205, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_2AF16, 0);
    script_2FB22(arg0, 0x41A00000, 0x1504A9);
    script_2B2BB(arg0, 60.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  script_2B31A(arg0, -9.49f);
  script_2B4BA(arg0, 23.37f, 28.79f, 1820, 0);
  script_2B31A(arg0, -4.91f);
  if (script_2B1DF(3)) {
    script_2B640(15);
    script_2B4BA(arg0, 21.52f, 31.27f, 546, 0);
    script_2B31A(arg0, 21.57f);
    script_2B4E5(arg0, 22.32f, 34.49f, 2184, 0);
    script_2B84A(arg0);
  } else {
    (void)((task_9D0 = -1) & 0x20);
  }
  end:;
}

static int32_t script_31192(int32_t arg0) {
  Vector3 local_F4;
  script_2C0F3(arg0);
  local_F4.x = -65.04f;
  (&local_F4)->z = 56.79f;
  Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2B4E5(arg0, -47.22f, 41.47f, 0x10000, 0);
  script_2B4E5(arg0, -24.34f, 42.29f, 1820, 0);
  script_2B4E5(arg0, -10.36f, 30.31f, 910, 1);
  script_2B640(2);
  if (script_2B1DF(0)) {
    script_2B4E5(arg0, 21.01f, 30.53f, 910, 0);
  }
  script_2C107(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  end:;
}

static int32_t script_3126E(int32_t arg0) {
  script_2B186(7);
  {
    Vector3 local_F4;
    local_F4.x = -41.3f;
    (&local_F4)->z = 41.8f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 47331;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  script_2B4E5(arg0, -24.34f, 42.29f, 0x10000, 0);
  script_2B4E5(arg0, -10.36f, 30.31f, 910, 1);
  script_2B4E5(arg0, 21.01f, 30.53f, 910, 0);
  script_2C107(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  end:;
}

static int32_t script_31312(int32_t arg0) {
  Vector3 local_F4;
  script_2C107(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  script_2B640(2);
  if (script_2B1DF(3)) {
    script_2C0F3(arg0);
    local_F4.x = 4.58f;
    (&local_F4)->z = 30.36f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
    Script_charaToggleMotEnabled(arg0, 1);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_2B4E5(arg0, 21.01f, 30.53f, 0x10000, 0);
    script_2B4BA(arg0, 22.47f, 32.78f, 1820, 0);
  }
  SCNF_WaitFrames_0000(10);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(20);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  end:;
}

__attribute__((scn_string_bindings(0, data_459F6, 1, data_45A02, 3, data_45A19, 4, data_45A25)))
static int32_t script_3140B(void) {
  int32_t unused_D4[11];
  int32_t local_D0;
  int32_t local_CC;
  int32_t local_C8;
  int32_t local_C4;
  int32_t local_C0;
  int8_t local_BF;
  int8_t local_BE;
  int8_t local_BD;
  int8_t local_BC;
  int8_t local_BB;
  int8_t local_BA;
  int8_t local_B9;
  uint8_t unused_AC[13];
  Vector3 local_A0;
  Vector3i local_94;
  float local_90;
  float local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t local_80;
  (void)((task_9D0 = 0) & 1);
  local_BD = local_80 = local_88 = 0;
  local_90 = Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
  local_8C = Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
  local_C4 = 0;
  while (local_C4 < 12) {
    *(&local_D0 + local_C4) = 0;
    local_C4 = local_C4 + 1;
  }
  (void)((task_99F = -1) & 2);
  Script_scenCreateTask(script_32F23, 0);
  Script_FACE(CHARA_TERY, 5, 1, 10);
  local_D0 = Script_scenCreateTask(script_30594, 1, CHARA_AKIR);
  *(&local_D0 + 1) = Script_scenCreateTask(script_31192, 1, CHARA_TERY);
  *(&local_D0 + 2) = Script_scenCreateTask(script_2FEFC, 1, CHARA_NORK);
  *(&local_D0 + 3) = Script_scenCreateTask(script_3017F, 1, CHARA_AKSK);
  *(&local_D0 + 4) = Script_scenCreateTask(script_2FDF6, 1, CHARA_YMGC);
  *(&local_D0 + 5) = Script_scenCreateTask(script_2FCDA, 1, SHEN_NAME('ABBX'));
  *(&local_D0 + 6) = Script_scenCreateTask(script_2FC02, 0);
  *(&local_D0 + 7) = Script_scenCreateTask(script_2B8B1, 2, SHEN_NAME('PIL4'), CHARA_AKSK);
  *(&local_D0 + 8) = Script_scenCreateTask(script_2B8B1, 2, SHEN_NAME('PIL0'), CHARA_AKIR);
  *(&local_D0 + 9) = Script_scenCreateTask(script_2B8B1, 2, SHEN_NAME('PIL1'), CHARA_TERY);
  *(&local_D0 + 10) = Script_scenCreateTask(script_30277, 1, CHARA_KURI);
  *(&local_D0 + 11) = Script_scenCreateTask(script_3045D, 1, CHARA_MRUA);
  script_2B990(2);
  local_BF = 0;
  local_BE = -1;
  local_BC = 0;
  local_BB = -1;
  local_B9 = 0;
  script_2B1B1(24);
  script_2B1B1(27);
  script_2B1B1(28);
  script_2B1B1(29);
  script_2B1B1(3);
  script_2C0F3(CHARA_YOSE);
  script_2C107(SHEN_NAME('D201'));
  script_2C107(SHEN_NAME('BIKE'));
  while (local_BF != -1) {
    if (task_9D0 & 0x80) {
      break;
    }
    if (local_BF != local_BE) {
      local_C8 = 0;
      local_BE = local_BF;
    } else {
      local_C8 = local_C8 + 1;
    }
    if (script_2B1DF(28)) {
      local_88 = local_88 + 1;
    }
    if (script_2B1DF(29)) {
      local_84 = local_84 + 1;
    }
    {
      int32_t unused_FF7C;
      int32_t local_FF78;
      local_FF78 = local_BF;
      switch (local_FF78) {
        case 0:
          if (local_C8 >= 30) {
            script_2B186(9);
            local_BF = 1;
          }
          break;
        case 1:
          local_BC = 1;
          script_2B1B1(1);
          script_2B186(10);
          local_BF = 2;
          break;
        case 2:
          if (local_C8 >= 45) {
            Script_scenCreateTask(script_2B693, 3, 64, 15, 0);
            local_BF = 3;
          }
          break;
        case 3:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_BD = local_BD + 1;
            } else {
              script_2B186(11);
            }
            local_BF = 4;
          }
          break;
        case 4:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_2B1DF(0)) {
              if (script_2B360(CHARA_AKIR, -46.79f)) {
                local_FF77 = 1;
              }
            } else {
              if (script_2B360(CHARA_AKIR, -46.79f)) {
                local_FF77 = 1;
              }
            }
            if (local_B9 == 0) {
              if (script_2B360(CHARA_AKIR, -50.1f)) {
                Script_srchControl((void *)*(&local_D0 + 1));
                *(&local_D0 + 1) = Script_scenCreateTask(script_3126E, 1, CHARA_TERY);
                local_B9 = 1;
              }
            }
            if (local_FF77) {
              local_BF = 5;
            }
            break;
          }
        case 5:
          local_BC = 2;
          script_2B1B1(1);
          local_BF = 6;
          break;
        case 6:
          if (local_C8 >= 15) {
            Script_scenCreateTask(script_2B693, 3, 64, 15, 0);
            local_BF = 7;
          }
          break;
        case 7:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_BD = local_BD + 1;
            }
            local_BF = 8;
          }
          break;
        case 8:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_2B1DF(0)) {
              if (script_2B360(CHARA_AKIR, -37.5f)) {
                local_FF77 = 1;
              }
            } else {
              if (script_2B360(CHARA_AKIR, -35.16f)) {
                local_FF77 = 1;
              }
            }
            if (local_FF77) {
              local_BF = 9;
            }
            break;
          }
        case 9:
          local_BC = 3;
          script_2B1B1(1);
          script_2B1B1(19);
          script_2B186(6);
          script_2B186(24);
          script_2B186(28);
          local_BF = 10;
          local_88 = 0;
          break;
        case 10:
          if (local_C8 >= 20) {
            Script_scenCreateTask(script_2B693, 3, 4, 15, 0);
            local_BF = 11;
          }
          break;
        case 11:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_BD = local_BD + 1;
            }
            local_BF = 12;
          }
          break;
        case 12:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_2B1DF(0)) {
              if (script_2B360(CHARA_AKIR, -27.8f)) {
                local_FF77 = 1;
              }
            } else {
              if (script_2B360(CHARA_AKIR, -25.0f)) {
                local_FF77 = 1;
              }
            }
            if (local_FF77) {
              if (script_2B1DF(0)) {
                local_BF = 13;
                script_2B186(19);
              } else {
                local_BF = 21;
                script_2B1B1(19);
              }
            }
            break;
          }
        case 13:
          local_BC = 4;
          script_2B1B1(1);
          script_2B186(17);
          local_BF = 14;
          break;
        case 14:
          if (local_C8 >= 0) {
            Script_scenCreateTask(script_2B693, 3, 64, 15, 0);
            local_BF = 15;
          }
          break;
        case 15:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_BD = local_BD + 1;
            }
            local_BF = 16;
          }
          break;
        case 16:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_2B1DF(0)) {
              if (script_2B360(CHARA_AKIR, -25.5f)) {
                local_FF77 = 1;
              }
            } else {
              if (script_2B360(CHARA_AKIR, -20.6f)) {
                local_FF77 = 1;
              }
            }
            if (local_FF77) {
              if (script_2B1DF(0)) {
                local_BF = 17;
                script_2B186(20);
              } else {
                local_BF = 25;
                script_2B1B1(20);
              }
            }
            break;
          }
        case 17:
          local_BC = 5;
          script_2B1B1(1);
          script_2B1B1(21);
          script_2B1B1(22);
          script_2B1B1(25);
          script_2B1B1(26);
          script_2B186(18);
          local_BF = 18;
          break;
        case 18:
          if (local_C8 >= 10) {
            Script_scenCreateTask(script_2B693, 3, 4, 15, 0);
            local_BF = 19;
          }
          break;
        case 19:
          if (script_2B1DF(1)) {
            script_2B186(25);
            if (script_2B1DF(0)) {
              script_2B186(26);
              local_BD = local_BD + 1;
            }
            local_BF = 20;
          }
          break;
        case 20:
          script_2B186(21);
          if (script_2B1DF(0)) {
            script_2B186(22);
          }
          local_BF = 25;
          break;
        case 21:
          local_BC = 6;
          script_2B1B1(1);
          script_2B1B1(25);
          script_2B1B1(26);
          script_2B186(23);
          local_BF = 22;
          break;
        case 22:
          if (local_C8 >= 0) {
            Script_scenCreateTask(script_2B693, 3, 4, 15, 0);
            local_BF = 23;
          }
          break;
        case 23:
          if (script_2B1DF(1)) {
            script_2B186(25);
            if (script_2B1DF(0)) {
              local_BD = local_BD + 1;
              script_2B186(26);
            }
            local_BF = 24;
          }
          break;
        case 24:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_2B1DF(0)) {
              local_FF77 = 1;
            } else {
              local_FF77 = 1;
            }
            if (local_FF77) {
              local_BF = 25;
            }
            break;
          }
        case 25:
          local_BC = 7;
          script_2B1B1(1);
          script_2B186(13);
          local_BF = 27;
          break;
        case 27:
          if (script_2B360(CHARA_AKIR, -17.5f)) {
            Script_scenCreateTask(script_2B693, 3, 0x100, 15, 0);
            local_BF = 26;
            script_2B186(27);
            script_2B186(29);
            local_84 = 0;
          }
          if (script_2B360(CHARA_AKIR, -20.0f)) {
            script_2B186(13);
          }
          break;
        case 26:
          script_2B186(12);
          local_BF = 28;
          break;
        case 28:
          if (script_2B1DF(1)) {
            if (script_2B1DF(0)) {
              local_BD = local_BD + 1;
            }
            if (local_BD >= 3 - task_994 / 2) {
              script_2B186(3);
            } else {
              script_2B1B1(3);
            }
            script_2B186(2);
            local_BF = 29;
          }
          break;
        case 29:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_2B1DF(0)) {
              if (script_2B360(CHARA_AKIR, -9.5f)) {
                local_FF77 = 1;
              }
            } else {
              if (script_2B360(CHARA_AKIR, -9.5f)) {
                local_FF77 = 1;
              }
            }
            if (local_FF77) {
              Script_srchControl((void *)*(&local_D0 + 1));
              local_BF = 30;
            }
            break;
          }
        case 30:
          local_BC = 8;
          *(&local_D0 + 1) = Script_scenCreateTask(script_31312, 1, CHARA_TERY);
          local_BF = 31;
          break;
        case 31:
          if (script_2B1DF(3)) {
            local_BF = 32;
          } else {
            local_BF = 33;
          }
          break;
        case 32:
          if (script_2B360(CHARA_TERY, 22.53f)) {
            script_2B186(15);
            local_BF = 33;
          }
          break;
        case 33:
          if (script_2B1DF(3)) {
            if (script_2B360(CHARA_AKIR, 20.77f)) {
              local_BF = 34;
            }
          } else {
            if (script_2B360(CHARA_AKIR, -5.91f)) {
              local_BF = 34;
            }
          }
          break;
        case 34:
          {
            int32_t local_FF74;
            if (script_2B1DF(3)) {
              local_FF74 = local_C8 + 1;
            } else {
              local_FF74 = 160;
            }
            if (local_C8 >= local_FF74) {
              Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
              if (script_2B1DF(3)) {
                Script_executeAicaCommand(2720, 2, 30);
              } else {
                Script_executeAicaCommand(2720, 2, 15);
              }
              local_BF = 35;
            }
            break;
          }
        case 35:
          if (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
            Script_filmCheckOrSetTransitionUtil(100, 255, 0, 0, 0, 255, 0, 0, 0);
            script_2B186(15);
            local_BF = -1;
          }
          break;
      }
    }
    {
      int32_t unused_FF7C;
      int32_t local_FF78;
      local_FF78 = local_88;
      switch (local_FF78) {
        case 80:
          script_2B186(13);
          break;
        case 150:
          script_2B186(12);
          break;
        case 153:
          if (task_9CA == 0) {
            Script_executeAicaCommand(0x2104A9, 0, 0);
          }
          break;
        case 156:
          if (task_9CA == 0) {
            Script_executeAicaCommand(0x2204A9, 0, 0);
          }
          break;
        case 160:
          if (task_9CA == 0) {
            Script_executeAicaCommand(0x2304A9, 0, 0);
          }
          break;
      }
    }
    if (local_BC != local_BB) {
      local_CC = 0;
      local_BB = local_BC;
    } else {
      local_CC = local_CC + 1;
    }
    {
      int32_t unused_FF7C;
      int32_t local_FF78;
      local_FF78 = local_BC;
      switch (local_FF78) {
        case 0:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 0:
              Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
              if (script_2B1DF(5)) {
                Script_seqEnterEvtCam(1480, 0, AUTH_None);
              }
              break;
            case 20:
              if (script_2B1DF(5)) {
                Script_seqEnterEvtCam(1600, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                goto L03216E;
              }
              L03216E:;
              break;
            case 5:
              Script_scenCreateTask(script_2AFCE, 0);
              break;
            case 29:
              Script_seqEnterEvtCam(1474, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
              break;
          }
          break;
        case 1:
          local_FF78 = local_CC;
          if (local_FF78 == 5) {
          } else if (local_FF78 == 84) {
            Script_seqEnterEvtCam(1310, 0, AUTH_None);
          } else {
            switch (local_FF78) {
              case 67:
                Script_executeAicaCommand(459945, 0, 0);
                break;
              case 110:
                if (script_2B1DF(0)) {
                  Script_seqEnterEvtCam(1475, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
                }
                break;
              case 124:
                if (script_2B1DF(0) == 0) {
                  Script_seqEnterEvtCam(1608, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                }
                break;
              case 141:
                if (script_2B1DF(0) == 0) {
                  Script_seqEnterEvtCam(1475, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
                }
                break;
              case 84:
                if (script_2B1DF(0) == 0) {
                  Script_scenCreateTask(script_2AF16, 0);
                }
                break;
              case 100:
                if (script_2B1DF(0) == 0) {
                  Script_scenCreateTask(script_2AFDA, 0);
                }
                break;
              case 78:
                if (script_2B1DF(0) == 1) {
                  Script_scenCreateTask(script_2AECE, 0);
                }
                break;
              case 87:
                if (script_2B1DF(0) == 1) {
                  Script_scenCreateTask(script_2AFE6, 0);
                }
                break;
              case 42:
                Script_seqEnterEvtCam(1507, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                script_2C107(CHARA_YOSE);
                script_2C107(SHEN_NAME('HAKO'));
                script_2C107(SHEN_NAME('FLIT'));
                break;
            }
          }
          break;
        case 2:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 0:
              Script_scenCreateTask(script_2B0C6, 0);
              Script_executeAicaCommand(0x2004A9, 0, 0);
              break;
            case 12:
              if (script_2B1DF(16) == 1) {
                Script_scenCreateTask(script_2AFF2, 0);
              }
              break;
            case 14:
              Script_seqEnterEvtCam(1401, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              break;
            case 15:
              if (!(script_2B1DF(16) == 1)) goto L032404;
              L032404:;
              break;
            case 36:
              script_2C107(CHARA_TURU);
              script_2C107(SHEN_NAME('CYCL'));
              if (script_2B1DF(0)) {
                Script_seqEnterEvtCam(1400, SHEN_NAME('PIL4'), SHEN_NAME('PIL4'));
              } else {
                Script_seqEnterEvtCam(1403, SHEN_NAME('PIL4'), SHEN_NAME('PIL4'));
              }
              break;
            case 42:
              if (script_2B1DF(0) == 0) {
                Script_executeAicaCommand(0x1404A9, 0, 0);
              }
              break;
            case 60:
              if (script_2B1DF(0) == 0) {
                Script_executeAicaCommand(0x1504A9, 0, 0);
              }
              break;
            case 50:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2B012, 0);
              }
              break;
            case 48:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2B0EA, 0);
              } else {
                Script_scenCreateTask(script_2AFF2, 0);
              }
              break;
            case 49:
              if (script_2B1DF(0)) {
                Script_scenCreateTask(script_2B0EA, 0);
              }
              break;
          }
          break;
        case 3:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 20:
              Script_seqEnterEvtCam(1401, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              local_A0.x = -32.962921f;
              (&local_A0)->y = 0.0f;
              (&local_A0)->z = 41.759495f;
              local_94.x = (&local_94)->z = 0;
              (&local_94)->y = 51154;
              Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_A0);
              Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_94, 0);
              break;
            case 40:
              if (script_2B1DF(0) == 0) {
                Script_seqEnterEvtCam(1330, 0, AUTH_None);
              } else {
                Script_seqEnterEvtCam(1331, 0, AUTH_None);
              }
              break;
            case 63:
              Script_executeAicaCommand(591017, 0, 0);
              break;
            case 68:
              Script_executeAicaCommand(0x1704A9, 0, 0);
              break;
            case 76:
              if (script_2B1DF(0) == 0) {
                Script_seqEnterEvtCam(1332, 0, AUTH_None);
                local_A0.x = -25.907467f;
                (&local_A0)->y = 0.0f;
                (&local_A0)->z = 40.646854f;
                local_94.x = (&local_94)->z = 0;
                (&local_94)->y = 50972;
                Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_A0);
                Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_94, 0);
                local_A0.x = -24.437901f;
                (&local_A0)->y = 0.0f;
                (&local_A0)->z = 40.055099f;
                local_94.x = (&local_94)->z = 0;
                (&local_94)->y = 21171;
                Script_EVT_SetPosition(CHARA_NORK, POS_XYZ, &local_A0);
                Script_EVT_SetRotation(CHARA_NORK, 0x38000000, &local_94, 0);
                goto L032703;
              }
              L032703:;
              break;
            case 49:
              if (script_2B1DF(0)) {
                Script_scenCreateTask(script_2AECE, 0);
              }
              break;
            case 52:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2B01E, 0);
                goto L032753;
              }
              L032753:;
              break;
            case 79:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2B02A, 0);
              }
              break;
            case 100:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2B036, 0);
              }
              break;
          }
          break;
        case 4:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 35:
              if (script_2B1DF(0) == 0) {
                Script_executeAicaCommand(0x1404A9, 0, 0);
              }
              break;
            case 42:
              if (script_2B1DF(0) == 0) {
                Script_executeAicaCommand(0x1504A9, 0, 0);
              }
              break;
            case 46:
              if (script_2B1DF(0) == 0) {
                Script_executeAicaCommand(0x1504A9, 0, 0);
              }
              break;
            case 30:
              if (script_2B1DF(0) == 0) {
                Script_scenCreateTask(script_2B096, 0);
              }
              break;
            case 20:
              if (script_2B1DF(0)) {
                Script_scenCreateTask(script_2B08A, 0);
              }
              break;
            case 45:
              if (script_2B1DF(0) == 0) {
                Script_seqEnterEvtCam(1477, 0, AUTH_None);
              }
              break;
          }
          break;
        case 5:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 70:
              Script_mapmSetCamFlags(0, 0);
              Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
              Script_seqEnterEvtCam(1503, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              break;
            case 30:
              if (script_2B1DF(0)) {
                Script_mapmSetCamFlags(0, 0);
                Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
                Script_seqEnterEvtCam(1601, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
                Script_motmToggleFlags(CHARA_KURI, SCEN_Disable, 0x40);
                local_A0.x = -21.811741f;
                (&local_A0)->y = 0.0f;
                (&local_A0)->z = 38.495483f;
                Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_A0);
                local_A0.x = -21.29567f;
                (&local_A0)->y = 0.0f;
                (&local_A0)->z = 39.595795f;
                Script_EVT_SetPosition(CHARA_KURI, 0x78000000, &local_A0);
                Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
                Script_motmToggleFlags(CHARA_KURI, SCEN_Enable, 0x40);
              } else {
                Script_executeAicaCommand(0x2004A9, 0, 0);
              }
              break;
            case 31:
              if (script_2B1DF(0) == 0) {
                Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
                Script_mapmSetCamFlags(0, 1);
                Script_seqEnterEvtCam(1476, 0, AUTH_None);
              }
              break;
          }
          break;
        case 6:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 4:
              Script_mapmSetCamFlags(0, 0);
              Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
              Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
              local_A0.x = -24.134434f;
              (&local_A0)->y = 0.0f;
              (&local_A0)->z = 39.272751f;
              Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_A0);
              local_94.x = (&local_94)->z = 0;
              (&local_94)->y = 55654;
              Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_94, 0);
              Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
              Script_seqEnterEvtCam(1503, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              break;
            case 31:
              if (script_2B1DF(0) == 0) {
                Script_executeAicaCommand(0x1404A9, 0, 0);
              }
              break;
            case 20:
              if (script_2B1DF(0) == 0) {
                Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
                Script_mapmSetCamFlags(0, 1);
                Script_seqEnterEvtCam(1476, 0, AUTH_None);
              }
              break;
          }
          break;
        case 7:
          local_FF78 = local_84;
          switch (local_FF78) {
            case 22:
              if (script_2B1DF(0)) {
                Script_seqEnterEvtCam(1611, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
            case 43:
              if (script_2B1DF(0) == 0) {
                Script_seqEnterEvtCam(1605, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
          }
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 72:
              if (script_2B1DF(0) == 0) {
                local_BA = 0;
              } else {
                local_BA = 1;
              }
              break;
            case 51:
              if (script_2B1DF(0)) {
                Script_scenCreateTask(script_2AECE, 0);
              }
              break;
          }
          break;
        case 8:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 10:
              if (script_2B1DF(3)) {
                Script_seqEnterEvtCam(1609, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
            case 30:
              break;
            case 35:
              if (script_2B1DF(3) == 0) {
                Script_seqEnterEvtCam(1606, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              } else {
                Script_executeAicaCommand(2720, 2, 15);
                local_C4 = local_C0 = 0;
                while (local_C4 < 12) {
                  if (*(&local_D0 + local_C4) == 0) {
                    Script_sprintf("UNDEL[%ld]\012", *(&local_D0 + local_C4));
                  } else {
                    Script_srchControl((void *)*(&local_D0 + local_C4));
                    Script_sprintf("DEL[%ld]\012", *(&local_D0 + local_C4));
                    local_C0 = local_C0 + 1;
                  }
                  *(&local_D0 + local_C4) = 0;
                  local_C4 = local_C4 + 1;
                }
                script_2C107(CHARA_TERY);
                Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
                script_2C299();
                while (task_99F & 8) {
                  SCNF_WaitFrames_0000(1);
                }
                script_2C0F3(CHARA_JIMY);
                Script_executeAicaCommand(2720, 2, 20);
                Script_LoadSND(0, 4, "a1_jimse.snd", 0);
                script_28EFD();
                Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_JIMSE_EVENT_DONE, 1);
                Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_D000_JIMSE_EVENT_DONE, 1);
                if (task_994) {
                  Script_PlayerStatsUtils(PS_Set70MBCounter1, Script_PlayerStatsUtils(PS_Get70MBCounter1) + 1);
                }
                Script_PlayerStatsUtils(PS_Set70MBCounter0, Script_PlayerStatsUtils(PS_Get70MBCounter0) + 1);
                local_BF = -1;
              }
              break;
          }
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_9CC = 0) & 2);
  script_2C107(CHARA_YOSE);
  script_2C107(CHARA_TERY);
  script_2C107(CHARA_NORK);
  script_2C107(CHARA_AKSK);
  script_2C107(CHARA_YMGC);
  script_2C107(CHARA_KURI);
  script_2C107(CHARA_MRUA);
  script_2C107(SHEN_NAME('ABBX'));
  script_2C107(CHARA_TURU);
  script_2C107(SHEN_NAME('BICY'));
  script_2C107(CHARA_NORK);
  Script_charaToggleMotEnabled(CHARA_NORK, 0);
  Script_SetCharaLookAtPosition(CHARA_NORK, -6, 0, 0);
  local_C4 = 0;
  local_C0 = 0;
  while (local_C4 < 12) {
    if (*(&local_D0 + local_C4) == 0) {
      Script_sprintf("UNDEL[%ld]\012", *(&local_D0 + local_C4));
    } else {
      Script_srchControl((void *)*(&local_D0 + local_C4));
      Script_sprintf("DEL[%ld]\012", *(&local_D0 + local_C4));
      local_C0 = local_C0 + 1;
    }
    *(&local_D0 + local_C4) = 0;
    local_C4 = local_C4 + 1;
  }
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, local_8C);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, local_90);
  script_2B186(4);
  end:;
}

static int32_t script_32EEA(int32_t arg0, float arg1) {
  for (;;) {
    if (arg1 >= 1.0f) {
      break;
    }
    arg1 = arg1 + 0.01f;
    Script_SetCharaMotionPlaybackRate(arg0, arg1);
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  end:;
}

__attribute__((scn_string_bindings(0, data_45A2F, 1, data_45A37, 2, data_45A46)))
static int32_t script_32F23(void) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CTERMO2");
  script_2C1A9("/scene/02/D000", "CTERMO3");
  return (task_99F = 0) & 4;
}

__attribute__((scn_string_bindings(0, data_45A4E, 1, data_45A55, 2, data_45A5C, 3, data_45A65, 4, data_45A6E, 5, data_45A77, 6, data_45A80, 7, data_45A88, 8, data_45A90, 9, data_45A98, 10, data_45AA0, 11, data_45AA8)))
static void script_32F4F(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_45238 + local_FC) != -1) {
      if (*(&data_45238 + local_FC) != 0) {
        script_2C189(*(&data_45238 + local_FC), task_954[local_FC]);
      }
      local_FC = local_FC + 1;
    }
  }
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(int32_t *)(data_45264 + (local_FC << 2)) != -1) {
      if (*(int32_t *)(data_45264 + (local_FC << 2)) != 0) {
        script_2C189(*(int32_t *)(data_45264 + (local_FC << 2)), task_980[local_FC]);
      }
      local_FC = local_FC + 1;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "CTERST")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CTERST");
  }
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "CTERBASE")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CTERBASE");
  }
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "CTERMISE")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CTERMISE");
  }
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "CTERMO1")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CTERMO1");
  }
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "CTERMO2")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CTERMO2");
  }
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "CTERMO3")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CTERMO3");
  }
  Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
  Script_shenRequestGameModuleTransition(3, SHEN_NAME('ARAR'), 0);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_330E9(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  float local_F0;
  float local_EC;
  int32_t unused_E0[3];
  int32_t local_DC;
  if (arg1 == 0) {
    (void)((task_99B = -1) & 2);
    (void)((task_99C = -1) & 2);
    (void)((task_99E = -1) & 2);
    task_994 = 0;
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_SetShadowRange(20.0f);
    (void)((task_997 = -1) & 2);
    (void)((task_998 = -1) & 2);
    Script_scenCreateTask(script_3362C, 0);
    Script_WeatherUtils_ForFIXC(14, 0);
    while (task_997 & 8) {
      SCNF_WaitFrames_0000(1);
    }
    Script_facemotUtils(CHARA_AKIR, 0, 0);
    Script_SetActionSelectorHUDElementVisibility(1);
    script_FC43();
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
    Script_FaceEx(CHARA_TERY, FACE_ExpressionEnabled, 0, 2);
    Script_refbSetActive(1);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    Script_charaSetRefboardFlagTo(CHARA_TERY, 1);
    Script_LoadSND(0, 2, "rim006.snd", 0);
  } else {
    if (task_994 < 3) {
      task_994 = task_994 + 1;
    } else {
      task_994 = 3;
    }
    Script_scenCreateTask(script_33588, 0);
    Script_SetActionSelectorHUDElementVisibility(1);
    script_2C107(CHARA_TURU);
    script_2C107(CHARA_SOBA);
  }
  Script_LoadSND(0, 4, "a1_jimma.snd", 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ccowUtils(CHARA_TERY, CCOW_DisableAll);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmToggleFlags(CHARA_TERY, SCEN_Enable, 0x40);
  local_F0 = Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 35.0f);
  local_EC = Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 35.0f);
  script_2C0F3(CHARA_TERY);
  Script_executeAicaCommand(68520, 0, 0);
  Script_executeAicaCommand(1184, 2, 112);
  Script_executeAicaCommand(2464, 2, 20);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  script_2C0F3(CHARA_JIMY);
  script_29456();
  script_2C107(CHARA_JIMY);
  Script_LoadSND(0, 4, "e1chasej.snd", 0);
  while (task_998 & 0x10) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetQteOverlayTimeoutTuning(4, QTE_SetTimeoutStep);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_TERY, 0);
  Script_FACE(CHARA_AKIR, 0, 0, 1);
  Script_FACE(CHARA_TERY, 0, 0, 1);
  SCNF_WaitFrames_0000(1);
  Script_scenToggleVisibleFor(SHEN_NAME('PIL0'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PIL1'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PIL2'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PIL3'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PIL4'), 1);
  task_995 = 0;
  local_F4 = 0;
  while (local_F4 < 6) {
    script_2B1B1(local_F4);
    local_F4 = local_F4 + 1;
  }
  script_2B186(5);
  local_F8 = 0;
  while (arg0 >= 0) {
    script_2B1B1(1);
    script_2B1B1(2);
    script_2B1B1(4);
    local_F4 = 6;
    while (local_F4 < 30) {
      script_2B1B1(local_F4);
      local_F4 = local_F4 + 1;
    }
    local_DC = arg0;
    switch (local_DC) {
      case 0:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
        while (task_99C & 1) {
          SCNF_WaitFrames_0000(1);
        }
        local_FC = Script_scenCreateTask(script_2E7A5, 0);
        break;
      case 1:
        local_FC = Script_scenCreateTask(script_2F9C7, 0);
        break;
      case 2:
        local_FC = Script_scenCreateTask(script_2F9E7, 0);
        break;
      case 3:
        local_FC = Script_scenCreateTask(script_3140B, 0);
        break;
      case 4:
        while (task_99B & 0x80) {
          SCNF_WaitFrames_0000(1);
        }
        local_FC = Script_scenCreateTask(script_2CE5F, 0);
        break;
    }
    while (1) {
      if (script_2B1DF(4)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_FC);
    if (arg0 == 4) {
      Script_scenCreateTask(script_335CC, 0);
      arg0 = 0;
    } else if (arg0 <= 2) {
      if (script_2B1DF(3)) {
        local_F8 = local_F8 | 1;
        arg0 = 3;
      } else {
        arg0 = -1;
      }
    } else {
      if (script_2B1DF(3)) {
        local_F8 = local_F8 | 2;
      }
      arg0 = -1;
    }
  }
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, local_EC);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, local_F0);
  return local_F8;
}

static int32_t script_33588(void) {
  Script_FACE(CHARA_AKIR, 0, 0, 1);
  Script_FACE(CHARA_TERY, 0, 0, 1);
  script_2C107(CHARA_YOSE);
  while (task_99A & 0x40) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_335CC(void) {
  SCNF_WaitFrames_0000(50);
  (void)((task_9C4 = -1) & 4);
  SCNF_WaitFrames_0000(1);
  if (task_999 & 0x20) {
    Script_fixoClearAttachment(SHEN_NAME('SEIR'));
  }
  script_2C107(CHARA_HANA);
  script_2C107(SHEN_NAME('SEIR'));
  script_2C107(SHEN_NAME('DCYC'));
  script_2C107(SHEN_NAME('DSOB'));
  end:;
}

__attribute__((scn_string_bindings(0, data_45AD5, 1, data_45ADB, 2, data_45AE1, 3, data_45AE5, 4, data_45AE9, 5, data_45AF8, 6, data_45B00, 7, data_45B08, 8, data_45B09, 9, data_45B11, 10, data_45B12, 11, data_45B1A, 12, data_45B1B, 13, data_45B2A, 14, data_45B32, 15, data_45B41, 16, data_45B48, 17, data_45B57, 18, data_45B60, 19, data_45B6F)))
static int32_t script_3362C(void) {
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "SHUKI")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "SHUKI");
  }
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "OMG")) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OMG");
  }
  Script_ReallocateUNFO(task_FC);
  SCNF_WaitFrames_0000(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  script_2C1A9("/scene/02/D000", "CTERMO1");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "CTERMO1", "");
  Script_AuthorUtils(0, 1, "CTERMO1", "");
  Script_AuthorUtils(0, 2, "CTERMO1", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_2C1A9("/scene/02/D000", "CTERMO2");
  script_2C1A9("/scene/02/D000", "CTERST");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, -1);
  Script_facemotUtils(CHARA_TERY, 0, 0);
  Script_facemotUtils(CHARA_JIMY, 0, 0);
  Script_facemotUtils(CHARA_JIMY, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  (void)((task_997 = 0) & 4);
  script_2C1A9("/scene/02/D000", "CTERBASE");
  (void)((task_998 = 0) & 4);
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(int32_t *)(data_45264 + (local_FC << 2)) != -1) {
      if (*(int32_t *)(data_45264 + (local_FC << 2)) != 0) {
        task_980[local_FC] = script_2C11B(*(int32_t *)(data_45264 + (local_FC << 2)));
      }
      local_FC = local_FC + 1;
    }
  }
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_45238 + local_FC) != -1) {
      if (local_FC >= 2) {
        (void)((task_99B = 0) & 0x80);
      }
      task_954[local_FC] = script_2C11B(*(&data_45238 + local_FC));
      local_FC = local_FC + 1;
    }
  }
  script_2C1A9("/scene/02/D000", "CTERMISE");
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_4527C + local_FC) != -1) {
      Script_scenToggleVisibleFor(*(&data_4527C + local_FC), 0);
      local_FC = local_FC + 1;
    }
  }
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_452A8 + local_FC) != -1) {
      Script_scenToggleVisibleFor(*(&data_452A8 + local_FC), 0);
      local_FC = local_FC + 1;
    }
  }
  (void)((task_99C = 0) & 2);
  (void)((task_99D = -1) & 2);
  (void)((task_99A = 0) & 2);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  return (task_99E = 0) & 2;
}

__attribute__((scn_string_bindings(0, data_45B78, 1, data_45B80, 2, data_45B8F)))
static void script_338DF(void) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CTERMO3");
  script_2C1A9("/scene/02/D000", "CTERMO2");
  (void)((task_99F = 0) & 2);
  return;
}

static void script_3390B(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_45270 + local_FC) != -1) {
      if (*(&data_45270 + local_FC) != 0) {
        script_2C189(*(&data_45270 + local_FC), task_980[local_FC]);
      }
      local_FC = local_FC + 1;
    }
  }
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(int32_t *)(data_45264 + (local_FC << 2)) != -1) {
      if (*(int32_t *)(data_45264 + (local_FC << 2)) != 0) {
        task_980[local_FC] = script_2C11B(*(int32_t *)(data_45264 + (local_FC << 2)));
      }
      local_FC = local_FC + 1;
    }
  }
  (void)((task_99B = 0) & 2);
  (void)((task_99C = 0) & 2);
  (void)((task_99D = -1) & 2);
  (void)((task_99A = 0) & 2);
  return;
}

static void script_339E2(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(int32_t *)(data_45264 + (local_FC << 2)) != -1) {
      if (*(int32_t *)(data_45264 + (local_FC << 2)) != 0) {
        script_2C189(*(int32_t *)(data_45264 + (local_FC << 2)), task_980[local_FC]);
      }
      local_FC = local_FC + 1;
    }
  }
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_45270 + local_FC) != -1) {
      if (*(&data_45270 + local_FC) != 0) {
        task_980[local_FC] = script_2C11B(*(&data_45270 + local_FC));
      }
      local_FC = local_FC + 1;
    }
  }
  (void)((task_99B = -1) & 2);
  (void)((task_99C = -1) & 2);
  (void)((task_99D = 0) & 2);
  (void)((task_99A = -1) & 2);
  return;
}

__attribute__((scn_string_bindings(0, data_45B97, 1, data_45B9E, 2, data_45BA5)))
static void script_33AB9(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int8_t local_EF;
  uint8_t unused_EC[3];
  (void)((local_EF = 0) & 2);
  task_940 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_executeAicaCommand(2720, 2, 10);
  Script_filmCheckOrSetTransitionUtil(10, 0, 0, 0, 0, 255, 0, 0, 0);
  Script_WeatherUtils_ForFIXC(13, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 30, 0);
  SCNF_WaitFrames_0000(1);
  Script_LoadVoiceFile(1, "01JIMB");
  Script_LoadVoiceFile(1, "E1004a");
  Script_LoadVoiceFile(1, "01JIM");
  Script_ReturnTimeDate(&local_F4);
  for (;;) {
    Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, 11, Script_PlayerStatsUtils(PS_Get70MBTimeCapSlot, 11) + 1);
    Script_SetNewDateTime(&local_F4);
    script_2C272();
    local_F0 = script_330E9(4, local_EF);
    (void)((local_EF = -1) & 0x10);
    if (local_F0 != 3) {
      script_2C299();
      if (local_F0 == 1) {
        while (task_99F & 8) {
          SCNF_WaitFrames_0000(1);
        }
        Script_LoadSND(0, 4, "a1_jimsi.snd", 0);
        Script_executeAicaCommand(2720, 2, 20);
        script_2C0F3(CHARA_JIMY);
        script_28F50();
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        script_2C107(CHARA_JIMY);
        (void)((task_99F = -1) & 0x10);
        script_338DF();
        Script_uiUtils(0, 35, 0);
        Script_sicnUtils(0, 1, 0);
        SCNF_WaitFrames_0000(90);
        Script_sicnUtils(1, 0, 0);
        while (task_99F & 8) {
          SCNF_WaitFrames_0000(1);
        }
      } else {
        task_940 = 1;
        Script_uiUtils(0, 35, 0);
        Script_sicnUtils(0, 1, 0);
        SCNF_WaitFrames_0000(90);
        Script_sicnUtils(1, 0, 0);
        Script_SetQteOverlayTimeoutTuning(1, QTE_TimeoutSession);
      }
      script_2C2C0();
    } else {
      break;
    }
  }
  script_32F4F();
  return;
}

static void script_33CA3(void) {
  task_948 = Script_GetSceneID();
  return;
}

__attribute__((scn_string_bindings(1, data_45BC6, 2, data_45BD2, 3, data_45BE0)))
static void script_33CAF(void) {
  int32_t local_FC;
  local_FC = task_948;
  if (local_FC == 1) {
    Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/01/D000", (int32_t)"SCRTEST.BIN", 0x52455443);
  } else if (local_FC == 2) {
    Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/02/D000", (int32_t)"SCRTEST.BIN", 0x52455443);
  } else {
    Script_sprintf(" Error : syuki init\012");
  }
  return;
}

static void script_33D06(void) {
  while (1) {
    Script_PreserveAccumulator_0182(1, 0, 11);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_33D25(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  Script_srchControl(0);
  return;
}

static int32_t script_33D49(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  task_9F0[arg1] = 0;
  task_9F8[arg1] = 0;
  task_A00[arg1] = 0;
  task_A08[arg1] = arg2;
  task_A10[arg1] = arg3;
  task_A18 = 0;
  Script_CharHandlers(arg0, 0, task_A08[arg1], 16);
  Script_CharHandlers(arg0, 1, task_A10[arg1], 16);
  for (;;) {
    task_9F0[arg1] = 0;
    while (task_9F0[arg1] == 0) {
      SCNF_WaitFrames_0000(1);
    }
    task_A18 = task_A00[arg1] | 0x40000000;
    Script_scenCreateTask(arg4, 3, arg0, arg1, task_9F0[arg1] + task_9F8[arg1]);
  }
  end:;
}

static int32_t script_33E20(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  local_FC = task_A18;
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

static void script_33EB7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 0, task_A08[arg1], 16);
  Script_CharHandlers(arg0, 1, task_A10[arg1], 16);
  Script_srchControl(0);
  return;
}

static void script_33F19(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = task_A18;
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
  Script_srchControl(0);
  return;
}

static void script_33F75(int32_t arg0, float arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = task_A18;
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

static void script_33FD2(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_33FEE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5) {
  int32_t local_FC;
  task_A34[arg1] = Script_scenCreateTask(script_33D49, 5, arg0, arg1, arg2, arg3, arg5);
  task_A1C[arg1] = arg0;
  task_A24[arg1] = 0;
  task_A2C[arg1] = 0;
  task_A3C = 4;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_A24[arg1]) {
      local_FC = task_A2C[arg1] * 12;
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
          task_9F0[arg1] = local_F8;
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 6);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      if (task_9F8[arg1] == 0) {
        Script_scenCreateTask(script_3435E, 4, Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 1), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 2), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 3));
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
          task_9F0[arg1] = local_F8;
        }
      }
      script_3425C();
      {
        int32_t local_F8;
        if (task_A48 == Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC)) {
          task_A44 = 1;
          local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 10);
        } else {
          if (task_9F8[arg1] == 0) {
            task_A50 = task_A50 + Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 4);
          }
          task_A44 = 0;
          local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 11);
        }
        if (local_F8 > 0) {
          task_9F0[arg1] = local_F8;
        }
      }
      script_3424C(arg1);
    }
  }
  end:;
}

static void script_34215(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  task_A24[arg0] = 1;
  task_A2C[arg0] = arg1;
  task_9F8[arg0] = arg2;
  task_A00[arg0] = arg3;
  return;
}

static void script_3424C(int32_t arg0) {
  task_A24[arg0] = 0;
  return;
}

static void script_3425C(void) {
  SCNF_WaitFrames_0000(2);
  while (task_A48 == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3427C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_34215(arg0, arg1, 0, arg3);
  script_34215(arg2, arg1, 1, arg3);
  while (task_A24[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_342B6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_34215(arg0, arg1, 0, arg3);
  script_34215(arg2, arg1, 1, arg3);
  script_34215(arg2 + 1, arg1, 2, arg3);
  while (task_A24[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_34302(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_34215(arg0, arg1, 0, arg4);
  script_34215(arg2, arg1, 1, arg4);
  script_34215(arg3, arg1, 2, arg4);
  while (task_A24[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3434B(int32_t arg0) {
  task_A3C = arg0;
  Script_SetQteOverlayTimeoutTuning(arg0, QTE_SetTimeoutStep);
  return;
}

static int32_t script_3435E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = arg1 + arg3;
  local_F8 = arg2 + arg3;
  Script_scenCreateTask(script_343C1, 2, arg1, arg3);
  Script_SetQteOverlayTimeoutTuning(local_FC, QTE_SetTimeoutMaximum);
  if (arg0 == 68) {
    SCNF_UIUtils(0x44, local_F8, 5, 0, 1, 0);
  } else {
    SCNF_UIUtils(arg0, local_F8, 2, 0, 1, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_343C1(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  task_A4C = arg0;
  task_A48 = 0;
  task_A40 = 0;
  local_F8 = 0xFFFF;
  for (;;) {
    local_FC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_A40 = task_A40 + 1;
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
      if (task_A40 >= arg0 + arg1 + 4) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((local_F8 & 0xFFF) == 0) {
    local_F8 = 0xFFFF;
  }
  task_A48 = local_F8 & 0xFFF;
  return Script_srchControl(0);
}

static int32_t script_34482(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_A54[arg1] = arg0;
    task_A5C[arg1][0] = (&local_F4)->x;
    task_A5C[arg1][1] = (&local_F4)->y;
    task_A5C[arg1][2] = (&local_F4)->z;
  }
  task_A74[arg1] = 0.0f;
  for (;;) {
    Vector3 local_F4;
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_A5C[arg1][0] = (&local_F4)->x;
    task_A5C[arg1][1] = (&local_F4)->y;
    task_A5C[arg1][2] = (&local_F4)->z;
    task_A74[arg1] = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, task_A5C[arg1][0] - task_A5C[arg2][0], task_A5C[arg1][2] - task_A5C[arg2][2]);
  }
  end:;
}

static void script_34570(void) {
  task_A7C = 0.0f;
  task_A80 = 0.0f;
  task_A84 = 0.0f;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetInputData(2, 0) & 4) {
      if (Script_GetInputData(0, 2) & 0x4000) {
        task_A7C = task_A7C - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x8000) {
        task_A7C = task_A7C + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x1000) {
        task_A80 = task_A80 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x2000) {
        task_A80 = task_A80 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x40) {
        task_A84 = task_A84 - 1.0f;
      }
      if (Script_GetInputData(0, 2) & 0x80) {
        task_A84 = task_A84 + 1.0f;
      }
    }
  }
  return;
}

static void script_3465E(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_34687(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4) {
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

static void script_34716(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  int32_t local_FC;
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_FC);
  Script_InitializeOsag(arg0);
  return;
}

static void script_34749(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_34763, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_34763(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  script_34687(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_3478E(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5) {
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

static void script_3480E(int32_t arg0, int32_t arg1, float arg2) {
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

static void script_34862(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_34878, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_34878(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  script_3480E(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_3489F(int32_t arg0, float arg1) {
  Vector3i local_F4;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F4, 0);
  return;
}

static void script_348C8(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return;
}

static void script_3491C(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_34930, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_34930(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_3489F(arg0, arg1);
  return Script_srchControl(0);
}

static void script_34955(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_3496D, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_3496D(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  script_348C8(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_34996(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_349AE, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_349AE(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  float local_FC;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_FC = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    script_348C8(arg0, 0.0f, 0.0f, local_FC);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_34A03(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    Script_EVT_GetPosition(task_A1C[local_E4], -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_A1C[local_E4], &local_E8, 0);
    task_A88[local_E4][0] = (&local_F4)->x;
    task_A88[local_E4][1] = (&local_F4)->y;
    task_A88[local_E4][2] = (&local_F4)->z;
    task_AA0[local_E4][0] = (&local_E8)->x;
    task_AA0[local_E4][1] = (&local_E8)->y;
    task_AA0[local_E4][2] = (&local_E8)->z;
    task_AB8[local_E4] = Script_GetCharaMotionRemainingFramesScaled(task_A1C[local_E4]);
    task_AC0[local_E4] = Script_scenCharaGetMotAnimId(task_A1C[local_E4]);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_34AE8(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    local_F4.x = task_A88[local_E4][0];
    (&local_F4)->y = task_A88[local_E4][1];
    (&local_F4)->z = task_A88[local_E4][2];
    local_E8.x = task_AA0[local_E4][0];
    (&local_E8)->y = task_AA0[local_E4][1];
    (&local_E8)->z = task_AA0[local_E4][2];
    Script_EVT_SetPosition(task_A1C[local_E4], POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_A1C[local_E4], 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(task_A1C[local_E4], task_AC0[local_E4], task_AB8[local_E4], -1.0f, -1.0f, 1.0f, 0);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_34BD1(int32_t arg0) {
  task_ACC = arg0;
  task_AC8 = 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_ACC, task_AC8);
  return;
}

static void script_34BEF(void) {
  int32_t local_FC;
  if (task_AC8 == 1) {
    local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
    Script_PlayerStatsUtils(PS_Set70MBCounter1, local_FC + 1);
  }
  if (task_AC8 < 255) {
    task_AC8 = task_AC8 + 1;
  }
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_ACC, task_AC8);
  return;
}

static void script_34C37(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter0);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, local_FC + 1);
  return;
}

static void script_34C52(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_34C6C(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    script_34C52();
  }
  return;
}

static void script_34CAA(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_34CCB(int32_t arg0) {
  L034CCD:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, arg0)) goto L034CCD;
  return;
}

static void script_34CE2(int32_t arg0) {
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
      script_34CCB(local_FC);
    }
  }
  (void)local_FC;
  return;
}

static void script_34D68(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static int32_t script_34D8B(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg2;
  switch (local_FC) {
    case 1:
      Script_scenCreateTask(script_33E20, 6, 30722, -1, 0x41700000, 0x3F800000, arg0, arg1);
      break;
    case 2:
      Script_scenCreateTask(script_33E20, 6, 30728, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 3:
      Script_scenCreateTask(script_33E20, 6, 30722, 20, 0x42DC0000, 0x3F800000, arg0, arg1);
      break;
    case 4:
      Script_scenCreateTask(script_33E20, 6, 30728, -1, 0x42AA0000, 0x3F800000, arg0, arg1);
      break;
    case 5:
      Script_scenCreateTask(script_33E20, 6, 30724, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 6:
      Script_scenCreateTask(script_33E20, 6, 30731, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 7:
      Script_scenCreateTask(script_33E20, 6, 30726, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 8:
      Script_scenCreateTask(script_33E20, 6, 30733, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 9:
      Script_scenCreateTask(script_33E20, 6, 30725, 2, 0x41200000, 0x3F800000, arg0, arg1);
      break;
    case 10:
      Script_scenCreateTask(script_33E20, 6, 30732, 0, 0, 0x3F800000, arg0, arg1);
      break;
    case 11:
      break;
    case 12:
      break;
    case 13:
      Script_scenCreateTask(script_33E20, 6, 30723, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 14:
      Script_scenCreateTask(script_33E20, 6, 30730, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 15:
      Script_scenCreateTask(script_33E20, 6, 30721, -1, 0x41F80000, 0x3F800000, arg0, arg1);
      break;
    case 16:
      Script_scenCreateTask(script_33E20, 6, 30727, -1, 0x41F80000, 0x3F800000, arg0, arg1);
      break;
    case 17:
      break;
    case 18:
      break;
  }
  return Script_srchControl(0);
}

static void script_34FFE(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_35010, 2, arg0, arg1);
  return;
}

static int32_t script_35010(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_StartOrFreeSEQ(0);
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1005A001");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1005A002");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1005A003");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1005A004");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1005B001");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1005B002");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1005B003");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1005B004");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1005B005");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1005B006");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1005B007");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1005B008");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1005B009");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1005B010");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1005B011");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1005B012");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1005B013");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1005B014");
      break;
  }
  return Script_srchControl(0);
}

static void script_351B1(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_351C3, 2, arg0, arg1);
  return;
}

static int32_t script_351C3(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static void script_351E7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_351FD, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_351FD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg1 == -1) {
    Script_seqEnterEvtCam(arg0, 0, AUTH_None);
  } else {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return Script_srchControl(0);
}

static void script_35237(int32_t arg0, int32_t arg1) {
  task_ADC = arg0;
  task_9D4 = 0;
  return;
}

static void script_35248(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      SCNT_ShowSubtitle("開始０");
      break;
    case 1:
      SCNT_ShowSubtitle("成功０");
      break;
    case 2:
      SCNT_ShowSubtitle("失敗０");
      break;
    case 3:
      SCNT_ShowSubtitle("開始１");
      break;
    case 4:
      SCNT_ShowSubtitle("成功１");
      break;
    case 5:
      SCNT_ShowSubtitle("失敗１");
      break;
    case 6:
      SCNT_ShowSubtitle("開始２");
      break;
    case 7:
      SCNT_ShowSubtitle("成功２");
      break;
    case 8:
      SCNT_ShowSubtitle("失敗２");
      break;
    case 9:
      SCNT_ShowSubtitle("開始３");
      break;
    case 10:
      SCNT_ShowSubtitle("成功３");
      break;
    case 11:
      SCNT_ShowSubtitle("失敗３");
      break;
    case 12:
      SCNT_ShowSubtitle("開始４");
      break;
    case 13:
      SCNT_ShowSubtitle("成功４");
      break;
    case 14:
      SCNT_ShowSubtitle("失敗４");
      break;
    case 15:
      SCNT_ShowSubtitle("成功５");
      break;
    case 16:
      SCNT_ShowSubtitle("失敗５");
      break;
  }
  Script_srchControl(0);
  return;
}

static int32_t script_35389(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_SetQteOverlayTimeoutTuning(0, QTE_TimeoutSession);
  SCNF_WaitFrames_0000(5);
  {
    int32_t local_E0;
    local_E0 = Script_PlayerStatsUtils(PS_Get70MBTimeCapSlot, 6);
    if (local_E0 < 127) {
      local_E0 = local_E0 + 1;
    }
    Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, 6, local_E0);
  }
  for (;;) {
    Script_charaDisplay(SHEN_NAME('BEER'), CDM_Show);
    script_3661D();
    SCNF_WaitFrames_0000(1);
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_ccowUtils(task_A1C[1], CCOW_DisableAll);
    Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 0, 0, 0, 0);
    local_E4 = Script_scenCreateTask(script_3568E, 2, arg0, arg1);
    task_A50 = 0;
    for (;;) {
      if (task_A50 >= 10) {
        Script_SetQteOverlayTimeoutTuning(1, QTE_TimeoutSession);
        Script_srchControl((void *)local_E4);
        if (Script_PlayerStatsUtils(PS_Get70MBTimeCapSlot, 6) == 1) {
          int32_t local_E0;
          local_E0 = Script_PlayerStatsUtils(PS_Get70MBCounter1);
          Script_PlayerStatsUtils(PS_Set70MBCounter1, local_E0 + 1);
        }
        Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
        Script_uiUtils(0, 0, 0);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 19);
        SCNF_WaitFrames_0000(1);
        break;
      }
      if (task_A50 == -1) {
        Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
        Script_srchControl((void *)local_E4);
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    if (task_A50 == -1) {
      int32_t local_E0;
      Script_SetQteOverlayTimeoutTuning(2, QTE_TimeoutSession);
      local_E0 = Script_PlayerStatsUtils(PS_Get70MBTimeCapSlot, 6);
      Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, 6, local_E0 | 0x80);
      local_E0 = Script_PlayerStatsUtils(PS_Get70MBTimeCapSlot, 6);
      script_34C37();
      Script_uiUtils(0, 0, 0);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 199, 1);
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_srchControl((void *)local_E4);
      SCNF_WaitFrames_0000(1);
      script_3697F();
    } else {
      if (task_9E8 == 1) {
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        SCNF_WaitFrames_0000(60);
        Script_uiUtils(0, 21, 0);
        Script_sicnUtils(0, 1, 1);
        local_F4.x = -114.446602f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = 99.066704f;
        local_E8.x = 0;
        (&local_E8)->y = 44236;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
        Script_convSetCamInactiveFlag(1);
        Script_SetGameStateTo(AUTH_None);
        SCNF_WaitFrames_0000(90);
        Script_sicnUtils(1, 0, 0);
        Script_uiUtils(0, 0, 0);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      }
      continue;
    }
  }
  end:;
}

static int32_t script_3568E(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  for (;;) {
    task_A50 = 0;
    (void)((task_9EC = 0) & 1);
    script_34687(arg0, -1, -112.6f, 99.41f, 245.7f);
    script_34687(task_A1C[1], arg0, -0.231f, -3.932f, 180.0f);
    script_351E7(8507, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_34FFE(5, -1);
    script_3427C(arg1, 0, 1, 0);
    if (task_A44) {
      script_35237(4, -1);
      script_34FFE(1, -1);
      script_34FFE(10, 30);
      script_34FFE(11, 145);
      script_364EB(155);
      script_351E7(8507, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      script_351E7(8509, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 60);
      script_351B1(0x1B76AB, 40);
      SCNF_WaitFrames_0000(230);
    } else {
      script_34687(task_A1C[1], arg0, -0.35f, -0.575f, 180.0f);
      script_351E7(8502, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      Script_scenCreateTask(script_33E20, 6, 30731, -1, 0, 0x3F800000, task_A1C[1], 1);
      script_351B1(328873, -1);
      script_351B1(394409, 70);
      script_351B1(394409, 140);
      script_36281();
      SCNF_WaitFrames_0000(30);
      task_AD0 = 0;
      while (task_A44 == 0) {
        script_34687(task_A1C[1], arg0, -0.425f, -0.125f, 180.0f);
        script_361E9();
        script_3427C(arg1, 1, 1, 0);
        (void)((task_9EC = -1) & 2);
        if (task_A44) {
          script_35237(7, -1);
          script_34687(task_A1C[1], arg0, -0.4f, -0.18f, 180.0f);
          script_34FFE(0, -1);
          script_34FFE(10, 30);
          script_34FFE(11, 130);
          script_364EB(140);
          script_351B1(0x1B76AB, 10);
          script_351E7(8509, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 30);
          SCNF_WaitFrames_0000(30);
          script_34687(arg0, -1, -109.9f, 101.0f, 245.7f);
          script_34687(task_A1C[1], arg0, -1.0f, 0.8f, 180.0f);
          SCNF_WaitFrames_0000(180);
        } else {
          Script_ReturnTimeDate(&local_F4);
          if (((&local_F4)->hours == 23) & ((&local_F4)->minutes > 30)) {
            Script_filmCheckOrSetTransitionUtil(180, 0, 0, 0, 0, 255, 255, 64, 128);
            SCNF_WaitFrames_0000(180);
            Script_executeAicaCommand(2720, 1, 30);
            Script_executeAicaCommand(2720, 2, 30);
            Script_executeAicaCommand(2720, 3, 30);
            Script_filmCheckOrSetTransitionUtil(30, 255, 255, 64, 128, 255, 0, 0, 0);
            SCNF_WaitFrames_0000(30);
            if (Script_PlayerStatsUtils(PS_Get70MBTimeCapSlot, 6) == 1) {
              local_F0 = Script_PlayerStatsUtils(PS_Get70MBCounter1);
              Script_PlayerStatsUtils(PS_Set70MBCounter1, local_F0 + 1);
            }
            Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
            SCNF_WaitFrames_0000(1);
          }
          script_351B1(394409, -1);
          script_35237(8, -1);
          if (task_9E8 == 1) {
            task_A50 = 0;
          }
          script_36281();
          script_351B1(394409, 20);
          script_351B1(394409, 90);
          script_351B1(394409, 160);
        }
      }
    }
    Script_PlayCharaMotion(task_A1C[1], MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_36DAD();
    Script_SetCharaLookAtPosition(CHARA_DICK, -2, 0, 0);
    script_34687(arg0, -1, -109.515999f, 100.806f, 245.7f);
    script_34687(task_A1C[1], arg0, -0.25f, 1.0f, 0.0f);
    script_351E7(8505, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    SCNF_WaitFrames_0000(1);
    script_3656A(arg0, task_A1C[1]);
    script_3427C(arg1, 2, 1, 0);
    if (task_A44) {
      script_35237(10, -1);
      script_365B6(arg0, -1, 6);
      script_351E7(8510, -1, -1, -1);
      script_351E7(8503, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 215);
      script_351E7(8504, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 290);
      script_34FFE(14, 22);
      script_34FFE(3, 87);
      script_34FFE(15, 162);
      script_34FFE(17, 247);
      script_351B1(0x2076AB, 2);
      script_351B1(0x2476AB, 22);
      script_351B1(0x2776AB, 42);
      script_351B1(685739, 162);
      Script_scenCreateTask(script_3610A, 0);
      SCNF_WaitFrames_0000(42);
      Script_SetCamShake(0.15f, 0.05f, 0.7f);
      script_33FD2(CHARA_AKIR);
      script_3605E(arg0);
    } else {
      script_365B6(arg0, 14, 1);
      script_351E7(8514, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 8);
      script_351E7(8513, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 53);
      script_34FFE(2, 14);
      script_34FFE(12, 78);
      script_34FFE(13, 143);
      script_35237(11, -1);
      script_351B1(0x2676AB, 168);
      Script_scenCreateTask(script_35F08, 0);
      SCNF_WaitFrames_0000(14);
      Script_SetCamShake(0.2f, 0.2f, 0.5f);
      SCNF_WaitFrames_0000(190);
      script_351B1(459945, 25);
      Script_ReturnTimeDate(&local_F4);
      task_A50 = 10;
      SCNF_WaitFrames_0000(1);
    }
    task_A50 = -1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_35CDE(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4) {
  SCNF_WaitFrames_0000(1);
  task_AE0[arg1] = arg0;
  task_AE8[arg1] = arg3;
  script_35D54(arg1, arg2, arg4);
  script_35E56(arg1);
  for (;;) {
    if (task_AF0[arg1]) {
      task_AF8[arg1] = Script_walk3D(ActivateGetIdx, arg0, task_B00[arg1]);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_35D54(int32_t arg0, float arg1, const void *arg2) {
  int32_t local_FC;
  task_AF0[arg0] = 0;
  task_AF8[arg0] = 0;
  task_B00[arg0] = arg1;
  local_FC = task_AE8[arg0];
  if (local_FC == 0) {
    Script_PlayCharaMotion(task_AE0[arg0], MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    goto L035DBD;
  }
  L035DBD:;
  Script_walk3D(SetupPath, task_AE0[arg0], arg2, 0);
  return;
}

static void script_35DD6(int32_t arg0, float arg1) {
  int32_t local_FC;
  task_B00[arg0] = arg1;
  task_AF0[arg0] = 1;
  local_FC = task_AE8[arg0];
  if (local_FC == 0) {
    Script_charaToggleMotEnabled(task_AE0[arg0], 0);
    Script_SetCharaMotionPlaybackRate(task_AE0[arg0], 1.0f);
    Script_PlayCharaMotion(task_AE0[arg0], MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    goto L035E55;
  }
  L035E55:;
  return;
}

static void script_35E56(int32_t arg0) {
  int32_t local_FC;
  task_AF0[arg0] = 0;
  local_FC = task_AE8[arg0];
  if (local_FC == 0) {
    Script_charaToggleMotEnabled(task_AE0[arg0], 1);
    Script_SetCharaMotionPlaybackRate(task_AE0[arg0], 1.0f);
    goto L035EA2;
  }
  L035EA2:;
  return;
}

static int32_t script_35EA3(int32_t arg0, const void *arg1, int32_t arg2, float arg3) {
  Script_PlayCharaMotion(arg0, arg2, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, arg1, 0, 0);
  for (;;) {
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, arg3) == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaToggleMotEnabled(arg0, 1);
  end:;
}

static int32_t script_35F08(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  script_351B1(197801, 15);
  SCNF_WaitFrames_0000(14);
  Script_fixoClearAttachment(SHEN_NAME('BEER'));
  Script_charaDisplay(SHEN_NAME('BEER'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BEEW'), CDM_Show);
  local_F4.x = -110.4254f;
  (&local_F4)->y = 1.043f;
  (&local_F4)->z = 100.668999f;
  local_E8.x = 31683;
  (&local_E8)->y = 32958;
  (&local_E8)->z = 59348;
  Script_EVT_SetPosition(SHEN_NAME('BEEW'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('BEEW'), 0x38000000, &local_E8, 0);
  local_E4 = 88;
  while (local_E4 < 544) {
    local_E0 = 0;
    while (local_E0 < 24) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_InterpolateAndScaleAngles(local_E0, local_E4, &local_F4, &local_E8, "DAT1");
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(SHEN_NAME('BEEW'), *(&data_45CA8 + local_E0), &local_F4, &local_E8, CHARA_PART_Set);
      local_E0 = local_E0 + 1;
    }
    local_E4 = local_E4 + 2;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_3605E(int32_t arg0) {
  script_34687(task_A1C[1], arg0, -0.35f, 1.1f, 183.6f);
  script_351E7(8512, -1, -1, -1);
  Script_PlayCharaMotion(CHARA_DICK, 30734, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(50);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_UP1_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.8f);
  SCNF_WaitFrames_0000(45);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_7);
  return;
}

static int32_t script_3610A(void) {
  int8_t local_FF;
  Vector3 local_F0;
  Vector3i local_E4;
  SCNF_WaitFrames_0000(37);
  Script_fixoClearAttachment(SHEN_NAME('BEER'));
  script_351B1(525481, -1);
  local_E4.x = 902;
  (&local_E4)->y = 0xB100;
  (&local_E4)->z = 17101;
  local_FF = 0;
  Script_EVT_GetPosition(SHEN_NAME('BEER'), -1, &local_F0, 0);
  Script_EVT_SetRotation(SHEN_NAME('BEER'), 0x38000000, &local_E4, 0);
  while (local_FF < 60) {
    local_F0.x = (&local_F0)->x + 0.02f - (float)local_FF * 0.0003f;
    (&local_F0)->z = (&local_F0)->z + 0.01f - (float)local_FF * 0.00015f;
    Script_EVT_SetPosition(SHEN_NAME('BEER'), POS_XYZ, &local_F0);
    Script_EVT_SetRotation(SHEN_NAME('BEER'), 0x38000000, &local_E4, 0);
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_361E9(void) {
  int8_t local_FF;
  int32_t local_F8;
  local_F8 = task_AD0;
  switch (local_F8) {
    case 0:
      script_351E7(8502, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      break;
    case 1:
      script_351E7(8515, -1, -1, -1);
      break;
    case 2:
      script_351E7(8516, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      break;
  }
  for (;;) {
    local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
    if (local_FF != task_AD0) {
      task_AD0 = local_FF;
      break;
    }
  }
  return;
}

static void script_36281(void) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  if (-(*(int8_t *)task_AD4 == 1) & -(*((int8_t *)task_AD4 + 1) == 1) & -(*((int8_t *)task_AD4 + 2) == 1) & -(*((int8_t *)task_AD4 + 3) == 1)) {
    local_FF = 0;
    while (local_FF < 4) {
      *((int8_t *)task_AD4 + local_FF) = 0;
      local_FF = local_FF + 1;
    }
  }
  local_FF = 0;
  while (local_FF < 2) {
    for (;;) {
      local_FE = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if ((*((int8_t *)task_AD4 + local_FE) != 1) & (local_FE != task_AD8)) {
        *((int8_t *)task_AD4 + local_FE) = 1;
        break;
      }
    }
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        script_34FFE(6, local_FF * 100);
        break;
      case 1:
        script_34FFE(7, local_FF * 100);
        break;
      case 2:
        script_34FFE(8, local_FF * 100);
        break;
      case 3:
        script_34FFE(9, local_FF * 100);
        break;
    }
    local_FF = local_FF + 1;
  }
  task_AD8 = local_FE;
  return;
}

static int32_t script_363A1(void) {
  Vector3 local_F4;
  local_F4.x = 0.0f;
  (&local_F4)->z = 0.0f;
  (&local_F4)->y = 0.25f;
  {
    int8_t local_F3;
    local_F3 = 0;
    while (local_F3 < 5) {
      Script_EVT_SetPosition(SHEN_NAME('LDUM'), -0x48000000, &local_F4);
      local_F3 = local_F3 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  (&local_F4)->y = -0.25f;
  {
    int8_t local_F3;
    local_F3 = 0;
    while (local_F3 < 20) {
      Script_EVT_SetPosition(SHEN_NAME('LDUM'), -0x48000000, &local_F4);
      local_F3 = local_F3 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  (&local_F4)->y = 0.25f;
  {
    int8_t local_F3;
    local_F3 = 0;
    while (local_F3 < 15) {
      Script_EVT_SetPosition(SHEN_NAME('LDUM'), -0x48000000, &local_F4);
      local_F3 = local_F3 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_365CA(CHARA_DICK, -1, 1);
  return Script_srchControl(0);
}

static int32_t script_36486(void) {
  Vector3 local_F4;
  int8_t local_F3;
  local_F3 = 0;
  while (local_F3 < 50) {
    script_34687(SHEN_NAME('LDUM'), 0x4B434944, 0.0f, 2.4f, 0.0f);
    local_F3 = local_F3 + 1;
    Script_EVT_GetPosition(SHEN_NAME('LDUM'), -1, &local_F4, 0);
    Script_SetLookPointDetails(CHARA_DICK, 1, &local_F4, 0);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_364EB(int32_t arg0) {
  Script_scenCreateTask(script_364FB, 1, arg0);
  return;
}

static int32_t script_364FB(int32_t arg0) {
  Vector3 local_F4;
  if (arg0 >= 0) {
    SCNF_WaitFrames_0000(arg0);
  }
  script_34687(SHEN_NAME('LDUM'), 0x4B434944, 0.0f, 2.0f, 0.0f);
  Script_EVT_GetPosition(SHEN_NAME('LDUM'), -1, &local_F4, 0);
  (&local_F4)->y = 2.4f;
  Script_EVT_SetPosition(SHEN_NAME('LDUM'), POS_XYZ, &local_F4);
  Script_scenCreateTask(script_363A1, 0);
  Script_scenCreateTask(script_36486, 0);
  end:;
}

static void script_3656A(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  script_365CA(arg0, -1, 1);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 98320, &local_F4, 2);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 5);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_365B6(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_365CA, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_365CA(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_SetCharaLookAtPosition(arg0, -arg2, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  end:;
}

static void script_365F6(void) {
  task_9E8 = 1;
  script_3674F();
  for (;;) {
    if (task_9DC == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3661D(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  Script_charaToggleMotEnabled(CHARA_DICK, 1);
  Script_motmToggleFlags(CHARA_DICK, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_DICK, MOTM_BLEND_FRAMES_30);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_charaDisplay(CHARA_DICK, CDM_Show);
  Script_charaDisplay(SHEN_NAME('BEER'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BEER'), CDM_Show);
  Script_SetCPAMFlags(CHARA_DICK, 0);
  Script_EVT_SetPosition(CHARA_DICK, POS_XYZ, &data_45E44);
  Script_EVT_SetRotation(CHARA_DICK, 0x38000000, &data_45E50, 0);
  script_351E7(8500, -1, -1, -1);
  script_34FFE(4, 80);
  Script_PlayCharaMotion(CHARA_DICK, 30734, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  script_36D2B();
  local_F0 = Script_scenCreateTask(script_35EA3, 4, CHARA_AKIR, &data_45C94, 655, 0x3D75C28F);
  SCNF_WaitFrames_0000(50);
  Script_scenCreateTask(script_36E35, 0);
  Script_srchControl((void *)local_F0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(175);
  return;
}

__attribute__((scn_string_bindings(0, data_45E74, 4, data_45EA3, 5, data_45EB3, 6, data_45EB8)))
static void script_3674F(void) {
  int8_t local_FF;
  Script_SetCameraFlags(0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_248B7();
  script_351E7(8500, -1, -1, -1);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 7);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_LoadSND(0, 0, "system1.snd", 0);
  Script_LoadSND(0, 2, "tsm002.snd", 0);
  Script_LoadSND(0, 4, "e1yoppar.snd", 0);
  Script_LoadSND(0, 5, "qtebat.snd", 0);
  task_9E4 = script_36F2B(0x4B434944);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_45E5C);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_45E68, 0);
  script_36FEB("/scene/02/D000/", "DYQB");
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 928, 1);
  task_9DC = 0;
  Script_LoadVoiceFile(1, "E1005");
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  while (local_FF < 4) {
    *((int8_t *)task_AD4 + local_FF) = 0;
    local_FF = local_FF + 1;
  }
  SCNF_WaitFrames_0000(1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_36C4D, 4, CHARA_DICK, 1, 2, 2);
  Script_scenCreateTask(script_36B0A, 2, CHARA_AKIR, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  task_9D8 = Script_scenCreateTask(script_35389, 2, CHARA_AKIR, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_45ED8, 1, data_45EDE)))
static void script_3697F(void) {
  task_9DC = 1;
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  SCNF_WaitFrames_0000(10);
  Script_SetCameraFlags(1);
  Script_SetActionSelectorHUDElementVisibility(0);
  Script_MoveObjectToObject(SHEN_NAME('DUM0'), -1);
  Script_MoveObjectToObject(SHEN_NAME('DUM1'), -1);
  Script_ReallocateUNFO(task_9E0);
  Script_LoadVoiceFile(0, "E1005");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "DYQB");
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  script_36F97(0x4B434944, task_9E4);
  script_29D3D();
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_45EC0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_45ECC, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_srchControl((void *)task_9D8);
  script_248D6(0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static int32_t script_36B0A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_34482, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_33FEE, 6, arg0, arg1, 2, 2, &data_45C04, script_34D8B);
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 9);
  Script_SetCharaFlags(arg0, 4);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_MoveObjectToObject(SHEN_NAME('DUM0'), arg0);
  Script_MoveObjectToObject(SHEN_NAME('DUM1'), 0x4B434944);
  Script_EVT_SetPosition(SHEN_NAME('DUM0'), POS_XYZ, &data_45EE4);
  Script_EVT_SetRotation(SHEN_NAME('DUM0'), 0x38000000, &data_45EF0, 0);
  Script_EVT_SetPosition(SHEN_NAME('DUM1'), POS_XYZ, &data_45EE4);
  Script_EVT_SetRotation(SHEN_NAME('DUM1'), 0x38000000, &data_45EF0, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_9DC == 1) {
      script_27D20(arg0);
      Script_ccowUtils(arg0, CCOW_EnableAll);
      Script_srchControl((void *)task_A34[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
  }
  end:;
}

static int32_t script_36C4D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_34482, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_33FEE, 6, arg0, arg1, 0, 2, &data_45C04, script_34D8B);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 9);
  Script_SetCharaFlags(arg0, 4);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCPAMFlags(arg0, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_9DC == 1) {
      Script_SetCPAMFlags(arg0, 1);
      Script_charaSetRefboardFlagTo(arg0, 0);
      Script_charaDisplay(arg0, CDM_Hide);
      Script_srchControl((void *)task_A34[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
  }
  end:;
}

static void script_36D2B(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_charaDisplay(SHEN_NAME('BEER'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BEEW'), CDM_Hide);
  Script_ccowUtils(CHARA_DICK, CCOW_DisableCharacters);
  Script_CharHandlers(CHARA_DICK, 0, 0, 16);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  local_F4.x = 0.085f;
  (&local_F4)->y = 0.019f;
  (&local_F4)->z = -0.023f;
  Script_fixoAttachEx(SHEN_NAME('BEER'), CHARA_DICK, 18, &local_F4, &local_E8);
  return;
}

static void script_36DAD(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_charaDisplay(SHEN_NAME('BEER'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BEEW'), CDM_Hide);
  Script_ccowUtils(CHARA_DICK, CCOW_DisableCharacters);
  Script_CharHandlers(CHARA_DICK, 0, 0, 16);
  local_F4.x = 0.106f;
  (&local_F4)->y = 0.036f;
  (&local_F4)->z = -0.017f;
  local_E8.x = 34039;
  (&local_E8)->y = 0;
  (&local_E8)->z = 64459;
  Script_fixoAttachEx(SHEN_NAME('BEER'), CHARA_DICK, 18, &local_F4, &local_E8);
  return;
}

static int32_t script_36E35(void) {
  int16_t local_FE;
  Vector3 local_F0;
  local_FE = 0;
  while (local_FE < 120) {
    Script_EVT_GetPosition(CHARA_DICK, 5, &local_F0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetCharaLookAtPosition(CHARA_AKIR, 2, &local_F0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 5);
    SCNF_WaitFrames_0000(1);
    local_FE = local_FE + 1;
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -2, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  return Script_srchControl(0);
}

static void script_36EAD(int32_t arg0) {
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
    L036F0A:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L036F0A;
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  }
  (void)local_FC;
  return;
}

static int32_t script_36F2B(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  if (local_FC == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    while (local_FC == 0) {
      SCNF_WaitFrames_0000(1);
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    }
    L036F7F:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L036F7F;
  }
  return local_FC;
}

static void script_36F97(int32_t arg0, int32_t arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  return;
}

static void script_36FAE(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_36FC8(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_36FEB(const char *arg0, const char *arg1) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1)) {
  } else {
    script_36FC8(arg0, arg1);
    script_36FAE();
  }
  return;
}

static void script_3700F(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) == 0) {
  } else {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_37033(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
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

static void script_37082(void) {
  task_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  return;
}

static void script_370A2(int32_t arg0, int32_t arg1, Vector3 *arg2, Vector3i *arg3, const void *arg4) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_InterpolateAndScaleAngles(arg0, arg1, arg2, arg3, arg4);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_45F28)))
static void script_370CA(int32_t arg0, const char *arg1) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, arg0, arg1, "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  return;
}

static void script_370F3(void) {
  Script_seqEnterEvtCam(0, 0, AUTH_None);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_3710D(int32_t arg0) {
  if (arg0 == 1) {
    Script_WeatherUtils_ForFIXC(13, 0);
    Script_WeatherTaskUtil(LoadSPR);
  } else {
    Script_WeatherUtils_ForFIXC(13, 1);
  }
  return;
}

static void script_37135(void) {
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  script_36FAE();
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  return;
}

static void script_37153(void) {
  int8_t local_FF;
  if (task_10A & 0x10) {
    (void)((local_FF = 0) & 2);
  } else {
    (void)((task_10A = -1) & 2);
    (void)((local_FF = -1) & 2);
  }
  (void)(local_FF & 0x80);
  return;
}

static void script_37186(void) {
  (void)((task_10A = 0) & 2);
  return;
}

static void script_37193(int32_t arg0) {
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 1);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_faceUpdateFor(arg0, 0, 0);
  Script_faceUpdateFor(arg0, 1, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_45F2C)))
static void script_371D2(void) {
  script_248B7();
  Script_LoadVoiceFile(1, "SA1115");
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  return;
}

__attribute__((scn_string_bindings(0, data_45F33)))
static void script_37222(void) {
  Script_LoadVoiceFile(0, "SA1115");
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  script_248D6(1);
  return;
}

static void script_37283(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
  local_FC = local_F8 + 4300;
  Script_seqEnterEvtCam(local_FC, CHARA_AKIR, CHARA_AKIR);
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(30);
  local_F4 = local_F8;
  if (local_F4 == 0) {
    Script_ShowUIMessageFromID("SA1115A001");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_F4 == 1) {
    Script_ShowUIMessageFromID("SA1115A002");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("SA1115A003");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static int32_t script_37344(void) {
  int8_t local_FF;
  (void)((local_FF = 0) & 0x10);
  if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_PIGGY_BANK_TAKEN) == 0) & ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_CASSETTE_REWARD_COMPLETE) & -(Script_GetCurrentEntryPoint() == 2)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DSKI_01MIT_CUTSCENE_PHASE_A_DONE) & -(Script_GetCurrentEntryPoint() == 4)))) {
    (void)((local_FF = -1) & 0x10);
  }
  return local_FF & 0x80;
}
