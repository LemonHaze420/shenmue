#include "act03_jd00.h"

__attribute__((scn_string_bindings(0, data_251B8)))
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

__attribute__((scn_string_bindings(0, data_25257)))
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

__attribute__((scn_string_bindings(1, data_2527D)))
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

__attribute__((scn_string_bindings(0, data_25287)))
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

__attribute__((scn_string_bindings(0, data_2529F)))
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

__attribute__((scn_string_bindings(0, data_252B7)))
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

__attribute__((scn_string_bindings(0, data_252CF)))
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

__attribute__((scn_string_bindings(0, data_252E7)))
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

__attribute__((scn_string_bindings(0, data_252FF)))
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

__attribute__((scn_string_bindings(0, data_25317)))
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
      Script_sprintf(data_2532F);
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

__attribute__((scn_string_bindings(0, data_25ED0, 8, data_25F14, 9, data_25F1D, 10, data_25F26, 11, data_25F2F, 12, data_25F38, 13, data_25F41)))
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

__attribute__((scn_string_bindings(0, data_25F46, 1, data_25F4B, 2, data_25F54, 3, data_25F59, 4, data_25F5E, 5, data_25F67, 6, data_25F70, 7, data_25F79)))
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
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/stream/", arg1) == 0) {
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
      task_CC = (char *)data_25FB7;
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
    Script_charaPosRotUtils(arg0, 152, &data_25FBC, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg0, 153, &data_25FC8, 0, CHARA_PART_Set);
  } else {
    Script_charaPosRotUtils(arg0, 152, &data_25FC8, 0, CHARA_PART_Set);
    Script_charaPosRotUtils(arg0, 153, &data_25FBC, 0, CHARA_PART_Set);
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

__attribute__((scn_string_bindings(0, data_25FD4)))
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

__attribute__((scn_string_bindings(0, data_25FD9)))
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
  task_CC = (char *)data_25FDE;
  task_DC = 0;
  task_DD = 0;
  task_DE = 0;
  task_E4 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_25FE3, 1, data_25FEA, 2, data_25FF7)))
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
  task_E4 = Script_LoadHandMot(3, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
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

__attribute__((scn_string_bindings(0, data_25FFC, 2, data_26019)))
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
  task_E4 = Script_LoadHandMot(3, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
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

__attribute__((scn_string_bindings(0, data_26026, 1, data_2602D)))
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
        script_44EA(arg3, 1, (int32_t)data_26034, -1);
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
            task_CC = (char *)data_26086;
            task_D0 = 0;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 9:
            task_CC = (char *)data_2608B;
            task_D0 = 1;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 10:
            task_CC = (char *)data_26090;
            task_D0 = 2;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 11:
            task_CC = (char *)data_26095;
            task_D0 = 3;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 12:
            task_CC = (char *)data_2609A;
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
  script_4596(arg0, arg1, (int32_t)&data_260A0, -1);
  script_45FB(arg0, arg1, (int32_t)data_260AC, -1);
  script_44EA(arg0, 1, (int32_t)data_260B8, -1);
  script_44EA(arg0, 0, (int32_t)data_260B8, -1);
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

__attribute__((scn_string_bindings(3, data_26121, 4, data_2612C)))
static void script_5350(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg2 == 0) {
    script_4129(task_C8, task_CC);
  }
  script_42CE(task_C8, arg0, 18, (int32_t)&data_260C4, (int32_t)&data_260D0, 82);
  script_4858(task_C8, 1, 0);
  script_4596(arg0, arg1, (int32_t)&data_260DC, -1);
  script_45FB(arg0, arg1, (int32_t)data_260E8, -1);
  script_44EA(arg0, 1, (int32_t)&data_260F4, -1);
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

__attribute__((scn_string_bindings(0, data_26198, 1, data_261A3)))
static void script_5641(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 == 0) {
    script_4129(task_C8, task_CC);
  }
  if (task_D0 == 4) {
    script_42CE(task_C8, arg0, 18, (int32_t)&data_26150, (int32_t)&data_2615C, 82);
    script_4596(arg0, arg1, (int32_t)&data_26168, -1);
    script_45FB(arg0, arg1, (int32_t)data_26174, -1);
    script_44EA(arg0, 1, (int32_t)data_2618C, -1);
    script_44A3(arg0, 1, 544, -1);
  } else {
    script_42CE(task_C8, arg0, 18, (int32_t)&data_26138, (int32_t)&data_26144, 82);
    script_4596(arg0, arg1, (int32_t)&data_26168, -1);
    script_45FB(arg0, arg1, (int32_t)data_26174, -1);
    script_44EA(arg0, 1, (int32_t)&data_26180, -1);
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
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, *(&data_25F80 + local_F4))) {
      Script_EVT_GetPosition(*(&data_25F80 + local_F4), -1, (Vector3 *)local_90[local_F4], ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(*(&data_25F80 + local_F4), (Vector3i *)local_FEC4[local_F4], 0x40000000);
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
      script_4A53(arg0, *(&data_25F80 + local_F9), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
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
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_E9DD(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_F0A6(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_271CC, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_271CC, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 4);
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
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_271CC, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_271CC, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_E9DD(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_130[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_C99B(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_271CC, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_271CC, SCND_ReadFloatBits, Script_ScenDataUtil(task_E8, SCND_ReadU32, task_130[arg2] * 13 + 6) * 5 + 4);
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

__attribute__((scn_string_bindings(0, data_27CEA)))
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_248) {
        local_FF78 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 8);
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

__attribute__((scn_string_bindings(0, data_27D06)))
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
          local_FC = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
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

__attribute__((scn_string_bindings(0, data_27D22)))
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
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 7);
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
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 8);
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

__attribute__((scn_string_bindings(0, data_27D3E)))
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

__attribute__((scn_string_bindings(0, data_27D5A)))
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
      local_8C = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_267AC, SCND_ReadU32, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_267AC, SCND_ReadFloatBits, Script_ScenDataUtil(data_261BC, SCND_ReadU8, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 4);
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

__attribute__((scn_string_bindings(0, data_27D76)))
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
        local_E0 = Script_ScenDataUtil(&data_271CC, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_271CC, SCND_ReadU32, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_271CC, SCND_ReadU32, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_271CC, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_271CC, SCND_ReadU32, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_271CC, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_271CC, SCND_ReadU32, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_271CC, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L00C745;
    }
  }
  L00C745:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_271CC, SCND_ReadFloatBits, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_271CC, SCND_ReadFloatBits, local_D4 * 5 + 2);
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

__attribute__((scn_string_bindings(0, data_27D92)))
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
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_F8, task_108, task_130, (shen_name *)task_158, task_180, arg0, arg5, data_261BC, data_267AC, (shen_name *)task_1D0);
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

__attribute__((scn_string_bindings(0, data_27E1D, 1, data_27E36)))
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

__attribute__((scn_string_bindings(0, data_27E4F)))
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

__attribute__((scn_string_bindings(0, data_27E5E)))
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

__attribute__((scn_string_bindings(0, data_27E6D, 1, data_27E7F)))
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
      task_1F8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_2726C, SCND_ReadScriptOffset, local_F8), 4, task_158[arg0], task_1D0[arg0], arg0, local_F8);
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
    task_1F8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_2726C, SCND_ReadScriptOffset, arg2), 4, task_158[arg0], task_1D0[arg0], arg0, arg2);
    task_1A8[arg0] = task_158[arg0];
    script_E7DD(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_27E92, 1, data_27EA4)))
static void script_FB36(int32_t arg0, int32_t arg1) {
  if ((task_1F8[arg0] == 0) & (arg1 >= 0)) {
    script_F0D8(task_158[arg0], task_1D0[arg0], task_130[arg0], arg1);
    task_1F8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_2726C, SCND_ReadScriptOffset, arg1), 4, task_158[arg0], task_1D0[arg0], arg0, arg1);
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

__attribute__((scn_string_bindings(0, data_28F2C)))
static void script_FECF(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_28F40)))
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

static void script_FF06(void) {
  task_2E0 = Script_freeTalkGetCurrentChara();
  return;
}

static void script_FF12(void) {
  *(&task_2E0 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_FF21(void) {
  return *(&task_2E0 + 2) = Script_freeTalkGetAndResetState();
}

__attribute__((scn_string_bindings(1, data_28F5F)))
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
    Script_EVT_SetPosition(SHEN_NAME('GT01'), POS_XYZ, &data_28F68);
    Script_EVT_SetPosition(SHEN_NAME('GT02'), POS_XYZ, &data_28F74);
    Script_EVT_SetPosition(SHEN_NAME('GT03'), POS_XYZ, &data_28F80);
    Script_EVT_SetRotation(SHEN_NAME('GT01'), 0x38000000, &data_28F8C, 0);
    Script_EVT_SetRotation(SHEN_NAME('GT02'), 0x38000000, &data_28F98, 0);
    Script_EVT_SetRotation(SHEN_NAME('GT03'), 0x38000000, &data_28FA4, 0);
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
    Script_gachaponUtils(0, data_2912C);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_291D5);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_29310);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_29445);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_294EC);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_29659);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_29852);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_29925);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_29996);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_299E3);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_29A1C);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_29B0B);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_29B6E);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_29BC3);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_29C0A);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_29C43);
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
      local_FE = *(int16_t *)((int8_t *)&data_28FD0 + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_28FE8 + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_2900C + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_29038 + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_2904C + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_29074 + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_29098 + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_290B0 + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_290C0 + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_290C8 + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_290D0 + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_290F4 + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_29104 + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_29110 + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_2911C + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_29124 + (arg1 << 1));
      break;
  }
  return local_FE;
}

