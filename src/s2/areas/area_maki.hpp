#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/event_characters.hpp"
#include "s2/search.hpp"
#include "common/math.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name MAKI_AREA{SHEN_NAME(MAKI)}, MAKI_TASK{SHEN_NAME(makM)}, MAKI_LIGHT_TASK{SHEN_NAME(makL)};
inline constexpr shen_name MAKI_RYO{SHEN_NAME(RYO_)}, MAKI_SHENHUA{SHEN_NAME(SIN_)}, MAKI_LIGHTER{SHEN_NAME(LIGH)};
inline constexpr Vector4 MAKI_LIGHT_COLOUR{.x = 1.0f, .y = 1.0f, .z = 0.5f, .w = 0.2f};
extern Vector4 makiLighterColour;
extern Vector3 makiLighterParameters, makiLighterOffset;
extern std::int32_t makiLighterPhase;
HLib::Task *QueueMakiLighterLight();
void MakiLighterLightCallback(void *);
struct MakiLocalBaseWork {
    std::int32_t cleanup;
    shen_name character, base;
};
static_assert(sizeof(MakiLocalBaseWork) == 0x0c);
struct MakiRuntime {
    HLib::Task *mainTask;
    HLib::Task *lightTask;
    std::int32_t state;
    std::int16_t heldCount, depositedCount;
    std::int32_t *result;
    std::uint16_t phase;
    std::byte reserved22[2];
    shen_name heldCharacters[16];
    std::uint32_t reserved64;
    EventReadCharactersWork *characters;
};
static_assert(sizeof(MakiRuntime) == 0x70);
static_assert(offsetof(MakiRuntime, heldCharacters) == 0x24 && offsetof(MakiRuntime, characters) == 0x68);
extern MakiRuntime makiRuntime;
extern SearchTarget makiFirewoodTargets[30];
extern std::int32_t makiSelectedFirewood, makiSavedCameraContext, makiPreviousDeposit, makiSavedRootMode, makiSavedBlendFrames;
extern Vector3 makiSavedPosition;
extern Vector3i makiSavedRotation;
void MakiMainCallback(void *);
void MakiLockPlayerForSequence();
void MakiRestorePlayerAfterSequence();
void MakiAttachLighter(std::int32_t delay);
void MakiRefreshHeldLogs();
std::int32_t MakiPickSingleLog(shen_name character);
std::int32_t MakiPickOneLog(shen_name character);
std::int32_t MakiPickTwoLogs(shen_name character);
std::int32_t MakiPickThreeLogs(shen_name character);
std::int32_t MakiDepositLogs();
std::int32_t MakiDiscussEmptyHands();
std::int32_t MakiCollectFirewood();
std::int32_t MakiReturnToCampfire();
std::int32_t MakiFinishCollecting();
std::int32_t MakiNightfall();
void MakiSetLocalBaseCallback(void *);
extern const ScriptAreaDefinition makiScriptDefinition;
} // namespace shenmue::s2::areas
