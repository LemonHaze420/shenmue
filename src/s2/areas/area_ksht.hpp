#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name KSHT_RYO{SHEN_NAME(RYO_)}, KSHT_SIN{SHEN_NAME(SIN_)}, KSHT_DUMMY{SHEN_NAME(DUMY)};
inline constexpr shen_name KSHT_TASK{SHEN_NAME(kshT)}, KSHT_INVALID{0xffffffffU};
struct KshtAuthorController {
    HLib::Task *task;
    std::uint32_t reserved08;
    std::int32_t selection;
    std::int16_t state;
    std::uint16_t reserved12;
    float savedFarDistance;
    std::int32_t *result;
};
static_assert(sizeof(KshtAuthorController) == 32 && offsetof(KshtAuthorController, state) == 16 && offsetof(KshtAuthorController, result) == 24);
extern KshtAuthorController kshtAuthorController;
extern std::int32_t kshtSelection, kshtPhase, kshtDelay, kshtAuthSelection, kshtRabbitDropCount;
extern std::uint64_t kshtAuthorMapLighting;
extern HLib::Task *kshtTask;
extern Vector3 kshtSavedPosition;
extern Vector3i kshtSavedRotation;
extern std::uint32_t kshtBreathEffects[2], kshtLeafEffect;
void QueueKshtAuthor(std::int32_t *selection);
void KshtAuthorCallback(void *);
void SetupKshtConversationPose(bool companion);
void QueueKshtWaitEye(float frames, float left, float right, std::int32_t delay);
void StartKshtZodiacConversation(void *);
void StartKshtWeatherConversation(void *);
void StartKshtNamesConversation(void *);
void StartKshtBadFeelingConversation(void *);
void StartKshtDreamConversation(void *);
void StartKshtDestinyConversation(void *);
void KshtWaitConversationMotion(void *);
void KshtExitCallback(void *);
void KshtExitWaitCallback(void *);
void KshtSimpleExitCallback(void *);
void KshtPrepareShortConversation(void *);
void KshtWaitShortConversation(void *);
void KshtPrepareHelp(void *);
void KshtWaitHelp(void *);
void KshtPrepareInstructions(void *);
void KshtPrepareFinalConversation(void *);
void KshtWaitInstructions(void *);
void KshtFreeTalkCallback(void *);
void KshtWaitAuthorCallback(void *);
void KshtWaitReturnCallback(void *);
void KshtWaitDrawingCallback(void *);
void QueueKshtSerif(std::int32_t message, std::int32_t delay);
extern const ScriptAreaDefinition kshtScriptDefinition;
} // namespace shenmue::s2::areas
