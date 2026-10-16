#pragma once
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct SaveMenuSlot {
    std::int32_t valid, index;
    shen_name map;
    std::int32_t area;
    std::array<char, 128> gameTime;
    std::array<char, 64> locationName, areaName;
    std::array<char, 128> saveTime;
};
static_assert(sizeof(SaveMenuSlot) == 400 && offsetof(SaveMenuSlot, gameTime) == 16 && offsetof(SaveMenuSlot, areaName) == 208);
static_assert(offsetof(SaveMenuSlot, saveTime) == 272);
inline constexpr std::array<const char *, 9> UI_SAVE_WINDOWS{"LoadScreen::Panel_GridHolder", "LoadScreen::Load_Screen_Panel", "LoadScreen::LoadEntry_Parent_#",
    "LoadScreen::LoadEntry_Background_#", "LoadScreen::LoadEntry_Texture_#", "LoadScreen::LoadEntry_Shading_#", "LoadScreen::LoadEntry_Icon_#",
    "LoadScreen::LoadEntry_MidText_#", "LoadScreen::LoadEntry_RightText_#"};
inline constexpr std::array<const char *, 7> UI_SAVE_LABELS{"LoadScreen::LoadEntry_Number", "LoadScreen::LoadEntry_GameLabel",
    "LoadScreen::LoadEntry_DataLabel", "LoadScreen::LoadEntry_PlaceName", "LoadScreen::LoadEntry_GameTime", "LoadScreen::LoadEntry_SaveTimeLabel",
    "LoadScreen::LoadEntry_SaveTime"};
inline constexpr std::array<const char *, 12> UI_SAVE_LAYOUTS{"LoadScreen::LoadEntry_Background_Spacepush", "LoadScreen::LoadEntryBackground_Game_Shading",
    "LoadScreen::LoadEntryBackground_Front_Shading", "LoadScreen::LoadEntry_ShadingSelected_Spacepush", "LoadScreen::LoadEntry_Icon_Spacepush",
    "LoadScreen::LoadEntry_Number_Spacepush", "LoadScreen::LoadEntry_GameLabel_Spacepush", "LoadScreen::LoadEntry_DataLabel_Spacepush",
    "LoadScreen::LoadEntry_PlaceName_Spacepush", "LoadScreen::LoadEntry_GameTime_Spacepush", "LoadScreen::LoadEntry_SaveTimeLabel_Spacepush",
    "LoadScreen::LoadEntry_SaveTime_Spacepush"};
inline constexpr std::array<const char *, 10> UI_SAVE_COLOUR_IMAGES{"LoadScreen::MainTexture_Game", "LoadScreen::MainTexture_Front",
    "LoadScreen::LoadEntryTexture_Game", "LoadScreen::LoadEntryTexture_Game2", "LoadScreen::LoadEntryTexture_Front2", "LoadScreen::LoadEntryTexture_Front",
    "LoadScreen::LoadEntryBackground_Game", "LoadScreen::LoadEntryBackground_Front", "LoadScreen::LoadEntryBackground_Selected", "LoadScreen::LoadRowIcon"};
inline constexpr std::array<const char *, 8> UI_SAVE_IMAGES{"LoadBackground", "LoadScreen::ImageFrame", "LoadScreen::ImageBase",
    "LoadScreen::MapAreaBackground", "LoadScreen::AreaDefault", "LoadScreen::MapDefault", "LoadScreen::AreaNoSave", "LoadScreen::MapNoSave"};
inline constexpr std::array<const char *, 2> UI_SAVE_BUTTONS{"LoadScreen::MainPanelButton", "LoadScreen::LoadEntry_Background"};
inline constexpr std::array<const char *, 6> UI_SAVE_AREA_IMAGES{
    "LoadScreen::Area_Aba", "LoadScreen::Area_Wan", "LoadScreen::Area_Kow", "LoadScreen::Area_Hei", "LoadScreen::Area_SM1", "LoadScreen::Area_SM2"};
inline constexpr std::array<const char *, 22> UI_SAVE_MAP_IMAGES{"LoadScreen::MAPS_AK00", "LoadScreen::MAPS_AR00", "LoadScreen::MAPS_AR03",
    "LoadScreen::MAPS_AB00", "LoadScreen::MAPS_WS00", "LoadScreen::MAPS_WK00", "LoadScreen::MAPS_WE00", "LoadScreen::MAPS_WN00", "LoadScreen::MAPS_WT00",
    "LoadScreen::MAPS_WB00", "LoadScreen::MAPS_WR00", "LoadScreen::MAPS_QR00", "LoadScreen::MAPS_Q100", "LoadScreen::MAPS_Q200", "LoadScreen::MAPS_Q300",
    "LoadScreen::MAPS_QA00", "LoadScreen::MAPS_QB00", "LoadScreen::MAPS_QC00", "LoadScreen::MAPS_QD00", "LoadScreen::MAPS_QE00", "LoadScreen::MAPS_QF00",
    "LoadScreen::MAPS_QUG0"};
