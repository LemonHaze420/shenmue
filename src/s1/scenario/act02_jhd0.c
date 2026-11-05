#include "act02_jhd0.h"

__attribute__((scn_string_bindings(0, data_20CF8)))
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
    while (*(&task_C + arg5) & 4) {
      SCNF_WaitFrames_0000(1);
    }
    arg2 = Script_KaidanUtils(CAM_STAIR_QUERY_GUIDE_ENTRY, arg0, task_0, (const void *)task_4, (const void *)task_8, task_20);
    if (arg2 >= 0) {
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_2);
      Script_ccowUtils(arg0, CCOW_DisableHeightAdjustment);
      if (*(&task_C + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      }
      (void)((local_CA = 0) & 1);
      local_E4 = arg2 * 6;
      *(int32_t *)&local_F4[1] = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, local_E4 + 1);
      *(int32_t *)&local_F4[0] = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, local_E4);
      *(int32_t *)&local_F4[2] = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, local_E4 + 2);
      local_DC = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, local_E4 + 4);
      local_E8[1] = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, local_E4 + 3);
      local_E0 = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, local_E4 + 5);
      local_D8 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1]);
      local_D4 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_E8[1]);
      script_20A3(arg0, arg5, arg2);
      *(&task_18 + arg5) = arg2 + 1;
      *((int8_t *)&task_44 + arg5) = 3;
      script_2D34(arg0, arg5, local_DC);
      Script_CharHandlers(arg0, 2, 0, 16);
      (void)((local_CD = -((local_CB = 0) != 0)) & 1);
      if (local_DC & 0xF00 & 0x400) {
        script_0A25(1, arg2);
        if (local_DC & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      if (local_DC & 0x100) {
        (void)((local_CA = -1) & 1);
      }
      if (*(&task_C + arg5) & 0x400) {
        Script_MotStepUtil(arg0, 0);
      }
      if ((local_DC & 0xF) <= 4) {
        Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
        if ((*(&task_C + arg5) & 8) == 0) {
          if (local_DC & 0x1000000) {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          } else {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          }
        }
        if (local_DC & 0x2000) {
          task_49 = 0;
        } else {
          uint8_t unused_C8[2];
          int32_t local_C4;
          local_C4 = *(&task_10 + arg5);
          switch (local_C4) {
            case 0:
              task_49 = 0;
              break;
            case 1:
              task_49 = 1;
              break;
            case 3:
              task_49 = 1;
              break;
            case 2:
              if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                task_49 = 1;
              } else {
                task_49 = 0;
              }
              break;
          }
        }
        if (task_49) {
          local_F4[0] = *local_F4 + local_D8 * -0.1f;
          local_F4[2] = local_F4[2] + local_D4 * -0.1f;
        } else {
          local_F4[0] = *local_F4 + local_D8 * 0.16f;
          local_F4[2] = local_F4[2] + local_D4 * 0.16f;
        }
        local_CF = script_1B07(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
        if (local_CA & 4) {
          Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_4);
        }
        (void)((local_CC = -1) & 1);
        Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
        Script_ccowUnknown(arg0, 1, 3.0f);
      } else {
        if ((local_DC & 0xF) <= 12) {
          if ((*(&task_C + arg5) & 8) == 0) {
            Script_scenCreateTask(script_24C5, 2, arg0, 0);
          }
          if (local_DC & 0x2000) {
            task_49 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_10 + arg5);
            switch (local_C4) {
              case 0:
                task_49 = 0;
                break;
              case 1:
                task_49 = 1;
                break;
              case 3:
                task_49 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_49 = 1;
                } else {
                  task_49 = 0;
                }
                break;
            }
          }
          if (task_49) {
            local_F4[0] = *local_F4 + local_D8 * -0.25f;
            local_F4[2] = local_F4[2] + local_D4 * -0.25f;
          } else {
            local_F4[0] = *local_F4 + local_D8 * 0.15f;
            local_F4[2] = local_F4[2] + local_D4 * 0.15f;
          }
          local_CF = local_E0 - script_1D96(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
          if (local_CA & 4) {
            Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_4);
          }
          Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
          Script_ccowUnknown(arg0, 1, 3.0f);
          (void)((local_CC = 0) & 0x80);
          goto L000525;
        }
      }
      L000525:;
      if ((local_E0 == 1) & ((local_DC & 0xF) <= 4)) {
        if (*((int8_t *)&task_47 + arg5) == 1) {
          Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
        }
        if (task_49) {
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
          local_CE = task_49;
          if (local_D0) {
            local_D0 = 0;
          } else if (local_DC & 0x2000) {
            task_49 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_10 + arg5);
            switch (local_C4) {
              case 0:
                task_49 = 0;
                break;
              case 1:
                task_49 = 1;
                break;
              case 3:
                task_49 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_49 = 1;
                } else {
                  task_49 = 0;
                }
                break;
            }
          }
          if (local_CB & 8) {
          } else {
            if (*(&task_C + arg5) & 0x100) {
              if (*(&task_C + arg5) & 2) {
                if ((Script_GetControllerInput(0) & 0x2000) > 0) {
                  (void)((local_CD = -1) & 4);
                }
              }
            }
            if (local_CC & 0x10) {
              local_CF = local_CF + script_0E0C(arg0, local_CF, local_E0, arg5, local_DC, arg2, local_CE - task_49);
            } else {
              local_CF = local_CF - script_12FC(arg0, local_CF, arg5, local_DC, arg2, local_CE - task_49);
            }
            if ((local_CF > 0) & (local_CF < local_E0)) {
              if (!(local_CD & 0x20)) goto L000767;
            }
          }
          L000767:;
        }
        if (local_D0) {
          if (((*(&task_C + arg5) & 0x10) == 0) & ((local_DC & 0xF) < 2)) {
            if (*((int8_t *)&task_47 + arg5) == 1) {
              Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
            }
            if (task_49) {
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
      if ((task_4A == 1) | (task_4A == 2)) {
        task_4A = 3;
      }
      if ((*(&task_C + arg5) & 2) > 0) {
        if (task_49) {
          Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
        } else {
          Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
        }
        if (task_50 & 4) {
          Script_sprintf("Kaidan : ");
          Script_ryoControl(RYO_RestoreControl);
          (void)((task_50 = 0) & 4);
        }
        script_2E09(arg0, arg5, local_DC);
      }
      Script_ccowUnknown(arg0, 2, 0.07f);
      Script_ccowUtils(arg0, CCOW_EnableHeightAdjustment);
      if (*((int8_t *)&task_45 + arg5) != 2) {
        *((int8_t *)&task_45 + arg5) = 0;
      }
      *((int8_t *)&task_46 + arg5) = 0;
      *((int8_t *)&task_44 + arg5) = 0;
      *(&task_18 + arg5) = -1;
      if (*(&task_C + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      }
    } else {
      (void)((task_50 = 0) & 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0932(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  *(&task_C + arg5) = arg4;
  *(&task_14 + arg5) = arg0;
  *(&task_18 + arg5) = -1;
  *(&task_10 + arg5) = arg1;
  *((int8_t *)&task_45 + arg5) = 0;
  *((int8_t *)&task_46 + arg5) = 0;
  Script_sprintf("========== initializing kaidan\012");
  if (arg3) {
    (void)((task_4B = 0) & 1);
    task_20 = 0x4000;
    task_48 = 1;
    task_4 = arg3;
    (void)((task_50 = 0) & 1);
    if (arg2) {
      Script_sprintf("please ENDOFKDATA use can't start shuki kaidan\012");
      task_0 = arg2;
    } else {
      Script_CycleManagerUtils(CYCLE_CMD_SET_GLOBAL_CYCLE_PTR, task_4, &task_18);
      task_0 = local_F8 = 0;
      while ((local_FC = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, local_F8)) != 0x47C35000) {
        task_0 = task_0 + 1;
        local_F8 = local_F8 + 6;
      }
    }
  } else {
    task_48 = task_48 + 1;
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
  while ((local_F8 = Script_ScenDataUtil((void *)task_8, SCND_ReadU32, local_FC)) != 3) {
    local_F0 = local_F8;
    if (local_F0 == 0) {
      local_FC = local_FC + 3;
    } else if (local_F0 == 1) {
      if (arg0) {
        if (Script_ScenDataUtil((void *)task_8, SCND_ReadU32, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_8, SCND_ReadU32, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_24[local_F4];
            break;
          } else {
            Script_PreserveAccumulator_012C(0, "Kaidan Function Error 1\012");
          }
        }
      }
      local_FC = local_FC + 3;
    } else if (local_F0 == 2) {
      if (arg0 == 0) {
        if (Script_ScenDataUtil((void *)task_8, SCND_ReadU32, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_8, SCND_ReadU32, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_24[local_F4];
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
    task_49 = 0;
  } else {
    local_F8 = *(&task_10 + arg1);
    switch (local_F8) {
      case 0:
        task_49 = 0;
        break;
      case 1:
        task_49 = 1;
        break;
      case 3:
        task_49 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_49 = 1;
        } else {
          task_49 = 0;
        }
        break;
    }
  }
  if (task_49) {
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
    task_49 = 0;
  } else {
    local_F8 = *(&task_10 + arg1);
    switch (local_F8) {
      case 0:
        task_49 = 0;
        break;
      case 1:
        task_49 = 1;
        break;
      case 3:
        task_49 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_49 = 1;
        } else {
          task_49 = 0;
        }
        break;
    }
  }
  if (task_49) {
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
  task_34 = 0;
  if (task_49) {
    local_FF = 4;
    if (arg1 < arg2 - 3) {
      Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
      if (arg6) {
        if (*((int8_t *)&task_47 + arg3) == 1) {
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
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg4 & 0xF00 & 0x800) {
        script_0A25(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else if (arg2 <= arg1 + 2) {
        if (*((int8_t *)&task_47 + arg3) == 0) {
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
          if (*((int8_t *)&task_47 + arg3) == 1) {
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
        if (*((int8_t *)&task_47 + arg3) == 0) {
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
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 < arg2 - 1) {
      if (arg6) {
        if (*((int8_t *)&task_47 + arg3) == 1) {
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
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg6) {
        if (*((int8_t *)&task_47 + arg3) == 1) {
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
          (void)((task_4C = -1) & 2);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg2 == arg1) {
          if (*((int8_t *)&task_47 + arg3) == 0) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        } else {
          if (*((int8_t *)&task_47 + arg3) == 1) {
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
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 3;
      }
    }
  }
  return local_FF;
}

static int8_t script_12FC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  if (task_49) {
    local_FF = 4;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&task_47 + arg2) == 1) {
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
      if (*((int8_t *)&task_45 + arg2) != 2) {
        *((int8_t *)&task_45 + arg2) = 0;
      }
      switch (arg1) {
        case 1:
          if (arg5) {
            if (*((int8_t *)&task_47 + arg2) == 1) {
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
            if (*((int8_t *)&task_47 + arg2) == 1) {
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
            if (*((int8_t *)&task_47 + arg2) == 1) {
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
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 2;
      }
      script_2DB9(arg0, arg2, arg3);
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg1 <= 2) {
          if (*((int8_t *)&task_47 + arg2) == 1) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        } else {
          if (*((int8_t *)&task_47 + arg2) == 0) {
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
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&task_47 + arg2) == 1) {
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
      if (*((int8_t *)&task_45 + arg2) != 2) {
        *((int8_t *)&task_45 + arg2) = 0;
      }
      if (arg1) {
        if (arg5) {
          if (*((int8_t *)&task_47 + arg2) == 1) {
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
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 2;
      }
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
        script_2DB9(arg0, arg2, arg3);
      } else {
        if (arg1) {
          if (*((int8_t *)&task_47 + arg2) == 1) {
            Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
          }
        } else {
          if (*((int8_t *)&task_47 + arg2) == 0) {
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
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 3;
      }
    }
  }
  return local_FF;
}

static void script_18C8(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_E8[3];
  if (arg5 & 0x2000) {
    task_49 = 0;
  } else {
    int32_t unused_D4[5];
    int32_t local_D0;
    local_D0 = *(&task_10 + arg4);
    switch (local_D0) {
      case 0:
        task_49 = 0;
        break;
      case 1:
        task_49 = 1;
        break;
      case 3:
        task_49 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_49 = 1;
        } else {
          task_49 = 0;
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
  if (task_49) {
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
  task_34 = 0;
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
  if (task_49) {
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
      task_34 = -0x80000000;
      *((int8_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2CB, 0, -1.0f, 2);
      local_B8 = 2;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2CB, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_34 = -0x80000000;
      *((int8_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C6, 0, -1.0f, 2);
      local_B8 = 1;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C6, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(0, data_20D97)))
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
  task_34 = 0;
  if (task_49) {
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
      task_34 = -0x80000000;
      *((int8_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_34 = -0x80000000;
      *((int8_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(arg0, CCOW_EnableAll);
  script_2D34(arg0, arg4, arg5);
  if (*(&task_C + arg4) & 2) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_50 = -1) & 4);
    }
    Script_ccowUtils(arg0, CCOW_DisableCharacters);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(1, data_20DBD)))
static void script_20A3(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  (void)((local_F7 = 0) & 4);
  arg2 = arg2 & -2;
  local_F8 = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, arg2 * 6 + 4);
  local_FC = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, (arg2 + 1) * 6 + 4);
  local_FC = local_FC | local_F8;
  if (local_FC & -0x80000000) {
    while (local_FC & -0x80000000) {
      if (local_F7 & 0x80) {
        if (task_54) {
        } else {
          Script_sprintf("wait ... shuki shuki shuki\012");
        }
      } else {
        if (*(&task_C + arg1) & 2) {
          if (Script_ryoControl(PCCTL_QueryShouldRun)) {
            Script_sprintf("Kaidan : ");
            Script_ryoControl(PCCTL_DisablePlayerControl);
            (void)((task_50 = -1) & 4);
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
      task_54 = (task_54 + 1) & 0xF;
      local_F8 = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, arg2 * 6 + 4);
      local_FC = Script_ScenDataUtil((void *)task_4, SCND_ReadU32, (arg2 + 1) * 6 + 4);
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
  if (~task_4B) {
    (void)((task_4B = -1) & 1);
    task_38 = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
    task_3C = Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, -1.0f);
    task_40 = Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, -1.0f);
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
    local_F4 = task_4A;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L0023A3;
    }
    L0023A3:;
    local_F8 = task_40 + (arg0 - task_40) * (float)local_FC / 10.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_4A;
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
    local_F4 = task_4A;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L002432;
    }
    L002432:;
    local_F8 = task_38 + (arg0 - task_38) * (float)local_FC / 50.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_4A;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, arg0);
    }
  }
  return;
}

static void script_248E(void) {
  Script_SetCameraSettings(6, 7);
  Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, task_38);
  Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, task_3C);
  Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, task_40);
  Script_KaidanUtils(CAM_STAIR_RESET_RETRACTION_LIMIT);
  (void)((task_4B = 0) & 2);
  return;
}

static int32_t script_24C5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  int8_t local_E6;
  task_1C = arg0;
  task_4A = 1;
  local_E7 = -1;
  (void)((local_E6 = 0) & 2);
  Script_SetGameStateTo(AUTH_None);
  while (task_1C == arg0) {
    if (Script_GetAuthorState_And_SetCamPos(&local_F4, &local_E8) != 17) {
      if (task_4A != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_4A;
        if (local_E0 == 1) {
          (void)((local_E6 = -1) & 2);
          script_22D1(arg1);
        } else if (local_E0 == 2) {
          script_248E();
        } else if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_1C = 0;
        } else {
          if (local_E0 == 4) goto L002572;
        }
        L002572:;
        local_E7 = task_4A;
      }
    } else {
      if (task_4A != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_4A;
        if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_1C = 0;
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
  if (task_4A != 0) {
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
  while (local_FF < task_48) {
    if (arg0 == *(&task_14 + local_FF)) {
      local_F8 = *(&task_18 + local_FF);
    }
    local_FF = local_FF + 1;
  }
  (void)local_F8;
  return;
}

static void script_2643(int32_t arg0) {
  task_8 = arg0;
  return;
}

static void script_264D(int32_t arg0) {
  task_20 = arg0;
  return;
}

static void script_2657(void) {
  (void)task_20;
  return;
}

__attribute__((scn_string_bindings(0, data_20DC7)))
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

__attribute__((scn_string_bindings(0, data_20DDF)))
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

__attribute__((scn_string_bindings(0, data_20DF7)))
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

__attribute__((scn_string_bindings(0, data_20E0F)))
static void script_28EF(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&task_46 + arg1) == 0) {
    *((int8_t *)&task_46 + arg1) = 1;
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

__attribute__((scn_string_bindings(0, data_20E27)))
static void script_29E5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&task_46 + arg1) == 0) {
    *((int8_t *)&task_46 + arg1) = 1;
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

__attribute__((scn_string_bindings(0, data_20E3F)))
static int32_t script_2ADB(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&task_45 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&task_45 + arg1) = 1;
    if (task_49) {
      local_FC = local_F8 = -0.001f;
    } else {
      local_FC = local_F8 = 0.0f;
    }
    while (*((int8_t *)&task_45 + arg1) == 1) {
      if (task_49) {
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
    if (*((int8_t *)&task_45 + arg1) != 2) {
      for (;;) {
        if (local_FC >= 0.0f) {
          break;
        }
        if (*((int8_t *)&task_45 + arg1) == 2) {
          *((int8_t *)&task_45 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC + 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&task_45 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_20E57)))
static int32_t script_2C03(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&task_45 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&task_45 + arg1) = 1;
    if (task_49) {
      local_FC = local_F8 = 0.16f;
    } else {
      local_FC = local_F8 = 0.08f;
    }
    while (*((int8_t *)&task_45 + arg1) == 1) {
      if (task_49) {
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
    if (*((int8_t *)&task_45 + arg1) != 2) {
      for (;;) {
        if (local_FC <= 0.0f) {
          break;
        }
        if (*((int8_t *)&task_45 + arg1) == 2) {
          *((int8_t *)&task_45 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC - 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&task_45 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_20E6F)))
static void script_2D34(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((*(&task_C + arg1) & 2) > 0) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_50 = -1) & 4);
    }
    Script_ccowUtils(arg0, CCOW_DisableCharacters);
  }
  if (*(&task_C + arg1) & 0x20) {
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
  if (*(&task_C + arg1) & 0x20) {
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
  if (*(&task_C + arg1) & 0x20) {
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

static int32_t script_387C(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_389A(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_38B9(int32_t arg0, int32_t arg1) {
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

static void script_3931(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_1BC != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_A4[arg2], arg3, arg4);
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

static int32_t script_3EE3(int32_t arg0) {
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

static void script_3FB4(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_CCD7(arg0, arg2, 0);
  return;
}

static void script_3FC4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C46C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_7750(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CCD7(arg0, arg2, 0);
  return;
}

static void script_40C8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_C46C(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_A4[arg2]);
    script_7750(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CCD7(arg0, arg2, 0);
  return;
}

static void script_426A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C46C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 4) == 0) {
      script_5C5D(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1BC) {
        script_9030(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_CCD7(arg0, arg2, 2);
  return;
}

static void script_43CB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C46C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 4) == 0) {
      script_5C5D(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1BC) {
        script_9030(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_CCD7(arg0, arg2, 2);
  return;
}

static void script_452C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_C46C(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_A4[arg2]);
      script_5C5D(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_1BC) {
        float local_F4;
        float local_F0;
        local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_C46C(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_A4[arg2]);
        script_9030(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_CCD7(arg0, arg2, 2);
  return;
}

static void script_47CA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    script_C46C(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_CCD7(arg0, arg2, 0);
  return;
}

static void script_4898(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_C46C(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_6BDC(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CCD7(arg0, arg2, 1);
  return;
}

static void script_48FF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_C46C(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_6BDC(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CCD7(arg0, arg2, 1);
  return;
}

static void script_4966(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_C46C(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_6BDC(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_CCD7(arg0, arg2, 1);
  return;
}

static void script_49AD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_C46C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_C46C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_7096(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CCD7(arg0, arg2, 2);
  return;
}

static void script_4AE3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  if (arg0 == task_1BC) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      local_FF7F = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_C46C(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_CB35(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_1C4)->x;
      (&local_9C)->y = task_1C4.y;
      (&local_9C)->z = task_1C4.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, CHARA_PART_Get);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, CHARA_PART_Get);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_1BC) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_1C4, *(&task_1D0 + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1BC) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1BC) {
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
      script_CB35(arg0, 1);
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_1BC) {
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
    script_CCD7(arg0, arg2, 0);
  } else {
    script_40C8(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_53A4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_C46C(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_C46C(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_9751(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CCD7(arg0, arg2, 0);
  return;
}

static int32_t script_54C2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_F4 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 6);
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
        local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_C46C(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L00567E;
            }
          }
          L00567E:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_1D0 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_1C4)->x, (&local_E4)->z - task_1C4.z);
        script_9EDB(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadFloatBits, Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_21E8C, SCND_ReadFloatBits, Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_C46C(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_A4[arg2]);
      script_9FB3(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadFloatBits, Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_21E8C, SCND_ReadFloatBits, Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_C46C(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_A4[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_A42A(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadFloatBits, Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_21E8C, SCND_ReadFloatBits, Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_C46C(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_A4[arg2]);
          script_9FB3(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L005B79;
      }
    }
  }
  L005B79:;
  script_CCD7(arg0, arg2, 0);
  end:;
}

static void script_5B87(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_3EE3(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_229AA)))
static void script_5C5D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_1BC) {
        local_FF78 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_3931(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_CB35(arg0, 0);
    local_C0.x = (&task_1C4)->x;
    (&local_C0)->y = task_1C4.y;
    (&local_C0)->z = task_1C4.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_FF78);
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
    if (task_1BC == arg0) {
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
          script_5B87(arg0, task_F4[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_1BC) {
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
                goto L0061E5;
              }
            }
            L0061E5:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L006223;
                }
              }
              L006223:;
              goto L006226;
            }
          }
        }
      }
      L006226:;
      if (task_1BC == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_1BC) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_1BC != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_1BC == arg0) {
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
                goto L0063F2;
              }
            }
            L0063F2:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L006430;
                }
              }
              L006430:;
              goto L006433;
            }
          }
          L006433:;
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
              goto L00648C;
            }
          }
        }
      }
      L00648C:;
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
      if (arg0 == task_1BC) {
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
  script_CB35(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_A4[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_1BC == arg0) {
        local_80 = (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_1BC == arg0) {
        local_80 = (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_1BC == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, CHARA_PART_Set);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, CHARA_PART_Set);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L006B6F;
      }
    }
  }
  L006B6F:;
  return;
}

static void script_6B70(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1BC == arg0) {
    local_FC = task_A4[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_6BDC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_1C4)->x;
  (&local_AC)->y = task_1C4.y;
  (&local_AC)->z = task_1C4.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
    local_80 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_1BC) {
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
        task_CC[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_80 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_1BC) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_CC[arg2] = 0;
      }
    }
    local_88 = task_A4[arg2];
    local_88 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_A4[arg2];
      local_88 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_CC[arg2] = 0;
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
          goto L007046;
        }
      }
    }
    L007046:;
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

__attribute__((scn_string_bindings(0, data_229C6)))
static void script_7096(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_CB35(arg0, 0);
  local_CC.x = (&task_1C4)->x;
  (&local_CC)->y = task_1C4.y;
  (&local_CC)->z = task_1C4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_1BC) {
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
  script_CB35(arg0, 1);
  local_FC = task_A4[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_229E2)))
static void script_7750(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      if (arg0 == task_1BC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_1BC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_F4[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_F4[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_1BC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_1BC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_1BC) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_F4[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_F4[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L008343;
      }
    }
  }
  L008343:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 7);
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
  script_3931(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_CB35(arg0, 0);
  local_D0.x = (&task_1C4)->x;
  (&local_D0)->y = task_1C4.y;
  (&local_D0)->z = task_1C4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_A4);
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
    if (arg0 == task_1BC) {
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
      if (arg0 == task_1BC) {
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
              script_5B87(arg0, task_F4[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_1BC) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_1BC) {
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
        local_FF60 = script_38B9((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L008948;
          }
        }
        L008948:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_38B9(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_38B9(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_1BC) {
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
          local_FF74 = (float)script_387C(script_38B9(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_1BC) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_1BC) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_A4[arg2];
        if (arg0 == task_1BC) {
          if (task_1BC == arg0) {
            local_A0 = (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_1BC) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L008DF4;
            }
          }
        }
      }
      L008DF4:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_1BC) {
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
      if (arg0 != task_1BC) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_CB35(arg0, 1);
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
  if (arg0 == task_1BC) {
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
      goto L00902F;
    }
  }
  L00902F:;
  return;
}

__attribute__((scn_string_bindings(0, data_229FE)))
static void script_9030(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_CB35(arg0, 0);
    local_C4.x = (&task_1C4)->x;
    (&local_C4)->y = task_1C4.y;
    (&local_C4)->z = task_1C4.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
          local_84 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_1BC) {
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
                goto L009354;
              }
            }
            L009354:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L009390;
                }
              }
              L009390:;
              goto L009393;
            }
          }
        }
      }
      L009393:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L0093D8;
          }
        }
      }
      L0093D8:;
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
  script_CB35(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_A4[arg2];
    local_9C = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_5C, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_5C, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_5C, SCND_WriteU32, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_22A1A)))
static void script_9751(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_2146C, SCND_ReadU32, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_2146C, SCND_ReadFloatBits, Script_ScenDataUtil(data_20E7C, SCND_ReadU8, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 4);
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
  script_CB35(arg0, 0);
  local_B0.x = (&task_1C4)->x;
  (&local_B0)->y = task_1C4.y;
  (&local_B0)->z = task_1C4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_1BC) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_1BC) {
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
  script_CB35(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, CHARA_PART_Set);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, CHARA_PART_Set);
  return;
}

static void script_9EDB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_22A36)))
static void script_9FB3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_D4 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadU32, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadU32, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadU32, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadU32, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadU32, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_21E8C, SCND_ReadU32, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L00A1D4;
    }
  }
  L00A1D4:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_21E8C, SCND_ReadFloatBits, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_21E8C, SCND_ReadFloatBits, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_1C4)->x;
  (&local_F4)->y = task_1C4.y;
  (&local_F4)->z = task_1C4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_1BC) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_1BC) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_22A52)))
static void script_A42A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  task_1E8 = 0;
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
        goto L00A736;
      }
    }
  }
  L00A736:;
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
  local_B8.x = (&task_1C4)->x;
  (&local_B8)->y = task_1C4.y;
  (&local_B8)->z = task_1C4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_1BC) {
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
      local_FF40 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1BC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1BC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
    local_FF6C = 0;
    L00B201:;
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
      if (!(local_FF6C >= 20)) goto L00B201;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_1BC) {
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

static void script_B3BD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    if (arg0 != task_1BC) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_3EE3(arg5));
    }
  }
  return;
}

static void script_B807(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_5C = arg1;
  task_60 = arg2;
  task_64 = arg3;
  task_68 = arg4;
  task_6C.mode = 0;
  task_6C.id = 0;
  task_6C.entry_idx = 0;
  task_1BC = arg0;
  (void)((task_1E0 = -1) & 1);
  task_1DC = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L00B871:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_144[local_F8] = local_FC;
  task_7C[local_F8] = -1;
  task_A4[local_F8] = -1;
  task_CC[local_F8] = 0;
  task_F4[local_F8] = 0;
  task_11C[local_F8] = 0;
  task_16C[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L00B871;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_5C, SCND_WriteU32, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_6C, task_7C, task_A4, (shen_name *)task_CC, task_F4, arg0, arg5, data_20E7C, data_2146C, (shen_name *)task_144);
  Script_scenCreateTask(script_C0C3, 0);
  return;
}

static void script_B993(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_5C = arg0;
  task_60 = arg1;
  task_64 = arg2;
  task_68 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_1DC = 1;
  return;
}

