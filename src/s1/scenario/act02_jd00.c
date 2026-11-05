#include "act02_jd00.h"

__attribute__((scn_string_bindings(0, data_2647C)))
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
      *((int8_t *)&*(int32_t *)&task_B0 + arg5) = 3;
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
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg5) == 1) {
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
            if (*((int8_t *)&*(int32_t *)&task_B3 + arg5) == 1) {
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
      if (*((int8_t *)&*(int32_t *)&task_B1 + arg5) != 2) {
        *((int8_t *)&*(int32_t *)&task_B1 + arg5) = 0;
      }
      *((int8_t *)&*(int32_t *)&task_B2 + arg5) = 0;
      *((int8_t *)&*(int32_t *)&task_B0 + arg5) = 0;
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
  *((int8_t *)&*(int32_t *)&task_B1 + arg5) = 0;
  *((int8_t *)&*(int32_t *)&task_B2 + arg5) = 0;
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
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 1) {
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
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 0) {
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
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 1) {
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
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 0) {
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
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 1) {
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
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 1) {
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
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 0) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        } else {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 1) {
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
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
      if (*((int8_t *)&*(int32_t *)&task_B1 + arg2) != 2) {
        *((int8_t *)&*(int32_t *)&task_B1 + arg2) = 0;
      }
      switch (arg1) {
        case 1:
          if (arg5) {
            if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
            if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
            if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        } else {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 0) {
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
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
      if (*((int8_t *)&*(int32_t *)&task_B1 + arg2) != 2) {
        *((int8_t *)&*(int32_t *)&task_B1 + arg2) = 0;
      }
      if (arg1) {
        if (arg5) {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        } else {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 0) {
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
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 0;
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
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 0;
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

__attribute__((scn_string_bindings(0, data_2651B)))
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
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 0;
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
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 0;
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

__attribute__((scn_string_bindings(1, data_26541)))
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

__attribute__((scn_string_bindings(0, data_2654B)))
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

__attribute__((scn_string_bindings(0, data_26563)))
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

__attribute__((scn_string_bindings(0, data_2657B)))
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

__attribute__((scn_string_bindings(0, data_26593)))
static void script_28EF(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&*(int32_t *)&task_B2 + arg1) == 0) {
    *((int8_t *)&*(int32_t *)&task_B2 + arg1) = 1;
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

__attribute__((scn_string_bindings(0, data_265AB)))
static void script_29E5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&*(int32_t *)&task_B2 + arg1) == 0) {
    *((int8_t *)&*(int32_t *)&task_B2 + arg1) = 1;
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

__attribute__((scn_string_bindings(0, data_265C3)))
static int32_t script_2ADB(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&*(int32_t *)&task_B1 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 1;
    if (task_B5) {
      local_FC = local_F8 = -0.001f;
    } else {
      local_FC = local_F8 = 0.0f;
    }
    while (*((int8_t *)&*(int32_t *)&task_B1 + arg1) == 1) {
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
    if (*((int8_t *)&*(int32_t *)&task_B1 + arg1) != 2) {
      for (;;) {
        if (local_FC >= 0.0f) {
          break;
        }
        if (*((int8_t *)&*(int32_t *)&task_B1 + arg1) == 2) {
          *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC + 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_265DB)))
static int32_t script_2C03(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&*(int32_t *)&task_B1 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 1;
    if (task_B5) {
      local_FC = local_F8 = 0.16f;
    } else {
      local_FC = local_F8 = 0.08f;
    }
    while (*((int8_t *)&*(int32_t *)&task_B1 + arg1) == 1) {
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
    if (*((int8_t *)&*(int32_t *)&task_B1 + arg1) != 2) {
      for (;;) {
        if (local_FC <= 0.0f) {
          break;
        }
        if (*((int8_t *)&*(int32_t *)&task_B1 + arg1) == 2) {
          *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC - 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

static void script_2D34(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((*(&task_78 + arg1) & 2) > 0) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf(data_265F3);
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

__attribute__((scn_string_bindings(0, data_27194, 8, data_271D8, 9, data_271E1, 10, data_271EA, 11, data_271F3, 12, data_271FC, 13, data_27205)))
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

__attribute__((scn_string_bindings(0, data_2720A, 1, data_2720F, 2, data_27218, 3, data_2721D, 4, data_27222, 5, data_2722B, 6, data_27234, 7, data_2723D)))
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
  task_DE = 0;
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
  task_DE = 1;
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
  if (task_DC == 0) {
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
  if (task_DC == 0) {
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
  if (task_DC == 0) {
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
  if (task_DC == 0) {
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
  if (task_DC == 0) {
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
  if (task_DC == 0) {
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
  if (task_DC == 0) {
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
  if (task_DC == 0) {
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
  if (task_DC == 0) {
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
  if (task_DC == 0) {
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
  if (task_DC == 0) {
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
      task_D0 = 5;
      task_CC = (char *)data_2727B;
      arg1 = (int32_t)task_CC;
    }
  }
  local_F8 = task_D0;
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
    Script_charaPosRotUtils(arg0, 152, &data_27280, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg0, 153, &data_2728C, 0, CHARA_PART_Set);
  } else {
    Script_charaPosRotUtils(arg0, 152, &data_2728C, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg0, 153, &data_27280, 0, CHARA_PART_Set);
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

__attribute__((scn_string_bindings(0, data_27298)))
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
            if (task_D0 == 5) {
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

__attribute__((scn_string_bindings(0, data_2729D)))
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
  task_C8 = 0x314E4143;
  task_D0 = 3;
  task_CC = (char *)data_272A2;
  task_DC = 0;
  task_DD = 0;
  task_DE = 0;
  task_E4 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_272A7, 1, data_272AE, 2, data_272BB)))
static void script_4B5C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Vector3 local_E8;
  script_4B22();
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, arg0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  task_D4 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_D4) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  }
  Script_LoadVoiceFile(1, "01JUCE");
  task_E4 = Script_LoadHandMot(2, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
  Script_scenCreateTask(script_4129, 2, arg2, "VEND");
  while (task_DE != 1) {
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

__attribute__((scn_string_bindings(0, data_272C0, 2, data_272DD)))
static void script_4CB9(int32_t arg0) {
  script_4B22();
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  task_D4 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_D4) {
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
  Script_sprintf("HandMot read by VEND\012");
  task_E4 = Script_LoadHandMot(2, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  return;
}

static void script_4D94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  while (task_DE != 1) {
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

__attribute__((scn_string_bindings(0, data_272EA, 1, data_272F1)))
static void script_4E35(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Script_charaSetRefboardFlagTo(arg3, 0);
  Script_ResetHandMotion(arg3, 0, 1, 0);
  Script_faceUpdateFor(arg3, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  if (task_E4 != 0) {
    Script_FreeTaskData(task_E4);
  }
  Script_LoadVoiceFile(0, "01JUCE");
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetGameStateTo(AUTH_None);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
  Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  if (task_D4) {
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(RYO_RestoreControl);
  }
  SCNF_WaitFrames_0000(1);
  script_419E("VEND");
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 2);
  Script_SetCPAMFlags(arg0, 1);
  task_DD = 14;
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
      task_DD = local_E4;
      local_D0 = local_E4;
      if (local_D0 == 1) {
        script_44EA(arg3, 1, (int32_t)data_272F8, -1);
        script_44A3(arg3, 1, 542, -1);
      } else if (local_D0 == 3) {
        task_DD = 1;
        Script_shenClearFreeState(0);
        Script_sicnUtils(1, 0, 0);
      } else if (local_D0 == 16) {
        Script_sicnUtils(1, 0, 0);
      } else if ((local_D0 == 2) | (local_D0 == 4)) {
        task_DD = 1;
      } else {
        switch (local_D0) {
          case 5:
            script_4246(0x3005A9, -1);
            break;
          case 6:
            script_4246(0x3405A9, 5);
            break;
          case 8:
            task_CC = (char *)data_2734A;
            task_D0 = 0;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 9:
            task_CC = (char *)data_2734F;
            task_D0 = 1;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 10:
            task_CC = (char *)data_27354;
            task_D0 = 2;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 11:
            task_CC = (char *)data_27359;
            task_D0 = 3;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 12:
            task_CC = (char *)data_2735E;
            task_D0 = 4;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
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
  script_4596(arg0, arg1, (int32_t)&data_27364, -1);
  script_45FB(arg0, arg1, (int32_t)data_27370, -1);
  script_44EA(arg0, 1, (int32_t)data_2737C, -1);
  script_44EA(arg0, 0, (int32_t)data_2737C, -1);
  script_44A3(arg0, 1, 541, -1);
  script_4433(arg0, 712, 0, -0x80000000, -1);
  script_4246(0x3105A9, 125);
  script_463F(4013, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(3, data_273E5, 4, data_273F0)))
static void script_5350(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg2 == 0) {
    script_4129(task_C8, task_CC);
  }
  script_42CE(task_C8, arg0, 18, (int32_t)&data_27388, (int32_t)&data_27394, 82);
  script_4858(task_C8, 1, 0);
  script_4596(arg0, arg1, (int32_t)&data_273A0, -1);
  script_45FB(arg0, arg1, (int32_t)data_273AC, -1);
  script_44EA(arg0, 1, (int32_t)&data_273B8, -1);
  script_44A3(arg0, 1, 545, -1);
  script_44A3(arg0, 0, 546, -1);
  script_4433(arg0, 45059, 0, -0x80000000, -1);
  script_4246(0x3905A9, 80);
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
  script_4246(0x3505A9, 108);
  script_4858(task_C8, 0, 108);
  script_444B(arg0, 45058, 0, 0x441C0000, 0, -1);
  script_4246(0x3205A9, 210);
  script_4246(0x3305A9, 330);
  script_4246(0x3205A9, 470);
  script_4246(0x3305A9, 500);
  script_4289((int32_t)"01JUCEA002", 550);
  script_4289((int32_t)"01JUCEA003", 400);
  script_463F(4015, arg1, arg1, -1);
  script_463F(4016, arg1, arg1, 575);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_16);
  script_4433(arg0, 45061, 0x42100000, 0, -1);
  script_4310(task_C8, 30);
  script_48DE(66, 32);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  script_419E(task_CC);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_2745C, 1, data_27467)))
static void script_5641(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 == 0) {
    script_4129(task_C8, task_CC);
  }
  if (task_D0 == 4) {
    script_42CE(task_C8, arg0, 18, (int32_t)&data_27414, (int32_t)&data_27420, 82);
    script_4596(arg0, arg1, (int32_t)&data_2742C, -1);
    script_45FB(arg0, arg1, (int32_t)data_27438, -1);
    script_44EA(arg0, 1, (int32_t)data_27450, -1);
    script_44A3(arg0, 1, 544, -1);
  } else {
    script_42CE(task_C8, arg0, 18, (int32_t)&data_273FC, (int32_t)&data_27408, 82);
    script_4596(arg0, arg1, (int32_t)&data_2742C, -1);
    script_45FB(arg0, arg1, (int32_t)data_27438, -1);
    script_44EA(arg0, 1, (int32_t)&data_27444, -1);
    script_44A3(arg0, 1, 545, -1);
  }
  script_4858(task_C8, 1, 0);
  script_44A3(arg0, 0, 546, -1);
  script_4433(arg0, 45059, 0, -0x80000000, -1);
  script_4246(0x3905A9, 80);
  script_4246(0x3505A9, 290);
  script_463F(4014, arg1, arg1, -1);
  script_4858(task_C8, 0, 290);
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
  if (task_D0 == 4) {
    script_4433(arg0, 45060, 0, 0, -1);
  } else {
    script_4433(arg0, 45058, 0, 0, -1);
  }
  script_4246(0x3205A9, 210);
  script_4246(0x3305A9, 330);
  script_4246(0x3205A9, 470);
  script_4246(0x3305A9, 500);
  script_4246(0x3805A9, 690);
  script_4289((int32_t)"01JUCEA002", 550);
  script_4289((int32_t)"01JUCEA003", 400);
  script_463F(4015, arg1, arg1, -1);
  script_463F(4016, arg1, arg1, 575);
  script_4310(task_C8, 690);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  script_419E(task_CC);
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
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, *(&data_27244 + local_F4))) {
      Script_EVT_GetPosition(*(&data_27244 + local_F4), -1, (Vector3 *)local_90[local_F4], ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(*(&data_27244 + local_F4), (Vector3i *)local_FEC4[local_F4], 0x40000000);
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
      script_4A53(arg0, *(&data_27244 + local_F9), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
      (void)((local_FB = 0) & 0x80);
      (void)((local_FC = 0) & 0x80);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_5D8F(int32_t arg0) {
  script_4B22();
  task_D8 = Script_scenCreateTask(script_592B, 1, arg0);
  return;
}

static void script_5DAA(void) {
  Script_srchControl((void *)task_D8);
  task_D8 = 0;
  return;
}

static void script_5DBB(void) {
  (void)task_DD;
  return;
}

static void script_5DC2(void) {
  Script_ExitCharaSelector();
  task_DC = 1;
  return;
}

static int32_t script_5DCF(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_5DED(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_5E0C(int32_t arg0, int32_t arg1) {
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

static void script_5E84(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_248 != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_130[arg2], arg3, arg4);
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

static int32_t script_6436(int32_t arg0) {
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

static void script_6507(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_F248(arg0, arg2, 0);
  return;
}

static void script_6517(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_E9DD(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_9CA3(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_F248(arg0, arg2, 0);
  return;
}

static void script_661B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_E9DD(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_130[arg2]);
    script_9CA3(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_F248(arg0, arg2, 0);
  return;
}

static void script_67BD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_E9DD(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 4) == 0) {
      script_81B0(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_248) {
        script_B5A1(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_F248(arg0, arg2, 2);
  return;
}

static void script_691E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_E9DD(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 4) == 0) {
      script_81B0(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_248) {
        script_B5A1(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_F248(arg0, arg2, 2);
  return;
}

static void script_6A7F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_E9DD(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_130[arg2]);
      script_81B0(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_248) {
        float local_F4;
        float local_F0;
        local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_E9DD(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_130[arg2]);
        script_B5A1(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_F248(arg0, arg2, 2);
  return;
}

static void script_6D1D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    script_E9DD(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_F248(arg0, arg2, 0);
  return;
}

static void script_6DEB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_912F(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_F248(arg0, arg2, 1);
  return;
}

static void script_6E52(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_912F(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_F248(arg0, arg2, 1);
  return;
}

static void script_6EB9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_E9DD(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_912F(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_F248(arg0, arg2, 1);
  return;
}

static void script_6F00(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_E9DD(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_E9DD(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_95E9(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_F248(arg0, arg2, 2);
  return;
}

static void script_7036(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  if (arg0 == task_248) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_FF7F = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_E9DD(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_F0A6(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_250)->x;
      (&local_9C)->y = task_250.y;
      (&local_9C)->z = task_250.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, CHARA_PART_Get);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, CHARA_PART_Get);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_248) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_250, *(&task_25C + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_248) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_248) {
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
      script_F0A6(arg0, 1);
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_248) {
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
    script_F248(arg0, arg2, 0);
  } else {
    script_661B(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_78F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_E9DD(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_E9DD(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_BCC2(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_F248(arg0, arg2, 0);
  return;
}

static int32_t script_7A15(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_F4 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6);
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
        local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_E9DD(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L007BD1;
            }
          }
          L007BD1:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_25C + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_250)->x, (&local_E4)->z - task_250.z);
        script_C44C(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_28490, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_28490, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_E9DD(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_130[arg2]);
      script_C524(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_28490, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_28490, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_E9DD(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_130[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_C99B(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_28490, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_28490, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_E9DD(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_130[arg2]);
          script_C524(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L0080CC;
      }
    }
  }
  L0080CC:;
  script_F248(arg0, arg2, 0);
  end:;
}

static void script_80DA(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_6436(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_28FAE)))
static void script_81B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_248) {
        local_FF78 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_5E84(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_F0A6(arg0, 0);
    local_C0.x = (&task_250)->x;
    (&local_C0)->y = task_250.y;
    (&local_C0)->z = task_250.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_FF78);
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
    if (task_248 == arg0) {
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
          script_80DA(arg0, task_180[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_248) {
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
                goto L008738;
              }
            }
            L008738:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L008776;
                }
              }
              L008776:;
              goto L008779;
            }
          }
        }
      }
      L008779:;
      if (task_248 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_248) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_248 != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_248 == arg0) {
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
                goto L008945;
              }
            }
            L008945:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L008983;
                }
              }
              L008983:;
              goto L008986;
            }
          }
          L008986:;
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
              goto L0089DF;
            }
          }
        }
      }
      L0089DF:;
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
      if (arg0 == task_248) {
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
  script_F0A6(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_130[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_248 == arg0) {
        local_80 = (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_248 == arg0) {
        local_80 = (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_248 == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L0090C2;
      }
    }
  }
  L0090C2:;
  return;
}

static void script_90C3(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_248 == arg0) {
    local_FC = task_130[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_912F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_250)->x;
  (&local_AC)->y = task_250.y;
  (&local_AC)->z = task_250.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
    local_80 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_248) {
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
        task_158[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_80 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_248) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_158[arg2] = 0;
      }
    }
    local_88 = task_130[arg2];
    local_88 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_130[arg2];
      local_88 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_158[arg2] = 0;
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
          goto L009599;
        }
      }
    }
    L009599:;
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

__attribute__((scn_string_bindings(0, data_28FCA)))
static void script_95E9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_F0A6(arg0, 0);
  local_CC.x = (&task_250)->x;
  (&local_CC)->y = task_250.y;
  (&local_CC)->z = task_250.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_248) {
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
  script_F0A6(arg0, 1);
  local_FC = task_130[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_28FE6)))
static void script_9CA3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      if (arg0 == task_248) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_248) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_180[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_180[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_248) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_248) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_248) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_180[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_180[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L00A896;
      }
    }
  }
  L00A896:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 7);
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
  script_5E84(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_F0A6(arg0, 0);
  local_D0.x = (&task_250)->x;
  (&local_D0)->y = task_250.y;
  (&local_D0)->z = task_250.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_A4);
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
    if (arg0 == task_248) {
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
      if (arg0 == task_248) {
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
            if ((task_130[arg2] == 15) & (arg4 == 0)) {
              local_FF50 = 0;
            }
            if (local_FF50) {
              script_80DA(arg0, task_180[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_248) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_248) {
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
        local_FF60 = script_5E0C((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L00AEB9;
          }
        }
        L00AEB9:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_5E0C(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_5E0C(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_248) {
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
          local_FF74 = (float)script_5DCF(script_5E0C(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_248) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_248) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_130[arg2];
        if (arg0 == task_248) {
          if (task_248 == arg0) {
            local_A0 = (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_248) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L00B365;
            }
          }
        }
      }
      L00B365:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_248) {
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
      if (arg0 != task_248) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_F0A6(arg0, 1);
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
  if (arg0 == task_248) {
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
      goto L00B5A0;
    }
  }
  L00B5A0:;
  return;
}

__attribute__((scn_string_bindings(0, data_29002)))
static void script_B5A1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_F0A6(arg0, 0);
    local_C4.x = (&task_250)->x;
    (&local_C4)->y = task_250.y;
    (&local_C4)->z = task_250.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
          local_84 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_248) {
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
                goto L00B8C5;
              }
            }
            L00B8C5:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L00B901;
                }
              }
              L00B901:;
              goto L00B904;
            }
          }
        }
      }
      L00B904:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L00B949;
          }
        }
      }
      L00B949:;
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
  script_F0A6(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_130[arg2];
    local_9C = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_E8, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_E8, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_E8, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2901E)))
static void script_BCC2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_27A70, SCND_ReadU32, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_27A70, SCND_ReadFloatBits, Script_ScenDataUtil(data_27480, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 4);
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
  script_F0A6(arg0, 0);
  local_B0.x = (&task_250)->x;
  (&local_B0)->y = task_250.y;
  (&local_B0)->z = task_250.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_248) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_248) {
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
  script_F0A6(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, CHARA_PART_Set);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, CHARA_PART_Set);
  return;
}

static void script_C44C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_2903A)))
static void script_C524(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_D4 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_28490, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_28490, SCND_ReadU32, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_28490, SCND_ReadU32, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_28490, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_28490, SCND_ReadU32, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_28490, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_28490, SCND_ReadU32, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_28490, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L00C745;
    }
  }
  L00C745:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_28490, SCND_ReadFloatBits, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_28490, SCND_ReadFloatBits, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_250)->x;
  (&local_F4)->y = task_250.y;
  (&local_F4)->z = task_250.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_248) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_248) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_29056)))
static void script_C99B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  task_274 = 0;
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
        goto L00CCA7;
      }
    }
  }
  L00CCA7:;
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
  local_B8.x = (&task_250)->x;
  (&local_B8)->y = task_250.y;
  (&local_B8)->z = task_250.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_248) {
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
      local_FF40 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_248) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_248) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    local_FF6C = 0;
    L00D772:;
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
      if (!(local_FF6C >= 20)) goto L00D772;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_248) {
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

static void script_D92E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    if (arg0 != task_248) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_6436(arg5));
    }
  }
  return;
}

static void script_DD78(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_E8 = arg1;
  task_EC = arg2;
  task_F0 = arg3;
  task_F4 = arg4;
  task_F8.mode = 0;
  task_F8.id = 0;
  task_F8.entry_idx = 0;
  task_248 = arg0;
  (void)((task_26C = -1) & 1);
  task_268 = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L00DDE2:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_1D0[local_F8] = local_FC;
  task_108[local_F8] = -1;
  task_130[local_F8] = -1;
  task_158[local_F8] = 0;
  task_180[local_F8] = 0;
  task_1A8[local_F8] = 0;
  task_1F8[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L00DDE2;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_E8, SCND_WriteU32, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_F8, task_108, task_130, (shen_name *)task_158, task_180, arg0, arg5, data_27480, data_27A70, (shen_name *)task_1D0);
  Script_scenCreateTask(script_E634, 0);
  return;
}

static void script_DF04(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_E8 = arg0;
  task_EC = arg1;
  task_F0 = arg2;
  task_F4 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_268 = 1;
  return;
}

