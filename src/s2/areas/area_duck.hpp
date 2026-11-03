#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_queued_dc.h"
#include "s2/event_library_tail_dc.h"

#define DUCK_AREA SHEN_NAME4('D', 'U', 'C', 'K')
#define DUCK_MAP SHEN_NAME4('W', 'T', '0', '0')
#define DUCK_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define DUCK_CAMERA SHEN_NAME4('C', 'A', 'M', '_')
#define DUCK_TASK SHEN_NAME4('d', 'u', 'c', 'k')
#define DUCK_RACER_COUNT 6
#define DUCK_PROFILE_COUNT 23
#define DUCK_REPLAY_FRAMES 90
#define DUCK_SPEED_FAST 0
#define DUCK_SPEED_NORMAL 1
#define DUCK_SPEED_SLOW 2
#define DUCK_VOICE_COUNT 16
#define DUCK_VOICE_LENGTH 12
#define DUCK_VOICE_PREFIX_COUNT 4
#define DUCK_VOICE_PREFIX_LENGTH 16
#define DUCK_VOICE_REPEAT 0x20000000
#define DUCK_VOICE_SKIPPABLE 0x40000000
#define DUCK_MIN(a, b) ((a) < (b) ? (a) : (b))
#define DUCK_MAX(a, b) ((a) > (b) ? (a) : (b))
#define DUCK_FLUSH(x) (__builtin_fabs(x) < 1.1920929e-07f ? 0.0f : (x))

typedef struct Vector3s {
    short x, y, z;
} Vector3s;

typedef struct DuckProfile {
    float speedNormal, speedMax, speedMin, probeBackoff, separationRadius;
    short style, guts, handling, collisionCost;
    float staminaBase, sizeScale, speedResponse;
} DuckProfile;

typedef struct DuckRaceActor {
    HLibTask *self;
    int reserved04;
    Vector3 position;
    Vector3i rotation;
    int targetYaw;
    shen_name name;
    unsigned int flags;
    unsigned int behavior;
    int index;
    int replaySlot;
    int collisionSlowTimer;
    int turnLockTimer;
    int actionTimer;
    int steerTimer;
    int currentMotion;
    int stamina;
    int maximumStamina;
    short profile;
    short handling;
    short speedTier;
    short guts;
    short collisionStaminaCost;
    short soundCooldown;
    float speed;
    float speedLevels[3];
    float separationRadius;
    float probeBackoff;
    float speedResponse;
    float sizeScale;
    float distance;
    int aiControlled;
    short style;
    short reserved8a;
    int finishTriggerSeen;
} DuckRaceActor;

typedef struct DuckReplaySample {
    Vector3 position;
    int motion;
    float yawDegrees;
    float motionFrame;
} DuckReplaySample;

typedef struct DuckCourseRange {
    Vector3 first, second;
} DuckCourseRange;

typedef struct DuckTurnTrigger {
    Vector3 position;
    int yaw;
    float valid;
} DuckTurnTrigger;

typedef struct DuckVoiceWork {
    HLibTask *parent;
    int delay;
    int index;
    int reserved0c;
    int ticks;
    unsigned int mode;
} DuckVoiceWork;

typedef short (*DuckStyleCallback)(DuckRaceActor *actor);

#define DUCK_STATE_INIT 0
#define DUCK_STATE_INTRO 1
#define DUCK_STATE_ENTRY_DIALOGUE 2
#define DUCK_STATE_PAY_ENTRY 3
#define DUCK_STATE_BETTING_INTRO 4
#define DUCK_STATE_INSUFFICIENT_FUNDS 5
#define DUCK_STATE_BET_MENU 6
#define DUCK_STATE_WAGER_INPUT 7
#define DUCK_STATE_SELECT_DUCK 8
#define DUCK_STATE_PADDOCK_VIEW 9
#define DUCK_STATE_DECLINE 10
#define DUCK_STATE_POST_RACE 11
#define DUCK_STATE_COUNTDOWN 12
#define DUCK_STATE_RESULTS 13
#define DUCK_STATE_RUNNING 14
#define DUCK_STATE_CLEANUP 15

typedef struct DuckEventWork {
    int reserved00;
    EventReadCharactersWork *npcLoad;
    short state;
    short phase;
    int wager;
    int selection;
} DuckEventWork;