static void script_B9C7(void) {
  int32_t local_FC;
  local_FC = 0;
  L00B9CF:;
  task_7C[local_FC] = -1;
  task_A4[local_FC] = -1;
  task_CC[local_FC] = 0;
  task_F4[local_FC] = 0;
  task_11C[local_FC] = 0;
  task_16C[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00B9CF;
  return;
}

static void script_BA30(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_1BC == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_1F0, 0);
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

static void script_BACD(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_1BC == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_1F0, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_BB07(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_BB6A(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_BBCD(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_5C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_BC04(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_5C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  return;
}

static void script_BC3B(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_A4[local_F0] == arg0) {
      local_EC = task_144[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[local_F0] * 13 + 1) & 0x20) {
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

static void script_BE7D(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_5C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_BF85(arg0);
  script_BC3B(arg0);
  return;
}

static void script_BEC6(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_5C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  script_BF93(arg0);
  script_BC3B(arg0);
  return;
}

static void script_BF0F(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_5C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_5C, SCND_WriteU32, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_BF85(int32_t arg0) {
  script_BF0F(arg0, 0);
  return;
}

static void script_BF93(int32_t arg0) {
  script_BF0F(arg0, 1);
  return;
}

static void script_BFA1(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_5C, SCND_WriteU32, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_C068(void) {
  script_BFA1(1);
  return;
}

static void script_C074(void) {
  script_BFA1(0);
  return;
}

static void script_C080(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_C0C3(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_1DC == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_7C[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_6C)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
        script_CE94(1, local_FC, local_F4, local_F8);
        if (script_D048(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_CE94(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_D25E(local_F4, local_F8);
    } else {
      task_1DC = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_C1D4(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_1BC, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_1BC, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_22ADD, 1, data_22AF6)))
static void script_C26C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_194[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_194[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_68, SCND_ReadU32, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_68, SCND_ReadU32, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_68, SCND_ReadU32, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_68, SCND_ReadU32, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_68, SCND_ReadU32, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_68, SCND_ReadU32, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_68, SCND_ReadU32, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_64, SCND_ReadScriptOffset, Script_ScenDataUtil(task_68, SCND_ReadU32, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_194[arg0] = -1;
    }
  } else {
    task_194[arg0] = -1;
  }
  return;
}

static void script_C43B(int32_t arg0) {
  if (task_194[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_194[arg0], 1);
    task_194[arg0] = -1;
  }
  return;
}

static int32_t script_C46C(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_1BC == arg0) {
    local_F4.x = (&task_1F0)->x;
    (&local_F4)->y = task_1F0.y;
    (&local_F4)->z = task_1F0.z;
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
        if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_5C, SCND_ReadU32, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_5C, SCND_ReadU32, arg6 * 13 + 1) & 0x80)) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L00C657;
        }
        L00C657:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00C6FA;
          }
        }
        L00C6FA:;
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
            goto L00C8A6;
          }
        }
      }
    }
    L00C8A6:;
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
          task_1C4.x = (&local_F4)->x;
          task_1C4.y = (&local_F4)->y;
          task_1C4.z = (&local_F4)->z;
          task_1D0 = (&local_C4)->x;
          *(&task_1D0 + 1) = (&local_C4)->y;
          *(&task_1D0 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_1C4.x = (&local_F4)->x;
          task_1C4.y = (&local_F4)->y;
          task_1C4.z = (&local_F4)->z;
          task_1D0 = (&local_C4)->x;
          *(&task_1D0 + 1) = (&local_C4)->y;
          *(&task_1D0 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_1C4, 0);
          task_1D0 = (&local_C4)->x;
          *(&task_1D0 + 1) = (&local_C4)->y;
          *(&task_1D0 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_CB35(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if (task_1BC == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, CCOW_EnableMap);
      } else {
        Script_ccowUtils(arg0, CCOW_DisableMap);
      }
    }
  }
  return;
}

static void script_CB67(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_1BC != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_5C, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_5C, SCND_ReadU32, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_22B0F)))
static void script_CCD7(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1BC == arg0) {
    task_6C.mode = 0;
    task_6C.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_1BC, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_1E0 & 0x80) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_A4[arg1];
        local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_A4[arg1];
      local_FC = Script_ScenDataUtil(task_5C, SCND_ReadU32, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_5C, SCND_ReadU32, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_7C[arg1] = 1;
  task_16C[arg1] = 0;
  task_11C[arg1] = 0;
  task_CC[arg1] = 0;
  if (task_194[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_194[arg1], 1);
    task_194[arg1] = -1;
  }
  return;
}

static void script_CE94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_C1D4(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_C1D4(arg2, arg3, local_FC);
          goto L00CF58;
        }
      }
      L00CF58:;
    } else {
      if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_C1D4(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_1BC, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L00CFF3;
          }
        }
      }
    }
  }
  L00CFF3:;
  if (task_16C[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_1EC) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_1EC) & task_1EC) {
    Script_sicnUtils(2, 4, 0);
  }
  task_1EC = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_22B1E)))
static int32_t script_D048(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg1] * 13 + 1) & 4) == 0)) {
      if (task_16C[arg1] == 0) {
        task_7C[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_CC[arg1] != task_1BC) & task_1EC) {
        if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg1] * 13 + 1) & 2) == 0) {
          task_7C[arg1] = 3;
          task_CC[arg1] = task_1BC;
          task_F4[arg1] = 0;
          task_6C.mode = 1;
          task_6C.entry_idx = task_A4[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_1BC, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_7C[arg1] = 6;
              task_CC[arg1] = task_1BC;
              task_F4[arg1] = 0;
              task_6C.mode = 0;
              task_6C.entry_idx = task_A4[arg1];
              local_FC = 0;
            }
            goto L00D1FC;
          }
        }
        L00D1FC:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_1BC, -1, &task_1F0, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_6C.id = task_1BC;
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

static void script_D25E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L00D266:;
  local_F8 = task_7C[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L00D286;
      L00D286:;
      script_D328(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L00D2A6;
      L00D2A6:;
      script_D389(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L00D2C4;
      L00D2C4:;
      script_D52E(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L00D2E6;
      L00D2E6:;
      script_D5C6(local_FC, arg1);
      break;
    case 2:
      script_D6A8(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00D266;
  return;
}

static void script_D328(int32_t arg0, int32_t arg1) {
  if (task_16C[arg0] == 0) {
    task_16C[arg0] = Script_scenCreateTask(script_54C2, 4, task_CC[arg0], task_144[arg0], arg0, arg1);
    task_11C[arg0] = task_CC[arg0];
    script_C26C(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_22B2D, 1, data_22B3F)))
static void script_D389(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_CC[arg0])) {
    if (task_16C[arg0] == 0) {
      if (task_CC[arg0] == task_1BC) {
        task_6C.mode = 1;
        task_6C.entry_idx = task_A4[arg0];
        goto L00D3DE;
      }
      L00D3DE:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_64, SCND_ReadU32, Script_ScenDataUtil(task_68, SCND_ReadU32, local_FC * 9 + 1) * 4 + 1);
      script_CB67(task_CC[arg0], task_144[arg0], task_A4[arg0], local_F8);
      task_16C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_21F2C, SCND_ReadScriptOffset, local_F8), 4, task_CC[arg0], task_144[arg0], arg0, local_F8);
      task_11C[arg0] = task_CC[arg0];
      if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_C26C(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_194[arg0] = -1;
      }
    }
  } else {
    if (task_7C[arg0] != 5) {
      task_7C[arg0] = 1;
      task_16C[arg0] = 0;
      task_11C[arg0] = 0;
      task_CC[arg0] = 0;
    }
  }
  return;
}

static void script_D52E(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_16C[arg0] == 0) & (arg2 >= 0)) {
    task_CC[arg0] = task_1BC;
    task_6C.mode = 2;
    task_6C.entry_idx = task_A4[arg0];
    task_16C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_21F2C, SCND_ReadScriptOffset, arg2), 4, task_CC[arg0], task_144[arg0], arg0, arg2);
    task_11C[arg0] = task_CC[arg0];
    script_C26C(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_22B52, 1, data_22B64)))
static void script_D5C6(int32_t arg0, int32_t arg1) {
  if ((task_16C[arg0] == 0) & (arg1 >= 0)) {
    script_CB67(task_CC[arg0], task_144[arg0], task_A4[arg0], arg1);
    task_16C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_21F2C, SCND_ReadScriptOffset, arg1), 4, task_CC[arg0], task_144[arg0], arg0, arg1);
    task_11C[arg0] = task_CC[arg0];
    if ((Script_ScenDataUtil(task_5C, SCND_ReadU32, task_A4[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_C26C(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_194[arg0] = -1;
    }
  }
  return;
}

static void script_D6A8(int32_t arg0) {
  int32_t local_FC;
  task_16C[arg0] = 0;
  task_11C[arg0] = 0;
  task_7C[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg0], 2);
  if (local_FC != -1) {
    task_CC[arg0] = 0;
  }
  return;
}

static void script_D6FF(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_D727(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_D77B(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_D7CB(int32_t arg0) {
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

static void script_D872(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_D7CB(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_D895(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_D8C8(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_D895(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_D8EB(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_D895(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_D94A(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_D95F(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_D96E(void) {
  Script_sprintf(data_22CC4);
  SCNT_Enqueue();
  return;
}

static void script_D97D(void) {
  return;
}

static void script_D980(void) {
  return;
}

static void script_D983(void) {
  return;
}

static void script_D986(void) {
  return;
}

static void script_D989(void) {
  (void)((task_208 = 0) & 0x20);
  return;
}

static int32_t script_D996(void) {
  return task_1FC = Script_freeTalkGetCurrentChara();
}

static void script_D9A2(void) {
  *(&task_1FC + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_D9B1(void) {
  return *(&task_1FC + 2) = Script_freeTalkGetAndResetState();
}

static void script_D9C0(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_D9C9(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_D9D8(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00DA04;
    }
  }
  L00DA04:;
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

static void script_DA65(int32_t arg0) {
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

static void script_DAB5(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DAE3(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DB11(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DB45(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_DB79(int32_t arg0) {
  return Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
}

static void script_DB97(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_DBD1(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static int32_t script_DC05(void) {
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
  end:;
}

static void script_DCFD(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static int32_t script_DD2D(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
        break;
      case 150:
        Script_FaceEx(CHARA_FUKU, FACE_ExpressionDisabled, 0, 1);
        break;
      case 270:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        Script_FaceEx(CHARA_FUKU, FACE_ExpressionEnabled, 0, 1);
        goto L00DDB4;
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00DDB4:;
  return Script_srchControl(0);
}

static void script_DDBB(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GART'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Show);
      goto L00DDE5;
    }
  }
  L00DDE5:;
  Script_scenToggleVisibleFor(SHEN_NAME('GART'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('SKAB'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('SKAB'), CDM_Show);
      goto L00DE19;
    }
  }
  L00DE19:;
  Script_scenToggleVisibleFor(SHEN_NAME('SKAB'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Show);
      goto L00DE4D;
    }
  }
  L00DE4D:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Show);
      goto L00DE81;
    }
  }
  L00DE81:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Show);
      goto L00DEB5;
    }
  }
  L00DEB5:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Show);
      goto L00DEE9;
    }
  }
  L00DEE9:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
      goto L00DF1D;
    }
  }
  L00DF1D:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Show);
      goto L00DF51;
    }
  }
  L00DF51:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L00DF85;
    }
  }
  L00DF85:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L00DFB9;
    }
  }
  L00DFB9:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L00DFED;
    }
  }
  L00DFED:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L00E021;
    }
  }
  L00E021:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Show);
      goto L00E055;
    }
  }
  L00E055:;
  Script_scenToggleVisibleFor(SHEN_NAME('LKUT'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Show);
      goto L00E089;
    }
  }
  L00E089:;
  Script_scenToggleVisibleFor(SHEN_NAME('RKUT'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Show);
      goto L00E0BD;
    }
  }
  L00E0BD:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAWA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KHKS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KHKS'), CDM_Show);
      goto L00E0F1;
    }
  }
  L00E0F1:;
  Script_scenToggleVisibleFor(SHEN_NAME('KHKS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
      goto L00E125;
    }
  }
  L00E125:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Show);
      goto L00E159;
    }
  }
  L00E159:;
  Script_scenToggleVisibleFor(SHEN_NAME('MKCS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAGI'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAGI'), CDM_Show);
      goto L00E18D;
    }
  }
  L00E18D:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAGI'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Show);
      goto L00E1C1;
    }
  }
  L00E1C1:;
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
      goto L00E1F5;
    }
  }
  L00E1F5:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
      goto L00E229;
    }
  }
  L00E229:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TUB2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TUB2'), CDM_Show);
      goto L00E25D;
    }
  }
  L00E25D:;
  Script_scenToggleVisibleFor(SHEN_NAME('TUB2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
      goto L00E291;
    }
  }
  L00E291:;
  Script_scenToggleVisibleFor(SHEN_NAME('POK2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
      goto L00E2C5;
    }
  }
  L00E2C5:;
  Script_scenToggleVisibleFor(SHEN_NAME('POS2'), 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_FCFB();
  local_F4 = Script_scenCreateTask(script_DD2D, 0);
  Script_FACE(CHARA_FUKU, 10, 0, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_F4);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_FaceEx(CHARA_FUKU, FACE_ExpressionEnabled, 0, 1);
  local_FC = Script_scenCreateTask(script_E386, 0);
  local_F8 = Script_scenCreateTask(script_FC76, 0);
  Script_shenSeqUtils(1);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_FD2F();
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  return;
}

static int32_t script_E386(void) {
  int32_t unused_F4[3];
  int16_t local_F2;
  uint8_t unused_F0[2];
  local_F2 = 0;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  for (;;) {
    if (local_F2 == 0) {
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
      Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_2386C, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_2386C, 0);
      Script_handSetFloatsFor(CHARA_FUKU, 1, data_2386C, 0);
      Script_handSetFloatsFor(CHARA_FUKU, 0, data_2386C, 0);
    }
    switch (local_F2) {
      case 740:
        Script_FaceEx(CHARA_FUKU, FACE_ExpressionDisabled, 0, 1);
        Script_ResetHandMotion(CHARA_FUKU, 0, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 1, 0);
        break;
      case 1032:
        Script_FaceEx(CHARA_FUKU, FACE_ExpressionEnabled, 0, 1);
        Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2386C, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2386C, 0);
        break;
    }
    switch (local_F2) {
      case 266:
        Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 10, 0);
        break;
      case 364:
        Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 0, 0);
        break;
      case 615:
        Script_FACE_LCG(CHARA_FUKU, FACE_BlinkAutomatic, 0, 0);
        break;
      case 850:
        Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
        break;
    }
    if (local_F2 == 1600) {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_F2 = local_F2 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_E59C(void) {
  int32_t unused_FC;
  script_FCFB();
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_FD2F();
  return;
}

static void script_E5C3(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (local_FC == 715) {
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_E615(void) {
  int32_t local_FC;
  script_FCFB();
  Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
  local_FC = Script_scenCreateTask(script_E668, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
  script_FD2F();
  return;
}

static int32_t script_E668(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t local_E0[3];
  Vector3i local_D4;
  Vector3i local_C8;
  Vector3i local_BC;
  int32_t local_B8;
  Script_SetMotnFlag(CHARA_FUKU, 2);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  local_C8.x = 0;
  local_BC.x = 0;
  (&local_C8)->y = 0;
  (&local_BC)->y = 0;
  (&local_C8)->z = 0;
  (&local_BC)->z = 1274;
  Script_charaPosRotUtils(SHEN_NAME('TEGS'), 152, 0, &local_C8, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('TEGS'), 153, 0, &local_BC, CHARA_PART_Set);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_B8 = local_FC;
    switch (local_B8) {
      case 1:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_234DC, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_23314, 1);
        Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_23314, 1);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_23314, 1);
        local_D4.x = -3094;
        (&local_D4)->y = -364;
        (&local_D4)->z = 2730;
        Script_SetHandRootRotation(CHARA_FUKU, 1, 0x15, &local_D4);
        local_D4.x = 0;
        (&local_D4)->y = 0;
        (&local_D4)->z = 2002;
        Script_SetHandRootRotation(CHARA_FUKU, 0, 0x15, &local_D4);
        local_EC.x = 0.123f;
        (&local_EC)->y = 0.016f;
        (&local_EC)->z = -0.052f;
        local_D4.x = 54413;
        (&local_D4)->y = 53684;
        (&local_D4)->z = 22373;
        Script_fixoAttachEx(SHEN_NAME('TEGS'), CHARA_AKIR, 18, &local_EC, &local_D4);
        local_D4.x = -4662;
        (&local_D4)->y = -691;
        (&local_D4)->z = 1742;
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_D4);
        local_E0[0] = 0;
        local_E0[1] = 0;
        local_E0[2] = 0;
        break;
      case 2:
        Script_FACE(CHARA_AKIR, 10, 3, 10);
        break;
      case 90:
        Script_FACE(CHARA_AKIR, 2, 0, 10);
        break;
      case 160:
        Script_FACE(CHARA_FUKU, 1, 4, 10);
        break;
      case 274:
        Script_FACE(CHARA_FUKU, 7, 5, 10);
        break;
      case 320:
        Script_SetCharaLookAtPosition(CHARA_FUKU, -98312, &local_EC, 0);
        break;
      case 350:
        Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Show);
        break;
      case 411:
        Script_FACE(CHARA_FUKU, 5, 5, 10);
        break;
      case 465:
        Script_FACE(CHARA_AKIR, 7, 0, 10);
        break;
      case 475:
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        break;
      case 487:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        Script_FACE(CHARA_AKIR, 2, 0, 10);
        break;
      case 511:
        Script_FACE(CHARA_FUKU, 0, 0, 10);
        break;
      case 580:
        Script_FACE(CHARA_FUKU, 4, 5, 10);
        break;
      case 660:
        Script_scenToggleActiveFor(CHARA_AKIR, 1);
        Script_scenToggleActiveFor(CHARA_AKIR, 1);
        break;
      case 715:
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2314C, 15);
        break;
      case 790:
        Script_FACE(CHARA_AKIR, 7, 0, 10);
        break;
      case 800:
        Script_SetCharaFlags(CHARA_AKIR, 0);
        Script_SetCharaFlags(CHARA_FUKU, 0);
        Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
        break;
      case 804:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        break;
      case 840:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        break;
      case 900:
        Script_FACE(CHARA_AKIR, 2, 0, 20);
        break;
      case 920:
        Script_executeAicaCommand(2720, 2, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    }
    if ((local_FC > 465) & (local_FC < 487)) {
      Script_EVT_GetPosition(CHARA_FUKU, 5, &local_EC, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 3);
    }
    if ((local_FC > 790) & (local_FC < 804)) {
      Script_EVT_GetPosition(CHARA_FUKU, 5, &local_EC, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 2);
    }
    if ((local_FC >= 400) & (local_FC <= 423)) {
      (&local_C8)->z = 1274;
      Script_charaPosRotUtils(SHEN_NAME('TEGS'), 152, 0, &local_C8, CHARA_PART_Add);
    }
    if ((local_FC >= 415) & (local_FC <= 438)) {
      (&local_BC)->z = 1274;
      Script_charaPosRotUtils(SHEN_NAME('TEGS'), 153, 0, &local_BC, CHARA_PART_Add);
    }
    if ((local_FC >= 760) & (local_FC <= 783)) {
      (&local_BC)->z = -1274;
      Script_charaPosRotUtils(SHEN_NAME('TEGS'), 153, 0, &local_BC, CHARA_PART_Add);
    }
    if ((local_FC >= 775) & (local_FC <= 798)) {
      (&local_C8)->z = -1274;
      Script_charaPosRotUtils(SHEN_NAME('TEGS'), 152, 0, &local_C8, CHARA_PART_Add);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('HOKI'));
  end:;
}

static void script_EC28(void) {
  int32_t local_FC;
  Script_charaDisplay(SHEN_NAME('MIRR'), CDM_Show);
  Script_charaSetRefboardFlagTo(SHEN_NAME('MIRR'), 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_FD4A();
  local_FC = Script_scenCreateTask(script_EC8D, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_FD2F();
  Script_srchControl((void *)local_FC);
  Script_charaDisplay(SHEN_NAME('MIRR'), CDM_Hide);
  return;
}

static int32_t script_EC8D(void) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = 0;
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_23950, 0);
  local_F0.x = 1274;
  (&local_F0)->y = -546;
  (&local_F0)->z = 728;
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_F0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  for (;;) {
    if (local_FC == 0) {
      Script_FACE(CHARA_AKIR, 2, 0, 10);
    } else if (local_FC == 121) {
      Script_FACE(CHARA_AKIR, 0, 0, 10);
    } else if (local_FC == 221) {
      Script_FACE(CHARA_AKIR, 2, 0, 10);
    } else if (local_FC == 441) {
      Script_FACE(CHARA_AKIR, 1, 0, 10);
    } else if (local_FC == 531) {
      Script_FACE(CHARA_AKIR, 2, 0, 10);
    } else if (local_FC == 690) {
      Script_FACE(CHARA_AKIR, 7, 0, 10);
    } else if (local_FC == 750) {
      Script_FACE(CHARA_AKIR, 0, 0, 10);
    } else {
      switch (local_FC) {
        case 761:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
        case 850:
          Script_FACE(CHARA_AKIR, 0, 0, 10);
          break;
        case 951:
          Script_FACE(CHARA_AKIR, 1, 0, 10);
          break;
        case 1150:
          Script_FACE(CHARA_AKIR, 8, 0, 10);
          break;
        case 1190:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
        case 1321:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
        case 1426:
          Script_FACE(CHARA_AKIR, 0, 0, 20);
          break;
        case 1710:
          Script_FACE(CHARA_AKIR, 2, 0, 20);
          break;
        case 750:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
      }
    }
    switch (local_FC) {
      case 0:
        Script_FACE(CHARA_FUKU, 0, 0, 10);
        break;
      case 221:
        Script_FACE(CHARA_FUKU, 2, 0, 10);
        break;
      case 370:
        Script_FACE(CHARA_FUKU, 6, 0, 10);
        break;
      case 441:
        Script_FACE(CHARA_FUKU, 1, 0, 10);
        break;
      case 510:
        Script_FACE(CHARA_FUKU, 7, 5, 10);
        break;
      case 531:
        Script_FACE(CHARA_FUKU, 4, 4, 10);
        break;
      case 640:
        Script_FACE(CHARA_FUKU, 5, 0, 10);
        break;
      case 720:
        Script_FACE(CHARA_FUKU, 0, 0, 10);
        break;
      case 740:
        Script_FACE(CHARA_FUKU, 5, 0, 10);
        break;
      case 761:
        Script_FACE(CHARA_FUKU, 1, 0, 10);
        break;
      case 820:
        Script_FACE(CHARA_FUKU, 7, 5, 10);
        break;
      case 840:
        Script_FACE(CHARA_FUKU, 7, 0, 10);
        break;
      case 951:
        Script_FACE(CHARA_FUKU, 4, 0, 10);
        break;
      case 1080:
        Script_FACE(CHARA_FUKU, 6, 0, 10);
        break;
      case 1190:
        Script_FACE(CHARA_FUKU, 1, 0, 10);
        break;
      case 1250:
        Script_FACE(CHARA_FUKU, 5, 3, 10);
        break;
      case 1321:
        Script_FACE(CHARA_FUKU, 0, 0, 10);
        break;
      case 1401:
        Script_FACE(CHARA_FUKU, 0, 0, 20);
        break;
      case 1440:
        Script_FACE(CHARA_FUKU, 6, 0, 25);
        break;
      case 1470:
        Script_FACE(CHARA_FUKU, 0, 0, 20);
        break;
      case 1500:
        Script_FACE(CHARA_FUKU, 4, 0, 20);
        break;
    }
    if (local_FC == 880) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 2, 0);
    }
    if (local_FC == 882) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkOpen, 2, 0);
    }
    if (local_FC == 885) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 2, 0);
    }
    if (local_FC == 887) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkOpen, 2, 0);
    }
    if (local_FC == 910) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 2, 0);
    }
    if (local_FC == 912) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkOpen, 2, 0);
    }
    if (local_FC == 915) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 2, 0);
    }
    if (local_FC == 917) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkOpen, 2, 0);
    }
    if (local_FC == 1200) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 2, 0);
    }
    if (local_FC == 1202) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkOpen, 2, 0);
    }
    if (local_FC == 1205) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 2, 0);
    }
    if (local_FC == 1207) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkOpen, 2, 0);
    }
    if (local_FC == 1220) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 2, 0);
    }
    if (local_FC == 1222) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkOpen, 2, 0);
    }
    if (local_FC == 1225) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 2, 0);
    }
    if (local_FC == 1227) {
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkOpen, 2, 0);
    }
    if (local_FC == 1770) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_F323(void) {
  int32_t local_FC;
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  script_FDDC(CHARA_JAKR);
  script_FDDC(CHARA_IWAO);
  local_FC = Script_scenCreateTask(script_F3E7, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_executeAicaCommand(2720, 2, 60);
  Script_SetMITKFlags(StartFadeOut, 0, 60);
  Script_filmCheckOrSetTransitionUtil(60, 150, 68, 50, 0, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_FE98(CHARA_JAKR);
  script_FE98(CHARA_IWAO);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static int32_t script_F3E7(void) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  int32_t local_F0;
  float local_EC;
  Vector3 local_E0;
  int32_t unused_D8[2];
  int32_t local_D4;
  Script_SetCharaFlags(CHARA_JAKR, 4);
  Script_SetCharaFlags(CHARA_IWAO, 4);
  Script_SetShadowFlags(CHARA_JAKR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_JAKR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(CHARA_JAKR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(CHARA_JAKR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"scroll", "SCROLL25.SPR");
  Script_SetMITKFlags(StartFadeIn, 5, 80);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  Script_SetFogEnabled(0);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(1);
  Script_SetVector4ByOffset(&data_23A34);
  (void)1;
  Script_charaDisplay(SHEN_NAME('MOON'), CDM_Show);
  Script_motmToggleFlags(CHARA_JAKR, SCEN_Disable, 0x40);
  Script_SetMotnFlag(CHARA_JAKR, 3);
  Script_scenCreateTask(script_DB79, 1, 80);
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4 = Script_scenCreateTask(script_DC05, 0);
  local_FC = 0;
  local_F0 = 100;
  while (Script_shenSeqUtils(-1)) {
    if (local_FC == 0) {
      Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
      Script_ResetHandMotion(CHARA_IWAO, 0, 0, 0);
      Script_executeAicaCommand(4008, 0, 0);
      Script_executeAicaCommand(1184, 2, local_F0);
    }
    if (local_FC == 1082) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
      Script_ResetHandMotion(CHARA_JAKR, 0, 1, 0);
      Script_ResetHandMotion(CHARA_JAKR, 0, 0, 0);
    }
    if (local_FC == 1475) {
      Script_charaDisplay(CHARA_JAKR, CDM_Hide);
    }
    if (local_FC == 1727) {
      Script_charaDisplay(CHARA_JAKR, CDM_Show);
    }
    if (local_FC == 1555) {
      local_E0.x = -10.3f;
      (&local_E0)->y = 1.85f;
      (&local_E0)->z = -19.1f;
      Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookAtPosition, &local_E0, 16);
    }
    local_D4 = local_FC & 3;
    switch (local_D4) {
      case 0:
        local_EC = 1.7f;
        break;
      case 1:
        local_EC = 1.52f;
        break;
      case 2:
        local_EC = 1.36f;
        break;
      case 3:
        local_EC = 1.2f;
        break;
    }
    local_EC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.75f) + 0.5f;
    Script_LightUtils(1, 4, local_EC);
    Script_LightUtils(2, 4, local_EC);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (local_FC > 2100) {
      Script_FACE_LCG(CHARA_JAKR, FACE_BlinkOpen, 3, 0);
    }
    if (!(local_FC == 2099)) goto L00F6D1;
    L00F6D1:;
  }
  Script_filmCheckOrSetTransitionUtil(80, 150, 68, 50, 0, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 2, 64);
  end:;
}

static void script_F70F(void) {
  int32_t local_FC;
  script_FCFB();
  local_FC = Script_scenCreateTask(script_F74C, 0);
  Script_shenSeqUtils(2);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_FD2F();
  Script_srchControl((void *)local_FC);
  return;
}

static int32_t script_F74C(void) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  local_FC = 0;
  for (;;) {
    local_EC = local_FC;
    switch (local_EC) {
      case 2:
        Script_FACE(CHARA_AKIR, 1, 0, 10);
        break;
      case 251:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        break;
      case 521:
        Script_FACE(CHARA_AKIR, 7, 0, 49);
        break;
      case 571:
        Script_FACE(CHARA_AKIR, 2, 0, 129);
        break;
      case 931:
        Script_FACE(CHARA_AKIR, 0, 0, 99);
        break;
      case 1651:
        Script_FACE(CHARA_AKIR, 2, 0, 99);
        break;
      case 1:
        Script_FACE(CHARA_FUKU, 9, 0, 10);
        break;
      case 181:
        Script_FACE(CHARA_FUKU, 0, 0, 10);
        break;
      case 361:
        Script_FACE(CHARA_FUKU, 7, 0, 99);
        break;
      case 461:
        Script_FACE(CHARA_FUKU, 2, 0, 59);
        break;
      case 881:
        Script_FACE(CHARA_FUKU, 3, 0, 10);
        break;
      case 1351:
        Script_FACE(CHARA_FUKU, 7, 0, 49);
        break;
      case 1521:
        Script_FACE(CHARA_FUKU, 6, 0, 49);
        break;
      case 1781:
        Script_FACE(CHARA_FUKU, 7, 0, 79);
        break;
      case 1861:
        Script_FACE(CHARA_FUKU, 3, 0, 119);
        break;
      case 1080:
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 1, 0);
        break;
      case 1321:
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2386C, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2386C, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2386C, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2386C, 0);
        break;
    }
    if ((local_FC > 1350) & (local_FC < 1450)) {
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F0, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_FUKU, FACE_LookAtPosition, &local_F0, 10);
    }
    if (local_FC == 1500) {
      Script_FaceLookAtTargetUtils(CHARA_FUKU, FACE_LookReset);
    }
    if (local_FC == 1950) {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(50, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_FA8C(void) {
  int32_t local_FC;
  if (-1) {
    Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_FUKU, CDM_Show);
      goto L00FAB6;
    }
  }
  L00FAB6:;
  Script_scenToggleVisibleFor(CHARA_FUKU, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DDO1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DDO1'), CDM_Show);
      goto L00FAEA;
    }
  }
  L00FAEA:;
  Script_scenToggleVisibleFor(SHEN_NAME('DDO1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DDO2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DDO2'), CDM_Show);
      goto L00FB1E;
    }
  }
  L00FB1E:;
  Script_scenToggleVisibleFor(SHEN_NAME('DDO2'), 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_FD8D();
  Script_mapmSetCamFlags(0, 0);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  local_FC = Script_scenCreateTask(script_FC04, 0);
  Script_shenSeqUtils(3);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_mapmSetCamFlags(0, 1);
  script_FDC1();
  Script_scenToggleVisibleFor(CHARA_FUKU, 1);
  Script_scenToggleVisibleFor(SHEN_NAME('DDO1'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('DDO2'), 1);
  Script_srchControl((void *)local_FC);
  return;
}

static int32_t script_FC04(void) {
  int32_t local_FC;
  local_FC = 0;
  Script_charaVisUtils(CHARA_INE_, 1);
  Script_charaDisplay(CHARA_INE_, CDM_Show);
  for (;;) {
    if (local_FC == 561) {
      Script_charaDisplay(CHARA_INE_, CDM_Hide);
      Script_mapmSetCamFlags(0, 1);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
    }
    if (local_FC == 734) {
      Script_charaDisplay(CHARA_INE_, CDM_Show);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_FC76(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if ((local_FC > 0) & (local_FC < 255)) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.0725f)) {
        Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 2, 0);
        local_FC = local_FC + 2;
        SCNF_WaitFrames_0000(2);
        Script_FACE_LCG(CHARA_FUKU, FACE_BlinkOpen, 2, 0);
        local_FC = local_FC + 2;
        SCNF_WaitFrames_0000(2);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_FCFB(void) {
  script_FDDC(CHARA_AKIR);
  script_FDDC(CHARA_FUKU);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  Script_SetVector4ByOffset(&data_23A68);
  (void)1;
  return;
}

static void script_FD2F(void) {
  script_FE98(CHARA_AKIR);
  script_FE98(CHARA_FUKU);
  return;
}

static void script_FD4A(void) {
  script_FDDC(CHARA_AKIR);
  script_FDDC(CHARA_FUKU);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  Script_SetVector4ByOffset(&data_23A88);
  (void)1;
  Script_SetCharaRefBoardPos(SHEN_NAME('MIRR'), &data_23A98);
  return;
}

static void script_FD8D(void) {
  script_FDDC(CHARA_AKIR);
  script_FDDC(CHARA_INE_);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetVector4ByOffset(&data_23AA8);
  (void)1;
  return;
}

static void script_FDC1(void) {
  script_FE98(CHARA_AKIR);
  script_FE98(CHARA_INE_);
  return;
}

static void script_FDDC(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_23068, 10);
  Script_handSetFloatsFor(arg0, 1, data_23068, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_23AB8);
  return;
}

static void script_FE78(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_FE98(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static void script_FEDE(void) {
  int32_t local_FC;
  script_105AA();
  local_FC = Script_scenCreateTask(script_10408, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_105EA();
  Script_srchControl((void *)local_FC);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GART'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Show);
      goto L00FF42;
    }
  }
  L00FF42:;
  Script_scenToggleVisibleFor(SHEN_NAME('GART'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('SKAB'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('SKAB'), CDM_Show);
      goto L00FF76;
    }
  }
  L00FF76:;
  Script_scenToggleVisibleFor(SHEN_NAME('SKAB'), 1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
    Script_scenToggleVisibleFor(SHEN_NAME('KAK1'), 1);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Show);
        goto L00FFC5;
      }
    }
    L00FFC5:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAK1'), 1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
    Script_scenToggleVisibleFor(SHEN_NAME('KAK2'), 1);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Show);
        goto L010014;
      }
    }
    L010014:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAK2'), 1);
  }
  if (0) {
    Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Show);
      goto L010048;
    }
  }
  L010048:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Show);
      goto L01007C;
    }
  }
  L01007C:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
      goto L0100B0;
    }
  }
  L0100B0:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Show);
      goto L0100E4;
    }
  }
  L0100E4:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L010118;
    }
  }
  L010118:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L01014C;
    }
  }
  L01014C:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L010180;
    }
  }
  L010180:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L0101B4;
    }
  }
  L0101B4:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Show);
      goto L0101E8;
    }
  }
  L0101E8:;
  Script_scenToggleVisibleFor(SHEN_NAME('LKUT'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Show);
      goto L01021C;
    }
  }
  L01021C:;
  Script_scenToggleVisibleFor(SHEN_NAME('RKUT'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('KAWA'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KHKS'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KHKS'), CDM_Show);
      goto L01025C;
    }
  }
  L01025C:;
  Script_scenToggleVisibleFor(SHEN_NAME('KHKS'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
      goto L010290;
    }
  }
  L010290:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Show);
      goto L0102C4;
    }
  }
  L0102C4:;
  Script_scenToggleVisibleFor(SHEN_NAME('MKCS'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KAGI'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KAGI'), CDM_Show);
      goto L0102F8;
    }
  }
  L0102F8:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAGI'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('TUB2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('TUB2'), CDM_Show);
      goto L010350;
    }
  }
  L010350:;
  Script_scenToggleVisibleFor(SHEN_NAME('TUB2'), 1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
        goto L01039D;
      }
    }
    L01039D:;
    Script_scenToggleVisibleFor(SHEN_NAME('POK2'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('POK2'), 1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
        goto L0103EC;
      }
    }
    L0103EC:;
    Script_scenToggleVisibleFor(SHEN_NAME('POS2'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('POS2'), 1);
  }
  return;
}