static void script_DF38(void) {
  int32_t local_FC;
  local_FC = 0;
  L00DF40:;
  task_108[local_FC] = -1;
  task_130[local_FC] = -1;
  task_158[local_FC] = 0;
  task_180[local_FC] = 0;
  task_1A8[local_FC] = 0;
  task_1F8[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00DF40;
  return;
}

static void script_DFA1(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_248 == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_27C, 0);
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

static void script_E03E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_248 == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_27C, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_E078(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_E0DB(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_E13E(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_E8, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_E175(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_E8, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_E1AC(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_130[local_F0] == arg0) {
      local_EC = task_1D0[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[local_F0] * 13 + 1) & 0x20) {
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

static void script_E3EE(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_E8, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_E4F6(arg0);
  script_E1AC(arg0);
  return;
}

static void script_E437(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_E8, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_E504(arg0);
  script_E1AC(arg0);
  return;
}

static void script_E480(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_E8, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_E8, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_E4F6(int32_t arg0) {
  script_E480(arg0, 0);
  return;
}

static void script_E504(int32_t arg0) {
  script_E480(arg0, 1);
  return;
}

static void script_E512(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_E8, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_E5D9(void) {
  script_E512(1);
  return;
}

static void script_E5E5(void) {
  script_E512(0);
  return;
}

static void script_E5F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_E634(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_268 == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_108[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_F8)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
        script_F404(1, local_FC, local_F4, local_F8);
        if (script_F5B8(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_F404(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_F7CE(local_F4, local_F8);
    } else {
      task_268 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_E745(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_248, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_248, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_290E1, 1, data_290FA)))
static void script_E7DD(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_220[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_220[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_F4, SCND_ReadU32, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_F4, SCND_ReadU32, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_F4, SCND_ReadU32, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_F4, SCND_ReadU32, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_F4, SCND_ReadU32, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_F4, SCND_ReadU32, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_F4, SCND_ReadU32, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_F0, SCND_ReadScriptOffset, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_220[arg0] = -1;
    }
  } else {
    task_220[arg0] = -1;
  }
  return;
}

static void script_E9AC(int32_t arg0) {
  if (task_220[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_220[arg0], 1);
    task_220[arg0] = -1;
  }
  return;
}

static int32_t script_E9DD(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_248 == arg0) {
    local_F4.x = (&task_27C)->x;
    (&local_F4)->y = task_27C.y;
    (&local_F4)->z = task_27C.z;
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
        if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_E8, SCND_ReadU32, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_E8, SCND_ReadU32, arg6 * 13 + 1) & 0x80)) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L00EBC8;
        }
        L00EBC8:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00EC6B;
          }
        }
        L00EC6B:;
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
            goto L00EE17;
          }
        }
      }
    }
    L00EE17:;
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
          task_250.x = (&local_F4)->x;
          task_250.y = (&local_F4)->y;
          task_250.z = (&local_F4)->z;
          task_25C = (&local_C4)->x;
          *(&task_25C + 1) = (&local_C4)->y;
          *(&task_25C + 2) = (&local_C4)->z;
          break;
        case 5:
          task_250.x = (&local_F4)->x;
          task_250.y = (&local_F4)->y;
          task_250.z = (&local_F4)->z;
          task_25C = (&local_C4)->x;
          *(&task_25C + 1) = (&local_C4)->y;
          *(&task_25C + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_250, 0);
          task_25C = (&local_C4)->x;
          *(&task_25C + 1) = (&local_C4)->y;
          *(&task_25C + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_F0A6(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if (task_248 == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, CCOW_EnableMap);
      } else {
        Script_ccowUtils(arg0, CCOW_DisableMap);
      }
    }
  }
  return;
}

static void script_F0D8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_248 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_E8, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_E8, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_29113)))
static void script_F248(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_248 == arg0) {
    task_F8.mode = 0;
    task_F8.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_248, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_26C & 2) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_130[arg1];
        local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_130[arg1];
      local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_E8, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_108[arg1] = 1;
  task_1F8[arg1] = 0;
  task_1A8[arg1] = 0;
  task_158[arg1] = 0;
  if (task_220[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_220[arg1], 1);
    task_220[arg1] = -1;
  }
  return;
}

static void script_F404(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_E745(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_E745(arg2, arg3, local_FC);
          goto L00F4C8;
        }
      }
      L00F4C8:;
    } else {
      if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_E745(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_248, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L00F563;
          }
        }
      }
    }
  }
  L00F563:;
  if (task_1F8[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_278) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_278) & task_278) {
    Script_sicnUtils(2, 4, 0);
  }
  task_278 = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_29122)))
static int32_t script_F5B8(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg1] * 13 + 1) & 4) == 0)) {
      if (task_1F8[arg1] == 0) {
        task_108[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_158[arg1] != task_248) & task_278) {
        if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg1] * 13 + 1) & 2) == 0) {
          task_108[arg1] = 3;
          task_158[arg1] = task_248;
          task_180[arg1] = 0;
          task_F8.mode = 1;
          task_F8.entry_idx = task_130[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_248, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_108[arg1] = 6;
              task_158[arg1] = task_248;
              task_180[arg1] = 0;
              task_F8.mode = 0;
              task_F8.entry_idx = task_130[arg1];
              local_FC = 0;
            }
            goto L00F76C;
          }
        }
        L00F76C:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_248, -1, &task_27C, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_F8.id = task_248;
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

static void script_F7CE(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L00F7D6:;
  local_F8 = task_108[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L00F7F6;
      L00F7F6:;
      script_F898(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L00F816;
      L00F816:;
      script_F8F9(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L00F834;
      L00F834:;
      script_FA9E(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L00F856;
      L00F856:;
      script_FB36(local_FC, arg1);
      break;
    case 2:
      script_FC18(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00F7D6;
  return;
}

static void script_F898(int32_t arg0, int32_t arg1) {
  if (task_1F8[arg0] == 0) {
    task_1F8[arg0] = Script_scenCreateTask(script_7A15, 4, task_158[arg0], task_1D0[arg0], arg0, arg1);
    task_1A8[arg0] = task_158[arg0];
    script_E7DD(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_29131, 1, data_29143)))
static void script_F8F9(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_158[arg0])) {
    if (task_1F8[arg0] == 0) {
      if (task_158[arg0] == task_248) {
        task_F8.mode = 1;
        task_F8.entry_idx = task_130[arg0];
        goto L00F94E;
      }
      L00F94E:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_F0, SCND_ReadU32, Script_ScenDataUtil(task_F4, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
      script_F0D8(task_158[arg0], task_1D0[arg0], task_130[arg0], local_F8);
      task_1F8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_28530, SCND_ReadScriptOffset, local_F8), 4, task_158[arg0], task_1D0[arg0], arg0, local_F8);
      task_1A8[arg0] = task_158[arg0];
      if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_E7DD(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_220[arg0] = -1;
      }
    }
  } else {
    if (task_108[arg0] != 5) {
      task_108[arg0] = 1;
      task_1F8[arg0] = 0;
      task_1A8[arg0] = 0;
      task_158[arg0] = 0;
    }
  }
  return;
}

static void script_FA9E(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_1F8[arg0] == 0) & (arg2 >= 0)) {
    task_158[arg0] = task_248;
    task_F8.mode = 2;
    task_F8.entry_idx = task_130[arg0];
    task_1F8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_28530, SCND_ReadScriptOffset, arg2), 4, task_158[arg0], task_1D0[arg0], arg0, arg2);
    task_1A8[arg0] = task_158[arg0];
    script_E7DD(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_29156, 1, data_29168)))
static void script_FB36(int32_t arg0, int32_t arg1) {
  if ((task_1F8[arg0] == 0) & (arg1 >= 0)) {
    script_F0D8(task_158[arg0], task_1D0[arg0], task_130[arg0], arg1);
    task_1F8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_28530, SCND_ReadScriptOffset, arg1), 4, task_158[arg0], task_1D0[arg0], arg0, arg1);
    task_1A8[arg0] = task_158[arg0];
    if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_E7DD(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_220[arg0] = -1;
    }
  }
  return;
}

static void script_FC18(int32_t arg0) {
  int32_t local_FC;
  task_1F8[arg0] = 0;
  task_1A8[arg0] = 0;
  task_108[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg0], 2);
  if (local_FC != -1) {
    task_158[arg0] = 0;
  }
  return;
}

static void script_FC6F(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_FC97(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_FCEB(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_FD3B(int32_t arg0) {
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

static void script_FDE2(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_FD3B(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_FE05(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_FE38(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_FE05(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_FE5B(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_FE05(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_FEBA(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

__attribute__((scn_string_bindings(0, data_2A1F0)))
static void script_FECF(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_2A204)))
static void script_FEDE(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_FEED(void) {
  return;
}

static void script_FEF0(void) {
  return;
}

static void script_FEF3(void) {
  return;
}

static void script_FEF6(void) {
  return;
}

static void script_FEF9(void) {
  (void)((task_2EC = 0) & 0x20);
  return;
}

static int32_t script_FF06(void) {
  return task_2E0 = Script_freeTalkGetCurrentChara();
}

static void script_FF12(void) {
  *(&task_2E0 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_FF21(void) {
  return *(&task_2E0 + 2) = Script_freeTalkGetAndResetState();
}

__attribute__((scn_string_bindings(1, data_2A223)))
static void script_FF30(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  task_2F0 = arg0;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_100B6();
  local_F8 = arg0;
  switch (local_F8) {
    case 2:
      Script_scenCreateTask(script_10322, 0);
      Script_shenSeqUtils(0);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 3:
      Script_scenCreateTask(script_103AA, 0);
      Script_shenSeqUtils(1);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 4:
      Script_LoadSND(0, 2, "bgm109.snd", 0);
      Script_executeAicaCommand(16296, 0, 0);
      Script_executeAicaCommand(1184, 2, 96);
      local_FC = Script_scenCreateTask(script_10322, 0);
      Script_shenSeqUtils(0);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_srchControl((void *)local_FC);
      if (Script_shenSeqUtils(-7) == 0) {
        local_FC = Script_scenCreateTask(script_103AA, 0);
        Script_shenSeqUtils(1);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
      }
      Script_executeAicaCommand(2720, 2, 20);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  Script_set_val_to_1(2, 0);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 94);
  script_10254();
  return;
}

static void script_100B6(void) {
  script_101CE(CHARA_AKIR);
  if (task_2F0 == 4) {
    script_1016B(SHEN_NAME('GT01'));
    script_1016B(SHEN_NAME('GT02'));
    script_1016B(SHEN_NAME('GT03'));
    Script_EVT_SetPosition(SHEN_NAME('GT01'), POS_XYZ, &data_2A22C);
    Script_EVT_SetPosition(SHEN_NAME('GT02'), POS_XYZ, &data_2A238);
    Script_EVT_SetPosition(SHEN_NAME('GT03'), POS_XYZ, &data_2A244);
    Script_EVT_SetRotation(SHEN_NAME('GT01'), 0x38000000, &data_2A250, 0);
    Script_EVT_SetRotation(SHEN_NAME('GT02'), 0x38000000, &data_2A25C, 0);
    Script_EVT_SetRotation(SHEN_NAME('GT03'), 0x38000000, &data_2A268, 0);
  }
  return;
}

static void script_1016B(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetHeightMode(arg0, CCOW_HeightUnchanged);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_scenToggleActiveFor(arg0, 0);
  return;
}

static void script_1018F(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_ccowSetHeightMode(arg0, CCOW_HeightUnchanged);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 8, 16);
  return;
}

static void script_101CE(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_ccowSetHeightMode(arg0, CCOW_HeightUnchanged);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 8, 16);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_charaVisUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetMotnFlag(arg0, -1);
  return;
}

static void script_10254(void) {
  script_102A8(CHARA_AKIR);
  if (task_2F0 == 4) {
    script_1029D(SHEN_NAME('GT01'));
    script_1029D(SHEN_NAME('GT02'));
    script_1029D(SHEN_NAME('GT03'));
  }
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_1029D(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Hide);
  return;
}

static void script_102A8(int32_t arg0) {
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_FACE(arg0, 0, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_SetMotnFlag(arg0, 0);
  return;
}

static int32_t script_10322(void) {
  int16_t local_FE;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_lightSetSlotEnabled(8, 0);
          Script_charaDisplay(SHEN_NAME('GT01'), CDM_Show);
          Script_scenCreateTask(script_1041E, 3, SHEN_NAME('GT01'), 7, 40);
          break;
        case 50:
          Script_scenCreateTask(script_1041E, 3, SHEN_NAME('GT02'), 8, 30);
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

static int32_t script_103AA(void) {
  int16_t local_FE;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_lightSetSlotEnabled(10, 0);
          Script_charaDisplay(SHEN_NAME('GT03'), CDM_Show);
          break;
        case 50:
          Script_scenCreateTask(script_1041E, 3, SHEN_NAME('GT03'), 10, 70);
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

static int32_t script_1041E(int32_t arg0, int32_t arg1, int32_t arg2) {
  float local_FC;
  int16_t local_FA;
  local_FA = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if (local_FA <= arg2) {
        local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.2f);
        if (!(local_FC >= 1.0f)) {
          Script_charaDisplay(arg0, CDM_Show);
          Script_lightSetSlotEnabled(arg1, 0);
        } else {
          Script_charaDisplay(arg0, CDM_Hide);
          Script_lightSetSlotEnabled(arg1, 1);
        }
      } else {
        Script_charaDisplay(arg0, CDM_Hide);
        Script_lightSetSlotEnabled(arg1, 1);
        Script_srchControl(0);
      }
    } else {
      Script_srchControl(0);
    }
    local_FA = local_FA + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_104B4(int32_t arg0) {
  int32_t local_FC;
  if (arg0 != -1) {
    task_2F4 = arg0;
  }
  local_FC = task_2F4;
  if (local_FC == 0) {
    Script_gachaponUtils(0, data_2A3F0);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_2A499);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_2A5D4);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_2A709);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_2A7B0);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_2A91D);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_2AB16);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_2ABE9);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_2AC5A);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_2ACA7);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_2ACE0);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_2ADCF);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_2AE32);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_2AE87);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_2AECE);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_2AF07);
  } else {
    task_2F4 = -1;
  }
  return;
}

static int16_t script_10655(int8_t arg0, int8_t arg1) {
  int16_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_FE = -1;
  if (arg0 == -1) {
    local_FD = task_2F4;
  } else {
    local_FD = arg0;
  }
  local_F8 = local_FD;
  switch (local_F8) {
    case 0:
      local_FE = *(int16_t *)((int8_t *)&data_2A294 + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_2A2AC + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_2A2D0 + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_2A2FC + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_2A310 + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_2A338 + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_2A35C + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_2A374 + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_2A384 + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_2A38C + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_2A394 + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_2A3B8 + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_2A3C8 + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_2A3D4 + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_2A3E0 + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_2A3E8 + (arg1 << 1));
      break;
  }
  return local_FE;
}

static void script_10840(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_2A274[*(int32_t *)arg0] - data_2A284[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A274[*(int32_t *)arg0] - data_2A284[*(int32_t *)arg0]);
  return;
}

static int32_t script_1088F(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_2A274[arg0] - data_2A284[arg0]) {
    (void)1;
  } else if (arg0 == 15) {
    (void)3;
  } else {
    (void)2;
  }
  end:;
}

static void script_108E2(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
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
        goto L010953;
      }
      L010953:;
      break;
    case 3:
      local_EC = arg2;
      if (local_EC == 6) {
        (void)((local_F3 = -1) & 1);
        goto L01097A;
      }
      L01097A:;
      break;
    case 4:
      local_EC = arg2;
      if ((local_EC == 7) | (local_EC == 15) | (local_EC == 16) | (local_EC == 17) | (local_EC == 18)) {
        (void)((local_F3 = -1) & 1);
        goto L0109B9;
      }
      L0109B9:;
      break;
    case 5:
      (void)((local_F3 = -1) & 1);
      break;
    case 6:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 2) | (local_EC == 3)) {
        (void)((local_F3 = -1) & 1);
        goto L010A07;
      }
      L010A07:;
      break;
    case 8:
      local_EC = arg2;
      if (local_EC == 3) {
        (void)((local_F3 = -1) & 1);
        goto L010A2E;
      }
      L010A2E:;
      break;
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
  if (local_F3 & 8) {
    arg2 = arg2 + data_2A274[arg1];
    local_F4.x = (&local_F4)->x * arg3;
    (&local_F4)->y = (&local_F4)->y * arg3;
    (&local_F4)->z = (&local_F4)->z * arg3;
  } else {
    local_F4.x = (&local_F4)->x * arg3 * 0.33333f;
    (&local_F4)->y = (&local_F4)->y * arg3 * 0.33333f;
    (&local_F4)->z = (&local_F4)->z * arg3 * 0.33333f;
  }
  if (local_F2 & 4) {
    script_104B4(arg1);
    Script_gachaponUtils(5, arg0, arg2);
  } else {
    script_104B4(arg1);
    Script_gachaponUtils(1, arg0, arg2);
  }
  if ((((scn3_bits){.f = (&local_F4)->x}).i != 0x3F800000) & (((scn3_bits){.f = (&local_F4)->x}).i != 0)) {
    Script_hmdlApplyScale(arg0, &local_F4);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2B000)))
static void script_10B19(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = 6;
  local_FC = 0;
  while (local_FC < 6) {
    local_F4 = 1 << local_FC;
    if (local_F8 & local_F4) {
      local_D8 = *(&data_2AF40 + local_FC);
      local_E8.x = *(float *)(data_2AF88 + local_FC * 20);
      (&local_E8)->y = *(float *)(data_2AF88 + (local_FC * 20 + 4));
      (&local_E8)->z = *(float *)(data_2AF88 + (local_FC * 20 + 8));
      local_DC.x = 0;
      (&local_DC)->y = *(int32_t *)(data_2AF88 + (local_FC * 20 + 12));
      (&local_DC)->z = 0;
      Script_EVT_SetPosition(local_D8, POS_XYZ, &local_E8);
      Script_EVT_SetRotation(local_D8, 0x38000000, &local_DC, 0);
      Script_charaDisplay(local_D8, CDM_Show);
      Script_SetOrGetWOCCValue(local_D8, *((int8_t *)data_2AF88 + (local_FC * 20 + 16)));
      Script_MoveObjectToObject(*(&data_2AF70 + local_FC), *(&data_2AF40 + local_FC));
      Script_EVT_SetPosition(*(&data_2AF70 + local_FC), POS_XYZ, &data_2B018);
      Script_EVT_SetRotation(*(&data_2AF70 + local_FC), 0x38000000, &data_2B024, 0);
      Script_charaDisplay(*(&data_2AF70 + local_FC), CDM_Show);
      goto L010C90;
    }
    L010C90:;
    local_FC = local_FC + 1;
  }
  task_2F8 = 0;
  (void)((task_300 = 0) & 2);
  (void)((task_301 = 0) & 2);
  (void)((task_302 = 0) & 2);
  return;
}

static int32_t script_10CC2(int32_t arg0) {
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
  if (task_300 & 4) {
    (void)((local_DB = 0) & 0x80);
  }
  return local_DB & 8;
}

