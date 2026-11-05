#include "act01_mk99.h"

void script_0030(void) {
  Script_scenCreateTask(script_0043, 1, CHARA_AKIR);
  return;
}

static int32_t script_0043(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoSetChara(arg0);
  return Script_ryoControl(RYO_RestoreControl);
}
