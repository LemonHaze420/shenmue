#include "act01_yd01.h"

static void script_0030(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0;
  if (local_F8 == 0) {
    local_FC = Script_scenCreateTask(script_02D5, 0);
    Script_filmCheckOrSetTransitionUtil(60, 255, 255, 255, 255, 150, 68, 50, 0);
    Script_SetMITKFlags(StartFadeIn, 5, 60);
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_FC);
    goto L00009A;
  }
  L00009A:;
  return;
}

__attribute__((scn_string_bindings(1, data_1634, 3, data_1646)))
static void script_009B(void) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/YD01/", "SAKR") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_015D(CHARA_IWAO);
  script_015D(CHARA_JAKR);
  Script_AuthorUtils(0, 0, "seqdata0.bin", "SAKR");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaSetRefboardFlagTo(CHARA_IWAO, 1);
  Script_SetCharaRefBoardPos(CHARA_IWAO, &data_1614);
  Script_charaSetRefboardFlagTo(CHARA_JAKR, 1);
  Script_SetCharaRefBoardPos(CHARA_JAKR, &data_1614);
  return;
}

static void script_015D(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 2);
  Script_facemotUtils(arg0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 8, 16);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_SetCPAMFlags(arg0, 0);
  Script_clmdSetFlags(arg0, 2, 0);
  return;
}

static void script_01FF(void) {
  Script_sakuraUtils(2, task_0);
  Script_sakuraUtils(3, task_0);
  Script_charaSetRefboardFlagTo(CHARA_IWAO, 2);
  Script_charaSetRefboardFlagTo(CHARA_JAKR, 2);
  script_025B(CHARA_JAKR);
  script_025B(CHARA_IWAO);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, data_164B);
  return;
}

