#include "act03_msbs.h"

static void script_0030(void);
static void script_0039(void);
static void script_0048(int32_t arg0);
static void script_00D5(int32_t arg0);
static int32_t script_0125(void);
static void script_0153(int32_t arg0);
static void script_0181(void);
static void script_01B5(int32_t arg0);
static void script_01E9(int32_t arg0);
static void script_0207(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0241(int32_t arg0, int32_t arg1);
static int32_t script_0275(void);
static void script_036D(int32_t arg0, int32_t arg1);
static void script_039D(void);
static int32_t script_0447(void);
static void script_07E3(void);
static void script_083E(void);
static void script_0859(int32_t arg0);
static void script_08FD(int32_t arg0);
static void script_0926(int32_t arg0);
static void script_0949(int32_t arg0);
static void script_098F(void);
static void script_0992(int32_t arg0);
static void script_0AE3(void);
static void script_0B84(void);
static int32_t script_0BF7(int32_t arg0);
static int32_t script_0E81(int32_t arg0);
static int32_t script_0ED7(void);
static void script_0EF4(void);
void script_0F98(void);
static int32_t script_0FA6(void);
static void script_1207(int32_t arg0, const Vector3 *arg1, const Vector3i *arg2);
static void script_1226(void);
static void script_12CF(void);

typedef struct Row_1414 { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; int32_t w6; } Row_1414;
static const Row_1414 data_1414[9] = {
  {1.0f, 1.0f, 1.0f, 16777215, 16777215, 16777215, 16777215},
  {20.0f, 800.0f, 1.0f, -1, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0}
};
static const int32_t data_1510[5] = {0, 0, 0, 0, 0};
static const int32_t data_1524[684] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650, -2912, -364, -4369, 1638,
  910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546, 0, 0, -21481, 0, 0,
  -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364, -6735, 1638, 910,
  2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847,
  0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546,
  -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182,
  1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0,
  -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0,
  -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0,
  0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546,
  -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279,
  0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0,
  -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456,
  0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0,
  -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0,
  182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364,
  -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738,
  0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189,
  2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0,
  -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0,
  -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0,
  182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_1FD4[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -9102, 0, 0, -6553, 0, 0, -5643, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_20B8[57] = {
  0, 0, -4915, 0, 1456, -1456, 0, 546, 1092, 0, -546, -9102, 0, 0, -6553, 0, 0, -5643, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -2912, 0, 0, -8738, 0, 182, 1638, 0, -910, -4551, 0, -728, -3094, 0, -364, -7099, 0, 0, 1638
};
static const int32_t data_219C[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -16748, 0, 0, -7827, 0, 0, -7645, 0, -182, 1274, 0, 0, -16566, 0, 0, -6189, 0, 0, -8009, 0, 0,
  1456, 0, 182, -17840, 0, 0, -3094, 0, 0, -8920, 0, 182, 1638, 0, -728, -9102, 0, -1274, -7463, 0, -910, -8738, 0, 0, 1638
};
static const FogColourData data_2280 = {.red = 0.1f, .green = 1055.0f, .blue = 0.41f, .colour = 0xFF1B214FU};
static const uint32_t data_2290[4] = {0, 0, 0, 0};
static const Vector4 data_22A0 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static Vector3i data_22B0 = {.x = 0, .y = 14108, .z = 0};
static Vector3i data_22BC = {.x = 0, .y = 30474, .z = 0};
static const Vector3 data_22C8 = {.x = 1.62f, .y = 1.0f, .z = 3.4f};
static const Vector3 data_22D4 = {.x = 3.1f, .y = 2.65f, .z = 8.0f};
static const Vector4 data_22E0 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static Vector3i data_22F0 = {.x = 2730, .y = 0, .z = 0};
static Vector3i data_22FC = {.x = -2730, .y = 0, .z = 0};
static const char data_2308[16] __attribute__((scn_string_literal)) = "/scene/03/MSBS/";
static const char data_2318[5] __attribute__((scn_string_literal)) = "MSBS";
static const char data_231D[6] __attribute__((scn_string_literal)) = "01MSO";
static const char data_2323[11] __attribute__((scn_string_literal)) = "bgm103.snd";
static const char data_232E[11] __attribute__((scn_string_literal)) = "a1_mso.snd";
static const char data_2339[12] __attribute__((scn_string_literal)) = "M_01MSO.BIN";
static const char data_2345[6] __attribute__((scn_string_literal)) = "01MSO";
static const char data_234B[12] __attribute__((scn_string_literal)) = "M_01BUS.BIN";
static const char data_2357[13] __attribute__((scn_string_literal)) = "a1_busno.snd";

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
  Script_ccowUtils(arg0, 2);
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
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_00D5(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_ccowSetFlag80(arg0, 2);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, 0);
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

static void script_036D(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_039D(void) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  script_07E3();
  local_F4 = Script_charaSetClipAngle(CHARA_AKIR, -1.0f);
  Script_charaSetClipAngle(CHARA_AKIR, 10.0f);
  local_FC = Script_scenCreateTask(script_0447, 0);
  local_F8 = Script_scenCreateTask(script_0275, 0);
  Script_shenSeqUtils(0);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_charaSetClipAngle(CHARA_AKIR, local_F4);
  script_083E();
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  return;
}

static int32_t script_0447(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_charaVisUtils(CHARA_AKIR, 1);
  local_E4 = 0;
  for (;;) {
    if (local_E4 == 0) {
      Script_FACE_LCG(CHARA_AKIR, 1, 0, 0);
      Script_FACE_LCG(CHARA_IWAO, 1, 0, 0);
    }
    if (local_E4 == 560) {
      Script_WeatherUtils_ForFIXC(13, 0);
      Script_WeatherUtils_ForFIXC(14, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 0, 0, 1, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 1, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 1, 0);
      Script_WeatherTaskUtil(ToggleFogTask, 0);
      Script_WeatherTaskUtil(ToggleCloudXYFlags, 0);
      Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
      Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('DDO1'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('DDO2'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Hide);
      Script_SpriteUtils(Scroll_Read3, 0, 0);
      Script_mapmSetRenderFlag(7, 0);
      Script_SetupLightFromTable(1);
      Script_FACE_LCG(CHARA_IWAO, 2, 14, 0);
      Script_SetFogEnabled(1);
      Script_SetFogColour(&data_2280);
      Script_SetFogFlags(0, data_2290);
    }
    if (local_E4 == 584) {
      Script_handSetFloatsFor(CHARA_IWAO, 1, data_1524, 10);
    }
    if (local_E4 == 629) {
      Script_handSetFloatsFor(CHARA_IWAO, 1, data_1FD4, 10);
    }
    if (local_E4 == 670) {
      Script_handSetFloatsFor(CHARA_IWAO, 1, data_20B8, 10);
      Script_handSetFloatsFor(CHARA_IWAO, 0, data_219C, 10);
      local_E8.x = -1092;
      (&local_E8)->y = 910;
      (&local_E8)->z = -2366;
      Script_SetHandRootRotation(CHARA_IWAO, 1, 0x15, &local_E8);
    }
    if (local_E4 == 747) {
      local_F4.x = -4.87f;
      (&local_F4)->y = 2.13f;
      (&local_F4)->z = -23.87f;
      Script_FaceLookAtTargetUtils(CHARA_IWAO, 2, &local_F4, 15);
    }
    if (local_E4 == 790) {
      Script_shenSetLightingMode(1);
      Script_LightUtils(0, 0, 2);
      Script_lightSetSlotEnabled(0, 1);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 0, 1, 0, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 0, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 1, 0, 0);
      Script_WeatherTaskUtil(ToggleFogTask, 1);
      Script_WeatherTaskUtil(ToggleCloudXYFlags, 1);
      Script_WeatherTaskUtil(ToggleSceneLightingFlags, 1);
      Script_WeatherTaskUtil(LoadSPR);
      Script_WeatherUtils_ForFIXC(13, 1);
      Script_WeatherUtils_ForFIXC(14, 1);
      Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('DDO1'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('DDO2'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Show);
      Script_mapmSetRenderFlag(7, 1);
    }
    if (local_E4 == 810) {
      Script_FACE_LCG(CHARA_AKIR, 2, 15, 0);
    }
    if (local_E4 == 1080) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_1524, 30);
    }
    if (local_E4 == 1110) {
      break;
    }
    local_E4 = local_E4 + 1;
    Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 4.3f, -13.5f, -16.1f, -11.6f, -17.8f, -31.4f, 1.2f, -33.0f, 0.0f, 10.0f);
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 2, 40);
  Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_07E3(void) {
  int32_t unused_E8[6];
  script_0859(CHARA_AKIR);
  script_0859(CHARA_IWAO);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetVector4ByOffset(&data_22A0);
  (void)1;
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_22A0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_IWAO, 0);
  return;
}

