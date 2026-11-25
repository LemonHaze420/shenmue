#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/author_control_state_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/screen_film_bars_dc.hpp"
#include "s2/texture_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/navigation_map.hpp"
#include <cstdio>

namespace shenmue {
namespace s2 {

namespace {

struct NavigationMapLayer {
    std::byte reserved[0x20];
    std::int32_t texture;
    float depth;
};

typedef char Layout_navigation_map_0[(sizeof(NavigationMapLayer) == 0x28) ? 1 : -1];

} // namespace

#define INIT_NAVIGATION_PACKET(packet, value)                                                                                                                  \
    do {                                                                                                                                                       \
        (packet).quad.flags = (value);                                                                                                                         \
        (packet).colour = UINT32_MAX;                                                                                                                          \
        (packet).secondaryColour = (value);                                                                                                                    \
        (packet).uv[0] = (packet).uv[1] = 0.0f;                                                                                                                \
        (packet).uv[2] = (packet).uv[3] = 1.0f;                                                                                                                \
        (packet).reservedTail = (value);                                                                                                                       \
    } while (0)

std::int32_t GetNavigationMapLoaded();
static void ComputeNavigationMapView();
static void DrawNavigationMap(void *);

namespace {

const float navigationMapCorners[4][2] = {{-128.0f, -128.0f}, {128.0f, -128.0f}, {-128.0f, 128.0f}, {128.0f, 128.0f}};
const float navigationMarkerCorners[4][2] = {{-4.0f, -4.0f}, {4.0f, -4.0f}, {-4.0f, 4.0f}, {4.0f, 4.0f}};
std::int32_t navigationMapLoaded = 0;
NavigationMapLayer navigationMapLayer;
NavigationMapLayer navigationMarkerLayer;
LegacySpritePacket navigationMapPacket;
LegacySpritePacket navigationMarkerPacket;
std::uint32_t navigationMarkerBlinkFrame = 0;

} // namespace

// @verified - d
void ResetNavigationMapState() {
    navigationMapLoaded = 0;
}

// @verified - d
void LoadNavigationMap() {
    std::int32_t index;
    const std::int32_t loaded = GetNavigationMapLoaded();
    if (loaded)
        return;
    char directory[128];
    std::sprintf(
        directory, "scene/%02d/%c%c%c%c", g_actNumber, g_currentMap & 0xff, (g_currentMap >> 8) & 0xff, (g_currentMap >> 16) & 0xff, g_currentMap >> 24);
    TextureChunk *const marker = static_cast<TextureChunk *>(shenLoadFile(directory, "NAVIMAP.SPR", SHEN_NAME(NMAP)));
    const TextureChunk *const map = reinterpret_cast<const TextureChunk *>(reinterpret_cast<const std::byte *>(marker) + marker->size);
    const std::int32_t markerTexture = LoadAndFindTEXN_Wrapper(marker);
    const std::int32_t mapTexture = LoadAndFindTEXN_Wrapper(map);
    HLib::FreeTaskData(marker);
    INIT_NAVIGATION_PACKET(navigationMarkerPacket, loaded);
    navigationMarkerPacket.quad.flags |= 0x400U;
    for (index = 0; index < 4; ++index) {
        navigationMarkerPacket.quad.corners[index * 2] = navigationMarkerCorners[index][0] + 512.0f;
        (&navigationMarkerPacket.quad.corners[1])[index * 2] = navigationMarkerCorners[index][1] + 512.0f;
    }
    navigationMarkerLayer.texture = markerTexture;
    navigationMarkerLayer.depth = 2.0f;
    INIT_NAVIGATION_PACKET(navigationMapPacket, 0);
    navigationMapPacket.quad.flags |= 0x400U;
    for (index = 0; index < 4; ++index) {
        navigationMapPacket.quad.corners[index * 2] = navigationMapCorners[index][0] + 512.0f;
        (&navigationMapPacket.quad.corners[1])[index * 2] = navigationMapCorners[index][1] + 512.0f;
    }
    navigationMapLayer.texture = mapTexture;
    navigationMapLayer.depth = 1.0f;
    navigationMarkerBlinkFrame = 0;
    navigationMapLoaded = 1;
}

void ReleaseNamedTexture_Wrapper(std::int32_t index);

// @verified - d
void ReleaseNavigationMap() {
    ReleaseNamedTexture_Wrapper(navigationMarkerLayer.texture);
    ReleaseNamedTexture_Wrapper(navigationMapLayer.texture);
    navigationMapLoaded = 0;
}

// @verified - d
void SetNavigationMapEnabled(std::int32_t enabled) {
    if (enabled) {
        if (!GetNavigationMapLoaded())
            LoadNavigationMap();
    } else if (GetNavigationMapLoaded()) {
        ReleaseNavigationMap();
    }
}

// @verified - d
std::int32_t GetNavigationMapLoaded() {
    return navigationMapLoaded;
}

// @verified - d
void UpdateNavigationMap() {
    if (navigationMapLoaded) {
        ComputeNavigationMapView();
        HLib::ExecuteCallbackNow(DrawNavigationMap, 0);
    }
}

// @verified - d
static void ComputeNavigationMapView() {
    std::int32_t index;
    const std::int32_t yaw = camCtrlGetNextRotY();
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    for (index = 0; index < 4; ++index) {
        const float x = navigationMapCorners[index][0] - (camera.x + 25.0f) * 0.75f;
        const float z = navigationMapCorners[index][1] - (camera.z + -25.0f) * 0.75f;
        navigationMapPacket.quad.corners[index * 2] = x * dc::sdk::CosFixed(yaw) - z * dc::sdk::SinFixed(yaw) + 512.0f;
        (&navigationMapPacket.quad.corners[1])[index * 2] = x * dc::sdk::SinFixed(yaw) + z * dc::sdk::CosFixed(yaw) + 320.0f;
    }
    for (index = 0; index < 4; ++index) {
        navigationMarkerPacket.quad.corners[index * 2] = navigationMarkerCorners[index][0] + 512.0f;
        (&navigationMarkerPacket.quad.corners[1])[index * 2] = navigationMarkerCorners[index][1] + 320.0f;
    }
}

// @verified - d
static void DrawNavigationMap(void *) {
    if (authGetLoaded()) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (static_cast<std::int32_t>(GetCinemaBarHeight(control->filmTask)))
            return;
    }
    DrawLegacySpritePacket(navigationMapPacket, navigationMapLayer.texture, navigationMapLayer.depth);
    if (navigationMarkerBlinkFrame <= 30)
        DrawLegacySpritePacket(navigationMarkerPacket, navigationMarkerLayer.texture, navigationMarkerLayer.depth);
    if (static_cast<std::int32_t>(++navigationMarkerBlinkFrame) > 59)
        navigationMarkerBlinkFrame = 0;
}

} // namespace s2
} // namespace shenmue

#endif
