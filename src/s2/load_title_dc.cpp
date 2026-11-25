#include "s2/load_title_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/legacy_sprite_slot.hpp"
#include "s2/legacy_sprite_types.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/texture_dc.hpp"
#include "platform/dc/sdk.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/map_control_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/window.hpp"
#endif

namespace shenmue {
namespace s2 {
std::int32_t g_loadScreenSpriteTexture = -1;
std::uint8_t g_loadingDrawActive = 0;
std::int8_t g_loadingDrawMode = 0;
std::uint8_t g_loadingDotFrame = 0;
std::int16_t g_loadTitleAlpha = 0;
std::uint32_t g_loadingSpin = 0;
float g_loadingSpinRate = 4.0f;
HLib::Task *g_memoriesTask;
std::int32_t g_memoryLoadingIconTextures[18];
std::int16_t g_memoryLoadingIconAlpha;
std::int16_t g_memoryLoadingIconAlphaStep;
std::int8_t g_memoryLoadingIconTimeOfDay;
std::int8_t g_memoryLoadingIconFadeIn;
HLib::Task *g_timeLimitTask;
HLib::Task *g_loadTitleTask;
HLib::Task *g_ktdeTask;
float g_endingTextX;
float g_endingTextY;
std::int32_t g_endingTextTextures[3];
std::int32_t g_loadTitleDigitTextures[37] = {};
std::int32_t g_loadTitleTexture;
std::int32_t g_loadFloorTexture;
std::uint32_t g_loadTitleFlags;
std::int8_t g_loadTitleFadeMode;
std::int32_t g_timeLimitDigitTextures[13];
std::int32_t g_timeLimitLabelTextures[2];
std::int32_t g_timeLimitSeconds;
std::int16_t g_timeLimitFrame;
std::int16_t g_timeLimitAlarmTimer;
std::int16_t g_timeLimitAlarmInterval;
std::uint16_t g_timeLimitHidden;
std::int8_t g_timeLimitRunning;

void ReleaseNamedTexture_Wrapper(std::int32_t index);

// @verified - d
void InitST_TitleFunc() {
    g_memoriesTask = 0;
    g_ktdeTask = 0;
    g_timeLimitTask = 0;
    g_loadTitleTask = 0;
    for (std::int32_t index = 0; index < 37; ++index)
        g_loadTitleDigitTextures[index] = -1;
    g_loadTitleTexture = -1;
    g_loadFloorTexture = -1;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void TimeLimit_Callback(void *);

void CreateTimeLimitTask(std::uint16_t hidden) {
    g_timeLimitHidden = hidden;
    if (!hidden) {
        void *const data = shenLoadFile("sprite", "SWLDNUM.SPR", SHEN_NAME4('L', 'D', 'T', 'D'));
        LoadTextureChunks(reinterpret_cast<const TextureChunk *>(data), g_timeLimitDigitTextures);
        HLib::FreeTaskData(data);
        g_timeLimitLabelTextures[0] = CreateCenteredWindowTextSurface(
            static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, "\273\304\244\352", 64, 0xffff, 0xf000, 1);
        g_timeLimitLabelTextures[1] = CreateCenteredWindowTextSurface(
            static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, "\241\247", 64, 0xffff, 0xf000, 1);
    }
    g_timeLimitTask = HLib::EnqueueTaskWithoutParameter(TimeLimit_Callback, 8, 0, SHEN_NAME4('T', 'M', 'L', 'T'));
    g_timeLimitRunning = 0;
    g_timeLimitFrame = 0;
    g_timeLimitAlarmInterval = 30;
    g_timeLimitAlarmTimer = 30;
}
#endif

void StartTimeLimit(std::int32_t seconds) {
    g_timeLimitRunning = 1;
    g_timeLimitAlarmInterval = 30;
    g_timeLimitAlarmTimer = 30;
    if (seconds) {
        if (seconds >= 86400)
            seconds = 86400;
        g_timeLimitSeconds = seconds;
        g_timeLimitFrame = 0;
    }
}

void StopTimeLimit() {
    g_timeLimitRunning = 0;
}

void ToggleTimeLimit() {
    g_timeLimitRunning = (g_timeLimitRunning + 1) & 1;
}

void DestroyTimeLimitTask() {
    if (!g_timeLimitTask)
        return;
    if (!g_timeLimitHidden) {
        for (std::int32_t index = 0; index < 13; ++index)
            ReleaseNamedTexture_Wrapper(g_timeLimitDigitTextures[index]);
        ReleaseNamedTexture_Wrapper(g_timeLimitLabelTextures[0]);
        ReleaseNamedTexture_Wrapper(g_timeLimitLabelTextures[1]);
    }
    HLib::DestroyTaskInternal(g_timeLimitTask);
    g_timeLimitTask = 0;
    g_timeLimitHidden = 0;
}

std::int32_t GetTimeLimitSeconds() {
    return g_timeLimitSeconds;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const std::int16_t timeLimitAlarmIntervals[11] = {2, 2, 5, 7, 9, 13, 17, 21, 24, 27, 30};

void DrawTimeLimit(void *);

void TimeLimit_Callback(void *) {
    if (!g_timeLimitRunning)
        return;
    if (++g_timeLimitFrame > 29) {
        g_timeLimitFrame = 0;
        if (--g_timeLimitSeconds < 1)
            g_timeLimitSeconds = 0;
        g_timeLimitAlarmInterval = timeLimitAlarmIntervals[g_timeLimitSeconds];
    }
    HLib::ExecuteCallbackNow(DrawTimeLimit, 0);
    const std::int32_t seconds = g_timeLimitSeconds;
    if (seconds > 10)
        return;
    if (++g_timeLimitAlarmTimer >= g_timeLimitAlarmInterval) {
        g_timeLimitAlarmTimer = 0;
        if (seconds > 0)
            ExecuteAicaCommand(0xd00ab, 0, 0);
    }
}
#endif

// @verified - d
void DrawTimeLimit(void *) {
    if (g_timeLimitHidden)
        return;
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    const std::int32_t seconds = g_timeLimitSeconds;
    std::int16_t hours = seconds / 3600;
    const std::int16_t hourTens = hours / 10;
    hours %= 10;
    std::int16_t remainder = seconds % 3600;
    std::int16_t minutes = remainder / 60;
    const std::int16_t minuteTens = minutes / 10;
    minutes %= 10;
    remainder %= 60;
    const std::int16_t secondTens = remainder / 10;
    remainder %= 10;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 208, 64, 64, 32);
    DrawLegacySpritePacket(packet, g_timeLimitLabelTextures[0], 5.0f);
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 248, 64, 32, 32);
    DrawLegacySpritePacket(packet, g_timeLimitDigitTextures[hourTens], 5.0f);
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 280, 64, 32, 32);
    DrawLegacySpritePacket(packet, g_timeLimitDigitTextures[hours], 5.0f);
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 304, 64, 64, 32);
    DrawLegacySpritePacket(packet, g_timeLimitLabelTextures[1], 5.0f);
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 328, 64, 32, 32);
    DrawLegacySpritePacket(packet, g_timeLimitDigitTextures[minuteTens], 5.0f);
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 360, 64, 32, 32);
    DrawLegacySpritePacket(packet, g_timeLimitDigitTextures[minutes], 5.0f);
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 384, 64, 64, 32);
    DrawLegacySpritePacket(packet, g_timeLimitLabelTextures[1], 5.0f);
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 408, 64, 32, 32);
    DrawLegacySpritePacket(packet, g_timeLimitDigitTextures[secondTens], 5.0f);
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 440, 64, 32, 32);
    DrawLegacySpritePacket(packet, g_timeLimitDigitTextures[remainder], 5.0f);
}