static void script_083E(void) {
  script_0949(CHARA_AKIR);
  script_0949(CHARA_IWAO);
  return;
}

static void script_0859(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_1FD4, 10);
  Script_handSetFloatsFor(arg0, 1, data_1FD4, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_scenToggleActiveFor(arg0, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  return;
}

static void script_08FD(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_0926(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 1);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  return;
}

static void script_0949(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static void script_098F(void) {
  return;
}

static void script_0992(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = 0;
  script_0030();
  script_0AE3();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &data_22B0, 0);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &data_22BC, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 160.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 80.0f);
  Script_SetShadowRange(160.0f);
  Script_SetCharaFlags(SHEN_NAME('AKI2'), 4);
  Script_SetCharaFlags(SHEN_NAME('CAR7'), 2);
  Script_SetCharaFlags(SHEN_NAME('CAR7'), 100, 1, &data_22C8);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 0:
        Script_executeAicaCommand(0x1D04A9, 0, 0);
        break;
      case 68:
        Script_scenCreateTask(script_0ED7, 0);
        break;
      case 100:
        Script_scenCreateTask(script_0BF7, 1, 0);
        break;
      case 125:
        Script_executeAicaCommand(0x1E04A9, 0, 0);
        Script_scenToggleActiveFor(SHEN_NAME('AKI2'), 0);
        local_F8 = Script_scenCreateTask(script_0E81, 1, SHEN_NAME('BUS_'));
        break;
      case 260:
        Script_srchControl((void *)local_F8);
        goto L000AE2;
        break;
    }
    local_FC = local_FC + 1;
    script_0EF4();
    SCNF_WaitFrames_0000(1);
  }
  L000AE2:;
  return;
}