static int32_t script_10408(void) {
  int32_t unused_E8[6];
  int32_t local_E4;
  int32_t local_E0;
  Script_charaVisUtils(CHARA_AKIR, 1);
  local_E4 = 0;
  for (;;) {
    local_E0 = local_E4;
    switch (local_E0) {
      case 0:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
        Script_FaceEx(CHARA_INE_, FACE_ExpressionDisabled, 0, 1);
        break;
      case 200:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        Script_FaceEx(CHARA_INE_, FACE_ExpressionEnabled, 0, 1);
        break;
      case 1670:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
        Script_FaceEx(CHARA_INE_, FACE_ExpressionDisabled, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_INE_, 0, 0, 0);
        Script_ResetHandMotion(CHARA_INE_, 0, 1, 0);
        break;
      case 1890:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        Script_FaceEx(CHARA_INE_, FACE_ExpressionEnabled, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_INE_, 1, 0, 0);
        Script_ResetHandMotion(CHARA_INE_, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_23068, 10);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_23068, 10);
        Script_handSetFloatsFor(CHARA_INE_, 0, data_23068, 10);
        Script_handSetFloatsFor(CHARA_INE_, 1, data_23068, 10);
        break;
    }
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_105AA(void) {
  int32_t unused_E8[6];
  script_10605(CHARA_AKIR);
  script_10605(CHARA_INE_);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_INE_, -1);
  Script_SetVector4ByOffset(&data_23AC8);
  (void)1;
  return;
}

static void script_105EA(void) {
  script_1070A(CHARA_AKIR);
  script_1070A(CHARA_INE_);
  return;
}

static void script_10605(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_23068, 10);
  Script_handSetFloatsFor(arg0, 1, data_23068, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaFlags(arg0, 4);
  Script_scenToggleActiveFor(arg0, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_23AD8);
  return;
}

static void script_106BE(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_106E7(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  return;
}

static void script_1070A(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static void script_10750(void) {
  if (task_234) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_234);
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x554B5546, 0);
    task_23C = 0;
  }
  if (task_238) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_238);
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5F454E49, 0);
    task_240 = 0;
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_107A8(void) {
  int32_t unused_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  task_234 = 0;
  task_238 = 0;
  if (task_23C == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x554B5546, 1);
    task_234 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x554B5546);
    while (task_234 == 0) {
      Script_sprintf("ふくちゃん読み込み失敗\012");
      SCNF_WaitFrames_0000(1);
      task_234 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x554B5546);
    }
    while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, task_234) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, 0x554B5546, 0);
    task_23C = 1;
    script_D6FF(CHARA_FUKU, -1000.0f, 0.0f, -1000.0f);
  }
  if (task_240 == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5F454E49, 1);
    task_238 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x5F454E49);
    while (task_238 == 0) {
      Script_sprintf("いねさん読み込み失敗\012");
      SCNF_WaitFrames_0000(1);
      task_238 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x5F454E49);
    }
    while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, task_238) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, 0x5F454E49, 0);
    task_240 = 1;
    script_D6FF(CHARA_INE_, -1000.0f, 0.0f, -1000.0f);
  }
  return;
}

static void script_10911(void) {
  if (task_229 == 0) {
    Script_SetGlobalSceneSlotValue34(1, 1);
    Script_SetGlobalSceneSlotValue34(3, 1);
    Script_PREP_Related(-1, 0);
    Script_SetupLightFromTable(0);
    Script_PREP_Related(0, 1);
  }
  return;
}

static void script_10949(void) {
  if (task_229 == 0) {
    Script_SetGlobalSceneSlotValue34(1, 0);
    Script_SetGlobalSceneSlotValue34(3, 0);
    Script_shenSetLightingMode(1);
    Script_PREP_Related(-1, 0);
  }
  return;
}

static int32_t script_10977(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  local_E8.x = (&local_E8)->x - (&local_F4)->x;
  (&local_E8)->z = (&local_E8)->z - (&local_F4)->z;
  Script_mtwkGetSetRotation(arg0, &local_DC, 0x2000000);
  (&local_DC)->y = (&local_DC)->y - Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E8)->x, (&local_E8)->z) + 7281;
  if (((&local_DC)->y < 14563) & ((&local_DC)->y > 0)) {
    local_D8 = 1;
  } else {
    local_D8 = 0;
  }
  end:;
}

static int32_t script_10A07(void) {
  int32_t unused_E0[8];
  task_268 = 0;
  Script_EVT_SetPosition(SHEN_NAME('KAK1'), POS_XYZ, &data_23B24);
  Script_EVT_SetPosition(SHEN_NAME('KAK2'), POS_XYZ, &data_23B30);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Hide);
  Script_ccowSetHeightMode(SHEN_NAME('DDO1'), CCOW_KeepHeight);
  Script_ccowSetHeightMode(SHEN_NAME('DDO2'), CCOW_KeepHeight);
  task_270 = 0;
  task_271 = 0;
  task_272 = 0;
  task_273 = 0;
  task_274 = 0;
  task_275 = 0;
  task_276 = 0;
  task_277 = 0;
  task_278 = 0;
  task_279 = 0;
  task_27A = 0;
  task_27B = 0;
  task_27C = 0;
  Script_scenToggleCharaFlag(0, 200, 0);
  task_27E = 0;
  task_27F = 0;
  task_280 = 0;
  task_281 = 0;
  task_282 = 0;
  task_283 = 0;
  task_284 = 0;
  task_288 = 0;
  Script_scenCreateTask(script_112EA, 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  for (;;) {
    if (task_268 == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_10B3C(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED)) {
    Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_23BA4);
  } else if (task_22C | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) == 0) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
        Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_240F4);
      } else {
        Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_23E2C);
      }
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
      Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_23F90);
    } else {
      Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_23C88);
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
      Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_246E4);
    } else {
      Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_243DC);
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
    Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_24560);
  } else {
    Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_24218);
  }
  return;
}

static void script_10C37(void) {
  return;
}

static void script_10C3A(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0))) {
    if (task_22C) {
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
      script_157DE();
    } else {
      Vector3 local_F4;
      Vector3i local_E8;
      local_F4.x = -2.3825f;
      (&local_F4)->y = 1.7465f;
      (&local_F4)->z = -24.4261f;
      local_E8.x = 2389;
      (&local_E8)->y = 64763;
      (&local_E8)->z = 62968;
      Script_EVT_SetPosition(SHEN_NAME('POK2'), POS_XYZ, &local_F4);
      Script_EVT_SetRotation(SHEN_NAME('POK2'), 0x38000000, &local_E8, 0);
      Script_EVT_SetPosition(SHEN_NAME('POS2'), POS_XYZ, &local_F4);
      Script_EVT_SetRotation(SHEN_NAME('POS2'), 0x38000000, &local_E8, 0);
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
    }
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -9.8f;
    (&local_F4)->y = 1.0565f;
    (&local_F4)->z = -24.112f;
    local_E8.x = 29190;
    (&local_E8)->y = 50373;
    (&local_E8)->z = 3288;
    Script_EVT_SetPosition(SHEN_NAME('KAK3'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('KAK3'), 0x38000000, &local_E8, 0);
    Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -9.8f;
    (&local_F4)->y = 1.0565f;
    (&local_F4)->z = -22.6f;
    local_E8.x = 29190;
    (&local_E8)->y = 50373;
    (&local_E8)->z = 3288;
    Script_EVT_SetPosition(SHEN_NAME('KAK4'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('KAK4'), 0x38000000, &local_E8, 0);
    Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Hide);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    Script_fixoAttachEx(SHEN_NAME('TUB2'), SHEN_NAME('SKAB'), 0, &data_24840, &data_2484C);
    Script_charaDisplay(SHEN_NAME('TUB2'), CDM_Show);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    Script_fixoAttachEx(SHEN_NAME('POK2'), SHEN_NAME('SKAB'), 0, &data_24828, &data_24834);
    Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &data_24858, 0);
    Script_scenToggleCharaFlag(0, 202, 0);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG)) {
    if (Script_GetOwnedItemCount(4, 0) == 0) {
      Script_GetCassetteFlags(4, 1, 0, 0);
    }
  }
  return;
}

static int32_t script_10F3A(void) {
  for (;;) {
    if (-(task_228 == 0) & task_22F) {
      task_260 = Script_srchUtils(-1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_10F63(void) {
  int32_t local_FC;
  SAVE_Script_UpdateForSaveData(0, 0);
  SAVE_Script_UpdateForSaveData(3, 0, 14400);
  for (;;) {
    if (-(task_228 == 0) & Script_GetOwnedItemCount(4, 0) & Script_SetTaskDataValue(CHARA_AKIR, &data_24864, &data_24870, 0)) {
      local_FC = SAVE_Script_UpdateForSaveData(4, 0);
      if ((local_FC == 0) & (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 0)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND)) {
          if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN) == 0)) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_shenClearFreeState(0);
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
            if (task_27D == 0) {
              task_27D = 1;
              Script_seqEnterEvtCam(1011, CHARA_AKIR, CHARA_AKIR);
              Script_ShowUIMessageFromID("E1011A109");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_seqEnterEvtCam(690, 0, AUTH_None);
              SCNF_WaitFrames_0000(30);
              Script_ShowUIMessageFromID("E1011A110");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            } else {
              Script_seqEnterEvtCam(1011, CHARA_AKIR, CHARA_AKIR);
              Script_ShowUIMessageFromID("E1011A111");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ShowUIMessageFromID("E1011A112");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_seqEnterEvtCam(690, 0, AUTH_None);
              SCNF_WaitFrames_0000(30);
              Script_ShowUIMessageFromID("E1011A113");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            }
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_HANGING_SCROLL_MEANING, 1);
            Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
            Script_convSetCamInactiveFlag(0);
            Script_SetGameStateTo(AUTH_None);
            Script_ryoControl(RYO_RestoreControl);
            SAVE_Script_UpdateForSaveData(3, 0, 14400);
            Script_shenClearFreeState(1);
          }
        } else {
          if (Script_GetOwnedItemCount(39, 0) == 0) {
            Script_seqEnterEvtCam(680, 0, AUTH_None);
            SCNF_WaitFrames_0000(1);
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x534B484B);
            if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KHKS_HINT_INTRO_SEEN)) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              Script_shenClearFreeState(0);
              Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              task_27D = 1;
              SCNF_WaitFrames_0000(30);
              task_228 = 1;
              Script_ShowUIMessageFromID("E1011A107");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ShowUIMessageFromID("E1011A108");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_srchUtils(SRCH_SCRIPT_Reset);
              task_27D = 0;
              Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_ANYTHING_IN_DADS_ROOM, 1);
              task_228 = 0;
              Script_convSetCamInactiveFlag(0);
              Script_SetGameStateTo(AUTH_None);
              Script_ryoControl(RYO_RestoreControl);
              Script_shenClearFreeState(1);
            } else {
              SCNF_WaitFrames_0000(30);
            }
            SAVE_Script_UpdateForSaveData(3, 0, 14400);
            goto L011276;
          }
        }
      }
    }
    L011276:;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_11288(void) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_248C4, &data_248D0, 0)) {
      if (local_FF == 0) {
        local_FF = 1;
        Script_SetMaxCharaSearchExtent(1, 0.1f);
      }
    } else {
      if (local_FF) {
        local_FF = 0;
        Script_SetMaxCharaSearchExtent(1, 3.0f);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_112EA(void) {
  int32_t unused_E8[6];
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  Vector3i local_D0;
  int32_t local_CC;
  int32_t local_C8;
  Script_SetMaxCharaSearchExtent(1, 3.0f);
  script_10B3C();
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  script_10C3A();
  Script_scenCreateTask(script_11288, 0);
  Script_scenCreateTask(script_10F3A, 0);
  Script_scenCreateTask(script_10F63, 0);
  for (;;) {
    if (-(task_228 == 0) & task_22F) {
      local_CC = Script_StepUtils(GET_STEP_HIGHER, 0);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_D0, 0x2000000);
      if ((task_260 == -1) & (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1)) {
        if (Script_GetControllerInput(1) & 0x400) {
          while (Script_IsPlayerAllowed(0) == 0) {
            SCNF_WaitFrames_0000(1);
          }
          if (task_280) {
            script_11C80();
          } else if (task_281) {
            script_11D65();
          } else if (task_283) {
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x534B484B);
          } else if (task_27F) {
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 11);
          } else if (task_284) {
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x324B4F50);
          } else if (task_288) {
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x4F4B4148);
          } else {
            if (task_22C) {
              script_11E6C();
              goto L011421;
            }
          }
        }
        L011421:;
      } else {
        local_C8 = task_260;
        switch (local_C8) {
          case 0x324B4F50:
            script_155FF();
            break;
          case 0x534B484B:
            break;
          case 0x54524147:
            break;
          case 0x324B414B:
            script_11C39();
            break;
          case 0x314B414B:
            script_11CEE();
            break;
          case 0x31524946:
            break;
          case 0x32524946:
            break;
          case 0x33524946:
            break;
          case 0x34524946:
            break;
          case 0x4F4B4148:
            if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 807) & -(Script_GetOwnedItemCount(36, 0) == 0) & -(Script_GetOwnedItemCount(35, 0) == 0) & -(task_22C == 0)) {
              script_16A8C();
            }
            break;
          case 10:
            script_11CEE();
            break;
          case 11:
            break;
        }
      }
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 0) {
        if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) == 0) & Script_GetOwnedItemCount(39, 0)) {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMYKE, 1);
          if (task_282 == 0) {
            task_282 = 1;
            Script_sicnUtils(0, 8, 1);
            Script_sicnUtils(2, 4, 0);
            task_283 = 0;
          }
        }
      } else {
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMYKE, 0);
        if (task_282) {
          task_282 = 0;
          Script_sicnUtils(2, 8, 0);
        }
        if (Script_SetTaskDataValue(CHARA_AKIR, &data_2490C, &data_24918, 0) & -((&local_D0)->y > 9102) & -((&local_D0)->y < 23665) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) == 0) & Script_GetOwnedItemCount(39, 0)) {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMYKE, 1);
          if (task_283 == 0) {
            task_283 = 1;
            Script_sicnUtils(0, 12, 1);
          }
        } else {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMYKE, 0);
          if (task_283) {
            task_283 = 0;
            Script_sicnUtils(1, 0, 0);
          }
        }
      }
      if (-(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 0) & -(Script_GetOwnedItemCount(39, 0) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) == 0) & -(task_27D == 0)) {
        script_169B9();
      } else {
        task_279 = 0;
      }
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 7) {
        if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 1);
          if (task_27E == 0) {
            task_27E = 1;
            Script_sicnUtils(0, 8, 1);
            Script_sicnUtils(2, 4, 0);
            task_27F = 0;
          }
        }
      } else {
        if (task_22C == 0) {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
        }
        if (task_27E) {
          task_27E = 0;
          Script_sicnUtils(2, 8, 0);
        }
        if (Script_SetTaskDataValue(CHARA_AKIR, &data_248F4, &data_24900, 0) & -(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 1);
          if (task_27F == 0) {
            task_27F = 1;
            Script_sicnUtils(0, 12, 1);
          }
        } else {
          if ((task_22C == 0) | (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) != -1)) {
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
          }
          if (task_27F) {
            task_27F = 0;
            Script_sicnUtils(1, 0, 0);
          }
        }
      }
      if (-(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 7) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
        script_16419();
      } else {
        task_277 = 0;
      }
      if (-(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 8) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
        script_1654D();
      } else {
        task_278 = 0;
      }
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 1) {
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_HIHY_EVENT_DONE) == 0) & (local_CC < 2)) {
          Script_srchUtils(SRCH_SCRIPT_Reset);
          script_1E5FD();
        } else {
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 807, 1);
          script_1592A();
        }
      } else {
        task_27B = 0;
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_248F4, &data_24900, 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN) == 0) & -((&local_D0)->y > 9102) & -((&local_D0)->y < 23665)) {
        if (task_280 == 0) {
          task_280 = 1;
          Script_sicnUtils(0, 4, 1);
        }
      } else {
        if (task_280) {
          task_280 = 0;
          Script_sicnUtils(2, 4, 0);
        }
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_248DC, &data_248E8, 0) & -((&local_D0)->y > 9102) & -((&local_D0)->y < 23665) & ((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0) & task_22C) | -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0))) {
        if (task_281 == 0) {
          task_281 = 1;
          Script_sicnUtils(0, 4, 1);
        }
      } else {
        if (task_281) {
          task_281 = 0;
          Script_sicnUtils(2, 4, 0);
        }
      }
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1) {
        if (Script_SetTaskDataValue(CHARA_AKIR, &data_249B4, &data_249C0, SHEN_NAME('POK2')) & script_10977(CHARA_AKIR, SHEN_NAME('POK2')) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) & -(task_22C == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0)) {
          if (task_284 == 0) {
            task_284 = 1;
            Script_sicnUtils(0, 4, 1);
          }
        } else {
          if (task_284) {
            task_284 = 0;
            Script_sicnUtils(2, 4, 0);
          }
        }
      } else {
        if (task_284) {
          task_284 = 0;
          Script_sicnUtils(2, 4, 0);
        }
      }
      if ((Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 2) | (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 3) | (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 4) | (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 5)) {
        script_158B4();
      } else {
        task_27A = 0;
      }
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 6) {
        if (task_288) {
          task_288 = 0;
          Script_sicnUtils(2, 4, 0);
        }
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 807) & -(Script_GetOwnedItemCount(36, 0) == 0) & -(Script_GetOwnedItemCount(35, 0) == 0) & -(task_22C == 0)) {
        } else {
          script_16A16();
        }
      } else {
        task_27C = 0;
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 807) & -(Script_GetOwnedItemCount(36, 0) == 0) & -(Script_GetOwnedItemCount(35, 0) == 0) & -(task_22C == 0) & Script_SetTaskDataValue(CHARA_AKIR, &data_249B4, &data_249C0, SHEN_NAME('HAKO'))) {
          if (task_288 == 0) {
            task_288 = 1;
            Script_sicnUtils(0, 4, 1);
          }
        } else {
          if (task_288) {
            task_288 = 0;
            Script_sicnUtils(2, 4, 0);
          }
        }
      }
      if (Script_IsPlayerAllowed(0)) {
        local_E4 = Script_cassetteUtils();
        local_E0 = (local_E4 & -0x10000) >> 16;
        local_DC = local_E4 & 0xFFFF;
        if ((local_E0 == 4) & (local_DC == 2)) {
          if ((task_27E == 0) & (task_27F == 0)) {
            script_145F3();
          } else {
            Script_GetCassetteFlags_0(4, 1, 0);
            task_27E = 0;
            task_27F = 0;
            Script_sicnUtils(1, 0, 0);
            script_14A93();
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
          }
        }
        if ((local_E0 == 39) & (local_DC == 2)) {
          Script_GetCassetteFlags_0(39, 1, 0);
          task_282 = 0;
          task_283 = 0;
          Script_sicnUtils(1, 0, 0);
          script_13568();
        }
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_249CC, &data_249D8, 0)) {
        if (task_22C) {
          script_15448();
        }
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_24924, &data_24930, 0)) {
        script_14DAE();
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_11C39(void) {
  task_280 = 0;
  Script_sicnUtils(2, 4, 0);
  if (Script_GetOwnedItemCount(4, 0)) {
    task_277 = 1;
    script_13F99(CHARA_AKIR);
    script_10B3C();
  } else {
    script_143EC(CHARA_AKIR);
  }
  return;
}

static void script_11C80(void) {
  Script_ryoControl(PCCTL_RequestNewMode, 5);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  task_280 = 0;
  Script_sicnUtils(2, 4, 0);
  if (Script_GetOwnedItemCount(4, 0)) {
    task_277 = 1;
    script_13F99(CHARA_AKIR);
    script_10B3C();
  } else {
    script_144E6(CHARA_AKIR);
  }
  return;
}

static void script_11CEE(void) {
  task_281 = 0;
  Script_sicnUtils(2, 4, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0) {
    if (task_22C) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0) {
        script_13C96(CHARA_AKIR);
        script_10B3C();
      } else {
        script_12764();
      }
    } else {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0) {
        script_141E5(CHARA_AKIR);
      }
    }
  }
  return;
}

static void script_11D65(void) {
  Script_ryoControl(PCCTL_RequestNewMode, 5);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  task_281 = 0;
  Script_sicnUtils(2, 4, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0) {
    if (task_22C) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0) {
        script_13C96(CHARA_AKIR);
        script_10B3C();
      } else {
        script_12764();
      }
    } else {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0) {
        script_142DF(CHARA_AKIR);
      }
    }
  }
  return;
}

static int32_t script_11E03(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_charaDisplay(arg0, CDM_Hide);
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    if (task_228 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMapControlTaskFlag(arg0, 1);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_249FC)))
static void script_11E6C(void) {
  Script_SetGameStateTo(AUTH_None);
  task_228 = 1;
  Script_shenClearFreeState(0);
  Script_ryoControl(PCCTL_RequestNewMode, 5);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_SetTaskDataValue(CHARA_AKIR, &data_23B78, &data_23B84, 0) == 0) {
    Vector3 local_F4;
    local_F4.x = -6.7f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -22.5f;
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_23AE8, 0.0f, 0, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_23B90, 0, 0);
    {
      int32_t unused_E8[3];
      int32_t local_E4;
      local_E4 = 0;
      while (local_E4 < 30) {
        Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f);
        local_E4 = local_E4 + 1;
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
    Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('PONK'), 0x38000000, &local_E8, 0);
    Script_EVT_SetPosition(SHEN_NAME('PONS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('PONS'), 0x38000000, &local_E8, 0);
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_scenCreateTask(script_15AA8, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7024, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('PONK'), 7058, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('PONS'), 7057, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_AddCharacterHandler(3, "BROOM", SHEN_NAME('DMY0'));
  Script_scenCreateTask(script_11E03, 1, SHEN_NAME('DMY0'));
  {
    int32_t unused_E8[6];
    int32_t local_E4;
    local_E4 = 0;
    while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
      int32_t local_E0;
      local_E0 = local_E4;
      switch (local_E0) {
        case 0:
          Script_seqEnterEvtCam(2001, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          break;
        case 50:
          Script_executeAicaCommand(853417, 0, 0);
          break;
        case 120:
          Script_seqEnterEvtCam(2002, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          break;
        case 140:
          Script_executeAicaCommand(0x1F05A9, 0, 0);
          break;
        case 330:
          Script_executeAicaCommand(787881, 0, 0);
          break;
        case 349:
          break;
      }
      local_E4 = local_E4 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  task_228 = 0;
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_24AAC, 1, data_24AB6)))
static void script_121DF(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t unused_CC[3];
  int32_t local_C8;
  local_F4.x = -10.4869f;
  (&local_F4)->y = 2.3514f;
  (&local_F4)->z = -23.1343f;
  task_228 = 1;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_24A04);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_24A10, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_scenCreateTask(script_152DD, 0);
  Script_fixoAttachEx(SHEN_NAME('PONK'), CHARA_AKIR, 12, &data_24A94, &data_24AA0);
  Script_fixoAttachEx(SHEN_NAME('PONS'), CHARA_AKIR, 12, &data_24A94, &data_24AA0);
  Script_PlayCharaMotion(CHARA_AKIR, 7030, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
  for (;;) {
    if (local_D8 == 721) {
      local_D8 = 0;
      break;
    } else {
      local_D8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_C8 = local_D8;
    switch (local_C8) {
      case 110:
        Script_ShowUIMessageFromID("E1011A055");
        break;
      case 225:
        Script_executeAicaCommand(787881, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('PONK'));
        Script_fixoClearAttachment(SHEN_NAME('PONS'));
        Script_fixoAttachEx(SHEN_NAME('PONK'), CHARA_AKIR, 18, &data_24A1C, &data_24A28);
        Script_fixoAttachEx(SHEN_NAME('PONS'), CHARA_AKIR, 18, &data_24A1C, &data_24A28);
        local_E8.x = (&data_24A1C)->x;
        (&local_E8)->y = (&data_24A1C)->y;
        (&local_E8)->z = (&data_24A1C)->z;
        break;
      case 286:
        Script_fixoClearAttachment(SHEN_NAME('PONS'));
        Script_fixoAttachEx(SHEN_NAME('PONS'), CHARA_AKIR, 12, &data_24A34, &data_24A40);
        break;
      case 367:
        Script_executeAicaCommand(0x1705A9, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('PONS'));
        break;
      case 438:
        local_DC.x = (&data_24A28)->x;
        (&local_DC)->y = (&data_24A28)->y;
        (&local_DC)->z = (&data_24A28)->z;
        local_E8.x = (&data_24A1C)->x;
        (&local_E8)->y = (&data_24A1C)->y;
        (&local_E8)->z = (&data_24A1C)->z;
        break;
      case 448:
        Script_executeAicaCommand(0x1805A9, 0, 0);
        break;
      case 460:
        break;
      case 599:
        Script_executeAicaCommand(0x1905A9, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('PONK'));
        Script_EVT_GetPosition(CHARA_AKIR, 18, &local_F4, 0);
        (&local_F4)->y = (&local_F4)->y - 0.13f;
        (&local_F4)->z = (&local_F4)->z + 0.03f;
        local_F4.x = (&local_F4)->x - 0.09f;
        Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &local_F4);
        Script_EVT_SetRotation(SHEN_NAME('PONK'), 0x38000000, &data_24A64, 0);
        break;
      case 700:
        Script_ShowUIMessageFromID("E1011A045");
        break;
      case 720:
        Script_fixoClearAttachment(SHEN_NAME('PONK'));
        Script_fixoAttachEx(SHEN_NAME('PONK'), SHEN_NAME('SKAB'), 0, &data_24A4C, &data_24A58);
        break;
    }
    if ((local_D8 > 438) & (local_D8 < 448)) {
      Script_fixoClearAttachment(SHEN_NAME('PONK'));
      (&local_DC)->y = (&local_DC)->y + 3640;
      (&local_E8)->y = (&local_E8)->y - 0.006f;
      Script_fixoAttachEx(SHEN_NAME('PONK'), CHARA_AKIR, 18, &local_E8, &local_DC);
    }
    if ((local_D8 > 599) & (local_D8 < 720)) {
      Script_EVT_GetPosition(CHARA_AKIR, 18, &local_E8, 0);
      local_F4.x = (&local_E8)->x - 0.09f;
      Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &local_F4);
    }
    if (local_D8 > 225) {
      if (local_D8 < 285) {
        local_E8.x = (&local_E8)->x + (0.087f - (&data_24A1C)->x) / (float)40;
        (&local_E8)->y = (&local_E8)->y + (0.514f - (&data_24A1C)->y) / (float)40;
        (&local_E8)->z = (&local_E8)->z + (-0.03f - (&data_24A1C)->z) / (float)40;
        Script_fixoClearAttachment(SHEN_NAME('PONS'));
        Script_fixoAttachEx(SHEN_NAME('PONS'), CHARA_AKIR, 18, &local_E8, &data_24A28);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED, 1);
  task_22C = 0;
  Script_ryoSleepUtils(3, 1, 1, 0);
  if (Script_GetOwnedItemCount(4, 0) & -(task_27E == 0) & -(task_27F == 0)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
  }
  Script_CharHandlers(CHARA_AKIR, 0, 3, 16);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED)) {
    script_12FFD();
  } else {
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_15);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(15);
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
    script_10B3C();
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ryoControl(RYO_RestoreControl);
  task_228 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_24B68)))
