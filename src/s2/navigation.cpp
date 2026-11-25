#include "shenmue.h"
#include "s2/legacy_sprite_types.hpp"
#include "common/native_sqrt.hpp"

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/navigation.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/navi_data_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/author_control_state_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/search_state_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/texture_cache_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/event_overlay.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/screen.hpp"
#include <float.h>
#include <math.h>
#include <cstring>
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/hlib.hpp"
#include "common/math.hpp"
#endif
#else
#include "s2/navigation.hpp"
#include "s2/navi_window.hpp"
#include "s2/legacy_sprite.hpp"
#include "d3t/texture.hpp"
#include "s2/area_event.hpp"
#include "s2/author.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/help.hpp"
#include "s2/gauge.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/navi.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>
#endif

namespace shenmue {
namespace s2 {

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

constexpr shen_name naviTaskName{SHEN_NAME(NAVI)};

HLib::Task *navigationTask{};

NaviWindowState naviWindow;

Vector4 naviWindowPosition{};

float naviWindowVelocity{};

LegacyIconSprite naviWindowHeadingSprite{}, naviWindowSelectionSprite{};

std::array<std::int32_t, 6> naviWindowLabelTextures, naviWindowSecondaryTextures, naviWindowLabels{}, naviWindowSecondaryLabels{};

std::array<std::int32_t, 2> naviWindowPreviewTextures{};

std::int32_t naviWindowPreviewTexture;

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
std::uint32_t NaviFolderEntryMask(std::int32_t page, std::uint16_t excludedFlags);
std::int32_t NaviFolderEntryCount(std::int32_t page, std::uint16_t excludedFlags);
bool NaviIsDisplayable(bool skipMapItemRequirement);
HLib::Task *NAVI_DummyWait_Enqueue(std::uint8_t nextFunctionIndex);
#if SHENMUE_S2_GCC295
HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::int8_t nextFunctionIndex);
#else
HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::uint8_t nextFunctionIndex);
#endif
std::int64_t OpenNaviWindow();
std::int64_t CloseNaviWindow(std::int32_t force);

namespace {

void ReleaseNaviWindowStrips(std::uint8_t flags);
void PublishNaviWindowLabel(NaviWindowState &work, const char *label, const char *secondary, std::int32_t slot);
std::uint32_t BuildNaviWindowHeading(std::int32_t slot, std::int32_t category);
void BuildNaviWindowRootLabels(NaviWindowState &work);
void BuildNaviWindowFolderLabels(NaviWindowState &work, std::int32_t category);
void ReleaseNaviWindowAllocations();
void LoadNaviWindowPageSource();
void RefreshNaviWindowPreview();
void InitializeNaviWindowSprites();
void DrawNaviWindowLabel(std::int32_t row, const Vector2 &position, std::uint32_t colour);
void DrawNaviWindow(void *);
void NAVI_CtrlNavi_Callback(void *taskData);
void NAVI_DummyWait_Callback(void *taskData);
void NVW_CtrlNaviWindow_Callback(void *);

} // namespace
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

struct NativeNaviWindowState {
    std::int32_t phase, page, requestedPage;
    std::uint32_t drawFlags;
    HLib::Task *task;
    std::int32_t savedNpcLoadState;
    std::uint32_t availablePages, availableEntries;
    std::int32_t selections[2];
    std::int32_t firstVisibleEntry, entryCount;
    std::int32_t mapHeight, mapWidth;
    float mapScaleX, mapScaleY;
    void *mapImageData, *labelScratch, *dummySpriteData;
};

typedef char NativeNaviWindowSizeCheck[sizeof(NativeNaviWindowState) == 0x4c ? 1 : -1];

namespace {

void DrawNaviBackground(std::int32_t x, std::int32_t y, float zoom);

} // namespace

void NAVI_CtrlNavi_Callback(void *data);

void NVW_CtrlNaviWindow_Callback(void *);
void ReleaseNamedTexture_Wrapper(std::int32_t index);

std::int32_t navCreate(const Vector3 &position, std::int32_t type);

bool ConstructNaviMap(std::uint32_t configIndex);

std::uint16_t GetCommonIconTextureHandle(std::uint16_t icon);

std::int32_t naviTextures[5];

LegacySpritePacket naviPackets[5], naviOverlayPacket, naviTrailPacket;

std::int32_t naviPhase;

float naviTriggerBlend;

std::uint32_t naviTiltAngle = 0x31c7;

float naviBackgroundWidthScale = 1.56f, naviBackgroundHeightScale = 1.3f;

std::uint8_t naviTrailStamp;

static shen_name naviRetainedMap;

std::int32_t naviBackgroundTexture = -1;

std::int32_t naviFloorTextures[6];

NativeNaviWindowState naviWindow;

NativeNaviWindowState *naviWindowWork = &naviWindow;

float naviWindowScales[2] = {1.5f, 0.875f};

ItemEffectPosition naviWindowPosition;

LegacyIconSprite naviWindowHeadingSprite, naviWindowSelectionSprite;

std::int32_t naviWindowLabelTextures[6], naviWindowSecondaryTextures[6], naviWindowLabels[6], naviWindowSecondaryLabels[6];

std::int32_t naviWindowPreviewTexture = -1;

std::int32_t naviWindowPreviewTextures[2];

std::int32_t RasterizeWindowTextLine4bpp(std::byte *destination, std::int32_t width, const char *text, std::int32_t mode, std::int32_t centered);

