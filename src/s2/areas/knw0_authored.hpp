#pragma once
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name KNW0_AUTHOR_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name KNW0_AUTHOR_RYO{SHEN_NAME(RYO_)}, KNW0_AUTHOR_KNK{SHEN_NAME(KNK_)};
inline constexpr shen_name KNW0_AUTHOR_THIRD{SHEN_NAME(AU_3)}, KNW0_AUTHOR_FOURTH{SHEN_NAME(AU_4)};
inline constexpr shen_name KNW0_AUTHOR_PROPS[]{SHEN_NAME(a250), SHEN_NAME(a266), SHEN_NAME(a251), SHEN_NAME(a264), SHEN_NAME(a252), SHEN_NAME(a265),
    SHEN_NAME(a253), SHEN_NAME(a263), SHEN_NAME(a254), SHEN_NAME(a262), SHEN_NAME(a255), SHEN_NAME(a261)};
struct Knw0AuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name author;
    std::uint16_t state, reserved12;
    std::int32_t savedPostFx;
};
static_assert(sizeof(Knw0AuthorWork) == 0x18 && offsetof(Knw0AuthorWork, state) == 0x10);
void Knw0PrepareAuthorCharacter(shen_name character);
void Knw0PrepareAuthorCharacters();
void Knw0StartAuthorLoop(Knw0AuthorWork *work);
void Knw0AuthorMainCallback(void *data);
void Knw0AuthorLoopCallback(void *data);
} // namespace shenmue::s2::areas