static void script_10840(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_28FB0[*(int32_t *)arg0] - data_28FC0[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_28FB0[*(int32_t *)arg0] - data_28FC0[*(int32_t *)arg0]);
  return;
}

static int32_t script_1088F(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_28FB0[arg0] - data_28FC0[arg0]) {
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
    arg2 = arg2 + data_28FB0[arg1];
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

__attribute__((scn_string_bindings(0, data_29D3C)))
static void script_10B19(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "GACH") == 0) {
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
      local_D8 = *(&data_29C7C + local_FC);
      local_E8.x = *(float *)(data_29CC4 + local_FC * 20);
      (&local_E8)->y = *(float *)(data_29CC4 + (local_FC * 20 + 4));
      (&local_E8)->z = *(float *)(data_29CC4 + (local_FC * 20 + 8));
      local_DC.x = 0;
      (&local_DC)->y = *(int32_t *)(data_29CC4 + (local_FC * 20 + 12));
      (&local_DC)->z = 0;
      Script_EVT_SetPosition(local_D8, POS_XYZ, &local_E8);
      Script_EVT_SetRotation(local_D8, 0x38000000, &local_DC, 0);
      Script_charaDisplay(local_D8, CDM_Show);
      Script_SetOrGetWOCCValue(local_D8, *((int8_t *)data_29CC4 + (local_FC * 20 + 16)));
      Script_MoveObjectToObject(*(&data_29CAC + local_FC), *(&data_29C7C + local_FC));
      Script_EVT_SetPosition(*(&data_29CAC + local_FC), POS_XYZ, &data_29D54);
      Script_EVT_SetRotation(*(&data_29CAC + local_FC), 0x38000000, &data_29D60, 0);
      Script_charaDisplay(*(&data_29CAC + local_FC), CDM_Show);
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
      local_D0 = *(&data_29C7C + local_E0);
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
      local_F0 = *(&data_29C7C + local_FE);
      if (script_10CC2(local_F0)) {
        Script_scenToggleVisibleFor(local_F0, 1);
        Script_scenToggleVisibleFor(*(&data_29CAC + local_FE), 1);
      } else {
        Script_scenToggleVisibleFor(local_F0, 0);
        Script_scenToggleVisibleFor(*(&data_29CAC + local_FE), 0);
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

__attribute__((scn_string_bindings(0, data_29D6C, 2, data_29D7F, 3, data_29D8F)))
static int32_t script_110B0(void) {
  Script_LoadVoiceFile(1, "E1066");
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 7, "e1gachap.snd", 0x4000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "HI_GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_29D97, 1, data_29D9C, 2, data_29DA2)))
static void script_11142(void) {
  Script_LoadSND(0, 7, "FREE", 0);
  Script_LoadVoiceFile(0, "E1066");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "HI_GACH");
  return;
}

__attribute__((scn_string_bindings(0, data_29DAA)))
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
      local_F0 = *(&data_29C7C + local_FC);
      local_EC = *(&data_29C94 + local_FC);
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
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_29DB4);
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
    if (task_2F8 == *(&data_29C7C + local_E8)) {
      local_EC = *((int8_t *)data_29CC4 + (local_E8 * 20 + 17));
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
      local_FF18 = (int32_t)data_29DC4;
      break;
    case 1:
      local_FF18 = (int32_t)data_29EA8;
      break;
    case 2:
      local_FF18 = (int32_t)data_29F8C;
      break;
    case 3:
      local_FF18 = (int32_t)data_2A070;
      break;
    case 4:
      local_FF18 = (int32_t)data_2A154;
      break;
    case 5:
      local_FF18 = (int32_t)data_2A238;
      break;
    case 6:
      local_FF18 = (int32_t)data_2A31C;
      break;
    case 7:
      local_FF18 = (int32_t)data_2A400;
      break;
    case 8:
      local_FF18 = (int32_t)data_2A4E4;
      break;
    case 9:
      {
        int32_t local_FF10;
        local_FF10 = 0;
        while (local_FF10 < 57) {
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_2A238 + (local_FF10 << 2));
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
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_2A238 + (local_FF10 << 2));
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
  local_F4.x = *(float *)&data_2A5D4;
  (&local_F4)->y = *(float *)(&data_2A5D4 + 1);
  (&local_F4)->z = *(float *)(&data_2A5D4 + 2);
  local_F0 = 20;
  while (local_F0) {
    local_F4.x = (&local_F4)->x + ((&data_2A5C8)->x - *(float *)&data_2A5D4) / 20.0f;
    (&local_F4)->y = (&local_F4)->y + ((&data_2A5C8)->y - *(float *)(&data_2A5D4 + 1)) / 20.0f;
    (&local_F4)->z = (&local_F4)->z + ((&data_2A5C8)->z - *(float *)(&data_2A5D4 + 2)) / 20.0f;
    Script_fixoClearAttachment(task_2FC);
    Script_fixoAttachEx(task_2FC, CHARA_AKIR, 12, &local_F4, &data_2A5E0);
    local_F0 = local_F0 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoAttachEx(task_2FC, CHARA_AKIR, 12, &data_2A5C8, &data_2A5E0);
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
      if (task_2F8 == *(&data_29C7C + local_FC)) {
        local_F8 = *(&data_29C94 + local_FC);
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
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_2A5EC);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_2A5F8, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_gachaponUtils(3, 0x4849454B, &data_2A61C);
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
    Script_fixoSetAttachmentOffset(SHEN_NAME('KEIH'), 0, &data_2A5EC, &data_2A5F8);
    local_C4.x = 0;
    (&local_C4)->y = -(&local_E8)->x;
    (&local_C4)->z = 0;
    Script_fixoAttachEx(SHEN_NAME('KEIH'), SHEN_NAME('YKUR'), 18, &data_2A604, &local_C4);
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
  Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &data_2A610);
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

__attribute__((scn_string_bindings(5, data_2A65A)))
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
  Script_SetMotnFlags(CHARA_AKIR, 17, &data_2A724, 0x39000000);
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
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_2A730);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_2A73C, 0);
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
      if (data_28FC0[local_F0] != 0) {
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
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_28FB0[local_F0]);
      break;
    case 1:
      local_F0 = 14;
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_28FB0[local_F0]);
      break;
    case 2:
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_28FB0[local_F0] - data_28FC0[local_F0]);
      break;
    case 3:
      local_D8 = data_28FB0[local_F0] - data_28FC0[local_F0] - 1;
      local_D4 = 0;
      while (local_D4 < data_28FC0[local_F0]) {
        if (Script_GetOwnedItemCount(script_10655(local_F0, local_D8 + local_D4), 0) == 0) {
          if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.666666687f)) {
            break;
          }
        }
        local_D4 = local_D4 + 1;
      }
      if (local_D4 < data_28FC0[local_F0]) {
        local_F4 = data_28FB0[local_F0] - local_D4 - 1;
      } else {
        local_D8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_28FC0[local_F0]) + 1;
        local_F4 = data_28FB0[local_F0] - local_D8;
      }
      break;
    case 4:
      local_F0 = 15;
      local_D8 = 0;
      while (local_D8 < data_28FB0[local_F0]) {
        if (Script_GetOwnedItemCount(script_10655(local_F0, local_D8), 0) == 0) {
          if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.333333343f)) {
            break;
          }
        }
        local_D8 = local_D8 + 1;
      }
      if (local_D8 < data_28FB0[local_F0]) {
        local_F4 = local_D8;
      } else {
        local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_28FB0[local_F0]);
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
  Script_InvestigationUtils(ShenmueUtils_SetSpecialAssetList, data_2A78C);
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_2A79C);
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
      script_13E2B(1);
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
      } else {
        if (local_DC == 136) {
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE, 1);
            Script_srchUtils(SRCH_SCRIPT_Reset);
            script_23E0B(1);
          }
          goto L013E1F;
        }
      }
    }
    L013E1F:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_13E2B(int32_t arg0) {
  if ((Script_IsPlayerAllowed(0) & -(arg0 == 0)) | (arg0 == 1)) {
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_sicnUtils(1, 0, 0);
    *((int8_t *)&*(int32_t *)&task_2DC + 1) = 0;
    Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, data_2A754);
    Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_2ACC0, 60);
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

static int32_t script_13F38(int32_t arg0) {
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

__attribute__((scn_string_bindings(0, data_2ACCC, 3, data_2ACE6, 7, data_2AD0A)))
static int32_t script_13F93(void) {
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
      goto L014093;
    }
  }
  L014093:;
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