static void script_025B(int32_t arg0) {
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

static int32_t script_02D5(void) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  Vector3i local_B8;
  Vector3i local_AC;
  int16_t local_AA;
  uint8_t unused_A8[2];
  local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
  local_D0.x = (&local_D0)->y = (&local_D0)->z = 0;
  local_C4.x = (&local_C4)->y = (&local_C4)->z = 0;
  local_B8.x = (&local_B8)->y = (&local_B8)->z = 0;
  local_AC.x = (&local_AC)->y = (&local_AC)->z = 0;
  local_AA = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_AA) {
        case 870:
          Script_ResetHandMotion(CHARA_JAKR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_JAKR, 1, data_1530, 0);
          break;
        case 1200:
          Script_handSetFloatsFor(CHARA_JAKR, 1, data_11A0, 10);
          break;
        case 1290:
          Script_handSetFloatsFor(CHARA_JAKR, 1, data_1530, 10);
          break;
        case 1350:
          Script_ResetHandMotion(CHARA_JAKR, 0, 1, 0);
          break;
      }
      switch (local_AA) {
        case 966:
          Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
          break;
        case 983:
          Script_ResetHandMotion(CHARA_IWAO, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_IWAO, 1, data_11A0, 0);
          break;
        case 1100:
          Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
          break;
      }
      if (local_AA == 1235) {
        Script_ResetHandMotion(CHARA_IWAO, 0, 0, 0);
      }
      switch (local_AA) {
        case 0:
          Script_ResetHandMotion(CHARA_IWAO, 1, 1, 0);
          Script_ResetHandMotion(CHARA_IWAO, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_IWAO, 1, data_11A0, 1);
          Script_handSetFloatsFor(CHARA_IWAO, 0, data_11A0, 1);
          Script_FACE(CHARA_JAKR, 2, 0, 10);
          break;
        case 240:
          Script_handSetFloatsFor(CHARA_IWAO, 1, data_1284, 20);
          Script_handSetFloatsFor(CHARA_IWAO, 0, data_1284, 20);
          break;
        case 250:
          Script_FACE(CHARA_IWAO, 2, 0, 1);
          break;
        case 313:
          Script_scenCreateTask(script_0F44, 3, CHARA_IWAO, 6, 2);
          break;
        case 314:
          Script_scenCreateTask(script_0EB2, 0);
          break;
        case 320:
          Script_scenCreateTask(script_0F84, 2, CHARA_IWAO, 2);
          break;
        case 379:
          Script_handSetFloatsFor(CHARA_IWAO, 1, data_1530, 10);
          Script_scenCreateTask(script_0F44, 3, CHARA_IWAO, 4, 2);
          break;
        case 388:
          Script_scenCreateTask(script_0F84, 2, CHARA_IWAO, 2);
          break;
        case 458:
          Script_handSetFloatsFor(CHARA_IWAO, 1, data_11A0, 676);
          Script_CharHandlers(CHARA_JAKR, 0, 0, 16);
          break;
        case 760:
          Script_handSetFloatsFor(CHARA_IWAO, 0, data_11A0, 10);
          break;
        case 770:
          Script_handSetFloatsFor(CHARA_IWAO, 0, data_10BC, 10);
          break;
        case 905:
          local_E8.x = 0;
          (&local_E8)->y = 0;
          (&local_E8)->z = 0;
          Script_SetHandRootRotation(CHARA_IWAO, 1, 0x15, &local_E8);
          break;
        case 922:
          local_DC[0] = (4642 - (&local_E8)->x) / 10;
          local_DC[1] = (-4441 - (&local_E8)->y) / 10;
          local_DC[2] = (3859 - (&local_E8)->z) / 10;
          break;
        case 930:
          Script_handSetFloatsFor(CHARA_IWAO, 1, data_1368, 10);
          break;
        case 940:
          Script_handSetFloatsFor(CHARA_IWAO, 1, data_11A0, 25);
          break;
        case 1110:
          Script_CharHandlers(CHARA_JAKR, 0, 8, 16);
          Script_handSetFloatsFor(CHARA_IWAO, 0, data_144C, 10);
          break;
        case 1150:
          Script_handSetFloatsFor(CHARA_IWAO, 0, data_11A0, 10);
          break;
        case 1296:
          Script_SetMaxInterpSpeed(0.0f);
          break;
        case 1310:
          Script_FACE(CHARA_IWAO, 1, 0, 30);
          break;
        case 1350:
          Script_SetMaxInterpSpeed(0.5f);
          break;
      }
      if ((local_AA > 0x300) & (local_AA < 780)) {
        local_D0.x = (&local_D0)->x + 479;
        (&local_D0)->y = (&local_D0)->y + 16;
        (&local_D0)->z = (&local_D0)->z + 115;
        Script_SetHandRootRotation(CHARA_IWAO, 0, 0x15, &local_D0);
      } else if (local_AA == 780) {
        local_B8.x = (&local_D0)->x;
        (&local_B8)->y = (&local_D0)->y;
        (&local_B8)->z = (&local_D0)->z;
      } else if ((local_AA > 814) & (local_AA < 834)) {
        local_D0.x = (&local_D0)->x + (1092 - (&local_B8)->x) / 19;
        (&local_D0)->y = (&local_D0)->y + (-1092 - (&local_B8)->y) / 19;
        (&local_D0)->z = (&local_D0)->z + (2002 - (&local_B8)->z) / 19;
        Script_SetHandRootRotation(CHARA_IWAO, 0, 0x15, &local_D0);
      } else if (local_AA == 834) {
        local_B8.x = (&local_D0)->x;
        (&local_B8)->y = (&local_D0)->y;
        (&local_B8)->z = (&local_D0)->z;
      } else if (local_AA == 855) {
        local_D0.x = local_B8.x = 4369;
        (&local_D0)->y = (&local_B8)->y = 3822;
        (&local_D0)->z = (&local_B8)->z = -182;
        Script_SetHandRootRotation(CHARA_IWAO, 0, 0x15, &local_D0);
      } else if ((local_AA > 855) & (local_AA < 870)) {
        local_D0.x = (&local_D0)->x + (728 - (&local_B8)->x) / 14;
        (&local_D0)->y = (&local_D0)->y + (-6389 - (&local_B8)->y) / 14;
        (&local_D0)->z = (&local_D0)->z + (4733 - (&local_B8)->z) / 14;
        Script_SetHandRootRotation(CHARA_IWAO, 0, 0x15, &local_D0);
      } else if (local_AA == 870) {
        local_B8.x = (&local_D0)->x;
        (&local_B8)->y = (&local_D0)->y;
        (&local_B8)->z = (&local_D0)->z;
      } else if ((local_AA >= 871) & (local_AA <= 885)) {
        local_D0.x = (&local_D0)->x + (3640 - (&local_B8)->x) / 15;
        (&local_D0)->y = (&local_D0)->y + (-2767 - (&local_B8)->y) / 15;
        (&local_D0)->z = (&local_D0)->z + (7664 - (&local_B8)->z) / 15;
        Script_SetHandRootRotation(CHARA_IWAO, 0, 0x15, &local_D0);
      } else if (local_AA == 886) {
        local_B8.x = (&local_D0)->x;
        (&local_B8)->y = (&local_D0)->y;
        (&local_B8)->z = (&local_D0)->z;
      } else if ((local_AA >= 887) & (local_AA <= 900)) {
        local_D0.x = (&local_D0)->x + (3822 - (&local_B8)->x) / 14;
        (&local_D0)->y = (&local_D0)->y + (-2767 - (&local_B8)->y) / 14;
        (&local_D0)->z = (&local_D0)->z + (4187 - (&local_B8)->z) / 14;
        Script_SetHandRootRotation(CHARA_IWAO, 0, 0x15, &local_D0);
      } else if (local_AA == 975) {
        local_B8.x = (&local_D0)->x;
        (&local_B8)->y = (&local_D0)->y;
        (&local_B8)->z = (&local_D0)->z;
      } else {
        if ((local_AA > 1040) & (local_AA < 1070)) {
          local_D0.x = (&local_D0)->x + (8920 - (&local_B8)->x) / 29;
          (&local_D0)->y = (&local_D0)->y + (1092 - (&local_B8)->y) / 29;
          (&local_D0)->z = (&local_D0)->z + (1820 - (&local_B8)->z) / 29;
          Script_SetHandRootRotation(CHARA_IWAO, 0, 0x15, &local_D0);
          goto L000AFD;
        }
      }
      L000AFD:;
      if ((local_AA > 825) & (local_AA < 845)) {
        local_C4.x = (&local_C4)->x + -76;
        (&local_C4)->y = (&local_C4)->y + -143;
        (&local_C4)->z = (&local_C4)->z + 57;
        Script_SetHandRootRotation(CHARA_IWAO, 1, 0x15, &local_C4);
      } else if (local_AA == 920) {
        local_AC.x = (&local_C4)->x;
        (&local_AC)->y = (&local_C4)->y;
        (&local_AC)->z = (&local_C4)->z;
      } else if ((local_AA > 905) & (local_AA < 915)) {
        local_C4.x = (&local_C4)->x + (-3458 - (&local_AC)->x) / 9;
        (&local_C4)->y = (&local_C4)->y + (-3458 - (&local_AC)->y) / 9;
        (&local_C4)->z = (&local_C4)->z + (2366 - (&local_AC)->z) / 9;
        Script_SetHandRootRotation(CHARA_IWAO, 1, 0x15, &local_C4);
      } else if (local_AA == 920) {
        local_AC.x = (&local_C4)->x;
        (&local_AC)->y = (&local_C4)->y;
        (&local_AC)->z = (&local_C4)->z;
      } else if ((local_AA > 922) & (local_AA < 932)) {
        local_C4.x = (&local_C4)->x + (-8009 - (&local_AC)->x) / 9;
        (&local_C4)->y = (&local_C4)->y + (-182 - (&local_AC)->y) / 9;
        (&local_C4)->z = (&local_C4)->z + (3640 - (&local_AC)->z) / 9;
        Script_SetHandRootRotation(CHARA_IWAO, 1, 0x15, &local_C4);
      } else if (local_AA == 940) {
        local_AC.x = (&local_C4)->x;
        (&local_AC)->y = (&local_C4)->y;
        (&local_AC)->z = (&local_C4)->z;
      } else {
        if ((local_AA > 950) & (local_AA < 970)) {
          local_C4.x = (&local_C4)->x + -(&local_AC)->x / 9;
          (&local_C4)->y = (&local_C4)->y + -(&local_AC)->y / 9;
          (&local_C4)->z = (&local_C4)->z + -(&local_AC)->z / 9;
          Script_SetHandRootRotation(CHARA_IWAO, 1, 0x15, &local_C4);
          goto L000D17;
        }
      }
      L000D17:;
      switch (local_AA) {
        case 0:
          Script_SetMotnFlag(CHARA_IWAO, -1);
          Script_lightSetGlobalSceneLighting(&data_1660);
          break;
        case 1396:
          Script_lightSetGlobalSceneLighting(&data_1670);
          break;
        case 1530:
          Script_FACE_LCG(CHARA_IWAO, FACE_BlinkOpen, 0, 0);
          break;
      }
      if (local_AA == 1440) {
        Script_executeAicaCommand(2720, 2, 50);
        Script_ConstructColourPair(50, 1, 150, 68, 50, 0, 255, 255, 255, 255);
        Script_SetMITKFlags(StartFadeOut, 0, 50);
      }
      local_AA = local_AA + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0DBE(void) {
  Script_sakuraUtils(15);
  {
    Vector3 local_F4;
    Vector3 local_E8;
    local_E8.x = 1.2f;
    (&local_E8)->y = 1.0f;
    (&local_E8)->z = 1.2f;
    local_F4.x = 9.52f;
    (&local_F4)->y = 3.6f;
    (&local_F4)->z = -27.72f;
    task_0 = Script_sakuraUtils(0, &local_F4, &local_E8, 9000, 0.0025f, 0);
  }
  {
    Vector3 local_F4;
    Vector3 local_E8;
    local_E8.x = 0.00768627459f;
    (&local_E8)->y = -0.00250980398f;
    (&local_E8)->z = -0.00235294108f;
    local_F4.x = -0.00360784307f;
    (&local_F4)->y = -0.00721568614f;
    (&local_F4)->z = 0.00188235298f;
    Script_sakuraUtils(6, task_0, &local_F4, &local_E8);
  }
  Script_sakuraUtils(13, task_0, 0.0235294122f, 0.0235294122f);
  Script_sakuraUtils(10, task_0, 0.305882365f);
  Script_sakuraUtils(7, task_0, 0.000674509851f);
  Script_sakuraUtils(1, task_0);
  SCNF_WaitFrames_0000(35);
  return;
}

