#pragma once
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name GABT_AUTHOR_TASK{SHEN_NAME(EVNT)}, GABT_AUTHOR_RYO{SHEN_NAME(RYO_)}, GABT_AUTHOR_GAI{SHEN_NAME(GAI2)};
inline constexpr shen_name GABT_AUTHOR_INTRO{SHEN_NAME(AU_2)}, GABT_AUTHOR_PRACTICE{SHEN_NAME(AU_0)}, GABT_AUTHOR_RESULT{SHEN_NAME(AU_1)};
inline constexpr shen_name GABT_AUTHOR_LIGHT_INTRO{SHEN_NAME(l304)}, GABT_AUTHOR_LIGHT_RESULT{SHEN_NAME(l701)};
inline constexpr shen_name GABT_AUTHOR_SCENERY[]{
    SHEN_NAME(a333), SHEN_NAME(a339), SHEN_NAME(a340), SHEN_NAME(a558), SHEN_NAME(a210), SHEN_NAME(a211), SHEN_NAME(a409)};
inline constexpr Vector4 GABT_AUTHOR_REFLECTOR{.x = 1, .y = 0, .z = 0, .w = 0};
struct GabtAuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name author;
    std::uint16_t state;
    std::byte reserved12[6];
};
struct GabtAuthorLoopWork {
    GabtAuthorWork *owner;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(GabtAuthorWork) == 0x18 && offsetof(GabtAuthorWork, state) == 0x10);
static_assert(sizeof(GabtAuthorLoopWork) == 0x10);
void GabtPrepareAuthorCharacter(shen_name character);
void GabtPrepareAuthorCharacters();
void GabtRestoreAuthorCharacters();
void GabtPrepareAuthorLighting(GabtAuthorWork *work);
void GabtAuthorMainCallback(void *data);
void GabtAuthorLoopCallback(void *data);
} // namespace shenmue::s2::areas
