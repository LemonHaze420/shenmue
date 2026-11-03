#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_queued_dc.h"
#include "s2/sk_event_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/areas/common/et_thr.h"
#define KNMN_AREA SHEN_NAME4('K', 'N', 'M', 'N')
#define KNMN_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define KNMN_KEM SHEN_NAME4('K', 'E', 'M', '_')
#define KNMN_DEKU SHEN_NAME4('D', 'E', 'K', 'U')
#define KNMN_INVALID 0xffffffffU

typedef struct KnmnOwnerWork {
    HLibTask *task, *child;
} KnmnOwnerWork;
typedef struct KnmnGameWork {
    HLibTask *task, *owner, *practiceTask;
    EventReadCharactersWork *characters;
    void *masterMotion;
    int reserved14[2];
    short state;
    char mode, round;
    unsigned int battleFrames;
} KnmnGameWork;
extern int knmnLeafEffect;
extern const shen_name knmnCharacters[];
extern const char knmnMasterMotion[], knmnAuthorPackage[], knmnPackage[], knmnAuthPackage[], knmnAuthor[], knmnFilename[], knmnReturnOverlay[];
void KnmnMakeGameCallback(void *);
void KnmnOwnerNullsub(void *);
void KnmnLoadPackages(KnmnGameWork *);
void KnmnStopCallback(void *);
void KnmnDebugEndCallback(void *);
void KnmnEndCallback(void *);
void KnmnWaitNpcCallback(void *);
void KnmnAutoCallback(void *);
void KnmnAuthFirstCallback(void *);
void KnmnAuthWazaCallback(void *);

extern const char knmnBlurModel[], knmnAudio[];
void KnmnInitBattleCallback(void *);
void KnmnInitButokuCallback(void *);
void KnmnWaitBattleRegularCallback(void *);
void KnmnWaitBattleFirstCallback(void *);
void KnmnWaitButokuCallback(void *);
void KnmnButokuCallback(void *);
void KnmnAuthFirstEndCallback(void *);
void KnmnAuthTomoCallback(void *);
void KnmnAuthShoukindaCallback(void *);
void KnmnInstallBattleDialogues(int line);
void KnmnInstallFirstEndConditions(void);
void KnmnInstallButokuEndConditions(void);
int KnmnTickFirstEndTimer(void);
int KnmnTickButokuEndTimer(void);
void KnmnRequestButokuBattleEnd(void);
HLibTask *KnmnQueuePractice(KnmnGameWork *);
int KnmnFinishPractice(HLibTask *);
void KnmnWazaFrameEvents(void *);

extern const char knmnWazaTitle[], knmnLeafDirectory[], knmnLeafTexture[];
void KnmnCreateLeafEmitter(int *effect, int count, float birthRate);
int KnmnInstallDialogueConditions(void *);
int KnmnResumeConditionalDialogues(void *);
int KnmnCycleVoice(const short *, unsigned short *, int);
int KnmnCyclePlayerFirstReactionVoice(void *);
int KnmnCyclePlayerSecondReactionVoice(void *);
int KnmnCyclePlayerEvadeVoice(void *);
int KnmnCycleOpponentHitVoice(void *);
int KnmnCyclePlayerMotionVoice(void *);
int KnmnCycleOpponentAttackVoice(void *);
int KnmnCyclePlayerAdviceVoice(void *);
int KnmnRequestFirstBattleEnd(void *);
int KnmnRequestButokuBattleEndCallback(void *);

typedef struct KnmnMoveResult {
    signed char status, phase, error, steps, force, timing, retained06, flags;
    int detail;
} KnmnMoveResult;
typedef struct TrainingLcdValues {
    int frames;
    const void *bitmap;
} TrainingLcdValues;
typedef struct TrainingFightState {
    unsigned char retained[8];
    TrainingLcdValues lcd;
    int fightSuspended;
} TrainingFightState;
typedef struct KnmnPracticeControl {
    int advicePending, timedAdvice, practiceWarning, practiceExpired;
    unsigned int repetitions, consecutiveRepetitions, previousRepetitions, attemptedMoves;
    int firstFailures[10], repeatedFailures[10];
    unsigned int restCount, conversationActive;
    int changeCamera, retained5a0, restartMotion;
    unsigned short demonstrationCamera, retained5aa;
    Thr *cameraSequence, *dialogueSequence, *timerSequence, *adviceSequence, *lcdSequence;
    int practice, result, resultIndex, practiceCamera;
    unsigned int elapsed, adviceElapsed, attempts;
    int repositionFrames[2];
    unsigned char retained5e4[0x18];
    TrainingFightState fight;
} KnmnPracticeControl;
typedef struct KnmnPracticeWork {
    Thr *sequence;
    HLibTask *task;
    int active, state;
    unsigned int frames;
    unsigned short camera, retained16;
    void *motions[16];
    unsigned char retained58[8];
    EventReadCharactersWork *characters;
    signed char learnedMove;
    unsigned char retained65;
    short resultFrames;
    KnmnMoveResult currentResult;
    KnmnMoveResult results[100];
    KnmnPracticeControl control;
    unsigned char retained610[0xc];
    unsigned char following, turning;
    unsigned char retained61e[0xa];
    unsigned char greeting;
    unsigned char retained629[3];
    KnmnGameWork *game;
    unsigned char retained630[0x1c];
} KnmnPracticeWork;
typedef struct KnmnFeedbackSelection {
    HLibTaskCallback callback;
    int argument;
} KnmnFeedbackSelection;
typedef char KnmnGameWorkSize[(sizeof(KnmnGameWork) == 0x24) ? 1 : -1];
typedef char KnmnPracticeWorkSize[(sizeof(KnmnPracticeWork) == 0x64c) ? 1 : -1];

