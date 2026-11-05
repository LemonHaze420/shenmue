#include "act03_betd.h"

static void script_0030(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_SetupLightFromTable(0);
      Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
      task_4 = Script_scenCreateTask(script_029E, 0);
      break;
    case 1:
      task_4 = Script_scenCreateTask(script_02A1, 0);
      break;
    case 2:
      Script_SetupLightFromTable(1);
      Script_PREP_Related(-1, 1);
      Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 5);
      task_4 = Script_scenCreateTask(script_02A4, 0);
      break;
  }
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)task_4);
  return;
}

static void script_00E4(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  return;
}

static void script_0134(int32_t arg0) {
  Script_ccowUtils(arg0, CCOW_DisableAll);
  Script_ccowSetHeightMode(arg0, CCOW_KeepHeight);
  Script_motmToggleFlags(arg0, SCEN_Disable, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_motmGetCharaFlags(arg0, MOTM_PoseNone, SCEN_Write);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, FACE_ExpressionEnabled, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_0DE8, 1);
  Script_handSetFloatsFor(arg0, 0, data_0DE8, 1);
  Script_FACE_LCG(arg0, FACE_BlinkAutomatic, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, FACE_LookReset);
  Script_InitializeOsag(arg0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  return;
}

static void script_01F6(void) {
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_125C);
  Script_SetCharaRefBoardPos(CHARA_FUKU, &data_125C);
  Script_SetCharaRefBoardPos(CHARA_SORY, &data_125C);
  Script_SetCharaRefBoardPos(CHARA_KURA, &data_125C);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_FUKU, 1);
  Script_charaSetRefboardFlagTo(CHARA_SORY, 1);
  Script_charaSetRefboardFlagTo(CHARA_KURA, 1);
  Script_SetFogEnabled(0);
  return;
}

static void script_026B(void) {
  script_0134(CHARA_AKIR);
  script_0134(CHARA_FUKU);
  script_0134(CHARA_SORY);
  script_00E4(CHARA_KURA);
  return;
}

static int32_t script_029E(void) {
  end:;
}

static int32_t script_02A1(void) {
  end:;
}

