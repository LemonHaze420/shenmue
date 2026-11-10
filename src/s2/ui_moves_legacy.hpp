#pragma once
#include "d3t/ui_runtime.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s2 {
enum LegacyMovesTab : std::int32_t { LegacyMovesTabMoves = 0, LegacyMovesTabScrolls = 1 };
inline constexpr std::array<const char *, 3> LEGACY_MOVES_STAT_LAYOUTS{"Power_Space", "Speed_Space", "Rigid_Space"};

std::int64_t ResetLegacyMovesView();
std::int64_t HideLegacyMoves();
void FillScrollListScrollData(std::int32_t row);
void ShowLegacyMoves();
std::int64_t RenderLegacyMoves();
bool CanLearnLegacyMovesScroll(std::int32_t scroll);
std::int64_t ReloadLegacyMoves();
void UpdateLegacyMovesStats(std::int32_t row);
void SyncLegacyMovesPanelAlpha();
void LegacyMovesSceneCallback();
std::int64_t GetLegacyMovesInitState();
void BindLegacyMovesSceneImage();
void BindLegacyMoves();
void UpdateLegacyMovesButtonPanel();
std::int64_t LegacyMovesInput();
std::uint32_t AdvanceLegacyMovesInit();
void UpdateLegacyMovesMoveTab(std::int32_t previousRow);
void UpdateLegacyMovesScrollTab(std::int32_t previousRow);
void RefreshLegacyMovesMoveRows();
void RefreshLegacyMovesScrollRows();
std::int32_t GetWazamakiGroupCount(std::int32_t category) noexcept;
} // namespace shenmue::s2
