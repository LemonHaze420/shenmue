#include "act01_op00.h"

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

static int32_t script_0125(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0153(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0181(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_01B5(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_01E9(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static int32_t script_0207(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Script_GetDataFromFightProgram(arg0, arg1);
  local_FC = 0;
  while (local_FC < arg2) {
    local_FC = local_FC + 1;
    Script_SetFightProgramStateBytes(arg0, local_FC, 1);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0241(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  end:;
}

static int32_t script_0275(void) {
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

static int32_t script_036D(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  end:;
}

static void script_039D(void) {
  Script_ambSoundUtils(1, 0, &data_E2E0, 15.0f, 50.0f, 0.8f, 2, &data_E340);
  Script_ambSoundUtils(2, 0, &data_E2EC, 18.0f, 30.0f, 0.8f, 2, &data_E3C8);
  Script_ambSoundUtils(3, 0, &data_E2F8, 20.0f, 30.0f, 0.8f, 2, &data_E450);
  Script_ambSoundUtils(4, 0, &data_E304, 8.0f, 100.0f, 0.8f, 0, &data_E4D8);
  Script_ambSoundUtils(5, 1, data_E310, 50.0f, 40.0f, 1.0f, 0, data_E560);
  Script_ambSoundUtils(6, 1, data_E31C, 50.0f, 40.0f, 0.8f, 0, data_E5E8);
  Script_ambSoundUtils(14, 1, &data_E328, 40.0f, 15.0f, 1.0f, 3, &data_E670);
  Script_ambSoundUtils(15, 1, &data_E334, 32.0f, 15.0f, 1.0f, 3, &data_E6F8);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static int32_t script_04CA(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  int32_t local_DC;
  Script_SpriteUtils(Scroll_Read3, 0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 140.0f);
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetCamFlags(0, 1);
  Script_clmdSetFlags(CHARA_AKIR, 2, 1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
        Script_FACE(CHARA_AKIR, 10, 0, 1);
        Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
        Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 0);
        Script_charaSetRefboardFlagTo(SHEN_NAME('RMJN'), 0);
        task_2F = 0;
        break;
      case 30:
        task_2F = 1;
        break;
      case 193:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
        Script_mapmSetRenderFlag(0, 1);
        Script_mapmSetRenderFlag(1, 0);
        Script_mapmSetRenderFlag(2, 0);
        Script_mapmSetRenderFlag(3, 0);
        Script_mapmSetRenderFlag(4, 1);
        break;
      case 280:
        Script_clmdSetFlags(CHARA_AKIR, 2, 0);
        break;
      case 360:
        Script_FACE(CHARA_AKIR, 0, 0, 1);
        break;
      case 410:
        Script_FACE(CHARA_AKIR, 2, 0, 1);
        break;
      case 414:
        Script_FACE_LCG(CHARA_AKIR, FACE_BlinkClose, 3, 4);
        break;
      case 488:
        Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 12, 0);
        break;
      case 531:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
        script_3105();
        break;
      case 535:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        break;
      case 616:
        Script_scenCreateTask(script_2F29, 0);
        break;
      case 696:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
        Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 0);
        break;
      case 697:
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionDisabled, 0, 1);
        break;
      case 787:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 140.0f);
        Script_FaceEx(CHARA_AKIR, FACE_ExpressionEnabled, 0, 1);
        break;
      case 711:
        local_E0.x = 0;
        (&local_E0)->y = 50972;
        (&local_E0)->z = 0;
        Script_EVT_SetRotation(SHEN_NAME('MNLF'), 0x38000000, &local_E0, 0);
        break;
    }
    if ((local_FC > 416) & (local_FC < 475)) {
      local_EC.x = 0.0276f;
      (&local_EC)->y = 0.2f;
      (&local_EC)->z = 14.4891f;
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 4);
    } else if ((local_FC > 488) & (local_FC <= 491)) {
      local_EC.x = -3.0f;
      (&local_EC)->y = 1.1f;
      (&local_EC)->z = 15.0f;
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 20);
    } else {
      if ((local_FC > 491) & (local_FC < 535)) {
        local_EC.x = -3.0f;
        (&local_EC)->y = 1.1f;
        (&local_EC)->z = 15.0f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 4);
        goto L000840;
      }
    }
    L000840:;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0855(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  int32_t local_DC;
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetCamFlags(0, 0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_FACE(CHARA_AKIR, 2, 0, 1);
        Script_FACE(CHARA_INE_, 10, 0, 1);
        Script_FACE_LCG(CHARA_INE_, FACE_BlinkClose, 1, 0);
        Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
        local_E0.x = 0;
        (&local_E0)->y = 56979;
        (&local_E0)->z = 0;
        Script_EVT_SetRotation(SHEN_NAME('MNLF'), 0x38000000, &local_E0, 0);
        break;
      case 101:
        Script_mapmSetCamFlags(0, 1);
        break;
      case 215:
        Script_SetCharaFlags(CHARA_AKIR, 0);
        break;
      case 230:
        Script_FACE(CHARA_AKIR, 7, 0, 2);
        Script_scenCreateTask(script_036D, 2, CHARA_AKIR, 3);
        break;
      case 267:
        Script_FACE(CHARA_AKIR, 0, 0, 3);
        break;
      case 294:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_E974, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_E974, 1);
        break;
      case 348:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        break;
      case 380:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        break;
      case 400:
        Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
        break;
      case 420:
        Script_FACE(CHARA_INE_, 10, 0, 10);
        Script_FACE_LCG(CHARA_INE_, FACE_BlinkOpen, 30, 0);
        break;
      case 443:
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
        break;
      case 458:
        Script_FACE(CHARA_INE_, 7, 5, 10);
        break;
      case 510:
        Script_FACE(CHARA_INE_, 3, 0, 10);
        break;
      case 540:
        Script_FACE(CHARA_INE_, 6, 0, 10);
        break;
      case 550:
        Script_FACE(CHARA_INE_, 2, 0, 10);
        break;
      case 580:
        Script_FACE(CHARA_INE_, 8, 2, 10);
        Script_FACE(CHARA_AKIR, 2, 0, 10);
        break;
      case 673:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 140.0f);
        break;
      case 700:
        Script_FACE(CHARA_INE_, 3, 0, 10);
        break;
      case 730:
        Script_FACE(CHARA_AKIR, 3, 0, 10);
        break;
      case 737:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_DD88, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_DD88, 1);
        break;
      case 738:
        Script_SetCharaFlags(CHARA_AKIR, 4);
        break;
      case 780:
        Script_FACE(CHARA_AKIR, 0, 0, 20);
        break;
      case 810:
        Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
        break;
      case 820:
        Script_FACE(CHARA_AKIR, 2, 0, 10);
        break;
      case 869:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        break;
    }
    if ((local_FC > 320) & (local_FC < 348)) {
      Script_EVT_GetPosition(CHARA_INE_, 5, &local_EC, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 2);
    } else if ((local_FC > 380) & (local_FC < 348)) {
      Script_EVT_GetPosition(CHARA_INE_, 5, &local_EC, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 2);
    } else if ((local_FC > 650) & (local_FC < 725)) {
      local_EC.x = -2.361f;
      (&local_EC)->y = 1.06f;
      (&local_EC)->z = -21.4349f;
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 3);
    } else if ((local_FC > 725) & (local_FC < 760)) {
      Script_EVT_GetPosition(CHARA_INE_, 5, &local_EC, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 3);
    } else {
      if ((local_FC > 0x300) & (local_FC < 868)) {
        local_EC.x = -2.361f;
        (&local_EC)->y = 1.06f;
        (&local_EC)->z = -21.4349f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 3);
        goto L000DB2;
      }
    }
    L000DB2:;
    if ((local_FC > 638) & (local_FC < 730)) {
      local_EC.x = -2.361f;
      (&local_EC)->y = 1.06f;
      (&local_EC)->z = -21.4349f;
      Script_FaceLookAtTargetUtils(CHARA_INE_, FACE_LookAtPosition, &local_EC, 3);
    } else {
      if ((local_FC > 780) & (local_FC < 820)) {
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_EC, 0);
        Script_FaceLookAtTargetUtils(CHARA_INE_, FACE_LookAtPosition, &local_EC, 3);
        goto L000E2A;
      }
    }
    L000E2A:;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0E3F(void) {
  int32_t local_FC;
  int32_t unused_E0[7];
  int32_t local_DC;
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetCamFlags(0, 1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_FACE(CHARA_AKIR, 2, 0, 1);
        Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
        break;
      case 243:
        Script_scenCreateTask(script_2F29, 0);
        break;
      case 284:
        Script_SetCharaFlags(CHARA_AKIR, 0);
        break;
      case 477:
        Script_mapmSetRenderFlag(0, 1);
        Script_mapmSetRenderFlag(1, 1);
        Script_mapmSetRenderFlag(2, 0);
        Script_mapmSetRenderFlag(3, 0);
        Script_mapmSetRenderFlag(4, 0);
        break;
      case 478:
        Script_SetCharaFlags(CHARA_AKIR, 4);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0F51(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t unused_E0[3];
  int32_t local_DC;
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetCamFlags(0, 1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_FACE(CHARA_AKIR, 2, 0, 1);
        Script_FACE(CHARA_FUKU, 4, 4, 1);
        Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
        break;
      case 95:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_D830, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_D830, 1);
        break;
      case 115:
        Script_scenCreateTask(script_036D, 2, CHARA_AKIR, 2);
        break;
      case 254:
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        break;
      case 263:
        Script_scenCreateTask(script_0207, 3, CHARA_FUKU, 1, 2);
        break;
      case 269:
        Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        break;
      case 280:
        Script_FACE(CHARA_AKIR, 4, 0, 10);
        break;
      case 287:
        Script_mapmSetRenderFlag(0, 1);
        Script_mapmSetRenderFlag(1, 0);
        Script_mapmSetRenderFlag(2, 0);
        Script_mapmSetRenderFlag(3, 0);
        Script_charaDisplay(CHARA_AKIR, CDM_Show);
        break;
      case 298:
        Script_scenCreateTask(script_0241, 2, CHARA_FUKU, 2);
        break;
      case 320:
        Script_FACE(CHARA_AKIR, 7, 0, 10);
        break;
      case 397:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_D830, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_D830, 1);
        break;
      case 400:
        Script_FACE(CHARA_AKIR, 8, 0, 15);
        break;
      case 410:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_DD88, 15);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_DD88, 15);
        break;
      case 420:
        Script_scenCreateTask(script_036D, 2, CHARA_AKIR, 2);
        break;
      case 430:
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        break;
      case 435:
        Script_FACE(CHARA_AKIR, 10, 0, 30);
        break;
      case 460:
        Script_FACE(CHARA_AKIR, 0, 0, 40);
        break;
      case 482:
        Script_FACE(CHARA_AKIR, 2, 0, 6);
        break;
      case 496:
        Script_FACE(CHARA_AKIR, 0, 0, 50);
        break;
      case 520:
        Script_FACE(CHARA_AKIR, 2, 0, 30);
        break;
      case 535:
        Script_FACE(CHARA_AKIR, 5, 0, 12);
        break;
      case 538:
        Script_FACE(CHARA_AKIR, 2, 0, 10);
        break;
      case 555:
        Script_FACE(CHARA_AKIR, 5, 0, 80);
        break;
      case 576:
        Script_FACE(CHARA_AKIR, 2, 0, 6);
        break;
      case 582:
        Script_FACE(CHARA_AKIR, 0, 0, 8);
        break;
      case 645:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        break;
      case 646:
        Script_mapmSetRenderFlag(0, 1);
        Script_mapmSetRenderFlag(1, 1);
        Script_mapmSetRenderFlag(2, 0);
        Script_mapmSetRenderFlag(3, 0);
        break;
      case 652:
        Script_FACE(CHARA_AKIR, 2, 0, 10);
        break;
      case 750:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        break;
      case 0x300:
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        break;
      case 770:
        Script_mapmSetRenderFlag(0, 1);
        Script_mapmSetRenderFlag(1, 0);
        Script_mapmSetRenderFlag(2, 0);
        Script_mapmSetRenderFlag(3, 0);
        Script_charaDisplay(CHARA_FUKU, CDM_Hide);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 140.0f);
        break;
      case 926:
        task_2F = 0;
        break;
    }
    if ((local_FC > 425) & (local_FC < 555)) {
      Script_EVT_GetPosition(CHARA_FUKU, 5, &local_EC, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 4);
    } else if ((local_FC > 555) & (local_FC < 576)) {
      Script_EVT_GetPosition(SHEN_NAME('ODR1'), -1, &local_EC, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 5);
    } else if ((local_FC > 576) & (local_FC < 645)) {
      Script_EVT_GetPosition(CHARA_FUKU, 5, &local_EC, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 3);
    } else {
      if ((local_FC < 682) & (local_FC > 750)) {
        Script_EVT_GetPosition(SHEN_NAME('ODR1'), -1, &local_EC, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_EC, 3);
        goto L001506;
      }
    }
    L001506:;
    if ((local_FC > 327) & (local_FC < 397)) {
      if (local_FC & 4) {
        Script_FACE(CHARA_FUKU, 4, 0, 12);
      } else {
        Script_FACE(CHARA_FUKU, 4, 1, 12);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1558(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t local_D8;
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Show);
      goto L001582;
    }
  }
  L001582:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Show);
      goto L0015B6;
    }
  }
  L0015B6:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Show);
      goto L0015EA;
    }
  }
  L0015EA:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Show);
      goto L00161E;
    }
  }
  L00161E:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Show);
      goto L001652;
    }
  }
  L001652:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI5'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI5'), CDM_Show);
      goto L001686;
    }
  }
  L001686:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI5'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Show);
      goto L0016BA;
    }
  }
  L0016BA:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Show);
      goto L0016EE;
    }
  }
  L0016EE:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Show);
      goto L001722;
    }
  }
  L001722:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Show);
      goto L001756;
    }
  }
  L001756:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Show);
      goto L00178A;
    }
  }
  L00178A:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Show);
      goto L0017BE;
    }
  }
  L0017BE:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS5'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Show);
      goto L0017F2;
    }
  }
  L0017F2:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS6'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Show);
      goto L001826;
    }
  }
  L001826:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS7'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Show);
      goto L00185A;
    }
  }
  L00185A:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS8'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Show);
      goto L00188E;
    }
  }
  L00188E:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS9'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASA'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASA'), CDM_Show);
      goto L0018C2;
    }
  }
  L0018C2:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASA'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASB'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASB'), CDM_Show);
      goto L0018F6;
    }
  }
  L0018F6:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASB'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASC'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASC'), CDM_Show);
      goto L00192A;
    }
  }
  L00192A:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASC'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASD'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASD'), CDM_Show);
      goto L00195E;
    }
  }
  L00195E:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASD'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASE'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASE'), CDM_Show);
      goto L001992;
    }
  }
  L001992:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASE'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASF'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASF'), CDM_Show);
      goto L0019C6;
    }
  }
  L0019C6:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASF'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Show);
      goto L0019FA;
    }
  }
  L0019FA:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Show);
      goto L001A2E;
    }
  }
  L001A2E:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Show);
      goto L001A62;
    }
  }
  L001A62:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Show);
      goto L001A96;
    }
  }
  L001A96:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Show);
      goto L001ACA;
    }
  }
  L001ACA:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN4'), 1);
  local_E8.x = 9.2f;
  (&local_E8)->y = -0.5f;
  (&local_E8)->z = 7.2f;
  Script_EVT_SetPosition(SHEN_NAME('KOI0'), POS_XYZ, &local_E8);
  local_F4.x = 1.4f;
  (&local_F4)->y = 1.4f;
  (&local_F4)->z = 1.4f;
  Script_hmdlApplyScale(SHEN_NAME('KOI0'), &local_F4);
  Script_tmnmUtils(SHEN_NAME('KOI0'), 57354, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('KOI0'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('KOI0'), 0, -1.0f, -1.0f, -1.0f, 0.6f, -1);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('KOI0'), &data_E78C, 0x10000, 72);
  local_E8.x = 9.2f;
  (&local_E8)->y = -0.6f;
  (&local_E8)->z = 8.2f;
  Script_EVT_SetPosition(SHEN_NAME('KOI1'), POS_XYZ, &local_E8);
  local_F4.x = 1.4f;
  (&local_F4)->y = 1.4f;
  (&local_F4)->z = 1.4f;
  Script_hmdlApplyScale(SHEN_NAME('KOI1'), &local_F4);
  Script_tmnmUtils(SHEN_NAME('KOI1'), 57354, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('KOI1'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('KOI1'), 0, -1.0f, -1.0f, -1.0f, 0.8f, -1);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('KOI1'), &data_E800, 0x10000, 72);
  local_E8.x = 5.7f;
  (&local_E8)->y = -0.25f;
  (&local_E8)->z = 7.6f;
  Script_EVT_SetPosition(SHEN_NAME('KOI2'), POS_XYZ, &local_E8);
  local_F4.x = 1.6f;
  (&local_F4)->y = 1.6f;
  (&local_F4)->z = 1.6f;
  Script_hmdlApplyScale(SHEN_NAME('KOI2'), &local_F4);
  Script_tmnmUtils(SHEN_NAME('KOI2'), 57354, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('KOI2'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('KOI2'), 0, -1.0f, -1.0f, -1.0f, 0.7f, -1);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('KOI2'), &data_E844, 0x10000, 72);
  local_E8.x = 6.5f;
  (&local_E8)->y = -0.5f;
  (&local_E8)->z = 9.2f;
  Script_EVT_SetPosition(SHEN_NAME('KOI3'), POS_XYZ, &local_E8);
  local_F4.x = 1.4f;
  (&local_F4)->y = 1.4f;
  (&local_F4)->z = 1.4f;
  Script_hmdlApplyScale(SHEN_NAME('KOI3'), &local_F4);
  Script_tmnmUtils(SHEN_NAME('KOI3'), 57354, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('KOI3'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('KOI3'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('KOI3'), &data_E888, 0x10000, 72);
  local_E8.x = 9.4f;
  (&local_E8)->y = -0.3f;
  (&local_E8)->z = 6.8f;
  Script_EVT_SetPosition(SHEN_NAME('KOI4'), POS_XYZ, &local_E8);
  local_F4.x = 1.8f;
  (&local_F4)->y = 1.8f;
  (&local_F4)->z = 1.8f;
  Script_hmdlApplyScale(SHEN_NAME('KOI4'), &local_F4);
  Script_tmnmUtils(SHEN_NAME('KOI4'), 57354, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('KOI4'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('KOI4'), 0, -1.0f, -1.0f, -1.0f, 0.9f, -1);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('KOI4'), &data_E8DC, 0x10000, 72);
  local_E8.x = 7.1f;
  (&local_E8)->y = -0.3f;
  (&local_E8)->z = 7.5f;
  Script_EVT_SetPosition(SHEN_NAME('KOI5'), POS_XYZ, &local_E8);
  local_F4.x = 1.5f;
  (&local_F4)->y = 1.5f;
  (&local_F4)->z = 1.5f;
  Script_hmdlApplyScale(SHEN_NAME('KOI5'), &local_F4);
  Script_tmnmUtils(SHEN_NAME('KOI5'), 57354, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('KOI5'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('KOI5'), 0, -1.0f, -1.0f, -1.0f, 1.1f, -1);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, SHEN_NAME('KOI5'), &data_E930, 0x10000, 72);
  local_F4.x = 1.2f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 1.2f;
  Script_hmdlApplyScale(SHEN_NAME('HAS3'), &local_F4);
  local_F4.x = 0.8f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 0.8f;
  Script_hmdlApplyScale(SHEN_NAME('HAS5'), &local_F4);
  local_F4.x = 1.2f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 1.2f;
  Script_hmdlApplyScale(SHEN_NAME('HAS6'), &local_F4);
  local_F4.x = 1.6f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 1.6f;
  Script_hmdlApplyScale(SHEN_NAME('HAS7'), &local_F4);
  local_F4.x = 0.6f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 0.6f;
  Script_hmdlApplyScale(SHEN_NAME('HAS8'), &local_F4);
  local_F4.x = 1.3f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 1.3f;
  Script_hmdlApplyScale(SHEN_NAME('HAS9'), &local_F4);
  local_F4.x = 0.7f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 0.7f;
  Script_hmdlApplyScale(SHEN_NAME('HASA'), &local_F4);
  local_F4.x = 0.9f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 0.9f;
  Script_hmdlApplyScale(SHEN_NAME('HASB'), &local_F4);
  local_F4.x = 1.2f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = 1.2f;
  Script_hmdlApplyScale(SHEN_NAME('HASC'), &local_F4);
  Script_MT5Loader_TRWV_TRWAVE(1, task_18);
  local_D8 = 0;
  while (local_D8 < 220) {
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('KOI0'), 0.008f);
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('KOI1'), 0.005f);
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('KOI2'), 0.007f);
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('KOI3'), 0.006f);
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('KOI4'), 0.01f);
    Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, SHEN_NAME('KOI5'), 0.006f);
    Script_EVT_GetPosition(SHEN_NAME('HAS0'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAS0'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAS1'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAS1'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAS2'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAS2'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAS3'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAS3'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAS4'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAS4'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAS5'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAS5'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAS6'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAS6'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAS7'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAS7'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAS8'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAS8'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAS9'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAS9'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HASA'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HASA'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HASB'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HASB'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HASC'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HASC'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HASD'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HASD'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HASE'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HASE'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HASF'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HASF'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAN0'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAN0'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAN1'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAN1'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAN2'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAN2'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAN3'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAN3'), POS_XYZ, &local_E8);
    Script_EVT_GetPosition(SHEN_NAME('HAN4'), -1, &local_E8, 0);
    local_DC.x = (&local_E8)->x - (&data_E780)->x;
    (&local_DC)->z = (&local_E8)->z - (&data_E780)->z;
    *(int32_t *)&(&local_E8)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_18, &local_DC);
    Script_EVT_SetPosition(SHEN_NAME('HAN4'), POS_XYZ, &local_E8);
    local_D8 = local_D8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_tmnmReset(SHEN_NAME('KOI0'));
  Script_tmnmReset(SHEN_NAME('KOI1'));
  Script_tmnmReset(SHEN_NAME('KOI2'));
  Script_tmnmReset(SHEN_NAME('KOI3'));
  Script_tmnmReset(SHEN_NAME('KOI4'));
  Script_tmnmReset(SHEN_NAME('KOI5'));
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Show);
      goto L0029BA;
    }
  }
  L0029BA:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Show);
      goto L0029EE;
    }
  }
  L0029EE:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Show);
      goto L002A22;
    }
  }
  L002A22:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Show);
      goto L002A56;
    }
  }
  L002A56:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Show);
      goto L002A8A;
    }
  }
  L002A8A:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI5'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI5'), CDM_Show);
      goto L002ABE;
    }
  }
  L002ABE:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI5'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Show);
      goto L002AF2;
    }
  }
  L002AF2:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Show);
      goto L002B26;
    }
  }
  L002B26:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Show);
      goto L002B5A;
    }
  }
  L002B5A:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Show);
      goto L002B8E;
    }
  }
  L002B8E:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Show);
      goto L002BC2;
    }
  }
  L002BC2:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Show);
      goto L002BF6;
    }
  }
  L002BF6:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS5'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Show);
      goto L002C2A;
    }
  }
  L002C2A:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS6'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Show);
      goto L002C5E;
    }
  }
  L002C5E:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS7'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Show);
      goto L002C92;
    }
  }
  L002C92:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS8'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Show);
      goto L002CC6;
    }
  }
  L002CC6:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS9'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASA'), CDM_Show);
      goto L002CFA;
    }
  }
  L002CFA:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASB'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASB'), CDM_Show);
      goto L002D2E;
    }
  }
  L002D2E:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASB'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASC'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASC'), CDM_Show);
      goto L002D62;
    }
  }
  L002D62:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASC'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASD'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASD'), CDM_Show);
      goto L002D96;
    }
  }
  L002D96:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASD'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASE'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASE'), CDM_Show);
      goto L002DCA;
    }
  }
  L002DCA:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASE'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASF'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASF'), CDM_Show);
      goto L002DFE;
    }
  }
  L002DFE:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASF'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Show);
      goto L002E32;
    }
  }
  L002E32:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Show);
      goto L002E66;
    }
  }
  L002E66:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Show);
      goto L002E9A;
    }
  }
  L002E9A:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Show);
      goto L002ECE;
    }
  }
  L002ECE:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Show);
      goto L002F02;
    }
  }
  L002F02:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN4'), 0);
  Script_MT5Loader_TRWV_TRWAVE(2, task_18);
  Script_MT5Loader_TRWV_TRWAVE(3, task_18);
  return Script_srchControl(0);
}