void MemoriesTask_Callback(void *);

void EnqueueMemoriesTask(std::int8_t timeOfDay, std::int16_t fadeFrames) {
    if (g_memoriesTask)
        return;
    void *const data = shenLoadFile("sprite", "MEMORIES.SPR", SHEN_NAME4('S', 'N', 'E', 'F'));
    LoadTextureChunks(reinterpret_cast<const TextureChunk *>(data), g_memoryLoadingIconTextures);
    HLib::FreeTaskData(data);
    g_memoriesTask = HLib::EnqueueTaskWithoutParameter(MemoriesTask_Callback, 8, 0, SHEN_NAME4('M', 'I', 'T', 'K'));
    g_memoryLoadingIconTimeOfDay = timeOfDay;
    g_memoryLoadingIconAlphaStep = fadeFrames;
    if (g_memoryLoadingIconAlphaStep <= 0)
        g_memoryLoadingIconAlphaStep = 1;
    g_memoryLoadingIconAlphaStep = 256 / g_memoryLoadingIconAlphaStep;
    g_memoryLoadingIconAlpha = 0;
    g_memoryLoadingIconFadeIn = 1;
}

void StartMemoriesFadeOut(std::int32_t fadeFrames) {
    g_memoryLoadingIconAlphaStep = fadeFrames;
    if (g_memoryLoadingIconAlphaStep <= 0)
        g_memoryLoadingIconAlphaStep = 1;
    g_memoryLoadingIconAlphaStep = 256 / g_memoryLoadingIconAlphaStep;
    g_memoryLoadingIconAlpha = 255;
    g_memoryLoadingIconFadeIn = 0;
}