void KnmnSetBattleDialogueFlags(void);

extern Vector3 knmnPlayerPracticePosition, knmnTeacherPracticePosition, knmnDummyPracticePosition;
extern int knmnPlayerPracticeYaw, knmnTeacherPracticeYaw, knmnDummyPracticeYaw;
extern unsigned char knmnPracticeLcd[194];
extern Thr *knmnQteDialogue;
void KnmnCheckPlayerPosition(int line);
HLibTask *KnmnQueuePractice(KnmnGameWork *game);
int KnmnFinishPractice(HLibTask *task);
void KnmnPracticeMainCallback(void *data);
void KnmnInitializeDemonstrationSequence(void *data);
void KnmnPrepareAuthorCharacters(KnmnPracticeWork *unused);
void KnmnRestoreAuthorCharacters(void);
void KnmnExitPracticeSequence(void *data);
KnmnFeedbackSelection *KnmnSelectFeedback(Thr *thr,KnmnFeedbackSelection *selection);
void KnmnFeedbackSequence(void *data);
int KnmnPracticeExitNullsub(Thr *thr,int practice);
void KnmnRepeatFeedbackSequence(void *data);
void KnmnSuccessSequence(void *data);
int KnmnNeedsFacingReset(int practice);
void KnmnPracticeRootSequence(void *data);
void KnmnPracticeSequence(void *data);
void KnmnPracticeCameraSequence(void *data);
int KnmnSelectPracticeCamera(KnmnPracticeWork *work,int practice);
void KnmnClearPracticeCamera(KnmnPracticeControl *control);
void KnmnSetTrainingCameraSequence(Thr *thr,unsigned int camera);
void KnmnResetActors(int practice);
void KnmnPracticeDecisionSequence(void *data);
void KnmnQteSequence(void *data);
void KnmnStartPracticeSequence(void *data);
void KnmnFinishPracticeSequence(void *data);
void KnmnAwardMoveSequence(void *data);
void KnmnFollowTeacher(void);
void KnmnFeedbackOpening(void *data);
void KnmnFeedbackClosing(void *data);
void KnmnQteFailureSequence(void *data);
void KnmnIdleAdviceSequence(void *data);
void KnmnStayAndStrikeSequence(void *data);
void KnmnRelaxShouldersSequence(void *data);
void KnmnRelaxShouldersLegsSequence(void *data);
void KnmnRelaxShouldersArmsSequence(void *data);
void KnmnRelaxWithoutGestureSequence(void *data);
void KnmnUseHandsSequence(void *data);
void KnmnUseLegsSequence(void *data);
void KnmnHandsSlowSequence(void *data);
void KnmnHandsLegsTogetherSequence(void *data);
void KnmnUnclassifiedFeedbackSequence(void *data);

