#include "act01_toki.h"

static int32_t script_0030(void) {
  for (;;) {
    Script_ClearAccumulator_00FE(1, 0x3F000000, 10, 2, 2);
    Script_ClearAccumulator_00FE(3, &data_107C, 8, 9);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0060(void) {
  Script_ClearAccumulator_00FE(0, "scene/02/model", "C1305S3G.MT5", "C1305S1G.MT5", 1);
  Script_scenCreateTask(script_0030, 0);
  return;
}

static void script_0086(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_008F(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_009E(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L0000CA;
    }
  }
  L0000CA:;
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

static void script_012B(int32_t arg0) {
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

static int32_t script_017B(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_01A9(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01D7(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_020B(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_023F(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_025D(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_0297(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_02CB(void) {
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

static void script_03C3(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_03F3(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Vector3i local_D8;
  int32_t local_D4;
  script_0086();
  script_0991();
  Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
  local_D8.x = 0;
  (&local_D8)->y = 0;
  (&local_D8)->z = 0;
  Script_charaPosRotUtils(SHEN_NAME('TEGS'), 152, 0, &local_D8, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('TEGS'), 153, 0, &local_D8, CHARA_PART_Set);
  local_FC = 0;
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_D4 = local_FC;
    switch (local_D4) {
      case 122:
        script_0AB3(CHARA_ASDA);
        break;
      case 386:
        Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
        script_0AEE(CHARA_ASDA);
        break;
      case 1129:
        script_0AB3(CHARA_ASDA);
        break;
      case 1707:
        Script_FACE(CHARA_AKIR, 1, 0, 16);
        script_0AB3(CHARA_AKIR);
        break;
      case 1875:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 1906:
        Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Show);
        local_F0.x = 0.168f;
        (&local_F0)->y = 0.07f;
        (&local_F0)->z = -0.044f;
        local_E4.x = 17658;
        (&local_E4)->y = 48423;
        (&local_E4)->z = 55887;
        Script_fixoAttachEx(SHEN_NAME('TEGS'), CHARA_AKIR, 18, &local_F0, &local_E4);
        break;
      case 2046:
        script_0AB3(CHARA_ASDA);
        Script_handSetFloatsFor(CHARA_ASDA, 1, data_1C58, 8);
        Script_handSetFloatsFor(CHARA_ASDA, 0, data_1D3C, 8);
        Script_fixoClearAttachment(SHEN_NAME('TEGS'));
        local_F0.x = 0.07f;
        (&local_F0)->y = 0.114f;
        (&local_F0)->z = -0.078f;
        local_E4.x = 5662;
        (&local_E4)->y = 36228;
        (&local_E4)->z = 45513;
        Script_fixoAttachEx(SHEN_NAME('TEGS'), CHARA_ASDA, 18, &local_F0, &local_E4);
        local_E4.x = -3640;
        (&local_E4)->y = 728;
        (&local_E4)->z = 4733;
        Script_SetHandRootRotation(CHARA_ASDA, 0, 0x15, &local_E4);
        break;
      case 3141:
        Script_FACE(CHARA_AKIR, 2, 0, 16);
        break;
      case 3300:
        Script_fixoClearAttachment(SHEN_NAME('TEGS'));
        local_F0.x = 0.05f;
        (&local_F0)->y = -0.005f;
        (&local_F0)->z = -0.118f;
        local_E4.x = 62277;
        (&local_E4)->y = 32406;
        (&local_E4)->z = 13473;
        Script_fixoAttachEx(SHEN_NAME('TEGS'), CHARA_ASDA, 18, &local_F0, &local_E4);
        break;
      case 3382:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 4125:
        Script_FACE(CHARA_AKIR, 2, 0, 16);
        break;
      case 4215:
        script_0AEE(CHARA_AKIR);
        script_0AEE(CHARA_ASDA);
        Script_fixoClearAttachment(SHEN_NAME('TEGS'));
        local_F0.x = 0.131f;
        (&local_F0)->y = 0.194f;
        (&local_F0)->z = -0.034f;
        local_E4.x = 2022;
        (&local_E4)->y = 36227;
        (&local_E4)->z = 43511;
        Script_fixoAttachEx(SHEN_NAME('TEGS'), CHARA_ASDA, 18, &local_F0, &local_E4);
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 4620:
        script_0AB3(CHARA_ASDA);
        local_E4.x = -3640;
        (&local_E4)->y = -2184;
        (&local_E4)->z = 4551;
        Script_SetHandRootRotation(CHARA_ASDA, 0, 0x15, &local_E4);
        Script_handSetFloatsFor(CHARA_ASDA, 1, data_1C58, 1);
        Script_handSetFloatsFor(CHARA_ASDA, 0, data_1D3C, 1);
        break;
      case 5051:
        Script_fixoClearAttachment(SHEN_NAME('TEGS'));
        local_F0.x = 0.168f;
        (&local_F0)->y = 0.07f;
        (&local_F0)->z = -0.044f;
        local_E4.x = 17658;
        (&local_E4)->y = 48423;
        (&local_E4)->z = 55887;
        Script_fixoAttachEx(SHEN_NAME('TEGS'), CHARA_AKIR, 18, &local_F0, &local_E4);
        local_D8.x = 0;
        (&local_D8)->y = 0;
        (&local_D8)->z = 0;
        Script_charaPosRotUtils(SHEN_NAME('TEGS'), 152, 0, &local_D8, CHARA_PART_Set);
        Script_charaPosRotUtils(SHEN_NAME('TEGS'), 153, 0, &local_D8, CHARA_PART_Set);
        break;
      case 5174:
        Script_fixoClearAttachment(SHEN_NAME('TEGS'));
        Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
        break;
      case 5295:
        goto L000990;
        break;
    }
    if ((local_FC >= 131) & (local_FC <= 339)) {
      Script_EVT_GetPosition(CHARA_ASDA, 5, &local_F0, 0);
      Script_SetCharaLookAtPosition(CHARA_AKIR, 98312, &local_F0, 0);
    }
    if ((local_FC >= 1972) & (local_FC <= 1991)) {
      (&local_D8)->z = (&local_D8)->z + 1456;
      Script_charaPosRotUtils(SHEN_NAME('TEGS'), 152, 0, &local_D8, CHARA_PART_Set);
    }
    if (local_FC == 1999) {
      (&local_D8)->z = 0;
    }
    if ((local_FC >= 2000) & (local_FC <= 2020)) {
      (&local_D8)->z = (&local_D8)->z - 1456;
      Script_charaPosRotUtils(SHEN_NAME('TEGS'), 153, 0, &local_D8, CHARA_PART_Set);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L000990:;
  return;
}

static void script_0991(void) {
  Vector4 local_F0;
  (&local_F0)->y = 0.3f;
  (&local_F0)->z = 0.3f;
  (&local_F0)->w = 0.3f;
  (void)1;
  Script_SetVector4ByOffset(&local_F0);
  script_009E(CHARA_AKIR);
  script_009E(CHARA_ASDA);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_ASDA, 1, 0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_ASDA, -1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_ASDA, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(CHARA_AKIR, 0);
  Script_SetCharaFlags(CHARA_ASDA, 0);
  Script_SetMaxInterpSpeed(0.4f);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_F0);
  Script_charaSetRefboardFlagTo(CHARA_ASDA, 1);
  Script_SetCharaRefBoardPos(CHARA_ASDA, &local_F0);
  return;
}

static void script_0A7A(void) {
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_ASDA, 0);
  script_012B(CHARA_AKIR);
  script_012B(CHARA_ASDA);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_0AB3(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_1700, 8);
  Script_handSetFloatsFor(arg0, 0, data_1700, 8);
  return;
}

static void script_0AEE(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

void script_0B18(void) {
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 46);
  Script_scenCreateTask(script_0B34, 1, 0x52494B41);
  return;
}

__attribute__((scn_string_bindings(0, data_1E20, 1, data_1E2D, 2, data_1E38, 3, data_1E3F, 4, data_1E4C, 5, data_1E5A, 6, data_1E65, 7, data_1E72, 8, data_1E7D, 9, data_1E84, 10, data_1E91, 11, data_1E9F)))
static int32_t script_0B34(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_GetCurrentEntryPoint();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(60);
  script_0D9D();
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (1) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 0:
        Script_LoadSND(0, 4, "a1_toura.snd", 0);
        Script_LoadSND(0, 2, "bgm050.snd", 0);
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 93);
        Script_LoadVoiceFile(1, "A0142B");
        Script_AuthorUtils(0, 0, "SEQDATA2.BIN", "SCENE/01/TOKI");
        local_F8 = Script_motiLoad(1, CHARA_TOKI, "M_0142.BIN");
        Script_scenCreateTask(script_017B, 0);
        script_03F3(local_FC);
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_AuthorUtils(AUTH_FREE, 0);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE, 1);
        Script_shenRequestGameModuleTransition(1, SHEN_NAME('D000'), 10);
        Script_ReallocateUNFO(local_F8);
        script_0A7A();
        break;
      case 10:
        Script_LoadSND(0, 4, "a1_toura.snd", 0);
        Script_LoadSND(0, 2, "bgm050.snd", 0);
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 93);
        Script_LoadVoiceFile(1, "A0142B");
        Script_AuthorUtils(0, 0, "SEQDATA2.BIN", "SCENE/01/TOKI");
        local_F8 = Script_motiLoad(1, CHARA_TOKI, "M_0142.BIN");
        Script_scenCreateTask(script_017B, 0);
        script_03F3(0);
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE, 1);
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 9, 0);
        Script_AuthorUtils(AUTH_FREE, 0);
        Script_shenRequestGameModuleTransition(1, SHEN_NAME('D000'), 10);
        break;
      case 99:
        script_0EBE();
        break;
    }
    SCNF_WaitFrames_0000(60);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0D9D(void) {
  Vector4 local_F0;
  int32_t local_D8[6];
  Vector3i local_CC;
  float local_C8;
  (&local_F0)->y = 0.3f;
  (&local_F0)->z = 0.3f;
  (&local_F0)->w = 0.3f;
  (void)1;
  Script_SetVector4ByOffset(&local_F0);
  (&local_F0)->y = 0.5f;
  (&local_F0)->z = 0.5f;
  (&local_F0)->w = 0.5f;
  Script_SetCharaRefBoardPos(SHEN_NAME('TEGS'), &local_F0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('TEGS'), 1);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 15.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
  local_D8[0] = 0x40C9999A;
  local_D8[1] = 0x3F970A3D;
  local_D8[2] = 0x3FB851EC;
  local_C8 = 0.0f;
  local_D8[3] = 0x3F800000;
  local_D8[4] = 0;
  local_D8[5] = 0;
  local_CC.x = (int32_t)(local_C8 / 360.0f * (float)0x10000);
  (&local_CC)->y = (int32_t)(local_C8 / 360.0f * (float)0x10000);
  (&local_CC)->z = (int32_t)(local_C8 / 360.0f * (float)0x10000);
  Script_EVT_ApplyRot((void *)local_D8, &local_CC);
  return;
}

__attribute__((scn_string_bindings(1, data_1EBC, 2, data_1EC9, 3, data_1ED4, 4, data_1EDB, 5, data_1EE8, 6, data_1EF6)))
static void script_0EBE(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = 0;
  while (1) {
    Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_GetControllerInput(0) != 0x400) {
      int32_t local_F4;
      local_F4 = local_F8;
      if (local_F4 == 0) {
        Script_PrintDebugString(3, 25, "ENTRY_0  0142_1.c");
        goto L000F25;
      }
      L000F25:;
      SCNF_WaitFrames_0000(1);
    }
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(60);
    {
      int32_t local_F4;
      local_F4 = local_F8;
      if (local_F4 == 0) {
        Script_LoadSND(0, 4, "a1_toura.snd", 0);
        Script_LoadSND(0, 2, "bgm050.snd", 0);
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 93);
        Script_LoadVoiceFile(1, "A0142B");
        Script_AuthorUtils(0, 0, "SEQDATA2.BIN", "SCENE/01/TOKI");
        local_FC = Script_motiLoad(1, CHARA_TOKI, "M_0142.BIN");
        Script_scenCreateTask(script_017B, 0);
        script_03F3(0);
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ReallocateUNFO(local_FC);
        script_0A7A();
        Script_AuthorUtils(AUTH_FREE, 0);
        goto L00103F;
      }
    }
    L00103F:;
    SCNF_WaitFrames_0000(60);
    SCNF_WaitFrames_0000(1);
    Script_executeAicaCommand(0x1200A0, 2, 0);
  }
  return;
}