void DrawMemoriesIcons(void *);

void MemoriesTask_Callback(void *) {
    if (g_memoryLoadingIconFadeIn) {
        g_memoryLoadingIconAlpha += g_memoryLoadingIconAlphaStep;
        if (g_memoryLoadingIconAlpha > 255)
            g_memoryLoadingIconAlpha = 255;
    } else {
        g_memoryLoadingIconAlpha -= g_memoryLoadingIconAlphaStep;
        if (g_memoryLoadingIconAlpha < 0)
            g_memoryLoadingIconAlpha = 0;
    }
    HLib::ExecuteCallbackNow(DrawMemoriesIcons, 0);
    if (!g_memoryLoadingIconAlpha) {
        for (std::int32_t index = 0; index < 18; ++index)
            ReleaseNamedTexture_Wrapper(g_memoryLoadingIconTextures[index]);
        HLib::DestroyTaskInternal(g_memoriesTask);
        g_memoriesTask = 0;
    }
}

struct MemoryQuadLayout {
    std::int16_t centerX, centerY;
    float scaleX, scaleY;
    std::int16_t flags;
};
static const MemoryQuadLayout kMemoryLayouts[20] = {{160, 120, 20.0f, 15.0f, 0x200}, {480, 120, 20.0f, 15.0f, 0x300}, {160, 360, 20.0f, 15.0f, 0},
    {480, 360, 20.0f, 15.0f, 0x100}, {120, 100, 15.0f, 10.0f, 0x200}, {520, 100, 15.0f, 10.0f, 0x300}, {120, 380, 15.0f, 10.0f, 0},
    {520, 380, 15.0f, 10.0f, 0x100}, {120, 110, 17.5f, 12.5f, 0x200}, {520, 110, 17.5f, 12.5f, 0x300}, {120, 370, 17.5f, 12.5f, 0},
    {520, 370, 17.5f, 12.5f, 0x100}, {160, 120, 20.0f, 15.0f, 0x200}, {480, 120, 20.0f, 15.0f, 0x300}, {160, 360, 20.0f, 15.0f, 0},
    {480, 360, 20.0f, 15.0f, 0x100}, {64, 84, 14.0f, 10.0f, 0x200}, {288, 84, 14.0f, 10.0f, 0x300}, {64, 244, 14.0f, 10.0f, 0},
    {288, 244, 14.0f, 10.0f, 0x100}};
