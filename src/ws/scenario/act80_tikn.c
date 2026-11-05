#include "act80_tikn.h"

static void script_0030(int32_t arg0) {
  (void)((task_2 = -1) & 1);
  (void)((task_3 = 0) & 1);
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
  (void)((task_0 = 0) & 1);
  return;
}

static void script_00CC(void) {
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (task_3 & 0x40) {
  } else {
    Script_shenClearFreeState(1);
  }
  return;
}

static void script_010E(void) {
  int8_t local_FF;
  (void)((local_FF = 0) & 1);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_shenClearFreeState(0);
  if (Script_ryoControl(PCCTL_QueryShouldRun)) {
    (void)((local_FF = -1) & 1);
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

static void script_019A(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  float local_F0;
  if (~task_2) {
  } else {
    Script_ReturnTimeDate(&local_F4);
    if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 0)) {
      (void)((task_3 = -1) & 8);
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

__attribute__((scn_string_bindings(0, data_1852D)))
static void script_0318(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  if (arg0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, arg1)) goto L0003FE;
    Script_ReturnTimeDate(&local_F4);
    if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 0)) goto L0003FE;
    Script_sicnUtils(0, 0, 1);
    (void)((task_0 = -1) & 8);
    (void)((task_1 = -1) & 8);
    while (1) {
      if (task_0 == 0) {
        break;
      }
      local_F0 = Script_GetControllerInput(1);
      if (task_1 & 0x10) {
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
    if (task_0 & 8) {
      Script_sicnUtils(1, 0, 0);
      (void)((task_0 = 0) & 2);
    }
  } else {
    Script_sicnUtils(0, 0, 0);
  }
  L0003FE:;
  return;
}

static int32_t script_03FF(void) {
  for (;;) {
    Script_ClearAccumulator_00FE(1, 0x3F000000, 10, 2, 2);
    Script_ClearAccumulator_00FE(3, &data_18534, 8, 9);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_042F(void) {
  Script_ClearAccumulator_00FE(0, "scene/02/model", "C1305S3G.MT5", "C1305S1G.MT5", 1);
  Script_scenCreateTask(script_03FF, 0);
  return;
}

static void script_0455(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_set_val_to_1(3, 0);
  return;
}

static void script_0466(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L000492;
    }
  }
  L000492:;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 5, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_0500(int32_t arg0) {
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

static int32_t script_0550(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_057E(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_05AC(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_05E0(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0614(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0632(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_066C(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_06A0(void) {
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

static void script_0798(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_07C8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4) * 5 + 1);
  *(int32_t *)&local_A4 = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4) * 5 + 2);
  local_98 = 0x425C0000;
  *(int32_t *)&local_A0 = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4) * 5 + 3);
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4) * 5 + 4);
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
      local_88 = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4) * 5);
      local_8C = 18;
      local_90 = 18;
      local_AC = 1.0f;
    } else {
      local_88 = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4) * 5) | -0x80000000;
      local_8C = 12;
      local_90 = 12;
      local_AC = -1.0f;
    }
    local_88 = local_88 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_A8 - 3.0f) << 16) & 0x7FFF0000);
    if (task_128 == arg0) {
      if (task_148) {
        Script_seqEnterEvtCam(arg4 * 4 + 800, arg1, arg1);
      }
    }
    script_4067(arg0, 0);
    local_C4.x = (&task_130)->x;
    (&local_C4)->y = task_130.y;
    (&local_C4)->z = task_130.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMoveTarget(arg0, &task_130, *(&task_13C + 1), local_88);
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
          local_FF78 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
          local_FF78 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_FF78 * 9 + 1) * 4 + 2);
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
        if (task_128 == arg0) {
          if (task_148) {
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
          local_FF74 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
          local_FF74 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_FF74 * 9 + 1) * 4 + 3);
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
  script_4067(arg0, 1);
  if (task_128 == arg0) {
    if (task_148) {
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
    local_88 = task_4C[arg2];
    local_FF78 = arg3;
    if ((local_FF78 == 9) | (local_FF78 == 10) | (local_FF78 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_DC, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_D0, 0, CHARA_PART_Set);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_DC);
      }
      if (task_128 == arg0) {
        local_90 = (Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_90, 1);
        local_90 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_90, 1);
      }
    } else if ((local_FF78 == 18) | (local_FF78 == 19) | (local_FF78 == 21) | (local_FF78 == 22)) {
      local_90 = (Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_90, 1);
      {
        int32_t local_FF74;
        local_FF74 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 1);
        local_FF74 = local_FF74 | 8;
        Script_ScenDataUtil(task_4, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
      }
      {
        int32_t local_FF74;
        local_FF74 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 1);
        local_FF74 = local_FF74 | 0x10;
        Script_ScenDataUtil(task_4, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
      }
      {
        int32_t local_FF74;
        local_FF74 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 1);
        local_FF74 = local_FF74 | 2;
        Script_ScenDataUtil(task_4, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
      }
    } else {
      if ((local_FF78 == 20) | (local_FF78 == 23)) {
        if ((arg4 == 0) | (arg4 == 1)) {
          local_90 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_90, 1);
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 0x10;
            Script_ScenDataUtil(task_4, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 4;
            Script_ScenDataUtil(task_4, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
        } else {
          local_90 = (Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_90, 1);
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 8;
            Script_ScenDataUtil(task_4, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 4;
            Script_ScenDataUtil(task_4, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
        }
        goto L00140C;
      }
    }
  }
  L00140C:;
  return;
}

static void script_140D(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_128 == arg0) {
    local_FC = task_4C[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_FC * 6 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_4, SCND_ReadU32, local_FC * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_1479(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_130)->x;
  (&local_AC)->y = task_130.y;
  (&local_AC)->z = task_130.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
    local_80 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_80 * 9 + 1) * 4 + 2);
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
          task_78[arg2] = arg0;
          local_98 = -local_98;
          local_84 = 0;
        }
        local_8C = 0;
      } else {
        if (local_8C == 0) {
          int32_t local_80;
          local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
          local_80 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
          if (local_80 != -1) {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
          local_93 = 0;
          local_8C = 1;
          local_98 = -local_98;
          local_84 = 0;
          task_78[arg2] = 0;
        }
      }
    } else {
      if (local_8C == 0) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
        local_80 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_78[arg2] = 0;
      }
    }
    if (task_128 == arg0) {
      local_88 = task_4C[arg2];
      local_88 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_88 * 6 + 2) & 0xFFFF;
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
        task_78[arg2] = 0;
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
          goto L001915;
        }
      }
    }
    L001915:;
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

static void script_1965(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5);
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
          local_FC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5);
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
          local_FC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          break;
      }
      break;
  }
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 2);
  local_BC = 0.0f;
  script_4067(arg0, 0);
  local_CC.x = (&task_130)->x;
  (&local_CC)->y = task_130.y;
  (&local_CC)->z = task_130.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMoveTarget(arg0, &task_130, *(&task_13C + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_A0 * 9 + 1) * 4 + 2);
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
  script_4067(arg0, 1);
  local_FC = task_4C[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_FC * 6 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_4, SCND_WriteU32, local_FC * 6 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_FC * 6 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_4, SCND_WriteU32, local_FC * 6 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_FC * 6 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_4, SCND_WriteU32, local_FC * 6 + 1, local_A0);
  }
  return;
}

static void script_1F51(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
        local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5);
        local_C0 = 1.0f;
        local_A8 = 18;
        local_A4 = 18;
        local_98 = 15473;
        local_94 = 17294;
        break;
      case 0:
        local_CF = 0;
        local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
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
        local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
        local_C0 = 1.0f;
        local_A8 = 12;
        local_A4 = 18;
        local_98 = 50062;
        local_94 = 48241;
        break;
      case 0:
        local_CF = 1;
        local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5);
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
        local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
        local_C0 = -1.0f;
        local_A8 = 12;
        local_A4 = 12;
        local_98 = 50062;
        local_94 = 48241;
        break;
      case 0:
        local_CF = 0;
        local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5);
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
        local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5);
        local_C0 = -1.0f;
        local_A8 = 18;
        local_A4 = 12;
        local_98 = 15473;
        local_94 = 17294;
        break;
      case 0:
        local_CF = 1;
        local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
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
          local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          local_C0 = 1.0f;
          local_A8 = 18;
          local_A4 = 18;
          local_98 = 48241;
          local_94 = 50062;
          break;
        case 1:
          local_CF = 0;
          local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          local_C0 = 1.0f;
          local_A8 = 12;
          local_A4 = 18;
          local_98 = 50062;
          local_94 = 48241;
          break;
        case 2:
          local_CF = 0;
          local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          local_C0 = -1.0f;
          local_A8 = 18;
          local_A4 = 12;
          local_98 = 15473;
          local_94 = 17294;
          break;
        case 3:
          local_CF = 1;
          local_AC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          local_C0 = -1.0f;
          local_A8 = 12;
          local_A4 = 12;
          local_98 = 17294;
          local_94 = 15473;
          break;
      }
      local_CE = 1;
      goto L002482;
    }
  }
  L002482:;
  *(int32_t *)&local_BC = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 1);
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 2);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 3);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 4);
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
  script_4067(arg0, 0);
  local_D0.x = (&task_130)->x;
  (&local_D0)->y = task_130.y;
  (&local_D0)->z = task_130.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMoveTarget(arg0, &task_130, *(&task_13C + 1), local_AC);
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
        local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
        local_84 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_84 * 9 + 1) * 4 + 2);
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
        local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
        local_84 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_84 * 9 + 1) * 4 + 3);
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
  script_4067(arg0, 1);
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

static void script_2A5E(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5) {
  int32_t local_FC;
  task_4 = arg1;
  task_8 = arg2;
  task_C = arg3;
  task_10 = arg4;
  task_14.mode = 0;
  task_14.id = 0;
  task_14.entry_idx = 0;
  task_128 = arg0;
  task_14C = 0;
  {
    int32_t local_F8;
    local_F8 = 0;
    L002AAB:;
    if (local_F8 == 10) {
      local_FC = Script_AddCharacterHandler(1, 0, 0x40786F62);
    } else {
      local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
    }
    Script_scenToggleVisibleFor(local_FC, 0);
    task_D0[local_F8] = local_FC;
    task_20[local_F8] = -1;
    task_4C[local_F8] = -1;
    task_78[local_F8] = 0;
    task_A4[local_F8] = 0;
    task_FC[local_F8] = 0;
    local_F8 = local_F8 + 1;
    if (!(local_F8 >= 11)) goto L002AAB;
  }
  {
    int32_t local_F8;
    local_F8 = 0;
    for (;;) {
      if (Script_ScenDataUtil(task_4, SCND_ReadU32, local_F8 * 6) == -1) {
        break;
      }
      Script_ScenDataUtil(task_4, SCND_WriteU32, local_F8 * 6 + 1, 0);
      local_F8 = local_F8 + 1;
    }
  }
  task_148 = 0;
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_14, task_20, task_4C, (shen_name *)task_78, arg0, arg5, data_19100, data_19174);
  Script_scenCreateTask(script_2F1E, 0);
  return;
}

static void script_2BC6(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_4 = arg0;
  task_8 = arg1;
  task_C = arg2;
  task_10 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_14C = 1;
  return;
}

static void script_2BFA(void) {
  int32_t local_FC;
  local_FC = 0;
  L002C02:;
  task_20[local_FC] = -1;
  local_FC = local_FC + 1;
  if (!(local_FC >= 11)) goto L002C02;
  return;
}

static void script_2C22(void) {
  return;
}

static void script_2C25(int32_t arg0) {
  return;
}

static void script_2C28(int32_t arg0, int32_t arg1) {
  DSWK_DoorUtils(DOOR_CMD_ALLOC_ENTRY_SLOT, arg0, arg1);
  return;
}

static void script_2C35(int32_t arg0, int32_t arg1) {
  return;
}

static void script_2C38(int32_t arg0, int32_t arg1) {
  DSWK_DoorUtils(DOOR_CMD_ALLOC_ENTRY_SLOT, arg0, arg1);
  return;
}

static void script_2C45(int32_t arg0) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F4);
  return;
}

static void script_2C5E(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_4, SCND_ReadU32, local_FC * 6) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_FC * 6 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_4, SCND_WriteU32, local_FC * 6 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_2CC1(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_4, SCND_ReadU32, local_FC * 6) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_4, SCND_ReadU32, local_FC * 6 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_4, SCND_WriteU32, local_FC * 6 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_2D24(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_4, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_4, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  return;
}

static void script_2D5B(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_4, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_4, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  return;
}

static void script_2D92(int32_t arg0) {
  task_148 = arg0;
  return;
}

static void script_2D9C(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_4C[local_F0] == arg0) {
      local_EC = task_D0[local_F0];
      if (Script_shenGetNode(local_EC, 8) & Script_shenGetNode(local_EC, 13)) {
        if (Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_F0] * 6 + 1) & 0x20) {
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

static void script_2E9E(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_4, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_4, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  script_2D9C(arg0);
  return;
}

static void script_2EDE(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_4, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_4, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  script_2D9C(arg0);
  return;
}

static int32_t script_2F1E(void) {
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
    if (task_14C == 0) {
      local_E8 = local_EC;
      local_E4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_E4 != -1) & -(task_20[local_E4] == 1) & Script_ryoControl(PCCTL_QueryShouldRun) & -(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1)) {
        local_F0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[local_E4], 2);
        local_EC = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 1) * 4 + 1);
        local_C0 = local_EC;
        if ((local_C0 == 17) | (local_C0 == 11) | (local_C0 == 12)) {
          local_CA = 0;
          if ((Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_E4] * 6 + 1) & 2) == 0) {
            if ((Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_E4] * 6 + 1) & 4) == 0) {
              task_20[local_E4] = 4;
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
          if ((Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_E4] * 6 + 1) & 2) == 0) {
            if (((Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_E4] * 6 + 1) & 4) == 0) & (Script_ryoControl(PCCTL_GetTatehiza) == 0)) {
              local_CB = 0;
              local_BC = script_3B1E(task_128, task_D0[local_E4], -1, 0.5f, 0.8f, local_EC);
              Script_mtwkGetSetRotation(task_128, (Vector3i *)local_AC, 0x42000000);
              Script_mtwkGetSetRotation(task_D0[local_E4], &local_A0, 0);
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
            if (-(task_78[local_E4] != task_128) & local_CB) {
              if ((Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_E4] * 6 + 1) & 2) == 0) {
                if ((Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_E4] * 6 + 1) & 4) == 0) {
                  task_20[local_E4] = 3;
                  task_78[local_E4] = task_128;
                  task_14.mode = 1;
                  task_14.entry_idx = task_4C[local_E4];
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
      L003320:;
      local_C0 = task_20[local_F4];
      switch (local_C0) {
        case 5:
          if (script_419D(task_78[local_F4], local_F4, 0) == 0) {
            if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_78[local_F4])) {
              local_BC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[local_F4], 2);
              local_B8 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_BC * 9 + 1) * 4 + 1);
              if (local_B8 == 17) {
              } else {
                script_408F(task_78[local_F4], task_4C[local_F4], local_B8);
              }
              task_FC[local_F4] = Script_scenCreateTask(Script_ScenDataUtil(&data_19228, SCND_ReadScriptOffset, local_B8), 4, task_78[local_F4], task_D0[local_F4], local_F4, local_B8);
              task_A4[local_F4] = task_78[local_F4];
              script_39D4(local_F4);
            } else {
              task_20[local_F4] = 1;
              task_FC[local_F4] = 0;
              task_A4[local_F4] = 0;
              task_78[local_F4] = 0;
            }
          }
          break;
        case 4:
          task_78[local_E4] = task_128;
          task_14.mode = 2;
          task_14.entry_idx = task_4C[local_E4];
          if (script_419D(task_78[local_F4], local_F4, 0) == 0) {
            task_FC[local_F4] = Script_scenCreateTask(Script_ScenDataUtil(&data_19228, SCND_ReadScriptOffset, local_EC), 4, task_78[local_F4], task_D0[local_F4], local_F4, local_EC);
            task_A4[local_F4] = task_78[local_F4];
            script_39D4(local_F4);
          }
          break;
        case 3:
          if (local_EC >= 0) {
            if (script_419D(task_78[local_F4], local_F4, 0) == 0) {
              script_408F(task_78[local_F4], task_4C[local_F4], local_EC);
              task_FC[local_F4] = Script_scenCreateTask(Script_ScenDataUtil(&data_19228, SCND_ReadScriptOffset, local_EC), 4, task_78[local_F4], task_D0[local_F4], local_F4, local_EC);
              task_A4[local_F4] = task_78[local_F4];
              script_39D4(local_F4);
            }
          }
          break;
        case 2:
          script_419D(task_78[local_F4], local_F4, 1);
          local_F0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[local_F4], 2);
          local_FC = task_D0[local_F4];
          if (local_F0 != -1) {
            *(int32_t *)&local_D8.x = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 5);
            *(int32_t *)&(&local_D8)->y = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 6);
            *(int32_t *)&(&local_D8)->z = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 7);
            Script_EVT_SetPosition(local_FC, POS_XYZ, &local_D8);
            local_CC.x = 0;
            (&local_CC)->y = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 8);
            (&local_CC)->z = 0;
            Script_EVT_SetRotation(local_FC, 0x38000000, &local_CC, 0);
            *(int32_t *)&local_D8.x = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 2);
            *(int32_t *)&(&local_D8)->y = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 3);
            *(int32_t *)&(&local_D8)->z = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 4);
            Script_hmdlApplyScale(local_FC, &local_D8);
            Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_C, SCND_ReadScriptOffset, Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 1) * 4), 0);
            Script_dynamicStuff(local_FC, (const char *)Script_ScenDataUtil(task_C, SCND_ReadScriptOffset, Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 1) * 4), 0);
            Script_charaDisplay(local_FC, CDM_Show);
            Script_charaVisUtils(local_FC, 0);
            local_C0 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_F0 * 9 + 1) * 4 + 1);
            if ((local_C0 == 20) | (local_C0 == 23)) {
              if (Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_F4] * 6 + 1) & 0x10) {
                Script_charaPosRotUtils(local_FC, 7, &data_192BC, 0, CHARA_PART_Add);
              } else {
                if (Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_F4] * 6 + 1) & 8) {
                  Script_charaPosRotUtils(local_FC, 12, &data_192C8, 0, CHARA_PART_Add);
                  goto L0037E7;
                }
              }
              L0037E7:;
            } else if ((local_C0 == 18) | (local_C0 == 19) | (local_C0 == 21) | (local_C0 == 22)) {
            } else if ((local_C0 == 25) | (local_C0 == 26)) {
              if (Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_F4] * 6 + 1) & 0x10) {
                Script_mtwkGetSetRotation(local_FC, (Vector3i *)&local_B4, 0);
                *(&local_B4 + 1) = *(&local_B4 + 1) + 15958;
                Script_EVT_SetRotation(local_FC, 0x38000000, (const Vector3i *)&local_B4, 0);
              }
            } else {
              if ((local_C0 == 24) | (local_C0 == 27)) {
                if (Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_F4] * 6 + 1) & 0x10) {
                  Script_mtwkGetSetRotation(local_FC, (Vector3i *)&local_B4, 0);
                  *(&local_B4 + 1) = *(&local_B4 + 1) - 15958;
                  Script_EVT_SetRotation(local_FC, 0x38000000, (const Vector3i *)&local_B4, 0);
                }
                goto L0038C4;
              }
            }
            L0038C4:;
            if (Script_shenGetNode(local_FC, 8) & Script_shenGetNode(local_FC, 13)) {
              if (Script_ScenDataUtil(task_4, SCND_ReadU32, task_4C[local_F4] * 6 + 1) & 0x20) {
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
            task_78[local_F4] = 0;
          } else {
            Script_charaDisplay(local_FC, CDM_Hide);
          }
          break;
      }
      local_F4 = local_F4 + 1;
      if (!(local_F4 >= 11)) goto L003320;
    } else {
      task_14C = 0;
    }
    local_C4 = DSWK_DoorUtils(DOOR_CMD_TIMER_GET_DELTA_SCALED);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_39D4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg0], 1);
  if (local_F8 != -1) {
    local_FC = 0x40786F62;
    *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 9 + 5);
    *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 9 + 6);
    *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 9 + 7);
    Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
    local_E0.x = 0;
    (&local_E0)->y = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 9 + 8);
    (&local_E0)->z = 0;
    Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
    *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 9 + 2);
    *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 9 + 3);
    *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 9 + 4);
    Script_hmdlApplyScale(local_FC, &local_EC);
    Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_C, SCND_ReadScriptOffset, Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 9 + 1) * 4), 0);
    Script_charaDisplay(local_FC, CDM_Show);
    Script_charaVisUtils(local_FC, 0);
  }
  return;
}

static int32_t script_3B1E(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5) {
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
        if ((local_B4 == 1) | (local_B4 == 2)) goto L003C1B;
      }
      L003C1B:;
    } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26)) {
      local_B4 = local_C0;
      if ((local_B4 == 2) | (local_B4 == 1)) {
        arg3 = (float)0 - arg3;
      } else {
        if ((local_B4 == 3) | (local_B4 == 0)) {
          arg3 = (float)0 - arg3;
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
          goto L003CAC;
        }
      }
      L003CAC:;
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
          goto L003DCB;
        }
      }
      L003DCB:;
    } else {
      local_B4 = local_C0;
      if ((local_B4 == 2) | (local_B4 == 1)) {
      } else {
        if ((local_B4 == 3) | (local_B4 == 0)) {
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
          goto L003E15;
        }
      }
    }
  }
  L003E15:;
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
        task_130.x = (&local_F4)->x;
        task_130.y = (&local_F4)->y;
        task_130.z = (&local_F4)->z;
        task_13C = (&local_C4)->x;
        *(&task_13C + 1) = (&local_C4)->y;
        *(&task_13C + 2) = (&local_C4)->z;
        break;
      case 5:
        task_130.x = (&local_F4)->x;
        task_130.y = (&local_F4)->y;
        task_130.z = (&local_F4)->z;
        task_13C = (&local_C4)->x;
        *(&task_13C + 1) = (&local_C4)->y;
        *(&task_13C + 2) = (&local_C4)->z;
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

static void script_4067(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if (arg1) {
      Script_ccowUtils(arg0, CCOW_EnableMap);
    } else {
      Script_ccowUtils(arg0, CCOW_DisableMap);
    }
  }
  return;
}

static void script_408F(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_128 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
    local_F8 = arg2;
    if ((local_F8 == 25) | (local_F8 == 27) | (local_F8 == 24) | (local_F8 == 26) | (local_F8 == 18) | (local_F8 == 19) | (local_F8 == 20) | (local_F8 == 21) | (local_F8 == 22) | (local_F8 == 23)) {
      local_FC = Script_ScenDataUtil(task_4, SCND_ReadU32, arg1 * 6 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 0);
      local_FC = (Script_ScenDataUtil(task_4, SCND_ReadU32, arg1 * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 0);
      goto L004148;
    }
    L004148:;
  } else {
    local_FC = Script_ScenDataUtil(task_4, SCND_ReadU32, arg1 * 6 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_FC, 0);
    local_FC = (Script_ScenDataUtil(task_4, SCND_ReadU32, arg1 * 6 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_FC, 0);
  }
  return;
}

static int32_t script_419D(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = 0;
  if (task_FC[arg1] != 0) {
    if (arg2) {
      if ((task_128 == task_A4[arg1]) & (task_128 != arg0)) {
        task_14.mode = 0;
        Script_PlayCharaMotion(task_128, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_ryoControl(RYO_RestoreControl);
        script_4067(task_A4[arg1], 1);
      }
      Script_srchControl((void *)task_FC[arg1]);
      task_FC[arg1] = 0;
      task_A4[arg1] = 0;
      task_20[arg1] = 1;
      local_FC = 0;
    } else {
      local_FC = 1;
    }
  }
  end:;
}

static void script_4259(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_128 == arg0) {
    task_14.mode = 0;
    if (arg2 == 0) {
      Script_PlayCharaMotion(task_128, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      local_F8 = task_4C[arg1];
      local_FC = Script_ScenDataUtil(task_4, SCND_ReadU32, local_F8 * 6 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_4, SCND_ReadU32, local_F8 * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      goto L0042F6;
    }
    L0042F6:;
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
  }
  task_20[arg1] = 1;
  task_FC[arg1] = 0;
  task_A4[arg1] = 0;
  task_78[arg1] = 0;
  Script_srchControl(0);
  return;
}

static void script_4348(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (task_128 == arg0) {
    task_14.mode = 0;
    Script_PlayCharaMotion(task_128, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
  }
  task_20[arg1] = 1;
  task_FC[arg1] = 0;
  task_A4[arg1] = 0;
  task_78[arg1] = 0;
  Script_srchControl(0);
  return;
}

static void script_43CC(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_4259(arg0, arg2, 0);
  return;
}

static void script_43DC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3B1E(arg0, arg1, -1, 0.0f, 0.5f, -1);
    if (local_FC == 0) {
      local_FC = script_3B1E(arg0, arg1, 5, 0.45f, 0.8f, arg3);
    } else {
      local_FC = script_3B1E(arg0, arg1, 5, 0.45f, 0.8f, arg3);
    }
    script_1F51(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_4259(arg0, arg2, 0);
  return;
}

static void script_446E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3B1E(arg0, arg1, 4, 0.5f, 0.8f, arg3);
    script_1F51(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_4259(arg0, arg2, 0);
  return;
}

static void script_44BB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3B1E(arg0, arg1, 5, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_07C8(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_4348(arg0, arg2, 0);
  return;
}

static void script_4520(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3B1E(arg0, arg1, 5, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_07C8(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_4348(arg0, arg2, 0);
  return;
}

static void script_4585(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3B1E(arg0, arg1, 4, 0.5f, 0.5f, arg3);
    script_07C8(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_4348(arg0, arg2, 0);
  return;
}

static void script_45D2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_3B1E(arg0, arg1, 0, 0.4f, 0.5f, arg3);
  }
  script_4259(arg0, arg2, 0);
  return;
}

static void script_4607(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3B1E(arg0, arg1, 4, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_1479(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_4259(arg0, arg2, 1);
  return;
}

static void script_466C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3B1E(arg0, arg1, 4, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_1479(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_4259(arg0, arg2, 1);
  return;
}

static void script_46D1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_3B1E(arg0, arg1, 4, 0.0f, 0.5f, arg3);
    script_1479(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_4259(arg0, arg2, 1);
  return;
}

static void script_4716(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3B1E(arg0, arg1, -1, 0.4f, 0.5f, arg3);
    if (local_FC == 0) {
      local_FC = script_3B1E(arg0, arg1, 5, 0.5f, 0.8f, arg3);
    } else {
      local_FC = script_3B1E(arg0, arg1, 5, 0.5f, 0.8f, arg3);
    }
    script_1965(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_4348(arg0, arg2, 0);
  return;
}

static void script_47AB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_9B = script_3B1E(arg0, arg1, 5, 0.0f, 0.5f, arg3);
    script_4067(arg0, 0);
    local_FC = Script_ScenDataUtil(data_19174, SCND_ReadU32, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4) * 5);
    local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_AC - 3.0f) << 16) & 0x7FFF0000);
    local_B4.x = (&task_130)->x;
    (&local_B4)->y = task_130.y;
    (&local_B4)->z = task_130.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMoveTarget(arg0, &task_130, *(&task_13C + 1), local_FC);
    while (Script_GetCharaMoveStatus(arg0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    *(int32_t *)&local_AC = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4) * 5 + 1);
    *(int32_t *)&local_A8 = Script_ScenDataUtil(data_19174, SCND_ReadFloatBits, Script_ScenDataUtil(data_19100, SCND_ReadU8, arg3 * 4) * 5 + 2);
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
            local_94 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
            local_94 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_94 * 9 + 1) * 4 + 2);
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
            local_94 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
            local_94 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_94 * 9 + 1) * 4 + 2);
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
    script_4067(arg0, 1);
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
      local_94 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_4C[arg2], 2);
      local_94 = Script_ScenDataUtil(task_C, SCND_ReadU32, Script_ScenDataUtil(task_10, SCND_ReadU32, local_94 * 9 + 1) * 4 + 3);
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
  script_4259(arg0, arg2, 0);
  return;
}

static int32_t script_4E4A(void) {
  end:;
}

static void script_4E4D(void) {
  return;
}