static int32_t script_2F29(void) {
  if (task_4 != 1) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_EA58);
    Script_SetFogFlags(0, data_EA68);
    Script_shenSetLightingMode(1);
    Script_lightDirLightEnable(0, &data_EA98);
    Script_lightSetGlobalSceneLighting(&data_EA78);
    Script_LightUtils(0, 2, &data_EA88);
    Script_LightUtils(0, 4, 1.0f);
    Script_lightSetSlotEnabled(0, 1);
    task_4 = 1;
  }
  end:;
}

static void script_2F96(void) {
  if (task_4 != 5) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_EAA4);
    Script_SetFogFlags(0, data_EAB4);
    Script_shenSetLightingMode(2);
    {
      int32_t unused_F0[4];
      Vector3 local_E4;
      local_E4.x = 0.0f;
      (&local_E4)->y = -0.71f;
      (&local_E4)->z = -0.71f;
      Script_lightDirLightEnable(1, &local_E4);
    }
    Script_lightSetGlobalSceneLighting(&data_EAC4);
    {
      Vector4 local_F0;
      local_F0.x = 1.0f;
      (&local_F0)->y = 0.82f;
      (&local_F0)->z = 0.82f;
      (&local_F0)->w = 1.0f;
      Script_LightUtils(1, 2, &local_F0);
    }
    Script_LightUtils(1, 4, 0.14f);
    Script_lightSetSlotEnabled(1, 1);
    {
      int32_t unused_F0[4];
      Vector3 local_E4;
      Vector3 local_D8;
      local_E4.x = 1.0f;
      (&local_E4)->y = -0.28f;
      (&local_E4)->z = 0.15f;
      local_D8.x = 9.0f;
      (&local_D8)->y = 1.72f;
      (&local_D8)->z = -7.22f;
      Script_lightSpotLightEnable(2, &local_D8, &local_E4);
    }
    {
      Vector4 local_F0;
      local_F0.x = 1.0f;
      (&local_F0)->y = 0.82f;
      (&local_F0)->z = 0.82f;
      (&local_F0)->w = 1.0f;
      Script_LightUtils(2, 2, &local_F0);
    }
    Script_LightUtils(2, 4, 1.0f);
    Script_LightUtils(2, 5, 9.0f);
    Script_LightUtils(2, 6, data_EAD4);
    Script_LightUtils(2, 10, 13653);
    Script_lightSetSlotEnabled(2, 1);
    task_4 = 5;
  }
  return;
}

static void script_3105(void) {
  if (task_4 != 23) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_EADC);
    Script_SetFogFlags(0, data_EAEC);
    Script_shenSetLightingMode(1);
    Script_lightDirLightEnable(0, &data_EB1C);
    Script_lightSetGlobalSceneLighting(&data_EAFC);
    Script_LightUtils(0, 2, &data_EB0C);
    Script_LightUtils(0, 4, 1.0f);
    Script_lightSetSlotEnabled(0, 1);
    task_4 = 23;
  }
  return;
}

