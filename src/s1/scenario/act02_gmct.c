#include "act02_gmct.h"

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

__attribute__((scn_string_bindings(1, data_316C)))
static void script_039D(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_0030();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 20.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  script_0948();
  local_F8 = Script_motiLoad(2, SHEN_NAME('GMCT'), "M_01123.BIN");
  Script_LoadSND(0, 3, "amb024.snd", 0);
  Script_LoadSND(0, 4, "a1_jytat.snd", 0);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  local_FC = Script_scenCreateTask(script_0451, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_09B1();
  Script_ReallocateUNFO(local_F8);
  Script_srchControl((void *)local_FC);
  Script_set_val_to_1(2, 0);
  return;
}

static int32_t script_0451(void) {
  int16_t local_FE;
  Vector3 local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  local_FE = 0;
  local_F0.x = 5.0f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = -1.95f;
  local_E4.x = 0;
  (&local_E4)->y = 38229;
  (&local_E4)->z = 0;
  Script_EVT_SetPosition(SHEN_NAME('SLOT'), POS_XYZ, &local_F0);
  Script_EVT_SetRotation(SHEN_NAME('SLOT'), 0x38000000, &local_E4, 0);
  Script_charaDisplay(SHEN_NAME('SLOT'), CDM_Show);
  Script_charaDisplay(CHARA_NITO, CDM_Hide);
  Script_charaDisplay(CHARA_CHAI, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TIK1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TIK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TIK3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TIK4'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  Script_charaVisUtils(SHEN_NAME('DOOR'), 1);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  script_1C79();
  for (;;) {
    if (Script_shenSeqUtils(-1)) {
      local_E0 = local_FE;
      switch (local_E0) {
        case 1:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_ResetHandMotion(CHARA_JIMY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_JIMY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_26B0, 1);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_26B0, 1);
          Script_handSetFloatsFor(CHARA_JIMY, 1, data_26B0, 1);
          Script_handSetFloatsFor(CHARA_JIMY, 0, data_26B0, 1);
          Script_FACE(CHARA_AKIR, 2, 0, 1);
          Script_ConfigureClipFlags(CHARA_AKIR, 2, 0);
          Script_EVT_GetPosition(CHARA_JIMY, 9, &local_F0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 1);
          Script_executeAicaCommand(24232, 0, 0);
          Script_executeAicaCommand(1184, 3, 87);
          break;
        case 110:
          break;
        case 283:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2EB4, 5);
          break;
        case 295:
          Script_FACE(CHARA_JIMY, 4, 0, 1);
          break;
        case 338:
          local_E4.x = -1454;
          (&local_E4)->y = 2547;
          (&local_E4)->z = 4912;
          Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_E4);
          Script_handSetFloatsFor(CHARA_JIMY, 1, data_26B0, 1);
          Script_EVT_GetPosition(CHARA_JIMY, 18, &local_F0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 8);
          break;
        case 310:
          Script_FACE(CHARA_AKIR, 2, 5, 10);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 8);
          break;
        case 379:
          Script_SetMotnFlag(CHARA_AKIR, -1);
          break;
        case 380:
          Script_EVT_GetPosition(CHARA_JIMY, 18, &local_F0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 8);
          break;
        case 610:
          Script_FACE(CHARA_JIMY, 7, 0, 1);
          break;
        case 640:
          local_E4.x = 9278;
          (&local_E4)->y = -1456;
          (&local_E4)->z = 4551;
          Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E4);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2F98, 1);
          break;
        case 650:
          Script_FACE(CHARA_JIMY, 4, 0, 1);
          break;
        case 659:
          script_0A12(CHARA_AKIR, CHARA_JIMY);
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 6, 0);
          break;
        case 665:
          break;
        case 710:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkAutomatic, 6, 0);
          break;
        case 713:
          Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F0, 0);
          Script_FaceLookAtTargetUtils(CHARA_JIMY, FACE_LookAtPosition, &local_F0, 4);
          break;
        case 810:
          Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
        case 940:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_JIMY, 0, 1, 0);
          break;
        case 1190:
          break;
        case 1210:
          Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 6, 0);
          break;
        case 1230:
          break;
      }
      if (local_FE > 1192) {
        Script_EVT_GetPosition(CHARA_AKIR, 15, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        local_F0.x = 3.76f;
        (&local_F0)->z = -3.17f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F0, 8);
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    } else {
      Script_sprintf("test\012");
      local_FE = 0;
      break;
    }
  }
  end:;
}

