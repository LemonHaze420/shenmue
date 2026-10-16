#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/gabt_authored.hpp"
#include "s2/areas/gabt_mole.hpp"
#include "s2/seqp_event.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name GABT_AREA{SHEN_NAME(GABT)}, GABT_SEQUENCE{SHEN_NAME(SEQP)}, GABT_BATTLE{SHEN_NAME(BSBT)};
inline constexpr shen_name GABT_PIVOT{SHEN_NAME(PDBG)}, GABT_ATTACHMENT{SHEN_NAME(ADBG)}, GABT_RETURN{SHEN_NAME(QE00)};
inline constexpr Vector3 GABT_PLAYER_POSITION{.x = 287, .y = 118, .z = 76.300003f};
inline constexpr Vector3 GABT_OPPONENT_POSITION{.x = 289.100006f, .y = 118, .z = 74.9000015f};
inline constexpr Vector3 GABT_SCENERY_POSITION{.x = 287.549988f, .y = 118, .z = 73.8649979f};
inline constexpr Vector3 GABT_SUCCESS_PIVOT_POSITION{.x = 287.779999f, .y = 118, .z = 76.8700027f};
inline constexpr Vector3 GABT_FAILURE_PIVOT_POSITION{.x = 287.870026f, .y = 118, .z = 76.7600021f};
inline constexpr Vector3 GABT_SUCCESS_OPPONENT_POSITION{.x = .212799996f, .y = 0, .z = 1.55840003f};
inline constexpr Vector3 GABT_FAILURE_OPPONENT_POSITION{.x = .547510028f, .y = 0, .z = 1.87778997f};
inline std::int32_t gabtOpponentLeftHandPose[19][3]{{0, 542, 57167}, {0, 1638, 3820}, {0, 60987, 4547}, {0, 0, 58619}, {0, 65172, 59895}, {0, 1820, 1638},
    {0, 65354, 1820}, {0, 0, 59713}, {0, 726, 58076}, {0, 64628, 730}, {0, 182, 1638}, {0, 0, 59349}, {0, 908, 58622}, {0, 62807, 1639}, {0, 364, 1272},
    {0, 0, 56619}, {0, 1091, 59350}, {0, 62623, 365}, {0, 0, 908}};
struct GabtEventWork {
    CompiledSequenceWork *sequence;
    HLib::Task *task;
    std::int32_t active, state, frames;
    std::uint32_t reserved1c;
    std::uint32_t motions[16];
    EventReadCharactersWork *characters;
    std::int32_t entry;
    std::uint32_t reserved6c;
    std::int32_t reserved70, moveTimer, authorFrames, reserved7c;
};
static_assert(sizeof(GabtEventWork) == 0x80 && offsetof(GabtEventWork, entry) == 0x68);
std::int32_t QueueGabtSuccessChoreography();
std::int32_t QueueGabtFailureChoreography();
void GabtSuccessChoreographyCallback(void *);
void GabtFailureChoreographyCallback(void *);
void GabtPrepareReflectors();
void GabtBattleSequenceCallback(void *);
void GabtRootSequenceCallback(void *);
void GabtRootCallback(void *);
void QueueGabtEvent(std::int32_t entry);
void InitializeGabtEvent();
void RunGabtEvent(std::uintptr_t argument);
extern const ScriptAreaDefinition gabtScriptDefinition;
} // namespace shenmue::s2::areas