static int32_t script_3172(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    switch (local_EC) {
      case 0:
        Script_FACE_LCG(CHARA_IWAO, FACE_BlinkOpen, 1, 0);
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkOpen, 1, 0);
        Script_FACE(CHARA_AKIR, 2, 6, 1);
        Script_FACE(CHARA_SORY, 0, 0, 1);
        Script_ResetHandMotion(CHARA_IWAO, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_D830, 1);
        task_2F = 0;
        break;
      case 30:
        task_2F = 1;
        break;
      case 260:
        Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
        break;
      case 318:
        Script_FACE(CHARA_SORY, 2, 0, 10);
        break;
      case 445:
        Script_FACE(CHARA_AKIR, 7, 0, 3);
        break;
      case 465:
        Script_FACE(CHARA_AKIR, 2, 0, 6);
        break;
      case 470:
        Script_FACE(CHARA_IWAO, 2, 6, 30);
        break;
      case 490:
        Script_EVT_GetPosition(CHARA_SORY, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        (&local_F4)->y = (&local_F4)->y + 0.2f;
        Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookAtPosition, &local_F4, 15);
        break;
      case 700:
        Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookReset);
        break;
      case 735:
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookReset);
        break;
    }
    if ((local_F0 > 703) & (local_F0 < 722)) {
      Script_EVT_GetPosition(SHEN_NAME('FIR2'), -1, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 4);
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_337F(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    switch (local_EC) {
      case 0:
        Script_CharHandlers(CHARA_IWAO, 2, 0, 16);
        break;
      case 207:
        Script_ResetHandMotion(CHARA_IWAO, 1, 1, 0);
        Script_ResetHandMotion(CHARA_IWAO, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_D830, 1);
        Script_handSetFloatsFor(CHARA_IWAO, 0, data_D830, 1);
        break;
      case 280:
        Script_osagUtil(CHARA_SORY, 0x40400000);
        break;
      case 320:
        Script_scenCreateTask(script_0207, 3, CHARA_SORY, 2, 2);
        break;
      case 321:
        Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
        Script_ResetHandMotion(CHARA_IWAO, 0, 0, 0);
        break;
      case 370:
        Script_osagUtil(CHARA_SORY, 0x3E0F5C29);
        break;
      case 385:
        Script_scenCreateTask(script_0241, 2, CHARA_SORY, 2);
        break;
      case 410:
        Script_osagUtil(CHARA_SORY, 0x40400000);
        break;
      case 415:
        Script_scenCreateTask(script_0207, 3, CHARA_SORY, 1, 2);
        break;
      case 440:
        Script_scenCreateTask(script_0241, 2, CHARA_SORY, 2);
        Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookReset);
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookReset);
        break;
      case 488:
        Script_FACE(CHARA_SORY, 2, 6, 30);
        break;
      case 489:
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkOpen, 1, 0);
        break;
      case 500:
        Script_osagUtil(CHARA_SORY, 0x3F800000);
        break;
      case 520:
        Script_ResetHandMotion(CHARA_IWAO, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_D914, 1);
        break;
      case 523:
        Script_FACE(CHARA_IWAO, 2, 6, 30);
        break;
      case 524:
        Script_FACE_LCG(CHARA_IWAO, FACE_BlinkOpen, 1, 0);
        break;
      case 525:
        Script_FACE(CHARA_IWAO, 2, 6, 30);
        break;
      case 528:
        Script_FACE(CHARA_IWAO, 4, 6, 15);
        break;
      case 531:
        Script_FACE(CHARA_IWAO, 2, 6, 10);
        break;
      case 534:
        Script_FACE(CHARA_IWAO, 4, 6, 15);
        break;
      case 537:
        Script_FACE(CHARA_IWAO, 2, 6, 10);
        break;
      case 540:
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_D914, 40);
        Script_FACE(CHARA_IWAO, 4, 6, 15);
        break;
      case 543:
        Script_FACE(CHARA_IWAO, 2, 6, 10);
        break;
      case 547:
        Script_FACE(CHARA_IWAO, 4, 6, 15);
        break;
      case 550:
        Script_FACE(CHARA_IWAO, 5, 0, 8);
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_D9F8, 30);
        break;
      case 552:
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_D914, 30);
        break;
      case 554:
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_D9F8, 30);
        break;
      case 556:
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_D914, 30);
        break;
      case 558:
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_D9F8, 30);
        break;
      case 565:
        Script_FACE(CHARA_IWAO, 2, 0, 10);
        Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
        break;
      case 570:
        Script_osagUtil(CHARA_SORY, 0x40400000);
        break;
      case 576:
        Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookReset);
        break;
      case 580:
        Script_scenCreateTask(script_0207, 3, CHARA_SORY, 2, 2);
        break;
      case 610:
        Script_scenCreateTask(script_0241, 2, CHARA_SORY, 2);
        break;
      case 654:
        Script_FACE(CHARA_IWAO, 4, 6, 30);
        break;
      case 656:
        Script_scenCreateTask(script_0207, 3, CHARA_IWAO, 1, 2);
        break;
      case 670:
        Script_scenCreateTask(script_0241, 2, CHARA_IWAO, 2);
        break;
      case 674:
        Script_FACE(CHARA_IWAO, 7, 6, 30);
        break;
      case 680:
        Script_FACE(CHARA_IWAO, 4, 6, 30);
        Script_CharHandlers(CHARA_IWAO, 2, 8, 16);
        break;
      case 760:
        Script_osagUtil(CHARA_SORY, 0x3F800000);
        break;
      case 764:
        Script_scenCreateTask(script_0207, 3, CHARA_IWAO, 1, 4);
        Script_ResetHandMotion(CHARA_IWAO, 1, 1, 0);
        Script_ResetHandMotion(CHARA_IWAO, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_DD88, 1);
        Script_handSetFloatsFor(CHARA_IWAO, 0, data_DD88, 1);
        break;
      case 770:
        Script_FACE(CHARA_SORY, 0, 6, 30);
        break;
      case 845:
        Script_scenCreateTask(script_0241, 2, CHARA_IWAO, 4);
        break;
    }
    if ((local_F0 > 0) & (local_F0 < 170)) {
      Script_EVT_GetPosition(CHARA_IWAO, 5, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 2);
    } else if ((local_F0 > 173) & (local_F0 < 260)) {
      local_F4.x = -3.2528f;
      (&local_F4)->y = 2.3257f;
      (&local_F4)->z = -16.972601f;
      Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 80);
    } else if ((local_F0 > 322) & (local_F0 < 440)) {
      Script_EVT_GetPosition(CHARA_SORY, 5, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookAtPosition, &local_F4, 2);
      Script_EVT_GetPosition(CHARA_IWAO, 5, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 2);
    } else {
      if ((local_F0 > 450) & (local_F0 < 576)) {
        Script_EVT_GetPosition(CHARA_SORY, 5, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookAtPosition, &local_F4, 2);
        goto L003AA6;
      }
    }
    L003AA6:;
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_3ABB(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    switch (local_EC) {
      case 0:
        Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
        Script_ResetHandMotion(CHARA_IWAO, 0, 0, 0);
        Script_charaDisplay(CHARA_KURA, CDM_Show);
        Script_charaDisplay(CHARA_KURB, CDM_Show);
        Script_FACE(CHARA_AKIR, 7, 4, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_D830, 30);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_D830, 30);
        break;
      case 20:
        Script_FACE(CHARA_AKIR, 2, 0, 2);
        break;
      case 50:
        Script_charaDisplay(CHARA_KURA, CDM_Hide);
        Script_charaDisplay(CHARA_KURB, CDM_Hide);
        break;
      case 65:
        Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
        Script_SetNewHandMotion(CHARA_SORY, 1, 9504, 0, 0, 0, 0, 1.0f, 0);
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 0, 0, 1);
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 1, 20.0f);
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 0, 0, 4);
        break;
      case 67:
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 0, 1, 4);
        break;
      case 77:
        Script_scenCreateTask(script_0207, 3, CHARA_AKIR, 2, 3);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_DD88, 30);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_DD88, 30);
        break;
      case 110:
        Script_scenCreateTask(script_0241, 2, CHARA_AKIR, 3);
        break;
      case 140:
        Script_FACE(CHARA_AKIR, 4, 6, 30);
        break;
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_3CED(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  int32_t local_E4;
  int32_t unused_DC[2];
  int32_t local_D8;
  int32_t local_D4;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_E4 = 0;
  local_D8 = 0x8000;
  while (Script_shenSeqUtils(-1)) {
    local_D4 = local_E4;
    switch (local_D4) {
      case 0:
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_FACE(CHARA_IWAO, 4, 6, 1);
        Script_FACE(CHARA_AKIR, 4, 6, 1);
        Script_handSetFloatsFor(CHARA_SORY, 1, data_D830, 1);
        Script_ResetHandMotion(CHARA_SORY, 0, 1, 0);
        Script_CharHandlers(CHARA_SORY, 2, 8, 16);
        break;
      case 70:
        Script_ResetHandMotion(CHARA_IWAO, 1, 1, 0);
        Script_ResetHandMotion(CHARA_IWAO, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_DD88, 1);
        Script_handSetFloatsFor(CHARA_IWAO, 0, data_E1FC, 1);
        break;
      case 226:
        local_F4.x = -3.05f;
        (&local_F4)->y = 0.35f;
        (&local_F4)->z = -28.52f;
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkOpen, 1, 0);
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 18);
        break;
      case 227:
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkClose, 16, 4);
        break;
      case 250:
        Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
        Script_ResetHandMotion(CHARA_IWAO, 0, 0, 0);
        break;
      case 298:
        local_F4.x = -8.05f;
        (&local_F4)->y = 0.6f;
        (&local_F4)->z = -28.52f;
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 9);
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkOpen, 16, 0);
        break;
      case 314:
        Script_FACE(CHARA_SORY, 7, 6, 3);
        break;
      case 317:
        Script_FACE(CHARA_SORY, 0, 6, 3);
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkOpen, 1, 0);
        break;
      case 350:
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookReset);
        break;
    }
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_3F5D(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    switch (local_E0) {
      case 0:
        Script_SetOsageFlags(0, 0.85f);
        Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
        Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_SORY, 1, data_D830, 1);
        Script_FACE(CHARA_AKIR, 4, 1, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_DD88, 1);
        local_E4.x = -4004;
        (&local_E4)->y = 5461;
        (&local_E4)->z = 0;
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E4);
        break;
      case 98:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_E1FC, 10);
        break;
      case 190:
        Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
        Script_SetNewHandMotion(CHARA_SORY, 0, 9503, 0, 0, 0, 0, 1.0f, 0);
        Script_ConfigHandMotionIK(CHARA_SORY, 0, 0, 0, 1);
        Script_ConfigHandMotionIK(CHARA_SORY, 0, 1, 20.0f);
        Script_ConfigHandMotionIK(CHARA_SORY, 0, 0, 0, 4);
        break;
      case 192:
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 0, 1, 4);
        break;
      case 283:
        Script_InitializeOsag(CHARA_SORY);
        Script_SetOsageFlags(0, 1.0f);
        break;
    }
    if ((local_F0 > 88) & (local_F0 < 98)) {
      local_E4.x = (&local_E4)->x + 145;
      (&local_E4)->y = (&local_E4)->y - 254;
      Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E4);
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_4160(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  script_6F0F(CHARA_SORY);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    switch (local_E0) {
      case 0:
        Script_SetOsageFlags(1, 0.8f);
        Script_SetOsageFlags(11, -0x3F400000);
        Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
        Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_SORY, 0, data_EB28, 1);
        Script_handSetFloatsFor(CHARA_SORY, 1, data_D830, 1);
        local_E4.x = 0;
        (&local_E4)->y = 10012;
        (&local_E4)->z = 182;
        Script_SetHandRootRotation(CHARA_SORY, 0, 0x15, &local_E4);
        Script_CharHandlers(CHARA_AKIR, 2, 2, 16);
        local_E4.x = 0;
        (&local_E4)->y = 0;
        (&local_E4)->z = 0;
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E4);
        Script_charaDisplay(CHARA_IWAO, CDM_Hide);
        Script_osagUtil(CHARA_SORY, 0x3DF5C28F);
        break;
      case 2:
        Script_ConfigHandMotionIK(CHARA_SORY, 0, 0, 1, 4);
        break;
      case 3:
        Script_FACE(CHARA_AKIR, 11, 1, 1);
        break;
      case 154:
        Script_charaDisplay(CHARA_IWAO, CDM_Show);
        script_6EE1(CHARA_SORY);
        break;
    }
    Script_EVT_GetPosition(CHARA_SORY, 12, &local_F4, 0);
    local_F4.x = (&local_F4)->x - 0.11f;
    (&local_F4)->y = (&local_F4)->y - 0.06f;
    (&local_F4)->z = (&local_F4)->z - 0.16f;
    Script_SetMotnFlags(CHARA_AKIR, 17, &local_F4, 0x78000000);
    Script_EVT_GetPosition(CHARA_SORY, 12, &local_F4, 0);
    local_F4.x = (&local_F4)->x - 0.12f;
    (&local_F4)->y = (&local_F4)->y - 0.04f;
    (&local_F4)->z = (&local_F4)->z + 0.08f;
    Script_SetMotnFlags(CHARA_AKIR, 11, &local_F4, 0x78000000);
    Script_CharHandlers(CHARA_AKIR, 2, 6, 16);
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_43B6(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    switch (local_E0) {
      case 0:
        Script_SetOsageFlags(1, 1.0f);
        Script_SetOsageFlags(11, 0);
        Script_SetOsageFlags(21, 0);
        Script_SetOsageFlags(31, 0);
        Script_CharHandlers(CHARA_AKIR, 2, 2, 16);
        Script_ResetHandMotion(CHARA_SORY, 0, 1, 0);
        Script_ResetHandMotion(CHARA_IWAO, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_IWAO, 0, data_EDD4, 1);
        local_E4.x = 2730;
        (&local_E4)->y = 2912;
        (&local_E4)->z = 0;
        Script_SetHandRootRotation(CHARA_IWAO, 0, 0x15, &local_E4);
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkClose, 1, 6);
        Script_clmdSetFlags(CHARA_SORY, 4, 0);
        Script_InitializeOsag(CHARA_SORY);
        Script_osagUtil(CHARA_SORY, 0x3F800000);
        break;
      case 3:
        Script_FACE(CHARA_AKIR, 11, 3, 1);
        Script_FACE(CHARA_IWAO, 4, 6, 1);
        break;
      case 8:
        Script_handSetFloatsFor(CHARA_IWAO, 0, data_EEB8, 4);
        break;
      case 114:
        Script_FACE(CHARA_AKIR, 11, 0, 10);
        Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SORY, 0, data_EB28, 1);
        local_E4.x = -182;
        (&local_E4)->y = 364;
        (&local_E4)->z = -7463;
        Script_SetHandRootRotation(CHARA_SORY, 0, 0x15, &local_E4);
        Script_ResetHandMotion(CHARA_IWAO, 0, 0, 0);
        break;
      case 122:
        script_6F0F(CHARA_AKIR);
        Script_FACE(CHARA_AKIR, 11, 2, 2090);
        break;
      case 123:
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookReset);
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkOpen, 1, 1);
        break;
    }
    if ((local_F0 > 1) & (local_F0 < 123)) {
      Script_EVT_GetPosition(SHEN_NAME('ODR1'), -1, &local_F4, 0);
      (&local_F4)->y = (&local_F4)->y + 1.1f;
      (&local_F4)->z = (&local_F4)->z;
      Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 3);
    }
    if ((local_F0 > 122) & (local_F0 < 127)) {
      (&local_E4)->y = (&local_E4)->y - 109;
      (&local_E4)->z = (&local_E4)->z - 36;
      Script_SetHandRootRotation(CHARA_SORY, 0, 0x15, &local_E4);
    }
    if ((local_F0 > 122) & (local_F0 < 180)) {
      if (local_F0 & 2) {
        Script_handSetFloatsFor(CHARA_SORY, 0, data_EC0C, 29);
        Script_FACE(CHARA_AKIR, 11, 2, 1029);
      } else {
        Script_handSetFloatsFor(CHARA_SORY, 0, data_EB28, 30);
        Script_FACE(CHARA_AKIR, 11, 5, 1029);
      }
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_46DE(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  int32_t local_DC;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_F0;
    switch (local_DC) {
      case 0:
        Script_SetOsageFlags(1, 0.85f);
        local_E0 = 0x3DE147AE;
        Script_CharHandlers(CHARA_AKIR, 2, 2, 16);
        Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_SORY, 1, data_D830, 1);
        local_E4.x = 0;
        (&local_E4)->y = 0;
        (&local_E4)->z = 0;
        Script_SetHandRootRotation(CHARA_SORY, 0, 0x15, &local_E4);
        Script_EVT_GetPosition(CHARA_IWAO, -1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 1);
        break;
      case 3:
        Script_FACE(CHARA_AKIR, 4, 1, 3);
        Script_FACE(CHARA_IWAO, 4, 6, 3);
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookReset);
        break;
      case 8:
        Script_FACE(CHARA_SORY, 7, 6, 20);
        break;
      case 13:
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkClose, 5, 0);
        break;
      case 18:
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkOpen, 10, 0);
        break;
      case 100:
        Script_FACE(CHARA_SORY, 0, 6, 10);
        break;
      case 211:
        Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
        Script_SetNewHandMotion(CHARA_SORY, 1, 9502, 0, 0, 0, 0, 1.0f, 0);
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 0, 0, 1);
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 1, 20.0f);
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 0, 0, 4);
        break;
      case 213:
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 0, 1, 4);
        break;
    }
    Script_EVT_GetPosition(CHARA_SORY, 12, &local_F4, 0);
    local_F4.x = (&local_F4)->x - 0.13f;
    (&local_F4)->y = (&local_F4)->y - 0.02f;
    (&local_F4)->z = (&local_F4)->z - 0.18f;
    Script_SetMotnFlags(CHARA_AKIR, 17, &local_F4, 0x78000000);
    Script_EVT_GetPosition(CHARA_SORY, 12, &local_F4, 0);
    local_F4.x = (&local_F4)->x - 0.12f;
    (&local_F4)->y = (&local_F4)->y - 0.04f;
    (&local_F4)->z = (&local_F4)->z + 0.08f;
    Script_SetMotnFlags(CHARA_AKIR, 11, &local_F4, 0x78000000);
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_49A9(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  script_6EE1(CHARA_AKIR);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    switch (local_EC) {
      case 0:
        Script_SetOsageFlags(1, 1.0f);
        Script_CharHandlers(CHARA_AKIR, 2, 2, 16);
        break;
      case 51:
        Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookReset);
        break;
      case 150:
        Script_ResetHandMotion(CHARA_IWAO, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_IWAO, 0, data_DD88, 1);
        break;
    }
    if (local_F0 < 51) {
      Script_EVT_GetPosition(CHARA_SORY, 5, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookAtPosition, &local_F4, 1);
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_4A9C(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    switch (local_EC) {
      case 0:
        Script_ResetHandMotion(CHARA_IWAO, 0, 0, 0);
        Script_CharHandlers(CHARA_AKIR, 2, 2, 16);
        break;
      case 5:
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkOpen, 5, 0);
        break;
      case 10:
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkClose, 30, 0);
        local_F4.x = -6.13f;
        (&local_F4)->y = 2.38f;
        (&local_F4)->z = -22.19f;
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 18);
        break;
      case 25:
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkOpen, 16, 2);
        break;
      case 70:
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookReset);
        break;
      case 110:
        Script_ResetHandMotion(CHARA_SORY, 0, 0, 0);
        Script_ResetHandMotion(CHARA_SORY, 0, 1, 0);
        break;
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_4BE1(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    switch (local_EC) {
      case 0:
        Script_FACE_LCG(CHARA_SORY, FACE_BlinkOpen, 1, 0);
        Script_scenCreateTask(script_0207, 3, CHARA_AKIR, 1, 3);
        break;
      case 45:
        Script_scenCreateTask(script_036D, 2, CHARA_SORY, 2);
        break;
      case 60:
        Script_scenCreateTask(script_0241, 2, CHARA_AKIR, 3);
        break;
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_4C9D(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    if (local_EC == 0) goto L004CE4;
    L004CE4:;
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_4CF9(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  Script_SetOsageFlags(1, 0.9f);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    switch (local_E0) {
      case 0:
        Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
        Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SORY, 1, data_DD88, 1);
        Script_handSetFloatsFor(CHARA_SORY, 0, data_DD88, 1);
        local_F4.x = 0.06f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = 0.08f;
        local_E4.x = 10376;
        (&local_E4)->y = 0;
        (&local_E4)->z = 50244;
        Script_fixoAttachEx(SHEN_NAME('RYUK'), CHARA_SORY, 12, &local_F4, &local_E4);
        Script_SetOsageFlags(0, 1.0f);
        Script_SetOsageFlags(10, 0);
        Script_SetOsageFlags(20, 0);
        Script_SetOsageFlags(30, 0);
        break;
      case 420:
        Script_fixoClearAttachment(SHEN_NAME('RYUK'));
        Script_SetOsageFlags(1, 1.0f);
        break;
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_4E3D(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    switch (local_EC) {
      case 0:
        Script_FACE(CHARA_IWAO, 4, 6, 1);
        break;
      case 149:
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookReset);
        break;
    }
    if ((local_F0 > 0) & (local_F0 < 149)) {
      Script_EVT_GetPosition(CHARA_IWAO, 5, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 2);
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_4EF1(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Vector3i local_E4;
  Vector3i local_D8;
  float local_D4;
  int32_t local_D0;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_D0 = local_F0;
    switch (local_D0) {
      case 0:
        local_D4 = 1.0f;
        Script_FACE(CHARA_IWAO, 4, 6, 1);
        Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
        Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SORY, 1, data_DD88, 1);
        Script_handSetFloatsFor(CHARA_SORY, 0, data_DD88, 1);
        Script_ResetHandMotion(CHARA_IWAO, 1, 1, 0);
        Script_ResetHandMotion(CHARA_IWAO, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_IWAO, 1, data_DD88, 1);
        Script_handSetFloatsFor(CHARA_IWAO, 0, data_DD88, 1);
        script_6F0F(CHARA_IWAO);
        break;
      case 117:
        Script_SetOsageFlags(1, 0.9f);
        break;
      case 120:
        Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
        Script_ResetHandMotion(CHARA_IWAO, 0, 0, 0);
        break;
      case 177:
        script_6EE1(CHARA_IWAO);
        break;
      case 178:
        Script_SetOsageFlags(1, 1.0f);
        break;
      case 300:
        script_6F0F(CHARA_AKIR);
        break;
      case 325:
        Script_scenCreateTask(script_0207, 3, CHARA_SORY, 2, 2);
        break;
      case 330:
        Script_handSetFloatsFor(CHARA_SORY, 1, data_ECF0, 8);
        Script_handSetFloatsFor(CHARA_SORY, 0, data_ECF0, 8);
        break;
      case 337:
        Script_osagUtil(CHARA_SORY, 0x3FB33333);
        break;
      case 339:
        Script_osagUtil(CHARA_SORY, 0x3DCCCCCD);
        break;
      case 340:
        local_E4.x = -2366;
        (&local_E4)->y = -1456;
        (&local_E4)->z = -364;
        Script_SetHandRootRotation(CHARA_SORY, 1, 0x15, &local_E4);
        local_D8.x = -182;
        (&local_D8)->y = -2038;
        (&local_D8)->z = 4915;
        Script_SetHandRootRotation(CHARA_SORY, 0, 0x15, &local_D8);
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookReset);
        break;
      case 342:
        Script_scenCreateTask(script_0241, 2, CHARA_SORY, 2);
        break;
      case 343:
        Script_scenCreateTask(script_0207, 3, CHARA_IWAO, 2, 2);
        break;
      case 345:
        Script_osagUtil(CHARA_SORY, 0x3C23D70A);
        break;
      case 361:
        Script_scenCreateTask(script_0241, 2, CHARA_IWAO, 2);
        break;
      case 367:
        Script_ResetHandMotion(CHARA_IWAO, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_IWAO, 0, data_EDD4, 1);
        break;
      case 390:
        Script_osagUtil(CHARA_SORY, 0x40400000);
        break;
      case 465:
        Script_FACE(CHARA_AKIR, 4, 1, 3);
        break;
      case 559:
        Script_InitializeOsag(CHARA_SORY);
        break;
    }
    if ((local_F0 > 224) & (local_F0 < 340)) {
      Script_EVT_GetPosition(CHARA_IWAO, 5, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 2);
    } else {
      if ((local_F0 > 344) & (local_F0 < 352)) {
        local_D4 = local_D4 - 0.05f;
        Script_SetOsageFlags(0, local_D4);
        goto L0052C3;
      }
    }
    L0052C3:;
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_52D8(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    if (local_E0 == 0) {
      Script_osagUtil(CHARA_SORY, 0x3DCCCCCD);
      Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
      Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
      Script_handSetFloatsFor(CHARA_SORY, 1, data_DD88, 1);
      Script_handSetFloatsFor(CHARA_SORY, 0, data_ECF0, 1);
      local_E4.x = 0;
      (&local_E4)->y = 0;
      (&local_E4)->z = 0;
      Script_SetHandRootRotation(CHARA_SORY, 1, 0x15, &local_E4);
      Script_SetHandRootRotation(CHARA_SORY, 0, 0x15, &local_E4);
      goto L0053AA;
    }
    L0053AA:;
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_53BF(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  {
    Vector3i local_E4;
    local_E4.x = 0;
    (&local_E4)->y = 14563;
    (&local_E4)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('ODR2'), 0x38000000, &local_E4, 0);
  }
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    Vector3i local_E4;
    int32_t local_E0;
    local_E0 = local_F0;
    switch (local_E0) {
      case 0:
        local_F4.x = 0.145f;
        (&local_F4)->y = 0.071f;
        (&local_F4)->z = -0.051f;
        local_E4.x = 55341;
        (&local_E4)->y = 5279;
        (&local_E4)->z = 6007;
        Script_fixoAttachEx(SHEN_NAME('RYUK'), CHARA_KURA, 18, &local_F4, &local_E4);
        Script_osagUtil(CHARA_SORY, 0x3F800000);
        break;
      case 39:
        Script_fixoClearAttachment(SHEN_NAME('RYUK'));
        break;
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_54BD(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    if (local_EC == 0) goto L005504;
    L005504:;
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_5519(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Vector3i local_E4;
  Vector4 local_D4;
  int32_t local_D0;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_D0 = local_F0;
    switch (local_D0) {
      case 0:
        Script_SetOsageFlags(0, 1.0f);
        Script_SetOsageFlags(10, 0x3C23D70A);
        Script_SetOsageFlags(20, 0x3C23D70A);
        Script_SetOsageFlags(30, 0x3C23D70A);
        Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SORY, 0, data_DD88, 1);
        local_F4.x = 0.164f;
        (&local_F4)->y = 0.073f;
        (&local_F4)->z = 0.021f;
        local_E4.x = 0x2000;
        (&local_E4)->y = 1820;
        (&local_E4)->z = 6189;
        Script_fixoAttachEx(SHEN_NAME('RYUK'), CHARA_SORY, 12, &local_F4, &local_E4);
        break;
      case 123:
        local_D4.x = 1.0f;
        (&local_D4)->y = 0.7f;
        (&local_D4)->z = 0.7f;
        (&local_D4)->w = 0.7f;
        Script_SetCharaRefBoardPos(SHEN_NAME('RYUK'), &local_D4);
        break;
      case 317:
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookReset);
        break;
      case 330:
        Script_SetOsageFlags(0, 0.23f);
        Script_SetOsageFlags(10, 0x4185999A);
        Script_SetOsageFlags(20, -0x3F69999A);
        Script_SetOsageFlags(30, 0x41E80000);
        break;
      case 444:
        Script_fixoClearAttachment(SHEN_NAME('RYUK'));
        break;
    }
    if (local_F0 < 60) {
      Script_EVT_GetPosition(CHARA_KURA, 5, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 3);
    } else {
      if ((local_F0 > 60) & (local_F0 < 317)) {
        Script_EVT_GetPosition(SHEN_NAME('RYUK'), -1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_SORY, FACE_LookAtPosition, &local_F4, 3);
        goto L005711;
      }
    }
    L005711:;
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_5726(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    switch (local_E0) {
      case 0:
        Script_FACE(CHARA_AKIR, 6, 6, 1);
        Script_FACE(CHARA_IWAO, 10, 6, 1);
        Script_FACE_LCG(CHARA_IWAO, FACE_BlinkClose, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_DD88, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_E1FC, 1);
        local_E4.x = 0;
        (&local_E4)->y = 0;
        (&local_E4)->z = 0;
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E4);
        break;
      case 11:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_E1FC, 10);
        break;
      case 53:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_DD88, 10);
        break;
      case 62:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_E1FC, 10);
        break;
      case 80:
        Script_FACE(CHARA_AKIR, 4, 1, 10);
        break;
      case 90:
        Script_FACE(CHARA_AKIR, 6, 6, 3);
        break;
      case 116:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_DD88, 15);
        break;
      case 120:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_E1FC, 10);
        break;
      case 168:
        Script_FACE(CHARA_AKIR, 4, 1, 3);
        break;
      case 172:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_DD88, 10);
        break;
      case 175:
        Script_FACE(CHARA_AKIR, 6, 6, 3);
        break;
      case 177:
        local_E4.x = 0;
        (&local_E4)->y = 0;
        (&local_E4)->z = 0;
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E4);
        break;
      case 192:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_E1FC, 8);
        break;
      case 259:
        Script_FACE(CHARA_AKIR, 4, 4, 3);
        break;
      case 265:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_DD88, 10);
        break;
      case 298:
        Script_FACE(CHARA_AKIR, 6, 1, 3);
        break;
      case 385:
        Script_FACE(CHARA_AKIR, 4, 2, 8);
        break;
      case 380:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_E1FC, 7);
        break;
      case 410:
        Script_FACE(CHARA_AKIR, 6, 6, 3);
        break;
      case 460:
        local_E4.x = 0;
        (&local_E4)->y = 0;
        (&local_E4)->z = 0;
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E4);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_DD88, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_DD88, 1);
        break;
      case 554:
        script_6EE1(CHARA_AKIR);
        break;
      case 555:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_E1FC, 10);
        break;
      case 604:
        Script_scenCreateTask(script_036D, 2, CHARA_AKIR, 2);
        break;
      case 630:
        Script_FACE_LCG(CHARA_IWAO, FACE_BlinkOpen, 15, 0);
        break;
      case 664:
        script_6F0F(CHARA_AKIR);
        script_6F0F(CHARA_IWAO);
        break;
      case 936:
        Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookReset);
        break;
      case 959:
        script_6EE1(CHARA_AKIR);
        script_6EE1(CHARA_IWAO);
        break;
      case 1207:
        Script_FACE_LCG(CHARA_IWAO, FACE_BlinkClose, 32, 0);
        break;
      case 0x500:
        Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 1, 0);
        break;
      case 1293:
        Script_FACE(CHARA_AKIR, 7, 4, 6);
        break;
      case 1296:
        Script_FACE(CHARA_AKIR, 6, 0, 5);
        break;
      case 1348:
        Script_EVT_GetPosition(CHARA_IWAO, 2, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 6);
        break;
      case 1354:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        break;
      case 1365:
        script_6F0F(CHARA_AKIR);
        break;
      case 1400:
        Script_EVT_GetPosition(CHARA_IWAO, 27, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 3);
        break;
      case 1410:
        Script_EVT_GetPosition(CHARA_IWAO, 5, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 3);
        break;
      case 1415:
        Script_EVT_GetPosition(CHARA_IWAO, 20, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 3);
        break;
      case 1422:
        Script_EVT_GetPosition(CHARA_IWAO, 5, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 3);
        break;
      case 1442:
        Script_EVT_GetPosition(CHARA_IWAO, 20, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 3);
        break;
      case 1462:
        Script_EVT_GetPosition(CHARA_IWAO, 5, &local_F4, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookAtPosition, &local_F4, 3);
        break;
      case 1480:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, FACE_LookReset);
        break;
      case 1506:
        Script_PREP_Related(-1, 0);
        Script_lightSetSlotEnabled(1, 0);
        Script_lightSetSlotEnabled(2, 0);
        Script_lightSetSlotEnabled(3, 0);
        Script_lightSetSlotEnabled(4, 0);
        Script_lightSetSlotEnabled(5, 0);
        Script_lightSetSlotEnabled(6, 1);
        Script_scenCreateTask(script_6B01, 2, 0, 60);
        break;
      case 1507:
        Script_FACE(CHARA_AKIR, 11, 2, 5);
        break;
      case 1508:
        Script_lightSetSlotEnabled(1, 1);
        Script_lightSetSlotEnabled(2, 1);
        Script_lightSetSlotEnabled(3, 1);
        Script_lightSetSlotEnabled(4, 1);
        Script_lightSetSlotEnabled(5, 1);
        Script_lightSetSlotEnabled(6, 0);
        break;
      case 1509:
        Script_lightSetSlotEnabled(1, 0);
        Script_lightSetSlotEnabled(2, 0);
        Script_lightSetSlotEnabled(3, 0);
        Script_lightSetSlotEnabled(4, 0);
        Script_lightSetSlotEnabled(5, 0);
        Script_lightSetSlotEnabled(6, 1);
        break;
      case 1513:
        Script_lightSetSlotEnabled(1, 1);
        Script_lightSetSlotEnabled(2, 1);
        Script_lightSetSlotEnabled(3, 1);
        Script_lightSetSlotEnabled(4, 1);
        Script_lightSetSlotEnabled(5, 1);
        Script_lightSetSlotEnabled(6, 0);
        break;
      case 1511:
        Script_FACE_LCG(CHARA_AKIR, FACE_BlinkClose, 3, 0);
        break;
      case 1626:
        Script_lightSetSlotEnabled(1, 0);
        Script_lightSetSlotEnabled(2, 0);
        Script_lightSetSlotEnabled(3, 0);
        Script_lightSetSlotEnabled(4, 0);
        Script_lightSetSlotEnabled(5, 0);
        Script_lightSetSlotEnabled(6, 1);
        Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 164, 255, 255, 255);
        break;
      case 1628:
        Script_lightSetSlotEnabled(1, 1);
        Script_lightSetSlotEnabled(2, 1);
        Script_lightSetSlotEnabled(3, 1);
        Script_lightSetSlotEnabled(4, 1);
        Script_lightSetSlotEnabled(5, 1);
        Script_lightSetSlotEnabled(6, 0);
        Script_filmCheckOrSetTransitionUtil(0, 164, 255, 255, 255, 0, 0, 0, 0);
        break;
      case 1629:
        Script_lightSetSlotEnabled(1, 0);
        Script_lightSetSlotEnabled(2, 0);
        Script_lightSetSlotEnabled(3, 0);
        Script_lightSetSlotEnabled(4, 0);
        Script_lightSetSlotEnabled(5, 0);
        Script_lightSetSlotEnabled(6, 1);
        Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 128, 255, 255, 255);
        break;
      case 1630:
        Script_lightSetSlotEnabled(1, 1);
        Script_lightSetSlotEnabled(2, 1);
        Script_lightSetSlotEnabled(3, 1);
        Script_lightSetSlotEnabled(4, 1);
        Script_lightSetSlotEnabled(5, 1);
        Script_lightSetSlotEnabled(6, 0);
        Script_filmCheckOrSetTransitionUtil(0, 128, 255, 255, 255, 0, 0, 0, 0);
        break;
    }
    if ((local_F0 > 745) & (local_F0 < 936)) {
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, 0);
      local_F4.x = (&local_F4)->x + 0.13f;
      Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookAtPosition, &local_F4, 20);
    }
    if ((local_F0 > 988) & (local_F0 < 1120)) {
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, 0);
      local_F4.x = (&local_F4)->x + 0.13f;
      Script_FaceLookAtTargetUtils(CHARA_IWAO, FACE_LookAtPosition, &local_F4, 50);
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_608A(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  float local_E0;
  int32_t local_DC;
  int8_t local_DB;
  int32_t local_D4;
  local_DB = Script_ExecuteAicaCommand(1, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 140.0f);
  script_6AA5(1, 128, 1);
  Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 128, 255, 255, 255);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 1);
  local_E0 = 0.1f;
  task_30 = 4;
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 1);
  task_8 = Script_WeatherUtils_ForFIXC(1, 0x4000);
  Script_WeatherUtils(WeatherScript_ProcessWeather, 5.0f);
  Script_WeatherUtils(WeatherScript_SetForcedRainValue, local_E0);
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Template, 0.0f, -0.15f, 0.0f, 0.01f, 0.01f, 0.01f);
  Script_WeatherTaskUtil(StorePreviousOperand, 0);
  Script_WeatherUtils(WeatherScript_SetRainEffectOffset, 0x20FFFFFF);
  Script_mtwkGetSetRotation(SHEN_NAME('THN2'), &local_E4, 0);
  Script_EVT_SetRotation(SHEN_NAME('THN5'), 0x38000000, &local_E4, 0);
  local_DC = 30;
  Script_executeAicaCommand(166, local_DB, local_DC);
  script_6F0F(CHARA_AKIR);
  script_6F0F(CHARA_SORY);
  script_6F0F(CHARA_IWAO);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_D4 = local_FC;
    switch (local_D4) {
      case 0:
        Script_scenCreateTask(script_6B01, 2, 0, 5);
        break;
      case 80:
        Script_executeAicaCommand(0x3804A9, 0, 0);
        break;
      case 138:
        script_6A54(1, 128, 1);
        Script_charaDisplay(SHEN_NAME('THN3'), CDM_Show);
        local_F0.x = 2.6f;
        (&local_F0)->y = 5.2f;
        (&local_F0)->z = 2.6f;
        Script_hmdlApplyScale(SHEN_NAME('THN3'), &local_F0);
        Script_scenCreateTask(script_6B01, 2, 1, 6);
        Script_scenCreateTask(script_6B5D, 3, 6, 0x3EA3D70A, 0x3CA3D70A);
        Script_scenCreateTask(script_6A37, 0);
        break;
      case 140:
        Script_charaDisplay(SHEN_NAME('THN3'), CDM_Hide);
        break;
      case 142:
        Script_charaDisplay(SHEN_NAME('THN1'), CDM_Show);
        local_F0.x = 2.6f;
        (&local_F0)->y = 4.7f;
        (&local_F0)->z = 2.6f;
        Script_hmdlApplyScale(SHEN_NAME('THN1'), &local_F0);
        Script_scenCreateTask(script_6B01, 2, 2, 6);
        break;
      case 144:
        script_6AA5(1, 128, 0);
        Script_charaDisplay(SHEN_NAME('THN1'), CDM_Hide);
        break;
      case 146:
        script_6A54(1, 128, 1);
        Script_charaDisplay(SHEN_NAME('THN4'), CDM_Show);
        local_F0.x = 2.6f;
        (&local_F0)->y = 3.9f;
        (&local_F0)->z = 2.6f;
        Script_hmdlApplyScale(SHEN_NAME('THN4'), &local_F0);
        Script_scenCreateTask(script_6B01, 2, 0, 6);
        Script_scenCreateTask(script_6B5D, 3, 6, 0x3EA3D70A, 0x3CA3D70A);
        Script_scenCreateTask(script_6A37, 0);
        break;
      case 148:
        script_6AA5(1, 128, 0);
        Script_charaDisplay(SHEN_NAME('THN4'), CDM_Hide);
        break;
      case 228:
        Script_executeAicaCommand(0x3804A9, 0, 0);
        break;
      case 292:
        script_6A54(1, 128, 1);
        Script_charaDisplay(SHEN_NAME('THN3'), CDM_Show);
        local_F0.x = 2.6f;
        (&local_F0)->y = 4.3f;
        (&local_F0)->z = 2.6f;
        Script_hmdlApplyScale(SHEN_NAME('THN3'), &local_F0);
        Script_scenCreateTask(script_6B01, 2, 1, 6);
        Script_scenCreateTask(script_6B5D, 3, 6, 0x3EF5C28F, 0x3CF5C28F);
        Script_scenCreateTask(script_6A37, 0);
        break;
      case 294:
        Script_charaDisplay(SHEN_NAME('THN3'), CDM_Hide);
        break;
      case 300:
        Script_charaDisplay(SHEN_NAME('THN4'), CDM_Show);
        local_F0.x = 2.6f;
        (&local_F0)->y = 4.5f;
        (&local_F0)->z = 2.6f;
        Script_hmdlApplyScale(SHEN_NAME('THN4'), &local_F0);
        Script_scenCreateTask(script_6B01, 2, 2, 6);
        Script_scenCreateTask(script_6B5D, 3, 6, 0x3EF5C28F, 0x3CF5C28F);
        break;
      case 302:
        script_6AA5(1, 128, 0);
        Script_charaDisplay(SHEN_NAME('THN4'), CDM_Hide);
        break;
      case 308:
        script_6A54(1, 128, 1);
        Script_charaDisplay(SHEN_NAME('THN1'), CDM_Show);
        local_F0.x = 3.6f;
        (&local_F0)->y = 5.6f;
        (&local_F0)->z = 3.6f;
        Script_hmdlApplyScale(SHEN_NAME('THN1'), &local_F0);
        Script_scenCreateTask(script_6B01, 2, 0, 6);
        Script_scenCreateTask(script_6A37, 0);
        break;
      case 312:
        Script_charaDisplay(SHEN_NAME('THN1'), CDM_Hide);
        break;
      case 317:
        Script_charaDisplay(SHEN_NAME('THN1'), CDM_Show);
        local_F0.x = 1.4f;
        (&local_F0)->y = 4.1f;
        (&local_F0)->z = 2.0f;
        Script_hmdlApplyScale(SHEN_NAME('THN1'), &local_F0);
        Script_scenCreateTask(script_6B01, 2, 1, 6);
        Script_scenCreateTask(script_6B5D, 3, 6, 0x3F23D70A, 0x3D23D70A);
        break;
      case 320:
        script_6AA5(1, 128, 0);
        Script_charaDisplay(SHEN_NAME('THN1'), CDM_Hide);
        break;
      case 400:
        Script_executeAicaCommand(0x3804A9, 0, 0);
        break;
      case 450:
        Script_executeAicaCommand(0x3804A9, 0, 0);
        break;
      case 470:
        task_2F = 0;
        break;
      case 498:
        Script_executeAicaCommand(166, local_DB, 0);
        Script_ExecuteAicaCommand(0, local_DB);
        break;
    }
    if ((local_FC > 150) & (local_FC < 230)) {
      local_DC = local_DC + 1;
      Script_executeAicaCommand(166, local_DB, local_DC);
    }
    if (-(local_FC > 150) & -(local_FC < 400) & -((local_FC & 3) == 0)) {
      local_E0 = local_E0 + 0.0032f;
      Script_WeatherUtils(WeatherScript_SetForcedRainValue, local_E0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_66FC(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t unused_D8[3];
  if (task_4 != 2) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_EF9C);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_EFAC);
    local_E4.x = -0.85f;
    (&local_E4)->y = -0.51f;
    (&local_E4)->z = -0.13f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.59f;
    (&local_F0)->z = 0.56f;
    (&local_F0)->w = 0.7f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, 1.0f);
    Script_lightSetSlotEnabled(5, 1);
    Script_LightUtils(5, 11, 1);
    local_E4.x = -3.14f;
    (&local_E4)->y = 2.33f;
    (&local_E4)->z = -17.03f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 4.0f);
    Script_LightUtils(1, 5, 3.0f);
    Script_LightUtils(1, 6, data_EFBC);
    Script_lightSetSlotEnabled(1, 1);
    Script_LightUtils(1, 11, 1);
    local_E4.x = -10.54f;
    (&local_E4)->y = 2.32f;
    (&local_E4)->z = -17.09f;
    Script_lightEnablePointLightAt(2, &local_E4);
    Script_LightUtils(2, 2, &local_F0);
    Script_LightUtils(2, 4, 4.0f);
    Script_LightUtils(2, 5, 3.0f);
    Script_LightUtils(2, 6, data_EFBC);
    Script_lightSetSlotEnabled(2, 1);
    Script_LightUtils(2, 11, 1);
    local_E4.x = -10.57f;
    (&local_E4)->y = 2.32f;
    (&local_E4)->z = -28.07f;
    Script_lightEnablePointLightAt(3, &local_E4);
    Script_LightUtils(3, 2, &local_F0);
    Script_LightUtils(3, 4, 4.0f);
    Script_LightUtils(3, 5, 3.0f);
    Script_LightUtils(3, 6, data_EFBC);
    Script_lightSetSlotEnabled(3, 1);
    Script_LightUtils(3, 11, 1);
    local_E4.x = -3.15f;
    (&local_E4)->y = 2.34f;
    (&local_E4)->z = -28.07f;
    Script_lightEnablePointLightAt(4, &local_E4);
    Script_LightUtils(4, 2, &local_F0);
    Script_LightUtils(4, 4, 4.0f);
    Script_LightUtils(4, 5, 3.0f);
    Script_LightUtils(4, 6, data_EFBC);
    Script_lightSetSlotEnabled(4, 1);
    Script_LightUtils(4, 11, 1);
    local_E4.x = -1.0f;
    (&local_E4)->y = -0.4f;
    (&local_E4)->z = -0.13f;
    Script_lightDirLightEnable(6, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 1.0f;
    Script_LightUtils(6, 2, &local_F0);
    Script_LightUtils(6, 4, 9.9f);
    Script_lightSetSlotEnabled(6, 0);
    script_6EE1(CHARA_AKIR);
    script_6EE1(CHARA_SORY);
    script_6EE1(CHARA_IWAO);
    SCNF_WaitFrames_0000(1);
    Script_PREP_Related(-1, 1);
    task_4 = 2;
  }
  end:;
}