static const MemoryQuadLayout kMemoryLayoutsVga[20] = {{160, 120, 20.0f, 15.0f, 0x200}, {480, 120, 20.0f, 15.0f, 0x300}, {160, 360, 20.0f, 15.0f, 0},
    {480, 360, 20.0f, 15.0f, 0x100}, {120, 80, 15.0f, 10.0f, 0x200}, {520, 80, 15.0f, 10.0f, 0x300}, {120, 400, 15.0f, 10.0f, 0},
    {520, 400, 15.0f, 10.0f, 0x100}, {120, 100, 17.5f, 12.5f, 0x200}, {520, 100, 17.5f, 12.5f, 0x300}, {120, 380, 17.5f, 12.5f, 0},
    {520, 380, 17.5f, 12.5f, 0x100}, {160, 120, 20.0f, 15.0f, 0x200}, {480, 120, 20.0f, 15.0f, 0x300}, {160, 360, 20.0f, 15.0f, 0},
    {480, 360, 20.0f, 15.0f, 0x100}, {64, 84, 14.0f, 10.0f, 0x200}, {288, 84, 14.0f, 10.0f, 0x300}, {64, 244, 14.0f, 10.0f, 0},
    {288, 244, 14.0f, 10.0f, 0x100}};

void DrawMemoriesIcons(void *) {
    dc::sdk::njTextureFilterMode(1);
    std::int32_t layoutIndex = 0;
    std::int32_t imageIndex = 0;
    switch (g_memoryLoadingIconTimeOfDay) {
    case 0:
    case 1:
        layoutIndex = 0;
        imageIndex = g_memoryLoadingIconTimeOfDay;
        break;
    case 2:
    case 3:
        layoutIndex = 4;
        imageIndex = g_memoryLoadingIconTimeOfDay;
        break;
    case 4:
    case 5:
        layoutIndex = 8;
        imageIndex = g_memoryLoadingIconTimeOfDay;
        break;
    case 6:
    case 7:
    case 8:
        layoutIndex = 12;
        imageIndex = g_memoryLoadingIconTimeOfDay - 2;
        break;
    case 9:
    case 10:
        layoutIndex = 16;
        imageIndex = g_memoryLoadingIconTimeOfDay - 2;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        layoutIndex = 0;
        imageIndex = g_memoryLoadingIconTimeOfDay - 2;
        break;
    }
    const MemoryQuadLayout *layout = !dc::sdk::syCblCheckCable() ? &kMemoryLayoutsVga[layoutIndex] : &kMemoryLayouts[layoutIndex];
    for (std::int32_t index = 0; index < 4; ++index) {
        LegacySpritePacket packet;
        INIT_LEGACY_SPRITE_PACKET(packet);
        packet.colour = (static_cast<std::uint32_t>(g_memoryLoadingIconAlpha) << 24) | 0xffffffU;
        TransformScreenSpace(packet.quad, HorizontalStretch, layout->centerX, layout->centerY, 16, 16, layout->scaleX, layout->scaleY, 0);
        packet.uv[0] = 0.1f;
        packet.uv[1] = 0.1f;
        packet.uv[2] = 0.9f;
        packet.uv[3] = 0.9f;
        packet.quad.flags |= layout->flags | 0x400U;
        DrawLegacySpritePacket(packet, g_memoryLoadingIconTextures[imageIndex], 1.0f);
        ++layout;
    }
    dc::sdk::njTextureFilterMode(0);
}

void InitEndingText() {
    g_ktdeTask = 0;
    g_endingTextX = -256.0f;
    g_endingTextY = 0.0f;
}

void EndingText_Callback(void *);

