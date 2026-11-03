#include "act03_nbik.h"

static void script_0030(void);
static void script_0039(void);
static void script_0048(int32_t arg0);
static void script_00D5(int32_t arg0);
static void script_0125(void);
static void script_0153(int32_t arg0);
static void script_0181(void);
static void script_01B5(int32_t arg0);
static void script_01E9(int32_t arg0);
static void script_0207(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0241(int32_t arg0, int32_t arg1);
static void script_0275(void);
static void script_036D(int32_t arg0, int32_t arg1);
static int32_t script_039D(void);
static int32_t script_03B8(void);
static int32_t script_03CD(void);
static int32_t script_03E3(void);
static int32_t script_03F8(void);
static int32_t script_040D(void);
static int32_t script_0422(void);
static int32_t script_0438(void);
static int32_t script_044D(void);
static int32_t script_0462(void);
static int32_t script_0477(void);
static int32_t script_048C(void);
static void script_049B(void);
static void script_0623(void);
static void script_0E36(void *arg0, void *arg1);
static int32_t script_0EBA(void);
static int32_t script_1029(void);
static void script_1201(void);
static void script_1382(void);
static void script_142C(int32_t arg0);
static void script_14C8(int32_t arg0);
static void script_14F1(int32_t arg0);
static void script_1514(int32_t arg0);
static int32_t script_155A(void);
static int32_t script_15B0(void);
void script_15E4(void);
static int32_t script_15F7(int32_t arg0);

static int32_t task_0;
static const float data_17A0[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_17AC[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_17BC[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_17C8[229] = {
  -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0,
  0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650,
  -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546,
  0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364,
  -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0,
  -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276
};
static const int32_t data_1B5C[513] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738,
  0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0,
  -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0,
  182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0,
  0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279,
  0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0,
  0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0,
  -910, -10922, -728, 546, 1638, 0, 0, -1820, 0, -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0,
  -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910,
  -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364, -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0,
  0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009,
  0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189, 2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0,
  -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0, -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0,
  0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274,
  0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_2360[57] = {
  0, 0, -6189, 0, 3640, -4187, -2002, 546, 546, 0, -1820, -12014, 0, 546, -17840, 0, 182, -12743, 0, 0, -1274, 0, 546, -11286, 0, 0, -14017, 0, 0,
  -15837, 0, 0, -1092, 0, 728, -9830, 0, 0, -14199, 0, 0, -14563, 0, 0, -1092, 0, -1638, -8009, 0, 0, -12014, -182, -3458, -14563, 0, -728, -1274
};
static const int32_t data_2444[57] = {
  0, 0, -8920, 0, 5643, -6371, 0, 1274, 1456, 0, -1820, -11832, 0, 0, -16384, 0, 364, -14199, 0, -182, -364, 0, 546, -7827, 0, 0, -13471, 0, 0,
  -18204, 0, 0, -182, 0, 728, -8738, 0, 0, -14199, 0, 0, -17112, 0, 0, -546, 0, -364, -8009, 0, 0, -12561, 0, -3458, -15109, 0, -364, -364
};
static const char data_2528[67] __attribute__((scn_string_literal)) = "●青Like a dream I once saw,＆it passes by as it touches my cheek.";
static const char data_256B[64] __attribute__((scn_string_literal)) = "●青This street leading to the ocean＆carries the smell of sea.";
static const char data_25AB[43] __attribute__((scn_string_literal)) = "●青Before love makes me tell a white lie.";
static const char data_25D6[64] __attribute__((scn_string_literal)) = "●青This loneliness...＆I want you to hold it in your arms now.";
static const char data_2616[38] __attribute__((scn_string_literal)) = "●青If you will always be by my side,";
static const char data_263C[47] __attribute__((scn_string_literal)) = "●青there'll be no sorrow or tears in my eyes.";
static const char data_266B[57] __attribute__((scn_string_literal)) = "●青Can't live without you.＆Why did you have to decide?";
static const char data_26A4[45] __attribute__((scn_string_literal)) = "●青You won't be aware of my pains and lies.";
static const char data_26D1[40] __attribute__((scn_string_literal)) = "●青Don't leave me alone, stay with me.";
static const char data_26F9[43] __attribute__((scn_string_literal)) = "●青I don't know if you'll ever look back.";
static const char data_2724[32] __attribute__((scn_string_literal)) = "●青I'll remember you forever.";
static const int32_t data_2744[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_276C[10] = {64625, 4915, 0, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_2794[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64625, 4915, 0, 0, 0, 0, 0, 0, 0, 0};
static const Vector4 data_27E4 = {.x = 0.1f, .y = 0.1f, .z = 0.1f, .w = 0.1f};
static const Vector4 data_27F4 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const FogColourData data_2804 = {.red = 10.0f, .green = 200.0f, .blue = 0.75f, .colour = 0xFF000612U};
static const uint32_t data_2814[4] = {0, 0, 0, 0};
static const char data_2824[] __attribute__((scn_string_literal)) = "sprite";
static const char data_282B[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const char data_2838[8] __attribute__((scn_string_literal)) = "01BIKEB";
static const char data_2840[13] __attribute__((scn_string_literal)) = "a1_nbike.snd";
static const char data_284D[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2854[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_2861[13] __attribute__((scn_string_literal)) = "M_01BIKE.BIN";
static const char data_286E[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2875[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_2882[13] __attribute__((scn_string_literal)) = "M_01BIKE.BIN";
static const char data_288F[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2896[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_28A3[13] __attribute__((scn_string_literal)) = "M_01BIKE.BIN";

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
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
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
  Script_FACE_LCG(CHARA_HRSK, 0, 0, 0);
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
  Script_FACE_LCG(CHARA_HRSK, 0, 0, 0);
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
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_1B5C, 10);
  Script_handSetFloatsFor(arg0, 1, data_1B5C, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_27F4);
  return;
}

static void script_14C8(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_14F1(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 1);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  return;
}

static void script_1514(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, 0);
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
    Script_charaPosRotUtils(SHEN_NAME('BIKE'), 23, 0, &local_F4, 1);
    Script_charaPosRotUtils(SHEN_NAME('BIKE'), 18, 0, &local_F4, 1);
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