static int32_t script_6A37(void) {
  Script_SetFogColour(&data_EFCC);
  SCNF_WaitFrames_0000(2);
  Script_SetFogColour(&data_EFDC);
  end:;
}

static void script_6A54(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_PREP_Related(-1, 0);
  task_4 = 3;
  Script_shenSetLightingMode(1);
  Script_lightSetGlobalSceneLighting(&data_EFEC);
  Script_LightUtils(0, 2, &data_EFFC);
  Script_LightUtils(0, 8, &data_F00C);
  Script_LightUtils(0, 4, 8.7f);
  Script_lightSetSlotEnabled(0, 1);
  return;
}

static void script_6AA5(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_SetFogFlags(0, data_F018);
  Script_PREP_Related(-1, 0);
  task_4 = 4;
  Script_shenSetLightingMode(1);
  Script_lightSetGlobalSceneLighting(&data_F028);
  Script_LightUtils(0, 2, &data_F038);
  Script_LightUtils(0, 8, &data_F048);
  Script_LightUtils(0, 4, 1.0f);
  Script_lightSetSlotEnabled(0, 1);
  return;
}

static int32_t script_6B01(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  SCNF_WaitFrames_0000(arg1);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_executeAicaCommand(0x2704A9, 0, 0);
      break;
    case 1:
      Script_executeAicaCommand(0x2804A9, 0, 0);
      break;
    case 2:
      Script_executeAicaCommand(0x3704A9, 0, 0);
      break;
  }
  end:;
}

static int32_t script_6B5D(int32_t arg0, int32_t arg1, int32_t arg2) {
  float local_FC;
  float local_F8;
  SCNF_WaitFrames_0000(arg0);
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.3f) - 0.15f;
  local_F8 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.3f) - 0.15f;
  if (!(local_FC >= 0.0f)) {
    local_FC = local_FC + 0.16f;
  } else {
    local_FC = local_FC - 0.16f;
  }
  if (!(local_F8 >= 0.0f)) {
    local_F8 = local_F8 + 0.16f;
  } else {
    local_F8 = local_F8 - 0.16f;
  }
  Script_SetCamShake(local_FC, local_F8, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.2f) + 0.35f);
  end:;
}

