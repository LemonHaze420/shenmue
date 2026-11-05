#include "act80_gsen.h"

static void script_0030(int32_t arg0) {
  (void)((task_8 = -1) & 1);
  (void)((task_C = 0) & 1);
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
  (void)((task_6 = 0) & 1);
  return;
}

static int32_t script_00CC(void) {
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (task_C & 2) {
  } else {
    Script_shenClearFreeState(1);
  }
  end:;
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
  if (~task_8) {
  } else {
    Script_ReturnTimeDate(&local_F4);
    if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 0)) {
      (void)((task_C = -1) & 8);
      Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
      Script_shenClearFreeState(0);
      Script_ryoControl(PCCTL_RequestNewMode, 1);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      if (task_0 != -1) {
        Script_srchControl((void *)task_0);
        Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
      }
      Script_executeAicaCommand(196768, 0, 0);
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

static int32_t script_0341(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  SCNF_WaitFrames_0000(60);
  if (arg0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, arg1)) goto L000430;
    Script_ReturnTimeDate(&local_F4);
    if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 0)) goto L000430;
    Script_sicnUtils(0, 0, 1);
    (void)((task_6 = -1) & 8);
    (void)((task_7 = -1) & 8);
    while (1) {
      if (task_6 == 0) {
        break;
      }
      local_F0 = Script_GetControllerInput(1);
      if (task_7 & 0x80) {
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
    if (task_6 & 0x40) {
      Script_sicnUtils(1, 0, 0);
      (void)((task_6 = 0) & 2);
    }
  } else {
    Script_sicnUtils(0, 0, 0);
  }
  L000430:;
}

static void script_0431(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4) * 5 + 1);
  *(int32_t *)&local_A4 = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4) * 5 + 2);
  local_98 = 0x425C0000;
  *(int32_t *)&local_A0 = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4) * 5 + 3);
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4) * 5 + 4);
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
      local_88 = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4) * 5);
      local_8C = 18;
      local_90 = 18;
      local_AC = 1.0f;
    } else {
      local_88 = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4) * 5) | -0x80000000;
      local_8C = 12;
      local_90 = 12;
      local_AC = -1.0f;
    }
    local_88 = local_88 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_A8 - 3.0f) << 16) & 0x7FFF0000);
    if (task_134 == arg0) {
      if (task_154) {
        Script_seqEnterEvtCam(arg4 * 4 + 800, arg1, arg1);
      }
    }
    script_3CD0(arg0, 0);
    local_C4.x = (&task_13C)->x;
    (&local_C4)->y = task_13C.y;
    (&local_C4)->z = task_13C.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMoveTarget(arg0, &task_13C, *(&task_148 + 1), local_88);
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
          local_FF78 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
          local_FF78 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_FF78 * 9 + 1) * 4 + 2);
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
        if (task_134 == arg0) {
          if (task_154) {
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
          local_FF74 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
          local_FF74 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_FF74 * 9 + 1) * 4 + 3);
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
  script_3CD0(arg0, 1);
  if (task_134 == arg0) {
    if (task_154) {
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
    local_88 = task_58[arg2];
    local_FF78 = arg3;
    if ((local_FF78 == 9) | (local_FF78 == 10) | (local_FF78 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_DC, 0, CHARA_PART_Set);
        Script_charaPosRotUtils(arg1, 7, &local_D0, 0, CHARA_PART_Set);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_DC);
      }
      if (task_134 == arg0) {
        local_90 = (Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_90, 1);
        local_90 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_90, 1);
      }
    } else if ((local_FF78 == 18) | (local_FF78 == 19) | (local_FF78 == 21) | (local_FF78 == 22)) {
      local_90 = (Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_90, 1);
      {
        int32_t local_FF74;
        local_FF74 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 1);
        local_FF74 = local_FF74 | 8;
        Script_ScenDataUtil(task_10, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
      }
      {
        int32_t local_FF74;
        local_FF74 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 1);
        local_FF74 = local_FF74 | 0x10;
        Script_ScenDataUtil(task_10, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
      }
      {
        int32_t local_FF74;
        local_FF74 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 1);
        local_FF74 = local_FF74 | 2;
        Script_ScenDataUtil(task_10, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
      }
    } else {
      if ((local_FF78 == 20) | (local_FF78 == 23)) {
        if ((arg4 == 0) | (arg4 == 1)) {
          local_90 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_90, 1);
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 0x10;
            Script_ScenDataUtil(task_10, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 4;
            Script_ScenDataUtil(task_10, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
        } else {
          local_90 = (Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_90, 1);
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 8;
            Script_ScenDataUtil(task_10, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
          {
            int32_t local_FF74;
            local_FF74 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 1);
            local_FF74 = local_FF74 | 4;
            Script_ScenDataUtil(task_10, SCND_WriteU32, local_88 * 6 + 1, local_FF74);
          }
        }
        goto L001075;
      }
    }
  }
  L001075:;
  return;
}

static void script_1076(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_134 == arg0) {
    local_FC = task_58[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_FC * 6 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_10, SCND_ReadU32, local_FC * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_10E2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_13C)->x;
  (&local_AC)->y = task_13C.y;
  (&local_AC)->z = task_13C.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
    local_80 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_80 * 9 + 1) * 4 + 2);
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
          task_84[arg2] = arg0;
          local_98 = -local_98;
          local_84 = 0;
        }
        local_8C = 0;
      } else {
        if (local_8C == 0) {
          int32_t local_80;
          local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
          local_80 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
          if (local_80 != -1) {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
          local_93 = 0;
          local_8C = 1;
          local_98 = -local_98;
          local_84 = 0;
          task_84[arg2] = 0;
        }
      }
    } else {
      if (local_8C == 0) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
        local_80 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_84[arg2] = 0;
      }
    }
    if (task_134 == arg0) {
      local_88 = task_58[arg2];
      local_88 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_88 * 6 + 2) & 0xFFFF;
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
        task_84[arg2] = 0;
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
          goto L00157E;
        }
      }
    }
    L00157E:;
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

static void script_15CE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5);
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
          local_FC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5);
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
          local_FC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          break;
      }
      break;
  }
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 2);
  local_BC = 0.0f;
  script_3CD0(arg0, 0);
  local_CC.x = (&task_13C)->x;
  (&local_CC)->y = task_13C.y;
  (&local_CC)->z = task_13C.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMoveTarget(arg0, &task_13C, *(&task_148 + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_A0 * 9 + 1) * 4 + 2);
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
  script_3CD0(arg0, 1);
  local_FC = task_58[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_FC * 6 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_10, SCND_WriteU32, local_FC * 6 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_FC * 6 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_10, SCND_WriteU32, local_FC * 6 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_FC * 6 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_10, SCND_WriteU32, local_FC * 6 + 1, local_A0);
  }
  return;
}

static void script_1BBA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
        local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5);
        local_C0 = 1.0f;
        local_A8 = 18;
        local_A4 = 18;
        local_98 = 15473;
        local_94 = 17294;
        break;
      case 0:
        local_CF = 0;
        local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
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
        local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
        local_C0 = 1.0f;
        local_A8 = 12;
        local_A4 = 18;
        local_98 = 50062;
        local_94 = 48241;
        break;
      case 0:
        local_CF = 1;
        local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5);
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
        local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
        local_C0 = -1.0f;
        local_A8 = 12;
        local_A4 = 12;
        local_98 = 50062;
        local_94 = 48241;
        break;
      case 0:
        local_CF = 0;
        local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5);
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
        local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5);
        local_C0 = -1.0f;
        local_A8 = 18;
        local_A4 = 12;
        local_98 = 15473;
        local_94 = 17294;
        break;
      case 0:
        local_CF = 1;
        local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
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
          local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          local_C0 = 1.0f;
          local_A8 = 18;
          local_A4 = 18;
          local_98 = 48241;
          local_94 = 50062;
          break;
        case 1:
          local_CF = 0;
          local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          local_C0 = 1.0f;
          local_A8 = 12;
          local_A4 = 18;
          local_98 = 50062;
          local_94 = 48241;
          break;
        case 2:
          local_CF = 0;
          local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5);
          local_C0 = -1.0f;
          local_A8 = 18;
          local_A4 = 12;
          local_98 = 15473;
          local_94 = 17294;
          break;
        case 3:
          local_CF = 1;
          local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5) | -0x80000000;
          local_C0 = -1.0f;
          local_A8 = 12;
          local_A4 = 12;
          local_98 = 17294;
          local_94 = 15473;
          break;
      }
      local_CE = 1;
      goto L0020EB;
    }
  }
  L0020EB:;
  *(int32_t *)&local_BC = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 1);
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 2);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 3);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4 + arg4) * 5 + 4);
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
  script_3CD0(arg0, 0);
  local_D0.x = (&task_13C)->x;
  (&local_D0)->y = task_13C.y;
  (&local_D0)->z = task_13C.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMoveTarget(arg0, &task_13C, *(&task_148 + 1), local_AC);
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
        local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
        local_84 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_84 * 9 + 1) * 4 + 2);
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
        local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
        local_84 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_84 * 9 + 1) * 4 + 3);
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
  script_3CD0(arg0, 1);
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

static void script_26C7(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5) {
  int32_t local_FC;
  task_10 = arg1;
  task_14 = arg2;
  task_18 = arg3;
  task_1C = arg4;
  task_20.mode = 0;
  task_20.id = 0;
  task_20.entry_idx = 0;
  task_134 = arg0;
  task_158 = 0;
  {
    int32_t local_F8;
    local_F8 = 0;
    L002714:;
    if (local_F8 == 10) {
      local_FC = Script_AddCharacterHandler(1, 0, 0x40786F62);
    } else {
      local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
    }
    Script_scenToggleVisibleFor(local_FC, 0);
    task_DC[local_F8] = local_FC;
    task_2C[local_F8] = -1;
    task_58[local_F8] = -1;
    task_84[local_F8] = 0;
    task_B0[local_F8] = 0;
    task_108[local_F8] = 0;
    local_F8 = local_F8 + 1;
    if (!(local_F8 >= 11)) goto L002714;
  }
  {
    int32_t local_F8;
    local_F8 = 0;
    for (;;) {
      if (Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 6) == -1) {
        break;
      }
      Script_ScenDataUtil(task_10, SCND_WriteU32, local_F8 * 6 + 1, 0);
      local_F8 = local_F8 + 1;
    }
  }
  task_154 = 0;
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_20, task_2C, task_58, (shen_name *)task_84, arg0, arg5, data_F828, data_F89C);
  Script_scenCreateTask(script_2B87, 0);
  return;
}

static void script_282F(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_10 = arg0;
  task_14 = arg1;
  task_18 = arg2;
  task_1C = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_158 = 1;
  return;
}

static void script_2863(void) {
  int32_t local_FC;
  local_FC = 0;
  L00286B:;
  task_2C[local_FC] = -1;
  local_FC = local_FC + 1;
  if (!(local_FC >= 11)) goto L00286B;
  return;
}

static void script_288B(void) {
  return;
}

static void script_288E(void) {
  return;
}

static void script_2891(int32_t arg0, int32_t arg1) {
  DSWK_DoorUtils(DOOR_CMD_ALLOC_ENTRY_SLOT, arg0, arg1);
  return;
}

static void script_289E(void) {
  return;
}

static void script_28A1(int32_t arg0, int32_t arg1) {
  DSWK_DoorUtils(DOOR_CMD_ALLOC_ENTRY_SLOT, arg0, arg1);
  return;
}

static void script_28AE(int32_t arg0) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F4);
  return;
}

static void script_28C7(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_10, SCND_ReadU32, local_FC * 6) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_FC * 6 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_10, SCND_WriteU32, local_FC * 6 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_292A(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_10, SCND_ReadU32, local_FC * 6) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_10, SCND_ReadU32, local_FC * 6 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_10, SCND_WriteU32, local_FC * 6 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_298D(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_10, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_10, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  return;
}

static void script_29C4(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_10, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_10, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  return;
}

static void script_29FB(int32_t arg0) {
  task_154 = arg0;
  return;
}

static void script_2A05(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_58[local_F0] == arg0) {
      local_EC = task_DC[local_F0];
      if (Script_shenGetNode(local_EC, 8) & Script_shenGetNode(local_EC, 13)) {
        if (Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_F0] * 6 + 1) & 0x20) {
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

static void script_2B07(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_10, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_10, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  script_2A05(arg0);
  return;
}

static void script_2B47(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_10, SCND_ReadU32, arg0 * 6 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_10, SCND_WriteU32, arg0 * 6 + 1, local_FC);
  script_2A05(arg0);
  return;
}

static int32_t script_2B87(void) {
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
    if (task_158 == 0) {
      local_E8 = local_EC;
      local_E4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_E4 != -1) & -(task_2C[local_E4] == 1) & Script_ryoControl(PCCTL_QueryShouldRun) & -(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1)) {
        local_F0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[local_E4], 2);
        local_EC = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 1) * 4 + 1);
        local_C0 = local_EC;
        if ((local_C0 == 17) | (local_C0 == 11) | (local_C0 == 12)) {
          local_CA = 0;
          if ((Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_E4] * 6 + 1) & 2) == 0) {
            if ((Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_E4] * 6 + 1) & 4) == 0) {
              task_2C[local_E4] = 4;
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
          if ((Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_E4] * 6 + 1) & 2) == 0) {
            if (((Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_E4] * 6 + 1) & 4) == 0) & (Script_ryoControl(PCCTL_GetTatehiza) == 0)) {
              local_CB = 0;
              local_BC = script_3787(task_134, task_DC[local_E4], -1, 0.5f, 0.8f, local_EC);
              Script_mtwkGetSetRotation(task_134, (Vector3i *)local_AC, 0x42000000);
              Script_mtwkGetSetRotation(task_DC[local_E4], &local_A0, 0);
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
            if (-(task_84[local_E4] != task_134) & local_CB) {
              if ((Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_E4] * 6 + 1) & 2) == 0) {
                if ((Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_E4] * 6 + 1) & 4) == 0) {
                  task_2C[local_E4] = 3;
                  task_84[local_E4] = task_134;
                  task_20.mode = 1;
                  task_20.entry_idx = task_58[local_E4];
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
      L002F89:;
      local_C0 = task_2C[local_F4];
      switch (local_C0) {
        case 5:
          if (script_3E06(task_84[local_F4], local_F4, 0) == 0) {
            if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_84[local_F4])) {
              local_BC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[local_F4], 2);
              local_B8 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_BC * 9 + 1) * 4 + 1);
              if (local_B8 == 17) {
              } else {
                script_3CF8(task_84[local_F4], task_58[local_F4], local_B8);
              }
              task_108[local_F4] = Script_scenCreateTask(Script_ScenDataUtil(&data_F950, SCND_ReadScriptOffset, local_B8), 4, task_84[local_F4], task_DC[local_F4], local_F4, local_B8);
              task_B0[local_F4] = task_84[local_F4];
              script_363D(local_F4);
            } else {
              task_2C[local_F4] = 1;
              task_108[local_F4] = 0;
              task_B0[local_F4] = 0;
              task_84[local_F4] = 0;
            }
          }
          break;
        case 4:
          task_84[local_E4] = task_134;
          task_20.mode = 2;
          task_20.entry_idx = task_58[local_E4];
          if (script_3E06(task_84[local_F4], local_F4, 0) == 0) {
            task_108[local_F4] = Script_scenCreateTask(Script_ScenDataUtil(&data_F950, SCND_ReadScriptOffset, local_EC), 4, task_84[local_F4], task_DC[local_F4], local_F4, local_EC);
            task_B0[local_F4] = task_84[local_F4];
            script_363D(local_F4);
          }
          break;
        case 3:
          if (local_EC >= 0) {
            if (script_3E06(task_84[local_F4], local_F4, 0) == 0) {
              script_3CF8(task_84[local_F4], task_58[local_F4], local_EC);
              task_108[local_F4] = Script_scenCreateTask(Script_ScenDataUtil(&data_F950, SCND_ReadScriptOffset, local_EC), 4, task_84[local_F4], task_DC[local_F4], local_F4, local_EC);
              task_B0[local_F4] = task_84[local_F4];
              script_363D(local_F4);
            }
          }
          break;
        case 2:
          script_3E06(task_84[local_F4], local_F4, 1);
          local_F0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[local_F4], 2);
          local_FC = task_DC[local_F4];
          if (local_F0 != -1) {
            *(int32_t *)&local_D8.x = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 5);
            *(int32_t *)&(&local_D8)->y = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 6);
            *(int32_t *)&(&local_D8)->z = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 7);
            Script_EVT_SetPosition(local_FC, POS_XYZ, &local_D8);
            local_CC.x = 0;
            (&local_CC)->y = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 8);
            (&local_CC)->z = 0;
            Script_EVT_SetRotation(local_FC, 0x38000000, &local_CC, 0);
            *(int32_t *)&local_D8.x = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 2);
            *(int32_t *)&(&local_D8)->y = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 3);
            *(int32_t *)&(&local_D8)->z = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 4);
            Script_hmdlApplyScale(local_FC, &local_D8);
            Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_18, SCND_ReadScriptOffset, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 1) * 4), 0);
            Script_dynamicStuff(local_FC, (const char *)Script_ScenDataUtil(task_18, SCND_ReadScriptOffset, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 1) * 4), 0);
            Script_charaDisplay(local_FC, CDM_Show);
            Script_charaVisUtils(local_FC, 0);
            local_C0 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F0 * 9 + 1) * 4 + 1);
            if ((local_C0 == 20) | (local_C0 == 23)) {
              if (Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_F4] * 6 + 1) & 0x10) {
                Script_charaPosRotUtils(local_FC, 7, &data_F9E4, 0, CHARA_PART_Add);
              } else {
                if (Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_F4] * 6 + 1) & 8) {
                  Script_charaPosRotUtils(local_FC, 12, &data_F9F0, 0, CHARA_PART_Add);
                  goto L003450;
                }
              }
              L003450:;
            } else if ((local_C0 == 18) | (local_C0 == 19) | (local_C0 == 21) | (local_C0 == 22)) {
            } else if ((local_C0 == 25) | (local_C0 == 26)) {
              if (Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_F4] * 6 + 1) & 0x10) {
                Script_mtwkGetSetRotation(local_FC, (Vector3i *)&local_B4, 0);
                *(&local_B4 + 1) = *(&local_B4 + 1) + 15958;
                Script_EVT_SetRotation(local_FC, 0x38000000, (const Vector3i *)&local_B4, 0);
              }
            } else {
              if ((local_C0 == 24) | (local_C0 == 27)) {
                if (Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_F4] * 6 + 1) & 0x10) {
                  Script_mtwkGetSetRotation(local_FC, (Vector3i *)&local_B4, 0);
                  *(&local_B4 + 1) = *(&local_B4 + 1) - 15958;
                  Script_EVT_SetRotation(local_FC, 0x38000000, (const Vector3i *)&local_B4, 0);
                }
                goto L00352D;
              }
            }
            L00352D:;
            if (Script_shenGetNode(local_FC, 8) & Script_shenGetNode(local_FC, 13)) {
              if (Script_ScenDataUtil(task_10, SCND_ReadU32, task_58[local_F4] * 6 + 1) & 0x20) {
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
            task_84[local_F4] = 0;
          } else {
            Script_charaDisplay(local_FC, CDM_Hide);
          }
          break;
      }
      local_F4 = local_F4 + 1;
      if (!(local_F4 >= 11)) goto L002F89;
    } else {
      task_158 = 0;
    }
    local_C4 = DSWK_DoorUtils(DOOR_CMD_TIMER_GET_DELTA_SCALED);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_363D(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg0], 1);
  if (local_F8 != -1) {
    local_FC = 0x40786F62;
    *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F8 * 9 + 5);
    *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F8 * 9 + 6);
    *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F8 * 9 + 7);
    Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
    local_E0.x = 0;
    (&local_E0)->y = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F8 * 9 + 8);
    (&local_E0)->z = 0;
    Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
    *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F8 * 9 + 2);
    *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F8 * 9 + 3);
    *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F8 * 9 + 4);
    Script_hmdlApplyScale(local_FC, &local_EC);
    Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_18, SCND_ReadScriptOffset, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_F8 * 9 + 1) * 4), 0);
    Script_charaDisplay(local_FC, CDM_Show);
    Script_charaVisUtils(local_FC, 0);
  }
  return;
}

static int32_t script_3787(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5) {
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
        if ((local_B4 == 1) | (local_B4 == 2)) goto L003884;
      }
      L003884:;
    } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26)) {
      local_B4 = local_C0;
      if ((local_B4 == 2) | (local_B4 == 1)) {
        arg3 = (float)0 - arg3;
      } else {
        if ((local_B4 == 3) | (local_B4 == 0)) {
          arg3 = (float)0 - arg3;
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
          goto L003915;
        }
      }
      L003915:;
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
          goto L003A34;
        }
      }
      L003A34:;
    } else {
      local_B4 = local_C0;
      if ((local_B4 == 2) | (local_B4 == 1)) {
      } else {
        if ((local_B4 == 3) | (local_B4 == 0)) {
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
          goto L003A7E;
        }
      }
    }
  }
  L003A7E:;
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
        task_13C.x = (&local_F4)->x;
        task_13C.y = (&local_F4)->y;
        task_13C.z = (&local_F4)->z;
        task_148 = (&local_C4)->x;
        *(&task_148 + 1) = (&local_C4)->y;
        *(&task_148 + 2) = (&local_C4)->z;
        break;
      case 5:
        task_13C.x = (&local_F4)->x;
        task_13C.y = (&local_F4)->y;
        task_13C.z = (&local_F4)->z;
        task_148 = (&local_C4)->x;
        *(&task_148 + 1) = (&local_C4)->y;
        *(&task_148 + 2) = (&local_C4)->z;
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

static void script_3CD0(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    if (arg1) {
      Script_ccowUtils(arg0, CCOW_EnableMap);
    } else {
      Script_ccowUtils(arg0, CCOW_DisableMap);
    }
  }
  return;
}

