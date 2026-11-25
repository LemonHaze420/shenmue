#include "shenmue.h"
#include "s2/legacy_sprite_types.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/full_map_menu.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/navigation.hpp"
#include "s2/hlib.hpp"
#include "s2/camera_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/pause.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "platform/dc/matrix.hpp"
#else
#include "s2/full_map_menu.hpp"

#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/hlib.hpp"
#include "s2/inventory.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/navi.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/ui.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

constexpr FullMapFloor aberdeenFloors[]{{6012, 128, 0, -11, 0, "UIMAPDWOPI", 0, 1, {}}, // @text EN: "Worker's Pier" | JA: "労湾碼頭"
    {6013, 128, 44, 26, 0, "UIMAPDQUST", 0, 1, {}},                                     // @text EN: "Queen's St." | JA: "クイーンズストリート"
    {6015, 128, 2, -76, 0, "UIMAPDFOPI", 0, 1, {}},                                     // @text EN: "Fortune's Pier" | JA: "開運碼頭"
    {6014, 128, 1, 65, 0, "UIMAPDBHW0", 0, 1, {}},                                      // @text EN: "Beverly Hills Wharf" | JA: "ビバリーヒルズワーフ"
    {6026, 256, 0, 0, 0, "UIMAPDWONG", 0, 4, {}},                                       // @text EN: "Wong" | JA: "ウォン"
    {6025, 256, 0, 0, 0, "UIMAPDREN0", 0, 4, {}},                                       // @text EN: "Ren" | JA: "レン"
    {-1000, 256, 0, 0, 0, nullptr, 0, 2, {}}};

constexpr FullMapFloor wanChaiFloors[]{{6011, 128, -84, -28, 0, "UIMAPDGMQR", 0, 1, {}}, // @text EN: "Green Market Qr." | JA: "菜珍街"
    {6008, 128, -57, 47, 0, "UIMAPDSCQR", 0, 1, {}},                                     // @text EN: "South Carmain Qr." | JA: "紅南街"
    {6009, 128, 20, 55, 0, "UIMAPDWMQR", 0, 1, {}},                                      // @text EN: "Wise Men's Qr." | JA: "太老街"
    {6027, 128, 84, 36, 0, "UIMAPDSCHI", 0, 1, {}},                                      // @text EN: "Scarlet Hills" | JA: "紅華台"
    {6010, 128, 48, 1, 0, "UIMAPDLCQR", 0, 1, {}},                                       // @text EN: "Lucky Charm Qr." | JA: "福仙街"
    {6006, 256, -21, -50, 0, "UIMAPDGOQR", 0, 1, {}},                                    // @text EN: "Golden Qr." | JA: "詠黄街"
    {6007, 128, -7, -15, 0, "UIMAPDWHQR", 0, 1, {}},                                     // @text EN: "White Dynasty Qr." | JA: "白秦街"
    {6018, 256, 0, 0, 0, "UIMAPDDYA0", 0, 4, {}}};                                       // @text EN: "Da Yuan Apartments" | JA: "大元楼"
constexpr FullMapFloor kowloonFloors[]{{6019, 128, -89, 21, 0, "UIMAPDDRST", 0, 1, {}},  // @text EN: "Dragon St." | JA: "龍影街"
    {6020, 256, -34, 27, 0, "UIMAPDTWQR", 0, 1, {}},                                     // @text EN: "Thousand White Qr." | JA: "白千街"
    {6022, 256, 37, -25, 0, "UIMAPDSTQR", 0, 1, {}},                                     // @text EN: "Stand Qr." | JA: "屋台街"
    {6021, 128, 17, 32, 0, "UIMAPDDIQR", 0, 1, {}},                                      // @text EN: "Dimsum Qr." | JA: "点心街"
    {6001, 256, 37, -25, 0, "UIMAPDABLD", 0, 4, {}},                                     // @text EN: "A Building" | JA: "九龍城Ａビル"
    {6002, 256, 37, -25, 0, "UIMAPDBBLD", 0, 4, {}},                                     // @text EN: "B Building" | JA: "九龍城Ｂビル"
    {6003, 256, 37, -25, 0, "UIMAPDCBLD", 0, 4, {}},                                     // @text EN: "C Building" | JA: "九龍城Ｃビル"
    {6004, 256, 37, -25, 0, "UIMAPDDBLD", 0, 4, {}},                                     // @text EN: "D Building" | JA: "九龍城Ｄビル"
    {6005, 256, 37, -25, 0, "UIMAPDEBLD", 0, 4, {}},                                     // @text EN: "E Building" | JA: "九龍城Ｅビル"
    {6023, 256, 37, -25, 0, "UIMAPDFBLD", 0, 4, {}},                                     // @text EN: "F Building" | JA: "九龍城Ｆビル"
    {6017, 256, 0, 0, 0, "UIMAPDSCOU", 0, 4, {}}};                                       // @text EN: "Scout" | JA: "スカウトマン"
