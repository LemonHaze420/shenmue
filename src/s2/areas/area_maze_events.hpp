#pragma once
#include "common/types.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name MAZE_EVENT_RYO{SHEN_NAME(RYO_)}, MAZE_EVENT_SHENHUA{SHEN_NAME(SIN_)};
void RestoreMazeCompanionControl();
void MazeTimeOverToForest4Callback(void *taskData);
void MazeTimeOverTo552ECallback(void *taskData);
} // namespace shenmue::s2::areas