static void script_12764(void) {
  int32_t unused_DC[9];
  int32_t local_D8;
  int32_t unused_CC[3];
  int32_t local_C8;
  Script_srchUtils(SRCH_SCRIPT_Reset);
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_24AC0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_24ACC, 0);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
  Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &data_24AC0);
  Script_EVT_SetRotation(SHEN_NAME('PONK'), 0x38000000, &data_24ACC, 0);
  Script_EVT_SetPosition(SHEN_NAME('PONS'), POS_XYZ, &data_24AC0);
  Script_EVT_SetRotation(SHEN_NAME('PONS'), 0x38000000, &data_24ACC, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 7039, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('PONK'), 7063, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('PONS'), 7068, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_seqEnterEvtCam(700, 0, AUTH_None);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      local_D8 = 0;
      break;
    } else {
      local_D8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_C8 = local_D8;
    switch (local_C8) {
      case 100:
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
        Script_executeAicaCommand(853417, 0, 0);
        break;
      case 190:
        Script_seqEnterEvtCam(701, 0, AUTH_None);
        break;
      case 245:
        Script_executeAicaCommand(0x1705A9, 0, 0);
        break;
      case 290:
        Script_seqEnterEvtCam(702, 0, AUTH_None);
        break;
      case 345:
        Script_executeAicaCommand(0x1805A9, 0, 0);
        break;
      case 350:
        Script_seqEnterEvtCam(703, 0, AUTH_None);
        break;
      case 400:
        Script_CharHandlers(CHARA_AKIR, 1, 14, 16);
        break;
      case 575:
        Script_executeAicaCommand(0x1905A9, 0, 0);
        break;
      case 590:
        Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
        Script_fixoAttachEx(SHEN_NAME('POK2'), SHEN_NAME('SKAB'), 0, &data_24B08, &data_24B14);
        Script_ShowUIMessageFromID("E1011A045");
        break;
      case 640:
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED)) {
          Script_executeAicaCommand(0x1B05A9, 0, 0);
        }
        break;
      case 650:
        goto L012A67;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L012A67:;
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED, 1);
  task_22C = 0;
  Script_ryoSleepUtils(3, 1, 1, 0);
  if (Script_GetOwnedItemCount(4, 0) & -(task_27E == 0) & -(task_27F == 0)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED)) {
    script_12FFD();
  } else {
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_15);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(15);
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
    script_10B3C();
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ryoControl(RYO_RestoreControl);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_24B72)))
static void script_12B83(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_seqEnterEvtCam(605, 0, AUTH_None);
  local_F4.x = -10.5762f;
  (&local_F4)->y = 1.1095f;
  (&local_F4)->z = -23.0063f;
  local_E8.x = 0;
  (&local_E8)->y = 0x4000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7030, 720.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    {
      Vector3i local_D8;
      int32_t local_D4;
      local_D4 = local_E4;
      switch (local_D4) {
        case 721:
          Script_scenCreateTask(script_1605F, 0);
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
          break;
        case 731:
          Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
          (&local_D8)->y = (&local_D8)->y + 27;
          Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
          break;
        case 830:
          Script_executeAicaCommand(0x1C05A9, 0, 0);
          break;
      }
    }
    if (local_E4 > 828) {
      if (local_E4 < 900) {
        Vector3i local_D8;
        Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
        (&local_D8)->y = (&local_D8)->y + (int32_t)(((float)(local_E4 - 828) * 0.009f + 0.8f) / 360.0f * (float)0x10000);
        Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
        Script_mtwkGetSetRotation(CHARA_AKIR, &task_254, 0);
        task_254.y = task_254.y + 91;
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_254, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED, 1);
  script_10B3C();
  Script_ProcessSubtitle("JINGURU01", 0);
  Script_scenToggleCharaFlag(0, 202, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_24B7C)))
static void script_12D94(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_seqEnterEvtCam(605, 0, AUTH_None);
  local_F4.x = -10.5762f;
  (&local_F4)->y = 1.1095f;
  (&local_F4)->z = -23.05f;
  local_E8.x = 0;
  (&local_E8)->y = 0x4000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_scenCreateTask(script_1605F, 0);
  Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
  Script_PlayCharaMotion(CHARA_AKIR, 7030, 820.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    {
      Vector3i local_D8;
      int32_t local_D4;
      local_D4 = local_E4;
      switch (local_D4) {
        case 731:
          Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
          (&local_D8)->y = (&local_D8)->y + 27;
          Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
          break;
        case 830:
          Script_executeAicaCommand(0x1C05A9, 0, 0);
          break;
      }
    }
    if (local_E4 > 828) {
      if (local_E4 < 900) {
        Vector3i local_D8;
        Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
        (&local_D8)->y = (&local_D8)->y + (int32_t)(((float)(local_E4 - 828) * 0.009f + 0.8f) / 360.0f * (float)0x10000);
        Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
        Script_mtwkGetSetRotation(CHARA_AKIR, &task_254, 0);
        task_254.y = task_254.y + 91;
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_254, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED, 1);
  script_10B3C();
  Script_ProcessSubtitle("JINGURU01", 0);
  Script_scenToggleCharaFlag(0, 202, 0);
  return;
}

static int32_t script_12F8B(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(605, 0, AUTH_None);
        break;
      case 125:
        Script_seqEnterEvtCam(606, 0, AUTH_None);
        break;
      case 300:
        Script_seqEnterEvtCam(607, 0, AUTH_None);
        goto L012FF6;
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L012FF6:;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_24B86)))
static void script_12FFD(void) {
  Vector3 local_F4;
  local_F4.x = -10.7769f;
  (&local_F4)->y = 1.102f;
  (&local_F4)->z = -22.7708f;
  {
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_scenCreateTask(script_1605F, 0);
  Script_scenCreateTask(script_12F8B, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7006, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    int32_t unused_E8[3];
    int32_t local_E4;
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    {
      Vector3i local_D8;
      int32_t local_D4;
      local_D4 = local_E4;
      switch (local_D4) {
        case 66:
          Script_CharHandlers(CHARA_AKIR, 0, 1, 16);
          break;
        case 83:
          Script_executeAicaCommand(0x1C05A9, 0, 0);
          break;
        case 86:
          Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
          break;
        case 350:
          goto L0131B4;
          break;
      }
      if ((local_E4 > 88) & (local_E4 < 198)) {
        Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
        (&local_D8)->y = (&local_D8)->y + 100;
        Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
      }
    }
    if ((local_E4 > 197) & (local_E4 < 231)) {
      Vector3i local_D8;
      Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
      (&local_D8)->y = (&local_D8)->y + 200;
      Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  L0131B4:;
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_ProcessSubtitle("JINGURU01", 0);
  SCNF_WaitFrames_0000(90);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  {
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED, 1);
  script_10B3C();
  Script_scenToggleCharaFlag(0, 202, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  return;
}

static void script_13271(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, 0x554B5546);
  SCNF_WaitFrames_0000(1);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  Script_charaDisplay(CHARA_FUKU, CDM_Show);
  local_F4.x = -1.4197f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -22.45f;
  local_E8.x = 0;
  (&local_E8)->y = 0xC000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_FUKU, &data_24B90, 0, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_FUKU, 0.05f) != -1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMapControlTaskFlag(CHARA_FUKU, 1);
  Script_srchControl(0);
  return;
}

static int32_t script_13366(void) {
  int32_t unused_E8[6];
  if (task_23C) {
    if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_FOLLOWUP_COMPLETE) == 0) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1077_INE_HANDOFF_DONE)) {
      Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_234);
      task_23C = 0;
    }
  } else {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x554B5546, 1);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  }
  task_22A = 1;
  return Script_srchControl(0);
}

static void script_133C4(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int8_t local_E7;
  local_E7 = 0;
  Script_charaPosRotUtils(SHEN_NAME('KHKS'), 152, &local_F4, &local_E8, CHARA_PART_Get);
  while (local_E7 < 60) {
    local_E8.x = (&local_E8)->x - 364;
    Script_charaPosRotUtils(SHEN_NAME('KHKS'), 152, &local_F4, &local_E8, CHARA_PART_Set);
    local_E7 = local_E7 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

static void script_13421(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int8_t local_E7;
  local_E7 = 0;
  Script_charaPosRotUtils(SHEN_NAME('KHKS'), 152, &local_F4, &local_E8, CHARA_PART_Get);
  while (local_E7 < 80) {
    local_E8.x = (&local_E8)->x + 273;
    Script_charaPosRotUtils(SHEN_NAME('KHKS'), 152, &local_F4, &local_E8, CHARA_PART_Set);
    local_E7 = local_E7 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

static int32_t script_1347E(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(611, 0, AUTH_None);
        break;
      case 90:
        Script_seqEnterEvtCam(612, 0, AUTH_None);
        break;
      case 150:
        Script_seqEnterEvtCam(613, 0, AUTH_None);
        break;
      case 290:
        Script_seqEnterEvtCam(614, 0, AUTH_None);
        break;
      case 410:
        Script_seqEnterEvtCam(615, 0, AUTH_None);
        break;
      case 530:
        Script_seqEnterEvtCam(616, 0, AUTH_None);
        break;
      case 630:
        Script_seqEnterEvtCam(617, 0, AUTH_None);
        break;
      case 750:
        Script_seqEnterEvtCam(610, 0, AUTH_None);
        goto L013561;
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L013561:;
  return Script_srchControl(0);
}

static void script_13568(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  task_228 = 1;
  Script_scenCreateTask(script_13366, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  local_F4.x = -10.11089f;
  (&local_F4)->y = 0.95036f;
  (&local_F4)->z = -25.216909f;
  local_E8.x = 0;
  (&local_E8)->y = 0x4000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_scenCreateTask(script_1347E, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7004, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_16190, 0);
  local_E4 = 0;
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    local_E0 = local_E4;
    switch (local_E0) {
      case 124:
        local_F4.x = -10.46f;
        (&local_F4)->y = 1.0556f;
        (&local_F4)->z = -25.2f;
        local_E8.x = 0;
        (&local_E8)->y = 0x4000;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(SHEN_NAME('KAGI'), POS_XYZ, &local_F4);
        Script_EVT_SetRotation(SHEN_NAME('KAGI'), 0x38000000, &local_E8, 0);
        Script_tmnmUtils(SHEN_NAME('KAGI'), 7065, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 120:
        Script_executeAicaCommand(0x2305A9, 0, 0);
        break;
      case 205:
        Script_executeAicaCommand(591273, 0, 0);
        break;
      case 260:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 14, 16);
        break;
      case 285:
        Script_executeAicaCommand(460201, 0, 0);
        Script_tmnmUtils(SHEN_NAME('KHKS'), 7067, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 380:
        Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
        break;
      case 436:
        local_F4.x = -10.46f;
        (&local_F4)->y = 1.0556f;
        (&local_F4)->z = -25.2f;
        local_E8.x = 0;
        (&local_E8)->y = 0x4000;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &local_F4);
        Script_EVT_SetRotation(SHEN_NAME('PONK'), 0x38000000, &local_E8, 0);
        Script_EVT_SetPosition(SHEN_NAME('PONS'), POS_XYZ, &local_F4);
        Script_EVT_SetRotation(SHEN_NAME('PONS'), 0x38000000, &local_E8, 0);
        Script_tmnmUtils(SHEN_NAME('PONK'), 7070, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_tmnmUtils(SHEN_NAME('PONS'), 7069, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
        local_F4.x = 0.06721f;
        (&local_F4)->y = 0.22969f;
        (&local_F4)->z = -0.03007f;
        local_E8.x = -34588;
        (&local_E8)->y = -32952;
        (&local_E8)->z = -31832;
        Script_fixoAttachEx(SHEN_NAME('POK2'), CHARA_AKIR, 12, &local_F4, &local_E8);
        Script_fixoAttachEx(SHEN_NAME('POS2'), CHARA_AKIR, 12, &local_F4, &local_E8);
        Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
        break;
      case 565:
        Script_CharHandlers(CHARA_AKIR, 1, 14, 16);
        break;
      case 570:
        Script_executeAicaCommand(0x1705A9, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('POK2'));
        Script_fixoClearAttachment(SHEN_NAME('POS2'));
        break;
      case 600:
        break;
      case 630:
        Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 13, 16);
        break;
      case 655:
        Script_tmnmUtils(SHEN_NAME('KHKS'), 7066, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 656:
        Script_tmnmUtils(SHEN_NAME('KAGI'), 7064, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 745:
        Script_executeAicaCommand(525737, 0, 0);
        break;
      case 750:
        Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
        break;
      case 775:
        Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
        local_F4.x = 0.0922f;
        (&local_F4)->y = 0.1018f;
        (&local_F4)->z = 0.00806f;
        local_E8.x = 0;
        (&local_E8)->y = 0;
        (&local_E8)->z = 0;
        Script_fixoAttachEx(SHEN_NAME('POK2'), CHARA_AKIR, 12, &local_F4, &local_E8);
        Script_fixoAttachEx(SHEN_NAME('POS2'), CHARA_AKIR, 12, &local_F4, &local_E8);
        break;
      case 800:
        Script_executeAicaCommand(0x3805A9, 0, 0);
        break;
      case 840:
        goto L013B25;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L013B25:;
  local_F4.x = -9.6f;
  (&local_F4)->y = 0.959f;
  (&local_F4)->z = -25.2668f;
  local_E8.x = 0;
  (&local_E8)->y = 0x4000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND, 1);
  SAVE_Script_UpdateForSaveData(3, 0, 14400);
  task_22C = 1;
  task_228 = 0;
  if (Script_GetOwnedItemCount(4, 0)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 1);
  }
  Script_ryoSleepUtils(3, 1, 1, 1);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_13C3B(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(631, 0, AUTH_None);
        break;
      case 270:
        Script_seqEnterEvtCam(632, 0, AUTH_None);
        goto L013C8F;
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L013C8F:;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_24C54)))
static void script_13C96(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t unused_EC;
  int32_t local_E4[2];
  int32_t local_E0;
  local_E4[1] = -0x42333333;
  local_E4[0] = 0x3D75C28F;
  local_FC = 0;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN, 1);
  task_228 = 1;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_24BF4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_24C00, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, 7044, 90.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_13C3B, 0);
  Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
  Script_CharHandlers(CHARA_AKIR, 1, 3, 16);
  for (;;) {
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (local_FC > 470) {
      break;
    }
    local_E0 = local_FC;
    switch (local_E0) {
      case 145:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        break;
      case 149:
        Script_tmnmUtils(SHEN_NAME('KAK1'), 7075, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_executeAicaCommand(0x1005A9, 0, 0);
        break;
      case 360:
        Script_executeAicaCommand(0x1105A9, 0, 0);
        Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
        Script_EVT_SetPosition(SHEN_NAME('KAK3'), POS_XYZ, &data_24C0C);
        Script_EVT_SetRotation(SHEN_NAME('KAK3'), 0x38000000, &data_24C18, 0);
        Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
        break;
      case 400:
        Script_ShowUIMessageFromID("E1011A078");
        break;
      case 470:
        Script_ShowUIMessageFromID("E1011A079");
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(90);
  local_F0.x = -10.62f;
  (&local_F0)->y = 1.1f;
  (&local_F0)->z = -23.29f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static void script_13EF6(void) {
  Script_seqEnterEvtCam(101, 0, AUTH_None);
  SCNF_WaitFrames_0000(225);
  Script_seqEnterEvtCam(102, 0, AUTH_None);
  SCNF_WaitFrames_0000(77);
  Script_seqEnterEvtCam(103, 0, AUTH_None);
  SCNF_WaitFrames_0000(218);
  return;
}

static int32_t script_13F31(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(621, 0, AUTH_None);
        break;
      case 270:
        Script_seqEnterEvtCam(622, 0, AUTH_None);
        break;
      case 335:
        goto L013F92;
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L013F92:;
  return Script_srchControl(0);
}

static void script_13F99(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t unused_E0[4];
  int32_t local_D8[2];
  int32_t local_D4;
  local_D8[1] = -0x42333333;
  local_D8[0] = 0x3D75C28F;
  local_FC = 0;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN, 1);
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_24C68);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_24C74, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7044, 90.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_13F31, 0);
  Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
  Script_CharHandlers(CHARA_AKIR, 1, 3, 16);
  Script_scenCreateTask(script_163B9, 0);
  for (;;) {
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (local_FC > 490) {
      break;
    }
    local_D4 = local_FC;
    switch (local_D4) {
      case 145:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        break;
      case 149:
        Script_tmnmUtils(SHEN_NAME('KAK2'), 7075, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_executeAicaCommand(0x1005A9, 0, 0);
        break;
      case 360:
        Script_executeAicaCommand(0x1105A9, 0, 0);
        Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Hide);
        Script_EVT_SetPosition(SHEN_NAME('KAK4'), POS_XYZ, &data_24C80);
        Script_EVT_SetRotation(SHEN_NAME('KAK4'), 0x38000000, &data_24C8C, 0);
        Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
        break;
      case 400:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(50);
  local_F0.x = -10.62f;
  (&local_F0)->y = 1.1f;
  (&local_F0)->z = -21.67f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static void script_141E5(int32_t arg0) {
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, SHEN_NAME('KAK1'), 40);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1005A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_162E6();
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1105A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static void script_142DF(int32_t arg0) {
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_seqEnterEvtCam(2051, 0, AUTH_None);
  SCNF_WaitFrames_0000(16);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, SHEN_NAME('KAK1'), 40);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1005A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_162E6();
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1105A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static void script_143EC(int32_t arg0) {
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, SHEN_NAME('KAK2'), 40);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1005A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_16213();
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1105A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static void script_144E6(int32_t arg0) {
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_seqEnterEvtCam(2050, 0, AUTH_None);
  SCNF_WaitFrames_0000(16);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, SHEN_NAME('KAK2'), 40);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1005A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_16213();
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1105A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_24CC8)))
static void script_145F3(void) {
  task_228 = 1;
  Script_shenClearFreeState(0);
  Script_ryoControl(PCCTL_RequestNewMode, 5);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_SetTaskDataValue(CHARA_AKIR, &data_23B78, &data_23B84, 0) == 0) {
    Vector3 local_F4;
    local_F4.x = -6.7f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -22.5f;
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_23AE8, 0.0f, 0, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_23B90, 0, 0);
    {
      int32_t unused_E8[3];
      int32_t local_E4;
      local_E4 = 0;
      while (local_E4 < 30) {
        Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f);
        local_E4 = local_E4 + 1;
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
    Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('PONK'), 0x38000000, &local_E8, 0);
    Script_EVT_SetPosition(SHEN_NAME('PONS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('PONS'), 0x38000000, &local_E8, 0);
    Script_EVT_SetPosition(SHEN_NAME('TUBA'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('TUBA'), 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 7042, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_AddCharacterHandler(3, "BROOM", SHEN_NAME('DMY0'));
  Script_scenCreateTask(script_11E03, 1, SHEN_NAME('DMY0'));
  {
    int32_t unused_E8[6];
    int32_t local_E4;
    local_E4 = 0;
    while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
      int32_t local_E0;
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
      local_E0 = local_E4;
      switch (local_E0) {
        case 1:
          Script_seqEnterEvtCam(720, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          Script_tmnmUtils(SHEN_NAME('PONK'), 7073, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('PONS'), 7071, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          break;
        case 50:
          Script_executeAicaCommand(853417, 0, 0);
          break;
        case 120:
          Script_seqEnterEvtCam(721, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          break;
        case 230:
          Script_executeAicaCommand(0x1705A9, 0, 0);
          break;
        case 240:
          Script_seqEnterEvtCam(722, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          break;
        case 333:
          Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Show);
          Script_tmnmUtils(SHEN_NAME('TUBA'), 7074, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          break;
        case 335:
          if (task_276 == 0) {
            Script_ShowUIMessageFromID("E1011A103");
          }
          break;
        case 490:
          if (task_276 == 0) {
            Script_ShowUIMessageFromID("E1011A104");
            task_276 = 1;
          } else {
            Script_ShowUIMessageFromID("E1011A105");
          }
          break;
        case 540:
          Script_seqEnterEvtCam(723, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          break;
        case 634:
          Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
          break;
        case 723:
          Script_tmnmUtils(SHEN_NAME('PONS'), 7072, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          break;
        case 820:
          Script_executeAicaCommand(787881, 0, 0);
          break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
  Script_ryoControl(RYO_RestoreControl);
  task_228 = 0;
  Script_SetGameStateTo(AUTH_None);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_14A50(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    if (local_F8 == 0) {
      Script_seqEnterEvtCam(640, 0, AUTH_None);
      break;
      goto L014A78;
    }
    L014A78:;
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_24D40, 1, data_24D4A)))
static void script_14A93(void) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  task_228 = 1;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED, 1);
  task_254.y = 0x4000;
  task_248.x = -10.745f;
  task_248.y = 1.12075f;
  task_248.z = -21.875f;
  local_FC = 0;
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_248);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_254, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_scenCreateTask(script_14A50, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7010, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(SHEN_NAME('TUB2'), CDM_Show);
  Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
  Script_CharHandlers(CHARA_AKIR, 1, 3, 16);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  for (;;) {
    if (local_FC > 330) {
      local_FC = 0;
      break;
    } else {
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_EC = local_FC;
    switch (local_EC) {
      case 91:
        Script_fixoAttachEx(SHEN_NAME('TUB2'), CHARA_AKIR, 18, &data_24CEC, &data_24CF8);
        Script_facemotUtils(CHARA_AKIR, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 43);
        break;
      case 111:
        Script_ShowUIMessageFromID("E1011A082");
        break;
      case 185:
        break;
      case 280:
        Script_executeAicaCommand(0x1205A9, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('TUBA'));
        Script_fixoAttachEx(SHEN_NAME('TUB2'), SHEN_NAME('SKAB'), 0, &data_24D28, &data_24D34);
        break;
      case 291:
        Script_ShowUIMessageFromID("E1011A045");
        break;
      case 320:
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED)) {
          Script_executeAicaCommand(0x1B05A9, 0, 0);
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED)) {
    script_12FFD();
  } else {
    local_F0.x = -10.44f;
    (&local_F0)->y = 1.1f;
    (&local_F0)->z = -21.6f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  }
  SCNF_WaitFrames_0000(1);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_14D3B(void) {
  int32_t unused_FC;
  Script_seqEnterEvtCam(501, 0, AUTH_None);
  SCNF_WaitFrames_0000(70);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(125);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_14DAE(void) {
  int32_t local_FC;
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    int32_t local_F8;
    local_F8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    switch (local_F8) {
      case 0:
        Script_ShowUIMessageFromID("E1011A116");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A117");
        break;
      case 2:
        Script_ShowUIMessageFromID("E1011A118");
        break;
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_24D54, 0, 0);
    for (;;) {
      if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f) == -1) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetGameStateTo(AUTH_None);
    Script_convSetCamInactiveFlag(1);
    Script_ryoControl(RYO_RestoreControl);
    task_228 = 0;
    Script_shenClearFreeState(1);
  } else {
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    SCNF_WaitFrames_0000(30);
    Script_charaToggleMotEnabled(CHARA_AKIR, 0);
    Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
    Script_scenCreateTask(script_14D3B, 0);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
    task_254.x = 0;
    task_254.y = 0x8000;
    task_254.z = 0;
    task_248.x = -12.6812f;
    task_248.y = 1.06f;
    task_248.z = -26.243f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_248);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_254, 0);
    Script_PlayCharaMotion(CHARA_AKIR, 7020, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      Script_RotateNodeUtils(CHARA_AKIR, &data_20CCC, 0);
      SCNF_WaitFrames_0000(1);
    }
    task_254.y = 65353;
    task_248.x = -12.7149f;
    task_248.y = 0.0f;
    task_248.z = -25.1996f;
    {
      Vector3i local_F0;
      (&local_F0)->y = task_254.y;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
    }
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_248);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_8);
    Script_charaToggleMotEnabled(CHARA_AKIR, 1);
    Script_PlayCharaMotion(CHARA_AKIR, 7019, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      Script_RotateNodeUtils(CHARA_AKIR, &data_20CCC, 0);
      SCNF_WaitFrames_0000(1);
    }
    task_254.y = 65353;
    task_248.x = -12.7713f;
    task_248.y = -1.309f;
    task_248.z = -25.15f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_248);
    {
      Vector3i local_F0;
      (&local_F0)->y = task_254.y;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
    }
    Script_PlayCharaMotion(CHARA_AKIR, 7019, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      Script_RotateNodeUtils(CHARA_AKIR, &data_20CCC, 0);
      SCNF_WaitFrames_0000(1);
    }
    task_228 = 0;
    local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
    if (local_FC < 20) {
      Script_shenRequestGameModuleTransition(2, SHEN_NAME('YDB1'), 0);
      SCNF_WaitFrames_0000(1);
    } else {
      Script_shenRequestGameModuleTransition(3, SHEN_NAME('YDB1'), 0);
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static void script_151D6(void) {
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_seqEnterEvtCam(501, 0, AUTH_None);
  Script_PlayCharaMotion(CHARA_AKIR, MN_A_WALK_L_02, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_24D88, 0, 0);
  for (;;) {
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f) == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_14DAE();
  task_268 = 1;
  return;
}

static void script_15275(void) {
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCurrentScrollEnabled(0);
  Script_SetFogFlags(2, 0);
  return;
}

static void script_152B4(void) {
  Script_seqEnterEvtCam(104, 0, AUTH_None);
  SCNF_WaitFrames_0000(184);
  Script_seqEnterEvtCam(105, 0, AUTH_None);
  SCNF_WaitFrames_0000(140);
  return;
}

static int32_t script_152DD(void) {
  Script_seqEnterEvtCam(700, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(190);
  Script_seqEnterEvtCam(701, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(90);
  Script_seqEnterEvtCam(702, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(135);
  Script_seqEnterEvtCam(703, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(45);
  Script_seqEnterEvtCam(605, 0, AUTH_None);
  end:;
}

static void script_15351(void) {
  int32_t unused_E8[6];
  Vector3i local_DC;
  int32_t unused_C4[6];
  Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_DC, 0);
  (&local_DC)->y = (&local_DC)->y + 0x4000;
  Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_DC, 0);
  Script_EVT_SetPosition(SHEN_NAME('KAK3'), POS_XYZ, &data_24E10);
  Script_EVT_SetRotation(SHEN_NAME('KAK3'), 0x38000000, &data_24E1C, 0);
  Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
  Script_EVT_SetPosition(SHEN_NAME('KAK4'), POS_XYZ, &data_24E28);
  Script_EVT_SetRotation(SHEN_NAME('KAK4'), 0x38000000, &data_24E34, 0);
  Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Hide);
  Script_scenToggleCharaFlag(0, 202, 0);
  for (;;) {
    if (Script_GetControllerInput(1) & 0x400) {
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_24DD4, &data_24DE0, 0)) {
        Script_scenCreateTask(script_14DAE, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_15448(void) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED, 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ShowUIMessageFromID(data_24E64);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_24E40, 0, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f) != -1) {
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 0;
  (&local_E8)->y = 0xC000;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7035, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (local_E4 == 90) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_E0 = local_E4;
    if (local_E0 == 70) {
      Script_fixoClearAttachment(SHEN_NAME('POK2'));
      Script_fixoClearAttachment(SHEN_NAME('POS2'));
      script_15817();
      goto L0155A2;
    }
    L0155A2:;
    SCNF_WaitFrames_0000(1);
  }
  task_22C = 0;
  if (Script_GetOwnedItemCount(4, 0) & -(task_27E == 0) & -(task_27F == 0)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
  }
  Script_ryoSleepUtils(3, 1, 1, 0);
  script_10B3C();
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_shenClearFreeState(1);
  return;
}

static void script_155FF(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  task_228 = 1;
  Script_shenClearFreeState(0);
  task_284 = 0;
  Script_sicnUtils(2, 4, 0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  task_22C = 1;
  Script_scenCreateTask(script_13366, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  local_F0.x = -3.1f;
  (&local_F0)->y = 1.0595f;
  (&local_F0)->z = -24.5f;
  local_E4.x = 0;
  (&local_E4)->y = 0xC000;
  (&local_E4)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E4, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_PlayCharaMotion(CHARA_AKIR, 7048, 60.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (local_FC == 165) {
      local_FC = 0;
      break;
    } else {
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_E0 = local_FC;
    if (local_E0 == 120) {
      Script_fixoAttachEx(SHEN_NAME('POK2'), CHARA_AKIR, 12, &data_24E88, &data_24E94);
      Script_fixoAttachEx(SHEN_NAME('POS2'), CHARA_AKIR, 12, &data_24E88, &data_24E94);
      goto L015765;
    }
    L015765:;
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  script_10B3C();
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  if (Script_GetOwnedItemCount(4, 0)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 1);
  }
  Script_ryoSleepUtils(3, 1, 1, 1);
  task_228 = 0;
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_shenClearFreeState(1);
  return;
}

static void script_157DE(void) {
  Script_fixoAttachEx(SHEN_NAME('POK2'), CHARA_AKIR, 12, &data_24EA0, &data_24EAC);
  Script_fixoAttachEx(SHEN_NAME('POS2'), CHARA_AKIR, 12, &data_24EA0, &data_24EAC);
  return;
}

static void script_15817(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = -2.3825f;
  (&local_F4)->y = 1.7465f;
  (&local_F4)->z = -24.4261f;
  local_E8.x = 2389;
  (&local_E8)->y = 64763;
  (&local_E8)->z = 62968;
  Script_EVT_SetPosition(SHEN_NAME('POK2'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('POK2'), 0x38000000, &local_E8, 0);
  Script_EVT_SetPosition(SHEN_NAME('POS2'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('POS2'), 0x38000000, &local_E8, 0);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
  return;
}

__attribute__((scn_string_bindings(0, data_24EB8)))
static void script_158B4(void) {
  if (task_27A == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_27A = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    Script_ShowUIMessageFromID("E1011A031");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_convSetCamInactiveFlag(0);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    task_228 = 0;
    Script_shenClearFreeState(1);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_24ECC)))
static void script_1592A(void) {
  if (task_27B == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_27B = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    if ((Script_GetOwnedItemCount(36, 0) == 0) & (Script_GetOwnedItemCount(35, 0) == 0)) {
      Script_ShowUIMessageFromID("E1011A101");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("E1011A031");
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_convSetCamInactiveFlag(0);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    task_228 = 0;
    Script_shenClearFreeState(1);
  }
  return;
}

static void script_159DE(void) {
  int32_t local_FC;
  local_FC = task_270;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1011A026");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1011A027");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      task_270 = 1;
      break;
    case 1:
      local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      switch (local_FC) {
        case 0:
          Script_ShowUIMessageFromID("E1011A028");
          break;
        case 1:
          Script_ShowUIMessageFromID("E1011A029");
          break;
        case 2:
          Script_ShowUIMessageFromID("E1011A030");
          break;
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  return;
}

static int32_t script_15AA8(void) {
  SCNF_WaitFrames_0000(135);
  {
    int32_t local_FC;
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A032");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A033");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(9);
  {
    int32_t local_FC;
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A074");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A077");
        break;
      case 2:
        Script_ShowUIMessageFromID("E1011A076");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_24F3A, 1, data_24F44, 2, data_24F4E, 3, data_24F58, 4, data_24F62)))
static void script_15B77(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_FIRST_SEARCH_HINT_SEEN)) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    switch (local_FC) {
      case 0:
        SCNF_WaitFrames_0000(20);
        Script_ShowUIMessageFromID("E1011A017");
        break;
      case 1:
        SCNF_WaitFrames_0000(30);
        Script_ShowUIMessageFromID("E1011A016");
        break;
      case 2:
        SCNF_WaitFrames_0000(15);
        Script_ShowUIMessageFromID("E1011A018");
        break;
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("E1011A007");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A016");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_FIRST_SEARCH_HINT_SEEN, 1);
  }
  Script_srchControl(0);
  return;
}

static void script_15C53(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_2A4 = 1;
  Script_scenCreateTask(script_15C9A, 0);
  while (task_2A4) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

__attribute__((scn_string_bindings(0, data_24F6C, 1, data_24F76, 4, data_24F94, 7, data_24FB2, 9, data_24FC6, 10, data_24FD0, 12, data_24FE4, 13, data_24FEE)))
static int32_t script_15C9A(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_FIRST_SEARCH_HINT_SEEN)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
      task_271 = 1;
    }
    local_FC = task_271;
    switch (local_FC) {
      case 0:
        SCNF_WaitFrames_0000(430);
        Script_ShowUIMessageFromID("E1011A034");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID("E1011A035");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_271 = 1;
        break;
      case 1:
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A036");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A037");
            break;
          case 2:
            Script_ShowUIMessageFromID("E1011A034");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A038");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A039");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A040");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A041");
            break;
          case 2:
            Script_ShowUIMessageFromID("E1011A042");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
  } else {
    SCNF_WaitFrames_0000(225);
    Script_ShowUIMessageFromID("E1011A007");
    SCNF_WaitFrames_0000(175);
    Script_ShowUIMessageFromID("E1011A008");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A034");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A035");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_FIRST_SEARCH_HINT_SEEN, 1);
    task_271 = 1;
  }
  task_2A4 = 0;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_24FF8, 1, data_25002, 2, data_2500C)))
static void script_15ED8(void) {
  SCNF_WaitFrames_0000(150);
  Script_ShowUIMessageFromID("E1011A082");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1011A045");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1011A007");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

__attribute__((scn_string_bindings(0, data_25016)))
static void script_15F44(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ShowUIMessageFromID("E1011A046");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_15F85(void) {
  int32_t local_FC;
  SCNF_WaitFrames_0000(600);
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1011A051");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1011A052");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1011A053");
      break;
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

__attribute__((scn_string_bindings(1, data_25048)))
static void script_15FFC(void) {
  SCNF_WaitFrames_0000(630);
  Script_executeAicaCommand(0x2105A9, 0, 0);
  Script_ShowUIMessageFromID("E1011A054");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_ShowUIMessageFromID("E1011A055");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

static int32_t script_1605F(void) {
  SCNF_WaitFrames_0000(90);
  Script_ShowUIMessageFromID("E1011A056");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(90);
  Script_ShowUIMessageFromID("E1011A057");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(1, data_25070, 2, data_2507A, 3, data_25084)))
static void script_160B4(void) {
  int32_t local_FC;
  if (task_272 == 0) {
    SCNF_WaitFrames_0000(102);
    Script_ShowUIMessageFromID("E1011A047");
    SCNF_WaitFrames_0000(180);
    Script_ShowUIMessageFromID("E1011A045");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A007");
    Script_seqEnterEvtCam(2006, 0, AUTH_None);
    SCNF_WaitFrames_0000(30);
    Script_ShowUIMessageFromID("E1011A046");
    task_272 = 1;
  } else {
    SCNF_WaitFrames_0000(300);
    Script_ShowUIMessageFromID("E1011A048");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A049");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A050");
        break;
    }
  }
  Script_srchControl(0);
  return;
}

static int32_t script_16190(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 90:
        Script_ShowUIMessageFromID("E1011A070");
        break;
      case 210:
        Script_ShowUIMessageFromID("E1011A071");
        break;
      case 390:
        Script_ShowUIMessageFromID("E1011A072");
        break;
      case 450:
        Script_ShowUIMessageFromID("E1011A087");
        goto L01620C;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L01620C:;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_250D4, 1, data_250DE, 2, data_250E8, 3, data_250F2, 4, data_250FC)))
static void script_16213(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_FIRST_SEARCH_REACTION_SEEN)) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A017");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A016");
        break;
      case 2:
        Script_ShowUIMessageFromID("E1011A018");
        break;
      case 3:
        Script_ShowUIMessageFromID("E1011A065");
        break;
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("E1011A007");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A063");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_FIRST_SEARCH_REACTION_SEEN, 1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_25110, 1, data_2511A, 2, data_25124, 3, data_2512E, 4, data_25138, 5, data_25142)))
static void script_162E6(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_FIRST_SEARCH_REACTION_SEEN)) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A017");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A016");
        break;
      case 2:
        Script_ShowUIMessageFromID("E1011A018");
        break;
      case 3:
        Script_ShowUIMessageFromID("E1011A065");
        break;
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("E1011A007");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A016");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_FIRST_SEARCH_REACTION_SEEN, 1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2514C, 1, data_25156)))
static int32_t script_163B9(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    local_F8 = local_FC;
    if (local_F8 == 300) {
      Script_ShowUIMessageFromID("E1011A040");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1011A078");
      break;
      goto L0163FE;
    }
    L0163FE:;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(3, data_2517E, 4, data_25188)))