static void script_0AE3(void) {
  Vector3 local_F4;
  int32_t unused_E4[4];
  GameDateTime local_DC;
  (void)1;
  Script_SetVector4ByOffset(&data_22E0);
  script_0048(SHEN_NAME('AKI2'));
  script_0048(CHARA_BUSS);
  Script_facemotUtils(SHEN_NAME('AKI2'), 1, 0);
  Script_SetCharaFlags(SHEN_NAME('BUS_'), 2);
  Script_SetCharaFlags(SHEN_NAME('BUS_'), 100, 1, &data_22D4);
  Script_scenToggleActiveFor(CHARA_BUSS, 0);
  Script_ReturnTimeDate(&local_DC);
  if ((&local_DC)->hours < 19) {
    local_F4.x = 0.0f;
    (&local_F4)->y = -100.0f;
    (&local_F4)->z = 0.0f;
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, 0);
  }
  return;
}

static void script_0B84(void) {
  Vector3 local_F4;
  (void)0;
  local_F4.x = 0.0f;
  (&local_F4)->y = -100.0f;
  (&local_F4)->z = 0.0f;
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, 0);
  script_00D5(SHEN_NAME('AKI2'));
  script_00D5(CHARA_BUSS);
  Script_scenToggleActiveFor(SHEN_NAME('AKI2'), 1);
  Script_FaceEx(SHEN_NAME('AKI2'), 1, 0, 2);
  Script_SetCharaFlags(SHEN_NAME('AKI2'), 4);
  return;
}

