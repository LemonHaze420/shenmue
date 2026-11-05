#include "act03_nbik.h"

static void script_0030(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_0039(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_0048(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L000074;
    }
  }
  L000074:;
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

static void script_00D5(int32_t arg0) {
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

static void script_0125(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0153(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0181(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01B5(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01E9(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0207(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_0241(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_0275(void) {
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

static void script_036D(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static int32_t script_039D(void) {
  SCNF_WaitFrames_0000(0);
  Script_ProcessSubtitleText("●青Like a dream I once saw,＆it passes by as it touches my cheek.");
  return Script_srchControl(0);
}

static int32_t script_03B8(void) {
  SCNF_WaitFrames_0000(70);
  Script_ProcessSubtitleText("●青This street leading to the ocean＆carries the smell of sea.");
  end:;
}

static int32_t script_03CD(void) {
  SCNF_WaitFrames_0000(435);
  Script_ProcessSubtitleText("●青Before love makes me tell a white lie.");
  end:;
}

static int32_t script_03E3(void) {
  SCNF_WaitFrames_0000(30);
  Script_ProcessSubtitleText("●青This loneliness...＆I want you to hold it in your arms now.");
  end:;
}

static int32_t script_03F8(void) {
  SCNF_WaitFrames_0000(50);
  Script_ProcessSubtitleText("●青If you will always be by my side,");
  end:;
}

static int32_t script_040D(void) {
  SCNF_WaitFrames_0000(0);
  Script_ProcessSubtitleText("●青there'll be no sorrow or tears in my eyes.");
  end:;
}

static int32_t script_0422(void) {
  SCNF_WaitFrames_0000(160);
  Script_ProcessSubtitleText("●青Can't live without you.＆Why did you have to decide?");
  end:;
}

static int32_t script_0438(void) {
  SCNF_WaitFrames_0000(0);
  Script_ProcessSubtitleText("●青You won't be aware of my pains and lies.");
  end:;
}

static int32_t script_044D(void) {
  SCNF_WaitFrames_0000(50);
  Script_ProcessSubtitleText("●青Don't leave me alone, stay with me.");
  end:;
}

static int32_t script_0462(void) {
  SCNF_WaitFrames_0000(100);
  Script_ProcessSubtitleText("●青I don't know if you'll ever look back.");
  end:;
}

static int32_t script_0477(void) {
  SCNF_WaitFrames_0000(35);
  Script_ProcessSubtitleText("●青I'll remember you forever.");
  end:;
}

static int32_t script_048C(void) {
  SCNF_WaitFrames_0000(150);
  Script_GetTaskData_0();
  end:;
}

static void script_049B(void) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 1);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetRenderFlag(8, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_mapmSetRenderFlag(10, 1);
  Script_mapmSetRenderFlag(11, 1);
  Script_mapmSetRenderFlag(12, 1);
  Script_mapmSetRenderFlag(13, 1);
  Script_mapmSetRenderFlag(14, 1);
  Script_mapmSetRenderFlag(15, 1);
  Script_mapmSetRenderFlag(16, 1);
  Script_mapmSetRenderFlag(17, 0);
  Script_mapmSetRenderFlag(18, 0);
  Script_mapmSetRenderFlag(19, 0);
  Script_mapmSetRenderFlag(20, 0);
  Script_mapmSetRenderFlag(21, 0);
  Script_mapmSetRenderFlag(22, 0);
  Script_mapmSetCamFlags(2, 17, 1);
  script_1201();
  local_FC = Script_scenCreateTask(script_0EBA, 0);
  local_F4 = Script_scenCreateTask(script_155A, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 300.0f);
  Script_SetDynamicMapStaticSceneMode(18, 1);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
  script_1382();
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F4);
  return;
}

static void script_0623(void) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 1);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetRenderFlag(8, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_mapmSetRenderFlag(10, 1);
  Script_mapmSetRenderFlag(11, 1);
  Script_mapmSetRenderFlag(12, 1);
  Script_mapmSetRenderFlag(13, 1);
  Script_mapmSetRenderFlag(14, 1);
  Script_mapmSetRenderFlag(15, 1);
  Script_mapmSetRenderFlag(16, 1);
  Script_mapmSetRenderFlag(17, 0);
  Script_mapmSetRenderFlag(18, 0);
  Script_mapmSetRenderFlag(19, 0);
  Script_mapmSetRenderFlag(20, 0);
  Script_mapmSetRenderFlag(21, 0);
  Script_mapmSetRenderFlag(22, 0);
  Script_mapmSetCamFlags(2, 17, 1);
  script_1201();
  local_FC = Script_scenCreateTask(script_0EBA, 0);
  local_F4 = Script_scenCreateTask(script_155A, 0);
  Script_scenCreateTask(script_15B0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 300.0f);
  Script_SetDynamicMapStaticSceneMode(18, 1);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_srchControl((void *)local_FC);
  Script_SetupLightFromTable(1);
  local_FC = Script_scenCreateTask(script_1029, 0);
  Script_PREP_Related(-1, 1);
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(9, 0);
  Script_mapmSetRenderFlag(10, 0);
  Script_mapmSetRenderFlag(11, 0);
  Script_mapmSetRenderFlag(12, 0);
  Script_mapmSetRenderFlag(13, 0);
  Script_mapmSetRenderFlag(14, 0);
  Script_mapmSetRenderFlag(15, 0);
  Script_mapmSetRenderFlag(16, 0);
  Script_mapmSetRenderFlag(17, 1);
  Script_mapmSetRenderFlag(18, 1);
  Script_mapmSetRenderFlag(19, 1);
  Script_mapmSetRenderFlag(20, 1);
  Script_mapmSetCamFlags(1, 19, 1);
  Script_mapmSetCamFlags(1, 20, 1);
  Script_mapmSetRenderFlag(21, 0);
  Script_mapmSetRenderFlag(22, 0);
  Script_SetDynamicMapStaticSceneMode(17, 0);
  Script_SetDynamicMapStaticSceneMode(18, 0);
  Script_shenSeqUtils(1);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_SetDynamicMapStaticSceneMode(18, 1);
  Script_shenSeqUtils(2);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(3);
  Script_scenCreateTask(script_039D, 0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_charaDisplay(SHEN_NAME('MOON'), CDM_Show);
  Script_shenSeqUtils(4);
  Script_scenCreateTask(script_03B8, 0);
  Script_scenCreateTask(script_03CD, 0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_charaDisplay(SHEN_NAME('MOON'), CDM_Hide);
  Script_shenSeqUtils(5);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_charaDisplay(SHEN_NAME('MOON'), CDM_Show);
  Script_shenSeqUtils(6);
  Script_scenCreateTask(script_03E3, 0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_mapmSetRenderFlag(20, 0);
  Script_charaDisplay(SHEN_NAME('MOON'), CDM_Hide);
  Script_shenSeqUtils(7);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(8);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(9);
  Script_scenCreateTask(script_03F8, 0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(10);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(11);
  Script_scenCreateTask(script_040D, 0);
  Script_scenCreateTask(script_0422, 0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(12);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(13);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(14);
  Script_scenCreateTask(script_0438, 0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_mapmSetCamFlags(1, 19, 0);
  Script_mapmSetCamFlags(1, 20, 0);
  Script_shenSeqUtils(15);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(16);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_charaDisplay(SHEN_NAME('YELL'), CDM_Hide);
  Script_shenSeqUtils(17);
  Script_scenCreateTask(script_044D, 0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(18);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_charaDisplay(SHEN_NAME('YELL'), CDM_Show);
  Script_shenSeqUtils(19);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_shenSeqUtils(20);
  Script_scenCreateTask(script_0462, 0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_charaDisplay(SHEN_NAME('MOON'), CDM_Show);
  Script_shenSeqUtils(21);
  Script_scenCreateTask(script_0477, 0);
  Script_scenCreateTask(script_048C, 0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    script_0E36((void *)local_FC, (void *)local_F4);
  }
  Script_mapmSetCamFlags(2, 17, 0);
  Script_charaDisplay(SHEN_NAME('MOON'), CDM_Hide);
  Script_ReturnTimeDate(&local_E8);
  if ((&local_E8)->month == 12) {
    if (((&local_E8)->day >= 19) & ((&local_E8)->day <= 25)) {
      Script_mapmSetRenderFlag(21, 1);
    }
  } else {
    if ((&local_E8)->month == 1) {
      if (((&local_E8)->day >= 1) & ((&local_E8)->day <= 5)) {
        Script_mapmSetRenderFlag(22, 1);
      }
      goto L000DD4;
    }
  }
  L000DD4:;
  Script_shenSeqUtils(22);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
  script_1382();
  Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F4);
  return;
}

static void script_0E36(void *arg0, void *arg1) {
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG, 1);
  Script_ReallocateUNFO(task_0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
  script_1382();
  Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_StartNozomiBikeSeq(0, 0, 0);
  Script_srchControl(arg0);
  Script_srchControl(arg1);
  Script_TimedTransitionUtils(TIMED_TRANSITION_SET_ENABLED, 1);
  Script_SetShouldSleepFlag(1);
  Script_shenRequestGameModuleTransition(3, SHEN_NAME('JOMO'), 4);
  return;
}

static int32_t script_0EBA(void) {
  int32_t unused_E8[6];
  Vector3i local_DC;
  int32_t local_D8;
  Script_ReadModelFormat7(CHARA_HRSK, 1, data_2744, 0);
  Script_FACE_LCG(CHARA_HRSK, FACE_BlinkAutomatic, 0, 0);
  Script_charaVisUtils(CHARA_AKIR, 1);
  Script_SetLampState(0, SHEN_NAME('BIKE'), 0);
  local_D8 = 0;
  for (;;) {
    if (local_D8 == 360) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
    }
    if (local_D8 == 353) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_2360, 1);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_2444, 1);
      local_DC.x = -2730;
      (&local_DC)->y = -2002;
      (&local_DC)->z = -182;
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_DC);
      local_DC.x = 2730;
      (&local_DC)->y = 3094;
      (&local_DC)->z = -1092;
      Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_DC);
    }
    if (local_D8 == 456) {
      Script_lightSetSlotEnabled(23, 1);
    }
    if (local_D8 == 540) {
      Script_ReadModelFormat7(CHARA_HRSK, 1, data_276C, 10);
    }
    if (local_D8 == 696) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 1);
    }
    if (local_D8 == 872) {
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

static int32_t script_1029(void) {
  int32_t unused_DC[9];
  int32_t local_D8;
  Script_ReadModelFormat7(CHARA_HRSK, 1, data_2794, 0);
  Script_FACE_LCG(CHARA_HRSK, FACE_BlinkAutomatic, 0, 0);
  Script_charaVisUtils(CHARA_AKIR, 1);
  Script_SetOsageFlags(3, 0.2f);
  local_D8 = 0;
  for (;;) {
    if (local_D8 == 0) {
      Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    if (local_D8 == 180) {
      Script_StartNozomiBikeSeq(1, 983045, 0);
    }
    if (local_D8 == 569) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 265.0f);
    }
    if (local_D8 == 875) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
    }
    if (local_D8 == 953) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 320.0f);
    }
    if (local_D8 == 242) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 0);
    }
    if (local_D8 == 250) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 1);
    }
    if (local_D8 == 1477) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 0);
    }
    if (local_D8 == 1488) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 1);
    }
    if (local_D8 == 1700) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 0);
    }
    if (local_D8 == 1714) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 1);
    }
    if (local_D8 == 2559) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 0);
    }
    if (local_D8 == 2570) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 1);
    }
    if (local_D8 == 3617) {
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

static void script_1201(void) {
  int32_t unused_E8[6];
  script_142C(CHARA_AKIR);
  script_142C(CHARA_HRSK);
  Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PINK'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('YELL'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('GREE'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BLUE'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SUJI'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('HIKA'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CAR3'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CAR5'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CAR7'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CAR8'), CDM_Show);
  Script_scenToggleActiveFor(SHEN_NAME('GREE'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('PINK'), 0);
  Script_charaVisUtils(SHEN_NAME('PINK'), 1);
  Script_charaSetClipAngle(SHEN_NAME('PINK'), 20.0f);
  Script_clmdSetFlags(CHARA_HRSK, 8, 1);
  Script_clmdSetFlags(CHARA_AKIR, 8, 1);
  Script_SetupClothData(CHARA_AKIR, 8);
  Script_SetupClothData(CHARA_HRSK, 7);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_HRSK, 4);
  Script_SetCharaFlags(SHEN_NAME('BIKE'), 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(SHEN_NAME('BIKE'), SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_HRSK, -1);
  Script_SetVector4ByOffset(&data_27E4);
  (void)1;
  return;
}

static void script_1382(void) {
  script_1514(CHARA_AKIR);
  script_1514(CHARA_HRSK);
  Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PINK'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YELL'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('GREE'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BLUE'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SUJI'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HIKA'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CAR3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CAR4'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CAR5'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CAR7'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CAR8'), CDM_Hide);
  return;
}

static void script_142C(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_1B5C, 10);
  Script_handSetFloatsFor(arg0, 1, data_1B5C, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_27F4);
  return;
}

static void script_14C8(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_14F1(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  return;
}

static void script_1514(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_EnableAll);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static int32_t script_155A(void) {
  Vector3i local_F4;
  SCNF_WaitFrames_0000(620);
  for (;;) {
    local_F4.x = -36408;
    (&local_F4)->y = 0;
    (&local_F4)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('BIKE'), 23, 0, &local_F4, CHARA_PART_Add);
    Script_charaPosRotUtils(SHEN_NAME('BIKE'), 18, 0, &local_F4, CHARA_PART_Add);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_15B0(void) {
  Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

void script_15E4(void) {
  Script_scenCreateTask(script_15F7, 1, CHARA_AKIR);
  return;
}

__attribute__((scn_string_bindings(0, data_2824, 4, data_284D, 5, data_2854, 6, data_2861, 7, data_286E, 8, data_2875, 9, data_2882, 10, data_288F, 11, data_2896, 12, data_28A3)))
static int32_t script_15F7(int32_t arg0) {
  int32_t unused_FC;
  int32_t local_F8;
  Script_SetFogEnabled(1);
  Script_SetFogColour(&data_2804);
  Script_PREP_Related(-1, 1);
  Script_SetFogFlags(0, data_2814);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL27.SPR");
  Script_LoadVoiceFile(1, "01BIKEB");
  Script_LoadSND(0, 4, "a1_nbike.snd", 0);
  Script_ryoSetChara(arg0);
  for (;;) {
    local_F8 = Script_GetCurrentEntryPoint();
    switch (local_F8) {
      case 0:
        Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL25.SPR");
        task_0 = Script_motiLoad(3, SHEN_NAME('NBIK'), "M_01BIKE.BIN");
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 833, 1);
        script_0623();
        Script_StartNozomiBikeSeq(0, 0, 0);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG, 1);
        Script_ReallocateUNFO(task_0);
        Script_TimedTransitionUtils(TIMED_TRANSITION_SET_ENABLED, 1);
        Script_SetShouldSleepFlag(1);
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('JOMO'), 4);
        goto L00179E;
        break;
      case 1:
        Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        if (Script_GetControllerInput(0) == 0x400) {
          Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL25.SPR");
          task_0 = Script_motiLoad(3, SHEN_NAME('NBIK'), "M_01BIKE.BIN");
          script_0623();
        }
        break;
      case 2:
        Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL25.SPR");
        task_0 = Script_motiLoad(3, SHEN_NAME('NBIK'), "M_01BIKE.BIN");
        SCNF_WaitFrames_0000(1);
        script_049B();
        Script_ReallocateUNFO(task_0);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L00179E:;
}