inline constexpr std::array<std::int16_t, 5> UI_SAVE_HINTS{45, 44, -1, -1, -1};
inline constexpr std::array<std::int32_t, 3> UI_SAVE_FRONT_IMAGES{1, 4, 5}, UI_SAVE_GAME_IMAGES{0, 2, 3};
inline constexpr std::array<std::uint32_t, 2> UI_SAVE_FRONT_COLOURS{0xff3f6a70U, 0xbe0c222fU}, UI_SAVE_GAME_COLOURS{0xff322b1cU, 0xdc272415U};
struct SaveMenuArea {
    shen_name pattern;
    std::int32_t image;
};
inline constexpr shen_name SAVE_MAP_0200{SHEN_NAME(0200)}, SAVE_MAP_WAT0{SHEN_NAME(WAT0)}, SAVE_MAP_KRF1{SHEN_NAME(KRF1)};
inline constexpr shen_name SAVE_MAP_CLS1{SHEN_NAME(CLS1)}, SAVE_MAP_CLS2{SHEN_NAME(CLS2)};
inline constexpr std::array<SaveMenuArea, 33> UI_SAVE_AREAS{
    {{.pattern = {SHEN_NAME(AK**)}, .image = 0}, {.pattern = {SHEN_NAME(AR**)}, .image = 1}, {.pattern = {SHEN_NAME(AR03)}, .image = 2},
        {.pattern = {SHEN_NAME(AB00)}, .image = 3}, {.pattern = {SHEN_NAME(WS**)}, .image = 4}, {.pattern = {SHEN_NAME(WK**)}, .image = 5},
        {.pattern = {SHEN_NAME(WE**)}, .image = 6}, {.pattern = {SHEN_NAME(WN**)}, .image = 7}, {.pattern = {SHEN_NAME(WT**)}, .image = 8},
        {.pattern = {SHEN_NAME(WB**)}, .image = 9}, {.pattern = {SHEN_NAME(WR**)}, .image = 10}, {.pattern = {SHEN_NAME(QR**)}, .image = 11},
        {.pattern = {SHEN_NAME(Q1**)}, .image = 12}, {.pattern = {SHEN_NAME(Q2**)}, .image = 13}, {.pattern = {SHEN_NAME(Q3**)}, .image = 14},
        {.pattern = {SHEN_NAME(QA**)}, .image = 15}, {.pattern = {SHEN_NAME(QB**)}, .image = 16}, {.pattern = {SHEN_NAME(QC**)}, .image = 17},
        {.pattern = {SHEN_NAME(QD**)}, .image = 18}, {.pattern = {SHEN_NAME(QE**)}, .image = 19}, {.pattern = {SHEN_NAME(QF**)}, .image = 20},
        {.pattern = {SHEN_NAME(QUG0)}, .image = 21}, {.pattern = {SHEN_NAME(KRH1)}, .image = 22}, {.pattern = {SHEN_NAME(KWM1)}, .image = 23},
        {.pattern = {SHEN_NAME(KRM3)}, .image = 24}, {.pattern = {SHEN_NAME(KWW1)}, .image = 25}, {.pattern = {SHEN_NAME(KRC1)}, .image = 26},
        {.pattern = {SHEN_NAME(KRK*)}, .image = 27}, {.pattern = {SHEN_NAME(KSH1)}, .image = 28}, {.pattern = {SHEN_NAME(KWW4)}, .image = 29},
        {.pattern = {SHEN_NAME(KES1)}, .image = 30}, {.pattern = SAVE_MAP_CLS1, .image = 0}, {.pattern = SAVE_MAP_CLS2, .image = 22}}};
inline constexpr std::array<SaveMenuArea, 19> UI_SAVE_AREA_ALIASES{{{.pattern = {SHEN_NAME(ARZ0)}, .image = 2}, {.pattern = {SHEN_NAME(QSFB)}, .image = 13},
    {.pattern = {SHEN_NAME(QGBT)}, .image = 13}, {.pattern = {SHEN_NAME(QSFC)}, .image = 13}, {.pattern = {SHEN_NAME(QKBT)}, .image = 20},
    {.pattern = {SHEN_NAME(QC00)}, .image = 17}, {.pattern = {SHEN_NAME(QSFD)}, .image = 12}, {.pattern = {SHEN_NAME(QSFA)}, .image = 12},
    {.pattern = {SHEN_NAME(QTB1)}, .image = 12}, {.pattern = {SHEN_NAME(QTB2)}, .image = 14}, {.pattern = {SHEN_NAME(QTB3)}, .image = 14},
    {.pattern = {SHEN_NAME(QTB4)}, .image = 13}, {.pattern = {SHEN_NAME(QEHH)}, .image = 15}, {.pattern = {SHEN_NAME(QRC0)}, .image = 15},
    {.pattern = {SHEN_NAME(QJBT)}, .image = 15}, {.pattern = {SHEN_NAME(QLBT)}, .image = 19}, {.pattern = {SHEN_NAME(KMZ*)}, .image = 27},
    {.pattern = SAVE_MAP_0200, .image = 11}, {.pattern = SAVE_MAP_KRF1, .image = 22}}};
