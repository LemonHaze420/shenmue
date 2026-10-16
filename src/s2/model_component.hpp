#pragma once

#include "s2/model_component_types.hpp"
#include "s2/character_control.hpp"
#include "s2/party_companion.hpp"
#include "s2/cloth.hpp"
#include "s2/hand_model.hpp"
#include "s2/hand_pose.hpp"
#include "s2/model_runtime.hpp"
#include "s2/game_display.hpp"
#include "s2/object_xforms.hpp"
#include "s2/reflector_board.hpp"
#include "s2/character_alpha.hpp"
#include "s2/footstep.hpp"
#include "s2/tail_lamp.hpp"

namespace shenmue::s2 {

struct MotionCollisionCluster;
struct MotionPose;
struct MotionWork;
inline constexpr shen_name AREA_PATTERN_MAZE{make_shen_name('K', 'M', 'Z', '?')};
inline constexpr shen_name AREA_PATTERN_KOWLOON{make_shen_name('Q', '?', '?', '?')};
bool MatchAreaNamePattern(shen_name area, shen_name pattern);

void *AllocateModelComponent(Character *character, shen_name token, std::uint32_t payloadSize, ModelComponentUpdate update,
    ModelComponentCleanup cleanup = nullptr, ModelComponentCore core = nullptr);
void *FindModelComponent(const Character *character, shen_name token);

std::int32_t SetFlagForToken(Character *character, shen_name token, std::int32_t enabled);

// Human CUFF admission precedes MHND/REFB; hand models follow CCM_.
bool AttachCharacterPresentationComponents(Character *character, bool alphaManaged, bool reflectorBoardRecord = false);

bool AttachCharacterHandComponents(
    Character *character, const char *modelImage, const char *leftHand, const char *rightHand, std::uint8_t flags = 0, std::int32_t externalDrawControl = 0);

HLib::Task *QueueModelManagerTask(std::uint8_t nextFunctionIndex);

void SetModelComponentCore(Character *character, shen_name token, ModelComponentCore core);

void RunModelComponentCoreCallbacks(Character *character);
void UpdateModelComponents(Character *character);
void FreeModelComponents(Character *character);

} // namespace shenmue::s2
