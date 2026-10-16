#pragma once
#include "s1/task/scen.hpp"
#include "s1/task/conv.hpp"
#include "logging.hpp"

#include "s1/archive_data.hpp"
#include "s1/boot.hpp"
#include "s1/audio_stream.hpp"
#include "s1/modules/arcade.hpp"
#include "s1/cycle_stack.hpp"
#include "s1/collision.hpp"
#include "s1/chrd.hpp"
#include "s1/task/char.hpp"
#include "s1/task/gcpn.hpp"
#include "s1/game_utils.hpp"
#include "s1/task/came.hpp"
#include "s1/task/cach.hpp"
#include "s1/task/cdtt.hpp"
#include "s1/task/time.hpp"
#include "s1/dynamic_map.hpp"
#include "s1/file_loader.hpp"
#include "s1/face.hpp"
#include "s1/task/film.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/task/effe.hpp"
#include "s1/initialization.hpp"
#include "s1/lighting.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/input.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/atck.hpp"
#include "s1/task/cuff.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/dswk.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/edtk.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/obje.hpp"
#include "s1/motion_registry.hpp"
#include "s1/motion.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/gull.hpp"
#include "s1/task/hato.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/task/lamp.hpp"
#include "s1/task/lght.hpp"
#include "s1/task/mitk.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/modu.hpp"
#include "s1/mt5_types.hpp"
#include "s1/task/ppad.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/qte.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/save_format.hpp"
#include "s1/scene.hpp"
#include "s1/task/scrl.hpp"
#include "s1/task/save.hpp"
#include "s1/task/kdan.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/sond.hpp"
#include "s1/subtitles.hpp"
#include "s1/timing.hpp"
#include "s1/task/tmex.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/updb.hpp"
#include "s1/task/wind.hpp"
#include "s1/task/wexe.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/trwv.hpp"
#include "s1/task/telm.hpp"
#include "s1/texture_cache.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/texture_loader.hpp"

#include <algorithm>

#include <array>
#include <bit>
#include <cctype>
#include <cstddef>
#include <cmath>
#include <numbers>
#include <cstdint>
#include <limits>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
#include <utility>