constexpr std::array<FullMapConfig, 3> configs{{{SHEN_NAME(A000), 2.0199999809265137f, 0.6700000166893005f, 0, aberdeenFloors, 223, 400, "A000_G.SPR",
                                                     "UIMAREABER", 0},                                              // @text EN: "Aberdeen" | JA: "香港仔"
    {SHEN_NAME(W000), 1.5f, 0.6700000166893005f, 0, wanChaiFloors, 188, 196, "W000_G.SPR", "UIMAREWACH", 0},        // @text EN: "Wan Chai" | JA: "湾仔"
        {SHEN_NAME(Q000), 1.25f, 0.6700000166893005f, 0, kowloonFloors, 240, 140, "Q000_G.SPR", "UIMAREKOWL", 0}}}; // @text EN: "Kowloon" | JA: "九龍城"
constexpr std::array<std::int32_t, 3> floorCounts{7, 8, 11};

constexpr std::array<Vector2, 4> tileCorners{{{-1, -1}, {1, -1}, {-1, 1}, {1, 1}}};

std::array<std::int32_t, 14> textures;

const FullMapConfig *activeConfig{};

std::int32_t textureCount{};

std::int32_t previousPlayerControl{};

std::int32_t pitch{}, yawDelta{};

float zoom{}, scrollX{}, scrollY{};

