#include "act80_taia.h"

static void script_0030(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_set_val_to_1(3, 0);
  return;
}

static void script_0041(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00006D;
    }
  }
  L00006D:;
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

static void script_00DB(int32_t arg0) {
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

static int32_t script_012B(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0159(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0187(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01BB(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01EF(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_020D(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_0247(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_027B(void) {
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

static void script_0373(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, FACE_BlinkClose, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, FACE_BlinkOpen, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_03A3(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_EC[4];
  Vector3 local_E0;
  int32_t unused_BC[9];
  Vector3i local_B0;
  int32_t local_A4[3];
  int32_t local_A0;
  script_0030();
  script_095B();
  if (arg0) {
    Script_scenCreateTask(script_012B, 0);
  }
  Script_shenSeqUtils(0);
  local_FC = 0;
  local_B0.x = 0;
  (&local_B0)->y = 0;
  (&local_B0)->z = 0;
  local_A4[0] = 0;
  local_A4[1] = 0;
  local_A4[2] = 0;
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
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1834, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_1834, 1);
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_1AE0);
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_1AE0);
        Script_SetHandRootRotation(CHARA_EIIU, 1, 0x15, &data_1AE0);
        Script_SetHandRootRotation(CHARA_EIIU, 0, 0x15, &data_1AE0);
        Script_FACE(CHARA_AKIR, 0, 0, 1);
        Script_FACE(CHARA_EIIU, 0, 0, 1);
        break;
      case 10:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1750, 30);
        break;
      case 84:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1834, 40);
        break;
      case 100:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1834, 40);
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
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1918, 13);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_1918, 13);
        break;
      case 288:
        break;
      case 294:
        break;
      case 255:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1834, 14);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_1834, 14);
        break;
      case 31:
        Script_handSetFloatsFor(CHARA_EIIU, 1, data_19FC, 140);
        Script_handSetFloatsFor(CHARA_EIIU, 0, data_19FC, 140);
        break;
      case 62:
        break;
      case 182:
        Script_handSetFloatsFor(CHARA_EIIU, 1, data_1750, 20);
        Script_handSetFloatsFor(CHARA_EIIU, 0, data_1750, 20);
        break;
      case 210:
        break;
      case 228:
        Script_handSetFloatsFor(CHARA_EIIU, 1, data_1834, 14);
        Script_handSetFloatsFor(CHARA_EIIU, 0, data_1834, 14);
        break;
      case 155:
        Script_mapmSetRenderFlag(2, 0);
        Script_mapmSetRenderFlag(6, 0);
        break;
      case 200:
        Script_mapmSetRenderFlag(2, 1);
        Script_mapmSetRenderFlag(6, 1);
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
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(80, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_0A7F();
  return;
}

static void script_095B(void) {
  int32_t unused_F4[3];
  Script_SetFogColour(&data_1AEC);
  Script_SetFogFlags(0, data_1AFC);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 35.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 35.0f);
  script_0041(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  script_0041(CHARA_EIIU);
  Script_facemotUtils(CHARA_EIIU, 1, 0);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_EIIU, 4);
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetRenderFlag(8, 1);
  Script_mapmSetRenderFlag(9, 0);
  Script_mapmSetRenderFlag(10, 1);
  Script_mapmSetRenderFlag(11, 0);
  Script_mapmSetRenderFlag(12, 0);
  Script_mapmSetRenderFlag(13, 0);
  Script_mapmSetRenderFlag(14, 0);
  Script_mapmSetRenderFlag(15, 0);
  Script_mapmSetRenderFlag(16, 0);
  Script_mapmSetRenderFlag(17, 0);
  Script_mapmSetRenderFlag(18, 1);
  Script_mapmSetRenderFlag(25, 0);
  Script_mapmSetCamFlags(1, 24, 0);
  Script_mapmSetCamFlags(0, 0);
  return;
}

static void script_0A7F(void) {
  int32_t unused_F4[3];
  script_00DB(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_EIIU, 0, 1, 0);
  Script_ResetHandMotion(CHARA_EIIU, 0, 0, 0);
  script_00DB(CHARA_EIIU);
  Script_SetGameStateTo(AUTH_None);
  return;
}

void script_0AE0(void) {
  Script_scenCreateTask(script_0AEE, 0);
  return;
}

static int32_t script_0AEE(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  int32_t local_F0;
  local_FC = Script_GetCurrentEntryPoint();
  while (1) {
    Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    local_F0 = local_FC;
    if (local_F0 == 0) {
      Script_LoadSND(0, 4, "a1_yuka2.snd", 0);
      local_F8 = Script_motiLoad(80, SHEN_NAME('TAIA'), "M_TAIA.BIN");
      Script_AuthorUtils(0, 0, "seqdata1.bin", "scene/80/TAIA");
      script_03A3(1);
      Script_AuthorUtils(AUTH_FREE, 0);
      Script_ReallocateUNFO(local_F8);
      goto L000B8C;
    }
    L000B8C:;
  }
  end:;
}
