#ifndef SHENMUE_S2_AREAS_COMMON_ET_STREET_H
#define SHENMUE_S2_AREAS_COMMON_ET_STREET_H

#include "s2/event_queued_dc.h"
#include "s2/event_library_tail_dc.h"

#define STREET_FIGHT_INVALID 0xffffffffU
#define TASK_SUEV SHEN_NAME4('s', 'u', 'e', 'v')

#define STREET_FIGHT_STRING(name) #name
#define STREET_FIGHT_PACKAGE_STRING(name) STREET_FIGHT_STRING(name)
#define STREET_FIGHT_PACKAGE STREET_FIGHT_PACKAGE_STRING(SHENMUE_STREET_FIGHT_PACKAGE)

#define STREET_FIGHT_PASTE(a, b) a##b
#define STREET_FIGHT_PACKAGE_VALUE(prefix, name) STREET_FIGHT_PASTE(prefix, name)
#define STREET_FIGHT_CHEN_POSITION STREET_FIGHT_PACKAGE_VALUE(STREET_FIGHT_CHEN_POSITION_, SHENMUE_STREET_FIGHT_PACKAGE)
#define STREET_FIGHT_CHEN_ANGLE STREET_FIGHT_PACKAGE_VALUE(STREET_FIGHT_CHEN_ANGLE_, SHENMUE_STREET_FIGHT_PACKAGE)
#define STREET_FIGHT_CHEN_POSITION_ABSF {0.0f, 0.0f, 0.0f}
#define STREET_FIGHT_CHEN_ANGLE_ABSF 0
#define STREET_FIGHT_CHEN_POSITION_QSFF {205.98f, 100.0f, 124.97f}
#define STREET_FIGHT_CHEN_ANGLE_QSFF 110
#define STREET_FIGHT_CHEN_POSITION_QGBT {0.0f, 0.0f, 0.0f}
#define STREET_FIGHT_CHEN_ANGLE_QGBT 0
#define STREET_FIGHT_CHEN_POSITION_QJBT {0.0f, 0.0f, 0.0f}
#define STREET_FIGHT_CHEN_ANGLE_QJBT 0
#define STREET_FIGHT_CHEN_POSITION_QKBT {0.0f, 0.0f, 0.0f}
#define STREET_FIGHT_CHEN_ANGLE_QKBT 0
#define STREET_FIGHT_CHEN_POSITION_QLBT {0.0f, 0.0f, 0.0f}
#define STREET_FIGHT_CHEN_ANGLE_QLBT 0
#define STREET_FIGHT_CHEN_POSITION_QSFA {182.52f, 100.0f, 230.57f}
#define STREET_FIGHT_CHEN_ANGLE_QSFA 120
#define STREET_FIGHT_CHEN_POSITION_QSFB {335.87f, 84.25f, 85.83f}
#define STREET_FIGHT_CHEN_ANGLE_QSFB 7
#define STREET_FIGHT_CHEN_POSITION_QSFC {269.91f, 94.81f, 33.97f}
#define STREET_FIGHT_CHEN_ANGLE_QSFC 172
#define STREET_FIGHT_CHEN_POSITION_QSFD {132.55f, 100.0f, 106.64f}
#define STREET_FIGHT_CHEN_ANGLE_QSFD 230
#define STREET_FIGHT_CHEN_POSITION_QSFE {199.1f, 100.0f, 251.68f}
#define STREET_FIGHT_CHEN_ANGLE_QSFE 315

#define STREET_FIGHT_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define STREET_FIGHT_BOOKIE SHEN_NAME4('F', 'U', 'Y', '_')
#define STREET_FIGHT_BASE SHEN_NAME4('B', 'A', 'S', 'E')

typedef struct StreetFightStartParameters {
    shen_name area;
    int scene;
    int subScene;
    int *signal;
    unsigned int reserved10;
    unsigned int flags;
} StreetFightStartParameters;

typedef struct StreetFightFighterDefinition {
    shen_name character;
    const char *blurModel;
    unsigned char fightStyle;
    unsigned char difficulty;
    unsigned char guardMode;
    unsigned char reserved0b;
    unsigned short hitPoints;
    unsigned short reserved0e;
    float strikeScale;
    float guardScale;
} StreetFightFighterDefinition;

