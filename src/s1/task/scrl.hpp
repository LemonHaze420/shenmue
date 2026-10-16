#pragma once

#include "s1/hlib.hpp"
#include "s1/screen_space.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstdint>

namespace shenmue::s1 {

struct ScenarioContext;

enum class SpriteUtilCommand : std::uint32_t {
    ReadSky = 0,
    ReadSlot = 1,
    LoadSlotAsSky = 2,
    ModifySky = 3,
    SetVerticalOffset = 4,
    SetHorizontalOffset = 5,
};

struct SpriteUtilParams {
    SpriteUtilCommand mode;
    std::int16_t value;
    std::uint16_t reserved006;
    std::int32_t offset;
    std::uint32_t slotOrIntensity;
};

static_assert(sizeof(SpriteUtilParams) == 0x10);





struct SkyTextureRecord {
    std::int32_t hasUpper;
    std::uint32_t upperColour;
    std::uint32_t clearColour;
    std::uint32_t fogTableReference;
    std::int32_t surface;
    std::int32_t upperSurface;
    float alphaStep;
    float alpha;
    char path[24];
    char file[24];
};
static_assert(sizeof(SkyTextureRecord) == 0x50);



inline constexpr std::array<Vector2, 4> kSkySegmentCentres{{{-1440.0f, 60.0f}, {-480.0f, 60.0f}, {480.0f, 60.0f}, {1440.0f, 60.0f}}};
inline constexpr std::array<Vector2, 4> kSkyQuadCorners{{{-480.0f, -240.0f}, {480.0f, -240.0f}, {-480.0f, 240.0f}, {480.0f, 240.0f}}};
inline constexpr std::array<Vector2, 4> kSkyUpperCentres{{{-1440.0f, -807.75f}, {-480.0f, -807.75f}, {480.0f, -807.75f}, {1440.0f, -807.75f}}};

extern std::array<SkyTextureRecord, 2> g_skyTextures;

extern std::array<std::int32_t, 3> g_skySpriteLoaded;
extern std::array<HLib::TaskData *, 3> g_skySpriteData;
extern char g_skySpritePaths[3][24];
extern char g_skySpriteFiles[3][24];


extern std::array<OriginalStrip, 4> g_skyPackets;
extern std::array<OriginalStrip, 4> g_skyUpperPackets;
extern std::array<OriginalStrip, 4> g_skyIncomingPackets;
extern std::array<OriginalStrip, 4> g_skyIncomingUpperPackets;
extern std::array<OriginalStrip, 4> g_skyCloudPackets;

extern float g_skyScrollX;
extern float g_skyScrollY;

extern std::uint64_t g_cloudXY[2];
extern std::int32_t g_bSkyEnabled;

extern std::int32_t g_skyTextureLoaded;
extern std::uint32_t g_shouldOverrideClearColour;
extern std::uint32_t g_overrideClearColour;
extern std::uint32_t g_SkyClearColour;
extern std::int32_t g_bShowSkyTexture;

extern std::int32_t g_skyMode;

extern std::int32_t g_skyFadeFrames;

extern std::int32_t g_skyFadeSlot;
extern std::int32_t g_skyFadeFreesSlot;
extern std::int32_t g_shouldAdjustSky;
extern std::uint32_t g_skyCorner;
extern std::int32_t g_spriteVerticalOffset;
extern std::int32_t g_spriteHorizontalOffset;


extern char g_scrollRequestPath[24];
extern char g_scrollRequestFile[24];
extern std::int32_t g_scrollRequestFadeFrames;
extern std::int32_t g_scrollFlag;
extern void *g_scrollCurrentInfo;
extern std::int32_t g_bShouldFreeActiveScroll;

extern std::int32_t g_scrollSpriteFlags0;
extern std::int32_t g_scrollSpriteFlags;
extern std::uint32_t g_skyLuminanceEnabled;
extern std::uint16_t g_skyLuminanceHigh;
extern std::uint16_t g_skyLuminanceMiddle;
extern std::uint16_t g_skyLuminanceLow;

void InitSky();
void ReadSCRL(void *mapInfo);
void LoadSPR_0(const void *sclm);
void SCRL_Callback_0(HLib::TaskData *taskData);
HLib::Task *SCRL_Init(void *mapInfo);
void LoadSpriteTexture(void *data, std::int32_t record);
void LoadSCRL(void *data, char *chunkName);
char *LoadSCR(void *data, char *chunkName, std::int32_t slot);
void Shenmue_ReadSprite_Wrapper(ScenarioContext *scenarioContext, const char *filename);
void ReadSprite_Wrapper(ScenarioContext *scenarioContext, const char *filename, std::int32_t slot);
void LoadSCRL1(std::int32_t shouldFreeSlot, std::int32_t slot);
void ControlSkyTextureFlags(std::int32_t slot, std::int32_t fadeFrames, std::int32_t freesSlot);
bool SCRL_LoadSprite(const char *filename, std::int32_t fadeFrames);
void RefreshAndFreeCurrentSkyScroll();
void SetShouldLoadWeatherSprites(std::int32_t shouldLoad);
bool SCRL_SetLuminanceEnabled(std::uint32_t enabled);
bool SCRL_SetLuminanceChannels(std::uint16_t high, std::uint16_t middle, std::uint16_t low);
void Script_SetScrollU(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