static void script_16419(void) {
  if (task_277 == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_277 = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED)) {
    } else {
      if (Script_GetOwnedItemCount(4, 0)) {
        Script_sicnUtils(2, 8, 0);
      }
      {
        int32_t local_FC;
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A089");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A081");
            break;
          case 2:
            Script_ShowUIMessageFromID("E1011A090");
            break;
        }
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      {
        int32_t local_FC;
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A040");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A042");
            break;
        }
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      if (Script_GetOwnedItemCount(4, 0)) {
        Script_sicnUtils(0, 8, 1);
      }
    }
    Script_ryoControl(RYO_RestoreControl);
    task_228 = 0;
    Script_shenClearFreeState(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_25192, 4, data_251BA)))
static void script_1654D(void) {
  if (task_278 == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_278 = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED)) {
      Script_ShowUIMessageFromID("E1011A031");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      {
        int32_t local_FC;
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A091");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A092");
            break;
        }
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      {
        int32_t local_FC;
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A093");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A042");
            break;
        }
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_ryoControl(RYO_RestoreControl);
    task_228 = 0;
    Script_shenClearFreeState(1);
  }
  return;
}

static int32_t script_1665E(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KHKS_HINT_INTRO_SEEN) == 0) {
    Script_ShowUIMessageFromID("E1011A094");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A095");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KHKS_HINT_INTRO_SEEN, 1);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_KAGI_KEY_TAKEN) == 0) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 193, 1);
    }
  } else {
    Script_ShowUIMessageFromID("E1011A096");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A097");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A098");
        break;
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return Script_srchControl(0);
}

static int32_t script_1674C(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 90;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(611, 0, AUTH_None);
        break;
      case 90:
        Script_seqEnterEvtCam(612, 0, AUTH_None);
        break;
      case 150:
        Script_seqEnterEvtCam(613, 0, AUTH_None);
        break;
      case 215:
        Script_scenCreateTask(script_1665E, 0);
        break;
      case 290:
        Script_seqEnterEvtCam(614, 0, AUTH_None);
        goto L016847;
        break;
      case 410:
        Script_seqEnterEvtCam(615, 0, AUTH_None);
        break;
      case 530:
        Script_seqEnterEvtCam(616, 0, AUTH_None);
        break;
      case 630:
        Script_seqEnterEvtCam(617, 0, AUTH_None);
        break;
      case 750:
        Script_seqEnterEvtCam(610, 0, AUTH_None);
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L016847:;
  return Script_srchControl(0);
}

static void script_1684E(void) {
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -10.03f;
    (&local_F4)->y = 0.959f;
    (&local_F4)->z = -25.2f;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_PlayCharaMotion(CHARA_AKIR, 7001, 90.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_1674C, 0);
  for (;;) {
    int32_t unused_E8[6];
    int32_t local_E4;
    if (local_E4 == 340) {
      break;
    }
    local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    SCNF_WaitFrames_0000(1);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -9.6f;
    (&local_F4)->y = 0.959f;
    (&local_F4)->z = -25.2668f;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  return;
}

static void script_169B9(void) {
  if (task_279 == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_279 = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    script_1684E();
    task_228 = 0;
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_convSetCamInactiveFlag(0);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_251F6)))
static void script_16A16(void) {
  if (task_27C == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_27C = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    Script_ShowUIMessageFromID("E1011A031");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    task_228 = 0;
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_convSetCamInactiveFlag(0);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
  }
  return;
}

static void script_16A8C(void) {
  if (task_27C == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    task_27C = 1;
    task_228 = 1;
    Script_shenClearFreeState(0);
    Script_GetCassetteFlags(36, 1, 0, 0);
    script_16B19();
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    SCNF_WaitFrames_0000(30);
    Script_scenCreateTask(script_13366, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    script_16C85();
    task_228 = 0;
    Script_shenClearFreeState(1);
  }
  return;
}

static void script_16B19(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_srchUtils(SRCH_SCRIPT_Reset);
  local_F4.x = -9.1394f;
  (&local_F4)->y = 0.9594f;
  (&local_F4)->z = -17.967699f;
  local_E8.x = 0;
  (&local_E8)->y = 21222;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(SHEN_NAME('HAKO'), CCOW_DisableAll);
  Script_EVT_GetPosition(SHEN_NAME('HAKO'), -1, &local_F4, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7025, 60.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(CHARA_AKIR, 0, 6, 16);
  Script_CharHandlers(CHARA_AKIR, 1, 6, 16);
  for (;;) {
    if (local_E4 == 120) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    SCNF_WaitFrames_0000(1);
    local_E0 = local_E4;
    if (local_E0 == 94) {
      Script_fixoAttachEx(SHEN_NAME('HAKO'), CHARA_AKIR, 12, &data_25200, &data_2520C);
      goto L016C67;
    }
    L016C67:;
  }
  Script_fixoClearAttachment(SHEN_NAME('HAKO'));
  return;
}

static int32_t script_16C74(void) {
  SCNF_StartIconSystemMenu(36);
  return Script_srchControl(0);
}

static void script_16C85(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  local_F4.x = -6.7998f;
  (&local_F4)->y = 0.96f;
  (&local_F4)->z = -17.5439f;
  local_E8.x = 0;
  (&local_E8)->y = 0x8000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  local_F4.x = -6.7998f;
  (&local_F4)->y = 1.06f;
  (&local_F4)->z = -16.9373f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(SHEN_NAME('HAKO'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('HAKO'), 0x38000000, &local_E8, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_228 = 1;
  Script_PlayCharaMotion(CHARA_AKIR, 7051, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (local_E4 > 760) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_E0 = local_E4;
    switch (local_E0) {
      case 1:
        Script_seqEnterEvtCam(711, 0, AUTH_None);
        break;
      case 90:
        local_F4.x = -6.7998f;
        (&local_F4)->y = 3.484f;
        (&local_F4)->z = -16.58f;
        Script_EVT_SetPosition(SHEN_NAME('GART'), POS_XYZ, &local_F4);
        Script_seqEnterEvtCam(712, 0, AUTH_None);
        break;
      case 100:
        Script_CharHandlers(CHARA_AKIR, 0, 5, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 5, 16);
        break;
      case 120:
        Script_tmnmUtils(SHEN_NAME('GART'), 7055, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_executeAicaCommand(0x1D05A9, 0, 0);
        break;
      case 130:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 14, 16);
        break;
      case 170:
        Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Show);
        break;
      case 180:
        Script_seqEnterEvtCam(713, 0, AUTH_None);
        Script_ShowUIMessageFromID("E1011A006");
        break;
      case 240:
        Script_seqEnterEvtCam(714, 0, AUTH_None);
        break;
      case 265:
        Script_executeAicaCommand(0x3205A9, 0, 0);
        break;
      case 300:
        Script_CharHandlers(CHARA_AKIR, 0, 6, 16);
        break;
      case 350:
        local_F4.x = 0.10222f;
        (&local_F4)->y = 0.01707f;
        (&local_F4)->z = -0.00826f;
        local_E8.x = 21407;
        (&local_E8)->y = 253;
        (&local_E8)->z = 16797;
        Script_fixoAttachEx(SHEN_NAME('MKCS'), CHARA_AKIR, 18, &local_F4, &local_E8);
        break;
      case 360:
        Script_seqEnterEvtCam(715, 0, AUTH_None);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 548, 1);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 903, 1);
        Script_scenCreateTask(script_16C74, 0);
        break;
      case 390:
        Script_ShowUIMessageFromID("E1011A102");
        break;
      case 480:
        Script_fixoClearAttachment(SHEN_NAME('MKCS'));
        Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Hide);
        break;
      case 500:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 14, 16);
        break;
      case 520:
        Script_seqEnterEvtCam(716, 0, AUTH_None);
        break;
      case 550:
        Script_CharHandlers(CHARA_AKIR, 0, 5, 16);
        break;
      case 560:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        Script_tmnmUtils(SHEN_NAME('GART'), 7056, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 645:
        Script_executeAicaCommand(0x1E05A9, 0, 0);
        Script_CharHandlers(CHARA_AKIR, 0, 6, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 6, 16);
        Script_tmnmReset(SHEN_NAME('GART'));
        break;
      case 646:
        local_F4.x = -6.7998f;
        (&local_F4)->y = 3.484f;
        (&local_F4)->z = -16.5422f;
        local_E8.x = 0;
        (&local_E8)->y = 0x8000;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(SHEN_NAME('GART'), POS_XYZ, &local_F4);
        Script_EVT_SetRotation(SHEN_NAME('GART'), 0x38000000, &local_E8, 0);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_SetGameStateTo(AUTH_None);
  Script_convSetCamInactiveFlag(1);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(SHEN_NAME('HAKO'), CCOW_EnableAll);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  task_228 = 0;
  return;
}

static int32_t script_171EE(void) {
  Script_scenCreateTask(script_0030, 6, CHARA_AKIR, 2, 0, &data_2522C, 2, 0);
  script_2643((int32_t)data_25260);
  task_24[0] = (int32_t)script_17C10;
  task_24[1] = (int32_t)script_17871;
  task_24[2] = (int32_t)script_17F1C;
  return Script_srchControl(0);
}

static void script_17244(void) {
  Script_scenCreateTask(script_176E5, 0);
  Script_scenCreateTask(script_171EE, 0);
  return;
}

static int32_t script_1725D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = arg3;
  if (local_FC == 1) goto L017271;
  L017271:;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenRequestGameModuleTransition(arg0, arg1, arg2);
  end:;
}

static int32_t script_172AD(void) {
  int8_t local_FF;
  Vector3i local_F0;
  task_28C.x = 0.0f;
  task_28C.z = 13.4303f;
  task_298.x = -2.0f;
  task_298.z = 9.56f;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F0, 0);
  if (Script_SetTaskDataValue(CHARA_AKIR, &task_28C, &task_298, 0) & -((&local_F0)->y > 10922) & -((&local_F0)->y < 54613)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A0160_FUKU_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_PRE_0172_FUKU_INE_STAGE_DONE) == 0)) {
      if (Script_IsPlayerAllowed(0)) {
        if (task_231 == 0) {
          task_231 = 1;
          Script_sicnUtils(0, 4, 1);
        }
      }
    }
    local_FF = 1;
  } else {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A0160_FUKU_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_PRE_0172_FUKU_INE_STAGE_DONE) == 0)) {
      if (Script_IsPlayerAllowed(0)) {
        if (task_231) {
          task_231 = 0;
          Script_sicnUtils(2, 4, 0);
        }
      }
    }
    local_FF = 0;
  }
  end:;
}

static void script_173A6(void) {
  int8_t local_FF;
  task_28C.x = -4.25f;
  task_28C.z = -0.24f;
  task_298.x = -1.95f;
  task_298.z = 1.05f;
  if (Script_SetTaskDataValue(CHARA_AKIR, &task_28C, &task_298, 0)) {
    local_FF = 2;
  } else {
    local_FF = 0;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_252A0, 4, data_252C8)))
static void script_173FA(int32_t arg0) {
  int32_t unused_F4[3];
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  Vector3i local_CC;
  local_D8 = arg0;
  switch (local_D8) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A0160_FUKU_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_PRE_0172_FUKU_INE_STAGE_DONE) == 0)) {
        while (Script_IsPlayerAllowed(0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_sicnUtils(2, 4, 0);
        task_228 = 1;
        Script_LoadVoiceFile(1, "SA1118");
        Script_seqEnterEvtCam(task_22B + 2, 0, AUTH_None);
        task_22B = task_22B + 1;
        if (task_22B > 2) {
          task_22B = 0;
        }
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
        local_E8.x = -1.4307f;
        (&local_E8)->y = 0.1f;
        (&local_E8)->z = 10.289f;
        local_DC.x = 0;
        (&local_DC)->y = 0x8000;
        (&local_DC)->z = 0;
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_E8);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_DC, 0);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_D8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        switch (local_D8) {
          case 0:
            Script_ShowUIMessageFromID("SA1118A001");
            break;
          case 1:
            Script_ShowUIMessageFromID("SA1118A002");
            break;
          case 2:
            Script_ShowUIMessageFromID("SA1118A003");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
        Script_charaToggleMotEnabled(CHARA_AKIR, 0);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TURN180_WAD_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        for (;;) {
          if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
            break;
          }
          Script_RotateNodeUtils(CHARA_AKIR, &data_20CCC, 0);
          SCNF_WaitFrames_0000(1);
        }
        Script_charaToggleMotEnabled(CHARA_AKIR, 1);
        (&local_CC)->y = 0;
        Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_CC, 0);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(40);
        Script_SetGameStateTo(AUTH_None);
        Script_convSetCamInactiveFlag(1);
        Script_ryoControl(RYO_RestoreControl);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
        Script_LoadVoiceFile(0, "SA1118");
        task_228 = 0;
      }
      break;
    case 2:
      script_18186();
      script_1725D(2, SHEN_NAME('JOMO'), 0, 0);
      SCNF_WaitFrames_0000(1);
      break;
  }
  return;
}

static int32_t script_176A7(int32_t arg0, int32_t arg1) {
  if (arg0) {
    (void)arg0;
  } else {
    (void)arg1;
  }
  end:;
}

static int32_t script_176B9(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_176A7(arg0, arg1);
  if (local_FC == 1) {
    arg0 = script_172AD();
    goto L0176E1;
  }
  L0176E1:;
  return arg0;
}

static int32_t script_176E5(void) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  local_FE = 1;
  local_FF = 0;
  local_FF = script_176B9(local_FF, local_FE);
  for (;;) {
    if (task_228 == 0) {
      local_F8 = local_FF;
      local_FF = script_176B9(local_FF, local_FE);
      if (local_FE == 6) {
        local_FE = 1;
      } else {
        local_FE = local_FE + 1;
      }
      if (local_FF) {
        if (Script_GetControllerInput(1) & 0x400) {
          script_173FA(local_FF);
        }
      }
      if ((&task_6C)->mode) {
        local_F8 = task_6C.entry_idx;
        switch (local_F8) {
          case 0:
            Script_ryoControl(PCCTL_DisablePlayerControl);
            SCNF_WaitFrames_0000(90);
            Script_executeAicaCommand(2720, 1, 30);
            Script_executeAicaCommand(2720, 2, 30);
            Script_executeAicaCommand(2720, 3, 30);
            Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
            while (Script_filmCheckOrSetTransitionUtil(-1)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_shenRequestGameModuleTransition(2, SHEN_NAME('JU00'), 1);
            SCNF_WaitFrames_0000(1);
            break;
          case 1:
            Script_ryoControl(PCCTL_DisablePlayerControl);
            SCNF_WaitFrames_0000(90);
            Script_executeAicaCommand(2720, 1, 30);
            Script_executeAicaCommand(2720, 2, 30);
            Script_executeAicaCommand(2720, 3, 30);
            Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
            while (Script_filmCheckOrSetTransitionUtil(-1)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 0);
            SCNF_WaitFrames_0000(1);
            break;
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_252CF)))
static int32_t script_17871(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t unused_CC;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_seqEnterEvtCam(307, 0, AUTH_None);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->y = (&local_F4)->y - 0.2f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_D0, 0);
  (&local_D0)->y = 0xC000;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D0, 0);
  Script_MoveObjectToObject(SHEN_NAME('LKUT'), 0x52494B41);
  local_DC.x = -0.2f;
  (&local_DC)->y = -0.19f;
  (&local_DC)->z = -0.3f;
  Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_DC);
  Script_EVT_GetPosition(SHEN_NAME('LKUT'), -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  local_D0.x = 0;
  (&local_D0)->y = 0;
  (&local_D0)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_D0, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('LKUT'), &local_D0, 0x40000000);
  Script_MoveObjectToObject(SHEN_NAME('LKUT'), -1);
  Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_DC);
  Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_D0, 0);
  Script_MoveObjectToObject(SHEN_NAME('RKUT'), 0x52494B41);
  local_DC.x = 0.0f;
  (&local_DC)->y = -0.19f;
  (&local_DC)->z = -0.3f;
  Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_DC);
  Script_EVT_GetPosition(SHEN_NAME('RKUT'), -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  local_D0.x = 0;
  (&local_D0)->y = 0;
  (&local_D0)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_D0, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('RKUT'), &local_D0, 0x40000000);
  Script_MoveObjectToObject(SHEN_NAME('RKUT'), -1);
  Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_DC);
  Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_D0, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, 0, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKB_M", 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_HAKU_KUTU_225D_F, 20.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(36);
  Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Hide);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_10949();
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  local_DC.x = 1.1237f;
  (&local_DC)->y = 0.1f;
  (&local_DC)->z = -22.45f;
  local_D0.x = 0;
  (&local_D0)->y = 0xC000;
  (&local_D0)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_DC);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(2);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_4C = 0) & 1);
  Script_cpctStateUtils(1, 0);
  end:;
}

static int32_t script_17C10(void) {
  (void)((task_233 = 0) & 1);
  if (1) {
    task_C = *&task_C | 8;
  } else {
    task_C = *&task_C & -9;
  }
  Script_seqEnterEvtCam(305, 0, AUTH_None);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  if (task_23C == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x554B5546, 1);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
    (&local_E8)->y = 0x4000;
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Show);
  Script_MoveObjectToObject(SHEN_NAME('LKUT'), 0x52494B41);
  {
    Vector3 local_F4;
    local_F4.x = -0.1f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.06f;
    Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_F4);
  }
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(SHEN_NAME('LKUT'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    {
      Vector3i local_E8;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_E8, 0);
    }
    {
      Vector3i local_E8;
      Script_mtwkGetSetRotation(SHEN_NAME('LKUT'), &local_E8, 0x40000000);
      Script_MoveObjectToObject(SHEN_NAME('LKUT'), -1);
      Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_F4);
      Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_E8, 0);
    }
  }
  Script_MoveObjectToObject(SHEN_NAME('RKUT'), 0x52494B41);
  {
    Vector3 local_F4;
    local_F4.x = 0.1f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.06f;
    Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_F4);
  }
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(SHEN_NAME('RKUT'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    {
      Vector3i local_E8;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_E8, 0);
    }
    {
      Vector3i local_E8;
      Script_mtwkGetSetRotation(SHEN_NAME('RKUT'), &local_E8, 0x40000000);
      Script_MoveObjectToObject(SHEN_NAME('RKUT'), -1);
      Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_F4);
      Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_E8, 0);
    }
  }
  Script_CreateNPCImageModule(CHARA_AKIR, data_252D5, 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_NUGU_KUTU_225D_F, 20.0f, -1.0f, -1.0f, 1.0f, 0);
  while ((int32_t)Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) < 40) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_4C = 0) & 2);
  Script_cpctStateUtils(1, 2);
  end:;
}

static int32_t script_17ED3(void) {
  SCNF_WaitFrames_0000(45);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  SCNF_WaitFrames_0000(30);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  return Script_srchControl(0);
}

static int32_t script_17F1C(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = -1.7591f;
  (&local_F4)->y = 0.9997f;
  (&local_F4)->z = -22.4f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0x4000;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_seqEnterEvtCam(306, 0, AUTH_None);
  Script_scenCreateTask(script_17ED3, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_CharHandlers(CHARA_AKIR, 0, 2, 16);
  Script_PlayCharaMotion(CHARA_AKIR, 7021, -1.0f, -1.0f, 32.0f, 2.0f, 0);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  if (0) {
    task_C = *&task_C | 8;
  } else {
    task_C = *&task_C & -9;
  }
  script_10911();
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_4C = 0) & 0x40);
  if (task_23C == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x554B5546, 0);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  }
  end:;
}

