#pragma once

#include "s1/date_time.hpp"
#include "s1/face.hpp"
#include "s1/task/char.hpp"
#include "s1/hlib.hpp"
#include "s1/task/telm.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s1 {

bool GetFlag(std::uint32_t flag);
struct ScenarioRegisters;
struct ScenarioContext;
struct ScenarioWindow;
struct PlayerControlWork;
struct ScenarioHeader;
struct ScenarioFunctionWork;
struct CVWK;
struct ScenarioSequence;
struct PadCaptureEntry;

using LegacyScenarioFunction = std::uint32_t (*)(ScenarioRegisters *);
using GeneralScenarioFunction = std::uint32_t (*)(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
using ShenmueScenarioFunction = void (*)(ScenarioContext *, void *);
using SCNFScenarioFunction = std::int64_t (*)(ScenarioContext *, void *);
using DoorScenarioFunction = void (*)(ScenarioContext *, void *);

inline constexpr std::size_t kLegacyScenarioFunctionCount = 1;
inline constexpr std::size_t kGeneralScenarioFunctionCount = 5;
inline constexpr std::size_t kShenmueScenarioFunctionCount = 467;
inline constexpr std::size_t kSCNFScenarioFunctionCount = 50;
inline constexpr std::size_t kDoorScenarioFunctionCount = 8;

extern std::array<LegacyScenarioFunction, kLegacyScenarioFunctionCount> LegacyFunctions;
extern std::array<GeneralScenarioFunction, kGeneralScenarioFunctionCount> GeneralPurposeFuncs;
extern std::array<ShenmueScenarioFunction, kShenmueScenarioFunctionCount> ShenmueScriptFuncs;
extern std::array<SCNFScenarioFunction, kSCNFScenarioFunctionCount> SCNF_Functions;
extern std::array<DoorScenarioFunction, kDoorScenarioFunctionCount> DSWKFunctions;
extern ScenarioHeader *g_pSCN_INFO_TaskData;

void SetStrForScnArg(ScenarioContext *context, std::int64_t offset, std::uint32_t opcode, ScenarioRegisters *registers);
void ExecuteRemovedScriptFn(std::int32_t index, ScenarioContext *context, std::int64_t unused, void *params);
std::int64_t ExecuteMemScriptFn(std::int32_t index, void *params);
void SetStatOrNotebookFlag(std::uint16_t flag, bool memo);
void SetShouldSleepFlag(std::uint8_t mode);


std::int32_t AcquireScenarioRelationQueue(std::uint32_t tag);
std::int32_t EnqueueScenarioRelationData(std::uint32_t handle, const void *source, std::int32_t size);
std::int32_t DequeueScenarioRelationData(std::uint32_t handle, void *destination, std::int32_t capacity);
std::int64_t ExecuteShenmueScriptFn(ScenarioContext *context, std::int32_t index, void *params);
std::int64_t ExecuteSCNFScriptFn(ScenarioContext *context, std::uint32_t index, std::int64_t stackPointer, std::int32_t argumentCount);
std::int64_t ExecuteDoorSupportFunc(ScenarioContext *context, std::uint32_t index, void *params);

#pragma pack(push, 1)
struct ScenarioTaskData {
    std::uint32_t name;
    std::uint32_t reserved_004;
    ScenarioWindow *scenarioWindow;
    ScenarioFunctionWork *scenarioFunctions;
    ScenarioSequence *scenarioConstants;
    CVWK *voiceWork;
    CharacterIdLookupNode *characterIdRoot;
    std::array<CharacterIdLookupNode *, 13> characterBuckets;
    std::byte reserved_098[0x220];
};
#pragma pack(pop)

static_assert(sizeof(ScenarioTaskData) == 0x2b8);
static_assert(offsetof(ScenarioTaskData, characterIdRoot) == 0x28);
static_assert(offsetof(ScenarioTaskData, characterBuckets) == 0x30);

HLib::Task *SCEN_Init();
bool SCEN_LoadCurrentMapScript(ScenarioWindow *window);
void SCN3_Process(const void *mapInfo);
void SCEN_Callback(HLib::TaskData *taskData);
void SCEN_ActivateFreeTalkRuntime();
void SCEN_CreateCharactersForSequence();
Character *SCEN_FindCharacterByFunctionId(ScenarioContext *context, std::uint32_t identifier);
bool SCEN_IsInteractionModalActive() noexcept;
bool SCEN_IsConversationCameraActive() noexcept;
bool SCEN_BlocksDoorCandidate() noexcept;
void SCEN_SetConversationPromptEnabled(bool enabled) noexcept;
bool SCEN_IsConversationPromptEnabled() noexcept;
std::uint8_t DetermineActNum() noexcept;
extern std::int32_t g_pendingSleepResumeLoad;
void SCEN_ResetTimedTransitions();
void SCEN_UpdateTimedTransitionScheduler();

extern std::uint8_t g_timedTransitionUpcoming;
extern GameDateTime g_timedTransitionUpcomingTime;
void EnqueueSCNT(std::uint32_t name, std::int16_t stateValue);
bool SCEN_IsRuntimeObjectBusy() noexcept;
std::uint32_t SCEN_GetActiveRuntimeObjectName() noexcept;
bool SCEN_HasActiveRuntimeObject() noexcept;
std::uint32_t SCEN_GetActiveRuntimeObjectFlags() noexcept;
void SCEN_UpdatePlayerSleepScheduler(PlayerControlWork &control);
bool SCEN_HasActiveScript() noexcept;
const std::array<std::uint32_t, 9> &SCEN_GetSearchCharacterList() noexcept;
std::span<const std::uint32_t> SCEN_ReferencedCharacterIds() noexcept;
std::uint32_t scenGetFirstNonRyoChara() noexcept;
extern std::uint32_t g_freeTalkInitialized;


extern std::int16_t g_debug_talk_status;
bool SCEN_HasRuntimeObject(std::uint32_t characterName) noexcept;
std::size_t SCEN_CopyFreeTalkParticipants(std::uint32_t characterName, std::uint32_t *participants, std::size_t capacity) noexcept;
const Vector3 *SCEN_GetFreeTalkLocation(std::uint32_t characterName, std::size_t locationIndex) noexcept;
bool SCEN_IsFreeTalkCharacterContextOwned(std::uint32_t characterName) noexcept;
bool SCEN_AreFreeTalkConstraintsDisabled() noexcept;
void SCEN_ClearFreeTalkRuntimeFlag() noexcept;

} // namespace shenmue::s1

