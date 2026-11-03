#include "act01_mf99.h"

static void script_0030(int32_t arg0, float arg1, float arg2);
static void script_0058(int32_t arg0, int32_t arg1);
void script_0082(void);
static int32_t script_0242(int32_t arg0);

static const float data_025C[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_0268[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_0278[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_0284 = -1;
static const char data_0288[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_0293[14] __attribute__((scn_string_literal)) = "scene/01/MFSY";
static const char data_02A1[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const uint8_t data_02AD[3] = {0};

static void script_0030(int32_t arg0, float arg1, float arg2) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = arg2;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_0058(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  local_F4.x = 0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

void script_0082(void) {
  script_0030(SHEN_NAME('BX00'), 14.046f, 67.762001f);
  script_0058(SHEN_NAME('BX00'), 57346);
  script_0030(SHEN_NAME('TAG0'), 14.046f, 67.762001f);
  script_0058(SHEN_NAME('TAG0'), 57346);
  script_0030(SHEN_NAME('BX01'), 14.9f, 66.9f);
  script_0058(SHEN_NAME('BX01'), 57346);
  script_0030(SHEN_NAME('TAG1'), 14.9f, 66.9f);
  script_0058(SHEN_NAME('TAG1'), 57346);
  script_0030(SHEN_NAME('BX02'), 28.9862f, 52.726101f);
  script_0058(SHEN_NAME('BX02'), 57346);
  script_0030(SHEN_NAME('TAG2'), 28.9862f, 52.726101f);
  script_0058(SHEN_NAME('TAG2'), 57346);
  script_0030(SHEN_NAME('BX03'), 29.853201f, 51.856098f);
  script_0058(SHEN_NAME('BX03'), 57346);
  script_0030(SHEN_NAME('TAG3'), 29.853201f, 51.856098f);
  script_0058(SHEN_NAME('TAG3'), 57346);
  script_0030(SHEN_NAME('BX04'), 30.7f, 51.0f);
  script_0058(SHEN_NAME('BX04'), 57346);
  script_0030(SHEN_NAME('TAG4'), 30.7f, 51.0f);
  script_0058(SHEN_NAME('TAG4'), 57346);
  Script_motiLoad(1, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/01/MFSY", (int32_t)"SCRTEST.BIN", 0x5953464D);
  Script_scenCreateTask(script_0242, 1, CHARA_AKIR);
  return;
}

static int32_t script_0242(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoSetChara(arg0);
  return Script_ryoControl(RYO_RestoreControl);
}
