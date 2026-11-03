#include "act01_0000.h"

void script_0030(void);
static int32_t script_0043(int32_t arg0);

static const float data_0074[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_0080[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_0090[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_009C = -1;

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