#pragma once

#include "s1/hlib.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

#pragma pack(push, 1)
struct ScenarioHeader {
    std::int32_t token;
    std::int32_t wholeSize;
    std::int32_t flags;
    std::int32_t functionOffset;
    std::int32_t dataSegmentOffset;
    std::int32_t lastBlockOffset;
    std::int32_t secondFunctionOffset;
    std::int32_t unknownOffset;
    std::int32_t entrypoint;
    std::int32_t duplicateLastBlockOffset;
    std::int32_t unknown1;
    std::int32_t unknown2;
};

struct ScenarioRegisters {
    std::uint32_t r0;
    std::uint32_t r1;
    std::uint32_t r2;
    std::uint32_t r3;
};

struct ScenarioBlock {
    shen_name actor;
    std::uint32_t entryIndex;
};

struct ScenarioStack {
    std::byte *instruction;
    ScenarioRegisters *stackPointer;
    ScenarioRegisters *framePointer;
    std::uint32_t accumulator;
    std::byte frame[524];
    std::uint16_t previousOperand;
    std::uint16_t trailingFlags;
};

struct ScenarioContext;
using SetScenarioStringArgumentFunction = void (*)(ScenarioContext *, std::int64_t, std::uint32_t, ScenarioRegisters *);
using RemovedScenarioFunctionExecutor = void (*)(std::int32_t, ScenarioContext *, std::int64_t, void *);
using MemoryScenarioFunctionExecutor = std::int64_t (*)(std::int32_t, void *);
using GameScenarioFunctionExecutor = std::int64_t (*)(ScenarioContext *, std::int32_t, void *);
using ScenarioFunctionExecutor = std::int64_t (*)(ScenarioContext *, std::uint32_t, std::int64_t, std::int32_t);
using DoorScenarioFunctionExecutor = std::int64_t (*)(ScenarioContext *, std::uint32_t, void *);