static void script_18090(void) {
  Vector3 local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  local_F4.x = -1.45f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 10.25f;
  while (Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, 0x8000, MN_AKI_AKI_PUSH_DOOR, 0) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_18113, 0);
  SCNF_WaitFrames_0000(40);
  Script_scenCreateTask(script_1725D, 4, 1, SHEN_NAME('JU00'), 1, 0);
  return;
}

static int32_t script_18113(void) {
  SCNF_WaitFrames_0000(40);
  for (;;) {
    {
      Vector3i local_F4;
      Script_mtwkGetSetRotation(SHEN_NAME('MONB'), &local_F4, 0);
      (&local_F4)->y = (&local_F4)->y - 546;
      Script_EVT_SetRotation(SHEN_NAME('MONB'), 0x38000000, &local_F4, 0);
    }
    {
      Vector3i local_F4;
      Script_mtwkGetSetRotation(SHEN_NAME('MONB'), &local_F4, 0);
      if (((&local_F4)->y >= 46421) & ((&local_F4)->y <= 47331)) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_18186(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowSetHeightMode(CHARA_AKIR, CCOW_KeepHeight);
  {
    Vector3 local_F4;
    local_F4.x = -3.48f;
    (&local_F4)->z = 0.263f;
    Script_charaToggleMotEnabled(CHARA_AKIR, 0);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_252F0, 0.0f, 0, 0);
  }
  Script_LineWalkUtils(LNWK_SCRIPT_PATCH_PATH_NODE, &data_25300, 0, -0x3FA147AE, 0x3E86A7F0, -45993, -45993);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_25300, 0, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.06f) == -1) {
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_DOWN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    }
  }
  {
    Vector3 local_F4;
    local_F4.x = -3.48f;
    (&local_F4)->z = -0.76f;
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_252F0, 0.0f, 0, 0);
  }
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_OPEN_HUSUMA_R, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static int32_t script_182D2(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_charaDisplay(arg0, CDM_Hide);
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    if (task_228 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMapControlTaskFlag(arg0, 1);
  return Script_srchControl(0);
}

static void script_1833B(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  while (task_228) {
    Script_PrintDebugString(1, 23, data_25314, local_FE);
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(2010, 0, AUTH_None);
        break;
      case 160:
        Script_seqEnterEvtCam(2011, 0, AUTH_None);
        break;
      case 300:
        Script_seqEnterEvtCam(2012, 0, AUTH_None);
        break;
      case 600:
        Script_seqEnterEvtCam(2013, 0, AUTH_None);
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

__attribute__((scn_string_bindings(1, data_25359)))
static int32_t script_183D9(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3i local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t unused_D0;
  GameDateTime local_C8;
  int32_t local_C4;
  int32_t local_C0;
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  local_D4 = 0;
  local_D8 = Script_motiLoad(2, SHEN_NAME('JHD0'), "M_01_FUK.BIN");
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_charaSetRefboardFlagTo(CHARA_FUKU, 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('TUBA'), 1);
  Script_SetCharaRefBoardPos(SHEN_NAME('TUBA'), &data_2533C);
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  local_F4.x = -0.84f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 1.82f;
  local_DC.x = 0;
  (&local_DC)->y = 0x8000;
  (&local_DC)->z = 0;
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_DC, 0);
  Script_PlayCharaMotion(CHARA_FUKU, 4104, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(CHARA_FUKU, 0, 0, 16);
  Script_CharHandlers(CHARA_FUKU, 1, 0, 16);
  SCNF_WaitFrames_0000(1);
  local_F4.x = -0.449f;
  (&local_F4)->y = -1.177f;
  (&local_F4)->z = -0.318f;
  local_DC.x = 2365;
  (&local_DC)->y = 0;
  (&local_DC)->z = 61350;
  Script_fixoAttachEx(SHEN_NAME('HOKI'), CHARA_FUKU, 18, &local_F4, &local_DC);
  Script_charaToggleMotEnabled(CHARA_FUKU, 1);
  Script_FaceLookAtTargetUtils(CHARA_FUKU, FACE_LookReset);
  Script_AddCharacterHandler(3, "BROOM", SHEN_NAME('DMY0'));
  Script_scenCreateTask(script_182D2, 1, SHEN_NAME('DMY0'));
  local_F4.x = -0.84f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 4.3852f;
  local_DC.x = 0;
  (&local_DC)->y = 0;
  (&local_DC)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_DC, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  Script_facemotUtils(CHARA_FUKU, 1, 0);
  Script_ResetHandMotion(CHARA_FUKU, 1, 1, 6);
  Script_ResetHandMotion(CHARA_FUKU, 1, 0, 6);
  for (;;) {
    local_C0 = local_D4;
    switch (local_C0) {
      case 0:
        Script_seqEnterEvtCam(2020, 0, AUTH_None);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AYUMIYORU_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_executeAicaCommand(267435, 0, 0);
        break;
      case 50:
        Script_executeAicaCommand(267435, 0, 0);
        break;
      case 60:
        Script_ShowUIMessageFromID("SA1037A001");
        Script_PlayCharaMotion(CHARA_FUKU, MN_OTH_FUK_TEOTOMEMIRU_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 80:
        Script_executeAicaCommand(267435, 0, 0);
        break;
      case 110:
        Script_seqEnterEvtCam(2021, 0, AUTH_None);
        Script_ShowUIMessageFromID("SA1037B001");
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_TUBATORIDASU_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 150:
        Script_seqEnterEvtCam(2022, 0, AUTH_None);
        Script_facemotUtils(CHARA_AKIR, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 43);
        local_F4.x = 0.115f;
        (&local_F4)->y = 0.016f;
        (&local_F4)->z = -0.091f;
        local_DC.x = 1635;
        (&local_DC)->y = 362;
        (&local_DC)->z = 58983;
        Script_fixoAttachEx(SHEN_NAME('TUBA'), CHARA_AKIR, 18, &local_F4, &local_DC);
        Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Show);
        local_F4.x = -0.424f;
        (&local_F4)->y = -1.177f;
        (&local_F4)->z = -0.318f;
        local_DC.x = 2365;
        (&local_DC)->y = 0;
        (&local_DC)->z = 61350;
        Script_fixoAttachEx(SHEN_NAME('HOKI'), CHARA_FUKU, 18, &local_F4, &local_DC);
        Script_ShowUIMessageFromID("SA1037A002");
        break;
      case 190:
        Script_PlayCharaMotion(CHARA_FUKU, MN_OTH_FUK_KAOTIKAZUKERU_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 240:
        Script_seqEnterEvtCam(2023, 0, AUTH_None);
        break;
      case 260:
        Script_ShowUIMessageFromID("SA1037B002");
        break;
      case 310:
        Script_PlayCharaMotion(CHARA_FUKU, MN_OTH_FUK_KAOAGERU_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 350:
        Script_seqEnterEvtCam(2024, 0, AUTH_None);
        Script_ShowUIMessageFromID("SA1037A003");
        break;
      case 380:
        Script_PlayCharaMotion(CHARA_FUKU, MN_OTH_FUK_KAOSORASU_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 500:
        Script_ShowUIMessageFromID("SA1037B003");
        break;
      case 620:
        Script_seqEnterEvtCam(2025, 0, AUTH_None);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_YUBISASI_MODOSU_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_ShowUIMessageFromID("SA1037A004");
        break;
      case 640:
        Script_PlayCharaMotion(CHARA_FUKU, MN_OTH_FUK_KAO_NOZOKI_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 700:
        Script_CharHandlers(CHARA_AKIR, 1, 8, 16);
        break;
      case 710:
        Script_PlayCharaMotion(CHARA_FUKU, MN_OTH_FUK_NOZOKI_LP_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 800:
        Script_seqEnterEvtCam(2026, 0, AUTH_None);
        Script_PlayCharaMotion(CHARA_FUKU, MN_OTH_FUK_KAOAGERU_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_ShowUIMessageFromID("SA1037B004");
        break;
      case 920:
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_KUBIYOKOFURI_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 940:
        Script_ShowUIMessageFromID("SA1037A005");
        break;
      case 965:
        Script_PlayCharaMotion(CHARA_FUKU, MN_OTH_FUK_HOUKIKATATEMOTI_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 995:
        Script_seqEnterEvtCam(2027, 0, AUTH_None);
        Script_ShowUIMessageFromID("SA1037B005");
        Script_ResetHandMotion(CHARA_FUKU, 0, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 0, 0);
        break;
      case 1025:
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_TUBASIMAU_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 1055:
        Script_ShowUIMessageFromID("SA1037A006");
        break;
      case 1095:
        Script_fixoClearAttachment(SHEN_NAME('TUBA'));
        Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ShowUIMessageFromID("SA1037B006");
        break;
      case 1165:
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_KUBITATEHURI_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 1185:
        Script_ShowUIMessageFromID("SA1037A007");
        Script_PlayCharaMotion(CHARA_FUKU, MN_OTH_FUK_HOUKIRYOUTEMOTI_TUBA_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 1215:
        Script_PlayCharaMotion(CHARA_FUKU, 4104, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        goto L018BBF;
        break;
    }
    local_D4 = local_D4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L018BBF:;
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  SCNF_WaitFrames_0000(30);
  Script_ReallocateUNFO(local_D8);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 2);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1037_SEQUENCE_COMPLETE, 1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  task_228 = 0;
  Script_ReturnTimeDate(&local_C8);
  local_C4 = (&local_C8)->hours * 60 + (&local_C8)->minutes;
  if ((local_C4 >= 480) & (local_C4 < 1410)) {
    Script_SetShouldSleepFlag(1);
    Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 0);
    SCNF_WaitFrames_0000(1);
  } else {
    Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_18C9C(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = script_D9B1();
    if (local_FC == 130) {
      if (script_D996() == 0x554B5546) {
        script_D989();
      }
    } else if (local_FC == 134) {
      if (script_D996() == 0x554B5546) {
        Script_scenCreateTask(script_19BA8, 0);
        script_D989();
      }
    } else {
      script_D989();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_18D08(void) {
  Script_scenCreateTask(script_18C9C, 0);
  return;
}

static void script_18D16(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_scenToggleActiveFor(SHEN_NAME('WIS1'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('WIS2'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('WIS3'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('WIS4'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('WIS5'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('WIS6'), 0);
  while (1) {
    if (task_228 == 0) {
      Script_ReturnTimeDate(&local_F4);
      if (((&local_F4)->hours < 9) | ((&local_F4)->hours >= 19)) {
        Script_charaDisplay(SHEN_NAME('WIS1'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('WIS2'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('WIS3'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('WIS4'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('WIS5'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('WIS6'), CDM_Hide);
      } else {
        Script_charaDisplay(SHEN_NAME('WIS1'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('WIS2'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('WIS3'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('WIS4'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('WIS5'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('WIS6'), CDM_Show);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_253FB, 2, data_25406, 3, data_25416, 4, data_2541B, 5, data_25420, 6, data_25421, 7, data_25426, 8, data_25427, 12, data_2544A, 13, data_2545A, 14, data_2545F, 15, data_25464, 16, data_25465, 17, data_2546A, 18, data_25477)))
static int32_t script_18E20(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  local_F4.x = -1.5f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -5.0f;
  local_E8.x = 12.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = -30.0f;
  while (Script_SetTaskDataValue(CHARA_AKIR, &local_F4, &local_E8, 0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_IsPlayerAllowed(0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  script_1A92D();
  task_228 = 1;
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SORA_SCENE_STARTED, 1);
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(0, 4, "a1_fukay.snd", 0);
  Script_LoadSND(0, 2, "bgm085.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 102);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "SORA") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "SORA", "");
  Script_AuthorUtils(0, 1, "SORA", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_DDBB();
  Script_ryoControl(PCCTL_DisablePlayerControl);
  if (Script_shenSeqUtils(-7)) {
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "SORA");
  task_22A = 1;
  Script_LoadSND(0, 2, "bgm077.snd", 0);
  Script_LoadSND(0, 4, "a1_ineko.snd", 0);
  Script_LoadVoiceFile(1, "01MIR");
  Script_executeAicaCommand(20392, 0, 0);
  Script_executeAicaCommand(1184, 2, 106);
  if (0) {
    Script_charaDisplay(CHARA_INE_, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_INE_, CDM_Show);
      goto L0190F1;
    }
  }
  L0190F1:;
  Script_scenToggleVisibleFor(CHARA_INE_, 1);
  Script_scenToggleActiveFor(CHARA_INE_, 1);
  Script_SetCharaFlags(CHARA_INE_, 4);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "INEK") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "INEK", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_mapmSetRenderFlag(0, 0);
  if (-1) {
    Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_FUKU, CDM_Show);
      goto L0191AC;
    }
  }
  L0191AC:;
  Script_scenToggleVisibleFor(CHARA_FUKU, 0);
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_FEDE();
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  SCNF_WaitFrames_0000(30);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_INE_, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "INEK");
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_AKIR_INE_DUO_SCENE_COMPLETE, 1);
  Script_LoadSND(0, 4, "f1doujou.snd", 0);
  Script_LoadSND(0, 2, "bgm009.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_charaToggleMotEnabled(CHARA_FUKU, 1);
  Script_SetGameStateTo(AUTH_None);
  Script_convSetCamInactiveFlag(1);
  Script_ryoControl(RYO_RestoreControl);
  script_1B16E();
  Script_mapmSetRenderFlag(0, 1);
  if (0) {
    Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_FUKU, CDM_Show);
      goto L0192FE;
    }
  }
  L0192FE:;
  Script_scenToggleVisibleFor(CHARA_FUKU, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  task_228 = 0;
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_shenClearFreeState(1);
  script_10750();
  end:;
}

__attribute__((scn_string_bindings(0, data_25720, 2, data_25739, 4, data_25753)))
static void script_19347(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 935) == 0)) {
  } else {
    task_314 = Script_MT5Loader_TRWV_TRWAVE(0, &data_25714, "model/object", "trlake1.pvr", "model/object", "trsea025.mt6", 0.25f, 0x1000);
    Script_MT5Loader_TRWV_TRWAVE(17, task_314, "model/object", data_25760);
    Script_MT5Loader_TRWV_TRWAVE(13, task_314, 0.0196078438f, 0.494117647f);
    Script_MT5Loader_TRWV_TRWAVE(14, task_314, -0.0196078438f, -0.0431372561f);
    Script_MT5Loader_TRWV_TRWAVE(16, task_314, 0, 0);
    Script_MT5Loader_TRWV_TRWAVE(21, task_314, 2, 2);
    Script_MT5Loader_TRWV_TRWAVE(24, task_314, 255, 169, 190, 125);
    Script_MT5Loader_TRWV_TRWAVE(10, task_314, 4.0f, 4.0f, 0, 1, 0, 1);
    Script_scenCreateTask(script_19616, 0);
  }
  script_19437((int32_t)&data_25484);
  return;
}

static void script_19437(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = 2.0f;
    (&local_F4)->y = 2.0f;
    (&local_F4)->z = 2.0f;
    Script_hmdlApplyScale(*(int32_t *)arg0, &local_F4);
  }
  Script_scenCreateTask(script_1959C, 3, *(int32_t *)arg0, &data_254F0, 0x3C03126F);
  {
    Vector3 local_F4;
    local_F4.x = 2.0f;
    (&local_F4)->y = 2.0f;
    (&local_F4)->z = 2.0f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 4), &local_F4);
  }
  Script_scenCreateTask(script_1959C, 3, *(int32_t *)(arg0 + 4), &data_25564, 0x3BA3D70A);
  {
    Vector3 local_F4;
    local_F4.x = 2.3f;
    (&local_F4)->y = 2.3f;
    (&local_F4)->z = 2.3f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 8), &local_F4);
  }
  Script_scenCreateTask(script_1959C, 3, *(int32_t *)(arg0 + 8), &data_255A8, 0x3BE56042);
  {
    Vector3 local_F4;
    local_F4.x = 2.0f;
    (&local_F4)->y = 2.0f;
    (&local_F4)->z = 2.0f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 12), &local_F4);
  }
  Script_scenCreateTask(script_1959C, 3, *(int32_t *)(arg0 + 12), &data_2565C, 0x3BC49BA6);
  {
    Vector3 local_F4;
    local_F4.x = 1.5f;
    (&local_F4)->y = 1.5f;
    (&local_F4)->z = 1.5f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 16), &local_F4);
  }
  Script_scenCreateTask(script_1959C, 3, *(int32_t *)(arg0 + 16), &data_256B0, 0x3C23D70A);
  return;
}

static int32_t script_1959C(int32_t arg0, const void *arg1, float arg2) {
  Script_tmnmUtils(arg0, 60426, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(arg0, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, arg1, 0x10000, 72);
  while (1) {
    if (task_228 == 0) {
      Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, arg2);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_19616(void) {
  script_19648((int32_t)&data_25498);
  while (1) {
    script_197E0((int32_t)&data_25498);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_19648(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = 1.2f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.2f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 12), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 0.8f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 0.8f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 20), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 1.2f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.2f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 24), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 1.6f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.6f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 28), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 0.6f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 0.6f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 32), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 1.3f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.3f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 36), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 0.7f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 0.7f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 40), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 0.9f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 0.9f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 48), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 1.2f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.2f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 52), &local_F4);
  }
  return;
}

static void script_197E0(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  local_FC = 0;
  while (*(int32_t *)(arg0 + (local_FC << 2)) != -1) {
    Script_EVT_GetPosition(*(int32_t *)(arg0 + (local_FC << 2)), -1, &local_F0, 0);
    local_E4.x = (&local_F0)->x - (&data_25714)->x;
    (&local_E4)->y = (&local_F0)->y;
    (&local_E4)->z = (&local_F0)->z - (&data_25714)->z;
    *(int32_t *)&(&local_F0)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_314, &local_E4);
    Script_EVT_SetPosition(*(int32_t *)(arg0 + (local_FC << 2)), POS_XYZ, &local_F0);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_19872(void) {
  Vector3i local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  task_318 = 0;
  task_31A = 0;
  Script_EVT_GetPosition(SHEN_NAME('SSOD'), -1, &local_E8, 0);
  for (;;) {
    Script_mtwkGetSetRotation(SHEN_NAME('SSOD'), &local_F4, 0);
    local_E4 = task_318;
    switch (local_E4) {
      case 0:
        if (task_31A == 540) {
          task_31A = 0;
          task_318 = 1;
        }
        break;
      case 1:
        (&local_F4)->z = (&local_F4)->z - 273;
        if (task_31A == 19) {
          Script_Execute3DSoundCommand_Wrapper(263593, 4, &local_E8, 20.0f, 100.0f, 0);
        }
        if (task_31A == 39) {
          task_31A = 0;
          task_318 = 2;
        }
        break;
      case 2:
        (&local_F4)->z = (&local_F4)->z + 546;
        if (task_31A == 18) {
          Script_Execute3DSoundCommand_Wrapper(132521, 4, &local_E8, 20.0f, 100.0f, 0);
        }
        if (task_31A == 19) {
          task_31A = 0;
          task_318 = 0;
          (&local_F4)->z = 0;
        }
        break;
    }
    task_31A = task_31A + 1;
    Script_EVT_SetRotation(SHEN_NAME('SSOD'), 0x38000000, &local_F4, 0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_199AE(void) {
  int32_t local_FC;
  script_19ACA();
  local_FC = Script_scenCreateTask(script_199F0, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_tmnmReset(SHEN_NAME('KAWA'));
  return;
}

static int32_t script_199F0(void) {
  int32_t local_FC;
  int32_t unused_F0[3];
  Vector3i local_E4;
  int32_t local_E0;
  local_FC = 0;
  Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Hide);
  Script_ccowSetHeightMode(SHEN_NAME('KAWA'), CCOW_KeepHeight);
  local_E4.x = 0;
  (&local_E4)->y = 4187;
  (&local_E4)->z = 0;
  Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E4);
  for (;;) {
    local_E0 = local_FC;
    switch (local_E0) {
      case 60:
        Script_tmnmUtils(SHEN_NAME('KAWA'), 4119, 0.0f, 100.0f, 0.0f, 1.0f, 0);
        break;
      case 61:
        Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Show);
        break;
      case 110:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_25790, 10);
        break;
      case 157:
        Script_executeAicaCommand(0x3705A9, 0, 0);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_19ACA(void) {
  script_19B01(CHARA_AKIR);
  Script_SetVector4ByOffset(&data_25874);
  (void)1;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_25874);
  return;
}

static void script_19B01(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_23068, 10);
  Script_handSetFloatsFor(arg0, 1, data_23068, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return;
}

static void script_19B88(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

__attribute__((scn_string_bindings(0, data_25884, 1, data_25894, 2, data_25899, 3, data_2589E, 4, data_2589F, 5, data_258A4, 7, data_258B0, 8, data_258B6, 9, data_258BC, 16, data_258FE, 17, data_25903)))
static int32_t script_19BA8(void) {
  while (Script_IsPlayerAllowed(0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_PlayerStatsUtils(PS_SetStatU8_2, Script_PlayerStatsUtils(PS_GetStatU8_2) + 1);
  task_228 = 1;
  Script_scenToggleActiveFor(CHARA_FUKU, 1);
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_10911();
  Script_scenToggleActiveFor(CHARA_FUKU, 1);
  Script_SetCharaFlags(CHARA_FUKU, 4);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.85f;
    (&local_F4)->y = 0.1f;
    (&local_F4)->z = -22.4f;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_E8, 0);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.85f;
    (&local_F4)->y = 0.1f;
    (&local_F4)->z = -22.6f;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_E8, 0);
  }
  Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Show);
  script_107A8();
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "KAKG") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "KAKG", "");
  Script_AuthorUtils(0, 1, "KAKG", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadSND(0, 2, "bgm012.snd", 0);
  Script_executeAicaCommand(9384, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKF_M", 0);
  Script_CreateNPCImageModule(CHARA_FUKU, "FUK_M", 0);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  script_199AE();
  Script_scenToggleCharaFlag(0, 200, 1);
  script_1A92D();
  script_1A248();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 15.0f);
  script_1B16E();
  Script_scenToggleCharaFlag(0, 200, 0);
  Script_shenClearFreeState(0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -7.0f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = -20.4f;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -7.0f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = -21.6f;
    local_E8.x = 0;
    (&local_E8)->y = 0x8000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKC_M", 0);
  Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 0, 0);
  Script_FACE_LCG(CHARA_FUKU, FACE_BlinkOpen, 0, 0);
  Script_seqEnterEvtCam(1011, CHARA_AKIR, CHARA_AKIR);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  {
    int32_t unused_E8[6];
    int32_t local_E4;
    local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    switch (local_E4) {
      case 0:
        Script_ShowUIMessageFromID("E1031A008");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1031A009");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t unused_E8[6];
    int32_t local_E4;
    local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    switch (local_E4) {
      case 0:
        Script_ShowUIMessageFromID("E1031A010");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1031A011");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  Script_seqEnterEvtCam(1012, CHARA_FUKU, CHARA_FUKU);
  {
    int32_t unused_E8[6];
    int32_t local_E4;
    local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    switch (local_E4) {
      case 0:
        Script_ShowUIMessageFromID("E1031B014");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1031B015");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Hide);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
  Script_FACE_LCG(CHARA_FUKU, FACE_BlinkAutomatic, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "KAKG");
  Script_LoadSND(0, 2, "bgm009.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_ccowUtils(CHARA_FUKU, CCOW_EnableAll);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCPAMFlags(CHARA_FUKU, 1);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ryoControl(RYO_RestoreControl);
  Script_shenClearFreeState(1);
  task_228 = 0;
  script_10750();
  end:;
}

__attribute__((scn_string_bindings(1, data_25914)))
static void script_1A248(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  local_E0 = 0;
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('DDO1'), 0x38000000, &local_E8, 0);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('DDO2'), 0x38000000, &local_E8, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 15.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 12.0f);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 0);
  Script_LoadVoiceFile(1, "E1031");
  Script_motmToggleFlags(CHARA_FUKU, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKF_M", 0);
  local_E4 = Script_motiLoad(2, SHEN_NAME('JHD0'), "M_FGT1.BIN");
  Script_vfSetup(0, 20, 20, 0, 0);
  Script_vfStageObjectUtils(0, 0, 0);
  Script_vfStageObjectUtils(2, 0, 2, 2, 0x3F000000, 0x3F000000, -0x40800000, 0, 0, -0x40800000);
  Script_vfStageObjectUtils(7, 0, 0x4F4B4148, 0);
  Script_vfStageObjectUtils(0, 1, 0);
  Script_vfStageObjectUtils(2, 1, 2, 2, 0x3F800000, 0x3E800000, -0x40000000, 0, 0, -0x41000000);
  Script_vfStageObjectUtils(7, 1, 0x534B484B, 1);
  Script_vfStageObjectUtils(13);
  task_2A8 = 0x52494B41;
  *(&task_2A8 + 1) = 0;
  *(&task_2A8 + 2) = 0;
  *(&task_2A8 + 3) = 0;
  *(&task_2A8 + 4) = 400;
  *(&task_2A8 + 5) = 0;
  *(&task_2A8 + 6) = 0;
  *(&task_2A8 + 7) = 0;
  *(&task_2A8 + 8) = 0;
  *(&task_2A8 + 9) = 0;
  *(&task_2A8 + 10) = 0;
  Script_vfUtils(&task_2A8);
  task_2A8 = 0x554B5546;
  *(&task_2A8 + 1) = 0;
  *(&task_2A8 + 2) = 1;
  *(&task_2A8 + 3) = 1;
  *(&task_2A8 + 4) = 400;
  *(&task_2A8 + 5) = 57;
  *(&task_2A8 + 6) = 0;
  *(&task_2A8 + 7) = 0;
  *(&task_2A8 + 8) = 0;
  *(&task_2A8 + 9) = 0;
  *(&task_2A8 + 10) = 0;
  Script_vfUtils(&task_2A8);
  Script_vfConfigureFormationGroup(CHARA_FUKU, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  SCNF_WaitFrames_0000(1);
  local_F4.x = -7.0f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = -20.4f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  local_F4.x = -7.0f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = -21.6f;
  local_E8.x = 0;
  (&local_E8)->y = 0x8000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_E8, 0);
  Script_seqEnterEvtCam(1023, 0, AUTH_None);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_FUKU, SCEN_Enable, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_seqEnterEvtCam(1010, 0, AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_FUKU, 1);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_REI_KAMAE_JYU, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(6);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_REI_KAMAE_JYU, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    local_E0 = local_E0 + 1;
    if (local_E0 == 15) {
      local_DC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      switch (local_DC) {
        case 0:
          Script_ShowUIMessageFromID("E1031B009");
          break;
        case 1:
          Script_ShowUIMessageFromID("E1031B007");
          break;
        case 2:
          Script_ShowUIMessageFromID("E1031B008");
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_IsInEventCam() == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_vfConfigureFormationGroup(CHARA_AKIR, VF_FORMATION_APPLY_FLAGS, VF_SfxEventPending, 100);
  Script_vfConfigureFormationGroup(CHARA_FUKU, VF_FORMATION_APPLY_FLAGS, VF_SfxEventPending, 100);
  SCN_Enqueue_VFGI(2, 99, 1.0f, 2, 240);
  while (SCN_GetIsFighting() != 2) {
    if (Script_CheckTaskStatus(CHARA_AKIR) & 1) {
      SCNF_WaitFrames_0000(1);
      Script_vfToggleBattleState(scnPostResultsSetup);
      break;
    }
    if (Script_CheckTaskStatus(CHARA_FUKU) & 1) {
      SCNF_WaitFrames_0000(1);
      Script_vfToggleBattleState(scnPostResultsSetup);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  Script_executeAicaCommand(2720, 2, 30);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ReallocateUNFO(local_E4);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x80);
  Script_motmToggleFlags(CHARA_FUKU, SCEN_Disable, 0x80);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_FUKU, 1);
  Script_GetFightingStats(0x52494B41, 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  local_E8.x = 0;
  (&local_E8)->y = 17560;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('DDO1'), 0x38000000, &local_E8, 0);
  local_E8.x = 0;
  (&local_E8)->y = 46514;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('DDO2'), 0x38000000, &local_E8, 0);
  return;
}

static void script_1A8AA(void) {
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_25944, 0, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f) != -1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_1A92D(void) {
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Show);
      goto L01A957;
    }
  }
  L01A957:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Show);
      goto L01A98B;
    }
  }
  L01A98B:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Show);
      goto L01A9BF;
    }
  }
  L01A9BF:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Show);
      goto L01A9F3;
    }
  }
  L01A9F3:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Show);
      goto L01AA27;
    }
  }
  L01AA27:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Show);
      goto L01AA5B;
    }
  }
  L01AA5B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Show);
      goto L01AA8F;
    }
  }
  L01AA8F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Show);
      goto L01AAC3;
    }
  }
  L01AAC3:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Show);
      goto L01AAF7;
    }
  }
  L01AAF7:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Show);
      goto L01AB2B;
    }
  }
  L01AB2B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Show);
      goto L01AB5F;
    }
  }
  L01AB5F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS5'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Show);
      goto L01AB93;
    }
  }
  L01AB93:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS6'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Show);
      goto L01ABC7;
    }
  }
  L01ABC7:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS7'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Show);
      goto L01ABFB;
    }
  }
  L01ABFB:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS8'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Show);
      goto L01AC2F;
    }
  }
  L01AC2F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS9'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASA'), CDM_Show);
      goto L01AC63;
    }
  }
  L01AC63:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASB'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASB'), CDM_Show);
      goto L01AC97;
    }
  }
  L01AC97:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASB'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASC'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASC'), CDM_Show);
      goto L01ACCB;
    }
  }
  L01ACCB:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASC'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASD'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASD'), CDM_Show);
      goto L01ACFF;
    }
  }
  L01ACFF:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASD'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASE'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASE'), CDM_Show);
      goto L01AD33;
    }
  }
  L01AD33:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASE'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASF'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASF'), CDM_Show);
      goto L01AD67;
    }
  }
  L01AD67:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASF'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Show);
      goto L01AD9B;
    }
  }
  L01AD9B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Show);
      goto L01ADCF;
    }
  }
  L01ADCF:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Show);
      goto L01AE03;
    }
  }
  L01AE03:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Show);
      goto L01AE37;
    }
  }
  L01AE37:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Show);
      goto L01AE6B;
    }
  }
  L01AE6B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('SSOD'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('SSOD'), CDM_Show);
      goto L01AE9F;
    }
  }
  L01AE9F:;
  Script_scenToggleVisibleFor(SHEN_NAME('SSOD'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Show);
      goto L01AED3;
    }
  }
  L01AED3:;
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
      goto L01AF07;
    }
  }
  L01AF07:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
      goto L01AF3B;
    }
  }
  L01AF3B:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Show);
      goto L01AF6F;
    }
  }
  L01AF6F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HOKI'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MOON'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MOON'), CDM_Show);
      goto L01AFA3;
    }
  }
  L01AFA3:;
  Script_scenToggleVisibleFor(SHEN_NAME('MOON'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L01AFD7;
    }
  }
  L01AFD7:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L01B00B;
    }
  }
  L01B00B:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L01B03F;
    }
  }
  L01B03F:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L01B073;
    }
  }
  L01B073:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
      goto L01B0A7;
    }
  }
  L01B0A7:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
      goto L01B0DB;
    }
  }
  L01B0DB:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52554B59)) {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
        goto L01B11E;
      }
    }
    L01B11E:;
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4C554B59)) {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Show);
        goto L01B161;
      }
    }
    L01B161:;
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  }
  return;
}