static void script_0948(void) {
  script_0048(CHARA_AKIR);
  script_0048(CHARA_JIMY);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_JIMY, 4);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_JIMY, 1, 0);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_JIMY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  return;
}

static void script_09B1(void) {
  script_00D5(CHARA_AKIR);
  script_00D5(CHARA_JIMY);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_JIMY, 0, 1, 0);
  Script_ResetHandMotion(CHARA_JIMY, 0, 0, 0);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_0A12(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Script_EVT_GetPosition(arg1, 5, &local_F4, 0);
  Script_SetCharaLookAtPosition(arg0, 98312, &local_F4, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookAtPosition, &local_F4, 8);
  return;
}

__attribute__((scn_string_bindings(0, data_36E4, 1, data_36EA, 2, data_36F5, 3, data_3701, 4, data_370C, 5, data_3717, 6, data_3723, 7, data_3730, 8, data_3735, 10, data_3747, 11, data_3754)))
static void script_0A3D(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Script_LoadVoiceFile(1, "A0172");
  local_F0 = Script_motiLoad(2, SHEN_NAME('GMCT'), "M_0172.BIN");
  Script_LoadSND_Wrapper("system1.snd", 0, "tsm001.snd", "amb024.snd", "a1_bgam.snd", "battle_1.snd", 0, 0);
  Script_executeAicaCommand(24232, 0, 0);
  Script_executeAicaCommand(1184, 3, 87);
  script_0D3D();
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_6C = 0;
  local_FC = Script_scenCreateTask(script_0F21, 0);
  local_F8 = Script_scenCreateTask(script_0F08, 0);
  local_F4 = Script_scenCreateTask(script_16D1, 0);
  Script_shenSeqUtils(1);
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 4, "e1batchi.snd", 0);
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  script_1CEF(CHARA_CHAI);
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  Script_srchControl((void *)local_F4);
  Script_ReallocateUNFO(local_F0);
  Script_LoadVoiceFile(0, "A0172");
  return;
}

__attribute__((scn_string_bindings(0, data_375A, 1, data_3760, 2, data_376B, 3, data_3776, 4, data_3782)))
static void script_0B94(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Script_LoadVoiceFile(1, "A0172");
  local_F4 = Script_motiLoad(2, SHEN_NAME('GMCT'), "M_0172.BIN");
  Script_LoadSND(0, 3, "amb024.snd", 0);
  Script_LoadSND(0, 4, "a1_bgam.snd", 0);
  Script_executeAicaCommand(24232, 0, 0);
  Script_executeAicaCommand(1184, 3, 87);
  script_0DB5();
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_6C = 0;
  local_FC = Script_scenCreateTask(script_173C, 0);
  local_F8 = Script_scenCreateTask(script_0F08, 0);
  Script_shenSeqUtils(2);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_set_val_to_1(2, 0);
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  Script_ReallocateUNFO(local_F4);
  Script_LoadVoiceFile(0, "A0172");
  return;
}

static void script_0C62(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 2);
  return;
}

static void script_0C9E(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 2);
  return;
}

static void script_0D3D(void) {
  script_0C9E(CHARA_AKIR);
  script_0C9E(CHARA_CHAI);
  Script_charaDisplay(CHARA_NITO, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SLOT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('TIK1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TIK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TIK3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TIK4'), CDM_Hide);
  Script_charaDisplay(CHARA_JIMY, CDM_Hide);
  script_0EE8();
  return;
}