namespace {

LegacyIconSprite naviFloorLabelSprite = nativeNaviFloorLabelSprite;

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t naviAreaConfigCount = 57;

// @verified - d
std::int32_t navGetNumEntriesFor(shen_name map) {
    for (std::int32_t index = 0; index < naviAreaConfigCount && nativeNaviAreaConfigs[index].map; ++index)
        if (map == nativeNaviAreaConfigs[index].map)
            return nativeNaviAreaConfigs[index].configIndex;
    return -1;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SelectNaviMapMarkers() {
    std::int32_t area = navGetNumEntriesFor(g_currentMap);
    for (std::int32_t index = 0; index < naviAreaConfigCount && nativeNaviAreaConfigs[index].map; ++index) {
        if (g_currentMap == nativeNaviAreaConfigs[index].map) {
            area = index;
            break;
        }
    }
    GetQronNextStream();
    std::uint32_t *marker = g_savedNaviMarkers;
    for (std::int32_t index = 0; index < 128; ++index, ++marker) {
        const std::uint32_t value = *marker & ~1U;
        *marker = value;
        if (((value >> 1) & 7) && ((value >> 4) & 63) == static_cast<std::uint32_t>(area))
            *marker = value | 1;
    }
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#define INIT_NAVI_PACKET(packet)                                                                                                                               \
    do {                                                                                                                                                       \
        (packet)->quad.flags = 0;                                                                                                                              \
        (packet)->colour = 0xffffffffU;                                                                                                                        \
        (packet)->secondaryColour = 0;                                                                                                                         \
        (packet)->uv[0] = (packet)->uv[1] = 0.0f;                                                                                                              \
        (packet)->uv[2] = (packet)->uv[3] = 1.0f;                                                                                                              \
        (packet)->reservedTail = 0;                                                                                                                            \
    } while (0)

void InitializeNaviIcons() {
    LegacySpritePacket *const packets = naviPackets;
    std::int32_t index = 0;
    do {
        naviTextures[index] = -1;
        INIT_NAVI_PACKET(&packets[index]);
        packets[index].quad.flags |= 0x400U;
    } while (++index < 5);
    const std::int8_t *rectangle;
    LegacySpritePacket *packet;
    packet = &naviPackets[1];
    naviTextures[1] = GetCommonIconTextureHandle(0x414);
    rectangle = GetCommonIconRect(0x414);
    packet->uv[0] = static_cast<float>(rectangle[0]) * 0.125f;
    packet->uv[1] = static_cast<float>(rectangle[1]) * 0.125f;
    packet->uv[2] = static_cast<float>(rectangle[2]) * 0.125f;
    packet->uv[3] = static_cast<float>(rectangle[3]) * 0.125f;
    packet = &naviPackets[4];
    naviTextures[4] = GetCommonIconTextureHandle(0x3b);
    rectangle = GetCommonIconRect(0x3b);
    packet->uv[0] = static_cast<float>(rectangle[0]) * 0.125f;
    packet->uv[1] = static_cast<float>(rectangle[1]) * 0.125f;
    packet->uv[2] = static_cast<float>(rectangle[2]) * 0.125f;
    packet->uv[3] = static_cast<float>(rectangle[3]) * 0.125f;
    packet = &naviPackets[2];
    naviTextures[2] = GetCommonIconTextureHandle(0x3c);
    rectangle = GetCommonIconRect(0x3c);
    packet->uv[0] = static_cast<float>(rectangle[0]) * 0.125f;
    packet->uv[1] = static_cast<float>(rectangle[1]) * 0.125f;
    packet->uv[2] = static_cast<float>(rectangle[2]) * 0.125f;
    packet->uv[3] = static_cast<float>(rectangle[3]) * 0.125f;
    packet = &naviPackets[3];
    naviTextures[3] = GetCommonIconTextureHandle(0x3d);
    rectangle = GetCommonIconRect(0x3d);
    packet->uv[0] = static_cast<float>(rectangle[0]) * 0.125f;
    packet->uv[1] = static_cast<float>(rectangle[1]) * 0.125f;
    packet->uv[2] = static_cast<float>(rectangle[2]) * 0.125f;
    packet->uv[3] = static_cast<float>(rectangle[3]) * 0.125f;
    naviBackgroundTexture = -1;
    INIT_NAVI_PACKET(&naviOverlayPacket);
    (&naviOverlayPacket)->quad.flags |= 0x400U;
    do {
        (&naviOverlayPacket)->colour = 0x80ffffffU;
    } while (0);
    INIT_NAVI_PACKET(&naviTrailPacket);
    (&naviTrailPacket)->quad.flags |= 0x400U;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
bool NaviIsDisplayable(bool skipMapItemRequirement) {
    if (naviFlags & 1U)
        return true;
    if (drGetCurrentAuthorId())
        return false;
    if (scenFindMapRegionName(0) == SHEN_NAME(ARK0))
        return false;
    if (scenFindMapRegionName(0) == SHEN_NAME(KESS))
        return false;
    if (GetShouldRenderIcons(ItemSelect))
        return false;
    if (IsScenarioBusyForMainIcon())
        return false;
    if (IsItemSelectable())
        return false;
    if (g_fightUiActive)
        return false;
    if (!IsPlayerControlEnabled())
        return false;
    const std::int32_t config = navGetNumEntriesFor(g_currentMap);
    if (!skipMapItemRequirement && config >= 0 && nativeNaviMapConfigs[config].itemId != -1 && !GetItemState(nativeNaviMapConfigs[config].itemId))
        return false;
    if (!IsScreenFilmNotShowing(reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask) &&
        !DoesScreenFilmColourExist(reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask))
        return false;
    if (authGetLoaded())
        return false;
    if (g_moveUiClockAndNavigationHold)
        return false;
    return true;
}

#else

bool NaviIsDisplayable(bool skipMapItemRequirement) {
    if (IsNaviDisplayOverridden())
        return true;
    if (GetCurrentAuthorId().name != 0 ||
        g_currentMap.name ==
            SHEN_NAME(ARK0) ||
                g_currentMap.name ==
                    SHEN_NAME(KESS) || GetShouldRenderIcons(RenderIconSlot::ItemSelect) || IsScenarioBusyForMainIcon() || IsItemSelectable() ||
                        g_fightUiActive != 0 || !plymIsSearchStartRequested()) {
        return false;
    }
    const std::int32_t configIndex = navGetNumEntriesFor(g_currentMap);
    const std::span<const NaviMapConfig> configs = GetNaviMapConfigs();
    if (!skipMapItemRequirement && configIndex >= 0 && static_cast<std::size_t>(configIndex) < configs.size()) {
        const std::int16_t itemId = configs[static_cast<std::size_t>(configIndex)].itemId;
        if (itemId != -1 && GetItemState(static_cast<std::uint16_t>(itemId)) == 0) {
            return false;
        }
    }
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control && IsScreenFilmNotShowing(control->screenFilmTask) && authGetLoaded() == 0 && g_moveUiClockAndNavigationHold == 0;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

const char *GetNaviFolderLabel(std::int32_t page) {
    return naviFolderPages[static_cast<std::size_t>(page)].label;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

const char *GetNaviFolderEntryLabel(std::int32_t page, std::int32_t entry) {
    return naviFolderPages[static_cast<std::size_t>(page)].entries[entry].label;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
HLib::Task *g_naviTask;
std::uint32_t naviFlags;
std::uint8_t *g_naviTrailTexturePixels = 0;
std::uint32_t g_savedNaviMarkers[128];
std::int32_t g_savedNaviMarkerCursor = 0;

static void ClearSavedNaviReloadState() {
    naviRetainedMap = 0;
}

static void ClearSavedNaviMarkers() {
    std::memset(g_savedNaviMarkers, 0, sizeof(g_savedNaviMarkers));
    g_savedNaviMarkerCursor = 0;
}

// @verified - d
void ResetNaviSavedData() {
    ClearSavedNaviMarkers();
    ClearSavedNaviReloadState();
}

// @verified - d
void SetNaviTaskEnabled(std::int32_t enabled) {
    if (g_naviTask)
        HLib::GetTaskParameter(g_naviTask);
    if (!g_naviTask)
        return;
    if (enabled) {
        HLib::ResolveDeferredCallback(g_naviTask);
        naviFlags |= 0x20U;
    } else {
        HLib::SetupTimedTask(g_naviTask, 0);
        naviFlags &= ~0x20U;
    }
}

std::int32_t IsNaviTaskEnabled() {
    void *data;
    if (g_naviTask)
        data = HLib::GetTaskParameter(g_naviTask);
    else
        data = 0;
    if (data && (naviFlags & 0x20U))
        return 1;
    return 0;
}

static std::uint32_t NaviTwiddleIndex(std::uint32_t x, std::uint32_t y) {
    std::uint32_t offset = 0, mask = 1;
    for (std::int32_t bit = 0; bit < 31; ++bit, mask <<= 1U) {
        std::uint32_t value = x & mask;
        if (value)
            offset += value * value * 2U;
        value = y & mask;
        if (value)
            offset += value * value;
    }
    return offset;
}

// @verified - d
void SetNaviTrailPixel(std::uint32_t x, std::uint32_t y, std::int32_t colour) {
    const std::int32_t byte = static_cast<std::int32_t>(NaviTwiddleIndex(x, y)) >> 1;
    if (static_cast<std::int32_t>(x) >= 0 && static_cast<std::int32_t>(y) >= 0 && g_naviTrailTexturePixels && byte < 0x8000) {
        std::uint8_t *pixel = &g_naviTrailTexturePixels[byte];
        const std::uint8_t value = y & 1U ? static_cast<std::uint8_t>((static_cast<std::uint32_t>(colour) << 4U) | (*pixel & 0xfU)) :
                                            static_cast<std::uint8_t>(colour | (*pixel & 0xf0U));
        *pixel = value;
        pixel = reinterpret_cast<std::uint8_t *>(eventExecutionRegions[2] + 0x782c);
        pixel[byte] = value;
    }
}

// @verified - d
void PlotNaviTrailLocation(const Vector3 *location, const NativeNaviMapConfig *config) {
    float x = location->x - config->originX;
    float y = -(location->z - config->originZ);
    const float scale = 256.0f / (config->scale * 128.0f);
#if SHENMUE_S2_GCC295
    x = x * scale + 128.0f;
    y = y * scale + 128.0f;
#else
    x = __builtin_fmaf(x, scale, 128.0f);
    y = __builtin_fmaf(y, scale, 128.0f);
#endif
    SetNaviTrailPixel(static_cast<std::uint8_t>(static_cast<std::int32_t>(x)), static_cast<std::uint8_t>(static_cast<std::int32_t>(y)), 0);
}

void SetNaviTrailStamp(std::int32_t stamp) {
    naviTrailStamp = stamp & 15;
    naviFlags |= 8U;
}

// @verified - d
void AddNaviTrailLocations(const Vector3 *locations, std::int32_t count) {
    void *data;
    if (g_naviTask)
        data = HLib::GetTaskParameter(g_naviTask);
    else
        data = 0;
    if (!data || !(naviFlags & 2U))
        return;
    for (std::int32_t index = 0; index < count; ++index)
        PlotNaviTrailLocation(locations++, *static_cast<NativeNaviMapConfig **>(data));
}
#else
static std::uint32_t naviFlags;
#endif

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void BeginNaviClipping(const NaviTaskData &task) {
    dc::sdk::SetScreenUserClipMode(2);
    dc::sdk::SetVertexListClipMode(0x16, 0);
    dc::sdk::SetVertexListClipping(3 - task.clipColumns, 12 - task.clipRows, 5, 14, 0x16);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void EndNaviClipping(const NaviTaskData &task) {
    dc::sdk::SetVertexListClipping(3 - task.clipColumns, 12 - task.clipRows, 5, 14, 0x16);
    dc::sdk::SetScreenUserClipMode(0);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
bool NaviFloorMaskIncludes(const std::uint64_t *mask, std::int32_t floor) {
    if (floor < -9)
        return false;
    floor += 9;
    const std::uint32_t bit = 1U << (floor & 31);
    if (reinterpret_cast<const std::uint32_t *>(mask)[floor / 32] & bit)
        return true;
    return false;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void DrawNaviFloorImages(const Matrix4x4 &transform, const NaviTaskData &task, float offsetX, float offsetZ, float scale, std::int32_t clipped) {
    Vector3 point;
    std::int32_t index;
    const NativeNaviFloorDefinition *definition = static_cast<const NativeNaviFloorDefinition *>(task.definition);
    if (!definition)
        return;
    const NativeNaviFloorImage *images = definition->images;
    for (index = 0; index < definition->imageCount; ++index, ++images) {

        if (!NaviFloorMaskIncludes(&images->floorMask, task.stream))
            continue;
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&transform);
        float x = images->x, z = images->z;
        Matrix4x4::preTranslate(x * scale + offsetX, 0.0f, z * scale + offsetZ);
        for (std::int32_t corner = 0; corner < 4; ++corner) {
            point.x = ((nativeNaviMapCorners[corner].x * static_cast<float>(images->width)) * 0.5f) * scale;
            point.z = ((nativeNaviMapCorners[corner].y * static_cast<float>(images->width)) * 0.5f) * scale;
            point.y = 0.0f;
            Matrix4x4::TransformPoint(point, point);
            naviTrailPacket.quad.points[corner].x = point.x;
            naviTrailPacket.quad.points[corner].y = point.z;
        }
        PopCurrentMatrix(1);
        const std::int32_t texture = naviFloorTextures[images->textureIndex];
        if (texture >= 0) {
            DrawLegacySpritePacket(naviTrailPacket, texture, static_cast<float>(index) * 0.01f + 1.8f);
            if (clipped)
                dc::sdk::SetVertexListClipMode(0x16, 2);
        }
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void StampNaviTrail(std::int32_t x, std::int32_t y, std::uint8_t colour, std::int32_t pattern) {
    const std::int32_t left = x - 3;
    const std::int32_t top = y - 3;
    for (std::int32_t row = 0; row < 8; ++row)
        for (std::int32_t column = 0; column < 8; ++column)
            if ((nativeNaviStampPatterns[pattern][row] >> column) & 1U)
                SetNaviTrailPixel(left + column, top + row, colour);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

bool IsCurrentAreaQA11();

namespace {

// @verified - d
void DrawNaviFloorNumber(std::int32_t floor) {
    LegacyIconSprite *sprite = &naviFloorLabelSprite;
    if (!NaviIsDisplayable(true))
        return;
    sprite->x = 52.0f;
    sprite->y = 398.0f;
    if (floor == -1000)
        return;
    if (IsCurrentAreaQA11()) {
        sprite->icon = 0x417;
        PrepareDrawSimple(*sprite, 0xff656565U);
        return;
    }
    if (floor <= 0)
        --floor;
    DrawMoneyDigits(36.0f, 398.0f, 1.0f, 3.0f, floor < 0 ? -floor : floor, 0xff656565U);
    sprite->icon = 0x419;
    PrepareDrawSimple(*sprite, 0xff656565U);
    if (floor <= 0) {
        sprite->x = (floor < 0 ? -floor : floor) > 9 ? 4.0f : 20.0f;
        sprite->icon = 0x413;
        PrepareDrawSimple(*sprite, 0xff656565U);
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void PrepareNaviFrame(void *data) {
    NaviTaskData *task = static_cast<NaviTaskData *>(data);
    Vector3 position;
    Matrix4x4 transform;
    Vector3 point, markerPoint, companionPoint, companionPosition;
    LegacySpritePacket packet;
    float centerX, centerY, zoom;
    std::int32_t drawMap;
    if (!task)
        return;
    zoom = static_cast<float>(task->trigger) / 255.0f;
    zoom = naviTriggerBlend = InterpolateTo(zoom, naviTriggerBlend, 0.6f);
    centerX = CameraTransitionValue(96.0f, 320.0f, zoom);
    centerY = CameraTransitionValue(384.0f, 240.0f, zoom);
    zoom = CameraTransitionValue(1.0f, 3.0f, zoom);
    task->stream = GetQronNextStream();
    DrawNaviFloorNumber(task->stream);
    std::int32_t cameraYaw = camCtrlGetNextRotY();
    plymUpdatePlayerSearchLocation(&position);
    naviTiltAngle = 0x31c7;
    std::int32_t rows = 3;
    Vector2 origin = *reinterpret_cast<const Vector2 *>(&task->config->originX);
    float scale = task->config->scale;
    float mapScale = 1.25f;
    if (naviFlags & 4U) {
        mapScale = 2.25f;
        rows = 4;
        naviTiltAngle = 0;
    }
    float offsetX = -position.x + origin.x;
    float offsetZ = -position.z + origin.y;
    if (naviFlags & 2U) {
        naviTiltAngle = 0;
        mapScale = 256.0f / (scale * 64.0f);
        float trailX = position.x - origin.x;
        float trailY = -(position.z - origin.y);
        float texelScale = 256.0f / (scale * 128.0f);
        trailX = trailX * texelScale + 128.0f;
        trailY = trailY * texelScale + 128.0f;
        std::uint32_t x = static_cast<std::int32_t>(trailX), y = static_cast<std::int32_t>(trailY);
        x &= 0xff;
        y &= 0xff;
        float delta = task->position.x - position.x;
        float distance = delta * delta;
        delta = task->position.z - position.z;
        distance += delta * delta;
        distance = shenmue::NativeSqrt(distance);
        if (naviFlags & 8U) {
            StampNaviTrail(x, y, naviTrailStamp, naviTrailStamp - 2);
            naviFlags &= ~8U;
        }
        float threshold = 16.0f;
        if (navGetNumEntriesFor(g_currentMap) == 16)
            threshold = 2.5f;
        if (distance > threshold) {
            task->position = position;
            SetNaviTrailPixel(x, y, 0);
        }
    }
    mapScale *= zoom;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(centerX, 0.0f, centerY);
    Matrix4x4::RotateAroundX_Wrapper(naviTiltAngle);
    Matrix4x4::RotateAroundY_Wrapper(-cameraYaw);
    Matrix4x4::Scale(mapScale, mapScale, mapScale);
    matrix_stack::GetMatrix(&transform);
    for (std::int32_t corner = 0; corner < 4; ++corner) {
        point.x = (nativeNaviMapCorners[corner].x * 64.0f) * scale + offsetX;
        point.z = (nativeNaviMapCorners[corner].y * 64.0f) * scale + offsetZ;
        point.y = 0.0f;
        Matrix4x4::TransformPoint(point, point);
        naviPackets[0].quad.points[corner].x = point.x;
        naviPackets[0].quad.points[corner].y = point.z;
    }
    PopCurrentMatrix(1);
    std::int32_t size = static_cast<std::int32_t>(16.0f * zoom);
    std::int32_t x, y;
    AdjustScreenAspectRatio(
        naviPackets[1].quad, static_cast<ScreenAlignment>(5), x = static_cast<std::int32_t>(centerX), y = static_cast<std::int32_t>(centerY), size, size);
    if (!NaviIsDisplayable(false) || naviTextures[0] == -1)
        return;
    drawMap = 1;
    if (naviFlags & 2U)
        DrawNaviBackground(x, y, zoom);
    naviPackets[0].colour = 0xa0ffffffU;
    naviTrailPacket.colour = 0xa0ffffffU;
    std::byte *clipSettings = reinterpret_cast<std::byte *>(task) + offsetof(NaviTaskData, clipColumns);
    *reinterpret_cast<std::int16_t *>(clipSettings) = 3;
    clipSettings += offsetof(NaviTaskData, clipRows) - offsetof(NaviTaskData, clipColumns);
    *reinterpret_cast<std::int16_t *>(clipSettings) = rows;
    clipSettings += offsetof(NaviTaskData, trigger) - offsetof(NaviTaskData, clipRows);
    if (*reinterpret_cast<const std::uint8_t *>(clipSettings) == 0)
        BeginNaviClipping(*task);
    if (naviFlags & 0x10U) {
        const NativeNaviFloorDefinition *definition = static_cast<const NativeNaviFloorDefinition *>(task->definition);
        drawMap = NaviFloorMaskIncludes(&definition->floorMask, task->stream);
        DrawNaviFloorImages(transform, *task, offsetX, offsetZ, scale, !task->trigger);
    }
    if (drawMap) {
        DrawLegacySpritePacket(naviPackets[0], naviTextures[0], 1.7f);
        if (!task->trigger)
            dc::sdk::SetVertexListClipMode(0x16, 2);
    }
    std::int32_t drawn = 0;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&transform);
    std::int32_t floor = task->stream;
    if (floor == -1000)
        floor = 63;
    float depth = 1.9f;
    float texelScale = 256.0f / (scale * 128.0f);
    float unitsX = 1.0f / texelScale;
    float unitsZ = 1.0f / texelScale;
    size = static_cast<std::int32_t>(12.0f * zoom);
    for (std::int32_t index = 0; index < 128; ++index) {
        std::int32_t slot = (g_savedNaviMarkerCursor + index) & 127;
        const std::int8_t *coordinates = reinterpret_cast<const std::int8_t *>(&g_savedNaviMarkers[slot]);
        std::uint32_t marker = g_savedNaviMarkers[slot];
        if ((marker & 1U) && ((marker >> 1) & 7U) && static_cast<std::int32_t>((marker >> 10) & 63U) == floor) {
            float markerX = coordinates[2], markerZ = coordinates[3];
            markerPoint.x = markerX * unitsX + offsetX;
            markerPoint.y = 0.0f;
            markerPoint.z = markerZ * unitsZ + offsetZ;
            Matrix4x4::TransformPoint(markerPoint, markerPoint);
            std::uint32_t type = (g_savedNaviMarkers[slot] >> 1) & 7U;
            LegacySpritePacket &markerPacket = naviPackets[type + 1];
            std::int32_t texture = naviTextures[type + 1];
            depth += 0.00078125f;
            AdjustScreenAspectRatio(markerPacket.quad, static_cast<ScreenAlignment>(5), static_cast<std::int32_t>(markerPoint.x),
                static_cast<std::int32_t>(markerPoint.z), size, size);
            DrawLegacySpritePacket(markerPacket, texture, depth);
            drawn = 1;
        }
    }
    const shen_name companionName = FindNavigationPartyCompanion(shen_name(0));
    if (companionName) {
        GetCharacterWorldPosition(ctrlGetScenarioChara(companionName), &companionPosition);
        companionPoint.x = (companionPosition.x - origin.x) + offsetX;
        companionPoint.y = 0.0f;
        companionPoint.z = (companionPosition.z - origin.y) + offsetZ;
        size = static_cast<std::int32_t>(10.0f * zoom);
        Matrix4x4::TransformPoint(companionPoint, companionPoint);
        packet = naviPackets[1];
        std::int32_t texture = naviTextures[1];
        AdjustScreenAspectRatio(
            packet.quad, static_cast<ScreenAlignment>(5), static_cast<std::int32_t>(companionPoint.x), static_cast<std::int32_t>(companionPoint.z), size, size);
        DrawLegacySpritePacket(packet, texture, 2.0f);
        drawn = 1;
    }
    PopCurrentMatrix(1);
    if (drawn && !task->trigger)
        dc::sdk::SetVertexListClipMode(0x16, 2);
    if (!task->trigger)
        EndNaviClipping(*task);
    if (naviTextures[1] != -1 && task->animation <= 21)
        DrawLegacySpritePacket(naviPackets[1], naviTextures[1], 2.0f);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void NAVI_DummyWait_Callback(void *data) {
    NaviTaskData &task = *static_cast<NaviTaskData *>(data);
    plymUpdatePlayerSearchLocation(&task.position);
    naviPhase = 0;
    naviTriggerBlend = 0.0f;
    task.stream = GetQronNextStream();
    HLib::SetCurrentTaskTo(NAVI_CtrlNavi_Callback);
}

#else

namespace {

void NAVI_DummyWait_Callback(void *taskData) {
    NaviTaskData *const data = static_cast<NaviTaskData *>(taskData);
    if (!data)
        return;
    InitializeNaviInputState(*data);
    HLib::SetCurrentTaskTo(NAVI_CtrlNavi_Callback);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void NAVI_CtrlNavi_Callback(void *data) {
    NaviTaskData &task = *static_cast<NaviTaskData *>(data);
    const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(GetMapPadControlEntry());
    if (input && g_naviTask && HLib::GetTaskParameter(g_naviTask) && NaviIsDisplayable(false) && plymIsSearchable() && !plymIsSearchBlocked() &&
        !IsScenarioBusy() && !g_fightUiActive && !srchCheckFlags(6) && srchCheckFlags(3) < 0) {
        switch (naviPhase) {
        case 0:
            if (btnCheck(SHEN_NAME(BTNV)))
                ++naviPhase;
            else
                RegisterButtonMediation(SHEN_NAME(BTNV), 0, 0x20000);
            break;
        case 1:
            if (!(input->held & 0x200U))
                ++naviPhase;
            else if ((input->held & 0xf0c0U) || input->triggers[0] || input->triggers[1] || input->leftStickX != 0x80 || input->leftStickY != 0x80)
                naviPhase = 3;
            break;
        case 2:
            if ((input->held & 0xf0c0U) || input->triggers[0] || input->triggers[1] || input->leftStickX != 0x80 || input->leftStickY != 0x80) {
                ++naviPhase;
            } else {
                const std::uint32_t buttons = btnCheck(SHEN_NAME(BTMK));
                if (buttons) {
                    std::int32_t type = buttons & 0x400 ? 2 : 0;
                    if (buttons & 4U)
                        type = 3;
                    if (buttons & 2U)
                        type = 4;
                    if (type)
                        navCreate(*plymUpdatePlayerSearchLocation(0), type);
                    ++naviPhase;
                } else {
                    RegisterButtonMediation(SHEN_NAME(BTMK), 0, 0xf0008);
                }
            }
            break;
        case 3:
            if (!(input->held & 0x200U))
                naviPhase = 0;
            break;
        default:
            naviPhase = 0;
            break;
        }
    } else {
        naviPhase = 0;
    }
    task.trigger = 0;
    if (input->held & 0x200U) {
        if (GetNormalTriggerMapping())
            task.trigger = input->triggers[0];
        else
            task.trigger = input->triggers[1];
    }
    if (++task.animation > 27)
        task.animation = 0;
    HLib::ExecuteCallbackNow(PrepareNaviFrame, &task);
}

#else

namespace {

void NAVI_CtrlNavi_Callback(void *taskData) {
    NaviTaskData *const data = static_cast<NaviTaskData *>(taskData);
    if (!data)
        return;

    const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(ctrlGetPadData());
    const bool admitted = input && navigationTask && HLib::GetTaskParameter(navigationTask) && NaviIsDisplayable(false) && plymIsSearchable() &&
        !plymIsSearchBlocked() && !IsScenarioBusy() && g_fightUiActive == 0 && srchCheckFlags(6) == 0 && srchCheckFlags(3) < 0;
    if (admitted)
        UpdateNaviInteraction(*input);
    else
        ResetNaviInputPhase();

    UpdateNaviInputAnimation(*data, *input);
    HLib::ExecuteCallbackNow(PrepareNaviFrame, data);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

static std::int32_t IsQronNaviArea(shen_name area);

static void ReleaseNaviMapTexture();

// @verified - d
void NAVI_DummyWait_Enqueue(std::int8_t queue) {
    InitializeNaviIcons();
    g_naviTask = 0;
    const std::int32_t config = navGetNumEntriesFor(g_currentMap);
    if (config >= 0) {
        g_naviTask = HLib::EnqueueTaskWithParameter(NAVI_DummyWait_Callback, queue, 0, sizeof(NaviTaskData), SHEN_NAME(NAVI));
        if (g_naviTask) {
            NaviTaskData *const data = static_cast<NaviTaskData *>(HLib::GetTaskParameter(g_naviTask));
            if (data) {
                data->config = nativeNaviMapConfigs + config;
                data->definition = 0;
                data->retained04 = 0;
                data->trigger = 0;
                data->animation = 0;
                g_naviTrailTexturePixels = 0;
                naviFlags = 0x20;
                if (!ConstructNaviMap(config)) {
                    ReleaseNaviMapTexture();
                    naviFlags &= ~0x20U;
                }
                if (IsQronNaviArea(g_currentMap))
                    naviFlags |= 4U;
            }
        }
    }
    SelectNaviMapMarkers();
}

static std::int32_t IsQronNaviArea(shen_name area) {
    const std::uint32_t name = area;
    if (static_cast<std::int8_t>(name) == 'Q') {
        if (static_cast<std::uint8_t>((static_cast<std::int16_t>(name) >> 8) - 'A') <= 5 || name == SHEN_NAME4('Q', 'J', 'B', 'T'))
            return 1;
        if (name == SHEN_NAME4('Q', 'L', 'B', 'T'))
            return 1;
        return 0;
    }
    return 0;
}

static void ReleaseNaviMapTexture() {
    if (naviTextures[0] != -1)
        ReleaseNamedTexture_Wrapper(naviTextures[0]);
    naviTextures[0] = -1;
}

#else

HLib::Task *NAVI_DummyWait_Enqueue(std::uint8_t nextFunctionIndex) {
    navigationTask = nullptr;

    const std::int32_t configIndex = navGetNumEntriesFor(g_currentMap);
    if (configIndex < 0) {
        SelectNaviMapMarkers(g_currentMap);
        return nullptr;
    }

    HLib::Task *const task = HLib::EnqueueTaskWithParameter(NAVI_DummyWait_Callback, nextFunctionIndex, 0, sizeof(NaviTaskData), naviTaskName);
    navigationTask = task;
    if (!task) {
        SelectNaviMapMarkers(g_currentMap);
        return nullptr;
    }
    InitializeNaviResources(task, configIndex);
    return task;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static void InitializeNaviWindowSprites() {
    naviWindowHeadingSprite.icon = 1553;
    naviWindowHeadingSprite.width = 160;
    naviWindowHeadingSprite.height = 3;
    naviWindowHeadingSprite.scaleY = naviWindowHeadingSprite.scaleX = 1.0f;
    naviWindowHeadingSprite.alignment = 4;
    naviWindowHeadingSprite.depth = 0.0f;
    naviWindowHeadingSprite.flags = 0;
    naviWindowHeadingSprite.rotation = 0;
    naviWindowHeadingSprite.x = 0.0f;
    naviWindowHeadingSprite.y = 0.0f;
    naviWindowSelectionSprite.icon = 771;
    naviWindowSelectionSprite.scaleX = naviWindowSelectionSprite.scaleY = 1.0f;
    naviWindowSelectionSprite.width = naviWindowSelectionSprite.height = 32;
    naviWindowSelectionSprite.depth = 6.0f;
    naviWindowSelectionSprite.rotation = 0;
    naviWindowSelectionSprite.flags = 0x400;
    naviWindowSelectionSprite.alignment = 5;
    naviWindowPosition.x = -300.0f;
    naviWindowPosition.y = 220.0f;
    naviWindowPosition.velocity = 1.0f;
    naviWindowPosition.reservedScale = 0.0f;
    naviWindowPosition.depth = 3.0f;
    for (std::int32_t index = 0; index < 6; ++index) {
        naviWindowLabels[index] = naviWindowSecondaryLabels[index] = -1;
        naviWindowLabelTextures[index] = naviWindowSecondaryTextures[index] = -1;
    }
    naviWindowPreviewTexture = -1;
}

#else

namespace {

void InitializeNaviWindowSprites() {
    naviWindowPosition = NWIN_POSITION_INITIAL;
    naviWindowVelocity = 0.1f;
    naviWindowHeadingSprite.x = naviWindowHeadingSprite.y = 0;
    naviWindowHeadingSprite.scaleX = naviWindowHeadingSprite.scaleY = 1;
    naviWindowHeadingSprite.depth = 0;
    naviWindowHeadingSprite.flags = 0;
    naviWindowHeadingSprite.rotation = 0;
    naviWindowHeadingSprite.icon = 1553;
    naviWindowHeadingSprite.width = 160;
    naviWindowHeadingSprite.height = 3;
    naviWindowHeadingSprite.alignment = 4;
    naviWindowSelectionSprite.scaleX = naviWindowSelectionSprite.scaleY = 1;
    naviWindowSelectionSprite.depth = 3.1f;
    naviWindowSelectionSprite.flags = 1024;
    naviWindowSelectionSprite.rotation = 0;
    naviWindowSelectionSprite.icon = 771;
    naviWindowSelectionSprite.width = naviWindowSelectionSprite.height = 32;
    naviWindowSelectionSprite.alignment = 5;
    for (std::int32_t index = 0; index < 6; ++index)
        naviWindowLabelTextures[index] = naviWindowSecondaryTextures[index] = naviWindowLabels[index] = naviWindowSecondaryLabels[index] = -1;
    naviWindowPreviewTexture = -1;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

static void ResetNaviWindowState(NativeNaviWindowState *work) {
    work->phase = 0;
    work->requestedPage = -1;
    work->page = 0;
    work->task = 0;
    work->drawFlags = 0;
    work->mapImageData = 0;
    work->labelScratch = 0;
    work->dummySpriteData = 0;
    work->availableEntries = 0;
    work->availablePages = 0;
    InitializeNaviWindowSprites();
}

HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::int8_t queue) {
    NativeNaviWindowState *const work = naviWindowWork;
    ResetNaviWindowState(work);
    work->task = HLib::EnqueueTaskWithoutParameter(NVW_CtrlNaviWindow_Callback, queue, 0, SHEN_NAME(NWIN));
    if (work->task)
        HLib::SetupTimedTask(work->task, 0);
    return work->task;
}

#else

#if SHENMUE_S2_GCC295
HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::int8_t nextFunctionIndex) {
#else
HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::uint8_t nextFunctionIndex) {
#endif
    naviWindow.phase = naviWindow.page = 0;
    naviWindow.requestedPage = -1;
    naviWindow.task = nullptr;
    naviWindow.drawFlags = 0;
    naviWindow.mapImageData = naviWindow.labelScratch = naviWindow.dummySpriteData = nullptr;
    naviWindow.availablePages = naviWindow.availableEntries = 0;
    InitializeNaviWindowSprites();
    naviWindow.task = HLib::EnqueueTaskWithoutParameter(NVW_CtrlNaviWindow_Callback, nextFunctionIndex, 0, TASK_NWIN.name);
    if (naviWindow.task)
        HLib::SetupTimedTask(naviWindow.task, 0);
    return naviWindow.task;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
bool IsCurrentMapNaviFolderAvailable() {
    const char area = static_cast<char>(g_currentMap.name & 0xffU);
    std::int32_t page = 0;
    while (static_cast<char>(naviFolderPages[page].area.name & 0xffU) != area)
        if (++page >= 3)
            return false;
    return NaviFolderEntryMask(page, 6) != 0 && NaviFolderEntryMask(page, 4) != 0;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
const NativeNaviFolderPage *navGetFolderPage(std::uint32_t page) {
    const NativeNaviFolderPage *result;
    if (page > 2)
        result = 0;
    else
        result = nativeNaviFolderPages + page;
    return result;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
const NativeNaviFolderEntry *navGetFolderEntries(std::uint32_t page) {
    const NativeNaviFolderEntry *result;
    if (page > 2)
        result = 0;
    else
        result = nativeNaviFolderPages[page].entries;
    return result;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t NaviFolderEntryCount(std::uint32_t page, std::uint16_t excluded) {
    std::int32_t count = 0;
    if (page < 3) {
        const NativeNaviFolderEntry *entry = nativeNaviFolderPages[page].entries;
        const std::int32_t total = nativeNaviFolderCounts[page];
        for (std::int32_t index = 0; index < total; ++index, ++entry)
            if (!(entry->flags & excluded))
                ++count;
    }
    return count;
}

#else

std::int32_t NaviFolderEntryCount(std::int32_t page, std::uint16_t excludedFlags) {
    if (page >= 3)
        return 0;
    std::int32_t count = 0;
    for (std::int32_t index = 0; index < NWIN_FOLDER_COUNTS[page]; ++index) {
        if (!(naviFolderPages[page].entries[index].flags & excludedFlags))
            ++count;
    }
    return count;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::uint32_t NaviFolderEntryMask(std::uint32_t page, std::uint16_t excluded) {
    std::uint32_t mask = 0;
    const NativeNaviFolderEntry *entry = navGetFolderEntries(page);
    if (entry) {
        const std::int32_t count = nativeNaviFolderCounts[page];
        for (std::int32_t index = 0; index < count; ++index, ++entry) {
            if (entry->flags & excluded)
                continue;
            if (entry->itemId < 0)
                mask |= 1U << index;
            else if (GetItemState(entry->itemId))
                mask |= 1U << index;
        }
    }
    return mask;
}

#else

std::uint32_t NaviFolderEntryMask(std::int32_t page, std::uint16_t excludedFlags) {
    if (page >= 3)
        return 0;
    const NaviFolderPage &folder = naviFolderPages[page];
    if (!folder.entries)
        return 0;
    std::uint32_t mask = 0, bit = 1;
    for (std::int32_t index = 0; index < NWIN_FOLDER_COUNTS[page]; ++index) {
        const NaviFolderEntry &entry = folder.entries[index];
        if (!(entry.flags & excludedFlags) && (entry.itemId < 0 || GetItemState(static_cast<std::uint16_t>(entry.itemId))))
            mask |= bit;
        bit = (bit << 1U) | (bit >> 31U);
    }
    return mask;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void *LoadNaviFolderSprite(std::uint32_t page) {
    const NativeNaviFolderPage *const folder = navGetFolderPage(page);
    return folder ? shenLoadFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), folder->sprite, SHEN_NAME(NVST)) : 0;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

static void ReleaseNaviWindowStrips(std::uint32_t flags) {
    if ((flags & 3U) == 0)
        return;
    for (std::int32_t index = 0; index < 6; ++index) {
        if (flags & 1U) {
            const std::int32_t label = naviWindowLabelTextures[index];
            if (label >= 0)
                ReleaseNamedTexture_Wrapper(label);
            const std::int32_t secondary = naviWindowSecondaryTextures[index];
            if (secondary >= 0)
                ReleaseNamedTexture_Wrapper(secondary);
            naviWindowLabelTextures[index] = naviWindowSecondaryTextures[index] = -1;
        }
        if (flags & 2U)
            naviWindowLabels[index] = naviWindowSecondaryLabels[index] = -1;
    }
}

#else

namespace {

void ReleaseNaviWindowStrips(std::uint8_t flags) {
    if (!(flags & 3U))
        return;
    for (std::int32_t index = 0; index < 6; ++index) {
        if (flags & 1U) {
            if (naviWindowLabelTextures[index] >= 0)
                ReleaseNamedTexture(naviWindowLabelTextures[index]);
            if (naviWindowSecondaryTextures[index] >= 0)
                ReleaseNamedTexture(naviWindowSecondaryTextures[index]);
            naviWindowSecondaryTextures[index] = naviWindowLabelTextures[index] = -1;
        }
        if (flags & 2U)
            naviWindowSecondaryLabels[index] = naviWindowLabels[index] = -1;
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static void ReleaseNaviWindowAllocations(NativeNaviWindowState *work) {
    if (work->mapImageData)
        HLib::FreeTaskData(work->mapImageData);
    if (work->labelScratch)
        HLib::FreeTaskData(work->labelScratch);
    if (work->dummySpriteData)
        HLib::FreeTaskData(work->dummySpriteData);
    work->labelScratch = work->mapImageData = work->dummySpriteData = 0;
}

#else

namespace {

void ReleaseNaviWindowAllocations() {
    if (naviWindow.mapImageData)
        HLib::FreeTaskData(naviWindow.mapImageData);
    if (naviWindow.labelScratch)
        HLib::FreeTaskData(naviWindow.labelScratch);
    if (naviWindow.dummySpriteData)
        HLib::FreeTaskData(naviWindow.dummySpriteData);
    naviWindow.dummySpriteData = naviWindow.mapImageData = naviWindow.labelScratch = nullptr;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static void PublishNaviWindowLabel(NativeNaviWindowState &work, const char *label, const char *secondary, std::int32_t slot) {
    if (label) {
        RasterizeWindowTextLine4bpp(static_cast<std::byte *>(work.labelScratch) + 0x2c, 256, label, 1, 0);
        ReplacePtxTexture(naviWindowLabelTextures[slot], static_cast<TextureChunk *>(work.labelScratch));
        naviWindowLabels[slot] = naviWindowLabelTextures[slot];
    }
    if (secondary) {
        RasterizeWindowTextLine4bpp(static_cast<std::byte *>(work.labelScratch) + 0x2c, 256, secondary, 0, 0);
        ReplacePtxTexture(naviWindowSecondaryTextures[slot], static_cast<TextureChunk *>(work.labelScratch));
        naviWindowSecondaryLabels[slot] = naviWindowSecondaryTextures[slot];
    }
}

#else

namespace {

void PublishNaviWindowLabel(NaviWindowState &work, const char *label, const char *secondary, std::int32_t slot) {
    if (label) {
        (void)RasterizeItemLabelNullsub(static_cast<std::byte *>(work.labelScratch), 256, label, 0, 0);
        (void)ReplacePtxTexture(naviWindowLabelTextures[slot], work.labelScratch);
        naviWindowLabels[slot] = naviWindowLabelTextures[slot];
    }
    if (secondary) {
        (void)RasterizeItemLabelNullsub(static_cast<std::byte *>(work.labelScratch), 256, secondary, 0, 0);
        (void)ReplacePtxTexture(naviWindowSecondaryTextures[slot], work.labelScratch);
        naviWindowSecondaryLabels[slot] = naviWindowSecondaryTextures[slot];
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static bool BuildNaviWindowHeading(std::int32_t slot, std::int32_t category) {
    bool available = false;
    NativeNaviWindowState *const work = naviWindowWork;
    const char *label = "\241\251\241\251\241\251";
    const char *secondary = "";
    if (NaviFolderEntryMask(category, 2)) {
        const NativeNaviFolderPage *const folder = navGetFolderPage(category);
        label = folder->label;
        secondary = folder->secondaryLabel;
        available = true;
    }
    PublishNaviWindowLabel(*work, label, secondary, slot);
    return available;
}

#else

namespace {

std::uint32_t BuildNaviWindowHeading(std::int32_t slot, std::int32_t category) {
    const bool available = NaviFolderEntryMask(category) != 0;
    const NaviFolderPage &folder = naviFolderPages[category];
    PublishNaviWindowLabel(naviWindow, available ? folder.label : "? ? ? ", available ? folder.secondaryLabel : "", slot);
    return available;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static void BuildNaviWindowRootLabels(NativeNaviWindowState &work) {
    ReleaseNaviWindowStrips(2);
    for (std::int32_t category = 0; category < 3; ++category)
        if (BuildNaviWindowHeading(category, category))
            work.availablePages |= 1U << category;
}

#else

namespace {

void BuildNaviWindowRootLabels(NaviWindowState &work) {
    ReleaseNaviWindowStrips(2);
    std::uint32_t bit = 1;
    for (std::int32_t category = 0; category < 3; ++category) {
        if (BuildNaviWindowHeading(category, category))
            work.availablePages |= bit;
        bit = (bit << 1U) | (bit >> 31U);
    }
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

const NaviFolderEntry *GetNaviWindowFolderEntries(std::int32_t category) {
    return category >= 3 ? nullptr : naviFolderPages[category].entries;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static void BuildNaviWindowFolderLabels(NativeNaviWindowState &work, std::int32_t category) {
    const std::int32_t count = NaviFolderEntryCount(category, 2);
    const std::uint32_t available = NaviFolderEntryMask(category, 2);
    const NativeNaviFolderEntry *const entries = navGetFolderEntries(category);
    ReleaseNaviWindowStrips(2);
    BuildNaviWindowHeading(0, category);
    if (!entries)
        return;
    for (std::int32_t row = 0; row < 5 && row + work.firstVisibleEntry < count; ++row) {
        const char *label = "\241\251\241\251\241\251", *secondary = "\241\241";
        const std::int32_t entry = row + work.firstVisibleEntry;
        if (available & (1U << entry)) {
            label = entries[entry].label;
            secondary = entries[entry].secondaryLabel;
        }
        PublishNaviWindowLabel(work, label, secondary, row + 1);
    }
}

#else

namespace {

void BuildNaviWindowFolderLabels(NaviWindowState &work, std::int32_t category) {
    const std::int32_t count = NaviFolderEntryCount(category);
    const std::uint32_t available = NaviFolderEntryMask(category);
    const NaviFolderEntry *const entries = GetNaviWindowFolderEntries(category);
    ReleaseNaviWindowStrips(2);
    (void)BuildNaviWindowHeading(0, category);
    if (!entries)
        return;
    for (std::int32_t row = 0; row < 5; ++row) {
        const std::int32_t entry = row + work.firstVisibleEntry;
        if (entry >= count)
            break;
        const bool unlocked = (available & (1U << (entry & 31))) != 0;
        PublishNaviWindowLabel(work, unlocked ? entries[entry].label : "? ? ? ", unlocked ? entries[entry].secondaryLabel : "  ", row + 1);
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static void LoadNaviWindowPageSource(NativeNaviWindowState *work) {
    if (work->mapImageData)
        HLib::FreeTaskData(work->mapImageData);
    if (!work->page)
        work->mapImageData = shenLoadFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "NAVI_IMG.SPR", SHEN_NAME(NAAA));
    else
        work->mapImageData = LoadNaviFolderSprite(work->selections[0]);
}

#else

namespace {

void LoadNaviWindowPageSource() {
    if (naviWindow.mapImageData) {
        HLib::FreeTaskData(naviWindow.mapImageData);
        naviWindow.mapImageData = nullptr;
    }
    if (!naviWindow.page)
        naviWindow.mapImageData = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "NAVI_IMG.SPR", NWIN_ROOT_FILE);
    else if (naviWindow.categorySelection < 3)
        naviWindow.mapImageData =
            LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), naviFolderPages[naviWindow.categorySelection].sprite, NWIN_FOLDER_FILE);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

static void RefreshNaviWindowPreview(NativeNaviWindowState *work) {
    TextureChunk *cursor = static_cast<TextureChunk *>(work->mapImageData);
    naviWindowPreviewTexture = -1;
    std::int32_t dummySize = 0;
    std::uint32_t selected;
    if (!work->page) {
        selected = work->selections[0];
        if (!(work->availablePages & (1U << selected))) {
            cursor = static_cast<TextureChunk *>(work->dummySpriteData);
            selected = 0;
        }
    } else {
        selected = work->selections[1] + work->firstVisibleEntry;
        if (!(work->availableEntries & (1U << selected))) {
            cursor = static_cast<TextureChunk *>(work->dummySpriteData);
            selected = 0;
            dummySize = 256;
        }
    }
    if (!cursor)
        return;
    std::uint32_t texture = 0;
    while (cursor->name) {
        if (cursor->name == SHEN_NAME(TEXN)) {
            if (texture == selected) {
                TextureChunk *const chunk = cursor;
                const std::int32_t slot = chunk->width != 128;
                ReplacePtxTexture(naviWindowPreviewTextures[slot], chunk);
                naviWindowPreviewTexture = naviWindowPreviewTextures[slot];
                if (dummySize) {
                    work->mapWidth = dummySize;
                    work->mapHeight = dummySize;
                } else {
                    work->mapWidth = static_cast<std::int16_t>(chunk->width);
                    work->mapHeight = static_cast<std::int16_t>(chunk->height);
                }
                return;
            }
            ++texture;
        }
        cursor = reinterpret_cast<TextureChunk *>(reinterpret_cast<std::byte *>(cursor) + cursor->size);
    }
}

#else

namespace {

void RefreshNaviWindowPreview() {
    const std::byte *cursor = static_cast<const std::byte *>(naviWindow.mapImageData);
    std::uint32_t selected = naviWindow.page ? naviWindow.entrySelection + naviWindow.firstVisibleEntry : naviWindow.categorySelection;
    const std::uint32_t available = naviWindow.page ? naviWindow.availableEntries : naviWindow.availablePages;
    naviWindowPreviewTexture = -1;
    std::int32_t dummySize = 0;
    if (!(available & (1U << (selected & 31U)))) {
        if (naviWindow.page)
            dummySize = 256;
        cursor = static_cast<const std::byte *>(naviWindow.dummySpriteData);
        selected = 0;
    }
    if (!cursor)
        return;
    std::int32_t texture = 0;
    for (;;) {
        std::uint32_t tag = 0, size = 0;
        std::memcpy(&tag, cursor, 4);
        if (!tag)
            return;
        if (tag == SHEN_NAME(TEXN) && texture++ == selected) {
            std::int16_t width = 0, height = 0;
            std::memcpy(&width, cursor + 40, 2);
            std::memcpy(&height, cursor + 42, 2);
            const std::int32_t slot = width != 128;
            (void)ReplacePtxTexture(naviWindowPreviewTextures[slot], cursor);
            naviWindowPreviewTexture = naviWindowPreviewTextures[slot];
            naviWindow.mapWidth = dummySize ? dummySize : width;
            naviWindow.mapHeight = dummySize ? dummySize : height;
            return;
        }
        std::memcpy(&size, cursor + 4, 4);
        cursor += size;
    }
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::uint64_t AllocateNaviWindowLabelTextures() {
    naviWindow.labelScratch = HLib::AllocateTaskData(GetTextureStorageSize(0x8089, 256, 16) + 48ULL, NWIN_LABEL_SCRATCH);
    if (!naviWindow.labelScratch)
        return 0;
    for (std::uint32_t index = 0; index < 6; ++index) {
        const std::uint32_t suffix = (index + 48U) << 24U;
        ParseGuiPtx(static_cast<std::byte *>(naviWindow.labelScratch), 256, 16, NWIN_FOLDER_FILE.name, suffix | make_shen_name('N', 'A', '0', '\0'));
        naviWindowLabelTextures[index] = static_cast<std::int32_t>(ReadTexnTexture(naviWindow.labelScratch));
        ParseGuiPtx(static_cast<std::byte *>(naviWindow.labelScratch), 256, 16, NWIN_FOLDER_FILE.name, suffix | make_shen_name('R', 'B', '0', '\0'));
        naviWindowSecondaryTextures[index] = static_cast<std::int32_t>(ReadTexnTexture(naviWindow.labelScratch));
    }
    return static_cast<std::uint32_t>(naviWindowSecondaryTextures[5]);
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int64_t InitializeNaviWindowResources() {
    std::size_t bytes = 0;
    (void)LoadLegacySprite("sprite", "NV_DUMMY.SPR", NWIN_DUMMY_FILE, naviWindow.dummySpriteData, bytes);
    if (!naviWindow.dummySpriteData)
        return 0;
    std::byte *const ptx = static_cast<std::byte *>(HLib::AllocateTaskData(GetTextureStorageSize(0x8081, 256, 256) + 48ULL, NWIN_TEXTURE_SCRATCH));
    if (!ptx)
        return 0;
    for (std::uint32_t index = 0; index < 2; ++index) {
        const std::uint16_t size = static_cast<std::uint16_t>(128 + 128 * index);
        ParseGuiPtx(ptx, size, size, NWIN_PREVIEW_TEXTURE.name, ((index + 48U) << 24U) | make_shen_name('N', 'V', '0', '\0'), 0x8081);
        naviWindowPreviewTextures[index] = static_cast<std::int32_t>(ReadTexnTexture(ptx));
    }
    HLib::FreeTaskData(ptx);
    AllocateNaviWindowLabelTextures();
    BuildNaviWindowRootLabels(naviWindow);
    LoadNaviWindowPageSource();
    RefreshNaviWindowPreview();
    return 1;
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static void DrawNaviWindowLabel(std::uint32_t row, const Vector2 &position, std::uint32_t colour) {
    LegacySpritePacket packet;
    std::uint32_t secondaryColour = 0xff000000U;
    secondaryColour |= colour;
    row &= ~0x10000000U;
    std::int32_t *labels = naviWindowLabels;
    INIT_NAVI_PACKET(&packet);
    packet.quad.flags |= 0x400;
    packet.secondaryColour = secondaryColour;
    AdjustScreenAspectRatio(packet.quad, VerticalStretch, static_cast<std::int32_t>(position.x), static_cast<std::int32_t>(position.y + 10), 256, 32);
    if (labels[row] >= 0)
        DrawLegacySpritePacket(packet, labels[row], 3.5f);
    labels = naviWindowSecondaryLabels;
    AdjustScreenAspectRatio(packet.quad, VerticalStretch, static_cast<std::int32_t>(position.x), static_cast<std::int32_t>(position.y - 18), 256, 16);
    if (labels[row] >= 0)
        DrawLegacySpritePacket(packet, labels[row], 3.5f);
}

#else

namespace {

void DrawNaviWindowLabel(std::int32_t row, const Vector2 &position, std::uint32_t colour) {
    LegacySpritePacket packet{.quad = {.flags = 1024}, .colour = 0xffffffffU, .secondaryColour = colour | 0xff000000U, .uv = {0, 0, 1, 1}};
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::VerticalStretch, TruncateSpriteCoordinate(position.x), TruncateSpriteCoordinate(position.y + 10), 256,
        32, ScreenMode::Centered);
    if (naviWindowLabels[row] >= 0)
        LegacySpriteNullsub(packet, naviWindowLabels[row], 0.6f);
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::VerticalStretch, TruncateSpriteCoordinate(position.x), TruncateSpriteCoordinate(position.y - 18), 256,
        16, ScreenMode::Centered);
    if (naviWindowSecondaryLabels[row] >= 0)
        LegacySpriteNullsub(packet, naviWindowSecondaryLabels[row], 0.6f);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

static void DrawNaviWindowScaleBar(const Vector2 *position, std::int16_t width, float depth) {
    naviWindowHeadingSprite.alignment = 6;
    naviWindowHeadingSprite.x = position->x;
    naviWindowHeadingSprite.y = position->y;
    naviWindowHeadingSprite.width = width;
    naviWindowHeadingSprite.depth = depth;
    naviWindowHeadingSprite.rotation = 0;
    PrepareDrawWithMode(naviWindowHeadingSprite);
    naviWindowHeadingSprite.alignment = 5;
    naviWindowHeadingSprite.width = 8;
    naviWindowHeadingSprite.rotation = 0x4000;
    PrepareDrawWithMode(naviWindowHeadingSprite);
    naviWindowHeadingSprite.x -= width;
    PrepareDrawWithMode(naviWindowHeadingSprite);
}

static void DrawNaviWindowScaleIcon(const Vector2 *position, std::int32_t distance) {
    DrawMoneyDigits(position->x - 16.0f, position->y, 0.75f, 6.0f, distance, 0xff7f7f7fU);
    LegacyIconSprite icon;
    icon.icon = 0x60e;
    icon.rotation = 0x4000;
    icon.width = icon.height = 16;
    icon.scaleX = icon.scaleY = 1.0f;
    icon.depth = 6.0f;
    icon.x = position->x;
    icon.y = position->y;
    icon.alignment = 5;
    icon.flags = 0x200;
    PrepareDrawWithMode(icon);
}

static void DrawNaviWindow(void *) {
    NativeNaviWindowState *const work = naviWindowWork;
    if (!(work->drawFlags & 7U))
        return;
    navGetFolderEntries(work->selections[0]);
    const float scale = naviWindowScales[work->page];
    LegacyIconSprite cursor;
    cursor.depth = 3.0f;
    cursor.rotation = 0;
    cursor.flags = 0;
    cursor.scaleX = 1.0f;
    cursor.scaleY = 1.0f;
    cursor.width = 32;
    cursor.height = 32;
    cursor.alignment = 5;
    cursor.x = naviWindowPosition.x - 200.0f + 20.0f;
    Vector2 position;
    position.x = naviWindowPosition.x;
    position.y = naviWindowPosition.y;
    DrawLegacyItemWindow(position, 0.0301f, 11, 9);
    position.x += 100.0f;
    if (naviWindowPreviewTexture >= 0) {
        const float left = position.x - 8.0f;
        const float top = position.y - 8.0f;
        LegacySpritePacket packet;
        do {
            INIT_LEGACY_SPRITE_PACKET(packet);
        } while (0);
        packet.colour = 0xff000000U;
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(left), static_cast<std::int32_t>(top), 256, 256);
        packet.quad.flags |= 0x400U;
        DrawLegacyColourPacket(packet, 2.945f);
        if (work->drawFlags & 2U) {
            packet.colour = 0xa0ffffffU;
            AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(left), static_cast<std::int32_t>(top), 256, 256);
            packet.quad.flags |= 0x400U;
            DrawLegacySpritePacket(packet, naviWindowPreviewTexture, 3.5f);
        }
        position.x = naviWindowPosition.x + 200.0f + 20.0f - 16.0f;
        position.y = naviWindowPosition.y + 160.0f + 20.0f - 16.0f;
        const float width = 100.0f / work->mapScaleX * scale;
        if (work->page == 1) {
            DrawNaviWindowScaleIcon(&position, 100);
            position.x -= 64.0f;
            DrawNaviWindowScaleBar(&position, static_cast<std::int16_t>(width), 6.0f);
        }
    }
    Vector2 labels;
    labels.x = naviWindowPosition.x - 200.0f;
    if (!work->page) {
        labels.y = naviWindowPosition.y - 56.0f;
        labels.y += 28.0f;
        for (std::int32_t row = 0; row < 3; ++row) {
            if (row == work->selections[0])
                DrawNaviWindowLabel(row, labels, 0xffff0000U);
            else
                DrawNaviWindowLabel(row, labels, 0);
            labels.y += 56.0f;
        }
    } else {
        std::int32_t rows = work->entryCount + 1;
        if (rows > 6)
            rows = 6;
        labels.y = naviWindowPosition.y - rows * 28.0f + 28.0f;
        DrawNaviWindowLabel(0, labels, 0xff00ff00U);
        labels.y += 56.0f;
        for (std::int32_t row = 0; row < 5; ++row) {
            if (row == work->selections[1])
                DrawNaviWindowLabel((row + 1) | 0x10000000U, labels, 0xffff0000U);
            else
                DrawNaviWindowLabel((row + 1) | 0x10000000U, labels, 0);
            labels.y += 56.0f;
        }
    }
}

#else

namespace {

void DrawNaviWindow(void *) {
    if (!(naviWindow.drawFlags & 7U))
        return;
    (void)GetNaviWindowFolderEntries(naviWindow.categorySelection);
    Vector2 position{naviWindowPosition.x, naviWindowPosition.y};
    DrawLegacyItemWindow(position, 0.11f, 11, 9, ScreenMode::Centered);
    position.x += 100;
    if (naviWindowPreviewTexture >= 0) {
        LegacySpritePacket packet{.quad = {.flags = 0}, .colour = 0xff000000U, .uv = {0, 0, 1, 1}};
        const std::int32_t x = TruncateSpriteCoordinate(position.x - 8), y = TruncateSpriteCoordinate(position.y - 8);
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, x, y, 256, 256, ScreenMode::Centered);
        packet.quad.flags |= 1024;
        (void)shenSubmitQuad(LegacyColourQuad{.quad = packet.quad, .colour = packet.colour}, 0.045f);
        if (naviWindow.drawFlags & 2U) {
            packet.colour = 0xa0ffffffU;
            AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, x, y, 256, 256, ScreenMode::Centered);
            packet.quad.flags |= 1024;
            LegacySpriteNullsub(packet, naviWindowPreviewTexture, 0.6f);
        }
        position.x = naviWindowPosition.x + 204;
        position.y = naviWindowPosition.y + 164;
    }
    Vector2 labelPosition{naviWindowPosition.x - 200, naviWindowPosition.y - 28};
    if (naviWindow.page) {
        const std::int32_t rows = naviWindow.entryCount + 1 > 6 ? 6 : naviWindow.entryCount + 1;
        labelPosition.y = naviWindowPosition.y - rows * 28.0f + 28;
        DrawNaviWindowLabel(0, labelPosition, 0xff00ff00U);
        labelPosition.y += 56;
        for (std::int32_t row = 0; row < 5; ++row) {
            DrawNaviWindowLabel(row + 1, labelPosition, row == naviWindow.entrySelection ? 0xffff0000U : 0xff000000U);
            labelPosition.y += 56;
        }
    } else {
        for (std::int32_t row = 0; row < 3; ++row) {
            DrawNaviWindowLabel(row, labelPosition, row == naviWindow.categorySelection ? 0xffff0000U : 0xff000000U);
            labelPosition.y += 56;
        }
    }
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void ReleaseNaviWindowPreviewTextures() {
    for (std::int32_t &texture : naviWindowPreviewTextures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void NVW_CtrlNaviWindow_Callback(void *) {
    NativeNaviWindowState *const work = naviWindowWork;
    std::uint16_t input = GetPadControlData(1);
    GetPadControlData(2);
    std::uint16_t help = 0;
    switch (work->phase) {
    case 0:
        break;
    case 1:
        if (AdvanceIconPosition(&naviWindowPosition, 320.0f))
            work->phase = 4;
        break;
    case 4: {
        work->drawFlags |= 2;
        help |= 0x600;
        if (work->page) {
            help &= ~0x400;
            input &= ~0x400;
        }
        if (input & 0x400) {
            if (work->availablePages & (1 << work->selections[0])) {
                work->requestedPage = work->page + 1;
                work->firstVisibleEntry = work->selections[1] = 0;
                work->phase = 3;
                ExecuteAicaCommand(0x500ab, 0, 0);
                input = 0;
            } else {
                ExecuteAicaCommand(0x800ab, 0, 0);
            }
        }
        if (input & 0x200) {
            if (work->page > 0) {
                work->requestedPage = work->page - 1;
                work->phase = 3;
            } else {
                work->phase = 5;
            }
            ExecuteAicaCommand(0x500ab, 0, 0);
            input = 0;
        }
        if (input & 0xf000) {
            std::int32_t total, visible;
            if (!work->page) {
                total = 3;
                visible = total;
            } else {
                total = NaviFolderEntryCount(work->selections[0], 2);
                if (total >= 5)
                    visible = 5;
                else
                    visible = total;
            }
            std::int32_t selection = work->selections[work->page];
            const std::int32_t previous = selection + work->firstVisibleEntry;
            if (input & 0x1000) {
                if (--selection < 0) {
                    selection = 0;
                    if (work->page == 1 && work->firstVisibleEntry > 0) {
                        --work->firstVisibleEntry;
                        ReleaseNaviWindowStrips(2);
                        BuildNaviWindowFolderLabels(*work, work->selections[0]);
                    }
                }
            }
            if (input & 0x2000) {
                if (++selection >= visible) {
                    selection = visible;
                    --selection;
                    if (work->page == 1 && selection + work->firstVisibleEntry + 1 < total) {
                        ++work->firstVisibleEntry;
                        ReleaseNaviWindowStrips(2);
                        BuildNaviWindowFolderLabels(*work, work->selections[0]);
                    }
                }
            }
            work->selections[work->page] = selection;
            work->entryCount = NaviFolderEntryCount(work->selections[0], 2);
            if (previous != work->firstVisibleEntry + selection) {
                work->drawFlags &= ~2;
                work->phase = 2;
                ExecuteAicaCommand(0x500ab, 0, 0);
            }
        }
        work->entryCount = NaviFolderEntryCount(work->selections[0], 2);
        work->availableEntries = NaviFolderEntryMask(work->selections[0], 2);
        RequestHelpIconSelection(help);
        break;
    }
    case 2:
        RefreshNaviWindowPreview(work);
        if (work->page)
            RequestHelpIconSelection(0x200);
        else
            RequestHelpIconSelection(0x600);
        work->phase = 4;
        break;
    case 3:
        if (RetreatIconPosition(&naviWindowPosition, -300.0f)) {
            work->page = work->requestedPage;
            ReleaseNaviWindowStrips(2);
            if (work->page)
                BuildNaviWindowFolderLabels(*work, work->selections[0]);
            else
                BuildNaviWindowRootLabels(*work);
            LoadNaviWindowPageSource(work);
            RefreshNaviWindowPreview(work);
            const NativeNaviFolderPage *const folder = navGetFolderPage(work->selections[0]);
            work->mapScaleX = folder->scaleX;
            work->mapScaleY = folder->scaleY;
            work->phase = 1;
        }
        break;
    case 5:
        if (RetreatIconPosition(&naviWindowPosition, -300.0f)) {
            for (std::int32_t index = 0; index < 2; ++index) {
                if (naviWindowPreviewTextures[index] != -1)
                    ReleaseNamedTexture_Wrapper(naviWindowPreviewTextures[index]);
                naviWindowPreviewTextures[index] = -1;
            }
            icsToggleIconState(3, 0);
            HLib::SetupTimedTask(work->task, 0);
            ReleaseNaviWindowStrips(3);
            naviWindowPreviewTexture = -1;
            ReleaseNaviWindowAllocations(work);
            work->phase = 0;
            EV_EventSetUpHelpIcon_0(0x11, 0);
            EV_NPC_NewLoadSw(work->savedNpcLoadState);
        }
        break;
    }
    HLib::ExecuteCallbackNow(DrawNaviWindow, 0);
}

#else

namespace {

void NVW_CtrlNaviWindow_Callback(void *) {
    std::uint16_t input = GetPadControlData(1);
    (void)GetPadControlData(2);
    switch (naviWindow.phase) {
    case 1:
        naviWindow.phase = 2;
        break;
    case 2:
        naviWindow.phase = 3;
        break;
    case 3:
        InitializeNaviWindowResources();
        naviWindow.drawFlags |= 7U;
        naviWindow.phase = 4;
        break;
    case 4:
        if (AdvanceIconPosition(naviWindowPosition.x, naviWindowVelocity, 320.0f))
            naviWindow.phase = 7;
        break;
    case 5:
        RefreshNaviWindowPreview();
        set_icon_select_flags_to(naviWindow.page == 0 ? 0x600U : 0x200U);
        naviWindow.phase = 7;
        break;
    case 6:
        if (RetreatIconPosition(naviWindowPosition.x, naviWindowVelocity, -300.0f - GetScreenAlignmentOffset())) {
            naviWindow.page = naviWindow.requestedPage;
            ReleaseNaviWindowStrips(2);
            if (naviWindow.page)
                BuildNaviWindowFolderLabels(naviWindow, naviWindow.categorySelection);
            else
                BuildNaviWindowRootLabels(naviWindow);
            LoadNaviWindowPageSource();
            RefreshNaviWindowPreview();
            naviWindow.mapScaleX = naviFolderPages[naviWindow.categorySelection].scaleX;
            naviWindow.mapScaleY = naviFolderPages[naviWindow.categorySelection].scaleY;
            naviWindow.phase = 4;
        }
        break;
    case 7: {
        naviWindow.drawFlags |= 2U;
        const std::uint32_t iconFlags = naviWindow.page == 0 ? 0x600U : 0x200U;
        if (naviWindow.page != 0)
            input &= ~0x400U;
        if (input & 0x400U) {
            if (naviWindow.availablePages & (1U << (naviWindow.categorySelection & 31))) {
                naviWindow.requestedPage = naviWindow.page + 1;
                naviWindow.entrySelection = naviWindow.firstVisibleEntry = 0;
                naviWindow.phase = 6;
                input = 0;
            } else {
                (void)ExecuteAicaCommand(0x800abU, 0, 0);
            }
        }
        if (input & 0x200U) {
            if (naviWindow.page <= 0)
                naviWindow.phase = 8;
            else {
                naviWindow.phase = 6;
                naviWindow.requestedPage = naviWindow.page - 1;
            }
            input = 0;
        }
        if (input & 0xf000U) {
            const std::int32_t total = naviWindow.page ? NaviFolderEntryCount(naviWindow.categorySelection) : 3;
            const std::int32_t visible = naviWindow.page ? (total < 5 ? total : 5) : 3;
            std::int32_t selection = naviWindow.page ? naviWindow.entrySelection : naviWindow.categorySelection;
            const std::int32_t previous = selection + naviWindow.firstVisibleEntry;
            if ((input & 0x1000U) && --selection < 0) {
                selection = 0;
                if (naviWindow.page == 1 && naviWindow.firstVisibleEntry > 0) {
                    --naviWindow.firstVisibleEntry;
                    ReleaseNaviWindowStrips(2);
                    BuildNaviWindowFolderLabels(naviWindow, naviWindow.categorySelection);
                }
            }
            if ((input & 0x2000U) && ++selection >= visible) {
                selection = visible - 1;
                if (naviWindow.page == 1 && visible + naviWindow.firstVisibleEntry < total) {
                    ++naviWindow.firstVisibleEntry;
                    ReleaseNaviWindowStrips(2);
                    BuildNaviWindowFolderLabels(naviWindow, naviWindow.categorySelection);
                }
            }
            if (naviWindow.page)
                naviWindow.entrySelection = selection;
            else
                naviWindow.categorySelection = selection;
            naviWindow.entryCount = NaviFolderEntryCount(naviWindow.categorySelection);
            if (selection + naviWindow.firstVisibleEntry != previous) {
                naviWindow.drawFlags &= ~2U;
                naviWindow.phase = 5;
            }
        }
        naviWindow.entryCount = NaviFolderEntryCount(naviWindow.categorySelection);
        naviWindow.availableEntries = NaviFolderEntryMask(naviWindow.categorySelection);
        set_icon_select_flags_to(iconFlags);
        break;
    }
    case 8:
        if (RetreatIconPosition(naviWindowPosition.x, naviWindowVelocity, -300.0f - GetScreenAlignmentOffset())) {
            ReleaseNaviWindowPreviewTextures();
            icsToggleIconState(3, 0);
            HLib::SetupTimedTask(naviWindow.task, 0);
            ReleaseNaviWindowStrips(3);
            naviWindowPreviewTexture = -1;
            ReleaseNaviWindowAllocations();
            naviWindow.phase = 0;
            EV_EventSetUpHelpIcon_0(0x17, 0);
            EV_NPC_NewLoadSw(naviWindow.savedNpcLoadState);
        }
        break;
    default:
        break;
    }
    HLib::ExecuteCallbackNow(DrawNaviWindow, nullptr);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static void CreateNaviWindowLabelTextures(NativeNaviWindowState &work) {
    work.labelScratch = HLib::AllocateTaskData(0x834, SHEN_NAME(NVHP));
    if (!work.labelScratch)
        return;
    for (std::int32_t index = 0; index < 6; ++index) {
        ParseGuiPtx(static_cast<TextureChunk *>(work.labelScratch), 16, 256, SHEN_NAME4('N', 'V', 'S', 'T'),
            SHEN_NAME4('N', 'A', '0', 0) | ((index + '0') << 24), 0x500);
        naviWindowLabelTextures[index] = LoadAndFindTEXN_Wrapper(static_cast<TextureChunk *>(work.labelScratch));
        ParseGuiPtx(static_cast<TextureChunk *>(work.labelScratch), 16, 256, SHEN_NAME4('N', 'V', 'S', 'T'),
            SHEN_NAME4('R', 'B', '0', 0) | ((index + '0') << 24), 0x500);
        naviWindowSecondaryTextures[index] = LoadAndFindTEXN_Wrapper(static_cast<TextureChunk *>(work.labelScratch));
    }
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t OpenNaviWindow() {
    NativeNaviWindowState *const work = naviWindowWork;
    std::int32_t index;
    work->savedNpcLoadState = GetNpcNewLoadState();
    work->labelScratch = work->mapImageData = work->dummySpriteData = 0;
    do {
        if (!work->task)
            break;
        naviWindowPreviewTexture = -1;
        work->availablePages = 0;
        for (index = 0; index < 3; ++index)
            if (NaviFolderEntryMask(index, 2))
                work->availablePages |= 1U << index;
        work->availableEntries = NaviFolderEntryMask(0, 2);
        work->entryCount = NaviFolderEntryCount(0, 2);
        if (!work->availablePages)
            break;
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        work->dummySpriteData = shenLoadFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "NV_DUMMY.SPR", SHEN_NAME(NVDY));
        TextureChunk *const scratch = static_cast<TextureChunk *>(HLib::AllocateTaskData(0x482c, SHEN_NAME(NTMP)));
        if (scratch) {
            for (index = 0; index < 2; ++index) {
                const std::int32_t size = (index + 1) * 128;
                ParseGuiPtx(scratch, size, size, SHEN_NAME4('N', 'V', 'T', 'P'), SHEN_NAME4('N', 'V', '0', 0) | ((index + '0') << 24), 0x302);
                naviWindowPreviewTextures[index] = LoadAndFindTEXN_Wrapper(scratch);
            }
            HLib::FreeTaskData(scratch);
        }
        CreateNaviWindowLabelTextures(*work);
        BuildNaviWindowRootLabels(*work);
        naviWindowPosition.x = -300.0f;
        work->phase = 1;
        work->drawFlags = 7;
        work->selections[0] = work->selections[1] = work->page = 0;
        LoadNaviWindowPageSource(work);
        RefreshNaviWindowPreview(work);
        EV_EventSetUpHelpIcon_0(0x74, 0);
        icsToggleIconState(3, 1);
        HLib::ResolveDeferredCallback(work->task);
        return 1;
    } while (0);
    return 0;
}

#else

std::int64_t OpenNaviWindow() {
    naviWindow.dummySpriteData = naviWindow.mapImageData = naviWindow.labelScratch = nullptr;
    if (!naviWindow.task)
        return 0;
    naviWindowPreviewTexture = -1;

    naviWindow.availablePages = 0;
    for (std::size_t page = 0; page < naviFolderPages.size(); ++page) {
        if (NaviFolderEntryMask(static_cast<std::int32_t>(page)) != 0)
            naviWindow.availablePages |= 1U << page;
    }
    naviWindow.availableEntries = NaviFolderEntryMask(0);
    naviWindow.entryCount = NaviFolderEntryCount(0);
    if (!naviWindow.availablePages)
        return 0;

    naviWindow.savedNpcLoadState = static_cast<std::int32_t>(GetNpcNewLoadState());
    EV_NPC_NewLoadSw(0);
    naviWindowPosition.x = -300.0f - GetScreenAlignmentOffset();
    naviWindow.phase = 1;
    naviWindow.page = 0;
    naviWindow.drawFlags = 0;
    naviWindow.categorySelection = 0;
    naviWindow.entrySelection = 0;
    EV_EventSetUpHelpIcon_0(0x74, 0);
    icsToggleIconState(3, 1);
    HLib::ResolveDeferredCallback(naviWindow.task);
    return 1;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t UpdateNaviWindow() {
    if (GetCurrentItemSelectState() & 8U)
        return 1;
    return 0;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t CloseNaviWindow(std::int32_t force) {
    NativeNaviWindowState *const work = naviWindowWork;
    if (work->task && force && work->phase) {
        ReleaseNaviWindowStrips(3);
        ReleaseNaviWindowAllocations(work);
        work->phase = 0;
        HLib::SetupTimedTask(work->task, 0);
        EV_NPC_NewLoadSw(work->savedNpcLoadState);
        return 1;
    }
    return 0;
}

#else

std::int64_t CloseNaviWindow(std::int32_t force) {
    if (!naviWindow.task || !force || !naviWindow.phase)
        return 0;
    ReleaseNaviWindowPreviewTextures();
    ReleaseNaviWindowStrips(3);
    ReleaseNaviWindowAllocations();
    naviWindow.phase = 0;
    icsToggleIconState(3, 0);
    HLib::SetupTimedTask(naviWindow.task, 0);
    EV_NPC_NewLoadSw(naviWindow.savedNpcLoadState);
    return 1;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void ReleaseNavigationTaskOwners() {
    (void)CloseNaviWindow(1);
    ReleaseNaviWindowAllocations();
    if (naviWindow.task)
        HLib::DestroyTask(naviWindow.task);
    if (navigationTask)
        HLib::DestroyTask(navigationTask);
    naviWindow.task = nullptr;
    navigationTask = nullptr;
    ReleaseNaviResources();
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t navCreate(const Vector3 &position, std::int32_t type) {
    const NaviTaskData *task;
    if (g_naviTask)
        task = reinterpret_cast<const NaviTaskData *>(HLib::GetTaskParameter(g_naviTask));
    else
        task = 0;
    if (!task)
        return -1;
    const NativeNaviMapConfig *config = task->config;
    std::int32_t area = navGetNumEntriesFor(g_currentMap);
    if (area == -1)
        return -1;
    for (std::int32_t index = 0; index < naviAreaConfigCount && nativeNaviAreaConfigs[index].map; ++index) {
        if (g_currentMap == nativeNaviAreaConfigs[index].map) {
            area = index;
            break;
        }
    }
    std::int32_t floor = task->stream;
    if (floor == -1000)
        floor = 63;
    NativeNaviMarker *marker = reinterpret_cast<NativeNaviMarker *>(&g_savedNaviMarkers[g_savedNaviMarkerCursor++]);
    if (g_savedNaviMarkerCursor > 127)
        g_savedNaviMarkerCursor = 0;
    std::int32_t markerType;
    switch (type) {
    case 2:
        markerType = 0;
        break;
    case 3:
        markerType = 1;
        break;
    case 4:
        markerType = 2;
        break;
    default:
        markerType = 0;
        break;
    }
    float x = position.x - config->originX;
    float z = position.z - config->originZ;
    const float scale = 256.0f / (config->scale * 128.0f);
    x *= scale;
    z *= scale;
    marker->active = 1;
    marker->type = markerType + 1;
    marker->x = static_cast<std::int32_t>(x);
    marker->z = static_cast<std::int32_t>(z);
    marker->area = area;
    marker->floor = floor;
    return g_savedNaviMarkerCursor;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void navPlaceMarker(const Vector3 *position, std::int32_t type) {
    std::int32_t marker;
    switch (type) {
    case 0:
    default:
        marker = 0;
        break;
    case 1:
        marker = 2;
        break;
    case 2:
        marker = 3;
        break;
    case 3:
        marker = 4;
        break;
    }
    navCreate(*position, marker);
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t IsNaviActive() {
    return naviPhase == 2;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::uint32_t SetNaviDisplayOverride(std::int32_t enabled) {
    const std::uint32_t previous = naviFlags & 1U;
    if (enabled)
        naviFlags |= 1U;
    else
        naviFlags &= ~1U;
    return previous;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static bool CanReuseNaviTrailTexture(shen_name previous, shen_name current) {
    for (std::int32_t index = 0; index < 7; ++index)
        if (nativeNaviRetainedMapPairs[index][0] == previous && nativeNaviRetainedMapPairs[index][1] == current)
            return true;
    return false;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static std::int32_t DoesNaviTrailTextureMatch(const TextureChunk *texture) {
    shen_name name = texture->name;
    if (name != SHEN_NAME(TEXN))
        return 0;
    name = texture->nameLow;
    if (name != SHEN_NAME(NAV0))
        return 0;
    if (name != SHEN_NAME(AUTO))
        return 0;
    if (texture->height != 256)
        return 0;
    if (texture->width != 256)
        return 0;
    if (texture->type != 0x502)
        return 0;
    return 1;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#define NAVI_RETAINED_TRAIL_TEXTURE reinterpret_cast<TextureChunk *>(eventExecutionRegions[2] + 0x7800)
#define INITIALIZE_NAVI_TRAIL_TEXTURE(texture)                                                                                                                 \
    {                                                                                                                                                          \
        ParseGuiPtx(texture, 256, 256, SHEN_NAME4('N', 'A', 'V', '0'), SHEN_NAME4('A', 'U', 'T', 'O'), 0x502);                                                 \
        end = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(texture) + 0x802c);                                                              \
        end[0] = end[1] = 0;                                                                                                                                   \
        std::memset((texture) + 1, 0xaa, 0x8000);                                                                                                              \
    }

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static void LoadNaviBackground() {
    TextureChunk *const data = static_cast<TextureChunk *>(shenLoadFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "NVBACK.SPR", SHEN_NAME(HOKR)));
    if (data) {
        naviBackgroundTexture = LoadAndFindTEXN_Wrapper(data);
        HLib::FreeTaskData(data);
    }
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
static bool LoadNaviFloorImages(const NativeNaviFloorDefinition &definition) {
    for (std::int32_t index = 0; index < 6; ++index)
        naviFloorTextures[index] = -1;
    void *const data = shenLoadFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), definition.filename, SHEN_NAME(HOKR));
    const std::uint32_t bytes = shenOpenFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), definition.filename);
    if (data) {
        ReadTitleSpriteTextures(data, bytes, naviFloorTextures);
        HLib::FreeTaskData(data);
        return true;
    }
    return false;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

bool ConstructNaviMap(std::uint32_t configIndex) {
    bool result;
    std::int32_t release = 1;
    TextureChunk *data;
    result = false;
    if (configIndex < 32) {
        const char *const stem = nativeNaviMapConfigs[configIndex].spriteStem;
        if (stem) {
            char filename[32];
            std::strcpy(filename, stem);
            std::strcat(filename, "NAV.SPR");
            data = static_cast<TextureChunk *>(shenLoadFile("sprite", filename, SHEN_NAME(NAVI)));
        } else {
            LoadNaviBackground();
            if (naviRetainedMap && CanReuseNaviTrailTexture(naviRetainedMap, g_currentMap)) {
                data = NAVI_RETAINED_TRAIL_TEXTURE;
                release = 0;
            } else {
                data = static_cast<TextureChunk *>(HLib::AllocateTaskData(0x8034, SHEN_NAME(NAVI)));
                if (data) {
                    std::uint32_t *end;
                    INITIALIZE_NAVI_TRAIL_TEXTURE(data);
                    if (!DoesNaviTrailTextureMatch(NAVI_RETAINED_TRAIL_TEXTURE))
                        INITIALIZE_NAVI_TRAIL_TEXTURE(NAVI_RETAINED_TRAIL_TEXTURE);
                }
            }
            naviRetainedMap = g_currentMap;
            naviFlags |= 2U;
        }
        if (data) {
            naviTextures[0] = LoadAndFindTEXN_Wrapper(data);
            if (naviTextures[0] >= 0) {
                g_naviTrailTexturePixels = g_lastLoadedTexturePixels;
                const std::int32_t floor = nativeNaviMapConfigs[configIndex].floorDefinition;
                if (floor >= 0 && LoadNaviFloorImages(nativeNaviFloorDefinitions[floor])) {
                    NaviTaskData *const task = g_naviTask ? static_cast<NaviTaskData *>(HLib::GetTaskParameter(g_naviTask)) : 0;
                    task->definition = nativeNaviFloorDefinitions + floor;
                    naviFlags |= 0x10U;
                }
            }
            result = true;
            if (release)
                HLib::FreeTaskData(data);
        }
    }
    return result;
}

#undef INITIALIZE_NAVI_TRAIL_TEXTURE
#undef NAVI_RETAINED_TRAIL_TEXTURE

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void DrawNaviBackground(std::int32_t x, std::int32_t y, float zoom) {
    if (naviBackgroundTexture > 0) {
        AdjustScreenAspectRatio(naviOverlayPacket.quad, static_cast<ScreenAlignment>(5), x, y,
            static_cast<std::int32_t>((naviBackgroundWidthScale * 128.0f) * zoom), static_cast<std::int32_t>((naviBackgroundHeightScale * 192.0f) * zoom));
        DrawLegacySpritePacket(naviOverlayPacket, naviBackgroundTexture, 1.6f);
    }
}

} // namespace

#endif

} // namespace s2
} // namespace shenmue
