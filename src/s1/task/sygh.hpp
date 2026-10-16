#pragma once

#include "s1/hlib.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct SYGHOverlayState {
    bool visible;
    bool opening;
    float x;
    float y;
    std::uint8_t animationSet;
    std::uint16_t moveId;
    std::uint16_t skillValue;
    std::array<char, 256> moveName;
    std::array<char, 256> instruction;
    std::array<std::array<char, 37>, 6> descriptionLines;
};

void InitializeMovesScreenRuntimeData();
HLib::Task *EnqueueSYGH();
void CtrlTrainingHelp_SYGH_Callback(HLib::TaskData *taskData);
void SYGH_Destroy();
void SYGH_SetMoveSkill(std::uint16_t animationSet, std::uint16_t moveId, std::uint16_t value);
void SYGH_RemoveMoveSkill(std::uint16_t animationSet, std::uint16_t moveId);
std::uint16_t SYGH_GetMoveSkill(std::uint16_t animationSet, std::uint16_t moveId);
void SYGH_ApplyMajorStepMoveSkillProgression();
void SYGH_OpenMoveList(std::uint16_t animationSet);
void SYGH_SetSelectedMove(std::uint16_t selection);
const SYGHOverlayState &SYGH_GetOverlayState() noexcept;

} // namespace shenmue::s1