static int32_t script_10D2C(int32_t arg0) {
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
    local_D4 = 6;
    if (local_D4 & (1 << local_E0)) {
      int32_t local_D0;
      local_D0 = *(&data_2AF40 + local_E0);
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
  script_13C35(arg0, &local_F4);
  script_13C35(arg0, &local_E8);
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

static int32_t script_10F5D(void) {
  int16_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F8 = 0;
  local_FE = 0;
  while (local_FE < 6) {
    local_F4 = 6;
    if (local_F4 & (1 << local_FE)) {
      local_F0 = *(&data_2AF40 + local_FE);
      if (script_10CC2(local_F0)) {
        Script_scenToggleVisibleFor(local_F0, 1);
        Script_scenToggleVisibleFor(*(&data_2AF70 + local_FE), 1);
      } else {
        Script_scenToggleVisibleFor(local_F0, 0);
        Script_scenToggleVisibleFor(*(&data_2AF70 + local_FE), 0);
      }
      if (script_10D2C(local_F0)) {
        local_F8 = local_F0;
        task_2F8 = local_F0;
      }
    }
    local_FE = local_FE + 1;
  }
  task_2F8 = local_F8;
  return local_F8;
}

static int32_t script_1100D(void) {
  int32_t local_FC;
  int8_t local_FB;
  local_FC = 0;
  (void)((local_FB = 0) & 2);
  if (Script_IsPlayerAllowed(0)) {
    if (script_10F5D()) {
      (void)((local_FB = -1) & 2);
      if (Script_GetControllerInput(1) == 0x400) {
        local_FC = 1;
      }
    }
  }
  if (local_FB & 8) {
    if (~task_301) {
      (void)((task_301 = -1) & 2);
      Script_sicnUtils(0, 5, 1);
      Script_uiUtils(0, 6, 0);
    }
  } else {
    if (task_301 & 8) {
      (void)((task_301 = 0) & 2);
      Script_sicnUtils(1, 0, 0);
      Script_uiUtils(0, 0, 0);
    }
  }
  return local_FC;
}

__attribute__((scn_string_bindings(0, data_2B030, 2, data_2B043, 3, data_2B053)))
static int32_t script_110B0(void) {
  Script_LoadVoiceFile(1, "E1066");
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 7, "e1gachap.snd", 0x4000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "HI_GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_2B05B, 1, data_2B060, 2, data_2B066)))
static void script_11142(void) {
  Script_LoadSND(0, 7, "FREE", 0);
  Script_LoadVoiceFile(0, "E1066");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "HI_GACH");
  return;
}

__attribute__((scn_string_bindings(0, data_2B06E)))
static void script_1116F(int32_t arg0) {
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
  local_F8 = 6;
  local_FC = 0;
  while (local_FC < 6) {
    local_F4 = 1 << local_FC;
    if (local_F8 & local_F4) {
      local_F0 = *(&data_2AF40 + local_FC);
      local_EC = *(&data_2AF58 + local_FC);
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

static void script_11241(void) {
  Vector3i local_F4;
  (void)((task_300 = -1) & 8);
  Script_scenCreateTask(script_110B0, 0);
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
  (void)((task_301 = 0) & 8);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  SCNF_WaitFrames_0000(1);
  Script_uiUtils(0, 6, 0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2B078);
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

static void script_113E8(void) {
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  script_11142();
  Script_SetGameStateTo(AUTH_None);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_uiUtils(0, 0, 0);
  (void)((task_300 = 0) & 8);
  Script_scenToggleVisibleFor(SHEN_NAME('DAMY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPG'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPB'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GABS'), 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_shenClearFreeState(1);
  (void)((task_300 = 0) & 8);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_114C2(void) {
  int32_t local_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  script_11D3C(CHARA_AKIR, 1, 8, 1);
  script_11D3C(CHARA_AKIR, 0, 8, 1);
  local_F4[0] = local_F4[1] = local_F4[2] = 0;
  local_E8 = 0;
  while (local_E8 < 6) {
    if (task_2F8 == *(&data_2AF40 + local_E8)) {
      local_EC = *((int8_t *)data_2AF88 + (local_E8 * 20 + 17));
      break;
    }
    local_E8 = local_E8 + 1;
  }
  local_F0 = Script_PlayerStatsUtils(PS_GetIndexedU16Table3Slot, local_EC);
  Script_PlayerStatsUtils(PS_SetIndexedU16Table3Slot, local_EC, local_F0 + 1);
  Script_InitializeOsag(CHARA_AKIR);
  return;
}

static void script_11586(void) {
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_gachaponUtils(2, 0x4849454B);
  return;
}

static void script_115B5(int32_t arg0) {
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
  script_13C35(arg0, &local_F4);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, (&local_D0)->y, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  local_F4.x = -0.0511f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.650771f;
  script_13C35(task_2F8, &local_F4);
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

static void script_1173E(int32_t arg0, int32_t arg1) {
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
  script_13C35(task_2F8, &local_F4);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_mtwkGetSetRotation(task_2F8, &local_E8, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static int32_t script_117BF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
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

static int32_t script_11879(void) {
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
      Script_scenCreateTask(script_119B4, 1, local_FC - 100.0f);
    }
    if (-!(local_FC < 170.0f) & ~local_FA) {
      (void)((local_FA = -1) & 2);
      Script_scenCreateTask(script_11A84, 0);
    }
    if (-!(local_FC < 215.0f) & ~local_F9) {
      (void)((local_F9 = -1) & 2);
      Script_scenCreateTask(script_11B4D, 0);
    }
    if (-!(local_FC < 280.0f) & ~local_F8) {
      (void)((local_F8 = -1) & 2);
      Script_scenCreateTask(script_11BF7, 0);
    }
    if (-!(local_FC < 300.0f) & ~local_F7) {
      (void)((local_F7 = -1) & 2);
      Script_scenCreateTask(script_11C41, 0);
    }
    if (-!(local_FC < 310.0f) & ~local_F6) {
      (void)((local_F6 = -1) & 2);
      Script_scenCreateTask(script_11F2D, 0);
    }
    if (local_F6 & 0x40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_119B4(float arg0) {
  int32_t local_F4[3];
  int32_t local_F0;
  if (!(40.0f - arg0 <= 0.0f)) {
    local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, 40.0f - arg0);
    script_11D3C(CHARA_AKIR, 1, 0, local_F0);
    local_F4[0] = 0;
    local_F4[1] = -5643;
    local_F4[2] = 728;
    script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], local_F0);
  }
  script_11D3C(CHARA_AKIR, 1, 1, 16);
  local_F4[1] = -4915;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  local_F4[1] = -5643;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  Script_scenCreateTask(script_12B88, 0);
  end:;
}

static int32_t script_11A84(void) {
  int32_t local_F4[3];
  script_11D3C(CHARA_AKIR, 1, 2, 40);
  local_F4[0] = -7827;
  local_F4[1] = 4187;
  local_F4[2] = 4187;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  local_F4[0] = -7827;
  local_F4[1] = 3458;
  local_F4[2] = 4187;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  Script_scenCreateTask(script_1243B, 1, 20);
  local_F4[0] = 6735;
  local_F4[1] = 2821;
  local_F4[2] = 1456;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 16);
  SCNF_WaitFrames_0000(4);
  end:;
}

static int32_t script_11B4D(void) {
  int32_t local_F4[3];
  script_11D3C(CHARA_AKIR, 1, 3, 40);
  local_F4[0] = -7827;
  local_F4[1] = 3458;
  local_F4[2] = 4187;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 24);
  Script_scenCreateTask(script_1243B, 1, 14);
  local_F4[0] = 6735;
  local_F4[1] = 2821;
  local_F4[2] = 1456;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 16);
  Script_scenCreateTask(script_12C48, 0);
  SCNF_WaitFrames_0000(4);
  Script_scenCreateTask(script_12F67, 0);
  end:;
}

static int32_t script_11BF7(void) {
  int32_t local_F4[3];
  script_11D3C(CHARA_AKIR, 1, 4, 20);
  local_F4[0] = 2548;
  local_F4[1] = -3822;
  local_F4[2] = -2184;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 20);
  end:;
}

static int32_t script_11C41(void) {
  int32_t local_F4[3];
  script_11D3C(CHARA_AKIR, 1, 5, 20);
  local_F4[0] = -364;
  local_F4[1] = -2730;
  local_F4[2] = -182;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 20);
  local_F4[0] = -546;
  local_F4[1] = -1274;
  local_F4[2] = 2730;
  Script_scenCreateTask(script_117BF, 5, 1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 40);
  local_F4[0] = -7645;
  local_F4[1] = 3822;
  local_F4[2] = 910;
  local_F4[0] = -2419;
  local_F4[1] = 4176;
  local_F4[2] = 5642;
  Script_scenCreateTask(script_117BF, 5, 0, *(int32_t *)local_F4, local_F4[1], local_F4[2], 30);
  script_11D3C(CHARA_AKIR, 1, 6, 10);
  script_11D3C(CHARA_AKIR, 0, 6, 10);
  end:;
}

static void script_11D3C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_FF20[56];
  int32_t local_FF1C;
  int32_t local_FF18;
  int32_t local_FF14;
  local_FF18 = -1;
  local_FF14 = arg2;
  switch (local_FF14) {
    case 0:
      local_FF18 = (int32_t)data_2B088;
      break;
    case 1:
      local_FF18 = (int32_t)data_2B16C;
      break;
    case 2:
      local_FF18 = (int32_t)data_2B250;
      break;
    case 3:
      local_FF18 = (int32_t)data_2B334;
      break;
    case 4:
      local_FF18 = (int32_t)data_2B418;
      break;
    case 5:
      local_FF18 = (int32_t)data_2B4FC;
      break;
    case 6:
      local_FF18 = (int32_t)data_2B5E0;
      break;
    case 7:
      local_FF18 = (int32_t)data_2B6C4;
      break;
    case 8:
      local_FF18 = (int32_t)data_2B7A8;
      break;
    case 9:
      {
        int32_t local_FF10;
        local_FF10 = 0;
        while (local_FF10 < 57) {
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_2B4FC + (local_FF10 << 2));
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
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_2B4FC + (local_FF10 << 2));
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

static int32_t script_11F2D(void) {
  Script_seqEnterEvtCam(7008, task_2F8, task_2F8);
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
    Script_fixoAttachEx(task_2FC, CHARA_AKIR, 18, &local_F4, &local_E8);
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
    Script_fixoAttachEx(SHEN_NAME('KEIH'), task_2FC, 18, &local_F4, &local_E8);
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
    Script_fixoAttachEx(task_2FC, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 410.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('GABS'));
  Script_scenCreateTask(script_123BB, 1, SHEN_NAME('GABS'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 430.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_11D3C(CHARA_AKIR, 1, 5, 20);
  {
    int32_t unused_F4[3];
    int32_t local_E8[3];
    local_E8[0] = 909;
    local_E8[1] = -1657;
    local_E8[2] = 1431;
    Script_scenCreateTask(script_117BF, 5, 1, *(int32_t *)local_E8, local_E8[1], local_E8[2], 20);
  }
  {
    int32_t unused_F4[3];
    int32_t local_E8[3];
    local_E8[0] = -38;
    local_E8[1] = 476;
    local_E8[2] = 9888;
    Script_scenCreateTask(script_117BF, 5, 0, *(int32_t *)local_E8, local_E8[1], local_E8[2], 20);
  }
  Script_scenCreateTask(script_122C9, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 450.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(7009, task_2F8, task_2F8);
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
  if (task_303 & 1) {
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
  Script_fixoClearAttachment(task_2FC);
  Script_scenCreateTask(script_123BB, 1, task_2FC);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  end:;
}

static int32_t script_122C9(void) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F4.x = *(float *)&data_2B898;
  (&local_F4)->y = *(float *)(&data_2B898 + 1);
  (&local_F4)->z = *(float *)(&data_2B898 + 2);
  local_F0 = 20;
  while (local_F0) {
    local_F4.x = (&local_F4)->x + ((&data_2B88C)->x - *(float *)&data_2B898) / 20.0f;
    (&local_F4)->y = (&local_F4)->y + ((&data_2B88C)->y - *(float *)(&data_2B898 + 1)) / 20.0f;
    (&local_F4)->z = (&local_F4)->z + ((&data_2B88C)->z - *(float *)(&data_2B898 + 2)) / 20.0f;
    Script_fixoClearAttachment(task_2FC);
    Script_fixoAttachEx(task_2FC, CHARA_AKIR, 12, &local_F4, &data_2B8A4);
    local_F0 = local_F0 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoAttachEx(task_2FC, CHARA_AKIR, 12, &data_2B88C, &data_2B8A4);
  return Script_srchControl(0);
}

static int32_t script_123BB(int32_t arg0) {
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

static int32_t script_1243B(int32_t arg0) {
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
      if (task_2F8 == *(&data_2AF40 + local_FC)) {
        local_F8 = *(&data_2AF58 + local_FC);
        break;
      }
      local_FC = local_FC + 1;
    }
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, CHARA_PART_Get);
    local_DC = (&local_E0)->y;
    local_FC = 0;
    L0124BB:;
    if (local_FC == arg0 * 6 / 10) {
      Script_executeAicaCommand(66729, 0, 0);
    }
    Script_charaPosRotUtils(local_F8, 152, 0, &local_EC, CHARA_PART_Add);
    (&local_E0)->y = (&local_E0)->y + (Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.003f) - 0.0015f);
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, CHARA_PART_Set);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (!(local_FC >= arg0)) goto L0124BB;
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

static void script_1256D(int32_t arg0) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_D0[3];
  Vector3i local_C4;
  Script_MoveObjectToObject(SHEN_NAME('KEIH'), task_2F8);
  (&local_E8)->y = (&local_E8)->z = 0;
  local_E8.x = 0;
  Script_EVT_SetRotation(SHEN_NAME('KEIH'), 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(SHEN_NAME('KEIH'), -1);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4849454B);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_2B8B0);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_2B8BC, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_gachaponUtils(3, 0x4849454B, &data_2B8E0);
  SCNF_WaitFrames_0000(1);
  local_E0 = Script_scenCreateTask(script_127D9, 0);
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
        goto L0126BA;
      }
    }
    L0126BA:;
    Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &local_E8);
    Script_fixoClearAttachment(SHEN_NAME('KEIH'));
    Script_fixoSetAttachmentOffset(SHEN_NAME('KEIH'), 0, &data_2B8B0, &data_2B8BC);
    local_C4.x = 0;
    (&local_C4)->y = -(&local_E8)->x;
    (&local_C4)->z = 0;
    Script_fixoAttachEx(SHEN_NAME('KEIH'), SHEN_NAME('YKUR'), 18, &data_2B8C8, &local_C4);
    SCNF_WaitFrames_0000(1);
    if (local_DC & 0x400) {
      if (~task_302) {
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
  script_135A1(arg0);
  Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &data_2B8D4);
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Hide);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 0, 1, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  return;
}

static int32_t script_127D9(void) {
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

static void script_128B1(void) {
  return;
}

__attribute__((scn_string_bindings(5, data_2B91E)))
static int32_t script_128B4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  while (task_302 & 0x80) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_302 = -1) & 0x10);
  local_F8 = arg0;
  switch (local_F8) {
    case 0:
      local_FC = script_13BE3(3, "E1066A001", "E1066A002", "E1066A003");
      break;
    case 1:
      local_FC = script_13BE3(2, "E1066A004", "E1066A005");
      break;
    case 2:
      local_FC = script_13BE3(3, "E1066A006", "E1066A007", "E1066A008");
      break;
    case 3:
      local_FC = script_13BE3(3, "E1066A009", "E1066A010", "E1066A011");
      break;
    case 4:
      local_FC = script_13BE3(2, "E1066A026", "E1066A027");
      break;
  }
  if (local_FC) {
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)((task_302 = 0) & 4);
  return Script_srchControl(0);
}

static int32_t script_129CB(int16_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  while (task_302 & 0x80) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_302 = -1) & 4);
  local_F8 = task_2F4;
  local_F4 = script_10655(local_F8, arg0);
  local_EC = script_1088F(local_F8, arg0);
  local_F0 = Script_GetOwnedItemCount(local_F4, 0);
  local_FC = 0;
  if (local_F0) {
    local_FC = script_13BE3(3, "E1066A019", "E1066A020", "E1066A021");
  } else {
    local_E8 = local_EC;
    if (local_E8 == 0) {
      local_FC = script_13BE3(5, "E1066A014", "E1066A015", "E1066A016", "E1066A017", "E1066A018");
    } else if (local_E8 == 1) {
      local_FC = script_13BE3(3, "E1066A022", "E1066A025", "E1066A024");
    } else {
      if ((local_E8 == 2) | (local_E8 == 3)) {
        local_FC = Script_ShowUIMessageFromID("E1066A023");
        goto L012ACF;
      }
    }
  }
  L012ACF:;
  if (local_FC) {
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return (task_302 = 0) & 0x10;
}

static int32_t script_12AF5(void) {
  int32_t local_FC;
  local_FC = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(-100, 1);
  Script_SetStats(STATS_CASH, local_FC - 100);
  return Script_srchControl(0);
}

static void script_12B1E(void) {
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

static int32_t script_12B88(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  float local_E4;
  float local_E0;
  Script_fixoClearAttachment(0x6E496F63);
  Script_MoveObjectToObject(0x6E496F63, task_2F8);
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

static int32_t script_12C48(void) {
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
    task_2FC = 0x47504347;
  } else if (local_B8 == 1) {
    task_2FC = 0x42504347;
  } else if (local_B8 == 2) {
    task_2FC = 0x52504347;
  } else {
    task_2FC = 0x59504347;
  }
  Script_EVT_GetPosition(task_2F8, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(task_2F8, &local_E8, 0);
  local_F4.x = (&local_F4)->x + (Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.2f + ((float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y)) * -0.1f);
  (&local_F4)->y = (&local_F4)->y + 0.5f;
  (&local_F4)->z = (&local_F4)->z + (Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.2f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * -0.1f);
  Script_EVT_SetPosition(task_2FC, POS_XYZ, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('GABS'), POS_XYZ, &local_F4);
  Script_charaDisplay(task_2FC, CDM_Show);
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
    Script_EVT_SetPosition(task_2FC, POS_XYZ, &local_F4);
    Script_EVT_SetPosition(SHEN_NAME('GABS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_2FC, 0x38000000, &local_E8, 0);
    Script_EVT_SetRotation(SHEN_NAME('GABS'), 0x38000000, &local_E8, 0);
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 910;
  (&local_E8)->y = 1274;
  (&local_E8)->z = 0x8000;
  Script_mtwkGetSetRotation(task_2FC, &local_DC, 0);
  *(float *)&local_DC.x = (float)((&local_E8)->x - (&local_DC)->x) / 10.0f;
  *(float *)&(&local_DC)->y = (float)((&local_E8)->y - (&local_DC)->y) / 10.0f;
  *(float *)&(&local_DC)->z = (float)((&local_E8)->z - (&local_DC)->z) / 10.0f;
  local_BC = 10;
  while (local_BC) {
    local_E8.x = (&local_E8)->x + (&local_DC)->x;
    (&local_E8)->y = (&local_E8)->y + (&local_DC)->y;
    (&local_E8)->z = (&local_E8)->z + (&local_DC)->z;
    script_13C6D(task_2F8, &local_E8);
    Script_EVT_SetRotation(task_2FC, 0x38000000, &local_E8, 0);
    Script_EVT_SetRotation(SHEN_NAME('GABS'), 0x38000000, &local_E8, 0);
    local_BC = local_BC - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(263337, 0, 0);
  end:;
}

static int32_t script_12F67(void) {
  Vector3 local_F4;
  Script_MoveObjectToObject(CHARA_AKIR, task_2F8);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  {
    int32_t local_F0;
    local_F0 = 15;
    while (local_F0) {
      (&local_F4)->z = (&local_F4)->z + 0.002f;
      Script_MoveObjectToObject(CHARA_AKIR, task_2F8);
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
      Script_MoveObjectToObject(CHARA_AKIR, task_2F8);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_MoveObjectToObject(CHARA_AKIR, -1);
      local_F0 = local_F0 - 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static void script_13044(void) {
  Script_SetMotnFlags(CHARA_AKIR, 17, &data_2B9E8, 0x39000000);
  return;
}

static int32_t script_1305C(void) {
  for (;;) {
    script_13044();
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1306F(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 1, 1, 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, 0);
  script_11D3C(SHEN_NAME('YKUR'), 1, 9, 1);
  SCNF_WaitFrames_0000(1);
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, SHEN_NAME('KEIH'));
  script_11D3C(SHEN_NAME('YKUR'), 1, 10, 1);
  L0130E3:;
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4849454B);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_2B9F4);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_2BA00, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  SCNF_WaitFrames_0000(1);
  if (!Script_GetHandControlBit3(SHEN_NAME('YKUR'), 1)) goto L0130E3;
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, 0);
  end:;
}

static void script_1314A(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  local_EC = 0;
  local_F0 = task_2F8;
  script_11241();
  Script_sicnUtils(0, 1, 1);
  for (;;) {
    if (script_13225(local_F0, local_EC)) {
      Script_shenClearFreeState(0);
      Script_sicnUtils(2, 1, 0);
      script_13471(local_F0, local_EC);
    } else {
      Script_shenClearFreeState(0);
      Script_sicnUtils(2, 1, 0);
      if (local_EC) {
        script_1116F(0);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
        Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetGameStateTo(AUTH_None);
        SCNF_WaitFrames_0000(30);
      }
      L013200:;
      SCNF_WaitFrames_0000(1);
      if (!~task_302) goto L013200;
      break;
    }
    local_EC = local_EC + 1;
  }
  script_113E8();
  return;
}

static int32_t script_13225(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  int32_t local_E8;
  int32_t local_E4;
  local_F8 = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    (void)0;
  } else {
    script_1173E(arg0, arg1);
    Script_EVT_GetPosition(arg0, -1, &local_EC, 0);
    (&local_EC)->y = (&local_EC)->y + 0.6f;
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 0);
    if (arg1 == 0) {
      Script_scenCreateTask(script_128B4, 1, 0);
      Script_seqEnterEvtCam(7000, arg0, arg0);
    } else {
      local_F8 = Script_scenCreateTask(script_1305C, 0);
      Script_scenCreateTask(script_128B4, 1, 4);
      Script_seqEnterEvtCam(7010, arg0, arg0);
    }
    L0132C9:;
    SCNF_WaitFrames_0000(1);
    if (!~task_302) goto L0132C9;
    if (arg1 == 0) {
      local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 40, 41, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    } else {
      local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 4, 34, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    }
    Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
    if ((local_FC == 40) | (local_FC == 4)) {
      local_E8 = script_3C48(1, 100, 1);
      local_E4 = local_E8;
      if (local_E4 == 0) {
        if (arg1 == 0) {
          Script_scenCreateTask(script_128B4, 1, 1);
        }
        local_FC = 1;
      } else if (local_E4 == 1) {
        Script_scenCreateTask(script_128B4, 1, 2);
        SCNF_SetCashValue(0, 1);
        local_FC = 0;
      } else {
        local_FC = 0;
      }
    } else {
      Script_scenCreateTask(script_128B4, 1, 3);
      local_FC = 0;
    }
    if (local_F8) {
      Script_srchControl((void *)local_F8);
    }
    (void)local_FC;
  }
  end:;
}

static void script_133DE(int32_t arg0) {
  int32_t local_FC;
  if (arg0 == 0) {
    local_FC = -0x40800000;
  } else {
    local_FC = 0x42FA0000;
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  }
  script_11D3C(CHARA_AKIR, 1, 7, 1);
  script_11D3C(CHARA_AKIR, 0, 8, 1);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, -1.0f, -1.0f, local_FC, 0.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.25f);
  script_12B1E();
  Script_scenCreateTask(script_11879, 0);
  return;
}

static void script_13471(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_shenClearFreeState(0);
  script_114C2();
  local_FC = script_136B7();
  if (arg1 == 0) {
    Script_seqEnterEvtCam(7006, arg0, arg0);
    script_1116F(1);
  } else {
    Script_seqEnterEvtCam(7011, arg0, arg0);
  }
  script_133DE(arg1);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_12AF5, 0);
  Script_scenCreateTask(script_1306F, 0);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_129CB, 1, local_FC);
  Script_shenClearFreeState(1);
  script_1256D(local_FC);
  script_11586();
  script_13527();
  return;
}

static void script_13527(void) {
  Vector3 local_F4;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, -1.0f, -1.0f, 135.0f, 0.0f, 0);
  script_13044();
  Script_EVT_GetPosition(task_2F8, -1, &local_F4, 0);
  (&local_F4)->y = (&local_F4)->y + 0.6f;
  Script_seqEnterEvtCam(7005, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  return;
}

static void script_135A1(int32_t arg0) {
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
  local_C8 = script_10655(-1, arg0);
  Script_ProcessSubtitle("JINGURU01", 0);
  Script_GetCassetteFlags(local_C8, 1, 0, 0);
  SCNF_StartIconSystemMenu(local_C8);
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  return;
}

static int32_t script_136B7(void) {
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
  local_F0 = Script_SetOrGetWOCCValue(task_2F8, -1);
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
  local_EC = 2;
  if (!(local_FC >= 0.333333313f)) {
    local_EC = 0;
  } else {
    local_FC = local_FC - 0.333333313f;
    if (!(local_FC >= 0.05f)) {
      if (data_2A284[local_F0] != 0) {
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
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A274[local_F0]);
      break;
    case 1:
      local_F0 = 14;
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A274[local_F0]);
      break;
    case 2:
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A274[local_F0] - data_2A284[local_F0]);
      break;
    case 3:
      local_D8 = data_2A274[local_F0] - data_2A284[local_F0] - 1;
      local_D4 = 0;
      while (local_D4 < data_2A284[local_F0]) {
        if (Script_GetOwnedItemCount(script_10655(local_F0, local_D8 + local_D4), 0) == 0) {
          if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.666666687f)) {
            break;
          }
        }
        local_D4 = local_D4 + 1;
      }
      if (local_D4 < data_2A284[local_F0]) {
        local_F4 = data_2A274[local_F0] - local_D4 - 1;
      } else {
        local_D8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A284[local_F0]) + 1;
        local_F4 = data_2A274[local_F0] - local_D8;
      }
      break;
    case 4:
      local_F0 = 15;
      local_D8 = 0;
      while (local_D8 < data_2A274[local_F0]) {
        if (Script_GetOwnedItemCount(script_10655(local_F0, local_D8), 0) == 0) {
          if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.333333343f)) {
            break;
          }
        }
        local_D8 = local_D8 + 1;
      }
      if (local_D8 < data_2A274[local_F0]) {
        local_F4 = local_D8;
      } else {
        local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A274[local_F0]);
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
    (void)((task_303 = -1) & 2);
  } else {
    (void)((task_303 = 0) & 2);
  }
  script_108E2(SHEN_NAME('KEIH'), local_F0, local_F4, local_F8);
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Hide);
  return local_F4;
}

static int32_t script_13BE3(int32_t arg0, const char *arg1, const char *arg2, ...) {
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

static void script_13C35(int32_t arg0, Vector3 *arg1) {
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), arg0);
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, arg1);
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), -1);
  Script_EVT_GetPosition(SHEN_NAME('DAMY'), -1, arg1, 0);
  return;
}

static void script_13C6D(int32_t arg0, Vector3i *arg1) {
  int32_t unused_F4[3];
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), arg0);
  Script_EVT_SetRotation(SHEN_NAME('DAMY'), 0x38000000, arg1, 0);
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), -1);
  Script_mtwkGetSetRotation(SHEN_NAME('DAMY'), arg1, 0);
  return;
}

