#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {



struct OwnedItemCatalogEntry {
    std::uint8_t iconIndex;
    const char *modelName;
    // Item-look preview zoom range in thousandths, stepped by CassetteMenu_HandleInput.
    std::int16_t previewZoomMin;
    std::int16_t previewZoomMax;
    std::uint32_t reserved14;
    const char *cassetteNameId;
    const char *itemNameId;
};
static_assert(sizeof(OwnedItemCatalogEntry) == 0x28);
static_assert(offsetof(OwnedItemCatalogEntry, modelName) == 0x08);
static_assert(offsetof(OwnedItemCatalogEntry, previewZoomMin) == 0x10);
static_assert(offsetof(OwnedItemCatalogEntry, previewZoomMax) == 0x12);
static_assert(offsetof(OwnedItemCatalogEntry, cassetteNameId) == 0x18);
static_assert(offsetof(OwnedItemCatalogEntry, itemNameId) == 0x20);


struct CassetteCatalogEntry {
    OwnedItemCatalogEntry entry;
    const char *streamName;
    std::uint32_t soundCommand;
    std::uint32_t reserved34;
};
static_assert(sizeof(CassetteCatalogEntry) == 0x38);
static_assert(offsetof(CassetteCatalogEntry, streamName) == 0x28);
static_assert(offsetof(CassetteCatalogEntry, soundCommand) == 0x30);


struct InventoryIconAtlasRect {
    std::uint8_t sheet;
    std::uint8_t left;
    std::uint8_t bottom;
    std::uint8_t right;
    std::uint8_t top;
};
static_assert(sizeof(InventoryIconAtlasRect) == 5);

inline constexpr std::uint16_t ITEM_SCROLL_STAB_ARMOR = 21;
constexpr std::uint16_t kOwnedItemCatalogCount = 235;
constexpr std::uint16_t kCassetteCatalogFirstItem = 255;
constexpr std::uint16_t kCassetteCatalogCount = 28;
// Mask sheets follow the twelve colour sheets in the icon-set name table.
constexpr std::int32_t kInventoryIconMaskSheetOffset = 12;


extern std::array<OwnedItemCatalogEntry, kOwnedItemCatalogCount> g_ownedItemCatalog;
// Shenmue::OriginalS1IconFilenames: SharedItems.ui image names indexed by atlas sheet.
extern const std::array<const char *, 24> g_inventoryIconSheetNames;

