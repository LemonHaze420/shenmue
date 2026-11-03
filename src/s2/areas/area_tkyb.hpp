#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_library_tail_dc.h"
#include "s2/sk_event_dc.h"
#include "s2/areas/common/et_abort.h"

#define TKYB_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define TKYB_GEN SHEN_NAME4('G', 'E', 'N', '_')
#define TKYB_RNT SHEN_NAME4('R', 'N', 'T', '_')
#define TKYB_VUN SHEN_NAME4('V', 'U', 'N', '_')
#define TKYB_CHO SHEN_NAME4('C', 'H', 'O', '_')
#define TKYB_CHY SHEN_NAME4('C', 'H', 'Y', '_')
#define TKYB_IKO SHEN_NAME4('I', 'K', 'O', '_')
#define TKYB_RIK SHEN_NAME4('R', 'I', 'K', '_')
#define TKYB_SBK SHEN_NAME4('S', 'B', 'K', '_')
#define TKYB_KNIFE_A SHEN_NAME4('K', 'N', 'F', 'A')
#define TKYB_KNIFE_B SHEN_NAME4('K', 'N', 'F', 'B')
#define TKYB_KNIFE_C SHEN_NAME4('K', 'N', 'F', 'C')
#define TKYB_KNIFE_D SHEN_NAME4('K', 'N', 'F', 'D')
#define TKYB_PROP_A132 SHEN_NAME4('a', '1', '3', '2')
#define TKYB_PROP_A129 SHEN_NAME4('a', '1', '2', '9')
#define TKYB_PROP_A038 SHEN_NAME4('a', '0', '3', '8')
#define TKYB_PROP_0001 SHEN_NAME4('0', '0', '0', '1')
#define TKYB_DUMMY SHEN_NAME4('d', 'u', 'm', 'y')
#define TKYB_BATTLE_GRID SHEN_NAME4('B', 'T', 'C', '1')
#define TKYB_DEFAULT_GRID SHEN_NAME4('0', '0', '0', '0')
#define TKYB_WK00 SHEN_NAME4('W', 'K', '0', '0')
#define TKYB_WTA0 SHEN_NAME4('W', 'T', 'A', '0')
#define TKYB_START_AUTHOR SHEN_NAME4('S', 'T', 'R', 'T')
#define TKYB_QTE_OK_AUTHOR SHEN_NAME4('Q', '_', 'O', 'K')
#define TKYB_QTE_NG_AUTHOR SHEN_NAME4('Q', '_', 'N', 'G')
#define TKYB_BATTLE_OK_AUTHOR SHEN_NAME4('B', '_', 'O', 'K')
#define TKYB_BATTLE_NG_AUTHOR SHEN_NAME4('B', '_', 'N', 'G')
#define TKYB_INVALID 0xffffffffU
#define TKYB_ABORT_SLOT 3
#define TKYB_ABORT_FRAMES 36000
#define TKYB_IDLE_MOTION 0xf002
#define TKYB_QTE_BUTTON 256
#define TKYB_RESULT_BATTLE_WON 1
#define TKYB_RESULT_QTE_WON 2
#define TKYB_RESULT_BATTLE_LOST 3

typedef struct TkybWork {
    HLibTask *task;
    EventReadCharactersWork *characters;
    int result;
    int frame;
    int reserved10[5];
    int dummyCounter;
    unsigned char fightStage;
} TkybWork;

void RunTkybEvent(int argument);
void TkybClearedCallback(void *parameter);
void TkybInitialize(HLibTask *task);
void TkybStartCallback(void *data);
void TkybFadeInCallback(void *data);
void TkybStartAuthorCallback(void *data);
void TkybKnifeStartCallback(void *data);
void TkybKnifeCameraCallback(void *data);
void TkybKnifeThrowCallback(void *data);
void TkybKnifeQteCallback(void *data);
void TkybUpdateKnives(TkybWork *work, int frame);
void TkybQteResultCallback(void *data);
void TkybFightSetupCallback(void *data);
void TkybFightWaitCallback(void *data);
void TkybFightUpdate(TkybWork *work);
void TkybBattleWonCallback(void *data);
void TkybBattleLostCallback(void *data);
void TkybAbortCallback(void *data);
void TkybExitCallback(void *data);
void TkybPrepareCharacter(shen_name character, int authored);
void TkybRestoreCharacter(shen_name character, int authored);
#endif