struct ScenarioWindow {
    HLib::Task *task;
    ScenarioHeader *info;
    ScenarioStack *stacks;
    std::uint32_t *entryOffsets;
    std::int32_t entryCount;
    std::uint32_t reserved024;
    ScenarioHeader *activeInfo;
    std::byte *stackLimit;
    std::byte *stackBase;
    std::byte *stackEnd;
    std::uint32_t stackSize;
    std::uint32_t reserved04c;
    ScenarioHeader *functionInfo;
    std::byte *dataSegment;
    std::uint64_t reserved060;
    std::byte *alternateStackLimit;
    std::byte *alternateStackBase;
    std::byte reserved078[0x10];
    SetScenarioStringArgumentFunction setStringArgument;
    RemovedScenarioFunctionExecutor executeRemovedFunction;
    MemoryScenarioFunctionExecutor executeMemoryFunction;
    GameScenarioFunctionExecutor executeGameFunction;
    ScenarioFunctionExecutor executeScenarioFunction;
    DoorScenarioFunctionExecutor executeDoorFunction;
    std::byte reserved0b8[0x18];
    std::byte *entrypoint;
    std::byte workspace[9600];
};
#pragma pack(pop)

struct ScenarioContext {
    ScenarioWindow *task;
    ScenarioStack *stack;
};




struct ScenTimelineEntry {
    float time;
    std::int32_t actorOrTask;
    void *payload;
    std::uint32_t command;
    std::uint32_t secondaryActor;
};

struct ScenarioFunctionWork {
    std::uint32_t pendingDeltaReset;
    std::uint32_t reserved004;
    HLib::Task *scenarioTask;
    std::uint8_t active;
    std::byte reserved011[3];
    std::uint32_t flags;
    std::uint8_t scntVariable18;
    std::uint8_t scntVariable19;
    std::byte reserved01a[6];
    void *field20;
    void *field28;
    ScenTimelineEntry *timelineCursor;
    std::uint32_t activeObjectName;
    std::uint32_t previousObjectName;
    Character *characterHandler;
    ScenarioHeader *scenarioChunk;
    PadCaptureEntry *currentPadEntry;
    PadCaptureEntry *restorePadEntry;
    std::int32_t scnaRelationHandle;
    std::int32_t scnfRelationHandle;
    std::byte runtime068[0x1c68];
};

static_assert(sizeof(ScenarioHeader) == 0x30);
static_assert(sizeof(ScenarioBlock) == 8);
static_assert(sizeof(ScenarioStack) == 0x22c);
static_assert(offsetof(ScenarioStack, accumulator) == 0x18);
static_assert(offsetof(ScenarioStack, frame) == 0x1c);
static_assert(offsetof(ScenarioStack, previousOperand) == 0x228);
static_assert(offsetof(ScenarioStack, trailingFlags) == 0x22a);
static_assert(sizeof(ScenarioWindow) == 0x2658);
static_assert(offsetof(ScenarioWindow, stacks) == 0x10);
static_assert(offsetof(ScenarioWindow, activeInfo) == 0x28);
static_assert(offsetof(ScenarioWindow, dataSegment) == 0x58);
static_assert(offsetof(ScenarioWindow, setStringArgument) == 0x88);
static_assert(offsetof(ScenarioWindow, entrypoint) == 0xd0);
static_assert(offsetof(ScenarioWindow, workspace) == 0xd8);
static_assert(sizeof(ScenTimelineEntry) == 0x18);
static_assert(offsetof(ScenTimelineEntry, payload) == 0x08);
static_assert(offsetof(ScenTimelineEntry, command) == 0x10);
static_assert(sizeof(ScenarioFunctionWork) == 0x1cd0);
static_assert(offsetof(ScenarioFunctionWork, timelineCursor) == 0x30);
static_assert(offsetof(ScenarioFunctionWork, active) == 0x10);
static_assert(offsetof(ScenarioFunctionWork, scenarioTask) == 0x08);
static_assert(offsetof(ScenarioFunctionWork, scenarioChunk) == 0x48);
static_assert(offsetof(ScenarioFunctionWork, scnaRelationHandle) == 0x60);
static_assert(offsetof(ScenarioFunctionWork, restorePadEntry) == 0x58);



struct ScenRuntimeKVPair {
    std::int32_t key;
    std::int32_t value;
};