LegacyIconSprite marker{};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void InitializeFullMapMenu();
std::int64_t OpenFullMapMenu();
std::int64_t UpdateFullMapMenu();
void CloseFullMapMenu();
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t CurrentConfigIndex() {
    const auto first = static_cast<std::uint8_t>(g_currentMap.name);
    for (std::size_t i = 0; i < configs.size(); ++i)
        if (static_cast<std::uint8_t>(configs[i].map.name) == first)
            return static_cast<std::int32_t>(i);
    return -1;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ProjectQuad(LegacyScreenQuad &quad, float halfWidth, const Vector3 &player) {
    for (std::size_t i = 0; i < tileCorners.size(); ++i) {
        Vector3 projected{};
        Matrix4x4::TransformPoint({halfWidth * tileCorners[i].x, 0, halfWidth * tileCorners[i].y}, projected);
        quad.corners[i * 2] = (320.0f - player.x) + projected.x;
        quad.corners[i * 2 + 1] = (240.0f - player.z) + projected.z;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::span<const FullMapConfig, 3> GetFullMapConfigs() noexcept {
    return configs;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::span<const FullMapFloor> GetFullMapFloors(std::int32_t index) noexcept {
    if (index < 0 || index >= 3)
        return {};
    return {configs[static_cast<std::size_t>(index)].floors, static_cast<std::size_t>(floorCounts[static_cast<std::size_t>(index)])};
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint32_t GetFullMapFloorMask(std::int32_t index, std::uint16_t excludedFlags) {
    std::uint32_t mask{}, bit = 1;
    for (const FullMapFloor &floor : GetFullMapFloors(index)) {
        if (!(floor.flags & excludedFlags) && (floor.item < 0 || GetItemState(static_cast<std::uint32_t>(floor.item))))
            mask |= bit;
        bit = (bit << 1) | (bit >> 31);
    }
    return mask;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static const NativeNaviFolderEntry *fullMapEntries = 0;

static std::uint32_t fullMapPreviousPlayerControl = 1;

static float fullMapScale = 1.0f;

static const Vector2 fullMapCorners[4] = {{-1.0f, -1.0f}, {1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, 1.0f}};

static std::int32_t fullMapTextureCount;

static struct {
    std::int32_t pitch, yawDelta, retained;
} fullMapView;

static Vector2 fullMapOrigin;

static float fullMapZoom, fullMapTileScale, fullMapScrollX, fullMapScrollY;

static LegacyIconSprite fullMapMarker;

static std::int32_t fullMapTextures[10];

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t FindFullMapPage(shen_name area) {
    for (std::int32_t page = 0; page <= 2; ++page) {
        const NativeNaviFolderPage *const folder = navGetFolderPage(page);
        if (folder && static_cast<std::int8_t>(area) == static_cast<std::int8_t>(folder->area))
            return static_cast<std::int32_t>(page);
    }
    return -1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void InitializeFullMapMenu() {
    for (std::int32_t index = 0; index < 10; ++index)
        fullMapTextures[index] = -1;
    fullMapMarker.width = fullMapMarker.height = 20;
    fullMapMarker.scaleX = fullMapMarker.scaleY = 1.0f;
    fullMapMarker.depth = 10.0f;
    fullMapMarker.alignment = 5;
    fullMapMarker.icon = 0x414;
    fullMapMarker.flags = 0;
    fullMapMarker.rotation = 0;
    fullMapMarker.x = 320.0f;
    fullMapMarker.y = 240.0f;
}

#else

void InitializeFullMapMenu() {
    marker = {.x = 320, .y = 240, .scaleX = 1, .scaleY = 1, .depth = 10, .icon = 1044, .width = 20, .height = 20, .alignment = 5};
    textures.fill(-1);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool CanOpenFullMapMenu() {
    const std::int32_t index = CurrentConfigIndex();
    return index >= 0 && GetFullMapFloorMask(index, 6) && GetFullMapFloorMask(index, 4);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t OpenFullMapMenu() {
    const NativeNaviFolderEntry *entries;
    std::uint32_t previousNpc;
    std::int32_t loaded = 0;
    const std::int32_t index = FindFullMapPage(g_currentMap);
    if (index < 0 || !NaviFolderEntryMask(index, 6))
        return 0;
    const std::uint32_t mask = NaviFolderEntryMask(index, 4);
    if (!mask)
        return 0;
    const NativeNaviFolderPage *const folder = navGetFolderPage(index);
    if (!folder)
        return 0;
    entries = navGetFolderEntries(index);
    if (!entries)
        return 0;
    const std::int32_t count = NaviFolderEntryCount(index, 0);
    previousNpc = GetNpcNewLoadState();
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    void *const data = LoadNaviFolderSprite(index);
    if (data) {
        const TextureChunk *cursor = reinterpret_cast<const TextureChunk *>(data);
        loaded = 0;
        std::int32_t parsed = 0;
        for (; parsed < count && cursor->name == SHEN_NAME4('T', 'E', 'X', 'N'); ++parsed) {
            if (mask & (1U << parsed)) {
                fullMapTextures[parsed] = LoadAndFindTEXN_Wrapper(cursor);
                ++loaded;
            }
            cursor = reinterpret_cast<const TextureChunk *>(reinterpret_cast<const std::uint8_t *>(cursor) + cursor->size);
        }
        fullMapTextureCount = parsed;
        HLib::FreeTaskData(data);
    }
    fullMapOrigin = *reinterpret_cast<const Vector2 *>(&folder->originX);
    fullMapScale = folder->scaleX;
    fullMapTileScale = folder->scaleY;
    fullMapEntries = entries;
    fullMapView.pitch = fullMapView.yawDelta = fullMapView.retained = 0;
    fullMapZoom = 1.0f / (fullMapTileScale * fullMapScale);
    EV_NPC_NewLoadSw(previousNpc);
    if (loaded > 0) {
        fullMapPreviousPlayerControl = SetPlayerControlEnabled(0);
        icsToggleIconState(6, 1);
        return 1;
    }
    return 0;
}

#else

std::int64_t OpenFullMapMenu() {
    const std::int32_t index = CurrentConfigIndex();
    if (index < 0 || !GetFullMapFloorMask(index, 6))
        return 0;
    const std::uint32_t floorMask = GetFullMapFloorMask(index, 4);
    if (!floorMask)
        return 0;
    const FullMapConfig &config = configs[static_cast<std::size_t>(index)];
    const std::uint32_t previousNpc = static_cast<std::uint32_t>(GetNpcNewLoadState());
    EV_ResetFreeTalkFlags();
    EV_NPC_NewLoadSw(0);
    textures.fill(-1);
    void *allocation{};
    std::uint32_t size{};
    allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), config.sprite, FULL_MAP_SPRITE_FILE, &size);
    std::int32_t loaded{}, parsed{};
    if (allocation) {
        char path[128]{};
        std::snprintf(path, sizeof(path), "/SPRITE/%s/", config.sprite);
        SetSpriteTexturePath(path);
        const auto *cursor = static_cast<const std::byte *>(allocation);
        const auto *const end = cursor + size;
        const std::int32_t count = floorCounts[static_cast<std::size_t>(index)];
        for (; parsed < count && end - cursor >= 8; ++parsed) {
            std::uint32_t tag{}, chunkSize{};
            std::memcpy(&tag, cursor, 4);
            std::memcpy(&chunkSize, cursor + 4, 4);
            if (tag != SHEN_NAME(TEXN) || chunkSize < 8 || chunkSize > static_cast<std::size_t>(end - cursor))
                break;
            if (floorMask & (1U << parsed)) {
                textures[static_cast<std::size_t>(parsed)] = static_cast<std::int32_t>(ReadTexnTexture(cursor));
                ++loaded;
            }
            cursor += chunkSize;
        }
        textureCount = parsed;
        HLib::FreeTaskData(allocation);
        SetSpriteTexturePath(nullptr);
    }
    activeConfig = &config;
    pitch = yawDelta = 0;
    zoom = 1.0f / (config.scale * config.tileScale);
    EV_NPC_NewLoadSw(previousNpc);
    if (!loaded)
        return 0;
    previousPlayerControl = SetPlayerControlEnabled(0);
    icsToggleIconState(6, 1);
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t UpdateFullMapMenu() {
    std::uint32_t trigger;
    std::uint16_t input;
    trigger = input = 0;
    if (!(g_loadingFlag & 2U)) {
        trigger = GetPadControlData(1);
        input = GetPadControlData(0);
    }
    std::int32_t yaw = -playerGetRotY();
    if (input & 2U) {
        if (input & 0x1000U)
            fullMapScrollY -= 5.0f;
        if (input & 0x2000U)
            fullMapScrollY += 5.0f;
        if (input & 0x4000U)
            fullMapScrollX += 5.0f;
        if (input & 0x8000U)
            fullMapScrollX -= 5.0f;
    } else {
        if (input & 0x4000U)
            fullMapView.yawDelta += 182;
        if (input & 0x8000U)
            fullMapView.yawDelta -= 182;
        if (input & 0x2000U) {
            fullMapView.pitch -= 182;
            if (fullMapView.pitch < -11832)
                fullMapView.pitch = -11832;
        }
        if (input & 0x1000U) {
            fullMapView.pitch += 182;
            if (fullMapView.pitch > 11832)
                fullMapView.pitch = 11832;
        }
        if (input & 0x40U)
            fullMapZoom += 0.05f;
        if (input & 0x80U)
            fullMapZoom -= 0.05f;
    }
    yaw = (yaw + fullMapView.yawDelta) & 0xffff;
    const float extent = fullMapScale * 384.0f * fullMapTileScale * fullMapZoom;
    if (128.0f > extent)
        fullMapZoom = 128.0f / (fullMapScale * 384.0f * fullMapTileScale);
    if (extent > 1024.0f)
        fullMapZoom = 1024.0f / (fullMapScale * 384.0f * fullMapTileScale);
    LegacySpritePacket tile, background;
    INIT_LEGACY_SPRITE_PACKET(tile);
    tile.colour = 0xa0ffffffU;
    INIT_LEGACY_SPRITE_PACKET(background);
    background.quad.flags |= 0x400U;
    tile.quad.flags |= 0x400U;
    background.colour = 0x7f003600U;
    Vector3 location;
    Vector3 corner;
    plymUpdatePlayerSearchLocation(&location);
    matrix_stack::PushUnitMatrix();
    Vector3 player;
    player.x = location.x - fullMapOrigin.x;
    player.z = location.z - fullMapOrigin.y;
    player.y = 0.0f;
    Matrix4x4::preTranslate(320.0f, 0.0f, 240.0f);
    Matrix4x4::RotateAroundX_Wrapper(fullMapView.pitch);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::Scale(fullMapZoom, fullMapZoom, fullMapZoom);
    Matrix4x4::TransformPoint(player, player);
    Matrix4x4::Scale(fullMapScale, fullMapScale, fullMapScale);
    std::int32_t index;
    for (index = 0; index < 4; ++index) {
        corner.x = fullMapCorners[index].x * 192.0f * fullMapTileScale;
        corner.z = fullMapCorners[index].y * 192.0f * fullMapTileScale;
        corner.y = 0.0f;
        Matrix4x4::TransformPoint(corner, corner);
        background.quad.points[index].x = 320.0f - player.x + corner.x;
        background.quad.points[index].y = 240.0f - player.z + corner.z;
    }
    for (std::int32_t texture = 0; texture < fullMapTextureCount; ++texture) {
        if (fullMapTextures[texture] < 0)
            continue;
        float halfWidth = fullMapEntries[texture].textureSize;
        halfWidth *= 0.5f;
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::preTranslate(fullMapEntries[texture].offset.x, 0.0f, fullMapEntries[texture].offset.y);
        for (index = 0; index < 4; ++index) {
            corner.x = fullMapCorners[index].x * halfWidth;
            corner.z = fullMapCorners[index].y * halfWidth;
            corner.y = 0.0f;
            Matrix4x4::TransformPoint(corner, corner);
            tile.quad.points[index].x = 320.0f - player.x + corner.x;
            tile.quad.points[index].y = 240.0f - player.z + corner.z;
        }
        PopCurrentMatrix(1);
        DrawLegacySpritePacket(tile, static_cast<std::uint32_t>(fullMapTextures[texture]), static_cast<float>(texture) * 0.1f + 2.0f);
    }
    PopCurrentMatrix(1);
    DrawLegacyColourPacket(background, 1.0f);
    DrawMainIconOverlay(0x76000000U, 0.11f);
    fullMapMarker.scaleX = fullMapMarker.scaleY = fullMapScale * fullMapTileScale * fullMapZoom;
    PrepareDrawWithMode(fullMapMarker);
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::int32_t facing = -yaw;
    shenSetRotY(control->ryo, facing);
    Vector3 camera, target;
    camCtrlGetNextCamPos(&camera);
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&camera);
    Matrix4x4::RotateAroundY_Wrapper(facing);
    Matrix4x4::preTranslate(0.0f, 0.0f, -10.0f);
    matrix_stack::GetTranslation(&target);
    PopCurrentMatrix(1);
    camCtrlSetTargetPosition(target);
    fullMapView.yawDelta = 0;
    RequestHelpIconSelection(0x200);
    if (trigger & 0x200U)
        return 0;
    return 1;
}

#else

std::int64_t UpdateFullMapMenu() {
    std::uint16_t input{}, trigger{};
    if (!(g_loadingFlag & 2U)) {
        trigger = GetPadControlData(1);
        input = GetPadControlData(0);
    }
    if (input & 2U) {
        if (input & 0x1000U)
            scrollY -= 5;
        if (input & 0x2000U)
            scrollY += 5;
        if (input & 0x4000U)
            scrollX += 5;
        if (input & 0x8000U)
            scrollX -= 5;
    } else {
        if (input & 0x4000U)
            yawDelta += 182;
        if (input & 0x8000U)
            yawDelta -= 182;
        if (input & 0x2000U)
            pitch = std::max(pitch - 182, -11832);
        if (input & 0x1000U)
            pitch = std::min(pitch + 182, 11832);
        if (input & 0x80U)
            zoom += 0.050000001f;
        if (input & 0x40U)
            zoom -= 0.050000001f;
    }
    const auto yaw = static_cast<std::uint16_t>(-static_cast<std::int16_t>(playerGetRotY()) + yawDelta);
    const float extent = (activeConfig->scale * 384.0f) * activeConfig->tileScale;
    const float previousExtent = extent * zoom;
    if (previousExtent < 128.0f)
        zoom = 128.0f / extent;
    if (previousExtent > 1024.0f)
        zoom = 1024.0f / extent;
    Vector3 location{};
    plymUpdatePlayerSearchLocation(&location);
    Vector3 player{location.x - activeConfig->originX, 0, location.z - activeConfig->originZ};
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(320, 0, 240);
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(pitch));
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::Scale(zoom, zoom, zoom);
    Matrix4x4::TransformPoint(player, player);
    Matrix4x4::Scale(activeConfig->scale, activeConfig->scale, activeConfig->scale);
    LegacyColourQuad background{.quad = {.flags = 0x400}, .colour = 0x7f003600U};
    ProjectQuad(background.quad, 192.0f * activeConfig->tileScale, player);
    LegacySpritePacket tile{.quad = {.flags = 0x400}, .colour = 0xa0ffffffU, .uv = {0, 0, 1, 1}};
    for (std::int32_t i = 0; i < textureCount; ++i) {
        const auto slot = static_cast<std::size_t>(i);
        if (textures[slot] < 0)
            continue;
        const FullMapFloor &floor = activeConfig->floors[slot];
        Matrix4x4_Push();
        Matrix4x4::preTranslate(floor.x, 0, floor.z);
        ProjectQuad(tile.quad, static_cast<float>(floor.width) * 0.5f, player);
        Matrix4x4::Pop();
        naviDraw(tile, textures[slot], static_cast<float>(i) * 0.1f + 2.0f);
    }
    Matrix4x4::Pop();
    shenSubmitQuad(background, 1.0f);
    vfSubmitQuad(0x76000000U, 0.11f);
    marker.scaleX = marker.scaleY = (activeConfig->tileScale * activeConfig->scale) * zoom;
    DrawLegacyIconSprite(marker, 0xff000000U, ScreenMode::Centered);
    const auto facing = static_cast<std::uint32_t>(-static_cast<std::int32_t>(yaw));
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    shenSetRotY(static_cast<Character *>(control->ryo), static_cast<std::int32_t>(facing));
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(camera.x, camera.y, camera.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(facing));
    Matrix4x4::preTranslate(0, 0, -10);
    const Vector3 target{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop();
    camCtrlSetTargetPosition(target);
    yawDelta = 0;
    set_icon_select_flags_to(512);
    return !(trigger & 0x200U) || IsHelpOverlayVisible(UIElement::GameHelp) ? 1 : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ReleaseNamedTexture_Wrapper(std::int32_t index);

// @verified - d
void CloseFullMapMenu() {
    for (std::int32_t index = 0; index < 10; ++index) {
        if (fullMapTextures[index] >= 0)
            ReleaseNamedTexture_Wrapper(fullMapTextures[index]);
        fullMapTextures[index] = -1;
    }
    icsToggleIconState(6, 0);
    SetPlayerControlEnabled(static_cast<std::int32_t>(fullMapPreviousPlayerControl));
}

#else

void CloseFullMapMenu() {
    for (std::int32_t &texture : textures) {
        if (texture >= 0)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
    icsToggleIconState(6, 0);
    SetPlayerControlEnabled(previousPlayerControl);
}

#endif

} // namespace s2
} // namespace shenmue
