#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_debug_dc.h"
#include "s2/event_queued_dc.h"

#define ANAN_TASK SHEN_NAME4('E', 'V', 'N', 'T')
#define ANAN_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define ANAN_MAN SHEN_NAME4('A', 'N', 'A', '_')
#define ANAN_BOARD SHEN_NAME4('b', 'r', 'd', '_')
#define ANAN_STOOL SHEN_NAME4('A', '_', 'K', 'Y')
#define ANAN_BASE SHEN_NAME4('b', 'a', 's', 'e')
#define ANAN_BUTTON SHEN_NAME4('B', 'T', 'B', 'A')
#define ANAN_INVALID 0xffffffffU
#define ANAN_NPC_REGION SHEN_NAME4('c', 'c', 'e', 'a')
#define ANAN_FREE SHEN_NAME4('$', 'f', 'r', 'e')
#define ANAN_EXIT_AREA SHEN_NAME4('W', 'S', '0', '0')
#define ANAN_START_AUTHOR SHEN_NAME4('S', 'T', 'R', 'T')
#define ANAN_ANGRY_AUTHOR SHEN_NAME4('A', 'N', 'G', 'R')
#define ANAN_TIME_FAIL_AUTHOR SHEN_NAME4('T', 'O', 'F', '1')
#define ANAN_TIME_END_AUTHOR SHEN_NAME4('T', 'O', 'F', '2')
#define ANAN_SUCCESS_AUTHOR SHEN_NAME4('S', 'U', 'C', 'C')
#define ANAN_FAIL_AUTHOR SHEN_NAME4('F', 'A', 'I', 'L')
#define ANAN_FAIL_FIRST_AUTHOR SHEN_NAME4('F', 'A', 'L', '1')
#define ANAN_FAIL_END_AUTHOR SHEN_NAME4('F', 'A', 'L', '2')
#define ANAN_FIFTH_AUTHOR SHEN_NAME4('F', '5', '_', '1')
#define ANAN_FIFTH_END_AUTHOR SHEN_NAME4('F', '5', '_', '2')
#define ANAN_WORLD 0x40000000U
#define ANAN_WORLD_ALL_AXES 0x78000000U
#define ANAN_ALL_AXES 0x38000000U
#define ANAN_DEBUG_ID 2
#define ANAN_EPSILON 1.1920929e-07f
typedef struct AnanPlacement {
    shen_name character;
    Vector3 position;
    Vector3i rotation;
} AnanPlacement;
typedef struct AnanGame {
    unsigned int gameFlags;
    int idleFrames;
    float tilt;
    int failures;
    void *directionVoice;
    void *responseVoice;
    int endHoldFrames;
    short inputPulse;
    unsigned char inputHistory[6];
} AnanGame;
typedef struct AnanWork {
    HLibTask *caller;
    HLibTask *self;
    unsigned int lifecycle, entry;
    AnanGame game;
    HLibTask *ryoTask;
    HLibTask *ananTask;
    shen_name targets[2];
    Vector3 positions[2];
    int transitionFrames[2];
    int positionModes[2];
} AnanWork;
typedef struct AnanPakWork {
    HLibTask *caller;
    int npcState, specialEntry;
} AnanPakWork;
typedef struct AnanWaitWork {
    HLibTask *caller;
    int updatePose;
} AnanWaitWork;
typedef void (*AnanVoiceCallback)(int event, int argument);
typedef int (*AnanAuthorCallback)(int frame);
typedef struct AnanAuthWork {
    HLibTask *caller;
    AnanAuthorCallback frameCallback;
    int fadeOut, finishBeforeUnload;
    shen_name author;
    shen_name *characters;
    shen_name *parents;
} AnanAuthWork;
typedef struct AnanVoiceWork {
    HLibTask *parent;
    void *conversation;
    int startTicks, endTicks;
    AnanVoiceCallback callback;
    int voice;
    char text[4];
} AnanVoiceWork;
typedef struct AnanCameraWork {
    int camera;
    shen_name base;
    int delay;
    char package[4];
} AnanCameraWork;
extern AnanPlacement ananGamePlacement[];
extern AnanPlacement ananStoppedPlacement[];
extern shen_name ananCharacters[];
extern shen_name ananAuthorCharacters[];
extern Vector3 ananBoardOrigin;
extern Vector3i ananBoardRotation;
extern AnanPlacement ananFirstEntryPlacement[];
extern AnanPlacement ananFirstChoicePlacement[];
extern AnanPlacement ananDeclinePlacement[];
extern AnanPlacement ananRepeatEntryPlacement[];
extern AnanPlacement ananExplanationPlacement[];
extern AnanPlacement ananRetryPlacement[];
extern AnanPlacement ananAngryPlacement[];
extern AnanPlacement ananTimeOverPlacement[];
extern const char ananPackage[];
extern const char ananCreateTaskError[];
void RunAnan(int entry);
void AnanInitializeGame(void *data);
void AnanRestartGame(void *data);
void AnanRunGame(void *data);
int AnanUpdateDirectionVoice(AnanWork *work);
int AnanUpdateGame(AnanWork *work);
void AnanStopLift(int confirming, int fixedCamera);
void AnanRetry(void *data);
void AnanSetLiftPose(float tilt);
void AnanSetLiftFrame(float tilt);
void AnanSetTargets(float tilt, int hand, int body);
void AnanClearInput(AnanGame *game);
int AnanHistoryInput(AnanGame *game, int index);
void AnanReadInput(AnanGame *game);
float AnanInputStep(AnanGame *game);
void AnanUpdatePose(AnanGame *game);
void AnanAskStraight(void *data);
void AnanWaitStraight(void *data);
void QueueAnanWait(int updatePose);
void AnanWait(void *data);
int AnanIsStraight(AnanGame *game);
void AnanFinish(void *data);
void AnanFinishAuthor(void *data);
void AnanLoadBoard(void *data);
void AnanInitialize(void *data);
void AnanWaitForNpc(void *data);
void AnanBeginEntry(void *data);
void AnanBeginFirstChoice(void *data);
void AnanWaitFirstChoice(void *data);
void AnanExplain(void *data);
void AnanBeginAngry(void *data);
void AnanPlayAngryAuthor(void *data);
void AnanBeginTimeOver(void *data);
void AnanPlayTimeOverAuthor(void *data);
void AnanFinishTimeOver(void *data);
void AnanSuccess(void *data);
void AnanFailure(void *data);
void AnanOfferRetry(void *data);
void AnanWaitRetry(void *data);
void AnanFinishFailure(void *data);
void AnanFinishFifthFailure(void *data);
void AnanTrackRyo(void *data);
void AnanTrackMan(void *data);
void AnanDebugSkip(void *data);
void AnanHideCharacter(shen_name character);
void AnanShowCharacter(shen_name character);
int QueueAnanAuthor(shen_name author, AnanAuthorCallback frameCallback, shen_name *characters, int fadeOut, int finishBeforeUnload);
void AnanInitializeAuthor(void *data);
void AnanWaitAuthor(void *data);
void QueueAnanVoice(const char *text, int voice, int startTicks, int endTicks, AnanVoiceCallback callback);
void AnanVoiceLoop(void *data);
void AnanVoiceResumeCaller(int event, int argument);
void AnanVoiceUnlockInput(int event, int argument);
void QueueAnanCamera(const char *package, int camera, shen_name base, int delay);
void AnanCamera(void *data);
void AnanSetBoardTilt(float tilt);
void AnanGetBoardRotation(Vector3i *rotation, float tilt);
void AnanGetHandPosition(float tilt, Vector3 *output);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct ConversationBuffer;
}
namespace shenmue::s2::areas {
inline constexpr shen_name ANAN_AREA{SHEN_NAME(ANAN)}, ANAN_EVENT{SHEN_NAME(EVNT)}, ANAN_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name ANAN_MAN{SHEN_NAME(ANA_)}, ANAN_BOARD{SHEN_NAME(brdX) | 0x07000000U}, ANAN_STOOL{SHEN_NAME(A_KY)};
inline constexpr shen_name ANAN_BASE{SHEN_NAME(base)}, ANAN_FREE{SHEN_NAME($fre)}, ANAN_INVALID{0xffffffffU};
inline constexpr shen_name ANAN_NPC_REGION{SHEN_NAME(ccea)}, ANAN_BUTTON{SHEN_NAME(BTBA)}, ANAN_EXIT_AREA{SHEN_NAME(WS00)};
struct AnanWork {
    HLib::Task *caller;
    HLib::Task *self;
    std::uint32_t lifecycle, entry;
    std::uint32_t gameFlags;
    std::int32_t idleFrames;
    float tilt;
    std::int32_t failures;
    ConversationBuffer *directionVoice;
    ConversationBuffer *responseVoice;
    std::int32_t endHoldFrames;
    std::int16_t inputPulse;
    std::uint8_t inputHistory[6];
    std::uint32_t reserved44;
    HLib::Task *ryoTask;
    HLib::Task *ananTask;
    shen_name targets[2];
    Vector3 positions[2];
    std::int32_t transitionFrames[2];
    std::int32_t positionModes[2];
};
struct AnanPakWork {
    HLib::Task *caller;
    std::uint64_t npcState;
    std::int32_t specialEntry, reserved14;
};
struct AnanAuthWork {
    HLib::Task *caller;
    std::uint32_t (*frameCallback)(std::int32_t);
    std::int32_t fadeOut, finishBeforeUnload;
    shen_name author;
    std::uint32_t reserved1c;
    shen_name *characters;
    shen_name *parents;
};
using AnanVoiceCallback = void (*)(std::int64_t, std::intptr_t);
#pragma pack(push, 4)
struct AnanVoiceWork {
    HLib::Task *caller;
    ConversationBuffer *dialogue;
    std::int32_t startDelay, remaining;
    AnanVoiceCallback callback;
    std::int32_t mode;
};
struct AnanWaitWork {
    HLib::Task *caller;
    std::int32_t updatePose;
};
#pragma pack(pop)
struct AnanCameraWork {
    std::int32_t camera;
    shen_name target;
    std::int32_t delay;
    std::uint8_t packageBytes[21];
};
static_assert(sizeof(AnanWork) == 0x88 && offsetof(AnanWork, gameFlags) == 0x18 && offsetof(AnanWork, tilt) == 0x20);
static_assert(offsetof(AnanWork, self) == 0x08 && offsetof(AnanWork, lifecycle) == 0x10 && offsetof(AnanWork, entry) == 0x14);
static_assert(offsetof(AnanWork, directionVoice) == 0x28 && offsetof(AnanWork, responseVoice) == 0x30 && offsetof(AnanWork, endHoldFrames) == 0x38);
static_assert(offsetof(AnanWork, inputPulse) == 0x3c && offsetof(AnanWork, inputHistory) == 0x3e && offsetof(AnanWork, ryoTask) == 0x48);
static_assert(offsetof(AnanWork, targets) == 0x58 && offsetof(AnanWork, positions) == 0x60);
static_assert(offsetof(AnanWork, transitionFrames) == 0x78 && offsetof(AnanWork, positionModes) == 0x80);
static_assert(sizeof(AnanPakWork) == 0x18 && offsetof(AnanPakWork, specialEntry) == 0x10);
static_assert(sizeof(AnanAuthWork) == 0x30 && offsetof(AnanAuthWork, characters) == 0x20);
static_assert(sizeof(AnanVoiceWork) == 0x24 && offsetof(AnanVoiceWork, callback) == 0x18 && sizeof(AnanWaitWork) == 12);
static_assert(offsetof(AnanCameraWork, packageBytes) == 12);
void InitializeAnan();
void RunAnan(std::uintptr_t);
void AnanInitialize(void *);
void AnanWaitForNpc(void *);
void AnanBeginEntry(void *);
void AnanLoadBoard(void *);
void AnanTrackRyo(void *);
void AnanTrackMan(void *);
void AnanFinish(void *);
void AnanFinishAuthor(void *);
void AnanBeginFirstChoice(void *);
void AnanWaitFirstChoice(void *);
void AnanExplain(void *);
void AnanBeginAngry(void *);
void AnanPlayAngryAuthor(void *);
void AnanBeginTimeOver(void *);
void AnanPlayTimeOverAuthor(void *);
void AnanFinishTimeOver(void *);
void AnanSuccess(void *);
void AnanFailure(void *);
void AnanOfferRetry(void *);
void AnanWaitRetry(void *);
void AnanFinishFailure(void *);
void AnanFinishFifthFailure(void *);
void AnanRetry(void *);
void AnanInitializeGame(void *);
void AnanRestartGame(void *);
void AnanRunGame(void *);
void AnanAskStraight(void *);
void AnanWaitStraight(void *);
void AnanInitializeAuthor(void *);
void AnanWaitAuthor(void *);
bool QueueAnanAuthor(shen_name, std::int32_t, std::int32_t);
void QueueAnanVoice(const char *, std::int32_t, std::int32_t, AnanVoiceCallback);
void AnanVoiceLoop(void *);
void AnanVoiceResumeCaller(std::int64_t, std::intptr_t);
void AnanVoiceUnlockInput(std::int64_t, std::intptr_t);
void QueueAnanCamera(std::int32_t, std::int32_t);
void AnanCamera(void *);
void QueueAnanWait();
void AnanWait(void *);
void AnanResetHelp();
void AnanSuspendCharacter(shen_name);
void AnanGetHandPosition(float, Vector3 *);
void AnanSetLiftPose(float);
void AnanSetLiftFrame(float);
void AnanUpdatePose(AnanWork *);
void AnanSetTargets(bool);
void AnanStopLift(std::int32_t, std::int32_t);
std::uint32_t AnanHistoryInput(const AnanWork *, std::int32_t);
void AnanReadInput(AnanWork *);
float AnanInputStep(AnanWork *);
void AnanUpdateDirectionVoice(AnanWork *);
std::int32_t AnanUpdateGame(AnanWork *);
bool AnanIsStraight(const AnanWork *);
extern const ScriptAreaDefinition ananScriptDefinition;
} // namespace shenmue::s2::areas
#endif