static void script_3CF8(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_134 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
    local_F8 = arg2;
    if ((local_F8 == 25) | (local_F8 == 27) | (local_F8 == 24) | (local_F8 == 26) | (local_F8 == 18) | (local_F8 == 19) | (local_F8 == 20) | (local_F8 == 21) | (local_F8 == 22) | (local_F8 == 23)) {
      local_FC = Script_ScenDataUtil(task_10, SCND_ReadU32, arg1 * 6 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 0);
      local_FC = (Script_ScenDataUtil(task_10, SCND_ReadU32, arg1 * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 0);
      goto L003DB1;
    }
    L003DB1:;
  } else {
    local_FC = Script_ScenDataUtil(task_10, SCND_ReadU32, arg1 * 6 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_FC, 0);
    local_FC = (Script_ScenDataUtil(task_10, SCND_ReadU32, arg1 * 6 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_FC, 0);
  }
  return;
}

static int32_t script_3E06(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = 0;
  if (task_108[arg1] != 0) {
    if (arg2) {
      if ((task_134 == task_B0[arg1]) & (task_134 != arg0)) {
        task_20.mode = 0;
        Script_PlayCharaMotion(task_134, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_ryoControl(RYO_RestoreControl);
        script_3CD0(task_B0[arg1], 1);
      }
      Script_srchControl((void *)task_108[arg1]);
      task_108[arg1] = 0;
      task_B0[arg1] = 0;
      task_2C[arg1] = 1;
      local_FC = 0;
    } else {
      local_FC = 1;
    }
  }
  end:;
}

static void script_3EC2(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_134 == arg0) {
    task_20.mode = 0;
    if (arg2 == 0) {
      Script_PlayCharaMotion(task_134, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      local_F8 = task_58[arg1];
      local_FC = Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 6 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_10, SCND_ReadU32, local_F8 * 6 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      goto L003F5F;
    }
    L003F5F:;
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
  }
  task_2C[arg1] = 1;
  task_108[arg1] = 0;
  task_B0[arg1] = 0;
  task_84[arg1] = 0;
  Script_srchControl(0);
  return;
}

static void script_3FB1(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (task_134 == arg0) {
    task_20.mode = 0;
    Script_PlayCharaMotion(task_134, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
  }
  task_2C[arg1] = 1;
  task_108[arg1] = 0;
  task_B0[arg1] = 0;
  task_84[arg1] = 0;
  Script_srchControl(0);
  return;
}

static void script_4035(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_3EC2(arg0, arg2, 0);
  return;
}

static void script_4045(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3787(arg0, arg1, -1, 0.0f, 0.5f, -1);
    if (local_FC == 0) {
      local_FC = script_3787(arg0, arg1, 5, 0.45f, 0.8f, arg3);
    } else {
      local_FC = script_3787(arg0, arg1, 5, 0.45f, 0.8f, arg3);
    }
    script_1BBA(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_3EC2(arg0, arg2, 0);
  return;
}

static void script_40D7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3787(arg0, arg1, 4, 0.5f, 0.8f, arg3);
    script_1BBA(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_3EC2(arg0, arg2, 0);
  return;
}

static void script_4124(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3787(arg0, arg1, 5, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_0431(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_3FB1(arg0, arg2, 0);
  return;
}

static void script_4189(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3787(arg0, arg1, 5, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_0431(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_3FB1(arg0, arg2, 0);
  return;
}

static void script_41EE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3787(arg0, arg1, 4, 0.5f, 0.5f, arg3);
    script_0431(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_3FB1(arg0, arg2, 0);
  return;
}

static void script_423B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_3787(arg0, arg1, 0, 0.4f, 0.5f, arg3);
  }
  script_3EC2(arg0, arg2, 0);
  return;
}

static void script_4270(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3787(arg0, arg1, 4, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_10E2(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_3EC2(arg0, arg2, 1);
  return;
}

static void script_42D5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3787(arg0, arg1, 4, 0.5f, 0.5f, arg3);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_10E2(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_3EC2(arg0, arg2, 1);
  return;
}

static void script_433A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    script_3787(arg0, arg1, 4, 0.0f, 0.5f, arg3);
    script_10E2(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_3EC2(arg0, arg2, 1);
  return;
}

static void script_437F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
    local_FC = script_3787(arg0, arg1, -1, 0.4f, 0.5f, arg3);
    if (local_FC == 0) {
      local_FC = script_3787(arg0, arg1, 5, 0.5f, 0.8f, arg3);
    } else {
      local_FC = script_3787(arg0, arg1, 5, 0.5f, 0.8f, arg3);
    }
    script_15CE(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_3FB1(arg0, arg2, 0);
  return;
}

static void script_4414(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_9B = script_3787(arg0, arg1, 5, 0.0f, 0.5f, arg3);
    script_3CD0(arg0, 0);
    local_FC = Script_ScenDataUtil(data_F89C, SCND_ReadU32, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4) * 5);
    local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_AC - 3.0f) << 16) & 0x7FFF0000);
    local_B4.x = (&task_13C)->x;
    (&local_B4)->y = task_13C.y;
    (&local_B4)->z = task_13C.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMoveTarget(arg0, &task_13C, *(&task_148 + 1), local_FC);
    while (Script_GetCharaMoveStatus(arg0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    *(int32_t *)&local_AC = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4) * 5 + 1);
    *(int32_t *)&local_A8 = Script_ScenDataUtil(data_F89C, SCND_ReadFloatBits, Script_ScenDataUtil(data_F828, SCND_ReadU8, arg3 * 4) * 5 + 2);
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
            local_94 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
            local_94 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_94 * 9 + 1) * 4 + 2);
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
            local_94 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
            local_94 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_94 * 9 + 1) * 4 + 2);
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
    script_3CD0(arg0, 1);
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
      local_94 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_58[arg2], 2);
      local_94 = Script_ScenDataUtil(task_18, SCND_ReadU32, Script_ScenDataUtil(task_1C, SCND_ReadU32, local_94 * 9 + 1) * 4 + 3);
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
  script_3EC2(arg0, arg2, 0);
  return;
}

static void script_4AB3(void) {
  script_4AC7(SHEN_NAME('DART'));
  script_4B8F();
  return;
}

static void script_4AC7(int32_t arg0) {
  task_174 = Script_GetHighScoreValue(Highscore_DART_Primary);
  task_176 = Script_GetHighScoreValue(Highscore_DART_Secondary);
  Script_GameUtils(arg0, GC_CommitStagedEntries, 0, 0, 0, 0);
  Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_F9FC, 9999, 0);
  Script_GameUtils(arg0, GC_StageNumericDisplayValue, 0, &data_FA14, 1.0f, &data_FA44);
  Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_FA08, 9999, 0);
  Script_GameUtils(arg0, GC_StageNumericDisplayValue, task_176, &data_FA20, 1.0f, &data_FA38);
  Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_FA08, 9999, 0);
  Script_GameUtils(arg0, GC_StageNumericDisplayValue, task_174, &data_FA2C, 1.0f, &data_FA38);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_4B8F(void) {
  Script_GameUtils(SHEN_NAME('SLTM'), GC_CommitStagedEntries, 0, 0, 0, 0);
  Script_GameUtils(SHEN_NAME('SLTM'), GC_ConfigureNumericDisplay, 0x100, &data_FA50, 0x5F5E0FF, 0);
  Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNumericDisplayValue, 0, &data_FA5C, 1.0f, 0);
  Script_GameUtils(SHEN_NAME('SLTM'), GC_ConfigureNumericDisplay, 0x100, &data_FA50, 999999, 0);
  Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNumericDisplayValue, 0, &data_FA74, 1.0f, 0);
  Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_FA68, 1.0f, 0);
  Script_SetCharaFlags(SHEN_NAME('SLTM'), 2);
  Script_SetCharaFlags(SHEN_NAME('SLTM'), 100, 0, &data_FA80);
  Script_SetCharaFlags(SHEN_NAME('SLTM'), 100, 1, &data_FA8C);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_4C65(void) {
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  return;
}

static void script_4C7A(void) {
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_uiUtils(0, 0, 0);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  return;
}

static void script_4C9A(void) {
  task_15C = 1;
  return;
}

static void script_4CA4(int8_t arg0) {
  arg0 = 1;
  if (arg0 == 1) {
    task_15C = 0;
  } else {
    task_15C = 4;
  }
  task_15C = 0;
  return;
}

static void script_4CCE(void) {
  script_4C9A();
  Script_sprintf(" START SLOT \012");
  script_4EE0();
  Script_sprintf(" END SLOT \012");
  script_4CA4(0);
  return;
}

static void script_4CF6(int32_t arg0, int32_t arg1) {
  task_188[arg0] = arg1;
  return;
}

static int32_t script_4D06(int32_t arg0) {
  int32_t local_FC;
  local_FC = task_188[arg0];
  return local_FC;
}

static int32_t script_4D1A(int32_t arg0) {
  task_188[1] = SCNF_RequestActionSelector(0x7FFF);
  return Script_srchControl(0);
}

static int32_t script_4D35(int32_t arg0) {
  script_5D01(arg0);
  return Script_srchControl(0);
}

static int32_t script_4D47(int32_t arg0) {
  script_5972(arg0);
  return Script_srchControl(0);
}

static int32_t script_4D59(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = SCNF_UIUtils(arg0, arg1, 1, 14, 16, 0);
  {
    int32_t local_F8;
    local_F8 = local_FC & 0xF000;
    task_188[2] = local_F8;
  }
  {
    int32_t local_F8;
    local_F8 = local_FC & 0x7FF;
    task_188[3] = local_F8;
  }
  return Script_srchControl(0);
}

static void script_4DA3(void) {
  int32_t local_FC;
  int32_t local_F8;
  (void)((task_8 = -1) & 1);
  local_FC = Script_GetControllerInput(0);
  local_F8 = local_FC;
  if (local_F8 == 0x8000) {
    script_4CF6(4, 2);
  } else if (local_F8 == 0x4000) {
    script_4CF6(4, 6);
  } else if (local_F8 == 0x2000) {
    script_4CF6(4, 4);
  } else if (local_F8 == 0x1000) {
    script_4CF6(4, 0);
  } else if (local_F8 == 0x400) {
    script_4CF6(4, 9);
  } else if (local_F8 == 0x200) {
    script_4CF6(4, 10);
  } else {
    script_4CF6(4, 13);
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_4E66(int32_t arg0) {
  (void)((task_8 = -1) & 1);
  task_188[1] = SCNF_RequestActionSelectorFromParams(0xFFFF, 36, 34, 0xFFFF, 0x7FFF);
  (void)((task_8 = 0) & 0x20);
  return Script_srchControl(0);
}

static int32_t script_4EA3(int32_t arg0) {
  (void)((task_8 = -1) & 0x20);
  task_188[1] = SCNF_RequestActionSelectorFromParams(0xFFFF, 37, 34, 0xFFFF, 0x7FFF);
  (void)((task_8 = 0) & 0x20);
  return Script_srchControl(0);
}

static void script_4EE0(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int16_t local_F2;
  int16_t local_F0;
  int16_t local_EE;
  int16_t local_EC;
  int16_t local_EA;
  int32_t local_E4;
  local_FC = 0;
  local_F2 = 2;
  while (local_F2 != 0) {
    local_F2 = *(int16_t *)((int8_t *)&data_FD0C + (local_FC << 1));
    local_F0 = *(int16_t *)((int8_t *)&data_FD0C + ((local_FC + 1) << 1));
    local_EE = *(int16_t *)((int8_t *)&data_FD0C + ((local_FC + 2) << 1));
    local_EC = *(int16_t *)((int8_t *)&data_FD0C + ((local_FC + 3) << 1));
    local_EA = *(int16_t *)((int8_t *)&data_FD0C + ((local_FC + 4) << 1));
    local_E4 = local_F2;
    switch (local_E4) {
      case 0:
        goto L0051C5;
        break;
      case 1:
        local_FC = local_F0 * 15 - 5;
        break;
      case 2:
        if (local_F0 == 0) {
          SCNF_WaitFrames_0000(1);
        } else {
          SCNF_WaitFrames_0000(local_F0);
        }
        break;
      case 3:
        task_188[1] = -1;
        Script_scenCreateTask(script_4D1A, 1, local_F0);
        break;
      case 4:
        task_188[3] = -1;
        Script_scenCreateTask(script_4D59, 2, local_EE, local_EC);
        break;
      case 16:
        script_4DA3();
        break;
      case 6:
        task_188[local_F0] = local_EE;
        break;
      case 7:
        task_188[local_F0] = task_188[local_F0] + local_EE;
        break;
      case 8:
        task_188[local_F0] = task_188[local_F0] - local_EE;
        break;
      case 9:
        local_F8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, (float)local_EE);
        task_188[local_F0] = local_F8;
        break;
      case 5:
        local_F8 = task_188[local_EE];
        if (local_F8 == local_EC) {
          local_FC = local_F0 * 15 - 5;
        }
        break;
      case 15:
        local_F8 = task_188[local_EE];
        local_F4 = task_188[local_EC];
        if (local_F8 == local_F4) {
          local_FC = local_F0 * 15 - 5;
        }
        break;
      case 10:
        script_54E2(local_EE, local_F0, local_EC);
        break;
      case 11:
        script_51C6(local_F0);
        break;
      case 14:
        script_5782(local_F0);
        break;
      case 13:
        Script_scenCreateTask(script_4D35, 1, local_F0);
        break;
      case 12:
        Script_scenCreateTask(script_4D47, 1, local_F0);
        SCNF_WaitFrames_0000(local_EE);
        break;
      case 17:
        task_188[1] = -1;
        Script_scenCreateTask(script_4E66, 1, local_F0);
        break;
      case 18:
        task_188[1] = -1;
        Script_scenCreateTask(script_4EA3, 1, local_F0);
        break;
    }
    local_FC = local_FC + 5;
  }
  L0051C5:;
  return;
}

static void script_51C6(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      break;
    case 1:
      script_5F21();
      break;
    case 2:
      script_6021();
      break;
    case 3:
      script_60DD();
      break;
    case 4:
      script_611E();
      break;
    case 5:
      script_6133();
      break;
    case 6:
      script_6382();
      break;
    case 7:
      script_6459();
      break;
    case 8:
      Script_scenCreateTask(script_64BD, 0);
      break;
    case 9:
      script_6D12();
      break;
    case 10:
      script_6CAB();
      break;
    case 11:
      Script_scenCreateTask(script_6F58, 0);
      break;
    case 12:
      script_750E();
      break;
    case 19:
      script_80EF();
      break;
    case 17:
      Script_scenCreateTask(script_83C6, 0);
      break;
    case 18:
      Script_scenCreateTask(script_811A, 0);
      break;
    case 13:
      Script_scenCreateTask(script_8CD0, 0);
      break;
    case 14:
      Script_scenCreateTask(script_89F5, 0);
      break;
    case 15:
      Script_scenCreateTask(script_8AB9, 0);
      break;
    case 16:
      script_8CAC();
      break;
    case 29:
      script_7CEE();
      break;
    case 20:
      script_7DBA();
      break;
    case 21:
      script_7E20();
      break;
    case 22:
      script_7E8A();
      break;
    case 23:
      script_7F1B();
      break;
    case 24:
      script_85AF();
      break;
    case 25:
      script_85D4();
      break;
    case 26:
      script_85F9();
      break;
    case 27:
      script_862A();
      break;
    case 28:
      script_8817();
      break;
    case 30:
      Script_scenCreateTask(script_7FCB, 0);
      break;
    case 31:
      Script_scenCreateTask(script_8004, 0);
      break;
    case 32:
      Script_scenCreateTask(script_803E, 0);
      break;
    case 33:
      Script_scenCreateTask(script_806E, 0);
      break;
    case 34:
      Script_scenCreateTask(script_8098, 0);
      break;
    case 35:
      script_80BF();
      break;
    case 36:
      script_80CF();
      break;
    case 37:
      script_80DF();
      break;
    case 39:
      script_6F22();
      break;
    case 38:
      script_6F15();
      break;
    case 40:
      script_622B();
      break;
    case 41:
      Script_scenCreateTask(script_6286, 0);
      break;
  }
  return;
}

static void script_54E2(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      local_FC = arg1;
      switch (local_FC) {
        case 0:
          break;
        case 1:
          Script_PlayCharaMotion(CHARA_AKIR, 30984, 0.0f, 149.0f, 0.0f, 1.5f, 0);
          break;
        case 2:
          Script_PlayCharaMotion(CHARA_AKIR, 30984, 0.0f, 60.0f, 0.0f, 1.0f, 0);
          break;
        case 3:
          Script_PlayCharaMotion(CHARA_AKIR, 30992, 0.0f, 39.0f, 0.0f, 1.0f, 0);
          break;
        case 4:
          Script_PlayCharaMotion(CHARA_AKIR, 30990, 0.0f, 26.0f, 0.0f, 2.0f, 0);
          SCNF_WaitFrames_0000(5);
          Script_PlayCharaMotion(CHARA_AKIR, 30992, 39.0f, 39.0f, 39.0f, 1.0f, 0);
          break;
        case 5:
          Script_PlayCharaMotion(CHARA_AKIR, 30991, 0.0f, 91.0f, 0.0f, 2.0f, 0);
          break;
        case 6:
          Script_PlayCharaMotion(CHARA_AKIR, 30983, 35.0f, 120.0f, 35.0f, 1.5f, 0);
          break;
        case 7:
          Script_PlayCharaMotion(CHARA_AKIR, 30983, 0.0f, 35.0f, 0.0f, 2.0f, 0);
          break;
        case 8:
          Script_PlayCharaMotion(CHARA_AKIR, 30983, 80.0f, 120.0f, 80.0f, 2.0f, 0);
          Script_SetNewHandMotion(CHARA_AKIR, 1, 761, 0, 0, 0, 0, 1.0f, 0);
          break;
        case 9:
          Script_PlayCharaMotion(CHARA_AKIR, 30983, 0.0f, 120.0f, 0.0f, 2.0f, 0);
          break;
        case 10:
          Script_SetMotnFlag_0(CHARA_AKIR, 26);
          Script_PlayCharaMotion(CHARA_AKIR, 31001, 0.0f, 103.0f, 0.0f, 2.0f, 0);
          SCNF_WaitFrames_0000(15);
          Script_SetNewHandMotion(CHARA_AKIR, 1, 762, 0x41700000, 0x42C80000, 0x41200000, 0, 2.0f, 0);
          SCNF_WaitFrames_0000(5);
          Script_executeAicaCommand(0x3606A9, 0, 0);
          break;
        case 11:
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, 0.0f, 60.0f, 0.0f, 1.0f, 30);
          break;
      }
      break;
    case 1:
      local_FC = arg1;
      if (local_FC == 0) goto L00577E;
      L00577E:;
      break;
  }
  return;
}

static void script_5782(int32_t arg0) {
  {
    int32_t local_FC;
    local_FC = arg0;
    switch (local_FC) {
      case 0:
        break;
      case 1:
        Script_seqEnterEvtCam(7100, 0, AUTH_None);
        break;
      case 2:
        Script_seqEnterEvtCam(7101, 0, AUTH_None);
        break;
      case 3:
        Script_seqEnterEvtCam(7102, 0, AUTH_None);
        break;
      case 4:
        Script_seqEnterEvtCam(7103, 0, AUTH_None);
        break;
      case 5:
        Script_seqEnterEvtCam(7104, 0, AUTH_None);
        break;
      case 6:
        Script_seqEnterEvtCam(7105, 0, AUTH_None);
        break;
      case 7:
        Script_seqEnterEvtCam(7106, 0, AUTH_None);
        break;
      case 8:
        Script_seqEnterEvtCam(7107, 0, AUTH_None);
        break;
      case 9:
        Script_seqEnterEvtCam(7108, 0, AUTH_None);
        break;
      case 10:
        Script_seqEnterEvtCam(7109, 0, AUTH_None);
        break;
      case 11:
        Script_seqEnterEvtCam(7110, 0, AUTH_None);
        break;
      case 12:
        Script_seqEnterEvtCam(7111, 0, AUTH_None);
        break;
      case 13:
        Script_seqEnterEvtCam(7112, 0, AUTH_None);
        break;
      case 14:
        Script_seqEnterEvtCam(7113, 0, AUTH_None);
        break;
      case 15:
        Script_seqEnterEvtCam(7118, 0, AUTH_None);
        break;
      case 16:
        Script_seqEnterEvtCam(7119, 0, AUTH_None);
        break;
      case 17:
        Script_seqEnterEvtCam(7120, 0, AUTH_None);
        break;
      case 18:
        Script_seqEnterEvtCam(7121, 0, AUTH_None);
        break;
    }
  }
  {
    int32_t local_FC;
    local_FC = arg0;
    if (local_FC == 0) {
    } else if (local_FC == 10) {
      Script_SetCameraFlags(1);
      Script_SetCamFactor(0.15f);
    } else {
      Script_SetCameraFlags(0);
      Script_SetCamFactor(0.0f);
    }
  }
  return;
}

