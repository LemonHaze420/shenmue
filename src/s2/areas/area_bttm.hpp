#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/common/et_thr.h"

#define AREA_BTTM SHEN_NAME4('B', 'T', 'T', 'M')
#define TAMURO_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define TAMURO_VUN SHEN_NAME4('V', 'U', 'N', '_')
#define TAMURO_SBK SHEN_NAME4('S', 'B', 'K', '_')
#define TAMURO_RKN SHEN_NAME4('R', 'K', 'N', '_')
#define TAMURO_SEQUENCE_THREAD SHEN_NAME4('S', 'E', 'Q', 'P')
#define TAMURO_BATTLE_THREAD SHEN_NAME4('B', 'S', 'B', 'T')
#define TAMURO_INVALID_CHARACTER 0xffffffff
#define TAMURO_BATTLE_WON 3
#define TAMURO_WIN_STEP_FLAG 349

enum TamuroMode {
    TAMURO_MODE_INIT,
    TAMURO_MODE_LOAD_NPC,
    TAMURO_MODE_INIT2,
    TAMURO_MODE_LOAD_PACKAGE,
    TAMURO_MODE_START,
    TAMURO_MODE_MAIN,
    TAMURO_MODE_EXIT,
    TAMURO_MODE_EXIT2,
    TAMURO_MODE_END
};
typedef struct TamuroWork {
    Thr *thr;
    HLibTask *task;
    int threadActive;
    int mode;
    int reserved10, reserved14;
    void *motions[16];
    int npcIndex;
    int npcLoaded;
    int battleStatus;
} TamuroWork;

void TamuroEntry(void *argument);
void StartTamuro(void);
void ExitTamuro(TamuroWork *work);
void TamuroMain(void *data);
void TamuroSequenceThread(void *data);
void TamuroBattleThread(void *data);
#else
#endif