static int32_t script_140D5(void) {
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

static void script_1417B(void) {
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

static int32_t script_141D4(void) {
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

static int32_t script_1420E(int32_t arg0) {
  Script_EVT_SetPosition(arg0, 0x78000000, &data_2AD10);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_ccowUtils(arg0, CCOW_DisableAll);
}

static void script_14231(void) {
  SCNF_WaitFrames_0000(2);
  while (Script_IsInEventCam() != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_14251(int32_t arg0) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 5, "e1shugyo.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static void script_1429C(void) {
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

static void script_142E3(void) {
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

static void script_14336(void) {
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

static void script_14392(void) {
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

static void script_143D9(void) {
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

__attribute__((scn_string_bindings(0, data_2ADA1)))
static void script_14435(void) {
  int32_t unused_F0[4];
  if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1) == 0) {
    Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  Script_LoadVoiceFile(1, "E1040");
  Script_scenCreateTask(script_1420E, 1, SHEN_NAME('LUV2'));
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_scenCreateTask(script_14251, 1, 1);
  Script_scenCreateTask(script_141D4, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MIMAWASU_K, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_2ADA7)))
static void script_14503(void) {
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

static void script_14595(void) {
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

static void script_145D2(void) {
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

static int32_t script_1460F(int32_t arg0) {
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

static void script_14680(int32_t arg0) {
  Vector3i local_F4;
  Script_uiUtils(0, 15, 0);
  Script_vfSetup(0, 20, 20, 0, 0);
  Script_vfStageAssignRingBoundary(&data_2ADB0, 6.0f);
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
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_2ADBC);
  (&local_F4)->y = 0x7000;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(2903, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_14231();
  SCN_Enqueue_VFGI(2, 99, 1.0f, 4, -1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_scenCreateTask(script_13F38, 1, arg0);
  return;
}

static int32_t script_1489B(void) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_GetFightingStats(-1, 2) == 1) {
    local_FC = 1;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    local_FC = 2;
  }
  if (script_140D5()) {
    local_FC = 2;
  }
  return local_FC;
}

static void script_148E3(void) {
  script_14392();
  return;
}

static void script_148EB(void) {
  SCN_Enqueue_VFGI(2, 99, 1.0f, 99, -1);
  return;
}

static void script_148FF(void) {
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
  if (script_13F93()) {
    local_FE = 1;
  }
  if (local_FE == 0) {
    script_14435();
    local_FF = 1;
  }
  for (;;) {
    script_14595();
    script_145D2();
    if (local_FE == 1) {
      break;
      local_FF = 14;
    }
    SCNF_WaitFrames_0000(1);
    local_F4 = local_FF;
    if (local_F4 == 1) {
      script_1429C();
      local_FF = 2;
    } else if (local_F4 == 2) {
      script_142E3();
      local_FF = 4;
    } else if ((local_F4 == 3) | (local_F4 == 13) | (local_F4 == 14)) {
      break;
    } else if ((local_F4 == 4) | (local_F4 == 12)) {
      local_FF = local_FF + script_1460F(local_FF);
    } else if (local_F4 == 5) {
      local_FE = 2;
      script_14336();
      script_14680(0);
      local_FF = 6;
    } else if (local_F4 == 6) {
      local_FF = local_FF + script_1489B();
    } else if ((local_F4 == 7) | (local_F4 == 8)) {
      local_FF = local_FF - 7 + 9;
    } else {
      switch (local_F4) {
        case 9:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_148E3();
            local_FF = 12;
          }
          break;
        case 10:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_148E3();
            local_FF = 14;
          }
          break;
        case 11:
          script_148EB();
          local_FF = 6;
          break;
      }
    }
  }
  if (local_FE != 1) {
    script_14503();
  }
  if (local_FE == 2) {
    script_1417B();
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

static void script_14C01(int32_t arg0, int32_t arg1) {
  task_304 = arg0;
  script_151FF(arg1);
  task_318 = 9999;
  task_31C = 9999;
  task_320 = 0;
  task_324 = 0;
  task_32D = 0;
  task_330 = 0;
  script_14F39();
  task_334 = 0;
  task_308 = 0x52554B59;
  task_30C = 0x4C554B59;
  return;
}

static void script_14C60(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  script_14C01(arg0, arg1);
  task_328 = Script_scenCreateTask(script_14C87, 4, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_14C87(float arg0, float arg1, float arg2, float arg3) {
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
          script_1536B();
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

static int32_t script_14E25(void) {
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

static void script_14EEC(const char *arg0, int32_t arg1) {
  task_338[arg1] = Script_TELMUtils(TELM_CMD_RegisterPhoneNumber, task_304, arg0);
  if (task_310 == arg1) {
    task_318 = task_338[arg1];
  }
  if (task_314 == arg1) {
    task_31C = task_338[arg1];
  }
  return;
}

static void script_14F39(void) {
  script_14EEC((const char *)&data_2AFC8, 0);
  script_14EEC((const char *)&data_2AFCC, 1);
  script_14EEC((const char *)&data_2AFD0, 2);
  script_14EEC((const char *)&data_2AFD4, 7);
  script_14EEC("61564", 3);
  script_14EEC("37742", 4);
  script_14EEC("37749", 5);
  script_14EEC("22550", 6);
  script_14EEC("046861564", 8);
  script_14EEC("046837742", 9);
  script_14EEC("046837749", 10);
  script_14EEC("046822550", 11);
  script_14EEC("2?????", 12);
  script_14EEC("5?????", 13);
  script_14EEC("7?????", 14);
  script_14EEC("8?????", 15);
  script_14EEC("3?????", 16);
  script_14EEC("4?????", 17);
  script_14EEC("6?????", 18);
  script_14EEC("9?????", 19);
  script_14EEC("0?????????", 20);
  script_14EEC("1?????", 21);
  return;
}

static int32_t script_15070(int32_t arg0) {
  if (arg0 > 0) {
    if ((arg0 == task_318) | (arg0 == task_31C)) {
      Script_scenCreateTask(script_14E25, 0);
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

static void script_150E9(void) {
  if (task_32D == 0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 185, 1);
    }
    task_32D = script_1514B();
    if (task_32D == 0) {
      task_32D = task_32D + 1;
      if (task_32E) {
        task_32F = 1;
      }
    }
  }
  return;
}

static int8_t script_1514B(void) {
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

__attribute__((scn_string_bindings(0, data_2B062, 3, data_2B07C, 4, data_2B086)))
static void script_15196(int32_t arg0) {
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

static void script_151FF(int32_t arg0) {
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

static void script_1522A(void) {
  Script_convSetCamInactiveFlag(0);
  if ((task_310 == 9998) | (task_310 == 9997)) {
    SCNF_SetCashValue(10, 1);
  }
  return;
}

static int32_t script_15251(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours <= 4) {
    arg0 = -1;
  } else {
    if (((&local_F4)->hours >= 23) & ((&local_F4)->minutes >= 30)) {
      arg0 = -1;
      goto L01528F;
    }
  }
  L01528F:;
  return arg0;
}

static int8_t script_15293(int32_t arg0) {
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

static void script_1536B(void) {
  script_150E9();
  script_15378();
  return;
}

static void script_15378(void) {
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
        goto L0153FC;
      }
    }
    L0153FC:;
    Script_scenToggleVisibleFor(task_30C, 1);
    if (0) {
      Script_charaDisplay(task_308, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_308, CDM_Show);
        goto L01542A;
      }
    }
    L01542A:;
    Script_scenToggleVisibleFor(task_308, 1);
    Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, task_30C, task_308);
    local_D8 = Script_charaSetClipAngle(task_304, -1.0f);
    Script_charaSetClipAngle(task_304, 1.0f);
    SCNF_WaitFrames_0000(1);
    script_213A2();
    script_16347();
    script_157E2();
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
          local_D3 = script_1514B();
          if (local_D3) {
            script_15196(local_D3);
            Script_sicnUtils(1, 0, 0);
            Script_uiUtils(0, 0, 0);
            break;
          } else {
            Script_sicnUtils(1, 0, 0);
            script_157E2();
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
            goto L015583;
          }
        }
        L015583:;
        local_E0 = local_E0 + 1;
      } else {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          MMTK_ScriptMemoToggle(1, 0);
          goto L0155A7;
        }
      }
      L0155A7:;
      SCNF_WaitFrames_0000(1);
    }
    if ((task_310 == 9998) | (task_310 == 4)) {
      script_21711();
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
    script_214A1();
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
        goto L0156A2;
      }
    }
    L0156A2:;
    Script_scenToggleVisibleFor(task_308, 0);
    if (-1) {
      Script_charaDisplay(task_30C, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_30C, CDM_Show);
        goto L0156D0;
      }
    }
    L0156D0:;
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
      script_213A2();
      script_15196(task_32D);
      script_214A1();
      Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_ryoControl(RYO_RestoreControl);
      Script_convSetCamInactiveFlag(1);
      Script_SetGameStateTo(AUTH_None);
      task_32D = 0;
      task_32F = 0;
      goto L0157E1;
    }
  }
  L0157E1:;
  return;
}

__attribute__((scn_string_bindings(1, data_2B098, 7, data_2B0D0, 8, data_2B0D6, 13, data_2B104, 14, data_2B10A, 15, data_2B110, 16, data_2B116, 18, data_2B126, 21, data_2B144, 23, data_2B154, 25, data_2B168, 28, data_2B182)))
static void script_157E2(void) {
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
    task_394 = script_15070(task_394);
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
      script_16405();
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
      script_1522A();
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
          script_1522A();
          local_EA = 0;
        }
      }
      if (task_394 == task_338[2]) {
        local_EB = 1;
      }
      if ((task_394 == task_338[4]) | (task_394 == task_338[9])) {
        local_E9 = script_15251(local_E9);
        if (local_E9 == -1) {
          task_394 = -10;
          local_EA = 0;
        }
      }
      if ((task_394 == task_338[3]) | (task_394 == task_338[8])) {
        local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0);
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) | (local_F0 >= 16)) {
          local_E6 = script_15251(local_E6);
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
        local_E8 = script_18CB1();
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
        local_E7 = script_19FD8();
        local_E7 = script_15251(local_E7);
        if (local_E7 == -1) {
          task_394 = -10;
          local_EA = 0;
        }
      }
      if (local_EA) {
        if (script_15293(local_EB)) {
          Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
          Script_shenClearFreeState(0);
          Script_LoadVoiceFile(1, "E1032");
          script_16405();
          script_21514();
          local_E5 = 0;
          if (task_394 == task_338[7]) {
            script_1642A();
          }
          if (task_394 == task_338[2]) {
            script_16B71();
          }
          if ((task_394 == task_338[5]) | (task_394 == task_338[10])) {
            script_18E67(local_E8);
          }
          if ((task_394 == task_338[6]) | (task_394 == task_338[11])) {
            script_1A56D(local_E7);
          }
          if ((task_394 == task_338[3]) | (task_394 == task_338[8])) {
            local_E5 = script_1EEF3();
          }
          if ((task_394 == task_338[4]) | (task_394 == task_338[9])) {
            script_1E42B();
          }
          script_217D9();
          if (local_E5) {
            script_1F47A();
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED, 1);
          }
          Script_LoadVoiceFile(0, "E1032");
          script_216FD();
          Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
          Script_shenClearFreeState(1);
          task_394 = 0;
        } else {
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
          script_1522A();
        }
      }
    }
    if (task_394 == -10) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_16405();
      if (script_15293(6)) {
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
      script_1522A();
    }
    if (task_394 == -11) {
      int32_t local_D0;
      Script_scenCreateTask(script_14E25, 0);
      script_16405();
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
      script_1522A();
    }
    if ((task_394 == task_338[12]) | (task_394 == task_338[13]) | (task_394 == task_338[14]) | (task_394 == task_338[15]) | (task_394 == task_338[16]) | (task_394 == task_338[17]) | (task_394 == task_338[18]) | (task_394 == task_338[19]) | (task_394 == task_338[20]) | (task_394 == task_338[21])) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_16405();
      Script_scenCreateTask(script_14E25, 0);
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
      script_1522A();
    }
    if (task_394 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(6219, task_304, task_304);
  script_16347();
  Script_sicnUtils(0, 5, 1);
  Script_uiUtils(0, 9, 0);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_16347(void) {
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

static void script_16405(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_GetStrayCatSceneVisitCountSlot, 2);
  local_FC = local_FC + 1;
  Script_PlayerStatsUtils(PS_SetStrayCatSceneVisitCountSlot, 2, local_FC);
  return;
}

__attribute__((scn_string_bindings(0, data_2B188, 5, data_2B1BA, 6, data_2B1C4, 7, data_2B1CE, 8, data_2B1D8, 14, data_2B214, 16, data_2B228, 17, data_2B232, 18, data_2B23C, 19, data_2B246, 21, data_2B25A, 22, data_2B264, 23, data_2B26E, 24, data_2B278, 25, data_2B282)))
static void script_1642A(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0)) {
    int32_t local_F8;
    task_320 = 81;
    Script_scenCreateTask(script_20A20, 0);
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
    Script_scenCreateTask(script_20A20, 0);
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

__attribute__((scn_string_bindings(5, data_2B318, 6, data_2B322, 7, data_2B32C, 8, data_2B336, 9, data_2B340, 10, data_2B34A, 11, data_2B354, 12, data_2B35E, 13, data_2B368, 14, data_2B372, 15, data_2B37C, 16, data_2B386, 17, data_2B390, 18, data_2B39A, 19, data_2B3A4, 20, data_2B3AE, 21, data_2B3B8, 22, data_2B3C2, 23, data_2B3CC, 24, data_2B3D6, 25, data_2B3E0, 26, data_2B3EA, 27, data_2B3F4, 28, data_2B3FE, 29, data_2B408, 30, data_2B412, 31, data_2B41C, 32, data_2B426, 33, data_2B430, 34, data_2B43A, 35, data_2B444, 36, data_2B44E, 37, data_2B458, 38, data_2B462, 39, data_2B46C, 40, data_2B476, 41, data_2B480, 42, data_2B48A, 43, data_2B494, 44, data_2B49E, 45, data_2B4A8, 46, data_2B4B2, 47, data_2B4BC, 48, data_2B4C6, 49, data_2B4D0, 50, data_2B4DA, 51, data_2B4E4, 52, data_2B4EE, 53, data_2B4F8, 54, data_2B502, 55, data_2B50C, 56, data_2B516, 57, data_2B520, 58, data_2B52A, 59, data_2B534, 60, data_2B53E, 61, data_2B548, 62, data_2B552, 63, data_2B55C, 64, data_2B566, 65, data_2B570, 66, data_2B57A, 67, data_2B584, 68, data_2B58E, 69, data_2B598, 70, data_2B5A2, 71, data_2B5AC, 72, data_2B5B6, 73, data_2B5C0, 74, data_2B5CA, 75, data_2B5D4, 76, data_2B5DE, 77, data_2B5E8, 78, data_2B5F2, 79, data_2B5FC, 80, data_2B606, 81, data_2B610, 82, data_2B61A, 83, data_2B624, 84, data_2B62E, 85, data_2B638, 86, data_2B642, 87, data_2B64C, 88, data_2B656, 89, data_2B660, 90, data_2B66A, 91, data_2B674, 92, data_2B67E, 93, data_2B688, 94, data_2B692, 95, data_2B69C, 96, data_2B6A6, 97, data_2B6B0, 98, data_2B6BA, 99, data_2B6C4, 100, data_2B6CE, 101, data_2B6D8, 102, data_2B6E2, 103, data_2B6EC, 104, data_2B6F6, 105, data_2B700, 106, data_2B70A, 107, data_2B714, 108, data_2B71E, 109, data_2B728, 110, data_2B732, 111, data_2B73C, 112, data_2B746, 113, data_2B750, 114, data_2B75A, 115, data_2B764, 116, data_2B76E, 117, data_2B778, 118, data_2B782, 119, data_2B78C, 120, data_2B796, 121, data_2B7A0, 122, data_2B7AA, 123, data_2B7B4, 124, data_2B7BE, 125, data_2B7C8, 126, data_2B7D2, 127, data_2B7DC, 128, data_2B7E6, 129, data_2B7F0, 130, data_2B7FA, 131, data_2B804, 132, data_2B80E, 133, data_2B818, 134, data_2B822, 135, data_2B82C, 136, data_2B836, 137, data_2B840, 138, data_2B84A, 139, data_2B854, 140, data_2B85E, 141, data_2B868, 142, data_2B872, 143, data_2B87C, 144, data_2B886, 145, data_2B890, 146, data_2B89A, 147, data_2B8A4, 148, data_2B8AE, 149, data_2B8B8, 150, data_2B8C2, 151, data_2B8CC, 152, data_2B8D6, 153, data_2B8E0, 154, data_2B8EA, 155, data_2B8F4, 156, data_2B8FE, 157, data_2B908, 158, data_2B912, 159, data_2B91C, 160, data_2B926, 161, data_2B930, 162, data_2B93A)))
static void script_16B71(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  task_320 = 70;
  Script_scenCreateTask(script_20A20, 0);
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

static int8_t script_18BF8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static int8_t script_18CB1(void) {
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
  if (script_18BF8(8, 30, 15, 10)) {
    local_EE = 0;
  } else if (script_18BF8(15, 10, 20, 30)) {
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
      goto L018DA6;
    }
  }
  L018DA6:;
  if (local_EF == 0) {
    local_EF = script_15251(local_EF);
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

__attribute__((scn_string_bindings(0, data_2B944, 8, data_2B994, 9, data_2B99E, 14, data_2B9D0, 17, data_2B9EE, 18, data_2B9F8, 19, data_2BA02, 21, data_2BA16, 25, data_2BA3E, 27, data_2BA52, 33, data_2BA8E, 40, data_2BAD4, 41, data_2BADE, 42, data_2BAE8, 43, data_2BAF2, 44, data_2BAFC, 48, data_2BB24, 49, data_2BB2E, 52, data_2BB4C, 53, data_2BB56, 57, data_2BB7E, 59, data_2BB92, 61, data_2BBA6, 62, data_2BBB0, 70, data_2BC00, 75, data_2BC32, 76, data_2BC3C, 77, data_2BC46, 82, data_2BC78, 83, data_2BC82, 84, data_2BC8C, 85, data_2BC96, 86, data_2BCA0, 87, data_2BCAA, 88, data_2BCB4, 92, data_2BCDC, 93, data_2BCE6)))
static void script_18E67(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 1:
      task_320 = 170;
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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

static int8_t script_19FD8(void) {
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
    if (script_18BF8(8, 30, 21, 0)) {
      local_EF = 1;
    } else {
      local_EF = 2;
    }
  } else if ((local_F0 == 13) | (local_F0 == 174)) {
    if (script_18BF8(8, 30, 22, 0)) {
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
      goto L01A351;
    }
  }
  L01A351:;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    local_EF = -1;
  } else if (script_18BF8(8, 30, 21, 0)) {
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
        goto L01A4FB;
      }
    }
  }
  L01A4FB:;
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

__attribute__((scn_string_bindings(0, data_2BCFA, 1, data_2BD04, 2, data_2BD0E, 4, data_2BD22, 5, data_2BD2C, 6, data_2BD36, 7, data_2BD40, 8, data_2BD4A, 9, data_2BD54, 10, data_2BD5E, 11, data_2BD68, 12, data_2BD72, 13, data_2BD7C, 15, data_2BD90, 16, data_2BD9A, 17, data_2BDA4, 18, data_2BDAE, 19, data_2BDB8, 20, data_2BDC2, 21, data_2BDCC, 22, data_2BDD6, 23, data_2BDE0, 25, data_2BDF4, 27, data_2BE08, 28, data_2BE12, 31, data_2BE30, 32, data_2BE3A, 33, data_2BE44, 38, data_2BE76, 39, data_2BE80, 40, data_2BE8A, 41, data_2BE94, 43, data_2BEA8, 44, data_2BEB2, 45, data_2BEBC, 56, data_2BF2A, 57, data_2BF34, 58, data_2BF3E, 59, data_2BF48, 60, data_2BF52, 61, data_2BF5C, 64, data_2BF7A, 65, data_2BF84, 66, data_2BF8E, 67, data_2BF98, 68, data_2BFA2, 69, data_2BFAC, 70, data_2BFB6, 71, data_2BFC0, 72, data_2BFCA, 74, data_2BFDE, 76, data_2BFF2, 78, data_2C006, 79, data_2C010, 80, data_2C01A, 81, data_2C024, 82, data_2C02E, 84, data_2C042, 86, data_2C056, 87, data_2C060, 88, data_2C06A, 89, data_2C074, 90, data_2C07E, 91, data_2C088, 92, data_2C092, 93, data_2C09C, 94, data_2C0A6, 96, data_2C0BA, 99, data_2C0D8, 100, data_2C0E2, 101, data_2C0EC, 102, data_2C0F6, 103, data_2C100, 104, data_2C10A, 106, data_2C11E, 107, data_2C128, 111, data_2C150, 113, data_2C164, 115, data_2C178, 116, data_2C182, 117, data_2C18C, 118, data_2C196, 119, data_2C1A0, 120, data_2C1AA, 125, data_2C1DC, 127, data_2C1F0, 128, data_2C1FA, 129, data_2C204, 132, data_2C222, 133, data_2C22C, 134, data_2C236, 135, data_2C240, 136, data_2C24A, 137, data_2C254, 138, data_2C25E, 139, data_2C268, 140, data_2C272, 141, data_2C27C, 142, data_2C286, 143, data_2C290, 144, data_2C29A, 150, data_2C2D6, 158, data_2C326, 159, data_2C330, 160, data_2C33A, 161, data_2C344, 163, data_2C358, 172, data_2C3B2, 173, data_2C3BC, 174, data_2C3C6, 175, data_2C3D0, 182, data_2C416, 185, data_2C434, 186, data_2C43E, 187, data_2C448, 198, data_2C4B6, 200, data_2C4CA, 201, data_2C4D4, 208, data_2C51A, 209, data_2C524, 216, data_2C56A, 224, data_2C5BA, 225, data_2C5C4, 226, data_2C5CE, 232, data_2C60A, 234, data_2C61E, 235, data_2C628, 237, data_2C63C, 239, data_2C650, 244, data_2C682, 245, data_2C68C, 256, data_2C6FA, 258, data_2C70E, 260, data_2C722, 261, data_2C72C, 265, data_2C754, 269, data_2C77C, 271, data_2C790, 273, data_2C7A4, 274, data_2C7AE, 275, data_2C7B8, 276, data_2C7C2, 277, data_2C7CC, 278, data_2C7D6, 280, data_2C7EA, 282, data_2C7FE, 285, data_2C81C, 288, data_2C83A, 289, data_2C844, 290, data_2C84E, 295, data_2C880, 297, data_2C894, 299, data_2C8A8, 300, data_2C8B2, 301, data_2C8BC, 302, data_2C8C6, 303, data_2C8D0, 304, data_2C8DA, 308, data_2C902, 311, data_2C920, 313, data_2C934, 315, data_2C948, 316, data_2C952, 317, data_2C95C, 318, data_2C966, 331, data_2C9E8)))
static void script_1A56D(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F4 = arg0;
  switch (local_F4) {
    case 1:
      task_320 = 90;
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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
      Script_scenCreateTask(script_20A20, 0);
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

__attribute__((scn_string_bindings(5, data_2CA24, 7, data_2CA38, 8, data_2CA42, 21, data_2CAC4, 22, data_2CACE, 30, data_2CB1E, 32, data_2CB32, 33, data_2CB3C, 48, data_2CBD2, 49, data_2CBDC)))
static void script_1E42B(void) {
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    task_320 = 250;
    Script_scenCreateTask(script_20A20, 0);
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
    Script_scenCreateTask(script_20A20, 0);
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

static int8_t script_1EEF3(void) {
  int8_t local_FF;
  local_FF = 0;
  task_320 = 55;
  Script_scenCreateTask(script_20A20, 0);
  local_FF = script_1EF1B();
  return local_FF;
}

static int8_t script_1EF1B(void) {
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
  script_1F036(local_F8);
  local_FC = script_1F188();
  script_1F1A3(local_FC);
  if (script_1F11F(local_F8, local_FC)) {
    if (local_F8 == 0) {
      local_F8 = 2;
    } else {
      local_F8 = 0;
    }
    task_320 = 56;
    Script_mapmSetCamFlags(0, 0);
    Script_charaVisUtils(CHARA_AKIR, 1);
    script_1F036(local_F8);
    local_FC = script_1F188();
    script_1F1A3(local_FC);
    Script_mapmSetCamFlags(0, 1);
    Script_charaVisUtils(CHARA_AKIR, 0);
    if (script_1F11F(local_F8, local_FC)) {
      local_F7 = script_1F579();
    } else {
      script_1F28C(local_FC);
    }
  } else {
    script_1F28C(local_FC);
  }
  return local_F7;
}

static void script_1F036(int32_t arg0) {
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

static int32_t script_1F11F(int32_t arg0, int32_t arg1) {
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

static int32_t script_1F188(void) {
  return SCNF_RequestActionSelectorFromParams_0008(7, 8, 6, 5, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
}

static void script_1F1A3(int32_t arg0) {
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

__attribute__((scn_string_bindings(1, data_2CC54, 2, data_2CC5E, 3, data_2CC68, 4, data_2CC72)))
static void script_1F28C(int32_t arg0) {
  int32_t local_FC;
  task_320 = 58;
  script_1F4B1(1);
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

static void script_1F47A(void) {
  task_324 = Script_ShowUIMessageFromID("E1032A051");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_1F4B1(int32_t arg0) {
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

__attribute__((scn_string_bindings(0, data_2CC9A, 1, data_2CCA4, 2, data_2CCAE, 3, data_2CCB8, 4, data_2CCC2, 5, data_2CCCC, 6, data_2CCD6, 7, data_2CCE0, 8, data_2CCEA, 10, data_2CCFE, 11, data_2CD08, 15, data_2CD30, 16, data_2CD3A, 17, data_2CD44, 18, data_2CD4E, 19, data_2CD58, 20, data_2CD62, 21, data_2CD6C, 22, data_2CD76, 23, data_2CD80, 24, data_2CD8A, 25, data_2CD94, 26, data_2CD9E, 27, data_2CDA8, 28, data_2CDB2, 29, data_2CDBC, 33, data_2CDE4, 34, data_2CDEE, 35, data_2CDF8, 36, data_2CE02, 37, data_2CE0C, 38, data_2CE16, 39, data_2CE20, 40, data_2CE2A, 41, data_2CE34, 42, data_2CE3E, 43, data_2CE48, 44, data_2CE52, 45, data_2CE5C, 46, data_2CE66, 47, data_2CE70, 48, data_2CE7A, 49, data_2CE84, 52, data_2CEA2, 53, data_2CEAC, 54, data_2CEB6, 59, data_2CEE8)))
static int8_t script_1F579(void) {
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
      script_1F4B1(0);
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
    script_1F4B1(1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  }
  return local_F7;
}

static void script_20256(void) {
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

static void script_20319(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  Vector3i local_C4;
  script_21711();
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 23.0f, 53.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  task_320 = 62;
  Script_scenCreateTask(script_20A20, 0);
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

static void script_20641(void) {
  Script_seqEnterEvtCam(6220, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
  script_20256();
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

static void script_206C6(void) {
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

static void script_20755(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_seqEnterEvtCam(6220, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_20256();
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
  local_E4 = Script_scenCreateTask(script_2096B, 0);
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

static int32_t script_2096B(void) {
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

static int32_t script_20A20(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_320;
    switch (local_FC) {
      case -1:
        goto L021394;
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
  L021394:;
  task_320 = 0;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_2CF1A, 1, data_2CF2A)))
static void script_213A2(void) {
  Vector4 local_F0;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  SCNF_WaitFrames_0000(1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "TBOX") == 0) {
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
    script_20641();
  } else {
    script_20755();
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_2CF2F)))
static void script_214A1(void) {
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_shenClearFreeState(0);
  if (task_32D < 0) {
    script_206C6();
  } else {
    script_20319();
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

static void script_21514(void) {
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
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2AE00, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 0, 0, 0, 1);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2AE00, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 1, 0, 0, 1);
  Script_scenCreateTask(script_2191E, 0);
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

static void script_216FD(void) {
  task_320 = -1;
  Script_TELMUtils(TELM_CMD_Cleanup, task_304);
  return;
}

static void script_21711(void) {
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

static void script_217D9(void) {
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
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2AEE4, 5);
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

static int32_t script_2191E(void) {
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

static void script_219A1(void) {
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

__attribute__((scn_string_bindings(0, data_2D0BC, 2, data_2D0D1, 4, data_2D0E6)))
static void script_219F7(void) {
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "MAPY") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "MAPN") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 5)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "MAPS") == 0) {
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
    script_219A1();
  }
  return;
}

static int32_t script_21BBB(void) {
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
          Script_scenCreateTask(script_21F0A, 0);
        } else {
          local_F1 = 0;
          Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Hide);
          if (-1) {
            Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Show);
              goto L021C8E;
            }
          }
          L021C8E:;
          Script_scenToggleVisibleFor(SHEN_NAME('TAXI'), 0);
          script_2219F();
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

static int8_t script_21E15(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static int32_t script_21ECE(void) {
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_2D0FB, 1, data_2D10B, 2, data_2D110, 3, data_2D115, 4, data_2D116, 5, data_2D11B, 6, data_2D11C)))
static int32_t script_21F0A(void) {
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
      script_24F33();
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
          goto L022002;
        }
      }
      L022002:;
      Script_scenToggleVisibleFor(SHEN_NAME('TAXI'), 0);
      Script_SetupLightFromTable(1);
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "JDYA") == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_AuthorUtils(0, 0, "JDYA", "");
      Script_AuthorUtils(0, 1, "JDYA", "");
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
      Script_scenCreateTask(script_21ECE, 0);
      script_FF30(4);
      Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_convSetCamInactiveFlag(1);
      Script_SetGameStateTo(AUTH_None);
      Script_lightResetSlotToDefaults(1);
      script_2219F();
      script_24F46();
      Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      script_24EF8();
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

static void script_2219F(void) {
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_SetCharaFlags(CHARA_AKIR, 0);
  return;
}

static void script_221BE(void) {
  Script_scenSetMapEvTbl((uint32_t *)&data_2D124, 4);
  return;
}

static void script_221CC(void) {
  return;
}

static void script_221CF(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    task_2DE = 3;
    Script_ryoSleepUtils(3, 1, 0, 3);
  }
  return;
}

static void script_221F2(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    task_2DE = 1;
    Script_ryoSleepUtils(3, 1, 0, 1);
  }
  return;
}

static void script_22215(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    task_2DE = 2;
    Script_ryoSleepUtils(3, 1, 0, 2);
  }
  return;
}

static void script_22238(void) {
  int32_t unused_FC;
  if (script_21E15(9, 0, 19, 0)) {
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

static int32_t script_2229F(void) {
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
        goto L022367;
      }
    }
    L022367:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_22373(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = arg3;
  if (local_FC == 1) {
    task_7C = 0;
    goto L02238E;
  }
  L02238E:;
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

static int32_t script_2240A(void) {
  int32_t local_FC;
  local_FC = 0;
  if ((&task_F8)->mode) {
    if (task_F8.entry_idx == 0) {
      local_FC = 2;
    }
  }
  return local_FC;
}

static int32_t script_22432(void) {
  int32_t local_FC;
  local_FC = 0;
  if (script_1100D()) {
    local_FC = 3;
  }
  return local_FC;
}

static int8_t script_2244C(void) {
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

static int32_t script_2253C(void) {
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

static int32_t script_22612(void) {
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

static int32_t script_22727(void) {
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
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) == 0) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 0) {
          if (Script_SetTaskDataValue(CHARA_AKIR, &task_288, &task_294, 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED) == 0)) {
            Script_ReturnTimeDate(&local_EC);
            if (script_21E15(19, 0, 23, 29) & ((-((&local_EC)->month == 12) & -((&local_EC)->day >= 26)) | -((&local_EC)->month < 5))) {
              local_F8 = 4;
            }
          }
        }
      }
    }
  }
  return local_F8;
}

static int8_t script_22866(void) {
  int8_t local_FF;
  local_FF = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_ASKED_FOR_MOTORCYCLE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    task_288.x = 11.1475f;
    task_288.y = 0.0f;
    task_288.z = 31.885401f;
    task_294.x = 6.5474f;
    task_294.y = 0.0f;
    task_294.z = 34.485298f;
    if (Script_SetTaskDataValue(CHARA_AKIR, &task_288, &task_294, 0)) {
      local_FF = 8;
    }
  }
  return local_FF;
}

static void script_228F5(int32_t arg0) {
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
    Script_shenRequestGameModuleTransition(3, SHEN_NAME('JABE'), 0);
    SCNF_WaitFrames_0000(1);
    goto L022972;
  }
  L022972:;
  return;
}

static int32_t script_22973(int32_t arg0, int32_t arg1) {
  if (arg0) {
    (void)arg0;
  } else {
    (void)arg1;
  }
  end:;
}

static int32_t script_22985(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  local_F8 = script_22973(arg0, arg1);
  switch (local_F8) {
    case 2:
      local_FC = script_2240A();
      break;
    case 6:
      local_FC = script_2244C();
      break;
    case 4:
      local_FC = script_22727();
      break;
    case 8:
      local_FC = script_22866();
      break;
  }
  if (Script_IsPlayerAllowed(0)) {
    if (local_FC == 0) {
      local_FC = script_22432();
    }
    if (local_FC == 0) {
      local_FC = script_2253C();
    }
    if (local_FC == 0) {
      local_FC = script_22612();
    }
  }
  return local_FC;
}

static void script_22A3C(void) {
  int32_t local_FC;
  local_FC = Script_UpdateTaskCollisionCache(0, 1, &task_2B8);
  if (local_FC != -1) {
    Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, local_FC);
  }
  return;
}

static int32_t script_22A62(void) {
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
    script_22A3C();
    if (Script_IsPlayerAllowed(0)) {
      uint8_t unused_FC[2];
      int32_t local_F8;
      local_F8 = task_2DE;
      switch (local_F8) {
        case 3:
          script_22373(3, SHEN_NAME('JU00'), 0, 0);
          break;
        case 1:
          script_22373(3, SHEN_NAME('D000'), 1, 0);
          break;
        case 2:
          script_22373(3, SHEN_NAME('D000'), 11, 0);
          break;
      }
    }
    if ((local_FE == 2) | Script_IsPlayerAllowed(0)) {
      local_FF = script_22985(local_FF, local_FE);
    } else {
      local_FF = 0;
    }
    local_FE = local_FE + 1;
    if (local_FE == 9) {
      local_FE = 2;
    }
    if ((local_FE == 3) | (local_FE == 7) | (local_FE == 5)) {
      local_FE = local_FE + 1;
      if (local_FE == 9) {
        local_FE = 2;
      }
    }
    {
      uint8_t unused_FC[2];
      int32_t local_F8;
      local_F8 = local_FF;
      switch (local_F8) {
        case 2:
          script_228F5(local_FF);
          break;
        case 6:
          script_148FF();
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
          script_13E2B(0);
          break;
        case 4:
          script_2313C(1);
          break;
        case 8:
          script_23E0B(2);
          break;
      }
    }
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      script_22238();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_22C97(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_25978, 8);
  Script_handSetFloatsFor(arg0, 0, data_25978, 8);
  return;
}

static void script_22CD2(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_22CFC(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F0[3];
  int32_t local_EC;
  Script_executeAicaCommand(19112, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_387C();
  script_22F42();
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

static void script_22DD9(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  if (Script_shenSeqUtils(-7) == 0) {
    script_387C();
    script_22F42();
    local_FC = 0;
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      local_EC = local_FC;
      switch (local_EC) {
        case 0:
          Script_FACE(CHARA_HRSK, 3, 0, 16);
          break;
        case 143:
          script_22CD2(CHARA_AKIR);
          script_22CD2(CHARA_HRSK);
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
          goto L022F31;
          break;
      }
      if ((local_FC >= 896) & (local_FC <= 1161)) {
        Script_EVT_GetPosition(CHARA_HRSK, 18, &local_F0, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 8);
      }
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
    }
    L022F31:;
    Script_FACE_LCG(CHARA_HRSK, FACE_BlinkAutomatic, 0, 0);
  }
  return;
}

static void script_22F42(void) {
  script_3894(CHARA_AKIR);
  script_3894(CHARA_HRSK);
  Script_SetMaxInterpSpeed(0.4f);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2D134);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_2D144);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_HRSK, 1, 0);
  script_22C97(CHARA_AKIR);
  script_22C97(CHARA_HRSK);
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

static void script_2304D(void) {
  script_230D6(CHARA_AKIR);
  Script_SetCharaFlags(CHARA_HRSK, 0);
  script_3921(CHARA_AKIR);
  script_3921(CHARA_HRSK);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  return;
}

static void script_2309B(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_25978, 8);
  Script_handSetFloatsFor(arg0, 0, data_25978, 8);
  return;
}

static void script_230D6(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static int32_t script_23100(void) {
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_2D154, 1, data_2D164, 2, data_2D169, 3, data_2D16E, 4, data_2D16F, 5, data_2D174, 7, data_2D17C, 8, data_2D181, 9, data_2D186, 10, data_2D18B)))
static void script_2313C(int32_t arg0) {
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
  script_24F33();
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_srchControl((void *)task_2D0);
  Script_srchControl((void *)task_328);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('YAMN'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('YAMN'), CDM_Show);
      goto L023213;
    }
  }
  L023213:;
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
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "DNOZ") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "DNOZ", "");
  Script_AuthorUtils(0, 1, "DNOZ", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_23477();
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
  Script_scenCreateTask(script_23100, 0);
  script_22CFC(0);
  script_22DD9(1);
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
  Script_shenRequestGameModuleTransition(3, SHEN_NAME('JOMO'), 3);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_2D210, 1, data_2D217)))