static int32_t script_13CA5(void) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3 local_E0;
  int32_t local_DC;
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  Script_InvestigationUtils(ShenmueUtils_SetSpecialAssetList, data_2BA50);
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_2BA60);
  local_EC.x = -13.0396f;
  (&local_EC)->y = 0.0f;
  (&local_EC)->z = 22.571501f;
  local_E0.x = -21.3396f;
  (&local_E0)->y = 0.0f;
  (&local_E0)->z = 37.871498f;
  for (;;) {
    local_FC = Script_srchUtils(-1);
    local_F8 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
    if (local_F8 == 0) {
      script_13DE7(1);
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &local_EC, &local_E0, 0)) {
      if (!(task_2B8.y >= 3.7f)) {
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 1);
      } else {
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 2);
      }
    } else {
      Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, -1);
    }
    if ((local_FC > 100) & (local_FC < 130)) {
    } else {
      local_DC = local_FC;
      if ((local_DC == 0x30484347) | (local_DC == 0x31484347) | (local_DC == 0x32484347) | (local_DC == 0x33484347) | (local_DC == 0x34484347) | (local_DC == 0x35484347)) {
        task_2F8 = local_FC;
        script_1314A();
        goto L013DDB;
      }
    }
    L013DDB:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_13DE7(int32_t arg0) {
  if ((Script_IsPlayerAllowed(0) & -(arg0 == 0)) | (arg0 == 1)) {
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_sicnUtils(1, 0, 0);
    *((int8_t *)&*(int32_t *)&task_2DC + 1) = 0;
    Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, data_2BA18);
    Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_2BF84, 60);
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
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}

static int32_t script_13EF4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetFightingStats(-1, 2) == 0) {
      local_FC = local_FC + 1;
    }
  }
  local_F8 = Script_PlayerStatsUtils(PS_GetShugyoCourseActiveFramesSlot, 1);
  local_F8 = local_F8 + local_FC;
  return Script_PlayerStatsUtils(PS_SetShugyoCourseActiveFramesSlot, 1, local_F8);
}

__attribute__((scn_string_bindings(0, data_2BF90, 3, data_2BFAA, 7, data_2BFCE)))
static int32_t script_13F4F(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int32_t local_E4;
  local_F4 = 0;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  Script_ReturnTimeDate(&local_E8);
  local_F8 = (&local_E8)->hours * 100 + (&local_E8)->minutes;
  local_E4 = local_FC;
  if (local_E4 == 1901) {
    if (local_F8 > 1100) {
      Script_LoadVoiceFile(1, "E1040");
      if ((int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f) > 0) {
        Script_ShowUIMessageFromID("E1040A028");
      } else {
        Script_ShowUIMessageFromID("E1040A029");
      }
      local_F4 = 1;
    }
  } else {
    if ((local_E4 == 2202) | (local_E4 == 2203)) {
      Script_LoadVoiceFile(1, "E1040");
      local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_E4 == 0) {
        Script_ShowUIMessageFromID("E1040A030");
      } else if (local_E4 == 1) {
        Script_ShowUIMessageFromID("E1040A031");
      } else {
        Script_ShowUIMessageFromID("E1040A032");
      }
      local_F4 = 1;
      goto L01404F;
    }
  }
  L01404F:;
  if (local_F4) {
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadVoiceFile(0, "E1040");
  }
  if ((local_F8 >= 2200) | (local_F8 <= 829)) {
    local_F4 = 1;
  }
  return local_F4;
}

static int32_t script_14091(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int32_t local_E4;
  local_F4 = 0;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  Script_ReturnTimeDate(&local_E8);
  local_F8 = (&local_E8)->hours * 100 + (&local_E8)->minutes;
  local_E4 = local_FC;
  switch (local_E4) {
    case 602:
      if (local_F8 > 1430) {
        local_F4 = 1;
      }
      break;
    case 1901:
      if (local_F8 > 1200) {
        local_F4 = 1;
      }
      break;
  }
  if (local_F8 >= 2200) {
    local_F4 = 1;
  }
  if (~local_F4 & -(local_F8 == 1900)) {
    local_F4 = 1;
  }
  return local_F4;
}

static void script_14137(void) {
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

static int32_t script_14190(void) {
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

static int32_t script_141CA(int32_t arg0) {
  Script_EVT_SetPosition(arg0, 0x78000000, &data_2BFD4);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_ccowUtils(arg0, CCOW_DisableAll);
}

static void script_141ED(void) {
  SCNF_WaitFrames_0000(2);
  while (Script_IsInEventCam() != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1420D(int32_t arg0) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 5, "e1shugyo.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static void script_14258(void) {
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

static void script_1429F(void) {
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

static void script_142F2(void) {
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

static void script_1434E(void) {
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

static void script_14395(void) {
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

__attribute__((scn_string_bindings(0, data_2C065)))
static void script_143F1(void) {
  int32_t unused_F0[4];
  if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1) == 0) {
    Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  Script_LoadVoiceFile(1, "E1040");
  Script_scenCreateTask(script_141CA, 1, SHEN_NAME('LUV2'));
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_scenCreateTask(script_1420D, 1, 1);
  Script_scenCreateTask(script_14190, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MIMAWASU_K, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_2C06B)))
static void script_144BF(void) {
  Script_vfToggleBattleState(scnPostResultsSetup);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1)) {
    Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 0);
  }
  Script_LoadVoiceFile(0, "E1040");
  while (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE, 0) != 7) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  return;
}

static void script_14551(void) {
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

static void script_1458E(void) {
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

static int32_t script_145CB(int32_t arg0) {
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

static void script_1463C(int32_t arg0) {
  Vector3i local_F4;
  Script_uiUtils(0, 15, 0);
  Script_vfSetup(0, 20, 20, 0, 0);
  Script_vfStageAssignRingBoundary(&data_2C074, 6.0f);
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
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_2C080);
  (&local_F4)->y = 0x7000;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(2903, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_141ED();
  SCN_Enqueue_VFGI(2, 99, 1.0f, 4, -1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_scenCreateTask(script_13EF4, 1, arg0);
  return;
}

static int32_t script_14857(void) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_GetFightingStats(-1, 2) == 1) {
    local_FC = 1;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    local_FC = 2;
  }
  if (script_14091()) {
    local_FC = 2;
  }
  return local_FC;
}

static void script_1489F(void) {
  script_1434E();
  return;
}

static void script_148A7(void) {
  SCN_Enqueue_VFGI(2, 99, 1.0f, 99, -1);
  return;
}

static void script_148BB(void) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_FE = 0;
  local_FF = 0;
  if (~Script_IsPlayerAllowed(0)) {
    local_FE = 1;
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_NEAR_NPC_COUNT_CLAMPED) > 0) {
    local_FE = 1;
  } else {
    if ((local_F8 > 100) & (local_F8 < 103)) {
      if (Script_scenCheckCharaExist(SHEN_NAME('YJHI'))) {
        Script_sprintf("\012 itou ga iru\012\012");
        local_FE = 1;
      }
      if (Script_scenCheckCharaExist(CHARA_MAYM)) {
        Script_sprintf("\012 mayumi ga iru\012\012");
        local_FE = 1;
      }
    }
    if (-(local_F8 > 1600) & -(local_F8 < 1902) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SECRET_OF_YIN_YANG) == 0)) {
      if (Script_scenCheckCharaExist(CHARA_YAMA)) {
        Script_sprintf("\012 yamagisi ga iru\012\012");
        local_FE = 1;
      }
    }
  }
  if (script_13F4F()) {
    local_FE = 1;
  }
  if (local_FE == 0) {
    script_143F1();
    local_FF = 1;
  }
  for (;;) {
    script_14551();
    script_1458E();
    if (local_FE == 1) {
      break;
      local_FF = 14;
    }
    SCNF_WaitFrames_0000(1);
    local_F4 = local_FF;
    if (local_F4 == 1) {
      script_14258();
      local_FF = 2;
    } else if (local_F4 == 2) {
      script_1429F();
      local_FF = 4;
    } else if ((local_F4 == 3) | (local_F4 == 13) | (local_F4 == 14)) {
      break;
    } else if ((local_F4 == 4) | (local_F4 == 12)) {
      local_FF = local_FF + script_145CB(local_FF);
    } else if (local_F4 == 5) {
      local_FE = 2;
      script_142F2();
      script_1463C(0);
      local_FF = 6;
    } else if (local_F4 == 6) {
      local_FF = local_FF + script_14857();
    } else if ((local_F4 == 7) | (local_F4 == 8)) {
      local_FF = local_FF - 7 + 9;
    } else {
      switch (local_F4) {
        case 9:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_1489F();
            local_FF = 12;
          }
          break;
        case 10:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_1489F();
            local_FF = 14;
          }
          break;
        case 11:
          script_148A7();
          local_FF = 6;
          break;
      }
    }
  }
  if (local_FE != 1) {
    script_144BF();
  }
  if (local_FE == 2) {
    script_14137();
    Script_PlayerStatsUtils(PS_SetShugyoCourseClearCountSlot, 1, Script_PlayerStatsUtils(PS_GetShugyoCourseClearCountSlot, 1) + 1);
  }
  if (local_FE != 1) {
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

static void script_14BBD(int32_t arg0, int32_t arg1) {
  task_304 = arg0;
  script_151BB(arg1);
  task_318 = 9999;
  task_31C = 9999;
  task_320 = 0;
  task_324 = 0;
  task_32D = 0;
  task_330 = 0;
  script_14EF5();
  task_334 = 0;
  task_308 = 0x52554B59;
  task_30C = 0x4C554B59;
  return;
}

static void script_14C1C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  script_14BBD(arg0, arg1);
  task_328 = Script_scenCreateTask(script_14C43, 4, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_14C43(float arg0, float arg1, float arg2, float arg3) {
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
  task_32E = 0;
  task_32F = 0;
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0x42000000);
    if (Script_CheckTargetWithinAngleRange(&local_F4, 2.0f, (int32_t)(local_D8 / 360.0f * (float)0x10000), 12743, &local_DC) & Script_CheckTargetWithinAngleRange(&local_DC, 2.0f, (&local_E8)->y, 12743, &local_F4)) {
      if (Script_IsPlayerAllowed(0) & -(task_32D == 0)) {
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
          task_32E = 1;
          script_15327();
          task_32E = 0;
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

static int32_t script_14DE1(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    task_32C = 0;
    for (;;) {
      if ((Script_GetControllerInput(1) == 0x400) | (Script_GetControllerInput(1) == 0x200)) {
        task_32C = 1;
        Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
        break;
      }
      local_FC = local_FC + 1;
      if (local_FC == 300) {
        task_32C = 1;
        Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  if (task_324) {
    int32_t local_FC;
    local_FC = 0;
    for (;;) {
      if ((task_324 == 0) | (local_FC < -200)) {
        break;
      }
      local_FC = local_FC - 5;
      Script_SetSequenceAudioVolume(0, (void *)task_324, local_FC);
      SCNF_WaitFrames_0000(1);
    }
    if (task_324) {
      Script_StartOrFreeSEQ((void *)task_324);
    }
  }
  return Script_srchControl(0);
}

static void script_14EA8(const char *arg0, int32_t arg1) {
  task_338[arg1] = Script_TELMUtils(TELM_CMD_RegisterPhoneNumber, task_304, arg0);
  if (task_310 == arg1) {
    task_318 = task_338[arg1];
  }
  if (task_314 == arg1) {
    task_31C = task_338[arg1];
  }
  return;
}

static void script_14EF5(void) {
  script_14EA8((const char *)&data_2C28C, 0);
  script_14EA8((const char *)&data_2C290, 1);
  script_14EA8((const char *)&data_2C294, 2);
  script_14EA8((const char *)&data_2C298, 7);
  script_14EA8("61564", 3);
  script_14EA8("37742", 4);
  script_14EA8("37749", 5);
  script_14EA8("22550", 6);
  script_14EA8("046861564", 8);
  script_14EA8("046837742", 9);
  script_14EA8("046837749", 10);
  script_14EA8("046822550", 11);
  script_14EA8("2?????", 12);
  script_14EA8("5?????", 13);
  script_14EA8("7?????", 14);
  script_14EA8("8?????", 15);
  script_14EA8("3?????", 16);
  script_14EA8("4?????", 17);
  script_14EA8("6?????", 18);
  script_14EA8("9?????", 19);
  script_14EA8("0?????????", 20);
  script_14EA8("1?????", 21);
  return;
}

static int32_t script_1502C(int32_t arg0) {
  if (arg0 > 0) {
    if ((arg0 == task_318) | (arg0 == task_31C)) {
      Script_scenCreateTask(script_14DE1, 0);
      for (;;) {
        Script_executeAicaCommand(394667, 0, 0);
        SCNF_WaitFrames_0000(15);
        Script_executeAicaCommand(460203, 0, 0);
        SCNF_WaitFrames_0000(15);
        if (task_32C) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      arg0 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_304);
    }
  }
  return arg0;
}

static void script_150A5(void) {
  if (task_32D == 0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 185, 1);
    }
    task_32D = script_15107();
    if (task_32D == 0) {
      task_32D = task_32D + 1;
      if (task_32E) {
        task_32F = 1;
      }
    }
  }
  return;
}

static int8_t script_15107(void) {
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

__attribute__((scn_string_bindings(0, data_2C326, 3, data_2C340, 4, data_2C34A)))
static void script_15152(int32_t arg0) {
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

static void script_151BB(int32_t arg0) {
  int32_t local_FC;
  task_310 = arg0;
  local_FC = arg0;
  if (local_FC == 4) {
    task_314 = 9;
  } else {
    task_314 = 9999;
  }
  return;
}

static void script_151E6(void) {
  Script_convSetCamInactiveFlag(0);
  if ((task_310 == 9998) | (task_310 == 9997)) {
    SCNF_SetCashValue(10, 1);
  }
  return;
}

static int32_t script_1520D(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours <= 4) {
    arg0 = -1;
  } else {
    if (((&local_F4)->hours >= 23) & ((&local_F4)->minutes >= 30)) {
      arg0 = -1;
      goto L01524B;
    }
  }
  L01524B:;
  return arg0;
}

static int8_t script_1524F(int32_t arg0) {
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

static void script_15327(void) {
  script_150A5();
  script_15334();
  return;
}

static void script_15334(void) {
  int32_t unused_F4[3];
  Vector4 local_E4;
  int32_t local_E0;
  int32_t local_DC;
  float local_D8;
  int32_t unused_D4;
  int8_t local_D3;
  uint8_t unused_D0[3];
  if (task_32D == 1) {
    Script_scenToggleActiveFor(CHARA_AKIR, 0);
    Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
    Script_scenToggleVisibleFor(task_304, 1);
    if (0) {
      Script_charaDisplay(task_30C, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_30C, CDM_Show);
        goto L0153B8;
      }
    }
    L0153B8:;
    Script_scenToggleVisibleFor(task_30C, 1);
    if (0) {
      Script_charaDisplay(task_308, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_308, CDM_Show);
        goto L0153E6;
      }
    }
    L0153E6:;
    Script_scenToggleVisibleFor(task_308, 1);
    Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, task_30C, task_308);
    local_D8 = Script_charaSetClipAngle(task_304, -1.0f);
    Script_charaSetClipAngle(task_304, 1.0f);
    SCNF_WaitFrames_0000(1);
    script_2135E();
    script_16303();
    script_1579E();
    local_E0 = 0;
    local_DC = 0;
    task_396 = 0;
    for (;;) {
      if (MMTK_ScriptMemoToggle(2, 0) == 0) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          task_396 = 1;
          Script_sicnUtils(1, 0, 0);
          Script_uiUtils(0, 0, 0);
          break;
        } else if (Script_GetControllerInput(1) == 0x400) {
          local_D3 = script_15107();
          if (local_D3) {
            script_15152(local_D3);
            Script_sicnUtils(1, 0, 0);
            Script_uiUtils(0, 0, 0);
            break;
          } else {
            Script_sicnUtils(1, 0, 0);
            script_1579E();
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
            goto L01553F;
          }
        }
        L01553F:;
        local_E0 = local_E0 + 1;
      } else {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          MMTK_ScriptMemoToggle(1, 0);
          goto L015563;
        }
      }
      L015563:;
      SCNF_WaitFrames_0000(1);
    }
    if ((task_310 == 9998) | (task_310 == 4)) {
      script_216CD();
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      Script_convSetCamInactiveFlag(0);
      Script_SetCPAMFlags(CHARA_AKIR, 1);
      Script_charaSetRefboardFlagTo(task_308, 0);
      Script_charaSetRefboardFlagTo(task_30C, 0);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_SetGameStateTo(AUTH_None);
      Script_SetCameraFieldOfView(10922);
      Script_scenToggleActiveFor(CHARA_AKIR, 1);
    }
    script_2145D();
    Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
    Script_ryoControl(RYO_RestoreControl);
    Script_charaSetRefboardFlagTo(task_308, 0);
    Script_charaSetRefboardFlagTo(task_30C, 0);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
    Script_charaSetClipAngle(task_304, local_D8);
    Script_scenToggleVisibleFor(task_304, 0);
    if (-1) {
      Script_charaDisplay(task_308, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_308, CDM_Show);
        goto L01565E;
      }
    }
    L01565E:;
    Script_scenToggleVisibleFor(task_308, 0);
    if (-1) {
      Script_charaDisplay(task_30C, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_30C, CDM_Show);
        goto L01568C;
      }
    }
    L01568C:;
    Script_scenToggleVisibleFor(task_30C, 0);
    task_32D = 0;
    task_32F = 0;
  } else {
    if ((task_32D == -1) | (task_32D == -10) | (task_32D == -15)) {
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
      script_2135E();
      script_15152(task_32D);
      script_2145D();
      Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_ryoControl(RYO_RestoreControl);
      Script_convSetCamInactiveFlag(1);
      Script_SetGameStateTo(AUTH_None);
      task_32D = 0;
      task_32F = 0;
      goto L01579D;
    }
  }
  L01579D:;
  return;
}