static void script_5972(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 42:
      break;
    case 0:
      Script_ShowUIMessageFromID("E1024A001");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1024A002");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1024A003");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1024A004");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1024A005");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1024A006");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1024A007");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1024A008");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1024A009");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1024A010");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1024A011");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1024A012");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1024A013");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1024A014");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1024A015");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1024A016");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1024A017");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1024A018");
      break;
    case 18:
      Script_ShowUIMessageFromID("E1024A019");
      break;
    case 19:
      Script_ShowUIMessageFromID("E1024A020");
      break;
    case 20:
      Script_ShowUIMessageFromID("E1024A021");
      break;
    case 21:
      Script_ShowUIMessageFromID("E1024A022");
      break;
    case 22:
      Script_ShowUIMessageFromID("E1024A023");
      break;
    case 23:
      Script_ShowUIMessageFromID("E1024A024");
      break;
    case 24:
      Script_ShowUIMessageFromID("E1024A025");
      break;
    case 25:
      Script_ShowUIMessageFromID("E1024A026");
      break;
    case 26:
      Script_ShowUIMessageFromID("E1024A027");
      break;
    case 27:
      Script_ShowUIMessageFromID("E1024A028");
      break;
    case 28:
      Script_ShowUIMessageFromID("E1024A029");
      break;
    case 29:
      Script_ShowUIMessageFromID("E1024A030");
      break;
    case 30:
      Script_ShowUIMessageFromID("E1024A031");
      break;
    case 31:
      Script_ShowUIMessageFromID("E1024B001");
      break;
    case 32:
      Script_ShowUIMessageFromID("E1024B002");
      break;
    case 33:
      Script_ShowUIMessageFromID("E1024B003");
      break;
    case 34:
      Script_ShowUIMessageFromID("E1024B004");
      break;
    case 35:
      Script_ShowUIMessageFromID("E1024B005");
      break;
    case 36:
      Script_ShowUIMessageFromID("E1024B006");
      break;
    case 37:
      Script_ShowUIMessageFromID("E1024B007");
      break;
    case 38:
      Script_ShowUIMessageFromID("E1024B008");
      break;
    case 39:
      Script_ShowUIMessageFromID("E1024B009");
      break;
    case 40:
      Script_ShowUIMessageFromID("E1024B010");
      break;
    case 41:
      Script_ShowUIMessageFromID("E1024B011");
      break;
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_5D01(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      break;
    case 1:
      Script_executeAicaCommand(0x2706A9, 0, 0);
      break;
    case 2:
      Script_executeAicaCommand(0x2806A9, 0, 0);
      break;
    case 3:
      Script_executeAicaCommand(1705, 0, 0);
      break;
    case 4:
      Script_executeAicaCommand(0x2906A9, 0, 0);
      break;
    case 5:
      Script_executeAicaCommand(0x2A06A9, 0, 0);
      break;
    case 6:
      Script_executeAicaCommand(0x2B06A9, 0, 0);
      SCNF_WaitFrames_0000(70);
      Script_executeAicaCommand(0x2B06A9, 0, 0);
      SCNF_WaitFrames_0000(70);
      Script_executeAicaCommand(0x2B06A9, 0, 0);
      SCNF_WaitFrames_0000(70);
      break;
    case 7:
      Script_executeAicaCommand(0x2C06A9, 0, 0);
      break;
    case 8:
      Script_executeAicaCommand(0x2D06A9, 0, 0);
      break;
    case 9:
      Script_executeAicaCommand(0x2E06A9, 0, 0);
      break;
    case 10:
      Script_executeAicaCommand(0x2F06A9, 0, 0);
      break;
    case 11:
      Script_executeAicaCommand(0x3006A9, 0, 0);
      break;
    case 12:
      Script_executeAicaCommand(0x3106A9, 0, 0);
      break;
    case 13:
      Script_executeAicaCommand(0x3206A9, 0, 0);
      break;
    case 14:
      Script_executeAicaCommand(0x3306A9, 0, 0);
      break;
    case 15:
      Script_executeAicaCommand(0x3706A9, 0, 0);
      break;
    case 16:
      Script_executeAicaCommand(0x3806A9, 0, 0);
      break;
    case 17:
      Script_executeAicaCommand(0x3406A9, 0, 0);
      break;
    case 18:
      Script_executeAicaCommand(0x3506A9, 0, 0);
      break;
    case 19:
      Script_executeAicaCommand(0x3606A9, 0, 0);
      break;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_112CC, 2, data_112E3)))
static void script_5F21(void) {
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "SLOT_P") == 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/80/GSEN/", "SLOT_P");
    while (1) {
      if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_uiUtils(0, 30, 0);
  (void)((task_8 = 0) & 0x40);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 1, 1, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 754, 0x42700000, 0x42820000, 0x42700000, 0, 2.0f, 0);
  Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 755, 0x41C80000, 0x41F00000, 0x41C80000, 0, 2.5f, 0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_112BC);
  Script_charaSetRefboardFlagTo(SHEN_NAME('SLTM'), 1);
  return;
}

__attribute__((scn_string_bindings(0, data_112EA)))
static void script_6021(void) {
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 0, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "SLOT_P");
  (void)((task_8 = -1) & 0x40);
  Script_uiUtils(0, 0, 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('SLTM'), 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 0);
  Script_charaDisplay(SHEN_NAME('C50D'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C50P'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C20R'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C20L'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0C'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0R'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0L'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C01M'), CDM_Hide);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  return;
}

static void script_60DD(void) {
  Script_SetCameraFlags(1);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_611E(void) {
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_6133(void) {
  script_4CF6(1, 0);
  script_4CF6(2, 0);
  script_4CF6(3, 0);
  script_4CF6(4, 0);
  script_4CF6(5, 0);
  script_4CF6(15, 0);
  script_4CF6(27, 0);
  script_4CF6(26, 0);
  script_4CF6(33, 0);
  script_4CF6(34, 0);
  script_4CF6(35, 0);
  script_4CF6(36, 0);
  script_4CF6(37, 0);
  script_4CF6(20, -255);
  script_4CF6(21, -255);
  script_4CF6(22, -255);
  script_4CF6(23, 0);
  script_4CF6(24, 0);
  script_4CF6(25, 0);
  script_4CF6(11, 0);
  script_4CF6(10, 2);
  script_4CF6(7, 2);
  return;
}

static void script_622B(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (local_FC < 42) {
    script_5972(local_FC);
    local_F8 = 0;
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
      local_F8 = local_F8 + 1;
    }
    Script_sprintf("VOICE_NO %03d  frame %d\012", local_FC + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_6286(void) {
  int32_t local_FC;
  while (1) {
    Script_PrintDebugString(5, 23, "COIN:%4d", script_4D06(42));
    Script_PrintDebugString(15, 23, "BET :%4d", script_4D06(15));
    Script_PrintDebugString(25, 23, "WIN :%4d", script_4D06(26));
    Script_PrintDebugString(5, 24, "CLDT:%4d", script_4D06(13));
    local_FC = script_4D06(7);
    switch (local_FC) {
      case 1:
        Script_PrintDebugString(15, 24, "HAND: PAY OUT");
        break;
      case 2:
        Script_PrintDebugString(15, 24, "HAND: BET    ");
        break;
      case 3:
        Script_PrintDebugString(15, 24, "HAND: START_B");
        break;
      case 4:
        Script_PrintDebugString(15, 24, "HAND: START_R");
        break;
    }
    if (script_4D06(5) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_6382(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_EC[3];
  local_FC = Script_shenGetStats(STATS_CASH);
  local_F8 = Script_shenGetStats(STATS_SEGA_COINS);
  Script_sprintf("coin  : %9d \012", local_F8);
  Script_sprintf("monay : %5d \012", local_FC);
  script_4CF6(41, local_FC);
  if (local_FC >= 100) {
    script_4CF6(8, 1);
  }
  script_6F22();
  if (script_4D06(0) == 1) {
    task_26C = 0;
  } else {
    task_26C = local_F8;
    local_F8 = 0;
  }
  if (local_F8 > 99999) {
    local_F8 = 99999;
  }
  if (local_F8 < 0) {
    local_F8 = 0;
  }
  script_4CF6(42, local_F8);
  script_4CF6(13, local_F8);
  if (local_F8 >= 1) {
    script_4CF6(19, 1);
  } else {
    script_4CF6(19, 0);
  }
  return;
}

static void script_6459(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_EC[3];
  local_FC = script_4D06(41);
  local_F8 = script_4D06(42);
  script_6F22();
  if (script_4D06(0) == 1) {
    local_F8 = task_26C + local_F8;
  } else {
    local_F8 = task_26C;
  }
  Script_SetStats(STATS_CASH, local_FC);
  Script_SetStats(STATS_SEGA_COINS, local_F8);
  script_4CF6(13, 0);
  return;
}

static int32_t script_64BD(void) {
  int32_t local_FC;
  int16_t local_FA;
  int16_t local_F8;
  int16_t local_F6;
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
  int32_t local_FF6C;
  task_234.x = 2730;
  task_234.y = 0;
  task_234.z = 0;
  task_240.x = 2730;
  task_240.y = 0;
  task_240.z = 0;
  task_24C.x = 2730;
  task_24C.y = 0;
  task_24C.z = 0;
  Script_charaPosRotUtils(SHEN_NAME('DRAM'), 153, 0, &task_234, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('DRAM'), 154, 0, &task_240, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('DRAM'), 155, 0, &task_24C, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 13, &local_D0, 0, CHARA_PART_Get);
  local_DC.x = (&local_D0)->x;
  (&local_DC)->y = (&local_D0)->y + 0.025f;
  (&local_DC)->z = (&local_D0)->z;
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 13, &local_DC, 0, CHARA_PART_Set);
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = -1.5f;
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 182, &local_C4, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 183, &local_B8, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 184, &local_AC, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 185, &local_A0, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 186, &local_94, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 192, &local_88, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 193, &local_FF7C, 0, CHARA_PART_Get);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 194, &local_FF70, 0, CHARA_PART_Get);
  Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 0);
  Script_SetCharaRefBoardPos(SHEN_NAME('DRAM'), &data_113B4);
  local_F6 = 0;
  while (1) {
    local_FC = script_4D06(13);
    local_FA = script_4D06(15);
    local_F8 = script_4D06(17);
    Script_GameUtils(SHEN_NAME('SLTM'), GC_CommitStagedEntries, 0, 0, 0, 0);
    Script_GameUtils(SHEN_NAME('SLTM'), GC_ConfigureNumericDisplay, 0x100, &data_11384, 0x5F5E0FF, 0);
    Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNumericDisplayValue, local_FC, &data_11390, 1.0f, 0);
    Script_GameUtils(SHEN_NAME('SLTM'), GC_ConfigureNumericDisplay, 0x100, &data_11384, 999999, 0);
    Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNumericDisplayValue, local_F8, &data_113A8, 1.0f, 0);
    local_FF6C = local_FA;
    switch (local_FF6C) {
      case 0:
        Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 292, &data_1139C, 1.0f, 0);
        script_4CF6(40, 0);
        script_4CF6(39, 1);
        script_4CF6(38, 1);
        break;
      case 1:
        Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 293, &data_1139C, 1.0f, 0);
        script_4CF6(38, 0);
        script_4CF6(40, 1);
        script_4CF6(33, 1);
        break;
      case 2:
        Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 294, &data_1139C, 1.0f, 0);
        script_4CF6(34, 1);
        break;
      case 3:
        Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 295, &data_1139C, 1.0f, 0);
        script_4CF6(35, 1);
        break;
      case 4:
        Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 296, &data_1139C, 1.0f, 0);
        script_4CF6(36, 1);
        break;
      case 5:
        Script_GameUtils(SHEN_NAME('SLTM'), GC_StageNodeData, 297, &data_1139C, 1.0f, 0);
        script_4CF6(37, 1);
        script_4CF6(39, 0);
        break;
    }
    if (local_FC == 0) {
      script_4CF6(39, 0);
      script_4CF6(38, 0);
    }
    if (local_FA > 0) {
      Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 1);
    } else {
      Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 0);
    }
    if (script_4D06(10) == 0) {
      script_4CF6(40, 0);
      script_4CF6(39, 0);
      Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 1);
    }
    if (script_4D06(10) == 1) {
      script_4CF6(17, script_4D06(26));
      if (local_F6 < 15) {
        script_4CF6(33, script_4D06(28));
        script_4CF6(34, script_4D06(29));
        script_4CF6(35, script_4D06(30));
        script_4CF6(36, script_4D06(31));
        script_4CF6(37, script_4D06(32));
        Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 1);
      } else {
        script_4CF6(33, 0);
        script_4CF6(34, 0);
        script_4CF6(35, 0);
        script_4CF6(36, 0);
        script_4CF6(37, 0);
        Script_charaSetRefboardFlagTo(SHEN_NAME('DRAM'), 0);
      }
    } else {
      local_F6 = 0;
    }
    if (script_4D06(33) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 182, &local_E8, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 182, &local_C4, 0, CHARA_PART_Set);
    }
    if (script_4D06(34) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 183, &local_E8, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 183, &local_B8, 0, CHARA_PART_Set);
    }
    if (script_4D06(35) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 184, &local_E8, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 184, &local_AC, 0, CHARA_PART_Set);
    }
    if (script_4D06(36) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 185, &local_E8, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 185, &local_A0, 0, CHARA_PART_Set);
    }
    if (script_4D06(37) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 186, &local_E8, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 186, &local_94, 0, CHARA_PART_Set);
    }
    if (script_4D06(38) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 192, &local_E8, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 192, &local_88, 0, CHARA_PART_Set);
    }
    if (script_4D06(39) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 193, &local_E8, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 193, &local_FF7C, 0, CHARA_PART_Set);
    }
    if (script_4D06(40) == 0) {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 194, &local_E8, 0, CHARA_PART_Set);
    } else {
      Script_charaPosRotUtils(SHEN_NAME('SLTM'), 194, &local_FF70, 0, CHARA_PART_Set);
    }
    if (script_4D06(5) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
    if (local_F6 > 30) {
      local_F6 = 0;
    } else {
      local_F6 = local_F6 + 1;
    }
  }
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 182, &local_C4, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 183, &local_B8, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 184, &local_AC, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 185, &local_A0, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 186, &local_94, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 192, &local_88, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 193, &local_FF7C, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 194, &local_FF70, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('SLTM'), 13, &local_D0, 0, CHARA_PART_Set);
  return Script_srchControl(0);
}