void CreateEndingText() {
    if (g_ktdeTask)
        return;
    g_ktdeTask = HLib::EnqueueTaskWithoutParameter(EndingText_Callback, 8, 0, SHEN_NAME4('E', 'D', 'T', 'K'));
    void *const data = shenLoadFile("sprite", "ENDMOJI0.SPR", SHEN_NAME4('E', 'D', 'M', 'J'));
    LoadTextureChunks(reinterpret_cast<const TextureChunk *>(data), g_endingTextTextures);
    HLib::FreeTaskData(data);
}

void DestroyEndingText() {
    if (!g_ktdeTask)
        return;
    for (std::int32_t index = 0; index < 3; ++index)
        ReleaseNamedTexture_Wrapper(g_endingTextTextures[index]);
    HLib::DestroyTaskInternal(g_ktdeTask);
    g_ktdeTask = 0;
}

void SetEndingTextPosition(float x, float y) {
    g_endingTextX = x;
    g_endingTextY = y;
}

void DrawEndingText(void *);

void EndingText_Callback(void *) {
    HLib::ExecuteCallbackNow(DrawEndingText, 0);
}

void DrawEndingText(void *) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    AdjustScreenAspectRatio(packet.quad, FullStretch, static_cast<std::int32_t>(g_endingTextX), static_cast<std::int32_t>(g_endingTextY), 512, 512);
    DrawLegacySpritePacket(packet, g_endingTextTextures[0], 6.0f);
    AdjustScreenAspectRatio(packet.quad, FullStretch, static_cast<std::int32_t>(g_endingTextX - 512.0f), static_cast<std::int32_t>(g_endingTextY), 512, 512);
    DrawLegacySpritePacket(packet, g_endingTextTextures[1], 6.0f);
    AdjustScreenAspectRatio(
        packet.quad, FullStretch, static_cast<std::int32_t>(g_endingTextX - 1024.0f), static_cast<std::int32_t>(g_endingTextY - 42.0f), 128, 128);
    DrawLegacySpritePacket(packet, g_endingTextTextures[2], 6.0f);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void scenSetTaskFlags(std::int32_t mode) {
#else
void scenSetTaskFlags(std::uint8_t mode) {
#endif
    const std::int8_t fadeMode = static_cast<std::int8_t>(mode);
    if (fadeMode == 9)
        g_loadTitleFlags |= 1U;
    else
        g_loadTitleFlags &= ~1U;
    if (g_loadTitleFadeMode != 4) {
        g_loadTitleFadeMode = fadeMode;
        if (g_loadTitleFlags)
            g_loadTitleFadeMode = 3;
    }
}

void ST_LoadTitleFadeIn_Main(void *);

// @verified - d
void SetLoadingDrawFlags(std::int32_t mode) {
    RegisterBorrowedLegacySpriteSlot(g_loadScreenSpriteTexture, 128, 128, 34, kLoadingSpriteRectangles);
    g_loadingDrawActive = 1;
    g_loadingDrawMode = mode;
    g_loadingDotFrame = 0;
    g_loadTitleAlpha = 255;
}

// @verified - d
void RetireLoadingDraw() {
    g_loadingDrawActive = 0;
    ReleaseBorrowedLegacySpriteSlot(34);
}

// @verified - d
std::int32_t IsLoadingDrawActive() {
    return static_cast<std::int8_t>(g_loadingDrawActive);
}

// @verified - d
void DrawLoadingSpinner(std::int32_t x, std::int32_t y) {
    switch (g_loadingDrawMode) {
    case 0:
        DrawLoadingLabel(0xff000000U);
        break;
    case 1:
        reinterpret_cast<void (*)()>(ST_LoadTitleFadeIn_Main)();
        break;
    }
    g_loadingSpin =
        (g_loadingSpin + (static_cast<std::uint32_t>(static_cast<std::int32_t>(((g_loadingSpinRate * -3.0f) * 8192.0f) / 45.0f)) & 0xffffU)) & 0xffffU;
    SetLegacySpriteSlotScale(0.81999999f, 0.81999999f);
    SetLegacySpriteSlotRotation(g_loadingSpin);
    SetLegacySpriteSlotColour(0xffc8c8c8U);
    DrawTempSpriteSlot(0x220000U, (static_cast<std::uint32_t>(x) << 16) | static_cast<std::uint32_t>(y), 9.0f, 0);
    SetLegacySpriteSlotColour(UINT32_MAX);
    SetLegacySpriteSlotRotation(0);
    SetLegacySpriteSlotScale(1.0f, 1.0f);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_loadingDotInterval = 14;

void DrawLoadingDots(std::int32_t x, std::int32_t y) {
    switch (g_loadingDrawMode) {
    case 0:
        DrawLoadingLabel(0xff000000U);
        break;
    case 1:
        ST_LoadTitleFadeIn_Main(0);
        break;
    }
    ++g_loadingDotFrame;
    if (static_cast<std::int8_t>(g_loadingDotFrame) >= g_loadingDotInterval * 6)
        g_loadingDotFrame = 0;
    SetLegacySpriteSlotScale(1.25f, 1.3125f);
    for (std::int32_t index = 0; index < static_cast<std::int8_t>(g_loadingDotFrame) / g_loadingDotInterval; ++index)
        DrawTempSpriteSlot(0x220001U, (static_cast<std::uint32_t>(x + index * 6) << 16) | static_cast<std::uint32_t>(y), 9.0f, 0);
    SetLegacySpriteSlotScale(1.0f, 1.0f);
}

void DrawLoadingModes() {
    LegacyIconSprite marker;
    switch (g_loadingDrawMode) {
    case 0:
    case 1:
        DrawLoadingDots(kLoadingSpritePositions[g_loadingDrawMode][0], kLoadingSpritePositions[g_loadingDrawMode][1]);
        break;
    case 2:
        DrawLoadingSpinner(kLoadingSpritePositions[g_loadingDrawMode][0], kLoadingSpritePositions[g_loadingDrawMode][1]);
        marker.width = marker.height = 38;
        marker.scaleX = marker.scaleY = 1.0f;
        marker.rotation = 0;
        marker.icon = 0;
        marker.x = 64.0f;
        marker.y = 460.0f;
        marker.flags = 0;
        marker.alignment = 9;
        marker.depth = 3.0f;
        PrepareDrawWithMode(marker);
        break;
    }
}
#else
void DrawLoadingDots(std::int32_t x, std::int32_t y) {
    if (!g_loadingDrawMode)
        DrawLoadingLabel(0xff000000U);
    else if (g_loadingDrawMode == 1)
        ST_LoadTitleFadeIn_Main(0);
    ++g_loadingDotFrame;
    if (g_loadingDotFrame > 83)
        g_loadingDotFrame = 0;
    SetLegacySpriteSlotScale(1.25f, 1.3125f);
    for (std::int32_t index = 0; index < g_loadingDotFrame / 14; ++index)
        DrawTempSpriteSlot(0x220001U, (static_cast<std::uint32_t>(x + index * 6) << 16) | static_cast<std::uint32_t>(y), 9.0f, 0);
    SetLegacySpriteSlotScale(1.0f, 1.0f);
}
void DrawLoadingModes() {
    if (g_loadingDrawMode < 0 || g_loadingDrawMode > 2)
        return;
    const std::int16_t *const position = kLoadingSpritePositions[g_loadingDrawMode];
    if (g_loadingDrawMode < 2) {
        DrawLoadingDots(position[0], position[1]);
        return;
    }
    DrawLoadingSpinner(position[0], position[1]);
    LegacyIconSprite marker;
    marker.width = 38;
    marker.height = 38;
    marker.scaleX = 1.0f;
    marker.scaleY = 1.0f;
    marker.rotation = 0;
    marker.icon = 0;
    marker.x = 64.0f;
    marker.y = 460.0f;
    marker.flags = 0;
    marker.alignment = 9;
    marker.depth = 3.0f;
    PrepareDrawWithMode(marker);
}
#endif
} // namespace s2
} // namespace shenmue
#endif