__attribute__((scn_string_bindings(1, data_2C35C, 7, data_2C394, 8, data_2C39A, 13, data_2C3C8, 14, data_2C3CE, 15, data_2C3D4, 16, data_2C3DA, 18, data_2C3EA, 21, data_2C408, 23, data_2C418, 25, data_2C42C, 28, data_2C446)))
static void script_1579E(void) {
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
  Script_TELMUtils(TELM_CMD_LoadHandMotionAndSetup, task_304);
  local_E4 = 0;
  task_397 = 0;
  for (;;) {
    task_394 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_304);
    task_394 = script_1502C(task_394);
    Script_ReturnTimeDate(&local_F4);
    if (MMTK_ScriptMemoToggle(2, 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
      Script_sprintf("メモ帳消去\012");
      MMTK_ScriptMemoToggle(1, 0);
    }
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) & -(task_394 != -4) & -(task_394 != 0) & -(local_E4 == 0)) {
      local_E4 = 1;
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
      task_394 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_304);
    }
    if ((task_394 == *task_338) | (task_394 == task_338[1])) {
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_163C1();
      if (task_394 == *task_338) {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
        if (local_D0 == 0) {
          task_324 = Script_ShowUIMessageFromID("E1032A001");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else if (local_D0 == 1) {
          task_324 = Script_ShowUIMessageFromID("E1032A002");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else {
          task_324 = Script_ShowUIMessageFromID("E1032A003");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        }
      } else {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
        if (local_D0 == 0) {
          task_324 = Script_ShowUIMessageFromID("E1032A004");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else {
          task_324 = Script_ShowUIMessageFromID("E1032A005");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        }
      }
      Script_LoadVoiceFile(0, "E1032");
      script_151E6();
    }
    if ((task_394 == task_338[7]) | (task_394 == task_338[2]) | (task_394 == task_338[3]) | (task_394 == task_338[8]) | (task_394 == task_338[4]) | (task_394 == task_338[9]) | (task_394 == task_338[5]) | (task_394 == task_338[10]) | (task_394 == task_338[6]) | (task_394 == task_338[11])) {
      local_EB = 2;
      local_EA = 1;
      if (task_394 == task_338[7]) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_OPERATOR_ADDRESS_LOOKUP_REFUSED)) {
          int32_t local_D0;
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
          SCNF_WaitFrames_0000(1);
          Script_LoadVoiceFile(1, "E1032");
          local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
          if (local_D0 == 0) {
            task_324 = Script_ShowUIMessageFromID("E1032A061");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
          } else if (local_D0 == 1) {
            task_324 = Script_ShowUIMessageFromID("E1032A062");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
          } else if (local_D0 == 2) {
            task_324 = Script_ShowUIMessageFromID("E1032A063");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
          } else {
            task_324 = Script_ShowUIMessageFromID("E1032A064");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
          }
          Script_LoadVoiceFile(0, "E1032");
          script_151E6();
          local_EA = 0;
        }
      }
      if (task_394 == task_338[2]) {
        local_EB = 1;
      }
      if ((task_394 == task_338[4]) | (task_394 == task_338[9])) {
        local_E9 = script_1520D(local_E9);
        if (local_E9 == -1) {
          task_394 = -10;
          local_EA = 0;
        }
      }
      if ((task_394 == task_338[3]) | (task_394 == task_338[8])) {
        local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0);
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) | (local_F0 >= 16)) {
          local_E6 = script_1520D(local_E6);
          if (local_E6 == -1) {
            task_394 = -10;
            local_EA = 0;
          }
        } else {
          task_394 = -10;
          local_EA = 0;
        }
        if ((local_EC >= 131) & (local_EC <= 154)) {
          task_394 = -10;
          local_EA = 0;
        }
      }
      if ((task_394 == task_338[5]) | (task_394 == task_338[10])) {
        local_E8 = script_18C6D();
        switch (local_E8) {
          case -1:
            task_394 = -10;
            local_EA = 0;
            break;
          case -2:
            task_394 = -11;
            local_EA = 0;
            break;
        }
      }
      if ((task_394 == task_338[6]) | (task_394 == task_338[11])) {
        local_E7 = script_19F94();
        local_E7 = script_1520D(local_E7);
        if (local_E7 == -1) {
          task_394 = -10;
          local_EA = 0;
        }
      }
      if (local_EA) {
        if (script_1524F(local_EB)) {
          Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
          Script_shenClearFreeState(0);
          Script_LoadVoiceFile(1, "E1032");
          script_163C1();
          script_214D0();
          local_E5 = 0;
          if (task_394 == task_338[7]) {
            script_163E6();
          }
          if (task_394 == task_338[2]) {
            script_16B2D();
          }
          if ((task_394 == task_338[5]) | (task_394 == task_338[10])) {
            script_18E23(local_E8);
          }
          if ((task_394 == task_338[6]) | (task_394 == task_338[11])) {
            script_1A529(local_E7);
          }
          if ((task_394 == task_338[3]) | (task_394 == task_338[8])) {
            local_E5 = script_1EEAF();
          }
          if ((task_394 == task_338[4]) | (task_394 == task_338[9])) {
            script_1E3E7();
          }
          script_21795();
          if (local_E5) {
            script_1F436();
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED, 1);
          }
          Script_LoadVoiceFile(0, "E1032");
          script_216B9();
          Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
          Script_shenClearFreeState(1);
          task_394 = 0;
        } else {
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
          script_151E6();
        }
      }
    }
    if (task_394 == -10) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_163C1();
      if (script_1524F(6)) {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
        if (local_D0 == 0) {
        } else if (local_D0 == 1) {
          task_324 = Script_ShowUIMessageFromID("E1032A031");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else if (local_D0 == 2) {
          task_324 = Script_ShowUIMessageFromID("E1032A029");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else if (local_D0 == 3) {
          task_324 = Script_ShowUIMessageFromID("E1032A032");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else {
          task_324 = Script_ShowUIMessageFromID("E1032A033");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        }
      }
      Script_LoadVoiceFile(0, "E1032");
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
      script_151E6();
    }
    if (task_394 == -11) {
      int32_t local_D0;
      Script_scenCreateTask(script_14DE1, 0);
      script_163C1();
      for (;;) {
        Script_executeAicaCommand(394667, 0, 0);
        SCNF_WaitFrames_0000(15);
        Script_executeAicaCommand(460203, 0, 0);
        SCNF_WaitFrames_0000(15);
        if (task_32C) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_D0 == 0) {
      } else if (local_D0 == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032A028");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_D0 == 2) {
        task_324 = Script_ShowUIMessageFromID("E1032A029");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A030");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
      task_394 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_304);
      script_151E6();
    }
    if ((task_394 == task_338[12]) | (task_394 == task_338[13]) | (task_394 == task_338[14]) | (task_394 == task_338[15]) | (task_394 == task_338[16]) | (task_394 == task_338[17]) | (task_394 == task_338[18]) | (task_394 == task_338[19]) | (task_394 == task_338[20]) | (task_394 == task_338[21])) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_163C1();
      Script_scenCreateTask(script_14DE1, 0);
      for (;;) {
        task_324 = Script_ShowUIMessageFromID("E1032G001");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        if (task_32C) {
          break;
        }
        task_324 = Script_ShowUIMessageFromID("E1032G002");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        if (task_32C) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "E1032");
      task_394 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_304);
      script_151E6();
    }
    if (task_394 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(6219, task_304, task_304);
  script_16303();
  Script_sicnUtils(0, 5, 1);
  Script_uiUtils(0, 9, 0);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_16303(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(task_304, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(task_304, &local_E8, 0);
  local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.75f;
  (&local_F4)->y = -0.3f;
  (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.75f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  (&local_DC)->y = (&local_E8)->y;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  Script_InitializeOsag(CHARA_AKIR);
  return;
}

static void script_163C1(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_GetStrayCatSceneVisitCountSlot, 2);
  local_FC = local_FC + 1;
  Script_PlayerStatsUtils(PS_SetStrayCatSceneVisitCountSlot, 2, local_FC);
  return;
}

__attribute__((scn_string_bindings(0, data_2C44C, 5, data_2C47E, 6, data_2C488, 7, data_2C492, 8, data_2C49C, 14, data_2C4D8, 16, data_2C4EC, 17, data_2C4F6, 18, data_2C500, 19, data_2C50A, 21, data_2C51E, 22, data_2C528, 23, data_2C532, 24, data_2C53C, 25, data_2C546)))
static void script_163E6(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0)) {
    int32_t local_F8;
    task_320 = 81;
    Script_scenCreateTask(script_209DC, 0);
    task_324 = Script_ShowUIMessageFromID("E1032I001");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A052");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032I007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032I008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    local_FC = SCNF_RequestActionSelectorFromParams_0008(27, 28, 29, 30, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
    task_320 = 82;
    local_F8 = local_FC;
    switch (local_F8) {
      case 27:
        task_324 = Script_ShowUIMessageFromID("E1032A097");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        break;
      case 28:
        task_324 = Script_ShowUIMessageFromID("E1032A055");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I012");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A056");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I013");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A057");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE, 1);
        break;
      case 29:
        task_324 = Script_ShowUIMessageFromID("E1032A059");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        break;
      case 30:
        task_324 = Script_ShowUIMessageFromID("E1032A060");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        break;
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_OPERATOR_ADDRESS_LOOKUP_REFUSED, 1);
  } else {
    task_320 = 80;
    Script_scenCreateTask(script_209DC, 0);
    task_324 = Script_ShowUIMessageFromID("E1032I001");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F8 == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032A007");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A006");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
      if (local_F8 == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032I002");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_F8 == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032I003");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_F8 == 2) {
        task_324 = Script_ShowUIMessageFromID("E1032I004");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_F8 == 3) {
        task_324 = Script_ShowUIMessageFromID("E1032I005");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032I006");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F8 == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032A008");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A009");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(5, data_2C5DC, 6, data_2C5E6, 7, data_2C5F0, 8, data_2C5FA, 9, data_2C604, 10, data_2C60E, 11, data_2C618, 12, data_2C622, 13, data_2C62C, 14, data_2C636, 15, data_2C640, 16, data_2C64A, 17, data_2C654, 18, data_2C65E, 19, data_2C668, 20, data_2C672, 21, data_2C67C, 22, data_2C686, 23, data_2C690, 24, data_2C69A, 25, data_2C6A4, 26, data_2C6AE, 27, data_2C6B8, 28, data_2C6C2, 29, data_2C6CC, 30, data_2C6D6, 31, data_2C6E0, 32, data_2C6EA, 33, data_2C6F4, 34, data_2C6FE, 35, data_2C708, 36, data_2C712, 37, data_2C71C, 38, data_2C726, 39, data_2C730, 40, data_2C73A, 41, data_2C744, 42, data_2C74E, 43, data_2C758, 44, data_2C762, 45, data_2C76C, 46, data_2C776, 47, data_2C780, 48, data_2C78A, 49, data_2C794, 50, data_2C79E, 51, data_2C7A8, 52, data_2C7B2, 53, data_2C7BC, 54, data_2C7C6, 55, data_2C7D0, 56, data_2C7DA, 57, data_2C7E4, 58, data_2C7EE, 59, data_2C7F8, 60, data_2C802, 61, data_2C80C, 62, data_2C816, 63, data_2C820, 64, data_2C82A, 65, data_2C834, 66, data_2C83E, 67, data_2C848, 68, data_2C852, 69, data_2C85C, 70, data_2C866, 71, data_2C870, 72, data_2C87A, 73, data_2C884, 74, data_2C88E, 75, data_2C898, 76, data_2C8A2, 77, data_2C8AC, 78, data_2C8B6, 79, data_2C8C0, 80, data_2C8CA, 81, data_2C8D4, 82, data_2C8DE, 83, data_2C8E8, 84, data_2C8F2, 85, data_2C8FC, 86, data_2C906, 87, data_2C910, 88, data_2C91A, 89, data_2C924, 90, data_2C92E, 91, data_2C938, 92, data_2C942, 93, data_2C94C, 94, data_2C956, 95, data_2C960, 96, data_2C96A, 97, data_2C974, 98, data_2C97E, 99, data_2C988, 100, data_2C992, 101, data_2C99C, 102, data_2C9A6, 103, data_2C9B0, 104, data_2C9BA, 105, data_2C9C4, 106, data_2C9CE, 107, data_2C9D8, 108, data_2C9E2, 109, data_2C9EC, 110, data_2C9F6, 111, data_2CA00, 112, data_2CA0A, 113, data_2CA14, 114, data_2CA1E, 115, data_2CA28, 116, data_2CA32, 117, data_2CA3C, 118, data_2CA46, 119, data_2CA50, 120, data_2CA5A, 121, data_2CA64, 122, data_2CA6E, 123, data_2CA78, 124, data_2CA82, 125, data_2CA8C, 126, data_2CA96, 127, data_2CAA0, 128, data_2CAAA, 129, data_2CAB4, 130, data_2CABE, 131, data_2CAC8, 132, data_2CAD2, 133, data_2CADC, 134, data_2CAE6, 135, data_2CAF0, 136, data_2CAFA, 137, data_2CB04, 138, data_2CB0E, 139, data_2CB18, 140, data_2CB22, 141, data_2CB2C, 142, data_2CB36, 143, data_2CB40, 144, data_2CB4A, 145, data_2CB54, 146, data_2CB5E, 147, data_2CB68, 148, data_2CB72, 149, data_2CB7C, 150, data_2CB86, 151, data_2CB90, 152, data_2CB9A, 153, data_2CBA4, 154, data_2CBAE, 155, data_2CBB8, 156, data_2CBC2, 157, data_2CBCC, 158, data_2CBD6, 159, data_2CBE0, 160, data_2CBEA, 161, data_2CBF4, 162, data_2CBFE)))
static void script_16B2D(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  task_320 = 70;
  Script_scenCreateTask(script_209DC, 0);
  task_324 = Script_ShowUIMessageFromID("E1032K001");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  task_324 = Script_ShowUIMessageFromID("E1032K002");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  task_324 = Script_ShowUIMessageFromID("E1032K019");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  task_324 = Script_ShowUIMessageFromID("E1032K020");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  task_324 = Script_ShowUIMessageFromID("E1032K022");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
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
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 2) {
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 3) {
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 4) {
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 5) {
    task_324 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 6) {
    task_324 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 7) {
    task_324 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 8) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 9) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 10) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 11) {
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 12) {
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 13) {
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 14) {
    task_324 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 15) {
    task_324 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 16) {
    task_324 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 17) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 18) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 19) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 20) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 21) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 22) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 23) {
    task_324 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 24) {
    task_324 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 25) {
    task_324 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 26) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 27) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 28) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 29) {
    task_324 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 30) {
    task_324 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 31) {
    task_324 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 32) {
    task_324 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 33) {
    task_324 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 34) {
    task_324 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 35) {
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 36) {
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 37) {
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 38) {
    task_324 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 39) {
    task_324 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 40) {
    task_324 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 41) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 42) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 43) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 44) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 45) {
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 46) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 47) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 48) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 54) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 50) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 51) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 52) {
    task_324 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 53) {
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 54) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 55) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 56) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 57) {
    task_324 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 58) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 59) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 60) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 61) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 62) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 63) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 64) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 65) {
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 66) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 67) {
    task_324 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else {
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  }
  return;
}

static int8_t script_18BB4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static int8_t script_18C6D(void) {
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
  if (script_18BB4(8, 30, 15, 10)) {
    local_EE = 0;
  } else if (script_18BB4(15, 10, 20, 30)) {
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
      goto L018D62;
    }
  }
  L018D62:;
  if (local_EF == 0) {
    local_EF = script_1520D(local_EF);
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

__attribute__((scn_string_bindings(0, data_2CC08, 8, data_2CC58, 9, data_2CC62, 14, data_2CC94, 17, data_2CCB2, 18, data_2CCBC, 19, data_2CCC6, 21, data_2CCDA, 25, data_2CD02, 27, data_2CD16, 33, data_2CD52, 40, data_2CD98, 41, data_2CDA2, 42, data_2CDAC, 43, data_2CDB6, 44, data_2CDC0, 48, data_2CDE8, 49, data_2CDF2, 52, data_2CE10, 53, data_2CE1A, 57, data_2CE42, 59, data_2CE56, 61, data_2CE6A, 62, data_2CE74, 70, data_2CEC4, 75, data_2CEF6, 76, data_2CF00, 77, data_2CF0A, 82, data_2CF3C, 83, data_2CF46, 84, data_2CF50, 85, data_2CF5A, 86, data_2CF64, 87, data_2CF6E, 88, data_2CF78, 92, data_2CFA0, 93, data_2CFAA)))
static void script_18E23(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 1:
      task_320 = 170;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032L001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A098");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A099");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L003");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A100");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 2:
      task_320 = 180;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032L001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L007");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L009");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A102");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 3:
      task_320 = 190;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032L011");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L012");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L013");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L014");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L015");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 191;
      task_324 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A105");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A106");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 4:
      task_320 = 200;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032L018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A107");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A108");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L020");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A109");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L022");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A110");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L023");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 5:
      task_320 = 210;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032M001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A112");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M003");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A113");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A114");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 6:
      task_320 = 220;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032M006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A115");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A122");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A123");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M020");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A114");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 7:
      task_320 = 230;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032N001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A124");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A125");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032N002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A126");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032N003");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 231;
      task_324 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A128");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032N004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A129");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032N005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 8:
      task_320 = 240;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032L011");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A117");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L024");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L025");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L026");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A118");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L023");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 9:
      task_324 = Script_ShowUIMessageFromID("E1032M001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A112");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M015");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A119");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A118");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
  }
  return;
}

static int8_t script_19F94(void) {
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
    if (script_18BB4(8, 30, 21, 0)) {
      local_EF = 1;
    } else {
      local_EF = 2;
    }
  } else if ((local_F0 == 13) | (local_F0 == 174)) {
    if (script_18BB4(8, 30, 22, 0)) {
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
      goto L01A30D;
    }
  }
  L01A30D:;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    local_EF = -1;
  } else if (script_18BB4(8, 30, 21, 0)) {
    local_EF = 1;
  } else {
    local_EF = 2;
  }
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
        goto L01A4B7;
      }
    }
  }
  L01A4B7:;
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

__attribute__((scn_string_bindings(0, data_2CFBE, 1, data_2CFC8, 2, data_2CFD2, 4, data_2CFE6, 5, data_2CFF0, 6, data_2CFFA, 7, data_2D004, 8, data_2D00E, 9, data_2D018, 10, data_2D022, 11, data_2D02C, 12, data_2D036, 13, data_2D040, 15, data_2D054, 16, data_2D05E, 17, data_2D068, 18, data_2D072, 19, data_2D07C, 20, data_2D086, 21, data_2D090, 22, data_2D09A, 23, data_2D0A4, 25, data_2D0B8, 27, data_2D0CC, 28, data_2D0D6, 31, data_2D0F4, 32, data_2D0FE, 33, data_2D108, 38, data_2D13A, 39, data_2D144, 40, data_2D14E, 41, data_2D158, 43, data_2D16C, 44, data_2D176, 45, data_2D180, 56, data_2D1EE, 57, data_2D1F8, 58, data_2D202, 59, data_2D20C, 60, data_2D216, 61, data_2D220, 64, data_2D23E, 65, data_2D248, 66, data_2D252, 67, data_2D25C, 68, data_2D266, 69, data_2D270, 70, data_2D27A, 71, data_2D284, 72, data_2D28E, 74, data_2D2A2, 76, data_2D2B6, 78, data_2D2CA, 79, data_2D2D4, 80, data_2D2DE, 81, data_2D2E8, 82, data_2D2F2, 84, data_2D306, 86, data_2D31A, 87, data_2D324, 88, data_2D32E, 89, data_2D338, 90, data_2D342, 91, data_2D34C, 92, data_2D356, 93, data_2D360, 94, data_2D36A, 96, data_2D37E, 99, data_2D39C, 100, data_2D3A6, 101, data_2D3B0, 102, data_2D3BA, 103, data_2D3C4, 104, data_2D3CE, 106, data_2D3E2, 107, data_2D3EC, 111, data_2D414, 113, data_2D428, 115, data_2D43C, 116, data_2D446, 117, data_2D450, 118, data_2D45A, 119, data_2D464, 120, data_2D46E, 125, data_2D4A0, 127, data_2D4B4, 128, data_2D4BE, 129, data_2D4C8, 132, data_2D4E6, 133, data_2D4F0, 134, data_2D4FA, 135, data_2D504, 136, data_2D50E, 137, data_2D518, 138, data_2D522, 139, data_2D52C, 140, data_2D536, 141, data_2D540, 142, data_2D54A, 143, data_2D554, 144, data_2D55E, 150, data_2D59A, 158, data_2D5EA, 159, data_2D5F4, 160, data_2D5FE, 161, data_2D608, 163, data_2D61C, 172, data_2D676, 173, data_2D680, 174, data_2D68A, 175, data_2D694, 182, data_2D6DA, 185, data_2D6F8, 186, data_2D702, 187, data_2D70C, 198, data_2D77A, 200, data_2D78E, 201, data_2D798, 208, data_2D7DE, 209, data_2D7E8, 216, data_2D82E, 224, data_2D87E, 225, data_2D888, 226, data_2D892, 232, data_2D8CE, 234, data_2D8E2, 235, data_2D8EC, 237, data_2D900, 239, data_2D914, 244, data_2D946, 245, data_2D950, 256, data_2D9BE, 258, data_2D9D2, 260, data_2D9E6, 261, data_2D9F0, 265, data_2DA18, 269, data_2DA40, 271, data_2DA54, 273, data_2DA68, 274, data_2DA72, 275, data_2DA7C, 276, data_2DA86, 277, data_2DA90, 278, data_2DA9A, 280, data_2DAAE, 282, data_2DAC2, 285, data_2DAE0, 288, data_2DAFE, 289, data_2DB08, 290, data_2DB12, 295, data_2DB44, 297, data_2DB58, 299, data_2DB6C, 300, data_2DB76, 301, data_2DB80, 302, data_2DB8A, 303, data_2DB94, 304, data_2DB9E, 308, data_2DBC6, 311, data_2DBE4, 313, data_2DBF8, 315, data_2DC0C, 316, data_2DC16, 317, data_2DC20, 318, data_2DC2A, 331, data_2DCAC)))
static void script_1A529(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F4 = arg0;
  switch (local_F4) {
    case 1:
      task_320 = 90;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032O001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A136");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A137");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O007");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A138");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A139");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 2:
      task_320 = 90;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032O001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A136");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A140");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O007");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A138");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A139");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 3:
      task_320 = 90;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P003");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A141");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A142");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SORRY_ABOUT_OTHER_DAY_CALL, 1);
      break;
    case 4:
      task_320 = 100;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A143");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P007");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A144");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 101;
      task_324 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P009");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FLOWERS_THANKS_CALL, 1);
      break;
    case 5:
      task_320 = 110;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A145");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P011");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A146");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P012");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A147");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P013");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A148");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P014");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_320 = 111;
      task_324 = Script_ShowUIMessageFromID("E1032A149");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P015");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_AFTER_RESCUE_CALL, 1);
      break;
    case 6:
      task_320 = 120;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A150");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A151");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 946, 1);
      break;
    case 7:
      task_320 = 130;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_320 = 131;
      task_324 = Script_ShowUIMessageFromID("E1032A153");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P022");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P023");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P024");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SPRUNG_ON_YOU_CALL, 1);
      break;
    case 8:
      task_320 = 140;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P025");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A155");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A156");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 141;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P027");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 9:
      task_320 = 150;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P028");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A158");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P030");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 151;
      task_324 = Script_ShowUIMessageFromID("E1032A159");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P066");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 10:
      task_320 = 160;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P032");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A160");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P033");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_320 = 161;
      if ((local_F8 == 13) | (local_F8 == 174)) {
        task_324 = Script_ShowUIMessageFromID("E1032P034");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
        local_F4 = local_FF;
        switch (local_F4) {
          case 0:
            task_324 = Script_ShowUIMessageFromID("E1032P080");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
            break;
          case 1:
            task_324 = Script_ShowUIMessageFromID("E1032P081");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
            break;
        }
      }
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P035");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A141");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P036");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 11:
      task_320 = 162;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P025");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A160");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P033");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_320 = 163;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F4 = local_FF;
      switch (local_F4) {
        case 0:
          task_324 = Script_ShowUIMessageFromID("E1032P082");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 1:
          task_324 = Script_ShowUIMessageFromID("E1032P083");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 2:
          task_324 = Script_ShowUIMessageFromID("E1032P084");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032P085");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
      }
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P086");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 12:
      task_320 = 370;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A165");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P037");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P120");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A173");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 371;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P027");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 537, 1);
      break;
    case 13:
      task_320 = 380;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A170");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P044");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A200");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A172");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P045");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A173");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P046");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P047");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 381;
      task_324 = Script_ShowUIMessageFromID("E1032A201");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P049");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A176");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P050");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A177");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P051");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SHRINE_KITTEN_CALL, 1);
      break;
    case 14:
      task_320 = 390;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A178");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P052");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A179");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A180");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A181");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 391;
      task_324 = Script_ShowUIMessageFromID("E1032P053");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P087");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P055");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A182");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P051");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KITTEN_RECOVERED_CALL, 1);
      break;
    case 15:
      task_320 = 400;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A183");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P038");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A184");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P056");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P088");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P058");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A220");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A185");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P059");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KEEP_KITTEN_CALL, 1);
      break;
    case 16:
      task_320 = 410;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P060");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A186");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P061");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A187");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P062");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 411;
      task_324 = Script_ShowUIMessageFromID("E1032A188");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P063");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(40);
      task_324 = Script_ShowUIMessageFromID("E1032P064");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P065");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A189");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P066");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A190");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_STAY_AWAY_FROM_HARBOR_CALL, 1);
      break;
    case 17:
      task_320 = 270;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P067");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A191");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A192");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P069");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A193");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TALK_TO_INE_CALL, 1);
      break;
    case 24:
      task_320 = 280;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P070");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A194");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P071");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A195");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P072");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P073");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A151");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P074");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P075");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P089");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P090");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 281;
      task_324 = Script_ShowUIMessageFromID("E1032A196");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P079");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A197");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FEELS_WATCHED_CALL, 1);
      break;
    case 21:
      task_320 = 290;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P091");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P092");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A203");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P093");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_324 = Script_ShowUIMessageFromID("E1032P094");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P095");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_RELAY_RACE_PHOTO_CALL, 1);
      break;
    case 26:
      task_320 = 300;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P096");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P097");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 301;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F4 = local_FF;
      switch (local_F4) {
        case 0:
          task_324 = Script_ShowUIMessageFromID("E1032P098");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 1:
          task_324 = Script_ShowUIMessageFromID("E1032P099");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 2:
          task_324 = Script_ShowUIMessageFromID("E1032P100");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
      }
      task_324 = Script_ShowUIMessageFromID("E1032A206");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 18:
      task_320 = 310;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P101");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A207");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P102");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A208");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P103");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 311;
      task_324 = Script_ShowUIMessageFromID("E1032A209");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P105");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A210");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P106");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P107");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P108");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_PARENTS_IN_VANCOUVER_CALL, 1);
      break;
    case 19:
      task_320 = 320;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P109");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P110");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A211");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P111");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P112");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 321;
      task_324 = Script_ShowUIMessageFromID("E1032A212");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P113");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A158");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P114");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P115");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_CALL_NAOYUKI_AND_ICHIRO_CALL, 1);
      break;
    case 25:
      task_320 = 330;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P117");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A214");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P118");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P119");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P120");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 331;
      task_324 = Script_ShowUIMessageFromID("E1032A215");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P121");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P123");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A217");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TRIP_INVITATION_CALL, 1);
      break;
    case 20:
      task_320 = 340;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P124");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P125");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A218");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P126");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P127");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P128");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_LAPIS_FORTUNETELLER_CALL, 1);
      break;
    case 23:
      task_320 = 350;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P109");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P117");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A214");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P129");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P130");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P131");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A219");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P132");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_320 = 351;
      task_324 = Script_ShowUIMessageFromID("E1032A153");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P133");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A220");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P134");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_NURSERY_TEACHER_CALL, 1);
      break;
    case 22:
      task_320 = 360;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A221");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P135");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P136");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A222");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P137");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P138");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A223");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P139");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 361;
      task_324 = Script_ShowUIMessageFromID("E1032A224");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_324 = Script_ShowUIMessageFromID("E1032P140");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A225");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A226");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FIRST_MEETING_CALL, 1);
      break;
  }
  return;
}

