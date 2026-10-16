#pragma once
#include "s1/ui_runtime.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s1::ui_runtime_detail {

struct ActionSelectorOwner {
    UIActionSelectorRequest request{-1, -1, -1, -1, 600, -1, 0};
    UIActionSelectorState state{UIActionSelectorState::Idle};
    bool openRequested{};
    std::int16_t selectedHelpToken{-1};
    std::int16_t elapsedFrames{};
    std::int8_t selectedChoice{-1};
    float panelStartX{-176.0f};
    float panelTargetX{144.0f};
    float panelHalfWidth{};
    std::array<std::int16_t, 4> choiceWidths{128, 128, 128, 128};
    float panelX{-176.0f};
    float panelVelocity{1.0f};
    std::uint8_t savedHelpToken{};
    std::uint8_t savedHelpButton{};
    std::int32_t savedPauseState{};
    bool resultAvailable{};
    bool resetRequested{};
    bool hasSelectedChoice{};
    bool valueModeActive{};
    std::array<std::int32_t, 4> displayChoiceTokens{};
    std::int32_t helpDetailsIndex{-1};
    std::uint32_t simulatedDirectionInput{};
    std::int32_t valueStep{};
    std::int32_t displayedValue{};
    std::int32_t maximumValue{};
    bool labelsDirty{};

    std::int32_t useValueLabels{};
};



struct ActionSelectorChoiceUISlot {
    UIButtonIconWidget *slotUi;
    d3t::UIImageWidget *braceUi;
    d3t::UILabelWidget *labelUi;
    std::array<char, 256> text;
};

static_assert(sizeof(ActionSelectorChoiceUISlot) == 0x118);

inline constexpr std::array<const char *, 4> kActionSelectorButtonIds{
    "ActionSelector::UpButton", "ActionSelector::LeftButton", "ActionSelector::RightButton", "ActionSelector::DownButton"};

inline constexpr std::array<const char *, 4> kActionSelectorBraceIds{
    "ActionSelector::UpBrace", "ActionSelector::LeftBrace", "ActionSelector::RightBrace", "ActionSelector::DownBrace"};

inline constexpr std::array<const char *, 4> kActionSelectorChoicePanelIds{
    "ActionSelector::UpPanel", "ActionSelector::LeftPanel", "ActionSelector::RightPanel", "ActionSelector::DownPanel"};

inline constexpr std::array<const char *, 4> kActionSelectorLabelIds{
    "ActionSelector::UpButtonLabel", "ActionSelector::LeftButtonLabel", "ActionSelector::RightButtonLabel", "ActionSelector::DownButtonLabel"};

inline constexpr std::array<const char *, 3> kActionSelectorMainPanelIds{
    "ActionSelector::MainPanel_OUT", "ActionSelector::MainPanel_IN", "ActionSelector::MainPanel"};

