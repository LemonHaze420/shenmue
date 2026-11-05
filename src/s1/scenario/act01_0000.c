#include "act01_0000.h"

void script_0030(void) {
  Script_scenCreateTask(script_0043, 1, CHARA_AKIR);
  return;
}

static int32_t script_0043(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoSetChara(arg0);
  Script_ryoControl(RYO_RestoreControl);
  SCNF_WaitFrames_0000(1);
  Script_shenClearFreeState(1);
  SCNF_WaitFrames_0000(1);
  end:;
}