__attribute__((scn_string_bindings(5, data_2DCE8, 7, data_2DCFC, 8, data_2DD06, 21, data_2DD88, 22, data_2DD92, 30, data_2DDE2, 32, data_2DDF6, 33, data_2DE00, 48, data_2DE96, 49, data_2DEA0)))
static void script_1E3E7(void) {
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    task_320 = 250;
    Script_scenCreateTask(script_209DC, 0);
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032E001");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032E002");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032E003");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    task_324 = Script_ShowUIMessageFromID("E1032A010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032E004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      switch (local_FC) {
        case 0:
          task_324 = Script_ShowUIMessageFromID("E1032A011");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032E005");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A012");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 1:
          task_324 = Script_ShowUIMessageFromID("E1032A013");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032E006");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A014");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 2:
          task_324 = Script_ShowUIMessageFromID("E1032A015");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032E007");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A016");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
      }
    }
    task_324 = Script_ShowUIMessageFromID("E1032E008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_320 = 251;
    task_324 = Script_ShowUIMessageFromID("E1032E009");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032E010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032E011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032E012");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032E013");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032A018");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A020");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    task_324 = Script_ShowUIMessageFromID("E1032A021");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else {
    task_320 = 260;
    Script_scenCreateTask(script_209DC, 0);
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032F001");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032F002");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032F003");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    task_324 = Script_ShowUIMessageFromID("E1032A022");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032F004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      switch (local_FC) {
        case 0:
          task_324 = Script_ShowUIMessageFromID("E1032A011");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032F005");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A012");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 1:
          task_324 = Script_ShowUIMessageFromID("E1032A013");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032F006");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A023");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 2:
          task_324 = Script_ShowUIMessageFromID("E1032A024");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032F007");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A025");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A026");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
      }
    }
    task_320 = 261;
    task_324 = Script_ShowUIMessageFromID("E1032F008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032F009");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032F010");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032F011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032F012");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032F013");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032F014");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032F015");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A018");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    task_324 = Script_ShowUIMessageFromID("E1032A027");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  }
  return;
}

static int8_t script_1EEAF(void) {
  int8_t local_FF;
  local_FF = 0;
  task_320 = 55;
  Script_scenCreateTask(script_209DC, 0);
  local_FF = script_1EED7();
  return local_FF;
}

static int8_t script_1EED7(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  local_F7 = 0;
  task_324 = Script_ShowUIMessageFromID("E1032A034");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    local_F8 = 0;
  } else {
    local_F8 = 2;
  }
  script_1EFF2(local_F8);
  local_FC = script_1F144();
  script_1F15F(local_FC);
  if (script_1F0DB(local_F8, local_FC)) {
    if (local_F8 == 0) {
      local_F8 = 2;
    } else {
      local_F8 = 0;
    }
    task_320 = 56;
    Script_mapmSetCamFlags(0, 0);
    Script_charaVisUtils(CHARA_AKIR, 1);
    script_1EFF2(local_F8);
    local_FC = script_1F144();
    script_1F15F(local_FC);
    Script_mapmSetCamFlags(0, 1);
    Script_charaVisUtils(CHARA_AKIR, 0);
    if (script_1F0DB(local_F8, local_FC)) {
      local_F7 = script_1F535();
    } else {
      script_1F248(local_FC);
    }
  } else {
    script_1F248(local_FC);
  }
  return local_F7;
}

static void script_1EFF2(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      task_324 = Script_ShowUIMessageFromID("E1032H005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 1:
      task_324 = Script_ShowUIMessageFromID("E1032H004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 2:
      task_324 = Script_ShowUIMessageFromID("E1032H009");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 3:
      task_324 = Script_ShowUIMessageFromID("E1032H006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
  }
  return;
}

static int32_t script_1F0DB(int32_t arg0, int32_t arg1) {
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

static int32_t script_1F144(void) {
  return SCNF_RequestActionSelectorFromParams_0008(7, 8, 6, 5, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
}

static void script_1F15F(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 5:
      task_324 = Script_ShowUIMessageFromID("E1032A042");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 6:
      task_324 = Script_ShowUIMessageFromID("E1032A040");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 7:
      task_324 = Script_ShowUIMessageFromID("E1032A043");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 8:
      task_324 = Script_ShowUIMessageFromID("E1032A041");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
  }
  return;
}

__attribute__((scn_string_bindings(1, data_2DF18, 2, data_2DF22, 3, data_2DF2C, 4, data_2DF36)))
static void script_1F248(int32_t arg0) {
  int32_t local_FC;
  task_320 = 58;
  script_1F46D(1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 714)) {
    if ((arg0 == 5) | (arg0 == 6) | (arg0 == 7) | (arg0 == 8)) {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_F8 == 0) {
      } else if (local_F8 == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032A045");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_F8 == 2) {
        task_324 = Script_ShowUIMessageFromID("E1032A046");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A047");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    } else {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_F8 == 0) {
      } else if (local_F8 == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032A046");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A047");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
  } else {
    task_324 = Script_ShowUIMessageFromID("E1032A044");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 9) {
    local_FC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 9);
    local_FC = local_FC + 1;
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 9, local_FC);
  }
  return;
}

static void script_1F436(void) {
  task_324 = Script_ShowUIMessageFromID("E1032A051");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_1F46D(int32_t arg0) {
  Script_executeAicaCommand(525739, 0, 0);
  SCNF_WaitFrames_0000(10);
  if (arg0) {
    task_324 = Script_ShowUIMessageFromID("E1032A039");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
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

__attribute__((scn_string_bindings(0, data_2DF5E, 1, data_2DF68, 2, data_2DF72, 3, data_2DF7C, 4, data_2DF86, 5, data_2DF90, 6, data_2DF9A, 7, data_2DFA4, 8, data_2DFAE, 10, data_2DFC2, 11, data_2DFCC, 15, data_2DFF4, 16, data_2DFFE, 17, data_2E008, 18, data_2E012, 19, data_2E01C, 20, data_2E026, 21, data_2E030, 22, data_2E03A, 23, data_2E044, 24, data_2E04E, 25, data_2E058, 26, data_2E062, 27, data_2E06C, 28, data_2E076, 29, data_2E080, 33, data_2E0A8, 34, data_2E0B2, 35, data_2E0BC, 36, data_2E0C6, 37, data_2E0D0, 38, data_2E0DA, 39, data_2E0E4, 40, data_2E0EE, 41, data_2E0F8, 42, data_2E102, 43, data_2E10C, 44, data_2E116, 45, data_2E120, 46, data_2E12A, 47, data_2E134, 48, data_2E13E, 49, data_2E148, 52, data_2E166, 53, data_2E170, 54, data_2E17A, 59, data_2E1AC)))
static int8_t script_1F535(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  int32_t local_F0;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F7 = 0;
  task_320 = 57;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (local_FC >= 17) {
    task_324 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A120");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032H010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A066");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032H011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    SCNF_WaitFrames_0000(60);
    if (((local_F8 >= 170) & (local_F8 <= 193)) | ((local_F8 >= 220) & (local_F8 <= 241))) {
      local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F0 == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032J001");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A131");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032J010");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032J013");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032J001");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A134");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032J015");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A135");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
      script_1F46D(0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 546)) {
      task_324 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J020");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 546, 0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED)) {
      task_324 = Script_ShowUIMessageFromID("E1032J016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A161");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A162");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
    } else {
      task_324 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A131");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A132");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J011");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J012");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) {
    task_320 = 64;
    task_324 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A120");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032H010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A066");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032H011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    SCNF_WaitFrames_0000(60);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 546)) {
      task_320 = 65;
      task_324 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J020");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 546, 0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED)) {
      task_320 = 66;
      task_324 = Script_ShowUIMessageFromID("E1032J016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A161");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A162");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE)) {
      task_324 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A067");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
    } else {
      task_324 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J003");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A069");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A070");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
    }
  } else {
    local_F7 = 1;
    task_324 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A048");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032H008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_320 = 63;
    task_324 = Script_ShowUIMessageFromID("E1032A049");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A050");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    script_1F46D(1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  }
  return local_F7;
}

static void script_20212(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_EVT_GetPosition(SHEN_NAME('TBOX'), -1, &local_F4, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('TBOX'), &local_E8, 0);
  (&local_E8)->y = (&local_E8)->y + -1820;
  local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 1.0f;
  (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 1.0f;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, (&local_E8)->y, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_202D5(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  Vector3i local_C4;
  script_216CD();
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 23.0f, 53.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  task_320 = 62;
  Script_scenCreateTask(script_209DC, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(263851, 0, 0);
  local_F4.x = 0;
  (&local_F4)->y = 1307;
  (&local_F4)->z = 0;
  local_E8.x = 0;
  (&local_E8)->y = -601;
  (&local_E8)->z = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 53.0f, 56.0f, -1.0f, 0.2f, 0);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &local_E8, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &local_F4, CHARA_PART_Add);
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = 0;
  (&local_F4)->y = 490;
  (&local_F4)->z = 0;
  local_E8.x = 0;
  (&local_E8)->y = -225;
  (&local_E8)->z = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 56.0f, 63.0f, -1.0f, 0.5f, 0);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &local_E8, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &local_F4, CHARA_PART_Add);
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 63.0f, 90.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(SHEN_NAME('TBOX'), -1, &local_DC, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('TBOX'), &local_D0, 0);
  local_DC.x = (&local_DC)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D0)->y) * 1.5f;
  (&local_DC)->z = (&local_DC)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D0)->y) * 1.5f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_DC);
  (&local_D0)->y = (&local_D0)->y + 0x8000;
  (&local_C4)->y = (&local_D0)->y;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_C4, 0);
  Script_charaSetRefboardFlagTo(task_308, 0);
  Script_charaSetRefboardFlagTo(task_30C, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &task_398, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &task_3A4, CHARA_PART_Set);
  task_320 = -1;
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCameraFieldOfView(10922);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_205FD(void) {
  Script_seqEnterEvtCam(6220, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
  script_20212();
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 10.0f, 50.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_20682(void) {
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 20.0f, 50.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x20);
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

static void script_20711(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_seqEnterEvtCam(6220, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_20212();
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 4);
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 25.0f, 51.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 51.0f, 85.0f, -1.0f, 1.0f, 0);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &task_398, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &task_3A4, CHARA_PART_Get);
  local_F4.x = 0;
  (&local_F4)->y = 713;
  (&local_F4)->z = 0;
  local_E8.x = 0;
  (&local_E8)->y = -328;
  (&local_E8)->z = 0;
  Script_executeAicaCommand(263851, 0, 0);
  local_E4 = Script_scenCreateTask(script_20927, 0);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &local_E8, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &local_F4, CHARA_PART_Add);
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 85.0f, 95.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &task_398, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &task_3A4, CHARA_PART_Set);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchControl((void *)local_E4);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_seqEnterEvtCam(6219, task_304, task_304);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_20927(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  int32_t local_CC;
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, &local_DC, &local_F4, CHARA_PART_Get);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, &local_D0, &local_E8, CHARA_PART_Get);
    Script_charaSetupXForm(SHEN_NAME('TBOX'), 21, &local_DC);
    local_CC = (&local_F4)->y + (&local_E8)->y + 51882;
    local_DC.x = (&local_DC)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_CC) * 0.6f;
    (&local_DC)->y = 1.2f;
    (&local_DC)->z = (&local_DC)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_CC) * 0.6f;
    Script_SetMotnFlags(CHARA_AKIR, 17, &local_DC, 0x38000000);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_209DC(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_320;
    switch (local_FC) {
      case -1:
        goto L021350;
        break;
      case 0:
        break;
      case 51:
        Script_seqEnterEvtCam(6222, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 53:
        Script_seqEnterEvtCam(6222, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 54:
        Script_seqEnterEvtCam(6222, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 55:
        Script_seqEnterEvtCam(6223, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 56:
        Script_seqEnterEvtCam(6224, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 57:
        Script_seqEnterEvtCam(6225, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 58:
        Script_seqEnterEvtCam(6226, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 62:
        Script_seqEnterEvtCam(6221, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 63:
        Script_seqEnterEvtCam(6227, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 64:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 65:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 66:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 70:
        local_FC = task_330;
        switch (local_FC) {
          case 0:
            Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 1:
            Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 2:
            Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 3:
            Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 4:
            Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 5:
            Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 6:
            Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
        }
        task_330 = task_330 + 1;
        if (task_330 > 6) {
          task_330 = 0;
        }
        break;
      case 80:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 81:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 82:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 90:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 100:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 101:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 110:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 111:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 120:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 130:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 131:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 140:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 141:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 150:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 151:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 160:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 161:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 162:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 163:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 170:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 180:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 190:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 191:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 200:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 210:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 220:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 230:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 231:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 240:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 250:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 251:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 260:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 261:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 270:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 280:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 281:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 290:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 300:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 301:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 310:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 311:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 320:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 321:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 330:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 331:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 340:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 350:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 351:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 360:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 361:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 370:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 371:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 380:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 381:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 390:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 391:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 400:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 410:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 411:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
    }
    if (task_320) {
      task_320 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  L021350:;
  task_320 = 0;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_2E1DE, 1, data_2E1EE)))
static void script_2135E(void) {
  Vector4 local_F0;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  SCNF_WaitFrames_0000(1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "TBOX") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_shenClearFreeState(0);
  local_F0.x = 0.8f;
  (&local_F0)->y = 0.8f;
  (&local_F0)->z = 0.8f;
  (&local_F0)->w = 0.8f;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(task_308, 1);
  Script_charaSetRefboardFlagTo(task_30C, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_F0);
  Script_SetCharaRefBoardPos(task_308, &local_F0);
  Script_SetCharaRefBoardPos(task_30C, &local_F0);
  if (task_32D < 0) {
    script_205FD();
  } else {
    script_20711();
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_2E1F3)))
static void script_2145D(void) {
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_shenClearFreeState(0);
  if (task_32D < 0) {
    script_20682();
  } else {
    script_202D5();
  }
  SCNF_WaitFrames_0000(1);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_shenClearFreeState(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TBOX");
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_214D0(void) {
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_EVT_GetPosition(task_304, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_304, &local_E8, 0);
    local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.52f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.52f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    {
      Vector3i local_DC;
      (&local_DC)->y = (&local_E8)->y - 1820;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
    }
  }
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 32003, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2C0C4, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 0, 0, 0, 1);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2C0C4, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 1, 0, 0, 1);
  Script_scenCreateTask(script_218DA, 0);
  SCNF_WaitFrames_0000(1);
  Script_TELMUtils(TELM_CMD_Cleanup, task_304);
  Script_TELMUtils(TELM_CMD_SetTargetTaskAndMode, task_304, 0x52494B41, 3);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -0.03f;
    (&local_F4)->y = -0.01f;
    (&local_F4)->z = -0.02f;
    local_E8.x = 59162;
    (&local_E8)->y = 7824;
    (&local_E8)->z = 62441;
    Script_TELMUtils(TELM_CMD_SetCustomReceiverOffset, task_304, &local_F4);
    Script_TELMUtils(TELM_CMD_SetCustomReceiverEuler, task_304, &local_E8);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_executeAicaCommand(591275, 0, 0);
  return;
}

static void script_216B9(void) {
  task_320 = -1;
  Script_TELMUtils(TELM_CMD_Cleanup, task_304);
  return;
}

static void script_216CD(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(task_304, -1, &local_F4, 0);
  {
    Vector3i local_E8;
    Script_mtwkGetSetRotation(task_304, &local_E8, 0);
    local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.5f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.5f;
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  {
    Vector3i local_E8;
    Vector3i local_DC;
    Script_mtwkGetSetRotation(SHEN_NAME('TBOX'), &local_E8, 0);
    (&local_DC)->y = (&local_E8)->y;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_InitializeOsag(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_21795(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  task_395 = 1;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 4);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_AKIR, 32004, -1.0f, 55.0f, -1.0f, 1.5f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2C1A8, 5);
  Script_TELMUtils(TELM_CMD_ResetPeerState, task_304);
  Script_executeAicaCommand(67243, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 32004, 55.0f, -1.0f, -1.0f, 1.5f, 0);
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
  Script_TELMUtils(TELM_CMD_SetCustomReceiverOffset, task_304, &local_F4);
  Script_TELMUtils(TELM_CMD_SetCustomReceiverEuler, task_304, &local_E8);
  return;
}

static int32_t script_218DA(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = -0.164926f;
  (&local_F4)->y = 1.545938f;
  (&local_F4)->z = -0.138711f;
  local_E8.x = -8009;
  (&local_E8)->y = 4187;
  (&local_E8)->z = -2184;
  task_395 = 0;
  for (;;) {
    if (task_395) {
      break;
    }
    Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E8);
    Script_SetMotnFlags(CHARA_AKIR, 11, &local_F4, 0x39000000);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_2195D(void) {
  Vector3i local_F4;
  local_F4.x = 0;
  (&local_F4)->y = -0x4000;
  (&local_F4)->z = 0;
  Script_charaPosRotUtils(SHEN_NAME('ITMN'), 7, 0, &local_F4, CHARA_PART_Set);
  (&local_F4)->y = 0x4000;
  Script_charaPosRotUtils(SHEN_NAME('ITMN'), 12, 0, &local_F4, CHARA_PART_Set);
  Script_scenToggleVisibleFor(SHEN_NAME('ITMN'), 0);
  return;
}

__attribute__((scn_string_bindings(0, data_2E380, 2, data_2E395, 4, data_2E3AA)))
static void script_219B3(void) {
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "MAPY") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "MAPN") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 5)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "MAPS") == 0) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_charaPosRotUtils(SHEN_NAME('YAMN'), 7, &local_F4, 0, CHARA_PART_Get);
    local_F4.x = (&local_F4)->x - 0.05f;
    local_E8.x = 0;
    (&local_E8)->y = -0x4000;
    (&local_E8)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('YAMN'), 7, &local_F4, &local_E8, CHARA_PART_Set);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_charaPosRotUtils(SHEN_NAME('YAMN'), 12, &local_F4, 0, CHARA_PART_Get);
    local_F4.x = (&local_F4)->x + 0.05f;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('YAMN'), 12, &local_F4, &local_E8, CHARA_PART_Set);
  }
  Script_ccowUtils(SHEN_NAME('YAMN'), CCOW_DisableAll);
  Script_scenToggleVisibleFor(SHEN_NAME('YAMN'), 0);
  {
    Vector3 local_F4;
    local_F4.x = 1.2f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.0f;
    Script_hmdlApplyScale(SHEN_NAME('ITMN'), &local_F4);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    Script_ccowUtils(SHEN_NAME('ITMN'), CCOW_EnableAll);
  } else {
    script_2195D();
  }
  return;
}

static int32_t script_21B77(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int8_t local_F1;
  int8_t local_F0;
  Script_shenSetLightingMode(1);
  local_F1 = -1;
  local_F3 = -1;
  local_F2 = 0;
  local_F0 = 0;
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(7, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    if ((&local_F4)->minutes != local_F3) {
      if ((-((&local_F4)->hours >= 19) | (-((&local_F4)->hours >= 0) & -((&local_F4)->hours <= 4))) & -(local_F1 != 0)) {
        if (local_F2 == 18) {
          local_F1 = 0;
          Script_scenCreateTask(script_21EC6, 0);
        } else {
          local_F1 = 0;
          Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Hide);
          if (-1) {
            Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Show);
              goto L021C4A;
            }
          }
          L021C4A:;
          Script_scenToggleVisibleFor(SHEN_NAME('TAXI'), 0);
          script_2215B();
        }
      }
      if (local_F0 < 3) {
        if (local_F0 == 0) {
          if (((&local_F4)->hours >= 18) & ((&local_F4)->minutes >= 30)) {
            Script_mapmSetRenderFlag(3, 1);
            Script_mapmSetRenderFlag(2, 0);
            local_F0 = 1;
          }
        }
        if (local_F0 == 1) {
          if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 0)) {
            Script_mapmSetRenderFlag(5, 1);
            Script_mapmSetRenderFlag(4, 0);
            local_F0 = 2;
          }
        }
        if (local_F0 == 2) {
          if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 30)) {
            Script_mapmSetRenderFlag(7, 1);
            Script_mapmSetRenderFlag(6, 0);
            local_F0 = 3;
          }
        }
        if ((&local_F4)->hours >= 20) {
          Script_mapmSetRenderFlag(3, 1);
          Script_mapmSetRenderFlag(5, 1);
          Script_mapmSetRenderFlag(7, 1);
          Script_mapmSetRenderFlag(2, 0);
          Script_mapmSetRenderFlag(4, 0);
          Script_mapmSetRenderFlag(6, 0);
          local_F0 = 3;
        }
      }
      if (local_F0 == 3) {
        if (((&local_F4)->hours >= 0) & ((&local_F4)->hours <= 4)) {
          Script_mapmSetRenderFlag(3, 0);
          Script_mapmSetRenderFlag(5, 0);
          Script_mapmSetRenderFlag(7, 0);
          Script_mapmSetRenderFlag(2, 1);
          Script_mapmSetRenderFlag(4, 1);
          Script_mapmSetRenderFlag(6, 1);
          local_F0 = 4;
        }
      }
      local_F3 = (&local_F4)->minutes;
      local_F2 = (&local_F4)->hours;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_21DD1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static int32_t script_21E8A(void) {
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_2E3BF, 1, data_2E3CF, 2, data_2E3D4, 3, data_2E3D9, 4, data_2E3DA, 5, data_2E3DF, 6, data_2E3E0)))
static int32_t script_21EC6(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  for (;;) {
    if (Script_ryoControl(PCCTL_QueryShouldRun) & -(Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) & -(task_300 == 0) & -(task_32D == 0) & Script_IsPlayerAllowed(0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
      Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
      Script_shenClearFreeState(0);
      Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      script_261F7();
      SCNF_WaitFrames_0000(1);
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Hide);
      if (-1) {
        Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Hide);
      } else {
        if (0) {
          Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Show);
          goto L021FBE;
        }
      }
      L021FBE:;
      Script_scenToggleVisibleFor(SHEN_NAME('TAXI'), 0);
      Script_SetupLightFromTable(1);
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "JDYA") == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_AuthorUtils(0, 0, "JDYA", "");
      Script_AuthorUtils(0, 1, "JDYA", "");
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
      Script_scenCreateTask(script_21E8A, 0);
      script_FF30(4);
      Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_convSetCamInactiveFlag(1);
      Script_SetGameStateTo(AUTH_None);
      Script_lightResetSlotToDefaults(1);
      script_2215B();
      script_2620A();
      Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      script_261BC();
      Script_AuthorUtils(AUTH_FREE, 0);
      Script_AuthorUtils(AUTH_FREE, 1);
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "JDYA");
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
      Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
      Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
      Script_shenClearFreeState(1);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_2215B(void) {
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_SetCharaFlags(CHARA_AKIR, 0);
  return;
}