static void script_6CAB(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = script_4D06(41);
  local_FC = script_4D06(0);
  local_F4 = local_F8 + local_FC;
  SCNF_SetCashValue(local_FC, 1);
  script_4CF6(41, local_F4);
  if (local_F4 >= 100) {
    script_4CF6(8, 1);
  } else {
    script_4CF6(8, 0);
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_6D12(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t unused_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  local_FC = script_4D06(13);
  local_F8 = script_4D06(14);
  local_F4 = script_4D06(18);
  local_F0 = local_FC + local_F4;
  if (local_F4 >= 0) {
    local_E4 = local_F8 + local_FC + local_F4;
    if (local_E4 > 99999) {
      local_F4 = 99999 - (local_F8 + local_FC);
      local_D8 = 99999 - local_F8;
      local_DC = local_F8;
    } else if (local_F0 > 99999) {
      local_D8 = 99999;
      local_DC = local_F8;
    } else {
      local_D8 = local_F0;
      local_DC = local_F8;
    }
    local_E8 = local_F4;
    while (local_FC < local_D8) {
      local_FC = local_FC + 1;
      local_F4 = local_F4 - 1;
      script_4CF6(13, local_FC);
      if (local_E8 <= 50) {
        SCNF_WaitFrames_0000(1);
      }
      local_E8 = local_E8 - 1;
    }
  } else {
    if (local_F0 < 0) {
      local_D8 = 0;
      local_DC = local_F8 + local_FC;
    } else {
      local_D8 = local_F0;
      local_DC = local_F8 - local_F4;
    }
    local_E8 = 0;
    while (local_FC > local_D8) {
      local_FC = local_FC - 1;
      local_F4 = local_F4 + 1;
      script_4CF6(13, local_FC);
      if (local_FC <= 50) {
        SCNF_WaitFrames_0000(1);
      }
      local_E8 = local_E8 + 1;
    }
    if (local_E8 < 50) {
      SCNF_WaitFrames_0000(100 - local_E8);
    } else {
      SCNF_WaitFrames_0000(50);
    }
  }
  local_E0 = local_D8 + local_DC;
  script_4CF6(42, local_E0);
  script_4CF6(14, local_DC);
  script_4CF6(18, local_F4);
  if (local_E0 >= 1) {
    script_4CF6(19, 1);
  } else {
    script_4CF6(19, 0);
  }
  local_D4 = script_4D06(41);
  local_D0 = script_4D06(42);
  Script_SetStats(STATS_CASH, local_D4);
  Script_SetStats(STATS_SEGA_COINS, local_D0);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_6F15(void) {
  (void)((task_8 = 0) & 4);
  return;
}

static void script_6F22(void) {
  int8_t local_FF;
  local_FF = Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E);
  if (local_FF == 0) {
    script_4CF6(0, 0);
  } else {
    script_4CF6(0, 1);
  }
  return;
}

static int32_t script_6F58(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3i local_AC;
  Vector3i local_A0;
  Vector3i local_94;
  int16_t local_92;
  local_F4.x = 4.6414f;
  (&local_F4)->y = 1.56f;
  (&local_F4)->z = -0.6147f;
  local_E8.x = 4.6414f;
  (&local_E8)->y = 1.64f;
  (&local_E8)->z = -0.6147f;
  local_DC.x = 4.6414f;
  (&local_DC)->y = 1.72f;
  (&local_DC)->z = -0.6147f;
  Script_lightResetSlotToDefaults(10);
  Script_lightResetSlotToDefaults(11);
  Script_lightResetSlotToDefaults(12);
  Script_LightUtils(10, 0, 4);
  Script_LightUtils(10, 2, &data_113C4);
  Script_LightUtils(10, 4, 5.0f);
  Script_LightUtils(10, 5, 3.0f);
  Script_LightUtils(10, 6, data_113F4);
  Script_LightUtils(10, 7, &local_F4);
  Script_LightUtils(10, 10, 5461);
  Script_LightUtils(11, 0, 4);
  Script_LightUtils(11, 2, &data_113D4);
  Script_LightUtils(11, 4, 5.0f);
  Script_LightUtils(11, 5, 3.0f);
  Script_LightUtils(11, 6, data_113F4);
  Script_LightUtils(11, 7, &local_E8);
  Script_LightUtils(11, 10, 5461);
  Script_LightUtils(12, 0, 4);
  Script_LightUtils(12, 2, &data_113E4);
  Script_LightUtils(12, 4, 5.0f);
  Script_LightUtils(12, 5, 3.0f);
  Script_LightUtils(12, 6, data_113F4);
  Script_LightUtils(12, 7, &local_DC);
  Script_LightUtils(12, 10, 5461);
  Script_lightSetSlotEnabled(10, 1);
  Script_lightSetSlotEnabled(11, 1);
  Script_lightSetSlotEnabled(12, 1);
  local_D0.x = 1.0f;
  (&local_D0)->y = 0.0f;
  (&local_D0)->z = -1.0f;
  local_C4.x = -1.0f;
  (&local_C4)->y = 0.0f;
  (&local_C4)->z = 1.0f;
  local_B8.x = 1.0f;
  (&local_B8)->y = 0.0f;
  (&local_B8)->z = 1.0f;
  Script_charaDisplay(SHEN_NAME('RMP1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMP2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMP3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMPY'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RMPR'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RMPG'), CDM_Show);
  local_92 = 0;
  while (1) {
    Script_mtwkGetSetRotation(SHEN_NAME('RMPY'), &local_AC, 0x40000000);
    Script_mtwkGetSetRotation(SHEN_NAME('RMPR'), &local_A0, 0x40000000);
    Script_mtwkGetSetRotation(SHEN_NAME('RMPG'), &local_94, 0x40000000);
    (&local_AC)->y = (&local_AC)->y + 4369;
    (&local_A0)->y = (&local_A0)->y + 4369;
    (&local_94)->y = (&local_94)->y + 4369;
    Script_EVT_SetRotation(SHEN_NAME('RMPY'), 0x38000000, &local_AC, 0);
    Script_EVT_SetRotation(SHEN_NAME('RMPR'), 0x38000000, &local_A0, 0);
    Script_EVT_SetRotation(SHEN_NAME('RMPG'), 0x38000000, &local_94, 0);
    if ((local_92 >= 0) & (local_92 <= 9)) {
      local_D0.x = (&local_D0)->x - 0.2f;
      local_C4.x = (&local_C4)->x + 0.2f;
      (&local_B8)->z = (&local_B8)->z - 0.2f;
    }
    if ((local_92 >= 10) & (local_92 <= 19)) {
      (&local_D0)->z = (&local_D0)->z + 0.2f;
      (&local_C4)->z = (&local_C4)->z - 0.2f;
      local_B8.x = (&local_B8)->x - 0.2f;
    }
    if ((local_92 >= 20) & (local_92 <= 29)) {
      local_D0.x = (&local_D0)->x + 0.2f;
      local_C4.x = (&local_C4)->x - 0.2f;
      (&local_B8)->z = (&local_B8)->z + 0.2f;
    }
    if ((local_92 >= 30) & (local_92 <= 39)) {
      (&local_D0)->z = (&local_D0)->z - 0.2f;
      (&local_C4)->z = (&local_C4)->z + 0.2f;
      local_B8.x = (&local_B8)->x + 0.2f;
    }
    Script_LightUtils(10, 8, &local_D0);
    Script_LightUtils(11, 8, &local_C4);
    Script_LightUtils(12, 8, &local_B8);
    local_92 = local_92 + 1;
    if (local_92 == 40) {
      local_92 = 0;
    }
    if (script_4D06(10) == 2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetRotation(SHEN_NAME('RMP1'), 0x38000000, &local_AC, 0);
  Script_EVT_SetRotation(SHEN_NAME('RMP2'), 0x38000000, &local_A0, 0);
  Script_EVT_SetRotation(SHEN_NAME('RMP3'), 0x38000000, &local_94, 0);
  Script_lightSetSlotEnabled(10, 0);
  Script_lightSetSlotEnabled(11, 0);
  Script_lightSetSlotEnabled(12, 0);
  Script_charaDisplay(SHEN_NAME('RMPY'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMPR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMPG'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RMP1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RMP2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RMP3'), CDM_Show);
  Script_sprintf("消えるよ1 \012");
  return Script_srchControl(0);
}

static void script_7412(void) {
  int16_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  int8_t local_FB;
  local_FE = 0;
  local_FD = 0;
  local_FC = 0;
  local_FB = 0;
  while (local_FE < 200) {
    if (Script_GetControllerInput(1) & 4) {
      script_4CF6(23, 1);
      local_FD = 1;
    }
    if (Script_GetControllerInput(1) & 0x400) {
      script_4CF6(24, 1);
      local_FC = 1;
    }
    if (Script_GetControllerInput(1) & 0x200) {
      script_4CF6(25, 1);
      local_FB = 1;
    }
    if (-(local_FD == 1) & -(local_FC == 1) & -(local_FB == 1)) {
      break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_4CF6(23, 1);
  script_4CF6(24, 1);
  script_4CF6(25, 1);
  script_4CF6(10, 1);
  Script_executeAicaCommand(0x2D06A9, 0, 0);
  Script_executeAicaCommand(0x3806A9, 0, 0);
  Script_srchControl(0);
  return;
}

static void script_750E(void) {
  int8_t local_FF;
  (void)((task_8 = 0) & 2);
  script_4CF6(10, 0);
  Script_scenCreateTask(script_75BC, 0);
  Script_scenCreateTask(script_766B, 0);
  Script_scenCreateTask(script_771A, 0);
  SCNF_WaitFrames_0000(1);
  Script_executeAicaCommand(0x2C06A9, 0, 0);
  Script_scenCreateTask(script_77EE, 0);
  local_FF = 0;
  while (local_FF < 15) {
    SCNF_WaitFrames_0000(1);
    local_FF = local_FF + 1;
  }
  script_85AF();
  script_5782(13);
  while (script_4D06(10) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_75BC(void) {
  int16_t local_FE;
  int16_t local_FC;
  int16_t local_FA;
  local_FA = 0;
  while (1) {
    task_234.x = (&task_234)->x + 2730;
    local_FA = local_FA + 1;
    if (local_FA == 2) {
      local_FA = 0;
    }
    Script_charaPosRotUtils(SHEN_NAME('DRAM'), 153, 0, &task_234, CHARA_PART_Set);
    if ((&task_234)->x == 68250) {
      task_234.x = 2730;
    }
    local_FE = script_4D06(20);
    local_FC = local_FE * 30 + 15;
    if ((local_FE != -255) & (local_FC == (&task_234)->x / 182)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2E06A9, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_766B(void) {
  int16_t local_FE;
  int16_t local_FC;
  int16_t local_FA;
  local_FA = 0;
  while (1) {
    task_240.x = (&task_240)->x + 2730;
    local_FA = local_FA + 1;
    if (local_FA == 2) {
      local_FA = 0;
    }
    Script_charaPosRotUtils(SHEN_NAME('DRAM'), 154, 0, &task_240, CHARA_PART_Set);
    if ((&task_240)->x == 68250) {
      task_240.x = 2730;
    }
    local_FE = script_4D06(21);
    local_FC = local_FE * 30 + 15;
    if ((local_FE != -255) & (local_FC == (&task_240)->x / 182)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2E06A9, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_771A(void) {
  int16_t local_FE;
  int16_t local_FC;
  int16_t local_FA;
  local_FA = 0;
  while (1) {
    task_24C.x = (&task_24C)->x + 2730;
    local_FA = local_FA + 1;
    if (local_FA == 2) {
      local_FA = 0;
    }
    Script_charaPosRotUtils(SHEN_NAME('DRAM'), 155, 0, &task_24C, CHARA_PART_Set);
    if ((&task_24C)->x == 68250) {
      task_24C.x = 2730;
    }
    local_FE = script_4D06(22);
    local_FC = local_FE * 30 + 15;
    if ((local_FE != -255) & (local_FC == (&task_24C)->x / 182)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_4CF6(10, 1);
  Script_executeAicaCommand(0x2D06A9, 0, 0);
  Script_executeAicaCommand(0x3806A9, 0, 0);
  Script_executeAicaCommand(0x2E06A9, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_77EE(void) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int8_t local_F9;
  int8_t local_F8;
  int8_t local_F7;
  int8_t local_F6;
  int8_t local_F5;
  int8_t local_F4;
  int16_t local_F2;
  int16_t local_F0;
  int16_t local_EE;
  int16_t local_EC;
  int16_t local_EA;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 12.0f);
  local_FB = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 12.0f);
  local_FA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 12.0f);
  local_F9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 12.0f);
  local_F8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 12.0f);
  local_F7 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 12.0f);
  local_F6 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 12.0f);
  local_F5 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 12.0f);
  local_F4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 12.0f);
  script_79FB(local_FC, local_FB, local_FA);
  local_EE = script_4D06(26);
  script_79FB(local_F9, local_F8, local_F7);
  local_EC = script_4D06(26);
  script_79FB(local_F6, local_F5, local_F4);
  local_EA = script_4D06(26);
  if (local_EE > local_EC) {
    if (local_EC > local_EA) {
      local_FF = local_F6;
      local_FE = local_F5;
      local_FD = local_F4;
    } else {
      local_FF = local_F9;
      local_FE = local_F8;
      local_FD = local_F7;
    }
  } else if (local_EE > local_EA) {
    local_FF = local_F6;
    local_FE = local_F5;
    local_FD = local_F4;
  } else {
    local_FF = local_FC;
    local_FE = local_FB;
    local_FD = local_FA;
  }
  local_F0 = script_4D06(6);
  if (local_F0 == 20) {
    local_FF = local_FC;
    local_FE = local_FB;
    local_FD = local_FA;
    script_4CF6(6, 0);
  }
  script_79FB(local_FF, local_FE, local_FD);
  Script_sprintf(" ---------- PLY_C: %2d \012", local_F0);
  local_F2 = 0;
  while (local_F2 < 165) {
    if (local_F2 == 100) {
      script_4CF6(20, local_FF);
    }
    if (local_F2 == 130) {
      script_4CF6(21, local_FE);
    }
    if (local_F2 == 160) {
      script_4CF6(22, local_FD);
    }
    local_F2 = local_F2 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_79FB(int8_t arg0, int8_t arg1, int8_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int16_t local_F6;
  int8_t local_F5;
  int8_t local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int8_t local_F1;
  int32_t local_EC;
  Script_sprintf(" d1:%2d  d2:%2d  d3:%2d ", arg0, arg1, arg2);
  local_FC = 0;
  local_F2 = 1;
  local_F5 = 1;
  local_F4 = 1;
  local_F3 = 1;
  script_4CF6(28, 0);
  script_4CF6(29, 0);
  script_4CF6(30, 0);
  script_4CF6(31, 0);
  script_4CF6(32, 0);
  while (local_F2 <= script_4D06(15)) {
    local_F8 = data_FB9C[arg0 + local_F5] * 100 + data_FBAC[arg1 + local_F4] * 10 + data_FBBC[arg2 + local_F3];
    local_F1 = 1;
    local_F6 = 0;
    while (local_F1 < 17) {
      if (local_F8 == *(int16_t *)((int8_t *)&data_FAB4 + (local_F1 << 1))) {
        local_F6 = local_F1;
      }
      local_F1 = local_F1 + 1;
    }
    if ((local_F8 > 120) & (local_F8 < 190)) {
      local_F6 = 18;
    }
    if ((local_F8 > 111) & (local_F8 < 119)) {
      local_F6 = 19;
    }
    if ((local_F8 == 555) & (local_F2 == 1)) {
      local_F6 = 20;
    }
    if ((local_F8 == 555) & (local_F2 == 2)) {
      local_F6 = 21;
    }
    if ((local_F8 == 555) & (local_F2 == 3)) {
      local_F6 = 22;
    }
    if ((local_F8 == 555) & (local_F2 == 4)) {
      local_F6 = 23;
    }
    if ((local_F8 == 555) & (local_F2 == 5)) {
      local_F6 = 24;
    }
    local_EC = local_F2;
    switch (local_EC) {
      case 1:
        script_4CF6(28, *(int16_t *)((int8_t *)&data_FAD8 + (local_F6 << 1)));
        local_F5 = 2;
        local_F4 = 2;
        local_F3 = 2;
        break;
      case 2:
        script_4CF6(29, *(int16_t *)((int8_t *)&data_FAD8 + (local_F6 << 1)));
        local_F5 = 0;
        local_F4 = 0;
        local_F3 = 0;
        break;
      case 3:
        script_4CF6(30, *(int16_t *)((int8_t *)&data_FAD8 + (local_F6 << 1)));
        local_F5 = 2;
        local_F4 = 1;
        local_F3 = 0;
        break;
      case 4:
        script_4CF6(31, *(int16_t *)((int8_t *)&data_FAD8 + (local_F6 << 1)));
        local_F5 = 0;
        local_F4 = 1;
        local_F3 = 2;
        break;
      case 5:
        script_4CF6(32, *(int16_t *)((int8_t *)&data_FAD8 + (local_F6 << 1)));
        break;
    }
    local_FC = local_FC + *(int16_t *)((int8_t *)&data_FAD8 + (local_F6 << 1));
    local_F2 = local_F2 + 1;
  }
  script_4CF6(26, local_FC);
  script_4CF6(18, local_FC);
  if (local_FC == 0) {
    script_4CF6(27, 0);
  }
  if (local_FC >= 2) {
    script_4CF6(27, 1);
  }
  if (local_FC >= 300) {
    script_4CF6(27, 2);
  }
  Script_sprintf(" RETURN: %4d \012", script_4D06(26));
  return;
}

static void script_7CEE(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 4.25f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -1.2f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 38775;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_7D54(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 4.19f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -1.2f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 38775;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_7DBA(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 4.3f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -1.26f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 38229;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_7E20(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 4.48f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = -0.935f;
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
  local_E8.x = 0xC000;
  (&local_E8)->y = 9102;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_E8, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_7E8A(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 4.354f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = -0.815f;
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
  local_E8.x = 0xC000;
  (&local_E8)->y = 5461;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_E8, 0);
  Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 755, 0x41D00000, 0x41F00000, 0x41D00000, 0, 2.0f, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_7F1B(void) {
  {
    Vector3 local_F4;
    local_F4.x = 0.0f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.0f;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_E8, 0);
  }
  {
    Vector3 local_F4;
    local_F4.x = 4.25f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -1.3f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 38775;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  return;
}

static int32_t script_7FCB(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(SHEN_NAME('SLTM'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
  return Script_srchControl(0);
}

static int32_t script_8004(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_NITO, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 2, &local_F4, 2);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 10);
  return Script_srchControl(0);
}

static int32_t script_803E(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(SHEN_NAME('TITL'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, 0, 0.0f, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_806E(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(CHARA_NITO, 1, &local_F4, 0);
  return Script_srchControl(0);
}

static int32_t script_8098(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(SHEN_NAME('SLTM'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(CHARA_NITO, -98312, 0, 0);
  end:;
}

static void script_80BF(void) {
  SCNF_WaitFrames_0000(1);
  SCNT_ShowSubtitle("スロットゲームで遊びますか？");
  return;
}

static void script_80CF(void) {
  SCNF_WaitFrames_0000(1);
  SCNT_ShowSubtitle("スロットゲームをやめますか？");
  return;
}

static void script_80DF(void) {
  SCNF_WaitFrames_0000(1);
  SCNT_ShowSubtitle("スロットゲームを続けますか？");
  return;
}

static void script_80EF(void) {
  int32_t local_FC;
  local_FC = script_4D06(42);
  Script_SetStats(STATS_SEGA_COINS, local_FC);
  SCNF_SetCashValue(0, 0);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_114A0, 1, data_114A5, 2, data_114AA)))
static int32_t script_811A(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3i local_DC;
  int32_t unused_D0[3];
  int32_t local_CC;
  int32_t unused_C4[2];
  int32_t local_C0;
  Script_charaDisplay(SHEN_NAME('C50D'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C50P'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C20R'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C20L'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0C'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0R'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CA0L'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('C01M'), CDM_Hide);
  local_C0 = script_4D06(14);
  local_CC = 0;
  while (local_CC < 50) {
    Script_InterpolateAndScaleAngles(local_CC, 2, &local_F4, &local_DC, "C50S");
    local_F4.x = (&local_F4)->x * (float)10;
    (&local_F4)->y = (&local_F4)->y * (float)10;
    (&local_F4)->z = (&local_F4)->z * (float)10;
    Script_charaPosRotUtils(SHEN_NAME('C50P'), *(&data_FBCC + local_CC), &local_F4, &local_DC, CHARA_PART_Set);
    local_CC = local_CC + 1;
  }
  SCNF_WaitFrames_0000(1);
  if ((local_C0 > 0) & (local_C0 <= 75)) {
    local_CC = 0;
    while (local_CC < local_C0) {
      Script_InterpolateAndScaleAngles(local_CC, 160, &local_F4, &local_DC, "C50S");
      local_F4.x = (&local_F4)->x * (float)10;
      (&local_F4)->y = (&local_F4)->y * (float)10;
      (&local_F4)->z = (&local_F4)->z * (float)10;
      Script_charaPosRotUtils(SHEN_NAME('C50P'), *(&data_FBCC + local_CC), &local_F4, &local_DC, CHARA_PART_Set);
      local_CC = local_CC + 1;
      if (local_CC == 50) {
        local_CC = 75;
      }
    }
    Script_charaDisplay(SHEN_NAME('C50P'), CDM_Show);
  }
  if ((local_C0 > 75) & (local_C0 <= 150)) {
    local_CC = 0;
    while (local_CC + 50 < local_C0) {
      Script_InterpolateAndScaleAngles(local_CC, 160, &local_F4, &local_DC, "C50S");
      local_F4.x = (&local_F4)->x * (float)10;
      (&local_F4)->y = (&local_F4)->y * (float)10;
      (&local_F4)->z = (&local_F4)->z * (float)10;
      Script_charaPosRotUtils(SHEN_NAME('C50P'), *(&data_FBCC + local_CC), &local_F4, &local_DC, CHARA_PART_Set);
      local_CC = local_CC + 1;
      if (local_CC == 50) {
        local_CC = 100;
      }
    }
    Script_charaDisplay(SHEN_NAME('C20R'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('C20L'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('C50P'), CDM_Show);
  }
  if ((local_C0 > 150) & (local_C0 <= 300)) {
    Script_charaDisplay(SHEN_NAME('C20R'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('C20L'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('CA0C'), CDM_Show);
  }
  if (local_C0 > 300) {
    Script_charaDisplay(SHEN_NAME('CA0R'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('CA0L'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('CA0C'), CDM_Show);
  }
  SCNF_WaitFrames_0000(1);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_114AF, 1, data_114B4)))
static int32_t script_83C6(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3i local_DC;
  int32_t unused_D0[3];
  int16_t local_CE;
  int16_t local_CC;
  int32_t local_C8;
  int32_t local_C4;
  int32_t local_C0;
  int32_t local_BC;
  local_C4 = script_4D06(13);
  local_C8 = local_C4 * -1;
  script_4CF6(18, local_C8);
  local_C0 = script_4D06(14);
  if (local_C4 <= 50) {
    local_BC = local_C0;
  } else {
    local_BC = local_C0 + local_C4 - 50;
  }
  script_4CF6(14, local_BC);
  script_51C6(18);
  SCNF_WaitFrames_0000(5);
  script_4CF6(14, local_C0);
  local_CE = 0;
  while (local_CE < 50) {
    Script_InterpolateAndScaleAngles(local_CE, 2, &local_F4, &local_DC, "C50R");
    local_F4.x = (&local_F4)->x * (float)10;
    (&local_F4)->y = (&local_F4)->y * (float)10;
    (&local_F4)->z = (&local_F4)->z * (float)10;
    Script_charaPosRotUtils(SHEN_NAME('C50D'), *(&data_FBCC + local_CE), &local_F4, &local_DC, CHARA_PART_Set);
    local_CE = local_CE + 1;
  }
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(SHEN_NAME('C50D'), CDM_Show);
  Script_scenCreateTask(script_4D35, 1, 17);
  local_CC = 2;
  while (local_CC < 160) {
    local_CE = 0;
    while (local_CE < 50) {
      Script_InterpolateAndScaleAngles(local_CE, local_CC, &local_F4, &local_DC, "C50R");
      local_F4.x = (&local_F4)->x * (float)10;
      (&local_F4)->y = (&local_F4)->y * (float)10;
      (&local_F4)->z = (&local_F4)->z * (float)10;
      Script_charaPosRotUtils(SHEN_NAME('C50D'), *(&data_FBCC + local_CE), &local_F4, &local_DC, CHARA_PART_Set);
      if (local_C4 - 1 > local_CE) {
        local_CE = local_CE + 1;
      } else {
        local_CE = 50;
      }
    }
    if (local_C4 * 2 + 30 > local_CC) {
      local_CC = local_CC + 2;
    } else {
      local_CC = 160;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_4D35, 1, 18);
  return Script_srchControl(0);
}

static void script_85AF(void) {
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  return;
}

static void script_85D4(void) {
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  return;
}

static void script_85F9(void) {
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  return;
}

static void script_862A(void) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3i local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int32_t local_D4;
  Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(SHEN_NAME('YKUR'), &local_DC, 0x40000000);
  local_DB = script_4D06(7);
  local_D4 = local_DB;
  switch (local_D4) {
    case 1:
      local_E8[0] = 0.0067f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.004f;
      break;
    case 2:
      local_E8[0] = 0.0067f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0035f;
      break;
    case 3:
      local_E8[0] = 0.0067f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.004f;
      break;
    case 4:
      local_E8[0] = 0.0067f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.008f;
      break;
    case 5:
      local_E8[0] = 0.0067f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.004f;
      Script_scenCreateTask(script_811A, 0);
      script_7D54();
      script_85F9();
      script_5782(11);
      script_54E2(0, 7, 0);
      SCNF_WaitFrames_0000(5);
      Script_SetNewHandMotion(CHARA_AKIR, 1, 762, 0, 0, 0, 0, 1.0f, 0);
      SCNF_WaitFrames_0000(12);
      break;
  }
  local_DA = 0;
  while (local_DA < 10) {
    local_F4.x = (&local_F4)->x - *local_E8;
    (&local_F4)->y = (&local_F4)->y - local_E8[1];
    (&local_F4)->z = (&local_F4)->z + local_E8[2];
    (&local_DC)->y = (&local_DC)->y - 182;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_DC, 0);
    SCNF_WaitFrames_0000(1);
    local_DA = local_DA + 1;
  }
  return;
}

static void script_8817(void) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3i local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int32_t local_D4;
  Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(SHEN_NAME('YKUR'), &local_DC, 0x40000000);
  local_DB = script_4D06(7);
  local_D4 = local_DB;
  switch (local_D4) {
    case 1:
      local_E8[0] = 0.0067f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.0035f;
      break;
    case 2:
      local_E8[0] = 0.0067f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.004f;
      break;
    case 3:
      local_E8[0] = 0.0067f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.008f;
      break;
    case 4:
      local_E8[0] = 0.0067f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.004f;
      script_54E2(0, 8, 0);
      Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 755, 0x41C80000, 0x41F00000, 0x41C80000, 0, 2.5f, 0);
      SCNF_WaitFrames_0000(20);
      script_85D4();
      script_5782(10);
      break;
    case 5:
      local_E8[0] = 0.0067f;
      local_E8[1] = 0.0f;
      local_E8[2] = 0.004f;
      break;
  }
  local_DA = 0;
  while (local_DA < 10) {
    local_F4.x = (&local_F4)->x + *local_E8;
    (&local_F4)->y = (&local_F4)->y - local_E8[1];
    (&local_F4)->z = (&local_F4)->z - local_E8[2];
    (&local_DC)->y = (&local_DC)->y + 182;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_DC, 0);
    SCNF_WaitFrames_0000(1);
    local_DA = local_DA + 1;
  }
  return;
}

static int32_t script_89F5(void) {
  Vector3i local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  Script_mtwkGetSetRotation(SHEN_NAME('YKUR'), &local_F4, 0);
  Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_E8, 0);
  local_E7 = 0;
  while (local_E7 < 10) {
    if (local_E7 < 5) {
      (&local_F4)->z = (&local_F4)->z - 182;
      (&local_E8)->y = (&local_E8)->y - 0.002f;
    }
    if (local_E7 > 4) {
      (&local_F4)->z = (&local_F4)->z + 182;
      (&local_E8)->y = (&local_E8)->y + 0.002f;
    }
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_F4, 0);
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_E8);
    SCNF_WaitFrames_0000(1);
    local_E7 = local_E7 + 1;
  }
  return Script_srchControl(0);
}

static int32_t script_8AB9(void) {
  Vector3i local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_E5;
  while (1) {
    Script_mtwkGetSetRotation(SHEN_NAME('YKUR'), &local_F4, 0);
    Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_E8, 0);
    local_E6 = script_4D06(11);
    local_E5 = script_4D06(15);
    if ((local_E6 == 0) | (local_E5 == 5)) {
      script_4CF6(11, 0);
    } else {
      local_E7 = 0;
      Script_scenCreateTask(script_4D35, 1, 12);
      while (local_E7 < 10) {
        if (local_E7 <= 4) {
          (&local_F4)->z = (&local_F4)->z - 182;
          (&local_E8)->y = (&local_E8)->y - 0.002f;
        }
        if (local_E7 == 4) {
          Script_scenCreateTask(script_4D35, 1, 1);
        }
        if (local_E7 >= 5) {
          (&local_F4)->z = (&local_F4)->z + 182;
          (&local_E8)->y = (&local_E8)->y + 0.002f;
        }
        Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_F4, 0);
        Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_E8);
        SCNF_WaitFrames_0000(1);
        local_E7 = local_E7 + 1;
      }
      script_4CF6(18, 0);
      script_4CF6(17, 0);
      script_4CF6(13, script_4D06(13) - 1);
      script_4CF6(15, script_4D06(15) + 1);
      script_4CF6(42, script_4D06(42) - 1);
      script_6D12();
      if (script_4D06(15) >= 5) {
        script_4CF6(11, 0);
      } else {
        script_4CF6(11, script_4D06(11) - 1);
      }
    }
    if (script_4D06(13) == 0) {
      script_4CF6(11, 0);
    }
    if (script_4D06(5) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_8CAC(void) {
  int32_t unused_F0[4];
  Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 755, 0, 0x42700000, 0, 0, 2.0f, 0);
  return;
}

static int32_t script_8CD0(void) {
  Vector3i local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  local_F4.x = 0;
  (&local_F4)->y = 0;
  (&local_F4)->z = 0;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  local_E7 = 0;
  while (local_E7 < 30) {
    if ((local_E7 >= 0) & (local_E7 < 3)) {
      local_F4.x = (&local_F4)->x - 182;
      (&local_E8)->y = 0.0f;
    }
    if ((local_E7 >= 3) & (local_E7 < 6)) {
      local_F4.x = (&local_F4)->x + 91;
      (&local_E8)->y = 0.0f;
    }
    if ((local_E7 >= 6) & (local_E7 < 9)) {
      local_F4.x = (&local_F4)->x + 364;
      (&local_E8)->y = 0.0f;
    }
    if ((local_E7 >= 9) & (local_E7 < 12)) {
      local_F4.x = (&local_F4)->x + 1092;
      (&local_E8)->y = -0.0065f;
    }
    if ((local_E7 >= 12) & (local_E7 < 15)) {
      local_F4.x = (&local_F4)->x + 819;
      (&local_E8)->y = -0.0085f;
    }
    if ((local_E7 >= 15) & (local_E7 < 18)) {
      local_F4.x = (&local_F4)->x;
      (&local_E8)->y = -0.005f;
    }
    if ((local_E7 >= 18) & (local_E7 < 21)) {
      local_F4.x = (&local_F4)->x - 728;
      (&local_E8)->y = 0.0045f;
    }
    if ((local_E7 >= 21) & (local_E7 < 24)) {
      local_F4.x = (&local_F4)->x - 728;
      (&local_E8)->y = 0.0075f;
    }
    if ((local_E7 >= 24) & (local_E7 < 27)) {
      local_F4.x = (&local_F4)->x - 728;
      (&local_E8)->y = 0.008f;
    }
    if ((local_E7 >= 27) & (local_E7 < 30)) {
      local_F4.x = (&local_F4)->x;
      (&local_E8)->y = 0.0f;
    }
    Script_charaPosRotUtils(SHEN_NAME('SLTM'), 13, 0, &local_F4, CHARA_PART_Set);
    Script_charaPosRotUtils(SHEN_NAME('SLTM'), 13, &local_E8, 0, CHARA_PART_Add);
    local_E7 = local_E7 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_8EB7(void) {
  script_8EDE(CHARA_AKIR);
  script_90F6(CHARA_AKIR);
  script_8FD9(CHARA_AKIR);
  return;
}

static void script_8EDE(int32_t arg0) {
  int32_t unused_E8[6];
  script_4C65();
  task_15C = 2;
  Script_motmToggleFlags(arg0, SCEN_Disable, 4);
  SCNF_WaitFrames_0000(1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetupLightFromTable(0);
  Script_refbSetActive(1);
  Script_uiUtils(0, 3, 0);
  script_B8BE(arg0, 2.0f, 0.0f, -5.8f);
  script_B8E6(arg0, 0.0f, 90.0f, 0.0f);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E) == 1) {
    Script_SetCPAMFlags(CHARA_NITO, 0);
  }
  task_292 = 1;
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_SetMotnFlag(arg0, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_30);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_8FD9(int32_t arg0) {
  Script_SetHighScoreValue(Highscore_DART_Primary, task_174);
  Script_SetHighScoreValue(Highscore_DART_Secondary, task_176);
  Script_charaDisplay(SHEN_NAME('YA_Y'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YA_R'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YA_B'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YA_G'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YA_K'), CDM_Hide);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetMotnFlag(arg0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_InitializeOsag(CHARA_AKIR);
  script_B61B(0);
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E) == 1) {
    Script_SetCPAMFlags(CHARA_NITO, 1);
  }
  Script_refbSetActive(0);
  Script_scenToggleActiveFor(arg0, 1);
  Script_ccowUtils(arg0, CCOW_EnableAll);
  script_B8BE(arg0, 0.73f, 0.0f, -6.01f);
  script_B8E6(arg0, 0.0f, 90.0f, 0.0f);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
  Script_ryoControl(RYO_RestoreControl);
  script_4C7A();
  task_15C = 0;
  return;
}

static void script_90F6(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  int32_t local_F0;
  local_FC = Script_scenCreateTask(script_B888, 0);
  local_F8 = Script_scenCreateTask(script_B321, 1, SHEN_NAME('DART'));
  for (;;) {
    local_F0 = task_292;
    switch (local_F0) {
      case 0:
        goto L009247;
        break;
      case 1:
        script_9254(arg0);
        break;
      case 2:
        script_9358(arg0);
        break;
      case 3:
        script_9384(arg0);
        break;
      case 4:
        script_93C3(arg0);
        break;
      case 5:
        script_9A33(arg0);
        break;
      case 6:
        script_AD35(arg0);
        break;
      case 7:
        script_ADE2(arg0);
        break;
      case 8:
        script_AE5B(arg0);
        break;
      case 9:
        script_AF4F(arg0);
        break;
      case 10:
        script_B13C(arg0);
        break;
      case 11:
        script_B31B(arg0);
        break;
      case 12:
        script_B31E(arg0);
        break;
      case 13:
        script_B896(arg0);
        break;
    }
  }
  L009247:;
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  return;
}

__attribute__((scn_string_bindings(6, data_114F5, 7, data_114FF)))
static void script_9254(int32_t arg0) {
  int32_t local_FC;
  script_B872(1);
  script_B8BE(arg0, 0.8f, 0.0f, -5.8f);
  script_B8E6(arg0, 0.0f, 90.0f, 0.0f);
  script_B61B(2);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_B848, 1, 9);
  script_B583(2.0f, "E1025A001", "E1025A002");
  script_B5FA();
  SCNF_WaitFrames_0000(45);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  local_FC = SCNF_RequestActionSelectorFromParams(0xFFFF, 36, 34, 0xFFFF, 0x7FFF);
  SCNF_WaitFrames_0000(1);
  if (local_FC == 36) {
    script_B583(3.0f, "E1025A003", "E1025A004", "E1025A005");
    script_B5FA();
    task_292 = 2;
  } else {
    script_B61B(7);
    script_B583(3.0f, "E1025A009", "E1025A010", "E1025A011");
    script_B5FA();
    task_292 = 0;
  }
  return;
}

static void script_9358(int32_t arg0) {
  if (Script_shenGetStats(STATS_CASH) == 0) {
    task_292 = 3;
  } else {
    task_292 = 4;
    Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  }
  return;
}

static void script_9384(int32_t arg0) {
  int32_t local_FC;
  script_B583(3.0f, "E1025A006", "E1025A007", "E1025A008");
  local_FC = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(local_FC, 1);
  script_B5FA();
  task_292 = 0;
  return;
}

static void script_93C3(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  script_B8BE(arg0, 0.8f, 0.0f, -5.8f);
  script_B8E6(arg0, 0.0f, 90.0f, 0.0f);
  Script_PlayCharaMotion(arg0, 30978, 1.0f, 169.0f, 1.0f, 1.0f, 0);
  Script_InitializeOsag(CHARA_AKIR);
  script_B872(0);
  Script_charaDisplay(SHEN_NAME('YA_Y'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YA_R'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YA_B'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YA_G'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YA_K'), CDM_Hide);
  local_F4.x = 1.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.5f;
  script_B61B(5);
  Script_charaToggleMotEnabled(arg0, 1);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_B848, 1, 6);
  Script_scenCreateTask(script_9A00, 0);
  Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == ((scn3_bits){.f = (float)30}).i) {
      Script_executeAicaCommand(67241, 0, 0);
      Script_EVT_GetPosition(arg0, 18, &local_E8, 0);
      Script_EVT_SetPosition(SHEN_NAME('YENS'), POS_XYZ, &local_E8);
    } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == ((scn3_bits){.f = (float)31}).i) {
      Vector3 local_DC;
      Vector3i local_D0;
      Script_charaDisplay(SHEN_NAME('YENS'), CDM_Show);
      local_DC.x = 0.124f;
      (&local_DC)->y = 0.012f;
      (&local_DC)->z = -0.085f;
      local_D0.x = 64990;
      (&local_D0)->y = 64260;
      (&local_D0)->z = 46788;
      Script_fixoAttachEx(SHEN_NAME('YENS'), arg0, 18, &local_DC, &local_D0);
    } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == ((scn3_bits){.f = (float)40}).i) {
      Script_SetNewHandMotion(arg0, 1, 751, 0, 0, 0, 0, 1.0f, 0);
    } else {
      if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == ((scn3_bits){.f = (float)130}).i) {
        Script_executeAicaCommand(1705, 0, 0);
        Script_charaDisplay(SHEN_NAME('YENS'), CDM_Hide);
        goto L0095BF;
      }
    }
    L0095BF:;
    Script_RotateNodeUtils(arg0, &data_F7D4, 1);
    SCNF_WaitFrames_0000(1);
  }
  task_290 = 0;
  script_B61B(14);
  script_B8BE(arg0, 0.8f, 0.0f, -5.755f);
  Script_SetNewHandMotion(arg0, 0, 758, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(arg0, 1, 759, 0, 0, 0, 0, 1.0f, 30);
  Script_PlayCharaMotion(arg0, 31002, 1.0f, 200.0f, 1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_executeAicaCommand(263849, 0, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == ((scn3_bits){.f = (float)55}).i) {
      Script_executeAicaCommand(329385, 0, 0);
      Script_EVT_GetPosition(arg0, 12, &local_E8, 0);
      Script_EVT_SetPosition(SHEN_NAME('YA_Y'), POS_XYZ, &local_E8);
      Script_EVT_GetPosition(arg0, 12, &local_E8, 0);
      Script_EVT_SetPosition(SHEN_NAME('YA_R'), POS_XYZ, &local_E8);
      Script_EVT_GetPosition(arg0, 12, &local_E8, 0);
      Script_EVT_SetPosition(SHEN_NAME('YA_B'), POS_XYZ, &local_E8);
      Script_EVT_GetPosition(arg0, 12, &local_E8, 0);
      Script_EVT_SetPosition(SHEN_NAME('YA_G'), POS_XYZ, &local_E8);
      Script_EVT_GetPosition(arg0, 12, &local_E8, 0);
      Script_EVT_SetPosition(SHEN_NAME('YA_K'), POS_XYZ, &local_E8);
    } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == ((scn3_bits){.f = (float)56}).i) {
      Script_charaDisplay(SHEN_NAME('YA_Y'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('YA_R'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('YA_B'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('YA_G'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('YA_K'), CDM_Show);
      {
        Vector3 local_DC;
        Vector3i local_D0;
        local_DC.x = 0.158f;
        (&local_DC)->y = 0.009f;
        (&local_DC)->z = 0.004f;
        local_D0.x = 0;
        (&local_D0)->y = 2001;
        (&local_D0)->z = 15469;
        Script_fixoAttachEx(SHEN_NAME('YA_Y'), arg0, 12, &local_DC, &local_D0);
      }
      {
        Vector3 local_DC;
        Vector3i local_D0;
        local_DC.x = 0.145f;
        (&local_DC)->y = 0.01f;
        (&local_DC)->z = -0.002f;
        local_D0.x = 0;
        (&local_D0)->y = 2001;
        (&local_D0)->z = 15469;
        Script_fixoAttachEx(SHEN_NAME('YA_R'), arg0, 12, &local_DC, &local_D0);
      }
      {
        Vector3 local_DC;
        Vector3i local_D0;
        local_DC.x = 0.128f;
        (&local_DC)->y = 0.017f;
        (&local_DC)->z = -0.007f;
        local_D0.x = 0;
        (&local_D0)->y = 2001;
        (&local_D0)->z = 15469;
        Script_fixoAttachEx(SHEN_NAME('YA_B'), arg0, 12, &local_DC, &local_D0);
      }
      {
        Vector3 local_DC;
        Vector3i local_D0;
        local_DC.x = 0.135f;
        (&local_DC)->y = 0.008f;
        (&local_DC)->z = 0.0f;
        local_D0.x = 0;
        (&local_D0)->y = 2001;
        (&local_D0)->z = 15469;
        Script_fixoAttachEx(SHEN_NAME('YA_G'), arg0, 12, &local_DC, &local_D0);
      }
      {
        Vector3 local_DC;
        Vector3i local_D0;
        local_DC.x = 0.15f;
        (&local_DC)->y = 0.009f;
        (&local_DC)->z = 0.004f;
        local_D0.x = 0;
        (&local_D0)->y = 2001;
        (&local_D0)->z = 15469;
        Script_fixoAttachEx(SHEN_NAME('YA_K'), arg0, 12, &local_DC, &local_D0);
      }
    } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == ((scn3_bits){.f = (float)125}).i) {
      script_B61B(10);
    } else if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == ((scn3_bits){.f = (float)180}).i) {
      Script_SetNewHandMotion(arg0, 1, 751, 0, 0, 0, 0, 1.0f, 0);
      Script_EVT_GetPosition(arg0, 18, &local_E8, 0);
      Script_EVT_SetPosition(SHEN_NAME('YA_Y'), POS_XYZ, &local_E8);
    } else {
      if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == ((scn3_bits){.f = (float)181}).i) {
        Vector3 local_DC;
        Vector3i local_D0;
        local_DC.x = 0.137f;
        (&local_DC)->y = 0.028f;
        (&local_DC)->z = -0.068f;
        local_D0.x = 0;
        (&local_D0)->y = 0;
        (&local_D0)->z = 15834;
        Script_fixoAttachEx(SHEN_NAME('YA_Y'), arg0, 18, &local_DC, &local_D0);
        goto L0099AB;
      }
    }
    L0099AB:;
    Script_RotateNodeUtils(arg0, &local_F4, 1);
    SCNF_WaitFrames_0000(1);
  }
  Script_charaToggleMotEnabled(arg0, 0);
  Script_PlayCharaMotion(arg0, 30999, 0.0f, 44.0f, 0.0f, 1.0f, 0);
  script_B80A(arg0);
  task_28E = 0;
  task_293 = 5;
  task_292 = 5;
  return;
}

static int32_t script_9A00(void) {
  task_274 = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(-100, 1);
  task_274 = task_274 - 100;
  Script_SetStats(STATS_CASH, task_274);
  return Script_srchControl(0);
}

static void script_9A33(int32_t arg0) {
  int32_t unused_F4[3];
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t unused_C0[4];
  Vector4 local_B0;
  int32_t local_AC;
  Script_SetCameraFlags(0);
  Script_LightUtils(15, 0, 3);
  Script_LightUtils(15, 2, &data_11528);
  Script_LightUtils(15, 4, 0.5f);
  Script_LightUtils(15, 5, 0.74f);
  Script_LightUtils(15, 6, data_11538);
  Script_LightUtils(15, 7, &data_11540);
  local_B0.x = 0.0f;
  (&local_B0)->y = -0.3f;
  (&local_B0)->z = -0.3f;
  (&local_B0)->w = -0.3f;
  Script_SetCharaRefBoardPos(arg0, &local_B0);
  script_B8BE(CHARA_AKIR, 2.0f, 0.0f, -5.8f);
  script_B8E6(CHARA_AKIR, 0.0f, 90.0f, 0.0f);
  Script_sprintf("涼座標セット\012");
  local_DC.x = 0.137f;
  (&local_DC)->y = 0.028f;
  (&local_DC)->z = -0.068f;
  local_D0.x = 0;
  (&local_D0)->y = 0;
  (&local_D0)->z = 15834;
  local_AC = task_293;
  switch (local_AC) {
    case 5:
      task_284 = 0x595F4159;
      break;
    case 4:
      task_284 = 0x525F4159;
      break;
    case 3:
      task_284 = 0x425F4159;
      break;
    case 2:
      task_284 = 0x475F4159;
      break;
    case 1:
      task_284 = 0x4B5F4159;
      script_B583(3.0f, "E1025A022", "E1025A023", "E1025A024");
      script_B5FA();
      break;
  }
  task_28C = 0;
  Script_PlayCharaMotion(arg0, 30999, 0.0f, 0.0f, 0.0f, 0.0f, 0);
  Script_EVT_GetPosition(arg0, 18, &local_E8, 0);
  Script_EVT_SetPosition(task_284, POS_XYZ, &local_E8);
  Script_fixoAttachEx(task_284, arg0, 18, &local_DC, &local_D0);
  Script_PlayCharaMotion(arg0, 30999, 0.0f, 44.0f, 0.0f, 1.0f, 0);
  Script_SetNewHandMotion(arg0, 1, 751, 0, 0, 0, 0, 1.0f, 0);
  script_B61B(3);
  Script_lightSetSlotEnabled(15, 1);
  for (;;) {
    script_9CB6(arg0);
    task_288 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (Script_GetControllerInput(1) == 0x400) {
      script_9D68(arg0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_B0.x = 0.0f;
  (&local_B0)->y = 0.0f;
  (&local_B0)->z = 0.0f;
  (&local_B0)->w = 0.0f;
  Script_SetCharaRefBoardPos(arg0, &local_B0);
  Script_lightSetSlotEnabled(15, 0);
  return;
}

static void script_9CB6(int32_t arg0) {
  Vector3i local_F4;
  int32_t unused_F0;
  int32_t local_E8[2];
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x40000000);
  if (task_28C == 90) {
    task_28C = 0;
    local_E8[1] = -36;
  } else if ((task_28C >= 0) & (task_28C <= 24)) {
    local_E8[1] = -36;
  } else if ((task_28C >= 25) & (task_28C <= 69)) {
    local_E8[1] = 36;
  } else {
    if ((task_28C >= 70) & (task_28C <= 89)) {
      local_E8[1] = -36;
      goto L009D39;
    }
  }
  L009D39:;
  (&local_F4)->y = (&local_F4)->y + local_E8[1];
  Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  task_28C = task_28C + 1;
  return;
}

static void script_9D68(int32_t arg0) {
  task_288 = Script_GetCharaMotionRemainingFramesScaled(arg0);
  Script_PlayCharaMotion(arg0, 30997, 0.0f, 70.0f, 0.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(arg0)}).i == 0x41D00000) {
      script_B61B(4);
      Script_executeAicaCommand(394921, 0, 0);
      Script_SetNewHandMotion(arg0, 1, 753, 0, 0, 0, 0, 1.0f, 0);
      Script_fixoClearAttachment(task_284);
      task_294 = 0;
      SCNF_WaitFrames_0000(1);
      Script_scenCreateTask(script_B848, 1, 13);
      Script_scenCreateTask(script_9F16, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_B80A(arg0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (task_294 == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  switch (task_28E) {
    case 100:
      task_292 = 7;
      break;
    case 50:
      task_292 = 8;
      break;
    case 40:
      task_292 = 8;
      break;
    case 30:
      task_292 = 8;
      break;
    case 20:
      task_292 = 8;
      break;
    case 10:
      task_292 = 8;
      break;
    case 0:
      task_292 = 9;
      break;
  }
  if (task_293 > 1) {
    Script_PlayCharaMotion(arg0, 30999, 0.0f, 0.0f, 0.0f, 0.0f, 0);
  } else {
    if (task_293 <= 1) {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      goto L009F15;
    }
  }
  L009F15:;
  return;
}

static int32_t script_9F16(void) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3 local_DC;
  float local_D8;
  float local_D4;
  float local_D0;
  int32_t unused_CC;
  float local_C8;
  float local_C4;
  int8_t local_C3;
  int8_t local_C2;
  uint8_t unused_C0[2];
  Vector3i local_B4;
  int8_t local_B3;
  local_B4.x = 0;
  (&local_B4)->y = 0;
  (&local_B4)->z = 0;
  Script_EVT_SetRotation(task_284, 0x78000000, &local_B4, 0);
  {
    uint8_t unused_B0[3];
    int32_t local_AC;
    local_AC = Script_FloatUtils(ScriptMathOp_FloatToInt, task_288);
    switch (local_AC) {
      case 0:
        local_D0 = 0.0f;
        break;
      case 1:
        local_D0 = -0.02f;
        break;
      case 2:
        local_D0 = -0.04f;
        break;
      case 3:
        local_D0 = -0.06f;
        break;
      case 4:
        local_D0 = -0.08f;
        break;
      case 5:
        local_D0 = -0.099999994f;
        break;
      case 6:
        local_D0 = -0.12f;
        break;
      case 7:
        local_D0 = -0.14f;
        break;
      case 8:
        local_D0 = -0.16f;
        break;
      case 9:
        local_D0 = -0.179999992f;
        break;
      case 10:
        local_D0 = -0.199999988f;
        break;
      case 11:
        local_D0 = -0.22f;
        break;
      case 12:
        local_D0 = -0.22f;
        break;
      case 13:
        local_D0 = -0.22f;
        break;
      case 14:
        local_D0 = -0.22f;
        break;
      case 15:
        local_D0 = -0.199999988f;
        break;
      case 16:
        local_D0 = -0.179999992f;
        break;
      case 17:
        local_D0 = -0.16f;
        break;
      case 18:
        local_D0 = -0.14f;
        break;
      case 19:
        local_D0 = -0.099999994f;
        break;
      case 20:
        local_D0 = -0.08f;
        break;
      case 21:
        local_D0 = -0.06f;
        break;
      case 22:
        local_D0 = -0.02f;
        break;
      case 23:
        local_D0 = 0.0f;
        break;
      case 24:
        local_D0 = 0.0f;
        break;
      case 25:
        local_D0 = 0.02f;
        break;
      case 26:
        local_D0 = 0.06f;
        break;
      case 27:
        local_D0 = 0.08f;
        break;
      case 28:
        local_D0 = 0.099999994f;
        break;
      case 29:
        local_D0 = 0.14f;
        break;
      case 30:
        local_D0 = 0.16f;
        break;
      case 31:
        local_D0 = 0.179999992f;
        break;
      case 32:
        local_D0 = 0.199999988f;
        break;
      case 33:
        local_D0 = 0.22f;
        break;
      case 34:
        local_D0 = 0.22f;
        break;
      case 35:
        local_D0 = 0.199999988f;
        break;
      case 36:
        local_D0 = 0.179999992f;
        break;
      case 37:
        local_D0 = 0.16f;
        break;
      case 38:
        local_D0 = 0.14f;
        break;
      case 39:
        local_D0 = 0.12f;
        break;
      case 40:
        local_D0 = 0.099999994f;
        break;
      case 41:
        local_D0 = 0.08f;
        break;
      case 42:
        local_D0 = 0.06f;
        break;
      case 43:
        local_D0 = 0.04f;
        break;
      case 44:
        local_D0 = 0.02f;
        break;
    }
  }
  {
    uint8_t unused_B0[3];
    int32_t local_AC;
    local_AC = task_28C;
    switch (local_AC) {
      case 1:
        local_D4 = 0.0f;
        break;
      case 2:
        local_D4 = -0.01f;
        break;
      case 3:
        local_D4 = -0.02f;
        break;
      case 4:
        local_D4 = -0.03f;
        break;
      case 5:
        local_D4 = -0.04f;
        break;
      case 6:
        local_D4 = -0.049999997f;
        break;
      case 7:
        local_D4 = -0.06f;
        break;
      case 8:
        local_D4 = -0.07f;
        break;
      case 9:
        local_D4 = -0.08f;
        break;
      case 10:
        local_D4 = -0.0899999961f;
        break;
      case 11:
        local_D4 = -0.099999994f;
        break;
      case 12:
        local_D4 = -0.11f;
        break;
      case 13:
        local_D4 = -0.12f;
        break;
      case 14:
        local_D4 = -0.13f;
        break;
      case 15:
        local_D4 = -0.14f;
        break;
      case 16:
        local_D4 = -0.149999991f;
        break;
      case 17:
        local_D4 = -0.16f;
        break;
      case 18:
        local_D4 = -0.17f;
        break;
      case 19:
        local_D4 = -0.179999992f;
        break;
      case 20:
        local_D4 = -0.19f;
        break;
      case 21:
        local_D4 = -0.199999988f;
        break;
      case 22:
        local_D4 = -0.21f;
        break;
      case 23:
        local_D4 = -0.22f;
        break;
      case 24:
        local_D4 = -0.229999989f;
        break;
      case 25:
        local_D4 = -0.24f;
        break;
      case 26:
        local_D4 = -0.229999989f;
        break;
      case 27:
        local_D4 = -0.22f;
        break;
      case 28:
        local_D4 = -0.21f;
        break;
      case 29:
        local_D4 = -0.199999988f;
        break;
      case 30:
        local_D4 = -0.19f;
        break;
      case 31:
        local_D4 = -0.179999992f;
        break;
      case 32:
        local_D4 = -0.17f;
        break;
      case 33:
        local_D4 = -0.16f;
        break;
      case 34:
        local_D4 = -0.149999991f;
        break;
      case 35:
        local_D4 = -0.14f;
        break;
      case 36:
        local_D4 = -0.13f;
        break;
      case 37:
        local_D4 = -0.12f;
        break;
      case 38:
        local_D4 = -0.11f;
        break;
      case 39:
        local_D4 = -0.099999994f;
        break;
      case 40:
        local_D4 = -0.0899999961f;
        break;
      case 41:
        local_D4 = -0.08f;
        break;
      case 42:
        local_D4 = -0.07f;
        break;
      case 43:
        local_D4 = -0.06f;
        break;
      case 44:
        local_D4 = -0.049999997f;
        break;
      case 45:
        local_D4 = -0.04f;
        break;
      case 46:
        local_D4 = -0.03f;
        break;
      case 47:
        local_D4 = -0.02f;
        break;
      case 48:
        local_D4 = -0.01f;
        break;
      case 49:
        local_D4 = 0.0f;
        break;
      case 50:
        local_D4 = 0.01f;
        break;
      case 51:
        local_D4 = 0.02f;
        break;
      case 52:
        local_D4 = 0.03f;
        break;
      case 53:
        local_D4 = 0.04f;
        break;
      case 54:
        local_D4 = 0.049999997f;
        break;
      case 55:
        local_D4 = 0.06f;
        break;
      case 56:
        local_D4 = 0.07f;
        break;
      case 57:
        local_D4 = 0.08f;
        break;
      case 58:
        local_D4 = 0.0899999961f;
        break;
      case 59:
        local_D4 = 0.099999994f;
        break;
      case 60:
        local_D4 = 0.11f;
        break;
      case 61:
        local_D4 = 0.12f;
        break;
      case 62:
        local_D4 = 0.13f;
        break;
      case 63:
        local_D4 = 0.14f;
        break;
      case 64:
        local_D4 = 0.149999991f;
        break;
      case 65:
        local_D4 = 0.16f;
        break;
      case 66:
        local_D4 = 0.17f;
        break;
      case 67:
        local_D4 = 0.179999992f;
        break;
      case 68:
        local_D4 = 0.19f;
        break;
      case 69:
        local_D4 = 0.199999988f;
        break;
      case 70:
        local_D4 = 0.21f;
        break;
      case 71:
        local_D4 = 0.199999988f;
        break;
      case 72:
        local_D4 = 0.19f;
        break;
      case 73:
        local_D4 = 0.179999992f;
        break;
      case 74:
        local_D4 = 0.17f;
        break;
      case 75:
        local_D4 = 0.16f;
        break;
      case 76:
        local_D4 = 0.149999991f;
        break;
      case 77:
        local_D4 = 0.14f;
        break;
      case 78:
        local_D4 = 0.13f;
        break;
      case 79:
        local_D4 = 0.12f;
        break;
      case 80:
        local_D4 = 0.11f;
        break;
      case 81:
        local_D4 = 0.099999994f;
        break;
      case 82:
        local_D4 = 0.0899999961f;
        break;
      case 83:
        local_D4 = 0.08f;
        break;
      case 84:
        local_D4 = 0.07f;
        break;
      case 85:
        local_D4 = 0.06f;
        break;
      case 86:
        local_D4 = 0.049999997f;
        break;
      case 87:
        local_D4 = 0.04f;
        break;
      case 88:
        local_D4 = 0.03f;
        break;
      case 89:
        local_D4 = 0.02f;
        break;
      case 90:
        local_D4 = 0.01f;
        break;
    }
  }
  local_C8 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.017f);
  local_C4 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.017f);
  local_C3 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
  local_C2 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
  switch (local_C3) {
    case 0:
      local_C8 = (float)0 - local_C8;
      break;
    case 1:
      (void)local_C8;
      break;
  }
  switch (local_C2) {
    case 0:
      local_C4 = (float)0 - local_C4;
      break;
    case 1:
      (void)local_C4;
      break;
  }
  Script_EVT_GetPosition(task_284, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  local_E8[0] = (0.22f - (&local_F4)->x) / (float)13;
  local_E8[1] = (local_D0 + local_C8 + 1.4062f - (&local_F4)->y) / (float)13;
  local_E8[2] = (local_D4 + local_C4 + -5.77f - (&local_F4)->z) / (float)13;
  local_B3 = 13;
  for (;;) {
    Script_EVT_GetPosition(task_284, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    local_F4.x = (&local_F4)->x + *local_E8;
    (&local_F4)->y = (&local_F4)->y + local_E8[1];
    (&local_F4)->z = (&local_F4)->z + local_E8[2];
    Script_EVT_SetPosition(task_284, 0x78000000, &local_F4);
    local_B3 = local_B3 - 1;
    if (local_B3 == 0) {
      uint8_t unused_B0[3];
      int32_t local_AC;
      local_AC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      switch (local_AC) {
        case 0:
          Script_executeAicaCommand(132777, 0, 0);
          break;
        case 1:
          Script_executeAicaCommand(198313, 0, 0);
          break;
      }
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_GetPosition(task_284, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 1.4062f - (&local_DC)->y;
  (&local_F4)->z = -5.8f - (&local_DC)->z;
  local_D8 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->y, (&local_F4)->z);
  if (!(local_D8 < (float)0) & !(local_D8 > 0.014f)) {
    task_28E = 100;
  } else if (!(local_D8 <= 0.014f) & !(local_D8 > 0.044f)) {
    task_28E = 50;
  } else if (!(local_D8 <= 0.044f) & !(local_D8 > 0.074f)) {
    task_28E = 40;
  } else if (!(local_D8 <= 0.074f) & !(local_D8 > 0.107438f)) {
    task_28E = 30;
  } else if (!(local_D8 <= 0.107438f) & !(local_D8 > 0.1432f)) {
    task_28E = 20;
  } else if (!(local_D8 <= 0.1432f) & !(local_D8 > 0.1949f)) {
    task_28E = 10;
  } else {
    if (!(local_D8 <= 0.1949f)) {
      task_28E = 0;
      goto L00AD27;
    }
  }
  L00AD27:;
  task_294 = 1;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_11578, 4, data_115A3, 5, data_115AD, 6, data_115B7, 9, data_115D5)))
static void script_AD35(int32_t arg0) {
  int32_t local_FC;
  script_B61B(2);
  script_B583(3.0f, "E1025A032", "E1025A033", "E1025A034");
  script_B5FA();
  SCNT_ShowSubtitle("やめますか？");
  local_FC = SCNF_RequestActionSelector(0x7FFF);
  SCNF_WaitFrames_0000(1);
  switch (local_FC) {
    case 1:
      script_B583(3.0f, "E1025A010", "E1025A011", "E1025A032");
      script_B5FA();
      task_292 = 0;
      break;
    case 0:
      script_B583(3.0f, "E1025A035", "E1025A036", "E1025A030");
      script_B5FA();
      task_292 = 5;
      break;
  }
  return;
}

static void script_ADE2(int32_t arg0) {
  script_B61B(8);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_B848, 1, 15);
  script_B574("E1025A015");
  script_B5FA();
  SCNF_WaitFrames_0000(24);
  script_AFDA(task_28E);
  task_293 = task_293 - 1;
  SCNF_WaitFrames_0000(1);
  if (task_293 == 0) {
    task_292 = 10;
  } else {
    task_292 = 5;
  }
  Script_SetCameraFlags(1);
  return;
}

__attribute__((scn_string_bindings(2, data_115FD, 3, data_11607, 4, data_11611, 5, data_1161B)))
static void script_AE5B(int32_t arg0) {
  script_B61B(8);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_B848, 1, 15);
  switch (task_28E) {
    case 50:
      script_B583(2.0f, "E1025A012", "E1025A018");
      break;
    case 40:
      script_B574("E1025A014");
      break;
    case 30:
      script_B574("E1025A014");
      break;
    case 20:
      script_B574("E1025A013");
      break;
    case 10:
      script_B574("E1025A013");
      break;
  }
  script_B5FA();
  SCNF_WaitFrames_0000(24);
  script_AFDA(task_28E);
  task_293 = task_293 - 1;
  SCNF_WaitFrames_0000(1);
  if (task_293 == 0) {
    task_292 = 10;
  } else {
    task_292 = 5;
  }
  Script_SetCameraFlags(1);
  return;
}

static void script_AF4F(int32_t arg0) {
  script_B61B(8);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_B848, 1, 15);
  script_B583(2.0f, "E1025A016", "E1025A017");
  script_B5FA();
  SCNF_WaitFrames_0000(48);
  script_AFDA(task_28E);
  SCNF_WaitFrames_0000(24);
  task_293 = task_293 - 1;
  SCNF_WaitFrames_0000(1);
  if (task_293 == 0) {
    task_292 = 10;
  } else {
    task_292 = 5;
  }
  Script_SetCameraFlags(1);
  return;
}

static void script_AFDA(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  SCNF_WaitFrames_0000(25);
  if (arg0 == 0) {
    SCNF_WaitFrames_0000(10);
  } else {
    switch (arg0) {
      case 10:
        local_FF = arg0 / 10;
        local_FE = 10;
        local_FC = 2;
        break;
      case 20:
        local_FF = arg0 / 20;
        local_FE = 20;
        local_FC = 2;
        break;
      case 30:
        local_FF = arg0 / 30;
        local_FE = 30;
        local_FC = 2;
        break;
      case 40:
        local_FF = arg0 / 40;
        local_FE = 40;
        local_FC = 1;
        break;
      case 50:
        local_FF = arg0 / 50;
        local_FE = 50;
        local_FC = 1;
        break;
      case 100:
        local_FF = arg0 / 50;
        local_FE = 50;
        local_FC = 1;
        break;
    }
    local_FD = local_FC;
    Script_executeAicaCommand(460457, 0, 0);
    for (;;) {
      if (local_FE == 0) {
        Script_executeAicaCommand(525993, 0, 0);
        break;
      } else if (local_FD == 0) {
        task_290 = task_290 + local_FF;
        local_FE = local_FE - 1;
        local_FD = local_FC;
      } else {
        if (local_FD != 0) {
          local_FD = local_FD - 1;
          goto L00B128;
        }
      }
      L00B128:;
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(30);
  }
  return;
}

__attribute__((scn_string_bindings(12, data_116D8, 13, data_116E2, 15, data_116F6)))
static void script_B13C(int32_t arg0) {
  int32_t local_FC;
  Script_LightUtils(15, 0, 3);
  Script_LightUtils(15, 2, &data_1163C);
  Script_LightUtils(15, 4, 0.5f);
  Script_LightUtils(15, 5, 0.84f);
  Script_LightUtils(15, 6, data_1164C);
  Script_LightUtils(15, 7, &data_11654);
  task_176 = task_290;
  if (task_176 > task_174) {
    task_174 = task_176;
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E) == 1) {
    if (task_290 >= 300) {
      Script_lightSetSlotEnabled(15, 1);
      script_B8BE(arg0, 3.2091f, 0.0f, -5.7386f);
      script_B8E6(arg0, 0.0f, 40.0f, 0.0f);
      script_B61B(12);
      script_B583(4.0f, "E1025B001", "E1025B002", "E1025B003", "E1025B004");
      script_B5FA();
      script_B872(2);
      script_B583(3.0f, "E1025A019", "E1025A020", "E1025A021");
      script_B5FA();
      SCNF_WaitFrames_0000(30);
      script_B872(4);
      SCNF_WaitFrames_0000(1);
    }
  }
  script_B872(3);
  script_B61B(7);
  SCNF_WaitFrames_0000(1);
  script_B583(3.0f, "E1025A025", "E1025A026", "E1025A027");
  script_B5FA();
  local_FC = SCNF_RequestActionSelectorFromParams(0xFFFF, 37, 34, 0xFFFF, 0x7FFF);
  SCNF_WaitFrames_0000(1);
  if (local_FC == 37) {
    script_B583(3.0f, "E1025A028", "E1025A029", "E1025A030");
    script_B5FA();
    task_292 = 2;
  } else {
    script_B583(3.0f, "E1025A011", "E1025A031", "E1025A032");
    script_B5FA();
    task_292 = 0;
  }
  return;
}

static void script_B31B(int32_t arg0) {
  return;
}

static void script_B31E(int32_t arg0) {
  return;
}

static int32_t script_B321(int32_t arg0) {
  for (;;) {
    Script_GameUtils(arg0, GC_CommitStagedEntries, 0, 0, 0, 0);
    Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_11700, 9999, 0);
    Script_GameUtils(arg0, GC_StageNumericDisplayValue, task_290, &data_11718, 1.0f, &data_11748);
    Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1170C, 9999, 0);
    Script_GameUtils(arg0, GC_StageNumericDisplayValue, task_176, &data_11724, 1.0f, &data_1173C);
    Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1170C, 9999, 0);
    Script_GameUtils(arg0, GC_StageNumericDisplayValue, task_174, &data_11730, 1.0f, &data_1173C);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_B3D3(int32_t arg0) {
  Vector3 local_F4;
  switch (task_295) {
    case 0:
      Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
      Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
      break;
    case 1:
      Script_EVT_GetPosition(SHEN_NAME('DART'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      (&local_F4)->y = (&local_F4)->y + 1.4f;
      Script_SetCharaLookAtPosition(arg0, 1, &local_F4, 0);
      Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 5);
      break;
    case 2:
      {
        Vector3 local_E8;
        Script_EVT_GetPosition(CHARA_NITO, 6, &local_F4, 0);
        local_E8.x = (&local_F4)->x + -0.6645f;
        (&local_E8)->y = (&local_F4)->y + -0.0931f;
        (&local_E8)->z = (&local_F4)->z + -0.9548f;
        Script_SetCharaLookAtPosition(arg0, 98334, &local_E8, 2);
        Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 15);
        break;
      }
    case 3:
      Script_EVT_GetPosition(SHEN_NAME('DART'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      (&local_F4)->y = (&local_F4)->y + 1.4f;
      Script_SetCharaLookAtPosition(arg0, 98334, &local_F4, 2);
      Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 15);
      break;
    case 4:
      {
        Vector3 local_E8;
        Script_EVT_GetPosition(CHARA_NITO, 6, &local_F4, 0);
        local_E8.x = (&local_F4)->x + -1.3917f;
        (&local_E8)->y = (&local_F4)->y + 0.0698f;
        (&local_E8)->z = (&local_F4)->z + -1.1371f;
        Script_SetCharaLookAtPosition(arg0, 65537, &local_E8, 2);
        Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 1);
        break;
      }
    case 5:
      break;
  }
  return;
}

static void script_B574(const char *arg0) {
  task_270 = Script_ShowUIMessageFromID(arg0);
  return;
}

static void script_B583(float arg0, const char *arg1, const char *arg2, ...) {
  int8_t local_FF;
  local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, arg0));
  switch (local_FF) {
    case 0:
      task_270 = Script_ShowUIMessageFromID(arg1);
      break;
    case 1:
      task_270 = Script_ShowUIMessageFromID(arg2);
      break;
    case 2:
      {
        va_list arguments0;
        va_start(arguments0, arg2);
        task_270 = Script_ShowUIMessageFromID(va_arg(arguments0, const char *));
        va_end(arguments0);
      }
      break;
    case 3:
      {
        va_list arguments1;
        va_start(arguments1, arg2);
        (void)va_arg(arguments1, const char *);
        task_270 = Script_ShowUIMessageFromID(va_arg(arguments1, const char *));
        va_end(arguments1);
      }
      break;
  }
  return;
}

static void script_B5FA(void) {
  SCNF_WaitFrames_0000(1);
  while (Script_convGetIsActive((const void *)task_270)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_B61B(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_SetGameStateTo(AUTH_None);
      break;
    case 1:
      Script_SetGameStateTo(AUTH_FirstPerson);
      break;
    case 2:
      Script_seqEnterEvtCam(7200, SHEN_NAME('DART'), SHEN_NAME('DART'));
      break;
    case 3:
      Script_seqEnterEvtCam(7201, SHEN_NAME('DART'), SHEN_NAME('DART'));
      break;
    case 4:
      Script_seqEnterEvtCam(7202, task_284, task_284);
      break;
    case 5:
      Script_seqEnterEvtCam(7203, SHEN_NAME('DART'), SHEN_NAME('DART'));
      break;
    case 6:
      Script_seqEnterEvtCam(7204, CHARA_AKIR, CHARA_AKIR);
      break;
    case 7:
      Script_seqEnterEvtCam(7205, CHARA_AKIR, CHARA_AKIR);
      break;
    case 8:
      Script_seqEnterEvtCam(7206, SHEN_NAME('DART'), SHEN_NAME('DART'));
      break;
    case 9:
      Script_seqEnterEvtCam(7207, SHEN_NAME('DART'), SHEN_NAME('DART'));
      break;
    case 10:
      Script_seqEnterEvtCam(7208, CHARA_AKIR, CHARA_AKIR);
      break;
    case 11:
      Script_seqEnterEvtCam(7209, 0, AUTH_None);
      break;
    case 12:
      Script_seqEnterEvtCam(7210, SHEN_NAME('DART'), SHEN_NAME('DART'));
      break;
    case 13:
      Script_seqEnterEvtCam(7211, task_284, task_284);
      break;
    case 14:
      Script_seqEnterEvtCam(7212, SHEN_NAME('DART'), SHEN_NAME('DART'));
      break;
    case 15:
      Script_seqEnterEvtCam(7213, SHEN_NAME('DART'), SHEN_NAME('DART'));
      break;
    case 16:
      Script_seqEnterEvtCam(7214, SHEN_NAME('DART'), SHEN_NAME('DART'));
      break;
    case 17:
      Script_seqEnterEvtCam(7215, SHEN_NAME('DART'), SHEN_NAME('DART'));
      break;
  }
  return;
}

static void script_B80A(int32_t arg0) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_B824(int32_t arg0, const Vector3 *arg1) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, arg1, 1);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_B848(int32_t arg0) {
  for (;;) {
    if (Script_IsInEventCam() == 0) {
      script_B61B(arg0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_B872(int32_t arg0) {
  task_295 = arg0;
  script_B3D3(CHARA_AKIR);
  return;
}

static int32_t script_B888(void) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_B896(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
  Script_ryoControl(RYO_RestoreControl);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_B8BE(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_B8E6(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static int32_t script_B93A(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_B98A(int32_t arg0) {
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

static void script_BA31(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_B98A(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_BA54(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_BA87(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_BA54(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_BAAA(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_BA54(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_BB09(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  return (int32_t)local_F8;
}

static void script_BB1E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_BEB4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_C542(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    task_2BC[local_FF] = -1;
    local_FF = local_FF + 1;
  }
  task_300 = -1;
  task_304 = -1;
  task_308 = -1;
  task_2E8 = -1;
  (void)((task_322 = 0) & 2);
  (void)((task_323 = 0) & 2);
  (void)((task_324 = 0) & 2);
  (void)((task_327 = 0) & 2);
  task_31C = 0x3F4CCCCD;
  (void)((task_325 = 0) & 2);
  task_321 = -1;
  script_C6D4();
  (void)((task_326 = 0) & 2);
  Script_scenUtils(SCENARIO_CMD_SET_RUNTIME_FLAG_80, 0);
  return;
}

static void script_C5E3(void) {
  (void)((task_322 = -1) & 2);
  while (task_322 & 0x10) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_C605(void) {
  return;
}

static void script_C608(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = -10;
  SCNF_CopyScenario_000E(1, task_2F8, (const void *)local_F8, 8);
  return;
}

static int32_t script_C629(int32_t arg0) {
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_298)) {
    if (Script_charaHasMotionwork(task_298) != 0) {
      (void)-1;
      goto L00C651;
    }
  }
  (void)0;
  L00C651:;
}

static void script_C652(int32_t arg0) {
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
        goto L00C6D3;
      }
    }
    (void)0;
  }
  L00C6D3:;
  return;
}

static void script_C6D4(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    task_29C[local_FF] = -1;
    local_FF = local_FF + 1;
  }
  return;
}

static void script_C6FF(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  Script_sprintf("FTLK: Get ID list(%s)\012", script_BB09(arg0));
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
      task_29C[local_F7] = -1;
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC)) {
        if (Script_charaHasMotionwork(local_FC)) {
          task_29C[local_F7] = local_FC;
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

static int8_t script_C7D1(int32_t arg0) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    if (task_29C[local_FF] == arg0) {
      break;
    }
    local_FF = local_FF + 1;
  }
  return local_FF;
}

static void script_C806(void) {
  script_C542();
  Script_scenUtils(SCENARIO_CMD_CREATE_SEQ_CHARACTERS);
  Script_scenCreateTask(script_C851, 0);
  return;
}

static void script_C81F(void) {
  script_C542();
  Script_scenUtils(SCENARIO_CMD_CREATE_SEQ_CHARACTERS);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_scenCreateTask(script_C851, 0);
  return;
}

static void script_C83E(void) {
  script_C542();
  Script_scenUtils(SCENARIO_CMD_CREATE_SEQ_CHARACTERS);
  script_C851();
  return;
}

__attribute__((scn_string_bindings(13, data_1193F, 14, data_11953, 15, data_11967)))
static int32_t script_C851(void) {
  int32_t local_F8[2];
  int32_t local_F4;
  int32_t local_F0;
  uint8_t unused_EF;
  int8_t local_EE;
  uint8_t unused_EC[2];
  int32_t local_E8;
  task_2F4 = SCNF_CopyScenario_000E(0, 0x414E4353);
  task_2F8 = SCNF_CopyScenario_000E(0, 0x464E4353);
  for (;;) {
    SCNF_CopyScenario_000E(2, task_2F4, &task_2EC, 8);
    local_F8[0] = *&task_2EC;
    local_F8[1] = -10;
    task_298 = *&task_2EC;
    local_F4 = *(&task_2EC + 1);
    task_2DC = task_298;
    (void)((local_EE = -(script_C629(task_298) != 0)) & 8);
    if ((local_F4 >= 140) & (local_F4 <= 149)) {
      Script_sprintf("FTLK: ATURN0%d start\012", local_F4 - 140);
      script_C6FF(task_298);
      script_D2D4(local_F4);
      Script_sprintf("FTLK: ATURN0%d end\012", local_F4 - 140);
    } else if ((local_F4 >= 40) & (local_F4 <= 49)) {
      Script_sprintf("FTLK: TURN0%d start\012", local_F4 - 40);
      script_C6FF(task_298);
      script_D15F(local_F4);
      Script_sprintf("FTLK: TURN0%d end\012", local_F4 - 40);
    } else if ((local_F4 >= 50) & (local_F4 <= 59)) {
      Script_sprintf("FTLK: RETURN0%d start\012", local_F4 - 50);
      script_C6FF(task_298);
      script_D3F1(local_F4);
      Script_sprintf("FTLK: RETURN0%d end\012", local_F4 - 50);
    } else if (((local_F4 >= 20) & (local_F4 <= 29)) | ((local_F4 >= 120) & (local_F4 <= 129))) {
      if (local_F4 < 120) {
        Script_sprintf("FTLK: POINT0%d start\012", local_F4 - 20);
      } else {
        Script_sprintf("FTLK: APOINT0%d start\012", local_F4 - 120);
      }
      script_C6FF(task_298);
      script_DAA3(task_298, local_F4);
      if (local_F4 < 120) {
        Script_sprintf("FTLK: POINT0%d end\012", local_F4 - 20);
      } else {
        Script_sprintf("FTLK: APOINT0%d end\012", local_F4 - 120);
      }
    } else {
      local_E8 = local_F4;
      if (local_E8 == -1) {
        Script_sprintf("FTLK: START start\012");
        *(&task_2DC + 1) = 101;
        script_C6FF(task_298);
        if (local_EE & -(~task_327 != 0)) {
          Vector3i object_0;
          Script_mtwkGetSetRotation(task_298, &object_0, 0);
          task_310 = (&object_0)->y;
          task_314 = Script_scenCharaGetMotAnimId(task_298);
          task_318 = task_298;
        }
        (void)((task_327 = -1) & 2);
        if (task_2E8 != -1) {
          Script_srchControl((void *)task_2E8);
          Script_sprintf("FTLK: Wait finish canseled(%d)\012", task_2E8);
          task_2E8 = -1;
        }
        task_2E8 = -1;
        if (~task_325 & ~task_326) {
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
        if (~Script_scenUtils(SCENARIO_CMD_QUERY_ACTIVE_FLAG_0x400000) & -(task_300 == -1) & local_EE & ~task_326) {
          script_D9CE(CHARA_AKIR, task_298, (const uint16_t *)&data_117B8);
          if (*task_2BC == -1) {
            if (task_298 == 0x4952554B) {
              Script_sprintf("FTLK: AKIRA ->> %s\012", script_BB09(task_298));
              task_2BC[0] = Script_scenCreateTask(script_B93A, 3, CHARA_AKIR, task_298, local_F0);
            } else if (Script_CycleManagerUtils(CYCLE_CMD_SIMSET_GLOBALFLAG_CHECK, task_298) == 1) {
              Script_sprintf("FTLK: AKIRA ->> %s\012", script_BB09(task_298));
              task_2BC[0] = Script_scenCreateTask(script_B93A, 3, CHARA_AKIR, task_298, local_F0);
            } else {
              Script_sprintf("FTLK: AKIRA -> %s\012", script_BB09(task_298));
              if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_298)) {
                Vector3 object_1;
                Script_EVT_GetPosition(task_298, 4, &object_1, ENTITY_XFORM_WORLD_SPACE);
                Script_SetLookPointDetails(CHARA_AKIR, 16, &object_1, local_F0);
                Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &object_1, 30);
              }
            }
          }
        }
        SCNF_CopyScenario_000E(1, task_2F8, (const void *)local_F8, 8);
        script_D119();
        Script_sprintf("FTLK: START end\012");
      } else if (local_E8 == -2) {
        Script_sprintf("FTLK: END start\012");
        task_2DC = -1;
        *(&task_2DC + 1) = 102;
        if (local_EE & 0x40) {
          Script_SetRyoMotionDetails(task_298);
        }
        while (task_304 != -1) {
          SCNF_WaitFrames_0000(1);
        }
        while (task_324 & -(task_308 != -1)) {
          SCNF_WaitFrames_0000(1);
        }
        local_F8[1] = -2;
        SCNF_CopyScenario_000E(1, task_2F8, (const void *)local_F8, 8);
        (void)((task_326 = 0) & 0x40);
        script_C6FF(task_298);
        script_D0F9();
        Script_sprintf("FTLK: END end\012");
      } else if (local_E8 == -8) {
        Script_sprintf("FTLK: FINISH start\012");
        if ((task_298 == 0x4155524D) | (task_298 == 0x444E4953)) {
          Script_PlayCharaMotion(task_298, task_314, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        } else {
          if (task_327 & 2) {
            script_D5C1(task_298);
            goto L00CD56;
          }
        }
        L00CD56:;
        local_F8[1] = -8;
        Script_sprintf("FTLK: FINISH send (%s)\012", script_BB09(task_298));
        SCNF_CopyScenario_000E(1, task_2F8, (const void *)local_F8, 8);
        script_D13C();
        (void)((task_327 = 0) & 1);
        *(&task_2DC + 1) = 103;
        Script_sprintf("FTLK: FINISH end\012");
      } else if (local_E8 == -3) {
        *(&task_2DC + 1) = 104;
      } else if (local_E8 == -4) {
        *(&task_2DC + 1) = 105;
      } else if (local_E8 == -5) {
        *(&task_2DC + 1) = 106;
      } else if (local_E8 == -6) {
        *(&task_2DC + 1) = 107;
      } else if (local_E8 == -7) {
        *(&task_2DC + 1) = 108;
      } else if (local_E8 == 13) {
        Script_sprintf("FTLK: POINT_END star\012");
        script_E10B(task_298);
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        }
        Script_sprintf("FTLK: POINT_END end\012");
        SCNF_CopyScenario_000E(1, task_2F8, (const void *)local_F8, 8);
      } else if (local_E8 == 14) {
        Script_sprintf("FTLK: APOINT_END start\012");
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_298)) {
          Script_SetCharaLookAtPosition(task_298, -13, 0, 0);
          Script_FaceLookAtTargetUtils(task_298, FACE_LookReset);
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        }
        Script_sprintf("FTLK: APOINT_END end\012");
        SCNF_CopyScenario_000E(1, task_2F8, (const void *)local_F8, 8);
      } else if (local_E8 == 10) {
        Script_sprintf("FTLK: SHOW_MEMO start\012");
        script_E36F();
        Script_sprintf("FTLK: SHOW_MEMO end\012");
        SCNF_CopyScenario_000E(1, task_2F8, (const void *)local_F8, 8);
      } else if (local_E8 == 15) {
        Script_sprintf("FTLK: SHOW_END start\012");
        script_E4AC();
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_298)) {
          Vector3 object_2;
          Script_EVT_GetPosition(task_298, 4, &object_2, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_AKIR, 16, &object_2, 2);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &object_2, 30);
        }
        Script_sprintf("FTLK: SHOW_END end\012");
        SCNF_CopyScenario_000E(1, task_2F8, (const void *)local_F8, 8);
      } else if (local_E8 == 60) {
        Script_sprintf("FTLK: WAIT start\012");
        SCNF_WaitFrames_0000(60);
        Script_sprintf("FTLK: WAIT end\012");
        SCNF_CopyScenario_000E(1, task_2F8, (const void *)local_F8, 8);
      } else {
        Script_sprintf("FTLK: EXTRA start\012", local_F4);
        *(&task_2DC + 1) = 109;
        *(&task_2DC + 2) = local_F4;
        script_C5E3();
        *(&task_2DC + 2) = -1;
        Script_sprintf("FTLK: EXTRA end\012", local_F4);
        SCNF_CopyScenario_000E(1, task_2F8, (const void *)local_F8, 8);
      }
    }
    *(&task_2DC + 1) = -1;
  }
  end:;
}

static void script_D02D(void) {
  (void)((task_322 = 0) & 0x80);
  return;
}

static void script_D03B(void) {
  (void)*&task_2DC;
  return;
}

static void script_D043(void) {
  (void)*(&task_2DC + 1);
  return;
}

static int32_t script_D04E(void) {
  return *(&task_2DC + 2);
}

static int32_t script_D059(void) {
  int32_t unused_FC;
  for (;;) {
    if (~task_327 | (Script_GetControllerInput(0) & 0xF244) | (Script_GetControllerInput(5) != 128) | (Script_GetControllerInput(6) != 128)) {
      script_DE24();
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
        Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
      }
      Script_SetMotnFlag(CHARA_AKIR, 0);
      script_D13C();
      script_C6D4();
      Script_sprintf("FTLK: Wait finish end\012");
      task_2E8 = -1;
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_D0F9(void) {
  task_2E8 = Script_scenCreateTask(script_D059, 0);
  Script_sprintf("FTLK: Wait finish start(%d)\012", task_2E8);
  return;
}

static void script_D119(void) {
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  return;
}

static void script_D13C(void) {
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  return;
}

static void script_D15F(int32_t arg0) {
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
    local_FC = task_29C[local_F2];
    local_F8 = task_29C[local_F1];
  }
  if ((Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC) == 0) | (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_F8) == 0)) {
    script_C608(task_298);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC) != 0) {
      local_FC = local_F8;
    }
    Script_sprintf("FTLK: %s not exists\012", script_BB09(local_FC));
  } else {
    int32_t unused_F4;
    int8_t local_F3;
    local_F3 = script_C7D1(local_FC);
    if (task_2BC[local_F3] != -1) {
      Script_srchControl((void *)task_2BC[local_F3]);
    }
    Script_sprintf("FTLK: %s ->> ", script_BB09(local_FC));
    Script_sprintf("%s\012", script_BB09(local_F8));
    task_2BC[local_F3] = Script_scenCreateTask(script_B93A, 3, local_FC, local_F8, 2);
    script_D45C(local_FC, local_F8);
    if ((local_FC != 0x4155524D) & (local_FC != 0x444E4953)) {
      Script_scenCreateTask(script_D54A, 1, local_FC);
    } else {
      Script_PlayCharaMotion(local_FC, Script_CycleManagerUtils(CYCLE_CMD_GET_VALUE_BY_CYCLE_ID, local_FC), -1.0f, -1.0f, -1.0f, 1.0f, 0);
    }
    script_C608(task_298);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_11B39, 1, data_11B49)))
static void script_D2D4(int32_t arg0) {
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
  script_DE18();
  local_FC = task_29C[arg0 - 140 + 1];
  if ((local_FC != -1) & (*task_2BC == -1)) {
    Script_sprintf("FTLK: AKIRA -> %s\012", script_BB09(local_FC));
    if (Script_CycleManagerUtils(CYCLE_CMD_SIMSET_GLOBALFLAG_CHECK, local_FC) == 1) {
      task_2BC[0] = Script_scenCreateTask(script_B93A, 3, CHARA_AKIR, local_FC, local_F8);
    } else {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC)) {
        Script_EVT_GetPosition(local_FC, 4, &local_EC, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(CHARA_AKIR, 16, &local_EC, local_F8);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 30);
      }
    }
  } else {
    Script_sprintf("FTLK: AKIRA -X %s\012", script_BB09(local_FC));
  }
  script_C608(task_298);
  return;
}

static void script_D3F1(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0 - 50 + 1;
  local_FC = task_29C[local_F8];
  if (task_2BC[arg0 - 50 + 1] != -1) {
    script_DD8A(local_F8);
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, local_FC)) {
      Script_SetCharaLookAtPosition(local_FC, -13, 0, 0);
      Script_FaceLookAtTargetUtils(local_FC, FACE_LookReset);
    }
  }
  script_C608(task_298);
  return;
}

