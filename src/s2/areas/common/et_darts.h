#ifndef SHENMUE_S2_AREAS_COMMON_ET_DARTS_H
#define SHENMUE_S2_AREAS_COMMON_ET_DARTS_H

#include "s2/event_queued_dc.h"

#define ET_DARTS_REVISION_WRS2 1
#define ET_DARTS_REVISION_DART 2
#ifndef SHENMUE_ET_DARTS_REVISION
#define SHENMUE_ET_DARTS_REVISION ET_DARTS_REVISION_WRS2
#endif

#define DARTS_COMPACT_BOARD SHEN_NAME4('D', 'A', 'R', '3')
#define DARTS_MATCH_OPPONENT 2

typedef struct EventDartsPlacement {
    int mode;
    Vector3i rotation;
    Vector3 position;
    shen_name character, parent;
} EventDartsPlacement;
typedef struct EventDartsEnemy {
    shen_name name;
    int aiIndex;
    HLibTask *matchTask;
    int reserved0c;
} EventDartsEnemy;

typedef struct EventDartsGame {
    char opponentMatch, mode, opponent;
    signed char aiParameters[2];
    unsigned char reserved05[3];
    int bet;
    int opponentScoreFloor;
    int bestScore, opponentScore;
    int reserved18;
    HLibTask *task;
    EventDartsPlacement *placement;
    EventDartsEnemy *enemy;
} EventDartsGame;
typedef struct EventDartsWork {
    int reserved00[9];
    HLibTask *task, *matchTask;
    int placement;
    int state;
    int reserved34;
    EventDartsGame game;
} EventDartsWork;
typedef struct EventDartsHelpIcon {
    int fov;
    int reserved04;
    int icon;
    shen_name character, alternate;
} EventDartsHelpIcon;

void EVT_EnsureDartsPackageMounted(void);
void EVT_ReleaseDartsPackage(void);
void EVT_StartDartsGame(EventDartsGame *game, int placement);
HLibTask *EVT_DartsEasySingle(EventDartsPlacement *placement, int count);
HLibTask *EVT_SetEnemyDarts(EventDartsPlacement *placement, EventDartsEnemy *enemy, int index);
void EVT_SetDartsHelpState(int state);
void EVT_QueueDartsHelpIcons(EventDartsHelpIcon *icons);

#endif
