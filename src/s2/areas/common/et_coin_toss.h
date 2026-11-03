#ifndef SHENMUE_S2_AREAS_COMMON_ET_COIN_TOSS_H
#define SHENMUE_S2_AREAS_COMMON_ET_COIN_TOSS_H

#include "s2/event_queued_dc.h"
#include "s2/event_library_tail_dc.h"

#define COIN_TOSS_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define COIN_TOSS_REN SHEN_NAME4('R', 'E', 'N', '_')
#define COIN_TOSS_BASE SHEN_NAME4('c', 'b', 'a', 's')
#define COIN_TOSS_IDLE SHEN_NAME4('I', 'D', 'L', 'E')
#define COIN_TOSS_INIT SHEN_NAME4('I', 'N', 'I', 'T')
#define COIN_TOSS_START SHEN_NAME4('S', 'T', 'R', 'T')
#define COIN_TOSS_SELECT SHEN_NAME4('S', 'E', 'L', '1')
#define COIN_TOSS_CHOICE SHEN_NAME4('S', 'E', 'L', '2')
#define COIN_TOSS_AFTER SHEN_NAME4('A', 'F', 'T', 'R')
#define COIN_TOSS_NONE SHEN_NAME4('N', 'O', 'N', 'E')
#define COIN_TOSS_END SHEN_NAME4('E', 'N', 'D', '1')
#define COIN_TOSS_FADE SHEN_NAME4('E', 'N', 'D', '2')

void CoinTossInit(const Vector3 *position, Vector3i *rotation);
int CoinTossUpdate(void);
void CoinTossStart(void);
void CoinTossSelect(void);
void CoinTossMenu(void);
void CoinTossChoose(void);
void CoinTossAfter(void);
void CoinTossNone(void);
void CoinTossFinish(void);

#endif