#pragma pack(push, 1)
struct ScenRuntimeObject {
    void *hashNext;
    shen_name name;
    std::uint8_t field0c;
    std::uint8_t runtimeState;
    std::int8_t runtimeStateValue;
    std::uint8_t byte0f;
    std::uint8_t byte10;
    std::uint8_t byte11;
    std::uint8_t byte12;
    std::uint8_t pad13;
    std::int32_t field14;
    std::uint8_t participantCount;
    std::int8_t stateByte19;
    std::uint8_t locationCount;
    std::int8_t kvPairCount;
    std::uint16_t scriptSlot;
    std::uint16_t pad1e;
    std::uint32_t runtimeFlags;
    std::uint16_t field24;
    std::int16_t gap26;
    std::int16_t field28;
    std::byte pad2a[6];
    void *participants;
    Vector3 *locations;
    Vector3 *data40;
    void *matrix;
    char *voicePath;
    ScenRuntimeKVPair *kvPairs;
    std::byte *scriptStart;
    void *lineTable;
    void *activeLineOrMarker;
    void *field78;
    std::int32_t subtitleSpeakerClass;
    std::byte pad84[4];
    void *fileHeader;
    std::byte *instructionPtr;
    std::byte *savedInstructionPtr;
    std::byte *alternateInstructionPtr;
    std::byte *relocatedPtrA8;
    std::uint8_t rangeMode;
    std::uint8_t padB1;
    std::int16_t rangeMin;
    std::int16_t rangeMax;
    std::uint16_t failCount1;
    std::uint16_t failCount2;
    std::uint16_t failCount3;
    std::uint16_t waitCount;
    std::byte padBE[2];
};
#pragma pack(pop)

static_assert(sizeof(ScenRuntimeObject) == 0xc0);

struct ScenRuntimeState {
    std::int32_t objectCount{};
    std::int32_t sequenceObjectCount{};
    ScenRuntimeObject *activeObject{};
    std::array<ScenRuntimeObject, 24> objects{};
    std::array<ScenRuntimeObject, 12> sequenceObjects{};
    std::array<std::uint32_t, 32> referencedCharacterIds{};
    std::array<float, 0x145 * 3> rangeValues{};
    std::uint32_t flags{};
    std::uint32_t activeObjectName{};
    std::uint32_t previousObjectName{};
    std::int32_t conditionResult{};
    std::int32_t randomBranchCount{};
    std::array<std::int32_t, 3> randomBranchValues{};
    std::array<std::int16_t, 4> actionSelectorBranchTokens{};
    std::array<std::uint32_t, 24> dynamicSlotNames{};
    std::int32_t dynamicSlotCursor{-1};
    std::uint32_t focusBestAngle{};
    float focusBestDistance{1000.0F};

    std::array<ScenTimelineEntry, 40> timelineEntries{};

    std::array<Character *, 24> referencedObjects{};

    std::int32_t referencedCharacterCount{};
    std::int32_t referencedObjectCount{};

    std::array<FaceLipCue, 40> generatedLipCues{};
};

extern ScenRuntimeState g_scenRuntimeState;
extern ScenarioFunctionWork *g_scenarioFunctionWork;
extern std::uint32_t g_scenHelpPromptOverride;

ScenarioWindow *ASEQ_SCNW_Init(HLib::Task *scenarioTask);
ScenarioStack *scenAllocateScntForScript(
    ScenarioWindow *scenarioWindow, ScenarioStack *parent, const void *entryInstruction, const std::uint32_t *arguments, std::int32_t argumentCount);
std::uint32_t SCN_Parse(ScenarioContext *context, ScenarioStack *stack, ScenarioRegisters *controlBase, std::byte *frameBase, std::byte *scratch);
std::uint32_t scenParseByPtr(ScenarioContext *context);
std::uint32_t scenParseByPtr2Frames(ScenarioWindow *scenarioWindow, ScenarioStack *stack);
void scenParseStackRecursively(ScenarioWindow *scenarioWindow);
void SCEN_ParseRecursive();
ScenarioStack *Startup_EventBase(ScenarioWindow *scenarioWindow, ScenarioHeader *info);

