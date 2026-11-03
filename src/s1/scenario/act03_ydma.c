#include "act03_ydma.h"

static int32_t script_0030(void);
static void script_0091(void);
static void script_009A(void);
static void script_00A9(int32_t arg0);
static void script_0136(int32_t arg0);
static int32_t script_0186(void);
static void script_01B4(int32_t arg0);
static void script_01E2(void);
static void script_0216(int32_t arg0);
static void script_024A(int32_t arg0);
static void script_0268(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_02A2(int32_t arg0, int32_t arg1);
static void script_02D6(void);
static void script_03CE(int32_t arg0, int32_t arg1);
static void script_03FE(int32_t arg0);
static void script_0609(void);
static void script_06A3(void);
static void script_06ED(int32_t arg0, int32_t arg1);
static void script_0751(void);
static void script_0EAB(void);
static void script_0F57(void);
static int32_t script_0F9A(void);
void script_102C(void);
static int32_t script_103A(void);
static void script_12D3(void);
static void script_13AB(void);
static void script_14CB(void);
static void script_15AF(void);

static const float data_19B4[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_19C0[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_19D0[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_19DC[742] = {
  -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0,
  0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650,
  -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546,
  0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364,
  -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0,
  -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0, -5461, 0, -2366, -1092, 0,
  -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007,
  0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546,
  -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0,
  182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0,
  0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640,
  0, -910, -2730, 0, -546, -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0,
  0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910,
  -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468,
  0, 0, -13653, 0, 0, 1456, 0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546,
  1638, 0, 0, -1820, 0, -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0,
  -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0,
  -5461, -2912, -2366, -364, -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0,
  -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556,
  0, -2366, -1092, -6189, 2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182,
  728, 0, 182, -13835, 0, 0, -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0,
  -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182,
  -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_2574[57] = {
  0, -546, -3458, 0, 1638, -2548, 0, 5825, -546, 0, 728, -10194, 0, 0, -11832, 0, 0, -15837, 0, 364, -3458, 0, 0, -8920, 0, 0, -15109, 0, -182,
  -14745, 0, 0, -2912, 0, 0, -13835, 0, 0, -11286, 0, 0, -14927, 0, 0, -2002, 0, 0, -12379, 0, 0, -8556, 0, -1820, -12925, -2002, -546, -1456
};
static const int32_t data_2658[114] = {
  -1820, 728, -6189, -2002, 1274, -13471, -618, 6371, 819, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0,
  0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0,
  -364, -11650, -2894, -1092, -11086, 1620, 2910, 0, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0,
  -18750, 0, 0, -546, 0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728
};
static const int32_t data_2820[57] = {
  -2912, -364, -7281, -3094, 1638, -14563, -1711, 6735, -455, 0, 364, -18932, 0, 182, -25668, 0, 546, -15291, 0, 364, -1820, 0, 0, -16748, 0, 0,
  -24393, 0, 1274, -16566, 0, 182, -2366, 0, 0, -23847, 0, 546, -22209, 0, 1820, -17840, 0, 546, -2912, 0, 910, -21481, 0, 728, -17840, 0, -4004,
  -17476, 0, -364, -3440
};
static const int32_t data_2904[114] = {
  0, 0, -5461, 0, -2366, -1092, 0, 2548, 546, 0, -546, -3822, 0, 1274, -2366, 0, 0, -1456, 0, 6007, 546, 0, 0, -3822, 0, 0, -2730, 0, 0, -5279, 0,
  2184, -182, 0, 182, -3822, 0, 0, -2912, 0, -728, -3458, 0, 728, -546, 0, -728, -3640, 0, -910, -2730, 0, 0, -2730, 364, -546, -910, 0, -182, -10376,
  0, -1092, -4733, 0, 3094, -182, 0, 0, -18022, 0, 0, -17112, 0, 182, -11286, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -11468, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -10740, 0, 0, -1092, 0, 910, -8556, 0, -3640, -6735, 0, -2184, -12561, 0, 0, 0
};
static const Vector3i data_2ACC = {.x = 2366, .y = 1637, .z = -2182};
static const int32_t data_2AD8[6] = {-5097, 546, 0, 1456, 2366, -9102};
static const Vector4 data_2AF0 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector3 data_2B00 = {.x = 0.034f, .y = 0.355f, .z = -0.168f};
static const Vector3i data_2B0C = {.x = 12833, .y = 50432, .z = 38218};
static const int32_t data_2B18[4] = {0, 0, -9935758, -9935758};
static const int32_t data_2B28[57] = {
  0, -546, -15109, -4004, 3913, -5643, -1274, 6917, -728, 0, 728, -10922, 0, -728, -12925, 0, 0, -14745, 728, 182, -2912, 0, 0, -8920, 0, 0, -15109,
  0, -182, -14745, 0, 0, -2912, 0, 0, -13835, 0, 0, -14927, 0, 0, -12196, 0, 0, -2002, 0, 0, -12379, 0, 0, -14017, 0, -1820, -9284, -2002, -546, -1456
};
static const int32_t data_2C0C[57] = {
  0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640,
  0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0
};
static const int32_t data_2CF0[57] = {
  0, 0, -2730, 0, 0, -910, 0, 546, -182, 0, 0, -8465, 0, 0, -4605, 0, 0, -5552, 0, 0, -182, 0, 0, -7281, 0, 0, -7482, 0, 0, -6189, 0, 0, 0, 0, 0,
  -6262, 0, 0, -8701, 0, 0, -4096, 0, 54, -455, 0, -2548, -9502, 0, -1820, -7008, 0, -546, -1037, 0, -618, -1492
};
static const int32_t data_2DD4[57] = {
  0, 0, -3094, 0, 0, 546, 0, 546, -182, 0, 0, -8465, 0, 0, -4605, 0, 0, -5552, 0, 0, -182, 0, 0, -7281, 0, 0, -7482, 0, 0, -6189, 0, 0, 0, 0, 0,
  -6262, 0, 0, -8701, 0, 0, -4096, 0, 54, -455, 0, -2548, -9502, 0, -1820, -7008, 0, -546, -1037, 0, -618, -1492
};
static const int32_t data_2EB8[57] = {
  0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0,
  1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const Vector4 data_2F9C = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_2FAC[16] __attribute__((scn_string_literal)) = "/scene/03/YDMA/";
static const char data_2FBC[5] __attribute__((scn_string_literal)) = "YDMA";
static const char data_2FC1[11] __attribute__((scn_string_literal)) = "bgm107.snd";
static const char data_2FCC[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2FD1[13] __attribute__((scn_string_literal)) = "a1_kstab.snd";
static const char data_2FDE[13] __attribute__((scn_string_literal)) = "a1_tabid.snd";
static const char data_2FEB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2FF0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2FF5[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_2FFB[11] __attribute__((scn_string_literal)) = "M_0180.BIN";
static const char data_3006[13] __attribute__((scn_string_literal)) = "HMOT0156.BIN";
static const char data_3013[12] __attribute__((scn_string_literal)) = "M_01152.BIN";
static const char data_301F[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_3025[11] __attribute__((scn_string_literal)) = "bgm071.snd";
static const char data_3030[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3035[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_303A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_303F[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3044[24] = "FREE\000\000\000\000\315\314\314=\000\000\264C\205\353\221?\033\031\027\377";
static const Vector3 data_305C = {.x = 0.01032f, .y = 0.04001f, .z = 0.02149f};
static const Vector3 data_3068 = {.x = -0.98f, .y = 0.29f, .z = 10.8789f};
static const Vector3i data_3074 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_3080 = {.x = -63.379681f, .y = 7.58001f, .z = 105.061493f};
static const Vector3 data_308C = {.x = -53.491001f, .y = 7.822f, .z = 100.286003f};
static const Vector3 data_3098 = {.x = 0.01032f, .y = -0.05999f, .z = 0.02149f};
static const Vector3i data_30A4 = {.x = 0, .y = 21955, .z = 0};
static const char data_30B0[8] __attribute__((scn_string_literal)) = "01152.c";
static const char data_30B8[3] __attribute__((scn_string_literal)) = ".c";
static const char data_30BB[4] __attribute__((scn_string_literal)) = "ALL";
static const char data_30BF[12] __attribute__((scn_string_literal)) = "M_01152.BIN";
static const char data_30CB[11] __attribute__((scn_string_literal)) = "bgm107.snd";
static const char data_30D6[13] __attribute__((scn_string_literal)) = "a1_tabid.snd";
static const char data_30E3[11] __attribute__((scn_string_literal)) = "tsm003.snd";
static const char data_30EE[13] __attribute__((scn_string_literal)) = "a1_kstab.snd";
static const char data_30FB[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_3101[11] __attribute__((scn_string_literal)) = "M_0180.BIN";
static const char data_310C[13] __attribute__((scn_string_literal)) = "HMOT0156.BIN";
static const char data_3119[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_311F[11] __attribute__((scn_string_literal)) = "tsm003.snd";
static const char data_312A[13] __attribute__((scn_string_literal)) = "a1_kstab.snd";
static const char data_3137[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_313D[11] __attribute__((scn_string_literal)) = "M_0180.BIN";
static const char data_3148[13] __attribute__((scn_string_literal)) = "HMOT0156.BIN";
static const char data_3155[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_315B[12] __attribute__((scn_string_literal)) = "M_01152.BIN";
static const char data_3167[11] __attribute__((scn_string_literal)) = "bgm107.snd";
static const char data_3172[14] __attribute__((scn_string_literal)) = "a1_tabid.snd";

static int32_t script_0030(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = Script_GetInputData(0, 0) & 0xF7FF;
    if (local_FC & 0x400) {
      if (Script_ProcessDialogueData(2) == 0) {
        Script_ProcessDialogueData(0);
      }
      local_FC = local_FC & 0xFBFF;
    } else {
      if (Script_ProcessDialogueData(2)) {
        Script_ProcessDialogueData(1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0091(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_009A(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_00A9(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L0000D5;
    }
  }
  L0000D5:;
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

static void script_0136(int32_t arg0) {
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

static int32_t script_0186(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_01B4(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01E2(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0216(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_024A(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0268(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_02A2(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_02D6(void) {
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

static void script_03CE(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_03FE(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  int32_t unused_DC;
  int32_t local_D8;
  script_0091();
  script_0609();
  script_06ED(CHARA_AKIR, SHEN_NAME('ABA1'));
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_2ACC);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2574, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2904, 0);
  local_EC.x = -0.216f;
  (&local_EC)->y = 0.339f;
  (&local_EC)->z = -0.072f;
  local_E0.x = 3183;
  (&local_E0)->y = 53887;
  (&local_E0)->z = 48590;
  Script_fixoAttachEx(SHEN_NAME('ABA1'), CHARA_AKIR, 18, &local_EC, &local_E0);
  Script_shenSeqUtils(arg0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_D8 = local_FC;
    switch (local_D8) {
      case 0:
        Script_FACE(CHARA_AKIR, 3, 0, 1);
        local_E0.x = 3458;
        (&local_E0)->y = 5279;
        (&local_E0)->z = 0;
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E0);
        break;
      case 135:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2820, 30);
        break;
      case 152:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2820, 30);
        break;
      case 230:
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_FACE_LCG(CHARA_AKIR, 2, 6, 0);
        break;
      case 433:
        Script_FACE(CHARA_AKIR, 2, 0, 20);
        break;
      case 730:
        goto L0005F5;
        break;
    }
    if ((local_FC > 150) & (local_FC < 213)) {
      if (local_FC & 1) {
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2658, 20);
      } else {
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2820, 20);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L0005F5:;
  Script_fixoClearAttachment(SHEN_NAME('ABA1'));
  Script_SetRyoMotionDetails(CHARA_AKIR);
  return;
}

static void script_0609(void) {
  script_00A9(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_mapmSetCamFlags(0, 1);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_SetVector4ByOffset(&data_2AF0);
  (void)1;
  return;
}

static void script_06A3(void) {
  script_0136(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_SetGameStateTo(AUTH_None);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_mapmSetCamFlags(0, 1);
  return;
}

static void script_06ED(int32_t arg0, int32_t arg1) {
  Script_ccowUtils(arg1, 2);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_RequestFightAnimation(arg0, 657, 0.0f, 14, 1);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, 18, &data_2B00, &data_2B0C);
  Script_CharHandlers(arg0, 0, 0, 16);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_charaDisplay(arg1, CDM_Show);
  return;
}

static void script_0751(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  script_0091();
  script_0EAB();
  Script_ccowSetFlag80(SHEN_NAME('ABAG'), 1);
  Script_scenCreateTask(script_0186, 0);
  Script_shenSeqUtils(0);
  local_F4 = Script_scenCreateTask(script_0F9A, 0);
  local_FC = 0;
  local_F8 = 127;
  while (Script_shenSeqUtils(-1)) {
    local_D8 = local_FC;
    switch (local_D8) {
      case 0:
        Script_motmGetCharaFlags(CHARA_AKIR, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
        Script_mapmSetCamFlags(0, 0);
        Script_scenToggleActiveFor(SHEN_NAME('FUTO'), 0);
        Script_FACE(CHARA_AKIR, 10, 0, 1);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2C0C, 10);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2C0C, 10);
        local_DC.x = 546;
        (&local_DC)->y = 1638;
        (&local_DC)->z = -1274;
        Script_SetHandRootRotation(CHARA_FUKU, 0, 0x15, &local_DC);
        local_DC.x = 546;
        (&local_DC)->y = -1638;
        (&local_DC)->z = -1820;
        Script_SetHandRootRotation(CHARA_FUKU, 1, 0x15, &local_DC);
        Script_motmSetBlendFrames(CHARA_FUKU, MOTM_BLEND_FRAMES_10);
        Script_charaDisplay(CHARA_INE_, CDM_Hide);
        Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        Script_charaDisplay(CHARA_FUKU, CDM_Hide);
        break;
      case 3:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2B28, 0);
        local_DC.x = -2366;
        (&local_DC)->y = -7827;
        (&local_DC)->z = 364;
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_DC);
        Script_charaDisplay(SHEN_NAME('ABAG'), CDM_Show);
        break;
      case 150:
        Script_mapmSetCamFlags(0, 1);
        Script_charaDisplay(CHARA_AKIR, CDM_Show);
        Script_charaDisplay(CHARA_FUKU, CDM_Show);
        break;
      case 230:
        local_E8.x = 0.198f;
        (&local_E8)->y = 0.012f;
        (&local_E8)->z = -0.061f;
        local_DC.x = 47331;
        (&local_DC)->y = 15109;
        (&local_DC)->z = 63715;
        Script_fixoAttachEx(SHEN_NAME('FUTO'), CHARA_FUKU, 18, &local_E8, &local_DC);
        Script_charaDisplay(SHEN_NAME('FUTO'), CDM_Show);
        break;
      case 240:
        Script_FACE(CHARA_AKIR, 7, 0, 7);
        break;
      case 288:
        Script_FACE(CHARA_FUKU, 8, 5, 10);
        break;
      case 330:
        break;
      case 370:
        Script_FACE(CHARA_FUKU, 3, 5, 10);
        break;
      case 390:
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2DD4, 10);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2CF0, 10);
        break;
      case 542:
        local_DC.x = 546;
        (&local_DC)->y = 910;
        (&local_DC)->z = -182;
        Script_SetHandRootRotation(CHARA_FUKU, 0, 0x15, &local_DC);
        break;
      case 630:
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        break;
      case 649:
        Script_fixoClearAttachment(SHEN_NAME('FUTO'));
        local_E8.x = 0.148f;
        (&local_E8)->y = 0.067f;
        (&local_E8)->z = 0.08f;
        local_DC.x = 2733;
        (&local_DC)->y = 8648;
        (&local_DC)->z = 33226;
        Script_fixoAttachEx(SHEN_NAME('FUTO'), CHARA_AKIR, 12, &local_E8, &local_DC);
        Script_charaDisplay(SHEN_NAME('FUTO'), CDM_Show);
        Script_FACE(CHARA_AKIR, 3, 4, 9);
        Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
        Script_SetNewHandMotion(CHARA_AKIR, 0, 611, 0, 0, 0, 0, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        while (Script_GetHandControlBit3(CHARA_AKIR, 0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 700:
        Script_ResetHandMotion(CHARA_FUKU, 0, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 1, 0);
        break;
      case 780:
        local_DC.x = 0;
        (&local_DC)->y = 0;
        (&local_DC)->z = 0;
        Script_SetHandRootRotation(CHARA_FUKU, 1, 0x15, &local_DC);
        local_DC.x = 0;
        (&local_DC)->y = 0;
        (&local_DC)->z = 0;
        Script_SetHandRootRotation(CHARA_FUKU, 0, 0x15, &local_DC);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2C0C, 10);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2C0C, 10);
        break;
      case 1307:
        Script_mapmSetCamFlags(0, 0);
        break;
      case 1308:
        Script_charaDisplay(SHEN_NAME('ABAG'), CDM_Hide);
        break;
      case 1383:
        Script_charaDisplay(SHEN_NAME('ABAG'), CDM_Show);
        break;
      case 1384:
        Script_mapmSetCamFlags(0, 1);
        break;
      case 1590:
        Script_FACE(CHARA_AKIR, 2, 0, 10);
        break;
      case 1563:
        Script_mapmSetCamFlags(0, 0);
        break;
      case 1659:
        Script_mapmSetCamFlags(0, 1);
        break;
      case 1730:
        Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        break;
      case 1750:
        Script_charaDisplay(CHARA_INE_, CDM_Show);
        Script_ResetHandMotion(CHARA_INE_, 1, 0, 0);
        Script_ResetHandMotion(CHARA_INE_, 1, 1, 0);
        local_DC.x = 2002;
        (&local_DC)->y = 546;
        (&local_DC)->z = 182;
        Script_SetHandRootRotation(CHARA_INE_, 1, 0x15, &local_DC);
        local_DC.x = -1274;
        (&local_DC)->y = -546;
        (&local_DC)->z = 364;
        Script_SetHandRootRotation(CHARA_INE_, 0, 0x15, &local_DC);
        Script_handSetFloatsFor(CHARA_INE_, 0, data_2EB8, 10);
        Script_handSetFloatsFor(CHARA_INE_, 1, data_2EB8, 10);
        break;
      case 1775:
        Script_charaDisplay(SHEN_NAME('FUTO'), CDM_Hide);
        break;
      case 1877:
        Script_FACE_LCG(CHARA_INE_, 1, 15, 0);
        break;
      case 2135:
        Script_FACE_LCG(CHARA_INE_, 2, 15, 0);
        break;
      case 2185:
        Script_FACE_LCG(CHARA_INE_, 1, 2, 0);
        break;
      case 2187:
        Script_FACE_LCG(CHARA_INE_, 2, 2, 0);
        break;
      case 2268:
        goto L000E52;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L000E52:;
  Script_executeAicaCommand(2720, 2, 64);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_F4);
  SCNF_WaitFrames_0000(1);
  Script_fixoClearAttachment(SHEN_NAME('FUTO'));
  script_0F57();
  Script_shenSeqUtils(-6);
  return;
}

static void script_0EAB(void) {
  script_00A9(CHARA_AKIR);
  script_00A9(CHARA_FUKU);
  script_00A9(CHARA_INE_);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_FUKU, 1, 0);
  Script_facemotUtils(CHARA_INE_, 1, 0);
  Script_SetVector4ByOffset(&data_2F9C);
  (void)1;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2F9C);
  Script_charaSetRefboardFlagTo(CHARA_FUKU, 1);
  Script_SetCharaRefBoardPos(CHARA_FUKU, &data_2F9C);
  Script_charaSetRefboardFlagTo(CHARA_INE_, 1);
  Script_SetCharaRefBoardPos(CHARA_INE_, &data_2F9C);
  return;
}

static void script_0F57(void) {
  script_0136(CHARA_AKIR);
  script_0136(CHARA_FUKU);
  script_0136(CHARA_INE_);
  Script_SetGameStateTo(AUTH_None);
  Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  Script_charaDisplay(CHARA_INE_, CDM_Hide);
  return;
}

static int32_t script_0F9A(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (local_FC == 150) {
      Script_tmnmUtils(SHEN_NAME('ABAG'), 5659, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    }
    if (local_FC == 650) {
      Script_tmnmUtils(SHEN_NAME('ABAG'), 5660, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    }
    if (local_FC == 1411) {
      Script_tmnmUtils(SHEN_NAME('ABAG'), 5658, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

void script_102C(void) {
  Script_scenCreateTask(script_103A, 0);
  return;
}

__attribute__((scn_string_bindings(2, data_2FC1, 3, data_2FCC, 4, data_2FD1, 5, data_2FDE, 6, data_2FEB, 7, data_2FF0, 8, data_2FF5, 9, data_2FFB, 10, data_3006, 11, data_3013, 12, data_301F, 14, data_3030, 15, data_3035, 16, data_303A, 17, data_303F)))
static int32_t script_103A(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FC = Script_GetCurrentEntryPoint();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoSetChara(CHARA_AKIR);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/YDMA/", "YDMA");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  script_13AB();
  while (1) {
    if (local_FC == 99) {
    } else {
      if (Script_GetSceneID() == 99) {
        Script_scenCreateTask(script_0030, 0);
      }
      script_13AB();
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_LoadSND_Wrapper(0, 0, "bgm107.snd", "FREE", "a1_kstab.snd", "a1_tabid.snd", "FREE", "FREE");
      Script_LoadVoiceFile(1, "A0180");
      local_F4 = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_0180.BIN");
      local_F0 = Script_LoadHandMot(3, SHEN_NAME('YDMA'), "HMOT0156.BIN");
      local_F8 = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_01152.BIN");
      Script_executeAicaCommand(16296, 0, 0);
      Script_executeAicaCommand(1184, 2, 95);
      script_0751();
      Script_executeAicaCommand(0x1200A0, 2, 0);
      Script_LoadVoiceFile(0, "A0180");
      SCNF_WaitFrames_0000(1);
      script_14CB();
      SCNF_WaitFrames_0000(1);
      Script_LoadSND_Wrapper(0, 0, "bgm071.snd", "FREE", "FREE", "FREE", "FREE", data_3044);
      Script_executeAicaCommand(19880, 0, 0);
      Script_executeAicaCommand(1184, 2, 105);
      Script_scenCreateTask(script_0186, 0);
      script_03FE(1);
      Script_executeAicaCommand(2720, 2, 60);
      Script_filmCheckOrSetTransitionUtil(80, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenSeqUtils(-4);
      Script_executeAicaCommand(0x1200A0, 2, 0);
      SCNF_WaitFrames_0000(60);
      Script_ReallocateUNFO(local_F8);
      Script_ReallocateUNFO(local_F4);
      Script_FreeTaskData(local_F0);
      script_06A3();
      Script_SetGameStateTo(AUTH_None);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 937, 1);
      if (Script_GetSceneID() == 99) {
        Script_shenRequestGameModuleTransition(99, SHEN_NAME('YDMA'), 0);
        SCNF_WaitFrames_0000(1);
      } else {
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('DXMS'), 0);
        SCNF_WaitFrames_0000(1);
      }
    }
  }
  end:;
}

static void script_12D3(void) {
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(0, 1);
    Script_mapmSetRenderFlag(1, 1);
    Script_mapmSetRenderFlag(2, 0);
    Script_mapmSetRenderFlag(3, 1);
    Script_mapmSetRenderFlag(4, 0);
    Script_mapmSetRenderFlag(5, 1);
    Script_mapmSetRenderFlag(6, 1);
    Script_mapmSetRenderFlag(7, 1);
    Script_mapmSetRenderFlag(8, 1);
    Script_mapmSetRenderFlag(9, 0);
    Script_mapmSetRenderFlag(10, 1);
  } else {
    Script_mapmSetRenderFlag(0, 1);
    Script_mapmSetRenderFlag(1, 1);
    Script_mapmSetRenderFlag(2, 1);
    Script_mapmSetRenderFlag(3, 0);
    Script_mapmSetRenderFlag(4, 1);
    Script_mapmSetRenderFlag(5, 0);
    Script_mapmSetRenderFlag(6, 0);
    Script_mapmSetRenderFlag(7, 1);
    Script_mapmSetRenderFlag(8, 1);
    Script_mapmSetRenderFlag(9, 1);
    Script_mapmSetRenderFlag(10, 0);
  }
  return;
}

static void script_13AB(void) {
  Vector4 local_F0;
  script_12D3();
  Script_mapmSetCamFlags(0, 0);
  Script_SetDynamicMapTransform(0, &data_305C, 0);
  Script_SetDynamicMapTransform(1, &data_305C, 0);
  Script_SetDynamicMapTransform(2, &data_305C, 0);
  Script_SetDynamicMapTransform(3, &data_305C, 0);
  Script_SetDynamicMapTransform(4, &data_305C, 0);
  Script_SetDynamicMapTransform(5, &data_305C, 0);
  Script_SetDynamicMapTransform(6, &data_305C, 0);
  Script_SetDynamicMapTransform(7, &data_305C, 0);
  Script_SetDynamicMapTransform(8, &data_305C, 0);
  Script_SetDynamicMapTransform(9, &data_305C, 0);
  Script_SetDynamicMapTransform(10, &data_305C, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  local_F0.x = 1.0f;
  (&local_F0)->y = 0.2f;
  (&local_F0)->z = 0.2f;
  (&local_F0)->w = 0.2f;
  Script_SetVector4ByOffset(&local_F0);
  (void)1;
  Script_EVT_SetPosition(SHEN_NAME('RDR2'), POS_XYZ, &data_3068);
  Script_EVT_SetRotation(SHEN_NAME('RDR2'), 0x38000000, &data_3074, 0);
  return;
}

static void script_14CB(void) {
  Script_mapmSetCamFlags(0, 0);
  Script_SetDynamicMapTransform(0, &data_3098, 0);
  Script_SetDynamicMapTransform(1, &data_3098, 0);
  Script_SetDynamicMapTransform(2, &data_3098, 0);
  Script_SetDynamicMapTransform(3, &data_3098, 0);
  Script_SetDynamicMapTransform(4, &data_3098, 0);
  Script_SetDynamicMapTransform(5, &data_3098, 0);
  Script_SetDynamicMapTransform(6, &data_3098, 0);
  Script_SetDynamicMapTransform(7, &data_3080, 22022);
  Script_SetDynamicMapTransform(8, &data_3080, 22022);
  Script_SetDynamicMapTransform(9, &data_3080, 22022);
  Script_SetDynamicMapTransform(10, &data_3080, 22022);
  Script_mapmSetRenderFlag(9, 0);
  Script_mapmSetRenderFlag(10, 0);
  Script_EVT_SetPosition(SHEN_NAME('RDR2'), POS_XYZ, &data_308C);
  Script_EVT_SetRotation(SHEN_NAME('RDR2'), 0x38000000, &data_30A4, 0);
  return;
}

__attribute__((scn_string_bindings(3, data_30BF, 4, data_30CB, 5, data_30D6, 6, data_30E3, 7, data_30EE, 8, data_30FB, 9, data_3101, 10, data_310C, 11, data_3119, 12, data_311F, 13, data_312A, 14, data_3137, 15, data_313D, 16, data_3148, 17, data_3155, 18, data_315B, 19, data_3167, 20, data_3172)))
static void script_15AF(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F8 = 0;
  while (1) {
    Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_GetControllerInput(0) != 0x400) {
      int32_t local_EC;
      if (Script_GetControllerInput(1) == 64) {
        local_F8 = local_F8 - 1;
      }
      if (Script_GetControllerInput(1) == 128) {
        local_F8 = local_F8 + 1;
      }
      if (local_F8 > 2) {
        local_F8 = 0;
      }
      if (local_F8 < 0) {
        local_F8 = 2;
      }
      local_EC = local_F8;
      switch (local_EC) {
        case 0:
          Script_PrintDebugString(3, 25, "01152.c");
          break;
        case 1:
          Script_PrintDebugString(3, 25, ".c");
          break;
        case 2:
          Script_PrintDebugString(3, 25, "ALL");
          break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    {
      int32_t local_EC;
      local_EC = local_F8;
      switch (local_EC) {
        case 0:
          script_14CB();
          local_FC = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_01152.BIN");
          Script_LoadSND_Wrapper(0, 0, "bgm107.snd", 0, "a1_tabid.snd", 0, 0, 0);
          Script_executeAicaCommand(16296, 0, 0);
          Script_executeAicaCommand(1184, 2, 105);
          Script_scenCreateTask(script_0186, 0);
          script_03FE(1);
          Script_executeAicaCommand(2720, 2, 60);
          Script_filmCheckOrSetTransitionUtil(80, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_shenSeqUtils(-4);
          Script_executeAicaCommand(0x1200A0, 2, 0);
          SCNF_WaitFrames_0000(60);
          Script_ReallocateUNFO(local_FC);
          script_06A3();
          Script_SetGameStateTo(AUTH_None);
          SCNF_WaitFrames_0000(1);
          break;
        case 1:
          script_13AB();
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_LoadSND_Wrapper(0, 0, "tsm003.snd", 0, "a1_kstab.snd", 0, 0, 0);
          Script_LoadVoiceFile(1, "A0180");
          local_F4 = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_0180.BIN");
          local_F0 = Script_LoadHandMot(3, SHEN_NAME('YDMA'), "HMOT0156.BIN");
          script_0751();
          Script_ReallocateUNFO(local_F4);
          Script_FreeTaskData(local_F0);
          Script_LoadVoiceFile(0, "A0180");
          Script_executeAicaCommand(0x1200A0, 2, 0);
          Script_executeAicaCommand(1184, 2, 0);
          SCNF_WaitFrames_0000(1);
          break;
        case 2:
          script_13AB();
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_LoadSND_Wrapper(0, 0, "tsm003.snd", 0, "a1_kstab.snd", 0, 0, 0);
          Script_LoadVoiceFile(1, "A0180");
          local_F4 = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_0180.BIN");
          local_F0 = Script_LoadHandMot(3, SHEN_NAME('YDMA'), "HMOT0156.BIN");
          script_0751();
          Script_ReallocateUNFO(local_F4);
          Script_FreeTaskData(local_F0);
          Script_LoadVoiceFile(0, "A0180");
          Script_executeAicaCommand(0x1200A0, 2, 0);
          Script_executeAicaCommand(1184, 2, 0);
          script_14CB();
          local_FC = Script_motiLoad(3, SHEN_NAME('YDMA'), "M_01152.BIN");
          Script_LoadSND_Wrapper(0, 0, "bgm107.snd", 0, "a1_tabid.snd", 0, 0, 0);
          Script_executeAicaCommand(16296, 0, 0);
          Script_executeAicaCommand(1184, 2, 105);
          Script_scenCreateTask(script_0186, 0);
          script_03FE(1);
          Script_executeAicaCommand(2720, 2, 60);
          Script_filmCheckOrSetTransitionUtil(80, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_shenSeqUtils(-4);
          Script_executeAicaCommand(0x1200A0, 2, 0);
          SCNF_WaitFrames_0000(60);
          Script_ReallocateUNFO(local_FC);
          script_06A3();
          break;
      }
    }
  }
  return;
}