static void script_2217A(void) {
  Script_scenSetMapEvTbl((uint32_t *)&data_2E3E8, 4);
  return;
}

static void script_22188(void) {
  return;
}

static void script_2218B(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    task_2DE = 3;
    Script_ryoSleepUtils(3, 1, 0, 3);
  }
  return;
}

static void script_221AE(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    task_2DE = 1;
    Script_ryoSleepUtils(3, 1, 0, 1);
  }
  return;
}

static void script_221D1(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    task_2DE = 2;
    Script_ryoSleepUtils(3, 1, 0, 2);
  }
  return;
}

static void script_221F4(void) {
  int32_t unused_FC;
  if (script_21DD1(9, 0, 19, 0)) {
    if (Script_ScenDataUtil(task_E8, SCND_ReadU32, 1) & 2) {
      script_E175(0);
      script_E3EE(0);
    }
  } else {
    if ((Script_ScenDataUtil(task_E8, SCND_ReadU32, 1) & 2) == 0) {
      script_E13E(0);
      script_E437(0);
    }
  }
  return;
}

static int32_t script_2225B(void) {
  float local_FC;
  int32_t local_F8;
  local_FC = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
  while (1) {
    local_F8 = *&task_84;
    if ((local_F8 == 11) | (local_F8 == 13)) {
      while (task_B6 != 1) {
        SCNF_WaitFrames_0000(1);
      }
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 1.5f);
      task_A4 = 1.5f;
      task_B0 = *(int8_t *)&*(int32_t *)&task_B0 | 1;
      for (;;) {
        if (!*(int8_t *)&*(int32_t *)&task_B0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      if ((local_F8 == 12) | (local_F8 == 14)) {
        while (task_B6 != 1) {
          SCNF_WaitFrames_0000(1);
        }
        task_A4 = local_FC;
        task_B0 = *(int8_t *)&*(int32_t *)&task_B0 | 1;
        for (;;) {
          if (!*(int8_t *)&*(int32_t *)&task_B0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        goto L022323;
      }
    }
    L022323:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2232F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = arg3;
  if (local_FC == 1) {
    task_7C = 0;
    goto L02234A;
  }
  L02234A:;
  Script_ryoControl(PCCTL_RequestNewMode, 3);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_RequestNewMode, 0);
  Script_shenRequestGameModuleTransition(arg0, arg1, arg2);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_223C6(void) {
  int32_t local_FC;
  local_FC = 0;
  if ((&task_F8)->mode) {
    if (task_F8.entry_idx == 0) {
      local_FC = 2;
    }
  }
  return local_FC;
}

static int32_t script_223EE(void) {
  int32_t local_FC;
  local_FC = 0;
  if (script_1100D()) {
    local_FC = 3;
  }
  return local_FC;
}

static int8_t script_22408(void) {
  int8_t local_FF;
  local_FF = 0;
  task_288.x = 21.4f;
  task_288.y = 0.0f;
  task_288.z = -8.46f;
  if (task_2D4 == 1) {
    if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &task_2B8, &task_288) <= 10.0f)) {
      task_2D4 = 0;
      Script_ryoSleepUtils(3, 1, 1, 0);
    }
  }
  if ((Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &task_288, 10.0f) == 0) & (task_2D4 == 0)) {
    task_288.x = 19.213699f;
    task_288.y = 0.0f;
    task_288.z = -7.6756f;
    task_294.x = 27.6138f;
    task_294.y = 0.0f;
    task_294.z = -10.7757f;
    if (Script_SetTaskDataValue(CHARA_AKIR, &task_288, &task_294, 0)) {
      task_2D4 = 1;
      Script_ryoSleepUtils(3, 1, 1, 1);
      local_FF = 6;
    }
  }
  return local_FF;
}

static int32_t script_224F8(void) {
  int32_t local_FC;
  local_FC = 0;
  task_288.x = 14.0555f;
  task_288.y = 0.0f;
  task_288.z = 18.608601f;
  if (Script_CheckTargetWithinAngleRange(&task_288, 1.8f, 0x4000, 12743, &task_2B8) & -(task_2C4.y < 63715) & -(task_2C4.y > 34588)) {
    if (*(int8_t *)&*(int32_t *)&task_2DC == 0) {
      Script_sicnUtils(0, 5, 1);
      *(int8_t *)&task_2DC = 1;
      Script_uiUtils(0, 25, 0);
    }
    if (Script_GetControllerInput(1) == 0x400) {
      local_FC = 7;
    }
  } else {
    if (*(int8_t *)&*(int32_t *)&task_2DC) {
      Script_sicnUtils(1, 0, 0);
      SCNF_RequestIconNotification(0, 0, 0, 2);
      *(int8_t *)&task_2DC = 0;
      Script_uiUtils(0, 0, 0);
    }
  }
  return local_FC;
}

static int32_t script_225CE(void) {
  int32_t local_FC;
  local_FC = 0;
  task_288.x = 15.0673f;
  task_288.y = 0.0f;
  task_288.z = 2.0482f;
  task_294.x = (&task_2B8)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, task_2C4.y) * 0.3f;
  task_294.y = task_2B8.y;
  task_294.z = task_2B8.z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, task_2C4.y) * 0.3f;
  if (Script_CheckTargetWithinAngleRange(&task_294, 1.4f, task_2C4.y, 7281, &task_288)) {
    if (*((int8_t *)&*(int32_t *)&task_2DC + 1) == 0) {
      Script_sicnUtils(0, 4, 1);
      *((int8_t *)&*(int32_t *)&task_2DC + 1) = 1;
    }
    if (Script_GetControllerInput(1) == 0x400) {
      local_FC = 5;
      if (*((int8_t *)&*(int32_t *)&task_2DC + 1)) {
        Script_sicnUtils(1, 0, 0);
        *((int8_t *)&*(int32_t *)&task_2DC + 1) = 0;
      }
    }
  } else {
    if (*((int8_t *)&*(int32_t *)&task_2DC + 1)) {
      Script_sicnUtils(1, 0, 0);
      *((int8_t *)&*(int32_t *)&task_2DC + 1) = 0;
    }
  }
  return local_FC;
}

static int32_t script_226E3(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  GameDateTime local_EC;
  local_F8 = 0;
  task_288.x = 14.78f;
  task_288.y = 0.0f;
  task_288.z = 7.57f;
  task_294.x = 8.73f;
  task_294.y = 0.0f;
  task_294.z = 0.64f;
  local_FC = Script_StepUtils(GET_STEP_LOWER, 0) + Script_StepUtils(GET_STEP_HIGHER, 0) * 100;
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1037_SEQUENCE_COMPLETE) == 0) & (Script_GetOwnedItemCount(4, 0) == 1)) {
    local_F8 = 0;
  } else {
    if (((local_FC >= 801) & (local_FC <= 1703)) | ((local_FC >= 1705) & (local_FC <= 2108))) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 0) {
        if (Script_SetTaskDataValue(CHARA_AKIR, &task_288, &task_294, 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED) == 0)) {
          Script_ReturnTimeDate(&local_EC);
          if (script_21DD1(19, 0, 23, 29) & ((-((&local_EC)->month == 12) & -((&local_EC)->day >= 26)) | -((&local_EC)->month < 5))) {
            local_F8 = 4;
          }
        }
      }
    }
  }
  return local_F8;
}

static void script_22806(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SECRET_OF_YIN_YANG) == 0) {
    if (((Script_StepUtils(GET_STEP_HIGHER, 0) == 19) & (Script_StepUtils(GET_STEP_LOWER, 0) == 1)) | ((Script_StepUtils(GET_STEP_HIGHER, 0) >= 16) & (Script_StepUtils(GET_STEP_HIGHER, 0) < 19))) {
      task_288.x = 16.0f;
      task_288.y = 0.0f;
      task_288.z = -2.0f;
      task_294.x = 27.0f;
      task_294.y = 0.0f;
      task_294.z = -18.0f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &task_288, &task_294, 0)) {
        if (Script_scenUtils(SCENARIO_CMD_QUERY_ACTIVE_RUNTIME_OBJECT, 0)) {
          if (script_FF06() == 0x414D4159) {
            Script_ReturnTimeDate(&local_F4);
            if ((((&local_F4)->hours == 8) & ((&local_F4)->minutes >= 30)) | (((&local_F4)->hours > 8) & ((&local_F4)->hours < 19))) {
              script_23FAE();
            }
          }
        }
      }
    }
  }
  return;
}

static int32_t script_228FC(void) {
  int32_t local_FC;
  Vector3i local_F0;
  int32_t local_EC;
  local_FC = 0;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F0, 0);
  task_288.x = -4.581f;
  task_288.y = 0.0f;
  task_288.z = 18.6539f;
  task_294.x = -5.681f;
  task_294.y = 0.0f;
  task_294.z = 22.6539f;
  task_2A0.x = -5.481f;
  task_2A0.y = 0.0f;
  task_2A0.z = 18.6539f;
  task_2AC.x = -10.081f;
  task_2AC.y = 0.0f;
  task_2AC.z = 20.0539f;
  if ((Script_SetTaskDataValue(CHARA_AKIR, &task_288, &task_294, 0) & -((&local_F0)->y > 0) & -((&local_F0)->y < 0x8000)) | (Script_SetTaskDataValue(CHARA_AKIR, &task_2A0, &task_2AC, 0) & -((&local_F0)->y > 0x4000) & -((&local_F0)->y < 0xC000))) {
    local_EC = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
    if (-(local_EC >= 131) & -(local_EC <= 154) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_TAIKE_EVENT_DONE) == 0) & script_21DD1(8, 30, 22, 59)) {
      local_FC = 9;
    }
  }
  return local_FC;
}

static void script_22A43(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 2) {
    SCNF_WaitFrames_0000(30);
    Script_executeAicaCommand(2720, 1, 30);
    Script_executeAicaCommand(2720, 2, 30);
    Script_executeAicaCommand(2720, 3, 30);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_shenRequestGameModuleTransition(2, SHEN_NAME('JABE'), 0);
    SCNF_WaitFrames_0000(1);
    goto L022AC0;
  }
  L022AC0:;
  return;
}

static int32_t script_22AC1(int32_t arg0, int32_t arg1) {
  if (arg0) {
    (void)arg0;
  } else {
    (void)arg1;
  }
  end:;
}

static int32_t script_22AD3(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  local_F8 = script_22AC1(arg0, arg1);
  switch (local_F8) {
    case 2:
      local_FC = script_223C6();
      break;
    case 6:
      local_FC = script_22408();
      break;
    case 4:
      local_FC = script_226E3();
      break;
    case 9:
      local_FC = script_228FC();
      break;
  }
  if (Script_IsPlayerAllowed(0)) {
    if (local_FC == 0) {
      local_FC = script_223EE();
    }
    if (local_FC == 0) {
      local_FC = script_224F8();
    }
    if (local_FC == 0) {
      local_FC = script_225CE();
    }
  }
  return local_FC;
}

static void script_22B8A(void) {
  int32_t local_FC;
  local_FC = Script_UpdateTaskCollisionCache(0, 1, &task_2B8);
  if (local_FC != -1) {
    Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, local_FC);
  }
  return;
}

static int32_t script_22BB0(void) {
  int8_t local_FF;
  int8_t local_FE;
  task_2D4 = 0;
  task_2DE = 0;
  local_FF = 0;
  for (;;) {
    if (local_FF == 2) {
      break;
    }
    *((int8_t *)&*(int32_t *)&task_2DC + local_FF) = 0;
    local_FF = local_FF + 1;
  }
  local_FF = local_FE = 1;
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &task_2B8, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &task_2C4, 0x42000000);
    script_22B8A();
    if (Script_IsPlayerAllowed(0)) {
      uint8_t unused_FC[2];
      int32_t local_F8;
      local_F8 = task_2DE;
      switch (local_F8) {
        case 3:
          script_2232F(2, SHEN_NAME('JU00'), 0, 0);
          break;
        case 1:
          script_2232F(2, SHEN_NAME('D000'), 1, 0);
          break;
        case 2:
          script_2232F(2, SHEN_NAME('D000'), 11, 0);
          break;
      }
    }
    if ((local_FE == 2) | Script_IsPlayerAllowed(0)) {
      local_FF = script_22AD3(local_FF, local_FE);
    } else {
      local_FF = 0;
    }
    local_FE = local_FE + 1;
    if (local_FE == 10) {
      local_FE = 2;
    }
    if ((local_FE == 3) | (local_FE == 7) | (local_FE == 5)) {
      local_FE = local_FE + 1;
      if (local_FE == 10) {
        local_FE = 2;
      }
    }
    {
      uint8_t unused_FC[2];
      int32_t local_F8;
      local_F8 = local_FF;
      switch (local_F8) {
        case 2:
          script_22A43(local_FF);
          break;
        case 6:
          script_148BB();
          break;
        case 7:
          if (script_3F53(100, 1)) {
            script_48F0(CHARA_AKIR, SHEN_NAME('VM_0'), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
          }
          if (*(int8_t *)&*(int32_t *)&task_2DC) {
            Script_sicnUtils(1, 0, 0);
            *(int8_t *)&task_2DC = 0;
            Script_uiUtils(0, 0, 0);
          }
          break;
        case 3:
          script_1314A();
          break;
        case 5:
          script_13DE7(0);
          break;
        case 4:
          script_23286(1);
          break;
        case 9:
          script_2439B();
          break;
      }
    }
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      script_221F4();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_22DE1(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_26C3C, 8);
  Script_handSetFloatsFor(arg0, 0, data_26C3C, 8);
  return;
}

static void script_22E1C(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_22E46(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F0[3];
  int32_t local_EC;
  Script_executeAicaCommand(19112, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_387C();
  script_2308C();
  local_FC = 0;
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_FC;
    switch (local_EC) {
      case 0:
        Script_FACE(CHARA_HRSK, 3, 0, 16);
        break;
      case 400:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
        break;
      case 950:
        Script_FACE(CHARA_HRSK, 0, 0, 16);
        break;
      case 1472:
        Script_FACE(CHARA_HRSK, 2, 0, 16);
        break;
      case 1560:
        Script_FACE(CHARA_HRSK, 3, 0, 16);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_22F23(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  if (Script_shenSeqUtils(-7) == 0) {
    script_387C();
    script_2308C();
    local_FC = 0;
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      local_EC = local_FC;
      switch (local_EC) {
        case 0:
          Script_FACE(CHARA_HRSK, 3, 0, 16);
          break;
        case 143:
          script_22E1C(CHARA_AKIR);
          script_22E1C(CHARA_HRSK);
          break;
        case 570:
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
          Script_scenToggleActiveFor(CHARA_AKIR, 0);
          Script_scenToggleActiveFor(CHARA_HRSK, 0);
          break;
        case 1000:
          Script_FACE_LCG(CHARA_HRSK, FACE_BlinkClose, 16, 0);
          break;
        case 1080:
          Script_FACE(CHARA_HRSK, 0, 0, 16);
          break;
        case 1162:
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
          break;
        case 1500:
          goto L02307B;
          break;
      }
      if ((local_FC >= 896) & (local_FC <= 1161)) {
        Script_EVT_GetPosition(CHARA_HRSK, 18, &local_F0, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 8);
      }
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
    }
    L02307B:;
    Script_FACE_LCG(CHARA_HRSK, FACE_BlinkAutomatic, 0, 0);
  }
  return;
}

static void script_2308C(void) {
  script_3894(CHARA_AKIR);
  script_3894(CHARA_HRSK);
  Script_SetMaxInterpSpeed(0.4f);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2E3F8);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_2E408);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_HRSK, 1, 0);
  script_22DE1(CHARA_AKIR);
  script_22DE1(CHARA_HRSK);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_HRSK, -1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_HRSK, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
  return;
}

static void script_23197(void) {
  script_23220(CHARA_AKIR);
  Script_SetCharaFlags(CHARA_HRSK, 0);
  script_3921(CHARA_AKIR);
  script_3921(CHARA_HRSK);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  return;
}

static void script_231E5(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_26C3C, 8);
  Script_handSetFloatsFor(arg0, 0, data_26C3C, 8);
  return;
}

static void script_23220(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static int32_t script_2324A(void) {
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_2E418, 1, data_2E428, 2, data_2E42D, 3, data_2E432, 4, data_2E433, 5, data_2E438, 7, data_2E440, 8, data_2E445, 9, data_2E44A, 10, data_2E44F)))
static void script_23286(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  GameDateTime local_F0;
  local_FC = 0;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 400, 0);
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(10, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED, 1);
  Script_SetShouldSleepFlag(0);
  script_261F7();
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_srchControl((void *)task_2D0);
  Script_srchControl((void *)task_328);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('YAMN'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('YAMN'), CDM_Show);
      goto L02335D;
    }
  }
  L02335D:;
  Script_scenToggleVisibleFor(SHEN_NAME('YAMN'), 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4B535248, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4B535248);
  while (local_FC == 0) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4B535248);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(CHARA_HRSK, MOTM_PoseNone, SCEN_Write);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "DNOZ") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "DNOZ", "");
  Script_AuthorUtils(0, 1, "DNOZ", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_235C1();
  Script_LoadVoiceFile(1, "01FULB");
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 1, "FREE", 0);
  Script_LoadSND(0, 2, "bgm068.snd", 0);
  Script_LoadSND(0, 4, "a1_nonam.snd", 0);
  Script_executeAicaCommand(19112, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  if (arg0 == 0) {
    Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  }
  Script_scenCreateTask(script_2324A, 0);
  script_22E46(0);
  script_22F23(1);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_ReturnTimeDate(&local_F0);
  if ((((&local_F0)->hours < 23) & ((&local_F0)->hours > 5)) | (((&local_F0)->hours == 23) & ((&local_F0)->minutes < 30))) {
    (&local_F0)->hours = 23;
    (&local_F0)->minutes = 30;
    (&local_F0)->seconds = 0;
    Script_SetNewDateTime(&local_F0);
  }
  Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 3);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_235C1(void) {
  Vector3 local_F4;
  Script_PREP_Related(-1, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(AndNotInstructionFlags, 0xC);
  Script_SetVector4ByOffset(&data_2E4B4);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_2E4C4);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_SetFogColour(&data_2E46C);
  Script_SetFogFlags(0, data_2E47C);
  Script_shenSetLightingMode(2);
  Script_lightSetGlobalSceneLighting(&data_2E48C);
  local_F4.x = 20.61f;
  (&local_F4)->y = 3.39f;
  (&local_F4)->z = -4.08f;
  Script_lightEnablePointLightAt(1, &local_F4);
  Script_LightUtils(1, 2, &data_2E49C);
  Script_LightUtils(1, 4, 1.0f);
  Script_LightUtils(1, 5, 3.69f);
  Script_LightUtils(1, 6, data_2E4AC);
  Script_lightSetSlotEnabled(1, 1);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(8, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL27.SPR");
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
  return;
}

static void script_2371F(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_23764(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_2E5CC, 1);
  Script_handSetFloatsFor(arg0, 0, data_2E5CC, 1);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return;
}

static void script_23814(void) {
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2EB24);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetShadowFlags(CHARA_YAMA, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_YAMA, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetCharaRefBoardPos(CHARA_YAMA, &data_2EB24);
  Script_charaSetRefboardFlagTo(CHARA_YAMA, 1);
  Script_SetCharaRefBoardPos(SHEN_NAME('PNXM'), &data_2EB24);
  Script_charaSetRefboardFlagTo(SHEN_NAME('PNXM'), 1);
  Script_SetVector4ByOffset(&data_2EB24);
  (void)1;
  return;
}

static void script_238B8(void) {
  script_23764(CHARA_AKIR);
  script_23764(CHARA_YAMA);
  script_23814();
  task_3B0 = 0;
  return;
}

static void script_238DF(int32_t arg0) {
  Script_executeAicaCommand(17832, 0, 0);
  Script_executeAicaCommand(1184, 2, 105);
  script_238B8();
  Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_charaSetRefboardFlagTo(SHEN_NAME('PNXM'), 0);
  Script_set_val_to_1(2, 0);
  return;
}

static int32_t script_2394A(void) {
  Vector3 local_F4;
  for (;;) {
    {
      int32_t local_F0;
      local_F0 = task_3B0;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 1929:
          Script_FACE(CHARA_AKIR, 2, 0, 30);
          break;
        case 2261:
          Script_FACE(CHARA_AKIR, 3, 0, 30);
          break;
        case 2453:
          Script_FACE(CHARA_AKIR, 0, 0, 30);
          break;
      }
    }
    if ((task_3B0 > 2661) & (task_3B0 < 2692)) {
      Script_EVT_GetPosition(SHEN_NAME('PNXM'), -1, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 8);
    } else {
      Script_EVT_GetPosition(CHARA_YAMA, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 8);
    }
    {
      int32_t local_F0;
      local_F0 = task_3B0;
      switch (local_F0) {
        case 407:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2E5CC, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2E5CC, 0);
          break;
        case 556:
          Script_charaDisplay(SHEN_NAME('PNXM'), CDM_Show);
          Script_fixoAttachEx(SHEN_NAME('PNXM'), CHARA_AKIR, 18, &data_2EB34, &data_2EB40);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2EA40, 0);
          break;
        case 678:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1591:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2E5CC, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2E5CC, 0);
          break;
        case 1928:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 2080:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2E5CC, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2E5CC, 0);
          break;
        case 2335:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 2336:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2EA40, 8);
          Script_fixoAttachEx(SHEN_NAME('PNXM'), CHARA_AKIR, 18, &data_2EB34, &data_2EB40);
          break;
        case 2642:
          Script_charaDisplay(SHEN_NAME('PNXM'), CDM_Hide);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2E5CC, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2E5CC, 0);
          break;
        case 2743:
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_3B0;
      switch (local_F0) {
        case 60:
          Script_ResetHandMotion(CHARA_YAMA, 1, 1, 0);
          Script_ResetHandMotion(CHARA_YAMA, 1, 0, 0);
          Script_SetHandRootRotation(CHARA_YAMA, 1, 0x15, &data_2EBB4);
          Script_handSetFloatsFor(CHARA_YAMA, 1, data_2E95C, 0);
          Script_handSetFloatsFor(CHARA_YAMA, 0, data_2E95C, 0);
          break;
        case 407:
          Script_ResetHandMotion(CHARA_YAMA, 0, 1, 0);
          Script_ResetHandMotion(CHARA_YAMA, 0, 0, 0);
          Script_ReadModelFormat7(CHARA_YAMA, 1, data_2EB64, 0);
          break;
        case 678:
          Script_fixoAttachEx(SHEN_NAME('PNXM'), CHARA_YAMA, 18, &data_2EB4C, &data_2EB58);
          Script_ResetHandMotion(CHARA_YAMA, 1, 1, 0);
          Script_ResetHandMotion(CHARA_YAMA, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_YAMA, 1, data_2EA40, 8);
          Script_handSetFloatsFor(CHARA_YAMA, 0, data_2E95C, 0);
          break;
        case 838:
          Script_ResetHandMotion(CHARA_YAMA, 0, 1, 0);
          Script_CharHandlers(CHARA_YAMA, 0, 11, 16);
          Script_ResetHandMotion(CHARA_YAMA, 0, 0, 0);
          Script_ReadModelFormat7(CHARA_YAMA, 1, data_2EB8C, 0);
          break;
        case 2613:
          Script_ResetHandMotion(CHARA_YAMA, 1, 1, 0);
          Script_ResetHandMotion(CHARA_YAMA, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_YAMA, 1, data_2E95C, 0);
          Script_handSetFloatsFor(CHARA_YAMA, 0, data_2E95C, 0);
          break;
        case 2400:
          Script_ResetHandMotion(CHARA_YAMA, 0, 1, 0);
          Script_ResetHandMotion(CHARA_YAMA, 0, 0, 0);
          break;
        case 2740:
          Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_3B0;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_YAMA, 10, 0, 0);
          Script_FACE_LCG(CHARA_YAMA, FACE_BlinkClose, 0, 0);
          break;
        case 60:
          Script_FACE_LCG(CHARA_YAMA, FACE_BlinkAutomatic, 0, 0);
          break;
        case 100:
          Script_FACE(CHARA_YAMA, 1, 0, 40);
          break;
        case 678:
          Script_FACE(CHARA_YAMA, 7, 0, 0);
          break;
        case 1592:
          Script_FACE(CHARA_YAMA, 0, 0, 0);
          break;
        case 1795:
          Script_FACE(CHARA_YAMA, 2, 0, 30);
          break;
      }
    }
    task_3B0 = task_3B0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_23F47(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_2EBC0, 1, data_2EBD0, 2, data_2EBD5, 3, data_2EBDA, 5, data_2EBE1, 6, data_2EBE6, 7, data_2EBEB, 8, data_2EBF0, 11, data_2EC13, 12, data_2EC18, 13, data_2EC1D, 14, data_2EC22, 15, data_2EC2D, 16, data_2EC34, 17, data_2EC41)))
