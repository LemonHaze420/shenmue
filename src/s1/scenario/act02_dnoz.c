#include "act02_dnoz.h"

static int32_t script_0030(void);
static void script_0060(void);
static void script_0086(void);
static void script_008F(void);
static void script_009E(int32_t arg0);
static void script_012B(int32_t arg0);
static int32_t script_017B(void);
static void script_01A9(int32_t arg0);
static void script_01D7(void);
static void script_020B(int32_t arg0);
static void script_023F(int32_t arg0);
static void script_025D(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0297(int32_t arg0, int32_t arg1);
static void script_02CB(void);
static void script_03C3(int32_t arg0, int32_t arg1);
static void script_03F3(int32_t arg0);
static void script_042F(int32_t arg0);
static void script_04E4(int32_t arg0);
static void script_0625(void);
static void script_071A(void);
static void script_0780(int32_t arg0);
static void script_07BB(int32_t arg0);
static void script_07E5(int32_t arg0);
static void script_08C2(int32_t arg0);
static void script_0A2B(void);
static void script_0B36(void);
static void script_0B84(int32_t arg0);
static void script_0BBF(int32_t arg0);
void script_0BE9(void);
static int32_t script_0BFC(int32_t arg0);
static void script_1010(void);
static void script_10E7(void);

static const float data_11C0[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_11CC[4] = {16777215, 16777215, 16777215, 16777215};
static const uint32_t data_11DC = 0xC3D48000;
static const float data_11E0[2] = {-1.8f, -210.0f};
static const char data_11E8[] __attribute__((scn_string_literal)) = "scene/02/model";
static const char data_11F7[] __attribute__((scn_string_literal)) = "C1305S3G.MT5";
static const char data_1204[] __attribute__((scn_string_literal)) = "C1305S1G.MT5";
static const uint8_t pad_1211[5] = {0};
static const uint8_t data_1216[16] = {
  0xA0, 0x41, 0x00, 0x00, 0x48, 0x44, 0x00, 0x00, 0x80, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};
static const uint8_t pad_1226[234] = {0};
static const int32_t data_1310[340] = {
  -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
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
  -1638, 0, 0, 1092
};
static const int32_t data_1860[342] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366,
  -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286,
  0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0, -2366, -728, 0, -364,
  -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735,
  0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364, -5279, 2548, -910, 0,
  -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182,
  -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189, 2912, 0, 0, -546,
  -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0, -16201, 0, 364,
  -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0,
  0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0,
  -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_1DB8[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -364, 1092, 0, -546, -6735, 0, 364, -4915, 0, 0, -3640, 0, -182, 1274, 0, 0, -6189, 0, -364, -2730, 0, 0, -5461, 0,
  -364, 2002, 0, 182, -8738, 0, -728, -2002, 0, -182, -6007, 0, -546, 2366, 0, -728, -5279, 0, -1274, -1820, 0, -1638, -5279, 0, 182, 2912
};
static const Vector4 data_1E9C = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const Vector4 data_1EAC = {.x = 1.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const Vector4 data_1EBC = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_1ECC = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_1EDC[6] __attribute__((scn_string_literal)) = "01SKI";
static const char data_1EE2[13] __attribute__((scn_string_literal)) = "a1_nozok.snd";
static const char data_1EEF[11] __attribute__((scn_string_literal)) = "bgm068.snd";
static const char data_1EFA[13] __attribute__((scn_string_literal)) = "SEQDATA4.BIN";
static const char data_1F07[14] __attribute__((scn_string_literal)) = "SCENE/02/DNOZ";
static const char data_1F15[13] __attribute__((scn_string_literal)) = "SEQDATA5.BIN";
static const char data_1F22[14] __attribute__((scn_string_literal)) = "SCENE/02/DNOZ";
static const char data_1F30[12] __attribute__((scn_string_literal)) = "M_01SKI.BIN";
static const char data_1F3C[6] __attribute__((scn_string_literal)) = "01SKI";
static const char data_1F42[7] __attribute__((scn_string_literal)) = "01FULB";
static const char data_1F49[12] __attribute__((scn_string_literal)) = "M_01FUL.BIN";
static const char data_1F55[13] __attribute__((scn_string_literal)) = "SEQDATA2.BIN";
static const char data_1F62[14] __attribute__((scn_string_literal)) = "SCENE/02/DNOZ";
static const char data_1F70[13] __attribute__((scn_string_literal)) = "SEQDATA3.BIN";
static const char data_1F7D[14] __attribute__((scn_string_literal)) = "SCENE/02/DNOZ";
static const char data_1F8B[13] __attribute__((scn_string_literal)) = "a1_nonam.snd";
static const char data_1F98[11] __attribute__((scn_string_literal)) = "bgm068.snd";
static const char data_1FA3[9] __attribute__((scn_string_literal)) = "01FULB";
static const FogColourData data_1FAC = {.red = 16.7f, .green = 780.0f, .blue = 1.0f, .colour = 0xFF0D0705U};
static const uint32_t data_1FBC[4] = {0x320E102EU, 0x320E102EU, 0x64555462U, 0x64555462U};
static const Vector4 data_1FCC = {.x = 0.25f, .y = 0.09f, .z = 0.19f, .w = 0.31f};
static const Vector4 data_1FDC = {.x = 1.0f, .y = 0.8f, .z = 0.63f, .w = 0.4f};
static const float data_1FEC[2] = {0.69f, 1.0f};
static const Vector4 data_1FF4 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_2004 = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_2014[] __attribute__((scn_string_literal)) = "sprite";
static const char data_201B[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const FogColourData data_2028 = {.red = 16.7f, .green = 780.0f, .blue = 1.0f, .colour = 0xFF0D0705U};
static const uint32_t data_2038[4] = {0x320E102EU, 0x320E102EU, 0x64555462U, 0x64555462U};
static const Vector4 data_2048 = {.x = 0.25f, .y = 0.09f, .z = 0.19f, .w = 0.31f};
static const Vector4 data_2058 = {.x = 1.0f, .y = 0.8f, .z = 0.63f, .w = 0.4f};
static const float data_2068[2] = {0.69f, 1.0f};
static const Vector4 data_2070 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_2080 = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_2090[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2097[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";

static int32_t script_0030(void) {
  for (;;) {
    Script_ClearAccumulator_00FE(1, 0x3F000000, 10, 2, 2);
    Script_ClearAccumulator_00FE(3, &data_11DC, 8, 9);
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
  Script_ccowUtils(arg0, 2);
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

static void script_012B(int32_t arg0) {
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
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_03F3(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_0086();
  script_0625();
  local_FC = 0;
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_042F(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_0086();
  script_0625();
  Script_shenSeqUtils(arg0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_FACE(CHARA_HRSK, 0, 0, 32);
        break;
      case 170:
        Script_executeAicaCommand(19112, 0, 0);
        Script_executeAicaCommand(1184, 2, 100);
        break;
      case 331:
        Script_FACE(CHARA_HRSK, 3, 0, 32);
        break;
      case 829:
        Script_FACE(CHARA_HRSK, 0, 0, 32);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_04E4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_shenSeqUtils(-7) == 0) {
    local_FC = 0;
    script_07BB(CHARA_AKIR);
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      local_F8 = local_FC;
      switch (local_F8) {
        case 0:
          Script_FACE(CHARA_HRSK, 3, 0, 32);
          break;
        case 710:
          Script_FACE(CHARA_HRSK, 0, 0, 28);
          break;
        case 825:
          Script_FACE(CHARA_HRSK, 3, 0, 32);
          break;
        case 1871:
          Script_FACE(CHARA_HRSK, 7, 0, 19);
          break;
        case 1900:
          script_0780(CHARA_AKIR);
          break;
        case 2247:
          Script_FACE(CHARA_HRSK, 0, 0, 32);
          break;
        case 2780:
          Script_handSetFloatsFor(CHARA_HRSK, 1, data_1DB8, 16);
          break;
        case 2801:
          Script_FACE(CHARA_HRSK, 3, 0, 19);
          break;
        case 2979:
          goto L000624;
          break;
      }
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  L000624:;
  return;
}

static void script_0625(void) {
  script_009E(CHARA_AKIR);
  script_009E(CHARA_HRSK);
  Script_SetMaxInterpSpeed(0.4f);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1E9C);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_1EAC);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_HRSK, 1, 0);
  script_0780(CHARA_AKIR);
  script_0780(CHARA_HRSK);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_HRSK, -1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_HRSK, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  return;
}

static void script_071A(void) {
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_HRSK, 0);
  script_07BB(CHARA_AKIR);
  script_07BB(CHARA_HRSK);
  script_012B(CHARA_AKIR);
  script_012B(CHARA_HRSK);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  return;
}

static void script_0780(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_1860, 8);
  Script_handSetFloatsFor(arg0, 0, data_1860, 8);
  return;
}

static void script_07BB(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_07E5(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F0[3];
  int32_t local_EC;
  Script_executeAicaCommand(19112, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_0086();
  script_0A2B();
  local_FC = 0;
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_FC;
    switch (local_EC) {
      case 0:
        Script_FACE(CHARA_HRSK, 3, 0, 16);
        break;
      case 400:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
        break;
      case 950:
        Script_FACE(CHARA_HRSK, 0, 0, 16);
        break;
      case 1472:
        Script_FACE(CHARA_HRSK, 2, 0, 16);
        break;
      case 1560:
        Script_FACE(CHARA_HRSK, 3, 0, 16);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_08C2(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  if (Script_shenSeqUtils(-7) == 0) {
    script_0086();
    script_0A2B();
    local_FC = 0;
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      local_EC = local_FC;
      switch (local_EC) {
        case 0:
          Script_FACE(CHARA_HRSK, 3, 0, 16);
          break;
        case 143:
          script_07BB(CHARA_AKIR);
          script_07BB(CHARA_HRSK);
          break;
        case 570:
          Script_scenToggleActiveFor(CHARA_AKIR, 0);
          Script_scenToggleActiveFor(CHARA_HRSK, 0);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
          break;
        case 1000:
          Script_FACE_LCG(CHARA_HRSK, 1, 16, 0);
          break;
        case 1080:
          Script_FACE(CHARA_HRSK, 0, 0, 16);
          break;
        case 1162:
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
          break;
        case 1500:
          goto L000A1A;
          break;
      }
      if ((local_FC >= 896) & (local_FC <= 1161)) {
        Script_EVT_GetPosition(CHARA_HRSK, 18, &local_F0, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F0, 8);
      }
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
    }
    L000A1A:;
    Script_FACE_LCG(CHARA_HRSK, 0, 0, 0);
  }
  return;
}

static void script_0A2B(void) {
  script_009E(CHARA_AKIR);
  script_009E(CHARA_HRSK);
  Script_SetMaxInterpSpeed(0.4f);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1EBC);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_1ECC);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_HRSK, 1, 0);
  script_0780(CHARA_AKIR);
  script_0780(CHARA_HRSK);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_HRSK, -1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_HRSK, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
  return;
}

static void script_0B36(void) {
  script_0BBF(CHARA_AKIR);
  Script_SetCharaFlags(CHARA_HRSK, 0);
  script_012B(CHARA_AKIR);
  script_012B(CHARA_HRSK);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  return;
}

static void script_0B84(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_1860, 8);
  Script_handSetFloatsFor(arg0, 0, data_1860, 8);
  return;
}

static void script_0BBF(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

void script_0BE9(void) {
  Script_scenCreateTask(script_0BFC, 1, 0x52494B41);
  return;
}

__attribute__((scn_string_bindings(0, data_1EDC, 2, data_1EEF, 4, data_1F07, 6, data_1F22, 8, data_1F3C, 9, data_1F42, 12, data_1F62, 14, data_1F7D, 16, data_1F98, 17, data_1FA3)))
static int32_t script_0BFC(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  local_FC = Script_GetCurrentEntryPoint();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 1);
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(8, 0);
    Script_mapmSetRenderFlag(9, 1);
    Script_mapmSetRenderFlag(10, 0);
    Script_mapmSetRenderFlag(11, 1);
    Script_mapmSetRenderFlag(12, 1);
  } else {
    Script_mapmSetRenderFlag(8, 1);
    Script_mapmSetRenderFlag(9, 0);
    Script_mapmSetRenderFlag(10, 1);
    Script_mapmSetRenderFlag(11, 0);
    Script_mapmSetRenderFlag(12, 0);
  }
  Script_WeatherUtils_ForFIXC(13, 0);
  Script_WeatherUtils_ForFIXC(14, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 1, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleCloudXYFlags, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(60);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (1) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 0:
        script_1010();
        Script_LoadVoiceFile(1, "01SKI");
        Script_LoadSND(0, 4, "a1_nozok.snd", 0);
        Script_LoadSND(0, 2, "bgm068.snd", 0);
        Script_AuthorUtils(0, 0, "SEQDATA4.BIN", "SCENE/02/DNOZ");
        Script_AuthorUtils(0, 1, "SEQDATA5.BIN", "SCENE/02/DNOZ");
        local_F8 = Script_motiLoad(2, SHEN_NAME('DNOZ'), "M_01SKI.BIN");
        Script_scenCreateTask(script_017B, 0);
        script_042F(0);
        script_04E4(1);
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        script_071A();
        Script_LoadVoiceFile(0, "01SKI");
        SCNF_WaitFrames_0000(60);
        Script_AuthorUtils(AUTH_FREE, 0);
        Script_AuthorUtils(AUTH_FREE, 1);
        Script_ReallocateUNFO(local_F8);
        Script_SetGameStateTo(AUTH_None);
        break;
      case 1:
        script_10E7();
        Script_LoadVoiceFile(1, "01FULB");
        local_F8 = Script_motiLoad(2, SHEN_NAME('DNOZ'), "M_01FUL.BIN");
        Script_AuthorUtils(0, 0, "SEQDATA2.BIN", "SCENE/02/DNOZ");
        Script_AuthorUtils(0, 1, "SEQDATA3.BIN", "SCENE/02/DNOZ");
        Script_LoadSND(0, 4, "a1_nonam.snd", 0);
        Script_LoadSND(0, 2, "bgm068.snd", 0);
        Script_scenCreateTask(script_017B, 0);
        script_07E5(0);
        script_08C2(1);
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        script_0B36();
        Script_LoadVoiceFile(0, "01FULB");
        SCNF_WaitFrames_0000(30);
        Script_AuthorUtils(AUTH_FREE, 0);
        Script_AuthorUtils(AUTH_FREE, 1);
        Script_ReallocateUNFO(local_F8);
        Script_SetGameStateTo(AUTH_None);
        break;
    }
    Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 1, 0);
    Script_WeatherTaskUtil(ToggleFogTask, 1);
    Script_WeatherTaskUtil(ToggleCloudXYFlags, 1);
    Script_WeatherTaskUtil(ToggleSceneLightingFlags, 1);
    SCNF_WaitFrames_0000(3);
    Script_WeatherTaskUtil(LoadSPR);
    Script_WeatherUtils_ForFIXC(13, 1);
    Script_WeatherUtils_ForFIXC(14, 1);
    if (local_FC == 0) {
      Script_ReturnTimeDate(&local_E8);
      (&local_E8)->hours = 23;
      (&local_E8)->minutes = 30;
      (&local_E8)->seconds = 0;
      Script_SetNewDateTime(&local_E8);
      Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
      SCNF_WaitFrames_0000(1);
    }
    Script_executeAicaCommand(0x1200A0, 2, 0);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_2014, 1, data_201B)))
static void script_1010(void) {
  Vector3 local_F4;
  (void)1;
  Script_SetVector4ByOffset(&data_1FF4);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_2004);
  Script_SetFogColour(&data_1FAC);
  Script_SetFogFlags(0, data_1FBC);
  Script_shenSetLightingMode(2);
  Script_lightSetGlobalSceneLighting(&data_1FCC);
  local_F4.x = 20.61f;
  (&local_F4)->y = 3.39f;
  (&local_F4)->z = -4.08f;
  Script_lightEnablePointLightAt(1, &local_F4);
  Script_LightUtils(1, 2, &data_1FDC);
  Script_LightUtils(1, 4, 1.0f);
  Script_LightUtils(1, 5, 3.69f);
  Script_LightUtils(1, 6, data_1FEC);
  Script_lightSetSlotEnabled(1, 1);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL27.SPR");
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
  return;
}

__attribute__((scn_string_bindings(0, data_2090, 1, data_2097)))
static void script_10E7(void) {
  Vector3 local_F4;
  (void)1;
  Script_SetVector4ByOffset(&data_2070);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_2080);
  Script_SetFogColour(&data_2028);
  Script_SetFogFlags(0, data_2038);
  Script_shenSetLightingMode(2);
  Script_lightSetGlobalSceneLighting(&data_2048);
  local_F4.x = 20.61f;
  (&local_F4)->y = 3.39f;
  (&local_F4)->z = -4.08f;
  Script_lightEnablePointLightAt(1, &local_F4);
  Script_LightUtils(1, 2, &data_2058);
  Script_LightUtils(1, 4, 1.0f);
  Script_LightUtils(1, 5, 3.69f);
  Script_LightUtils(1, 6, data_2068);
  Script_lightSetSlotEnabled(1, 1);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL27.SPR");
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
  return;
}