static void script_1B16E(void) {
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Show);
      goto L01B198;
    }
  }
  L01B198:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Show);
      goto L01B1CC;
    }
  }
  L01B1CC:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Show);
      goto L01B200;
    }
  }
  L01B200:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Show);
      goto L01B234;
    }
  }
  L01B234:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Show);
      goto L01B268;
    }
  }
  L01B268:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Show);
      goto L01B29C;
    }
  }
  L01B29C:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Show);
      goto L01B2D0;
    }
  }
  L01B2D0:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Show);
      goto L01B304;
    }
  }
  L01B304:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Show);
      goto L01B338;
    }
  }
  L01B338:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Show);
      goto L01B36C;
    }
  }
  L01B36C:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Show);
      goto L01B3A0;
    }
  }
  L01B3A0:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS5'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Show);
      goto L01B3D4;
    }
  }
  L01B3D4:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS6'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Show);
      goto L01B408;
    }
  }
  L01B408:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS7'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Show);
      goto L01B43C;
    }
  }
  L01B43C:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS8'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Show);
      goto L01B470;
    }
  }
  L01B470:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS9'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASA'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASA'), CDM_Show);
      goto L01B4A4;
    }
  }
  L01B4A4:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASA'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASB'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASB'), CDM_Show);
      goto L01B4D8;
    }
  }
  L01B4D8:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASB'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASC'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASC'), CDM_Show);
      goto L01B50C;
    }
  }
  L01B50C:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASC'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASD'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASD'), CDM_Show);
      goto L01B540;
    }
  }
  L01B540:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASD'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASE'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASE'), CDM_Show);
      goto L01B574;
    }
  }
  L01B574:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASE'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASF'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASF'), CDM_Show);
      goto L01B5A8;
    }
  }
  L01B5A8:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASF'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Show);
      goto L01B5DC;
    }
  }
  L01B5DC:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Show);
      goto L01B610;
    }
  }
  L01B610:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Show);
      goto L01B644;
    }
  }
  L01B644:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Show);
      goto L01B678;
    }
  }
  L01B678:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Show);
      goto L01B6AC;
    }
  }
  L01B6AC:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('SSOD'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('SSOD'), CDM_Show);
      goto L01B6E0;
    }
  }
  L01B6E0:;
  Script_scenToggleVisibleFor(SHEN_NAME('SSOD'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
        goto L01B72C;
      }
    }
    L01B72C:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
        goto L01B77B;
      }
    }
    L01B77B:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 1);
  }
  Script_scenToggleVisibleFor(SHEN_NAME('HOKI'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L01B7CA;
    }
  }
  L01B7CA:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L01B7FE;
    }
  }
  L01B7FE:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L01B832;
    }
  }
  L01B832:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L01B866;
    }
  }
  L01B866:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 1);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52554B59)) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4C554B59)) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  }
  return;
}

static int32_t script_1B8C1(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetFightingStats(-1, 2) != 1) {
      local_FC = local_FC + 1;
    }
  }
  local_F8 = Script_PlayerStatsUtils(PS_GetShugyoCourseActiveFramesSlot, 0);
  local_F8 = local_F8 + local_FC;
  return Script_PlayerStatsUtils(PS_SetShugyoCourseActiveFramesSlot, 0, local_F8);
}

__attribute__((scn_string_bindings(0, data_25958, 3, data_25972, 7, data_25996)))
static int32_t script_1B91C(void) {
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
      goto L01BA1C;
    }
  }
  L01BA1C:;
  if (local_F4) {
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadVoiceFile(0, "E1040");
  }
  if ((local_FC >= 1501) & (local_FC <= 1601)) {
    local_F4 = 1;
  }
  if ((local_F8 >= 2200) | (local_F8 <= 829)) {
    local_F4 = 1;
  }
  return local_F4;
}

static int32_t script_1BA75(void) {
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

static void script_1BB1B(void) {
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

static void script_1BB74(void) {
  Script_seqEnterEvtCam(2900, CHARA_AKIR, CHARA_AKIR);
  Script_shenClearFreeState(0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsInEventCam() == 0) {
      break;
    }
  }
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_1BBAE(int32_t arg0) {
  Script_EVT_SetPosition(arg0, 0x78000000, &data_2599C);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_ccowUtils(arg0, CCOW_DisableAll);
}

static void script_1BBD1(void) {
  SCNF_WaitFrames_0000(2);
  while (Script_IsInEventCam() != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1BBF1(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 5, "e1shugyo.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static void script_1BC3C(void) {
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

static void script_1BC83(void) {
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

static void script_1BCD6(void) {
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

static void script_1BD32(void) {
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

static void script_1BD79(void) {
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

__attribute__((scn_string_bindings(0, data_25A2D)))
static void script_1BDD5(void) {
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_RequestNewMode, 5);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_LoadVoiceFile(1, "E1040");
  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_1BBAE, 1, SHEN_NAME('LUV2'));
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_scenCreateTask(script_1BBF1, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_25A33)))
static void script_1BE5D(void) {
  Script_vfToggleBattleState(scnPostResultsSetup);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadVoiceFile(0, "E1040");
  SCNF_WaitFrames_0000(1);
  while (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE, 0) != 7) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  return;
}

static void script_1BED5(void) {
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

static void script_1BF12(void) {
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

static int32_t script_1BF4F(int32_t arg0) {
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

static void script_1BFC0(int32_t arg0) {
  Vector3i local_F4;
  Script_uiUtils(0, 15, 0);
  Script_vfSetup(0, 20, 20, 0, 0);
  Script_vfStageObjectUtils(0, 0, 0);
  Script_vfStageObjectUtils(2, 0, 2, 2, 0x3F000000, 0x3F000000, -0x40800000, 0, 0, -0x40800000);
  Script_vfStageObjectUtils(7, 0, 0x4F4B4148, 0);
  Script_vfStageObjectUtils(0, 1, 0);
  Script_vfStageObjectUtils(2, 1, 2, 2, 0x3F800000, 0x3E800000, -0x40000000, 0, 0, -0x41000000);
  Script_vfStageObjectUtils(7, 1, 0x534B484B, 1);
  Script_vfStageObjectUtils(13);
  task_2A8 = 0x52494B41;
  *(&task_2A8 + 1) = 0;
  *(&task_2A8 + 2) = 0;
  *(&task_2A8 + 3) = 0;
  *(&task_2A8 + 4) = 5;
  *(&task_2A8 + 5) = 0;
  *(&task_2A8 + 6) = 0;
  *(&task_2A8 + 7) = 0;
  *(&task_2A8 + 8) = 0;
  *(&task_2A8 + 9) = 0;
  *(&task_2A8 + 10) = 0;
  Script_vfUtils(&task_2A8);
  task_2A8 = 0x3256554C;
  *(&task_2A8 + 1) = 0;
  *(&task_2A8 + 2) = 1;
  *(&task_2A8 + 3) = 1;
  *(&task_2A8 + 4) = 5;
  *(&task_2A8 + 5) = 21;
  *(&task_2A8 + 6) = 0;
  *(&task_2A8 + 7) = 0;
  *(&task_2A8 + 8) = 0;
  *(&task_2A8 + 9) = 0;
  *(&task_2A8 + 10) = 0;
  Script_vfUtils(&task_2A8);
  Script_vfConfigureFormationGroup(SHEN_NAME('LUV2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_25A3C);
  (&local_F4)->y = 0;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(2903, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_1BBD1();
  SCNF_WaitFrames_0000(1);
  SCN_Enqueue_VFGI(2, 99, 1.0f, 4, -1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_scenCreateTask(script_1B8C1, 1, arg0);
  return;
}

static int32_t script_1C25A(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_GetFightingStats(-1, 2) == 1) {
    local_FC = 1;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    local_FC = 2;
  }
  if (script_1BA75()) {
    local_FC = 2;
  }
  return local_FC;
}

static void script_1C2A2(void) {
  script_1BD32();
  return;
}

static void script_1C2AA(void) {
  SCN_Enqueue_VFGI(2, 99, 1.0f, 99, -1);
  return;
}

static void script_1C2BE(int8_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  local_FF = 1;
  if (~Script_IsPlayerAllowed(0)) {
    local_FE = 1;
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_NEAR_NPC_COUNT_CLAMPED) > 0) {
    local_FE = 1;
  }
  if (script_1B91C()) {
    local_FE = 1;
  }
  if (local_FE == 0) {
    script_1BDD5();
    local_FF = 1;
  }
  for (;;) {
    script_1BED5();
    script_1BF12();
    if (local_FE == 1) {
      break;
      local_FF = 14;
    }
    SCNF_WaitFrames_0000(1);
    local_F8 = local_FF;
    if (local_F8 == 0) {
      local_FF = 1;
    } else if (local_F8 == 1) {
      script_1BC83();
      local_FF = 4;
    } else if ((local_F8 == 3) | (local_F8 == 13) | (local_F8 == 14)) {
      break;
    } else if ((local_F8 == 4) | (local_F8 == 12)) {
      local_FF = local_FF + script_1BF4F(local_FF);
    } else if (local_F8 == 5) {
      local_FE = 2;
      script_1BCD6();
      script_1BFC0(0);
      local_FF = 6;
    } else if (local_F8 == 6) {
      local_FF = local_FF + script_1C25A(arg0);
    } else if ((local_F8 == 7) | (local_F8 == 8)) {
      local_FF = local_FF - 7 + 9;
    } else {
      switch (local_F8) {
        case 9:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_1C2A2();
            local_FF = 12;
          }
          break;
        case 10:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_1C2A2();
            local_FF = 14;
          }
          break;
        case 11:
          script_1C2AA();
          local_FF = 6;
          break;
      }
    }
  }
  if (local_FE != 1) {
    script_1BE5D();
  }
  if (local_FE == 2) {
    script_1BB1B();
    Script_PlayerStatsUtils(PS_SetShugyoCourseClearCountSlot, 0, Script_PlayerStatsUtils(PS_GetShugyoCourseClearCountSlot, 0) + 1);
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

__attribute__((scn_string_bindings(2, data_25A5C)))
static int32_t script_1C521(void) {
  int32_t local_F4[3];
  int32_t local_E8[3];
  local_F4[0] = -0x3ECCD220;
  local_F4[1] = 0x3F666666;
  local_F4[2] = -0x3E7CE83E;
  local_E8[0] = -0x3F9ED289;
  local_E8[1] = 0x3F99999A;
  local_E8[2] = -0x3E1B7B16;
  for (;;) {
    if ((task_22C == 0) & (script_1C65B(0) == 1)) {
    } else {
      SCNF_WaitFrames_0000(1);
      continue;
    }
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
    task_228 = 1;
    Script_scenToggleCharaFlag(0, 200, 1);
    if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1) == 0) {
      Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 1);
    }
    Script_GetBlurModel(CHARA_AKIR, "YGB_B.MT5");
    script_1C2BE(0);
    Script_GetBlurModel(CHARA_AKIR, "YKA_B.MT5");
    if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1)) {
      Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 0);
    }
    Script_scenToggleCharaFlag(0, 200, 0);
    task_228 = 0;
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    Script_LoadSND(0, 5, "battle_1.snd", 0);
  }
  return Script_srchControl(0);
}

static int8_t script_1C65B(int8_t arg0) {
  int8_t local_FF;
  float local_F8;
  int32_t local_F4;
  int8_t local_F3;
  int32_t local_EC;
  local_FF = 0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &task_298, ENTITY_XFORM_WORLD_SPACE);
  local_EC = arg0;
  if (local_EC == 0) {
    task_28C.x = -7.5f;
    task_28C.y = 0.0f;
    task_28C.z = -22.5f;
    local_F8 = 2.0f;
    local_F4 = 1;
    goto L01C6B8;
  }
  L01C6B8:;
  local_F3 = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
    if (Script_SetTaskDataValue(CHARA_FUKU, &data_25A6C, &data_25A78, 0)) {
      local_F3 = 1;
    }
  }
  if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &task_28C, &task_298) >= local_F8) & (local_F3 == 0)) {
    if (task_233 == 0) {
      (void)((task_233 = -1) & 1);
      local_FF = local_F4;
    }
  } else {
    if (!(task_233 == -1)) goto L01C72C;
  }
  L01C72C:;
  return local_FF;
}

static void script_1C730(void) {
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_25A84, 0, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f) != -1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_1C7B3(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  float local_E0;
  float local_DC;
  float local_D8;
  float local_D4;
  float local_D0;
  float local_CC;
  float local_C4[2];
  int32_t unused_B8[3];
  int32_t local_B4;
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(0, 2, "bgm109.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 93);
  script_1CDE7();
  script_D9C0();
  script_1CB8D();
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_lightSetSlotEnabled(1, 0);
  Script_lightSetSlotEnabled(8, 0);
  Script_lightSetSlotEnabled(10, 0);
  Script_lightSetSlotEnabled(5, 0);
  script_1CC60();
  local_C4[0] = 0.0f;
  local_C4[1] = 0.0f;
  Script_LightUtils(3, 6, (const float *)local_C4);
  Script_LightUtils(2, 6, (const float *)local_C4);
  Script_LightUtils(6, 6, (const float *)local_C4);
  Script_LightUtils(7, 6, (const float *)local_C4);
  Script_LightUtils(8, 6, (const float *)local_C4);
  Script_shenSeqUtils(0);
  local_F4 = 0;
  local_F0 = 0;
  local_EC = 0;
  local_E8 = 0;
  local_E4 = 0;
  local_E0 = 0.0f;
  local_DC = 0.0f;
  local_D8 = 0.1725f;
  local_D4 = 0.331818193f;
  local_D4 = 0.0f;
  local_D0 = 0.0f;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_B4 = local_FC;
    if (local_B4 == 100) {
    } else if (local_B4 == 0) {
      local_F4 = 1;
      local_E8 = 1;
    } else {
      switch (local_B4) {
        case 0:
          local_E8 = 1;
          break;
        case 76:
          local_F0 = 1;
          break;
        case 80:
          local_E4 = 1;
          break;
        case 165:
          local_EC = 1;
          break;
        case 130:
          script_1DB90();
          break;
        case 220:
          script_1D23B();
          break;
        case 300:
          Script_executeAicaCommand(2720, 2, 30);
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
    if (!(local_D8 >= 0.69f) & (local_F4 == 1)) {
      local_D8 = local_D8 + 0.00575f;
      local_C4[0] = local_D8;
      local_C4[1] = local_D8;
      Script_LightUtils(2, 6, (const float *)local_C4);
    }
    if (!(local_D4 >= 0.4f) & (local_E8 == 1)) {
      local_D4 = local_D4 + 0.00363636366f;
      local_C4[0] = local_D4;
      local_C4[1] = local_D4;
      Script_LightUtils(7, 6, (const float *)local_C4);
    }
    if (!(local_DC >= 0.48f) & (local_F0 == 1)) {
      local_DC = local_DC + 0.00399999972f;
      local_C4[0] = local_DC;
      local_C4[1] = local_DC;
      Script_LightUtils(6, 6, (const float *)local_C4);
    } else if (!(local_DC >= 0.6f) & (local_F0 == 1)) {
      local_DC = local_DC + 0.00500000035f;
      local_C4[0] = local_DC;
      local_C4[1] = local_DC;
      Script_LightUtils(6, 6, (const float *)local_C4);
    } else {
      if (local_F0 == 1) {
        local_CC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.1f);
        local_DC = 4.8f;
        local_DC = local_DC - local_CC;
        Script_LightUtils(6, 5, local_DC);
        goto L01CAEB;
      }
    }
    L01CAEB:;
    if (!(local_D0 >= 0.58f) & (local_E4 == 1)) {
      local_D0 = local_D0 + 0.00483333319f;
      local_C4[0] = local_D0;
      local_C4[1] = local_D0;
      Script_LightUtils(8, 6, (const float *)local_C4);
    }
    if (!(local_E0 >= 0.71f) & (local_EC == 1)) {
      local_E0 = local_E0 + 0.00709999958f;
      local_C4[0] = local_E0;
      local_C4[1] = local_E0;
      Script_LightUtils(3, 6, (const float *)local_C4);
    }
  }
  script_1CBD8();
  script_1D786();
  Script_LoadSND(0, 2, data_25AA3, 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  return;
}

static void script_1CB8D(void) {
  int32_t unused_E8[6];
  Script_LODSnapshotUtils(DISP_CMD_SaveDistancesAndCaptureLodState);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_SetGlobalSceneSlotValue34(1, 0);
  Script_SetGlobalSceneSlotValue34(3, 0);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(8);
  Script_mapmSetCamFlags(0, 0);
  return;
}

static void script_1CBD8(void) {
  script_DA65(CHARA_AKIR);
  Script_SetGameStateTo(AUTH_None);
  Script_LODSnapshotUtils(DISP_CMD_RestoreDistancesAndApplyLodState);
  return;
}

static void script_1CBF4(int32_t arg0, int32_t arg1) {
  float local_FC;
  int16_t local_FA;
  local_FA = 0;
  while (1) {
    if (local_FA <= arg1) {
      local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.2f);
      if (!(local_FC >= 1.0f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    } else {
      Script_lightSetSlotEnabled(arg0, 1);
      Script_srchControl(0);
    }
    local_FA = local_FA + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1CC60(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  local_E4.x = 8.05f;
  (&local_E4)->y = 1.42f;
  (&local_E4)->z = 11.56f;
  Script_lightEnablePointLightAt(6, &local_E4);
  local_F0.x = 1.0f;
  (&local_F0)->y = 0.83f;
  (&local_F0)->z = 0.53f;
  (&local_F0)->w = 0.39f;
  Script_LightUtils(6, 2, &local_F0);
  Script_LightUtils(6, 4, 0.8f);
  Script_LightUtils(6, 5, 4.8f);
  local_DC[0] = 0x3F3AE148;
  local_DC[1] = 0x3F3AE148;
  Script_LightUtils(6, 6, (const float *)local_DC);
  Script_lightSetSlotEnabled(6, 1);
  return;
}

static void script_1CD04(void) {
  float local_FC;
  float local_F8;
  float local_F4;
  float local_F0;
  float local_EC;
  Vector3i local_E0;
  local_F8 = 0.0f;
  local_F4 = 0.0f;
  local_F0 = 0.0f;
  local_EC = 0.0f;
  local_FC = 357.0f;
  while (1) {
    if (!(local_F8 > 2.0f)) {
      local_F4 = local_F4 + 0.003f;
      local_F8 = local_F8 + local_F4;
    }
    if (!(local_FC > 358.0f)) {
      if (!(local_F8 <= local_F0)) {
        local_EC = local_EC + 0.001f;
      } else {
        local_EC = local_EC + 0.008f;
      }
      local_F0 = local_F0 + local_EC;
      local_FC = local_FC - local_F8;
      local_FC = local_FC + local_F0;
      local_E0.x = 0;
      (&local_E0)->y = 6006;
      (&local_E0)->z = (int32_t)(local_FC / 360.0f * (float)0x10000);
      Script_EVT_SetRotation(SHEN_NAME('SSOD'), 0x38000000, &local_E0, 0);
    } else {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1CDE7(void) {
  if (-1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L01CE11;
    }
  }
  L01CE11:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
    if (-1) {
      Script_charaDisplay(CHARA_FUKU, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_FUKU, CDM_Show);
        goto L01CE54;
      }
    }
    L01CE54:;
    Script_scenToggleVisibleFor(CHARA_FUKU, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x5F454E49)) {
    if (-1) {
      Script_charaDisplay(CHARA_INE_, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_INE_, CDM_Show);
        goto L01CE97;
      }
    }
    L01CE97:;
    Script_scenToggleVisibleFor(CHARA_INE_, 0);
  }
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Show);
      goto L01CECB;
    }
  }
  L01CECB:;
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GART'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Show);
      goto L01CEFF;
    }
  }
  L01CEFF:;
  Script_scenToggleVisibleFor(SHEN_NAME('GART'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
      goto L01CF33;
    }
  }
  L01CF33:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
      goto L01CF67;
    }
  }
  L01CF67:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Show);
      goto L01CF9B;
    }
  }
  L01CF9B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HOKI'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MOON'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MOON'), CDM_Show);
      goto L01CFCF;
    }
  }
  L01CFCF:;
  Script_scenToggleVisibleFor(SHEN_NAME('MOON'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L01D003;
    }
  }
  L01D003:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L01D037;
    }
  }
  L01D037:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L01D06B;
    }
  }
  L01D06B:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L01D09F;
    }
  }
  L01D09F:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
      goto L01D0D3;
    }
  }
  L01D0D3:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
      goto L01D107;
    }
  }
  L01D107:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
      goto L01D13B;
    }
  }
  L01D13B:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
      goto L01D16F;
    }
  }
  L01D16F:;
  Script_scenToggleVisibleFor(SHEN_NAME('POK2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
      goto L01D1A3;
    }
  }
  L01D1A3:;
  Script_scenToggleVisibleFor(SHEN_NAME('POS2'), 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52554B59)) {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
        goto L01D1E6;
      }
    }
    L01D1E6:;
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4C554B59)) {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Show);
        goto L01D229;
      }
    }
    L01D229:;
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  }
  script_1D23B();
  return;
}

static void script_1D23B(void) {
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Show);
      goto L01D265;
    }
  }
  L01D265:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Show);
      goto L01D299;
    }
  }
  L01D299:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Show);
      goto L01D2CD;
    }
  }
  L01D2CD:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Show);
      goto L01D301;
    }
  }
  L01D301:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Show);
      goto L01D335;
    }
  }
  L01D335:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Show);
      goto L01D369;
    }
  }
  L01D369:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Show);
      goto L01D39D;
    }
  }
  L01D39D:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Show);
      goto L01D3D1;
    }
  }
  L01D3D1:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Show);
      goto L01D405;
    }
  }
  L01D405:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Show);
      goto L01D439;
    }
  }
  L01D439:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Show);
      goto L01D46D;
    }
  }
  L01D46D:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS5'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Show);
      goto L01D4A1;
    }
  }
  L01D4A1:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS6'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Show);
      goto L01D4D5;
    }
  }
  L01D4D5:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS7'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Show);
      goto L01D509;
    }
  }
  L01D509:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS8'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Show);
      goto L01D53D;
    }
  }
  L01D53D:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS9'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASA'), CDM_Show);
      goto L01D571;
    }
  }
  L01D571:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASB'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASB'), CDM_Show);
      goto L01D5A5;
    }
  }
  L01D5A5:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASB'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASC'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASC'), CDM_Show);
      goto L01D5D9;
    }
  }
  L01D5D9:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASC'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASD'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASD'), CDM_Show);
      goto L01D60D;
    }
  }
  L01D60D:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASD'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASE'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASE'), CDM_Show);
      goto L01D641;
    }
  }
  L01D641:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASE'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASF'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASF'), CDM_Show);
      goto L01D675;
    }
  }
  L01D675:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASF'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Show);
      goto L01D6A9;
    }
  }
  L01D6A9:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Show);
      goto L01D6DD;
    }
  }
  L01D6DD:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Show);
      goto L01D711;
    }
  }
  L01D711:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Show);
      goto L01D745;
    }
  }
  L01D745:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Show);
      goto L01D779;
    }
  }
  L01D779:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN4'), 0);
  return;
}

static void script_1D786(void) {
  if (0) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L01D7B0;
    }
  }
  L01D7B0:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 1);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x554B5546)) {
    if (0) {
      Script_charaDisplay(CHARA_FUKU, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(CHARA_FUKU, CDM_Show);
        goto L01D7F3;
      }
    }
    L01D7F3:;
    Script_scenToggleVisibleFor(CHARA_FUKU, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x5F454E49)) {
    if (-1) {
      Script_charaDisplay(CHARA_INE_, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_INE_, CDM_Show);
        goto L01D836;
      }
    }
    L01D836:;
    Script_scenToggleVisibleFor(CHARA_INE_, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x5F454E49)) {
    Script_scenToggleVisibleFor(CHARA_INE_, 1);
  }
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GART'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Show);
      goto L01D891;
    }
  }
  L01D891:;
  Script_scenToggleVisibleFor(SHEN_NAME('GART'), 1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
        goto L01D8E1;
      }
    }
    L01D8E1:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
        goto L01D940;
      }
    }
    L01D940:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 1);
  }
  Script_scenToggleVisibleFor(SHEN_NAME('HOKI'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L01D98F;
    }
  }
  L01D98F:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L01D9C3;
    }
  }
  L01D9C3:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L01D9F7;
    }
  }
  L01D9F7:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L01DA2B;
    }
  }
  L01DA2B:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
      goto L01DA5F;
    }
  }
  L01DA5F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 1);
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED) | task_22C) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
        goto L01DAD6;
      }
    }
    L01DAD6:;
    Script_scenToggleVisibleFor(SHEN_NAME('POK2'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('POK2'), 1);
  }
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED) | task_22C) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
        goto L01DB39;
      }
    }
    L01DB39:;
    Script_scenToggleVisibleFor(SHEN_NAME('POS2'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('POS2'), 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52554B59)) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4C554B59)) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  }
  script_1DB90();
  return;
}

static void script_1DB90(void) {
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Show);
      goto L01DBBA;
    }
  }
  L01DBBA:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Show);
      goto L01DBEE;
    }
  }
  L01DBEE:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Show);
      goto L01DC22;
    }
  }
  L01DC22:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Show);
      goto L01DC56;
    }
  }
  L01DC56:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Show);
      goto L01DC8A;
    }
  }
  L01DC8A:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Show);
      goto L01DCBE;
    }
  }
  L01DCBE:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Show);
      goto L01DCF2;
    }
  }
  L01DCF2:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Show);
      goto L01DD26;
    }
  }
  L01DD26:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Show);
      goto L01DD5A;
    }
  }
  L01DD5A:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Show);
      goto L01DD8E;
    }
  }
  L01DD8E:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Show);
      goto L01DDC2;
    }
  }
  L01DDC2:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS5'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Show);
      goto L01DDF6;
    }
  }
  L01DDF6:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS6'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Show);
      goto L01DE2A;
    }
  }
  L01DE2A:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS7'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Show);
      goto L01DE5E;
    }
  }
  L01DE5E:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS8'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Show);
      goto L01DE92;
    }
  }
  L01DE92:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS9'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASA'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASA'), CDM_Show);
      goto L01DEC6;
    }
  }
  L01DEC6:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASA'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASB'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASB'), CDM_Show);
      goto L01DEFA;
    }
  }
  L01DEFA:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASB'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASC'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASC'), CDM_Show);
      goto L01DF2E;
    }
  }
  L01DF2E:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASC'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASD'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASD'), CDM_Show);
      goto L01DF62;
    }
  }
  L01DF62:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASD'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASE'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASE'), CDM_Show);
      goto L01DF96;
    }
  }
  L01DF96:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASE'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASF'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASF'), CDM_Show);
      goto L01DFCA;
    }
  }
  L01DFCA:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASF'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Show);
      goto L01DFFE;
    }
  }
  L01DFFE:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Show);
      goto L01E032;
    }
  }
  L01E032:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Show);
      goto L01E066;
    }
  }
  L01E066:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Show);
      goto L01E09A;
    }
  }
  L01E09A:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Show);
      goto L01E0CE;
    }
  }
  L01E0CE:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN4'), 1);
  return;
}