static int32_t script_6BF7(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  while (1) {
    Script_mtwkGetSetRotation(SHEN_NAME('ODR1'), &local_E8, 0);
    Script_mtwkGetSetRotation(SHEN_NAME('ODR2'), &local_F4, 0);
    local_D0.x = 1.0f;
    (&local_D0)->y = 1.0f;
    (&local_D0)->z = 1.0f;
    if (!((float)(&local_F4)->y * 45.0f / 8192.0f <= 90.0f) & !((float)(&local_F4)->y * 45.0f / 8192.0f >= 270.0f)) {
      (&local_F4)->y = 0x4000;
    }
    if (!((float)(&local_E8)->y * 45.0f / 8192.0f <= 90.0f) & !((float)(&local_E8)->y * 45.0f / 8192.0f >= 270.0f)) {
      (&local_E8)->y = -0x4000;
    }
    {
      float local_CC;
      float local_C8;
      local_CC = (1.0f - Script_MathUtils(SCRIPT_MATH_ABS, Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_F4)->y))) * 0.5f;
      local_C8 = (1.0f - Script_MathUtils(SCRIPT_MATH_ABS, Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y))) * 0.5f;
      (&local_D0)->z = (local_CC + local_C8) * 1.5f;
    }
    local_DC.x = -2.308f;
    (&local_DC)->y = 1.07f;
    (&local_DC)->z = -22.46f;
    {
      float local_CC;
      float local_C8;
      float local_C4;
      local_CC = Script_MathUtils(SCRIPT_MATH_ABS, Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_F4)->y));
      local_C8 = 1.0f - Script_MathUtils(SCRIPT_MATH_ABS, Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_F4)->y));
      local_C4 = 1.0f - Script_MathUtils(SCRIPT_MATH_ABS, Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y));
      (&local_DC)->z = local_CC + (local_C8 + local_C4) / 2.0f + -23.46f;
    }
    Script_EVT_SetPosition(SHEN_NAME('HIK_'), POS_XYZ, &local_DC);
    Script_hmdlApplyScale(SHEN_NAME('HIK_'), &local_D0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_6DCD(void) {
  int32_t local_F0[4];
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int32_t local_CC;
  int32_t local_C8;
  local_EC = 0;
  local_E8 = 7360;
  while (local_EC < local_E8) {
    local_E4 = local_EC * -79 / local_E8 + 255;
    local_E0 = local_EC * -32 / local_E8 + 32;
    local_DC = local_EC * -38 / local_E8 + 38;
    local_D8 = local_EC * -34 / local_E8 + 34;
    local_D4 = local_EC * 88 / local_E8 + 167;
    local_D0 = local_EC * -120 / local_E8 + 120;
    local_CC = local_EC * -124 / local_E8 + 124;
    local_C8 = local_EC * -116 / local_E8 + 116;
    local_F0[0] = local_E4 * 0x1000000 + local_E0 * 0x10000 + local_DC * 0x100 + local_D8;
    local_F0[1] = *(int32_t *)local_F0;
    local_F0[2] = local_D4 * 0x1000000 + local_D0 * 0x10000 + local_CC * 0x100 + local_C8;
    local_F0[3] = local_F0[2];
    Script_SetFogFlags(0, (const uint32_t *)local_F0);
    local_EC = local_EC + 1;
    SCNF_WaitFrames_0000(1);
  }
  task_3C = 0;
  return Script_srchControl(0);
}

static void script_6EE1(int32_t arg0) {
  Script_SetCharaRefBoardPos(arg0, &data_F054);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetRefBoardValues(arg0, 2, 0);
  Script_SetRefBoardValues(arg0, 5, 0);
  return;
}

static void script_6F0F(int32_t arg0) {
  Script_SetCharaRefBoardPos(arg0, &data_F064);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetRefBoardValues(arg0, 0, 0);
  Script_SetRefBoardValues(arg0, 3, 0);
  return;
}

static int32_t script_6F3D(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    switch (local_E0) {
      case 0:
        Script_ResetHandMotion(CHARA_INE_, 1, 1, 0);
        Script_ResetHandMotion(CHARA_INE_, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_INE_, 1, data_DD88, 1);
        Script_handSetFloatsFor(CHARA_INE_, 0, data_DD88, 1);
        Script_FACE(CHARA_INE_, 1, 2, 1);
        break;
      case 398:
        Script_FACE(CHARA_INE_, 0, 0, 10);
        break;
      case 447:
        Script_FACE(CHARA_INE_, 7, 1, 7);
        break;
      case 449:
        Script_FACE(CHARA_INE_, 2, 0, 12);
        break;
      case 566:
        local_F4.x = 0.111f;
        (&local_F4)->y = 0.115f;
        (&local_F4)->z = 0.022f;
        local_E4.x = 182;
        (&local_E4)->y = 60802;
        (&local_E4)->z = 64625;
        Script_fixoAttachEx(SHEN_NAME('FUTO'), CHARA_INE_, 12, &local_F4, &local_E4);
        Script_handSetFloatsFor(CHARA_INE_, 0, data_F074, 35);
        local_E4.x = 0;
        (&local_E4)->y = 0;
        (&local_E4)->z = 5097;
        Script_SetHandRootRotation(CHARA_INE_, 0, 0x15, &local_E4);
        break;
      case 625:
        Script_handSetFloatsFor(CHARA_INE_, 1, data_D830, 500);
        break;
      case 670:
        task_2F = 0;
        break;
      case 700:
        Script_fixoClearAttachment(SHEN_NAME('FUTO'));
        break;
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_712B(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t unused_E4[3];
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetCamFlags(0, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    switch (local_E0) {
      case 0:
        task_2F = 0;
        break;
      case 60:
        task_2F = 1;
        break;
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_71AA(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t unused_E4[3];
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetCamFlags(0, 1);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    if (local_E0 == 0) {
      Script_clmdSetFlags(CHARA_KWMT, 2, 1);
      local_F4.x = 10000.0f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 0.0f;
      Script_EVT_SetPosition(SHEN_NAME('BIKE'), POS_XYZ, &local_F4);
      Script_SetShadowFlags(CHARA_KWMT, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
      goto L007240;
    }
    L007240:;
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_7255(void) {
  if (task_4 != 22) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F158);
    Script_SetFogFlags(0, data_F168);
    Script_shenSetLightingMode(1);
    Script_lightDirLightEnable(0, &data_F198);
    Script_lightSetGlobalSceneLighting(&data_F178);
    Script_LightUtils(0, 2, &data_F188);
    Script_LightUtils(0, 4, 1.2f);
    Script_lightSetSlotEnabled(0, 1);
    task_4 = 22;
  }
  return;
}

static int32_t script_72C2(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t unused_E4[3];
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    switch (local_E0) {
      case 0:
        task_2F = 0;
        Script_FACE_LCG(SHEN_NAME('AKI_'), FACE_BlinkClose, 1, 0);
        Script_ResetHandMotion(SHEN_NAME('AKI_'), 1, 1, 0);
        Script_ResetHandMotion(SHEN_NAME('AKI_'), 1, 0, 0);
        Script_handSetFloatsFor(SHEN_NAME('AKI_'), 1, data_DD88, 1);
        Script_handSetFloatsFor(SHEN_NAME('AKI_'), 0, data_DD88, 1);
        Script_FACE(SHEN_NAME('AKI_'), 4, 1, 1);
        break;
      case 51:
        Script_FACE(SHEN_NAME('AKI_'), 10, 0, 10);
        break;
      case 60:
        task_2F = 1;
        break;
      case 71:
        Script_FACE(SHEN_NAME('AKI_'), 4, 1, 15);
        break;
      case 86:
        Script_FACE(SHEN_NAME('AKI_'), 10, 3, 15);
        break;
      case 95:
        Script_FACE(SHEN_NAME('AKI_'), 4, 1, 15);
        break;
      case 125:
        Script_FACE(SHEN_NAME('AKI_'), 10, 3, 15);
        break;
      case 160:
        Script_FACE(SHEN_NAME('AKI_'), 10, 1, 15);
        break;
      case 210:
        Script_FACE(SHEN_NAME('AKI_'), 4, 1, 15);
        break;
      case 230:
        Script_FACE(SHEN_NAME('AKI_'), 10, 1, 15);
        break;
      case 268:
        task_2F = 0;
        Script_filmCheckOrSetTransitionUtil(27, 0, 255, 255, 255, 255, 255, 255, 255);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_74DF(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t unused_E4[3];
  int32_t local_E0;
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 0);
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_F0;
    switch (local_E0) {
      case 0:
        Script_FACE_LCG(SHEN_NAME('AKI_'), FACE_BlinkClose, 1, 0);
        Script_ResetHandMotion(SHEN_NAME('AKI_'), 1, 1, 0);
        Script_ResetHandMotion(SHEN_NAME('AKI_'), 1, 0, 0);
        Script_handSetFloatsFor(SHEN_NAME('AKI_'), 1, data_DD88, 1);
        Script_handSetFloatsFor(SHEN_NAME('AKI_'), 0, data_DD88, 1);
        Script_FACE(SHEN_NAME('AKI_'), 10, 0, 1);
        break;
      case 5:
        task_2F = 1;
        break;
      case 10:
        Script_FACE(SHEN_NAME('AKI_'), 7, 2, 20);
        break;
      case 21:
        Script_FACE_LCG(SHEN_NAME('AKI_'), FACE_BlinkOpen, 6, 0);
        Script_FACE(SHEN_NAME('AKI_'), 7, 0, 2);
        break;
      case 33:
        Script_FACE(SHEN_NAME('AKI_'), 8, 2, 15);
        break;
      case 43:
        Script_FACE(SHEN_NAME('AKI_'), 3, 0, 15);
        break;
      case 55:
        Script_FACE(SHEN_NAME('AKI_'), 4, 0, 10);
        break;
      case 83:
        Script_FACE(SHEN_NAME('AKI_'), 10, 0, 20);
        break;
      case 120:
        Script_FACE(SHEN_NAME('AKI_'), 4, 0, 10);
        break;
      case 126:
        Script_FACE(SHEN_NAME('AKI_'), 3, 0, 10);
        break;
      case 139:
        Script_FACE(SHEN_NAME('AKI_'), 4, 0, 20);
        break;
      case 145:
        Script_FACE(SHEN_NAME('AKI_'), 10, 0, 10);
        break;
      case 155:
        Script_FACE(SHEN_NAME('AKI_'), 3, 0, 16);
        break;
      case 175:
        Script_FACE(SHEN_NAME('AKI_'), 4, 0, 60);
        break;
      case 197:
        Script_FACE(SHEN_NAME('AKI_'), 4, 0, 20);
        break;
      case 204:
        Script_scenCreateTask(script_036D, 2, SHEN_NAME('AKI_'), 2);
        break;
      case 207:
        Script_scenCreateTask(script_036D, 2, SHEN_NAME('AKI_'), 2);
        break;
      case 215:
        Script_FACE(SHEN_NAME('AKI_'), 10, 1, 80);
        break;
      case 255:
        Script_FACE(SHEN_NAME('AKI_'), 10, 0, 38);
        break;
      case 275:
        Script_FACE(SHEN_NAME('AKI_'), 10, 1, 80);
        break;
      case 290:
        task_2F = 0;
        Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 315:
        Script_FACE(SHEN_NAME('AKI_'), 10, 0, 10);
        break;
      case 325:
        Script_FACE(SHEN_NAME('AKI_'), 0, 0, 60);
        break;
      case 345:
        Script_FACE(SHEN_NAME('AKI_'), 10, 1, 80);
        break;
      case 385:
        Script_FACE(SHEN_NAME('AKI_'), 10, 0, 38);
        break;
      case 405:
        Script_FACE(SHEN_NAME('AKI_'), 10, 1, 80);
        break;
      case 445:
        Script_FACE(SHEN_NAME('AKI_'), 10, 0, 38);
        break;
      case 465:
        Script_FACE(SHEN_NAME('AKI_'), 10, 1, 80);
        break;
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_78ED(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    if (local_EC == 5) {
      task_2F = 1;
      goto L007917;
    }
    L007917:;
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_792C(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    if (local_EC == 46) {
      script_8D66();
      goto L007954;
    }
    L007954:;
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_7969(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_F0;
    switch (local_EC) {
      case 18:
        script_89E9();
        break;
      case 165:
        Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
        Script_SetNewHandMotion(CHARA_SORY, 1, 9504, 0, 0, 0, 0, 1.0f, 0);
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 0, 0, 1);
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 1, 20.0f);
        Script_ConfigHandMotionIK(CHARA_SORY, 1, 0, 0, 4);
        break;
    }
    local_F0 = local_F0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_7A18(void) {
  if (task_4 != 20) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F1A4);
    Script_SetFogFlags(0, data_F1B4);
    Script_shenSetLightingMode(1);
    Script_lightSetGlobalSceneLighting(&data_F1C4);
    Script_lightDirLightEnable(0, &data_F1E4);
    Script_LightUtils(0, 2, &data_F1D4);
    Script_LightUtils(0, 4, 0.8f);
    Script_lightSetSlotEnabled(0, 1);
    Script_SetCharaRefBoardPos(SHEN_NAME('AKI_'), &data_F1F0);
    Script_charaSetRefboardFlagTo(SHEN_NAME('AKI_'), 1);
    task_4 = 20;
  }
  return;
}

static void script_7AA0(void) {
  if (task_4 != 21) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F200);
    Script_SetFogFlags(0, data_F210);
    Script_shenSetLightingMode(1);
    Script_lightSetGlobalSceneLighting(&data_F220);
    Script_lightDirLightEnable(0, &data_F240);
    Script_LightUtils(0, 2, &data_F230);
    Script_LightUtils(0, 4, 0.8f);
    Script_lightSetSlotEnabled(0, 1);
    Script_SetCharaRefBoardPos(SHEN_NAME('AKI_'), &data_F24C);
    Script_charaSetRefboardFlagTo(SHEN_NAME('AKI_'), 1);
    task_4 = 21;
  }
  return;
}

static void script_7B28(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 6) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F25C);
    Script_SetFogFlags(0, data_F26C);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F27C);
    local_E4.x = -0.47f;
    (&local_E4)->y = -0.22f;
    (&local_E4)->z = -0.86f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -2.9f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -7.11f;
    (&local_E4)->y = 2.46f;
    (&local_E4)->z = -21.75f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 7.1f);
    Script_LightUtils(1, 5, 2.01f);
    local_DC[0] = 0x3ED70A3D;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    task_4 = 6;
  }
  return;
}

static void script_7C96(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 7) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F28C);
    Script_SetFogFlags(0, data_F29C);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F2AC);
    local_E4.x = 0.63f;
    (&local_E4)->y = 0.12f;
    (&local_E4)->z = 0.76f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -4.8f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -6.08f;
    (&local_E4)->y = 1.95f;
    (&local_E4)->z = -21.43f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 1.0f);
    Script_LightUtils(1, 5, 2.0f);
    local_DC[0] = 0x3EC7AE14;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    task_4 = 7;
  }
  return;
}

static void script_7E04(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 8) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F2BC);
    Script_SetFogFlags(0, data_F2CC);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F2DC);
    local_E4.x = -0.32f;
    (&local_E4)->y = 0.53f;
    (&local_E4)->z = 0.79f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -7.9f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -8.55f;
    (&local_E4)->y = 2.74f;
    (&local_E4)->z = -22.03f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 4.8f);
    Script_LightUtils(1, 5, 2.0f);
    local_DC[0] = 0x3EA3D70A;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    task_4 = 8;
  }
  return;
}

static void script_7F72(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 9) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F2EC);
    Script_SetFogFlags(0, data_F2FC);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F30C);
    local_E4.x = 0.54f;
    (&local_E4)->y = 0.1f;
    (&local_E4)->z = -0.84f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -10.0f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -6.09f;
    (&local_E4)->y = 2.67f;
    (&local_E4)->z = -21.77f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 1.0f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 0.8f);
    Script_LightUtils(1, 5, 1.35f);
    local_DC[0] = 0x3E3851EC;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    task_4 = 9;
  }
  return;
}

static void script_80E0(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 10) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F31C);
    Script_SetFogFlags(0, data_F32C);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F33C);
    local_E4.x = -0.22f;
    (&local_E4)->y = -0.05f;
    (&local_E4)->z = -0.97f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -10.0f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -6.42f;
    (&local_E4)->y = 2.66f;
    (&local_E4)->z = -21.88f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 0.9f);
    Script_LightUtils(1, 5, 0.45f);
    local_DC[0] = 0x3F35C28F;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    local_E4.x = -5.89f;
    (&local_E4)->y = 2.28f;
    (&local_E4)->z = -21.6f;
    Script_lightEnablePointLightAt(2, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 1.0f;
    Script_LightUtils(2, 2, &local_F0);
    Script_LightUtils(2, 4, 2.3f);
    Script_LightUtils(2, 5, 0.43f);
    local_DC[0] = 0x3F266666;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(2, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(2, 1);
    task_4 = 10;
  }
  return;
}

static void script_82EF(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 11) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F34C);
    Script_SetFogFlags(0, data_F35C);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F36C);
    local_E4.x = 0.67f;
    (&local_E4)->y = 0.7f;
    (&local_E4)->z = 0.24f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -10.0f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -7.33f;
    (&local_E4)->y = 1.89f;
    (&local_E4)->z = -21.81f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 1.5f);
    Script_LightUtils(1, 5, 0.62f);
    local_DC[0] = 0x3F1EB852;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    local_E4.x = -7.63f;
    (&local_E4)->y = 1.84f;
    (&local_E4)->z = -20.94f;
    Script_lightEnablePointLightAt(2, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 1.0f;
    Script_LightUtils(2, 2, &local_F0);
    Script_LightUtils(2, 4, 2.0f);
    Script_LightUtils(2, 5, 0.08f);
    local_DC[0] = 0x3F147AE1;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(2, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(2, 1);
    task_4 = 11;
  }
  return;
}

static void script_84FE(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 12) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F37C);
    Script_SetFogFlags(0, data_F38C);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F39C);
    local_E4.x = -0.5f;
    (&local_E4)->y = 0.48f;
    (&local_E4)->z = -0.72f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -10.0f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -7.87f;
    (&local_E4)->y = 2.49f;
    (&local_E4)->z = -22.79f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 0.9f);
    Script_LightUtils(1, 5, 0.7f);
    local_DC[0] = 0x3E851EB8;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    task_4 = 12;
  }
  return;
}

static void script_866C(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 13) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F3AC);
    Script_SetFogFlags(0, data_F3BC);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F3CC);
    local_E4.x = -0.46f;
    (&local_E4)->y = -0.17f;
    (&local_E4)->z = 0.46f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -3.7f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -7.92f;
    (&local_E4)->y = 2.65f;
    (&local_E4)->z = -22.46f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 0.9f);
    Script_LightUtils(1, 5, 0.67f);
    local_DC[0] = 0x3E8F5C29;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    task_4 = 13;
  }
  return;
}

static void script_87DA(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 14) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F3DC);
    Script_SetFogFlags(0, data_F3EC);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F3FC);
    local_E4.x = -0.93f;
    (&local_E4)->y = -0.26f;
    (&local_E4)->z = 0.25f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -3.7f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -5.04f;
    (&local_E4)->y = 1.75f;
    (&local_E4)->z = -20.81f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 2.0f);
    Script_LightUtils(1, 5, 0.79f);
    local_DC[0] = 0x3EE147AE;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    local_E4.x = -4.21f;
    (&local_E4)->y = 1.09f;
    (&local_E4)->z = -21.35f;
    Script_lightEnablePointLightAt(2, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(2, 2, &local_F0);
    Script_LightUtils(2, 4, 1.4f);
    Script_LightUtils(2, 5, 0.33f);
    local_DC[0] = 0x3E947AE1;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(2, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(2, 1);
    task_4 = 14;
  }
  return;
}

static void script_89E9(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 15) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F40C);
    Script_SetFogFlags(0, data_F41C);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F42C);
    local_E4.x = 0.48f;
    (&local_E4)->y = 0.44f;
    (&local_E4)->z = 0.76f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -3.7f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -3.66f;
    (&local_E4)->y = 1.44f;
    (&local_E4)->z = -21.19f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 2.5f);
    Script_LightUtils(1, 5, 0.49f);
    local_DC[0] = 0x3F000000;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    local_E4.x = -4.13f;
    (&local_E4)->y = 1.09f;
    (&local_E4)->z = -21.37f;
    Script_lightEnablePointLightAt(2, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(2, 2, &local_F0);
    Script_LightUtils(2, 4, 1.4f);
    Script_LightUtils(2, 5, 0.85f);
    local_DC[0] = 0x3E23D70A;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(2, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(2, 1);
    task_4 = 15;
  }
  return;
}

static void script_8BF8(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 16) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F43C);
    Script_SetFogFlags(0, data_F44C);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F45C);
    local_E4.x = -0.78f;
    (&local_E4)->y = -0.46f;
    (&local_E4)->z = -0.42f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -10.0f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -5.94f;
    (&local_E4)->y = 2.42f;
    (&local_E4)->z = -21.46f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, -0.7f);
    Script_LightUtils(1, 5, 1.06f);
    local_DC[0] = 0x3E947AE1;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    task_4 = 16;
  }
  return;
}

static void script_8D66(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 17) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F46C);
    Script_SetFogFlags(0, data_F47C);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F48C);
    local_E4.x = 0.01f;
    (&local_E4)->y = -0.48f;
    (&local_E4)->z = 0.88f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -10.0f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -6.4f;
    (&local_E4)->y = 2.46f;
    (&local_E4)->z = -21.27f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, -0.7f);
    Script_LightUtils(1, 5, 0.35f);
    local_DC[0] = 0x3F400000;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    task_4 = 17;
  }
  return;
}

static void script_8ED4(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 18) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F49C);
    Script_SetFogFlags(0, data_F4AC);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F4BC);
    local_E4.x = 0.74f;
    (&local_E4)->y = 0.6f;
    (&local_E4)->z = -0.29f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -10.0f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -6.5f;
    (&local_E4)->y = 1.21f;
    (&local_E4)->z = -20.56f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 2.9f);
    Script_LightUtils(1, 5, 0.87f);
    local_DC[0] = 0x3F47AE14;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    task_4 = 18;
  }
  return;
}

static void script_9042(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 19) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F4CC);
    Script_SetFogFlags(0, data_F4DC);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F4EC);
    local_E4.x = 0.39f;
    (&local_E4)->y = -0.74f;
    (&local_E4)->z = -0.54f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -10.0f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -6.02f;
    (&local_E4)->y = 2.04f;
    (&local_E4)->z = -20.92f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.81f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 6.5f);
    Script_LightUtils(1, 5, 0.48f);
    local_DC[0] = 0x3F3AE148;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    local_E4.x = -6.29f;
    (&local_E4)->y = 1.75f;
    (&local_E4)->z = -20.17f;
    Script_lightEnablePointLightAt(2, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(2, 2, &local_F0);
    Script_LightUtils(2, 4, 0.5f);
    Script_LightUtils(2, 5, 0.71f);
    local_DC[0] = 0x3F2B851F;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(2, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(2, 1);
    task_4 = 19;
  }
  return;
}