static void script_D45C(int32_t arg0, int32_t arg1) {
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
    task_30C = (&local_DC)->y;
  } else if (local_D0 < 0) {
    local_D4 = 0x10000 - local_D4;
    task_30C = ((&local_DC)->y - local_D4 * 6 / 10) & 0xFFFF;
  } else {
    task_30C = (local_D4 * 6 / 10 + (&local_DC)->y) & 0xFFFF;
  }
  (void)0;
  return;
}

static int32_t script_D54A(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_VALUE_BY_CYCLE_ID, arg0);
  if ((local_FC == -1) | (local_FC == 0)) {
    local_FC = Script_scenCharaGetMotAnimId(arg0);
  }
  Script_sprintf("FTLK: %s 's talk motion is %d\012", script_BB09(arg0), local_FC);
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_SIMSET_MODE01, arg0) == 0) {
    script_D609(arg0, task_30C, 1043, local_FC);
  } else {
    script_D609(arg0, task_30C, 32514, local_FC);
  }
  end:;
}

static void script_D5C1(int32_t arg0) {
  if (arg0 != task_318) {
  } else if (Script_CycleManagerUtils(CYCLE_CMD_GET_SIMSET_MODE01, arg0) == 0) {
    script_D609(arg0, task_310, 1043, task_314);
  } else {
    script_D609(arg0, task_310, 32514, task_314);
  }
  return;
}