static void script_0DB5(void) {
  script_0C9E(CHARA_AKIR);
  script_0C9E(CHARA_CHAI);
  script_0C62(CHARA_NITO);
  Script_charaDisplay(CHARA_NITO, CDM_Show);
  Script_charaDisplay(SHEN_NAME('SLOT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  Script_charaVisUtils(SHEN_NAME('DOOR'), 1);
  Script_charaDisplay(CHARA_JIMY, CDM_Hide);
  script_0EE8();
  return;
}

static void script_0E19(void) {
  script_0C9E(CHARA_AKIR);
  script_0C9E(CHARA_CHAI);
  script_0C9E(CHARA_FUKU);
  Script_charaDisplay(CHARA_NITO, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SLOT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  Script_charaVisUtils(SHEN_NAME('DOOR'), 1);
  Script_charaDisplay(CHARA_JIMY, CDM_Hide);
  script_0EE8();
  return;
}

static void script_0E7D(void) {
  Script_mapmSetRenderFlag(2, 0);
  script_0C9E(CHARA_AKIR);
  script_0C9E(CHARA_CHAI);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SLOT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('TIK1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TIK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TIK3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TIK4'), CDM_Hide);
  script_0EE8();
  return;
}

static void script_0EE8(void) {
  Script_SetVector4ByOffset(&data_3788);
  (void)1;
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  return;
}

static int32_t script_0F08(void) {
  for (;;) {
    task_6C = task_6C + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0F21(void) {
  Vector3 local_F4;
  local_F4.x = 5.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -1.95f;
  {
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 38229;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('SLOT'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('SLOT'), 0x38000000, &local_E8, 0);
  }
  script_1C79();
  for (;;) {
    if (task_6C == 1) {
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_318C, 10);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_318C, 10);
      Script_SetMotnFlag(CHARA_AKIR, -1);
    } else if (task_6C == 233) {
      local_F4.x = 2.04f;
      (&local_F4)->y = 1.62f;
      (&local_F4)->z = -2.81f;
      local_F4.x = 0.05f;
      (&local_F4)->y = 1.62f;
      (&local_F4)->z = -2.5f;
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 3);
    } else if (task_6C == 239) {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    } else if (task_6C == 331) {
      local_F4.x = 5.18f;
      (&local_F4)->y = 1.6f;
      (&local_F4)->z = -2.18f;
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 3);
    } else if (task_6C == 342) {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
    } else if (task_6C == 359) {
      Script_executeAicaCommand(3496, 0, 0);
      Script_executeAicaCommand(1184, 2, 93);
    } else if (task_6C == 360) {
      Script_FACE(CHARA_AKIR, 4, 0, 7);
    } else if (task_6C == 500) {
      Script_FACE(CHARA_AKIR, 3, 2, 7);
    } else if ((task_6C > 930) & (task_6C < 1010)) {
      Script_EVT_GetPosition(CHARA_CHAI, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 8);
    } else {
      switch (task_6C) {
        case 1010:
          Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
          break;
        case 1810:
          Script_FACE(CHARA_AKIR, 5, 0, 7);
          break;
        case 2090:
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2404, 15);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2404, 15);
          break;
      }
    }
    if (task_6C == 940) {
      Script_FACE(CHARA_AKIR, 10, 0, 3);
    }
    if (task_6C == 975) {
      Script_FACE(CHARA_AKIR, 2, 0, 3);
    }
    if (task_6C == 1) {
      Script_handSetFloatsFor(CHARA_CHAI, 0, data_318C, 10);
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_3438, 10);
      Script_SetMotnFlag(CHARA_CHAI, -1);
      Script_FaceLookAtTargetUtils(CHARA_CHAI, FACE_LookReset);
    } else if (task_6C == 804) {
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_318C, 5);
    } else if (task_6C == 1080) {
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_3600, 10);
    } else if (task_6C == 1110) {
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_318C, 10);
    } else if (task_6C == 1320) {
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_351C, 10);
    } else if (task_6C == 1370) {
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_318C, 10);
    } else if (task_6C == 1408) {
      Script_handSetFloatsFor(CHARA_CHAI, 0, data_3600, 10);
    } else if (task_6C == 1426) {
      Script_handSetFloatsFor(CHARA_CHAI, 0, data_318C, 10);
    } else if (task_6C == 1525) {
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_3600, 10);
    } else if (task_6C == 0x600) {
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_318C, 10);
    } else if ((task_6C > 1554) & (task_6C < 1800)) {
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      (&local_F4)->y = (&local_F4)->y + 0.1f;
      (&local_F4)->z = (&local_F4)->z + 1.3f;
      Script_FaceLookAtTargetUtils(CHARA_CHAI, FACE_LookAtPosition, &local_F4, 0x3F800000);
    } else if (task_6C > 1857) {
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      (&local_F4)->y = (&local_F4)->y + 0.1f;
      (&local_F4)->z = (&local_F4)->z + 1.3f;
      Script_FaceLookAtTargetUtils(CHARA_CHAI, FACE_LookAtPosition, &local_F4, 1);
    } else {
      if (task_6C > 1886) {
        Script_FaceLookAtTargetUtils(CHARA_CHAI, FACE_LookReset);
        goto L001405;
      }
    }
    L001405:;
    switch (task_6C) {
      case 1:
        Script_FACE(CHARA_CHAI, 2, 0, 1);
        break;
      case 700:
        Script_FACE(CHARA_CHAI, 10, 0, 2);
        break;
      case 845:
        Script_FACE(CHARA_CHAI, 7, 0, 2);
        break;
      case 1850:
        Script_FACE(CHARA_CHAI, 2, 0, 2);
        break;
    }
    Script_SetFightProgramStateBytes(CHARA_CHAI, 2, 1);
    switch (task_6C) {
      case 641:
        Script_GetDataFromFightProgram(CHARA_CHAI, 1);
        break;
      case 655:
        Script_GetDataFromFightProgram(CHARA_CHAI, 0);
        Script_SetFightProgramStateBytes(CHARA_CHAI, 2, 1);
        break;
      case 1285:
        Script_GetDataFromFightProgram(CHARA_CHAI, 1);
        break;
      case 1299:
        Script_GetDataFromFightProgram(CHARA_CHAI, 0);
        Script_SetFightProgramStateBytes(CHARA_CHAI, 4, 0);
        break;
      case 1900:
        Script_GetDataFromFightProgram(CHARA_CHAI, 1);
        break;
      case 1945:
        Script_GetDataFromFightProgram(CHARA_CHAI, 0);
        break;
    }
    Script_clmdSetFlags(CHARA_CHAI, 2, 0);
    switch (task_6C) {
      case 10:
        {
          Vector3i local_E8;
          local_F4.x = 0.135f;
          (&local_F4)->y = 0.068f;
          (&local_F4)->z = -0.066f;
          local_E8.x = 60748;
          (&local_E8)->y = 178;
          (&local_E8)->z = 15448;
          Script_charaDisplay(SHEN_NAME('TIK1'), CDM_Show);
          Script_fixoAttachEx(SHEN_NAME('TIK1'), CHARA_CHAI, 18, &local_F4, &local_E8);
          break;
        }
      case 803:
        Script_charaDisplay(SHEN_NAME('TIK1'), CDM_Hide);
        break;
      case 804:
        {
          Vector3i local_E8;
          local_F4.x = 0.134f;
          (&local_F4)->y = 0.025f;
          (&local_F4)->z = -0.043f;
          local_E8.x = 52774;
          (&local_E8)->y = 8188;
          (&local_E8)->z = 64607;
          Script_charaDisplay(SHEN_NAME('TIK4'), CDM_Show);
          Script_fixoAttachEx(SHEN_NAME('TIK4'), CHARA_CHAI, 18, &local_F4, &local_E8);
          break;
        }
      case 841:
        {
          Vector3i local_E8;
          local_F4.x = 0.161f;
          (&local_F4)->y = 0.0f;
          (&local_F4)->z = 0.043f;
          local_E8.x = 52774;
          (&local_E8)->y = 8188;
          (&local_E8)->z = 64607;
          Script_charaDisplay(SHEN_NAME('TIK4'), CDM_Show);
          Script_fixoAttachEx(SHEN_NAME('TIK4'), CHARA_CHAI, 5, &local_F4, &local_E8);
          break;
        }
      case 940:
        Script_charaDisplay(SHEN_NAME('TIK4'), CDM_Hide);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_16D1(void) {
  for (;;) {
    if ((task_6C > 1632) & (task_6C < 1900)) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.025f)) {
        Script_handSetFloatsFor(CHARA_CHAI, 0, data_3354, 2);
        SCNF_WaitFrames_0000(2);
        Script_handSetFloatsFor(CHARA_CHAI, 0, data_318C, 4);
        SCNF_WaitFrames_0000(4);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_173C(void) {
  int32_t unused_FC;
  Script_SetFogFlags(0, data_3798);
  script_1C79();
  for (;;) {
    switch (task_6C) {
      case 1:
        Script_SetMotnFlag(CHARA_AKIR, -1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2404, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2404, 1);
        Script_FACE(CHARA_AKIR, 2, 0, 7);
        break;
      case 360:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_318C, 25);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_318C, 25);
        break;
      case 805:
        Script_FACE(CHARA_AKIR, 7, 0, 7);
        break;
    }
    Script_clmdSetFlags(CHARA_CHAI, 2, 0);
    switch (task_6C) {
      case 1:
        Script_handSetFloatsFor(CHARA_CHAI, 0, data_318C, 10);
        Script_handSetFloatsFor(CHARA_CHAI, 1, data_318C, 10);
        Script_FACE_LCG(CHARA_CHAI, FACE_BlinkClose, 1, 0);
        Script_FaceLookAtTargetUtils(CHARA_CHAI, FACE_LookReset);
        Script_FACE(CHARA_CHAI, 4, 0, 7);
        break;
      case 550:
        Script_FACE_LCG(CHARA_CHAI, FACE_BlinkOpen, 10, 0);
        break;
      case 650:
        Script_FACE(CHARA_CHAI, 9, 0, 7);
        break;
    }
    if (task_6C == 1) {
      Script_CharHandlers(CHARA_NITO, 2, 8, 16);
    }
    if ((task_6C >= 710) & (task_6C < 730)) {
      script_1CA6(0.045f);
    }
    if ((task_6C >= 757) & (task_6C < 797)) {
      script_1CA6(-0.0225f);
    }
    if (task_6C == 1130) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_37A8, 1, data_37AF, 2, data_37BA, 3, data_37C5, 6, data_37E5)))