static int32_t script_02A4(void) {
  int32_t local_FC;
  task_0 = 0;
  for (;;) {
    local_FC = task_0;
    switch (local_FC) {
      case 0:
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 110);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_SORY, 0, 1, 0);
        Script_ResetHandMotion(CHARA_SORY, 0, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 0, 0);
        Script_SetOsageFlags(0, 0.8f);
        Script_SetOsageFlags(1, 0.8f);
        break;
      case 65:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_0DE8, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_0DE8, 1);
        Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
        Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SORY, 1, data_0DE8, 1);
        Script_handSetFloatsFor(CHARA_SORY, 0, data_0DE8, 1);
        Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_0DE8, 1);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_0DE8, 1);
        break;
      case 238:
        Script_SetMaxInterpSpeed(0.0f);
        break;
      case 326:
        Script_FACE_LCG(CHARA_FUKU, FACE_BlinkClose, 4, 0);
        break;
      case 349:
        Script_SetMaxInterpSpeed(0.6f);
        break;
      case 350:
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_SORY, 0, 1, 0);
        Script_ResetHandMotion(CHARA_SORY, 0, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 0, 0);
        break;
      case 465:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_0DE8, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_0DE8, 1);
        Script_ResetHandMotion(CHARA_SORY, 1, 1, 0);
        Script_ResetHandMotion(CHARA_SORY, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_SORY, 1, data_0DE8, 1);
        Script_handSetFloatsFor(CHARA_SORY, 0, data_0DE8, 1);
        Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_0DE8, 1);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_0DE8, 1);
        break;
      case 771:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_0D04, 8);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_0D04, 8);
        break;
      case 785:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_0ECC, 15);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_0ECC, 15);
        break;
      case 915:
        Script_GetDataFromFightProgram(CHARA_SORY, 4);
        Script_GetDataFromFightProgram(CHARA_SORY, 5);
        Script_SetFightProgramStateBytes(CHARA_SORY, 3, 0);
        break;
      case 930:
        Script_GetDataFromFightProgram(CHARA_SORY, 0);
        break;
      case 931:
        Script_GetDataFromFightProgram(CHARA_AKIR, 4);
        Script_GetDataFromFightProgram(CHARA_AKIR, 5);
        Script_SetFightProgramStateBytes(CHARA_AKIR, 3, 0);
        break;
      case 947:
        Script_GetDataFromFightProgram(CHARA_AKIR, 0);
        break;
      case 948:
        Script_GetDataFromFightProgram(CHARA_SORY, 1);
        Script_SetFightProgramStateBytes(CHARA_SORY, 4, 1);
        Script_handSetFloatsFor(CHARA_SORY, 1, data_1178, 8);
        Script_handSetFloatsFor(CHARA_SORY, 0, data_1178, 8);
        break;
      case 960:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_0D04, 8);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_0D04, 8);
        break;
      case 980:
        Script_filmCheckOrSetTransitionUtil(15, 0, 255, 255, 255, 255, 255, 255, 255);
        break;
      case 994:
        Script_GetDataFromFightProgram(CHARA_SORY, 0);
        break;
    }
    task_0 = task_0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0784(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t unused_D8[3];
  Script_PREP_Related(-1, 0);
  Script_SetFogColour(&data_126C);
  Script_shenSetLightingMode(2);
  Script_lightSetGlobalSceneLighting(&data_127C);
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
  Script_LightUtils(1, 6, data_128C);
  Script_lightSetSlotEnabled(1, 1);
  Script_LightUtils(1, 11, 1);
  local_E4.x = -10.54f;
  (&local_E4)->y = 2.32f;
  (&local_E4)->z = -17.09f;
  Script_lightEnablePointLightAt(2, &local_E4);
  Script_LightUtils(2, 2, &local_F0);
  Script_LightUtils(2, 4, 4.0f);
  Script_LightUtils(2, 5, 3.0f);
  Script_LightUtils(2, 6, data_128C);
  Script_lightSetSlotEnabled(2, 1);
  Script_LightUtils(2, 11, 1);
  local_E4.x = -10.57f;
  (&local_E4)->y = 2.32f;
  (&local_E4)->z = -28.07f;
  Script_lightEnablePointLightAt(3, &local_E4);
  Script_LightUtils(3, 2, &local_F0);
  Script_LightUtils(3, 4, 4.0f);
  Script_LightUtils(3, 5, 3.0f);
  Script_LightUtils(3, 6, data_128C);
  Script_lightSetSlotEnabled(3, 1);
  Script_LightUtils(3, 11, 1);
  local_E4.x = -3.15f;
  (&local_E4)->y = 2.34f;
  (&local_E4)->z = -28.07f;
  Script_lightEnablePointLightAt(4, &local_E4);
  Script_LightUtils(4, 2, &local_F0);
  Script_LightUtils(4, 4, 4.0f);
  Script_LightUtils(4, 5, 3.0f);
  Script_LightUtils(4, 6, data_128C);
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
  Script_SetVector4ByOffset(&data_129C);
  SCNF_WaitFrames_0000(1);
  Script_PREP_Related(-1, 1);
  return;
}

__attribute__((scn_string_bindings(0, data_12BC, 4, data_12E8)))
static int32_t script_0A94(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_ryoSetChara(arg0);
  Script_LoadVoiceFile(1, "01BEDA");
  local_FC = Script_motiLoad(3, SHEN_NAME('BETD'), "M_01BEDA.BIN");
  Script_LoadSND(0, 2, "bgm127.snd", 0);
  Script_LoadSND(0, 4, "a1_baden.snd", 0);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(9, 0);
  Script_mapmSetRenderFlag(10, 0);
  script_01F6();
  script_026B();
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = Script_GetCurrentEntryPoint();
  if (local_F8 == 0) {
    script_0030(0);
    script_0030(1);
    script_0030(2);
    goto L000B8D;
  }
  L000B8D:;
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_ReallocateUNFO(local_FC);
  Script_LoadVoiceFile(0, "01BEDA");
  SCNF_WaitFrames_0000(1);
  Script_SetFogEnabled(0);
  Script_SetCurrentScrollEnabled(0);
  Script_SetFogFlags(2, -0x1000000);
  Script_SetFogFlags(0, data_12AC);
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  Script_charaDisplay(CHARA_SORY, CDM_Hide);
  Script_charaDisplay(CHARA_KURA, CDM_Hide);
  Script_uiUtils(1, 14, 0);
  SCNF_WaitFrames_0000(60);
  Script_uiUtils(5, 30, 1);
  Script_filmCheckOrSetTransitionUtil(90, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(300);
  Script_uiUtils(7, 60, 1);
  SCNF_WaitFrames_0000(90);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  MODU_Script_shenRequestGameModule(Display, 0, 0, 0);
  end:;
}

void script_0CC5(void) {
  Script_scenCreateTask(script_0A94, 1, CHARA_AKIR);
  return;
}