static void script_23477(void) {
  Vector3 local_F4;
  Script_PREP_Related(-1, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(AndNotInstructionFlags, 0xC);
  Script_SetVector4ByOffset(&data_2D1F0);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_2D200);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_SetFogColour(&data_2D1A8);
  Script_SetFogFlags(0, data_2D1B8);
  Script_shenSetLightingMode(2);
  Script_lightSetGlobalSceneLighting(&data_2D1C8);
  local_F4.x = 20.61f;
  (&local_F4)->y = 3.39f;
  (&local_F4)->z = -4.08f;
  Script_lightEnablePointLightAt(1, &local_F4);
  Script_LightUtils(1, 2, &data_2D1D8);
  Script_LightUtils(1, 4, 1.0f);
  Script_LightUtils(1, 5, 3.69f);
  Script_LightUtils(1, 6, data_2D1E8);
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

static void script_235D5(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_23986();
  local_FC = Script_scenCreateTask(script_23645, 0);
  local_F8 = Script_scenCreateTask(script_23D2A, 2, SHEN_NAME('KEY_'), 2);
  local_F4 = Script_scenCreateTask(script_23BC7, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_23A34();
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  Script_srchControl((void *)local_F4);
  return;
}

static int32_t script_23645(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  Script_charaVisUtils(CHARA_AKIR, 1);
  Script_charaVisUtils(CHARA_ITOH, 1);
  Script_CreateNPCImageModule(CHARA_AKIR, 0, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKB_M", 0);
  local_D8 = 0;
  local_E8.x = 7.56368f;
  (&local_E8)->y = 1.09045f;
  (&local_E8)->z = 33.4426f;
  Script_EVT_SetPosition(SHEN_NAME('KEY_'), POS_XYZ, &local_E8);
  local_DC.x = -728;
  (&local_DC)->y = -1638;
  (&local_DC)->z = 0;
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_DC);
  for (;;) {
    if (local_D8 == 1) {
      local_F4.x = 0.1996f;
      (&local_F4)->y = 0.1647f;
      (&local_F4)->z = 0.472100019f;
      local_DC.x = 0;
      (&local_DC)->y = 0;
      (&local_DC)->z = 0;
      Script_fixoAttachEx(SHEN_NAME('DMMY'), SHEN_NAME('BIKE'), 0, &local_F4, &local_DC);
      local_F4.x = 0.1896f;
      (&local_F4)->y = 0.3047f;
      (&local_F4)->z = 0.8963f;
      local_DC.x = 0;
      (&local_DC)->y = 0;
      (&local_DC)->z = 0;
      Script_fixoAttachEx(SHEN_NAME('DMM2'), SHEN_NAME('BIKE'), 0, &local_F4, &local_DC);
    }
    if (local_D8 == 766) {
      Script_FACE(CHARA_AKIR, 1, 0, 10);
    }
    if (local_D8 == 840) {
      Script_FACE(CHARA_AKIR, 0, 0, 10);
    }
    if (local_D8 == 980) {
      Script_charaDisplay(SHEN_NAME('KEY_'), CDM_Hide);
    }
    if (local_D8 == 1210) {
      local_DC.x = -182;
      (&local_DC)->y = 0;
      (&local_DC)->z = -1092;
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_DC);
    }
    if (local_D8 == 1211) {
      Script_CreateNPCImageModule(CHARA_AKIR, "YKG_M", 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_25894, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_25894, 0);
    }
    if (local_D8 == 1265) {
      local_F4.x = 0.023f;
      (&local_F4)->y = 0.002f;
      (&local_F4)->z = 0.145f;
      local_DC.x = 17019;
      (&local_DC)->y = 94;
      (&local_DC)->z = 48610;
      Script_fixoAttachEx(SHEN_NAME('HELM'), CHARA_AKIR, 5, &local_F4, &local_DC);
    }
    if (local_D8 == 1365) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_2D224, 10);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_2D308, 10);
      local_DC.x = 1092;
      (&local_DC)->y = 0;
      (&local_DC)->z = -364;
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_DC);
      local_DC.x = -546;
      (&local_DC)->y = 4369;
      (&local_DC)->z = 0;
      Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_DC);
    }
    if (local_D8 == 1400) {
      Script_mapmSetRenderFlag(9, 1);
    }
    if (local_D8 == 1580) {
      Script_executeAicaCommand(2720, 2, 60);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_D8 = local_D8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_23986(void) {
  script_23A84(CHARA_AKIR);
  script_23B20(CHARA_ITOH);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_ITOH, -1);
  Script_ccowSetHeightMode(SHEN_NAME('BIKE'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('SUJI'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('HIKA'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('HELM'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('TAIL'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('KEY_'), CCOW_KeepHeight);
  Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('HELM'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('TAIL'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KEY_'), CDM_Show);
  Script_SetVector4ByOffset(&data_2D3F8);
  (void)1;
  return;
}

static void script_23A34(void) {
  script_23B81(CHARA_AKIR);
  script_23B5E(CHARA_ITOH);
  Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HELM'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TAIL'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('KEY_'), CDM_Hide);
  Script_fixoClearAttachment(SHEN_NAME('HELM'));
  return;
}

static void script_23A84(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_256CC, 10);
  Script_handSetFloatsFor(arg0, 1, data_256CC, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_2D408);
  return;
}

static void script_23B20(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_2D418);
  return;
}

static void script_23B5E(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  return;
}

static void script_23B81(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static int32_t script_23BC7(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  float local_D8;
  float local_D4;
  int32_t unused_D0;
  Vector3i local_C4;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  for (;;) {
    local_F4.x = (&local_E8)->x;
    (&local_F4)->y = (&local_E8)->y;
    (&local_F4)->z = (&local_E8)->z;
    Script_EVT_GetPosition(SHEN_NAME('BIKE'), -1, &local_E8, 0);
    local_DC[0] = (&local_E8)->x - (&local_F4)->x;
    local_DC[1] = (&local_E8)->y - (&local_F4)->y;
    local_DC[2] = (&local_E8)->z - (&local_F4)->z;
    local_DC[0] = *local_DC * *local_DC;
    local_DC[1] = local_DC[1] * local_DC[1];
    local_DC[2] = local_DC[2] * local_DC[2];
    local_D8 = *local_DC * *local_DC + local_DC[1] * local_DC[1] + local_DC[2] * local_DC[2];
    local_D4 = local_D8 * 1000000.0f;
    if (!(local_D4 <= 300.0f)) {
      local_D4 = 300.0f;
    }
    if (!(local_D4 >= 0.5f)) {
      local_D4 = 0.0f;
    }
    local_C4.x = -(int32_t)(local_D4 / 360.0f * (float)0x10000);
    (&local_C4)->y = 0;
    (&local_C4)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('BIKE'), 23, 0, &local_C4, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('BIKE'), 18, 0, &local_C4, CHARA_PART_Add);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_2D430)))
static int32_t script_23D2A(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_charaDisplay(SHEN_NAME('KEY_'), CDM_Hide);
  local_E0 = 0;
  local_E4 = arg1;
  SCNF_WaitFrames_0000(856);
  Script_charaDisplay(arg0, CDM_Show);
  while (local_E4 < 10000) {
    while (local_E0 < 2) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "BIKE");
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_InterpolateAndScaleAngles(local_E0, local_E4, &local_F4, &local_E8, "DAT0");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(SHEN_NAME('KEY_'), Script_ScenDataUtil(&data_2D428, SCND_ReadU32, local_E0), &local_F4, &local_E8, CHARA_PART_Set);
      local_E0 = local_E0 + 1;
    }
    local_E0 = 0;
    local_E4 = local_E4 + 2;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_2D45C, 1, data_2D463, 2, data_2D470, 3, data_2D475, 4, data_2D47A, 5, data_2D48A, 6, data_2D48F, 7, data_2D494, 9, data_2D49C, 10, data_2D4A1, 11, data_2D4A6, 12, data_2D4AB)))