static void script_23FAE(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 400, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SECRET_OF_YIN_YANG, 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_261F7();
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x414D4159, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x414D4159);
  while (local_FC == 0) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x414D4159);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(CHARA_YAMA, MOTM_PoseNone, SCEN_Write);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "INYO") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "INYO", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadVoiceFile(1, "01INY");
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 1, "FREE", 0);
  Script_LoadSND(0, 1, "amb019.snd", 0);
  Script_LoadSND(0, 2, "bgm052.snd", 0);
  Script_LoadSND(0, 4, "a1_inyou.snd", 0);
  Script_executeAicaCommand(17832, 0, 0);
  Script_executeAicaCommand(1184, 2, 110);
  script_26216();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  local_F8 = Script_scenCreateTask(script_2394A, 0);
  script_238DF(0);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_23F47(CHARA_AKIR);
  Script_srchControl((void *)local_F8);
  if (local_FC) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_FC);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x414D4159, 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 1, "FREE", 0);
  Script_LoadSND(0, 1, "amb019.snd", 0);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_LoadSND(0, 4, "f1jutadn.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_26216();
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "INYO");
  script_2620A();
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_convSetCamInactiveFlag(1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 65.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 17.0f);
  Script_SetGameStateTo(AUTH_None);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 400, 0);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_2EE20, 1, data_2EE25, 2, data_2EE2A, 3, data_2EE2F, 6, data_2EE4C, 7, data_2EE5C, 8, data_2EE61, 9, data_2EE67, 10, data_2EE6C, 11, data_2EE6D, 12, data_2EE72, 13, data_2EE73, 14, data_2EE78, 15, data_2EE7D, 16, data_2EE82, 21, data_2EEB7, 22, data_2EEBC, 23, data_2EEC1, 24, data_2EEC6, 27, data_2EEE3, 28, data_2EEE8, 29, data_2EEED, 30, data_2EEF2, 31, data_2EEF7, 32, data_2EEFE, 33, data_2EF0B)))
static void script_2439B(void) {
  int32_t unused_E8[6];
  int8_t local_E7;
  uint8_t unused_E4[3];
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_TAIKE_EVENT_DONE, 1);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  Script_scenCreateTask(script_24E45, 0);
  Script_executeAicaCommand(2720, 2, 64);
  SCNF_WaitFrames_0000(60);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm116.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 86);
  Script_LoadSND(0, 4, "a1_taike.snd", 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5953494B, 1);
  local_E0 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x5953494B);
  while (local_E0 == 0) {
    SCNF_WaitFrames_0000(1);
    local_E0 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x5953494B);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_E0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(CHARA_KISY, MOTM_PoseNone, SCEN_Write);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "KSBT") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  script_24FC4();
  Script_LoadVoiceFile(1, "A0167");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "KSBT", "");
  Script_AuthorUtils(0, 1, "KSBT", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_executeAicaCommand(2720, 1, 64);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  local_E7 = 0;
  Script_SetCPAMFlags(CHARA_KISY, 0);
  Script_scenToggleVisibleFor(CHARA_KISY, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2EE10);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_2EE10);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  script_24F4A(0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2EE10);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_2EE10);
  Script_executeAicaCommand(2720, 2, 30);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCPAMFlags(CHARA_KISY, 1);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm013.snd", 0);
  Script_executeAicaCommand(9640, 0, 0);
  Script_executeAicaCommand(1184, 2, 104);
  Script_LoadSND(0, 4, "e1kishou.snd", 0);
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  local_D8 = Script_motiLoad(2, SHEN_NAME('JD00'), "M_FGT1.BIN");
  Script_vfSetup(0, 20, 20, 0, 0);
  {
    Vector3 local_C8;
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_C8, 0);
    Script_vfStageAssignRingBoundary(&local_C8, 5.0f);
  }
  Script_ryoSetChara(CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  task_0 = 0x52494B41;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 200;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(CHARA_AKIR, VF_FORMATION_APPLY_FLAGS, VF_HP_TEAM_BASED, 49);
  task_0 = 0x5953494B;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 200;
  *(&task_0 + 5) = 20;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(CHARA_KISY, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(CHARA_KISY, VF_FORMATION_APPLY_FLAGS, VF_HP_TEAM_BASED, 49);
  Script_vfConfigureFormationGroup(CHARA_AKIR, VF_FORMATION_APPLY_FLAGS, VF_SfxEventPending, 50);
  Script_vfConfigureFormationGroup(CHARA_KISY, VF_FORMATION_APPLY_FLAGS, VF_SfxEventPending, 50);
  SCN_Enqueue_VFGI(2, 99, 1.0f, 0, -1);
  local_E7 = 1;
  local_D4 = 7200;
  for (;;) {
    int32_t local_D0;
    if (Script_CheckTaskStatus(CHARA_AKIR)) {
      local_E7 = 3;
    }
    if (Script_CheckTaskStatus(CHARA_KISY)) {
      local_E7 = 4;
    }
    local_D0 = local_E7;
    if ((local_D0 == 3) | (local_D0 == 4)) {
      break;
      goto L02495B;
    }
    L02495B:;
    SCNF_WaitFrames_0000(1);
    local_D4 = local_D4 - 1;
    if (local_D4 <= 0) {
      break;
    }
  }
  Script_executeAicaCommand(2720, 2, 20);
  SCNF_WaitFrames_0000(10);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_vfToggleBattleState(scnPostBattleDelay);
  for (;;) {
    if (SCN_GetIsFighting() == 2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_motmToggleFlags(CHARA_KISY, SCEN_Disable, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x80);
  Script_SetCPAMFlags(CHARA_KISY, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x49414843, 1);
  local_DC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x49414843);
  while (local_DC == 0) {
    SCNF_WaitFrames_0000(1);
    local_DC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x49414843);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_DC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(CHARA_CHAI, MOTM_PoseNone, SCEN_Write);
  SCNF_WaitFrames_0000(1);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm024.snd", 0);
  Script_executeAicaCommand(12456, 0, 0);
  Script_executeAicaCommand(1184, 2, 86);
  Script_LoadSND(0, 4, "a1_bdygd.snd", 0);
  Script_scenToggleVisibleFor(CHARA_CHAI, 1);
  script_101CE(CHARA_CHAI);
  Script_charaVisUtils(CHARA_CHAI, 1);
  Script_SetCharaFlags(CHARA_CHAI, 4);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2EE10);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_2EE10);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 1);
  Script_SetCharaRefBoardPos(CHARA_CHAI, &data_2EE10);
  Script_motmGetCharaFlags(CHARA_KISY, MOTM_PoseNone, SCEN_Write);
  Script_SetMotnFlag(CHARA_KISY, -1);
  script_24F4A(1);
  Script_executeAicaCommand(2720, 2, 30);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 2);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 2);
  Script_mapmSetCamFlags(0, 1);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCameraSettings(5, 6, 1, 0);
  SCNF_WaitFrames_0000(1);
  script_2500F();
  SCNF_WaitFrames_0000(1);
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_DC);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x49414843, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_E0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5953494B, 0);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "KSBT");
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(30);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_LoadSND(0, 4, "f1jutadn.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_executeAicaCommand(2720, 1, 120);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_SetActionSelectorHUDElementVisibility(0);
  Script_ReallocateUNFO(local_D8);
  Script_LoadVoiceFile(0, "A0167");
  task_0 = -6;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 5;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_24E45(void) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = 60;
  if (Script_scenCharaGetMotAnimId(CHARA_AKIR) != 1374) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.7f);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  while (local_FC > 0) {
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_F0, 0);
    (&local_F0)->y = (&local_F0)->y & 0xFFFF;
    if ((&local_F0)->y > 0x8000) {
      Vector3i local_E4;
      (&local_E4)->y = -127;
      Script_EVT_SetRotation(CHARA_AKIR, -0x70000000, &local_E4, 0);
    } else {
      Vector3i local_E4;
      (&local_E4)->y = 127;
      Script_EVT_SetRotation(CHARA_AKIR, -0x70000000, &local_E4, 0);
    }
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC - 1;
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_24F4A(int32_t arg0) {
  int32_t unused_FC;
  int32_t local_F8;
  task_2F0 = arg0;
  local_F8 = arg0;
  switch (local_F8) {
    case 0:
      Script_scenCreateTask(script_25036, 0);
      Script_shenSeqUtils(arg0);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 1:
      Script_scenCreateTask(script_2521B, 0);
      Script_shenSeqUtils(arg0);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  Script_set_val_to_1(2, 0);
  return;
}

static void script_24FC4(void) {
  script_101CE(CHARA_AKIR);
  script_101CE(CHARA_KISY);
  (void)1;
  Script_facemotUtils(CHARA_KISY, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
  return;
}

static void script_2500F(void) {
  script_102A8(CHARA_AKIR);
  script_102A8(CHARA_KISY);
  script_102A8(CHARA_CHAI);
  return;
}

static int32_t script_25036(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 186:
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &data_2EF14, 2);
          break;
        case 220:
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
          break;
      }
      switch (local_FE) {
        case 0:
          Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
          Script_CharHandlers(CHARA_KISY, 0, 8, 16);
          Script_CharHandlers(CHARA_KISY, 1, 8, 16);
          Script_CharHandlers(CHARA_AKIR, 0, 8, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 8, 16);
          break;
        case 235:
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
          break;
        case 550:
          Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 1, 0, 0);
          break;
        case 580:
          Script_handSetFloatsFor(CHARA_KISY, 1, data_2ED2C, 50);
          Script_SetHandRootRotation(CHARA_KISY, 0, 0x15, &data_2EF20);
          Script_handSetFloatsFor(CHARA_KISY, 0, data_2EC48, 50);
          break;
      }
      switch (local_FE) {
        case 0:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          Script_SetMotnFlag(CHARA_AKIR, -1);
          break;
        case 200:
          Script_FACE(CHARA_AKIR, 2, 0, 30);
          break;
        case 587:
          Script_FACE(CHARA_AKIR, 0, 0, 30);
          Script_executeAicaCommand(2720, 2, 64);
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

static int32_t script_2521B(void) {
  int16_t local_FE;
  uint8_t unused_FC[2];
  Vector3 local_F0;
  int32_t unused_E4[3];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
          Script_ReadModelFormat7(CHARA_KISY, 0, data_2EF2C, 10);
          Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
          Script_CharHandlers(CHARA_KISY, 1, 0, 16);
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
          break;
        case 50:
          Script_CharHandlers(CHARA_KISY, 0, 8, 16);
          Script_CharHandlers(CHARA_KISY, 1, 8, 16);
          break;
        case 330:
          Script_CharHandlers(CHARA_AKIR, 0, 8, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 8, 16);
          break;
      }
      if (local_FE == 4) {
        Script_mapmSetCamFlags(0, 0);
      }
      if ((local_FE >= 760) & (local_FE <= 1141)) {
        Script_EVT_GetPosition(CHARA_KISY, 5, &local_F0, 0);
        Script_SetLookPointDetails(CHARA_AKIR, 0x18000, &local_F0, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 1);
      } else {
        if (local_FE == 1142) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
          goto L02539D;
        }
      }
      L02539D:;
      switch (local_FE) {
        case 65:
          Script_FACE(CHARA_KISY, 1, 4, 10);
          break;
        case 198:
          Script_FACE(CHARA_KISY, 0, 0, 0);
          break;
        case 360:
          Script_FACE(CHARA_AKIR, 2, 0, 0);
          break;
        case 655:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 0, 0);
          break;
        case 660:
          Script_FACE(CHARA_AKIR, 7, 0, 20);
          break;
        case 730:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 6, 0);
          Script_FACE(CHARA_AKIR, 0, 0, 30);
          break;
        case 1060:
          Script_FACE(CHARA_AKIR, 2, 0, 20);
          break;
        case 1150:
          Script_FACE(CHARA_AKIR, 0, 0, 30);
          break;
        case 1550:
          Script_executeAicaCommand(2720, 2, 30);
          break;
        case 1580:
          Script_scenCreateTask(script_25563, 0);
          break;
        case 1760:
          Script_FACE(CHARA_CHAI, 4, 4, 30);
          break;
      }
      switch (local_FE) {
        case 1900:
          Script_GetDataFromFightProgram(CHARA_CHAI, 1);
          Script_SetFightProgramStateBytes(CHARA_CHAI, 2, 1);
          break;
        case 1940:
          Script_GetDataFromFightProgram(CHARA_CHAI, 0);
          break;
        case 2040:
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
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

static int32_t script_25563(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 2, "tsm001.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  Script_executeAicaCommand(3496, 0, 0);
  Script_executeAicaCommand(1184, 2, 86);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(1, data_2EF6A)))
void script_255CA(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MOBJ.BIN");
  local_F8 = Script_ryoSleepUtils(0);
  script_26216();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 65.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 17.0f);
  Script_SetMapFlags(0, 1);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 19);
  script_2217A();
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    Script_SetShouldSleepFlag(0);
  }
  local_FC = Script_GetCurrentEntryPoint();
  SCNF_WaitFrames_0000(1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "JD00") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_219B3();
  script_10B19();
  if (local_FC == 20) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE, 1);
    SCNF_WaitFrames_0000(1);
  }
  script_DD78(CHARA_AKIR, (DSWKDoorEntry *)data_2917C, (int32_t *)data_29760, (DoorResourceDescriptor *)&data_29934, (MapObjectRecord *)data_29A24, 10, 0);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &task_2B8, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_AKIR, &task_2C4, 0x42000000);
  SCNF_WaitFrames_0000(1);
  task_2D8 = Script_scenCreateTask(script_21B77, 0);
  script_25E0D(CHARA_AKIR);
  task_2D0 = Script_scenCreateTask(script_0030, 6, CHARA_AKIR, 2, 0, &data_2E1F8, 2, 0);
  script_2643((int32_t)data_2E34C);
  Script_scenCreateTask(script_25909, 0);
  Script_scenCreateTask(script_22BB0, 0);
  Script_scenCreateTask(script_13CA5, 0);
  Script_scenCreateTask(script_2225B, 0);
  script_14C1C(0x4C455442, 9997, -0x3F5E84B6, 0, 0x4192C189, 0);
  script_26418();
  Script_MotStepUtil(CHARA_AKIR, 269);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_25933();
  script_E078();
  if (script_21DD1(9, 0, 19, 0)) {
    script_E175(0);
  }
  script_25C91();
  local_F4 = local_FC;
  switch (local_F4) {
    case 30:
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      script_23286(0);
      break;
    case 10:
      Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
      script_23FAE();
      break;
  }
  if (local_F8 != 0) {
    Script_ryoSleepUtils(1);
    task_2DE = Script_ryoSleepUtils(3, 0, 0);
    task_2D4 = Script_ryoSleepUtils(3, 0, 1);
  } else {
    Script_ryoSleepUtils(3, 1, 0, 0);
    Script_ryoSleepUtils(3, 1, 1, 0);
  }
  Script_ryoSleepUtils(2, 1);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static int32_t script_25909(void) {
  script_FECF();
  for (;;) {
    script_22806();
    if (script_FF21() != -1) {
      script_FEF9();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_25933(void) {
  script_E5F1(6, 0, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(0, 5, 4, -1, 0x2502AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(4, 6, 1, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(3, 7, 1, 6, 0x2202AB, 0x2702AB, 0x2602AB, 0x444E4F4B);
  DSWK_AssignDoorEntryConfig(9, 6, 1, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(16, 7, 1, 6, 0x2202AB, 0x2702AB, 0x2602AB, 0x4E454154);
  DSWK_AssignDoorEntryConfig(27, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(13, 7, 1, 3, 0x2202AB, 0x2702AB, 0x2602AB, 0x41544F4B);
  DSWK_AssignDoorEntryConfig(2, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(22, 6, 1, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(6, 7, 1, 3, 0x2102AB, 0x2902AB, 0x2802AB, 0x55534159);
  DSWK_AssignDoorEntryConfig(26, 2, 8, -1, -1, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(15, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(17, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(12, 6, 1, -1, 0x2002AB, -1, -1, -1);
  {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, 157);
    local_FC = local_FC & -65;
    local_FC = local_FC & -129;
    local_FC = local_FC | 0x40;
    Script_ScenDataUtil(task_E8, SCND_WriteU32, 157, local_FC);
  }
  DSWK_AssignDoorEntryConfig(23, 7, 1, 4, 0x2202AB, 0x2702AB, 0x2602AB, 0x53484B54);
  DSWK_AssignDoorEntryConfig(18, 6, 1, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(24, 6, 1, -1, 0x2002AB, -1, -1, -1);
  {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_E8, SCND_ReadU32, 313);
    local_FC = local_FC & -65;
    local_FC = local_FC & -129;
    local_FC = local_FC | 0x40;
    Script_ScenDataUtil(task_E8, SCND_WriteU32, 313, local_FC);
  }
  DSWK_AssignDoorEntryConfig(20, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(21, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(19, 6, 1, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(11, 7, 1, 5, 0x2202AB, 0x2702AB, 0x2602AB, 0x57474455);
  DSWK_AssignDoorEntryConfig(5, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(7, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(14, 7, 1, 2, 0x2202AB, 0x2702AB, 0x2602AB, 0x524F4F44);
  DSWK_AssignDoorEntryConfig(25, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(1, 7, 1, 5, 0x2202AB, 0x2702AB, 0x2602AB, 0x524F4F44);
  DSWK_AssignDoorEntryConfig(8, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(10, 6, 1, -1, 0x2202AB, -1, -1, -1);
  return;
}

static void script_25C91(void) {
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/02/D000", (int32_t)"SCRTEST.BIN", 0x3030444A);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  SCNF_WaitFrames_0000(1);
  script_260BE();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_25CC9(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_2F00C, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_2EFD4, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_2EF9C, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_2EFB8, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_2EFF0, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_25E01(int32_t arg0) {
  script_25E19(arg0);
  return;
}

static void script_25E0D(int32_t arg0) {
  script_25E19(arg0);
  return;
}

static void script_25E19(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L025E52;
    }
  }
  L025E52:;
  Script_ryoSetChara(arg0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_25CC9();
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

static void script_25F25(int32_t arg0) {
  script_25E0D(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_25F3C(int32_t arg0) {
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

static void script_25F7D(int32_t arg0) {
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

static void script_25FC1(int32_t arg0) {
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

static void script_2603F(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_25FC1(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_25FC1(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_25FC1(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_25FC1(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_25FC1(5);
  } else {
    script_25FC1(0);
  }
  return;
}

static void script_260BE(void) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    if (local_FF == 1) {
      break;
    }
    *(&task_3B4 + local_FF) = 0;
    local_FF = local_FF + 1;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE) == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x494B4B52, 1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4D544143, 1);
  }
  return;
}

static void script_26129(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
  while (local_FC == 0) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  *(&task_3B4 + arg1) = local_FC;
  return;
}

static void script_26190(int32_t arg0, int32_t arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, *(&task_3B4 + arg1));
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  *(&task_3B4 + arg1) = 0;
  return;
}

static void script_261BC(void) {
  int32_t unused_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  SCNF_WaitFrames_0000(1);
  if (*&task_3B4) {
    script_26190(0x414D4159, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_261F7(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  return;
}

static void script_2620A(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_26216(void) {
  Script_ambSoundUtils(1, 0, &data_2F044, 12.0f, 30.0f, 0.9f, 2, &data_2F0C8);
  Script_ambSoundUtils(2, 0, &data_2F050, 10.0f, 30.0f, 0.9f, 2, &data_2F150);
  Script_ambSoundUtils(3, 0, &data_2F05C, 5.0f, 20.0f, 0.9f, 2, &data_2F1D8);
  Script_ambSoundUtils(4, 0, &data_2F068, 10.0f, 25.0f, 0.9f, 2, &data_2F260);
  Script_ambSoundUtils(5, 0, &data_2F074, 22.0f, 30.0f, 0.9f, 2, &data_2F2E8);
  Script_ambSoundUtils(6, 0, &data_2F080, 5.0f, 20.0f, 0.9f, 2, &data_2F370);
  Script_ambSoundUtils(7, 0, &data_2F08C, 20.0f, 30.0f, 0.9f, 2, &data_2F3F8);
  Script_ambSoundUtils(8, 1, data_2F098, 50.0f, 30.0f, 1.0f, 0, data_2F480);
  Script_ambSoundUtils(9, 1, data_2F0A4, 50.0f, 30.0f, 1.0f, 0, data_2F508);
  Script_ambSoundUtils(10, 1, data_2F0B0, 50.0f, 30.0f, 1.0f, 0, &data_2F590);
  Script_ambSoundUtils(15, 1, data_2F0BC, 200.0f, 50.0f, 1.0f, 3, &data_2F618);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

__attribute__((scn_string_bindings(0, data_2F6A0, 1, data_2F6A5, 2, data_2F6AA, 3, data_2F6AF, 4, data_2F6B6)))
static void script_263AF(void) {
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_LoadSND(0, 4, "f1jutadn.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  return;
}

static void script_26418(void) {
  int32_t local_FC;
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_2F6C4, 21);
  local_FC = 0;
  while (local_FC < 21) {
    Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_FC, 1);
    local_FC = local_FC + 1;
  }
  return;
}