static int32_t script_0BF7(int32_t arg0) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  Vector3i local_F0;
  Vector3i local_E4;
  Vector3i local_D8;
  Vector3i local_CC;
  Vector3i local_C0;
  Vector3i local_B4;
  local_F0.x = 0;
  (&local_F0)->y = 0;
  (&local_F0)->z = 0;
  local_E4.x = 0;
  (&local_E4)->y = 0;
  (&local_E4)->z = 0;
  local_D8.x = 0;
  (&local_D8)->y = 0;
  (&local_D8)->z = 0;
  local_CC.x = 0;
  (&local_CC)->y = 0;
  (&local_CC)->z = 0;
  local_C0.x = 0;
  (&local_C0)->y = 0;
  (&local_C0)->z = 0;
  local_B4.x = 0;
  (&local_B4)->y = 0;
  (&local_B4)->z = 0;
  {
    int32_t local_B0;
    local_B0 = arg0;
    switch (local_B0) {
      case 0:
        (&local_F0)->y = 14654;
        (&local_E4)->y = -30474;
        break;
      case 2:
        (&local_D8)->y = 14654;
        (&local_CC)->y = -30474;
        (&local_C0)->y = -14654;
        (&local_B4)->y = 30474;
        break;
    }
  }
  local_FF = 0;
  while (local_FF <= 30) {
    int32_t local_B0;
    local_B0 = arg0;
    switch (local_B0) {
      case 0:
        (&local_F0)->y = (&local_F0)->y - 455;
        (&local_E4)->y = (&local_E4)->y + 983;
        break;
      case 1:
        (&local_F0)->y = (&local_F0)->y + 455;
        (&local_E4)->y = (&local_E4)->y - 983;
        break;
      case 2:
        (&local_D8)->y = (&local_D8)->y - 455;
        (&local_CC)->y = (&local_CC)->y + 983;
        (&local_C0)->y = (&local_C0)->y + 455;
        (&local_B4)->y = (&local_B4)->y - 983;
        break;
      case 3:
        (&local_D8)->y = (&local_D8)->y + 455;
        (&local_CC)->y = (&local_CC)->y - 983;
        (&local_C0)->y = (&local_C0)->y - 455;
        (&local_B4)->y = (&local_B4)->y + 983;
        break;
    }
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &local_F0, 0);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &local_E4, 0);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &local_D8, 0);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &local_CC, 0);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &local_C0, 0);
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &local_B4, 0);
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_0E81(int32_t arg0) {
  while (Script_shenSeqUtils(-1)) {
    Script_charaPosRotUtils(arg0, 163, 0, &data_22F0, 1);
    Script_charaPosRotUtils(arg0, 173, 0, &data_22F0, 1);
    Script_charaPosRotUtils(arg0, 182, 0, &data_22FC, 1);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_0ED7(void) {
  SCNF_SetCashValue(-160, 1);
  SCNF_WaitFrames_0000(200);
  return Script_srchControl(0);
}

static void script_0EF4(void) {
  Vector3 local_F4;
  float local_F0;
  float local_EC;
  float local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  float local_D8;
  float local_D4;
  Script_EVT_GetPosition(SHEN_NAME('BUS_'), -1, &local_F4, 0);
  local_F0 = (&local_F4)->x - 4.0f;
  local_EC = (&local_F4)->z - 4.0f;
  local_E8 = (&local_F4)->x + 4.0f;
  local_E4 = (&local_F4)->z - 4.0f;
  local_E0 = (&local_F4)->x + 4.0f;
  local_DC = (&local_F4)->z + 4.0f;
  local_D8 = (&local_F4)->x - 4.0f;
  local_D4 = (&local_F4)->z + 4.0f;
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, local_F0, local_EC, local_E8, local_E4, local_E0, local_DC, local_D8, local_D4, 0.0f, 3.5f);
  return;
}

void script_0F98(void) {
  Script_scenCreateTask(script_0FA6, 0);
  return;
}

__attribute__((scn_string_bindings(2, data_231D, 6, data_2345)))
static int32_t script_0FA6(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  GameDateTime local_EC;
  int32_t local_E8;
  local_FC = Script_GetCurrentEntryPoint();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ReturnTimeDate(&local_EC);
  (&local_EC)->hours = 10;
  (&local_EC)->minutes = 0;
  Script_SetNewDateTime(&local_EC);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(60);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MSBS/", "MSBS");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  while (1) {
    local_E8 = local_FC;
    if (local_E8 == 0) {
      script_1226();
      Script_LoadVoiceFile(1, "01MSO");
      Script_LoadSND(0, 2, "bgm103.snd", 0);
      Script_LoadSND(0, 4, "a1_mso.snd", 0);
      Script_executeAicaCommand(16296, 0, 0);
      Script_executeAicaCommand(1184, 2, 100);
      local_F8 = Script_motiLoad(3, SHEN_NAME('MSBS'), "M_01MSO.BIN");
      script_039D();
      Script_ReallocateUNFO(local_F8);
      Script_LoadVoiceFile(0, "01MSO");
      Script_charaDisplay(CHARA_AKIR, CDM_Hide);
      script_12CF();
      local_F8 = Script_motiLoad(3, SHEN_NAME('MSBS'), "M_01BUS.BIN");
      Script_LoadSND(0, 4, "a1_busno.snd", 0);
      Script_scenCreateTask(script_0125, 0);
      script_0992(1);
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenSeqUtils(-4);
      Script_ReallocateUNFO(local_F8);
      script_0B84();
      Script_executeAicaCommand(0x1200A0, 2, 0);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 935, 1);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 0, 1, 1, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 1, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 1, 1, 0);
      Script_WeatherTaskUtil(ToggleFogTask, 1);
      Script_WeatherTaskUtil(ToggleCloudXYFlags, 1);
      Script_WeatherTaskUtil(ToggleSceneLightingFlags, 1);
      SCNF_WaitFrames_0000(3);
      Script_WeatherTaskUtil(LoadSPR);
      Script_WeatherUtils_ForFIXC(13, 1);
      Script_WeatherUtils_ForFIXC(14, 1);
      Script_shenRequestGameModuleTransition(3, SHEN_NAME('MFSY'), 4);
      SCNF_WaitFrames_0000(1);
      goto L0011FB;
    }
    L0011FB:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1207(int32_t arg0, const Vector3 *arg1, const Vector3i *arg2) {
  Script_EVT_SetPosition(arg0, POS_XYZ, arg1);
  Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
  return;
}

