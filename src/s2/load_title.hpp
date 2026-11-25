#pragma once

#include "common/types.hpp"
#include "s2/hlib.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/legacy_sprite_types.hpp"
#else
#include "s2/legacy_sprite.hpp"
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SHENMUE_TITLE_CONSTANT const
#define SHENMUE_LOAD_TITLE_DATA static const
#else
#include <array>
#include <cstddef>

#include <cstdint>
#define SHENMUE_TITLE_CONSTANT inline constexpr
#define SHENMUE_LOAD_TITLE_DATA inline constexpr
#endif

namespace shenmue {
namespace s2 {
struct MapInfoChunk;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
inline shen_name SubtitleWaitTaskName() {
    return shen_name(SHEN_NAME4('S', 'B', 'T', 'S'));
}
inline shen_name SubtitleSpriteFileName() {
    return shen_name(SHEN_NAME4('L', 'D', 'G', 'J'));
}
#define SUBTITLE_WAIT_TASK SubtitleWaitTaskName()
#define SUBTITLE_SPRITE_FILE SubtitleSpriteFileName()
#else
inline constexpr shen_name SUBTITLE_WAIT_TASK{SHEN_NAME(SBTS)};
inline constexpr shen_name SUBTITLE_SPRITE_FILE{SHEN_NAME(LDGJ)};
#endif

// SBTS storage bound to the task by AllocateTASK.
struct SubtitleSpriteWork {
    HLib::Task *task;
    LegacySpritePacket sprite;
    float x;
    float y;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t textures[4];
#else
    std::array<std::int32_t, 4> textures;
#endif
    float rotation;
    std::int16_t alpha;
    std::int16_t alphaStep;
    std::int16_t stacked;
};
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char SubtitleSpriteWorkLayoutCheck[(sizeof(SubtitleSpriteWork) == 0x80 && offsetof(SubtitleSpriteWork, sprite) == 4 &&
                                               offsetof(SubtitleSpriteWork, x) == 0x5c && offsetof(SubtitleSpriteWork, textures) == 0x64 &&
                                               offsetof(SubtitleSpriteWork, rotation) == 0x74 && offsetof(SubtitleSpriteWork, alpha) == 0x78 &&
                                               offsetof(SubtitleSpriteWork, alphaStep) == 0x7a && offsetof(SubtitleSpriteWork, stacked) == 0x7c) ?
        1 :
        -1];
#else
typedef char Layout_load_title_0[(sizeof(SubtitleSpriteWork) == 0x80) ? 1 : -1];
typedef char Layout_load_title_1[(offsetof(SubtitleSpriteWork, sprite) == 0x04) ? 1 : -1];
typedef char Layout_load_title_2[(offsetof(SubtitleSpriteWork, x) == 0x5c) ? 1 : -1];
typedef char Layout_load_title_3[(offsetof(SubtitleSpriteWork, textures) == 0x64) ? 1 : -1];
typedef char Layout_load_title_4[(offsetof(SubtitleSpriteWork, rotation) == 0x74) ? 1 : -1];
typedef char Layout_load_title_5[(offsetof(SubtitleSpriteWork, alpha) == 0x78) ? 1 : -1];
typedef char Layout_load_title_6[(offsetof(SubtitleSpriteWork, alphaStep) == 0x7a) ? 1 : -1];
typedef char Layout_load_title_7[(offsetof(SubtitleSpriteWork, stacked) == 0x7c) ? 1 : -1];
#endif
#else
typedef char Layout_load_title_8[(sizeof(SubtitleSpriteWork) == 0x88) ? 1 : -1];
typedef char Layout_load_title_9[(offsetof(SubtitleSpriteWork, sprite) == 0x08) ? 1 : -1];
typedef char Layout_load_title_10[(offsetof(SubtitleSpriteWork, x) == 0x60) ? 1 : -1];
typedef char Layout_load_title_11[(offsetof(SubtitleSpriteWork, textures) == 0x68) ? 1 : -1];
typedef char Layout_load_title_12[(offsetof(SubtitleSpriteWork, rotation) == 0x78) ? 1 : -1];
typedef char Layout_load_title_13[(offsetof(SubtitleSpriteWork, alpha) == 0x7c) ? 1 : -1];
typedef char Layout_load_title_14[(offsetof(SubtitleSpriteWork, alphaStep) == 0x7e) ? 1 : -1];
typedef char Layout_load_title_15[(offsetof(SubtitleSpriteWork, stacked) == 0x80) ? 1 : -1];
#endif

// Rows of the four-texture SUBTITLE.SPR stack: centre Y and height of each 512-wide strip.
#define SHENMUE_SUBTITLE_STACK_Y_VALUES {240, 209, 310, 240}
#define SHENMUE_SUBTITLE_STACK_HEIGHT_VALUES {512, 128, 64, 512}
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern const std::int16_t SUBTITLE_STACK_Y[4];
extern const std::int16_t SUBTITLE_STACK_HEIGHT[4];
#else
SHENMUE_LOAD_TITLE_DATA std::array<std::int16_t, 4> SUBTITLE_STACK_Y SHENMUE_SUBTITLE_STACK_Y_VALUES;
SHENMUE_LOAD_TITLE_DATA std::array<std::int16_t, 4> SUBTITLE_STACK_HEIGHT SHENMUE_SUBTITLE_STACK_HEIGHT_VALUES;
#endif
// 0x10000 / 3600: the per-frame rotation of the first stacked strip.
SHENMUE_TITLE_CONSTANT float SUBTITLE_ROTATION_STEP = 18.204445f;

struct LoadTitleEntry {
    const char *key;
    const char *alias;
};

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
struct MapRegionEntry {
    const char *area;
    const char *map;
    std::array<float, 4> bounds;
};

#endif

struct FloorTitleEntry {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::uint32_t area;
#else
    const char *area;
#endif
    std::int32_t entry;
    const char *sprite;
};
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ReleaseLoadTitleFade();
void DoLoadStuff(std::uint32_t author, bool nextStream);
void ST_LoadTitleFadeIn();
void ST_LoadTitleFade();
void Queue_ST_LoadTitleFadeIn(shen_name author);
#else
// CDTT has no task parameter block; this aggregates its separate native globals.
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

// Native ST loading title owner (CDTT): the loading-screen widget content,
// the legacy title/floor/date sprites (nullsub submissions on PC) and the
// white/black fade quads of fade modes 1 and 6.
void InitST_TitleFunc();
void ReleaseLoadTitleFade();
void DoLoadStuff(std::uint32_t author, bool nextStream);
shen_name GetMapRegionName(const Vector3 &position);
void SetTemporaryAreaJumpAuthor(shen_name author);
void ResetTemporaryAreaJumpAuthor();
void ST_LoadTitleFadeIn();
void LoadTitleFadeIn();
void ST_LoadTitleFade();
void Queue_ST_LoadTitleFadeIn(shen_name author);
std::uint64_t scenSetTaskFlags(std::uint8_t mode);
std::int16_t GetLoadTitleAlpha();
void SetLoadingDrawFlags(std::int8_t mode);
void DrawLoadingModes();
void RetireLoadingDraw();
void ResetLoadingDrawState();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsLoadingDrawActive();
#else
bool IsLoadingDrawActive();
#endif

#endif

// SBTS: a sprite subtitle held on a nullsub task until faded in, then retired after its fade-out. PC v1.07 reaches these only
// through FTBL slots 144-146; no compiled event code calls them.
HLib::Task *Queue_ST_WaitSubTitle(const char *filename);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ST_FadeInSubTitle(HLib::Task *task, float x, float y, std::int16_t alphaStep);
#else
std::int64_t ST_FadeInSubTitle(HLib::Task *task, float x, float y, std::int16_t alphaStep);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ST_FadeOutSubTitle(HLib::Task *task);
#else
std::int64_t ST_FadeOutSubTitle(HLib::Task *task);
#endif
} // namespace s2
} // namespace shenmue

#undef SHENMUE_TITLE_CONSTANT

#undef SHENMUE_LOAD_TITLE_DATA