std::int16_t SCEN_GetCurrentScenarioStep() noexcept;
void SCEN_SetCurrentScenarioStep(std::int16_t step) noexcept;
std::uint8_t SCEN_GetCurrentHelpToken() noexcept;
void SCEN_SetHelpToken(std::uint8_t token, std::uint8_t button = 0) noexcept;

} // namespace shenmue::s1

#pragma once

#include "s1/hlib.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

#pragma pack(push, 1)
struct AnimatedMovementFunctionData {
    std::uint32_t characterId;
    std::uint32_t movementIndex;
};

struct AnimatedMovementRuntime {
    std::uint32_t characterId;
    std::uint16_t flags;
    std::uint8_t reserved006;
    std::uint8_t delay;
    std::byte reserved008[0x10];
    Vector3 savedPosition;
    Vector3i savedEuler;
    Vector3 savedRoot;
    std::uint32_t savedInterpolationFlags;
    std::int32_t frame;
    std::uint32_t reserved044;
    FcvSplineReader channels[9];
};

struct AnimatedMovementSample {
    Vector3 position;
    Vector3i euler;
    Vector3 root;
};

static_assert(sizeof(AnimatedMovementFunctionData) == 8);
static_assert(sizeof(AnimatedMovementRuntime) == 0x1f8);
static_assert(offsetof(AnimatedMovementRuntime, savedPosition) == 0x18);
static_assert(offsetof(AnimatedMovementRuntime, channels) == 0x48);
static_assert(sizeof(AnimatedMovementSample) == 0x24);
struct ScenarioSequenceHeader {
    HLib::Task *scenarioTask;
    std::uint8_t active;
    std::uint8_t pendingTrackSwitch;
    std::uint8_t needsNextFrame;
    std::uint8_t unknown0b;
    std::int16_t currentTrackIndex;
    std::int16_t unknown0e;
    void *track;
    void *sequence;
    void *camera;
    void *movement;
    void *stringData;
    void *lipData;
};

struct ScenarioSequence {
    ScenarioSequenceHeader header;
    void *sequenceCursor;
    std::uint64_t reserved048;
    std::int32_t frameIndex;
    float frameTime;
    std::int32_t frameCount;
    std::int32_t updateMode;
    float skipPromptTimer;
    std::int32_t delayFrames;
    std::int32_t delayLatched;
    std::int32_t skipRequested;
    float voiceTimer;
    std::int32_t reserved074;
    void *voiceCharacter;
    std::int32_t scriptScratch;
    std::int32_t soundAngleTimers[17];
    struct FunctionSlot {
        std::uint32_t functionIndex;
        std::uint32_t reserved004;
        std::int64_t (*function)(std::int32_t, void *, void *);
        void *scriptData;
        void *runtimeState;
    } functionSlots[70];
    std::byte reserved988[0x20];
    struct RuntimeState {
        std::uint32_t frameId;
        std::uint8_t flags;
        std::byte reserved005[3];
        std::uint16_t counter;
        std::byte data00a[0x44a];
    } runtimeStates[35];
    std::byte runtimeClearTail[0x454];
    std::uint16_t inputMask;
    std::uint16_t reservedA57a;
    std::int32_t updateRepeatCount;
    std::int32_t reservedA580;
    std::byte tailA584[0x1c];
};

struct ScnfRecordHeader {
    char name[4];
    std::uint32_t size;
    std::uint8_t participantCount;
    std::uint8_t reserved09;
    std::uint8_t locationCount;
    std::uint8_t kvPairCount;
    std::uint32_t reserved0c;
    std::uint32_t voicePathOffset;
    std::uint32_t scriptOffset;
    std::uint32_t lineTableOffset;
    std::uint32_t reserved1c;
    std::uint32_t participantBlockOffset;
    std::uint32_t data40Offset;
    std::uint32_t locationTableOffset;
    std::uint32_t kvTableOffset;
};

struct ScenarioTrackSlot {
    void *fileBlock;
    void *track;
    void *movementPayload;
    std::uint32_t loaded;
    std::uint32_t enabled;
};
#pragma pack(pop)