static void script_23E0B(int32_t arg0) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t unused_F4;
  GameDateTime local_EC;
  int32_t local_E8;
  if (arg0 == 2) {
    Script_ryoControl(PCCTL_RequestNewMode, 1);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(10, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    {
      GameDateTime local_E0;
      int8_t local_DF;
      Script_ReturnTimeDate(&local_E0);
      local_DF = ((&local_E0)->hours * 60 + (&local_E0)->minutes) / 6;
      Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 26, local_DF);
    }
    {
      GameDateTime local_E0;
      int8_t local_DF;
      int8_t local_DE;
      Script_ReturnTimeDate(&local_E0);
      if ((&local_E0)->month == 12) {
        local_DE = 0;
      } else {
        local_DE = (&local_E0)->month;
      }
      local_DF = (local_DE << 5) | (&local_E0)->day;
      Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 18, local_DF);
    }
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 27, 0);
    Script_scenToggleVisibleFor(SHEN_NAME('ITMN'), 1);
    script_219A1();
  }
  Script_mapmSetRenderFlag(9, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(AndNotInstructionFlags, 0xC);
  Script_SetFogColour(&data_2D43C);
  Script_SetFogFlags(0, data_2D44C);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL27.SPR");
  Script_lightResetSlotToDefaults(0);
  Script_SetupLightFromTable(2);
  if (arg0) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "IBIK")) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "IBIK");
    }
  }
  script_24F33();
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_SetShouldSleepFlag(0);
  Script_srchControl((void *)task_2D0);
  Script_srchControl((void *)task_328);
  Script_scenToggleVisibleFor(SHEN_NAME('VM_0'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('VMG0'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCH1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCH2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GBX1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GBX2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TBOX'), 0);
  Script_charaDisplay(SHEN_NAME('TBOX'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('BTEL'), 0);
  Script_charaDisplay(SHEN_NAME('BTEL'), CDM_Hide);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x484F5449, 1);
  local_F8 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x484F5449);
  while (local_F8 == 0) {
    SCNF_WaitFrames_0000(1);
    local_F8 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x484F5449);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_F8) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMapControlTaskFlag(SHEN_NAME('BIKE'), 1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "BIKE") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "BIKE", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_mapEvUtils(0x4D450000, CHARA_AKIR, 0, 2, 0);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  Script_scenToggleActiveFor(CHARA_ITOH, 0);
  Script_motmGetCharaFlags(CHARA_ITOH, MOTM_PoseNone, SCEN_Write);
  Script_LoadVoiceFile(1, "01BIKE");
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 1, "FREE", 0);
  Script_LoadSND(0, 2, "bgm200.snd", 0);
  Script_LoadSND(0, 4, "a1_bikek.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 102);
  script_23986();
  local_E8 = Script_scenCreateTask(script_23645, 0);
  {
    int32_t local_E4;
    int32_t local_E0;
    local_E4 = Script_scenCreateTask(script_23D2A, 2, SHEN_NAME('KEY_'), 2);
    local_E0 = Script_scenCreateTask(script_23BC7, 0);
    Script_shenSeqUtils(0);
    if (arg0 == 0) {
      Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
    }
    Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    script_23A34();
    Script_srchControl((void *)local_E8);
    Script_srchControl((void *)local_E0);
    Script_srchControl((void *)local_E4);
  }
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_ReturnTimeDate(&local_EC);
  if ((&local_EC)->hours > 3) {
    Script_IncrementDateTimeByNum(1);
    Script_ReturnTimeDate(&local_EC);
  }
  (&local_EC)->hours = 3;
  (&local_EC)->minutes = 0;
  Script_SetNewDateTime(&local_EC);
  Script_shenRequestGameModuleTransition(3, SHEN_NAME('FBGG'), 1);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(1, data_2D4D3, 3, data_2D4E8, 4, data_2D4F8)))