static void script_4E50(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_4E78(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static int32_t script_4ECC(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_4F1C(int32_t arg0) {
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

static void script_4FC3(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_4F1C(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_4FE6(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_5019(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_4FE6(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_503C(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_4FE6(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_509B(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  return (int32_t)local_F8;
}

static void script_50B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_5446(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_5AD4(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    task_174[local_FF] = -1;
    local_FF = local_FF + 1;
  }
  task_1B8 = -1;
  task_1BC = -1;
  task_1C0 = -1;
  task_1A0 = -1;
  (void)((task_1DA = 0) & 2);
  (void)((task_1DB = 0) & 2);
  (void)((task_1DC = 0) & 2);
  (void)((task_1DF = 0) & 2);
  task_1D4 = 0x3F4CCCCD;
  (void)((task_1DD = 0) & 2);
  task_1D9 = -1;
  script_5C66();
  (void)((task_1DE = 0) & 2);
  Script_scenUtils(SCENARIO_CMD_SET_RUNTIME_FLAG_80, 0);
  return;
}

static void script_5B75(void) {
  (void)((task_1DA = -1) & 2);
  while (task_1DA & 1) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_5B97(void) {
  return;
}

static void script_5B9A(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = -10;
  SCNF_CopyScenario_000E(1, task_1B0, (const void *)local_F8, 8);
  return;
}

static int32_t script_5BBB(int32_t arg0) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_150)) {
    if (Script_charaHasMotionwork(task_150) != 0) {
      (void)-1;
      goto L005BE3;
    }
  }
  (void)0;
  L005BE3:;
}

static void script_5BE4(int32_t arg0) {
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
        goto L005C65;
      }
    }
    (void)0;
  }
  L005C65:;
  return;
}

static void script_5C66(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    task_154[local_FF] = -1;
    local_FF = local_FF + 1;
  }
  return;
}

static void script_5C91(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  Script_sprintf("FTLK: Get ID list(%s)\012", script_509B(arg0));
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
      task_154[local_F7] = -1;
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC)) {
        if (Script_charaHasMotionwork(local_FC)) {
          task_154[local_F7] = local_FC;
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

static int8_t script_5D63(int32_t arg0) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    if (task_154[local_FF] == arg0) {
      break;
    }
    local_FF = local_FF + 1;
  }
  return local_FF;
}

static int32_t script_5D98(void) {
  script_5AD4();
  Script_scenUtils(SCENARIO_CMD_CREATE_SEQ_CHARACTERS);
  Script_scenCreateTask(script_5DE3, 0);
  end:;
}

static void script_5DB1(void) {
  script_5AD4();
  Script_scenUtils(SCENARIO_CMD_CREATE_SEQ_CHARACTERS);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_scenCreateTask(script_5DE3, 0);
  return;
}

static void script_5DD0(void) {
  script_5AD4();
  Script_scenUtils(SCENARIO_CMD_CREATE_SEQ_CHARACTERS);
  script_5DE3();
  return;
}

__attribute__((scn_string_bindings(13, data_1965F, 14, data_19673, 15, data_19687)))
static int32_t script_5DE3(void) {
  int32_t local_F8[2];
  int32_t local_F4;
  int32_t local_F0;
  uint8_t unused_EF;
  int8_t local_EE;
  uint8_t unused_EC[2];
  int32_t local_E8;
  task_1AC = SCNF_CopyScenario_000E(0, 0x414E4353);
  task_1B0 = SCNF_CopyScenario_000E(0, 0x464E4353);
  for (;;) {
    SCNF_CopyScenario_000E(2, task_1AC, &task_1A4, 8);
    local_F8[0] = *&task_1A4;
    local_F8[1] = -10;
    task_150 = *&task_1A4;
    local_F4 = *(&task_1A4 + 1);
    task_194 = task_150;
    (void)((local_EE = -(script_5BBB(task_150) != 0)) & 8);
    if ((local_F4 >= 140) & (local_F4 <= 149)) {
      Script_sprintf("FTLK: ATURN0%d start\012", local_F4 - 140);
      script_5C91(task_150);
      script_6866(local_F4);
      Script_sprintf("FTLK: ATURN0%d end\012", local_F4 - 140);
    } else if ((local_F4 >= 40) & (local_F4 <= 49)) {
      Script_sprintf("FTLK: TURN0%d start\012", local_F4 - 40);
      script_5C91(task_150);
      script_66F1(local_F4);
      Script_sprintf("FTLK: TURN0%d end\012", local_F4 - 40);
    } else if ((local_F4 >= 50) & (local_F4 <= 59)) {
      Script_sprintf("FTLK: RETURN0%d start\012", local_F4 - 50);
      script_5C91(task_150);
      script_6983(local_F4);
      Script_sprintf("FTLK: RETURN0%d end\012", local_F4 - 50);
    } else if (((local_F4 >= 20) & (local_F4 <= 29)) | ((local_F4 >= 120) & (local_F4 <= 129))) {
      if (local_F4 < 120) {
        Script_sprintf("FTLK: POINT0%d start\012", local_F4 - 20);
      } else {
        Script_sprintf("FTLK: APOINT0%d start\012", local_F4 - 120);
      }
      script_5C91(task_150);
      script_7035(task_150, local_F4);
      if (local_F4 < 120) {
        Script_sprintf("FTLK: POINT0%d end\012", local_F4 - 20);
      } else {
        Script_sprintf("FTLK: APOINT0%d end\012", local_F4 - 120);
      }
    } else {
      local_E8 = local_F4;
      if (local_E8 == -1) {
        Script_sprintf("FTLK: START start\012");
        *(&task_194 + 1) = 101;
        script_5C91(task_150);
        if (local_EE & -(~task_1DF != 0)) {
          Vector3i object_0;
          Script_mtwkGetSetRotation(task_150, &object_0, 0);
          task_1C8 = (&object_0)->y;
          task_1CC = Script_scenCharaGetMotAnimId(task_150);
          task_1D0 = task_150;
        }
        (void)((task_1DF = -1) & 0x20);
        if (task_1A0 != -1) {
          Script_srchControl((void *)task_1A0);
          Script_sprintf("FTLK: Wait finish canseled(%d)\012", task_1A0);
          task_1A0 = -1;
        }
        task_1A0 = -1;
        if (~task_1DD & ~task_1DE) {
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
        if (~Script_scenUtils(SCENARIO_CMD_QUERY_ACTIVE_FLAG_0x400000) & -(task_1B8 == -1) & local_EE & ~task_1DE) {
          script_6F60(CHARA_AKIR, task_150, data_194D8);
          if (*task_174 == -1) {
            if (task_150 == 0x4952554B) {
              Script_sprintf("FTLK: AKIRA ->> %s\012", script_509B(task_150));
              task_174[0] = Script_scenCreateTask(script_4ECC, 3, CHARA_AKIR, task_150, local_F0);
            } else if (Script_CycleManagerUtils(CYCLE_CMD_SIMSET_GLOBALFLAG_CHECK, task_150) == 1) {
              Script_sprintf("FTLK: AKIRA ->> %s\012", script_509B(task_150));
              task_174[0] = Script_scenCreateTask(script_4ECC, 3, CHARA_AKIR, task_150, local_F0);
            } else {
              Script_sprintf("FTLK: AKIRA -> %s\012", script_509B(task_150));
              if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_150)) {
                Vector3 object_1;
                Script_EVT_GetPosition(task_150, 4, &object_1, ENTITY_XFORM_WORLD_SPACE);
                Script_SetLookPointDetails(CHARA_AKIR, 16, &object_1, local_F0);
                Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &object_1, 30);
              }
            }
          }
        }
        SCNF_CopyScenario_000E(1, task_1B0, (const void *)local_F8, 8);
        script_66AB();
        Script_sprintf("FTLK: START end\012");
      } else if (local_E8 == -2) {
        Script_sprintf("FTLK: END start\012");
        task_194 = -1;
        *(&task_194 + 1) = 102;
        if (local_EE & 0x40) {
          Script_SetRyoMotionDetails(task_150);
        }
        while (task_1BC != -1) {
          SCNF_WaitFrames_0000(1);
        }
        while (task_1DC & -(task_1C0 != -1)) {
          SCNF_WaitFrames_0000(1);
        }
        local_F8[1] = -2;
        SCNF_CopyScenario_000E(1, task_1B0, (const void *)local_F8, 8);
        (void)((task_1DE = 0) & 0x40);
        script_5C91(task_150);
        script_668B();
        Script_sprintf("FTLK: END end\012");
      } else if (local_E8 == -8) {
        Script_sprintf("FTLK: FINISH start\012");
        if ((task_150 == 0x4155524D) | (task_150 == 0x444E4953)) {
          Script_PlayCharaMotion(task_150, task_1CC, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        } else {
          if (task_1DF & 0x20) {
            script_6B53(task_150);
            goto L0062E8;
          }
        }
        L0062E8:;
        local_F8[1] = -8;
        Script_sprintf("FTLK: FINISH send (%s)\012", script_509B(task_150));
        SCNF_CopyScenario_000E(1, task_1B0, (const void *)local_F8, 8);
        script_66CE();
        (void)((task_1DF = 0) & 1);
        *(&task_194 + 1) = 103;
        Script_sprintf("FTLK: FINISH end\012");
      } else if (local_E8 == -3) {
        *(&task_194 + 1) = 104;
      } else if (local_E8 == -4) {
        *(&task_194 + 1) = 105;
      } else if (local_E8 == -5) {
        *(&task_194 + 1) = 106;
      } else if (local_E8 == -6) {
        *(&task_194 + 1) = 107;
      } else if (local_E8 == -7) {
        *(&task_194 + 1) = 108;
      } else if (local_E8 == 13) {
        Script_sprintf("FTLK: POINT_END star\012");
        script_769C(task_150);
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        }
        Script_sprintf("FTLK: POINT_END end\012");
        SCNF_CopyScenario_000E(1, task_1B0, (const void *)local_F8, 8);
      } else if (local_E8 == 14) {
        Script_sprintf("FTLK: APOINT_END start\012");
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_150)) {
          Script_SetCharaLookAtPosition(task_150, -13, 0, 0);
          Script_FaceLookAtTargetUtils(task_150, FACE_LookReset);
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        }
        Script_sprintf("FTLK: APOINT_END end\012");
        SCNF_CopyScenario_000E(1, task_1B0, (const void *)local_F8, 8);
      } else if (local_E8 == 10) {
        Script_sprintf("FTLK: SHOW_MEMO start\012");
        script_78FE();
        Script_sprintf("FTLK: SHOW_MEMO end\012");
        SCNF_CopyScenario_000E(1, task_1B0, (const void *)local_F8, 8);
      } else if (local_E8 == 15) {
        Script_sprintf("FTLK: SHOW_END start\012");
        script_7A3A();
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_150)) {
          Vector3 object_2;
          Script_EVT_GetPosition(task_150, 4, &object_2, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_AKIR, 16, &object_2, 2);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &object_2, 30);
        }
        Script_sprintf("FTLK: SHOW_END end\012");
        SCNF_CopyScenario_000E(1, task_1B0, (const void *)local_F8, 8);
      } else if (local_E8 == 60) {
        Script_sprintf("FTLK: WAIT start\012");
        SCNF_WaitFrames_0000(60);
        Script_sprintf("FTLK: WAIT end\012");
        SCNF_CopyScenario_000E(1, task_1B0, (const void *)local_F8, 8);
      } else {
        Script_sprintf("FTLK: EXTRA start\012", local_F4);
        *(&task_194 + 1) = 109;
        *(&task_194 + 2) = local_F4;
        script_5B75();
        *(&task_194 + 2) = -1;
        Script_sprintf("FTLK: EXTRA end\012", local_F4);
        SCNF_CopyScenario_000E(1, task_1B0, (const void *)local_F8, 8);
      }
    }
    *(&task_194 + 1) = -1;
  }
  end:;
}

static void script_65BF(void) {
  (void)((task_1DA = 0) & 0x80);
  return;
}

static void script_65CD(void) {
  (void)*&task_194;
  return;
}

static void script_65D5(void) {
  (void)*(&task_194 + 1);
  return;
}

static int32_t script_65E0(void) {
  return *(&task_194 + 2);
}

static int32_t script_65EB(void) {
  int32_t unused_FC;
  for (;;) {
    if (~task_1DF | (Script_GetControllerInput(0) & 0xF244) | (Script_GetControllerInput(5) != 128) | (Script_GetControllerInput(6) != 128)) {
      script_73B5();
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
        Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      }
      Script_SetMotnFlag(CHARA_AKIR, 0);
      script_66CE();
      script_5C66();
      Script_sprintf("FTLK: Wait finish end\012");
      task_1A0 = -1;
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_668B(void) {
  task_1A0 = Script_scenCreateTask(script_65EB, 0);
  Script_sprintf("FTLK: Wait finish start(%d)\012", task_1A0);
  return;
}

static void script_66AB(void) {
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  return;
}

static void script_66CE(void) {
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  return;
}

static void script_66F1(int32_t arg0) {
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
    local_FC = task_154[local_F2];
    local_F8 = task_154[local_F1];
  }
  if ((Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC) == 0) | (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_F8) == 0)) {
    script_5B9A(task_150);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC) != 0) {
      local_FC = local_F8;
    }
    Script_sprintf("FTLK: %s not exists\012", script_509B(local_FC));
  } else {
    int32_t unused_F4;
    int8_t local_F3;
    local_F3 = script_5D63(local_FC);
    if (task_174[local_F3] != -1) {
      Script_srchControl((void *)task_174[local_F3]);
    }
    Script_sprintf("FTLK: %s ->> ", script_509B(local_FC));
    Script_sprintf("%s\012", script_509B(local_F8));
    task_174[local_F3] = Script_scenCreateTask(script_4ECC, 3, local_FC, local_F8, 2);
    script_69EE(local_FC, local_F8);
    if ((local_FC != 0x4155524D) & (local_FC != 0x444E4953)) {
      Script_scenCreateTask(script_6ADC, 1, local_FC);
    } else {
      Script_PlayCharaMotion(local_FC, Script_CycleManagerUtils(CYCLE_CMD_GET_VALUE_BY_CYCLE_ID, local_FC), -1.0f, -1.0f, -1.0f, 1.0f, 0);
    }
    script_5B9A(task_150);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_19859, 1, data_19869)))
static void script_6866(int32_t arg0) {
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
  script_73A9();
  local_FC = task_154[arg0 - 140 + 1];
  if ((local_FC != -1) & (*task_174 == -1)) {
    Script_sprintf("FTLK: AKIRA -> %s\012", script_509B(local_FC));
    if (Script_CycleManagerUtils(CYCLE_CMD_SIMSET_GLOBALFLAG_CHECK, local_FC) == 1) {
      task_174[0] = Script_scenCreateTask(script_4ECC, 3, CHARA_AKIR, local_FC, local_F8);
    } else {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC)) {
        Script_EVT_GetPosition(local_FC, 4, &local_EC, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(CHARA_AKIR, 16, &local_EC, local_F8);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 30);
      }
    }
  } else {
    Script_sprintf("FTLK: AKIRA -X %s\012", script_509B(local_FC));
  }
  script_5B9A(task_150);
  return;
}

static void script_6983(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0 - 50 + 1;
  local_FC = task_154[local_F8];
  if (task_174[arg0 - 50 + 1] != -1) {
    script_731B(local_F8);
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC)) {
      Script_SetCharaLookAtPosition(local_FC, -13, 0, 0);
      Script_FaceLookAtTargetUtils(local_FC, FACE_LookReset);
    }
  }
  script_5B9A(task_150);
  return;
}

static void script_69EE(int32_t arg0, int32_t arg1) {
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
    task_1C4 = (&local_DC)->y;
  } else if (local_D0 < 0) {
    local_D4 = 0x10000 - local_D4;
    task_1C4 = ((&local_DC)->y - local_D4 * 6 / 10) & 0xFFFF;
  } else {
    task_1C4 = (local_D4 * 6 / 10 + (&local_DC)->y) & 0xFFFF;
  }
  (void)0;
  return;
}

static int32_t script_6ADC(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_VALUE_BY_CYCLE_ID, arg0);
  if ((local_FC == -1) | (local_FC == 0)) {
    local_FC = Script_scenCharaGetMotAnimId(arg0);
  }
  Script_sprintf("FTLK: %s 's talk motion is %d\012", script_509B(arg0), local_FC);
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_SIMSET_MODE01, arg0) == 0) {
    script_6B9B(arg0, task_1C4, 1043, local_FC);
  } else {
    script_6B9B(arg0, task_1C4, 32514, local_FC);
  }
  end:;
}

static void script_6B53(int32_t arg0) {
  if (arg0 != task_1D0) {
  } else if (Script_CycleManagerUtils(CYCLE_CMD_GET_SIMSET_MODE01, arg0) == 0) {
    script_6B9B(arg0, task_1C8, 1043, task_1CC);
  } else {
    script_6B9B(arg0, task_1C8, 32514, task_1CC);
  }
  return;
}

static void script_6B9B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  (void)((task_1DB = -1) & 1);
  *(int32_t *)&local_D8 = task_1D4;
  task_1D4 = 0x3F4CCCCD;
  local_D4 = 1.0f / local_D8;
  if (~Script_scenCheckCharaExist(arg0)) {
    Script_sprintf("FTLK: Turn canseled 1\012");
    (void)((task_1DB = 0) & 1);
  } else {
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    if ((&local_E8)->y == arg1) {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_18);
      Script_PlayCharaMotion(arg0, arg3, 0.0f, 0.0f, 0.0f, 1.0f, 0);
      Script_sprintf("FTLK: Set motion %d to %s 1\012", arg3, script_509B(arg0));
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
        local_E4 = script_6EE8((&local_E8)->y - 0x4000, arg1);
      } else {
        local_E4 = script_6EE8((&local_E8)->y + 0x4000, arg1);
      }
      SCNF_WaitFrames_0000((int32_t)((float)local_DC / 2.0f));
      if (~Script_scenCheckCharaExist(arg0)) {
        Script_sprintf("FTLK: Turn canseled 2\012");
        (void)((task_1DB = 0) & 0x10);
        goto L006EE7;
      }
      Script_motmUtilsEx(6, arg0, local_E4);
      Script_motmToggleFlags(arg0, SCEN_Disable, 4);
      Script_motmUtilsEx(0, arg0, 1);
      Script_PlayCharaMotion(arg0, arg2, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = (float)local_FC - local_E0}).i, ((scn3_bits){.f = local_E0}).i, local_D8, 0);
      if (Script_motmUtilsEx(7, 0, arg3)) {
        SCNF_WaitFrames_0000((int32_t)((float)local_FC * local_D4) - (int32_t)local_E0 * 2);
        if (~Script_scenCheckCharaExist(arg0)) {
          Script_sprintf("FTLK: Turn canseled 3\012");
          (void)((task_1DB = 0) & 0x10);
          goto L006EE7;
        }
        Script_PlayCharaMotion(arg0, arg3, 0.0f, 0.0f, 0.0f, local_D8, 0);
        Script_sprintf("FTLK: Set motion %d to %s 2\012", arg3, script_509B(arg0));
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
          (void)((task_1DB = 0) & 2);
          goto L006EE7;
        }
        Script_motmUtilsEx(0, arg0, 1);
        Script_PlayCharaMotion(arg0, arg3, 0.0f, 0.0f, 0.0f, local_D8, 0);
        Script_sprintf("FTLK: Set motion %d to %s 3\012", arg3, script_509B(arg0));
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
          (void)((task_1DB = 0) & 1);
          goto L006EE7;
        }
        Script_motmUtilsEx(0, arg0, local_F8);
      }
    }
    (void)((task_1DB = 0) & 1);
  }
  L006EE7:;
  return;
}

static int32_t script_6EE8(int32_t arg0, int32_t arg1) {
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

static void script_6F60(int32_t arg0, int32_t arg1, const uint16_t *arg2) {
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

static void script_7035(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  int32_t local_D8;
  int32_t local_D4;
  (void)((task_1DC = -1) & 0x10);
  {
    int32_t local_D0;
    local_D0 = script_5D63(arg0);
    if (local_D0 != 8) {
      script_731B(local_D0);
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
    script_5B9A(task_150);
  } else {
    *(int32_t *)&local_E4 = Script_ScenDataUtil((void *)local_D4, SCND_ReadU32, 0);
    *(int32_t *)&local_E0 = Script_ScenDataUtil((void *)local_D4, SCND_ReadU32, 1);
    *(int32_t *)&local_DC = Script_ScenDataUtil((void *)local_D4, SCND_ReadU32, 2);
    Script_sprintf("FTLK: POINT TO (%f, %f, %f)\012", local_E4, local_E0, local_DC);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    local_D8 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, local_E4 - (&local_F4)->x, local_DC - (&local_F4)->z);
    local_D8 = script_6EE8((&local_E8)->y, local_D8) & 0xFFFF;
    if (local_D8 < 0x8000) {
      task_1C4 = (&local_E8)->y + local_D8 - 0x4000;
    } else {
      task_1C4 = (&local_E8)->y + local_D8 + 0x4000;
    }
    if ((local_D8 >= 21845) & (local_D8 <= 43690)) {
      Script_scenCreateTask(script_6ADC, 1, arg0);
      task_1D4 = 0x3FC00000;
      SCNF_WaitFrames_0000(1);
    }
    script_5B9A(task_150);
    while (task_1DB & 2) {
      SCNF_WaitFrames_0000(1);
    }
    (void)((task_1DB = 0) & 0x10);
    task_1D4 = 0x3F4CCCCD;
    if (arg1 < 120) {
      script_7678(arg0, 15, ((scn3_bits){.f = local_E4}).i, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = local_DC}).i, 2, 75, -1);
    } else {
      script_747A(arg0, ((scn3_bits){.f = local_E4}).i, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = local_DC}).i, 75);
    }
    SCNF_WaitFrames_0000(30);
    script_73A9();
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
      Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    }
    if (task_1DD & 8) {
      script_747A(CHARA_AKIR, ((scn3_bits){.f = local_E4}).i, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = local_DC}).i, 75);
    } else {
      script_7274(CHARA_AKIR, local_E4, local_E0, local_DC, data_194D8);
    }
  }
  return;
}

static void script_7274(int32_t arg0, float arg1, float arg2, float arg3, const uint16_t *arg4) {
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
  (void)((task_1DC = 0) & 0x10);
  return;
}

static void script_731B(int32_t arg0) {
  if (task_174[arg0] == -1) {
  } else {
    Script_srchControl((void *)task_174[arg0]);
    SCNF_WaitFrames_0000(1);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_154[arg0])) {
      Script_SetCharaLookAtPosition(task_154[arg0], -13, 0, 0);
      Script_FaceLookAtTargetUtils(task_154[arg0], FACE_LookReset);
    }
    Script_sprintf("FTLK: %s finished looking\012", script_509B(task_154[arg0]));
    task_174[arg0] = -1;
  }
  return;
}

static void script_73A9(void) {
  script_731B(0);
  return;
}

static void script_73B5(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    script_731B(local_FF);
    local_FF = local_FF + 1;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_199CE, 1, data_199E2)))
static int32_t script_73DC(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
    Script_sprintf("FTLK: %s not exist\012", script_509B(arg0));
  } else {
    Script_SetCharaLookAtPosition(arg0, 10, &local_F4, 0);
    SCNF_WaitFrames_0000(arg4);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("FTLK: %s not exist\012", script_509B(arg0));
    } else {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
        Script_SetCharaLookAtPosition(arg0, -13, 0, 0);
        Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
      }
    }
  }
  end:;
}

static void script_747A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_73DC, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

__attribute__((scn_string_bindings(0, data_199F6, 1, data_19A0A, 2, data_19A1A)))
static int32_t script_7492(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5, int32_t arg6, int32_t arg7) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int8_t local_E7;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
    Script_sprintf("FTLK: %s not exist\012", script_509B(arg0));
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
      Script_sprintf("FTLK: %s not exist\012", script_509B(arg0));
    } else {
      script_769C(arg0);
      if (local_EC == -3) {
        Script_CharHandlers(arg0, -1, 0, 0);
      } else {
        Script_CharHandlers(arg0, local_F0, local_EC, 20);
      }
      if (arg7 != -1) {
        Script_PlayCharaMotion(arg0, arg7, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (task_1DF & 0x20) {
        if (arg0 == 0x52494B41) {
          if (*task_174 == -1) {
            task_174[0] = Script_scenCreateTask(script_4ECC, 3, CHARA_AKIR, task_150, local_E8);
          }
        } else {
          local_E7 = script_5D63(arg0);
          if (task_174[local_E7] == -1) {
            task_174[local_E7] = Script_scenCreateTask(script_4ECC, 3, arg0, CHARA_AKIR, 2);
          }
        }
      }
      task_1C0 = -1;
    }
  }
  end:;
}

static void script_7678(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7) {
  task_1C0 = Script_scenCreateTask(script_7492, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
  return;
}

static void script_769C(int32_t arg0) {
  Script_HandleLookPoint(arg0, -98312, 0, 2);
  return;
}

static void script_76AF(float arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = arg2;
  Script_convCtrlCamApply(1);
  Script_RunAuthorAndSetState(0, arg3, 0, 0, 0, &local_F4, 0, 0.0f, 0, 0);
  return;
}

static void script_76EA(const Vector3 *arg0, float arg1) {
  Script_convCtrlCamApply(1);
  Script_RunAuthorAndSetState(0, arg1, 0, 0, 0, arg0, 0, 0.0f, 0, 0);
  return;
}

static void script_770D(void) {
  Script_convCtrlCamApply(0);
  return;
}

static void script_7717(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  task_1D8 = arg1;
  local_FC = arg1;
  if (local_FC == 1) {
    task_1B4 = arg0;
    Script_SetImageModuleTaskDataFlag(arg0, 0);
  } else {
    task_1D8 = 0;
  }
  return;
}

static int32_t script_774A(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  if (task_1DD & 8) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_CHUGOSI_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_MID_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  if (task_1D8 == 1) {
    local_F4.x = 0.091f;
    (&local_F4)->y = 0.052f;
    (&local_F4)->z = 0.016f;
    local_E8.x = 0xA000;
    (&local_E8)->y = 0x5000;
    (&local_E8)->z = 0x5000;
    SCNF_WaitFrames_0000(30);
    if (0) {
      Script_charaDisplay(task_1B4, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_1B4, CDM_Show);
        goto L00784E;
      }
    }
    L00784E:;
    Script_scenToggleVisibleFor(task_1B4, 1);
    Script_fixoAttachEx(task_1B4, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  if (task_1DD & 8) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_CHUGOSI_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  return task_1B8 = -1;
}

static void script_78FE(void) {
  task_1B8 = Script_scenCreateTask(script_774A, 0);
  return;
}

static int32_t script_7912(void) {
  if (task_1B8 != -1) {
    Script_srchControl((void *)task_1B8);
    task_1B8 = -1;
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 4);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
  if (task_1DD & 8) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_CHUGOSI_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_MID_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  if (task_1B4 != 0) {
    SCNF_WaitFrames_0000(25);
    Script_fixoClearAttachment(task_1B4);
    if (-1) {
      Script_charaDisplay(task_1B4, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_1B4, CDM_Show);
        goto L0079E7;
      }
    }
    L0079E7:;
    Script_scenToggleVisibleFor(task_1B4, 0);
  }
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return task_1BC = -1;
}

static void script_7A3A(void) {
  task_1BC = Script_scenCreateTask(script_7912, 0);
  return;
}

static int32_t script_7A4E(int32_t arg0) {
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 8);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_184E0, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_7B2A();
  end:;
}

static int32_t script_7ACF(void) {
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_184E0, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 8);
  end:;
}

static void script_7B2A(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  for (;;) {
    Script_EVT_GetPosition(task_150, 4, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_E8)->y - (&local_F4)->y <= 0.3f)) {
      script_7BCC();
      break;
    }
    if (~task_1DD) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_7B84(int32_t arg0) {
  if (task_1DD & 8) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_scenCreateTask(script_7A4E, 1, arg0);
    (void)((task_1DD = -1) & 0x10);
  }
  return;
}

static void script_7BCC(void) {
  Script_scenCreateTask(script_7ACF, 0);
  (void)((task_1DD = 0) & 0x10);
  return;
}

static void script_7BE4(int32_t arg0) {
  Script_FACE(arg0, 7, 5, 30);
  SCNF_WaitFrames_0000(30);
  Script_FACE(arg0, 0, -1, 12);
  return;
}

static void script_7C09(int32_t arg0) {
  Script_FACE(arg0, 2, 0, 60);
  SCNF_WaitFrames_0000(60);
  Script_FACE(arg0, 0, -1, 12);
  return;
}

static int32_t script_7C2E(int32_t arg0, int32_t arg1) {
  Script_FACE(arg0, 2, 2, 60);
  SCNF_WaitFrames_0000(arg1);
  Script_FACE(arg0, 0, -1, 12);
  end:;
}

static void script_7C53(void) {
  Script_ShowUIMessageFromID("E1004AA001");
  return;
}

static void script_7C5F(void) {
  return;
}

static void script_7C62(void) {
  Script_ShowUIMessageFromID("E1004AB001");
  return;
}

static void script_7C6E(void) {
  Script_ShowUIMessageFromID("E1004AA002");
  return;
}

static void script_7C7A(void) {
  Script_ShowUIMessageFromID("E1004AB002");
  return;
}

static int32_t script_7C86(void) {
  return Script_ShowUIMessageFromID("E1004AA003");
}

static int32_t script_7C92(void) {
  return Script_ShowUIMessageFromID("TE1004AA003");
}

static int32_t script_7C9E(void) {
  return Script_ShowUIMessageFromID("TE1004AD001");
}

static void script_7CAA(void) {
  return;
}

static int32_t script_7CAD(void) {
  return Script_ShowUIMessageFromID("TE1004AA004");
}

static int32_t script_7CB9(void) {
  return Script_ShowUIMessageFromID("TE1004AE001");
}

static void script_7CC5(void) {
  Script_ShowUIMessageFromID("TE1004AA006");
  return;
}

__attribute__((scn_string_bindings(0, data_19AEF)))
static int32_t script_7CD1(void) {
  return Script_ShowUIMessageFromID("TE1004AA005");
}

__attribute__((scn_string_bindings(0, data_19AFB)))
static int32_t script_7CDD(void) {
  return Script_ShowUIMessageFromID("TE1004AA026");
}

static void script_7CE9(void) {
  SCNT_ShowSubtitle("「よっと！」");
  return;
}

static void script_7CF1(void) {
  SCNT_ShowSubtitle("「おわっと！」");
  return;
}

static int32_t script_7CF9(void) {
  SCNF_WaitFrames_0000(30);
  return Script_ShowUIMessageFromID("TE1004AI001");
}

static int32_t script_7D0D(void) {
  return Script_ShowUIMessageFromID("TE1004AE012");
}

static int32_t script_7D19(void) {
  return Script_ShowUIMessageFromID("TE1004AA010");
}

static int32_t script_7D25(void) {
  return Script_ShowUIMessageFromID("TE1004AA011");
}

static void script_7D31(void) {
  SCNT_ShowSubtitle("「クソ、ジミーの野郎」");
  return;
}

static void script_7D39(void) {
  Script_ShowUIMessageFromID("TE1004AA024");
  return;
}

static int32_t script_7D45(void) {
  float local_FC;
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
  if (!(local_FC >= 0.33f)) {
    Script_ShowUIMessageFromID("TE1004AA012");
  } else if (!(local_FC >= 0.66f)) {
    Script_ShowUIMessageFromID("TE1004AA013");
  } else {
    Script_ShowUIMessageFromID("TE1004AA014");
  }
  end:;
}

static int32_t script_7D91(void) {
  return Script_ShowUIMessageFromID("TE1004AA015");
}

static int32_t script_7D9D(void) {
  return Script_ShowUIMessageFromID("TE1004AJ002");
}

static int32_t script_7DA9(void) {
  return Script_ShowUIMessageFromID("TE1004AJ001");
}

static int32_t script_7DB5(void) {
  return Script_ShowUIMessageFromID("TE1004AE003");
}

static void script_7DC1(void) {
  return;
}

static int32_t script_7DC4(void) {
  return Script_ShowUIMessageFromID("TE1004AG002");
}

__attribute__((scn_string_bindings(0, data_19BD6)))
static int32_t script_7DD0(void) {
  return Script_ShowUIMessageFromID("TE1004AA023");
}

static int32_t script_7DDC(void) {
  return Script_ShowUIMessageFromID("TE1004AL002");
}

static int32_t script_7DE8(void) {
  return Script_ShowUIMessageFromID("TE1004AA017");
}

static int32_t script_7DF4(void) {
  return Script_ShowUIMessageFromID("TE1004AL003");
}

static void script_7E00(void) {
  Script_ShowUIMessageFromID("E1004AH001");
  return;
}

__attribute__((scn_string_bindings(0, data_19C11)))
static void script_7E0C(void) {
  Script_ShowUIMessageFromID("TE1004AC001");
  return;
}

static void script_7E18(void) {
  Script_ShowUIMessageFromID("E1004AH003");
  return;
}

static int32_t script_7E24(void) {
  return Script_ShowUIMessageFromID("TE1004AA025");
}

static int32_t script_7E30(void) {
  return Script_ShowUIMessageFromID("TE1004AE004");
}

__attribute__((scn_string_bindings(0, data_19C40)))
static int32_t script_7E3C(void) {
  return Script_ShowUIMessageFromID("TE1004AA026");
}

static int32_t script_7E48(void) {
  return Script_ShowUIMessageFromID("TE1004AN002");
}

static int32_t script_7E54(void) {
  return Script_ShowUIMessageFromID("TE1004AN001");
}

__attribute__((scn_string_bindings(0, data_19C64)))
static void script_7E60(void) {
  Script_ShowUIMessageFromID("TE1004AO002");
  return;
}

static void script_7E6C(void) {
  Script_ShowUIMessageFromID("TE1004AO004");
  return;
}

static int32_t script_7E78(void) {
  return Script_ShowUIMessageFromID("TE1004AA008");
}

__attribute__((scn_string_bindings(0, data_19C88)))
static int32_t script_7E84(void) {
  return Script_ShowUIMessageFromID("TE1004AO002");
}

__attribute__((scn_string_bindings(0, data_19C94)))
static int32_t script_7E90(void) {
  return Script_ShowUIMessageFromID("TE1004AA023");
}

static int32_t script_7E9C(void) {
  return Script_ShowUIMessageFromID("TE1004AA016");
}

__attribute__((scn_string_bindings(0, data_19CAC)))
static int32_t script_7EA8(void) {
  return Script_ShowUIMessageFromID("TE1004AG001");
}

static int32_t script_7EB4(void) {
  return Script_ShowUIMessageFromID("TE1004AA001");
}

static int32_t script_7EC0(void) {
  return Script_ShowUIMessageFromID("TE1004AB002");
}

static int32_t script_7ECC(void) {
  return Script_ShowUIMessageFromID("TE1004AC002");
}

__attribute__((scn_string_bindings(0, data_19CDC)))
static int32_t script_7ED8(void) {
  return Script_ShowUIMessageFromID("TE1004AC001");
}

static void script_7EE4(void) {
  Script_ShowUIMessageFromID("E1004AI003");
  return;
}

static void script_7EF0(void) {
  Script_ShowUIMessageFromID("E1004AA022");
  return;
}

static int32_t script_7EFC(void) {
  return Script_ShowUIMessageFromID("TE1004AA007");
}

static int32_t script_7F08(void) {
  return Script_ShowUIMessageFromID("E1004AL003");
}

static int32_t script_7F14(void) {
  return Script_ShowUIMessageFromID("TE1004AF001");
}

static int32_t script_7F20(void) {
  end:;
}

static void script_7F23(void) {
  Script_ShowUIMessageFromID("E1004AA040");
  return;
}

static void script_7F2F(void) {
  Script_ShowUIMessageFromID("E1004AB009");
  return;
}

static void script_7F3B(int32_t arg0) {
  Script_ScenDataUtil(&task_24C, SCND_WriteU8, arg0 / 8, Script_ScenDataUtil(&task_24C, SCND_ReadU8, arg0 / 8) | (1 << (arg0 % 8)));
  return;
}

static void script_7F66(int32_t arg0) {
  Script_ScenDataUtil(&task_24C, SCND_WriteU8, arg0 / 8, Script_ScenDataUtil(&task_24C, SCND_ReadU8, arg0 / 8) & ((1 << (arg0 % 8)) ^ -1));
  return;
}

static int32_t script_7F94(int32_t arg0) {
  return Script_ScenDataUtil(&task_24C, SCND_ReadU8, arg0 / 8) & (1 << (arg0 % 8));
}

static void script_7FB1(float arg0, float arg1, float arg2, float arg3, float arg4, float arg5) {
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

static int32_t script_7FE8(int32_t arg0, float arg1, float arg2, float arg3, float arg4) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  script_7FB1((&local_F4)->x, (&local_F4)->z, arg1, arg2, arg3, arg4);
  end:;
}

static void script_800F(int32_t arg0, float arg1, float arg2, float arg3, float arg4) {
  Vector3 local_F4;
  Vector3 local_E8;
  local_F4.x = arg1;
  (&local_F4)->z = arg2;
  local_E8.x = arg3;
  (&local_E8)->z = arg4;
  (void)(Script_SetTaskDataValue(arg0, &local_F4, &local_E8, 0) & -(script_7FE8(arg0, arg1, arg2, arg3, arg4) <= 0));
  return;
}

