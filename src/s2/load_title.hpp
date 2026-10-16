#pragma once

#include "common/types.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_sprite.hpp"
#include <array>
#include <cstddef>

#include <cstdint>

namespace shenmue::s2 {
struct MapInfoChunk;

inline constexpr shen_name SUBTITLE_WAIT_TASK{SHEN_NAME(SBTS)};
inline constexpr shen_name SUBTITLE_SPRITE_FILE{SHEN_NAME(LDGJ)};


struct SubtitleSpriteWork {
    HLib::Task *task;
    LegacySpritePacket sprite;
    float x;
    float y;
    std::array<std::int32_t, 4> textures;
    float rotation;
    std::int16_t alpha;
    std::int16_t alphaStep;
    std::int16_t stacked;
};
static_assert(sizeof(SubtitleSpriteWork) == 0x88);
static_assert(offsetof(SubtitleSpriteWork, sprite) == 0x08);
static_assert(offsetof(SubtitleSpriteWork, x) == 0x60);
static_assert(offsetof(SubtitleSpriteWork, textures) == 0x68);
static_assert(offsetof(SubtitleSpriteWork, rotation) == 0x78);
static_assert(offsetof(SubtitleSpriteWork, alpha) == 0x7c);
static_assert(offsetof(SubtitleSpriteWork, alphaStep) == 0x7e);
static_assert(offsetof(SubtitleSpriteWork, stacked) == 0x80);

// Rows of the four-texture SUBTITLE.SPR stack: centre Y and height of each 512-wide strip.
inline constexpr std::array<std::int16_t, 4> SUBTITLE_STACK_Y{240, 209, 310, 240};
inline constexpr std::array<std::int16_t, 4> SUBTITLE_STACK_HEIGHT{512, 128, 64, 512};
// 0x10000 / 3600: the per-frame rotation of the first stacked strip.
inline constexpr float SUBTITLE_ROTATION_STEP = 18.204445f;

struct LoadTitleEntry {
    const char *key;
    const char *alias;
};

struct MapRegionEntry {
    const char *area;
    const char *map;
    std::array<float, 4> bounds;
};

struct FloorTitleEntry {
    const char *area;
    std::int32_t entry;
    const char *sprite;
};

struct LoadTitleState {
    HLib::Task *titleTask;                      // g_pST_TitleFunc
    std::int16_t alpha;                         // g_pST_TaskFlags
    std::int32_t fadeInActive;                  // g_pST_TaskFlags_1
    std::uint32_t flags;                        // g_pST_TaskFlags_0
    std::uint8_t fadeMode;                      // g_ST_Fade_Flag
    const MapInfoChunk *loadScene;              // g_pLSCN
    std::int32_t stream;                        // g_stLoadStream
    std::uint32_t author;                       // g_stLoadAuthor
    std::int32_t titleTexture;                  // g_stTitleTexture
    std::int32_t floorTexture;                  // g_stFloorTexture
    std::uint16_t titleWidth;                   // g_tmp_width
    std::uint16_t floorWidth;                   // g_stFloorWidth
    std::int32_t labelTexture;                  // g_stLabelTexture
    std::array<std::int32_t, 36> digitTextures; // g_stDigitTextures
    std::int32_t digitCount;                    // g_stDigitCount
    std::array<std::int8_t, 12> date;           // Date_YearTens .. Date_WeekUnit
    std::int8_t meridiemSprite;                 // g_stMeridiemSprite
    std::int8_t loadingDrawMode{};
    std::uint8_t loadingDrawActive{};
    std::uint8_t loadingDotFrame{};
    std::uint16_t loadingSpin{};
};

enum DateDigit : std::size_t { YearTens, YearUnits, MonthTens, MonthUnits, DayTens, DayUnits, HourTens, HourUnits, MinuteTens, MinuteUnits, Week };




void InitST_TitleFunc();
void ReleaseLoadTitleFade();
void DoLoadStuff(std::uint32_t author, bool nextStream);
shen_name GetMapRegionName(const Vector3 &position);
void SetTemporaryAreaJumpAuthor(shen_name author);
void ResetTemporaryAreaJumpAuthor();
void ST_LoadTitleFadeIn();
void ST_LoadTitleFade();
void Queue_ST_LoadTitleFadeIn(shen_name author);
std::uint64_t scenSetTaskFlags(std::uint8_t mode);
std::int16_t GetLoadTitleAlpha();
void SetLoadingDrawFlags(std::int8_t mode);
void DrawLoadingModes();
void RetireLoadingDraw();
void ResetLoadingDrawState();
bool IsLoadingDrawActive();



HLib::Task *Queue_ST_WaitSubTitle(const char *filename);
std::int64_t ST_FadeInSubTitle(HLib::Task *task, float x, float y, std::int16_t alphaStep);
std::int64_t ST_FadeOutSubTitle(HLib::Task *task);

} // namespace shenmue::s2