typedef struct StreetFightEventWork {
    HLibTask *task;
    shen_name fighters[11];
    shen_name area;
    int state;
    int battleStatus;
    int battleFrames;
    int motions[16];
    int previousNpcBatchCount;
    EventReadCharactersWork *characterReadTask;
    int frameCounter;
    HLibTask *audienceTask;
    Vector3 playerPosition;
    Vector3 bookiePosition;
    int fighterIndex;
    unsigned int dustEffect;
    float previousFarClip;
    float previousNearClip;
    unsigned int reservedb8;
    const StreetFightFighterDefinition *fighterDefinitions[11];
    StreetFightStartParameters *startParameters;
    int rank;
    int stage;
    unsigned int reservedf4[2];
    int auxiliaryState;
    unsigned char fadeFrames;
    unsigned char rankHistory[0x20];
    unsigned char attempts;
    unsigned char completedOpponents;
    unsigned char hudEnabled;
    unsigned char audienceResultPublished;
    unsigned char reserved125[7];
    signed char roundResults[3];
    signed char round;
    signed char wins;
    signed char losses;
    unsigned short reserved132;
    int returnToQron;
    int paidEntry;
} StreetFightEventWork;

typedef struct StreetFightCharacterLoadWork {
    HLibTask *task;
    HLibTask *parent;
    const shen_name *characters;
    int index;
    unsigned int state;
} StreetFightCharacterLoadWork;

typedef struct StreetFightNpcWaitWork {
    HLibTask *parent;
    HLibTask *self;
    unsigned int frames;
} StreetFightNpcWaitWork;

typedef struct StreetFightHighModelWork {
    HLibTask *task;
    shen_name character;
    int selector;
    int state;
} StreetFightHighModelWork;

typedef int (*StreetFightSkipFunction)(void *argument);
typedef struct StreetFightSkipWait {
    char state;
    char pressed;
    unsigned char reserved02[2];
    int frames;
    int window;
    int skipped;
    StreetFightSkipFunction function;
    void *argument;
} StreetFightSkipWait;

typedef struct StreetFightLookWork {
    shen_name character;
    shen_name target;
    int reserved08;
    int frames;
    unsigned short flags;
    unsigned short reserved12;
} StreetFightLookWork;

typedef struct StreetFightMotionSpeed {
    int cleanup;
    shen_name character;
    int reserved08;
    int reserved0c;
    float speed;
    int reserved14;
} StreetFightMotionSpeed;

typedef struct StreetFightAudienceWork {
    unsigned int mode;
    HLibTask *task;
    shen_name fighters[4];
    unsigned char fighterCount;
    unsigned char state;
    unsigned char reserved1a;
    unsigned char entityCount;
    Vector3 stagePosition;
    unsigned char reserved28[0x12];
    unsigned short actionFrames;
    short soundFrequency;
    short soundVolume;
    float radius;
    float radiusRange;
    Vector3 corridorStart;
    Vector3 corridorEnd;
    float corridorLength;
    Vector3 soundCentre;
    Vector3 soundPositions[30];
    unsigned char materialBank;
    unsigned char reserved1d9;
    unsigned short loopDelay;
    const shen_name *characterNames;
    unsigned int lastSound;
} StreetFightAudienceWork;