static int32_t script_0EB2(void) {
  int32_t local_FC;
  local_FC = Script_EffectUtils(0, &data_1680, &data_168C, 60, 0.025f, 0, 0);
  Script_EffectUtils(9, local_FC, 91, 72, 25);
  Script_EffectUtils(8, local_FC, 1.0f, 1.0f);
  Script_EffectUtils(11, local_FC, 114);
  Script_EffectUtils(12, local_FC, 17);
  Script_EffectUtils(1, local_FC);
  SCNF_WaitFrames_0000(15);
  Script_EffectUtils(7, local_FC, 0.0f);
  SCNF_WaitFrames_0000(20);
  Script_EffectUtils(2, local_FC);
  return Script_EffectUtils(3, local_FC);
}

static int32_t script_0F44(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Script_GetDataFromFightProgram(arg0, arg1);
  local_FC = 0;
  while (local_FC < arg2) {
    local_FC = local_FC + 1;
    Script_SetFightProgramStateBytes(arg0, local_FC, 1);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_0F84(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = 1;
  while (local_FC < arg1) {
    Script_SetFightProgramStateBytes(arg0, local_FC, 1);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return Script_srchControl(0);
}

void script_0FC4(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_filmCheckOrSetTransitionUtil(0, 255, 255, 255, 255, 0, 0, 0, 0);
  script_107D();
  Script_scenCreateTask(script_0FFC, 1, CHARA_AKIR);
  return;
}

__attribute__((scn_string_bindings(0, data_1698, 1, data_169E)))
static int32_t script_0FFC(int32_t arg0) {
  Script_executeAicaCommand(4008, 0, 0);
  Script_executeAicaCommand(1184, 2, 90);
  Script_ryoSetChara(arg0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_LoadVoiceFile(1, "A0128");
  script_009B();
  script_0DBE();
  script_0030(0);
  script_01FF();
  Script_LoadVoiceFile(0, "A0128");
  Script_cdttUtil(Mode1);
  Script_shenRequestGameModuleTransition(1, SHEN_NAME('JHD0'), 5);
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_107D(void) {
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  return;
}