static_assert(sizeof(ScenarioSequenceHeader) == 0x40);
static_assert(offsetof(ScenarioSequence, frameCount) == 0x58);
static_assert(offsetof(ScenarioSequence, soundAngleTimers) == 0x84);
static_assert(offsetof(ScenarioSequence, functionSlots) == 0xc8);
static_assert(offsetof(ScenarioSequence, runtimeStates) == 0x9a8);
static_assert(offsetof(ScenarioSequence, inputMask) == 0xa578);
static_assert(offsetof(ScenarioSequence, updateRepeatCount) == 0xa57c);
static_assert(sizeof(ScenarioSequence::FunctionSlot) == 0x20);
static_assert(sizeof(ScenarioSequence::RuntimeState) == 0x454);
static_assert(sizeof(ScenarioSequence) == 0xa5a0);
static_assert(sizeof(ScenarioTrackSlot) == 0x20);
static_assert(sizeof(ScnfRecordHeader) == 0x30);

extern ScenarioSequence *g_pSCNC;
extern std::array<ScenarioTrackSlot, 70> g_SCNCTrackSlots;
extern std::int32_t g_SCNCSelectedTrackIndex;
extern std::int32_t g_scenarioTaskDeltaTime;
bool SCEN_IsConversationCameraOverrideActive() noexcept;
bool SCEN_TryFinalizeReadyRuntimeObject() noexcept;
void SCEN_FinalizeReadyPlayerControlRuntimeObjects() noexcept;

ScenarioSequence *ASEQ_SCNC_Init(HLib::Task *scenarioTask);
void ASEQ_InitCameraTrackPointers(const void *scenarioExtensionChunk);
bool SEQ_ParseInternal(const void *track);
void ASEQ_ApplyCameraKeyframes(ScenarioSequence *sequence);
void SEQ_SetAutomatedSkip(bool enabled);
void SEQ_ParseFrame(ScenarioSequence *sequence);
void SEQ_Cleanup();



struct ScenarioStepTable {
    std::uint8_t steps[32];
    std::uint32_t notebookJinglePending;
    std::uint32_t pendingCharacterName;
    std::uint8_t bitFlags64[8];
    std::uint32_t notebookJingleDelayFrames;
    std::uint32_t reserved34[19];
};
static_assert(sizeof(ScenarioStepTable) == 0x80);
static_assert(offsetof(ScenarioStepTable, notebookJinglePending) == 0x20);
static_assert(offsetof(ScenarioStepTable, pendingCharacterName) == 0x24);
static_assert(offsetof(ScenarioStepTable, bitFlags64) == 0x28);
static_assert(offsetof(ScenarioStepTable, notebookJingleDelayFrames) == 0x30);
extern ScenarioStepTable g_scenarioStepLut;

bool SCEN_IsPlayerAllowed();


void charaGetPosForNode(Character *character, std::int32_t nodeId, Vector3 *output);