static void script_D609(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  (void)((task_323 = -1) & 1);
  *(int32_t *)&local_D8 = task_31C;
  task_31C = 0x3F4CCCCD;
  local_D4 = 1.0f / local_D8;
  if (~Script_scenCheckCharaExist(arg0)) {
    Script_sprintf("FTLK: Turn canseled 1\012");
    (void)((task_323 = 0) & 1);
  } else {
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    if ((&local_E8)->y == arg1) {
      Script_motmToggleFlags(arg0, SCEN_Enable, 0x40);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_18);
      Script_PlayCharaMotion(arg0, arg3, 0.0f, 0.0f, 0.0f, 1.0f, 0);
      Script_sprintf("FTLK: Set motion %d to %s 1\012", arg3, script_BB09(arg0));
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
        local_E4 = script_D956((&local_E8)->y - 0x4000, arg1);
      } else {
        local_E4 = script_D956((&local_E8)->y + 0x4000, arg1);
      }
      SCNF_WaitFrames_0000((int32_t)((float)local_DC / 2.0f));
      if (~Script_scenCheckCharaExist(arg0)) {
        Script_sprintf("FTLK: Turn canseled 2\012");
        (void)((task_323 = 0) & 0x10);
        goto L00D955;
      }
      Script_motmUtilsEx(6, arg0, local_E4);
      Script_motmToggleFlags(arg0, SCEN_Disable, 4);
      Script_motmUtilsEx(0, arg0, 1);
      Script_PlayCharaMotion(arg0, arg2, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = (float)local_FC - local_E0}).i, ((scn3_bits){.f = local_E0}).i, local_D8, 0);
      if (Script_motmUtilsEx(7, 0, arg3)) {
        SCNF_WaitFrames_0000((int32_t)((float)local_FC * local_D4) - (int32_t)local_E0 * 2);
        if (~Script_scenCheckCharaExist(arg0)) {
          Script_sprintf("FTLK: Turn canseled 3\012");
          (void)((task_323 = 0) & 0x10);
          goto L00D955;
        }
        Script_PlayCharaMotion(arg0, arg3, 0.0f, 0.0f, 0.0f, local_D8, 0);
        Script_sprintf("FTLK: Set motion %d to %s 2\012", arg3, script_BB09(arg0));
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
          (void)((task_323 = 0) & 2);
          goto L00D955;
        }
        Script_motmUtilsEx(0, arg0, 1);
        Script_PlayCharaMotion(arg0, arg3, 0.0f, 0.0f, 0.0f, local_D8, 0);
        Script_sprintf("FTLK: Set motion %d to %s 3\012", arg3, script_BB09(arg0));
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
          (void)((task_323 = 0) & 1);
          goto L00D955;
        }
        Script_motmUtilsEx(0, arg0, local_F8);
      }
    }
    (void)((task_323 = 0) & 1);
  }
  L00D955:;
  return;
}

