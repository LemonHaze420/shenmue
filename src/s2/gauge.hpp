#pragma once

#include "shenmue.h"

#include <cstdint>
#include <array>

#include "common/types.hpp"
namespace shenmue { namespace d3t {
class UIDefinition;
struct UIGaugeState;
} }

#define SHENMUE_GAUGE_NOEXCEPT noexcept

namespace shenmue { namespace s2 {

inline constexpr shen_name TASK_FIGHT_GAUGE = SHEN_NAME(EGAG);
inline constexpr std::array<std::array<float, 2>, 6> FIGHT_GAUGE_BASE_POSITIONS{{
    {66.0f, 78.0f}, {130.0f, 78.0f}, {194.0f, 78.0f}, {258.0f, 78.0f}, {322.0f, 78.0f}, {386.0f, 78.0f}}};
inline constexpr std::array<std::array<float, 2>, 12> FIGHT_GAUGE_ORB_POSITIONS{{
    {-38.0f, 0.0f}, {-34.0f, -17.0f}, {-21.0f, -31.0f}, {-4.0f, -38.0f}, {14.0f, -35.0f}, {29.0f, -25.0f},
    {38.0f, -8.0f}, {38.0f, 9.0f}, {29.0f, 26.0f}, {14.0f, 36.0f}, {-4.0f, 39.0f}, {-21.0f, 32.0f}
}};
inline constexpr std::array<std::int32_t, 2> FIGHT_GAUGE_DUAL_TEXTURES{1, 2};


std::int64_t CtrlGauge_Init() SHENMUE_GAUGE_NOEXCEPT;
std::int64_t LoadGauge();
std::int64_t ReleaseGauge();
bool IsFightHealthUIActive() SHENMUE_GAUGE_NOEXCEPT;

std::int64_t SetFightGaugeVisible(std::int32_t visible) SHENMUE_GAUGE_NOEXCEPT;
std::int64_t SetFightGaugeLayout(std::int32_t layout) SHENMUE_GAUGE_NOEXCEPT;
double UpdateDualFightGauge(float playerPercent, float enemyPercent) SHENMUE_GAUGE_NOEXCEPT;
d3t::UIGaugeState *UpdateSingleFightGauge(float percent) SHENMUE_GAUGE_NOEXCEPT;

// Modern UI root 2 is a separate owner from the queue-8 EGAG task.
inline constexpr std::array<const char *, 3> FIGHT_GAUGE_PANELS{"HUDGauge::Round_Panel_1P", "HUDGauge::Round_Panel_2P", "HUDGauge::LevelBar"};
inline constexpr std::array<const char *, 5> FIGHT_GAUGE_ROUND_IMAGES{"HUDGauge::BarOutline_Image", "HUDGauge::Round_Counter_1P_1",
    "HUDGauge::Round_Counter_1P_2", "HUDGauge::Round_Counter_2P_1", "HUDGauge::Round_Counter_2P_2"};
void BindFightGaugeDefinition(d3t::UIDefinition &definition);
void UpdateFightGaugeDefinition();
void ReleaseFightGaugeDefinition() SHENMUE_GAUGE_NOEXCEPT;
bool IsFightGaugeInputActive() SHENMUE_GAUGE_NOEXCEPT;
bool IsFightGaugeOverlayInputActive() SHENMUE_GAUGE_NOEXCEPT;
std::int32_t GetFightGaugeDisplayLayout() SHENMUE_GAUGE_NOEXCEPT;
void SetFightGaugeDisplayLayout(std::int32_t layout) SHENMUE_GAUGE_NOEXCEPT;
void SetPlayerEnemyPercents(float player, float enemy) SHENMUE_GAUGE_NOEXCEPT;
d3t::UIGaugeState *SetFightGaugePercentSingle(float percent) SHENMUE_GAUGE_NOEXCEPT;
void SetFightGaugeSecondaryOverlay(std::int32_t enabled) SHENMUE_GAUGE_NOEXCEPT;
void SetFightGaugePrimaryUniform(std::int32_t uniform) SHENMUE_GAUGE_NOEXCEPT;
void SetFightGaugePrimaryVisuals(std::int32_t uniform, float alpha) SHENMUE_GAUGE_NOEXCEPT;
void FillFightGaugeSecondaryOrbStates(std::int32_t colour) SHENMUE_GAUGE_NOEXCEPT;
void ActivateFightGaugePrimary() SHENMUE_GAUGE_NOEXCEPT;
void SetFightGaugeTrainingProgress(float progress) SHENMUE_GAUGE_NOEXCEPT;
void ClearFightGaugeTrainingVisibility() SHENMUE_GAUGE_NOEXCEPT;


} } // namespace shenmue::s2

#undef SHENMUE_GAUGE_NOEXCEPT