inline constexpr std::array<std::uint32_t, 301> kValidScenCharacterNames{{SHEN_NAME(AKIR), SHEN_NAME(SORY), SHEN_NAME(IWAO), SHEN_NAME(JAKR), SHEN_NAME(FUKU),
    SHEN_NAME(INE_), SHEN_NAME(KISY), SHEN_NAME(TAIJ), SHEN_NAME(CHAI), SHEN_NAME(KURA), SHEN_NAME(KURB), SHEN_NAME(KAME), SHEN_NAME(JIMY), SHEN_NAME(TONY),
    SHEN_NAME(SMTH), SHEN_NAME(AKMI), SHEN_NAME(YOKO), SHEN_NAME(YUKK), SHEN_NAME(SYZU), SHEN_NAME(JUKY), SHEN_NAME(OISI), SHEN_NAME(NAMS), SHEN_NAME(SETA),
    SHEN_NAME(MTUK), SHEN_NAME(HORS), SHEN_NAME(SNKC), SHEN_NAME(YOSE), SHEN_NAME(MRIG), SHEN_NAME(NITO), SHEN_NAME(TSUC), SHEN_NAME(HATO), SHEN_NAME(ITOI),
    SHEN_NAME(TURU), SHEN_NAME(KURI), SHEN_NAME(AOKI), SHEN_NAME(MADA), SHEN_NAME(ETKO), SHEN_NAME(BUSS), SHEN_NAME(TOM_), SHEN_NAME(HIRA), SHEN_NAME(HISA),
    SHEN_NAME(YURI), SHEN_NAME(NANS), SHEN_NAME(HRKO), SHEN_NAME(AKSK), SHEN_NAME(MRUA), SHEN_NAME(HOND), SHEN_NAME(ONRR), SHEN_NAME(TATM), SHEN_NAME(KYUR),
    SHEN_NAME(UNO_), SHEN_NAME(KYOH), SHEN_NAME(ASDA), SHEN_NAME(YAMA), SHEN_NAME(MISM), SHEN_NAME(KOND), SHEN_NAME(RNKT), SHEN_NAME(NORK), 0x00000000u,
    SHEN_NAME(YNHS), SHEN_NAME(KKBN), SHEN_NAME(YOPA), SHEN_NAME(CATM), SHEN_NAME(KJIY), 0x00000000u, SHEN_NAME(EIIU), SHEN_NAME(PEDR), SHEN_NAME(PAUR),
    SHEN_NAME(TOM1), SHEN_NAME(KEBA), SHEN_NAME(KEBB), SHEN_NAME(KEBC), SHEN_NAME(KEBD), SHEN_NAME(STP1), SHEN_NAME(SAGA), SHEN_NAME(MSYU), SHEN_NAME(RYBI),
    SHEN_NAME(KYAS), SHEN_NAME(SIIN), SHEN_NAME(SKGK), 0x00000000u, SHEN_NAME(KIM_), 0x00000000u, SHEN_NAME(MIKM), SHEN_NAME(NSMR), SHEN_NAME(TKYM),
    SHEN_NAME(MTUR), SHEN_NAME(EDST), SHEN_NAME(MYKN), SHEN_NAME(TMNY), SHEN_NAME(MYTY), SHEN_NAME(HROT), SHEN_NAME(KYKW), SHEN_NAME(MWAK), SHEN_NAME(MKII),
    SHEN_NAME(HREO), SHEN_NAME(SCKO), SHEN_NAME(EIKK), SHEN_NAME(KUKT), SHEN_NAME(JOHN), 0x00000000u, SHEN_NAME(CMAL), SHEN_NAME(SATM), SHEN_NAME(BIKA),
    SHEN_NAME(BIKB), SHEN_NAME(TMMR), SHEN_NAME(MEYS), SHEN_NAME(TYHG), SHEN_NAME(KJHS), SHEN_NAME(TYMK), 0x00000000u, 0x00000000u, SHEN_NAME(SIND),
    SHEN_NAME(MASR), SHEN_NAME(ONO_), SHEN_NAME(MIKI), SHEN_NAME(HATR), 0x00000000u, SHEN_NAME(YAMO), SHEN_NAME(NOMR), SHEN_NAME(MEGM), SHEN_NAME(MAYM),
    SHEN_NAME(YKDM), SHEN_NAME(YOS1), SHEN_NAME(YOS2), SHEN_NAME(YOB1), SHEN_NAME(YOB2), SHEN_NAME(YJJI), SHEN_NAME(YBBA), SHEN_NAME(SUMI), SHEN_NAME(KOMN),
    SHEN_NAME(SAJO), 0x00000000u, SHEN_NAME(SKMT), SHEN_NAME(KOTA), SHEN_NAME(YASU), SHEN_NAME(KAYO), 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
    SHEN_NAME(TTYA), SHEN_NAME(SOBA), SHEN_NAME(ASOU), SHEN_NAME(HINA), SHEN_NAME(YSHY), SHEN_NAME(MJNT), 0x00000000u, SHEN_NAME(KKEN), SHEN_NAME(TKNB),
    SHEN_NAME(TOSK), SHEN_NAME(SERA), SHEN_NAME(HMRO), SHEN_NAME(BSJM), SHEN_NAME(BOB_), SHEN_NAME(FKSM), SHEN_NAME(KJMA), SHEN_NAME(NRSK), SHEN_NAME(ASNO),
    SHEN_NAME(NAKA), SHEN_NAME(NMKI), SHEN_NAME(YMGC), SHEN_NAME(KOGA), SHEN_NAME(TMRA), SHEN_NAME(MITA), SHEN_NAME(YAYI), SHEN_NAME(HTSI), SHEN_NAME(JOE_),
    SHEN_NAME(SAKI), SHEN_NAME(MIHO), SHEN_NAME(ENDO), SHEN_NAME(YOSI), SHEN_NAME(TKSA), SHEN_NAME(TAGW), SHEN_NAME(RYKO), SHEN_NAME(IRIE), SHEN_NAME(KURT),
    SHEN_NAME(HANA), SHEN_NAME(SIMZ), 0x00000000u, SHEN_NAME(TTAY), SHEN_NAME(DICK), SHEN_NAME(MARK), SHEN_NAME(RBRT), SHEN_NAME(HARY), SHEN_NAME(JONZ),
    SHEN_NAME(TAKI), SHEN_NAME(HIRI), SHEN_NAME(TAKE), SHEN_NAME(AKTG), SHEN_NAME(TOKI), SHEN_NAME(KNJI), SHEN_NAME(RIKA), SHEN_NAME(STSI), SHEN_NAME(GORO),
    SHEN_NAME(MKMR), SHEN_NAME(TDSI), SHEN_NAME(HRNO), 0x00000000u, SHEN_NAME(KUDO), SHEN_NAME(MURI), SHEN_NAME(FLD1), SHEN_NAME(FLD2), SHEN_NAME(FLD3),
    SHEN_NAME(FLD4), SHEN_NAME(FLD5), SHEN_NAME(FLD6), SHEN_NAME(FLD7), SHEN_NAME(FLD8), SHEN_NAME(FLD9), SHEN_NAME(FLDA), SHEN_NAME(FLDB), SHEN_NAME(FLDC),
    SHEN_NAME(FLDD), SHEN_NAME(KEBE), SHEN_NAME(KEBF), SHEN_NAME(KEBG), SHEN_NAME(KEBH), SHEN_NAME(KEBI), SHEN_NAME(KEBJ), SHEN_NAME(KEBK), SHEN_NAME(KEBL),
    SHEN_NAME(YOHI), SHEN_NAME(NGAI), SHEN_NAME(ITOH), SHEN_NAME(SKRD), SHEN_NAME(YJIH), SHEN_NAME(TJMA), SHEN_NAME(JONO), SHEN_NAME(UDGW), SHEN_NAME(OKYS),
    SHEN_NAME(ISDA), SHEN_NAME(MURA), SHEN_NAME(SNGA), SHEN_NAME(MITI), SHEN_NAME(MSTA), SHEN_NAME(MRSK), SHEN_NAME(MNWA), SHEN_NAME(HTNK), SHEN_NAME(HRSK),
    SHEN_NAME(NMNO), SHEN_NAME(ISYM), SHEN_NAME(KYMA), SHEN_NAME(ENKI), SHEN_NAME(NGSM), SHEN_NAME(TKHS), SHEN_NAME(HIDE), SHEN_NAME(KTRO), SHEN_NAME(KENI),
    SHEN_NAME(KYHN), SHEN_NAME(YMST), SHEN_NAME(SNDO), SHEN_NAME(NMTO), SHEN_NAME(KWMT), SHEN_NAME(TAEN), SHEN_NAME(SZEN), SHEN_NAME(KYSN), SHEN_NAME(SATO),
    SHEN_NAME(YSKT), SHEN_NAME(STAK), SHEN_NAME(IZWA), 0x00000000u, SHEN_NAME(SGRH), SHEN_NAME(MTRI), SHEN_NAME(TERY), SHEN_NAME(YKHI), SHEN_NAME(SBNK),
    SHEN_NAME(DTRY), 0x00000000u, 0x00000000u, SHEN_NAME(RINS), SHEN_NAME(ECHO), 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, SHEN_NAME(GRKN),
    SHEN_NAME(MAGO), SHEN_NAME(CHRL), 0x00000000u, SHEN_NAME(MORN), SHEN_NAME(HDEI), SHEN_NAME(TEYI), SHEN_NAME(TOMH), SHEN_NAME(FLDE), SHEN_NAME(FLDF),
    SHEN_NAME(FLDG), SHEN_NAME(TMHN), SHEN_NAME(DJUN), 0x00000000u, SHEN_NAME(HSOA), SHEN_NAME(GJHM), SHEN_NAME(GJBM), SHEN_NAME(GJHF), SHEN_NAME(GJBF),
    SHEN_NAME(YUMM), SHEN_NAME(SYKU), SHEN_NAME(HTSK), SHEN_NAME(SMIK), SHEN_NAME(SNJY), SHEN_NAME(SINF)}};

} // namespace shenmue::s1