unsigned int ControlStreetFightActionChoice(int enabled);
unsigned int PollStreetFightActionChoice(void);
unsigned int ControlStreetFightMoneyMessage(int enabled);
unsigned int PollStreetFightMoneyMessage(void);
void SetStreetFightCharacterVisible(shen_name character, int visible);
void StreetFightLookCallback(void *data);
void QueueStreetFightLook(shen_name character, shen_name target, int reserved08, int delay, int frames, unsigned short flags);
void StreetFightNullsub(shen_name character, shen_name target, int mode, int delay, int frames);
void ReadStreetFightCharacterPosition(shen_name character, shen_name target, int mode);
void ClearStreetFightLook(shen_name character, int mode);
void StreetFightMotionSpeedCallback(void *data);
int QueueStreetFightMotionSpeed(shen_name character, float speed, int delay);
HLibTask *QueueStreetFightAudience(unsigned char count, unsigned int mode);
void ClearStreetFightAudienceFighters(HLibTask *task);
void AddStreetFightAudienceFighter(HLibTask *task, shen_name fighter);
void SetStreetFightAudienceCorridor(HLibTask *task, const Vector3 *start, const Vector3 *end);
void SetStreetFightAudienceCentre(HLibTask *task, const Vector3 *centre);
void SetStreetFightAudienceRadius(HLibTask *task, float radius, float range);
unsigned int SetStreetFightAudienceMode(HLibTask *task, unsigned int bits, int set);
void PlaceStreetFightCharacter(shen_name character, float degrees, const Vector3 *position);
int IsStreetFightAudienceArranged(HLibTask *task);
void StartStreetFightAudienceOpening(HLibTask *task, unsigned short delay);
void ResumeStreetFightAudienceOpening(HLibTask *task);
void StartStreetFightAudienceDefeat(HLibTask *task, unsigned short delay);
void StartStreetFightAudienceVictory(HLibTask *task, unsigned short delay);
void ResumeStreetFightAudienceVictory(HLibTask *task);
unsigned int ChangeStreetFightAudienceMode(HLibTask *task, unsigned int bits, int set);
void StreetFightAudienceCallback(void *data);
void PlayStreetFightAudienceBattleSounds(StreetFightAudienceWork *work);
void ControlStreetFightAudienceVisibility(StreetFightAudienceWork *work);
void ControlStreetFightAudiencePositions(StreetFightAudienceWork *work);
void FindStreetFightAudienceSpace(StreetFightAudienceWork *work, int entity);
int IsStreetFightAudienceCorridorBlocked(StreetFightAudienceWork *work, const Vector3 *point);
void PlayStreetFightAudienceArrivalSounds(StreetFightAudienceWork *work);
void PlayStreetFightAudienceOpeningSounds(StreetFightAudienceWork *work);
void PlayStreetFightAudienceDefeatSounds(StreetFightAudienceWork *work);
void PlayStreetFightAudienceVictorySounds(StreetFightAudienceWork *work);
void ReleaseStreetFightAudience(HLibTask *task);
void QueueStreetFightPlayerReaction(shen_name fighter, int playerWon);
void StreetFightRestoreChaseCamera(void *argument);
void StreetFightReactionNullsub(void *argument);
void StreetFightReactionEndNullsub(void *argument);
unsigned int CreateStreetFightDustEffect(void);
void StartStreetFightDust(StreetFightEventWork *work);
void StopStreetFightDust(StreetFightEventWork *work);
void QueueStreetFightSkipWait(StreetFightSkipFunction function, void *argument, int wait);
void StreetFightSkipWaitCallback(void *data);
int IsStreetFightSkipped(void);
int CanPerformForAreaFlags(shen_name area);
void ResetStreetFightStoryFlag(int stage);
int CommitStreetFightStoryRank(int stage);
int PlayStreetFightVictoryAppearance(void);
int PlayStreetFightKakuInvitation(void);
void PlaceStreetFightCharacterForArea(shen_name area, shen_name character);
int IsStreetFightBestOfThree(void);
int GetStreetFightStageOrder(int stage);
int GetStreetFightStageFlag(int stage);
void SetStreetFightStageFlag(int stage);
int StreetFightFalseNullsub(void);
void ResetStreetFightRounds(StreetFightEventWork *work);
int RecordStreetFightRound(StreetFightEventWork *work);
int GetStreetFightStageForArea(shen_name area);
int GetStreetFightWinLimit(int stage);
int GetStreetFightLossLimit(int stage);
int RecordStreetFightVictory(int stage);
int RecordStreetFightDefeat(int stage);
int GetHelpIconForArea(shen_name area);
void StreetFightNpcWaitBeginCallback(void *data);
void StreetFightNpcWaitEndCallback(void *data);
int QueueStreetFightNpcWait(unsigned int flags);
void QueueStreetFightHighModel(shen_name character, int selector, int state, int delay);
void StreetFightHighModelCallback(void *data);

extern int streetFightSkipped;

#endif
