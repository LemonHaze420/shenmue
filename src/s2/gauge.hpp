#pragma once

#include "shenmue.h"

#include <cstdint>
#if !SHENMUE_IS_DC
#include <array>
#endif

#include "common/types.hpp"
namespace shenmue {
namespace d3t {
class UIDefinition;
struct UIGaugeState;
} // namespace d3t
} // namespace shenmue

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define TASK_FIGHT_GAUGE SHEN_NAME(GAGE)
const char *GetSpecialWfScrollLabel();
static const float FIGHT_GAUGE_BASE_POSITIONS[6][2] = {
    {66.0f, 414.0f}, {130.0f, 414.0f}, {194.0f, 414.0f}, {258.0f, 414.0f}, {322.0f, 414.0f}, {386.0f, 414.0f}};
#else
inline constexpr shen_name TASK_FIGHT_GAUGE = SHEN_NAME(GAGE);
inline constexpr std::array<std::array<float, 2>, 6> FIGHT_GAUGE_BASE_POSITIONS{
    {{66.0f, 78.0f}, {130.0f, 78.0f}, {194.0f, 78.0f}, {258.0f, 78.0f}, {322.0f, 78.0f}, {386.0f, 78.0f}}};
#endif
#if SHENMUE_IS_DC
static const float FIGHT_GAUGE_ORB_POSITIONS[12][2] = {
#else
inline constexpr std::array<std::array<float, 2>, 12> FIGHT_GAUGE_ORB_POSITIONS {{
#endif
    {-38.0f, 0.0f}, {-34.0f, -17.0f}, {-21.0f, -31.0f}, {-4.0f, -38.0f}, {14.0f, -35.0f}, {29.0f, -25.0f}, {38.0f, -8.0f}, {38.0f, 9.0f}, {29.0f, 26.0f},
    {14.0f, 36.0f}, {-4.0f, 39.0f}, {-21.0f, 32.0f}
#if SHENMUE_IS_DC
};
#else
}};
#endif
#if SHENMUE_IS_DC
static const std::int32_t FIGHT_GAUGE_DUAL_TEXTURES[2] = {1, 2};
#else
inline constexpr std::array<std::int32_t, 2> FIGHT_GAUGE_DUAL_TEXTURES{1, 2};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void CtrlGauge_Init();
std::int32_t LoadGauge();
std::int32_t ReleaseGauge();
bool IsFightHealthUIActive();
std::int32_t SetFightGaugeVisible(std::int32_t visible);
std::int32_t SetFightGaugeLayout(std::int32_t layout);
float UpdateDualFightGauge(float playerPercent, float enemyPercent);
void UpdateSingleFightGauge(float percent);
#else
// PC v1.07 CtrlGauge_Init/LoadGauge/ReleaseGauge lifecycle.
std::int64_t CtrlGauge_Init();
std::int64_t LoadGauge();
std::int64_t ReleaseGauge();
bool IsFightHealthUIActive();

std::int64_t SetFightGaugeVisible(std::int32_t visible);
std::int64_t SetFightGaugeLayout(std::int32_t layout);
double UpdateDualFightGauge(float playerPercent, float enemyPercent);
d3t::UIGaugeState *UpdateSingleFightGauge(float percent);

// Modern UI root 2 is a separate owner from the queue-8 GAGE task.
#if !SHENMUE_IS_DC
inline constexpr std::array<const char *, 3> FIGHT_GAUGE_PANELS{"HUDGauge::Round_Panel_1P", "HUDGauge::Round_Panel_2P", "HUDGauge::LevelBar"};
#endif
#if !SHENMUE_IS_DC
inline constexpr std::array<const char *, 5> FIGHT_GAUGE_ROUND_IMAGES{"HUDGauge::BarOutline_Image", "HUDGauge::Round_Counter_1P_1",
    "HUDGauge::Round_Counter_1P_2", "HUDGauge::Round_Counter_2P_1", "HUDGauge::Round_Counter_2P_2"};
#endif
void BindFightGaugeDefinition(d3t::UIDefinition &definition);
void UpdateFightGaugeDefinition();
void ReleaseFightGaugeDefinition();
bool IsFightGaugeInputActive();
bool IsFightGaugeOverlayInputActive();
std::int32_t GetFightGaugeDisplayLayout();
void SetFightGaugeDisplayLayout(std::int32_t layout);
void SetPlayerEnemyPercents(float player, float enemy);
d3t::UIGaugeState *SetFightGaugePercentSingle(float percent);
void SetFightGaugeSecondaryOverlay(std::int32_t enabled);
void SetFightGaugePrimaryUniform(std::int32_t uniform);
void SetFightGaugePrimaryVisuals(std::int32_t uniform, float alpha);
void FillFightGaugeSecondaryOrbStates(std::int32_t colour);
void ActivateFightGaugePrimary();
void SetFightGaugeTrainingProgress(float progress);
void ClearFightGaugeTrainingVisibility();

#endif

} // namespace s2
} // namespace shenmue