inline constexpr std::array<const char*, 6> UI_SAVE_LOCATION_TOKENS{"UIMAREABER", "UIMAREWACH", "UIMAREKOWL", "UIMAREGUIL", "UIMARECLDA", nullptr}; // @text UIMAREABER EN: "Aberdeen" | JA: "香港仔" ; UIMAREWACH EN: "Wan Chai" | JA: "湾仔" ; UIMAREKOWL EN: "Kowloon" | JA: "九龍城" ; UIMAREGUIL EN: "Guilin" | JA: "桂林" ; UIMARECLDA EN: "Cleared Game File" | JA: "クリアファイル"
inline constexpr std::array<const char*, 33> UI_SAVE_AREA_TOKENS{"UISAREFOPI", "UISAREWOPI", "UISAREQUST", "UISAREBHW0", "UISAREGMQ0", // @text UISAREFOPI EN: "Fortune's Pier" | JA: "開運碼頭" ; UISAREWOPI EN: "Worker's Pier" | JA: "労湾碼頭" ; UISAREQUST EN: "Queen's St." | JA: "クイーンズストリート" ; UISAREBHW0 EN: "Beverly Hills Wharf" | JA: "ビバリーヒルズワーフ" ; UISAREGMQ0 EN: "Green Market Qr." | JA: "菜珍街"
    "UISARESCQ0", "UISAREGOQR", "UISARELCQ0", "UISAREWMQ0", "UISARESCHI", "UISAREWDQ0", "UISAREDRST", "UISARETWQ0", "UISARESTQR", // @text UISARESCQ0 EN: "South Carmain Qr." | JA: "紅南街" ; UISAREGOQR EN: "Golden Qr." | JA: "詠黄街" ; UISARELCQ0 EN: "Lucky Charm Qr." | JA: "福仙街" ; UISAREWMQ0 EN: "Wise Men's Qr." | JA: "太老街" ; UISARESCHI EN: "Scarlet Hills" | JA: "紅華台" ; UISAREWDQ0 EN: "White Dynasty Qr." | JA: "白秦街" ; UISAREDRST EN: "Dragon St." | JA: "龍影街" ; UISARETWQ0 EN: "Thousand White Qr." | JA: "白千街" ; UISARESTQR EN: "Stand Qr." | JA: "屋台街"
    "UISAREDIQR", "UISAREABLD", "UISAREBBLD", "UISARECBLD", "UISAREDBLD", "UISAREEBLD", "UISAREFBLD", "UISAREUNDE", "UISARELANG", // @text UISAREDIQR EN: "Dimsum Qr." | JA: "点心街" ; UISAREABLD EN: "A bldg." | JA: "九龍城Ａビル" ; UISAREBBLD EN: "B bldg." | JA: "九龍城Ｂビル" ; UISARECBLD EN: "C bldg." | JA: "九龍城Ｃビル" ; UISAREDBLD EN: "D bldg." | JA: "九龍城Ｄビル" ; UISAREEBLD EN: "E bldg." | JA: "九龍城Ｅビル" ; UISAREFBLD EN: "F bldg." | JA: "九龍城Ｆビル" ; UISAREUNDE EN: "Underground" | JA: "地下" ; UISARELANG EN: "Langhuishan" | JA: "狼回山"
    "UISAREGRFI", "UISAREYING", "UISAREPTAW", "UISARECRAG", "UISAREFORE", "UISARESHHO", "UISARECBT0", "UISARESTPI", "UISARESHM1", "UISARESHM2"}; // @text UISAREGRFI EN: "Green Field" | JA: "緑野" ; UISAREYING EN: "Yingshuihe" | JA: "蛍水川" ; UISAREPTAW EN: "Path Through a Wood" | JA: "林道" ; UISARECRAG EN: "Crag" | JA: "岩峰" ; UISAREFORE EN: "Forest" | JA: "森林" ; UISARESHHO EN: "Shenhua's House" | JA: "莎花の家" ; UISARECBT0 EN: "Cloud Bird Trail" | JA: "雲鳥道" ; UISARESTPI EN: "Stone Pit" | JA: "石切場" ; UISARESHM1 EN: "Shenmue I" | JA: "シェンムー" ; UISARESHM2 EN: "Shenmue II" | JA: "シェンムー II"
inline constexpr std::array<const char*, 7> UI_SAVE_WEEKDAYS{"UILOADSUN0", "UILOADMON0", "UILOADTUE0", "UILOADWED0", "UILOADTHU0", "UILOADFRI0", "UILOADSAT0"}; // @text UILOADSUN0 EN: "Sun." | JA: "日" ; UILOADMON0 EN: "Mon." | JA: "月" ; UILOADTUE0 EN: "Tue." | JA: "火" ; UILOADWED0 EN: "Wed." | JA: "水" ; UILOADTHU0 EN: "Thu." | JA: "木" ; UILOADFRI0 EN: "Fri." | JA: "金" ; UILOADSAT0 EN: "Sat." | JA: "土"
}