static int32_t script_D956(int32_t arg0, int32_t arg1) {
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

static void script_D9CE(int32_t arg0, int32_t arg1, const uint16_t *arg2) {
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

static void script_DAA3(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  int32_t local_D8;
  int32_t local_D4;
  (void)((task_324 = -1) & 0x10);
  {
    int32_t local_D0;
    local_D0 = script_C7D1(arg0);
    if (local_D0 != 8) {
      script_DD8A(local_D0);
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
    script_C608(task_298);
  } else {
    *(int32_t *)&local_E4 = Script_ScenDataUtil((void *)local_D4, SCND_ReadU32, 0);
    *(int32_t *)&local_E0 = Script_ScenDataUtil((void *)local_D4, SCND_ReadU32, 1);
    *(int32_t *)&local_DC = Script_ScenDataUtil((void *)local_D4, SCND_ReadU32, 2);
    Script_sprintf("FTLK: POINT TO (%f, %f, %f)\012", local_E4, local_E0, local_DC);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    local_D8 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, local_E4 - (&local_F4)->x, local_DC - (&local_F4)->z);
    local_D8 = script_D956((&local_E8)->y, local_D8) & 0xFFFF;
    if (local_D8 < 0x8000) {
      task_30C = (&local_E8)->y + local_D8 - 0x4000;
    } else {
      task_30C = (&local_E8)->y + local_D8 + 0x4000;
    }
    if ((local_D8 >= 21845) & (local_D8 <= 43690)) {
      Script_scenCreateTask(script_D54A, 1, arg0);
      task_31C = 0x3FC00000;
      SCNF_WaitFrames_0000(1);
    }
    script_C608(task_298);
    while (task_323 & 0x20) {
      SCNF_WaitFrames_0000(1);
    }
    (void)((task_323 = 0) & 0x10);
    task_31C = 0x3F4CCCCD;
    if (arg1 < 120) {
      script_E0E7(arg0, 15, ((scn3_bits){.f = local_E4}).i, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = local_DC}).i, 2, 75, -1);
    } else {
      script_DEE9(arg0, ((scn3_bits){.f = local_E4}).i, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = local_DC}).i, 75);
    }
    SCNF_WaitFrames_0000(30);
    script_DE18();
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
      Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    }
    if (task_325 & 0x80) {
      script_DEE9(CHARA_AKIR, ((scn3_bits){.f = local_E4}).i, ((scn3_bits){.f = local_E0}).i, ((scn3_bits){.f = local_DC}).i, 75);
    } else {
      script_DCE3(CHARA_AKIR, local_E4, local_E0, local_DC, (const uint16_t *)&data_117B8);
    }
  }
  return;
}

static void script_DCE3(int32_t arg0, float arg1, float arg2, float arg3, const uint16_t *arg4) {
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
  (void)((task_324 = 0) & 0x10);
  return;
}

static void script_DD8A(int32_t arg0) {
  if (task_2BC[arg0] == -1) {
  } else {
    Script_srchControl((void *)task_2BC[arg0]);
    SCNF_WaitFrames_0000(1);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, task_29C[arg0])) {
      Script_SetCharaLookAtPosition(task_29C[arg0], -13, 0, 0);
      Script_FaceLookAtTargetUtils(task_29C[arg0], FACE_LookReset);
    }
    Script_sprintf("FTLK: %s finished looking\012", script_BB09(task_29C[arg0]));
    task_2BC[arg0] = -1;
  }
  return;
}

static void script_DE18(void) {
  script_DD8A(0);
  return;
}

static void script_DE24(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 8) {
    script_DD8A(local_FF);
    local_FF = local_FF + 1;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_11CAE, 1, data_11CC2)))
static int32_t script_DE4B(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
    Script_sprintf("FTLK: %s not exist\012", script_BB09(arg0));
  } else {
    Script_SetCharaLookAtPosition(arg0, 10, &local_F4, 0);
    SCNF_WaitFrames_0000(arg4);
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
      Script_sprintf("FTLK: %s not exist\012", script_BB09(arg0));
    } else {
      if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0)) {
        Script_SetCharaLookAtPosition(arg0, -13, 0, 0);
        Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
      }
    }
  }
  end:;
}

static void script_DEE9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_DE4B, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