static void script_8054(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_8070(int32_t arg0, float arg1) {
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= arg1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_8094(int32_t arg0, float arg1) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  return -!((&local_F4)->x > arg1);
}

static void script_80AA(int32_t arg0, float arg1) {
  while (1) {
    if (script_8094(arg0, arg1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_80CF(int32_t arg0, float arg1) {
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

static int32_t script_80FC(int32_t arg0, float arg1) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  return -!((&local_F4)->z > arg1);
}

static int32_t script_8115(int32_t arg0, float arg1) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  return -!((&local_F4)->x < arg1);
}

static void script_812B(int32_t arg0, float arg1) {
  while (1) {
    if (script_80FC(arg0, arg1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_8150(int32_t arg0, float arg1) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  return -!((&local_F4)->z < arg1);
}

static void script_8169(int32_t arg0, float arg1) {
  while (1) {
    if (script_8150(arg0, arg1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_818E(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
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

static int32_t script_826F(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
  while (1) {
    if (script_818E(arg0, arg1, arg2, arg3, arg4)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_829A(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
  Vector3 local_F4;
  int32_t local_F0;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  local_F0 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, arg1 - (&local_F4)->x, arg2 - (&local_F4)->z);
  script_826F(arg0, arg1, arg2, arg3, arg4);
  if ((local_F0 >= 0xE000) | (local_F0 < 0x2000)) {
    script_812B(arg0, arg2);
  } else if (local_F0 < 0x6000) {
    script_80AA(arg0, arg1);
  } else if (local_F0 < 0xA000) {
    script_8169(arg0, arg2);
  } else {
    script_80CF(arg0, arg1);
  }
  return;
}

static void script_8332(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
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
    script_818E(arg0, arg1, arg2, arg4, arg5);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_83A9(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_818E(arg0, arg1, arg2, arg4, arg5);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_83F5(int32_t arg0) {
  while (script_7F94(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_8414(int32_t arg0) {
  while (script_7F94(arg0) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_8433(int32_t arg0, int32_t arg1, int32_t arg2) {
  return SCNF_UIUtils(arg0, arg1, 2, arg2, 15, 0);
}

static int32_t script_8448(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_7F66(1);
  local_FC = 0;
  local_F4 = Script_scenCreateTask(script_8433, 3, arg0, arg1, arg2);
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
    script_7F3B(0);
  } else {
    script_7F66(0);
  }
  script_7F3B(1);
  return local_F8;
}

static void script_8517(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_7F66(1);
  local_FC = 0;
  local_F4 = Script_scenCreateTask(script_8433, 3, arg0, arg1, arg2);
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
    script_7F3B(0);
  } else {
    script_7F66(0);
  }
  script_7F3B(1);
  (void)local_F8;
  return;
}

static void script_85AF(int32_t arg0, int32_t arg1, float arg2) {
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

static void script_85F6(int32_t arg0) {
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_8624(int32_t arg0) {
  Script_motmToggleFlags(arg0, SCEN_Disable, 4);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  Script_RotateNodeUtils(arg0, &data_184E0, 0);
  return;
}

static int32_t script_865C(int32_t arg0, int32_t arg1) {
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

static void script_86B5(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4, int8_t arg5) {
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
  script_8ECC(arg0);
  return;
}

static void script_873B(int32_t arg0) {
  int32_t unused_D8[10];
  int32_t local_D4;
  local_D4 = arg0;
  switch (local_D4) {
    case 1:
      script_86B5(SHEN_NAME('HAKO'), -73.63f, 63.37f, 2730, -1, 0);
      script_86B5(CHARA_ONO_, -118.3f, 85.4f, 10012, 8237, 0);
      Script_CharHandlers(CHARA_ONO_, 2, 0, 16);
      if (0) {
        Script_charaDisplay(CHARA_YOSE, CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(CHARA_YOSE, CDM_Show);
          goto L0087BF;
        }
      }
      L0087BF:;
      Script_scenToggleVisibleFor(CHARA_YOSE, 1);
      script_86B5(CHARA_YOSE, -71.4f, 64.01f, 0, 8212, 0);
      break;
    case 2:
      if (0) {
        Script_charaDisplay(CHARA_KOND, CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(CHARA_KOND, CDM_Show);
          goto L00881C;
        }
      }
      L00881C:;
      Script_scenToggleVisibleFor(CHARA_KOND, 1);
      script_86B5(CHARA_KOND, -63.76f, 49.76f, 50972, 8223, 1);
      if (0) {
        Script_charaDisplay(CHARA_MISM, CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(CHARA_MISM, CDM_Show);
          goto L008870;
        }
      }
      L008870:;
      Script_scenToggleVisibleFor(CHARA_MISM, 1);
      script_86B5(CHARA_MISM, -61.77f, 48.93f, 20935, 8236, 1);
      if (0) {
        Script_charaDisplay(CHARA_SUMI, CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(CHARA_SUMI, CDM_Show);
          goto L0088C2;
        }
      }
      L0088C2:;
      Script_scenToggleVisibleFor(CHARA_SUMI, 1);
      script_86B5(CHARA_SUMI, -21.53f, 36.55f, 41870, 8223, 1);
      break;
  }
  return;
}

static void script_88F2(void) {
  {
    int32_t local_FC;
    local_FC = 0x57474455;
    script_8ECC(local_FC);
    Script_motmToggleFlags(local_FC, SCEN_Disable, 0x40);
    script_86B5(CHARA_UDGW, 7.16f, 27.63f, 16748, -1, 1);
    Script_motmToggleFlags(local_FC, SCEN_Enable, 4);
    Script_charaToggleMotEnabled(local_FC, 1);
    Script_PlayCharaMotion(local_FC, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(local_FC, 0.7f);
    Script_RequestFightAnimation(local_FC, 8224, 0.0f, 1, 0);
  }
  {
    int32_t local_FC;
    local_FC = 0x414D594B;
    Script_motmToggleFlags(local_FC, SCEN_Disable, 0x40);
    script_8ECC(local_FC);
    script_86B5(CHARA_KYMA, 7.16f, 28.14f, 16748, -1, 1);
    Script_motmToggleFlags(local_FC, SCEN_Enable, 4);
    Script_charaToggleMotEnabled(local_FC, 1);
    Script_PlayCharaMotion(local_FC, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(local_FC, 0.7f);
    Script_RequestFightAnimation(local_FC, 8225, 0.0f, 1, 0);
  }
  return;
}

static void script_89FD(int32_t arg0) {
  Vector3 local_F4;
  for (;;) {
    Script_GetAuthorState_And_SetCamPos(&local_F4, 0);
    Script_SetCharaLookAtPosition(arg0, 1, &local_F4, 0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_8A20(int32_t arg0, int32_t arg1, void *arg2, int32_t arg3, const void *arg4) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = 0;
  while (local_E4 < arg3) {
    Script_InterpolateAndScaleAngles(local_E4, arg1, &local_F4, &local_E8, arg4);
    local_F4.x = (&local_F4)->x * 10.0f;
    (&local_F4)->y = (&local_F4)->y * 10.0f;
    (&local_F4)->z = (&local_F4)->z * 10.0f;
    Script_charaPosRotUtils(arg0, Script_ScenDataUtil(arg2, SCND_ReadU32, local_E4), &local_F4, &local_E8, CHARA_PART_Set);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_8A99(int32_t arg0, int32_t arg1, void *arg2, int32_t arg3, const void *arg4) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = 0;
  while (local_E4 < arg3) {
    Script_InterpolateAndScaleAngles(local_E4, arg1, &local_F4, &local_E8, arg4);
    local_F4.x = (&local_F4)->x * 10.0f;
    (&local_F4)->y = (&local_F4)->y * 1.0f;
    (&local_F4)->z = (&local_F4)->z * 10.0f;
    Script_charaPosRotUtils(arg0, Script_ScenDataUtil(arg2, SCND_ReadU32, local_E4), &local_F4, &local_E8, CHARA_PART_Set);
    local_E4 = local_E4 + 1;
  }
  return;
}

static int32_t script_8B12(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_8ECC(arg0);
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
  while (1) {
    script_8A20(arg0, local_FC / 10, (void *)data_19D44, 10, "BIKE");
    if (script_7F94(12)) {
      local_FC = local_FC + local_F8;
      if (local_FC > 900) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_8BB4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_8ECC(arg0);
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
  while (1) {
    script_8A20(arg0, local_FC / 10, (void *)&data_19D74, 7, "KANB");
    if (script_7F94(11)) {
      local_FC = local_FC + local_F8;
      if (local_FC > 900) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_8C56(int32_t arg0) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  int32_t local_F8;
  int32_t local_F4;
  local_FF = 0;
  script_8ECC(arg0);
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
  while (1) {
    script_8A20(arg0, local_F8 / 10, (void *)data_19D98, 34, "APLE");
    if (script_7F94(7)) {
      if (local_FF == 0) {
        Script_executeAicaCommand(0x1C00A9, 0, 0);
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

__attribute__((scn_string_bindings(0, data_19E6C, 1, data_19E71, 2, data_19E76)))
static int32_t script_8D1A(int32_t arg0) {
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
  script_8A20(arg0, local_FC / 10, (void *)&data_19E28, 17, "SOBA");
  script_8A20(arg0, local_FC / 10, (void *)&data_19E28, 17, "SOBA");
  script_83F5(10);
  local_FC = 290;
  if (script_7F94(0) == 0) {
    while (1) {
      script_8A20(arg0, local_FC / 10, (void *)&data_19E28, 17, "SOBA");
      local_FC = local_FC + local_F8;
      if (local_FC >= 2230) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static int32_t script_8DFA(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 2;
  local_F8 = 3;
  while (1) {
    script_8A20(arg0, local_FC, (void *)&data_19E7C, 4, "HAKO");
    local_FC = local_FC + local_F8;
    if (local_FC >= 260) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_8E47(int32_t arg0) {
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
  return local_FC;
}

static void script_8EAC(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  return;
}

static void script_8ECC(int32_t arg0) {
  Script_scenToggleVisibleFor(arg0, 1);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_8EE0(int32_t arg0) {
  Script_scenToggleVisibleFor(arg0, 0);
  Script_charaDisplay(arg0, CDM_Hide);
  return;
}

static void script_8EF4(int32_t arg0, int32_t arg1, float arg2, float arg3) {
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

static void script_8F66(int32_t arg0, int32_t arg1) {
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, 0, 0, 16);
  Script_PlayCharaMotion(arg0, arg1, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  return;
}

static void script_8FA0(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_PlayCharaMotion(arg0, arg1, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  return;
}

static int32_t script_8FC6(int32_t arg0) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t unused_E4[3];
  Script_seqEnterEvtCam(1463, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
  script_8EE0(SHEN_NAME('DORR'));
  script_8EE0(SHEN_NAME('DORL'));
  script_8EE0(SHEN_NAME('BOXR'));
  Script_mapmSetRenderFlag(2, 1);
  script_8ECC(arg0);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  script_8624(arg0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  script_85F6(arg0);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, 3, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_19ECC, (int32_t)&data_19E94, 4);
  Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.29f);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  script_80AA(arg0, -92.5f);
  if (script_7F94(0) == 0) {
    script_80AA(arg0, -93.5f);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, 0, 0, 16);
    Script_PlayCharaMotion(arg0, 8195, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    Script_charaToggleMotEnabled(arg0, 1);
    script_8054(arg0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(arg0, 0);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_19F40, (int32_t)&data_19E94, 4);
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.29f);
  }
  script_80AA(arg0, -107.2f);
  script_83F5(1);
  if (script_7F94(0) == 0) {
    script_7F3B(10);
    script_8F66(arg0, 8208);
    script_8070(arg0, 30.0f);
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x20);
    Script_PlayCharaMotion(arg0, 8208, 25.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x20);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(arg0, 0);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_19F84, (int32_t)&data_19E94, 4);
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.29f);
  }
  while (Script_LineWalkUtils(LNWK_SCRIPT_GET_STATUS, arg0) != 5) {
    SCNF_WaitFrames_0000(1);
  }
  script_7F3B(13);
  script_7F3B(4);
  local_F0 = 2;
  while (local_F0 < 9) {
    Script_srchControl((void *)task_258[local_F0]);
    local_F0 = local_F0 + 1;
  }
  if (script_7F94(10) == 0) {
    Script_srchControl((void *)task_258[1]);
  }
  SCNF_WaitFrames_0000(2);
  end:;
}

static int32_t script_92D0(int32_t arg0) {
  Vector3i local_F4;
  int32_t unused_F0;
  Vector3 local_E4;
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  script_8ECC(arg0);
  script_8624(arg0);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  script_85F6(arg0);
  local_E4.x = -84.65f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 82.328201f;
  Script_EVT_SetPosition(arg0, 0x78000000, &local_E4);
  local_F4.x = (&local_F4)->z = 0;
  (&local_F4)->y = 18204;
  Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_19FA8, (int32_t)&data_19E94, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.26f) >= 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  end:;
}

static void script_93BC(int32_t arg0, int32_t arg1) {
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
  script_8ECC(arg1);
  return;
}

static int32_t script_947B(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  Vector3i local_D0;
  Script_ccowUtils(CHARA_SOBA, CCOW_DisableAll);
  Script_motmToggleFlags(CHARA_SOBA, SCEN_Enable, 4);
  Script_PlayCharaMotion(CHARA_SOBA, 8234, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  (void)((task_27D = -1) & 4);
  (void)((task_27C = 0) & 4);
  (void)((task_27E = 0) & 4);
  while (1) {
    if (task_27C & 0x10) {
      break;
    }
    if (task_27D == 0) {
      if (Script_IsInEventCam() == 0) {
        break;
      }
    }
    Script_EVT_GetPosition(SHEN_NAME('DDD0'), -1, &local_F4, 0);
    Script_mtwkGetSetRotation(SHEN_NAME('DDD0'), &local_DC, 0);
    local_DC.x = (&local_DC)->z = 0;
    Script_EVT_GetPosition(SHEN_NAME('BIKE'), -1, &local_E8, 0);
    Script_mtwkGetSetRotation(SHEN_NAME('BIKE'), &local_D0, 0);
    (&local_F4)->y = -5.0f;
    (&local_E8)->y = -5.0f;
    Script_EVT_SetPosition(SHEN_NAME('DDD0'), POS_XYZ, &local_F4);
    Script_EVT_SetPosition(SHEN_NAME('DCYC'), POS_XYZ, &local_E8);
    (&local_F4)->y = 0.0f;
    (&local_E8)->y = 0.0f;
    Script_EVT_SetPosition(CHARA_SOBA, POS_XYZ, &local_F4);
    if (task_27E & 0x40) {
      (&local_DC)->y = (&local_DC)->y - 9102;
    }
    Script_EVT_SetRotation(CHARA_SOBA, 0x38000000, &local_DC, 0);
    Script_EVT_SetRotation(SHEN_NAME('DCYC'), 0x38000000, &local_D0, 0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_95E8(void) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t unused_D8[6];
  SCNF_WaitFrames_0000(5);
  Script_ccowSetHeightMode(SHEN_NAME('SEIR'), CCOW_KeepHeight);
  Script_EVT_GetPosition(SHEN_NAME('SEIR'), -1, &local_F0, ENTITY_XFORM_WORLD_SPACE);
  Script_fixoClearAttachment(SHEN_NAME('SEIR'));
  local_FC = 20;
  while (local_FC) {
    Script_EVT_GetPosition(SHEN_NAME('SEIR'), -1, &local_F0, ENTITY_XFORM_WORLD_SPACE);
    local_F0.x = (&local_F0)->x + 0.1f;
    Script_EVT_SetPosition(SHEN_NAME('SEIR'), 0x78000000, &local_F0);
    local_FC = local_FC - 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_9668(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  (void)((task_254 = -1) & 8);
  script_8ECC(SHEN_NAME('DDD0'));
  Script_charaDisplay(SHEN_NAME('DDD0'), CDM_Hide);
  script_8ECC(SHEN_NAME('BIKE'));
  script_8ECC(CHARA_SOBA);
  script_8ECC(SHEN_NAME('DCYC'));
  script_8ECC(SHEN_NAME('SEIR'));
  local_F4.x = (&local_F4)->z = 0.0f;
  (&local_F4)->y = -5.0f;
  Script_EVT_SetPosition(SHEN_NAME('DDD0'), 0x78000000, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('BIKE'), 0x78000000, &local_F4);
  Script_scenCreateTask(script_947B, 0);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_SOBA, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(CHARA_SOBA, 8234, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_vehiUtils(VehicleCommand_AttachRiderToVehicle, SHEN_NAME('DDD0'), 0x454B4942, 0);
  Script_vehiUtils(VehicleCommand_Unknown8_SetFlag, SHEN_NAME('DDD0'), 0);
  Script_vehiUtils(VehicleCommand_StartPathFollow, SHEN_NAME('DDD0'), &data_1A01C, 5, 1);
  Script_vehiUtils(VehicleCommand_SetMaxSpeed, SHEN_NAME('DDD0'), 13.0f);
  local_E8.x = (&local_E8)->z = 0;
  (&local_E8)->y = 46603;
  Script_EVT_SetRotation(SHEN_NAME('DDD0'), 0x78000000, &local_E8, 0);
  Script_EVT_SetRotation(CHARA_SOBA, 0x78000000, &local_E8, 0);
  Script_EVT_SetRotation(SHEN_NAME('DCYC'), 0x78000000, &local_E8, 0);
  Script_EVT_SetRotation(SHEN_NAME('BIKE'), 0x78000000, &local_E8, 0);
  script_83F5(9);
  Script_vehiUtils(VehicleCommand_SetControlProfile, SHEN_NAME('DDD0'), 1, 2.0f);
  script_93BC(CHARA_SOBA, SHEN_NAME('SEIR'));
  script_83F5(1);
  script_80AA(CHARA_AKIR, -107.3f);
  Script_vehiUtils(VehicleCommand_SetControlProfile, SHEN_NAME('DDD0'), 3, 0.96f);
  script_83F5(10);
  if (script_7F94(0) == 0) {
    (void)((task_254 = 0) & 8);
    if (task_27F & 0x80) {
      SCNF_WaitFrames_0000(1);
    } else {
      SCNF_WaitFrames_0000(3);
    }
    (void)((task_27E = -1) & 8);
    Script_PlayCharaMotion(CHARA_SOBA, 8235, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_95E8, 0);
    while (Script_vehiUtils(VehicleCommand_GetState, SHEN_NAME('DDD0')) != 4) {
      SCNF_WaitFrames_0000(1);
    }
  }
  while (Script_vehiUtils(VehicleCommand_GetState, SHEN_NAME('DDD0')) != 4) {
    SCNF_WaitFrames_0000(1);
  }
  return (task_27D = 0) & 8;
}

static int32_t script_98F8(int32_t arg0) {
  script_8ECC(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
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
  script_83F5(7);
  Script_charaToggleMotEnabled(arg0, 1);
  script_80AA(CHARA_AKIR, -93.0f);
  if (script_7F94(0)) {
    script_8FA0(arg0, 8240, 0x42400000);
    (void)((task_27F = -1) & 0x20);
  } else {
    script_8FA0(arg0, 8238, 0x423C0000);
    (void)((task_27F = 0) & 0x20);
  }
  end:;
}

static void script_99CE(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  for (;;) {
    if (task_252) {
      break;
    }
    Script_EVT_GetPosition(CHARA_AKIR, 3, &local_F4, 0);
    Script_EVT_GetPosition(CHARA_EIIU, 3, &local_E8, 0);
    Script_SetCharaLookAtPosition(CHARA_AKIR, 2, &local_E8, 0);
    Script_SetCharaLookAtPosition(CHARA_EIIU, 2, &local_F4, 0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_9A22(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  int8_t local_F3;
  int8_t local_F2;
  int8_t local_F1;
  int8_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  local_EF = local_EE = 0;
  task_251 = 0;
  task_258[0] = Script_scenCreateTask(script_8FC6, 1, CHARA_AKIR);
  task_258[1] = Script_scenCreateTask(script_8D1A, 1, SHEN_NAME('SEIR'));
  task_258[2] = Script_scenCreateTask(script_92D0, 1, CHARA_EIIU);
  task_258[4] = Script_scenCreateTask(script_9668, 0);
  task_258[7] = Script_scenCreateTask(script_865C, 2, SHEN_NAME('PIL2'), CHARA_SOBA);
  task_258[3] = Script_scenCreateTask(script_98F8, 1, CHARA_HANA);
  task_258[8] = Script_scenCreateTask(script_865C, 2, SHEN_NAME('PIL3'), CHARA_HANA);
  task_258[5] = Script_scenCreateTask(script_865C, 2, SHEN_NAME('PIL0'), CHARA_AKIR);
  task_258[6] = Script_scenCreateTask(script_865C, 2, SHEN_NAME('PIL1'), CHARA_EIIU);
  local_F3 = 0;
  local_F2 = -1;
  local_F1 = 0;
  local_F0 = -1;
  while (local_F3 != -1) {
    if (local_F3 != local_F2) {
      local_F8 = 0;
      local_F2 = local_F3;
    } else {
      local_F8 = local_F8 + 1;
    }
    {
      uint8_t unused_E8[6];
      int32_t local_E4;
      local_E4 = local_F3;
      switch (local_E4) {
        case 0:
          local_F3 = 5;
          break;
        case 5:
          if (script_8094(CHARA_AKIR, -84.5f)) {
            script_7F3B(7);
            local_F3 = 6;
          }
          break;
        case 6:
          script_7F66(1);
          script_7F3B(7);
          local_F3 = 7;
          local_F1 = 2;
          break;
        case 7:
          Script_scenCreateTask(script_8448, 3, 64, 15, 0);
          local_F3 = 8;
          break;
        case 8:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_EF = local_EF + 1;
            } else {
              local_EE = local_EE + 1;
            }
            local_F3 = 9;
          }
          break;
        case 9:
          if (script_8094(CHARA_AKIR, -97.5f)) {
            script_7F3B(9);
            if (script_8094(CHARA_AKIR, -103.6f)) {
              local_F3 = 10;
            }
          }
          break;
        case 10:
          script_7F66(1);
          script_7F3B(9);
          local_F3 = 11;
          local_F1 = 3;
          break;
        case 11:
          Script_scenCreateTask(script_8448, 3, 4, 12, 0);
          local_F3 = 12;
          break;
        case 12:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_EF = local_EF + 1;
            } else {
              local_EE = local_EE + 1;
            }
            local_F3 = 13;
          }
          break;
        case 13:
          local_F3 = 14;
          break;
        case 14:
          task_251 = local_EE;
          script_7F3B(3);
          script_7F3B(2);
          break;
      }
    }
    if (local_F1 != local_F0) {
      local_FC = 0;
      local_F0 = local_F1;
    } else {
      local_FC = local_FC + 1;
    }
    {
      uint8_t unused_E8[6];
      int32_t local_E4;
      local_E4 = local_F1;
      switch (local_E4) {
        case 0:
          local_E4 = local_FC;
          switch (local_E4) {
            case 0:
              Script_scenCreateTask(script_7C86, 0);
              Script_executeAicaCommand(81832, 0, 0);
              break;
            case 2:
              Script_scenCreateTask(script_A0B2, 0);
              break;
          }
          break;
        case 2:
          local_E4 = local_FC;
          switch (local_E4) {
            case 20:
              if (script_7F94(0)) {
                Script_seqEnterEvtCam(1471, 0, AUTH_None);
              } else {
                Script_seqEnterEvtCam(1458, SHEN_NAME('PIL3'), SHEN_NAME('PIL3'));
              }
              break;
            case 31:
              if (script_7F94(0)) {
                Script_scenCreateTask(script_7EB4, 0);
              }
              break;
            case 40:
              Script_scenCreateTask(script_7EC0, 0);
              break;
            case 42:
              if (script_7F94(0)) {
                Script_executeAicaCommand(458921, 0, 0);
              } else {
                Script_executeAicaCommand(655529, 0, 0);
              }
              break;
            case 57:
              if (script_7F94(0)) {
                Script_ShowUIMessageFromID("TE1004AA028");
                Script_seqEnterEvtCam(1451, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              } else {
                Script_ShowUIMessageFromID("TE1004AA002");
              }
              break;
            case 117:
              if (script_7F94(0) == 0) {
                Script_seqEnterEvtCam(1451, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
          }
          break;
        case 3:
          local_E4 = local_FC;
          switch (local_E4) {
            case 18:
              if (script_7F94(0)) {
                Script_seqEnterEvtCam(1465, 0, AUTH_None);
              } else {
                Script_seqEnterEvtCam(1467, 0, AUTH_None);
              }
              break;
            case 24:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7CD1, 0);
              }
              break;
            case 25:
              if (script_7F94(0) == 0) {
                Script_executeAicaCommand(0x1E00A9, 0, 0);
              }
              break;
            case 27:
              if (script_7F94(0)) {
                Script_scenCreateTask(script_7ED8, 0);
                Script_executeAicaCommand(66729, 0, 0);
              }
              break;
            case 33:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7ECC, 0);
              }
              break;
            case 50:
              if (script_7F94(0) == 0) {
                Script_executeAicaCommand(0x1800A9, 0, 0);
              }
              break;
            case 55:
              if (script_7F94(0)) {
                Script_convSetCamInactiveFlag(0);
                Script_SetGameStateTo(AUTH_None);
              }
              break;
            case 65:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7D19, 0);
              }
              break;
            case 73:
              if (script_7F94(0) == 0) {
                Script_convSetCamInactiveFlag(0);
                Script_SetGameStateTo(AUTH_None);
              }
              break;
          }
          break;
      }
    }
    if (script_7F94(13)) {
      local_F3 = -1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_A080(void) {
  int32_t local_FC;
  local_FC = 0;
  while (local_FC < 500) {
    Script_sprintf("check count = %d\012", local_FC);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_A0B2(void) {
  int32_t local_FC;
  Script_ShowUIMessageFromID("TE1004AE005");
  local_FC = 40;
  while (local_FC) {
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC - 1;
  }
  return Script_ShowUIMessageFromID("TE1004AE006");
}

__attribute__((scn_string_bindings(0, data_1A0AA)))
static int32_t script_A0E7(int32_t arg0) {
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00A10B;
    }
  }
  L00A10B:;
  Script_scenToggleVisibleFor(arg0, 1);
  script_8EE0(arg0);
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
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_83F5(21);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.5f);
  script_83F5(1);
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    local_E8.x = -92.004501f;
    (&local_E8)->z = 61.267799f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E8);
  }
  if (script_7F94(0)) {
    script_8ECC(arg0);
  } else {
    script_8EE0(arg0);
  }
  script_83F5(14);
  script_83F5(1);
  if (script_7F94(0)) {
    Vector3 local_F4;
    Script_PlayCharaMotion(arg0, 8241, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(20);
    Script_ShowUIMessageFromID("TE1004AG001");
    script_8070(arg0, 30.0f);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetCharaLookAtPosition(arg0, 1, &local_F4, 0);
    script_8070(arg0, 60.0f);
    Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
    script_8054(arg0);
  } else {
    Vector3 local_F4;
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    (&local_F4)->y = 0.1f;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
    Script_PlayCharaMotion(arg0, 8216, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_8070(arg0, 5.0f);
    Script_scenCreateTask(script_7DC4, 0);
    script_8070(arg0, 15.0f);
    Script_executeAicaCommand(721065, 0, 0);
    script_8070(arg0, 34.0f);
    Script_executeAicaCommand(786601, 0, 0);
    script_8054(arg0);
  }
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.6f);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  end:;
}

static int32_t script_A39D(int32_t arg0) {
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

static int32_t script_A3DC(int32_t arg0) {
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00A400;
    }
  }
  L00A400:;
  Script_scenToggleVisibleFor(arg0, 1);
  script_8EE0(arg0);
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
  script_83F5(21);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_83F5(1);
  if (script_7F94(0)) {
    Script_PlayCharaMotion(arg0, 8222, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_charaToggleMotEnabled(arg0, 0);
    Script_PlayCharaMotion(arg0, 8215, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_8070(arg0, 10.0f);
    Script_executeAicaCommand(721065, 0, 0);
    script_8070(arg0, 80.0f);
    Script_scenCreateTask(script_A39D, 1, arg0);
    Script_charaToggleMotEnabled(arg0, 1);
    Script_SetCharaMotionPlaybackRate(arg0, 0.8f);
  }
  end:;
}

static int32_t script_A54A(int32_t arg0) {
  int32_t unused_FC;
  Vector3 local_F0;
  Script_charaToggleMotEnabled(arg0, 1);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  local_F0.x = -103.89f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = 95.71f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
  {
    Vector3i local_E4;
    (&local_E4)->y = 25211;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E4, 0);
  }
  script_8ECC(CHARA_EIIU);
  Script_charaToggleMotEnabled(CHARA_EIIU, 1);
  {
    Vector3 local_E4;
    local_E4.x = -114.5f;
    (&local_E4)->z = 98.9f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_E4);
  }
  {
    Vector3i local_E4;
    (&local_E4)->y = 12196;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E4, 0);
  }
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_8ECC(CHARA_EIIU);
  script_812B(arg0, 97.07f);
  script_826F(arg0, -121.55f, 88.14f, 910, 0);
  script_812B(arg0, 88.7f);
  script_826F(arg0, -119.75f, 85.2f, 2730, 0);
  script_8ECC(CHARA_EIIU);
  script_83F5(1);
  if (script_7F94(0)) {
    Script_executeAicaCommand(458921, 0, 0);
    script_80CF(arg0, -125.0f);
    script_8332(arg0, -120.1f, 81.63f, 1.8f, 1820, 1);
  } else {
    int32_t unused_E4[3];
    Vector3i local_D8;
    script_80CF(arg0, -120.3f);
    Script_scenCreateTask(script_7E9C, 0);
    Script_PlayCharaMotion(arg0, 8193, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(25);
    Script_SetCharaMotionPlaybackRate(CHARA_ONO_, 0.0f);
    SCNF_WaitFrames_0000(25);
    Script_motmToggleFlags(CHARA_ONO_, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(CHARA_ONO_, 8229, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_ONO_, 1.2f);
    Script_motmSetBlendFrames(CHARA_ONO_, MOTM_BLEND_FRAMES_15);
    SCNF_WaitFrames_0000(10);
    (&local_D8)->y = 4551;
    Script_EVT_SetRotation(CHARA_ONO_, MATRIX_XFORM_Y, &local_D8, 0);
    Script_CharHandlers(CHARA_ONO_, 0, 8, 16);
    script_8054(arg0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  script_829A(arg0, -111.33f, 72.31f, 910, 0);
  script_826F(arg0, -109.99f, 71.53f, 2730, 0);
  script_83F5(1);
  if (script_7F94(0)) {
    Vector3 local_E4;
    Vector3i local_D8;
    int32_t local_D4;
    script_80CF(arg0, -108.98f);
    Script_PlayCharaMotion(arg0, 8209, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(458921, 0, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.25f);
    script_8054(arg0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    Script_EVT_GetPosition(arg0, -1, &local_E4, 0);
    Script_mtwkGetSetRotation(arg0, &local_D8, 0);
    local_D4 = script_C534((&local_E4)->x, (&local_E4)->z, -94.62f, 64.94f);
    local_D4 = local_D4 - (&local_D8)->y;
    if (local_D4 < 0) {
      Script_sprintf("LEFT[%d]\012", local_D4);
      script_829A(arg0, -94.62f, 64.94f, 910, 1);
    } else {
      Script_sprintf("RIGHT[%d]\012", local_D4);
      script_829A(arg0, -94.62f, 64.94f, 910, 0);
    }
  } else {
    script_80CF(arg0, -109.48f);
    Script_PlayCharaMotion(arg0, 8208, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_8070(arg0, 36.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_8332(arg0, -100.88f, 68.64f, 1.0f, 1820, 0);
    script_829A(arg0, -94.62f, 64.94f, 910, 1);
  }
  script_829A(arg0, -84.97f, 62.46f, 546, 0);
  {
    Vector3 local_E4;
    local_E4.x = -87.12f;
    (&local_E4)->z = 63.04f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E4);
  }
  script_8EE0(CHARA_PAUR);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  script_826F(arg0, -71.7f, 62.08f, 546, 0);
  script_83F5(1);
  if (script_7F94(0)) {
    script_80CF(arg0, -79.06f);
    Script_executeAicaCommand(917673, 0, 0);
    Script_PlayCharaMotion(arg0, 8231, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
    script_8070(arg0, 38.0f);
    Script_executeAicaCommand(983209, 0, 0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_6);
    script_829A(arg0, -68.4f, 60.19f, 910, 1);
  } else {
    script_80CF(arg0, -75.83f);
    Script_PlayCharaMotion(arg0, 8207, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(20);
    Script_executeAicaCommand(786601, 0, 0);
    script_8070(arg0, 60.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_829A(arg0, -68.4f, 60.19f, 2730, 1);
  }
  script_83F5(2);
  script_8EE0(CHARA_EIIU);
  script_8EE0(CHARA_NORK);
  script_8EE0(CHARA_AKSK);
  script_8EE0(CHARA_YMGC);
  script_8EE0(CHARA_MNWA);
  script_8EE0(CHARA_STAK);
  if (script_7F94(3) == 0) {
    Script_PlayCharaMotion(arg0, 8202, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(589993, 0, 0);
    script_8070(arg0, 25.0f);
    Script_charaToggleMotEnabled(arg0, 0);
    Script_PlayCharaMotion(arg0, 8201, 15.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      Script_RotateNodeUtils(arg0, &data_184E0, 0);
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(1);
    script_8054(arg0);
  } else {
    Vector3 local_E4;
    Script_PlayCharaMotion(arg0, 8202, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(589993, 0, 0);
    script_8070(arg0, 55.0f);
    SCNF_WaitFrames_0000(5);
    local_E4.x = -62.32f;
    (&local_E4)->y = 1.75f;
    (&local_E4)->z = 53.59f;
    Script_SetCharaLookAtPosition(arg0, 1, &local_E4, 2);
  }
  end:;
}

static int32_t script_ACD7(int32_t arg0) {
  Vector3 local_F4;
  if (script_7F94(0)) {
    script_80CF(arg0, -97.89f);
    Script_PlayCharaMotion(arg0, 8210, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(458921, 0, 0);
    script_8070(arg0, 37.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    local_F4.x = -97.3181f;
    (&local_F4)->z = 65.080803f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  } else {
    script_80CF(arg0, -98.54f);
    Script_PlayCharaMotion(arg0, 8196, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_8070(arg0, 54.0f);
    Script_scenCreateTask(script_7E90, 0);
    script_8054(arg0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
  }
  script_7F3B(19);
  return Script_srchControl(0);
}

static int32_t script_AE76(int32_t arg0) {
  if (script_7F94(0)) {
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(arg0, 8210, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(458921, 0, 0);
    SCNF_WaitFrames_0000(5);
    Script_scenCreateTask(script_7EFC, 0);
    script_8054(arg0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_12);
  } else {
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(arg0, 8197, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_8070(arg0, 60.0f);
    Script_scenCreateTask(script_7E78, 0);
    script_8054(arg0);
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  }
  script_7F3B(20);
  return Script_srchControl(0);
}

static int32_t script_AF8F(int32_t arg0) {
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
  script_8ECC(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_83F5(9);
  script_8ECC(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_START000, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_8054(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_829A(arg0, -122.51f, 86.21f, 910, 0);
  script_829A(arg0, -110.18f, 72.35f, 910, 0);
  script_8EE0(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  end:;
}

static int32_t script_B0AA(int32_t arg0) {
  Vector3 local_F4;
  script_8ECC(arg0);
  local_F4.x = -109.84f;
  (&local_F4)->z = 72.73f;
  Script_EVT_SetPosition(CHARA_EIIU, 0x28000000, &local_F4);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  script_829A(arg0, -104.61f, 69.53f, 0x10000, 0);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(arg0, 8228, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_8054(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_829A(arg0, -94.62f, 64.94f, 910, 0);
  script_826F(arg0, -92.4f, 64.32f, 2730, 0);
  script_829A(arg0, -84.63f, 62.45f, 910, 0);
  script_829A(arg0, -70.11f, 63.25f, 910, 0);
  script_8EE0(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  end:;
}

static int32_t script_B1F3(int32_t arg0) {
  Vector3 local_F4;
  script_8ECC(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  local_F4.x = -77.69f;
  (&local_F4)->z = 62.72f;
  Script_EVT_SetPosition(CHARA_EIIU, 0x28000000, &local_F4);
  script_826F(arg0, -73.42f, 62.46f, 0x10000, 0);
  script_80CF(arg0, -75.47f);
  Script_PlayCharaMotion(arg0, 8227, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_8054(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_829A(arg0, -62.53f, 54.16f, 1820, 1);
  Script_charaToggleMotEnabled(arg0, 0);
  script_8EE0(arg0);
  end:;
}

static int32_t script_B2E1(void) {
  int32_t unused_DC[9];
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int32_t local_CC;
  int32_t local_C8;
  int8_t local_C7;
  int8_t local_C6;
  int8_t local_C5;
  int8_t local_C4;
  int8_t local_C3;
  int8_t local_C2;
  int8_t local_C1;
  local_C3 = task_251;
  local_C2 = task_251;
  (void)((task_280 = 0) & 1);
  (void)((task_281 = -1) & 1);
  script_7F66(21);
  script_7F66(4);
  local_CC = 0;
  while (local_CC < 10) {
    *(&local_D8 + local_CC) = 0;
    local_CC = local_CC + 1;
  }
  local_D8 = Script_scenCreateTask(script_A54A, 1, CHARA_AKIR);
  *(&local_D8 + 1) = Script_scenCreateTask(script_AF8F, 1, CHARA_EIIU);
  *(&local_D8 + 2) = Script_scenCreateTask(script_8C56, 1, SHEN_NAME('FLIT'));
  *(&local_D8 + 3) = 0;
  *(&local_D8 + 4) = Script_scenCreateTask(script_8BB4, 1, SHEN_NAME('D201'));
  *(&local_D8 + 5) = Script_scenCreateTask(script_8B12, 1, SHEN_NAME('CBIK'));
  *(&local_D8 + 6) = Script_scenCreateTask(script_A0E7, 1, CHARA_NOMR);
  *(&local_D8 + 7) = Script_scenCreateTask(script_A3DC, 1, CHARA_PAUR);
  *(&local_D8 + 8) = Script_scenCreateTask(script_865C, 2, SHEN_NAME('PIL0'), CHARA_AKIR);
  *(&local_D8 + 9) = Script_scenCreateTask(script_865C, 2, SHEN_NAME('PIL1'), CHARA_EIIU);
  script_873B(1);
  local_C7 = 0;
  local_C6 = -1;
  local_C5 = 0;
  local_C4 = -1;
  while (local_C7 != -1) {
    if (local_C7 != local_C6) {
      local_D0 = 0;
      local_C6 = local_C7;
    } else {
      local_D0 = local_D0 + 1;
    }
    task_248 = task_248 + 1;
    {
      uint8_t unused_C0;
      int32_t local_BC;
      local_BC = local_C7;
      switch (local_BC) {
        case 0:
          if (local_D0 >= 5) {
            local_C7 = 4;
          }
          break;
        case 4:
          script_7F3B(9);
          script_7F3B(10);
          if (script_8094(CHARA_AKIR, -114.15f)) {
            local_C7 = 5;
          }
          break;
        case 5:
          local_C5 = 1;
          task_248 = 0;
          script_7F66(1);
          local_C7 = 6;
          break;
        case 6:
          if (script_8094(CHARA_AKIR, -115.0f)) {
            local_C7 = 7;
          }
          break;
        case 7:
          if (local_D0 >= 44) {
            Script_scenCreateTask(script_8448, 3, 64, 15, 0);
            local_C7 = 8;
          }
          break;
        case 8:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_C3 = local_C3 + 1;
            } else {
              local_C2 = local_C2 + 1;
            }
            local_C7 = 9;
          }
          break;
        case 9:
          {
            int8_t local_BB;
            local_BB = 0;
            if (script_7F94(0)) {
              if (script_8115(CHARA_AKIR, -116.6f)) {
                local_BB = 1;
              }
            } else {
              if (script_8115(CHARA_AKIR, -119.8f)) {
                script_7F3B(12);
              }
              if (script_8115(CHARA_AKIR, -116.6f)) {
                local_BB = 1;
              }
            }
            if (local_BB) {
              Script_srchControl((void *)*(&local_D8 + 1));
              local_C7 = 10;
            }
            break;
          }
        case 10:
          local_C5 = 2;
          script_7F66(1);
          *(&local_D8 + 1) = Script_scenCreateTask(script_B0AA, 1, CHARA_EIIU);
          local_C7 = 11;
          break;
        case 11:
          if (script_8115(CHARA_EIIU, -108.39f)) {
            local_C7 = 13;
          }
          break;
        case 13:
          if (local_D0 >= 15) {
            Script_scenCreateTask(script_8448, 3, 4, 15, 0);
            local_C7 = 12;
          }
          break;
        case 12:
          if (script_8115(CHARA_EIIU, -103.51f)) {
            script_7F3B(11);
            local_C7 = 14;
          }
          break;
        case 14:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_C3 = local_C3 + 1;
            } else {
              local_C2 = local_C2 + 1;
            }
            local_C7 = 15;
          }
          break;
        case 15:
          {
            int8_t local_BB;
            local_BB = 0;
            if (script_7F94(0)) {
              if (script_8115(CHARA_AKIR, -102.92f)) {
                local_BB = 1;
              }
            } else {
              if (script_8115(CHARA_AKIR, -102.92f)) {
                local_BB = 1;
              }
            }
            if (local_BB) {
              local_C7 = 16;
            }
            break;
          }
        case 16:
          local_C5 = 3;
          script_7F66(1);
          script_7F66(19);
          script_7F66(20);
          script_7F66(14);
          script_7F66(16);
          script_7F66(15);
          script_7F66(17);
          local_C7 = 17;
          break;
        case 17:
          if (script_8115(CHARA_AKIR, -101.0f)) {
            local_C7 = 18;
          }
          break;
        case 18:
          if (local_D0 == 0) {
            Script_scenCreateTask(script_8448, 3, 64, 15, 0);
            local_C7 = 19;
            script_7F3B(21);
          }
          break;
        case 19:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_C3 = local_C3 + 1;
              script_7F3B(15);
            } else {
              script_7F66(15);
              local_C2 = local_C2 + 1;
            }
            Script_scenCreateTask(script_ACD7, 1, CHARA_AKIR);
            local_C7 = 20;
          }
          break;
        case 20:
          if (script_7F94(19)) {
            if (script_7F94(0)) {
              local_C7 = 21;
              script_7F66(1);
              script_7F66(0);
              script_7F3B(14);
              script_7F3B(15);
              script_7F66(17);
            } else {
              script_7F66(15);
              local_C7 = 23;
            }
          }
          break;
        case 21:
          script_83F5(14);
          if (local_D0 == 0) {
            Script_scenCreateTask(script_8448, 3, 4, 12, 0);
          }
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              script_7F3B(17);
              local_C3 = local_C3 + 1;
            } else {
              script_7F66(17);
              local_C2 = local_C2 + 1;
            }
            Script_scenCreateTask(script_AE76, 1, CHARA_AKIR);
            local_C7 = 22;
          }
          break;
        case 22:
          if (script_7F94(20)) {
            local_C7 = 23;
          }
          break;
        case 23:
          {
            int8_t local_BB;
            local_BB = 0;
            if (script_8115(CHARA_AKIR, -89.9f)) {
              local_BB = 1;
            }
            if (local_BB) {
              Script_srchControl((void *)*(&local_D8 + 1));
              local_C7 = 24;
              script_7F66(21);
            }
            break;
          }
        case 24:
          local_C5 = 4;
          script_7F66(1);
          *(&local_D8 + 1) = Script_scenCreateTask(script_B1F3, 1, CHARA_EIIU);
          local_C7 = 25;
          break;
        case 25:
          if (script_8115(CHARA_EIIU, -73.75f)) {
            script_7F3B(7);
            local_C7 = 26;
          }
          break;
        case 26:
          if (local_D0 >= 10) {
            Script_scenCreateTask(script_8448, 3, 0x100, 15, 0);
            local_C7 = 27;
          }
          break;
        case 27:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_C3 = local_C3 + 1;
            } else {
              local_C2 = local_C2 + 1;
            }
            local_C7 = 28;
          }
          break;
        case 28:
          if (script_8115(CHARA_AKIR, -73.8f)) {
            local_C7 = 29;
          }
          break;
        case 29:
          if (script_8115(CHARA_AKIR, -68.4f)) {
            local_C5 = 5;
            script_7F3B(2);
            if (local_C2 <= (task_250 + 1) / 2 + 3) {
              script_7F3B(3);
              local_C7 = 30;
            } else {
              script_7F66(3);
              local_C7 = 31;
            }
          }
          break;
        case 30:
          if (local_D0 >= 20) {
            local_C7 = -1;
          }
          break;
        case 31:
          if (local_D0 >= 260) {
            Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
            if (task_1FC & 8) {
              Script_executeAicaCommand(2720, 5, 15);
              Script_uiUtils(0, 37, 0);
            }
            local_C7 = 32;
          }
          break;
        case 32:
          if (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
            local_C7 = 33;
          }
          break;
        case 33:
          if (local_D0 >= 20) {
            local_C7 = -1;
          }
          break;
      }
    }
    if (local_C5 != local_C4) {
      local_D4 = 0;
      local_C4 = local_C5;
    } else {
      local_D4 = local_D4 + 1;
    }
    {
      uint8_t unused_C0;
      int32_t local_BC;
      local_BC = task_248;
      if (local_BC == 320) {
        if (!(script_7F94(0) == 0)) goto L00BC22;
        L00BC22:;
        goto L00BC25;
      }
    }
    L00BC25:;
    {
      uint8_t unused_C0;
      int32_t local_BC;
      local_BC = local_C5;
      switch (local_BC) {
        case 1:
          local_BC = local_D4;
          switch (local_BC) {
            case 0:
              script_8ECC(CHARA_ONO_);
              script_8ECC(CHARA_EIIU);
              break;
            case 1:
              Script_seqEnterEvtCam(1516, 0, AUTH_None);
              break;
            case 25:
              if (task_280 == 0) {
                (void)((task_280 = -1) & 2);
                Script_scenCreateTask(script_C55B, 0);
              }
              break;
            case 63:
              Script_seqEnterEvtCam(1503, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              break;
            case 68:
              if (script_7F94(0)) {
                Script_seqEnterEvtCam(1012, 0, AUTH_None);
                Script_scenCreateTask(script_7C92, 0);
              }
              break;
            case 90:
              if (script_7F94(0) == 0) {
                Script_seqEnterEvtCam(1015, 0, AUTH_None);
              }
              break;
            case 98:
              if (script_7F94(0) == 0) {
                Script_executeAicaCommand(0x1B00A9, 0, 0);
              }
              break;
            case 100:
              if (script_7F94(0) == 1) {
                Script_seqEnterEvtCam(1509, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
            case 110:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7C9E, 0);
              }
              break;
            case 115:
              if (script_7F94(0)) {
                Script_scenCreateTask(script_C5A8, 0);
              }
              break;
            case 120:
              if (script_7F94(0) == 0) {
                Script_executeAicaCommand(0x1500A9, 0, 0);
              }
              break;
            case 130:
              if (script_7F94(0) == 0) {
                Script_seqEnterEvtCam(1514, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
            case 135:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7CAD, 0);
              }
              break;
            case 150:
              if (!(script_7F94(0) == 0)) goto L00BE53;
              L00BE53:;
              break;
          }
          break;
        case 2:
          local_BC = local_D4;
          switch (local_BC) {
            case 5:
              Script_seqEnterEvtCam(1474, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
              (void)((task_281 = 0) & 2);
              break;
            case 12:
              Script_scenCreateTask(script_C590, 0);
              break;
            case 24:
              Script_scenCreateTask(script_C5A8, 0);
              break;
            case 37:
              Script_scenCreateTask(script_7CB9, 0);
              break;
            case 38:
              Script_executeAicaCommand(0x1F00A9, 0, 0);
              break;
            case 50:
              Script_executeAicaCommand(0x1400A9, 0, 0);
              break;
            case 52:
              if (script_7F94(0)) {
                Script_scenCreateTask(script_7C92, 0);
              }
              break;
            case 58:
              if (script_7F94(0) == 0) {
                script_8ECC(CHARA_PAUR);
                script_8EE0(SHEN_NAME('BIKE'));
                script_8EE0(CHARA_ONO_);
                Script_seqEnterEvtCam(1517, 0, AUTH_None);
              }
              break;
            case 59:
              Script_executeAicaCommand(0x1600A9, 0, 0);
              break;
            case 65:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7CD1, 0);
              }
              break;
            case 70:
              if (script_7F94(0) == 1) {
                local_C1 = 1;
              } else {
                local_C1 = 0;
              }
              break;
            case 84:
              if (script_7F94(0)) {
                script_8ECC(CHARA_PAUR);
                script_8EE0(SHEN_NAME('BIKE'));
                script_8EE0(CHARA_ONO_);
                Script_seqEnterEvtCam(1507, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                Script_scenCreateTask(script_C59C, 0);
              }
              break;
            case 94:
              if (script_7F94(0) == 0) {
                Script_seqEnterEvtCam(1507, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                Script_scenCreateTask(script_C59C, 0);
              }
              break;
          }
          break;
        case 3:
          local_BC = local_D4;
          switch (local_BC) {
            case 65:
              if (script_7F94(0)) {
                Script_seqEnterEvtCam(1473, 0, AUTH_None);
              }
              break;
            case 27:
              if (script_7F94(0) == 1) {
                Script_seqEnterEvtCam(1035, 0, AUTH_None);
              }
              break;
            case 90:
              if (script_7F94(15) == 1) {
                if (script_7F94(0) == 0) {
                  Script_seqEnterEvtCam(1036, 0, AUTH_None);
                } else {
                  Script_seqEnterEvtCam(1034, CHARA_AKIR, CHARA_AKIR);
                }
              }
              break;
            case 70:
              if (script_7F94(15) == 0) {
                Script_seqEnterEvtCam(1509, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
            case 93:
              if (script_7F94(17) == 0) {
                Script_seqEnterEvtCam(1525, 0, AUTH_None);
              }
              break;
            case 31:
              if (script_7F94(0)) {
                Script_scenCreateTask(script_7C92, 0);
              }
              break;
            case 36:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7F14, 0);
              }
              break;
            case 71:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7F20, 0);
              }
              break;
          }
          break;
        case 4:
          local_BC = local_D4;
          switch (local_BC) {
            case 0:
              Script_seqEnterEvtCam(1519, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              script_8EE0(CHARA_PAUR);
              script_8EE0(CHARA_NOMR);
              script_8EE0(SHEN_NAME('D201'));
              script_8EE0(CHARA_EIIU);
              break;
            case 2:
              Script_scenCreateTask(script_7CDD, 0);
              break;
            case 19:
              Script_seqEnterEvtCam(1040, 0, AUTH_None);
              break;
            case 35:
              Script_scenCreateTask(script_7D0D, 0);
              break;
            case 45:
              Script_executeAicaCommand(0x1A00A9, 0, 0);
              break;
            case 47:
              Script_executeAicaCommand(0x1A00A9, 0, 0);
              break;
            case 58:
              if (script_7F94(0)) {
                Script_scenCreateTask(script_7C92, 0);
              }
              break;
            case 60:
              if (script_7F94(0) == 0) {
                Script_seqEnterEvtCam(1041, 0, AUTH_None);
              } else {
                Script_seqEnterEvtCam(1522, 0, AUTH_None);
              }
              break;
            case 66:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7CD1, 0);
              }
              break;
            case 70:
              Script_scenCreateTask(script_7CF9, 0);
              break;
            case 75:
              if (!script_7F94(0)) goto L00C369;
              L00C369:;
              break;
            case 80:
              Script_scenCreateTask(script_7D25, 0);
              break;
            case 90:
              if (script_7F94(0) == 0) {
                Script_seqEnterEvtCam(1520, 0, AUTH_None);
              }
              break;
            case 113:
              Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
              if (script_7F94(0)) {
                Script_seqEnterEvtCam(1524, 0, AUTH_None);
              }
              break;
            case 115:
              if (!(script_7F94(0) == 0)) goto L00C3F3;
              L00C3F3:;
              break;
          }
          break;
        case 5:
          local_BC = local_D4;
          switch (local_BC) {
            case 15:
              if (!(script_7F94(3) == 0)) goto L00C422;
              L00C422:;
              break;
            case 100:
              if (script_7F94(3) == 0) {
                Script_seqEnterEvtCam(1050, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                Script_scenCreateTask(script_7C2E, 2, CHARA_AKIR, 60);
              }
              break;
            case 101:
              if (script_7F94(3) == 0) {
                Script_scenCreateTask(script_7D45, 0);
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
  script_7F3B(4);
  end:;
}

static int32_t script_C50A(void) {
  script_7F3B(4);
  script_7F3B(3);
  end:;
}

static int32_t script_C51F(void) {
  script_7F3B(4);
  script_7F3B(3);
  end:;
}

static int32_t script_C534(float arg0, float arg1, float arg2, float arg3) {
  float local_FC;
  float local_F8;
  int32_t local_F4;
  local_FC = arg2 - arg0;
  local_F8 = arg3 - arg1;
  local_F4 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, local_FC, local_F8);
  return local_F4;
}

static int32_t script_C55B(void) {
  int32_t local_FC;
  Script_ShowUIMessageFromID("TE1004AA029");
  local_FC = 25;
  while (local_FC) {
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC - 1;
  }
  return Script_ShowUIMessageFromID("TE1004AE007");
}

__attribute__((scn_string_bindings(0, data_1A0E3)))
static int32_t script_C590(void) {
  return Script_ShowUIMessageFromID("TE1004AA030");
}

static int32_t script_C59C(void) {
  return Script_ShowUIMessageFromID("TE1004AA033");
}

static int32_t script_C5A8(void) {
  int8_t local_FF;
  local_FF = 50;
  while (local_FF) {
    if (task_281 == 0) {
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

static int32_t script_C5EB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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

static void script_C633(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_C5EB, 6, arg0, arg2, arg3, arg1, arg4, arg5);
  return;
}

static int32_t script_C64D(int32_t arg0, float arg1, int32_t arg2) {
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= arg1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(arg2, 0, 0);
  end:;
}

static void script_C673(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_C64D, 3, arg0, arg1, arg2);
  return;
}

static void script_C687(void) {
  script_8EE0(CHARA_AKSK);
  script_8EE0(SHEN_NAME('DDD1'));
  script_8EE0(SHEN_NAME('BICY'));
  script_8EE0(SHEN_NAME('D201'));
  script_8EE0(SHEN_NAME('CBIK'));
  return;
}

static void script_C6C6(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case -1:
      break;
    case 0:
      script_8EE0(SHEN_NAME('DDD1'));
      script_8EE0(SHEN_NAME('BIKE'));
      break;
    case 1:
      script_8EE0(CHARA_AKSK);
      script_8EE0(CHARA_MISM);
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

static int32_t script_C753(void) {
  script_8ECC(SHEN_NAME('DDD1'));
  script_8ECC(SHEN_NAME('BICY'));
  Script_PlayCharaMotion(SHEN_NAME('DDD1'), MN_AKI_AKI_BIKE2_LP, 0.0f, 127.0f, 1.0f, 1.0f, 0);
  Script_vehiUtils(VehicleCommand_AttachRiderToVehicle, SHEN_NAME('DDD1'), 0x59434942, 0);
  Script_vehiUtils(VehicleCommand_Unknown8_SetFlag, SHEN_NAME('DDD1'), 0);
  Script_vehiUtils(VehicleCommand_StartPathFollow, SHEN_NAME('DDD1'), &data_1A0FC, 3, 1);
  Script_vehiUtils(VehicleCommand_SetMaxSpeed, SHEN_NAME('DDD1'), 18.0f);
  Script_vehiUtils(VehicleCommand_SetSpeedMotion, SHEN_NAME('DDD1'), 0, 1106);
  Script_vehiUtils(VehicleCommand_SetSpeedMotion, SHEN_NAME('DDD1'), 1, 1107);
  script_83F5(10);
  Script_vehiUtils(VehicleCommand_SetControlProfile, SHEN_NAME('DDD1'), 2, 0.0f);
  script_83F5(11);
  return Script_vehiUtils(VehicleCommand_SetControlProfile, SHEN_NAME('DDD1'), 3, 0.94f);
}

static int32_t script_C82B(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t unused_D0[3];
  Vector3i local_C4;
  int32_t unused_B8[3];
  int32_t local_B4;
  script_8ECC(arg0);
  Script_MoveObjectToObject(arg0, 0x43474D59);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  local_C4.x = 0;
  (&local_C4)->y = 0;
  (&local_C4)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
  script_83F5(13);
  local_B4 = Script_scenCreateTask(script_8DFA, 1, arg0);
  while (1) {
    if (script_7F94(12)) {
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

static int32_t script_C947(int32_t arg0) {
  Vector3 local_F4;
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00C96B;
    }
  }
  L00C96B:;
  Script_scenToggleVisibleFor(arg0, 1);
  script_8EE0(arg0);
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
  script_8EE0(arg0);
  script_83F5(13);
  script_8ECC(arg0);
  Script_PlayCharaMotion(arg0, 8243, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  (void)((task_284 = -1) & 2);
  while (task_284 & 0x20) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = 1.6f;
    Script_SetLookPointDetails(arg0, 6, &local_F4, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
  end:;
}

static int32_t script_CA4D(int32_t arg0) {
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00CA71;
    }
  }
  L00CA71:;
  Script_scenToggleVisibleFor(arg0, 1);
  script_8ECC(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
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
  script_826F(arg0, -24.57f, 40.05f, 0x10000, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_83F5(6);
  script_8ECC(arg0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_83F5(1);
  if (script_7F94(0) == 0) {
    Vector3 local_F4;
    int32_t local_F0;
    (void)((task_282 = -1) & 1);
    script_8EF4(arg0, 5, -24.18627f, 39.859962f);
    Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
    Script_charaToggleMotEnabled(arg0, 1);
    Script_PlayCharaMotion(arg0, 8242, 6.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
    Script_scenCreateTask(script_FB56, 0);
    script_8070(arg0, 61.0f);
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
    (void)((task_282 = 0) & 2);
    SCNF_WaitFrames_0000(14);
    Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
    Script_PlayCharaMotion(arg0, 8226, 60.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      Script_RotateNodeUtils(arg0, &data_184E0, 0);
      SCNF_WaitFrames_0000(1);
    }
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  }
  end:;
}

static int32_t script_CCD9(int32_t arg0) {
  script_8ECC(arg0);
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
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_83F5(7);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_85AF(arg0, CHARA_EIIU, 0.9f);
  script_7F3B(16);
  Script_PlayCharaMotion(arg0, 8213, 28.0f, -1.0f, -1.0f, 1.0f, 0);
  script_8054(arg0);
  Script_PlayCharaMotion(arg0, 8214, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  end:;
}

static int32_t script_CDD0(int32_t arg0) {
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
  script_8EE0(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_83F5(24);
  script_8ECC(arg0);
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
  script_83F5(17);
  if (script_7F94(19)) {
    script_83F5(17);
    script_83F5(1);
    if (script_7F94(0)) {
      Script_PlayCharaMotion(arg0, 8222, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      script_8054(arg0);
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(arg0, 1.1f);
    } else {
      Script_charaToggleMotEnabled(arg0, 0);
      Script_PlayCharaMotion(arg0, 8218, 10.0f, -1.0f, -1.0f, 1.0f, 0);
      for (;;) {
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
        Script_RotateNodeUtils(arg0, &data_184E0, 0);
        SCNF_WaitFrames_0000(1);
      }
      Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      script_8054(arg0);
    }
  }
  script_7F66(24);
  end:;
}

static int32_t script_CFB5(int32_t arg0) {
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
  script_8EE0(arg0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(arg0, 8239, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_83F5(24);
  script_8ECC(arg0);
  script_83F5(25);
  if (script_7F94(0)) {
    Script_ShowUIMessageFromID("TE1004AO001");
  } else {
    Script_PlayCharaMotion(arg0, 8219, 2.1019477e-44f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(arg0, 0);
    script_8070(arg0, 30.0f);
    script_8054(arg0);
  }
  script_7F66(25);
  end:;
}

static void script_D0B0(int32_t arg0) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, CCOW_DisableAll);
  script_8EE0(arg0);
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKSK, -1, &local_F4, 0);
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1A120, 4, data_1A150, 5, data_1A15C)))
static int32_t script_D0EB(int32_t arg0) {
  Vector3 local_F4;
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  {
    Vector3 local_E8;
    local_E8.x = -68.4f;
    (&local_E8)->z = 60.19f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_E8);
  }
  script_826F(arg0, -62.38f, 53.68f, 0x10000, 0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_18);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_83F5(9);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_829A(arg0, -62.38f, 53.68f, 2730, 0);
  script_826F(arg0, -46.66f, 42.91f, 910, 0);
  script_80CF(arg0, -54.97f);
  script_83F5(1);
  if (script_7F94(0)) {
    script_80CF(arg0, -54.03f);
    Script_executeAicaCommand(524457, 0, 0);
    script_8332(arg0, -52.5f, 45.17f, 1.4f, 1820, 1);
    script_826F(arg0, -46.66f, 42.91f, 728, 0);
  } else {
    script_80CF(arg0, -54.97f);
    Script_PlayCharaMotion(arg0, 8208, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(589993, 0, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    script_8070(arg0, 36.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    script_8332(arg0, -52.45f, 45.76f, 1.8f, 1820, 1);
    script_826F(arg0, -46.66f, 42.91f, 910, 0);
  }
  script_80CF(arg0, -46.66f);
  script_826F(arg0, -42.83f, 42.77f, 728, 0);
  script_8EE0(SHEN_NAME('DDD1'));
  script_8EE0(SHEN_NAME('BIKE'));
  script_83F5(1);
  if (script_7F94(0)) {
    script_80CF(arg0, -37.67f);
    Script_PlayCharaMotion(CHARA_AKSK, 8217, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(458921, 0, 0);
    Script_SetCharaMotionPlaybackRate(CHARA_AKSK, 2.0f);
    script_8332(arg0, -37.27f, 42.3f, 1.4f, 910, 1);
    script_826F(arg0, -25.92f, 40.36f, 910, 0);
  } else {
    Vector3i local_E8;
    script_80CF(arg0, -37.83f);
    (&local_E8)->y = 7281;
    Script_EVT_SetRotation(CHARA_AKSK, -0x70000000, &local_E8, 0);
    Script_charaToggleMotEnabled(CHARA_AKSK, 1);
    Script_PlayCharaMotion(CHARA_AKSK, 8220, 65.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(arg0, 8195, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    Script_ccowSetHeightMode(CHARA_AKSK, CCOW_KeepHeight);
    Script_EVT_GetPosition(CHARA_AKSK, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = (&local_F4)->y + 0.05f;
    Script_EVT_SetPosition(CHARA_AKSK, 0x78000000, &local_F4);
    script_8070(CHARA_AKSK, 122.0f);
    Script_scenCreateTask(script_7F08, 0);
    script_8054(arg0);
    Script_ccowSetHeightMode(CHARA_AKSK, CCOW_FollowGround);
    Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, 8.0f, 0.8f, 0);
    Script_scenCreateTask(script_FC34, 2, CHARA_AKIR, 0x3F4CCCCD);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    while (1) {
      if (script_826F(arg0, -25.92f, 40.36f, 1820, 1)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_80CF(arg0, -27.92f);
  script_83F5(1);
  if (script_7F94(0)) {
    script_80CF(arg0, -27.62f);
    script_8332(arg0, -24.64f, 41.82f, 0.9f, 1820, 0);
    Script_executeAicaCommand(458921, 0, 0);
    script_7F3B(19);
    script_826F(arg0, -8.7349f, 30.3598f, 910, 1);
  } else {
    script_80CF(arg0, -27.82f);
    Script_PlayCharaMotion(arg0, 8194, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_executeAicaCommand(589993, 0, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
    script_8054(arg0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    Script_SetCharaMotionPlaybackRate(arg0, 0.7f);
    script_826F(arg0, -8.7349f, 30.3598f, 910, 1);
  }
  (void)((task_283 = 0) & 0x10);
  if (script_7F94(19)) {
    script_83F5(17);
    script_83F5(1);
    if (script_7F94(0)) {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x80);
      Script_PlayCharaMotion(arg0, 8206, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
      Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
      Script_executeAicaCommand(458921, 0, 0);
      script_7F3B(20);
    } else {
      script_826F(arg0, -8.7349f, 30.3598f, 910, 1);
      Script_PlayCharaMotion(arg0, 8204, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
      script_8070(arg0, 74.0f);
      Script_scenCreateTask(script_7DD0, 0);
      script_8054(arg0);
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    }
  } else {
    if (script_7F94(19) == 0) {
      (void)((task_283 = -1) & 0x10);
      {
        Vector3 local_E8;
        local_E8.x = -24.6513f;
        (&local_E8)->z = 40.043301f;
        Script_EVT_SetPosition(arg0, 0x28000000, &local_E8);
      }
      {
        Vector3i local_E8;
        (&local_E8)->y = 55655;
        Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
      }
      Script_mapmSetCamFlags(0, 0);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
      Script_seqEnterEvtCam(1503, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
      script_83F5(23);
      script_83F5(1);
      if (script_7F94(0)) {
        Vector3i local_E8;
        Script_PlayCharaMotion(arg0, 8209, 9.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        Script_executeAicaCommand(458921, 0, 0);
        script_8070(arg0, 31.0f);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        (&local_E8)->y = 55655;
        Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
      } else {
        Vector3i local_E8;
        (&local_E8)->y = 52925;
        Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E8, 0);
        Script_ShowUIMessageFromID("TE1004AA005");
        Script_PlayCharaMotion(arg0, 8205, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        SCNF_WaitFrames_0000(3);
        Script_executeAicaCommand(196777, 0, 0);
        SCNF_WaitFrames_0000(17);
        Script_ShowUIMessageFromID("TE1004AO003");
        SCNF_WaitFrames_0000(10);
        Script_ShowUIMessageFromID("TE1004AA021");
        script_8054(arg0);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        script_826F(arg0, -8.7349f, 30.3598f, 910, 1);
      }
      goto L00D94B;
    }
  }
  L00D94B:;
  if (script_7F94(19)) {
    script_C687();
    if (script_7F94(20)) {
      script_83F5(18);
      script_83F5(1);
      if (script_7F94(0)) {
        SCNF_WaitFrames_0000(8);
        Script_ShowUIMessageFromID("TE1004AA022");
        Script_executeAicaCommand(524457, 0, 0);
        Script_mapmSetCamFlags(0, 0);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
        script_8054(arg0);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      } else {
        Script_PlayCharaMotion(arg0, 8205, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
        SCNF_WaitFrames_0000(10);
        Script_ShowUIMessageFromID("TE1004AA023");
        SCNF_WaitFrames_0000(10);
        Script_ShowUIMessageFromID("TE1004AO002");
        script_8054(arg0);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
      }
    }
    script_826F(arg0, -8.7349f, 30.3598f, 910, 1);
  }
  script_80CF(arg0, -20.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
  Script_mapmSetCamFlags(0, 1);
  Script_seqEnterEvtCam(1503, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
  script_8EE0(CHARA_MNWA);
  script_8EE0(CHARA_STAK);
  script_8EE0(CHARA_NORK);
  script_8EE0(CHARA_SUMI);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  script_83F5(12);
  script_80CF(arg0, -16.83f);
  script_83F5(2);
  if (script_7F94(0)) {
    script_80CF(arg0, -13.83f);
    Script_executeAicaCommand(917673, 0, 0);
    Script_PlayCharaMotion(arg0, 8231, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
    script_8070(arg0, 36.0f);
    Script_executeAicaCommand(983209, 0, 0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
  } else {
    script_80CF(arg0, -12.83f);
    Script_PlayCharaMotion(arg0, 8207, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_7CD1, 0);
    script_C673(arg0, 0x41A00000, 786601);
    script_8070(arg0, 60.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  script_80CF(arg0, -9.49f);
  script_826F(arg0, 23.37f, 28.79f, 1820, 0);
  script_80CF(arg0, -4.91f);
  if (script_7F94(3)) {
    script_83F5(15);
    script_826F(arg0, 21.52f, 31.27f, 546, 0);
    script_80CF(arg0, 21.57f);
    script_829A(arg0, 22.32f, 34.49f, 2184, 0);
    script_2C35(0x55494945, 3);
    script_85F6(arg0);
  } else {
    Script_PlayCharaMotion(arg0, 8202, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 0.7f);
    Script_executeAicaCommand(589993, 0, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    script_8070(arg0, 25.0f);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_LOOK_LR, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  }
  end:;
}

static int32_t script_DD59(int32_t arg0) {
  Vector3 local_F4;
  script_8ECC(arg0);
  local_F4.x = -65.04f;
  (&local_F4)->z = 56.79f;
  Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_829A(arg0, -47.22f, 41.47f, 0x10000, 0);
  script_829A(arg0, -24.34f, 42.29f, 1820, 0);
  script_829A(arg0, -10.36f, 30.31f, 910, 1);
  script_83F5(2);
  if (script_7F94(0)) {
    script_829A(arg0, 21.01f, 30.53f, 910, 0);
  }
  script_8EE0(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  end:;
}

static int32_t script_DE35(int32_t arg0) {
  script_7F3B(7);
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
  script_829A(arg0, -24.34f, 42.29f, 0x10000, 0);
  script_829A(arg0, -10.36f, 30.31f, 910, 1);
  script_829A(arg0, 21.01f, 30.53f, 910, 0);
  script_8EE0(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  end:;
}

static int32_t script_DED9(int32_t arg0) {
  Vector3 local_F4;
  script_8EE0(arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  script_83F5(2);
  if (script_7F94(3)) {
    script_8ECC(arg0);
    local_F4.x = 4.58f;
    (&local_F4)->z = 30.36f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
    Script_charaToggleMotEnabled(arg0, 1);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_829A(arg0, 21.01f, 30.53f, 0x10000, 0);
    script_826F(arg0, 22.47f, 32.78f, 1820, 0);
  }
  SCNF_WaitFrames_0000(10);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(20);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  end:;
}

__attribute__((scn_string_bindings(2, data_1A180, 3, data_1A18C, 4, data_1A198, 5, data_1A1A2, 6, data_1A1AE)))
static int32_t script_DFD1(void) {
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
  int32_t unused_94[3];
  float local_90;
  float local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t local_80;
  local_BD = 0;
  local_80 = 0;
  local_88 = 0;
  local_90 = Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
  local_8C = Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
  local_C4 = 0;
  while (local_C4 < 12) {
    *(&local_D0 + local_C4) = 0;
    local_C4 = local_C4 + 1;
  }
  Script_FACE(CHARA_EIIU, 5, 1, 10);
  local_D0 = Script_scenCreateTask(script_D0EB, 1, CHARA_AKIR);
  *(&local_D0 + 1) = Script_scenCreateTask(script_DD59, 1, CHARA_EIIU);
  *(&local_D0 + 2) = Script_scenCreateTask(script_CA4D, 1, CHARA_NORK);
  *(&local_D0 + 3) = Script_scenCreateTask(script_CCD9, 1, CHARA_AKSK);
  *(&local_D0 + 4) = Script_scenCreateTask(script_C947, 1, CHARA_YMGC);
  *(&local_D0 + 5) = Script_scenCreateTask(script_C82B, 1, SHEN_NAME('ABBX'));
  *(&local_D0 + 6) = Script_scenCreateTask(script_C753, 0);
  *(&local_D0 + 7) = Script_scenCreateTask(script_865C, 2, SHEN_NAME('PIL4'), CHARA_AKSK);
  *(&local_D0 + 8) = Script_scenCreateTask(script_865C, 2, SHEN_NAME('PIL0'), CHARA_AKIR);
  *(&local_D0 + 9) = Script_scenCreateTask(script_865C, 2, SHEN_NAME('PIL1'), CHARA_EIIU);
  *(&local_D0 + 10) = Script_scenCreateTask(script_CDD0, 1, CHARA_MNWA);
  *(&local_D0 + 11) = Script_scenCreateTask(script_CFB5, 1, CHARA_STAK);
  script_8ECC(SHEN_NAME('CAR4'));
  script_873B(2);
  local_BF = 0;
  local_BE = -1;
  local_BC = 0;
  local_BB = -1;
  local_B9 = 0;
  script_7F66(24);
  script_7F66(27);
  script_7F66(28);
  script_7F66(29);
  script_7F66(3);
  script_8ECC(CHARA_YOSE);
  script_8EE0(SHEN_NAME('D201'));
  script_8EE0(SHEN_NAME('CBIK'));
  while (local_BF != -1) {
    if (local_BF != local_BE) {
      local_C8 = 0;
      local_BE = local_BF;
    } else {
      local_C8 = local_C8 + 1;
    }
    if (script_7F94(28)) {
      local_88 = local_88 + 1;
    }
    if (script_7F94(29)) {
      local_84 = local_84 + 1;
    }
    {
      int32_t unused_FF7C;
      int32_t local_FF78;
      local_FF78 = local_BF;
      switch (local_FF78) {
        case 0:
          if (local_C8 >= 30) {
            script_7F3B(9);
            local_BF = 1;
          }
          break;
        case 1:
          local_BC = 1;
          script_7F66(1);
          script_7F3B(10);
          local_BF = 2;
          break;
        case 2:
          if (local_C8 >= 45) {
            Script_scenCreateTask(script_8448, 3, 64, 15, 0);
            local_BF = 3;
          }
          break;
        case 3:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_BD = local_BD + 1;
            } else {
              script_7F3B(11);
            }
            local_BF = 4;
          }
          break;
        case 4:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_7F94(0)) {
              if (script_8115(CHARA_AKIR, -46.79f)) {
                local_FF77 = 1;
              }
            } else {
              if (script_8115(CHARA_AKIR, -46.79f)) {
                local_FF77 = 1;
              }
            }
            if (local_B9 == 0) {
              if (script_8115(CHARA_AKIR, -50.1f)) {
                Script_srchControl((void *)*(&local_D0 + 1));
                *(&local_D0 + 1) = Script_scenCreateTask(script_DE35, 1, CHARA_EIIU);
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
          script_7F66(1);
          local_BF = 6;
          break;
        case 6:
          if (local_C8 >= 15) {
            Script_scenCreateTask(script_8448, 3, 64, 15, 0);
            local_BF = 7;
          }
          break;
        case 7:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_BD = local_BD + 1;
            }
            local_BF = 8;
          }
          break;
        case 8:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_7F94(0)) {
              if (script_8115(CHARA_AKIR, -37.5f)) {
                local_FF77 = 1;
              }
            } else {
              if (script_8115(CHARA_AKIR, -35.16f)) {
                local_FF77 = 1;
              }
            }
            if (local_FF77) {
              local_BF = 9;
            }
            break;
          }
        case 9:
          script_8EE0(CHARA_KOND);
          script_8EE0(CHARA_MISM);
          local_BC = 3;
          script_7F66(1);
          script_7F66(19);
          script_7F3B(6);
          script_7F3B(24);
          script_7F3B(28);
          local_BF = 10;
          local_88 = 0;
          break;
        case 10:
          if (local_C8 >= 20) {
            Script_scenCreateTask(script_8448, 3, 4, 15, 0);
            local_BF = 11;
          }
          break;
        case 11:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_BD = local_BD + 1;
            }
            local_BF = 12;
          }
          break;
        case 12:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_7F94(0)) {
              if (script_8115(CHARA_AKIR, -27.8f)) {
                local_FF77 = 1;
              }
            } else {
              if (script_8115(CHARA_AKIR, -25.0f)) {
                local_FF77 = 1;
              }
            }
            if (local_FF77) {
              if (script_7F94(0)) {
                local_BF = 13;
                script_7F3B(19);
              } else {
                local_BF = 21;
                script_7F66(19);
              }
            }
            break;
          }
        case 13:
          local_BC = 4;
          script_7F66(1);
          script_7F3B(17);
          local_BF = 14;
          break;
        case 14:
          if (local_C8 >= 0) {
            Script_scenCreateTask(script_8448, 3, 64, 15, 0);
            local_BF = 15;
          }
          break;
        case 15:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_BD = local_BD + 1;
            }
            local_BF = 16;
          }
          break;
        case 16:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_7F94(0)) {
              if (script_8115(CHARA_AKIR, -25.5f)) {
                local_FF77 = 1;
              }
            } else {
              if (script_8115(CHARA_AKIR, -20.6f)) {
                local_FF77 = 1;
              }
            }
            if (local_FF77) {
              if (script_7F94(0)) {
                local_BF = 17;
                script_7F3B(20);
              } else {
                local_BF = 25;
                script_7F66(20);
              }
            }
            break;
          }
        case 17:
          local_BC = 5;
          script_7F66(1);
          script_7F66(21);
          script_7F66(22);
          script_7F66(25);
          script_7F66(26);
          script_7F3B(18);
          local_BF = 18;
          break;
        case 18:
          if (local_C8 >= 10) {
            Script_scenCreateTask(script_8448, 3, 4, 15, 0);
            local_BF = 19;
          }
          break;
        case 19:
          if (script_7F94(1)) {
            script_7F3B(25);
            if (script_7F94(0)) {
              script_7F3B(26);
              local_BD = local_BD + 1;
            }
            local_BF = 20;
          }
          break;
        case 20:
          script_7F3B(21);
          if (script_7F94(0)) {
            script_7F3B(22);
          }
          local_BF = 25;
          break;
        case 21:
          local_BC = 6;
          script_7F66(1);
          script_7F66(25);
          script_7F66(26);
          script_7F3B(23);
          local_BF = 22;
          break;
        case 22:
          if (local_C8 >= 0) {
            Script_scenCreateTask(script_8448, 3, 4, 15, 0);
            local_BF = 23;
          }
          break;
        case 23:
          if (script_7F94(1)) {
            script_7F3B(25);
            if (script_7F94(0)) {
              local_BD = local_BD + 1;
              script_7F3B(26);
            }
            local_BF = 24;
          }
          break;
        case 24:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_7F94(0)) {
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
          script_8ECC(CHARA_UDGW);
          script_8ECC(CHARA_KYMA);
          local_BC = 7;
          script_7F66(1);
          script_7F3B(13);
          local_BF = 27;
          break;
        case 27:
          if (script_8115(CHARA_AKIR, -17.5f)) {
            Script_scenCreateTask(script_8448, 3, 0x100, 15, 0);
            local_BF = 26;
            script_7F3B(27);
            script_7F3B(29);
            local_84 = 0;
          }
          if (script_8115(CHARA_AKIR, -20.0f)) {
            script_7F3B(13);
          }
          break;
        case 26:
          script_7F3B(12);
          local_BF = 28;
          break;
        case 28:
          if (script_7F94(1)) {
            if (script_7F94(0)) {
              local_BD = local_BD + 1;
            }
            if (local_BD >= 3 - task_250 / 2) {
              script_7F3B(3);
            } else {
              script_7F66(3);
            }
            script_7F3B(2);
            local_BF = 29;
          }
          break;
        case 29:
          {
            int8_t local_FF77;
            local_FF77 = 0;
            if (script_7F94(0)) {
              if (script_8115(CHARA_AKIR, -9.5f)) {
                local_FF77 = 1;
              }
            } else {
              if (script_8115(CHARA_AKIR, -9.5f)) {
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
          *(&local_D0 + 1) = Script_scenCreateTask(script_DED9, 1, CHARA_EIIU);
          local_BF = 31;
          break;
        case 31:
          if (script_7F94(3)) {
            local_BF = 32;
          } else {
            local_BF = 33;
          }
          break;
        case 32:
          if (script_8115(CHARA_EIIU, 22.53f)) {
            script_7F3B(15);
            local_BF = 33;
          }
          break;
        case 33:
          if (script_7F94(3)) {
            if (script_8115(CHARA_AKIR, 20.77f)) {
              local_BF = 34;
            }
          } else {
            if (script_8115(CHARA_AKIR, -5.91f)) {
              local_BF = 34;
            }
          }
          break;
        case 34:
          {
            int32_t local_FF74;
            if (script_7F94(3)) {
              local_FF74 = local_C8 + 1;
            } else {
              local_FF74 = 160;
            }
            if (local_C8 >= local_FF74) {
              Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
              if (script_7F94(3)) {
                Script_executeAicaCommand(2720, 5, 30);
              } else {
                if (task_1FC & 8) {
                  Script_executeAicaCommand(2720, 5, 15);
                  Script_uiUtils(0, 37, 0);
                }
              }
              local_BF = 35;
            }
            break;
          }
        case 35:
          if (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
            Script_filmCheckOrSetTransitionUtil(100, 255, 0, 0, 0, 255, 0, 0, 0);
            script_7F3B(15);
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
          script_7F3B(13);
          break;
        case 150:
          script_7F3B(12);
          break;
        case 153:
          if (task_282 == 0) {
            Script_executeAicaCommand(0x1100A9, 0, 0);
          }
          break;
        case 156:
          if (task_282 == 0) {
            Script_executeAicaCommand(0x1200A9, 0, 0);
          }
          break;
        case 160:
          if (task_282 == 0) {
            Script_executeAicaCommand(0x1300A9, 0, 0);
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
              if (script_7F94(5)) {
                Script_seqEnterEvtCam(1480, 0, AUTH_None);
              }
              break;
            case 20:
              if (script_7F94(5)) {
                Script_seqEnterEvtCam(1600, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                goto L00ED5E;
              }
              L00ED5E:;
              break;
            case 5:
              Script_scenCreateTask(script_7D91, 0);
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
                Script_executeAicaCommand(0x1D00A9, 0, 0);
                break;
              case 110:
                if (script_7F94(0)) {
                  Script_seqEnterEvtCam(1475, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
                }
                break;
              case 124:
                if (script_7F94(0) == 0) {
                  Script_seqEnterEvtCam(1608, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                }
                break;
              case 141:
                if (script_7F94(0) == 0) {
                  Script_seqEnterEvtCam(1475, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
                }
                break;
              case 84:
                if (script_7F94(0) == 0) {
                  Script_scenCreateTask(script_7CD1, 0);
                }
                break;
              case 100:
                if (script_7F94(0) == 0) {
                  Script_scenCreateTask(script_7D9D, 0);
                }
                break;
              case 78:
                if (script_7F94(0) == 1) {
                  Script_scenCreateTask(script_7C92, 0);
                }
                break;
              case 87:
                if (script_7F94(0) == 1) {
                  Script_scenCreateTask(script_7DA9, 0);
                }
                break;
              case 42:
                Script_seqEnterEvtCam(1507, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                Script_ShowUIMessageFromID("TE1004AE009");
                script_8EE0(CHARA_YOSE);
                script_8EE0(SHEN_NAME('HAKO'));
                script_8EE0(SHEN_NAME('FLIT'));
                break;
            }
          }
          break;
        case 2:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 0:
              Script_scenCreateTask(script_7E84, 0);
              Script_executeAicaCommand(196777, 0, 0);
              break;
            case 12:
              if (script_7F94(16) == 1) {
                Script_scenCreateTask(script_7DB5, 0);
              }
              break;
            case 14:
              Script_seqEnterEvtCam(1401, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              break;
            case 15:
              if (!(script_7F94(16) == 1)) goto L00EFFD;
              L00EFFD:;
              break;
            case 36:
              script_8EE0(SHEN_NAME('DDD1'));
              script_8EE0(SHEN_NAME('BICY'));
              script_8EE0(CHARA_KOND);
              script_8EE0(CHARA_MISM);
              if (script_7F94(0)) {
                Script_seqEnterEvtCam(1400, SHEN_NAME('PIL4'), SHEN_NAME('PIL4'));
              } else {
                Script_seqEnterEvtCam(1403, SHEN_NAME('PIL4'), SHEN_NAME('PIL4'));
              }
              break;
            case 42:
              if (script_7F94(0) == 0) {
                Script_executeAicaCommand(721065, 0, 0);
              }
              break;
            case 60:
              if (script_7F94(0) == 0) {
                Script_executeAicaCommand(786601, 0, 0);
              }
              break;
            case 50:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7DD0, 0);
              }
              break;
            case 48:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7EA8, 0);
              } else {
                Script_scenCreateTask(script_7DB5, 0);
              }
              break;
            case 49:
              if (script_7F94(0)) {
                Script_scenCreateTask(script_7EA8, 0);
              }
              break;
          }
          break;
        case 3:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 20:
              Script_seqEnterEvtCam(1401, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              break;
            case 40:
              if (script_7F94(0) == 0) {
                Script_seqEnterEvtCam(1330, 0, AUTH_None);
              } else {
                Script_seqEnterEvtCam(1331, 0, AUTH_None);
              }
              break;
            case 63:
              Script_executeAicaCommand(0x1700A9, 0, 0);
              break;
            case 68:
              Script_executeAicaCommand(983209, 0, 0);
              break;
            case 76:
              if (script_7F94(0) == 0) {
                Script_seqEnterEvtCam(1332, 0, AUTH_None);
              }
              break;
            case 49:
              if (script_7F94(0)) {
                Script_scenCreateTask(script_7C92, 0);
              }
              break;
            case 52:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7DDC, 0);
              } else {
                Script_ShowUIMessageFromID("TE1004AE011");
              }
              break;
            case 79:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7DE8, 0);
              }
              break;
            case 100:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7DF4, 0);
              }
              break;
          }
          break;
        case 4:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 35:
              if (script_7F94(0) == 0) {
                Script_executeAicaCommand(721065, 0, 0);
              }
              break;
            case 42:
              if (script_7F94(0) == 0) {
                Script_executeAicaCommand(786601, 0, 0);
              }
              break;
            case 46:
              if (script_7F94(0) == 0) {
                Script_executeAicaCommand(786601, 0, 0);
              }
              break;
            case 30:
              if (script_7F94(0) == 0) {
                Script_scenCreateTask(script_7E54, 0);
              }
              break;
            case 20:
              if (script_7F94(0)) {
                Script_scenCreateTask(script_7E48, 0);
              }
              break;
            case 45:
              if (script_7F94(0) == 0) {
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
              if (script_7F94(0)) {
                Script_mapmSetCamFlags(0, 0);
                Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
                Script_seqEnterEvtCam(1601, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
                Script_motmToggleFlags(CHARA_MNWA, SCEN_Disable, 0x40);
                local_A0.x = -21.811741f;
                (&local_A0)->y = 0.0f;
                (&local_A0)->z = 38.495483f;
                Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_A0);
                local_A0.x = -21.29567f;
                (&local_A0)->y = 0.0f;
                (&local_A0)->z = 39.595795f;
                Script_EVT_SetPosition(CHARA_MNWA, 0x78000000, &local_A0);
                Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
                Script_motmToggleFlags(CHARA_MNWA, SCEN_Enable, 0x40);
              } else {
                Script_executeAicaCommand(196777, 0, 0);
              }
              break;
            case 31:
              if (script_7F94(0) == 0) {
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
              Script_seqEnterEvtCam(1503, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              break;
            case 31:
              if (script_7F94(0) == 0) {
                Script_executeAicaCommand(721065, 0, 0);
              }
              break;
            case 20:
              if (script_7F94(0) == 0) {
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
              if (script_7F94(0)) {
                Script_seqEnterEvtCam(1611, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
            case 43:
              if (script_7F94(0) == 0) {
                Script_seqEnterEvtCam(1605, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
          }
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 72:
              if (script_7F94(0) == 0) {
                local_BA = 0;
              } else {
                local_BA = 1;
              }
              break;
            case 51:
              if (script_7F94(0)) {
                Script_scenCreateTask(script_7C92, 0);
              }
              break;
          }
          break;
        case 8:
          local_FF78 = local_CC;
          switch (local_FF78) {
            case 10:
              if (script_7F94(3)) {
                Script_seqEnterEvtCam(1609, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
                Script_ShowUIMessageFromID("TE1004AA030");
              }
              break;
            case 30:
              script_88F2();
              break;
            case 35:
              if (script_7F94(3) == 0) {
                Script_seqEnterEvtCam(1606, SHEN_NAME('PIL0'), SHEN_NAME('PIL0'));
              }
              break;
            case 50:
              if (script_7F94(3)) {
                Script_seqEnterEvtCam(1607, SHEN_NAME('PIL1'), SHEN_NAME('PIL1'));
                Script_scenCreateTask(script_7E30, 0);
              }
              break;
            case 65:
              if (script_7F94(3) == 0) {
                Script_scenCreateTask(script_7E24, 0);
              }
              break;
            case 75:
              if (script_7F94(3)) {
                Script_seqEnterEvtCam(1602, 0, AUTH_None);
                Script_FACE(CHARA_EIIU, 4, 4, 1);
              }
              break;
            case 85:
              if (script_7F94(3)) {
                Script_scenCreateTask(script_7E3C, 0);
              }
              break;
            case 115:
              if (script_7F94(3)) {
                Script_executeAicaCommand(2720, 5, 15);
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
                Script_scenCreateTask(script_FA88, 0);
                Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
                while (Script_filmCheckOrSetTransitionUtil(-1)) {
                  SCNF_WaitFrames_0000(1);
                }
                Script_shenClearFreeState(0);
                Script_shenRequestGameModuleTransition(80, SHEN_NAME('EPLG'), 0);
              }
              break;
          }
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_284 = 0) & 2);
  script_8EE0(CHARA_YOSE);
  script_8EE0(CHARA_EIIU);
  script_8EE0(CHARA_NORK);
  script_8EE0(CHARA_AKSK);
  script_8EE0(CHARA_YMGC);
  script_8EE0(CHARA_MNWA);
  script_8EE0(CHARA_STAK);
  script_8EE0(CHARA_UDGW);
  script_8EE0(CHARA_KYMA);
  Script_SetRyoMotionDetails(CHARA_UDGW);
  Script_SetRyoMotionDetails(CHARA_KYMA);
  script_8EE0(SHEN_NAME('DDD1'));
  script_8EE0(SHEN_NAME('BIKE'));
  script_8EE0(CHARA_NORK);
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
  script_7F3B(4);
  end:;
}

static void script_FA25(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  while (arg1) {
    local_F4.x = (&local_F4)->x + arg2;
    (&local_F4)->y = (&local_F4)->y + arg3;
    (&local_F4)->z = (&local_F4)->z + arg4;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1A1B8, 1, data_1A1C1, 2, data_1A1C6, 3, data_1A1CE)))
static int32_t script_FA88(void) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CJIM_PAK");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CTER");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "AUTHOR1");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "FACEPAK");
  script_8EAC(CHARA_EIIU, task_204);
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_19A70 + local_FC) != -1) {
      script_8EAC(*(&data_19A70 + local_FC), task_23C[local_FC]);
      local_FC = local_FC + 1;
    }
  }
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_19A30 + local_FC) != -1) {
      script_8EAC(*(&data_19A30 + local_FC), task_208[local_FC]);
      local_FC = local_FC + 1;
    }
  }
  end:;
}

static int32_t script_FB56(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  int32_t local_E4;
  float local_E0;
  Script_CharHandlers(CHARA_NORK, 0, 0, 16);
  local_E4 = 0;
  local_E0 = 0.0f;
  while (1) {
    if (Script_IsCharaMotionFinished(CHARA_NORK)) {
      break;
    }
    if (local_E4 > 110) {
      break;
    } else {
      if (local_E4 > 80) {
        Script_EVT_GetPosition(CHARA_AKIR, 12, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        if (local_E4 < 95) {
          local_E0 = local_E0 + 0.001f;
        }
        (&local_F4)->y = (&local_F4)->y + local_E0;
        Script_SetMotnFlags(CHARA_NORK, 17, &local_F4, 0x78000000);
        goto L00FBED;
      }
    }
    L00FBED:;
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  while (Script_IsInEventCam()) {
    Script_SetMotnFlags(CHARA_NORK, 17, &local_F4, 0x78000000);
    SCNF_WaitFrames_0000(1);
  }
  return Script_CharHandlers(CHARA_NORK, 0, 1, 16);
}

static int32_t script_FC34(int32_t arg0, float arg1) {
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

static int32_t script_FC6D(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t unused_DC[2];
  GameDateTime local_D4;
  int8_t local_D3;
  uint8_t unused_D0[3];
  local_E4 = -1;
  (void)((local_D3 = -1) & 4);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  if (!((&local_F4)->x <= -72.65f) & !((&local_F4)->x >= -70.87f)) {
    if (!((&local_F4)->z <= 79.36f) & !((&local_F4)->z >= 80.73f)) {
      if (Script_ryoControl(PCCTL_GetTatehiza)) {
        if (task_202 & 2) {
          (void)((task_202 = 0) & 4);
          SCNF_RequestIconNotification(0, 0, 0, 2);
        }
      } else {
        Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0x42000000);
        if (((&local_E8)->y < 0x4000) | ((&local_E8)->y > 0xC000)) {
          (void)((local_D3 = 0) & 1);
          Script_ReturnTimeDate(&local_D4);
          if ((&local_D4)->hours >= 16) {
            if (task_202 == 0) {
              SCNF_RequestIconNotification(0x100, 0, 0, 1);
              (void)((task_202 = -1) & 1);
            }
          }
          if (Script_GetControllerInput(1) & 0x400) {
            if ((&local_D4)->hours >= 16) {
              SCNF_RequestIconNotification(0, 0, 0, 2);
              local_E4 = 0;
            } else {
              local_E4 = 1;
            }
          }
        } else {
          if (task_202 & 2) {
            (void)((task_202 = 0) & 1);
            SCNF_RequestIconNotification(0, 0, 0, 2);
          }
        }
      }
    }
  }
  if (local_D3 & 8) {
    if (task_202 & 2) {
      (void)((task_202 = 0) & 1);
      SCNF_RequestIconNotification(0, 0, 0, 2);
    }
  }
  return local_E4;
}

static void script_FDE9(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_PreserveAccumulator_00ED(1);
  Script_PreserveAccumulator_011C(1, 1000);
  Script_PreserveAccumulator_011C(0, 0, 60);
  Script_PreserveAccumulator_011C(4, 0x3E2E147B, 0x3E2E147B);
  local_FC = 0;
  local_F8 = 0x3C23D70A;
  while (local_FC < 100) {
    Script_PreserveAccumulator_011C(0, local_F8, 60);
    local_F8 = local_F8 = 0x3C23D70A;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1A1D6, 1, data_1A1DF, 2, data_1A1E5)))
static int32_t script_FE5C(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  float local_F0;
  float local_EC;
  Vector3 local_E0;
  int32_t local_DC;
  if (arg1 == 0) {
    task_250 = 0;
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "STARTMOT");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "SHUKI");
    Script_FreeTaskData(task_1F8);
    Script_ReallocateUNFO(task_1F4);
    Script_SetShadowRange(20.0f);
    (void)((task_253 = -1) & 2);
    (void)((task_256 = -1) & 2);
    (void)((task_257 = -1) & 2);
    Script_scenCreateTask(script_102C2, 0);
    Script_seqEnterEvtCam(1479, CHARA_AKIR, CHARA_AKIR);
    script_10699();
    Script_WeatherUtils_ForFIXC(14, 0);
    while (task_253 & 0x40) {
      SCNF_WaitFrames_0000(1);
    }
    Script_facemotUtils(CHARA_EIIU, 0, 0);
    Script_SetQteOverlayTimeoutTuning(QTE_TimeoutBegin);
    script_105C4(0);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 989, 1);
    Script_sprintf("save!\012");
    Script_CaptureTrialSaveState();
    Script_SetActionSelectorHUDElementVisibility(1);
    script_2C5E();
    Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
    Script_FaceEx(CHARA_EIIU, FACE_ExpressionEnabled, 0, 2);
    Script_SetCPAMFlags(CHARA_AKIR, 0);
    Script_SetCPAMFlags(CHARA_EIIU, 0);
  } else {
    task_250 = task_250 + 1;
    Script_scenCreateTask(script_104FF, 0);
    script_105C4(1);
    Script_SetActionSelectorHUDElementVisibility(1);
    while (task_255 & 1) {
      SCNF_WaitFrames_0000(1);
    }
    Script_SetQteOverlayTimeoutTuning(QTE_TimeoutAdvance);
  }
  local_E0.x = -74.057549f;
  (&local_E0)->y = 0.0724f;
  (&local_E0)->z = 80.285713f;
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_E0);
  Script_FACE(CHARA_AKIR, 0, 0, 1);
  Script_FACE(CHARA_EIIU, 4, 0, 1);
  script_2D5B(3);
  SCNF_WaitFrames_0000(1);
  script_8ECC(CHARA_EIIU);
  Script_scenToggleVisibleFor(SHEN_NAME('PIL0'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PIL1'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PIL2'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PIL3'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PIL4'), 1);
  task_251 = 0;
  local_F4 = 0;
  while (local_F4 < 6) {
    script_7F66(local_F4);
    local_F4 = local_F4 + 1;
  }
  script_7F3B(5);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ccowUtils(CHARA_EIIU, CCOW_DisableAll);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_motmToggleFlags(CHARA_EIIU, SCEN_Enable, 0x40);
  local_F0 = Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
  local_EC = Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
  local_F8 = 0;
  while (arg0 >= 0) {
    script_7F66(1);
    script_7F66(2);
    script_7F66(4);
    local_F4 = 6;
    while (local_F4 < 30) {
      script_7F66(local_F4);
      local_F4 = local_F4 + 1;
    }
    local_DC = arg0;
    switch (local_DC) {
      case 0:
        while (task_257 & 4) {
          SCNF_WaitFrames_0000(1);
        }
        local_FC = Script_scenCreateTask(script_B2E1, 0);
        break;
      case 1:
        local_FC = Script_scenCreateTask(script_C50A, 0);
        break;
      case 2:
        local_FC = Script_scenCreateTask(script_C51F, 0);
        break;
      case 3:
        local_FC = Script_scenCreateTask(script_DFD1, 0);
        break;
      case 4:
        while (task_256 & 2) {
          SCNF_WaitFrames_0000(1);
        }
        local_FC = Script_scenCreateTask(script_9A22, 0);
        Script_seqEnterEvtCam(1478, 0, AUTH_None);
        break;
    }
    while (1) {
      if (script_7F94(4)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_FC);
    if (arg0 == 4) {
      Script_scenCreateTask(script_10558, 0);
      arg0 = 0;
    } else if (arg0 <= 2) {
      if (script_7F94(3)) {
        local_F8 = local_F8 | 1;
        arg0 = 3;
      } else {
        arg0 = -1;
      }
    } else {
      if (script_7F94(3)) {
        local_F8 = local_F8 | 2;
      }
      arg0 = -1;
    }
  }
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, local_EC);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, local_F0);
  if (!(-(local_F8 == 3) | -(arg2 == 0))) goto L0102BE;
  L0102BE:;
  return local_F8;
}

__attribute__((scn_string_bindings(0, data_1A1EC, 1, data_1A1FC)))
static int32_t script_102C2(void) {
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  SCNF_WaitFrames_0000(1);
  task_204 = script_8E47(CHARA_EIIU);
  (void)((task_253 = 0) & 0x20);
  Script_refbSetActive(1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_EIIU, 1);
  script_1791E("/scene/80/TIKN/", "CJIM_PAK");
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_19A64 + local_FC) != -1) {
      task_23C[local_FC] = script_8E47(*(&data_19A64 + local_FC));
      local_FC = local_FC + 1;
    }
  }
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_19A30 + local_FC) != -1) {
      if (local_FC >= 2) {
        (void)((task_256 = 0) & 0x80);
      }
      task_208[local_FC] = script_8E47(*(&data_19A30 + local_FC));
      local_FC = local_FC + 1;
    }
  }
  (void)((task_257 = 0) & 2);
  (void)((task_255 = 0) & 2);
  return Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
}

static int32_t script_103D9(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_19A70 + local_FC) != -1) {
      script_8EAC(*(&data_19A70 + local_FC), task_23C[local_FC]);
      local_FC = local_FC + 1;
    }
  }
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_19A64 + local_FC) != -1) {
      task_23C[local_FC] = script_8E47(*(&data_19A64 + local_FC));
      local_FC = local_FC + 1;
    }
  }
  return (task_255 = 0) & 2;
}

static int32_t script_1046C(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_19A64 + local_FC) != -1) {
      script_8EAC(*(&data_19A64 + local_FC), task_23C[local_FC]);
      local_FC = local_FC + 1;
    }
  }
  {
    int32_t local_FC;
    local_FC = 0;
    while (*(&data_19A70 + local_FC) != -1) {
      task_23C[local_FC] = script_8E47(*(&data_19A70 + local_FC));
      local_FC = local_FC + 1;
    }
  }
  return (task_255 = -1) & 2;
}

static int32_t script_104FF(void) {
  Script_FACE(CHARA_AKIR, 0, 0, 1);
  Script_FACE(CHARA_EIIU, 0, 0, 1);
  Script_scenCreateTask(script_103D9, 0);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_10558(void) {
  SCNF_WaitFrames_0000(50);
  (void)((task_27C = -1) & 2);
  SCNF_WaitFrames_0000(1);
  if (task_254 & 0x80) {
    Script_fixoClearAttachment(SHEN_NAME('SEIR'));
  }
  script_8EE0(CHARA_HANA);
  script_8EE0(SHEN_NAME('SEIR'));
  script_8EE0(SHEN_NAME('DCYC'));
  script_8EE0(CHARA_SOBA);
  Script_scenCreateTask(script_1046C, 0);
  end:;
}

static void script_105C4(int32_t arg0) {
  int8_t local_FF;
  script_2C25(1);
  script_8ECC(CHARA_EIIU);
  script_8ECC(SHEN_NAME('DORR'));
  script_8ECC(SHEN_NAME('DORL'));
  script_8ECC(SHEN_NAME('BOXR'));
  Script_ccowUtils(CHARA_EIIU, CCOW_DisableAll);
  script_14D95(0);
  if (arg0) {
    (void)((local_FF = -(Script_shenSeqUtils(-7) != 0)) & 0x10);
  }
  if (local_FF & 0x80) {
  } else {
    Script_charaToggleMotEnabled(CHARA_AKIR, 1);
    Script_charaToggleMotEnabled(CHARA_EIIU, 1);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(CHARA_EIIU, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  script_2C25(0);
  return;
}

static void script_10699(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  Script_ccowSetHeightMode(CHARA_AKIR, CCOW_KeepHeight);
  local_F4.x = -71.45f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 80.16f;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x42000000);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_DC, 0);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_E8)->x - (&local_F4)->x, (&local_E8)->x - (&local_F4)->x) <= 1.0f)) {
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, 0, 0.0f, 0, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    while (1) {
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_E8)->x - (&local_F4)->x, (&local_E8)->x - (&local_F4)->x) >= 1.0f)) {
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  local_F4.x = -73.0f;
  (&local_F4)->z = 74.4f;
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, 0, 0.0f, 0, 0);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  return;
}

static void script_10812(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_F3;
  uint8_t unused_F0[3];
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_srchControl((void *)task_1EC);
  script_1695E();
  Script_srchControl((void *)task_1F0);
  (void)((task_1FC = -1) & 0x20);
  Script_ReturnTimeDate(&local_F4);
  Script_sicnUtils(1, 0, 0);
  local_F3 = 0;
  while (1) {
    Script_SetNewDateTime(&local_F4);
    Script_executeAicaCommand(2720, 1, 1);
    if (script_FE5C(4, local_F3, local_F3 - 2) == 3) {
      break;
    }
    local_F3 = local_F3 + 1;
    if (local_F3 >= 3) {
      break;
    }
    if (local_F3 == 2) {
      (void)((task_1FC = 0) & 2);
    }
    Script_uiUtils(5, 30, local_F3 + 3);
    Script_sicnUtils(0, 0, 1);
    if (local_F3 == 1) {
      Script_ClearInputFlags();
    }
    SCNF_WaitFrames_0000(90);
    Script_uiUtils(7, 30, local_F3 + 3);
    SCNF_WaitFrames_0000(30);
    Script_sicnUtils(1, 0, 0);
  }
  if (local_F3 >= 3) {
    Script_scenCreateTask(script_FA88, 0);
    Script_uiUtils(5, 60, 1);
    Script_executeAicaCommand(2720, 5, 100);
    SCNF_WaitFrames_0000(210);
    Script_uiUtils(7, 60, 1);
    SCNF_WaitFrames_0000(60);
    SCNF_WaitFrames_0000(5);
    MODU_Script_shenRequestGameModule(Display, 80, 0x4E4B4954, 0);
  }
  return;
}

static int32_t script_10958(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_1097C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  task_2B4[arg1] = 0;
  task_2C0[arg1] = 0;
  task_2CC[arg1] = 0;
  task_2D8[arg1] = arg2;
  task_2E4[arg1] = arg3;
  task_2F0 = 0;
  Script_CharHandlers(arg0, 0, task_2D8[arg1], 16);
  Script_CharHandlers(arg0, 1, task_2E4[arg1], 16);
  for (;;) {
    task_2B4[arg1] = 0;
    while (task_2B4[arg1] == 0) {
      SCNF_WaitFrames_0000(1);
    }
    task_2F0 = task_2CC[arg1] | 0x40000000;
    Script_scenCreateTask(arg4, 3, arg0, arg1, task_2B4[arg1] + task_2C0[arg1]);
  }
  end:;
}

static int32_t script_10A53(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  local_FC = task_2F0;
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

static int32_t script_10AEA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 0, task_2D8[arg1], 16);
  Script_CharHandlers(arg0, 1, task_2E4[arg1], 16);
  return Script_srchControl(0);
}

static int32_t script_10B4C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = task_2F0;
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

static void script_10BA8(int32_t arg0, float arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = task_2F0;
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

static void script_10C05(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_10C21(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5) {
  int32_t local_FC;
  task_318[arg1] = Script_scenCreateTask(script_1097C, 5, arg0, arg1, arg2, arg3, arg5);
  task_2F4[arg1] = arg0;
  task_300[arg1] = 0;
  task_30C[arg1] = 0;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_300[arg1]) {
      local_FC = task_30C[arg1] * 12;
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
          task_2B4[arg1] = local_F8;
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 6);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      if (task_2C0[arg1] == 0) {
        Script_scenCreateTask(script_10F77, 4, Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 1), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 2), Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 3));
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
          task_2B4[arg1] = local_F8;
        }
      }
      script_10E88();
      {
        int32_t local_F8;
        if (task_32C == Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC)) {
          task_328 = 1;
          local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 10);
        } else {
          if (task_2C0[arg1] == 0) {
            task_334 = task_334 + Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 4);
          }
          task_328 = 0;
          local_F8 = Script_ScenDataUtil(arg4, SCND_ReadU32, local_FC + 11);
        }
        if (local_F8 > 0) {
          task_2B4[arg1] = local_F8;
        }
      }
      script_10E78(arg1);
    }
  }
  end:;
}

static void script_10E41(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  task_300[arg0] = 1;
  task_30C[arg0] = arg1;
  task_2C0[arg0] = arg2;
  task_2CC[arg0] = arg3;
  return;
}

static void script_10E78(int32_t arg0) {
  task_300[arg0] = 0;
  return;
}

static void script_10E88(void) {
  SCNF_WaitFrames_0000(2);
  while (task_32C == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_10EA8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_10E41(arg0, arg1, 0, arg3);
  script_10E41(arg2, arg1, 1, arg3);
  while (task_300[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_10EE2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_10E41(arg0, arg1, 0, arg3);
  script_10E41(arg2, arg1, 1, arg3);
  script_10E41(arg2 + 1, arg1, 2, arg3);
  while (task_300[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_10F2E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_10E41(arg0, arg1, 0, arg4);
  script_10E41(arg2, arg1, 1, arg4);
  script_10E41(arg3, arg1, 2, arg4);
  while (task_300[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_10F77(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_10FA5, 2, arg1, arg3);
  SCNF_UIUtils(arg0, arg2 + task_3E0, 2, arg3, 15, 0);
  return Script_srchControl(0);
}

static int32_t script_10FA5(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  task_330 = arg0;
  task_32C = 0;
  task_324 = 0;
  local_F8 = 0xFFFF;
  for (;;) {
    local_FC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_324 = task_324 + 1;
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
      if (task_324 >= arg0 + arg1 + 4) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((local_F8 & 0xFFF) == 0) {
    local_F8 = 0xFFFF;
  }
  task_32C = local_F8 & 0xFFF;
  return Script_srchControl(0);
}

static int32_t script_11066(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_338[arg1] = arg0;
    task_344[arg1][0] = (&local_F4)->x;
    task_344[arg1][1] = (&local_F4)->y;
    task_344[arg1][2] = (&local_F4)->z;
  }
  task_368[arg1] = 0.0f;
  for (;;) {
    Vector3 local_F4;
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_344[arg1][0] = (&local_F4)->x;
    task_344[arg1][1] = (&local_F4)->y;
    task_344[arg1][2] = (&local_F4)->z;
    task_368[arg1] = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, task_344[arg1][0] - task_344[arg2][0], task_344[arg1][2] - task_344[arg2][2]);
  }
  end:;
}

static void script_11154(void) {
  task_374 = 0.0f;
  task_378 = 0.0f;
  task_37C = 0.0f;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    Script_PrintDebugString(2, 24, "PX: %f\012", task_374);
    Script_PrintDebugString(2, 25, "PZ: %f\012", task_378);
    Script_PrintDebugString(2, 26, "AY: %f\012", task_37C);
    if (Script_GetInputData(2, 0) & 4) {
      if (Script_GetInputData(0, 2) & 0x4000) {
        task_374 = task_374 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x8000) {
        task_374 = task_374 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x1000) {
        task_378 = task_378 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x2000) {
        task_378 = task_378 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x40) {
        task_37C = task_37C - 1.0f;
      }
      if (Script_GetInputData(0, 2) & 0x80) {
        task_37C = task_37C + 1.0f;
      }
    }
  }
  return;
}

static void script_11272(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  Script_MoveObjectToObject(arg0, -1);
  Script_sprintf("DPos = %f, %f\012", (&local_F4)->x, (&local_F4)->z);
  Script_sprintf("DAng = %f\012", (int32_t)((float)(&local_E8)->y / 360.0f * (float)0x10000));
  return;
}

static void script_112CB(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  Vector3 local_D8;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_D8.x = arg2;
  (&local_D8)->z = arg3;
  Script_EVT_SetPosition(arg0, 0x28000000, &local_D8);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg4 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  return;
}

static void script_1133B(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  int32_t local_FC;
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_FC);
  return;
}

static void script_11367(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_11381, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_11381(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  script_112CB(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_113AC(int32_t arg0, int32_t arg1, float arg2) {
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

static void script_11400(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_11416, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_11416(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  script_113AC(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_1143D(int32_t arg0, float arg1) {
  Vector3i local_F4;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F4, 0);
  return;
}

static void script_11466(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return;
}

static void script_114BA(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_114CE, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_114CE(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_1143D(arg0, arg1);
  return Script_srchControl(0);
}

static void script_114F3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1150B, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_1150B(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  script_11466(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_11534(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1154C, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_1154C(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  float local_FC;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_FC = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    script_11466(arg0, 0.0f, 0.0f, local_FC);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_115A1(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    Script_EVT_GetPosition(task_2F4[local_E4], -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_2F4[local_E4], &local_E8, 0);
    task_380[local_E4][0] = (&local_F4)->x;
    task_380[local_E4][1] = (&local_F4)->y;
    task_380[local_E4][2] = (&local_F4)->z;
    task_3A4[local_E4][0] = (&local_E8)->x;
    task_3A4[local_E4][1] = (&local_E8)->y;
    task_3A4[local_E4][2] = (&local_E8)->z;
    task_3C8[local_E4] = Script_GetCharaMotionRemainingFramesScaled(task_2F4[local_E4]);
    task_3D4[local_E4] = Script_scenCharaGetMotAnimId(task_2F4[local_E4]);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_11686(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    local_F4.x = task_380[local_E4][0];
    (&local_F4)->y = task_380[local_E4][1];
    (&local_F4)->z = task_380[local_E4][2];
    local_E8.x = task_3A4[local_E4][0];
    (&local_E8)->y = task_3A4[local_E4][1];
    (&local_E8)->z = task_3A4[local_E4][2];
    Script_EVT_SetPosition(task_2F4[local_E4], POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_2F4[local_E4], 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(task_2F4[local_E4], task_3D4[local_E4], task_3C8[local_E4], -1.0f, -1.0f, 1.0f, 0);
    local_E4 = local_E4 + 1;
  }
  return;
}

static int32_t script_1176F(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg2;
  switch (local_FC) {
    case 1:
      break;
    case 2:
      Script_scenCreateTask(script_10A53, 6, 31006, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 3:
      Script_scenCreateTask(script_10958, 2, 169, 8);
      Script_scenCreateTask(script_10A53, 6, 30980, -1, 0, 0x3F800000, arg0, arg1);
      script_1219F(CHARA_AKIR, 4, 0, 1, -1);
      script_1219F(CHARA_AKIR, 4, 1, 5, 30);
      script_1219F(CHARA_AKIR, 0, -1, 5, 90);
      break;
    case 4:
      Script_scenCreateTask(script_10AEA, 5, arg0, arg1, 0, -1, 20);
      Script_scenCreateTask(script_10A53, 6, 31005, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 5:
      Script_scenCreateTask(script_10B4C, 4, arg0, 1, 10, 10);
      Script_scenCreateTask(script_10A53, 6, 30978, -1, 0x41B80000, 0x3F4CCCCD, arg0, arg1);
      break;
    case 6:
      Script_scenCreateTask(script_10958, 2, 327849, 5);
      Script_scenCreateTask(script_10958, 2, 589993, 20);
      Script_scenCreateTask(script_10A53, 6, 31003, -1, 0x41A00000, 0x3F4CCCCD, arg0, arg1);
      break;
    case 7:
      Script_scenCreateTask(script_10B4C, 4, arg0, 1, 11, 4);
      Script_scenCreateTask(script_10A53, 6, 30979, -1, 0x41000000, 0x3F800000, arg0, arg1);
      break;
    case 8:
      Script_scenCreateTask(script_10958, 2, 65705, 17);
      Script_scenCreateTask(script_10958, 2, 327849, 13);
      Script_scenCreateTask(script_10958, 2, 852137, 58);
      Script_scenCreateTask(script_10A53, 6, 31004, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 9:
      break;
    case 10:
      Script_scenCreateTask(script_10A53, 6, 31006, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 11:
      Script_scenCreateTask(script_10958, 2, 65705, 96);
      Script_scenCreateTask(script_10A53, 6, 31022, -1, 0x41400000, 0x3F800000, arg0, arg1);
      break;
    case 12:
      Script_scenCreateTask(script_10958, 2, 327849, 6);
      Script_scenCreateTask(script_10958, 2, 852137, 56);
      Script_scenCreateTask(script_10A53, 6, 31023, -1, 0x41400000, 0x3F800000, arg0, arg1);
      break;
    case 13:
      break;
    case 14:
      Script_scenCreateTask(script_10A53, 6, 31008, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 15:
      Script_scenCreateTask(script_10958, 2, 169, 20);
      Script_scenCreateTask(script_10AEA, 5, arg0, arg1, 0, 20, 15);
      Script_scenCreateTask(script_10A53, 6, 30985, -1, 0x41000000, 0x3F800000, arg0, arg1);
      break;
    case 16:
      Script_scenCreateTask(script_10958, 2, 852137, 42);
      Script_scenCreateTask(script_10A53, 6, 31008, -1, 0x41200000, 0x3F800000, arg0, arg1);
      break;
    case 17:
      Script_scenCreateTask(script_10A53, 6, 787, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_10A53, 6, 30986, 20, 0, 0x3F800000, arg0, arg1);
      script_1219F(CHARA_AKIR, 4, 0, 1, -1);
      script_1219F(CHARA_AKIR, 4, 1, 5, 30);
      script_1219F(CHARA_AKIR, 0, -1, 5, 90);
      break;
    case 18:
      Script_scenCreateTask(script_10A53, 6, 31009, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_10A53, 6, 31010, 31, 0, 0x3F800000, arg0, arg1);
      break;
  }
  return Script_srchControl(0);
}

static void script_11BC0(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_11BD2, 2, arg0, arg1);
  return;
}

static int32_t script_11BD2(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("TE02A001");
      break;
    case 1:
      Script_ShowUIMessageFromID("TE02A002");
      break;
    case 2:
      Script_ShowUIMessageFromID("TE02A003");
      break;
    case 3:
      Script_ShowUIMessageFromID("TE02A004");
      break;
    case 4:
      Script_ShowUIMessageFromID("TE02A005");
      break;
    case 5:
      Script_ShowUIMessageFromID("TE02A006");
      break;
    case 6:
      Script_ShowUIMessageFromID("TE02A007");
      break;
    case 7:
      Script_ShowUIMessageFromID("TE02A008");
      break;
    case 8:
      Script_ShowUIMessageFromID("TE02A009");
      break;
    case 9:
      Script_ShowUIMessageFromID("TE02A010");
      break;
    case 10:
      Script_ShowUIMessageFromID("TE02A011");
      break;
    case 11:
      Script_ShowUIMessageFromID("TE02A012");
      break;
    case 12:
      Script_ShowUIMessageFromID("TE02A014");
      break;
    case 13:
      Script_ShowUIMessageFromID("TE02A013");
      break;
    case 14:
      Script_ShowUIMessageFromID("TE02B001");
      break;
    case 15:
      Script_ShowUIMessageFromID("TE02B002");
      break;
    case 16:
      Script_ShowUIMessageFromID("TE02B003");
      break;
    case 17:
      Script_ShowUIMessageFromID("TE02B004");
      break;
    case 18:
      Script_ShowUIMessageFromID("TE02B005");
      break;
    case 19:
      Script_ShowUIMessageFromID("TE02B006");
      break;
    case 20:
      Script_ShowUIMessageFromID("TE02B007");
      break;
    case 21:
      Script_ShowUIMessageFromID("TE02B008");
      break;
    case 22:
      Script_ShowUIMessageFromID("TE02B009");
      break;
    case 23:
      Script_ShowUIMessageFromID("TE02B010");
      break;
    case 24:
      Script_ShowUIMessageFromID("TE02B011");
      break;
    case 25:
      Script_ShowUIMessageFromID("TE02B012");
      break;
    case 26:
      Script_ShowUIMessageFromID("TE02B013");
      break;
    case 27:
      Script_ShowUIMessageFromID("TE02B014");
      break;
    case 28:
      Script_ShowUIMessageFromID("TE02B015");
      break;
    case 29:
      Script_ShowUIMessageFromID("TE02B016");
      break;
    case 30:
      Script_ShowUIMessageFromID("TE02B017");
      break;
    case 31:
      Script_ShowUIMessageFromID("TE02B018");
      break;
    case 32:
      Script_ShowUIMessageFromID("TE02B019");
      break;
    case 33:
      Script_ShowUIMessageFromID("TE02C001");
      break;
    case 34:
      Script_ShowUIMessageFromID("TE02C002");
      break;
    case 35:
      Script_ShowUIMessageFromID("TE02C003");
      break;
    case 36:
      Script_ShowUIMessageFromID("TE02C004");
      break;
    case 37:
      Script_ShowUIMessageFromID("TE02C005");
      break;
    case 38:
      Script_ShowUIMessageFromID("TE02C006");
      break;
    case 39:
      Script_ShowUIMessageFromID("TE02C007");
      break;
    case 40:
      Script_ShowUIMessageFromID("TE02C008");
      break;
    case 41:
      Script_ShowUIMessageFromID("TE02C009");
      break;
    case 42:
      Script_ShowUIMessageFromID("TE02C010");
      break;
    case 43:
      Script_ShowUIMessageFromID("TE02C011");
      break;
    case 44:
      Script_ShowUIMessageFromID("TE02C012");
      break;
    case 45:
      Script_ShowUIMessageFromID("TE02C013");
      break;
    case 46:
      Script_ShowUIMessageFromID("TE02C014");
      break;
    case 47:
      Script_ShowUIMessageFromID("TE02C015");
      break;
    case 48:
      Script_ShowUIMessageFromID("TE02C016");
      break;
    case 49:
      Script_ShowUIMessageFromID("TE02C017");
      break;
    case 50:
      Script_ShowUIMessageFromID("TE02C018");
      break;
    case 51:
      Script_ShowUIMessageFromID("TE02C020");
      break;
    case 52:
      Script_ShowUIMessageFromID("TE02C019");
      break;
  }
  return Script_srchControl(0);
}

static void script_12052(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_12064, 2, arg0, arg1);
  return;
}

static int32_t script_12064(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static void script_12088(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_1209A, 2, arg0, arg1);
  return;
}

static int32_t script_1209A(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_InitializeOsag(arg0);
  return Script_srchControl(0);
}

static void script_120BB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_120D1, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_120D1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_120F5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_1210B, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_1210B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  script_1214B(arg0, 2);
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(arg0, arg2, &local_F4, 0);
  return Script_srchControl(0);
}

static void script_1214B(int32_t arg0, int32_t arg1) {
  Script_SetCharaLookAtPosition(arg0, -arg1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  return;
}

static void script_12166(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_1217A, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_1217A(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_1214B(arg0, arg1);
  return Script_srchControl(0);
}

static void script_1219F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_121B7, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_121B7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_121DE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Script_scenCreateTask(script_121FA, 7, arg2, arg6, arg3, arg4, arg0, arg1, arg5);
  return;
}

static int32_t script_121FA(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (arg6 == 3) {
    Script_motmToggleFlags(arg4, SCEN_Disable, 0x40);
  } else {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x40);
  }
  if (arg6 == 2) {
    Script_charaToggleMotEnabled(arg4, 0);
  } else {
    Script_charaToggleMotEnabled(arg4, 1);
  }
  if (arg6 != 1) {
    Script_motmToggleFlags(arg4, SCEN_Disable, 0x80);
  }
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (arg6 == 1) {
    Script_motmToggleFlags(arg4, SCEN_Enable, 0x80);
  }
  return Script_srchControl(0);
}

static void script_1229B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_122B1, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_122B1(float arg0, float arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetCamShake(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_122D6(int32_t arg0, int32_t arg1) {
  task_3E8 = arg0;
  task_288 = 0;
  return;
}

static void script_122E7(int32_t arg0, int32_t arg1) {
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
      SCNT_ShowSubtitle("開始５");
      break;
    case 16:
      SCNT_ShowSubtitle("成功５");
      break;
    case 17:
      SCNT_ShowSubtitle("失敗５");
      break;
    case 18:
      SCNT_ShowSubtitle("開始６");
      break;
    case 19:
      SCNT_ShowSubtitle("成功６");
      break;
    case 20:
      SCNT_ShowSubtitle("失敗６");
      break;
    case 42:
      SCNT_ShowSubtitle("オート１");
      break;
    case 43:
      SCNT_ShowSubtitle("オート２");
      break;
    case 44:
      SCNT_ShowSubtitle("オート３");
      break;
  }
  Script_srchControl(0);
  return;
}

static int32_t script_1249F(int32_t arg0, int32_t arg1) {
  int32_t unused_E8[6];
  int32_t local_E4;
  int32_t local_E0;
  L0124A1:;
  Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 0, 0, 0, 0);
  Script_uiUtils(0, 37, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_ccowUtils(task_2F4[1], CCOW_DisableAll);
    Script_ccowUtils(task_2F4[2], CCOW_DisableAll);
    local_E4 = Script_scenCreateTask(script_12892, 2, arg0, arg1);
    task_334 = 0;
    for (;;) {
      if (task_334 >= 10) {
        Script_srchControl((void *)local_E4);
        local_E0 = task_3E8;
        switch (local_E0) {
          case 8:
            SCNF_WaitFrames_0000(10);
            script_113AC(arg0, 0x45534142, 340.0f);
            Script_motmSetBlendFrames(CHARA_ENKI, MOTM_BLEND_FRAMES_30);
            script_121DE(arg0, arg1, 30996, 0x420C0000, 0x3F800000, 1, -1);
            script_121DE(CHARA_ENKI, 1, 31015, 0, 0x3F800000, 1, 3);
            Script_scenCreateTask(script_10958, 2, 852137, 10);
            SCNF_WaitFrames_0000(60);
            Script_motmSetBlendFrames(CHARA_ENKI, MOTM_BLEND_FRAMES_7);
            break;
          case 20:
            SCNF_WaitFrames_0000(10);
            script_113AC(arg0, 0x45534142, 180.0f);
            script_121DE(arg0, arg1, 30996, 0x420C0000, 0x3F800000, 1, -1);
            Script_motmSetBlendFrames(CHARA_ENKI, MOTM_BLEND_FRAMES_30);
            script_121DE(CHARA_ENKI, 1, 31015, 0, 0x3F800000, 1, 3);
            Script_scenCreateTask(script_10958, 2, 852137, 10);
            SCNF_WaitFrames_0000(60);
            Script_motmSetBlendFrames(CHARA_ENKI, MOTM_BLEND_FRAMES_7);
            break;
        }
        Script_srchControl((void *)local_E4);
        if (task_3E4 == 2) {
          script_13D65(arg0, arg1);
        }
        Script_SetQteOverlayTimeoutTuning(QTE_TimeoutAdvance);
        break;
      }
      if (task_334 == -1) {
        Script_uiUtils(0, 0, 0);
        Script_srchControl((void *)local_E4);
        script_13649(arg0, arg1);
        Script_SetQteOverlayTimeoutTuning(QTE_TimeoutReset);
        script_14700();
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    if (task_3E4 != 2) {
      script_11BC0(29, -1);
      script_11BC0(26, 70);
      SCNF_WaitFrames_0000(150);
      script_1214B(CHARA_ENKI, 2);
    }
    task_3E4 = task_3E4 + 1;
    if (task_3E4 == 1) {
      Script_executeAicaCommand(2720, 5, 15);
    } else {
      Script_executeAicaCommand(2720, 5, 100);
    }
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    script_112CB(CHARA_AKIR, 0x45534142, -106.0f, 105.1f, 48.0f);
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
    if (task_3E4 != 3) {
      if (task_3E4 == 1) {
        SCNF_WaitFrames_0000(30);
        Script_executeAicaCommand(0x1100A0, 5, 0);
        Script_ClearInputFlags();
      }
      task_3E0 = task_3E0 + 4;
      Script_uiUtils(5, 30, task_3E4 + 3);
      Script_sicnUtils(0, 0, 1);
      SCNF_WaitFrames_0000(90);
      Script_uiUtils(7, 30, task_3E4 + 3);
      SCNF_WaitFrames_0000(30);
      Script_sicnUtils(1, 0, 0);
      Script_executeAicaCommand(2464, 5, 10);
      Script_executeAicaCommand(278440, 0, 0);
      Script_SetNewDateTime(&task_29C);
    } else {
      Script_uiUtils(5, 60, 1);
      Script_executeAicaCommand(2720, 1, 100);
      SCNF_WaitFrames_0000(210);
      Script_uiUtils(7, 60, 1);
      SCNF_WaitFrames_0000(60);
      Script_executeAicaCommand(65696, 0, 0);
      MODU_Script_shenRequestGameModule(Display, 0, 0, 0);
    }
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
    script_121DE(CHARA_AKIR, 0, 655, 0, 0x3F800000, 3, -1);
  }
  goto L0124A1;
  end:;
}

static int32_t script_12892(int32_t arg0, int32_t arg1) {
  script_1214B(CHARA_AKIR, 1);
  script_132DE(arg0, arg1);
  script_1426E(1, 1, 1);
  script_120BB(8801, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_11BC0(24, -1);
  script_122D6(3, -1);
  script_112CB(task_2F4[1], arg0, -0.4f, -2.1f, 185.0f);
  script_10EA8(arg1, 0, 1, 0);
  if (task_328 == 0) {
    script_122D6(5, 10);
    script_120BB(8802, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 18);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
    script_11BC0(30, -1);
    script_12052(65705, 35);
    script_121DE(arg0, arg1, 30977, 0, 0x3F800000, 1, 25);
    script_121DE(CHARA_ENKI, 1, 31002, 0x41C80000, 0x3F800000, 1, 25);
    SCNF_WaitFrames_0000(70);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
    script_112CB(task_2F4[1], arg0, -0.4f, -1.95f, 195.0f);
    script_11BC0(27, -1);
    script_120BB(8801, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_122D6(6, -1);
    script_10EA8(arg1, 0, 1, 0);
    if (task_328 == 0) {
      int8_t local_FF;
      script_122D6(8, 10);
      script_1426E(1, 0, 0);
      SCNF_WaitFrames_0000(6);
      script_120BB(8803, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
      Script_motmToggleFlags(arg0, SCEN_Enable, 8);
      Script_motmToggleFlags(CHARA_ENKI, SCEN_Enable, 8);
      local_FF = 0;
      while (local_FF < 2) {
        SCNF_WaitFrames_0000(14);
        script_12052(169, 11);
        script_11BC0(25, -1);
        script_121DE(arg0, arg1, 30986, 0x41400000, 0x3F800000, 1, -1);
        script_121DE(CHARA_ENKI, 1, 31010, 0x41400000, 0x3F800000, 1, -1);
        local_FF = local_FF + 1;
      }
      Script_motmToggleFlags(arg0, SCEN_Disable, 8);
      Script_motmToggleFlags(CHARA_ENKI, SCEN_Disable, 8);
      task_334 = 15;
      SCNF_WaitFrames_0000(1);
    } else {
      script_122D6(7, 10);
      script_11BC0(21, -1);
      script_120BB(8800, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      script_114BA(task_2F4[1], 0x42340000, -1);
      script_10C05(arg0);
    }
  } else {
    script_122D6(4, 10);
    script_11BC0(21, -1);
    script_120BB(8800, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_114BA(task_2F4[1], 0x42340000, -1);
    script_10C05(arg0);
  }
  script_121DE(arg0, arg1, 787, 0, 0x3F800000, 1, -1);
  script_120BB(8808, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_112CB(arg0, 0x45534142, -111.3f, 99.5f, 225.0f);
  script_112CB(task_2F4[2], arg0, -0.2f, -2.0f, 185.0f);
  script_11BC0(38, -1);
  script_122D6(9, -1);
  script_10EA8(arg1, 1, 2, 0);
  if (task_328 == 0) {
    script_122D6(11, 10);
    script_120BB(8810, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
    script_11BC0(39, 50);
    SCNF_WaitFrames_0000(40);
    script_121DE(arg0, arg1, 787, 0, 0x3F800000, 1, -1);
    script_112CB(task_2F4[2], arg0, -0.2f, -2.0f, 185.0f);
    script_120BB(8811, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_122D6(12, -1);
    script_10EA8(arg1, 1, 2, 0);
    if (task_328 == 0) {
      script_122D6(14, 10);
      Script_SetCharaFlags(CHARA_ENKI, 0);
      Script_SetCharaFlags(CHARA_NGSM, 0);
      script_1219F(CHARA_AKIR, 4, 0, 1, -1);
      script_120BB(8813, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      script_120BB(8812, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 20);
      script_11367(arg0, 0x4D53474E, 0, -0x41000000, 0x43480000, 20);
      script_11367(task_2F4[1], arg0, -0x41666666, 0x3FC00000, 0x41200000, 20);
      script_11BC0(30, 30);
      script_12052(65705, 30);
      script_121DE(CHARA_ENKI, arg1, 31015, 0, 0x3F800000, 0, -1);
      script_121DE(arg0, arg1, 804, 0, 0x3F333333, 2, 30);
      script_121DE(arg0, arg1, 804, 0x41880000, 0x3FC00000, 2, 47);
      script_121DE(CHARA_NGSM, 2, 31002, 0x41C80000, 0x3F800000, 1, 20);
      script_121DE(CHARA_ENKI, 1, 31021, 0x41F00000, 0x3F800000, 0, 25);
      script_14137(arg0, 30, 10, 0, -0x428A3D71);
      script_1229B(0x3D4CCCCD, 0x3E4CCCCD, 0x3F000000, 55);
      script_12052(131241, 50);
      script_12052(852137, 60);
      SCNF_WaitFrames_0000(70);
      Script_SetCharaFlags(CHARA_ENKI, 4);
      Script_SetCharaFlags(CHARA_NGSM, 4);
      script_1219F(CHARA_AKIR, 0, -1, 5, 90);
      if (task_3E4 != 2) {
        script_120BB(8828, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
        script_11367(task_2F4[1], 0x45534142, -0x3D1F999A, 0x42C3051F, 0x43480000, -1);
        script_121DE(CHARA_ENKI, arg1, 31015, 0, 0x3F800000, 3, -1);
        script_120F5(CHARA_ENKI, arg0, 1, 10);
      }
      task_334 = 15;
      SCNF_WaitFrames_0000(1);
    } else {
      script_122D6(13, 10);
      script_120BB(8809, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      script_11BC0(41, 20);
      SCNF_WaitFrames_0000(65);
    }
  } else {
    script_122D6(10, 10);
    script_120BB(8809, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_11BC0(43, 20);
    SCNF_WaitFrames_0000(65);
  }
  script_121DE(arg0, arg1, 1076, 0, 0x3F800000, 0, -1);
  script_121DE(arg0, arg1, 787, 0, 0x3F800000, 0, 10);
  script_11400(arg0, -1, 0x43340000, 10);
  SCNF_WaitFrames_0000(25);
  script_122D6(15, -1);
  script_11BC0(28, -1);
  script_112CB(arg0, 0x45534142, -112.8f, 99.6f, 110.0f);
  script_112CB(task_2F4[1], arg0, 0.4f, -2.33f, 165.0f);
  script_120BB(8801, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_10EA8(arg1, 2, 1, 0);
  if (task_328 == 0) {
    script_122D6(17, 10);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
    script_120BB(8804, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 10);
    script_12052(65705, 30);
    script_121DE(arg0, arg1, 30977, 0, 0x3F800000, 1, 25);
    script_121DE(CHARA_ENKI, 1, 31002, 0x41C80000, 0x3F800000, 1, 20);
    script_11BC0(27, 70);
    SCNF_WaitFrames_0000(70);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
    script_120BB(8801, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_112CB(task_2F4[1], arg0, 0.3f, -2.15f, 165.0f);
    script_122D6(18, -1);
    script_10EA8(arg1, 3, 1, 0);
    if (task_328 == 0) {
      int8_t local_FF;
      script_122D6(20, 10);
      script_120BB(8805, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 10);
      SCNF_WaitFrames_0000(16);
      script_114BA(task_2F4[1], 0x420C0000, 14);
      Script_motmToggleFlags(arg0, SCEN_Enable, 8);
      Script_motmToggleFlags(CHARA_ENKI, SCEN_Enable, 8);
      script_11BC0(30, -1);
      local_FF = 0;
      while (local_FF < 2) {
        SCNF_WaitFrames_0000(14);
        script_12052(169, 11);
        script_121DE(arg0, arg1, 30986, 0x41400000, 0x3F800000, 1, -1);
        script_121DE(CHARA_ENKI, 1, 31010, 0x41400000, 0x3F800000, 1, -1);
        local_FF = local_FF + 1;
      }
      Script_motmToggleFlags(arg0, SCEN_Disable, 8);
      Script_motmToggleFlags(CHARA_ENKI, SCEN_Disable, 8);
      task_334 = 15;
      SCNF_WaitFrames_0000(1);
    } else {
      script_122D6(19, 10);
      script_120BB(8806, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 30);
      script_1229B(0x3E4CCCCD, 0x3DCCCCCD, 0x3F333333, 40);
      script_11BC0(23, 15);
      SCNF_WaitFrames_0000(60);
    }
  } else {
    script_122D6(16, 10);
    script_120BB(8807, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 30);
    script_1229B(0x3E4CCCCD, 0x3DCCCCCD, 0x3F333333, 57);
    script_11BC0(22, 55);
    script_11BC0(23, 100);
    SCNF_WaitFrames_0000(120);
  }
  task_334 = -1;
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_132DE(int32_t arg0, int32_t arg1) {
  int32_t unused_E0[8];
  script_122D6(42, -1);
  script_1426E(0, 1, 1);
  script_112CB(task_2F4[1], 0x45534142, -114.7f, 97.53f, 190.0f);
  script_112CB(task_2F4[2], 0x45534142, -115.5f, 98.0f, 195.0f);
  script_121DE(CHARA_ENKI, 1, 30990, 0, 0x3F800000, 0, -1);
  script_121DE(CHARA_NGSM, 2, 30990, 0, 0x3F800000, 0, -1);
  Script_scenCreateTask(script_140E1, 2, CHARA_ENKI, 40);
  Script_scenCreateTask(script_140E1, 2, CHARA_NGSM, 60);
  SCNF_WaitFrames_0000(100);
  script_1426E(1, 1, 1);
  Script_executeAicaCommand(278440, 0, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, SCEN_Disable, 4);
  SCNF_WaitFrames_0000(1);
  script_112CB(*task_2F4, 0x45534142, -111.8f, 100.7f, 60.0f);
  script_120BB(8814, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_120BB(8816, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 60);
  script_120BB(8815, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 360);
  script_120BB(8817, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 510);
  script_114BA(*task_2F4, 0x432A0000, 100);
  script_114BA(task_2F4[1], 0x43160000, 60);
  script_114BA(task_2F4[2], 0x439D8000, 80);
  script_114BA(task_2F4[2], 0x43340000, 140);
  script_120F5(arg0, CHARA_ENKI, 2, 50);
  script_12166(arg0, 2, 100);
  script_121DE(CHARA_AKIR, arg1, 30984, 0, 0x3F800000, 1, -1);
  script_121DE(CHARA_AKIR, arg1, 89, 0, 0x3F800000, 0, 100);
  script_121DE(CHARA_AKIR, arg1, 904, 0, 0x3F800000, 0, 560);
  script_121DE(CHARA_ENKI, 1, 30998, 0, 0x3F800000, 1, -1);
  script_121DE(CHARA_ENKI, 1, 31015, 0, 0x3F800000, 3, 60);
  script_121DE(CHARA_NGSM, 2, 30990, 0, 0x3F800000, 0, -1);
  script_121DE(CHARA_NGSM, 2, 36, 0, 0x3FC00000, 0, 80);
  script_121DE(CHARA_NGSM, 2, 31015, 0, 0x3F800000, 0, 140);
  script_12052(0x1000A9, 10);
  script_11BC0(14, 30);
  script_11BC0(15, 160);
  script_11BC0(33, 250);
  script_11BC0(16, 370);
  script_11BC0(5, 520);
  script_11BC0(17, 610);
  SCNF_WaitFrames_0000(510);
  script_1426E(0, 1, 1);
  SCNF_WaitFrames_0000(180);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
  script_121DE(CHARA_AKIR, arg1, 787, 0, 0x3F333333, 0, -1);
  SCNF_WaitFrames_0000(40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  return;
}

__attribute__((scn_string_bindings(0, data_1A597, 1, data_1A59E, 2, data_1A5A5, 3, data_1A5AC)))
static void script_13649(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  script_122D6(43, -1);
  Script_InitializeOsag(arg0);
  Script_InitializeOsag(CHARA_ENKI);
  Script_InitializeOsag(CHARA_NGSM);
  script_120BB(8824, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_12088(CHARA_ENKI, -1);
  script_12088(CHARA_NGSM, -1);
  script_112CB(arg0, 0x45534142, -113.8f, 101.04f, 330.0f);
  script_112CB(task_2F4[1], 0x45534142, -113.0f, 100.18f, 230.0f);
  script_112CB(task_2F4[2], 0x45534142, -111.4f, 101.08f, 250.0f);
  script_121DE(task_2F4[1], 1, 31013, 0, 0x3F800000, 3, -1);
  script_121DE(task_2F4[2], 2, 30993, 0, 0x3F000000, 3, -1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  (&local_F4)->y = 0.0f;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_2F4[1], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_2F4[2], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_120F5(arg0, CHARA_NGSM, 2, 100);
  script_12166(arg0, 2, 170);
  script_11BC0(31, -1);
  SCNF_WaitFrames_0000(170);
  script_1426E(1, 0, 1);
  Script_ReturnTimeDate(&local_E8);
  script_120BB(8818, SHEN_NAME('DUM2'), SHEN_NAME('DUM2'), -1);
  script_120BB(8819, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 125);
  script_120BB(8820, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 305);
  script_120BB(8821, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 595);
  script_112CB(arg0, 0x45534142, -112.28f, 100.0f, 250.0f);
  script_112CB(task_2F4[2], arg0, 0.75f, -1.2f, 0.0f);
  script_121DE(task_2F4[2], 2, 30993, 0x41F00000, 0x3F800000, 3, -1);
  script_121DE(arg0, arg1, 30989, 0x41F00000, 0x3F800000, 3, -1);
  script_121DE(task_2F4[2], 2, 30992, 0, 0x3F800000, 0, 124);
  script_121DE(arg0, arg1, 30988, 0, 0x3F800000, 0, 124);
  script_121DE(task_2F4[2], 2, 30991, 0, 0x3F800000, 0, 565);
  script_121DE(arg0, arg1, 30987, 0, 0x3F800000, 0, 565);
  script_11BC0(52, 580);
  script_11BC0(6, 125);
  script_11BC0(7, 185);
  script_11BC0(49, 270);
  script_11BC0(10, 455);
  script_11BC0(48, 70);
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED) == 1) | ((&local_E8)->hours >= 16)) {
    script_11BC0(50, 500);
    script_11BC0(11, 645);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SEEK_THE_THREE_BLADES, 1);
    Script_sprintf("save!\012");
    Script_CaptureTrialSaveState();
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_RETURN_DONE, 1);
    Script_sprintf("save!\012");
    Script_CaptureTrialSaveState();
  } else {
    script_11BC0(51, 500);
    script_11BC0(12, 645);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED, 1);
    Script_sprintf("save!\012");
    Script_CaptureTrialSaveState();
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_ARAR_ETUKO_REWARD_STAGE_ACTIVE, 1);
    Script_sprintf("save!\012");
    Script_CaptureTrialSaveState();
  }
  script_120F5(CHARA_NGSM, CHARA_AKIR, 2, 125);
  script_120F5(CHARA_NGSM, CHARA_AKIR, 2, 300);
  script_12166(CHARA_NGSM, 2, 430);
  script_120F5(CHARA_NGSM, CHARA_AKIR, 2, 480);
  script_12166(CHARA_NGSM, 2, 575);
  SCNF_WaitFrames_0000(690);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  Script_motmSetBlendFrames(task_2F4[1], MOTM_BLEND_FRAMES_5);
  Script_motmSetBlendFrames(task_2F4[2], MOTM_BLEND_FRAMES_5);
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED) == 1) | ((&local_E8)->hours >= 16)) {
    script_120BB(8822, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 8);
    script_121DE(arg0, arg1, 1076, 0, 0x3F800000, 0, -1);
    script_121DE(arg0, arg1, 655, 0, 0x3F666666, 0, 14);
    script_11400(arg0, 0x45534142, 0x42F00000, 14);
    script_141F5(arg0, 45, 30, -0x40800000);
    SCNF_WaitFrames_0000(90);
    script_11BC0(32, -1);
    script_120BB(8823, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
    script_112CB(task_2F4[1], 0x45534142, -112.8f, 100.2f, 230.0f);
    script_112CB(task_2F4[2], 0x45534142, -112.0f, 100.5f, 220.0f);
    script_121DE(task_2F4[1], 1, 31017, 0x41700000, 0x3F800000, 3, -1);
    script_121DE(task_2F4[2], 2, 30994, 0x41700000, 0x3F800000, 3, -1);
    SCNF_WaitFrames_0000(150);
  } else {
    script_120BB(8829, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
    script_121DE(arg0, arg1, 655, 0, 0x3F800000, 0, -1);
    SCNF_WaitFrames_0000(90);
    script_141F5(arg0, -1, 50, -0x40800000);
    script_1426E(1, 1, 1);
    script_11BC0(32, -1);
    script_120BB(8823, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
    script_112CB(task_2F4[1], 0x45534142, -112.8f, 100.2f, 230.0f);
    script_112CB(task_2F4[2], 0x45534142, -112.0f, 100.5f, 220.0f);
    script_121DE(task_2F4[1], 1, 31017, 0x41700000, 0x3F800000, 3, -1);
    script_121DE(task_2F4[2], 2, 30994, 0x41700000, 0x3F800000, 3, -1);
    SCNF_WaitFrames_0000(150);
    script_112CB(arg0, -1, -57.7f, 49.63f, 303.31f);
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_1426E(1, 1, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_13D65(int32_t arg0, int32_t arg1) {
  int32_t unused_F4[3];
  script_122D6(44, -1);
  script_1426E(0, 0, 0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  script_120BB(8825, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_12088(CHARA_ENKI, -1);
  script_12088(CHARA_NGSM, -1);
  Script_InitializeOsag(arg0);
  Script_InitializeOsag(CHARA_ENKI);
  Script_InitializeOsag(CHARA_NGSM);
  script_11367(arg0, 0x45534142, -0x3D200000, 0x42C66666, 0x437A0000, -1);
  script_11367(task_2F4[2], 0x45534142, -0x3D210000, 0x42CA999A, 0x43B18000, -1);
  script_11367(task_2F4[1], 0x45534142, -0x3D1E3333, 0x42C86666, 0x41200000, -1);
  script_121DE(arg0, arg1, 30983, 0, 0x3F000000, 3, -1);
  script_121DE(arg0, arg1, 30983, 0x43960000, 0x3F800000, 0, 600);
  script_121DE(task_2F4[1], 1, 31014, 0x41F00000, 0x3F4CCCCD, 3, -1);
  script_121DE(task_2F4[1], 1, 30990, 0, 0x3F800000, 0, 480);
  script_121DE(task_2F4[2], 2, 30990, 0, 0x3F800000, 3, -1);
  script_121DE(task_2F4[2], 2, 31015, 0, 0x3F800000, 0, 30);
  script_121DE(task_2F4[2], 2, 30990, 0, 0x3F800000, 0, 495);
  script_120F5(CHARA_NGSM, CHARA_ENKI, 2, 5);
  script_120F5(CHARA_ENKI, CHARA_AKIR, 2, 5);
  script_120F5(CHARA_NGSM, CHARA_AKIR, 2, 95);
  script_12166(CHARA_NGSM, 2, 495);
  script_12166(CHARA_ENKI, 2, 480);
  script_11BC0(36, -1);
  script_11BC0(19, 90);
  script_11BC0(20, 210);
  script_11BC0(37, 320);
  script_12052(262313, 60);
  script_12052(262313, 80);
  script_12052(262313, 110);
  script_12052(262313, 130);
  script_141F5(task_2F4[1], 480, 16, -0x3EE80000);
  script_141F5(task_2F4[2], 495, 16, -0x3F000000);
  script_1219F(CHARA_AKIR, 11, 5, 7, -1);
  script_1219F(CHARA_AKIR, 11, 0, 7, 300);
  script_1219F(CHARA_AKIR, 11, 2, 7, 360);
  script_1219F(CHARA_AKIR, 11, 3, 7, 420);
  script_1219F(CHARA_AKIR, 4, 1, 7, 480);
  script_1219F(CHARA_AKIR, 4, 2, 7, 560);
  script_1219F(CHARA_AKIR, 4, 5, 7, 640);
  script_1219F(CHARA_AKIR, 4, 0, 7, 700);
  script_1219F(CHARA_AKIR, 0, -1, 7, 760);
  SCNF_WaitFrames_0000(450);
  script_1426E(1, 1, 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  return;
}

static int32_t script_140E1(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  Vector3i local_F0;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FF = 0;
  while (local_FF < 60) {
    (&local_F0)->y = 182;
    Script_EVT_SetRotation(arg0, -0x70000000, &local_F0, 0);
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_14137(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1414F, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_1414F(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4) {
  int8_t local_FF;
  Vector3 local_F0;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FF = 0;
  while (local_FF < arg2) {
    local_F0.x = arg3;
    (&local_F0)->z = arg4;
    Script_EVT_SetPosition(arg0, -0x58000000, &local_F0);
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_141A8(int32_t arg0, int32_t arg1) {
  int16_t local_FE;
  Vector3 local_F0;
  local_FE = 0;
  while (local_FE < arg1) {
    Script_EVT_GetPosition(arg0, 18, &local_F0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetMotnFlags(CHARA_ENKI, 17, &local_F0, 0x78000000);
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

static void script_141F5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_1420B, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_1420B(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
  int8_t local_FF;
  uint8_t unused_F0[15];
  Vector3i local_E4;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FF = 0;
  while (local_FF < arg2) {
    (&local_E4)->y = (int32_t)(arg3 / 360.0f * (float)0x10000);
    Script_EVT_SetRotation(arg0, -0x70000000, &local_E4, 0);
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_1426E(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenToggleActiveFor(CHARA_AKIR, arg0);
  Script_scenToggleActiveFor(CHARA_ENKI, arg1);
  Script_scenToggleActiveFor(CHARA_NGSM, arg2);
  return;
}

__attribute__((scn_string_bindings(0, data_1A5B3, 1, data_1A5BA)))
static void script_14295(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3 local_DC;
  int32_t unused_D8;
  GameDateTime local_D0;
  Script_ReturnTimeDate(&local_D0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 996)) {
  } else if (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE) != 7) {
  } else if (((&local_D0)->hours >= 17) & ((&local_D0)->minutes >= 30)) {
  } else if ((&local_D0)->hours >= 18) {
  } else {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, 0);
    local_F4.x = -64.5f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 54.1f;
    if (Script_CheckTargetWithinAngleRange(&local_F4, 4.0f, 0x8000, 0x8000, &local_DC)) {
      if (task_294 == -1) {
        if ((task_298 == -1) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED) == 1)) {
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SEEK_THE_THREE_BLADES)) {
            int8_t local_CF;
            local_CF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 8.0f));
            if (local_CF >= 5) {
              (void)((task_294 = 0) & 0x10);
              goto L014508;
            }
          } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED) | ((&local_D0)->hours >= 16)) {
            int8_t local_CF;
            local_CF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
            if (local_CF == 0) {
              (void)((task_294 = 0) & 0x10);
              goto L014508;
            }
          } else {
            int8_t local_CF;
            local_CF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
            if (local_CF == 0) {
              (void)((task_294 = 0) & 0x10);
              goto L014508;
            }
          }
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 996, 1);
          Script_sprintf("save!\012");
          Script_CaptureTrialSaveState();
          script_14509();
          Script_MoveObjectToObject(SHEN_NAME('DUM0'), -1);
          Script_MoveObjectToObject(SHEN_NAME('DUM1'), -1);
          Script_MoveObjectToObject(SHEN_NAME('DUM2'), -1);
          Script_MoveObjectToObject(CHARA_AKIR, -1);
          Script_MoveObjectToObject(CHARA_ENKI, -1);
          Script_MoveObjectToObject(CHARA_NGSM, -1);
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 997, 1);
          Script_sprintf("save!\012");
          Script_CaptureTrialSaveState();
        }
      }
    }
    local_F4.x = -58.1f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 49.4f;
    if (Script_CheckTargetWithinAngleRange(&local_F4, 4.0f, 0x8000, 0x8000, &local_DC)) {
      (void)((task_294 = -1) & 4);
    }
    local_F4.x = -66.7f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 60.3f;
    if (Script_CheckTargetWithinAngleRange(&local_F4, 4.5f, 0x8000, 0x8000, &local_DC)) {
      (void)((task_294 = 0) & 8);
      (void)((task_298 = -1) & 8);
    }
  }
  L014508:;
  return;
}

static void script_14509(void) {
  script_14530();
  task_288 = 0;
  for (;;) {
    if (task_290 == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_14530(void) {
  Script_WeatherUtils_ForFIXC(14, 0);
  Script_ReturnTimeDate(&task_29C);
  task_3E0 = 0;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_SetCameraFlags(0);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_sicnUtils(1, 0, 0);
  SCNF_WaitFrames_0000(1);
  task_290 = 0;
  Script_MoveObjectToObject(SHEN_NAME('DUM0'), 0x52494B41);
  Script_EVT_SetPosition(SHEN_NAME('BASE'), POS_XYZ, &data_1A5C4);
  Script_EVT_SetRotation(SHEN_NAME('BASE'), 0x38000000, &data_1A5D0, 0);
  Script_EVT_SetPosition(SHEN_NAME('DUM0'), POS_XYZ, &data_1A5DC);
  Script_EVT_SetRotation(SHEN_NAME('DUM0'), 0x38000000, &data_1A5E8, 0);
  Script_MoveObjectToObject(CHARA_AKIR, 0x45534142);
  SCNF_WaitFrames_0000(1);
  script_120BB(8826, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_112CB(CHARA_AKIR, 0x45534142, -106.0f, 105.1f, 48.0f);
  script_121DE(CHARA_AKIR, 0, 655, 0, 0x3F800000, 3, -1);
  Script_SetQteOverlayTimeoutTuning(QTE_TimeoutBegin);
  SCNF_WaitFrames_0000(1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  task_2AC = script_1845F(0x494B4E45);
  *(&task_2AC + 1) = script_1845F(0x4D53474E);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_scenCreateTask(script_149D6, 4, CHARA_NGSM, 2, 2, 0);
  Script_scenCreateTask(script_149D6, 4, CHARA_ENKI, 1, 2, 0);
  Script_scenCreateTask(script_1486C, 2, CHARA_AKIR, 0);
  task_28C = Script_scenCreateTask(script_1249F, 2, CHARA_AKIR, 0);
  Script_executeAicaCommand(2720, 5, 80);
  return;
}

static void script_14700(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  task_290 = 1;
  Script_SetCameraFlags(1);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_MoveObjectToObject(CHARA_ENKI, -1);
  Script_MoveObjectToObject(CHARA_NGSM, -1);
  Script_MoveObjectToObject(SHEN_NAME('DUM0'), -1);
  Script_MoveObjectToObject(SHEN_NAME('DUM1'), -1);
  Script_MoveObjectToObject(SHEN_NAME('DUM2'), -1);
  SCNF_WaitFrames_0000(1);
  Script_WeatherUtils_ForFIXC(14, 1);
  Script_charaSetRefboardFlagTo(CHARA_ENKI, 0);
  Script_charaSetRefboardFlagTo(CHARA_NGSM, 0);
  Script_SetCPAMFlags(CHARA_ENKI, 1);
  Script_SetCPAMFlags(CHARA_NGSM, 1);
  SCNF_WaitFrames_0000(1);
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  script_184C6(0x494B4E45, *&task_2AC);
  script_184C6(0x4D53474E, *(&task_2AC + 1));
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_executeAicaCommand(2720, 5, 20);
  Script_ryoControl(RYO_RestoreControl);
  Script_ReturnTimeDate(&local_F4);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(60);
  if ((&local_F4)->hours < 19) {
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_executeAicaCommand(2464, 5, 20);
    Script_executeAicaCommand(147368, 0, 0);
  }
  Script_SetActionSelectorHUDElementVisibility(0);
  Script_srchControl((void *)task_28C);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_1486C(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_11066, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_10C21, 6, arg0, arg1, 2, 2, &data_1A238, script_1176F);
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(arg0, 4);
  Script_MoveObjectToObject(SHEN_NAME('DUM1'), 0x494B4E45);
  Script_MoveObjectToObject(SHEN_NAME('DUM2'), 0x4D53474E);
  Script_EVT_SetPosition(SHEN_NAME('DUM0'), POS_XYZ, &data_1A5F4);
  Script_EVT_SetRotation(SHEN_NAME('DUM0'), 0x38000000, &data_1A600, 0);
  Script_EVT_SetPosition(SHEN_NAME('DUM1'), POS_XYZ, &data_1A5F4);
  Script_EVT_SetRotation(SHEN_NAME('DUM1'), 0x38000000, &data_1A600, 0);
  Script_EVT_SetPosition(SHEN_NAME('DUM2'), POS_XYZ, &data_1A5F4);
  Script_EVT_SetRotation(SHEN_NAME('DUM2'), 0x38000000, &data_1A600, 0);
  Script_MoveObjectToObject(CHARA_ENKI, 0x45534142);
  Script_MoveObjectToObject(CHARA_NGSM, 0x45534142);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_290 == 1) {
      Script_SetCPAMFlags(arg0, 1);
      Script_ccowUtils(arg0, CCOW_EnableAll);
      Script_srchControl((void *)task_318[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
  }
  end:;
}

static int32_t script_149D6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_11066, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_10C21, 6, arg0, arg1, 0, 2, &data_1A238, script_1176F);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_290 == 1) {
      Script_srchControl((void *)task_318[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
  }
  end:;
}

static void script_14A99(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_0455();
  script_14C38();
  local_FC = 0;
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 181:
        Script_FACE(CHARA_AKIR, 1, 0, 16);
        break;
      case 190:
        Script_SetCameraFlags(1);
        break;
      case 281:
        Script_FACE(CHARA_AKIR, 2, 0, 16);
        break;
      case 351:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 641:
        Script_FACE(CHARA_AKIR, 2, 0, 16);
        break;
      case 781:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 1390:
        Script_handSetFloatsFor(CHARA_TOM1, 1, data_18C8C, 8);
        break;
      case 1434:
        Script_handSetFloatsFor(CHARA_TOM1, 1, data_18BA8, 16);
        break;
      case 1661:
        Script_FACE(CHARA_AKIR, 1, 0, 16);
        break;
      case 1811:
        Script_FACE(CHARA_AKIR, 2, 0, 16);
        break;
    }
    if (local_FC > 170) {
      if (task_203 & 4) {
        script_14CEC(CHARA_AKIR);
        (void)((task_203 = 0) & 0x10);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_14C38(void) {
  int32_t unused_F4[3];
  script_0466(CHARA_AKIR);
  script_0466(CHARA_TOM1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_TOM1, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_facemotUtils(CHARA_TOM1, 1, 0);
  script_14CEC(CHARA_TOM1);
  Script_SetMaxInterpSpeed(0.4f);
  Script_motmToggleFlags(CHARA_TOM1, SCEN_Disable, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_SetCharaFlags(CHARA_TOM1, 4);
  return;
}

static void script_14CB8(void) {
  Script_SetCharaFlags(CHARA_TOM1, 0);
  script_14D51(CHARA_AKIR);
  script_0500(CHARA_TOM1);
  Script_facemotUtils(CHARA_TOM1, 0, 0);
  return;
}

static void script_14CEC(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_18BA8, 8);
  Script_handSetFloatsFor(arg0, 0, data_18BA8, 8);
  return;
}

static void script_14D27(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_14D51(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  return;
}

static void script_14D95(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_EC[4];
  Vector3 local_E0;
  int32_t unused_BC[9];
  Vector3i local_B0;
  int32_t local_A4[3];
  int32_t local_A0;
  script_0455();
  script_15366();
  if (arg0) {
    Script_scenCreateTask(script_0550, 0);
  }
  Script_shenSeqUtils(1);
  local_FC = 0;
  local_B0.x = 0;
  (&local_B0)->y = 0;
  (&local_B0)->z = 0;
  local_A4[0] = 0;
  local_A4[1] = 0;
  local_A4[2] = 0;
  Script_shenSeqUtils(-8, Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 989));
  while (Script_shenSeqUtils(-1)) {
    local_A0 = local_FC;
    switch (local_A0) {
      case 1:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        Script_FaceEx(CHARA_EIIU, FACE_ExpressionEnabled, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_EIIU, 1, 1, 0);
        Script_ResetHandMotion(CHARA_EIIU, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1A724, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_1A724, 1);
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_1A9D0);
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_1A9D0);
        Script_SetHandRootRotation(CHARA_EIIU, 1, 0x15, &data_1A9D0);
        Script_SetHandRootRotation(CHARA_EIIU, 0, 0x15, &data_1A9D0);
        Script_FACE(CHARA_AKIR, 0, 0, 1);
        Script_FACE(CHARA_EIIU, 0, 0, 1);
        break;
      case 10:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1A640, 30);
        break;
      case 84:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1A724, 40);
        break;
      case 100:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1A724, 40);
        break;
      case 53:
        Script_FACE(CHARA_AKIR, 7, 1, 10);
        break;
      case 88:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        break;
      case 105:
        Script_FACE(CHARA_EIIU, 7, 5, 5);
        break;
      case 128:
        Script_FACE(CHARA_EIIU, 4, 2, 20);
        break;
      case 149:
        Script_FACE(CHARA_AKIR, 1, 0, 1);
        break;
      case 164:
        Script_FACE(CHARA_AKIR, 3, 0, 7);
        break;
      case 180:
        Script_FACE(CHARA_EIIU, 5, 2, 15);
        break;
      case 202:
        Script_FACE(CHARA_AKIR, 4, 0, 6);
        break;
      case 201:
        Script_FACE(CHARA_EIIU, 4, 4, 1);
        break;
      case 222:
        Script_FACE(CHARA_AKIR, 7, 0, 4);
        break;
      case 248:
        Script_FACE(CHARA_AKIR, 2, 0, 14);
        break;
      case 206:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1A808, 13);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_1A808, 13);
        break;
      case 288:
        break;
      case 294:
        break;
      case 255:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1A724, 14);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_1A724, 14);
        break;
      case 31:
        Script_handSetFloatsFor(CHARA_EIIU, 1, data_1A8EC, 140);
        Script_handSetFloatsFor(CHARA_EIIU, 0, data_1A8EC, 140);
        break;
      case 62:
        break;
      case 182:
        Script_handSetFloatsFor(CHARA_EIIU, 1, data_1A640, 20);
        Script_handSetFloatsFor(CHARA_EIIU, 0, data_1A640, 20);
        break;
      case 210:
        break;
      case 228:
        Script_handSetFloatsFor(CHARA_EIIU, 1, data_1A724, 14);
        Script_handSetFloatsFor(CHARA_EIIU, 0, data_1A724, 14);
        break;
      case 155:
        break;
      case 200:
        break;
      case 106:
        local_E0.x = -71.314003f;
        (&local_E0)->y = 1.7043f;
        (&local_E0)->z = 79.587196f;
        Script_FaceLookAtTargetUtils(CHARA_EIIU, FACE_LookAtPosition, &local_E0, 15);
        break;
      case 127:
        Script_FaceLookAtTargetUtils(CHARA_EIIU, FACE_LookReset);
        break;
      case 286:
        Script_shenSeqUtils(-6);
        goto L015318;
        break;
    }
    local_FC = local_FC + 1;
    if (-(local_FC > 0) & -(local_FC < 50) & -((&local_B0)->x >= -7645)) {
      Script_handSetFor(CHARA_AKIR, 1, &local_B0);
      local_B0.x = (&local_B0)->x - 182;
      (&local_B0)->y = (&local_B0)->y + 25;
      (&local_B0)->z = (&local_B0)->z + 69;
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_B0);
    }
    if ((local_FC > 84) & ((&local_B0)->x < 0)) {
      Script_handSetFor(CHARA_AKIR, 1, &local_B0);
      local_B0.x = (&local_B0)->x + 182;
      (&local_B0)->y = (&local_B0)->y - 25;
      (&local_B0)->z = (&local_B0)->z - 69;
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_B0);
    }
    SCNF_WaitFrames_0000(1);
  }
  L015318:;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 989, 1);
  Script_sprintf(data_1A9DC);
  Script_CaptureTrialSaveState();
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(80, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_153A6();
  return;
}

static void script_15366(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  script_1543C(CHARA_AKIR);
  script_1543C(CHARA_EIIU);
  local_F0 = Script_GetCurrentEntryPoint();
  if (local_F0 == 0) {
    Script_mapmSetRenderFlag(2, 0);
  } else {
    Script_mapmSetRenderFlag(2, 0);
  }
  return;
}

static void script_153A6(void) {
  int32_t unused_F4[3];
  script_14D51(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_15401(CHARA_EIIU);
  Script_ResetHandMotion(CHARA_EIIU, 0, 1, 0);
  Script_ResetHandMotion(CHARA_EIIU, 0, 0, 0);
  return;
}

static void script_15401(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_ccowSetHeightMode(arg0, CCOW_FollowGround);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_CharHandlers(arg0, -1, 0, 0);
  return;
}

static void script_1543C(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L015468;
    }
  }
  L015468:;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 5, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_154CC(void) {
  task_3EC = 0;
  (void)((task_3F0 = -1) & 4);
  return;
}

static int32_t script_154E0(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_1AA04, 21);
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 20, 0);
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    local_F0 = Script_WeatherUtils_0(WeatherDataUtil_GetWeatherClassForDateTime, (&local_F4)->year, (&local_F4)->month, (&local_F4)->day, (&local_F4)->hours, (&local_F4)->minutes);
    if (task_3EC == 0) {
      if ((local_F0 == 2) | (local_F0 == 3)) {
        if (task_3F0 == 0) {
          int8_t local_EF;
          local_EF = 0;
          while (local_EF < 20) {
            Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_EF, 1);
            local_EF = local_EF + 1;
          }
          (void)((task_3F0 = -1) & 2);
        }
      } else {
        if (task_3F0 == -1) {
          int8_t local_EF;
          local_EF = 0;
          while (local_EF < 20) {
            Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_EF, 0);
            local_EF = local_EF + 1;
          }
          (void)((task_3F0 = 0) & 2);
        }
      }
    } else {
      if (task_3F0 == -1) {
        int8_t local_EF;
        local_EF = 0;
        while (local_EF < 20) {
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_EF, 0);
          local_EF = local_EF + 1;
        }
        (void)((task_3F0 = 0) & 2);
      }
      Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 20, 1);
      while (task_3EC == 1) {
        SCNF_WaitFrames_0000(1);
      }
      Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 20, 0);
    }
    SCNF_WaitFrames_0000(30);
  }
  end:;
}

static void script_1562C(int32_t arg0) {
  if (arg0 == 1) {
    task_3EC = 1;
  } else {
    task_3EC = 0;
  }
  return;
}

static void script_15649(void) {
  int8_t local_FF;
  if (task_3EC == 1) {
    local_FF = 1;
  } else {
    local_FF = 0;
  }
  (void)local_FF;
  return;
}

__attribute__((scn_string_bindings(0, data_1AEBC)))
static int32_t script_15668(int32_t arg0, const char *arg1) {
  task_40A = 0;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) != 0) {
      break;
    }
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/80/TIKN/", arg1) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  }
  task_40A = 1;
}

static void script_156DD(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static int32_t script_156FE(int32_t arg0, const char *arg1, int32_t arg2) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, arg0, arg1, arg2);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static int32_t script_15743(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (task_408 == 0) {
    Script_executeAicaCommand(arg0, 0, 0);
  }
  return Script_srchControl(0);
}

static void script_15771(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_156FE, 3, arg0, arg1, arg2);
  return;
}

static void script_15785(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_15743, 2, arg0, arg1);
  return;
}

static int32_t script_15797(const char *arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (task_408 == 0) {
    Script_StartOrFreeSEQ(0);
    Script_ShowUIMessageFromID(arg0);
  }
  return Script_srchControl(0);
}

static void script_157C8(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_15797, 2, arg0, arg1);
  return;
}

static int32_t script_157DA(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (task_408 == 0) {
    Script_fixoAttachEx(arg0, arg1, arg2, arg3, arg4);
  }
  return Script_srchControl(0);
}

static void script_1580D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_157DA, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_15827(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_fixoClearAttachment(arg0);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_srchControl(0);
}

static void script_1584F(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_15827, 2, arg0, arg1);
  return;
}

static void script_15861(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1587D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  if (task_408 == 0) {
    if (arg3 & -0x80000000) {
      Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
    } else {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
    }
    Script_PlayCharaMotion(arg0, arg1, arg2, -1.0f, -1.0f, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static void script_158DF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1587D, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_158F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_408 == 0) {
    Script_SetNewHandMotion(arg0, arg1, arg2, 0, 0, 0, 0, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static void script_15935(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_158F7, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_1594B(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_408 == 0) {
    Script_SetHandRootRotation(arg0, arg1, 0x15, arg2);
  }
  return Script_srchControl(0);
}

static void script_1597C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_1594B, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_15992(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  if (task_408 == 0) {
    Script_FACE(arg0, arg1, arg2, arg3);
  }
  return Script_srchControl(0);
}

static void script_159C3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_15992, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_159DB(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3) {
  int32_t local_FC;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_408 == 0) {
    local_FC = Script_CEWPRelated(arg0);
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
    Script_MoveObjectToObject(arg0, local_FC);
  }
  return Script_srchControl(0);
}

static void script_15A28(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_159DB, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_15A3E(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  int32_t local_FC;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_408 == 0) {
    local_FC = Script_CEWPRelated(arg0);
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
    Script_MoveObjectToObject(arg0, local_FC);
  }
  return Script_srchControl(0);
}

static void script_15A8D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_15A3E, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_15AA3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_408 == 0) {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return Script_srchControl(0);
}

static void script_15AD1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_15AA3, 4, arg0, arg1, arg2, arg3);
  return;
}

static void script_15AE7(int32_t arg0) {
  Vector4 local_F0;
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.1f;
  (&local_F0)->z = 0.1f;
  (&local_F0)->w = 0.1f;
  Script_SetCharaRefBoardPos(arg0, &local_F0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  return;
}

static int32_t script_15B26(int32_t arg0) {
  SCNF_WaitFrames_0000(118);
  SCNF_SetCashValue(-100, 1);
  arg0 = arg0 - 100;
  Script_SetStats(STATS_CASH, arg0);
  end:;
}

static int32_t script_15B4C(int32_t arg0) {
  SCNF_SetCashValue(100, 1);
  arg0 = arg0 + 100;
  Script_SetStats(STATS_CASH, arg0);
  end:;
}

static void script_15B6A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_shenGetStats(STATS_CASH);
  if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
    if (local_FC > 99) {
      script_15D4E(arg0, arg1, arg2, arg3);
      Script_scenCreateTask(script_15B26, 1, local_FC);
      script_1620E(arg0, arg1);
      local_F8 = script_1601B(arg0, arg1, arg2, arg3, 1);
      local_F4 = local_F8;
      switch (local_F4) {
        case 12:
          script_162AF(arg0, arg1);
          break;
        case 15:
          local_FC = Script_shenGetStats(STATS_CASH);
          Script_scenCreateTask(script_15B4C, 1, local_FC);
          break;
      }
      script_15F65(arg0, arg1, arg2, arg3);
    }
  }
  return;
}

static void script_15C1C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_15D4E(arg0, arg1, arg2, arg3);
  script_1601B(arg0, arg1, arg2, arg3, 0);
  script_15F65(arg0, arg1, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_1AECC)))
static void script_15C4E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_shenGetStats(STATS_CASH);
  if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
    if (local_FC > 99) {
      script_15E6C(arg0);
      Script_scenCreateTask(script_15B26, 1, local_FC);
      Script_scenCreateTask(script_15668, 2, arg2, "VEND");
      script_1620E(arg0, arg1);
      script_15EE3(arg0, arg1, arg2, arg3);
      local_F8 = script_1601B(arg0, arg1, arg2, arg3, 1);
      local_F4 = local_F8;
      switch (local_F4) {
        case 12:
          script_162AF(arg0, arg1);
          break;
        case 15:
          local_FC = Script_shenGetStats(STATS_CASH);
          Script_scenCreateTask(script_15B4C, 1, local_FC);
          break;
      }
      script_15F65(arg0, arg1, arg2, arg3);
    }
  }
  return;
}

static void script_15D1B(void) {
  task_3F4 = 0x314E4143;
  task_3FC = 3;
  task_3F8 = (char *)data_1AED1;
  task_408 = 0;
  task_409 = 0;
  task_40A = 0;
  return;
}

__attribute__((scn_string_bindings(1, data_1AEDD)))
static void script_15D4E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Vector3 local_E8;
  script_15D1B();
  task_400 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_400) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  }
  Script_LoadVoiceFile(1, "01JUCE");
  Script_scenCreateTask(script_15668, 2, arg2, "VEND");
  while (task_40A != 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_facemotUtils(arg0, 1, 0);
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
  Script_InitMOTMGlobalState(arg0, arg2, arg3);
  return;
}

static void script_15E6C(int32_t arg0) {
  script_15D1B();
  task_400 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_400) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ccowUtils(arg0, CCOW_DisableAll);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  }
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  return;
}

static void script_15EE3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  while (task_40A != 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_facemotUtils(arg3, 1, 0);
  Script_ResetHandMotion(arg3, 1, 1, 0);
  Script_charaDisplay(arg1, CDM_Hide);
  Script_ccowUtils(arg1, CCOW_DisableAll);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_E8, 0x40000000);
  Script_EVT_SetPosition(arg2, 0x78000000, &local_F4);
  Script_EVT_SetRotation(arg2, 0x78000000, &local_E8, 0);
  Script_InitMOTMGlobalState(arg0, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_1AEE2)))
static void script_15F65(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Script_ResetHandMotion(arg3, 0, 1, 0);
  Script_faceUpdateFor(arg3, 1, 0);
  script_156DD("VEND");
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  if (task_400) {
    if (task_408 == 0) {
      Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
      Script_ccowUtils(arg0, CCOW_EnableAll);
      Script_SetGameStateTo(AUTH_None);
      Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
      Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_ryoControl(RYO_RestoreControl);
    }
  }
  task_409 = 13;
  return;
}

static int32_t script_1601B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t unused_E8[6];
  int32_t local_E4;
  int32_t local_E0;
  int8_t local_DF;
  uint8_t unused_DC[3];
  int32_t local_D8;
  local_E4 = 1;
  local_E0 = -1;
  (void)((local_DF = -1) & 1);
  for (;;) {
    if (local_E0 != local_E4) {
      local_E0 = local_E4;
      task_409 = local_E4;
      local_D8 = local_E4;
      if (local_D8 == 1) {
        script_1597C(arg3, 1, (int32_t)data_1AEE8, -1);
        script_15935(arg3, 1, 542, -1);
      } else if ((local_D8 == 2) | (local_D8 == 3) | (local_D8 == 4)) {
        task_409 = 1;
      } else {
        switch (local_D8) {
          case 5:
            script_15785(525481, -1);
            break;
          case 6:
            script_15785(0x1A04A9, 5);
            break;
          case 8:
            task_3F8 = (char *)data_1AEF4;
            task_3FC = 0;
            if (arg4) {
              script_15668(task_3F4, task_3F8);
              script_15AE7(task_3F4);
            }
            break;
          case 9:
            task_3F8 = (char *)data_1AEF9;
            task_3FC = 1;
            if (arg4) {
              script_15668(task_3F4, task_3F8);
              script_15AE7(task_3F4);
            }
            break;
          case 10:
            task_3F8 = (char *)data_1AEFE;
            task_3FC = 2;
            if (arg4) {
              script_15668(task_3F4, task_3F8);
              script_15AE7(task_3F4);
            }
            break;
          case 11:
            task_3F8 = (char *)data_1AF03;
            task_3FC = 3;
            if (arg4) {
              script_15668(task_3F4, task_3F8);
              script_15AE7(task_3F4);
            }
            break;
        }
      }
    }
    local_E4 = Script_UpdateCharaSelectionScene(arg0, arg2, arg3, CHARA_SELECT_IDLE);
    if ((local_E0 == 12) | (local_E0 == 13) | (local_E0 == 14) | (local_E0 == 15)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaDisplay(arg1, CDM_Show);
  Script_charaDisplay(arg2, CDM_Hide);
  Script_ccowUtils(arg1, CCOW_EnableAll);
  return local_E0;
}

static void script_1620E(int32_t arg0, int32_t arg1) {
  script_15A28(arg0, arg1, (int32_t)&data_1AF08, -1);
  script_15A8D(arg0, arg1, (int32_t)data_1AF14, -1);
  script_1597C(arg0, 1, (int32_t)data_1AF20, -1);
  script_1597C(arg0, 0, (int32_t)data_1AF20, -1);
  script_15935(arg0, 1, 541, -1);
  script_158DF(arg0, 45058, 0, -0x80000000, -1);
  script_15785(787625, 125);
  script_15AD1(4013, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_15861(arg0);
  return;
}

static void script_162AF(int32_t arg0, int32_t arg1) {
  script_15668(task_3F4, task_3F8);
  script_1580D(task_3F4, arg0, 18, (int32_t)&data_1AF2C, (int32_t)&data_1AF38, 82);
  script_15A28(arg0, arg1, (int32_t)&data_1AF44, -1);
  script_15A8D(arg0, arg1, (int32_t)data_1AF50, -1);
  script_1597C(arg0, 1, (int32_t)&data_1AF5C, -1);
  script_15935(arg0, 1, 544, -1);
  script_15935(arg0, 0, 545, -1);
  script_158DF(arg0, 45060, 0, -0x80000000, -1);
  script_15785(0x1D04A9, 80);
  script_15785(0x1B04A9, 290);
  script_15AD1(4014, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_15861(arg0);
  script_159C3(arg0, 10, 5, 16, 180);
  script_159C3(arg0, 9, 5, 16, 251);
  script_159C3(arg0, 4, 5, 16, 351);
  script_159C3(arg0, 1, 4, 16, 391);
  script_159C3(arg0, 9, 5, 16, 426);
  script_159C3(arg0, 10, 1, 16, 521);
  script_159C3(arg0, 1, 2, 16, 545);
  script_159C3(arg0, 0, 0, 16, 596);
  script_158DF(arg0, 45059, 0, 0, -1);
  script_15785(984233, 210);
  script_15785(984233, 290);
  script_15785(984233, 470);
  script_15785(0x1C04A9, 690);
  script_157C8((int32_t)"01JUCEA002", 550);
  script_157C8((int32_t)"01JUCEA003", 400);
  script_15AD1(4015, arg1, arg1, -1);
  script_15AD1(4016, arg1, arg1, 575);
  script_1584F(task_3F4, 690);
  SCNF_WaitFrames_0000(10);
  script_15861(arg0);
  script_156DD(task_3F8);
  return;
}

static int32_t script_164CB(int32_t arg0) {
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
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, *(&data_1AE9C + local_F4))) {
      Script_EVT_GetPosition(*(&data_1AE9C + local_F4), -1, (Vector3 *)local_90[local_F4], ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(*(&data_1AE9C + local_F4), (Vector3i *)local_FEC4[local_F4], 0x40000000);
      local_FF24[local_F4][0] = local_90[local_F4][0] + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FEC4[local_F4][1]) * 0.3f;
      local_FF24[local_F4][1] = local_90[local_F4][1];
      local_FF24[local_F4][2] = local_90[local_F4][2] + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FEC4[local_F4][1]) * 0.3f;
    }
    local_F4 = local_F4 + 1;
  }
  for (;;) {
    local_F9 = -1;
    (void)((local_FB = 0) & 2);
    (void)((task_40B = local_FF) & 2);
    (void)((task_40C = local_FE) & 2);
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
          Script_uiUtils(0, 27, 0);
          (void)((local_FD = -1) & 0x80);
        }
      } else {
        if (local_FD == -1) {
          Script_uiUtils(0, 0, 0);
        }
        (void)((local_FD = 0) & 0x80);
      }
      if (local_FE == 0) {
        Script_sicnUtils(0, 0, 1);
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
      script_15C4E(arg0, *(&data_1AE9C + local_F9), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
      (void)((local_FB = 0) & 0x80);
      (void)((local_FC = 0) & 0x80);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_16943(int32_t arg0) {
  script_15D1B();
  task_404 = Script_scenCreateTask(script_164CB, 1, arg0);
  return;
}

static void script_1695E(void) {
  Script_srchControl((void *)task_404);
  task_404 = 0;
  if (task_40B == -1) {
    SCNF_RequestIconNotification(0, 0, 0, 2);
    (void)((task_40B = 0) & 0x10);
  }
  if (task_40C == -1) {
    Script_sicnUtils(1, 0, 0);
    (void)((task_40C = 0) & 0x10);
  }
  return;
}

static int32_t script_169B0(void) {
  return task_409;
}

static void script_169B7(void) {
  Script_ExitCharaSelector();
  task_408 = 1;
  return;
}

static int32_t script_169C4(void) {
  while (task_1FF == 0) {
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &task_410, ENTITY_XFORM_WORLD_SPACE);
    if (!(task_410.z >= (&task_410)->x * 0.56f + 12.3f)) {
      (void)((task_1FD = 0) & 0x80);
      script_16E48(CHARA_IZWA, 12292, 23.0f, 29.0f, 25.1194f, 24.059099f, 0x8000);
    }
    if (!((&task_410)->x <= 10.0f) & !(task_410.z <= 56.0f)) {
      (void)((task_1FD = 0) & 1);
      script_16E48(CHARA_HIDE, 12289, 17.6f, 50.0f, 18.241501f, 59.507801f, 0);
    }
    if (!((&task_410)->x >= -80.0f) & !(task_410.z >= 70.0f)) {
      (void)((task_1FD = 0) & 1);
      script_16E48(CHARA_KEBG, 12290, -70.0f, 62.0f, -82.508301f, 61.310101f, 47331);
    }
    if (!((&task_410)->x >= -76.0f) & !(task_410.z <= 75.0f)) {
      (void)((task_1FD = 0) & 1);
      script_16E48(CHARA_STP1, 12293, -70.0f, 82.0f, -79.1819f, 80.479202f, 42416);
    }
    if (-!(task_410.z <= 80.0f) & -!((&task_410)->x <= -55.4f) & -!(task_410.z >= (&task_410)->x * 1.5f + 167.5f)) {
      (void)((task_1FD = 0) & 1);
      script_16E48(CHARA_MIKI, 12294, -62.0f, 89.0f, -51.521999f, 83.022003f, 23483);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_16B95(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("T1260B001");
      break;
    case 1:
      Script_ShowUIMessageFromID("T1260B002");
      break;
    case 2:
      Script_ShowUIMessageFromID("T1260B003");
      break;
    case 3:
      Script_ShowUIMessageFromID("T1260B004");
      break;
    case 4:
      Script_ShowUIMessageFromID("T1246B001");
      break;
    case 5:
      Script_ShowUIMessageFromID("T1246B002");
      break;
    case 6:
      Script_ShowUIMessageFromID("T1246B003");
      break;
    case 7:
      Script_ShowUIMessageFromID("T1246B004");
      break;
    case 8:
      Script_ShowUIMessageFromID("T1216B001");
      break;
    case 9:
      Script_ShowUIMessageFromID("T1216B002");
      break;
    case 10:
      Script_ShowUIMessageFromID("T1216B003");
      break;
    case 11:
      Script_ShowUIMessageFromID("T1216B004");
      break;
    case 12:
      Script_ShowUIMessageFromID("T1073B001");
      break;
    case 13:
      Script_ShowUIMessageFromID("T1073B002");
      break;
    case 14:
      Script_ShowUIMessageFromID("T1073B003");
      break;
    case 15:
      Script_ShowUIMessageFromID("T1073B004");
      break;
    case 16:
      Script_ShowUIMessageFromID("T1115B001");
      break;
    case 17:
      Script_ShowUIMessageFromID("T1115B002");
      break;
    case 18:
      Script_ShowUIMessageFromID("T1115B003");
      break;
    case 19:
      Script_ShowUIMessageFromID("T1115B004");
      break;
  }
  return;
}

static void script_16D42(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  if (Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3) > 0) {
    Script_ConversationUtils(0, 0, arg0, 0x52494B41, 0.0f, 200.0f);
  } else {
    Script_ConversationUtils(0, 1, arg0, 0x52494B41, 0.0f, 200.0f);
  }
  local_F8 = arg0;
  switch (local_F8) {
    case 0x41575A49:
      local_FF = 0;
      break;
    case 0x45444948:
      local_FF = 4;
      break;
    case 0x4742454B:
      local_FF = 8;
      break;
    case 0x31505453:
      local_FF = 12;
      break;
    case 0x494B494D:
      local_FF = 16;
      break;
  }
  script_16B95(Script_FloatUtils(ScriptMathOp_RandomIntBelow, 2) + local_FF);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_16B95(Script_FloatUtils(ScriptMathOp_RandomIntBelow, 2) + local_FF + 2);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_16E48(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int8_t local_E3;
  Vector3 local_D4;
  if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100)) {
  } else if (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE) != 7) {
  } else {
    Script_sprintf("stopper start\012");
    Script_GetConversationDetails(CONV_DETAILS_MODE_DISABLE_PROMPT);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    SCNF_WaitFrames_0000(1);
    (void)((local_E3 = 0) & 0x40);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    local_F4.x = arg4;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = arg5;
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("chara load start\012");
      local_E4 = script_1845F(arg0);
      Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
      local_E8.x = (&local_E8)->z = 0;
      (&local_E8)->y = arg6;
      Script_EVT_SetRotation(arg0, 0x78000000, &local_E8, 0);
      (void)((local_E3 = -1) & 1);
      Script_PlayCharaMotion(arg0, arg1, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      SCNF_WaitFrames_0000(1);
    }
    Script_SetCPAMFlags(arg0, 0);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 900)) {
      Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      Script_scenUtils(SCENARIO_CMD_ENQUEUE_SCNT, arg0, 0);
      SCNF_WaitFrames_0000(10);
      while (~Script_ryoControl(PCCTL_QueryShouldRun)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    } else {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
        Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D4, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(arg0, 16, &local_D4, 2);
        Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_D4, 30);
      }
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
        Script_EVT_GetPosition(arg0, 4, &local_D4, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(CHARA_AKIR, 16, &local_D4, 2);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_D4, 30);
      }
      script_16D42(arg0);
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
        Script_SetCharaLookAtPosition(arg0, -13, 0, 0);
        Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
      }
    }
    local_F4.x = arg2;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = arg3;
    script_73B5();
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
      Script_SetCharaLookAtPosition(CHARA_AKIR, -10, 0, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    }
    Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    Script_SetCPAMFlags(arg0, 1);
    Script_SetGameStateTo(AUTH_None);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_194D8, 0.0f, 0, 0x200000);
    Script_charaToggleMotEnabled(CHARA_AKIR, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      Script_RotateNodeUtils(CHARA_AKIR, &data_184E0, 0);
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      Script_RotateNodeUtils(CHARA_AKIR, &data_184E0, 0);
      SCNF_WaitFrames_0000(1);
    }
    Script_charaToggleMotEnabled(CHARA_AKIR, -1);
    Script_ryoControl(RYO_RestoreControl);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    if (local_E3 & 8) {
      script_184C6(arg0, local_E4);
    }
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    Script_GetConversationDetails(CONV_DETAILS_MODE_ENABLE_PROMPT);
    (void)((task_1FD = -1) & 0x10);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1B075)))
void script_171D4(void) {
  script_172BB();
  script_172F0();
  script_154CC();
  script_173AE();
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/80/TIKN", (int32_t)"SCRTEST.BIN", 0x30303044);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  Script_scenCreateTask(script_1812E, 0);
  Script_scenCreateTask(script_174B6, 0);
  task_1F0 = Script_scenCreateTask(script_169C4, 0);
  Script_scenCreateTask(script_4E4A, 0);
  Script_scenCreateTask(script_154E0, 1, &data_1AA04);
  script_2A5E(CHARA_AKIR, (DSWKDoorEntry *)data_192D4, (int32_t *)data_1934C, (DoorResourceDescriptor *)&data_1939C, (MapObjectRecord *)data_193DC, 10);
  task_1EC = script_5D98();
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_2C5E();
  script_2D5B(3);
  script_2D5B(2);
  script_2D5B(1);
  script_16943(CHARA_AKIR);
  return;
}

static void script_172BB(void) {
  (void)((task_1FF = 0) & 0x10);
  (void)((task_200 = 0) & 0x10);
  (void)((task_201 = 0) & 0x10);
  (void)((task_202 = 0) & 0x10);
  (void)((task_203 = 0) & 0x10);
  return;
}

__attribute__((scn_string_bindings(1, data_1B09C)))
static void script_172F0(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetMapFlags(0, 1);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_refbSetActive(1);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_shenSetLightingMode(1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 80.0f);
  Script_motnUtils(MOTN_SetBudgetCapacity, 5);
  Script_AuthorUtils(0, 1, "seqdata2.bin", "scene/80/TIKN");
  task_1F4 = Script_motiLoad(80, SHEN_NAME('TIKN'), "M_DJUC.BIN");
  task_1F8 = Script_LoadHandMot(80, SHEN_NAME('TIKN'), "HMOT_CAN.BIN");
  Script_LoadVoiceFile(1, "ALLVOICE");
  Script_uiUtils(1, 14, 0);
  return;
}

static void script_173AE(void) {
  int32_t local_FC;
  if (Script_GetCurrentEntryPoint() == 0) {
    script_1562C(1);
    script_1740B(12, 0);
    Script_SetStats(STATS_CASH, 2000);
  } else {
    local_FC = Script_GetCurrentEntryPoint();
    if (local_FC == 5) {
      script_1562C(1);
      script_1740B(18, 30);
      Script_SetStats(STATS_CASH, 2000);
      goto L01740A;
    }
  }
  L01740A:;
  return;
}

static void script_1740B(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_ReturnTimeDate(&task_1E0);
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f);
  task_1E0.year = 86;
  task_1E0.month = 11;
  task_1E0.day = local_FC + 11;
  task_1E0.dayOfWeek = local_FC + 2;
  task_1E0.hours = arg0;
  task_1E0.minutes = arg1;
  task_1E0.seconds = 0;
  Script_SetNewDateTime(&task_1E0);
  return;
}

static int32_t script_17476(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  while (1) {
    Script_ReturnTimeDate(&task_1E0);
    task_1E0.hours = arg0;
    task_1E0.minutes = arg1;
    task_1E0.seconds = 0;
    Script_SetNewDateTime(&task_1E0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_1B0CB, 1, data_1B0D8, 2, data_1B0E6, 3, data_1B0F3)))
static int32_t script_174B6(void) {
  int32_t unused_FC;
  Script_uiUtils(0, 0, 0);
  script_0030(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  if (Script_GetCurrentEntryPoint() == 0) {
    Script_SetActionSelectorHUDElementVisibility(1);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_AuthorUtils(0, 0, "SEQDATA1.BIN", "SCENE/80/TIKN");
    script_17975();
    Script_SetActionSelectorHUDElementVisibility(0);
    (void)((task_200 = -1) & 1);
    Script_FACE(CHARA_AKIR, 0, 0, 1);
    Script_executeAicaCommand(2464, 5, 30);
    Script_executeAicaCommand(147368, 0, 0);
    Script_shenClearFreeState(1);
  } else if (Script_GetCurrentEntryPoint() == 5) {
    Script_SetActionSelectorHUDElementVisibility(1);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_AuthorUtils(0, 0, "SEQDATA1.BIN", "SCENE/80/TIKN");
    script_17975();
    Script_SetActionSelectorHUDElementVisibility(0);
    (void)((task_200 = -1) & 1);
    Script_FACE(CHARA_AKIR, 0, 0, 1);
    Script_executeAicaCommand(2464, 5, 30);
    Script_executeAicaCommand(147368, 0, 0);
    Script_shenClearFreeState(1);
  } else {
    script_17F8A();
    (void)((task_200 = -1) & 1);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
  }
  Script_sicnUtils(0, 0, 0);
  while (1) {
    script_177F6();
    script_1781B();
    script_176FC();
    script_17774();
    script_178AB();
    Script_ReturnTimeDate(&task_1E0);
    if ((task_1E0.hours >= 19) & (task_1E0.minutes >= 0)) {
      if (~Script_ryoControl(PCCTL_QueryShouldRun)) {
        if (script_169B0() == 1) {
          task_408 = 1;
          Script_ExitCharaSelector();
          script_1695E();
          script_019A();
        }
      } else {
        task_408 = 1;
        script_1695E();
        script_019A();
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_176B3(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    (void)((task_1FE = 0) & 8);
  } else {
    (void)((task_1FE = -1) & 8);
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4155524D, 1);
  }
  return;
}

static void script_176FC(void) {
  int8_t local_FF;
  if (task_1FF == 0) {
  } else {
    (void)((local_FF = 0) & 8);
    if (task_1FE & 0x20) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
        Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4155524D, 0);
        (void)((task_1FE = 0) & 8);
        (void)((local_FF = -1) & 8);
      }
    }
    if (local_FF & 0x80) {
      Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    }
  }
  return;
}

static void script_17774(void) {
  int32_t local_FC;
  if ((&task_14)->mode) {
    local_FC = task_14.entry_idx;
    switch (local_FC) {
      case 1:
        (void)((task_0 = 0) & 4);
        SCNF_WaitFrames_0000(60);
        script_010E();
        Script_shenRequestGameModuleTransition(80, SHEN_NAME('SUSI'), 0);
        break;
      case 2:
        (void)((task_0 = 0) & 4);
        SCNF_WaitFrames_0000(60);
        script_010E();
        Script_shenRequestGameModuleTransition(80, SHEN_NAME('TOUR'), 0);
        break;
      case 3:
        break;
    }
  }
  return;
}

static void script_177F6(void) {
  Script_scenUtils(SCENARIO_CMD_SET_CHARA_RADIUS_SQUARED, 0x45534F59, 4.0f);
  if (script_65E0() != -1) {
    script_65BF();
  }
  return;
}

static void script_1781B(void) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_201 == 0) {
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100)) {
    if (task_202 & 2) {
      (void)((task_202 = 0) & 4);
      SCNF_RequestIconNotification(0, 0, 0, 2);
    }
  } else if (~Script_ryoControl(PCCTL_QueryShouldRun)) {
  } else {
    script_14295();
    local_FC = script_FC6D();
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        script_10812();
        break;
      case 1:
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
        script_17C45();
        Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        break;
    }
  }
  return;
}

static void script_178AB(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  if (!((&local_F4)->x <= 20.05f) & !((&local_F4)->x >= 24.55f)) {
    if (!((&local_F4)->z <= 32.9f) & !((&local_F4)->z >= 35.0f)) {
      (void)((task_0 = 0) & 4);
      Script_ryoControl(PCCTL_RequestNewMode, 1);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_010E();
      Script_shenRequestGameModuleTransition(80, SHEN_NAME('GSEN'), 0);
    }
  }
  return;
}

static void script_1791E(const char *arg0, const char *arg1) {
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

__attribute__((scn_string_bindings(0, data_1B114, 1, data_1B11B, 2, data_1B122)))
static void script_17975(void) {
  Vector3i local_F4;
  int32_t local_F0;
  int32_t unused_EC;
  int32_t local_E8;
  int8_t local_E7;
  uint8_t unused_E4[3];
  if (Script_GetCurrentEntryPoint()) {
    local_F0 = Script_scenCreateTask(script_17476, 2, 18, 30);
  } else {
    local_F0 = Script_scenCreateTask(script_17476, 2, 12, 0);
  }
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  (void)((local_E7 = -(script_1821F() != 0)) & 1);
  Script_scenUtils(SCENARIO_CMD_RESET_RUNTIME_STATE);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 993, 1);
  Script_sprintf("save!\012");
  Script_CaptureTrialSaveState();
  if (local_E7 & 0x80) {
  } else {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 991, 1);
    Script_sprintf("save!\012");
    Script_CaptureTrialSaveState();
  }
  Script_refbSetActive(1);
  Script_SetVector4ByOffset(&data_1B104);
  Script_SetCameraFlags(0);
  Script_scenToggleVisibleFor(SHEN_NAME('CAR0'), 1);
  while (task_1FF == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_E8 = script_8E47(CHARA_TOM1);
  Script_charaSetRefboardFlagTo(CHARA_TOM1, 1);
  script_17F8A();
  if (local_E7 & 0x80) {
    SCNF_WaitFrames_0000(30);
  } else {
    Script_executeAicaCommand(2720, 5, 10);
    Script_shenSeqUtils(-8, Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 991));
  }
  Script_scenCreateTask(script_0550, 0);
  script_14A99(0);
  script_14D27(CHARA_AKIR);
  if (local_E7 & 0x80) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 991, 1);
    Script_sprintf("save!\012");
    Script_CaptureTrialSaveState();
  }
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_executeAicaCommand(2464, 5, 30);
  Script_filmCheckOrSetTransitionUtil(10, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ResetHandMotion(CHARA_TOM1, 0, 1, 0);
  Script_ResetHandMotion(CHARA_TOM1, 0, 0, 0);
  Script_FaceEx(CHARA_TOM1, FACE_ExpressionDisabled, 0, 2);
  Script_faceUpdateFor(CHARA_TOM1, 0, 0);
  Script_faceUpdateFor(CHARA_TOM1, 1, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_14CB8();
  Script_charaSetRefboardFlagTo(CHARA_TOM1, 0);
  Script_SetMapControlTaskFlag(SHEN_NAME('CAR0'), 1);
  script_8EAC(CHARA_TOM1, local_E8);
  Script_seqEnterEvtCam(3, 0, AUTH_None);
  Script_SetCameraFieldOfView(10922);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  local_F4.x = (&local_F4)->z = 0;
  (&local_F4)->y = 22653;
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(10, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_ryoControl(RYO_RestoreControl);
  Script_SetCameraFlags(1);
  Script_srchControl((void *)local_F0);
  return;
}

static void script_17C45(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(CHARA_AKIR, CCOW_DisableAll);
  Script_ccowSetHeightMode(CHARA_AKIR, CCOW_KeepHeight);
  local_F4.x = -71.45f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 80.16f;
  local_E8.x = (&local_E8)->z = 0;
  (&local_E8)->y = 5927;
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_E8, 0);
  Script_seqEnterEvtCam(2, CHARA_AKIR, CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_PULL_DOOR, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  while (1) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 15.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_17ED7, 0);
  while (1) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 20.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
  Script_scenCreateTask(script_17EFC, 0);
  local_F4.x = -71.75f;
  (&local_F4)->y = 2.04f;
  (&local_F4)->z = 79.4f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 2, &local_F4, 0);
  SCNF_WaitFrames_0000(60);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  SCNF_WaitFrames_0000(20);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_TURN_180_R, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  while (1) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  (&local_E8)->y = 38695;
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  while (1) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_ccowSetHeightMode(CHARA_AKIR, CCOW_FollowGround);
  Script_ccowUtils(CHARA_AKIR, CCOW_EnableAll);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static int32_t script_17ED7(void) {
  Script_executeAicaCommand(0x2904A9, 0, 0);
  SCNF_WaitFrames_0000(20);
  Script_executeAicaCommand(0x2A04A9, 0, 0);
  end:;
}

__attribute__((scn_string_bindings(2, data_1B13F)))
static int32_t script_17EFC(void) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_VISIT_LIU_BARBER)) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  } else {
    local_FC = 2;
  }
  local_F8 = local_FC;
  switch (local_F8) {
    case 0:
      Script_ShowUIMessageFromID("TS1001A003");
      break;
    case 1:
      Script_ShowUIMessageFromID("TS1001A002");
      break;
    case 2:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SEEK_THE_THREE_BLADES)) {
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_VISIT_LIU_BARBER, 1);
        Script_sprintf("save!\012");
        Script_CaptureTrialSaveState();
      }
      Script_ShowUIMessageFromID("TS1001A001");
      break;
  }
  end:;
}

static void script_17F8A(void) {
  Script_ambSoundUtils(1, 1, &data_1B154, 60.0f, 50.0f, 1.0f, 0, &data_1B1D8);
  Script_ambSoundUtils(2, 1, &data_1B160, 30.0f, 150.0f, 1.0f, 2, data_1B25C);
  Script_ambSoundUtils(3, 0, &data_1B16C, 20.0f, 60.0f, 0.8f, 0, data_1B2E0);
  Script_ambSoundUtils(4, 0, &data_1B178, 3.0f, 10.0f, 0.8f, 0, data_1B364);
  Script_ambSoundUtils(5, 1, &data_1B184, 5.0f, 95.0f, 0.7f, 0, &data_1B3E8);
  Script_ambSoundUtils(6, 0, &data_1B190, 1.5f, 3.0f, 0.8f, 0, data_1B46C);
  Script_ambSoundUtils(7, 1, &data_1B19C, 30.0f, 50.0f, 0.8f, 2, &data_1B4F0);
  Script_ambSoundUtils(8, 1, &data_1B1A8, 30.0f, 150.0f, 1.0f, 0, data_1B574);
  Script_ambSoundUtils(9, 0, &data_1B1B4, 3.0f, 40.0f, 1.0f, 0, data_1B5F8);
  Script_ambSoundUtils(10, 1, &data_1B1C0, 28.0f, 40.0f, 1.0f, 0, data_1B67C);
  Script_ambSoundUtils(12, 1, &data_1B1CC, 30.0f, 170.0f, 1.0f, 0, &data_1B700);
  Script_executeAicaCommand(2464, 1, 15);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

__attribute__((scn_string_bindings(0, data_1B784, 1, data_1B794, 2, data_1B79C, 3, data_1B7AC, 4, data_1B7B2, 5, data_1B7C2, 6, data_1B7CB, 7, data_1B7DB, 8, data_1B7E3, 9, data_1B7F3)))
static int32_t script_1812E(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  script_1791E("/scene/80/TIKN/", "FACEPAK");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, -1);
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 2);
  (void)((task_203 = -1) & 1);
  script_1791E("/scene/80/TIKN/", "SHUKI");
  script_1791E("/scene/80/TIKN/", "STARTMOT");
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  script_1791E("/scene/80/TIKN/", "AUTHOR1");
  (void)((task_1FF = -1) & 1);
  script_176B3();
  script_1791E("/scene/80/TIKN/", "CTER");
  while (task_200 == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 1);
  return (task_201 = -1) & 1;
}

static int32_t script_1821F(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_E0[4];
  Vector3 local_D4;
  int32_t local_D0;
  int32_t local_CC;
  int32_t unused_C8;
  local_CC = 0;
  if (MMTK_ScriptMemoToggle(2, 0) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 993)) {
    script_1562C(0);
  } else {
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_D4, ENTITY_XFORM_WORLD_SPACE);
    (&local_D4)->y = (&local_D4)->y - 40.0f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_D4);
    (&local_D4)->y = (&local_D4)->y + 40.0f;
    Script_WeatherTaskUtil(ToggleCloudXYFlags, 0);
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
    Script_SetFogFlags(1, (uint32_t *)&local_F0);
    local_E0[0] = local_E0[1] = local_E0[2] = local_E0[3] = -0x1000000;
    Script_SetFogFlags(0, (const uint32_t *)local_E0);
    local_D0 = 3;
    while (local_D0) {
      local_D0 = local_D0 - 1;
      Script_mapmSetRenderFlag(local_D0, 0);
    }
    Script_executeAicaCommand(2720, 5, 10);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(1);
    MMTK_ScriptMemoToggle(0, 1);
    while (MMTK_ScriptMemoToggle(2, 0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    while (1) {
      if (Script_GetControllerInput(1) & 0x400) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(1);
    MMTK_ScriptMemoToggle(1, 0);
    SCNF_WaitFrames_0000(30);
    Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
    Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_D4);
    SCNF_WaitFrames_0000(1);
    Script_charaDisplay(CHARA_AKIR, CDM_Show);
    Script_SetFogFlags(0, (const uint32_t *)&local_F0);
    Script_WeatherTaskUtil(ToggleCloudXYFlags, 1);
    local_D0 = 3;
    while (local_D0) {
      local_D0 = local_D0 - 1;
      Script_mapmSetRenderFlag(local_D0, 1);
    }
    script_1562C(0);
    local_CC = 1;
  }
  return local_CC;
}

static void script_18440(void) {
  while (1) {
    Script_PreserveAccumulator_0182(1, 0, 11);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1845F(int32_t arg0) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
  while (local_FC == 0) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
  }
  L0184A5:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L0184A5;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return local_FC;
}

static void script_184C6(int32_t arg0, int32_t arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  return;
}