namespace shenmue::s1 {
namespace scen_detail {}
using namespace scen_detail;

extern std::uint8_t g_disableClockAndActionSelector;

std::uint32_t GetDebugLogFlag();
extern float g_searchTargetDistance;

extern std::uint8_t g_timeValue;

extern std::uint8_t g_shouldTimeAdvance;

extern std::uint8_t g_globalFlags[1];

extern std::int32_t g_bGlobalFlag;

extern std::array<float, 2> g_debugTextPosition;

extern std::string g_debugText;

extern std::int32_t g_saveScreenFlag;

extern std::uint32_t g_searchLockOnFlags;

extern std::int32_t g_searchAnyInputPressedLatched;

extern std::int32_t g_searchAPressedLatched;

extern std::uint32_t g_searchTemporaryFocusedCharacter;

void RecalculateDayInTheWeek(GameDateTime *dateTime);
std::int32_t ExecuteAICACommand_Wrapper(std::uint32_t volume);
void ReleaseEnvironmentAicaBank(std::uint32_t bank);
extern std::int32_t g_actNumber;

extern std::array<std::uint8_t, 128> g_shenmueFlags;

extern ScenarioStepTable g_scenarioStepLut;

extern std::uint32_t g_scenHelpPromptOverride;

extern std::int32_t g_pendingSleepResumeLoad;

extern Vector3 g_ryoHomeSleepPosition;

extern std::uint16_t g_ryoHomeSleepYaw;

extern std::uint8_t g_temporaryRyoEventFlags;

extern std::int16_t g_defaultKaidanId;

extern std::int32_t g_temporaryKaidanId;

extern std::int32_t g_playerKaidanGuideActive;

extern float g_animatedSequencePlayDistance;

extern bool g_automatedSequenceSkip;

extern std::uint8_t g_shouldSleep;

extern std::uint8_t g_sleepSaveTrainingReadiness;

extern std::int32_t g_sleepPromptWindowStartMinutes;

extern std::int32_t g_sleepPromptWindowEndMinutes;

extern std::int32_t g_forcedSleepWindowStartMinutes;

extern std::int32_t g_forcedSleepWindowEndMinutes;

extern shen_name g_homeAreaId;

extern std::int32_t g_homeSceneId;

extern std::uint32_t g_requestedEntryPoint;

extern ScenarioHeader *g_pSCN_INFO_TaskData;

void SRCH_Reset();
extern std::array<std::uint8_t, 16> g_ryoHomeSleepAuxBlock;

extern std::uint16_t g_ryoHomeSleepGuideWord;

std::uint8_t DetermineActNum() noexcept;

extern ScenRuntimeState g_scenRuntimeState;

extern ScenarioFunctionWork *g_scenarioFunctionWork;

namespace scen_detail {

extern std::int16_t g_currentScenarioStep;

template <typename T> T *ScenarioPointer(std::uint32_t offset);
extern GameDateTime g_scenarioDateScratch;

inline constexpr std::uint32_t kScenarioFlagOneShotPrompt = 0x400U;
extern HLib::Task *g_scenOwnedSequenceTask;

extern bool g_scenSequenceObjectsInitialized;

extern CVWKRuntimeSlot *g_pAudio;

std::uint32_t &ScenFlags();

void UpdateScenActiveRuntimeObject();
ScenRuntimeObject *FindScenRuntimeObject(std::uint32_t name);

void ResetScenScriptRuntimeState();
void CreateScenSequenceCharacters();

ScenRuntimeObject *FindScenRuntimeObject(std::uint32_t name);

void UpdateScenActiveRuntimeObject();

bool ScenRuntimeObjectBusy();

void UpdateScenTaskTimer();

} // namespace scen_detail

void EnqueueSCNT(std::uint32_t name, std::int16_t stateValue);

namespace scen_detail {

void SCNT_InitWrapper();

void ResetScenScriptRuntimeState();

HLib::Task *CurrentScenarioTask();

} // namespace scen_detail

void SetStatOrNotebookFlag(std::uint16_t flag, bool memo);

namespace scen_detail {

void SetScenRuntimeObjectRange(std::uint32_t name, std::uint32_t mode, std::uint32_t center, std::uint32_t radius);

std::uint32_t QueryScenRuntimeObject(ScenarioContext *context, std::uint32_t name, std::uint32_t query, std::uint32_t auxiliary);

std::uint32_t StoreScenRuntimeObjectText(std::uint32_t name);

} // namespace scen_detail

struct ScenarioRelationQueue {
    std::uint32_t tag{};
    std::uint32_t count{};
    std::byte *read{};
    std::byte *write{};
    std::array<std::byte, 112> buffer{};
};
extern ScenarioRelationQueue *g_scenarioRelationQueues;

std::int32_t AcquireScenarioRelationQueue(std::uint32_t tag);

std::int32_t EnqueueScenarioRelationData(std::uint32_t handle, const void *source, std::int32_t size);

std::int32_t DequeueScenarioRelationData(std::uint32_t handle, void *destination, std::int32_t capacity);

namespace scen_detail {

#pragma pack(push, 1)
struct CreateScenarioTaskParameters {
    std::int32_t functionOffset;
    std::int32_t argumentCount;
    std::uint32_t arguments[1];
};
#pragma pack(pop)

#pragma pack(push, 1)

struct UiUtilityParameters {
    std::int32_t mode;
    std::uint8_t helpIconId;
    std::byte reserved05[3];
    std::uint8_t buttonId;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CameraSettingsParameters {
    std::int32_t mode;
    float value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct StepUtilityParameters {
    std::int32_t mode;
    std::uint16_t requestedStep;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct SetMapFlagsParameters {
    std::uint32_t flagIndex;
    std::uint32_t enabled;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct GameModuleTransitionParameters {
    std::int32_t sceneId;
    std::uint32_t areaId;
    std::uint32_t entryPoint;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct MathUtilityParameters {
    std::int32_t mode;
    std::uint32_t value1;
    std::uint32_t value2;
    std::uint32_t offset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct DebugStringParameters {
    float x;
    float y;
    std::uint32_t formatOffset;
    std::uint32_t arguments[1];
};
#pragma pack(pop)

#pragma pack(push, 1)

struct AddCharacterHandlerParameters {
    std::uint32_t characterType;
    std::uint32_t modelNameOffset;
    shen_name characterId;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CharacterBoundsParameters {
    shen_name characterId;
    std::uint32_t firstCornerOffset;
    std::uint32_t secondCornerOffset;
    shen_name anchorCharacterId;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct TargetAngleRangeParameters {
    std::uint32_t originOffset;
    float maximumDistance;
    std::uint32_t direction;
    std::int32_t maximumAngle;
    std::uint32_t targetOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FixedObjectAttachmentOffsetParameters {
    shen_name characterId;
    std::uint32_t useModelNode;
    std::uint32_t positionOffsetOrNodeKind;
    std::uint32_t rotationOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct RefBoardValuesParameters {
    shen_name characterId;
    std::int32_t type;
    std::uint32_t field024Offset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct PlayerControlParameters {
    std::uint32_t mode;
    std::uint32_t argument;
    std::uint32_t argument2;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CharacterScaleParameters {
    std::uint32_t characterId;
    std::uint32_t scaleOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FogFlagsParameters {
    std::uint32_t mode;
    std::uint32_t offset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FogColourData {
    float red;
    float green;
    float blue;
    std::uint32_t colour;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FilmTransitionParameters {
    std::uint32_t mode;
    std::uint32_t redA;
    std::uint32_t greenA;
    std::uint32_t blueA;
    std::uint32_t alphaA;
    std::uint32_t redB;
    std::uint32_t greenB;
    std::uint32_t blueB;
    std::uint32_t alphaB;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct SequenceUtilityParameters {
    std::int32_t mode;
    std::int32_t offset;
    std::uint32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct EventCameraParameters {
    std::uint32_t sequenceIndex;
    shen_name targetCharacterId;
    shen_name eyeCharacterId;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct SequencePlayRootParameters {
    std::uint32_t positionOffset;
    std::uint32_t rotationOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CharacterVisibilityParameters {
    shen_name characterId;
    std::int32_t mode;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct EventSetPositionParameters {
    shen_name characterId;
    std::int32_t flags;
    std::uint32_t positionOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct EventGetPositionParameters {
    shen_name characterId;
    std::int32_t nodeId;
    std::uint32_t positionOffset;
    std::uint32_t flags;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct EventSetRotationParameters {
    shen_name characterId;
    std::int32_t flags;
    std::uint32_t rotationOffset;
    std::uint32_t turnParametersOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HomeAreaParameters {
    std::int32_t promptHour;
    std::int32_t promptMinute;
    std::int32_t forcedHour;
    std::int32_t forcedMinute;
    std::int32_t sceneId;
    shen_name areaId;
    std::uint32_t entryPoint;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct GlobalSceneSlotValueParameters {
    std::uint32_t slot;
    std::int32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

#pragma pack(pop)

#pragma pack(push, 1)

struct MapEventUtilsParameters {
    std::uint32_t mode;
    shen_name characterId;
    std::int32_t reserved008;
    std::uint32_t index;
    std::int32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct AssignCharacterEventParameters {
    shen_name characterId;
    std::uint32_t mode;
    std::uint32_t eventGroup;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CharacterControlValueParameters {
    shen_name characterId;
    std::int32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct GetRotationParameters {
    shen_name characterId;
    std::uint32_t rotationOffset;
    std::uint32_t mode;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct PlayCharacterMotionParameters {
    shen_name characterId;
    std::uint16_t animationId;
    std::uint16_t reserved006;
    Vector3 frameRange;
    float playbackRate;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct RotateNodeParameters {
    shen_name characterId;
    std::uint32_t scaleOffset;
    std::uint32_t overwrite;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CharacterInteractionLimitsParameters {
    shen_name characterId;
    Vector3i limits;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct InputDataParameters {
    std::int32_t inputIndex;
    std::uint32_t inputType;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct OwnedItemMenuParameters {
    std::uint32_t mode;
    std::uint32_t itemId;
    std::uint32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct InventoryChangeParameters {
    std::uint16_t id;
    std::uint16_t reserved002;
    std::uint8_t amount;
    std::byte reserved005[7];
    std::uint32_t isCassette;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct InventoryRemoveParameters {
    std::uint16_t id;
    std::uint16_t reserved002;
    std::uint8_t amount;
    std::byte reserved005[3];
    std::uint32_t isCassette;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct InventoryQueryParameters {
    std::uint16_t id;
    std::uint16_t reserved002;
    std::uint32_t isCassette;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct MotionPlaybackRateParameters {
    shen_name characterId;
    float playbackRate;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FloatUtilityParameters {
    std::uint32_t mode;
    float value1;
    float value2;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct TmnmParameters {
    shen_name characterId;
    std::uint32_t animationId;
    float startFrame;
    float endFrame;
    float currentFrame;
    float frameStep;
    std::int32_t flags;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct LineWalkParameters {
    std::uint32_t mode;
    std::int32_t characterOrPathOffset;
    std::int32_t value;
    std::uint32_t value2;
    std::uint32_t value3;
    std::uint32_t value4;
    std::uint32_t value5;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ScenarioCharacterVisibilityParameters {
    shen_name characterId;
    std::int32_t mode;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct PrepParameters {
    std::int32_t mapIndex;
    std::int32_t operation;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CharacterFlagsParameters {
    shen_name characterId;
    std::int32_t mode;
    std::int32_t value;
    std::int32_t extra;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ShadowFlagsParameters {
    shen_name characterId;
    std::int32_t mode;
    std::uint32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct DirectionalLightParameters {
    std::uint32_t lightIndex;
    std::uint32_t directionOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct SpotLightParameters {
    std::uint32_t lightIndex;
    std::uint32_t positionOffset;
    std::uint32_t directionOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct PointLightParameters {
    std::uint32_t lightIndex;
    std::uint32_t positionOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ImageModuleFlagParameters {
    shen_name characterId;
    std::uint32_t requestedImage;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct GlobalSceneLightingParameters {
    std::uint32_t colourOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct LightSlotEnabledParameters {
    std::uint32_t lightIndex;
    std::uint32_t enabled;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct DynamicMapRenderFlagParameters {
    std::uint32_t mapIndex;
    std::uint32_t enabled;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct DynamicMapCameraFlagParameters {
    std::int32_t mode;
    std::int32_t mapIndexOrValue;
    std::int32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FightProgramStateParameters {
    shen_name characterId;
    std::uint8_t poseCacheColumns;
    std::byte reserved05[3];
    std::uint8_t poseCacheRows;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct LodUtilityParameters {
    std::int32_t mode;
    float value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct LightUtilityParameters {
    std::uint32_t lightIndex;
    std::int32_t mode;
    std::uint32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FaceMotionUtilityParameters {
    shen_name characterId;
    std::int32_t mode;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FaceExpressionParameters {
    shen_name characterId;
    std::int32_t eyeMode;
    std::int32_t eyePatternId;
    std::int32_t timing;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FaceParameters {
    shen_name characterId;
    std::uint16_t channel;
    std::uint16_t reserved_06;
    std::uint16_t morph;
    std::uint16_t reserved_0a;
    std::uint16_t timing;
    std::uint16_t reserved_0e;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FaceLookAtTargetParameters {
    shen_name characterId;
    std::int32_t mode;
    std::uint32_t targetOffset;
    std::uint16_t blendFrames;
    std::uint16_t reserved_0e;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FaceLcgParameters {
    shen_name characterId;
    std::int32_t valueA;
    std::int32_t valueB;
    std::int32_t timing;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CharacterHandParameters {
    shen_name characterId;
    std::int32_t mode;
    std::int32_t poseIndex;
    std::int32_t frameCount;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandNamedParameters {
    shen_name characterId;
    std::int32_t side;
    std::uint32_t namedIndex;
    std::uint32_t outputOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandMotionResetParameters {
    shen_name characterId;
    std::int8_t status;
    std::byte reserved05[3];
    std::int32_t side;
    std::int16_t sequenceId;
    std::byte reserved0e[2];
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandMotionSetParameters {
    shen_name characterId;
    std::int32_t side;
    std::int16_t sequenceId;
    std::byte reserved0a[2];
    std::int16_t startFrame;
    std::byte reserved0e[2];
    std::int16_t endFrame;
    std::byte reserved12[2];
    std::int16_t initialFrame;
    std::byte reserved16[2];
    std::uint16_t control;
    std::byte reserved1a[2];
    float frameStep;
    std::int16_t blendFrames;
    std::byte reserved22[2];
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandFloatsParameters {
    shen_name characterId;
    std::int32_t side;
    std::uint32_t anglesOffset;
    std::uint16_t blendFrames;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandControlParameters {
    shen_name characterId;
    std::int32_t side;
    std::int32_t mode;
    std::uint32_t value;
    std::uint8_t mask;
    std::byte reserved11[3];
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandRootRotationParameters {
    shen_name characterId;
    std::int32_t side;
    std::uint32_t mask;
    std::uint32_t rotationOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct OsageFlagsParameters {
    std::int32_t mode;
    std::uint32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct AicaCommandParameters {
    std::uint32_t mode;
    std::uint32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct DirectAicaCommandParameters {
    std::uint32_t command;
    std::uint32_t bank;
    std::int32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct MotnTargetParameters {
    shen_name characterId;
    std::int32_t nodeId;
    std::uint32_t targetOffset;
    std::uint32_t flags;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct LoadSoundBankParameters {
    std::uint32_t reserved00;
    std::int32_t bank;
    std::uint32_t filenameOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CameraShakeParameters {
    float previous;
    float current;
    float damping;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct AmbientSoundParameters {
    std::int32_t index;
    std::int32_t value;
    std::uint32_t sourceOffset;
    float positionX;
    float positionY;
    float positionZ;
    std::uint32_t flags;
    std::uint32_t auxiliaryOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct AmbientSoundCommandParameters {
    std::int32_t mode;
    std::uint32_t command;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ScenarioRuntimeUtilityParameters {
    std::int32_t command;
    std::int32_t argument0;
    std::uint32_t argument1;
    std::uint32_t argument2;
    std::uint32_t argument3;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CpamFlagsParameters {
    shen_name characterId;
    std::uint32_t enabled;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct ClothModelFlagsParameters {
    shen_name characterId;
    std::uint8_t value;
    std::byte reserved_05[3];
    std::int32_t operation;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct OsageUtilityParameters {
    shen_name characterId;
    std::int32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FightProgramParameters {
    shen_name characterId;
    std::uint8_t active;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct EventPackAfsParameters {
    std::int32_t mode;
    std::int32_t operation;
    std::uint32_t offset2;
    std::uint32_t offset;
    std::int32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct MotionLoadParameters {
    std::int32_t sceneId;
    shen_name areaId;
    std::uint32_t filenameOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct MotionTransformParameters {
    shen_name targetCharacterId;
    std::int32_t targetAnchorNodeId;
    shen_name sourceCharacterId;
    std::uint32_t sourceModelNodeKind;
    std::uint32_t translationOffset;
    std::uint32_t eulerOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct MotionScaleParameters {
    shen_name characterId;
    std::int32_t mode;
    float value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct LookPointParameters {
    shen_name characterId;
    std::int32_t angle;
    std::uint32_t targetOffset;
    std::int32_t mode;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct RyoSleepParameters {
    std::int32_t mode;
    std::int32_t operation;
    std::int32_t offset;
    std::uint32_t value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct MotStepParameters {
    shen_name characterId;
    std::uint8_t value;
    std::uint8_t selector;
    std::uint16_t reserved;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct FixedObjectAttachParameters {
    shen_name targetCharacterId;
    shen_name ownerCharacterId;
    std::int32_t nodeId;
    std::uint32_t positionOffset;
    std::uint32_t rotationOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct VoiceLoadParameters {
    std::int32_t mode;
    std::uint32_t nameOffset;
};
#pragma pack(pop)

template <typename T> T *ScenarioPointer(std::uint32_t offset) {
    return offset ? reinterpret_cast<T *>(g_pCurrentTaskData + static_cast<std::int32_t>(offset)) : nullptr;
}

class ScriptArguments {
  public:
    ScriptArguments(ScenarioContext *context, void *rawParams) : context_(context), words_(static_cast<const std::uint32_t *>(rawParams)) {
    }

    std::uint32_t word(std::size_t index) const {
        return words_ ? words_[index] : 0;
    }
    std::int32_t signedWord(std::size_t index) const {
        return static_cast<std::int32_t>(word(index));
    }
    float scalar(std::size_t index) const {
        return std::bit_cast<float>(word(index));
    }
    template <typename T> T *pointer(std::size_t index) const {
        return ScenarioPointer<T>(word(index));
    }
    void result(std::uint32_t value) const {
        if (context_ && context_->stack)
            context_->stack->accumulator = value;
    }
    void result(bool value) const {
        result(static_cast<std::uint32_t>(value));
    }
    void result(std::uint8_t value) const {
        result(static_cast<std::uint32_t>(value));
    }
    void result(std::uint16_t value) const {
        result(static_cast<std::uint32_t>(value));
    }
    void result(std::int32_t value) const {
        result(static_cast<std::uint32_t>(value));
    }
    void result(float value) const {
        result(std::bit_cast<std::uint32_t>(value));
    }

  private:
    ScenarioContext *context_;
    const std::uint32_t *words_;
};
void Script_SetNewDateTime(ScenarioContext *, void *rawParams);

void Script_ReturnTimeDate(ScenarioContext *, void *rawParams);

void Script_scenSetMapEvTbl(ScenarioContext *context, void *rawParams);

void Script_scenCheckCharaExist(ScenarioContext *context, void *rawParams);

shen_name NextGeneratedCharacterId(HLib::Task *scenarioTask);

void Script_AddCharacterHandler(ScenarioContext *context, void *rawParams);

void Script_SetTaskDataValue(ScenarioContext *context, void *rawParams);

void Script_CheckTargetWithinAngleRange(ScenarioContext *context, void *rawParams);

void Script_fixoSetAttachmentOffset(ScenarioContext *context, void *rawParams);

void Script_SetGameStateTo(ScenarioContext *, void *rawParams);

void Script_SetCameraFieldOfView(ScenarioContext *, void *rawParams);

void Script_seqEnterEvtCam(ScenarioContext *context, void *rawParams);

void Script_IsInEventCam(ScenarioContext *context, void *);

void Script_seqSetPlayRoot(ScenarioContext *, void *rawParams);

void Script_GetCurrentFrameCount(ScenarioContext *context, void *);

void Script_AllocSpline(ScenarioContext *, void *rawParams);

void Script_UpdateGameState_Camera(ScenarioContext *, void *rawParams);

void Script_seqGetCamInterpActive(ScenarioContext *context, void *);

void Script_SetTaskExecutionSuspended(ScenarioContext *context, void *rawParams);

void Script_shenRequestGameModuleTransition(ScenarioContext *, void *rawParams);

void Script_ctrlProcessReqScene(ScenarioContext *, void *);

void Script_CreateScenarioTask(ScenarioContext *context, void *rawParams);

void Script_srchControl(ScenarioContext *context, void *rawParams);

void Script_SetMapFlags(ScenarioContext *, void *rawParams);

void Script_StepUtils(ScenarioContext *context, void *rawParams);

void Script_SetCameraSettings(ScenarioContext *context, void *rawParams);

Character *ScenarioCharacter(ScenarioContext *context, std::uint32_t identifier);
void Script_PlayerControlUtils(ScenarioContext *context, void *rawParams);

void Script_uiUtils(ScenarioContext *, void *rawParams);

void Script_ToggleCPCTForChara(ScenarioContext *context, void *rawParams);

void Script_SetMapControlTaskFlag(ScenarioContext *context, void *rawParams);

void Script_SetHomeArea(ScenarioContext *, void *rawParams);

void Script_GetCPCTFlags(ScenarioContext *context, void *rawParams);

void Script_MathUtils(ScenarioContext *context, void *rawParams);

void Script_EVT_SetPosition(ScenarioContext *context, void *rawParams);

void Script_motnGetEffectorPos(Character *character, std::int32_t nodeId, Vector3 *output);

void Script_motnGetLocalPos(Character *character, std::int32_t nodeId, Vector3 *output);

void Script_ApplyMatrixTransformationWithRotation(Character *character, std::int32_t nodeId, Vector3 *output);

} // namespace scen_detail

void charaGetPosForNode(Character *character, std::int32_t nodeId, Vector3 *output);

namespace scen_detail {

void Script_EVT_GetPosition(ScenarioContext *context, void *rawParams);

void Script_mtwkGetSetRotation(ScenarioContext *context, void *rawParams);

std::int32_t ApplyScriptEulerFlags(Character *character, std::int32_t signedFlags, const Vector3i *source);

void Script_EVT_SetRotation(ScenarioContext *context, void *rawParams);

void Script_scenToggleCharaFlag(ScenarioContext *, void *rawParams);

void Script_mapEvUtils(ScenarioContext *context, void *rawParams);

void Script_mapEvGetCurrentEvt(ScenarioContext *context, void *rawParams);

void Script_scenSetAssignCVWK(ScenarioContext *context, void *rawParams);

void Script_SetOrGetWOCCValue(ScenarioContext *context, void *rawParams);

void Script_RotateNodeUtils(ScenarioContext *context, void *rawParams);

void Script_PlayCharaMotion(ScenarioContext *context, void *rawParams);

void Script_IsCharaMotionFinished(ScenarioContext *context, void *rawParams);

void Script_GetCharaMotionRemainingFramesScaled(ScenarioContext *context, void *rawParams);

void Script_ccowSetDistanceInteractLimits(ScenarioContext *context, void *rawParams);

void Script_get_WOCC_0(ScenarioContext *context, void *rawParams);

void Script_GetControllerInput(ScenarioContext *context, void *rawParams);

void Script_GetInputData(ScenarioContext *context, void *rawParams);

void Script_SetOwnedItemMenuFlagIfOwned(ScenarioContext *context, void *rawParams);

void Script_GetCassetteFlags(ScenarioContext *context, void *rawParams);

void Script_GetCassetteFlags_0(ScenarioContext *context, void *rawParams);

void Script_GetShenmueCassette(ScenarioContext *context, void *rawParams);

void Script_SetCharaMotionPlaybackRate(ScenarioContext *context, void *rawParams);

void Script_FloatUtils(ScenarioContext *context, void *rawParams);

void Script_tmnmUtils(ScenarioContext *context, void *rawParams);

void Script_tmnmCheckState(ScenarioContext *context, void *rawParams);

void Script_tmnmReset(ScenarioContext *context, void *rawParams);

void Script_LineWalkUtils(ScenarioContext *context, void *rawParams);

void Script_SetFogFlags(ScenarioContext *context, void *rawParams);

void Script_hmdlApplyScale(ScenarioContext *context, void *rawParams);

void Script_SetFogColour(ScenarioContext *, void *rawParams);

void Script_GetFogColour(ScenarioContext *, void *rawParams);

void Script_SetFogEnabled(ScenarioContext *, void *rawParams);

void Script_filmCheckOrSetTransitionUtil(ScenarioContext *context, void *rawParams);

void Script_EVT_ryoToggleFreeState(ScenarioContext *, void *rawParams);

void Script_SetActionSelectorHUDElementVisibility(ScenarioContext *, void *rawParams);

void Script_SetCharaVisibility(ScenarioContext *context, void *rawParams);

void Script_scenToggleVisibleFor(ScenarioContext *context, void *rawParams);

void Script_scenToggleActiveFor(ScenarioContext *context, void *rawParams);

void Script_SetCharaFlags(ScenarioContext *context, void *rawParams);

void Script_shenSetLightingMode(ScenarioContext *, void *rawParams);

void Script_SetShadowFlags(ScenarioContext *context, void *rawParams);

void Script_lightDirLightEnable(ScenarioContext *, void *rawParams);

void Script_lightEnablePointLightAt(ScenarioContext *, void *rawParams);

void Script_lightSpotLightEnable(ScenarioContext *, void *rawParams);

void Script_lightSetGlobalSceneLighting(ScenarioContext *, void *rawParams);

void Script_lightSetSlotEnabled(ScenarioContext *context, void *rawParams);

void Script_mapmSetRenderFlag(ScenarioContext *context, void *rawParams);

void Script_mapmSetCamFlags(ScenarioContext *context, void *rawParams);

void Script_SetFightProgramStateBytes(ScenarioContext *context, void *rawParams);

void Script_LODUtils(ScenarioContext *context, void *rawParams);

void Script_LightUtils(ScenarioContext *, void *rawParams);

void Script_lightSetGlobalLightAndKey(ScenarioContext *, void *rawParams);

void Script_SetupLightFromTable(ScenarioContext *, void *rawParams);

void Script_PREP_Related(ScenarioContext *, void *rawParams);

void Script_shenSeqUtils(ScenarioContext *context, void *rawParams);

void Script_ccowUtils(ScenarioContext *context, void *rawParams);

void Script_ccowSetFlag80(ScenarioContext *context, void *rawParams);

void Script_motmToggleFlags(ScenarioContext *context, void *rawParams);

void Script_charaToggleMotEnabled(ScenarioContext *context, void *rawParams);

void Script_SetMotnFlag(ScenarioContext *context, void *rawParams);

void Script_executeAicaCommand(ScenarioContext *, void *rawParams);

void Script_SetMotnFlags(ScenarioContext *context, void *rawParams);

void Script_InitializeOsag(ScenarioContext *context, void *rawParams);

void Script_scenUtils(ScenarioContext *context, void *rawParams);

void Script_LoadSND(ScenarioContext *, void *rawParams);

void Script_SetCamShake(ScenarioContext *, void *rawParams);

void Script_ambSoundUtils(ScenarioContext *, void *rawParams);

void Script_Ambient1SoundUtils(ScenarioContext *, void *rawParams);

void Script_SetCharaLookAtPosition(ScenarioContext *context, void *rawParams);

void Script_SetLookPointDetails(ScenarioContext *context, void *rawParams);

void Script_GetSetMotionScale(ScenarioContext *context, void *rawParams);

void Script_SetupTransformMatrixForMotionTask(ScenarioContext *context, void *rawParams);

void Script_SetCharaRefBoardPos(ScenarioContext *context, void *rawParams);

void Script_charaSetRefboardFlagTo(ScenarioContext *context, void *rawParams);

void Script_facemotUtils(ScenarioContext *context, void *rawParams);

void Script_FaceEx(ScenarioContext *context, void *rawParams);

void Script_FACE(ScenarioContext *context, void *rawParams);

void Script_FaceLookAtTargetUtils(ScenarioContext *context, void *rawParams);

void Script_FACE_LCG(ScenarioContext *context, void *rawParams);

void Script_ReadModelFormat7(ScenarioContext *context, void *rawParams);

void Script_CharHandlers(ScenarioContext *context, void *rawParams);

void Script_SetCPAMFlags(ScenarioContext *context, void *rawParams);

void Script_clmdSetFlags(ScenarioContext *context, void *rawParams);

void Script_osagUtil(ScenarioContext *context, void *rawParams);

void Script_GetDataFromFightProgram(ScenarioContext *context, void *rawParams);

void Script_EV_PAK_AFS_Utils(ScenarioContext *context, void *rawParams);

void Script_motiLoad(ScenarioContext *context, void *rawParams);

void Script_LoadHandMot(ScenarioContext *context, void *rawParams);

HandModelWork *FindScenarioHand(ScenarioContext *context, shen_name characterId, std::int32_t side);

void Script_handGetNamed(ScenarioContext *context, void *rawParams);

void Script_handSetFloatsFor(ScenarioContext *context, void *rawParams);

void Script_ConfigHandMotionIK(ScenarioContext *context, void *rawParams);

void Script_SetHandRootRotation(ScenarioContext *context, void *rawParams);

void Script_SetOsageFlags(ScenarioContext *, void *rawParams);

void Script_ExecuteAicaCommand(ScenarioContext *context, void *rawParams);

void Script_ResetHandMotion(ScenarioContext *context, void *rawParams);

void Script_SetNewHandMotion(ScenarioContext *context, void *rawParams);

void Script_LoadVoiceFile(ScenarioContext *context, void *rawParams);

void Script_ProcessSubtitle(ScenarioContext *context, void *rawParams);

void Script_SetCameraFlags(ScenarioContext *, void *rawParams);

void Script_cdttLoadScrUtils(ScenarioContext *, void *rawParams);

void Script_WeatherUtils_ForFIXC(ScenarioContext *context, void *rawParams);

void Script_GetSetWeatherSettings(ScenarioContext *context, void *rawParams);

void Script_WeatherUtils_0(ScenarioContext *context, void *rawParams);

void Script_WeatherTaskUtil(ScenarioContext *, void *rawParams);

void Script_WeatherUtils(ScenarioContext *, void *rawParams);

void Script_StartOrFreeSEQ(ScenarioContext *, void *rawParams);

void Script_SetImageModuleTaskDataFlag(ScenarioContext *context, void *rawParams);

void Script_SetRefBoardValues(ScenarioContext *context, void *rawParams);

void Script_fixoClearAttachment(ScenarioContext *context, void *rawParams);

void Script_fixoAttachEx(ScenarioContext *context, void *rawParams);

void Script_MotStepUtil(ScenarioContext *context, void *rawParams);

} // namespace scen_detail

void SetShouldSleepFlag(std::uint8_t mode);

namespace scen_detail {

void Script_SetShouldSleepFlag(ScenarioContext *, void *rawParams);

void RyoMoveToHomeSleep();

void Script_ryoSleepUtils(ScenarioContext *context, void *rawParams);

struct ScnfUiUtilsParameters {
    std::uint16_t expectedMask;
    std::byte reserved02[2];
    std::int16_t timeoutFrames;
    std::byte reserved06[2];
    std::uint8_t promptMode;
    std::byte reserved09[3];
    std::uint8_t bonusFrames;
    std::byte reserved0d[7];
    std::int32_t command;
};
std::int64_t SCNF_UIUtils(ScenarioContext *context, void *rawParams);

std::int64_t SCNF_GetSfxOverlayResultIfIdle(ScenarioContext *context, void *);

std::int64_t SCNF_RequestActionSelectorFromParams(ScenarioContext *, void *rawParams);

std::int64_t SCNF_GetSelectedHelpToken(ScenarioContext *context, void *);

std::int64_t SCNF_RequestActionSelector(ScenarioContext *, void *rawParams);

std::int64_t SCNF_GetSelectedBinary(ScenarioContext *context, void *);

std::int64_t SCNF_SetCashValue(ScenarioContext *, void *rawParams);

std::int64_t SCNF_WaitCashOverlayIdle(ScenarioContext *, void *);

std::int64_t SCNF_StartCashOverlaySelector(ScenarioContext *, void *rawParams);

std::int64_t SCNF_GetCashOverlayResult(ScenarioContext *context, void *);

std::int64_t SCNF_RequestIconNotification(ScenarioContext *, void *rawParams);

std::int64_t SCNF_WaitIconNotification(ScenarioContext *, void *);

std::int64_t SCNF_RequestSleepSaveTraining(ScenarioContext *, void *rawParams);

std::int64_t SCNF_WaitSleepSaveTraining(ScenarioContext *context, void *);

std::int64_t SCNF_StartIconSystemMenu(ScenarioContext *, void *rawParams);

std::int64_t SCNF_WaitIconSystemMenu(ScenarioContext *, void *);

std::int64_t SCNF_PrepareCassetteUI(ScenarioContext *context, void *);

std::int64_t SCNF_WaitCassetteUI(ScenarioContext *context, void *);

std::int32_t BuildTimedHelpSelectorChoices(std::uint32_t mode, TimedHelpSelectorChoice *choices);

std::int64_t SCNF_ShowHelpMessage(ScenarioContext *context, void *rawParams);

std::int64_t SCNF_GetSelectedChoiceIndex(ScenarioContext *context, void *rawParams);

std::int64_t SCNF_CopyScenario(ScenarioContext *context, void *rawParams);

std::int64_t SCNF_WaitFrames(ScenarioContext *, void *rawParams);

std::int64_t SCNF_ReturnZero(ScenarioContext *, void *);

std::int64_t SCNF_ReturnMinusOne(ScenarioContext *context, void *);

std::int64_t SCNF_CompleteImmediately(ScenarioContext *, void *);

std::uint32_t LegacyReturnZero(ScenarioRegisters *);

std::uint32_t GeneralReturnZero(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);

std::uint32_t GeneralMemset(std::uint32_t destinationOffset, std::uint32_t value, std::uint32_t size, std::uint32_t);

std::uint32_t GeneralMemcpy(std::uint32_t destinationOffset, std::uint32_t sourceOffset, std::uint32_t size, std::uint32_t);

std::uint32_t GeneralStrcpy(std::uint32_t destinationOffset, std::uint32_t sourceOffset, std::uint32_t, std::uint32_t);

std::uint32_t GeneralGetDebugLogFlag(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);

void Script_PreserveAccumulator(ScenarioContext *, void *);

void Script_ClearAccumulator(ScenarioContext *context, void *);

void Script_shenGetTaskBase(ScenarioContext *context, void *);

void Script_GetCurrentEntryPoint(ScenarioContext *context, void *);

void Script_set_val_to_1(ScenarioContext *, void *rawParams);

void Script_srchSetDistanceTarget(ScenarioContext *, void *rawParams);

void Script_ClearAccumulatorIfArgumentZero(ScenarioContext *context, void *rawParams);

struct FormattedScenarioString {
    std::array<char, 256> message{};
    std::uint32_t substitutionLength{};
};
FormattedScenarioString FormatScenarioString(std::uint32_t formatOffset, const std::uint32_t *arguments);

void ConvertScenarioMessageFromShiftJis(std::array<char, 256> &message);

void Script_sprintf(ScenarioContext *context, void *rawParams);

void Script_PrintDebugString(ScenarioContext *, void *rawParams);

PackSlot *ScriptPackSlot(std::uint32_t word);

std::uint32_t ScriptPackSlotWord(const PackSlot *slot);

void Script_CycleManagerUtils(ScenarioContext *context, void *rawParams);

void Script_timeSetUpdateEnabled(ScenarioContext *, void *rawParams);

void Script_GetSceneID(ScenarioContext *context, void *);

void Script_GetGlobalFlag(ScenarioContext *context, void *);

void Script_SetGlobalSceneSlotValue34(ScenarioContext *, void *rawParams);

void Script_SetNewDateTimeWrapper(ScenarioContext *context, void *rawParams);

void Script_CompareDates(ScenarioContext *context, void *rawParams);

void Script_IncrementDateTimeByNum(ScenarioContext *context, void *rawParams);

void Script_SetDateAndTime_Wrapper(ScenarioContext *context, void *rawParams);

void Script_shenSetCurrentDate(ScenarioContext *context, void *rawParams);

void Script_GetDateTimeUpdateHistory(ScenarioContext *context, void *rawParams);

void Script_SetCameraCollisionEnabled(ScenarioContext *context, void *rawParams);

void Script_camSetRotationStepSize(ScenarioContext *context, void *rawParams);

void Script_SetMaxInterpSpeed(ScenarioContext *context, void *rawParams);

void Script_SetCamFactor(ScenarioContext *context, void *rawParams);

void Script_GetTempAngleInDegrees(ScenarioContext *context, void *rawParams);

void Script_GetScriptedCameraState(ScenarioContext *context, void *);

void Script_SetNewCameraState(ScenarioContext *context, void *rawParams);

std::uint32_t ParseControlPointCodeDigits(std::uint32_t code);

void Script_cpctGetCurrControlMode(ScenarioContext *context, void *);

void Script_GetCurrentControlPointCode(ScenarioContext *context, void *);

void Script_ParseDigitsForInt(ScenarioContext *context, void *);

void Script_navSetCurrentPos(ScenarioContext *context, void *rawParams);

void Script_GetHighScoreValue(ScenarioContext *context, void *rawParams);

void Script_SetHighScoreValue(ScenarioContext *context, void *rawParams);

} // namespace scen_detail

void SCEN_FinalizeReadyPlayerControlRuntimeObjects() noexcept;

namespace scen_detail {

Character *ScenarioCharacter(ScenarioContext *context, std::uint32_t identifier);

void Script_charaSetClipAngle(ScenarioContext *context, void *rawParams);

void Script_GetMotionFlag200(ScenarioContext *context, void *rawParams);

void Script_HasMotionRootAngleFlags2003(ScenarioContext *context, void *rawParams);

void Script_charaHasMotionwork(ScenarioContext *context, void *rawParams);

void Script_ConstructColourPair(ScenarioContext *context, void *rawParams);

void Script_SetClampedParameterValue(ScenarioContext *context, void *rawParams);

void Script_lightResetSlotToDefaults(ScenarioContext *context, void *rawParams);

void Script_lightResetAndEnableDefaultSlot(ScenarioContext *context, void *rawParams);

void Script_SetShadowRange(ScenarioContext *context, void *rawParams);

void Script_SetAnimatedSequenceCollisionCheckDisabled(ScenarioContext *context, void *rawParams);

void Script_ASEQGlobalFloat(ScenarioContext *context, void *rawParams);

void Script_ClearInputFlags(ScenarioContext *, void *);

void Script_SetClipperFlag(ScenarioContext *context, void *rawParams);

void Script_refbSetActive(ScenarioContext *context, void *rawParams);

void Script_GetEngineInfo(ScenarioContext *context, void *rawParams);

void Script_convSetCamInactiveFlag(ScenarioContext *context, void *rawParams);

void Script_convGetIsActive(ScenarioContext *context, void *rawParams);

void Script_freeTalkGetAndResetState(ScenarioContext *context, void *);

void Script_GetFreeTalkStatus(ScenarioContext *context, void *);

void Script_freeTalkGetCurrentChara(ScenarioContext *context, void *);

void Script_SetDynamicMapStaticSceneMode(ScenarioContext *context, void *rawParams);

void Script_SetDynamicMapDebugDrawFlag(ScenarioContext *, void *rawParams);

void Script_GetLightPosition(ScenarioContext *context, void *rawParams);

void Script_charaVisUtils(ScenarioContext *context, void *rawParams);

void Script_SetMaxCharaSearchExtent(ScenarioContext *context, void *rawParams);

void Script_skyCtrlLuminance(ScenarioContext *context, void *rawParams);

void Script_SetVector4ByOffset(ScenarioContext *context, void *rawParams);

void Script_SetEffectParams(ScenarioContext *context, void *rawParams);

float CameraDistance(const Vector3 &target, const Vector3 &eye);

float CameraFixedHalfAngleTangent(std::int32_t halfAngle);

void Script_SetAnimatedCameraFov(ScenarioContext *context, void *rawParams);

void Script_shenGetStats(ScenarioContext *context, void *rawParams);

void Script_SetStats(ScenarioContext *context, void *rawParams);

void Script_SetICval(ScenarioContext *, void *);

void Script_LoadCharacterDefinition(ScenarioContext *context, void *rawParams);

void Script_SetRyoMotionDetails(ScenarioContext *context, void *rawParams);

void Script_SetBaseMotionScale(ScenarioContext *context, void *rawParams);

void Script_GetBatteryLifePair(ScenarioContext *context, void *rawParams);

void Script_GetOrResetScenarioStepUniqueItemCount(ScenarioContext *context, void *rawParams);

void Script_SetCurrentScrollEnabled(ScenarioContext *, void *rawParams);

void Script_SetEmulatorFlags(ScenarioContext *context, void *rawParams);

void Script_GetCurrentDrawMode(ScenarioContext *context, void *rawParams);

void Script_motmSetBlendFrames(ScenarioContext *context, void *rawParams);

void Script_scenCharaGetMotAnimId(ScenarioContext *context, void *rawParams);

void Script_SetMotnFlag_0(ScenarioContext *context, void *rawParams);

void Script_motiSetAngFlags(ScenarioContext *context, void *rawParams);

void Script_GetChidPositionAndRotation(ScenarioContext *context, void *rawParams);

void Script_charaSetRenderOpacity(ScenarioContext *context, void *rawParams);

void Script_GetAuthorState_And_SetCamPos(ScenarioContext *context, void *rawParams);

void Script_GetCycleManagerDeltaTime(ScenarioContext *context, void *rawParams);

void Script_GetFrameTime(ScenarioContext *context, void *rawParams);

void Script_LoadSND_Wrapper(ScenarioContext *context, void *rawParams);

void Script_cpctFlagUtil(ScenarioContext *context, void *rawParams);

void Script_cassetteUtils(ScenarioContext *context, void *rawParams);

void Script_FreeTaskData(ScenarioContext *context, void *rawParams);

void Script_GetHandControlBit3(ScenarioContext *context, void *rawParams);

void Script_handSetFor(ScenarioContext *context, void *rawParams);

void Script_BeginClipTween(ScenarioContext *context, void *rawParams);

void Script_ConfigureClipFlags(ScenarioContext *context, void *rawParams);

void Script_motDetailFlagUtil(ScenarioContext *context, void *rawParams);

void Script_SetPackTaskFlags(ScenarioContext *context, void *rawParams);

void Script_ProcessSubtitleText(ScenarioContext *context, void *rawParams);

void Script_ShowUIMessageFromID(ScenarioContext *context, void *rawParams);

void Script_faceGetFilename(ScenarioContext *, void *);

void Script_SetMaxNPCsNear(ScenarioContext *context, void *rawParams);

void Script_GetCycleManagerState(ScenarioContext *context, void *rawParams);

void Script_UpdateCCOWCandidateObjects(ScenarioContext *context, void *rawParams);

void Script_SetDynamicMapTransform(ScenarioContext *context, void *rawParams);

void Script_SetCycleManagerData(ScenarioContext *context, void *rawParams);

void Script_ReallocateUNFO(ScenarioContext *context, void *rawParams);

void Script_CalculateMatrixRotationFromParams(ScenarioContext *context, void *rawParams);

void Script_EVT_SetFacingTowardPoint(ScenarioContext *context, void *rawParams);

void Script_MoveObjectToObject(ScenarioContext *context, void *rawParams);

void Script_ptmvControlTrailFollow(ScenarioContext *context, void *rawParams);

void Script_faceSetGeneratedLipCues(ScenarioContext *context, void *rawParams);

void Script_CreateNPCImageModule(ScenarioContext *context, void *rawParams);

void Script_CEWPRelated(ScenarioContext *context, void *rawParams);

void Script_CopyAndModifyChid(ScenarioContext *context, void *rawParams);

void Script_ScenDataUtil(ScenarioContext *context, void *rawParams);

void Script_SetSearchFov(ScenarioContext *context, void *rawParams);

void Script_convCtrlCamApply(ScenarioContext *context, void *rawParams);

void Script_RequestFightAnimation(ScenarioContext *context, void *rawParams);

extern bool g_scenarioConversationPromptEnabled;

void Script_shenCashUtils(ScenarioContext *, void *rawParams);

void Script_GetConversationDetails(ScenarioContext *context, void *rawParams);

void Script_GetRemainingFrames(ScenarioContext *context, void *);

void Script_CheckTaskStatus(ScenarioContext *context, void *rawParams);

void Script_maprLoadRes(ScenarioContext *context, void *rawParams);

void Script_charaPosRotUtils(ScenarioContext *context, void *rawParams);

void Script_HandleLookPoint(ScenarioContext *context, void *rawParams);

void Script_cpctStateUtils(ScenarioContext *context, void *rawParams);

void Script_RunAuthorAndSetState(ScenarioContext *context, void *rawParams);

void Script_walk3D(ScenarioContext *context, void *rawParams);

void Script_ReinitFightState(ScenarioContext *context, void *rawParams);

void Script_faceUpdateFor(ScenarioContext *context, void *rawParams);

void Script_EVT_ApplyRot(ScenarioContext *context, void *rawParams);

void Script_srchUtils(ScenarioContext *context, void *rawParams);

void Script_GetHandPosition(ScenarioContext *context, void *rawParams);

void Script_charaSetupXForm(ScenarioContext *context, void *rawParams);

void Script_PTTMRelated(ScenarioContext *context, void *rawParams);

void Script_UpdatePathConstraints(ScenarioContext *context, void *rawParams);

void Script_InvestigationUtils(ScenarioContext *context, void *rawParams);

void Script_patrolUtils(ScenarioContext *context, void *rawParams);

void FreeAuthorSequenceTrack(std::int32_t index);

void LoadAuthorSequenceTrack(std::int32_t index, const char *identifier, const char *packageName);

void Script_AuthorUtils(ScenarioContext *context, void *rawParams);

void Script_SetupClothData(ScenarioContext *context, void *rawParams);

enum class CharaSelectState : std::uint32_t {
    None = 0,
    Idle = 1,
    SwitchSlot = 2,
    ConfirmCameraMove = 3,
    MoveProxyToFocus = 4,
    BounceForward = 5,
    BounceBack = 6,
    PreDispatchWait = 7,
    DispatchSlot0To2 = 8,
    DispatchSlot4 = 9,
    DispatchSlot3 = 10,
    DispatchSlot5 = 11,
    DispatchExtra = 12,
    Cleanup = 13,
    Exit14 = 14,
    Exit15 = 15,
    Cancel = 16,
};

struct ScenarioSelectorState {
    Character *owner = nullptr;
    Character *rig = nullptr;
    Character *selected = nullptr;
    Mt5RuntimeNode *rigRoot = nullptr;
    Vector3 rigBasePosition{};
    Vector3i rigBaseRotation{};
    std::array<Vector3, 12> cameraTargetAnchors{};
    std::array<Vector3, 12> cameraEyeAnchors{};
    Vector3 previousProxyPosition{};
    Vector3 transitionProxyTarget{};
    Vector3 proxyStepDelta{};
    Vector3 cameraTargetStepDelta{};
    Vector3 cameraEyeStepDelta{};
    CharaSelectState state = CharaSelectState::None;
    std::int32_t frame = 0;
    std::int32_t selectedSlot = 3;
    std::int32_t previousSlot = 3;
    std::int32_t transitionFrameCount = 0;
    std::int32_t repeatLeft = 0;
    std::int32_t repeatRight = 0;
    std::int32_t repeatUp = 0;
    std::int32_t repeatDown = 0;
};
extern ScenarioSelectorState g_scenarioSelector;

extern const std::array<float, 5> kFastSelectionCurve;

extern const std::array<float, 7> kSlowSelectionCurve;

extern const std::array<std::int32_t, 8> kSelectionSlots;

void BuildSelectionCameraAnchors(ScenarioSelectorState &selection);

Vector3 SelectionRigPoint(const ScenarioSelectorState &selection, std::uint16_t firstNodeKind, std::uint16_t secondNodeKind, const Vector3 &offset);

Vector3 SelectionProxyPoint(const ScenarioSelectorState &selection, std::int32_t slot, const Vector3 &offset);

void SetSelectionProxyPosition(ScenarioSelectorState &selection, const Vector3 &position);

Vector3 SelectionProxyPosition(const ScenarioSelectorState &selection);

void ApplySelectionCamera(const Vector3 &eye, const Vector3 &target);

void AdjustSelectionMarkerZ(ScenarioSelectorState &selection, std::int32_t slot, double delta);

void PressSelectionSlot(ScenarioSelectorState &selection, std::int32_t slot, double delta);

Vector3 AddSelectionVector(const Vector3 &lhs, const Vector3 &rhs);

Vector3 SubtractSelectionVector(const Vector3 &lhs, const Vector3 &rhs);

Vector3 ScaleSelectionVector(const Vector3 &value, float scale);

Vector3 LerpSelectionVector(const Vector3 &from, const Vector3 &to, float factor);

void ResetSelectionRepeatCounters(ScenarioSelectorState &selection);

void UpdateSelectionRepeatCounters(ScenarioSelectorState &selection, const PadCaptureEntry *input);

void HandleSelectionNavigation(ScenarioSelectorState &selection, const PadCaptureEntry *input);

void Script_InitMOTMGlobalState(ScenarioContext *context, void *rawParams);

void Script_UpdateCharaSelectionScene(ScenarioContext *context, void *rawParams);

void Script_wmptUtils(ScenarioContext *context, void *rawParams);

void Script_motmUtilsEx(ScenarioContext *context, void *rawParams);

void SubmitScenarioMove(ScenarioContext *context, void *rawParams, bool payload, bool playbackRate, std::uint32_t mode);

void Script_scenCharaSubmitMoveTargetRequest(ScenarioContext *context, void *rawParams);

void Script_IsMoveTargetRequestActive(ScenarioContext *context, void *rawParams);

void Script_SetLampState(ScenarioContext *context, void *rawParams);

void Script_shenGetNode(ScenarioContext *context, void *rawParams);

void Script_sicnUtils(ScenarioContext *context, void *rawParams);

void Script_SetCharaMoveTarget(ScenarioContext *context, void *rawParams);

void Script_GetCharaMoveStatus(ScenarioContext *context, void *rawParams);

void Script_KaidanUtils(ScenarioContext *context, void *rawParams);

void Script_GetBlurModel(ScenarioContext *context, void *rawParams);

void Script_dynamicStuff(ScenarioContext *context, void *rawParams);

void Script_ExitCharaSelector(ScenarioContext *, void *);

void Script_HandleLookPointAnimationUpdate(ScenarioContext *context, void *rawParams);

void Script_ConversationUtils(ScenarioContext *context, void *rawParams);

void Script_gachaponUtils(ScenarioContext *context, void *rawParams);

enum class TimedTransitionState : std::uint8_t {
    Queued = 5,
    WaitForPromptStart = 7,
    WaitForPromptResult = 8,
    StartMessage = 11,
    WaitForMessageEnd = 12,
    StartMessageAlt = 13,
    StartFadeOut = 14,
    CommitMapTransition = 15,
};

enum TimedTransitionFlags : std::uint8_t {
    TimedTransitionRollbackToPreviousDay2330 = 0x01,
    TimedTransitionAreaScopedPrunable = 0x02,
    TimedTransitionCaptureCurrentArea = 0x04,
    TimedTransitionUpdateMemoItem150 = 0x08,
};
#pragma pack(push, 1)

struct TimedTransitionParameters {
    std::uint32_t mode{};
    std::uint32_t sceneId{};
    std::uint32_t areaId{};
    std::uint32_t entryId{};
    std::int32_t conditionOffset{};
    std::int32_t timeRange{};
    std::uint32_t flags{};
};
#pragma pack(pop)

extern bool g_scenarioTimedTransitionEnabled;

std::uint32_t RegisterTimedTransition(const TimedTransitionParameters &parameters);

void Script_TimedTransitionUtils(ScenarioContext *context, void *rawParams);

std::uint32_t UpdateSleepPromptScenario(std::int32_t mode, bool forced);

void Script_wazaUtils(ScenarioContext *context, void *rawParams);

void Script_charaSubmitMoveRequestEx(ScenarioContext *context, void *rawParams);

void Script_SubmitCharaMoveTarget(ScenarioContext *context, void *rawParams);

} // namespace scen_detail

bool SCEN_IsPlayerAllowed();

namespace scen_detail {

void Script_IsPlayerAllowed(ScenarioContext *context, void *);

void Script_EventCamReader(ScenarioContext *context, void *rawParams);

void Script_GetCharaPos(ScenarioContext *context, void *rawParams);

void Script_SetCharaMoveTargetEx(ScenarioContext *context, void *rawParams);

void Script_SetCharaXMPTMode(ScenarioContext *context, void *rawParams);

void Script_aicaGetOrSetState(ScenarioContext *context, void *rawParams);

void Script_KaidanSetY(ScenarioContext *context, void *rawParams);

void Script_LODSnapshotUtils(ScenarioContext *context, void *rawParams);

void Script_InitINSHForScene(ScenarioContext *context, void *rawParams);

} // namespace scen_detail

void Script_EnqueueSCNT(ScenarioContext *context, void *rawParams);

bool SCEN_HasActiveScript() noexcept;

bool SCEN_HasSleepTransitionFinished(const PlayerControlWork &player, std::int32_t mode);

void SCEN_UpdatePlayerSleepScheduler(PlayerControlWork &player);

void SCEN_SetCurrentScenarioStep(std::int16_t step) noexcept;

void SCEN_SetHelpToken(std::uint8_t token, std::uint8_t button) noexcept;

void Script_SpriteUtils(ScenarioContext *context, void *rawParams);

namespace scen_detail {

std::uint32_t Pop(ScenarioRegisters *&stackPointer);

} // namespace scen_detail

ScenarioStack *scenAllocateScntForScript(
    ScenarioWindow *scenarioWindow, ScenarioStack *parent, const void *entryInstruction, const std::uint32_t *arguments, std::int32_t argumentCount);

extern ScenarioSequence *g_pSCNC;

extern std::array<ScenarioTrackSlot, 70> g_SCNCTrackSlots;

extern std::int32_t g_SCNCSelectedTrackIndex;

namespace scen_detail {

#pragma pack(push, 1)
struct SequenceChunk {
    std::uint32_t token;
    std::uint32_t size;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct SequenceHeaderChunk {
    SequenceChunk chunk;
    std::uint32_t flags;
    std::int32_t frameCount;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct CameraFunctionData {
    std::int32_t frameId;
    std::uint32_t offset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct MotionFunctionData {
    std::uint32_t characterId;
    std::int16_t animationId;
    std::byte reserved006[2];
    std::int32_t firstFrame;
    std::int32_t lastFrame;
    std::uint8_t flags;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct VoiceFunctionData {
    std::uint32_t characterId;
    std::uint32_t stringIndex;
    std::int32_t lipIndex;
    std::int32_t soundCommand;
    float durationSeconds;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct SoundEffectFunctionData {
    std::uint32_t characterId;
    std::uint32_t command;
    std::uint32_t reserved008;
    std::int32_t useDefaults;
    std::int32_t volume;
    std::int32_t bank;
    std::int32_t channel;
    std::uint32_t flags;
    float angleDuration;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct EffectFunctionData {
    std::uint32_t characterId;
    std::int16_t channel;
    std::byte reserved006[2];
    std::int16_t morph;
    std::byte reserved00a[2];
    std::int16_t timing;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct MotionRuntimeState {
    std::uint32_t characterId;
    std::uint16_t flags;
    std::byte reserved006[2];
    std::int16_t framesRemaining;
    std::byte reserved00a[0x0e];
    std::int32_t saved018;
    std::uint64_t savedMotion;
    Vector3i savedEntityEuler;
    std::byte reserved030[0x0c];
    std::int32_t savedInterpolationFlags;
};
#pragma pack(pop)

void SEQ_ConvertMLWDToUNFO(ScenarioSequence *sequence);

using AnimatedSequenceFunction = std::int64_t (*)(std::int32_t, void *, void *);
std::int64_t AnimatedSequenceNull(std::int32_t, void *, void *);

extern std::int32_t g_seqRemainingKeyframes;

extern const float *g_seqFcvKeyframePtr;

std::int64_t ASEQ_ANIMATED_CAM(std::int32_t, void *rawData, void *);

void AllocSPLI_Wrapper(AnimatedMovementRuntime *runtime);

bool AnimatedMoveObject_ReadFCVKey_Internal(AnimatedMovementRuntime *runtime, AnimatedMovementSample *output);

std::int64_t ASEQ_ANIMATED_MOV(std::int32_t mode, void *rawData, void *rawRuntime);

std::int64_t ASEQ_MOTION(std::int32_t mode, void *rawData, void *rawRuntime);

std::int64_t ASEQ_EFFECT(std::int32_t, void *rawData, void *);

const std::byte *SequenceIndexedPayload(const void *chunk, std::uint32_t index);

std::int64_t ASEQ_VOICE(std::int32_t mode, void *rawData, void *rawRuntime);

std::int16_t ASEQ_CalculateAngle(std::uint32_t characterId);

std::int64_t ASEQ_SOUND_EFFECT(std::int32_t mode, void *rawData, void *);

extern const std::array<AnimatedSequenceFunction, 7> kAnimatedSequenceFunctions;

const SequenceChunk *FindTrackChild(const SequenceChunk *track, std::uint32_t token);

void SEQ_ExecuteFunction(std::uint8_t functionIndex, void *scriptData);

void SEQ_UpdateSkipInput(ScenarioSequence *sequence);

void SEQ_UpdateFCVCurves(ScenarioSequence *sequence);

void SEQ_UpdateTasksAndCameras(ScenarioSequence *sequence);

} // namespace scen_detail

void SEQ_SetAutomatedSkip(bool enabled);

ScenarioSequence *ASEQ_SCNC_Init(HLib::Task *scenarioTask);

void ASEQ_InitCameraTrackPointers(const void *scenarioExtensionChunk);

void ASEQ_ApplyCameraKeyframes(ScenarioSequence *sequence);

bool SEQ_ParseInternal(const void *trackData);

void SEQ_ParseFrame(ScenarioSequence *sequence);

void SEQ_Cleanup();

} // namespace shenmue::s1

namespace shenmue::s1::scen_detail {

#pragma pack(push, 1)
struct PerspectiveCameraParameters {
    std::int32_t horizontalFov;
    std::int32_t reserved04;
    std::int32_t verticalHeight;
    std::int32_t reserved0c;
};
#pragma pack(pop)

static_assert(sizeof(PerspectiveCameraParameters) == 0x10);

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1::scen_detail {

#pragma pack(push, 1)
struct ScenLineEntry {
    std::uint32_t textOffset{};
    std::uint32_t lineIdOffset{};
    char code[4]{};
    float delaySeconds{};
};
#pragma pack(pop)

static_assert(sizeof(ScenLineEntry) == 0x10);

inline constexpr std::size_t kScntScriptSlotCount = 0x145;

inline constexpr std::size_t kScntWordsPerSlot = 6;

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1::scen_detail {

bool ScenCycleEntryEligible(const CycleManager::NpcRecord &npc, bool runningPrograms, bool &carry);

struct ScenRuntimeContainer {
    std::uint32_t tag;
    std::uint32_t wholeSize;
    std::uint32_t version;
    std::uint32_t flags;
};

using ScenRuntimeRecord = ScnfRecordHeader;

struct ScenOwnedRecordStorage {
    std::vector<std::byte> scstBytes;
    HLib::TaskData *scenarioFile{};
};

template <typename Callback> void ForEachEligibleScenCycleNpc(Callback &&callback) {
    if (g_cycleTalkSuppressed)
        return;
    bool carry = false;
    for (const CycleProgramList list : {CycleProgramList_SimSet, CycleProgramList_Running}) {
        const bool runningPrograms = list == CycleProgramList_Running;
        for (const CycleManager::NpcRecord *npc = CycleManager::NextProgramInList(list); npc; npc = CycleManager::NextProgramInList(list, npc)) {
            if (!ScenCycleEntryEligible(*npc, runningPrograms, carry))
                continue;
            carry = false;
            if (!callback(*npc))
                return;
        }
    }
}

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1::scen_detail {

#pragma pack(push, 1)
struct SequenceFunctionFrame {
    std::int32_t frameIndex;
    std::uint8_t functionIndex;
};
#pragma pack(pop)

} // namespace shenmue::s1::scen_detail

namespace shenmue::s1::scen_detail {

struct ScenExprValue {
    std::uint8_t type{};
    std::int32_t value{};
};

} // namespace shenmue::s1::scen_detail