static void script_194C(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Script_LoadVoiceFile(1, "A0172A");
  local_F4 = Script_motiLoad(2, SHEN_NAME('GMCT'), "M_0172.BIN");
  Script_LoadSND(0, 3, "amb024.snd", 0);
  Script_LoadSND(0, 4, "a1_bgam.snd", 0);
  Script_executeAicaCommand(24232, 0, 0);
  Script_executeAicaCommand(1184, 3, 87);
  script_0E19();
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_6C = 0;
  local_FC = Script_scenCreateTask(script_1A56, 0);
  local_F8 = Script_scenCreateTask(script_0F08, 0);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL00.SPR");
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenSeqUtils(3);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_set_val_to_1(2, 0);
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  Script_ReallocateUNFO(local_F4);
  Script_LoadVoiceFile(0, "A0172A");
  return;
}

static int32_t script_1A56(void) {
  int32_t unused_FC;
  Vector3i local_F0;
  Script_SetFogFlags(0, data_37EC);
  for (;;) {
    if (task_6C == 1) {
      Script_SetMotnFlag(CHARA_AKIR, -1);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_318C, 1);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_318C, 1);
      Script_FACE(CHARA_AKIR, 4, 0, 7);
      local_F0.x = 0;
      (&local_F0)->y = 4733;
      (&local_F0)->z = 0;
      Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_F0);
    }
    Script_clmdSetFlags(CHARA_CHAI, 2, 0);
    switch (task_6C) {
      case 1:
        Script_handSetFloatsFor(CHARA_CHAI, 0, data_318C, 10);
        Script_handSetFloatsFor(CHARA_CHAI, 1, data_318C, 10);
        Script_FACE_LCG(CHARA_CHAI, FACE_BlinkOpen, 1, 0);
        Script_FaceLookAtTargetUtils(CHARA_CHAI, FACE_LookReset);
        Script_FACE(CHARA_CHAI, 4, 0, 7);
        break;
      case 200:
        Script_handSetFloatsFor(CHARA_CHAI, 1, data_351C, 10);
        break;
      case 260:
        Script_FACE(CHARA_CHAI, 2, 0, 7);
        break;
      case 600:
        Script_FACE_LCG(CHARA_CHAI, FACE_BlinkOpen, 10, 0);
        break;
    }
    if (task_6C == 1) {
      Script_handSetFloatsFor(CHARA_FUKU, 0, data_318C, 1);
      Script_handSetFloatsFor(CHARA_FUKU, 1, data_318C, 1);
      Script_FACE(CHARA_FUKU, 5, 0, 7);
      Script_FACE_LCG(CHARA_FUKU, FACE_BlinkAutomatic, 6, 0);
    }
    if (task_6C == 1) {
      script_1C79();
    }
    if ((task_6C >= 90) & (task_6C <= 105)) {
      script_1CA6(0.05f);
    }
    if (task_6C == 417) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1C79(void) {
  Script_charaPosRotUtils(SHEN_NAME('DOOR'), 12, &data_37FC, 0, CHARA_PART_Set);
  Script_charaPosRotUtils(SHEN_NAME('DOOR'), 7, &data_37FC, 0, CHARA_PART_Set);
  return;
}