static void script_9251(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  if (task_4 != 24) {
    Script_PREP_Related(-1, 0);
    Script_SetFogColour(&data_F4FC);
    Script_SetFogFlags(0, data_F50C);
    Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
    Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
    Script_shenSetLightingMode(2);
    Script_lightSetGlobalSceneLighting(&data_F51C);
    local_E4.x = -0.47f;
    (&local_E4)->y = -0.22f;
    (&local_E4)->z = -0.86f;
    Script_lightDirLightEnable(5, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 0.06f;
    (&local_F0)->z = 0.06f;
    (&local_F0)->w = 0.06f;
    Script_LightUtils(5, 2, &local_F0);
    Script_LightUtils(5, 4, -2.9f);
    Script_lightSetSlotEnabled(5, 1);
    local_E4.x = -4.58f;
    (&local_E4)->y = 1.61f;
    (&local_E4)->z = -20.98f;
    Script_lightEnablePointLightAt(1, &local_E4);
    local_F0.x = 1.0f;
    (&local_F0)->y = 1.0f;
    (&local_F0)->z = 1.0f;
    (&local_F0)->w = 0.73f;
    Script_LightUtils(1, 2, &local_F0);
    Script_LightUtils(1, 4, 2.4f);
    Script_LightUtils(1, 5, 0.53f);
    local_DC[0] = 0x3EDC28F6;
    local_DC[1] = 0x3F800000;
    Script_LightUtils(1, 6, (const float *)local_DC);
    Script_lightSetSlotEnabled(1, 1);
    task_4 = 24;
  }
  return;
}

static void script_93BF(void) {
  task_30 = 0;
  return;
}

__attribute__((scn_string_bindings(9, data_F588, 11, data_F5A1, 13, data_F5BB, 15, data_F5D4, 16, data_F5E5, 17, data_F5F6, 18, data_F607, 19, data_F618)))
static void script_93C9(void) {
  script_D406(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_ryoSetChara(CHARA_AKIR);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  task_0 = Script_EV_PAK_AFS_Utils(EVPAK_MODE_AFS_READ, 0, "scene/01/OP00", "OP99.afs");
  task_1C = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 0, 1);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_1D = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 2, 3);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_29 = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 26, 27);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_2B = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 30, 31);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motiLoad(1, SHEN_NAME('OP00'), "M_0101A.BIN");
  Script_motiLoad(1, SHEN_NAME('OP00'), "M_UO.BIN");
  Script_LoadHandMot(1, SHEN_NAME('OP00'), "HMOT0102.BIN");
  Script_motiLoad(1, SHEN_NAME('OP00'), "M_01103.BIN");
  Script_motiLoad(1, SHEN_NAME('OP00'), "M_0119.BIN");
  Script_LoadVoiceFile(1, "A0114");
  Script_ProcessSubtitle("OPEN1", 0);
  Script_SetCameraFlags(0);
  task_18 = Script_MT5Loader_TRWV_TRWAVE(0, &data_E780, "model/object", "trlake1.pvr", "model/object", "trsea025.mt6", 0.25f, 0x1000);
  Script_MT5Loader_TRWV_TRWAVE(17, task_18, "model/object", "grid025.bin");
  Script_MT5Loader_TRWV_TRWAVE(13, task_18, 0.0196078438f, 0.494117647f);
  Script_MT5Loader_TRWV_TRWAVE(14, task_18, -0.0196078438f, -0.0431372561f);
  Script_MT5Loader_TRWV_TRWAVE(16, task_18, 0, 0);
  Script_MT5Loader_TRWV_TRWAVE(21, task_18, 2, 2);
  Script_MT5Loader_TRWV_TRWAVE(24, task_18, 255, 169, 190, 125);
  Script_MT5Loader_TRWV_TRWAVE(10, task_18, 4.0f, 4.0f, 0, 1, 0, 1);
  Script_MT5Loader_TRWV_TRWAVE(11, task_18, 0.44f, -0.78f, -0.46f);
  Script_MT5Loader_TRWV_TRWAVE(2, task_18);
  Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  Script_SpriteUtils(Scroll_Read3, 0, 0);
  Script_WeatherUtils_ForFIXC(10);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 1);
  task_C = Script_WeatherUtils_ForFIXC(1, 0x4000);
  Script_WeatherUtils(WeatherScript_InitSnowSystems, 7.0f);
  Script_WeatherUtils(WeatherScript_ApplySnowGameData, 0.1f, 1);
  Script_WeatherUtils(WeatherScript_SetWccpClipSet1Template, 0.0f, -0.0475f, 0.0f, 0.04f, 0.022f, 0.04f);
  Script_WeatherUtils(WeatherScript_SetSnowScalar0, 0.05f);
  Script_WeatherTaskUtil(StorePreviousOperand, 0);
  Script_WeatherUtils(WeatherScript_UpdateRainOffset, -0x382C18);
  task_30 = 1;
  Script_scenCreateTask(script_2F29, 0);
  Script_scenToggleActiveFor(SHEN_NAME('RMJN'), 1);
  Script_FACE_LCG(CHARA_AKIR, FACE_BlinkOpen, 1, 0);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 0);
  Script_SetFightProgramStateBytes(CHARA_AKIR, 0, 0);
  Script_GetDataFromFightProgram(CHARA_AKIR, 0);
  if (0) {
    Script_charaDisplay(SHEN_NAME('MNLF'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('MNLF'), CDM_Show);
      goto L00977E;
    }
  }
  L00977E:;
  Script_scenToggleVisibleFor(SHEN_NAME('MNLF'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('MNRG'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('MNRG'), CDM_Show);
      goto L0097B2;
    }
  }
  L0097B2:;
  Script_scenToggleVisibleFor(SHEN_NAME('MNRG'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('RMJN'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('RMJN'), CDM_Show);
      goto L0097E6;
    }
  }
  L0097E6:;
  Script_scenToggleVisibleFor(SHEN_NAME('RMJN'), 1);
  Script_charaDisplay(SHEN_NAME('KNBS'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KNBU'), CDM_Show);
  Script_scenCreateTask(script_0153, 1, 1);
  {
    int32_t local_FC;
    local_FC = Script_scenCreateTask(script_04CA, 0);
    Script_scenCreateTask(script_CF5D, 0);
    Script_shenSeqUtils(0);
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_FC);
  }
  if (task_2E) {
  } else {
    while (1) {
      if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    script_D406(CHARA_INE_);
    task_1D = 0;
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#002#003");
    task_2B = 0;
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#030#031");
    Script_scenCreateTask(script_1558, 0);
    {
      int32_t local_FC;
      local_FC = Script_scenCreateTask(script_0855, 0);
      Script_scenCreateTask(script_CFDA, 0);
      Script_shenSeqUtils(1);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_srchControl((void *)local_FC);
    }
    if (task_2E) {
    } else {
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      if (0) {
        Script_charaDisplay(SHEN_NAME('ODR1'), CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(SHEN_NAME('ODR1'), CDM_Show);
          goto L009952;
        }
      }
      L009952:;
      Script_scenToggleVisibleFor(SHEN_NAME('ODR1'), 1);
      if (0) {
        Script_charaDisplay(SHEN_NAME('ODR2'), CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(SHEN_NAME('ODR2'), CDM_Show);
          goto L009986;
        }
      }
      L009986:;
      Script_scenToggleVisibleFor(SHEN_NAME('ODR2'), 1);
      task_1C = 0;
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#000#001");
      if (-1) {
        Script_charaDisplay(CHARA_INE_, CDM_Hide);
      } else {
        if (0) {
          Script_charaDisplay(CHARA_INE_, CDM_Show);
          goto L0099CF;
        }
      }
      L0099CF:;
      Script_scenToggleVisibleFor(CHARA_INE_, 0);
      script_2F96();
      {
        int32_t local_FC;
        local_FC = Script_scenCreateTask(script_0E3F, 0);
        Script_scenCreateTask(script_D06C, 0);
        Script_shenSeqUtils(2);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
      }
      if (task_2E) {
      } else {
        int32_t local_FC;
        while (1) {
          if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_WeatherUtils(WeatherScript_ClearSnowTaskData);
        Script_WeatherUtils_ForFIXC(7, (void *)task_C);
        task_30 = 0;
        script_D406(CHARA_FUKU);
        task_2A = 0;
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#028#029");
        task_C = Script_WeatherUtils_ForFIXC(6, 5.5f, 4.0f, -23.0f, 6.2f, 8.0f, 26.0f);
        task_10 = Script_WeatherUtils_ForFIXC(6, 0.0f, 4.0f, -6.0f, 10.0f, 8.0f, 16.0f);
        Script_WeatherUtils(WeatherScript_InitSnowSystems, 7.0f);
        Script_WeatherUtils(WeatherScript_SetForcedSnowValue, 0.1f);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet1Template, 0.0f, -0.0475f, 0.0f, 0.04f, 0.022f, 0.04f);
        Script_WeatherUtils(WeatherScript_SetSnowScalar0, 0.05f);
        Script_WeatherTaskUtil(StorePreviousOperand, 0);
        Script_WeatherUtils(WeatherScript_UpdateRainOffset, -0x382C18);
        task_30 = 2;
        local_FC = Script_scenCreateTask(script_0F51, 0);
        Script_scenCreateTask(script_D0B9, 0);
        Script_shenSeqUtils(3);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
        if (task_2E) {
        } else {
          while (1) {
            if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
          Script_WeatherUtils(WeatherScript_ClearSnowTaskData);
          Script_WeatherUtils_ForFIXC(7, (void *)task_C);
          Script_WeatherUtils_ForFIXC(7, (void *)task_10);
          task_30 = 0;
          task_20 = 0;
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#008#009");
        }
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_F629, 1, data_F63A, 2, data_F64B, 3, data_F65C, 4, data_F66D, 5, data_F67E, 6, data_F68F, 7, data_F6A0, 8, data_F6B1, 9, data_F6C2, 10, data_F6D3, 11, data_F6E4, 12, data_F6F5, 13, data_F706, 14, data_F717, 15, data_F728)))
static void script_9BD7(void) {
  int32_t local_FC;
  while (task_2D) {
    SCNF_WaitFrames_0000(1);
  }
  task_34 = 0;
  task_38 = 0;
  task_3C = 0;
  local_FC = task_30;
  switch (local_FC) {
    case 1:
      Script_WeatherUtils(WeatherScript_ClearSnowTaskData);
      Script_WeatherUtils_ForFIXC(7, (void *)task_C);
      task_30 = 0;
      break;
    case 2:
      Script_WeatherUtils(WeatherScript_ClearSnowTaskData);
      Script_WeatherUtils_ForFIXC(7, (void *)task_C);
      Script_WeatherUtils_ForFIXC(7, (void *)task_10);
      task_30 = 0;
      break;
  }
  task_30 = 0;
  if (task_1C != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 0, 1);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#000#001");
    }
    task_1C = 0;
  }
  if (task_1D != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 2, 3);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#002#003");
    }
    task_1D = 0;
  }
  if (task_1E != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 4, 5);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#004#005");
    }
    task_1E = 1;
  }
  Script_facemotUtils(CHARA_INE_, 0, 0);
  Script_facemotUtils(CHARA_INE_, 1, 0);
  if (task_1F != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 6, 7);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#006#007");
    }
    task_1F = 1;
  }
  if (task_20 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 8, 9);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#008#009");
    }
    task_20 = 0;
  }
  if (task_21 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 10, 11);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#010#011");
    }
    task_21 = 1;
  }
  if (task_22 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 12, 13);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#012#013");
    }
    task_22 = 1;
  }
  if (task_23 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 14, 15);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#014#015");
    }
    task_23 = 0;
  }
  if (task_24 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 16, 17);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#016#017");
    }
    task_24 = 1;
  }
  Script_facemotUtils(CHARA_IWAO, 0, 0);
  Script_facemotUtils(CHARA_IWAO, 1, 0);
  if (task_25 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 18, 19);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#018#019");
    }
    task_25 = 1;
  }
  Script_facemotUtils(CHARA_SORY, 0, 0);
  Script_facemotUtils(CHARA_SORY, 1, 0);
  if (task_26 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 20, 21);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#020#021");
    }
    task_26 = 0;
  }
  if (task_27 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 22, 23);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#022#023");
    }
    task_27 = 0;
  }
  if (task_28 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 24, 25);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#024#025");
    }
    task_28 = 0;
  }
  if (task_29 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 26, 27);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#026#027");
    }
    task_29 = 1;
  }
  if (task_2A != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 28, 29);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#028#029");
    }
    task_2A = 0;
  }
  if (task_2B != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 30, 31);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#030#031");
    }
    task_2B = 0;
  }
  if (-1) {
    Script_charaDisplay(CHARA_INE_, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_INE_, CDM_Show);
      goto L00A2A6;
    }
  }
  L00A2A6:;
  Script_scenToggleVisibleFor(CHARA_INE_, 0);
  task_2F = 1;
  return;
}