inline const std::array<CassetteCatalogEntry, kCassetteCatalogCount> g_cassetteCatalog{{
    {.entry = {.iconIndex = 0xc4, .modelName = "MT2K6GO", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSGOGO"}, // @text EN: "GoGo" | JA: "GoGo"
        .streamName = "bgm045",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xc9, .modelName = "MT2K6FE", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSFTSO"}, // @text EN: "Feel Tired Song" | JA: "Feel Tired Song"
        .streamName = "bgm021",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xc7, .modelName = "MT2K6HI", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSHIPD"}, // @text EN: "Hip de Hop" | JA: "Hip de Hop"
        .streamName = "ykm002",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xcc, .modelName = "MT2K6LI", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSLFEE"}, // @text EN: "Like A Feeling" | JA: "Like A Feeling"
        .streamName = "tsm002",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xc5, .modelName = "MT2K6HA", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSHEAR"}, // @text EN: "Heart Beats" | JA: "Heart Beats"
        .streamName = "ykm003",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xca, .modelName = "MT2K6FL", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSFLOW"}, // @text EN: "Flower Girl" | JA: "Flower Girl"
        .streamName = "bgm016",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xc3, .modelName = "MT2K6AN", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSATRE"}, // @text EN: "Antiquity Tree" | JA: "Antiquity Tree"
        .streamName = "bgm015",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xcb, .modelName = "MT2K6DA", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSDOLD"}, // @text EN: "Dandy Old Man" | JA: "Dandy Old Man"
        .streamName = "bgm014",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xc6, .modelName = "MT2K6JA", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSLIQU"}, // @text EN: "Liquor" | JA: "Liquor"
        .streamName = "bgm047",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xc8, .modelName = "MT2K6LN", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSLIND"}, // @text EN: "Linda" | JA: "Linda"
        .streamName = "bgm048",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xd0, .modelName = "MT20GOR", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSMSSH"}, // @text EN: "Magical Sound Shower" | JA: "Magical Sound Shower"
        .streamName = "bgm061",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xd2, .modelName = "MT20GHO", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSHANG"}, // @text EN: "HANG ON" | JA: "HANG-ON"
        .streamName = "bgm058",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xd1, .modelName = "MT20GSH", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSSHAR"}, // @text EN: "Space Harrier" | JA: "Space Harrier"
        .streamName = "bgm057",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xd3, .modelName = "MT20GAB", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSFTOF"}, // @text EN: "Final Take Off" | JA: "Final Take Off"
        .streamName = "bgm056",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xce, .modelName = "MT20GDY", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSDEST"}, // @text EN: "Destiny" | JA: "Destiny"
        .streamName = "bgm074",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xc1, .modelName = "MT20GBZ", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSBOZN"}, // @text EN: "Boz Nov" | JA: "Boz Nov"
        .streamName = "bgm075",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xcd, .modelName = "MT20GBW", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSBEWI"}, // @text EN: "Be-witch" | JA: "Be-witch"
        .streamName = "bgm072",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xd4, .modelName = "MT2K6MJ", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSMJQ0"}, // @text EN: "MJQ" | JA: "MJQ"
        .streamName = "bgm010",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xdb, .modelName = "MT2K6HL", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSHAR1"}, // @text EN: "Harbor Bar" | JA: "Harbor Bar"
        .streamName = "bgm053",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xcf, .modelName = "MT2K6NA", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSNANA"}, // @text EN: "NaNa" | JA: "NaNa"
        .streamName = "bgm054",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xd9, .modelName = "MT2K6SP", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSSPID"}, // @text EN: "Spider" | JA: "Spider"
        .streamName = "bgm060",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xda, .modelName = "MT2K6GY", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSGLYF"}, // @text EN: "Glyfada" | JA: "Glyfada"
        .streamName = "bgm092",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xd7, .modelName = "MT2K6YA", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSYADA"}, // @text EN: "Y.A.D.A" | JA: "Y.A.D.A"
        .streamName = "bgm089",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xd5, .modelName = "MT2K6YB", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSYBLU"}, // @text EN: "Yokosuka Blues" | JA: "Yokosuka Blues"
        .streamName = "bgm090",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xd6, .modelName = "MT2K6ST", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSSTRO"}, // @text EN: "Strong" | JA: "Strong"
        .streamName = "bgm125",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xd8, .modelName = "MT2K6HB", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSHAR2"}, // @text EN: "Harbor Beats" | JA: "Harbor Beats"
        .streamName = "bgm124",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xc0, .modelName = "MT2K6MT", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSSHEN"}, // @text EN: "Shenmue" | JA: "Shenmue"
        .streamName = "tsm004",
        .soundCommand = 0x3fa8},
    {.entry = {.iconIndex = 0xc2, .modelName = "MT2K6SM", .previewZoomMin = 700, .previewZoomMax = 1550, .cassetteNameId = "UICASSSHAH"}, // @text EN: "Shenhua" | JA: "Shenhua"
        .streamName = "bgm136",
        .soundCommand = 0x3fa8},
}};

inline const std::array<InventoryIconAtlasRect, 254> g_inventoryIconAtlasRects{{
    {.sheet = 3, .left = 0, .bottom = 2, .right = 2, .top = 4},
    {.sheet = 2, .left = 0, .bottom = 7, .right = 1, .top = 8},
    {.sheet = 2, .left = 1, .bottom = 7, .right = 2, .top = 8},
    {.sheet = 2, .left = 2, .bottom = 7, .right = 3, .top = 8},
    {.sheet = 2, .left = 3, .bottom = 7, .right = 4, .top = 8},
    {.sheet = 2, .left = 0, .bottom = 6, .right = 1, .top = 7},
    {.sheet = 2, .left = 1, .bottom = 6, .right = 2, .top = 7},
    {.sheet = 2, .left = 3, .bottom = 6, .right = 4, .top = 7},
    {.sheet = 2, .left = 0, .bottom = 5, .right = 1, .top = 6},
    {.sheet = 2, .left = 1, .bottom = 5, .right = 2, .top = 6},
    {.sheet = 2, .left = 2, .bottom = 5, .right = 3, .top = 6},
    {.sheet = 11, .left = 4, .bottom = 2, .right = 5, .top = 3},
    {.sheet = 2, .left = 0, .bottom = 4, .right = 1, .top = 5},
    {.sheet = 2, .left = 1, .bottom = 4, .right = 2, .top = 5},
    {.sheet = 2, .left = 2, .bottom = 4, .right = 3, .top = 5},
    {.sheet = 7, .left = 2, .bottom = 4, .right = 4, .top = 6},
    {.sheet = 11, .left = 7, .bottom = 1, .right = 8, .top = 2},
    {.sheet = 11, .left = 6, .bottom = 1, .right = 7, .top = 2},
    {.sheet = 11, .left = 5, .bottom = 1, .right = 6, .top = 2},
    {.sheet = 11, .left = 7, .bottom = 3, .right = 8, .top = 4},
    {.sheet = 2, .left = 1, .bottom = 3, .right = 2, .top = 4},
    {.sheet = 2, .left = 2, .bottom = 3, .right = 3, .top = 4},
    {.sheet = 2, .left = 3, .bottom = 3, .right = 4, .top = 4},
    {.sheet = 2, .left = 0, .bottom = 2, .right = 1, .top = 3},
    {.sheet = 2, .left = 1, .bottom = 2, .right = 2, .top = 3},
    {.sheet = 2, .left = 2, .bottom = 2, .right = 3, .top = 3},
    {.sheet = 2, .left = 0, .bottom = 1, .right = 1, .top = 2},
    {.sheet = 11, .left = 5, .bottom = 2, .right = 6, .top = 3},
    {.sheet = 11, .left = 3, .bottom = 6, .right = 4, .top = 7},
    {.sheet = 2, .left = 3, .bottom = 2, .right = 4, .top = 3},
    {.sheet = 11, .left = 6, .bottom = 2, .right = 7, .top = 3},
    {.sheet = 11, .left = 7, .bottom = 2, .right = 8, .top = 3},
    {.sheet = 11, .left = 3, .bottom = 0, .right = 4, .top = 1},
    {.sheet = 2, .left = 1, .bottom = 1, .right = 2, .top = 2},
    {.sheet = 2, .left = 0, .bottom = 3, .right = 1, .top = 4},
    {.sheet = 11, .left = 4, .bottom = 1, .right = 5, .top = 2},
    {.sheet = 2, .left = 2, .bottom = 1, .right = 3, .top = 2},
    {.sheet = 2, .left = 3, .bottom = 1, .right = 4, .top = 2},
    {.sheet = 2, .left = 0, .bottom = 0, .right = 1, .top = 1},
    {.sheet = 2, .left = 1, .bottom = 0, .right = 2, .top = 1},
    {.sheet = 2, .left = 2, .bottom = 0, .right = 3, .top = 1},
    {.sheet = 2, .left = 3, .bottom = 0, .right = 4, .top = 1},
    {.sheet = 2, .left = 4, .bottom = 3, .right = 5, .top = 4},
    {.sheet = 2, .left = 5, .bottom = 7, .right = 6, .top = 8},
    {.sheet = 2, .left = 6, .bottom = 7, .right = 7, .top = 8},
    {.sheet = 2, .left = 7, .bottom = 7, .right = 8, .top = 8},
    {.sheet = 2, .left = 6, .bottom = 4, .right = 7, .top = 5},
    {.sheet = 9, .left = 0, .bottom = 7, .right = 1, .top = 8},
    {.sheet = 9, .left = 2, .bottom = 7, .right = 3, .top = 8},
    {.sheet = 9, .left = 3, .bottom = 7, .right = 4, .top = 8},
    {.sheet = 9, .left = 1, .bottom = 3, .right = 2, .top = 4},
    {.sheet = 9, .left = 0, .bottom = 6, .right = 1, .top = 7},
    {.sheet = 9, .left = 2, .bottom = 6, .right = 3, .top = 7},
    {.sheet = 9, .left = 3, .bottom = 6, .right = 4, .top = 7},
    {.sheet = 9, .left = 1, .bottom = 2, .right = 2, .top = 3},
    {.sheet = 9, .left = 4, .bottom = 7, .right = 5, .top = 8},
    {.sheet = 9, .left = 5, .bottom = 7, .right = 6, .top = 8},
    {.sheet = 9, .left = 6, .bottom = 7, .right = 7, .top = 8},
    {.sheet = 9, .left = 7, .bottom = 7, .right = 8, .top = 8},
    {.sheet = 9, .left = 0, .bottom = 4, .right = 1, .top = 5},
    {.sheet = 9, .left = 1, .bottom = 4, .right = 2, .top = 5},
    {.sheet = 9, .left = 2, .bottom = 4, .right = 3, .top = 5},
    {.sheet = 9, .left = 3, .bottom = 4, .right = 4, .top = 5},
    {.sheet = 9, .left = 0, .bottom = 5, .right = 1, .top = 6},
    {.sheet = 9, .left = 1, .bottom = 5, .right = 2, .top = 6},
    {.sheet = 9, .left = 2, .bottom = 5, .right = 3, .top = 6},
    {.sheet = 9, .left = 3, .bottom = 5, .right = 4, .top = 6},
    {.sheet = 9, .left = 4, .bottom = 6, .right = 5, .top = 7},
    {.sheet = 9, .left = 5, .bottom = 6, .right = 6, .top = 7},
    {.sheet = 9, .left = 6, .bottom = 6, .right = 7, .top = 7},
    {.sheet = 9, .left = 3, .bottom = 1, .right = 4, .top = 2},
    {.sheet = 9, .left = 4, .bottom = 5, .right = 5, .top = 6},
    {.sheet = 9, .left = 5, .bottom = 5, .right = 6, .top = 6},
    {.sheet = 9, .left = 6, .bottom = 5, .right = 7, .top = 6},
    {.sheet = 9, .left = 1, .bottom = 1, .right = 2, .top = 2},
    {.sheet = 9, .left = 4, .bottom = 4, .right = 5, .top = 5},
    {.sheet = 9, .left = 5, .bottom = 4, .right = 6, .top = 5},
    {.sheet = 9, .left = 6, .bottom = 4, .right = 7, .top = 5},
    {.sheet = 9, .left = 3, .bottom = 3, .right = 4, .top = 4},
    {.sheet = 9, .left = 7, .bottom = 4, .right = 8, .top = 5},
    {.sheet = 9, .left = 3, .bottom = 2, .right = 4, .top = 3},
    {.sheet = 9, .left = 7, .bottom = 6, .right = 8, .top = 7},
    {.sheet = 9, .left = 7, .bottom = 5, .right = 8, .top = 6},
    {.sheet = 10, .left = 1, .bottom = 3, .right = 2, .top = 4},
    {.sheet = 10, .left = 0, .bottom = 3, .right = 1, .top = 4},
    {.sheet = 9, .left = 1, .bottom = 0, .right = 2, .top = 1},
    {.sheet = 10, .left = 1, .bottom = 7, .right = 2, .top = 8},
    {.sheet = 11, .left = 0, .bottom = 0, .right = 1, .top = 1},
    {.sheet = 10, .left = 3, .bottom = 7, .right = 4, .top = 8},
    {.sheet = 10, .left = 0, .bottom = 6, .right = 1, .top = 7},
    {.sheet = 10, .left = 1, .bottom = 6, .right = 2, .top = 7},
    {.sheet = 10, .left = 3, .bottom = 5, .right = 4, .top = 6},
    {.sheet = 10, .left = 2, .bottom = 5, .right = 3, .top = 6},
    {.sheet = 10, .left = 0, .bottom = 5, .right = 1, .top = 6},
    {.sheet = 10, .left = 2, .bottom = 6, .right = 3, .top = 7},
    {.sheet = 10, .left = 1, .bottom = 5, .right = 2, .top = 6},
    {.sheet = 9, .left = 7, .bottom = 1, .right = 8, .top = 2},
    {.sheet = 10, .left = 2, .bottom = 7, .right = 3, .top = 8},
    {.sheet = 10, .left = 3, .bottom = 6, .right = 4, .top = 7},
    {.sheet = 10, .left = 3, .bottom = 4, .right = 4, .top = 5},
    {.sheet = 9, .left = 5, .bottom = 1, .right = 6, .top = 2},
    {.sheet = 9, .left = 4, .bottom = 1, .right = 5, .top = 2},
    {.sheet = 10, .left = 5, .bottom = 4, .right = 6, .top = 5},
    {.sheet = 10, .left = 6, .bottom = 4, .right = 7, .top = 5},
    {.sheet = 9, .left = 6, .bottom = 1, .right = 7, .top = 2},
    {.sheet = 10, .left = 2, .bottom = 4, .right = 3, .top = 5},
    {.sheet = 9, .left = 5, .bottom = 3, .right = 6, .top = 4},
    {.sheet = 9, .left = 4, .bottom = 3, .right = 5, .top = 4},
    {.sheet = 9, .left = 4, .bottom = 0, .right = 5, .top = 1},
    {.sheet = 9, .left = 6, .bottom = 3, .right = 7, .top = 4},
    {.sheet = 9, .left = 7, .bottom = 3, .right = 8, .top = 4},
    {.sheet = 9, .left = 4, .bottom = 2, .right = 5, .top = 3},
    {.sheet = 9, .left = 2, .bottom = 0, .right = 3, .top = 1},
    {.sheet = 9, .left = 3, .bottom = 0, .right = 4, .top = 1},
    {.sheet = 9, .left = 6, .bottom = 0, .right = 7, .top = 1},
    {.sheet = 9, .left = 7, .bottom = 0, .right = 8, .top = 1},
    {.sheet = 9, .left = 5, .bottom = 0, .right = 6, .top = 1},
    {.sheet = 11, .left = 7, .bottom = 7, .right = 8, .top = 8},
    {.sheet = 10, .left = 4, .bottom = 6, .right = 5, .top = 7},
    {.sheet = 9, .left = 6, .bottom = 2, .right = 7, .top = 3},
    {.sheet = 10, .left = 0, .bottom = 2, .right = 1, .top = 3},
    {.sheet = 10, .left = 0, .bottom = 4, .right = 1, .top = 5},
    {.sheet = 10, .left = 6, .bottom = 7, .right = 7, .top = 8},
    {.sheet = 10, .left = 7, .bottom = 7, .right = 8, .top = 8},
    {.sheet = 10, .left = 1, .bottom = 4, .right = 2, .top = 5},
    {.sheet = 10, .left = 4, .bottom = 7, .right = 5, .top = 8},
    {.sheet = 10, .left = 5, .bottom = 7, .right = 6, .top = 8},
    {.sheet = 10, .left = 5, .bottom = 6, .right = 6, .top = 7},
    {.sheet = 10, .left = 4, .bottom = 4, .right = 5, .top = 5},
    {.sheet = 11, .left = 2, .bottom = 2, .right = 3, .top = 3},
    {.sheet = 11, .left = 3, .bottom = 2, .right = 4, .top = 3},
    {.sheet = 11, .left = 0, .bottom = 1, .right = 1, .top = 2},
    {.sheet = 11, .left = 1, .bottom = 1, .right = 2, .top = 2},
    {.sheet = 11, .left = 2, .bottom = 1, .right = 3, .top = 2},
    {.sheet = 11, .left = 3, .bottom = 1, .right = 4, .top = 2},
    {.sheet = 10, .left = 6, .bottom = 6, .right = 7, .top = 7},
    {.sheet = 10, .left = 7, .bottom = 6, .right = 8, .top = 7},
    {.sheet = 10, .left = 4, .bottom = 5, .right = 5, .top = 6},
    {.sheet = 9, .left = 4, .bottom = 0, .right = 5, .top = 1},
    {.sheet = 9, .left = 7, .bottom = 2, .right = 8, .top = 3},
    {.sheet = 10, .left = 2, .bottom = 3, .right = 3, .top = 4},
    {.sheet = 10, .left = 5, .bottom = 5, .right = 6, .top = 6},
    {.sheet = 10, .left = 6, .bottom = 5, .right = 7, .top = 6},
    {.sheet = 10, .left = 7, .bottom = 5, .right = 8, .top = 6},
    {.sheet = 10, .left = 1, .bottom = 2, .right = 2, .top = 3},
    {.sheet = 10, .left = 3, .bottom = 2, .right = 4, .top = 3},
    {.sheet = 10, .left = 0, .bottom = 1, .right = 1, .top = 2},
    {.sheet = 11, .left = 5, .bottom = 5, .right = 6, .top = 6},
    {.sheet = 11, .left = 6, .bottom = 5, .right = 7, .top = 6},
    {.sheet = 11, .left = 4, .bottom = 6, .right = 5, .top = 7},
    {.sheet = 11, .left = 5, .bottom = 6, .right = 6, .top = 7},
    {.sheet = 11, .left = 6, .bottom = 6, .right = 7, .top = 7},
    {.sheet = 11, .left = 7, .bottom = 6, .right = 8, .top = 7},
    {.sheet = 11, .left = 4, .bottom = 5, .right = 5, .top = 6},
    {.sheet = 10, .left = 3, .bottom = 1, .right = 4, .top = 2},
    {.sheet = 9, .left = 0, .bottom = 1, .right = 1, .top = 2},
    {.sheet = 10, .left = 0, .bottom = 0, .right = 1, .top = 1},
    {.sheet = 10, .left = 4, .bottom = 0, .right = 5, .top = 1},
    {.sheet = 10, .left = 5, .bottom = 0, .right = 6, .top = 1},
    {.sheet = 10, .left = 1, .bottom = 0, .right = 2, .top = 1},
    {.sheet = 11, .left = 3, .bottom = 3, .right = 4, .top = 4},
    {.sheet = 10, .left = 3, .bottom = 0, .right = 4, .top = 1},
    {.sheet = 9, .left = 1, .bottom = 7, .right = 2, .top = 8},
    {.sheet = 10, .left = 4, .bottom = 3, .right = 5, .top = 4},
    {.sheet = 10, .left = 5, .bottom = 3, .right = 6, .top = 4},
    {.sheet = 10, .left = 6, .bottom = 3, .right = 7, .top = 4},
    {.sheet = 10, .left = 7, .bottom = 3, .right = 8, .top = 4},
    {.sheet = 10, .left = 5, .bottom = 2, .right = 6, .top = 3},
    {.sheet = 10, .left = 4, .bottom = 2, .right = 5, .top = 3},
    {.sheet = 10, .left = 6, .bottom = 2, .right = 7, .top = 3},
    {.sheet = 10, .left = 7, .bottom = 1, .right = 8, .top = 2},
    {.sheet = 10, .left = 4, .bottom = 1, .right = 5, .top = 2},
    {.sheet = 10, .left = 5, .bottom = 1, .right = 6, .top = 2},
    {.sheet = 10, .left = 6, .bottom = 1, .right = 7, .top = 2},
    {.sheet = 10, .left = 6, .bottom = 0, .right = 7, .top = 1},
    {.sheet = 10, .left = 7, .bottom = 0, .right = 8, .top = 1},
    {.sheet = 11, .left = 0, .bottom = 7, .right = 1, .top = 8},
    {.sheet = 11, .left = 1, .bottom = 7, .right = 2, .top = 8},
    {.sheet = 11, .left = 2, .bottom = 7, .right = 3, .top = 8},
    {.sheet = 11, .left = 3, .bottom = 7, .right = 4, .top = 8},
    {.sheet = 11, .left = 0, .bottom = 6, .right = 1, .top = 7},
    {.sheet = 11, .left = 1, .bottom = 6, .right = 2, .top = 7},
    {.sheet = 11, .left = 3, .bottom = 5, .right = 4, .top = 6},
    {.sheet = 11, .left = 2, .bottom = 5, .right = 3, .top = 6},
    {.sheet = 10, .left = 7, .bottom = 4, .right = 8, .top = 5},
    {.sheet = 11, .left = 1, .bottom = 2, .right = 2, .top = 3},
    {.sheet = 11, .left = 0, .bottom = 2, .right = 1, .top = 3},
    {.sheet = 11, .left = 0, .bottom = 4, .right = 1, .top = 5},
    {.sheet = 11, .left = 1, .bottom = 4, .right = 2, .top = 5},
    {.sheet = 11, .left = 2, .bottom = 4, .right = 3, .top = 5},
    {.sheet = 11, .left = 3, .bottom = 4, .right = 4, .top = 5},
    {.sheet = 9, .left = 1, .bottom = 6, .right = 2, .top = 7},
    {.sheet = 2, .left = 2, .bottom = 6, .right = 3, .top = 7},
    {.sheet = 2, .left = 3, .bottom = 5, .right = 4, .top = 6},
    {.sheet = 2, .left = 4, .bottom = 7, .right = 5, .top = 8},
    {.sheet = 2, .left = 4, .bottom = 6, .right = 5, .top = 7},
    {.sheet = 2, .left = 5, .bottom = 6, .right = 6, .top = 7},
    {.sheet = 2, .left = 6, .bottom = 6, .right = 7, .top = 7},
    {.sheet = 2, .left = 7, .bottom = 6, .right = 8, .top = 7},
    {.sheet = 2, .left = 4, .bottom = 5, .right = 5, .top = 6},
    {.sheet = 2, .left = 5, .bottom = 5, .right = 6, .top = 6},
    {.sheet = 2, .left = 6, .bottom = 5, .right = 7, .top = 6},
    {.sheet = 2, .left = 7, .bottom = 5, .right = 8, .top = 6},
    {.sheet = 2, .left = 4, .bottom = 4, .right = 5, .top = 5},
    {.sheet = 2, .left = 5, .bottom = 4, .right = 6, .top = 5},
    {.sheet = 2, .left = 5, .bottom = 3, .right = 6, .top = 4},
    {.sheet = 2, .left = 6, .bottom = 3, .right = 7, .top = 4},
    {.sheet = 2, .left = 7, .bottom = 3, .right = 8, .top = 4},
    {.sheet = 2, .left = 4, .bottom = 2, .right = 5, .top = 3},
    {.sheet = 2, .left = 5, .bottom = 2, .right = 6, .top = 3},
    {.sheet = 2, .left = 6, .bottom = 2, .right = 7, .top = 3},
    {.sheet = 2, .left = 7, .bottom = 2, .right = 8, .top = 3},
    {.sheet = 2, .left = 4, .bottom = 1, .right = 5, .top = 2},
    {.sheet = 2, .left = 5, .bottom = 1, .right = 6, .top = 2},
    {.sheet = 2, .left = 6, .bottom = 1, .right = 7, .top = 2},
    {.sheet = 2, .left = 7, .bottom = 1, .right = 8, .top = 2},
    {.sheet = 2, .left = 4, .bottom = 0, .right = 5, .top = 1},
    {.sheet = 2, .left = 5, .bottom = 0, .right = 6, .top = 1},
    {.sheet = 2, .left = 6, .bottom = 0, .right = 7, .top = 1},
    {.sheet = 2, .left = 7, .bottom = 0, .right = 8, .top = 1},
    {.sheet = 2, .left = 7, .bottom = 4, .right = 8, .top = 5},
    {.sheet = 11, .left = 0, .bottom = 5, .right = 1, .top = 6},
    {.sheet = 11, .left = 2, .bottom = 6, .right = 3, .top = 7},
    {.sheet = 11, .left = 1, .bottom = 5, .right = 2, .top = 6},
    {.sheet = 11, .left = 0, .bottom = 3, .right = 1, .top = 4},
    {.sheet = 11, .left = 1, .bottom = 3, .right = 2, .top = 4},
    {.sheet = 11, .left = 2, .bottom = 3, .right = 3, .top = 4},
    {.sheet = 11, .left = 7, .bottom = 5, .right = 8, .top = 6},
    {.sheet = 11, .left = 4, .bottom = 4, .right = 5, .top = 5},
    {.sheet = 11, .left = 5, .bottom = 4, .right = 6, .top = 5},
    {.sheet = 11, .left = 6, .bottom = 4, .right = 7, .top = 5},
    {.sheet = 11, .left = 7, .bottom = 4, .right = 8, .top = 5},
    {.sheet = 11, .left = 4, .bottom = 3, .right = 5, .top = 4},
    {.sheet = 11, .left = 5, .bottom = 3, .right = 6, .top = 4},
    {.sheet = 11, .left = 6, .bottom = 3, .right = 7, .top = 4},
    {.sheet = 11, .left = 4, .bottom = 0, .right = 5, .top = 1},
    {.sheet = 11, .left = 5, .bottom = 0, .right = 6, .top = 1},
    {.sheet = 11, .left = 6, .bottom = 0, .right = 7, .top = 1},
    {.sheet = 11, .left = 7, .bottom = 0, .right = 8, .top = 1},
    {.sheet = 9, .left = 2, .bottom = 1, .right = 3, .top = 2},
    {.sheet = 9, .left = 2, .bottom = 2, .right = 3, .top = 3},
    {.sheet = 9, .left = 0, .bottom = 0, .right = 1, .top = 1},
    {.sheet = 9, .left = 2, .bottom = 3, .right = 3, .top = 4},
    {.sheet = 9, .left = 0, .bottom = 3, .right = 1, .top = 4},
    {.sheet = 9, .left = 0, .bottom = 2, .right = 1, .top = 3},
    {.sheet = 7, .left = 0, .bottom = 6, .right = 2, .top = 8},
    {.sheet = 7, .left = 2, .bottom = 6, .right = 4, .top = 8},
    {.sheet = 7, .left = 4, .bottom = 6, .right = 6, .top = 8},
    {.sheet = 7, .left = 0, .bottom = 4, .right = 2, .top = 6},
    {.sheet = 7, .left = 6, .bottom = 6, .right = 8, .top = 8},
    {.sheet = 7, .left = 4, .bottom = 4, .right = 6, .top = 6},
    {.sheet = 7, .left = 6, .bottom = 4, .right = 8, .top = 6},
    {.sheet = 4, .left = 0, .bottom = 0, .right = 2, .top = 2},
    {.sheet = 4, .left = 0, .bottom = 4, .right = 2, .top = 6},
}};

} // namespace shenmue::s1
