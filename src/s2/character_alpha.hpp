#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

#pragma pack(push, 4)
struct CharacterAlphaState {
    std::int32_t alphaSteps;
    std::uint32_t frameCount;
};

#pragma pack(pop)

static_assert(sizeof(CharacterAlphaState) == 0x08);
static_assert(offsetof(CharacterAlphaState, alphaSteps) == 0x00);
static_assert(offsetof(CharacterAlphaState, frameCount) == 0x04);

bool AttachCharacterAlphaComponent(Character *character);

void npcSetAlphaOnOff(Character *character, bool enabled);
std::int64_t cpamSetFlag(Character *character, std::int32_t enabled);
bool shenCheckCharaHasAlphaManager(const Character *character);

} // namespace shenmue::s2