__attribute__((scn_string_bindings(1, data_F762, 2, data_F773, 3, data_F784)))
static void script_A2BA(void) {
  script_D406(CHARA_SORY);
  script_D406(CHARA_IWAO);
  script_D406(CHARA_KURA);
  script_D406(CHARA_KURB);
  Script_charaDisplay(SHEN_NAME('KANB'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Show);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HIK_'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HIK_'), CDM_Show);
      goto L00A335;
    }
  }
  L00A335:;
  Script_scenToggleVisibleFor(SHEN_NAME('HIK_'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Show);
      goto L00A369;
    }
  }
  L00A369:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Show);
      goto L00A39D;
    }
  }
  L00A39D:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
      goto L00A3D1;
    }
  }
  L00A3D1:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Show);
      goto L00A405;
    }
  }
  L00A405:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('ODR1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('ODR1'), CDM_Show);
      goto L00A439;
    }
  }
  L00A439:;
  Script_scenToggleVisibleFor(SHEN_NAME('ODR1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('ODR2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('ODR2'), CDM_Show);
      goto L00A46D;
    }
  }
  L00A46D:;
  Script_scenToggleVisibleFor(SHEN_NAME('ODR2'), 1);
  Script_StartOrFreeSEQ(0);
  Script_ProcessSubtitle("OPEN2", 0);
  task_30 = 3;
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 1);
  task_C = Script_WeatherUtils_ForFIXC(6, 4.8f, 2.8f, -23.0f, 3.0f, 5.0f, 18.0f);
  task_10 = Script_WeatherUtils_ForFIXC(6, 1.6f, 2.8f, -13.5f, 3.0f, 5.0f, 6.0f);
  task_14 = Script_WeatherUtils_ForFIXC(6, 1.6f, 4.5f, -31.5f, 3.0f, 3.0f, 6.0f);
  Script_WeatherUtils(WeatherScript_InitSnowSystems, 7.0f);
  Script_WeatherUtils(WeatherScript_SetForcedSnowValue, 0.2f);
  Script_WeatherUtils(WeatherScript_SetSnowScalar0, 0.12f);
  Script_WeatherTaskUtil(StorePreviousOperand, 0);
  Script_WeatherUtils(WeatherScript_UpdateRainOffset, -0x382C18);
  Script_scenCreateTask(script_66FC, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  Script_SetCharaFlags(CHARA_AKIR, 0);
  Script_SetCharaFlags(CHARA_IWAO, 0);
  Script_SetCharaFlags(CHARA_SORY, 0);
  Script_SetCharaFlags(CHARA_KURA, 0);
  Script_SetCharaFlags(CHARA_KURB, 0);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_SORY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KURA, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KURB, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 0);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 0);
  Script_SetShadowFlags(CHARA_SORY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 0);
  Script_SetShadowFlags(CHARA_KURA, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 0);
  Script_SetShadowFlags(CHARA_KURB, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 0);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_SetShadowFlags(CHARA_SORY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_SetShadowFlags(CHARA_KURA, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_SetShadowFlags(CHARA_KURB, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  task_34 = Script_scenCreateTask(script_6BF7, 0);
  task_38 = Script_scenCreateTask(script_0275, 0);
  task_3C = Script_scenCreateTask(script_6DCD, 0);
  {
    int32_t local_FC;
    local_FC = Script_scenCreateTask(script_3172, 0);
    Script_shenSeqUtils(4);
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_FC);
  }
  if (task_2E) {
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_AKIR, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_AKIR, CDM_Show);
        goto L00A723;
      }
    }
    L00A723:;
    Script_scenToggleVisibleFor(CHARA_AKIR, 0);
    if (-1) {
      Script_charaDisplay(CHARA_KURA, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_KURA, CDM_Show);
        goto L00A757;
      }
    }
    L00A757:;
    Script_scenToggleVisibleFor(CHARA_KURA, 0);
    if (-1) {
      Script_charaDisplay(CHARA_KURB, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_KURB, CDM_Show);
        goto L00A78B;
      }
    }
    L00A78B:;
    Script_scenToggleVisibleFor(CHARA_KURB, 0);
    {
      int32_t local_FC;
      local_FC = Script_scenCreateTask(script_337F, 0);
      Script_shenSeqUtils(18);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_srchControl((void *)local_FC);
    }
    if (task_2E) {
    } else {
      if (0) {
        Script_charaDisplay(CHARA_AKIR, CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(CHARA_AKIR, CDM_Show);
          goto L00A7F9;
        }
      }
      L00A7F9:;
      Script_scenToggleVisibleFor(CHARA_AKIR, 1);
      if (0) {
        Script_charaDisplay(CHARA_KURA, CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(CHARA_KURA, CDM_Show);
          goto L00A82D;
        }
      }
      L00A82D:;
      Script_scenToggleVisibleFor(CHARA_KURA, 1);
      if (0) {
        Script_charaDisplay(CHARA_KURB, CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(CHARA_KURB, CDM_Show);
          goto L00A861;
        }
      }
      L00A861:;
      Script_scenToggleVisibleFor(CHARA_KURB, 1);
      {
        int32_t local_FC;
        local_FC = Script_scenCreateTask(script_3ABB, 0);
        Script_shenSeqUtils(19);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
      }
      if (task_2E) {
      } else {
        if (-1) {
          Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_AKIR, CDM_Show);
            goto L00A8CF;
          }
        }
        L00A8CF:;
        Script_scenToggleVisibleFor(CHARA_AKIR, 0);
        if (-1) {
          Script_charaDisplay(CHARA_KURA, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_KURA, CDM_Show);
            goto L00A903;
          }
        }
        L00A903:;
        Script_scenToggleVisibleFor(CHARA_KURA, 0);
        if (-1) {
          Script_charaDisplay(CHARA_KURB, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_KURB, CDM_Show);
            goto L00A937;
          }
        }
        L00A937:;
        Script_scenToggleVisibleFor(CHARA_KURB, 0);
        {
          int32_t local_FC;
          local_FC = Script_scenCreateTask(script_3CED, 0);
          Script_shenSeqUtils(5);
          while (Script_shenSeqUtils(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_srchControl((void *)local_FC);
        }
        if (task_2E) {
        } else {
          if (0) {
            Script_charaDisplay(CHARA_AKIR, CDM_Hide);
          } else {
            if (-1) {
              Script_charaDisplay(CHARA_AKIR, CDM_Show);
              goto L00A9A5;
            }
          }
          L00A9A5:;
          Script_scenToggleVisibleFor(CHARA_AKIR, 1);
          if (-1) {
            Script_charaDisplay(CHARA_IWAO, CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(CHARA_IWAO, CDM_Show);
              goto L00A9D9;
            }
          }
          L00A9D9:;
          Script_scenToggleVisibleFor(CHARA_IWAO, 0);
          {
            int32_t local_FC;
            local_FC = Script_scenCreateTask(script_3F5D, 0);
            Script_shenSeqUtils(6);
            while (Script_shenSeqUtils(-1)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_srchControl((void *)local_FC);
          }
          if (task_2E) {
          } else {
            if (0) {
              Script_charaDisplay(CHARA_KURA, CDM_Hide);
            } else {
              if (-1) {
                Script_charaDisplay(CHARA_KURA, CDM_Show);
                goto L00AA47;
              }
            }
            L00AA47:;
            Script_scenToggleVisibleFor(CHARA_KURA, 1);
            if (0) {
              Script_charaDisplay(CHARA_KURB, CDM_Hide);
            } else {
              if (-1) {
                Script_charaDisplay(CHARA_KURB, CDM_Show);
                goto L00AA7B;
              }
            }
            L00AA7B:;
            Script_scenToggleVisibleFor(CHARA_KURB, 1);
            {
              int32_t local_FC;
              local_FC = Script_scenCreateTask(script_4160, 0);
              Script_shenSeqUtils(7);
              while (Script_shenSeqUtils(-1)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_srchControl((void *)local_FC);
            }
            if (task_2E) {
            } else {
              if (0) {
                Script_charaDisplay(CHARA_IWAO, CDM_Hide);
              } else {
                if (-1) {
                  Script_charaDisplay(CHARA_IWAO, CDM_Show);
                  goto L00AAE9;
                }
              }
              L00AAE9:;
              Script_scenToggleVisibleFor(CHARA_IWAO, 1);
              if (-1) {
                Script_charaDisplay(CHARA_KURA, CDM_Hide);
              } else {
                if (0) {
                  Script_charaDisplay(CHARA_KURA, CDM_Show);
                  goto L00AB1D;
                }
              }
              L00AB1D:;
              Script_scenToggleVisibleFor(CHARA_KURA, 0);
              if (-1) {
                Script_charaDisplay(CHARA_KURB, CDM_Hide);
              } else {
                if (0) {
                  Script_charaDisplay(CHARA_KURB, CDM_Show);
                  goto L00AB51;
                }
              }
              L00AB51:;
              Script_scenToggleVisibleFor(CHARA_KURB, 0);
              {
                int32_t local_FC;
                local_FC = Script_scenCreateTask(script_43B6, 0);
                Script_shenSeqUtils(8);
                while (Script_shenSeqUtils(-1)) {
                  SCNF_WaitFrames_0000(1);
                }
                Script_srchControl((void *)local_FC);
              }
              if (task_2E) {
              } else {
                if (-1) {
                  Script_charaDisplay(CHARA_IWAO, CDM_Hide);
                } else {
                  if (0) {
                    Script_charaDisplay(CHARA_IWAO, CDM_Show);
                    goto L00ABBF;
                  }
                }
                L00ABBF:;
                Script_scenToggleVisibleFor(CHARA_IWAO, 0);
                {
                  int32_t local_FC;
                  local_FC = Script_scenCreateTask(script_46DE, 0);
                  Script_shenSeqUtils(9);
                  while (Script_shenSeqUtils(-1)) {
                    SCNF_WaitFrames_0000(1);
                  }
                  Script_srchControl((void *)local_FC);
                }
                if (task_2E) {
                } else {
                  if (0) {
                    Script_charaDisplay(CHARA_IWAO, CDM_Hide);
                  } else {
                    if (-1) {
                      Script_charaDisplay(CHARA_IWAO, CDM_Show);
                      goto L00AC2D;
                    }
                  }
                  L00AC2D:;
                  Script_scenToggleVisibleFor(CHARA_IWAO, 1);
                  {
                    int32_t local_FC;
                    local_FC = Script_scenCreateTask(script_49A9, 0);
                    Script_shenSeqUtils(10);
                    while (Script_shenSeqUtils(-1)) {
                      SCNF_WaitFrames_0000(1);
                    }
                    Script_srchControl((void *)local_FC);
                  }
                  if (task_2E) {
                  } else {
                    {
                      int32_t local_FC;
                      local_FC = Script_scenCreateTask(script_4A9C, 0);
                      Script_shenSeqUtils(11);
                      while (Script_shenSeqUtils(-1)) {
                        SCNF_WaitFrames_0000(1);
                      }
                      Script_srchControl((void *)local_FC);
                    }
                    if (task_2E) {
                    } else {
                      if (-1) {
                        Script_charaDisplay(CHARA_IWAO, CDM_Hide);
                      } else {
                        if (0) {
                          Script_charaDisplay(CHARA_IWAO, CDM_Show);
                          goto L00ACD5;
                        }
                      }
                      L00ACD5:;
                      Script_scenToggleVisibleFor(CHARA_IWAO, 0);
                      {
                        int32_t local_FC;
                        local_FC = Script_scenCreateTask(script_4BE1, 0);
                        Script_shenSeqUtils(12);
                        while (Script_shenSeqUtils(-1)) {
                          SCNF_WaitFrames_0000(1);
                        }
                        Script_srchControl((void *)local_FC);
                      }
                      if (task_2E) {
                      } else {
                        if (-1) {
                          Script_charaDisplay(CHARA_AKIR, CDM_Hide);
                        } else {
                          if (0) {
                            Script_charaDisplay(CHARA_AKIR, CDM_Show);
                            goto L00AD43;
                          }
                        }
                        L00AD43:;
                        Script_scenToggleVisibleFor(CHARA_AKIR, 0);
                        if (0) {
                          Script_charaDisplay(CHARA_KURA, CDM_Hide);
                        } else {
                          if (-1) {
                            Script_charaDisplay(CHARA_KURA, CDM_Show);
                            goto L00AD77;
                          }
                        }
                        L00AD77:;
                        Script_scenToggleVisibleFor(CHARA_KURA, 1);
                        if (0) {
                          Script_charaDisplay(CHARA_KURB, CDM_Hide);
                        } else {
                          if (-1) {
                            Script_charaDisplay(CHARA_KURB, CDM_Show);
                            goto L00ADAB;
                          }
                        }
                        L00ADAB:;
                        Script_scenToggleVisibleFor(CHARA_KURB, 1);
                        Script_shenSeqUtils(13);
                        while (Script_shenSeqUtils(-1)) {
                          SCNF_WaitFrames_0000(1);
                        }
                        if (task_2E) {
                        } else {
                          if (-1) {
                            Script_charaDisplay(CHARA_KURA, CDM_Hide);
                          } else {
                            if (0) {
                              Script_charaDisplay(CHARA_KURA, CDM_Show);
                              goto L00AE03;
                            }
                          }
                          L00AE03:;
                          Script_scenToggleVisibleFor(CHARA_KURA, 0);
                          if (-1) {
                            Script_charaDisplay(CHARA_KURB, CDM_Hide);
                          } else {
                            if (0) {
                              Script_charaDisplay(CHARA_KURB, CDM_Show);
                              goto L00AE37;
                            }
                          }
                          L00AE37:;
                          Script_scenToggleVisibleFor(CHARA_KURB, 0);
                          if (0) {
                            Script_charaDisplay(CHARA_IWAO, CDM_Hide);
                          } else {
                            if (-1) {
                              Script_charaDisplay(CHARA_IWAO, CDM_Show);
                              goto L00AE6B;
                            }
                          }
                          L00AE6B:;
                          Script_scenToggleVisibleFor(CHARA_IWAO, 1);
                          {
                            int32_t local_FC;
                            local_FC = Script_scenCreateTask(script_4E3D, 0);
                            Script_shenSeqUtils(22);
                            while (Script_shenSeqUtils(-1)) {
                              SCNF_WaitFrames_0000(1);
                            }
                            Script_srchControl((void *)local_FC);
                          }
                          if (task_2E) {
                          } else {
                            if (0) {
                              Script_charaDisplay(CHARA_AKIR, CDM_Hide);
                            } else {
                              if (-1) {
                                Script_charaDisplay(CHARA_AKIR, CDM_Show);
                                goto L00AED9;
                              }
                            }
                            L00AED9:;
                            Script_scenToggleVisibleFor(CHARA_AKIR, 1);
                            {
                              int32_t local_FC;
                              local_FC = Script_scenCreateTask(script_4EF1, 0);
                              Script_shenSeqUtils(23);
                              while (Script_shenSeqUtils(-1)) {
                                SCNF_WaitFrames_0000(1);
                              }
                              Script_srchControl((void *)local_FC);
                            }
                            if (task_2E) {
                            } else {
                              {
                                int32_t local_FC;
                                local_FC = Script_scenCreateTask(script_52D8, 0);
                                Script_shenSeqUtils(14);
                                while (Script_shenSeqUtils(-1)) {
                                  SCNF_WaitFrames_0000(1);
                                }
                                Script_srchControl((void *)local_FC);
                              }
                              if (task_2E) {
                              } else {
                                if (-1) {
                                  Script_charaDisplay(CHARA_IWAO, CDM_Hide);
                                } else {
                                  if (0) {
                                    Script_charaDisplay(CHARA_IWAO, CDM_Show);
                                    goto L00AF81;
                                  }
                                }
                                L00AF81:;
                                Script_scenToggleVisibleFor(CHARA_IWAO, 0);
                                if (-1) {
                                  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
                                } else {
                                  if (0) {
                                    Script_charaDisplay(CHARA_AKIR, CDM_Show);
                                    goto L00AFB5;
                                  }
                                }
                                L00AFB5:;
                                Script_scenToggleVisibleFor(CHARA_AKIR, 0);
                                if (-1) {
                                  Script_charaDisplay(CHARA_SORY, CDM_Hide);
                                } else {
                                  if (0) {
                                    Script_charaDisplay(CHARA_SORY, CDM_Show);
                                    goto L00AFE9;
                                  }
                                }
                                L00AFE9:;
                                Script_scenToggleVisibleFor(CHARA_SORY, 0);
                                if (0) {
                                  Script_charaDisplay(CHARA_KURA, CDM_Hide);
                                } else {
                                  if (-1) {
                                    Script_charaDisplay(CHARA_KURA, CDM_Show);
                                    goto L00B01D;
                                  }
                                }
                                L00B01D:;
                                Script_scenToggleVisibleFor(CHARA_KURA, 1);
                                if (0) {
                                  Script_charaDisplay(SHEN_NAME('RYUK'), CDM_Hide);
                                } else {
                                  if (-1) {
                                    Script_charaDisplay(SHEN_NAME('RYUK'), CDM_Show);
                                    goto L00B051;
                                  }
                                }
                                L00B051:;
                                Script_scenToggleVisibleFor(SHEN_NAME('RYUK'), 1);
                                {
                                  int32_t unused_FC;
                                  Vector3i local_F0;
                                  local_F0.x = 0;
                                  (&local_F0)->y = 14563;
                                  (&local_F0)->z = 0;
                                  Script_EVT_SetRotation(SHEN_NAME('ODR2'), 0x38000000, &local_F0, 0);
                                }
                                Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 40.0f);
                                Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
                                {
                                  int32_t local_FC;
                                  local_FC = Script_scenCreateTask(script_53BF, 0);
                                  Script_shenSeqUtils(15);
                                  while (Script_shenSeqUtils(-1)) {
                                    SCNF_WaitFrames_0000(1);
                                  }
                                  Script_srchControl((void *)local_FC);
                                }
                                if (task_2E) {
                                } else {
                                  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
                                  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
                                  {
                                    int32_t unused_FC;
                                    Vector3i local_F0;
                                    local_F0.x = 0;
                                    (&local_F0)->y = 0;
                                    (&local_F0)->z = 0;
                                    Script_EVT_SetRotation(SHEN_NAME('ODR2'), 0x38000000, &local_F0, 0);
                                  }
                                  if (0) {
                                    Script_charaDisplay(CHARA_SORY, CDM_Hide);
                                  } else {
                                    if (-1) {
                                      Script_charaDisplay(CHARA_SORY, CDM_Show);
                                      goto L00B13D;
                                    }
                                  }
                                  L00B13D:;
                                  Script_scenToggleVisibleFor(CHARA_SORY, 1);
                                  if (-1) {
                                    Script_charaDisplay(CHARA_KURA, CDM_Hide);
                                  } else {
                                    if (0) {
                                      Script_charaDisplay(CHARA_KURA, CDM_Show);
                                      goto L00B171;
                                    }
                                  }
                                  L00B171:;
                                  Script_scenToggleVisibleFor(CHARA_KURA, 0);
                                  if (-1) {
                                    Script_charaDisplay(SHEN_NAME('RYUK'), CDM_Hide);
                                  } else {
                                    if (0) {
                                      Script_charaDisplay(SHEN_NAME('RYUK'), CDM_Show);
                                      goto L00B1A5;
                                    }
                                  }
                                  L00B1A5:;
                                  Script_scenToggleVisibleFor(SHEN_NAME('RYUK'), 0);
                                  Script_shenSeqUtils(16);
                                  while (Script_shenSeqUtils(-1)) {
                                    SCNF_WaitFrames_0000(1);
                                  }
                                  if (task_2E) {
                                  } else {
                                    if (0) {
                                      Script_charaDisplay(CHARA_KURA, CDM_Hide);
                                    } else {
                                      if (-1) {
                                        Script_charaDisplay(CHARA_KURA, CDM_Show);
                                        goto L00B1FD;
                                      }
                                    }
                                    L00B1FD:;
                                    Script_scenToggleVisibleFor(CHARA_KURA, 1);
                                    if (0) {
                                      Script_charaDisplay(SHEN_NAME('RYUK'), CDM_Hide);
                                    } else {
                                      if (-1) {
                                        Script_charaDisplay(SHEN_NAME('RYUK'), CDM_Show);
                                        goto L00B231;
                                      }
                                    }
                                    L00B231:;
                                    Script_scenToggleVisibleFor(SHEN_NAME('RYUK'), 1);
                                    {
                                      int32_t local_FC;
                                      local_FC = Script_scenCreateTask(script_5519, 0);
                                      Script_shenSeqUtils(17);
                                      while (Script_shenSeqUtils(-1)) {
                                        SCNF_WaitFrames_0000(1);
                                      }
                                      Script_srchControl((void *)local_FC);
                                    }
                                    if (task_2E) {
                                    } else {
                                      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
                                      Script_SetFogFlags(0, data_F74C);
                                      if (0) {
                                        Script_charaDisplay(CHARA_KURB, CDM_Hide);
                                      } else {
                                        if (-1) {
                                          Script_charaDisplay(CHARA_KURB, CDM_Show);
                                          goto L00B2B5;
                                        }
                                      }
                                      L00B2B5:;
                                      Script_scenToggleVisibleFor(CHARA_KURB, 1);
                                      {
                                        int32_t local_FC;
                                        local_FC = Script_scenCreateTask(script_4CF9, 0);
                                        Script_scenCreateTask(script_D1EE, 0);
                                        Script_shenSeqUtils(20);
                                        while (Script_shenSeqUtils(-1)) {
                                          SCNF_WaitFrames_0000(1);
                                        }
                                        Script_srchControl((void *)local_FC);
                                      }
                                      if (task_2E) {
                                      } else {
                                        while (1) {
                                          if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
                                            break;
                                          }
                                          SCNF_WaitFrames_0000(1);
                                        }
                                        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 20.0f);
                                        Script_SetFogFlags(0, data_F73C);
                                        task_22 = 0;
                                        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#012#013");
                                        if (-1) {
                                          Script_charaDisplay(CHARA_SORY, CDM_Hide);
                                        } else {
                                          if (0) {
                                            Script_charaDisplay(CHARA_SORY, CDM_Show);
                                            goto L00B37E;
                                          }
                                        }
                                        L00B37E:;
                                        Script_scenToggleVisibleFor(CHARA_SORY, 0);
                                        if (0) {
                                          Script_charaDisplay(CHARA_AKIR, CDM_Hide);
                                        } else {
                                          if (-1) {
                                            Script_charaDisplay(CHARA_AKIR, CDM_Show);
                                            goto L00B3B2;
                                          }
                                        }
                                        L00B3B2:;
                                        Script_scenToggleVisibleFor(CHARA_AKIR, 1);
                                        if (0) {
                                          Script_charaDisplay(CHARA_IWAO, CDM_Hide);
                                        } else {
                                          if (-1) {
                                            Script_charaDisplay(CHARA_IWAO, CDM_Show);
                                            goto L00B3E6;
                                          }
                                        }
                                        L00B3E6:;
                                        Script_scenToggleVisibleFor(CHARA_IWAO, 1);
                                        {
                                          int32_t local_FC;
                                          local_FC = Script_scenCreateTask(script_5726, 0);
                                          Script_shenSeqUtils(21);
                                          while (Script_shenSeqUtils(-1)) {
                                            SCNF_WaitFrames_0000(1);
                                          }
                                          Script_srchControl((void *)local_FC);
                                        }
                                        if (task_2E) {
                                        } else {
                                          int32_t local_FC;
                                          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
                                          Script_WeatherUtils(WeatherScript_ClearSnowTaskData);
                                          Script_WeatherUtils_ForFIXC(7, (void *)task_C);
                                          Script_WeatherUtils_ForFIXC(7, (void *)task_10);
                                          Script_WeatherUtils_ForFIXC(7, (void *)task_14);
                                          Script_srchControl((void *)task_34);
                                          task_34 = 0;
                                          Script_srchControl((void *)task_38);
                                          task_38 = 0;
                                          task_21 = 0;
                                          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#010#011");
                                          if (-1) {
                                            Script_charaDisplay(CHARA_IWAO, CDM_Hide);
                                          } else {
                                            if (0) {
                                              Script_charaDisplay(CHARA_IWAO, CDM_Show);
                                              goto L00B4B5;
                                            }
                                          }
                                          L00B4B5:;
                                          Script_scenToggleVisibleFor(CHARA_IWAO, 0);
                                          if (-1) {
                                            Script_charaDisplay(CHARA_AKIR, CDM_Hide);
                                          } else {
                                            if (0) {
                                              Script_charaDisplay(CHARA_AKIR, CDM_Show);
                                              goto L00B4E9;
                                            }
                                          }
                                          L00B4E9:;
                                          Script_scenToggleVisibleFor(CHARA_AKIR, 0);
                                          Script_scenToggleVisibleFor(SHEN_NAME('THN1'), 1);
                                          Script_scenToggleVisibleFor(SHEN_NAME('THN2'), 1);
                                          Script_scenToggleVisibleFor(SHEN_NAME('THN3'), 1);
                                          Script_scenToggleVisibleFor(SHEN_NAME('THN4'), 1);
                                          Script_scenToggleVisibleFor(SHEN_NAME('THN5'), 1);
                                          local_FC = Script_scenCreateTask(script_608A, 0);
                                          Script_shenSeqUtils(24);
                                          while (Script_shenSeqUtils(-1)) {
                                            SCNF_WaitFrames_0000(1);
                                          }
                                          Script_srchControl((void *)local_FC);
                                          if (task_2E) {
                                          } else {
                                            Script_filmCheckOrSetTransitionUtil(80, 0, 0, 0, 0, 255, 0, 0, 0);
                                            while (Script_filmCheckOrSetTransitionUtil(-1)) {
                                              SCNF_WaitFrames_0000(1);
                                            }
                                            Script_WeatherUtils(WeatherScript_FreeWeatherRenderData);
                                            Script_WeatherUtils_ForFIXC(7, (void *)task_8);
                                            task_23 = 0;
                                            Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#014#015");
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_F795, 1, data_F7A6, 2, data_F7B7, 3, data_F7C8, 4, data_F7D9, 5, data_F7EA, 6, data_F7FB, 7, data_F80C, 8, data_F81D, 9, data_F82E, 10, data_F83F, 11, data_F850, 12, data_F861, 13, data_F872, 14, data_F883, 15, data_F894)))
static void script_B5BD(void) {
  int32_t local_FC;
  while (task_2D) {
    SCNF_WaitFrames_0000(1);
  }
  if (task_34) {
    Script_srchControl((void *)task_34);
  }
  if (task_38) {
    Script_srchControl((void *)task_38);
  }
  if (task_3C) {
    Script_srchControl((void *)task_3C);
  }
  local_FC = task_30;
  switch (local_FC) {
    case 3:
      Script_WeatherUtils(WeatherScript_ClearSnowTaskData);
      Script_WeatherUtils_ForFIXC(7, (void *)task_C);
      Script_WeatherUtils_ForFIXC(7, (void *)task_10);
      Script_WeatherUtils_ForFIXC(7, (void *)task_14);
      break;
    case 4:
      Script_WeatherUtils(WeatherScript_FreeWeatherRenderData);
      Script_WeatherUtils_ForFIXC(7, (void *)task_8);
      break;
  }
  task_30 = 0;
  if (task_1C != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 0, 1);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#000#001");
    }
    task_1C = 1;
  }
  if (task_1D != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 2, 3);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#002#003");
    }
    task_1D = 0;
  }
  if (task_1E != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 4, 5);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#004#005");
    }
    task_1E = 1;
  }
  Script_facemotUtils(CHARA_INE_, 0, 0);
  Script_facemotUtils(CHARA_INE_, 1, 0);
  if (task_1F != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 6, 7);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#006#007");
    }
    task_1F = 1;
  }
  if (task_20 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 8, 9);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#008#009");
    }
    task_20 = 0;
  }
  if (task_21 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 10, 11);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#010#011");
    }
    task_21 = 0;
  }
  if (task_22 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 12, 13);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#012#013");
    }
    task_22 = 0;
  }
  if (task_23 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 14, 15);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#014#015");
    }
    task_23 = 0;
  }
  if (task_24 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 16, 17);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#016#017");
    }
    task_24 = 1;
  }
  Script_facemotUtils(CHARA_IWAO, 0, 0);
  Script_facemotUtils(CHARA_IWAO, 1, 0);
  if (task_25 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 18, 19);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#018#019");
    }
    task_25 = 1;
  }
  Script_facemotUtils(CHARA_SORY, 0, 0);
  Script_facemotUtils(CHARA_SORY, 1, 0);
  if (task_26 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 20, 21);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#020#021");
    }
    task_26 = 0;
  }
  if (task_27 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 22, 23);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#022#023");
    }
    task_27 = 0;
  }
  if (task_28 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 24, 25);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#024#025");
    }
    task_28 = 0;
  }
  if (task_29 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 26, 27);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#026#027");
    }
    task_29 = 1;
  }
  if (task_2A != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 28, 29);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#028#029");
    }
    task_2A = 0;
  }
  if (task_2B != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 30, 31);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#030#031");
    }
    task_2B = 0;
  }
  if (-1) {
    Script_charaDisplay(CHARA_SORY, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_SORY, CDM_Show);
      goto L00BC9D;
    }
  }
  L00BC9D:;
  Script_scenToggleVisibleFor(CHARA_SORY, 0);
  if (-1) {
    Script_charaDisplay(CHARA_IWAO, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_IWAO, CDM_Show);
      goto L00BCD1;
    }
  }
  L00BCD1:;
  Script_scenToggleVisibleFor(CHARA_IWAO, 0);
  Script_SetOsageFlags(0, 1.0f);
  Script_SetOsageFlags(1, 1.0f);
  Script_SetOsageFlags(10, 0);
  Script_SetOsageFlags(11, 0);
  Script_SetOsageFlags(20, 0);
  Script_SetOsageFlags(21, 0);
  Script_SetOsageFlags(30, 0);
  Script_SetOsageFlags(31, 0);
  task_2F = 1;
  return;
}

