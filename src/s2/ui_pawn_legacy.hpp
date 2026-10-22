#pragma once
#include "d3t/ui_runtime.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s2 {
enum LegacyPawnFlyerMode : std::int32_t { PawnFlyerInspect = 0, PawnFlyerSets = 1, PawnFlyerItems = 2 };
inline constexpr std::array<std::array<char, 64>, 12> PAWN_LEGACY_SINGLE_WIDGETS{
    {"Pawnshop_Single_ButtonBar_AButton_Parent", "Pawnshop_Single_ButtonBar_AButton", "Pawnshop_Single_ButtonBar_AButton_Icon",
        "Pawnshop_Single_ButtonBar_AButton_Label", "Pawnshop_Single_ButtonBar_XButton_Parent", "Pawnshop_Single_ButtonBar_XButton",
        "Pawnshop_Single_ButtonBar_XButton_Icon", "Pawnshop_Single_ButtonBar_XButton_Label", "Pawnshop_Single_ButtonBar_BButton_Parent",
        "Pawnshop_Single_ButtonBar_BButton", "Pawnshop_Single_ButtonBar_BButton_Icon", "Pawnshop_Single_ButtonBar_BButton_Label"}};
inline constexpr std::array<std::array<char, 64>, 8> PAWN_LEGACY_SET_WIDGETS{{"Pawnshop_Set_ButtonBar_AButton_Parent", "Pawnshop_Set_ButtonBar_AButton",
    "Pawnshop_Set_ButtonBar_AButton_Icon", "Pawnshop_Set_ButtonBar_AButton_Label", "Pawnshop_Set_ButtonBar_BButton_Parent", "Pawnshop_Set_ButtonBar_BButton",
    "Pawnshop_Set_ButtonBar_BButton_Icon", "Pawnshop_Set_ButtonBar_BButton_Label"}};
inline constexpr std::array<const char *, 3> PAWN_LEGACY_HEADERS{"UIPAWNCOLL", "UIPAWNCOLL", "UIPAWNSING"};

std::uint32_t AdvanceLegacyPawnFlyerInit();
void BindLegacyPawnFlyer();
std::int64_t RenderLegacyPawnFlyer();
std::int64_t LegacyPawnFlyerInput();
void ConfigureLegacyPawnFlyer(std::int32_t shop, std::int32_t mode);
void SetLegacyPawnFlyerResult(std::int32_t result);
std::int64_t GetLegacyPawnFlyerResult();
void PublishLegacyPawnCollection();
void RefreshLegacyPawnItems();
void RefreshLegacyPawnSet(std::int32_t index);
void RefreshLegacyPawnValues();
void InitializeLegacyPawnFlyerNullsub();
std::int64_t LegacyPawnFlyerReady();
} // namespace shenmue::s2