typedef struct DuckHudText DuckHudText;
struct DuckHudText {
    unsigned int size;
    shen_name allocationToken;
    char text[12];
    unsigned int glyphCache[12];
    int glyphTextures[12];
    float depth;
    unsigned int colour;
    float scaleX, scaleY, x, y, spacing;
    void (*release)(DuckHudText *text);
    void (*draw)(DuckHudText *text);
    signed char (*loadGlyph)(const void *texture);
};

typedef struct DuckSpritePacket {
    int flags;
    unsigned char reserved04[32];
    unsigned int colour;
    unsigned char reserved28[12];
    int reserved34;
    unsigned char reserved38[12];
    float u0, v0, u1, v1;
    int reserved54;
} DuckSpritePacket;

typedef struct DuckStatusBar {
    int element;
    DuckSpritePacket packet;
} DuckStatusBar;

void *memset(void *destination, int value, unsigned int size);

extern const DuckProfile duckProfiles[DUCK_PROFILE_COUNT];

int duckArrivalCount;
short duckEntrantCount;
shen_name duckPlaceNames[DUCK_RACER_COUNT];
shen_name duckRaceRoster[DUCK_RACER_COUNT];
int duckArrivalTicks[DUCK_RACER_COUNT];
int duckRaceExperiencePublished;
DuckStatusBar duckStatusBars[5];
int duckFollowCameraEnabled;

void InitializeDuckRaceCamera(void);
void UpdateDuckRaceFollowCamera(void);
void SnapDuckRaceCameraTarget(void);
void SetDuckRacePreviewCameraTarget(int actor);
void InitializeDuckRaceCourse(void);
void ResetDuckRaceRuntime(void);
void SetupDuckRacePlayer(unsigned short slot);
void QueueDuckRaceActor(int profile);
void DestroyDuckRaceActors(void);
void PlaceDuckRaceActors(void);
void InitializeDuckRaceActor(DuckRaceActor *actor, int profile);
void DuckRaceActor_Callback(void *data);
void DuckRaceActorNullsub(DuckRaceActor *actor);
void TickDuckRaceActor(DuckRaceActor *actor);
void TickDuckRacePlayer(DuckRaceActor *actor);
void TickDuckRaceAi(DuckRaceActor *actor);
void AdvanceDuckRaceActor(DuckRaceActor *actor);
void UpdateDuckRaceTurnZones(DuckRaceActor *actor);
void ResolveDuckRaceActorCollisions(DuckRaceActor *actor);
void UpdateDuckRaceTurnExperience(DuckRaceActor *actor);
void UpdateDuckRaceHopRecovery(DuckRaceActor *actor);
void UpdateDuckRacePlayerSpeed(DuckRaceActor *actor);
void UpdateDuckRacePlayerHopInput(DuckRaceActor *actor);
void CheckDuckRaceTurnTriggers(DuckRaceActor *actor);
void UpdateDuckRaceTurnRecovery(DuckRaceActor *actor);
void AvoidDuckRaceCourseMarkers(DuckRaceActor *actor);
void CheckDuckRaceJumpBounds(DuckRaceActor *actor);
void CheckDuckRaceCourseBounds(DuckRaceActor *actor);
void UpdateDuckRaceAiSpeed(DuckRaceActor *actor);
void UpdateDuckRaceReplayActor(DuckRaceActor *actor);
void UpdateDuckRaceActorExperience(DuckRaceActor *actor);
int CalculateDuckRaceExperience(int increment);
void UpdateDuckRaceActorMotion(DuckRaceActor *actor);
short FindDuckRaceCourseSegment(shen_name name);
DuckRaceActor *GetDuckRaceActor(unsigned short index);
void DuckRaceActorTickNullsub1(DuckRaceActor *actor);
void DuckRaceActorTickNullsub2(DuckRaceActor *actor);
void QueueDuckRaceReplay(void);
void DuckReplay_Callback(void *data);
shen_name GetDuckProfileName(int profile);
short GetDuckRaceActorProfile(int index);
short HoldOffSpeed(DuckRaceActor *actor);
short FastStartSpeed(DuckRaceActor *actor);
short QuickLegsSpeed(DuckRaceActor *actor);
short HomeStretchSpeed(DuckRaceActor *actor);

