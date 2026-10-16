#pragma once
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name TASK_NWIN = SHEN_NAME(NWIN);
inline constexpr shen_name NWIN_DUMMY_FILE = SHEN_NAME(NVDY);
inline constexpr shen_name NWIN_TEXTURE_SCRATCH = SHEN_NAME(NTMP);
inline constexpr shen_name NWIN_LABEL_SCRATCH = SHEN_NAME(NVHP);
inline constexpr shen_name NWIN_ROOT_FILE = SHEN_NAME(NAAA);
inline constexpr shen_name NWIN_FOLDER_FILE = SHEN_NAME(NVST);
inline constexpr shen_name NWIN_PREVIEW_TEXTURE = SHEN_NAME(NVTP);
inline constexpr Vector4 NWIN_POSITION_INITIAL{-300.0f, 220.0f, 1.0f, 0.0f};
struct NaviFolderEntry {
    std::int16_t itemId, textureSize;
    Vector3 offset;
    const char* label;
    const char* secondaryLabel;
    std::uint16_t flags;
    std::array<std::byte, 6> reserved22;
};
struct NaviFolderPage {
    shen_name area;
    float scaleX, scaleY;
    std::uint32_t reserved0c;
    const NaviFolderEntry* entries;
    float originX, originY;
    const char* sprite;
    const char* label;
    const char* secondaryLabel;
};
struct NaviWindowState {
    std::int32_t phase, page, requestedPage;
    std::uint32_t drawFlags;
    HLib::Task* task;
    std::int32_t savedNpcLoadState;
    std::uint32_t availablePages, availableEntries;
    std::int32_t categorySelection, entrySelection, firstVisibleEntry, entryCount;
    std::int32_t mapHeight, mapWidth;
    float mapScaleX, mapScaleY;
    std::uint32_t reserved44;
    void* mapImageData;
    void* labelScratch;
    void* dummySpriteData;
};
static_assert(sizeof(NaviFolderEntry) == 0x28);
static_assert(offsetof(NaviFolderEntry, label) == 0x10);
static_assert(offsetof(NaviFolderEntry, secondaryLabel) == 0x18);
static_assert(offsetof(NaviFolderEntry, flags) == 0x20);
static_assert(sizeof(NaviFolderPage) == 0x38);
static_assert(offsetof(NaviFolderPage, entries) == 0x10);
static_assert(offsetof(NaviFolderPage, sprite) == 0x20);
static_assert(offsetof(NaviFolderPage, label) == 0x28);
static_assert(sizeof(NaviWindowState) == 0x60);
static_assert(offsetof(NaviWindowState, task) == 0x10);
static_assert(offsetof(NaviWindowState, availablePages) == 0x1c);
static_assert(offsetof(NaviWindowState, categorySelection) == 0x24);
static_assert(offsetof(NaviWindowState, entryCount) == 0x30);
static_assert(offsetof(NaviWindowState, mapHeight) == 0x34);
static_assert(offsetof(NaviWindowState, mapScaleX) == 0x3c);
static_assert(offsetof(NaviWindowState, mapImageData) == 0x48);
static_assert(offsetof(NaviWindowState, labelScratch) == 0x50);
static_assert(offsetof(NaviWindowState, dummySpriteData) == 0x58);
inline constexpr std::array<NaviFolderEntry, 7> aberdeenFolder{{
    {.itemId=6012, .textureSize=128, .offset={0,-11,0}, .label="UIMAPDWOPI", .flags=1}, // @text EN: "Worker's Pier" | JA: "労湾碼頭"
    {.itemId=6013, .textureSize=128, .offset={44,26,0}, .label="UIMAPDQUST", .flags=1}, // @text EN: "Queen's St." | JA: "クイーンズストリート"
    {.itemId=6015, .textureSize=128, .offset={2,-76,0}, .label="UIMAPDFOPI", .flags=1}, // @text EN: "Fortune's Pier" | JA: "開運碼頭"
    {.itemId=6014, .textureSize=128, .offset={1,65,0}, .label="UIMAPDBHW0", .flags=1}, // @text EN: "Beverly Hills Wharf" | JA: "ビバリーヒルズワーフ"
    {.itemId=6026, .textureSize=256, .label="UIMAPDWONG", .flags=4}, // @text EN: "Wong" | JA: "ウォン"
    {.itemId=6025, .textureSize=256, .label="UIMAPDREN0", .flags=4}, // @text EN: "Ren" | JA: "レン"
    {.itemId=-1000, .textureSize=256, .flags=2}
}};
inline constexpr std::array<NaviFolderEntry, 8> wanChaiFolder{{
    {.itemId=6011, .textureSize=128, .offset={-84,-28,0}, .label="UIMAPDGMQR", .flags=1}, // @text EN: "Green Market Qr." | JA: "菜珍街"
    {.itemId=6008, .textureSize=128, .offset={-57,47,0}, .label="UIMAPDSCQR", .flags=1}, // @text EN: "South Carmain Qr." | JA: "紅南街"
    {.itemId=6009, .textureSize=128, .offset={20,55,0}, .label="UIMAPDWMQR", .flags=1}, // @text EN: "Wise Men's Qr." | JA: "太老街"
    {.itemId=6027, .textureSize=128, .offset={84,36,0}, .label="UIMAPDSCHI", .flags=1}, // @text EN: "Scarlet Hills" | JA: "紅華台"
    {.itemId=6010, .textureSize=128, .offset={48,1,0}, .label="UIMAPDLCQR", .flags=1}, // @text EN: "Lucky Charm Qr." | JA: "福仙街"
    {.itemId=6006, .textureSize=256, .offset={-21,-50,0}, .label="UIMAPDGOQR", .flags=1}, // @text EN: "Golden Qr." | JA: "詠黄街"
    {.itemId=6007, .textureSize=128, .offset={-7,-15,0}, .label="UIMAPDWHQR", .flags=1}, // @text EN: "White Dynasty Qr." | JA: "白秦街"
    {.itemId=6018, .textureSize=256, .label="UIMAPDDYA0", .flags=4} // @text EN: "Da Yuan Apartments" | JA: "大元楼"
}};
inline constexpr std::array<NaviFolderEntry, 11> kowloonFolder{{
    {.itemId=6019, .textureSize=128, .offset={-89,21,0}, .label="UIMAPDDRST", .flags=1}, // @text EN: "Dragon St." | JA: "龍影街"
    {.itemId=6020, .textureSize=256, .offset={-34,27,0}, .label="UIMAPDTWQR", .flags=1}, // @text EN: "Thousand White Qr." | JA: "白千街"
    {.itemId=6022, .textureSize=256, .offset={37,-25,0}, .label="UIMAPDSTQR", .flags=1}, // @text EN: "Stand Qr." | JA: "屋台街"
    {.itemId=6021, .textureSize=128, .offset={17,32,0}, .label="UIMAPDDIQR", .flags=1}, // @text EN: "Dimsum Qr." | JA: "点心街"
    {.itemId=6001, .textureSize=256, .offset={37,-25,0}, .label="UIMAPDABLD", .flags=4}, // @text EN: "A Building" | JA: "九龍城Ａビル"
    {.itemId=6002, .textureSize=256, .offset={37,-25,0}, .label="UIMAPDBBLD", .flags=4}, // @text EN: "B Building" | JA: "九龍城Ｂビル"
    {.itemId=6003, .textureSize=256, .offset={37,-25,0}, .label="UIMAPDCBLD", .flags=4}, // @text EN: "C Building" | JA: "九龍城Ｃビル"
    {.itemId=6004, .textureSize=256, .offset={37,-25,0}, .label="UIMAPDDBLD", .flags=4}, // @text EN: "D Building" | JA: "九龍城Ｄビル"
    {.itemId=6005, .textureSize=256, .offset={37,-25,0}, .label="UIMAPDEBLD", .flags=4}, // @text EN: "E Building" | JA: "九龍城Ｅビル"
    {.itemId=6023, .textureSize=256, .offset={37,-25,0}, .label="UIMAPDFBLD", .flags=4}, // @text EN: "F Building" | JA: "九龍城Ｆビル"
    {.itemId=6017, .textureSize=256, .label="UIMAPDSCOU", .flags=4} // @text EN: "Scout" | JA: "スカウトマン"
}};
inline constexpr std::array<std::int32_t, 3> NWIN_FOLDER_COUNTS{7,8,11};
inline constexpr std::array<NaviFolderPage, 3> naviFolderPages{{
    {.area=SHEN_NAME(A000), .scaleX=2.02f, .scaleY=0.67f, .entries=aberdeenFolder.data(), .originX=223, .originY=400,
        .sprite="A000_G.SPR", .label="UIMAREABER"}, // @text EN: "Aberdeen" | JA: "香港仔"
    {.area=SHEN_NAME(W000), .scaleX=1.5f, .scaleY=0.67f, .entries=wanChaiFolder.data(), .originX=188, .originY=196,
        .sprite="W000_G.SPR", .label="UIMAREWACH"}, // @text EN: "Wan Chai" | JA: "湾仔"
    {.area=SHEN_NAME(Q000), .scaleX=1.25f, .scaleY=0.67f, .entries=kowloonFolder.data(), .originX=240, .originY=140,
        .sprite="Q000_G.SPR", .label="UIMAREKOWL"} // @text EN: "Kowloon" | JA: "九龍城"
}};
} // namespace shenmue::s2