void script_242A7(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MOBJ.BIN");
  local_F8 = Script_ryoSleepUtils(0);
  script_24F52();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 65.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 17.0f);
  Script_SetMapFlags(0, 1);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 19);
  script_221BE();
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    Script_SetShouldSleepFlag(0);
  }
  local_FC = Script_GetCurrentEntryPoint();
  SCNF_WaitFrames_0000(1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "JD00") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_219F7();
  script_10B19();
  if (local_FC == 20) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE, 1);
    SCNF_WaitFrames_0000(1);
  }
  script_DD78(CHARA_AKIR, (DSWKDoorEntry *)data_27EB8, (int32_t *)data_2849C, (DoorResourceDescriptor *)&data_28670, (MapObjectRecord *)data_28760, 10, 0);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &task_2B8, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_AKIR, &task_2C4, 0x42000000);
  SCNF_WaitFrames_0000(1);
  task_2D8 = Script_scenCreateTask(script_21BBB, 0);
  script_24B49(CHARA_AKIR);
  task_2D0 = Script_scenCreateTask(script_0030, 6, CHARA_AKIR, 2, 0, &data_2CF34, 2, 0);
  script_2643((int32_t)data_2D088);
  Script_scenCreateTask(script_2464A, 0);
  Script_scenCreateTask(script_22A62, 0);
  Script_scenCreateTask(script_13CA5, 0);
  Script_scenCreateTask(script_2229F, 0);
  script_14C60(0x4C455442, 9997, -0x3F5E84B6, 0, 0x4192C189, 0);
  script_25154();
  Script_MotStepUtil(CHARA_AKIR, 269);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_2466F();
  script_E078();
  if (script_21E15(9, 0, 19, 0)) {
    script_E175(0);
  }
  script_249CD();
  local_F4 = local_FC;
  switch (local_F4) {
    case 30:
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      script_2313C(0);
      break;
    case 40:
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      script_23E0B(0);
      break;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JD00/", "IBIK") == 0) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
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

static int32_t script_2464A(void) {
  script_FECF();
  for (;;) {
    if (script_FF21() != -1) {
      script_FEF9();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2466F(void) {
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

static void script_249CD(void) {
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/03/D000", (int32_t)"SCRTEST.BIN", 0x3030444A);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  SCNF_WaitFrames_0000(1);
  script_24DFA();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_24A05(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_2D588, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_2D550, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_2D518, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_2D534, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_2D56C, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_24B3D(int32_t arg0) {
  script_24B55(arg0);
  return;
}

static void script_24B49(int32_t arg0) {
  script_24B55(arg0);
  return;
}

static void script_24B55(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L024B8E;
    }
  }
  L024B8E:;
  Script_ryoSetChara(arg0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_24A05();
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

static void script_24C61(int32_t arg0) {
  script_24B49(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_24C78(int32_t arg0) {
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

static void script_24CB9(int32_t arg0) {
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

static void script_24CFD(int32_t arg0) {
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

static void script_24D7B(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_24CFD(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_24CFD(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_24CFD(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_24CFD(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_24CFD(5);
  } else {
    script_24CFD(0);
  }
  return;
}

static void script_24DFA(void) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    if (local_FF == 1) {
      break;
    }
    *(&task_3B0 + local_FF) = 0;
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

static void script_24E65(int32_t arg0, int32_t arg1) {
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
  *(&task_3B0 + arg1) = local_FC;
  return;
}

static void script_24ECC(int32_t arg0, int32_t arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, *(&task_3B0 + arg1));
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  *(&task_3B0 + arg1) = 0;
  return;
}

static void script_24EF8(void) {
  int32_t unused_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  SCNF_WaitFrames_0000(1);
  if (*&task_3B0) {
    script_24ECC(0x414D4159, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_24F33(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  return;
}

static void script_24F46(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_24F52(void) {
  Script_ambSoundUtils(1, 0, &data_2D5C0, 12.0f, 30.0f, 0.9f, 2, &data_2D644);
  Script_ambSoundUtils(2, 0, &data_2D5CC, 10.0f, 30.0f, 0.9f, 2, &data_2D6CC);
  Script_ambSoundUtils(3, 0, &data_2D5D8, 5.0f, 20.0f, 0.9f, 2, &data_2D754);
  Script_ambSoundUtils(4, 0, &data_2D5E4, 10.0f, 25.0f, 0.9f, 2, &data_2D7DC);
  Script_ambSoundUtils(5, 0, &data_2D5F0, 22.0f, 30.0f, 0.9f, 2, &data_2D864);
  Script_ambSoundUtils(6, 0, &data_2D5FC, 5.0f, 20.0f, 0.9f, 2, &data_2D8EC);
  Script_ambSoundUtils(7, 0, &data_2D608, 20.0f, 30.0f, 0.9f, 2, &data_2D974);
  Script_ambSoundUtils(8, 1, data_2D614, 50.0f, 30.0f, 1.0f, 0, data_2D9FC);
  Script_ambSoundUtils(9, 1, data_2D620, 50.0f, 30.0f, 1.0f, 0, data_2DA84);
  Script_ambSoundUtils(10, 1, data_2D62C, 50.0f, 30.0f, 1.0f, 0, &data_2DB0C);
  Script_ambSoundUtils(15, 1, data_2D638, 200.0f, 50.0f, 1.0f, 3, &data_2DB94);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

__attribute__((scn_string_bindings(0, data_2DC1C, 1, data_2DC21, 2, data_2DC26, 3, data_2DC2B)))
static void script_250EB(void) {
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_LoadSND(0, 4, "f1jutadn.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  return;
}

static void script_25154(void) {
  int32_t local_FC;
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_2DC40, 21);
  local_FC = 0;
  while (local_FC < 21) {
    Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_FC, 1);
    local_FC = local_FC + 1;
  }
  return;
}
