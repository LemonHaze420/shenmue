#pragma once
#include <array>
#include <cstdint>

namespace shenmue::s1::ui_runtime_detail {

inline constexpr std::int32_t kUiGameTraining = 3;


inline constexpr std::array<const char *, 5> kGameTrainingFocusTitles{nullptr, "UITRAIALL2", "UITRAIHAN2", "UITRAILEG2",
    "UITRAITHR2"}; // @text UITRAIALL2 EN: "Train All Moves Equally" | JA: "全技をまんべんなく鍛錬します。" ; UITRAIHAN2 EN: "Train Hand Moves Only" | JA: "手技のみ鍛錬します。" ; UITRAILEG2 EN: "Train Leg Moves Only" | JA: "足技のみ鍛錬します。" ; UITRAITHR2 EN: "Train Throw Moves Only" | JA: "投技のみ鍛錬します。"


inline constexpr std::array<const char *, 84> kGameTrainingLabelImages{"GameTraining::AllMovesOff_En", "GameTraining::HandMovesOff_En",
    "GameTraining::LegMovesOff_En", "GameTraining::ThrowMovesOff_En", "GameTraining::AllMovesOn_En", "GameTraining::HandMovesOn_En",
    "GameTraining::LegMovesOn_En", "GameTraining::ThrowMovesOn_En", "GameTraining::TrainAllMoves_En", "GameTraining::TrainHandMoves_En",
    "GameTraining::TrainLegMoves_En", "GameTraining::TrainThrowMoves_En", "GameTraining::AllMovesOff_Fr", "GameTraining::HandMovesOff_Fr",
    "GameTraining::LegMovesOff_Fr", "GameTraining::ThrowMovesOff_Fr", "GameTraining::AllMovesOn_Fr", "GameTraining::HandMovesOn_Fr",
    "GameTraining::LegMovesOn_Fr", "GameTraining::ThrowMovesOn_Fr", "GameTraining::TrainAllMoves_Fr", "GameTraining::TrainHandMoves_Fr",
    "GameTraining::TrainLegMoves_Fr", "GameTraining::TrainThrowMoves_Fr", "GameTraining::AllMovesOff_Ge", "GameTraining::HandMovesOff_Ge",
    "GameTraining::LegMovesOff_Ge", "GameTraining::ThrowMovesOff_Ge", "GameTraining::AllMovesOn_Ge", "GameTraining::HandMovesOn_Ge",
    "GameTraining::LegMovesOn_Ge", "GameTraining::ThrowMovesOn_Ge", "GameTraining::TrainAllMoves_Ge", "GameTraining::TrainHandMoves_Ge",
    "GameTraining::TrainLegMoves_Ge", "GameTraining::TrainThrowMoves_Ge", "GameTraining::AllMovesOff_Kr", "GameTraining::HandMovesOff_Kr",
    "GameTraining::LegMovesOff_Kr", "GameTraining::ThrowMovesOff_Kr", "GameTraining::AllMovesOn_Kr", "GameTraining::HandMovesOn_Kr",
    "GameTraining::LegMovesOn_Kr", "GameTraining::ThrowMovesOn_Kr", "GameTraining::TrainAllMoves_Kr", "GameTraining::TrainHandMoves_Kr",
    "GameTraining::TrainLegMoves_Kr", "GameTraining::TrainThrowMoves_Kr", "GameTraining::AllMovesOff_Tc", "GameTraining::HandMovesOff_Tc",
    "GameTraining::LegMovesOff_Tc", "GameTraining::ThrowMovesOff_Tc", "GameTraining::AllMovesOn_Tc", "GameTraining::HandMovesOn_Tc",
    "GameTraining::LegMovesOn_Tc", "GameTraining::ThrowMovesOn_Tc", "GameTraining::TrainAllMoves_Tc", "GameTraining::TrainHandMoves_Tc",
    "GameTraining::TrainLegMoves_Tc", "GameTraining::TrainThrowMoves_Tc", "GameTraining::AllMovesOff_Sc", "GameTraining::HandMovesOff_Sc",
    "GameTraining::LegMovesOff_Sc", "GameTraining::ThrowMovesOff_Sc", "GameTraining::AllMovesOn_Sc", "GameTraining::HandMovesOn_Sc",
    "GameTraining::LegMovesOn_Sc", "GameTraining::ThrowMovesOn_Sc", "GameTraining::TrainAllMoves_Sc", "GameTraining::TrainHandMoves_Sc",
    "GameTraining::TrainLegMoves_Sc", "GameTraining::TrainThrowMoves_Sc", "GameTraining::AllMovesOff_Jp", "GameTraining::HandMovesOff_Jp",
    "GameTraining::LegMovesOff_Jp", "GameTraining::ThrowMovesOff_Jp", "GameTraining::AllMovesOn_Jp", "GameTraining::HandMovesOn_Jp",
    "GameTraining::LegMovesOn_Jp", "GameTraining::ThrowMovesOn_Jp", "GameTraining::TrainAllMoves_Jp", "GameTraining::TrainHandMoves_Jp",
    "GameTraining::TrainLegMoves_Jp", "GameTraining::TrainThrowMoves_Jp"};


inline constexpr std::array<const char *, 5> kGameTrainingLabelIds{
    "GameTraining::TrainMoves", "GameTraining::AllMoves", "GameTraining::HandMoves", "GameTraining::ThrowMoves", "GameTraining::LegMoves"};


inline constexpr std::array<const char *, 3> kGameTrainingResultPanels{
    "GameTraining::ResultParent", "GameTraining::ResultTexture", "GameTraining::ResultBorder"};

inline constexpr std::array<const char *, 8> kGameTrainingPickerPanels{"GameTraining::RootPanel", "GameTraining::MainPanel", "GameTraining::CentrePanel",
    "GameTraining::CentreTextPanel", "GameTraining::UpParent", "GameTraining::LeftParent", "GameTraining::DownParent", "GameTraining::RightParent"};





inline constexpr std::array<const char *, 4> kGameTrainingLegacySets{"All", "Hand", "Leg", "Throw"};

} // namespace shenmue::s1::ui_runtime_detail