static void script_1226(void) {
  int32_t unused_E8[6];
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 1);
  Script_WeatherTaskUtil(InitTaskData);
  SCNF_WaitFrames_0000(1);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('AKI2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('AKI2'), CDM_Show);
      goto L001271;
    }
  }
  L001271:;
  Script_scenToggleVisibleFor(SHEN_NAME('AKI2'), 0);
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetCamFlags(0, 0);
  return;
}

static void script_12CF(void) {
  if (0) {
    Script_charaDisplay(SHEN_NAME('AKI2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('AKI2'), CDM_Show);
      goto L0012F9;
    }
  }
  L0012F9:;
  Script_scenToggleVisibleFor(SHEN_NAME('AKI2'), 1);
  if (-1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L00132D;
    }
  }
  L00132D:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 0);
  Script_mapmSetCamFlags(0, 1);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 1);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 1);
  Script_mapmSetRenderFlag(8, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_mapmSetRenderFlag(10, 1);
  Script_mapmSetRenderFlag(15, 1);
  Script_mapmSetRenderFlag(16, 1);
  Script_mapmSetRenderFlag(17, 1);
  Script_mapmSetRenderFlag(18, 1);
  Script_mapmSetRenderFlag(19, 1);
  Script_mapmSetRenderFlag(23, 1);
  return;
}