__attribute__((scn_string_bindings(0, data_F8A5, 1, data_F8AA, 2, data_F8AF, 3, data_F8B4, 8, data_F8E3, 9, data_F8F4, 10, data_F905, 11, data_F916)))
static void script_BD33(void) {
  int32_t local_FC;
  Vector3i local_F0;
  Script_StartOrFreeSEQ(0);
  Script_cdttLoadScrUtils(CDTT_CMD_Clear, 0);
  SCNF_WaitFrames_0000(30);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 1, "FREE", 0);
  Script_LoadSND(0, 1, "amb036.snd", 0);
  Script_LoadSND(0, 2, "bgm049.snd", 0);
  Script_LoadSND(0, 4, "a1_tkine.snd", 0);
  Script_LoadVoiceFile(1, "A01103");
  task_2B = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 30, 31);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 17.0f);
  Script_cdttLoadScrUtils(CDTT_CMD_Init, 0);
  script_039D();
  Script_executeAicaCommand(17064, 0, 0);
  Script_executeAicaCommand(1184, 2, 96);
  script_7255();
  if (-1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L00BE6C;
    }
  }
  L00BE6C:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 0);
  Script_scenCreateTask(script_0125, 0);
  local_FC = Script_scenCreateTask(script_712B, 0);
  Script_scenCreateTask(script_D288, 0);
  Script_shenSeqUtils(26);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  if (task_2E) {
  } else {
    while (1) {
      if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    if (0) {
      Script_charaDisplay(SHEN_NAME('MNRG'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('MNRG'), CDM_Show);
        goto L00BF15;
      }
    }
    L00BF15:;
    Script_scenToggleVisibleFor(SHEN_NAME('MNRG'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('MNLF'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('MNLF'), CDM_Show);
        goto L00BF49;
      }
    }
    L00BF49:;
    Script_scenToggleVisibleFor(SHEN_NAME('MNLF'), 1);
    if (0) {
      Script_charaDisplay(CHARA_KWMT, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(CHARA_KWMT, CDM_Show);
        goto L00BF7D;
      }
    }
    L00BF7D:;
    Script_scenToggleVisibleFor(CHARA_KWMT, 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Show);
        goto L00BFB1;
      }
    }
    L00BFB1:;
    Script_scenToggleVisibleFor(SHEN_NAME('BIKE'), 1);
    local_F0.x = 0;
    (&local_F0)->y = 16019;
    (&local_F0)->z = 0;
    Script_EVT_SetRotation(SHEN_NAME('MNRG'), 0x38000000, &local_F0, 0);
    local_FC = Script_scenCreateTask(script_71AA, 0);
    Script_shenSeqUtils(27);
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_FC);
    if (task_2E) {
    } else {
      task_1C = 0;
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#000#001");
      task_26 = 0;
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#020#021");
      if (0) {
        Script_charaDisplay(SHEN_NAME('FUTO'), CDM_Hide);
      } else {
        if (-1) {
          Script_charaDisplay(SHEN_NAME('FUTO'), CDM_Show);
          goto L00C074;
        }
      }
      L00C074:;
      Script_scenToggleVisibleFor(SHEN_NAME('FUTO'), 1);
      script_D406(CHARA_INE_);
      local_FC = Script_scenCreateTask(script_6F3D, 0);
      Script_scenCreateTask(script_D34D, 0);
      Script_shenSeqUtils(25);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_srchControl((void *)local_FC);
      if (task_2E) {
      } else {
        while (1) {
          if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        task_29 = 0;
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#026#027");
        task_27 = 0;
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#022#023");
        Script_executeAicaCommand(0x1200A0, 1, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_F927, 1, data_F938, 2, data_F949, 3, data_F95A, 4, data_F96B, 5, data_F97C, 6, data_F98D, 7, data_F99E, 8, data_F9AF, 9, data_F9C0, 10, data_F9D1, 11, data_F9E2, 12, data_F9F3, 13, data_FA04, 14, data_FA15, 15, data_FA26)))
static void script_C164(void) {
  while (task_2D) {
    SCNF_WaitFrames_0000(1);
  }
  if (task_1C != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 0, 1);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#000#001");
    }
    task_1C = 0;
  }
  if (task_1D != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 2, 3);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#002#003");
    }
    task_1D = 0;
  }
  if (task_1E != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 4, 5);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#004#005");
    }
    task_1E = 0;
  }
  if (task_1F != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 6, 7);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#006#007");
    }
    task_1F = 0;
  }
  if (task_20 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 8, 9);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#008#009");
    }
    task_20 = 0;
  }
  if (task_21 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 10, 11);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#010#011");
    }
    task_21 = 0;
  }
  if (task_22 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 12, 13);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#012#013");
    }
    task_22 = 0;
  }
  if (task_23 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 14, 15);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#014#015");
    }
    task_23 = 0;
  }
  if (task_24 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 16, 17);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#016#017");
    }
    task_24 = 1;
  }
  Script_facemotUtils(CHARA_IWAO, 0, 0);
  Script_facemotUtils(CHARA_IWAO, 1, 0);
  if (task_25 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 18, 19);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#018#019");
    }
    task_25 = 1;
  }
  Script_facemotUtils(CHARA_SORY, 0, 0);
  Script_facemotUtils(CHARA_SORY, 1, 0);
  if (task_26 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 20, 21);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#020#021");
    }
    task_26 = 0;
  }
  if (task_27 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 22, 23);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#022#023");
    }
    task_27 = 0;
  }
  if (task_28 != 1) {
    if (1) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 24, 25);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#024#025");
    }
    task_28 = 1;
  }
  Script_facemotUtils(SHEN_NAME('AKI_'), 0, 0);
  Script_facemotUtils(SHEN_NAME('AKI_'), 1, 0);
  if (task_29 != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 26, 27);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#026#027");
    }
    task_29 = 0;
  }
  if (task_2A != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 28, 29);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#028#029");
    }
    task_2A = 0;
  }
  if (task_2B != 0) {
    if (0) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 30, 31);
      while (1) {
        if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "OP99.afs#030#031");
    }
    task_2B = 0;
  }
  if (-1) {
    Script_charaDisplay(CHARA_SORY, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_SORY, CDM_Show);
      goto L00C7BE;
    }
  }
  L00C7BE:;
  Script_scenToggleVisibleFor(CHARA_SORY, 0);
  if (-1) {
    Script_charaDisplay(CHARA_IWAO, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_IWAO, CDM_Show);
      goto L00C7F2;
    }
  }
  L00C7F2:;
  Script_scenToggleVisibleFor(CHARA_IWAO, 0);
  task_2F = 1;
  return;
}

static void script_C806(void) {
  Script_LoadVoiceFile(1, "A0119");
  Script_LoadSND(0, 4, "a1_unasa.snd", 0);
  script_D406(SHEN_NAME('AKI_'));
  script_D406(CHARA_IWAO);
  script_D406(CHARA_SORY);
  Script_SetFogFlags(2, 0);
  script_7A18();
  Script_scenCreateTask(script_D3B4, 0);
  Script_scenCreateTask(script_0125, 0);
  if (0) {
    Script_charaDisplay(SHEN_NAME('AKI_'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('AKI_'), CDM_Show);
      goto L00C893;
    }
  }
  L00C893:;
  Script_scenToggleVisibleFor(SHEN_NAME('AKI_'), 1);
  {
    int32_t local_FC;
    local_FC = Script_scenCreateTask(script_72C2, 0);
    Script_shenSeqUtils(28);
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_FC);
  }
  if (task_2E) {
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('AKI_'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('AKI_'), CDM_Show);
        goto L00C901;
      }
    }
    L00C901:;
    Script_scenToggleVisibleFor(SHEN_NAME('AKI_'), 0);
    if (0) {
      Script_charaDisplay(CHARA_IWAO, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(CHARA_IWAO, CDM_Show);
        goto L00C935;
      }
    }
    L00C935:;
    Script_scenToggleVisibleFor(CHARA_IWAO, 1);
    if (0) {
      Script_charaDisplay(CHARA_SORY, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(CHARA_SORY, CDM_Show);
        goto L00C969;
      }
    }
    L00C969:;
    Script_scenToggleVisibleFor(CHARA_SORY, 1);
    SCNF_WaitFrames_0000(20);
    Script_scenCreateTask(script_01B5, 1, 5);
    script_D3DD();
    Script_FACE(CHARA_SORY, 5, 0, 1);
    Script_FACE(CHARA_IWAO, 4, 6, 1);
    Script_SetMotnFlag(CHARA_SORY, -1);
    Script_SetMotnFlag(CHARA_IWAO, -1);
    Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
    Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
    Script_ResetHandMotion(CHARA_IWAO, 1, 1, 0);
    Script_ResetHandMotion(CHARA_IWAO, 1, 0, 0);
    Script_handSetFloatsFor(CHARA_SORY, 1, data_DD88, 1);
    Script_handSetFloatsFor(CHARA_SORY, 0, data_DD88, 1);
    Script_handSetFloatsFor(CHARA_IWAO, 1, data_DD88, 1);
    Script_handSetFloatsFor(CHARA_IWAO, 0, data_DD88, 1);
    if (task_2E) {
    } else {
      script_7B28();
      Script_charaDisplay(CHARA_SORY, CDM_Show);
      {
        int32_t local_FC;
        local_FC = Script_scenCreateTask(script_78ED, 0);
        Script_shenSeqUtils(30);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
      }
      if (task_2E) {
      } else {
        script_7C96();
        Script_shenSeqUtils(31);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        if (task_2E) {
        } else {
          script_7E04();
          Script_shenSeqUtils(32);
          while (Script_shenSeqUtils(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          if (task_2E) {
          } else {
            script_866C();
            Script_shenSeqUtils(33);
            while (Script_shenSeqUtils(-1)) {
              SCNF_WaitFrames_0000(1);
            }
            if (task_2E) {
            } else {
              script_82EF();
              Script_shenSeqUtils(34);
              while (Script_shenSeqUtils(-1)) {
                SCNF_WaitFrames_0000(1);
              }
              if (task_2E) {
              } else {
                script_7F72();
                Script_shenSeqUtils(35);
                while (Script_shenSeqUtils(-1)) {
                  SCNF_WaitFrames_0000(1);
                }
                if (task_2E) {
                } else {
                  script_80E0();
                  Script_shenSeqUtils(36);
                  while (Script_shenSeqUtils(-1)) {
                    SCNF_WaitFrames_0000(1);
                  }
                  if (task_2E) {
                  } else {
                    script_8BF8();
                    Script_charaDisplay(CHARA_IWAO, CDM_Show);
                    {
                      int32_t local_FC;
                      local_FC = Script_scenCreateTask(script_792C, 0);
                      Script_shenSeqUtils(37);
                      while (Script_shenSeqUtils(-1)) {
                        SCNF_WaitFrames_0000(1);
                      }
                      Script_srchControl((void *)local_FC);
                    }
                    if (task_2E) {
                    } else {
                      Script_charaDisplay(CHARA_IWAO, CDM_Hide);
                      script_8ED4();
                      Script_shenSeqUtils(38);
                      while (Script_shenSeqUtils(-1)) {
                        SCNF_WaitFrames_0000(1);
                      }
                      if (task_2E) {
                      } else {
                        script_9042();
                        Script_shenSeqUtils(39);
                        while (Script_shenSeqUtils(-1)) {
                          SCNF_WaitFrames_0000(1);
                        }
                        if (task_2E) {
                        } else {
                          script_9042();
                          Script_shenSeqUtils(40);
                          while (Script_shenSeqUtils(-1)) {
                            SCNF_WaitFrames_0000(1);
                          }
                          if (task_2E) {
                          } else {
                            script_9042();
                            Script_shenSeqUtils(41);
                            while (Script_shenSeqUtils(-1)) {
                              SCNF_WaitFrames_0000(1);
                            }
                            if (task_2E) {
                            } else {
                              script_9042();
                              Script_shenSeqUtils(42);
                              while (Script_shenSeqUtils(-1)) {
                                SCNF_WaitFrames_0000(1);
                              }
                              if (task_2E) {
                              } else {
                                script_87DA();
                                Script_charaDisplay(CHARA_SORY, CDM_Hide);
                                Script_charaDisplay(CHARA_IWAO, CDM_Show);
                                Script_shenSeqUtils(43);
                                while (Script_shenSeqUtils(-1)) {
                                  SCNF_WaitFrames_0000(1);
                                }
                                if (task_2E) {
                                } else {
                                  script_9251();
                                  {
                                    int32_t local_FC;
                                    local_FC = Script_scenCreateTask(script_7969, 0);
                                    Script_shenSeqUtils(44);
                                    while (Script_shenSeqUtils(-1)) {
                                      SCNF_WaitFrames_0000(1);
                                    }
                                    Script_srchControl((void *)local_FC);
                                  }
                                  if (task_2E) {
                                  } else {
                                    script_7C96();
                                    Script_charaDisplay(CHARA_IWAO, CDM_Hide);
                                    Script_charaDisplay(CHARA_SORY, CDM_Show);
                                    Script_SetOsageFlags(1, 0.7f);
                                    Script_shenSeqUtils(45);
                                    while (Script_shenSeqUtils(-1)) {
                                      SCNF_WaitFrames_0000(1);
                                    }
                                    if (task_2E) {
                                    } else {
                                      script_89E9();
                                      Script_charaDisplay(CHARA_SORY, CDM_Hide);
                                      Script_ResetHandMotion(CHARA_SORY, 0, 1, 0);
                                      Script_ResetHandMotion(CHARA_SORY, 0, 0, 0);
                                      Script_charaDisplay(CHARA_IWAO, CDM_Show);
                                      Script_SetOsageFlags(1, 1.0f);
                                      Script_shenSeqUtils(46);
                                      while (Script_shenSeqUtils(-1)) {
                                        SCNF_WaitFrames_0000(1);
                                      }
                                      if (task_2E) {
                                      } else {
                                        Script_executeAicaCommand(2720, 2, 20);
                                        task_2F = 0;
                                        Script_filmCheckOrSetTransitionUtil(20, 0, 255, 255, 255, 255, 255, 255, 255);
                                        while (Script_filmCheckOrSetTransitionUtil(-1)) {
                                          SCNF_WaitFrames_0000(1);
                                        }
                                        script_D3B4();
                                        Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
                                        Script_ResetHandMotion(CHARA_IWAO, 0, 0, 0);
                                        SCNF_WaitFrames_0000(20);
                                        if (task_2E) {
                                        } else {
                                          int32_t local_FC;
                                          script_7A18();
                                          Script_scenCreateTask(script_01B5, 1, 5);
                                          if (-1) {
                                            Script_charaDisplay(CHARA_IWAO, CDM_Hide);
                                          } else {
                                            if (0) {
                                              Script_charaDisplay(CHARA_IWAO, CDM_Show);
                                              goto L00CEA3;
                                            }
                                          }
                                          L00CEA3:;
                                          Script_scenToggleVisibleFor(CHARA_IWAO, 0);
                                          if (-1) {
                                            Script_charaDisplay(CHARA_SORY, CDM_Hide);
                                          } else {
                                            if (0) {
                                              Script_charaDisplay(CHARA_SORY, CDM_Show);
                                              goto L00CED7;
                                            }
                                          }
                                          L00CED7:;
                                          Script_scenToggleVisibleFor(CHARA_SORY, 0);
                                          if (0) {
                                            Script_charaDisplay(SHEN_NAME('AKI_'), CDM_Hide);
                                          } else {
                                            if (-1) {
                                              Script_charaDisplay(SHEN_NAME('AKI_'), CDM_Show);
                                              goto L00CF0B;
                                            }
                                          }
                                          L00CF0B:;
                                          Script_scenToggleVisibleFor(SHEN_NAME('AKI_'), 1);
                                          local_FC = Script_scenCreateTask(script_74DF, 0);
                                          Script_shenSeqUtils(29);
                                          while (Script_shenSeqUtils(-1)) {
                                            SCNF_WaitFrames_0000(1);
                                          }
                                          Script_srchControl((void *)local_FC);
                                          if (task_2E) {
                                          } else {
                                            Script_executeAicaCommand(2720, 2, 30);
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return;
}

static int32_t script_CF5D(void) {
  task_2D = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  task_1E = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 4, 5);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_facemotUtils(CHARA_INE_, 0, 0);
  Script_facemotUtils(CHARA_INE_, 1, 0);
  task_2D = 0;
}

static int32_t script_CFDA(void) {
  task_2D = 1;
  task_1F = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 6, 7);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_2A = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 28, 29);
  Script_mapmSetRenderFlag(4, 0);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_2D = 0;
}

static int32_t script_D06C(void) {
  task_2D = 1;
  task_20 = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 8, 9);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_2D = 0;
}

static int32_t script_D0B9(void) {
  task_2D = 1;
  task_21 = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 10, 11);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_24 = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 16, 17);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_facemotUtils(CHARA_IWAO, 0, 0);
  Script_facemotUtils(CHARA_IWAO, 1, 0);
  task_22 = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 12, 13);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_25 = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 18, 19);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_facemotUtils(CHARA_SORY, 0, 0);
  Script_facemotUtils(CHARA_SORY, 1, 0);
  task_2D = 0;
}

static int32_t script_D1EE(void) {
  task_2D = 1;
  task_23 = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 14, 15);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_2D = 0;
}

static void script_D23B(void) {
  task_2D = 1;
  task_2B = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 30, 31);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_2D = 0;
  return;
}

static int32_t script_D288(void) {
  task_2D = 1;
  task_1C = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 0, 1);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_26 = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 20, 21);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_27 = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 22, 23);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_2D = 0;
}

static int32_t script_D34D(void) {
  task_2D = 1;
  task_28 = 1;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)task_0, 24, 25);
  while (1) {
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_facemotUtils(SHEN_NAME('AKI_'), 0, 0);
  Script_facemotUtils(SHEN_NAME('AKI_'), 1, 0);
  task_2D = 0;
}

static int32_t script_D3B4(void) {
  Script_LoadSND(0, 2, "bgm120.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  end:;
}

static void script_D3DD(void) {
  Script_LoadSND(0, 2, "tsm006.snd", 0);
  Script_executeAicaCommand(4776, 0, 0);
  Script_executeAicaCommand(1184, 2, 120);
  return;
}

static void script_D406(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00D432;
    }
  }
  L00D432:;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCharaRefBoardPos(arg0, &data_FA60);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, 1, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  Script_clmdSetFlags(arg0, 8, 1);
  Script_osagUtil(arg0, 0x3F800000);
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_D4DD(void) {
  int32_t local_FC;
  task_1C = 0;
  task_1D = 0;
  task_1E = 0;
  task_1F = 0;
  task_20 = 0;
  task_21 = 0;
  task_22 = 0;
  task_23 = 0;
  task_24 = 0;
  task_25 = 0;
  task_26 = 0;
  task_27 = 0;
  task_28 = 0;
  task_29 = 0;
  task_2A = 0;
  task_2B = 0;
  task_2C = 0;
  local_FC = 0;
  while (local_FC < 47) {
    Script_shenSeqUtils(-12, local_FC, 1);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_D57C(void) {
  task_2F = 1;
  SCNF_WaitFrames_0000(30);
  while (1) {
    task_2E = 0;
    while (task_2E == 0) {
      if (Script_shenSeqUtils(-10)) {
        while (task_2F == 0) {
          SCNF_WaitFrames_0000(1);
        }
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_shenSeqUtils(-6);
        task_2E = 1;
        task_2F = 0;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

void script_D610(void) {
  Script_scenCreateTask(script_D61E, 0);
  return;
}

static int32_t script_D61E(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t unused_F0;
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_SetActionSelectorHUDElementVisibility(1);
  script_D4DD();
  Script_scenCreateTask(script_D57C, 0);
  script_93BF();
  script_93C9();
  if (task_2E) {
    script_9BD7();
    Script_scenCreateTask(script_0153, 1, 30);
  } else {
    script_9BD7();
  }
  script_A2BA();
  Script_ReturnTimeDate(&local_F4);
  (&local_F4)->month = 12;
  (&local_F4)->day = 3;
  (&local_F4)->hours = 8;
  (&local_F4)->minutes = 30;
  Script_SetNewDateTime(&local_F4);
  script_B5BD();
  script_BD33();
  script_C164();
  script_C806();
  Script_ReturnTimeDate(&local_F4);
  (&local_F4)->month = 12;
  (&local_F4)->day = 3;
  (&local_F4)->hours = 8;
  (&local_F4)->minutes = 55;
  Script_SetNewDateTime(&local_F4);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_SLEEP_RESUME_INITIALIZED, 1);
  Script_shenRequestGameModuleTransition(1, SHEN_NAME('JOMO'), 1);
  SCNF_WaitFrames_0000(1);
  end:;
}
