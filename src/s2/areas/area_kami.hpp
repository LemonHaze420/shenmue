#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name KAMI_TASK{SHEN_NAME(kamM)};
inline constexpr shen_name KAMI_LOAD_TASK{SHEN_NAME(load)};
inline constexpr shen_name KAMI_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name KAMI_BARBER{SHEN_NAME(CHY_)};
inline constexpr shen_name KAMI_RAZOR{SHEN_NAME(KMSR)};
inline constexpr shen_name KAMI_INVALID{0xffffffffU};
inline constexpr std::uint16_t KAMI_PLAYER_SEATED_MOTION = 0x7f03;
inline constexpr std::uint16_t KAMI_RAZOR_APPROACH_MOTION = 0x1003;
inline constexpr std::uint16_t KAMI_RAZOR_HOLD_MOTION = 0x1002;
struct KamiAuthorRequest {
    std::int32_t command, state, nextState, qteContinuation, failureState, timer, savedSkip, reserved1c;
};
static_assert(sizeof(KamiAuthorRequest) == 32);
static_assert(offsetof(KamiAuthorRequest, failureState) == 16);
static_assert(offsetof(KamiAuthorRequest, timer) == 20);
void AreaKAMIInit();
void AreaKAMIMain(std::uintptr_t parameter);
void KAMIInitializeCallback(void *data);
void KAMIStateCallback(void *data);
void KAMIBeginRazorIntroduction();
void KAMIBeginRazorRepeat();
void KAMIRestoreSuccessfulGameplay();
void KAMIRestoreFailedGameplay();
void KAMIFinish();
extern const ScriptAreaDefinition areaKAMIScriptDefinition;
} // namespace shenmue::s2::areas