__attribute__((scn_string_bindings(0, data_11CD6, 1, data_11CEA, 2, data_11CFA)))
static int32_t script_DF01(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5, int32_t arg6, int32_t arg7) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int8_t local_E7;
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, arg0) == 0) {
    Script_sprintf("FTLK: %s not exist\012", script_BB09(arg0));
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
      Script_sprintf("FTLK: %s not exist\012", script_BB09(arg0));
    } else {
      script_E10B(arg0);
      if (local_EC == -3) {
        Script_CharHandlers(arg0, -1, 0, 0);
      } else {
        Script_CharHandlers(arg0, local_F0, local_EC, 20);
      }
      if (arg7 != -1) {
        Script_PlayCharaMotion(arg0, arg7, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (task_327 & 2) {
        if (arg0 == 0x52494B41) {
          if (*task_2BC == -1) {
            task_2BC[0] = Script_scenCreateTask(script_B93A, 3, CHARA_AKIR, task_298, local_E8);
          }
        } else {
          local_E7 = script_C7D1(arg0);
          if (task_2BC[local_E7] == -1) {
            task_2BC[local_E7] = Script_scenCreateTask(script_B93A, 3, arg0, CHARA_AKIR, 2);
          }
        }
      }
      task_308 = -1;
    }
  }
  end:;
}

static void script_E0E7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7) {
  task_308 = Script_scenCreateTask(script_DF01, 8, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
  return;
}

static void script_E10B(int32_t arg0) {
  Script_HandleLookPoint(arg0, -98312, 0, 2);
  return;
}

static void script_E11E(float arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = arg2;
  Script_convCtrlCamApply(1);
  Script_RunAuthorAndSetState(0, arg3, 0, 0, 0, &local_F4, 0, 0.0f, 0, 0);
  return;
}

static void script_E159(const Vector3 *arg0, float arg1) {
  Script_convCtrlCamApply(1);
  Script_RunAuthorAndSetState(0, arg1, 0, 0, 0, arg0, 0, 0.0f, 0, 0);
  return;
}

static void script_E17C(void) {
  Script_convCtrlCamApply(0);
  return;
}

static void script_E186(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  task_320 = arg1;
  local_FC = arg1;
  if (local_FC == 1) {
    task_2FC = arg0;
    Script_SetImageModuleTaskDataFlag(arg0, 0);
  } else {
    task_320 = 0;
  }
  return;
}

static int32_t script_E1B9(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x40);
  if (task_325 & 0x80) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_CHUGOSI_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_MID_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  if (task_320 == 1) {
    local_F4.x = 0.091f;
    (&local_F4)->y = 0.052f;
    (&local_F4)->z = 0.016f;
    local_E8.x = 0xA000;
    (&local_E8)->y = 0x5000;
    (&local_E8)->z = 0x5000;
    SCNF_WaitFrames_0000(30);
    if (0) {
      Script_charaDisplay(task_2FC, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_2FC, CDM_Show);
        goto L00E2BE;
      }
    }
    L00E2BE:;
    Script_scenToggleVisibleFor(task_2FC, 1);
    Script_fixoAttachEx(task_2FC, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x40);
  if (task_325 & 0x80) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_CHUGOSI_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  return task_300 = -1;
}

static void script_E36F(void) {
  task_300 = Script_scenCreateTask(script_E1B9, 0);
  return;
}

static int32_t script_E383(void) {
  if (task_300 != -1) {
    Script_srchControl((void *)task_300);
    task_300 = -1;
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 4);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
  if (task_325 & 0x80) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_CHUGOSI_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEND_LETTER_MID_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  if (task_2FC != 0) {
    SCNF_WaitFrames_0000(25);
    Script_fixoClearAttachment(task_2FC);
    if (-1) {
      Script_charaDisplay(task_2FC, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_2FC, CDM_Show);
        goto L00E459;
      }
    }
    L00E459:;
    Script_scenToggleVisibleFor(task_2FC, 0);
  }
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return task_304 = -1;
}

static void script_E4AC(void) {
  task_304 = Script_scenCreateTask(script_E383, 0);
  return;
}

static int32_t script_E4C0(int32_t arg0) {
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 8);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_F7D4, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_E59C();
  end:;
}

static int32_t script_E541(void) {
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_F7D4, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 8);
  end:;
}

static void script_E59C(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  for (;;) {
    Script_EVT_GetPosition(task_298, 4, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_E8)->y - (&local_F4)->y <= 0.3f)) {
      script_E63F();
      break;
    }
    if (~task_325) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_E5F6(int32_t arg0) {
  if (task_325 & 0x80) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_scenCreateTask(script_E4C0, 1, arg0);
    (void)((task_325 = -1) & 0x10);
  }
  return;
}

static void script_E63F(void) {
  Script_scenCreateTask(script_E541, 0);
  (void)((task_325 = 0) & 0x10);
  return;
}

static void script_E657(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int32_t local_CC;
  int8_t local_CB;
  local_CC = 0;
  (void)((local_CB = 0) & 0x10);
  script_DD8A(1);
  Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 10, 0);
  Script_motmToggleFlags(CHARA_NITO, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_NITO, MOTM_BLEND_FRAMES_10);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  if (~Script_scenCheckCharaExist(CHARA_NITO)) {
    local_D8 = script_F74A(0x4F54494E);
    (void)((local_CB = -1) & 0x40);
  }
  Script_EVT_GetPosition(CHARA_NITO, -1, &local_E8, 0);
  script_B8BE(CHARA_AKIR, 3.8f, 0.0f, -6.6f);
  script_B8E6(CHARA_AKIR, 0.0f, 270.0f, 0.0f);
  script_B8BE(CHARA_NITO, 4.95f, 0.0f, -6.44f);
  script_B8E6(CHARA_NITO, 0.0f, 90.0f, 0.0f);
  script_EDD9();
  Script_seqEnterEvtCam(100, 0, AUTH_None);
  script_D02D();
  if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
    uint8_t unused_C8[3];
    Vector3 local_BC;
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_BC, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_NITO, 16, &local_BC, 2);
    Script_FaceLookAtTargetUtils(CHARA_NITO, FACE_LookAtPosition, &local_BC, 30);
  }
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 756);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 756, 8, 8, 8, 4, 1.0f, 0);
  Script_scenCreateTask(script_EF00, 0);
  Script_SetCPAMFlags(CHARA_NITO, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('MEIS'), 1);
  for (;;) {
    uint8_t unused_C8[3];
    int32_t local_C4;
    local_C4 = Script_scenUtils(SCENARIO_CMD_QUERY_ACTIVE_RUNTIME_OBJECT, 0);
    switch (local_C4) {
      Vector3 local_B8;
      case 0x30313041:
        if (local_CC != 0x30313041) {
          if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
            Script_EVT_GetPosition(CHARA_AKIR, 4, &local_B8, ENTITY_XFORM_WORLD_SPACE);
            Script_SetLookPointDetails(CHARA_NITO, 16, &local_B8, 2);
            Script_FaceLookAtTargetUtils(CHARA_NITO, FACE_LookAtPosition, &local_B8, 30);
          }
        }
        break;
      case 0x31313042:
        if (local_CC != 0x31313042) {
          local_CC = 0x31313042;
          local_F4.x = 4.996f;
          (&local_F4)->y = 1.552f;
          (&local_F4)->z = -6.539f;
          Script_SetCharaLookAtPosition(CHARA_AKIR, 16, &local_F4, 2);
          if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E)) {
            Script_SetCharaLookAtPosition(CHARA_NITO, -10, 0, 0);
            Script_FaceLookAtTargetUtils(CHARA_NITO, FACE_LookReset);
          }
          local_D0 = Script_scenCreateTask(script_EE14, 0);
          Script_PlayCharaMotion(CHARA_NITO, 24577, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_SetCharaMotionPlaybackRate(CHARA_NITO, 0.7f);
          SCNF_WaitFrames_0000(1);
          SCNF_WaitFrames_0000(18);
          Script_charaDisplay(SHEN_NAME('MEIS'), CDM_Show);
          local_F4.x = 0.152f;
          (&local_F4)->y = -0.0013f;
          (&local_F4)->z = -0.037f;
          local_DC.x = 11829;
          (&local_DC)->y = 26945;
          (&local_DC)->z = 33122;
          Script_fixoAttachEx(SHEN_NAME('MEIS'), CHARA_NITO, 18, &local_F4, &local_DC);
          SCNF_WaitFrames_0000(32);
          if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
            Script_EVT_GetPosition(CHARA_AKIR, 4, &local_B8, ENTITY_XFORM_WORLD_SPACE);
            Script_SetLookPointDetails(CHARA_NITO, 16, &local_B8, 2);
            Script_FaceLookAtTargetUtils(CHARA_NITO, FACE_LookAtPosition, &local_B8, 30);
          }
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_GET_LETTER, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(21);
          local_D4 = Script_scenCreateTask(script_EE44, 5, CHARA_AKIR, 0x4087B22D, 0x3FAD7A78, -0x3F342752, 0x41A00000);
        }
        break;
      case 0x31313041:
        if (local_CC != 0x31313041) {
          local_CC = 0x31313041;
          local_F4.x = 0.1261f;
          (&local_F4)->y = 0.0195f;
          (&local_F4)->z = -0.0904f;
          local_DC.x = 11191;
          (&local_DC)->y = 44564;
          (&local_DC)->z = 46835;
          Script_fixoAttachEx(SHEN_NAME('MEIS'), CHARA_AKIR, 18, &local_F4, &local_DC);
          Script_seqEnterEvtCam(115, CHARA_AKIR, CHARA_AKIR);
          Script_motmToggleFlags(CHARA_NITO, SCEN_Disable, 0x40);
          Script_PlayCharaMotion(CHARA_NITO, 24578, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_SetCharaMotionPlaybackRate(CHARA_NITO, 1.0f);
          Script_srchControl((void *)local_D0);
          local_D0 = -1;
          Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
          if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E)) {
            Script_EVT_GetPosition(CHARA_NITO, 4, &local_B8, ENTITY_XFORM_WORLD_SPACE);
            Script_SetLookPointDetails(CHARA_AKIR, 16, &local_B8, 2);
            Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_B8, 30);
          }
        }
        break;
      case 0x32313042:
        if (local_CC != 0x32313042) {
          local_CC = 0x32313042;
          if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E) & Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
            Script_EVT_GetPosition(CHARA_AKIR, 4, &local_B8, ENTITY_XFORM_WORLD_SPACE);
            Script_SetLookPointDetails(CHARA_NITO, 16, &local_B8, 2);
            Script_FaceLookAtTargetUtils(CHARA_NITO, FACE_LookAtPosition, &local_B8, 30);
          }
        }
        break;
      case 0x32313041:
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
        Script_srchControl((void *)local_D4);
        local_D4 = -1;
        if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x52494B41)) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -10, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        }
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_KEEP_LETTER, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.4f);
        Script_GetCassetteFlags(34, 1, 4);
        Script_scenCreateTask(script_EDFD, 0);
        SCNF_WaitFrames_0000(30);
        if (-1) {
          Script_charaDisplay(SHEN_NAME('MEIS'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('MEIS'), CDM_Show);
            goto L00EC9F;
          }
        }
        L00EC9F:;
        Script_scenToggleVisibleFor(SHEN_NAME('MEIS'), 0);
        Script_EVT_SetPosition(CHARA_NITO, POS_XYZ, &local_E8);
        while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        goto L00ECF6;
        break;
    }
    if (task_328 > 900) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L00ECF6:;
  SCNF_WaitFrames_0000(1);
  if (local_D4 != -1) {
    Script_srchControl((void *)local_D4);
  }
  if (local_D0 != -1) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
    Script_srchControl((void *)local_D0);
  }
  Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_NITO, 1.0f);
  Script_motmToggleFlags(CHARA_NITO, SCEN_Enable, 0x40);
  Script_motmSetBlendFrames(CHARA_NITO, MOTM_BLEND_FRAMES_5);
  Script_SetCPAMFlags(CHARA_NITO, 1);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MEIS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MEIS'), CDM_Show);
      goto L00EDA6;
    }
  }
  L00EDA6:;
  Script_scenToggleVisibleFor(SHEN_NAME('MEIS'), 0);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  if (local_CB & 8) {
    script_F7BA(0x4F54494E, local_D8);
  }
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 1);
  return;
}

static void script_EDD9(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_NITO, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 20, &local_F4, 2);
  return;
}

static int32_t script_EDFD(void) {
  Script_ProcessSubtitle("JINGURU01", 0);
  SCNF_StartIconSystemMenu(34);
  end:;
}

static int32_t script_EE14(void) {
  Vector3 local_F4;
  for (;;) {
    Script_EVT_GetPosition(CHARA_NITO, 18, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 2);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_EE44(int32_t arg0, float arg1, float arg2, float arg3, float arg4) {
  Vector3 local_F4;
  float local_E8[3];
  float local_E4;
  float local_E0;
  float local_DC;
  float local_D8;
  local_E8[0] = arg1;
  local_E8[1] = arg2;
  local_E8[2] = arg3;
  Script_EVT_GetPosition(arg0, 17, &local_F4, 0);
  local_E4 = (*local_E8 - (&local_F4)->x) / arg4;
  local_E0 = (local_E8[1] - (&local_F4)->y) / arg4;
  local_DC = (local_E8[2] - (&local_F4)->z) / arg4;
  local_D8 = 0.0f;
  for (;;) {
    Script_SetMotnFlags(arg0, 17, &local_F4, 0x38000000);
    if (!(local_D8 >= arg4)) {
      local_F4.x = (&local_F4)->x + local_E4;
      (&local_F4)->y = (&local_F4)->y + local_E0;
      (&local_F4)->z = (&local_F4)->z + local_DC;
      local_D8 = local_D8 + 1.0f;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_EF00(void) {
  int32_t local_FC;
  task_328 = 0;
  for (;;) {
    local_FC = task_328;
    switch (local_FC) {
      case 0:
        Script_FACE(CHARA_AKIR, 2, 1, 60);
        break;
      case 76:
        Script_seqEnterEvtCam(110, CHARA_AKIR, CHARA_AKIR);
        break;
      case 271:
        Script_seqEnterEvtCam(120, CHARA_AKIR, CHARA_AKIR);
        break;
      case 301:
        Script_seqEnterEvtCam(130, CHARA_NITO, CHARA_NITO);
        break;
      case 387:
        Script_FACE(CHARA_AKIR, 1, 0, 30);
        Script_seqEnterEvtCam(140, 0, AUTH_None);
        break;
      case 420:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        Script_seqEnterEvtCam(150, 0, AUTH_None);
        goto L00EFEC;
        break;
    }
    task_328 = task_328 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00EFEC:;
}

void script_EFED(void) {
  task_0 = -1;
  (void)((task_5 = -1) & 2);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_motiLoad(80, SHEN_NAME('TIKN'), "M_MOBJ.BIN");
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/80/GSEN", (int32_t)"SCRTEST.BIN", 0x4E455347);
  Script_uiUtils(1, 14, 0);
  Script_LoadVoiceFile(1, "ALLVOICE");
  script_26C7(CHARA_AKIR, (DSWKDoorEntry *)&data_11754, (int32_t *)data_11770, (DoorResourceDescriptor *)&data_11784, (MapObjectRecord *)&data_11794, 10);
  Script_scenCreateTask(script_F249, 1, CHARA_AKIR);
  Script_scenCreateTask(script_F0A4, 0);
  script_C806();
  return;
}

__attribute__((scn_string_bindings(0, data_11D46, 2, data_11D5D)))
static int32_t script_F0A4(void) {
  Vector3 local_F4;
  (void)((task_4 = 0) & 2);
  while (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/80/GSEN", "MODELS2") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_292A();
  script_4AB3();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, -1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  local_F4.x = 0.1f;
  (&local_F4)->y = 0.1f;
  (&local_F4)->z = 0.1f;
  if (0) {
    Script_charaDisplay(SHEN_NAME('YENS'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YENS'), CDM_Show);
      goto L00F182;
    }
  }
  L00F182:;
  Script_scenToggleVisibleFor(SHEN_NAME('YENS'), 1);
  Script_hmdlApplyScale(SHEN_NAME('YENS'), &local_F4);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('YENS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('YENS'), CDM_Show);
      goto L00F1C1;
    }
  }
  L00F1C1:;
  Script_scenToggleVisibleFor(SHEN_NAME('YENS'), 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/80/GSEN", "MOTION") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, -1);
  Script_LoadHandMot(80, SHEN_NAME('GSEN'), "HMOT_GAM.BIN");
  Script_facemotUtils(SHEN_NAME('YKUR'), 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  return (task_4 = -1) & 0x20;
}

static int32_t script_F249(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int8_t local_DB;
  int8_t local_DA;
  Script_scenCreateTask(script_00CC, 0);
  script_0030(arg0);
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, -0.6f, -8.6f, 6.4f, -8.6f, 6.4f, 1.0f, -0.6f, 1.0f, -0.1f, 3.0f);
  Script_uiUtils(0, 44, 0);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  Script_uiUtils(0, 44, 0);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 3);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 7);
  (void)((local_DB = 0) & 0x40);
  (void)((local_DA = 0) & 0x40);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_28C7();
  Script_scenCreateTask(script_0341, 2, 1, 995);
  for (;;) {
    Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_DC, 0x42000000);
    if (!((&local_F4)->z <= 0.0f)) {
      (void)((task_5 = 0) & 1);
      (void)((task_6 = 0) & 1);
      script_010E();
      Script_shenRequestGameModuleTransition(80, SHEN_NAME('TIKN'), 3);
    }
    if (task_0 == -1) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        script_019A();
      }
    } else {
      script_019A();
    }
    local_E8.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y) * 0.3f;
    (&local_E8)->y = (&local_F4)->y;
    (&local_E8)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y) * 0.3f;
    if (task_4 & 0x10) {
      if (local_DB & 8) {
        if (Script_ryoControl(PCCTL_GetTatehiza) != 0) {
          (void)((local_DA = -1) & 0x10);
          SCNF_RequestIconNotification(0, 0, 0, 2);
        } else {
          if (local_DA & 4) {
            (void)((local_DA = 0) & 0x10);
            SCNF_RequestIconNotification(0x100, 0, 0, 1);
            goto L00F414;
          }
        }
      }
      L00F414:;
      if (Script_CheckTargetWithinAngleRange(&local_E8, 1.4f, (&local_DC)->y, 7281, &data_11D80)) {
        if (~local_DB) {
          (void)((local_DB = -1) & 0x10);
          if (task_6 & 0x40) {
            (void)((task_7 = 0) & 0x10);
          } else {
            Script_sicnUtils(0, 0, 1);
          }
          SCNF_RequestIconNotification(0x100, 0, 0, 1);
          Script_uiUtils(0, 3, 0);
        } else {
          if (Script_GetControllerInput(1) & 0x400 & ~local_DA) {
            (void)((local_DB = 0) & 0x10);
            if (task_6 & 0x40) {
              (void)((task_7 = 0) & 0x10);
            }
            SCNF_RequestIconNotification(0, 0, 0, 2);
            task_0 = Script_scenCreateTask(script_F714, 1, 0);
            while (task_0 != -1) {
              SCNF_WaitFrames_0000(1);
              script_019A();
            }
            goto L00F4ED;
          }
        }
        L00F4ED:;
      } else if (Script_CheckTargetWithinAngleRange(&local_E8, 1.3f, (&local_DC)->y, 7281, &data_11D8C) & -((&task_20)->mode != 2)) {
        if (~local_DB) {
          (void)((local_DB = -1) & 1);
          if (task_6 & 0x40) {
            (void)((task_7 = 0) & 1);
          } else {
            Script_sicnUtils(0, 0, 1);
          }
          SCNF_RequestIconNotification(0x100, 0, 0, 1);
          Script_uiUtils(0, 30, 0);
        } else {
          if (Script_GetControllerInput(1) & 0x400 & ~local_DA) {
            (void)((local_DB = 0) & 1);
            if (task_6 & 0x40) {
              (void)((task_7 = 0) & 1);
            }
            SCNF_RequestIconNotification(0, 0, 0, 2);
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            task_0 = Script_scenCreateTask(script_F714, 1, 1);
            while (task_0 != -1) {
              SCNF_WaitFrames_0000(1);
              script_019A();
            }
            goto L00F5F7;
          }
        }
        L00F5F7:;
      } else {
        if (local_DB & 8) {
          (void)((local_DB = 0) & 1);
          if (task_6 & 0x40) {
            (void)((task_7 = -1) & 1);
          } else {
            Script_sicnUtils(1, 0, 0);
          }
          Script_uiUtils(0, 44, 0);
          SCNF_RequestIconNotification(0, 0, 0, 2);
          goto L00F64A;
        }
      }
    }
    L00F64A:;
    if (script_D04E() == 130) {
      (void)((task_5 = 0) & 1);
      if (task_6 & 0x40) {
        Script_sicnUtils(1, 0, 0);
      }
      Script_shenClearFreeState(0);
      Script_scenUtils(SCENARIO_CMD_SET_RUNTIME_FLAG_800_INVERTED, 0);
      Script_convCtrlCamApply(1);
      script_E657();
      Script_scenUtils(SCENARIO_CMD_SET_RUNTIME_FLAG_800_INVERTED, 1);
      Script_shenClearFreeState(1);
      if (task_6 & 0x40) {
        Script_sicnUtils(0, 0, 1);
        Script_uiUtils(0, 44, 0);
      }
      Script_SetGameStateTo(AUTH_None);
      (void)((task_5 = -1) & 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_F6DD(void) {
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_HAS_CHARA, 0x4F54494E)) {
      Script_charaSetRenderOpacity(CHARA_NITO, 1.0f);
      Script_charaDisplay(CHARA_NITO, CDM_Show);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_F714(int32_t arg0) {
  int32_t local_FC;
  if (arg0 == 0) {
    local_FC = Script_scenCreateTask(script_F6DD, 0);
    script_8EB7();
    Script_srchControl((void *)local_FC);
  } else {
    script_4CCE();
  }
  return task_0 = -1;
}

static int32_t script_F74A(int32_t arg0) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
  while (local_FC == 0) {
    Script_sprintf("読み込み失敗\012");
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
  }
  L00F799:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L00F799;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return local_FC;
}

static void script_F7BA(int32_t arg0, int32_t arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  return;
}