static void script_1CA6(float arg0) {
  Vector3 local_F4;
  local_F4.x = arg0;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_charaPosRotUtils(SHEN_NAME('DOOR'), 12, &local_F4, 0, CHARA_PART_Add);
  local_F4.x = (float)0 - arg0;
  Script_charaPosRotUtils(SHEN_NAME('DOOR'), 7, &local_F4, 0, CHARA_PART_Add);
  return;
}

static void script_1CEF(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_motiLoad(2, SHEN_NAME('GMCT'), "M_FGT4.BIN");
  Script_vfSetup(0, 20, 20, 0, 0);
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
  task_0 = arg0;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 600;
  *(&task_0 + 5) = 6;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(arg0, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_EVT_SetPosition(CHARA_CHAI, 0x78000000, &data_3814);
  {
    Vector3i local_F0;
    (&local_F0)->y = 64129;
    Script_EVT_SetRotation(CHARA_CHAI, MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_3808);
  {
    Vector3i local_F0;
    (&local_F0)->y = 31323;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Enable, 0x80);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  {
    Vector3 local_F0;
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F0, 0);
    Script_vfStageAssignRingBoundary(&local_F0, 20.0f);
  }
  SCN_Enqueue_VFGI(2, 99, 1.0f, 0, -1);
  SCNF_WaitFrames_0000(2);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_ReallocateUNFO(local_FC);
  Script_motmToggleFlags(CHARA_AKIR, SCEN_Disable, 0x80);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x80);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_GetFightingStats(0x52494B41, 0);
  return;
}

