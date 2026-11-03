#include "act01_mk99.h"

void script_0030(void);
static int32_t script_0043(int32_t arg0);

static const float data_005C[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_0068[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_0078[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_0084 = -1;

void script_0030(void) {
  Script_scenCreateTask(script_0043, 1, CHARA_AKIR);
  return;
}

static int32_t script_0043(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoSetChara(arg0);
  return Script_ryoControl(RYO_RestoreControl);
}