inline constexpr std::array<const char *, 76> kActionSelectorStringIds{{
    "UIACTSDUMM", "UIACTSDNTR", "UIACTSHELP",
    "UIACTSDNHE", // @text UIACTSDUMM EN: "Dummy" | JA: "ダミー" ; UIACTSDNTR EN: "Don't Try" | JA: "やめる" ; UIACTSHELP EN: "Help" | JA: "する" ; UIACTSDNHE EN: "Don't Help" | JA: "しない"
    "UIACTSBUYM", "UIACTSMOTH", "UIACTSNINE",
    "UIACTSFATH", // @text UIACTSBUYM EN: "Buy More" | JA: "続ける" ; UIACTSMOTH EN: "Mother's Earth" | JA: "母なる地" ; UIACTSNINE EN: "Nine Dragons" | JA: "九つの龍" ; UIACTSFATH EN: "Father's Heaven" | JA: "父なる天"
    "UIACTSCOMR", "UIMISS0000", "UIMISS0000", "UIMISS0000", // @text EN: "Comrades" | JA: "我が友"
    "UIMISS0000", "UIMISS0000", "UIMISS0000", "UIMISS0000", "UIMISS0000", "UIACTSDNSP", "UIACTSHELL",
    "UIACTSTELN", // @text UIACTSDNSP EN: "Don't Speak" | JA: "喋らない" ; UIACTSHELL EN: "Hello" | JA: "もしもし" ; UIACTSTELN EN: "Tel.Number" | JA: "電話番号"
    "UIACTSABOU", "UIACTSDRIE", "UIACTSFRIE",
    "UIACTSMIMI", // @text UIACTSABOU EN: "About Letter" | JA: "手紙の事" ; UIACTSDRIE EN: "Dried Fish" | JA: "煮干し" ; UIACTSFRIE EN: "Fried Tofu" | JA: "油揚げ" ; UIACTSMIMI EN: "Mimi" | JA: "ミミ"
    "UIACTSTORA", "UIACTSCHIB", "UIACTSTAMA",
    "UIACTSWARE", // @text UIACTSTORA EN: "Tora" | JA: "トラ" ; UIACTSCHIB EN: "Chibi" | JA: "チビ" ; UIACTSTAMA EN: "Tama" | JA: "タマ" ; UIACTSWARE EN: "Warehouse#8" | JA: "第８倉庫"
    "UIACTSAREA", "UIACTSADDR", "UIACTSCHEN",
    "UIACTSPET0", // @text UIACTSAREA EN: "Area" | JA: "地域" ; UIACTSADDR EN: "Address" | JA: "住所" ; UIACTSCHEN EN: "Chen" | JA: "陳" ; UIACTSPET0 EN: "Pet" | JA: "なでる"
    "UIACTSLOOK", "UIACTSCALL", "UIACTSQUIT",
    "UIACTSSASU", // @text UIACTSLOOK EN: "Look" | JA: "ながめる" ; UIACTSCALL EN: "Call" | JA: "呼ぶ" ; UIACTSQUIT EN: "Quit" | JA: "やめる" ; UIACTSSASU EN: "Sasuke" | JA: "サスケ"
    "UIACTSPLAY", "UIACTSCONT", "UIACTSYES0",
    "UIACTSNO00", // @text UIACTSPLAY EN: "Play" | JA: "プレイする" ; UIACTSCONT EN: "Continue" | JA: "続ける" ; UIACTSYES0 EN: "Yes" | JA: "オン" ; UIACTSNO00 EN: "No" | JA: "しない"
    "UIACTSBUY0", "UIACTSDNBU", "UIACTSSPAC",
    "UIACTSHANG", // @text UIACTSBUY0 EN: "Buy" | JA: "買う" ; UIACTSDNBU EN: "Don't Buy" | JA: "買わない" ; UIACTSSPAC EN: "Space" | JA: "ハリアー" ; UIACTSHANG EN: "Hang On" | JA: "ハングオン"
    "UIMISS0000", "UIMISS0000", "UIACTSPLAY", "UIACTSDNPL", // @text UIACTSPLAY EN: "Play" | JA: "プレイする" ; UIACTSDNPL EN: "Don't Play" | JA: "やめる"
    "UIMISS0000", "UIACTSTRY0", "UIMISS0000", "UIMISS0000", // @text EN: "Try" | JA: "占う"
    "UIACTSGAMB", "UIACTSDOBU", "UIACTSSAKU",
    "UIACTSYAMA", // @text UIACTSGAMB EN: "Gamble" | JA: "勝利の扉" ; UIACTSDOBU EN: "Dobuita" | JA: "ドブ板" ; UIACTSSAKU EN: "Sakuragaoka" | JA: "桜ヶ丘" ; UIACTSYAMA EN: "Yamanose" | JA: "山の瀬"
    "UIACTSYARD", "UIMISS0000", "UIACTSPRAC",
    "UIACTSJOIN", // @text UIACTSYARD EN: "Yard" | JA: "庭" ; UIACTSPRAC EN: "Practice" | JA: "練習する" ; UIACTSJOIN EN: "Join" | JA: "参加する"
    "UIACTSDECL", "UIMISS0000", "UIACTSACCE",
    "UIACTSGVDE", // @text UIACTSDECL EN: "Decline" | JA: "断る" ; UIACTSACCE EN: "Accept" | JA: "教わる" ; UIACTSGVDE EN: "Give Demo" | JA: "見せる"
    "UIACTSCLUE", "UIACTSFUTU", "UIMISS0000", "UIMISS0000", // @text UIACTSCLUE EN: "Clue" | JA: "幸運の鍵" ; UIACTSFUTU EN: "Future" | JA: "運命の糸"
    "UIACTSRIGH", "UIACTSLEFT", "UIACTSSLEE",
    "UIACTSSAVE", // @text UIACTSRIGH EN: "Right" | JA: "右" ; UIACTSLEFT EN: "Left" | JA: "左" ; UIACTSSLEE EN: "Sleep" | JA: "寝る" ; UIACTSSAVE EN: "Save" | JA: "記録"
    "UIACTSSETT", "UIACTSTRAI", "UIBUTTCANC",
    "UIASBETMAX", // @text UIACTSSETT EN: "Settings" | JA: "設定" ; UIACTSTRAI EN: "Training" | JA: "修行" ; UIBUTTCANC EN: "Cancel" | JA: "キャンセル" ; UIASBETMAX EN: "MAX" | JA: "MAX"
}};


inline constexpr std::array<std::uint8_t, 8> kActionSelectorChoiceByInputCode{0, 4, 3, 4, 2, 4, 1, 4};

} // namespace shenmue::s1::ui_runtime_detail
