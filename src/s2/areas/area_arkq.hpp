#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
inline constexpr shen_name ARKQ_AREA{SHEN_NAME(ARKQ)}, ARKQ_TASK{SHEN_NAME(arkq)}, ARKQ_EVENT{SHEN_NAME(EVNT)};
inline constexpr shen_name ARKQ_TURN_TASK{SHEN_NAME(knif)}, ARKQ_LIGHT_TASK{SHEN_NAME(lght)};
inline constexpr shen_name ARKQ_RYO{SHEN_NAME(RYO_)}, ARKQ_REN{SHEN_NAME(REN_)}, ARKQ_SAM{SHEN_NAME(SAM_)};
inline constexpr shen_name ARKQ_LLY{SHEN_NAME(LLY_)}, ARKQ_CLJ{SHEN_NAME(CLJ_)}, ARKQ_BASE{SHEN_NAME(BASE)}, ARKQ_DUMMY{SHEN_NAME(DUMM)};
inline constexpr shen_name ARKQ_KNIFE{SHEN_NAME(a320)}, ARKQ_HAND_ANCHOR{SHEN_NAME(a095)}, ARKQ_LIGHT_ANCHOR{SHEN_NAME(a316)};
inline constexpr shen_name ARKQ_MONEY{SHEN_NAME(a118)}, ARKQ_KNIFE_IMAGE{SHEN_NAME(ab20)}, ARKQ_INVALID{0xffffffffU};
inline constexpr shen_name ARKQ_INTRO{SHEN_NAME(STRT)}, ARKQ_SUCCESS_AUTHOR{SHEN_NAME(LOSE)}, ARKQ_FAILURE_AUTHOR{SHEN_NAME(WON_)};
inline constexpr shen_name ARKQ_LIGHT{SHEN_NAME(l103)}, ARKQ_SUCCESS_LIGHT{SHEN_NAME(l046)};
inline constexpr shen_name ARKQ_SUCCESS_MAP{SHEN_NAME(AB00)}, ARKQ_FAILURE_MAP{SHEN_NAME(0059)};
inline constexpr shen_name ARKQ_CHARACTERS[]{ARKQ_RYO, ARKQ_REN, ARKQ_SAM, ARKQ_LLY, ARKQ_CLJ, ARKQ_INVALID};
inline constexpr shen_name ARKQ_SPECTATORS[]{ARKQ_SAM, ARKQ_LLY, ARKQ_CLJ};
inline constexpr Vector3 ARKQ_KNIFE_OFFSET{.x = 0, .y = 0, .z = -.07f};
inline constexpr Vector3i ARKQ_KNIFE_ROTATION{.x = 0x6000, .y = 0xd000, .z = 0xdddd};

inline std::int32_t arkqOpenHandPose[57]{0, 726, 63535, 0, 364, 64990, 0, 62806, 4368, 0, 65172, 46972, 0, 0, 57710, 0, 1638, 55527, 0, 546, 1456, 0, 0, 52615,
    0, 0, 54798, 0, 1091, 56254, 0, 546, 909, 0, 0, 56983, 0, 0, 56801, 0, 364, 58620, 0, 546, 908, 0, 0, 53524, 0, 0, 58620, 0, 546, 59349, 0, 546, 1091};
inline std::int32_t arkqKnifeHandPose[57]{0, 726, 63535, 0, 364, 64990, 0, 62806, 4368, 0, 65172, 46972, 0, 0, 57710, 0, 1638, 55527, 0, 546, 1456, 0, 0, 52615,
    0, 0, 54798, 0, 1091, 56254, 0, 546, 909, 0, 0, 56983, 0, 0, 56801, 0, 364, 58620, 0, 546, 908, 0, 0, 53524, 0, 0, 58620, 0, 546, 59349, 0, 546, 1091};
struct ArkqWork {
    HLib::Task *task;
    std::byte reserved08[8];
    std::uint16_t commandPhase;
    std::uint16_t inputMasks[3];
    std::int16_t state;
    std::uint16_t reserved1a;
    std::uint32_t scratch[2];
    std::uint32_t reserved24;
    EventReadCharactersWork *characters;
    std::byte reserved30[16];
};
static_assert(sizeof(ArkqWork) == 0x40 && offsetof(ArkqWork, commandPhase) == 0x10 && offsetof(ArkqWork, state) == 0x18);
static_assert(offsetof(ArkqWork, scratch) == 0x1c && offsetof(ArkqWork, characters) == 0x28);
struct ArkqGlobals {
    std::int32_t authorStopDelay;
    std::uint32_t glints[10];
    std::uint32_t reserved2c[4];
    std::int32_t knifeTurnFrame, knifeLightFrame;
};
struct ArkqCharacterPauseWork {
    std::int32_t cleanup;
    shen_name character;
    std::int32_t paused;
    float delay;
};
static_assert(sizeof(ArkqGlobals) == 0x44 && offsetof(ArkqGlobals, knifeTurnFrame) == 0x3c && offsetof(ArkqGlobals, knifeLightFrame) == 0x40);
static_assert(sizeof(ArkqCharacterPauseWork) == 0x10);
extern ArkqGlobals arkqGlobals;
void InitializeArkqEvent();
void RunArkqEvent(std::uintptr_t argument);
void ArkqMainCallback(void *data);
void ArkqCleanupCallback(void *data);
void ArkqTurnKnifeCallback(void *data);
void ArkqKnifeLightCallback(void *data);
void ArkqIntroAuthorCallback(void *data);
void ArkqFreeGlintsCallback(void *data);
void ArkqSuspendGlintCallback(void *data);
void ArkqCharacterPauseCallback(void *data);
std::uint32_t CreateArkqKnifeGlint();
std::int32_t ArkqBeginKnifeDisplay(ArkqWork &work);
void QueueArkqKnifeAttachment(std::int32_t bone, const Vector3 &position, std::int32_t delay);
std::int32_t ArkqFirstDodge();
std::int32_t ArkqFirstFailure(std::int32_t delay);
std::int32_t ArkqSecondDodge(std::int32_t delay);
std::int32_t ArkqSecondFailure(std::int32_t delay);
std::int32_t ArkqThirdFailure(std::int32_t delay);
std::int32_t ArkqAllDodges();
extern const ScriptAreaDefinition arkqScriptDefinition;
} // namespace shenmue::s2::areas