#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/seqp_event.hpp"
#include <cstddef>
#include <cstring>
namespace shenmue::s2::areas {
inline constexpr shen_name KNMN_AREA{SHEN_NAME(KNMN)}, KNMN_TASK{SHEN_NAME(EVNT)}, KNMN_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name KNMN_KEM{SHEN_NAME(KEM_)}, KNMN_DEKU{SHEN_NAME(DEKU)}, KNMN_INVALID{0xffffffffU};
inline constexpr shen_name KNMN_CHARACTERS[]{KNMN_RYO, KNMN_KEM, KNMN_INVALID};
struct KnmnGameWork {
    HLib::Task *task, *owner, *practiceTask;
    EventReadCharactersWork *characters;
    std::uint32_t masterMotion;
    std::byte reserved24[8];
    std::uint16_t state;
    std::uint8_t mode, round;
    std::uint32_t battleFrames, reserved34;
};
static_assert(sizeof(KnmnGameWork) == 0x38 && offsetof(KnmnGameWork, state) == 0x2c);
struct KnmnMoveResult {
    std::int8_t status, phase, error, steps, force, timing, reserved06, flags;
    std::int32_t detail;
};
struct KnmnPracticeWork {
    CompiledSequenceWork *sequence;
    HLib::Task *task;
    std::int32_t active, state;
    std::uint32_t frames;
    std::uint16_t camera, reserved1e;
    std::uint32_t motions[16];
    std::byte reserved60[8];
    EventReadCharactersWork *characters;
    std::uint32_t reserved70;
    std::int8_t learnedMove;
    std::uint8_t reserved75;
    std::int16_t resultFrames;
    KnmnMoveResult results[100];
    std::byte reserved528[0x10];
    std::int32_t advicePending, timedAdvice, practiceWarning, practiceExpired;
    std::uint32_t repetitions, consecutiveRepetitions, previousRepetitions, attemptedMoves;
    std::int32_t firstFailures[10], repeatedFailures[10];
    std::uint32_t restCount, conversationActive;
    std::int32_t changeCamera, reserved5b4, restartMotion;
    std::uint16_t demonstrationCamera, reserved5be;
    CompiledSequenceWork *cameraSequence, *dialogueSequence, *timerSequence, *adviceSequence, *lcdSequence;
    std::int32_t practice, result, resultIndex, practiceCamera;
    std::uint32_t elapsed, adviceElapsed, attempts;
    std::int32_t repositionFrames[2];
    std::byte reserved60c[0x24];
    std::int32_t lcdFrames;
    std::uint32_t reserved634;
    const void *lcdBitmap;
    std::int32_t fightSuspended;
    std::byte reserved644[0xc];
    std::uint8_t reserved650, turning;
    std::byte reserved652[0xa];
    std::int32_t reserved65c;
    KnmnGameWork *game;
    std::byte reserved668[0x20];
};
static_assert(sizeof(KnmnPracticeWork) == 0x688 && offsetof(KnmnPracticeWork, results) == 0x78);
static_assert(offsetof(KnmnPracticeWork, advicePending) == 0x538 && offsetof(KnmnPracticeWork, game) == 0x660);
extern std::int32_t knmnLeafEffect, knmnEndRequested, knmnEndTimer;
extern std::uint32_t knmnConditionalDialogues[3];
extern CompiledSequenceWork *knmnQteDialogue;
void InitializeKnmnEvent();
void RunKnmnEvent(std::uintptr_t);
void KnmnAuthFirstCallback(void *);
void KnmnAutoCallback(void *);
void KnmnInitBattleCallback(void *);
void KnmnInitButokuCallback(void *);
void KnmnStopCallback(void *);
void KnmnButokuCallback(void *);
void KnmnWaitBattleFirstCallback(void *);
void KnmnWaitBattleRegularCallback(void *);
void KnmnWaitButokuCallback(void *);
void KnmnAuthFirstEndCallback(void *);
void KnmnAuthShoukindaCallback(void *);
void KnmnAuthTomoCallback(void *);
void KnmnEndCallback(void *);
void KnmnAuthWazaCallback(void *);
void KnmnWazaFrameEvents();
void KnmnPracticeMainCallback(void *);
void KnmnPracticeRootSequence(void *);
void KnmnStartPracticeSequence(void *);
void KnmnFinishPracticeSequence(void *);
void KnmnAwardMoveSequence(void *);
void KnmnInitializeDemonstrationSequence(void *);
void KnmnPracticeDecisionSequence(void *);
void KnmnPracticeSequence(void *);
void KnmnQteSequence(void *);
void KnmnFeedbackSequence(void *);
void KnmnExitPracticeSequence(void *);
void KnmnSuccessSequence(void *);
void KnmnQteFailureSequence(void *);
void KnmnResetActors(std::int32_t);
void KnmnInstallBattleDialogues();
void KnmnInstallFirstEndConditions();
void KnmnInstallButokuEndConditions();
inline void KnmnAdvance(CompiledSequenceWork &work, std::uint16_t mode) {
    work.previousMode = work.mode;
    work.mode = mode;
    work.continueAfterWait = 1;
    work.continueLoop = 1;
}
inline void KnmnWait(CompiledSequenceWork &work, std::uint16_t mode, std::uint16_t next) {
    work.mode = mode;
    work.nextMode = next;
    work.continueAfterWait = 1;
}
inline void KnmnWait(CompiledSequenceWork &work, std::uint16_t mode, std::uint16_t next, std::int32_t delay) {
    KnmnWait(work, mode, next);
    work.delay = delay;
}
template <class T> T KnmnValue(const void *data, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<const std::byte *>(data) + offset, sizeof(value));
    return value;
}
template <class T> void KnmnValue(void *data, std::size_t offset, T value) {
    std::memcpy(static_cast<std::byte *>(data) + offset, &value, sizeof(value));
}
CompiledSequenceWork *KnmnChild(CompiledSequenceWork &, HLib::TaskCallback, shen_name, std::int32_t);
void KnmnComplete(CompiledSequenceWork &, bool result = false);
void KnmnKillSequence(CompiledSequenceWork *);
extern const ScriptAreaDefinition knmnScriptDefinition;
} // namespace shenmue::s2::areas
#endif