static int32_t script_1F3D(void) {
  for (;;) {
    if (task_6C > 2170) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_382B, 1, data_3831, 2, data_383C, 3, data_3848, 4, data_3853, 5, data_385E, 6, data_386A, 7, data_3877)))
static void script_1FA2(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_LoadVoiceFile(1, "A0172");
  local_EC = Script_motiLoad(2, SHEN_NAME('GMCT'), "M_0172.BIN");
  Script_LoadSND_Wrapper("system1.snd", 0, "tsm001.snd", "amb024.snd", "a1_bgam.snd", "battle_1.snd", 0, 0);
  Script_executeAicaCommand(24232, 0, 0);
  Script_executeAicaCommand(1184, 3, 87);
  script_0E7D();
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_6C = 0;
  local_FC = Script_scenCreateTask(script_0F21, 0);
  local_F8 = Script_scenCreateTask(script_0F08, 0);
  local_F4 = Script_scenCreateTask(script_16D1, 0);
  local_F0 = Script_scenCreateTask(script_1F3D, 0);
  Script_shenSeqUtils(1);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  Script_srchControl((void *)local_F4);
  Script_srchControl((void *)local_F0);
  Script_ReallocateUNFO(local_EC);
  Script_LoadVoiceFile(0, "A0172");
  return;
}

void script_20C3(void) {
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 1018, 1);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 37);
  Script_scenCreateTask(script_20EA, 1, CHARA_AKIR);
  return;
}

static int32_t script_20EA(int32_t arg0) {
  int32_t unused_FC;
  int32_t local_F8;
  Script_ryoSetChara(arg0);
  while (1) {
    local_F8 = Script_GetCurrentEntryPoint();
    switch (local_F8) {
      case 0:
        script_039D(0);
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('YD01'), 2);
        break;
      case 1:
        script_0A3D();
        if (Script_GetFightingStats(0x52494B41, 0) == 3) {
          task_0 = -6;
          *(&task_0 + 1) = 0;
          *(&task_0 + 2) = 0;
          *(&task_0 + 3) = 0;
          *(&task_0 + 4) = 0;
          *(&task_0 + 5) = 4;
          *(&task_0 + 6) = 0;
          *(&task_0 + 7) = 0;
          *(&task_0 + 8) = 0;
          *(&task_0 + 9) = 0;
          *(&task_0 + 10) = 0;
          Script_vfUtils(&task_0);
          script_0B94();
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED, 1);
          Script_shenRequestGameModuleTransition(2, SHEN_NAME('D000'), 6);
        } else {
          task_0 = -7;
          *(&task_0 + 1) = 0;
          *(&task_0 + 2) = 0;
          *(&task_0 + 3) = 0;
          *(&task_0 + 4) = 0;
          *(&task_0 + 5) = 255;
          *(&task_0 + 6) = 0;
          *(&task_0 + 7) = 0;
          *(&task_0 + 8) = 0;
          *(&task_0 + 9) = 0;
          *(&task_0 + 10) = 0;
          Script_vfUtils(&task_0);
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED, 1);
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED, 1);
          script_194C();
          Script_shenRequestGameModuleTransition(Script_GetSceneID(), SHEN_NAME('JOMO'), 19);
        }
        break;
      case 2:
        Script_PreserveAccumulator_0141(50, 51);
        script_194C();
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('ARAR'), 1);
        break;
      case 98:
        script_0B94();
        break;
      case 99:
        script_0A3D();
        if (Script_GetFightingStats(0x52494B41, 0) == 3) {
          script_0B94();
        } else {
          script_194C();
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  end:;
}
