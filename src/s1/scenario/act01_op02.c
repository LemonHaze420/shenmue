#include "act01_op02.h"

static void script_0030(int32_t arg0);
static void script_0C00(void);
static void script_0C5A(void);
static int32_t script_0D6C(void);
static void script_0E2B(int32_t arg0);
static void script_0E58(int32_t arg0);
static void script_0EF2(void);
static void script_0F03(int32_t arg0);
static void script_0F0F(int32_t arg0);
void script_0F33(void);
static void script_0F51(void);
static void script_0F6F(void);
static void script_0F85(void);
static void script_0F9B(void);
static void script_0FBD(void);
static void script_0FEF(void);
static int32_t script_1011(int32_t arg0);

static const float data_1094[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_10A0[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_10B0[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_10BC = -1;
static const Vector4 data_10C0 = {.x = 1.0f, .y = 0.4f, .z = 0.3f, .w = 0.3f};
static const int32_t data_10D0[57] = {
  0, 0, 1638, 0, 0, -1456, 0, 0, -728, 0, 0, -2366, 0, 0, -2366, 0, 0, -1456, 0, 0, 0, 0, 0, -7463, 0, 1274, -10012, 0, 728, 1092, 0, 0, 0, 0, 0,
  -9102, 0, 2002, -12743, 0, 2366, 728, 0, 0, 0, 0, 0, -9648, 0, -1820, -10012, 0, 2184, -4369, 0, 0, 0
};
static const int32_t data_11B4[57] = {
  0, 0, 1638, 0, 0, -1456, 0, 0, -728, 0, 0, -2366, 0, 0, -2366, 0, 0, -1456, 0, 0, 0, 0, 0, -8009, 0, 1274, -9102, 0, 728, -364, 0, 0, 0, 0, 0,
  -9102, 0, 2002, -12743, 0, 2366, 728, 0, 0, 0, 0, 0, -9648, 0, -1820, -10012, 0, 2184, -4369, 0, 0, 0
};
static const Vector3i data_1298 = {.x = 10194, .y = 0, .z = 0};
static const char data_12A4[5] __attribute__((scn_string_literal)) = "KAKU";
static const char data_12A9[5] __attribute__((scn_string_literal)) = "HAWK";
static const char data_12AE[16] __attribute__((scn_string_literal)) = "/scene/01/OP02/";
static const char data_12BE[5] __attribute__((scn_string_literal)) = "SHDR";
static const char data_12C3[5] __attribute__((scn_string_literal)) = "SHDR";
static const Vector4 data_12C8 = {.x = 1.0f, .y = 0.4f, .z = 0.3f, .w = 0.3f};
static const char data_12D8[16] __attribute__((scn_string_literal)) = "/scene/01/OP02/";
static const char data_12E8[9] __attribute__((scn_string_literal)) = "OP02.afs";
static const char data_12F1[7] __attribute__((scn_string_literal)) = "SHDR";
static const FogColourData data_12F8 = {.red = 20.0f, .green = 800.0f, .blue = 0.36f, .colour = 0xFFFF6900U};
static const uint32_t data_1308[4] = {0x96413D23U, 0x96413D23U, 0, 0};
static const uint32_t data_1318[4] = {0xB0952D23U, 0xB0952D23U, 0x22787270U, 0x22787270U};
static const FogColourData data_1328 = {.red = 20.0f, .green = 800.0f, .blue = 0.36f, .colour = 0xFFFF6900U};
static const uint32_t data_1338[4] = {0x96413D23U, 0x96413D23U, 0, 0};
static const uint32_t data_1348[4] = {0xB0952D23U, 0xB0952D23U, 0x22787270U, 0x22787270U};
static const Vector3 data_1358 = {.x = 5020.0f, .y = -210.0f, .z = -5620.0f};
static const char data_1364[8] __attribute__((scn_string_literal)) = "A0100";

static void script_0030(int32_t arg0) {
  int32_t unused_F8[2];
  int16_t local_F6;
  int16_t local_F4;
  int32_t local_E4[4];
  FogColourData local_D4;
  int32_t unused_C4[4];
  int32_t local_C0;
  int32_t local_BC;
  int32_t local_B8;
  int32_t local_B4;
  int32_t local_B0;
  int32_t local_AC;
  int32_t local_A8;
  int32_t local_A4;
  float local_A0;
  float local_9C;
  float local_98;
  int32_t local_94;
  int32_t local_90;
  int32_t local_8C;
  int32_t local_88;
  Vector3 local_FF7C;
  int32_t local_FF78;
  local_D4.red = 20.0f;
  (&local_D4)->green = 800.0f;
  (&local_D4)->blue = 0.36f;
  (&local_D4)->colour = -0x9700;
  local_A0 = -2.4875f;
  local_9C = -4.375f;
  local_98 = -0.0325000025f;
  Script_shenSeqUtils(-9, 0, 0);
  Script_shenSeqUtils(-9, 1, 0);
  Script_shenSeqUtils(-9, 2, 0);
  Script_shenSeqUtils(-9, 3, 0);
  Script_shenSeqUtils(-9, 4, 0);
  Script_shenSeqUtils(-9, 5, 0);
  Script_shenSeqUtils(-9, 6, 0);
  Script_executeAicaCommand(11176, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_filmCheckOrSetTransitionUtil(50, 255, 0, 0, 0, 0, 0, 0, 0);
  local_FF78 = arg0;
  if (local_FF78 == 7) {
    Script_scenCreateTask(script_0D6C, 0);
    script_0F6F();
    local_F6 = 0;
    Script_shenSeqUtils(0);
    while (Script_shenSeqUtils(-1)) {
      if (local_F6 == 0) {
        Script_tmnmUtils(SHEN_NAME('HAWK'), 57346, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
        Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
      }
      script_0C00();
      local_F6 = local_F6 + 1;
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
    script_0F85();
    local_F6 = 0;
    Script_shenSeqUtils(1);
    while (Script_shenSeqUtils(-1)) {
      switch (local_F6) {
        case 0:
          Script_mapmSetRenderFlag(1, 0);
          Script_charaDisplay(CHARA_SINF, CDM_Hide);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57346, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
        case 101:
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57345, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
        case 152:
          Script_CreateNPCImageModule(SHEN_NAME('HAWK'), "KAKU", 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57350, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
        case 196:
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57352, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
        case 253:
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57351, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
        case 281:
          Script_CreateNPCImageModule(SHEN_NAME('HAWK'), "HAWK", 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57349, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
      }
      script_0C00();
      local_F6 = local_F6 + 1;
      SCNF_WaitFrames_0000(1);
    }
    local_F6 = 0;
    Script_shenSeqUtils(2);
    while (Script_shenSeqUtils(-1)) {
      if (local_F6 == 0) {
        Script_charaDisplay(CHARA_SINF, CDM_Show);
      }
      script_0C00();
      local_F6 = local_F6 + 1;
      SCNF_WaitFrames_0000(1);
    }
    script_0F9B();
    local_F6 = 0;
    Script_shenSeqUtils(3);
    while (Script_shenSeqUtils(-1)) {
      switch (local_F6) {
        case 0:
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57348, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
        case 167:
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57347, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
        case 234:
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57346, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
      }
      script_0C00();
      local_F6 = local_F6 + 1;
      SCNF_WaitFrames_0000(1);
    }
    script_0FEF();
    local_F6 = 0;
    Script_shenSeqUtils(6);
    while (Script_shenSeqUtils(-1)) {
      switch (local_F6) {
        case 0:
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57346, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
        case 75:
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57345, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
        case 153:
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57347, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
        case 220:
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57346, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          break;
      }
      script_0C00();
      local_F6 = local_F6 + 1;
      SCNF_WaitFrames_0000(1);
    }
    script_0FBD();
    local_F6 = 0;
    Script_shenSeqUtils(4);
    while (Script_shenSeqUtils(-1)) {
      switch (local_F6) {
        case 0:
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
          Script_mapmSetRenderFlag(0, 0);
          Script_SpriteUtils(ModifySky, 0, 1, 560);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 57346, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
          Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
          Script_charaSetRefboardFlagTo(CHARA_SINF, 0);
          Script_ResetHandMotion(CHARA_SINF, 0, 0, 0);
          Script_ResetHandMotion(CHARA_SINF, 0, 1, 0);
          Script_FaceEx(CHARA_SINF, 0, 0, 2);
          break;
        case 70:
          Script_charaSetRefboardFlagTo(CHARA_SINF, 1);
          Script_SetCharaRefBoardPos(CHARA_SINF, &data_10C0);
          break;
        case 300:
          Script_FaceEx(CHARA_SINF, 1, 0, 2);
          Script_ResetHandMotion(CHARA_SINF, 1, 0, 0);
          Script_ResetHandMotion(CHARA_SINF, 1, 1, 0);
          Script_ResetHandMotion(CHARA_SINF, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_SINF, 1, data_11B4, 0);
          Script_handSetFloatsFor(CHARA_SINF, 0, data_10D0, 0);
          Script_SetHandRootRotation(CHARA_SINF, 0, 0x15, &data_1298);
          break;
      }
      switch (local_F6) {
        case 0:
          Script_FACE_LCG(CHARA_SINF, 1, 0, 0);
          break;
        case 430:
          Script_FACE_LCG(CHARA_SINF, 2, 12, 0);
          break;
        case 442:
          Script_FACE_LCG(CHARA_SINF, 0, 6, 0);
          break;
      }
      script_0C00();
      local_F6 = local_F6 + 1;
      SCNF_WaitFrames_0000(1);
    }
    local_F4 = local_F6 = 0;
    Script_shenSeqUtils(5);
    while (Script_shenSeqUtils(-1)) {
      switch (local_F6) {
        case 0:
          Script_SetupLightFromTable(1);
          Script_charaDisplay(CHARA_SINF, CDM_Show);
          Script_charaDisplay(SHEN_NAME('HAWK'), CDM_Hide);
          Script_FACE_LCG(CHARA_SINF, 2, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_SINF, 0);
          break;
        case 13:
          Script_FACE_LCG(CHARA_SINF, 1, 3, 0);
          break;
        case 17:
          Script_FACE_LCG(CHARA_SINF, 2, 3, 0);
          break;
        case 20:
          local_FF7C.x = 0.791f;
          (&local_FF7C)->y = 1.505997f;
          (&local_FF7C)->z = -3.77f;
          Script_FaceLookAtTargetUtils(CHARA_SINF, 2, &local_FF7C, 6);
          break;
      }
      if ((local_F6 >= 19) & (local_F6 < 27)) {
        local_94 = 255;
        local_90 = local_F4 * -156 / 8 + 255;
        local_8C = local_F4 * -67 / 8 + 105;
        local_88 = local_F4 * 187 / 8;
        local_D4.red = (&local_D4)->red + local_A0;
        (&local_D4)->green = (&local_D4)->green + local_9C;
        (&local_D4)->blue = (&local_D4)->blue + local_98;
        (&local_D4)->colour = local_94 * 0x1000000 + local_90 * 0x10000 + local_8C * 0x100 + local_88;
        Script_SetFogColour(&local_D4);
        local_C0 = local_F4 * -150 / 8 + 150;
        local_BC = local_F4 * -65 / 8 + 65;
        local_B8 = local_F4 * -42 / 8 + 61;
        local_B4 = local_F4 * 46 / 8 + 35;
        local_B0 = local_F4 * 215 / 8;
        local_AC = local_F4 * 13 / 8;
        local_A8 = 0;
        local_A4 = local_F4 * 65 / 8;
        local_E4[0] = local_C0 * 0x1000000 + local_BC * 0x10000 + local_B8 * 0x100 + local_B4;
        local_E4[1] = *(int32_t *)local_E4;
        local_E4[2] = local_B0 * 0x1000000 + local_AC * 0x10000 + local_A8 * 0x100 + local_A4;
        local_E4[3] = local_E4[2];
        Script_SetFogFlags(0, (const uint32_t *)local_E4);
        local_F4 = local_F4 + 1;
      }
      script_0C00();
      local_F6 = local_F6 + 1;
      SCNF_WaitFrames_0000(1);
    }
    Script_executeAicaCommand(2720, 2, 30);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    goto L000BFA;
  }
  L000BFA:;
  script_0EF2();
  return;
}

static void script_0C00(void) {
  if (Script_GetInputData(0, 0) == 0x800) {
    Script_executeAicaCommand(2720, 2, 30);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    MODU_Script_shenRequestGameModule(Display, 1, 0x3230504F, 49);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_12AE, 1, data_12BE, 2, data_12C3)))
static void script_0C5A(void) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/OP02/", "SHDR") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "SHDR");
  script_0E2B(SHEN_NAME('HAWK'));
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  if (-1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L000CF1;
    }
  }
  L000CF1:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_tmnmUtils(SHEN_NAME('HAWK'), 57346, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('HAWK'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
  return;
}

__attribute__((scn_string_bindings(0, data_12D8)))
static int32_t script_0D6C(void) {
  int32_t local_FC;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  local_FC = Script_EV_PAK_AFS_Utils(EVPAK_MODE_AFS_READ, 0, "/scene/01/OP02/", "OP02.afs");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 8, (void *)local_FC, 0, 1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_0E58(CHARA_SINF);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_charaSetRefboardFlagTo(CHARA_SINF, 1);
  Script_SetCharaRefBoardPos(CHARA_SINF, &data_12C8);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 100.0f);
  Script_SetOsageFlags(2, 0.075f);
  Script_SetupClothData(CHARA_SINF, 4);
  return Script_EV_PAK_AFS_Utils(EVPAK_MODE_AFS_READ, 1, (void *)local_FC);
}

static void script_0E2B(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetFlag80(arg0, 0);
  Script_ccowUtils(arg0, 2);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_0E58(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_ccowSetFlag80(arg0, 0);
  Script_ccowUtils(arg0, 2);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_FACE_LCG(arg0, 0, 6, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCPAMFlags(arg0, 0);
  Script_clmdSetFlags(arg0, 8, 1);
  Script_SetMotnFlag(arg0, -1);
  return;
}

__attribute__((scn_string_bindings(0, data_12F1)))
static void script_0EF2(void) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "SHDR");
  return;
}

static void script_0F03(int32_t arg0) {
  Script_SetCPAMFlags(arg0, 1);
  return;
}

static void script_0F0F(int32_t arg0) {
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  return;
}

void script_0F33(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_0F51();
  Script_scenCreateTask(script_1011, 1, CHARA_AKIR);
  return;
}

static void script_0F51(void) {
  Script_SetFogFlags(0, data_1308);
  Script_SetFogColour(&data_12F8);
  Script_SetupLightFromTable(0);
  return;
}

static void script_0F6F(void) {
  Script_SpriteUtils(ScrollVerticalOffset, 727);
  Script_SpriteUtils(ScrollHorizontalOffset, 0);
  return;
}

static void script_0F85(void) {
  Script_SpriteUtils(ScrollVerticalOffset, 363);
  Script_SpriteUtils(ScrollHorizontalOffset, 0);
  return;
}

static void script_0F9B(void) {
  Script_SpriteUtils(ScrollVerticalOffset, -272);
  Script_SpriteUtils(ScrollHorizontalOffset, 22755);
  Script_SetFogFlags(0, data_1318);
  return;
}

static void script_0FBD(void) {
  Script_SpriteUtils(ScrollVerticalOffset, -272);
  Script_SpriteUtils(ScrollHorizontalOffset, 22755);
  Script_SetFogFlags(0, data_1338);
  Script_SetFogColour(&data_1328);
  Script_SetupLightFromTable(0);
  return;
}

static void script_0FEF(void) {
  Script_SpriteUtils(ScrollVerticalOffset, -272);
  Script_SpriteUtils(ScrollHorizontalOffset, 22755);
  Script_SetFogFlags(0, data_1348);
  return;
}

static int32_t script_1011(int32_t arg0) {
  int32_t unused_FC;
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  script_0C5A();
  Script_ryoSetChara(arg0);
  Script_mapmSetCamFlags(0, 0);
  Script_SetDynamicMapTransform(1, &data_1358, 0xC000);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 200.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
  Script_SpriteUtils(Scroll_Read3, 0, 0);
  Script_LoadVoiceFile(1, "A0100");
  script_0030(7);
  MODU_Script_shenRequestGameModule(Display, 1, 0x3230504F, 0);
  end:;
}