void StartDuckRace(void);
void QueueDuckRace(void *parameter);
int InitializeDuckRace(DuckEventWork *work);
void SetupDuckRaceActors(void);
void StartDuckRaceEnd(DuckEventWork *work);
void DuckRace_End(void *data);
void DuckRace_Callback(void *data);
void RunDuckRaceIntro(DuckEventWork *work);
int RunDuckRaceCountdown(DuckEventWork *work);
void RunDuckRace(DuckEventWork *work);
void UpdateDuckRaceStatusHud(void);
void RunDuckRacePostRace(DuckEventWork *work);
int GetDuckRaceTicks(void);
int GetDuckRaceState(void);
void SelectDuckRaceProfiles(void);
void CalculateDuckRaceOdds(void);
int RunDuckRaceEntryDialogue(DuckEventWork *work);
int RunDuckRacePayEntry(DuckEventWork *work);
void UpdateDuckRaceOddsBoard(void);
int RunDuckRaceBettingIntro(DuckEventWork *work);
int RunDuckRaceBetMenu(DuckEventWork *work);
int RunDuckRaceInsufficientFunds(DuckEventWork *work);
int RunDuckRaceWagerInput(DuckEventWork *work);
int RunDuckRaceSelection(DuckEventWork *work);
int RunDuckRacePaddock(DuckEventWork *work);
int ShowDuckRacePaddock(DuckEventWork *work);
int BrowseDuckRacePaddock(DuckEventWork *work);
void QueueDuckPaddockInfo(int index);
void DuckPaddockInfo_Callback(void *data);
int RunDuckRaceDecline(DuckEventWork *work);
void SetDuckRaceCrowd(int mode);
void ShowDuckRaceCrowd(void);
void DuckRaceCheer_Callback(void *data);
int RunDuckRaceResults(DuckEventWork *work);
int RunDuckRaceBettingWin(DuckEventWork *work);
int RunDuckRaceBettingLoss(DuckEventWork *work);
int RunDuckRaceSpecialWin(DuckEventWork *work);
int RunDuckRaceSpecialLoss(DuckEventWork *work);
void AwardDuckRacePrize(void *argument);
void DuckRaceMoney_Callback(void *data);
void UpdateDuckRacePlacementText(int replaySlot, int actor, int ticks);
void DuckDebugNullsub(void *data);
void CreateDuckRaceDust(void);
void UpdateDuckRaceDust(int actor, float speed);
void DuckEffectNullsub(void);
void DuckEffectUnusedNullsub(void);
void DuckObstacles(int configuration);
float DuckLerp(float target, float current, float rate);
short DuckBlendAngle(int target, int current, float rate);
unsigned short DuckYawBetween(const Vector3 *from, const Vector3 *to);
void DuckVoiceSequence_Callback(void *data);
void QueueDuckVoiceSequence(HLibTask *parent, int delay, unsigned int mode, int timer);
void SetDuckVoicePrefix(int prefix, const char *text);
void AddDuckVoice(int prefix, const char *suffix);
void PlayDuckVoice(int prefix, const char *suffix, int delay);
#else
#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2 {
struct EventReadCharactersWork;
}
namespace shenmue::s2::areas {
enum class DuckState : std::uint16_t {
    Init,
    IntroAuth,
    EntryDialogue,
    PayEntry,
    BettingIntro,
    InsufficientFunds,
    BetMenu,
    WagerInput,
    SelectDuck,
    PaddockView,
    Decline,
    PostRaceHold,
    Countdown,
    Results,
    Running,
    Cleanup
};
struct DuckEventWork {
    std::uint64_t reserved0;
    EventReadCharactersWork *npcLoad;
    DuckState state;
    std::uint16_t phase;
    std::int32_t wager, selection, reserved1c;
};
static_assert(sizeof(DuckEventWork) == 0x20 && offsetof(DuckEventWork, state) == 0x10);

std::int64_t DuckRaceRunBettingWinSequence(const DuckEventWork *work);
std::int64_t DuckRaceRunBettingLossSequence();
std::int64_t DuckRaceRunSpecialWinSequence();
std::int64_t DuckRaceRunSpecialLossSequence();
void AreaDUCKInit();
void AreaDUCKMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaDUCKScriptDefinition;
} // namespace shenmue::s2::areas
#endif
