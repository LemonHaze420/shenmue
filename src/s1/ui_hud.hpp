#pragma once
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1::ui_runtime_detail {

struct MoveLearnedPresentationOwner {
    std::array<char, 256> trainingText{};
    std::array<char, 256> moveNameText{};
    std::array<char, 256> resultText{};
    std::uint16_t savedUiState{};
    std::uint8_t mode{};
    std::uint8_t animationSetId{};
    std::uint16_t moveId{};
    bool active{};
};

// Game HUD: Practice (GameHudTraining.ui), and its search prompt set.
inline constexpr std::int32_t kUiGameHudPractice = 5;

inline constexpr std::array<std::int16_t, 4> kPracticeHudPromptIcons{48, 49, 47, 50};

inline constexpr std::int32_t kUiMoveLearned = 43;

inline constexpr std::size_t kHudPromptConfirm = 0;

inline constexpr std::size_t kHudPromptCancel = 1;

inline constexpr std::size_t kHudPromptEvent = 2;

inline constexpr std::size_t kHudPromptSystem = 3;

inline constexpr std::size_t kHudPromptCamera = 4;


inline constexpr std::array<const char *, 17> kGameHudWindowIds{"GameHUD::MainHudParent", "GameHUD::ButtonBParent", "GameHUD::ButtonBPanel", "GameHUD::ButtonB",
    "GameHUD::ButtonBIcon", "GameHUD::ButtonAPanel", "GameHUD::ButtonA", "GameHUD::ButtonAIcon", "GameHUD::ButtonYPanel", "GameHUD::ButtonY",
    "GameHUD::ButtonYIcon", "GameHUD::ButtonXPanel", "GameHUD::ButtonX", "GameHUD::ButtonXIcon", "GameHUD::CameraPanel", "GameHUD::CameraButtonPanel",
    "GameHUD::CameraIconPanel"};



inline constexpr std::array<const char *, 6> kGameHudSpriteIds{
    "GameHUD_ConfirmSprite", "GameHUD_CancelSprite", "GameHUD_EventSprite", "GameHUD_SystemSprite", "GameHUD_CameraSprite", "GameHUD_CameraStateSprite"};


inline constexpr std::array<const char *, 5> kGameHudButtonIconIds{
    "GameHUD_ConfirmIcon", "GameHUD_CancelIcon", "GameHUD_EventIcon", "GameHUD_SystemIcon", "GameHUD_CameraIcon"};


inline constexpr std::array<std::int32_t, 5> kSharedItemMenuPromptSet{0xffff, 44, 58, 0xffff, 0xffff};





inline constexpr std::size_t kUiGameHudRace = 0x42;

inline constexpr std::array<const char *, 3> kRaceHudDialImages{"RaceDial", "ForkliftDial", "RaceDialHand"};

inline constexpr std::array<const char *, 7> kRaceHudTextImages{
    "RaceText_En", "RaceText_En", "RaceText_Fr", "RaceText_De", "RaceText_Kr", "RaceText_CT", "RaceText_CS"};

inline constexpr std::array<const char *, 46> kRaceHudSprites{"Spr_Time", "Spr_Tim1", "Spr_Tim2", "Spr_Tim3", "Spr_Tim4", "Spr_Tim5", "Spr_Tim_S1",
    "Spr_Tim_S2", "Spr_Lap", "Spr_Lap_1", "Spr_Lap_2", "Spr_Lap1", "Spr_Lap1_1", "Spr_Lap1_2", "Spr_Lap1_3", "Spr_Lap1_4", "Spr_Lap1_5", "Spr_Lap1_S1",
    "Spr_Lap1_S2", "Spr_Lap2", "Spr_Lap2_1", "Spr_Lap2_2", "Spr_Lap2_3", "Spr_Lap2_4", "Spr_Lap2_5", "Spr_Lap2_S1", "Spr_Lap2_S2", "Spr_Lap3", "Spr_Lap3_1",
    "Spr_Lap3_2", "Spr_Lap3_3", "Spr_Lap3_4", "Spr_Lap3_5", "Spr_Lap3_S1", "Spr_Lap3_S2", "Spr_RTim", "Spr_RTim_1", "Spr_RTim_2", "Spr_RTim_3", "Spr_RTim_4",
    "Spr_RTim_5", "Spr_RTim_S1", "Spr_RTim_S2", "Spr_Rnk", "Spr_Rnk_1", "Spr_Rnk_2"};

inline constexpr std::array<const char *, 7> kRaceHudInfoWindows{
    "TopLeftInfo1", "TopLeftInfo_Lap", "TopLeftInfo2", "TopLeftInfo3", "TopLeftInfo4", "TopLeftInfo_Rank", "TopInfoRTimer"};

inline constexpr std::uint32_t kUiGameHudSkip = 74;

} // namespace shenmue::s1::ui_runtime_detail