static void script_1E0DB(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_25B40, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_25B08, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_25AD0, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_25AEC, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_25B24, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_1E213(int32_t arg0) {
  script_1E22B(arg0);
  return;
}

static void script_1E21F(int32_t arg0) {
  script_1E22B(arg0);
  return;
}

static void script_1E22B(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L01E264;
    }
  }
  L01E264:;
  Script_ryoSetChara(arg0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionDisabled, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_1E0DB();
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

static void script_1E337(int32_t arg0) {
  script_1E21F(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1E34E(int32_t arg0) {
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

static void script_1E38F(int32_t arg0) {
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

static void script_1E3D3(int32_t arg0) {
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

static void script_1E451(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_1E3D3(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_1E3D3(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_1E3D3(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_1E3D3(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_1E3D3(5);
  } else {
    script_1E3D3(0);
  }
  return;
}

static void script_1E4D0(void) {
  Script_ambSoundUtils(1, 0, &data_25B78, 15.0f, 50.0f, 0.8f, 2, &data_25BD8);
  Script_ambSoundUtils(2, 0, &data_25B84, 18.0f, 30.0f, 0.8f, 2, &data_25C60);
  Script_ambSoundUtils(3, 0, &data_25B90, 20.0f, 30.0f, 0.8f, 2, &data_25CE8);
  Script_ambSoundUtils(4, 0, &data_25B9C, 8.0f, 100.0f, 0.8f, 0, &data_25D70);
  Script_ambSoundUtils(5, 1, data_25BA8, 50.0f, 40.0f, 1.0f, 0, data_25DF8);
  Script_ambSoundUtils(6, 1, data_25BB4, 50.0f, 40.0f, 0.8f, 0, data_25E80);
  Script_ambSoundUtils(14, 1, &data_25BC0, 40.0f, 15.0f, 1.0f, 3, &data_25F08);
  Script_ambSoundUtils(15, 1, &data_25BCC, 32.0f, 15.0f, 1.0f, 3, &data_25F90);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_1E5FD(void) {
  return;
}

__attribute__((scn_string_bindings(0, data_26018, 1, data_26024, 2, data_2602F, 4, data_26047, 5, data_26054, 6, data_26060, 7, data_2606B, 8, data_26076, 9, data_26083, 10, data_26090, 13, data_260B2, 15, data_260C7, 17, data_260DC, 18, data_260EC)))
void script_1E600(void) {
  int32_t unused_E8[6];
  task_244 = Script_ryoSleepUtils(0);
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    Script_SetShouldSleepFlag(0);
  }
  if (-(Script_GetCurrentEntryPoint() == 1) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DKTY_TUBA_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1037_SEQUENCE_COMPLETE) == 0)) {
    Script_SetShouldSleepFlag(0);
  }
  if (-(Script_GetCurrentEntryPoint() == 6) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 869) == 0)) {
    Script_SetShouldSleepFlag(0);
    Script_LoadSND_Wrapper("system1.snd", "amb036.snd", "bgm085.snd", 0, "a1_houha.snd", "battle_1.snd", 0, 0);
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 100);
  } else {
    Script_LoadSND_Wrapper("system1.snd", "amb036.snd", "bgm009.snd", 0, "f1doujou.snd", "battle_1.snd", 0, 0);
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 100);
  }
  script_1E4D0();
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 255, 0, 0, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "JHD0") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_LoadHandMot(2, SHEN_NAME('JHD0'), "HMOT_YKA.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  task_224 = Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 0);
  if (task_224 & 2) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPSN") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPSU") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_1F6A8();
  if (-(Script_GetCurrentEntryPoint() == 6) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 869) == 0)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "HOUO") == 0) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  script_19347();
  script_B807(CHARA_AKIR, (DSWKDoorEntry *)data_22B78, (int32_t *)data_22C14, (DoorResourceDescriptor *)&data_22C38, (MapObjectRecord *)data_22C58, 3, 0);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  SCNF_WaitFrames_0000(1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A0160_FUKU_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_PRE_0172_FUKU_INE_STAGE_DONE) == 0)) {
    script_BBCD(0);
  }
  Script_motiLoad(2, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/02/JOMO", (int32_t)"SCRTEST.BIN", 0x3044484A);
  script_1F3F9();
  Script_shenSetLightingMode(1);
  Script_PREP_Related(-1, 0);
  task_228 = 0;
  task_22B = 0;
  task_231 = 0;
  task_22C = 0;
  Script_scenToggleCharaFlag(0, 200, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  SCNF_WaitFrames_0000(1);
  Script_ryoSleepUtils(2, 1);
  if (task_244 != 0) {
    Script_ryoSleepUtils(1);
    task_22C = Script_ryoSleepUtils(3, 0, 1);
  } else {
    Script_ryoSleepUtils(3, 1, 1, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  script_1E213(CHARA_AKIR);
  Script_scenCreateTask(script_1E9E2, 1, 0x52494B41);
  script_17244();
  Script_scenCreateTask(script_19872, 0);
  script_18D08();
  script_D95F();
  return;
}

__attribute__((scn_string_bindings(0, data_26130, 3, data_26142, 4, data_26148, 5, data_2614D, 6, data_2614E, 7, data_26153, 8, data_26159, 9, data_26164, 11, data_26178, 13, data_26184, 15, data_26190)))
static int32_t script_1E9E2(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_DC[6];
  Vector3i local_D0;
  int32_t unused_CC;
  GameDateTime local_C4;
  int32_t local_C0;
  int32_t local_BC;
  int32_t unused_B0[3];
  script_1F493();
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 255, 0, 0, 0);
  task_229 = 0;
  task_22A = 0;
  Script_ReturnTimeDate(&local_C4);
  local_C0 = (&local_C4)->hours * 60 + (&local_C4)->minutes;
  local_BC = Script_StepUtils(GET_STEP_HIGHER, 0);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  Script_scenCreateTask(script_1F525, 0);
  Script_cpctStateUtils(1, 0);
  task_232 = 1;
  Script_scenCreateTask(script_1F198, 0);
  if (Script_SetTaskDataValue(CHARA_AKIR, &data_26118, &data_26124, 0)) {
    Script_cpctStateUtils(1, 2);
    Script_CreateNPCImageModule(CHARA_AKIR, "YKC_M", 0);
    local_F4.x = 0.85f;
    (&local_F4)->y = 0.1f;
    (&local_F4)->z = -22.4f;
    local_D0.x = 0;
    (&local_D0)->y = 0x4000;
    (&local_D0)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_D0, 0);
    local_F4.x = 0.85f;
    (&local_F4)->y = 0.1f;
    (&local_F4)->z = -22.6f;
    local_D0.x = 0;
    (&local_D0)->y = 0x4000;
    (&local_D0)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_D0, 0);
    Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Show);
    script_10911();
  }
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  task_10 = 0;
  Script_LoadVoiceFile(1, "E1011");
  Script_scenCreateTask(script_10A07, 0);
  if (-(Script_GetCurrentEntryPoint() == 6) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 869) == 0)) {
    Script_LoadVoiceFile(1, "01DIS");
    script_107A8();
    Script_CreateNPCImageModule(CHARA_FUKU, "FUK_M", 0);
    Script_scenToggleActiveFor(CHARA_FUKU, 1);
    Script_SetCharaFlags(CHARA_FUKU, 4);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    Script_charaSetRefboardFlagTo(CHARA_FUKU, 1);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    task_228 = 1;
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_AuthorUtils(0, 0, "HOUO", "");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    script_10911();
    Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    script_EC28();
    if (Script_shenSeqUtils(-7)) {
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(1);
    Script_AuthorUtils(AUTH_FREE, 0);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "HOUO");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 869, 1);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_ReturnTimeDate(&local_C4);
    local_C0 = (&local_C4)->hours * 60 + (&local_C4)->minutes;
    if ((local_C0 >= 480) & (local_C0 < 1410)) {
      local_F4.x = 3.6714f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -22.493f;
      local_D0.x = 0;
      (&local_D0)->y = 0xC000;
      (&local_D0)->z = 0;
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D0, 0);
      Script_CreateNPCImageModule(CHARA_AKIR, "YKB_M", 0);
      Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Hide);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
      Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 0);
      script_10949();
      Script_LoadSND(0, 2, "bgm009.snd", 0);
      Script_LoadSND(0, 4, "f1doujou.snd", 0);
      Script_executeAicaCommand(16296, 0, 0);
      Script_executeAicaCommand(1184, 2, 100);
      SCNF_WaitFrames_0000(1);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 0);
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      Script_SetGameStateTo(AUTH_None);
      Script_convSetCamInactiveFlag(1);
      Script_ryoControl(RYO_RestoreControl);
      Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
      task_228 = 0;
      Script_shenClearFreeState(1);
      script_10750();
      Script_SetShouldSleepFlag(1);
    } else {
      Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
      SCNF_WaitFrames_0000(1);
    }
  } else if (-(Script_GetCurrentEntryPoint() == 1) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DKTY_TUBA_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1037_SEQUENCE_COMPLETE) == 0)) {
    Script_LoadVoiceFile(1, "SA1037");
    script_107A8();
    Script_CreateNPCImageModule(CHARA_FUKU, "FUB_M", 0);
    Script_scenToggleActiveFor(CHARA_FUKU, 1);
    Script_SetCharaFlags(CHARA_FUKU, 4);
    task_22A = 1;
    task_228 = 1;
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 1021, 1);
    Script_scenCreateTask(script_183D9, 0);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SORA_SCENE_PENDING) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SORA_SCENE_STARTED) == 0)) {
    Script_LoadVoiceFile(1, "01SOR");
    script_107A8();
    Script_CreateNPCImageModule(CHARA_FUKU, "FUB_M", 0);
    Script_CreateNPCImageModule(CHARA_INE_, "INE_M", 0);
    Script_scenToggleActiveFor(CHARA_FUKU, 1);
    Script_SetCharaFlags(CHARA_FUKU, 4);
    if (-1) {
      Script_charaDisplay(CHARA_INE_, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_INE_, CDM_Show);
        goto L01EFE2;
      }
    }
    L01EFE2:;
    Script_scenToggleVisibleFor(CHARA_INE_, 0);
    local_F4.x = 7.5f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -26.8f;
    local_D0.x = 0;
    (&local_D0)->y = 0;
    (&local_D0)->z = 0;
    Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_D0, 0);
    Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_CreateNPCImageModule(CHARA_AKIR, "YKB_M", 0);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    Script_charaSetRefboardFlagTo(CHARA_FUKU, 1);
    Script_scenCreateTask(script_18E20, 0);
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
  } else {
    if (task_22C) {
      Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
      Script_ryoControl(RYO_RestoreControl);
      Script_shenClearFreeState(1);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SORA_SCENE_PENDING) == 0)) {
      Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
      Script_ryoControl(RYO_RestoreControl);
      Script_shenClearFreeState(1);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DKTY_TUBA_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE) == 0)) {
      Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
      Script_ryoControl(RYO_RestoreControl);
      Script_shenClearFreeState(1);
    } else if (local_BC == 17) {
      Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
      Script_ryoControl(RYO_RestoreControl);
      Script_shenClearFreeState(1);
    } else {
      Script_ryoControl(RYO_RestoreControl);
      Script_shenClearFreeState(1);
    }
    Script_scenCreateTask(script_1C521, 0);
  }
  Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  task_232 = 0;
}

__attribute__((scn_string_bindings(0, data_26196, 1, data_261A6, 2, data_261AB, 3, data_261BB, 4, data_261C0, 5, data_261C5, 6, data_261C6, 7, data_261CB, 8, data_261CC, 9, data_261D1)))
static int32_t script_1F198(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  while (task_232) {
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    if (task_229 == 0) {
      if ((&local_F4)->hours > 18) {
        local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
        if (local_F0 == 1140) {
          while ((Script_IsPlayerAllowed(0) == 0) | (task_228 != 0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
          while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
          Script_EVT_GetPosition(CHARA_AKIR, -1, &task_20C, 0);
          Script_mtwkGetSetRotation(CHARA_AKIR, &task_218, 0);
          if (task_224 & 2) {
            while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPM2") == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPM1") == 0) {
              SCNF_WaitFrames_0000(1);
            }
          }
          while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
          if (task_224 & 2) {
            Script_AuthorUtils(0, 0, "MPM2", "");
          } else {
            Script_AuthorUtils(0, 0, "MPM1", "");
          }
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
          script_1C7B3(1);
          Script_AuthorUtils(AUTH_FREE, 0);
          if (task_224 & 2) {
            Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MPM2");
          } else {
            Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MPM1");
          }
          Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_20C);
          Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_218, 0);
          Script_InitializeOsag(CHARA_AKIR);
          Script_SetupLightFromTable(4);
          Script_PREP_Related(-1, 1);
          Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
          Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
          Script_convSetCamInactiveFlag(1);
          Script_SetGameStateTo(AUTH_None);
          Script_ryoControl(RYO_RestoreControl);
        } else {
          Script_SetGlobalSceneSlotValue34(1, 0);
          Script_SetGlobalSceneSlotValue34(3, 0);
          Script_PREP_Related(-1, 0);
          Script_SetupLightFromTable(4);
          Script_PREP_Related(-1, 1);
        }
        task_229 = 1;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1F3F9(void) {
  int32_t unused_F8[2];
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  SCNF_WaitFrames_0000(1);
  Script_SetupLightFromTable(0);
  Script_SetFogColour(&data_26280);
  Script_SetVector4ByOffset(&data_26290);
  (void)1;
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetMapFlags(0, 1);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_261D8, 3);
  return;
}

static void script_1F459(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 15.0f);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_1F47A(void) {
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherTaskUtil(InitTaskData);
  Script_shenSetLightingMode(1);
  return;
}

static void script_1F493(void) {
  int32_t unused_FC;
  int32_t local_F8;
  L01F495:;
  if (1) {
    local_F8 = Script_GetCurrentEntryPoint();
    goto L01F4A8;
    goto L01F495;
  }
  L01F4A8:;
  return;
}

static void script_1F4A9(int32_t arg0) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    while (local_FC == 0) {
      SCNF_WaitFrames_0000(1);
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    }
    while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, arg0, 1);
  }
  return;
}

static void script_1F51F(void) {
  return;
}

static void script_1F522(void) {
  return;
}

static int32_t script_1F525(void) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  task_22D = 0;
  task_22E = 0;
  task_22F = 0;
  task_230 = 0;
  local_FF = 0;
  local_FE = 0;
  local_FD = 0;
  local_FC = 0;
  SCNF_WaitFrames_0000(1);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 16);
  for (;;) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_262B0, &data_262BC, 0)) {
      if (task_22D == 0) {
        task_22D = 1;
        Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 11);
        local_FF = local_FF + 1;
      }
    } else {
      if (task_22D) {
        task_22D = 0;
      }
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_262C8, &data_262D4, 0)) {
      if (task_22E == 0) {
        task_22E = 1;
        Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 12);
        local_FE = local_FE + 1;
      }
    } else {
      if (task_22E) {
        task_22E = 0;
      }
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_262E0, &data_262EC, 0)) {
      if (task_22F == 0) {
        task_22F = 1;
        Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 13);
        local_FD = local_FD + 1;
      }
    } else {
      if (task_22F) {
        task_22F = 0;
      }
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_262F8, &data_26304, 0)) {
      if (task_230 == 0) {
        task_230 = 1;
        Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 10);
        local_FC = local_FC + 1;
      }
    } else {
      if (task_230) {
        task_230 = 0;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_26310, 2, data_26325, 4, data_2633A, 6, data_2634F, 7, data_2635F, 8, data_26364, 9, data_26374, 10, data_26379, 11, data_26389, 12, data_2638E, 13, data_2639E, 14, data_263A3, 15, data_263B3, 16, data_263B8, 17, data_263C8, 18, data_263CD, 19, data_263DD, 20, data_263E2)))
static void script_1F6A8(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->month == 3) {
    if (((&local_F4)->day == 25) | ((&local_F4)->day == 26) | ((&local_F4)->day == 27)) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPS1") == 0) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      if (((&local_F4)->day == 28) | ((&local_F4)->day == 29) | ((&local_F4)->day == 30) | ((&local_F4)->day == 31)) {
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPS2") == 0) {
          SCNF_WaitFrames_0000(1);
        }
        goto L01F75B;
      }
    }
    L01F75B:;
  } else {
    if ((&local_F4)->month == 4) {
      if (((&local_F4)->day == 1) | ((&local_F4)->day == 2) | ((&local_F4)->day == 3) | ((&local_F4)->day == 4) | ((&local_F4)->day == 5) | ((&local_F4)->day == 6) | ((&local_F4)->day == 7)) {
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPS3") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 8) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 40, 0.00025f, 0);
        script_1FCAF(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPS4") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 9) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 800, 0.005f, 0);
        script_1FCAF(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPS4") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 10) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 480, 0.003f, 0);
        script_1FCAF(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPS5") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 11) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 400, 0.0025f, 0);
        script_1FCAF(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPS5") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 12) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 240, 0.002f, 0);
        script_1FCAF(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPS6") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 13) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 160, 0.0015f, 0);
        script_1FCAF(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPS6") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 14) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 80, 0.0005f, 0);
        script_1FCAF(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", "MPS6") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else {
        if ((&local_F4)->day == 15) {
          while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JHD0/", data_263F2) == 0) {
            SCNF_WaitFrames_0000(1);
          }
          goto L01FC94;
        }
      }
      L01FC94:;
      goto L01FC97;
    }
  }
  L01FC97:;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1FCAF(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_E4;
  GameDateTime local_DC;
  int32_t local_D8;
  Script_ReturnTimeDate(&local_DC);
  local_D8 = Script_WeatherUtils_0(WeatherDataUtil_GetWeatherClassForDateTime, (&local_DC)->year, (&local_DC)->month, (&local_DC)->day, (&local_DC)->hours, (&local_DC)->minutes);
  if (local_D8 == 2) {
    local_F4.x = -0.00360784307f;
    (&local_F4)->y = -0.0200784318f;
    (&local_F4)->z = 0.00188235298f;
  } else {
    local_F4.x = -0.00360784307f;
    (&local_F4)->y = -0.0122352941f;
    (&local_F4)->z = 0.00188235298f;
  }
  local_E8.x = -0.00235294108f;
  (&local_E8)->y = -0.00250980398f;
  (&local_E8)->z = -0.00235294108f;
  Script_sakuraUtils(6, arg0, &local_F4, &local_E8);
  return;
}

static void script_1FD5A(int32_t arg0) {
  if (arg0 < 0) {
    (void)-arg0;
    return;
  } else {
    (void)arg0;
    return;
  }
}

static void script_1FD78(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static void script_1FD97(int32_t arg0) {
  if (arg0 != 0) {
    Script_sprintf("WAZA Warning : handle is not NULL, and create same handle !\012");
  }
  return;
}

static void script_1FDAC(int32_t arg0, int32_t arg1) {
  for (;;) {
    if (*(&task_348 + 3) & 2) {
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

static int32_t script_1FE62(int32_t arg0) {
  task_86C = arg0;
  *(&task_86C + 1) = 0;
  return (int32_t)&task_86C;
}

static void script_1FE7A(int32_t arg0) {
  Script_sprintf("%s\012", script_1FE62(arg0));
  return;
}

static void script_1FE90(int32_t arg0) {
  Script_sprintf("Konnna Error ha Detekonai hazu : %d\012", arg0);
  return;
}

static void script_1FE9E(void) {
  Vector3 local_F4;
  SCNF_WaitFrames_0000(60);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 0.1f;
  for (;;) {
    if (*(&task_338 + 1) & 0x4000) {
      local_F4.x = (&local_F4)->x - 0.1f;
    }
    if (*(&task_338 + 1) & 0x8000) {
      local_F4.x = (&local_F4)->x + 0.1f;
    }
    if (*(&task_338 + 1) & 0x1000) {
      (&local_F4)->z = (&local_F4)->z - 0.1f;
    }
    if (*(&task_338 + 1) & 0x2000) {
      (&local_F4)->z = (&local_F4)->z + 0.1f;
    }
    Script_TranslateRotateScale_Wrapper(1, 0x3F800000, &local_F4, &data_26500, &data_264F4);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1FF68(void) {
  return;
}

static void script_1FF6B(void) {
  task_368 = 0;
  while (1) {
    task_368 = task_368 + 1;
    task_338 = Script_GetInputData(0, 0);
    *(&task_338 + 1) = Script_GetInputData(1, 0);
    *(&task_338 + 2) = Script_GetInputData(2, 0);
    *(&task_338 + 3) = Script_GetInputData(3, 0);
    task_348 = Script_GetInputData(0, 1);
    *(&task_348 + 1) = Script_GetInputData(1, 1);
    *(&task_348 + 2) = Script_GetInputData(2, 1);
    *(&task_348 + 3) = Script_GetInputData(3, 1);
    task_358 = Script_GetInputData(0, 2);
    *(&task_358 + 1) = Script_GetInputData(1, 2);
    *(&task_358 + 2) = Script_GetInputData(2, 2);
    *(&task_358 + 3) = Script_GetInputData(3, 2);
    if (*(&task_348 + 1) & 2) {
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

static void script_200B9(int32_t arg0) {
  task_36C = arg0;
  Script_seqEnterEvtCam(arg0, 0, AUTH_None);
  return;
}

static int32_t script_200CD(int32_t arg0, int32_t arg1, int32_t arg2) {
  SCNF_WaitFrames_0000(arg2);
  script_200B9(arg0);
  SCNF_WaitFrames_0000(1);
  Script_AllocSpline(arg1);
  end:;
}

static void script_200EF(int32_t arg0, int32_t arg1, int32_t arg2) {
  arg2 = arg2 / 2;
  if (arg2) {
    arg2 = arg2 - 1;
    Script_scenCreateTask(script_200CD, 3, arg0, arg1, arg2);
  } else {
    script_200B9(arg0);
    SCNF_WaitFrames_0000(1);
    Script_AllocSpline(arg1);
  }
  return;
}

static void script_20135(int32_t arg0, int32_t arg1, int32_t arg2) {
  task_36C = arg0;
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return;
}

static void script_20149(int32_t arg0) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
  }
  return;
}

static void script_20163(void) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsInEventCam() == 0) {
      break;
    }
  }
  return;
}

static void script_2017E(int32_t arg0) {
  script_200B9(arg0);
  SCNF_WaitFrames_0000(2);
  for (;;) {
    script_20163();
    script_200B9(arg0);
    SCNF_WaitFrames_0000(2);
  }
  return;
}

static void script_201AB(int32_t arg0, int32_t arg1) {
  while (arg0) {
    SCNF_WaitFrames_0000(1);
    arg0 = arg0 - 1;
  }
  Script_executeAicaCommand(arg1, 0, 0);
  return;
}

static void script_201D2(void) {
  task_874 = 0;
  return;
}

static void script_201DC(void) {
  for (;;) {
    if (task_874 == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_201F7(void) {
  task_874 = 1;
  return;
}

static void script_20201(int32_t arg0) {
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

static int32_t script_20265(float arg0, float arg1, float arg2, float arg3) {
  int32_t unused_FC;
  int32_t local_F4[2];
  arg0 = arg2 - arg0;
  arg1 = arg3 - arg1;
  return local_F4[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, arg0, arg1);
}

static int32_t script_2028C(int32_t arg0, int32_t arg1) {
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

static int32_t script_20304(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    task_878[arg1] = Script_wmptUtils(arg0, arg3, arg2);
    if (task_878[arg1] == 0) {
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

static void script_203B0(int32_t arg0, int32_t arg1, int32_t arg2) {
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
    task_878[arg1] = 0;
  } else {
    local_DC[0] = *(float *)arg2 - (&local_F4)->x;
    local_DC[2] = *(float *)(arg2 + 8) - (&local_F4)->z;
    local_C0[1] = script_20265((&local_F4)->x, (&local_F4)->z, *(float *)arg2, *(float *)(arg2 + 8));
    local_D8 = script_2028C(local_C0[1], (&local_E8)->y);
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
    task_878[arg1] = 1000;
    local_B4 = Script_scenCreateTask(script_20304, 4, arg0, arg1, local_BC, arg2);
  }
  return;
}

static void script_20576(int32_t arg0) {
  for (;;) {
    if (task_878[arg0] == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_20598(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_20673(void) {
  for (;;) {
    task_37C = task_37C + 1;
    task_380 = task_380 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_20697(int32_t arg0) {
  arg0 = 0;
  *(int8_t *)task_3A0[arg0] = *(int8_t *)&task_394;
  *(int8_t *)&task_3A0[arg0][1] = *((int8_t *)&task_394 + 1);
  *(int8_t *)&task_3A0[arg0][2] = *((int8_t *)&task_394 + 2);
  *(int8_t *)&task_3A0[arg0][3] = *((int8_t *)&task_394 + 3);
  *(int8_t *)&task_3A0[arg0][4] = *((int8_t *)&task_394 + 4);
  *(int8_t *)&task_3A0[arg0][5] = *((int8_t *)&task_394 + 5);
  *(int8_t *)&task_3A0[arg0][6] = *((int8_t *)&task_394 + 6);
  *(int8_t *)&task_3A0[arg0][7] = *((int8_t *)&task_394 + 7);
  *(int32_t *)&task_3A0[arg0][8] = *(int32_t *)(&task_394 + 8);
  return;
}

static void script_20757(void) {
  (void)0;
  return;
}

static void script_2075D(void) {
  *(int8_t *)&task_3A0[0][2];
  return;
}

static void script_20768(int32_t arg0) {
  int32_t unused_F8[2];
  for (;;) {
    if (arg0 == 9) {
      if ((task_378 != task_374) & (task_374 % 3 == 0)) {
        task_390 = 1;
      }
      if ((task_37C != 0) & (task_37C % 450 == 0)) {
        task_390 = 1;
        task_391 = 1;
      }
    } else {
      if (((task_380 != 0) & (task_380 % 300 == 0)) | ((task_378 != task_374) & (task_374 % 3 == 0))) {
        task_390 = 1;
      }
      if ((task_37C != 0) & (task_37C % 2700 == 0)) {
        task_390 = 1;
        task_393 = 1;
      } else {
        if ((task_37C != 0) & (task_37C % 1800 == 0)) {
          task_390 = 1;
          task_392 = 1;
          goto L02082F;
        }
      }
    }
    L02082F:;
    task_378 = task_374;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_20843(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_867 = 1;
  local_F0 = 0;
  for (;;) {
    if (task_867 == 0) {
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

static void script_208A8(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_868 = 1;
  local_F0 = 0;
  for (;;) {
    if (task_868 == 0) {
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

static void script_2090D(void) {
  int32_t local_FC;
  task_866 = 0;
  local_FC = 0;
  for (;;) {
    *((int8_t *)task_850 + local_FC) = 0;
    *((int8_t *)task_85C + local_FC) = 0;
    local_FC = local_FC + 1;
    if (local_FC >= 10) {
      break;
    }
  }
  return;
}

static void script_20949(void) {
  for (;;) {
    if (task_866 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_20964(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_209D8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_F4[3];
  script_201F7();
  task_866 = 1;
  script_20964(arg0, arg1, arg2, arg3);
  task_866 = 0;
  return;
}

static void script_209FD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  script_201F7();
  task_866 = 1;
  script_20964(arg0, arg1, arg2, arg3);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
  task_866 = 0;
  return;
}

static void script_20A46(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  script_201F7();
  task_866 = 1;
  script_20964(arg0, arg1, arg2, arg3);
  local_F0 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  if (local_F0 == 0) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
    goto L020AA4;
  }
  L020AA4:;
  task_866 = 0;
  return;
}

static void script_20AAC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  script_201F7();
  task_866 = 1;
  SCNF_WaitFrames_0000(arg3);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
  Script_PlayCharaMotion(arg0, 20998, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(30);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  script_20964(arg0, arg1, arg2, 0);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
  script_20149(arg0);
  Script_PlayCharaMotion(arg0, 21000, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_866 = 0;
  return;
}

static void script_20B74(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  task_866 = 1;
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  SCNF_WaitFrames_0000(arg2);
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  task_866 = 0;
  script_201F7();
  return;
}

static void script_20BCA(void) {
  task_8A0 = 1;
  task_8A4 = 0;
  for (;;) {
    if (task_8A0) {
      task_8A0 = task_8A0 - 1;
      if (task_8A0 == 0) {
        task_8A4 = 0;
        Script_LCDM_Utils(0, (const void *)task_8A4);
        Script_sicnUtils(2, 0x800, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_20C1F(int32_t arg0) {
  task_8A4 = arg0;
  if (task_8A4 == 0) {
    task_8A0 = 0;
    Script_LCDM_Utils(1, 0);
    Script_sicnUtils(2, 0x800, 0);
  } else {
    task_8A0 = 600;
    Script_LCDM_Utils(1, (const void *)1);
    Script_sicnUtils(0, 0x800, 0);
  }
  Script_LCDM_Utils(0, (const void *)task_8A4);
  return;
}

static void script_20C79(const char *arg0, const char *arg1) {
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
