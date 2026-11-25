#include "s2/map_load_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/air_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/flag_wind_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/map_vertex_cache_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/pak_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/scene_state_dc.hpp"
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <bit>
#endif
#include <cstdio>
#include <cstring>
#include "s2/lighting_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "s2/hlib.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/search_dc.hpp"
#include "s2/map_draw_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/mt7.hpp"
#include <cmath>
#include "s2/model_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/map_control_dc.hpp"

namespace shenmue {
namespace s2 {

void CacheMapCameraDistances(std::int32_t restore);
MT7Node *GetMapModelRoot(std::int32_t index);
const MapModelClipRecord *GetMapModelClipRecords(std::int32_t index);
MT7Model *GetMapModelReader(std::int32_t index);
void SetMapModelReader(MT7Model *model, std::int32_t index);
std::uint32_t GetMapModelGridCullingEnabled(std::int32_t index);
void SetMapModelGridCullingEnabled(std::uint32_t enabled, std::int32_t index);
std::uint32_t GetMapModelEnabled(std::int32_t index);
void SetMapModelEnabled(std::uint32_t enabled, std::int32_t index);
std::int32_t ReadMapPackCount(const MapInfoChunk *chunk);
const shen_name *ReadMapAreaOverride(const MapInfoChunk *chunk);
bool ReadEntryMapLayers(const MapInfoChunk *chunk);
std::int32_t ReadMapPackageMode(const MapInfoChunk *chunk);
MapModelCommandCell *GetMapModelCommandCells(std::int32_t index);
MapModelClipRecord **GetMapModelCommandRouting(std::int32_t index);
void SetDefaultVS();
void InitializeMapVertexCache();
std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour);
void SetupModelDataDefaultVS(MT7Node *root);

std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour);
void SetupModelDataDefaultVS(MT7Node *root);
void InitializeMapFieldDepthSlots();
void FreeMapFieldDepthSlot(std::int32_t index);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
PakGlobalData *RequestMapPackage(const char *directory, const char *name);
#endif
std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour);
void SetupModelDataDefaultVS(MT7Node *root);
std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour);
void SetupModelDataDefaultVS(MT7Node *root);
std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour);
void SetupModelDataDefaultVS(MT7Node *root);
std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour);
void SetupModelDataDefaultVS(MT7Node *root);
std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour);
void AssignLightProgram(std::uint32_t mapSlot);
void PushGlobalLightingProgram();
void ApplyCharacterGeometryLighting(Character *character, std::int32_t replaceDrawCallback);
void SetupModelDataDefaultVS(MT7Node *root);
void ResetCharacterGeometryLighting(Character *character);
void SetMapModelSlotEnabled(std::int32_t enabled, std::int32_t index);
std::uint32_t GetGeometryLightColour(Vector4 *colour);
std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour);
MapLoadState g_mapLoadState = {};
MapModelCache g_mapModelCache = {};
shen_name g_mapAreaOverride;
const MapModelClipRecord *g_mapVertexCacheRecords;
std::int32_t g_mapVertexCacheRecordIndex;
Vector3 *g_mapVertexCacheVertices;
std::int32_t g_mapVertexCacheVertexCount;
std::int32_t g_mapVertexCacheEnabled = 0;
Vector3 *savedLightPositions = 0;
MapModelRuntimeSlot g_mapModelSlots[32];
void SetupModelForImageModule(MT7Node *root);
void UpdatePropSearchVisibility(MT7Node *root, std::uint32_t parentMarker, std::int32_t enabled);
std::uint32_t srchRegisterAndPush(MT7Node *root);
PropReaderSlot g_propReaderSlots[17];
PropState g_propState;

// @verified - d
void ResetMapModelDrawCache() {
    char *const cached = reinterpret_cast<char *>(&g_mapModelCache);
    *reinterpret_cast<std::int32_t *>(cached + 0) = -1;
    *reinterpret_cast<std::int32_t *>(cached + 4) = -1;
    *reinterpret_cast<std::int32_t *>(cached + 8) = -1;
    CacheMapCameraDistances(0);
}

// @verified - d
void CacheMapCameraDistances(std::int32_t restore) {
    if (restore) {
        camCtrlSetFarDistance(g_mapModelCache.farDistance);
        camCtrlSetNearDistance(g_mapModelCache.nearDistance);
        camCtrlSetNpcRadius(g_mapModelCache.npcRadius);
    } else {
        g_mapModelCache.farDistance = camCtrlGetFarDistance();
        g_mapModelCache.nearDistance = camCtrlGetNearDistance();
        g_mapModelCache.npcRadius = camCtrlGetNpcRadius();
    }
}

// @verified - d
void CacheMapModelEnabled(std::int32_t restore) {
    std::int32_t *const cached = &g_mapModelCache.enabled;
    if (restore) {
        std::uint32_t bits = static_cast<std::uint32_t>(*cached);
        for (std::uint32_t index = 0; index < 32; bits >>= 1U, ++index)
            SetMapModelEnabled((bits & 1U) ? 1U : 0U, index);
    } else {
        std::uint32_t bits = 0;
        for (std::uint32_t index = 0; index < 32; ++index)
            if (GetMapModelEnabled(index))
                bits |= 1U << index;
        *cached = static_cast<std::int32_t>(bits);
    }
}

// @verified - d
void CacheMapFieldDepth(std::int32_t restore) {
    if (restore) {
        std::uint32_t bits = static_cast<std::uint32_t>(g_mapModelCache.fieldDepth);
        for (std::uint32_t index = 0; index < 32; bits >>= 1U, ++index)
            SetMapFieldDepthSlotEnabled(bits & 1U ? 1 : 0, index);
    } else {
        std::uint32_t bits = 0;
        for (std::uint32_t index = 0; index < 32; ++index)
            if (GetMapFieldDepthSlotEnabled(index))
                bits |= 1U << index;
        g_mapModelCache.fieldDepth = static_cast<std::int32_t>(bits);
    }
}

// @verified - d
void CacheMapGridCulling(std::int32_t restore) {
    if (restore) {
        std::uint32_t bits = static_cast<std::uint32_t>(g_mapModelCache.gridCulling);
        for (std::uint32_t index = 0; index < 32; bits >>= 1U, ++index)
            SetMapModelGridCullingEnabled(bits & 1U ? 1U : 0U, index);
    } else {
        std::uint32_t bits = 0;
        for (std::uint32_t index = 0; index < 32; ++index)
            if (GetMapModelGridCullingEnabled(index))
                bits |= 1U << index;
        g_mapModelCache.gridCulling = static_cast<std::int32_t>(bits);
    }
}

// @verified - d
void MapModelSlotCacheUtil(std::int32_t operation) {
    switch (operation) {
    case 4:
        CacheMapCameraDistances(0);
        break;
    case 5:
        CacheMapCameraDistances(1);
        break;
    case 2:
        CacheMapModelEnabled(0);
        CacheMapFieldDepth(0);
        CacheMapGridCulling(0);
        break;
    case 3:
        CacheMapModelEnabled(1);
        CacheMapFieldDepth(1);
        CacheMapGridCulling(1);
        break;
    }
}

// @verified - d
void ClearMapModelSlot(MapModelRuntimeSlot &slot) {
    slot.enabled = 1;
    slot.clipRecords = 0;
    slot.reader = 0;
    slot.commandCellCount = 0;
    slot.commandCells = 0;
    slot.commandRouting = 0;
    slot.position.x = 0.0f;
    slot.position.y = 0.0f;
    slot.position.z = 0.0f;
    slot.rotationY = 0;
    slot.ownsReader = 0;
    slot.usesCommandRouting = 1;
    slot.alternateTraversalEnabled = 0;
    slot.gridCullingEnabled = 1;
    slot.drawableRecordCount = 0;
    slot.screenSpaceRecordCount = 0;
    slot.nearRecordCount = 0;
    slot.dangerRecordCount = 0;
    slot.type4RecordCount = 0;
    slot.alpha = 1.0f;
    slot.weatherLightingOverride = 0;
    slot.auxiliaryAllocation = 0;
}

// @verified - d
void InitializeMapLightingPlacement(MapLightingPlacement *placement) {
    placement->enabled = 1;
    placement->mapSlot = 0x7fff;
    placement->position.x = 0.0f;
    placement->position.y = 0.0f;
    placement->position.z = 0.0f;
    placement->rotationY = 0;
    placement->active = 1;
}

// @verified - d
void SetupModel(std::int32_t index, MT7Model *model) {
    if (GetMapModelRoot(index))
        return;
    SetMapModelReader(model, index);
    AllocForCLIP(GetMapModelRoot(index), index);
    if (model->root) {
        SetupModelForImageModule(model->root);
        model->root->position.x = 0.0f;
        model->root->position.y = 0.0f;
        model->root->position.z = 0.0f;
        model->root->rotation.x = 0;
        model->root->rotation.y = 0;
        model->root->rotation.z = 0;
        model->root->scale.x = 1.0f;
        model->root->scale.y = 1.0f;
        model->root->scale.z = 1.0f;
    }
    g_mapModelSlots[index].renderMode = 0;
    g_mapModelSlots[index].drawMode = 0;
}

// @verified - d
void RegisterPackageMapModel(std::int32_t index, MT7Model *model) {
    SetupModel(index, model);
    g_mapModelSlots[index].ownsReader = 0;
}

// @verified - d
void SetupOwnedMapModel(void *source, std::int32_t index) {
    SetupModel(index, InitializeMT7Model(source));
    g_mapModelSlots[index].ownsReader = 1;
}

// @verified - d
std::int32_t FindFreeMapModelSlot() {
    for (std::int32_t index = 0; index < 32; ++index) {
        if (!g_mapModelSlots[index].reader)
            return index;
    }
    return -1;
}

// @verified - d
std::int32_t FindMapModelSlot(const MT7Model *model) {
    for (std::int32_t index = 0; index < 32; ++index) {
        if (g_mapModelSlots[index].reader == model)
            return index;
    }
    return -1;
}

// @verified - d
void FreeMapModelSlot(std::int32_t index) {
    if (index >= 32 || !g_mapModelSlots[index].reader)
        return;
    if (g_mapModelSlots[index].ownsReader)
        ReleaseMT7Model(GetMapModelReader(index));
    HLib::FreeTaskData(const_cast<MapModelClipRecord *>(GetMapModelClipRecords(index)));
    HLib::FreeTaskData(GetMapModelCommandCells(index));
    HLib::FreeTaskData(GetMapModelCommandRouting(index));
    HLib::FreeTaskData(g_mapModelSlots[index].auxiliaryAllocation);
    FreeMapFieldDepthSlot(index);
    ClearMapModelSlot(g_mapModelSlots[index]);
}

void MapModelSlotNullsub() {
}

// @verified - d
void *LoadPackageMapModel(PakGlobalData *package, void *data, std::int32_t visible) {
    const std::uint32_t magic = *reinterpret_cast<const std::uint32_t *>(data);
    if ((magic ^ SHEN_NAME(MD07)) & 0xff00ffffU)
        return 0;
    MT7Model *const model = InitializeMT7Model(data);
    if (package->flags & PakKeepRetained)
        HLib::SetTaskDataMotionFlag(model);
    const std::int32_t index = FindFreeMapModelSlot();
    RegisterPackageMapModel(index, model);
    if (!visible)
        SetMapModelEnabled(0, index);
    std::int32_t slot = 0;
    if (package->mapHandles[0] == -1) {
        package->mapHandles[0] = index;
    } else {
        for (++slot; slot <= 31; ++slot) {
            if (package->mapHandles[slot] == -1) {
                package->mapHandles[slot] = index;
                break;
            }
        }
    }
    return model;
}

// @verified - d
void LoadEntryMapLayers(const char *directory) {
    std::int32_t index = 0;
    const std::int32_t *layers = g_mapLoadState.layers;
    for (; index < 32; ++index) {
        const std::int32_t layer = layers[index];
        if (layer == -1)
            return;
        char filename[64];
        if (layer == 0)
            std::sprintf(filename, "MAP.MT7");
        else
            std::sprintf(filename, "MAP%02d.MT7", static_cast<int>(layer));
        SetupOwnedMapModel(shenLoadFile(directory, filename, SHEN_NAME4('M', 'A', 'P', ' ')), index);
    }
}

// @verified - d
void LoadMapPacks(std::int32_t count, const char *directory) {
    SetupOwnedMapModel(shenLoadFile(directory, "MAP.MT7", SHEN_NAME4('M', 'A', 'P', ' ')), 0);
    if (count) {
        for (std::int32_t index = 1; index <= count; ++index) {
            char filename[64];
            std::sprintf(filename, "MAP%02d.MT7", static_cast<int>(index));
            SetupOwnedMapModel(shenLoadFile(directory, filename, SHEN_NAME4('M', 'A', 'P', ' ')), index);
        }
    }
}

// @verified - d
void LoadMapPackages(std::int32_t count, const char *directory) {
    if (!count)
        count = 1;
    for (std::int32_t index = 0; index < count; ++index) {
        char name[64];
        std::sprintf(name, "MPK%02d", static_cast<int>(index));
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        RequestMapPackage(directory, name);
#else
        ReadPakFileFromDirectory(directory, name, 0);
#endif
        pakLoadFlush();
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LoadMap(std::int32_t act, shen_name area, MapInfoChunk *mapInfo) {
    const volatile std::int32_t count = ReadMapPackCount(FindNativeChunk("MAPR", mapInfo));
    const shen_name *const overrideArea = ReadMapAreaOverride(FindNativeChunk("MAPP", mapInfo));
    const bool entryLayers = ReadEntryMapLayers(FindNativeChunk("MAPE", mapInfo));
    const std::int32_t packageMode = ReadMapPackageMode(FindNativeChunk("MAPT", mapInfo));
    const MapRenderProfile &profile = MatchMapRenderProfile(area);
    camCtrlSetFarDistance(profile.farDistance);
    camCtrlSetNearDistance(profile.nearDistance);
    camCtrlSetNpcRadius(profile.npcRadius);
    ResetMapModelDrawCache();
    std::int32_t index = 0;
    do {
        ClearMapModelSlot(g_mapModelSlots[index]);
    } while (++index <= 31);
    index = 0;
    do {
        InitializeMapLightingPlacement(&g_mapLoadState.placements[index]);
    } while (++index <= 31);
    InitializeMapFieldDepthSlots();

    char directory[64];
    shen_name directoryArea = area;
    char *const areaBytes = reinterpret_cast<char *>(&directoryArea);
    if (overrideArea) {
        const char *const source = reinterpret_cast<const char *>(overrideArea);
        for (index = 0; index <= 3; ++index)
            areaBytes[index] = source[index];
    }
    std::sprintf(directory, "scene/%02d/%c%c%c%c", static_cast<int>(act), areaBytes[0], areaBytes[1], areaBytes[2], areaBytes[3]);
    if (entryLayers) {
        LoadEntryMapLayers(directory);
        return;
    }
    if (count < 0)
        return;
    if (packageMode)
        LoadMapPackages(count, directory);
    else
        LoadMapPacks(count, directory);
    InitializeMapVertexCache();
}
#else
// @verified - d
void LoadMap(std::int32_t act, shen_name area, MapInfoChunk *mapInfo) {
    const std::int32_t count = ReadMapPackCount(FindNativeChunk("MAPR", mapInfo));
    const shen_name *const overrideArea = ReadMapAreaOverride(FindNativeChunk("MAPP", mapInfo));
    const bool entryLayers = ReadEntryMapLayers(FindNativeChunk("MAPE", mapInfo));
    const std::int32_t packageMode = ReadMapPackageMode(FindNativeChunk("MAPT", mapInfo));
    const MapRenderProfile &profile = MatchMapRenderProfile(area);
    camCtrlSetFarDistance(profile.farDistance);
    camCtrlSetNearDistance(profile.nearDistance);
    camCtrlSetNpcRadius(profile.npcRadius);
    ResetMapModelDrawCache();
    for (std::size_t rangeIndex106 = 0; rangeIndex106 < sizeof(g_mapModelSlots) / sizeof((g_mapModelSlots)[0]); ++rangeIndex106) {
        MapModelRuntimeSlot &slot = (g_mapModelSlots)[rangeIndex106];
        ClearMapModelSlot(slot);
    }
    for (std::size_t rangeIndex107 = 0; rangeIndex107 < sizeof(g_mapLoadState.placements) / sizeof((g_mapLoadState.placements)[0]); ++rangeIndex107) {
        MapLightingPlacement &placement = (g_mapLoadState.placements)[rangeIndex107];
        InitializeMapLightingPlacement(&placement);
    }
    InitializeMapFieldDepthSlots();
    if (overrideArea)
        area = *overrideArea;
    char directory[64];
    std::sprintf(directory, "scene/%02d/%c%c%c%c", static_cast<int>(act), static_cast<char>(area), static_cast<char>(area >> 8U),
        static_cast<char>(area >> 16U), static_cast<char>(area >> 24U));
    if (entryLayers)
        LoadEntryMapLayers(directory);
    else if (count >= 0) {
        if (packageMode)
            LoadMapPackages(count, directory);
        else
            LoadMapPacks(count, directory);
        InitializeMapVertexCache();
    }
}
#endif

// @verified - d
void ResetPropReaderSlots() {
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(sizeof(g_propReaderSlots) / sizeof(g_propReaderSlots[0])); ++index) {
        g_propReaderSlots[index].previous = -1;
        g_propReaderSlots[index].next = -1;
        g_propReaderSlots[index].reader = 0;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdatePropRoot() {
    MT7Node *root = g_propState.root;
    if (!root)
        return;
    SetupModelForImageModule(root);
    root->position.x = 0.0f;
    root->position.y = 0.0f;
    root->position.z = 0.0f;
    root->rotation.x = 0;
    root->rotation.y = 0;
    root->rotation.z = 0;
    root->scale.x = 1.0f;
    root->scale.y = 1.0f;
    root->scale.z = 1.0f;
    g_propState.flagWind = AllocateFlagObject(root);
    UpdatePropSearchVisibility(root, 0, 0);
    srchRegisterAndPush(root->child);
}
#else
// @verified - d
void UpdatePropRoot() {
    if (!g_propState.root)
        return;
    SetupModelForImageModule(g_propState.root);
    g_propState.root->position = (__typeof__(g_propState.root->position)){0.0f, 0.0f, 0.0f};
    g_propState.root->rotation = (__typeof__(g_propState.root->rotation)){0, 0, 0};
    g_propState.root->scale = (__typeof__(g_propState.root->scale)){1.0f, 1.0f, 1.0f};
    g_propState.flagWind = AllocateFlagObject(g_propState.root);
    UpdatePropSearchVisibility(g_propState.root, 0, 0);
    srchRegisterAndPush(g_propState.root->child);
}
#endif

// @verified - d
std::int32_t RegisterPropReader(MT7Model *reader) {
    std::int32_t slot, index;
    MT7Node *root, *last;
    if (!reader || !reader->root)
        return -1;
    for (slot = 1; slot <= 16; ++slot)
        if (g_propReaderSlots[slot].previous == -1)
            break;
    if (slot == 17)
        return -1;
    index = slot;
    slot = 0;
    while (g_propReaderSlots[slot].next != -1)
        slot = g_propReaderSlots[slot].next;
    g_propReaderSlots[slot].next = static_cast<std::int16_t>(index);
    g_propReaderSlots[index].previous = static_cast<std::int16_t>(slot);
    g_propReaderSlots[index].reader = reader;
    root = reader->root;
    if (g_propState.root) {
        last = g_propState.root->child;
        while (last->sibling)
            last = last->sibling;
        last->sibling = root->child;
        for (last = root->child; last; last = last->sibling)
            last->parent = g_propState.root;
        return index;
    }
    g_propState.root = root;
    return index;
}

// @verified - d
std::int32_t RegisterPackagePropReader(MT7Model *reader) {
    const std::int32_t index = RegisterPropReader(reader);
    if (index != -1) {
        for (MT7Node *node = reader->root->child; node; node = node->sibling) {
            node->position.x -= g_propState.position.x;
            node->position.y -= g_propState.position.y;
            node->position.z -= g_propState.position.z;
        }
    }
    UpdatePropRoot();
    return index;
}

// @verified - d
void *LoadPackageProp(void *data, std::uint32_t *handle) {
    const std::uint32_t magic = *reinterpret_cast<const std::uint32_t *>(data);
    if ((magic ^ SHEN_NAME(MD07)) & 0xff00ffffU)
        return 0;
    MT7Model *const model = InitializeMT7Model(data);
    *handle = static_cast<std::uint32_t>(RegisterPackagePropReader(model));
    return model;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RemovePropReader(std::uint32_t slotIndex) {
    if (!g_propState.root || slotIndex > 16)
        return;
    if (!g_propReaderSlots[slotIndex].reader)
        return;

    MT7Node *first = g_propReaderSlots[slotIndex].reader->root->child;
    const std::uint32_t index = slotIndex;
    MT7Node *next;
    if (g_propReaderSlots[index].next != -1) {
        const std::int32_t nextIndex = g_propReaderSlots[index].next;
        next = g_propReaderSlots[nextIndex].reader->root->child;
        g_propReaderSlots[nextIndex].previous = g_propReaderSlots[index].previous;
    } else {
        next = 0;
    }
    g_propReaderSlots[g_propReaderSlots[index].previous].next = g_propReaderSlots[index].next;
    if (g_propState.root == g_propReaderSlots[index].reader->root) {
        if (next) {
            g_propState.root = g_propReaderSlots[g_propReaderSlots[index].next].reader->root;
            do {
                next->parent = g_propState.root;
                next = next->sibling;
            } while (next);
        } else {
            g_propState.root = 0;
        }
    } else {
        MT7Node *previous = g_propState.root->child;
        while (first != previous->sibling)
            previous = previous->sibling;
        previous->sibling = next;
    }
    g_propReaderSlots[index].previous = -1;
    g_propReaderSlots[index].next = -1;
    g_propReaderSlots[index].reader = 0;
}
#else
// @verified - d
void RemovePropReader(std::uint32_t index) {
    if (!g_propState.root || index >= 17 || !g_propReaderSlots[index].reader)
        return;
    PropReaderSlot &slot = g_propReaderSlots[index];
    MT7Node *const first = slot.reader->root->child;
    MT7Node *next = 0;
    if (slot.next != -1) {
        next = g_propReaderSlots[slot.next].reader->root->child;
        g_propReaderSlots[slot.next].previous = slot.previous;
    }
    g_propReaderSlots[slot.previous].next = slot.next;
    if (g_propState.root == slot.reader->root) {
        if (!next)
            g_propState.root = 0;
        else {
            g_propState.root = g_propReaderSlots[slot.next].reader->root;
            do {
                next->parent = g_propState.root;
                next = next->sibling;
            } while (next);
        }
    } else {
        MT7Node *previous = g_propState.root->child;
        while (previous->sibling != first)
            previous = previous->sibling;
        previous->sibling = next;
    }
    slot.previous = -1;
    slot.next = -1;
    slot.reader = 0;
}
#endif

// @verified - d
void RemovePackagePropReader(std::uint32_t index) {
    RemovePropReader(index);
    UpdatePropRoot();
}

// @verified - d
void LoadPROP(const char *directory, const char *name) {
    RegisterPropReader(InitializeMT7Model(shenLoadFile(directory, name, PROP_MODEL_STORAGE)));
}

void ReleasePropReader(std::uint32_t index) {
    MT7Model *model = 0;
    if (index < 17 && g_propReaderSlots[index].reader)
        model = g_propReaderSlots[index].reader;
    RemovePropReader(index);
    ReleaseMT7Model(model);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LoadPROPs(std::int32_t act, shen_name area) {
    g_propState.root = 0;
    g_propState.position.x = g_propState.position.y = g_propState.position.z = 0.0f;
    g_propState.flagWind = 0;
    g_propState.lightingTraversalResult = 0;
    ResetPropReaderSlots();
    char directory[64];
    std::sprintf(directory, "scene/%02d/%c%c%c%c", static_cast<int>(act), static_cast<char>(area), static_cast<char>(area >> 8U),
        static_cast<char>(static_cast<std::int32_t>(area << 8U) >> 24), static_cast<char>(static_cast<std::int32_t>(area) >> 24));
    char filename[16];
    std::memcpy(filename, "PROP.MT7", sizeof("PROP.MT7"));
    std::int32_t index = 0;
    while (shenOpenFile(directory, filename)) {
        LoadPROP(directory, filename);
        ++index;
        const std::int32_t tens = index / 10;
        filename[4] = static_cast<char>(tens + '0');
        filename[5] = static_cast<char>(index - tens * 10 + '0');
        filename[6] = '\0';
        std::strcat(filename, ".MT7");
    }
    UpdatePropRoot();
}
#else
// @verified - d
void LoadPROPs(std::int32_t act, shen_name area) {
    g_propState.root = 0;
    g_propState.position = __typeof__(g_propState.position)();
    g_propState.flagWind = 0;
    g_propState.lightingTraversalResult = 0;
    ResetPropReaderSlots();
    char directory[64];
    std::sprintf(directory, "scene/%02d/%c%c%c%c", static_cast<int>(act), static_cast<char>(area), static_cast<char>(area >> 8U),
        static_cast<char>(area >> 16U), static_cast<char>(area >> 24U));
    char filename[12] = "PROP.MT7";
    std::int8_t index = 0;
    while (shenOpenFile(directory, filename)) {
        LoadPROP(directory, filename);
        index = static_cast<std::int8_t>(static_cast<std::uint8_t>(static_cast<std::uint8_t>(index) + 1U));
        filename[4] = static_cast<char>(index / 10 + '0');
        filename[5] = static_cast<char>(index % 10 + '0');
        filename[6] = '\0';
        std::strcat(filename, ".MT7");
    }
    UpdatePropRoot();
}
#endif

// @verified - d
MT7Node *GetPropRoot() {
    return g_propState.root;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetPropVector3(const Vector3 *position) {
    g_propState.position = *position;
}
#endif

// @verified - d
void GetPropVector4(Vector3 *position) {
    *position = g_propState.position;
}

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetPropHierarchyDisplayFlag(MT7Node *node, std::uint32_t enabled) {
    if (!node)
        return;
    if (enabled)
        node->kind = 0;
    else
        node->kind = 1;
    if (node->child)
        SetPropHierarchyDisplayFlag(node->child, enabled);
    if (node->sibling)
        SetPropHierarchyDisplayFlag(node->sibling, enabled);
}
#endif

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetDisplayFlagForProp(shen_name name, std::uint32_t enabled) {
    if (!g_propState.root)
        return;
    MT7Node *const node = FindModelCharacterRecursive(g_propState.root, name);
    if (!node)
        return;
    if (enabled)
        node->kind = 0;
    else
        node->kind = 1;
    if (node->child)
        SetPropHierarchyDisplayFlag(node->child, enabled);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t HasPropCharacterZeroUpperSignature(shen_name name) {
    std::uint32_t result = 0;
    MT7Node *const root = g_propState.root;
    if (root) {
        const MT7Node *const node = FindModelCharacterRecursive(root, name);
        if (node)
            result = node->kind == 0;
    }
    return result;
}
#endif

// @verified - d
MT7Node *GetMapModelRoot(std::int32_t index) {
    if (!g_mapModelSlots[index].reader)
        return 0;
    return g_mapModelSlots[index].reader->root;
}

// @verified - d
const MapModelClipRecord *GetMapModelClipRecords(std::int32_t index) {
    return g_mapModelSlots[index].clipRecords;
}

// @verified - d
void SetMapModelClipRecords(MapModelClipRecord *value, std::int32_t index) {
    if (index < 32)
        g_mapModelSlots[index].clipRecords = value;
}

// @verified - d
MT7Model *GetMapModelReader(std::int32_t index) {
    return g_mapModelSlots[index].reader;
}

// @verified - d
void SetMapModelReader(MT7Model *model, std::int32_t index) {
    if (index < 32)
        g_mapModelSlots[index].reader = model;
}

// @verified - d
std::uint32_t GetMapModelGridCullingEnabled(std::int32_t index) {
    return g_mapModelSlots[index].gridCullingEnabled;
}

// @verified - d
void SetMapModelGridCullingEnabled(std::uint32_t enabled, std::int32_t index) {
    g_mapModelSlots[index].gridCullingEnabled = enabled;
}

// @verified - d
std::uint32_t GetMapModelEnabled(std::int32_t index) {
    return g_mapModelSlots[index].enabled;
}

// @verified - d
void SetMapModelEnabled(std::uint32_t enabled, std::int32_t index) {
    if (index < 32)
        g_mapModelSlots[index].enabled = enabled;
}

// @verified - d
float GetMapModelHeight(std::int32_t index) {
    return g_mapModelSlots[index].position.y;
}

// @verified - d
void SetMapModelHeight(std::int32_t index, float height) {
    if (index < 32)
        g_mapModelSlots[index].position.y = height;
}

// @verified - d
void GetMapModelPosition(Vector3 *position, std::int32_t index) {
    position->x = g_mapModelSlots[index].position.x;
    position->y = g_mapModelSlots[index].position.y;
    position->z = g_mapModelSlots[index].position.z;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetMapModelSlotPosition(const Vector3 *position, std::int32_t index) {
    if (index >= 32)
        return;
    g_mapModelSlots[index].position.x = position->x;
    g_mapModelSlots[index].position.y = position->y;
    g_mapModelSlots[index].position.z = position->z;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t SetMapModelSlotRotation(std::int32_t rotationY, std::int32_t index) {
    const std::uint32_t offset = static_cast<std::uint32_t>(index) * static_cast<std::uint32_t>(sizeof(MapModelRuntimeSlot));
    *reinterpret_cast<std::uint32_t *>(reinterpret_cast<char *>(&g_mapModelSlots[0].rotationY) + offset) = static_cast<std::uint32_t>(rotationY);
    return offset;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t SetMapModelSlotTransform(std::int32_t index, const Vector3 *position, std::int32_t rotationY) {
    SetMapModelSlotPosition(position, index);
    return SetMapModelSlotRotation(rotationY, index);
}
#endif

// @verified - d
std::uint32_t GetMapModelRotationY(std::int32_t index) {
    return g_mapModelSlots[index].rotationY;
}

// @verified - d
float GetMapModelAlpha(std::int32_t index) {
    return g_mapModelSlots[index].alpha;
}

// @verified - d
void SetMapModelAlpha(std::int32_t index, float alpha) {
    if (alpha > 1.0f)
        alpha = 1.0f;
    if (std::fabs(alpha) < 0.00000011920929f)
        alpha = 0.0f;
    g_mapModelSlots[index].alpha = alpha;
}

// @verified - d
std::uint32_t GetMapModelWeatherLightingOverride(std::int32_t index) {
    return g_mapModelSlots[index].weatherLightingOverride;
}

// @verified - d
void EV_AssignLightParams(std::uint32_t value, std::int32_t index) {
    g_mapModelSlots[index].weatherLightingOverride = value;
}

// @verified - d
std::int32_t ReadMapPackCount(const MapInfoChunk *chunk) {
    if (!chunk)
        return 0;
    std::int32_t count = *reinterpret_cast<const std::int32_t *>(chunk + 1);
    if (count > 31)
        count = 31;
    if (count <= -1)
        count = -1;
    return count;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const shen_name *ReadMapAreaOverride(const MapInfoChunk *chunk) {
    if (!chunk)
        return 0;
    const char *const source = reinterpret_cast<const char *>(chunk);
    const char *const first = source + sizeof(MapInfoChunk);
    char *const area = reinterpret_cast<char *>(&g_mapAreaOverride);
    area[0] = *first;
    area[1] = source[sizeof(MapInfoChunk) + 1];
    area[2] = source[sizeof(MapInfoChunk) + 2];
    area[3] = source[sizeof(MapInfoChunk) + 3];
    return &g_mapAreaOverride;
}
#else
// @verified - d
const shen_name *ReadMapAreaOverride(const MapInfoChunk *chunk) {
    if (!chunk)
        return 0;
    std::memcpy(&g_mapAreaOverride, chunk + 1, sizeof(g_mapAreaOverride));
    return &g_mapAreaOverride;
}
#endif

// @verified - d
void ReadMapBlockSize(const MapInfoChunk *chunk) {
    float size;
    if (!chunk)
        size = 45.0f;
    else
        size = *reinterpret_cast<const float *>(chunk + 1);
    SetMapCellSize(size);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool ReadEntryMapLayers(const MapInfoChunk *chunk) {
    if (!chunk)
        return false;
    std::int32_t index = 0;
    std::int32_t lastLayer = 31;
    std::int32_t *layers = g_mapLoadState.layers;
    std::int32_t emptyLayer = -1;
    do {
        layers[index] = emptyLayer;
    } while (++index <= lastLayer);
    const std::int32_t *cursor = reinterpret_cast<const std::int32_t *>(chunk + 1);
    for (;;) {
        if (*cursor == g_entryPoint) {
            ++cursor;
            while (*cursor != -1)
                ++cursor;
            ++cursor;
            index = 0;
            while (*cursor != -1)
                g_mapLoadState.layers[index++] = *cursor++;
            layers = g_mapLoadState.layers;
            layers[index] = -1;
            return true;
        }
        if (*cursor == -1)
            return false;
        ++cursor;
        if (*cursor == -1) {
            ++cursor;
            while (*cursor != -1)
                ++cursor;
            ++cursor;
        }
    }
}
#else
// @verified - d
bool ReadEntryMapLayers(const MapInfoChunk *chunk) {
    if (!chunk)
        return false;
    for (std::size_t rangeIndex105 = 0; rangeIndex105 < sizeof(g_mapLoadState.layers) / sizeof((g_mapLoadState.layers)[0]); ++rangeIndex105) {
        std::int32_t &layer = (g_mapLoadState.layers)[rangeIndex105];
        layer = -1;
    }
    const std::int32_t *cursor = reinterpret_cast<const std::int32_t *>(chunk + 1);
    while (*cursor != g_entryPoint) {
        if (*cursor == -1)
            return false;
        ++cursor;
        if (*cursor == -1) {
            ++cursor;
            while (*cursor != -1)
                ++cursor;
            ++cursor;
        }
    }
    while (*cursor != -1)
        ++cursor;
    ++cursor;
    std::size_t offset = offsetof(MapLoadState, layers);
    while (*cursor != -1) {
        std::memcpy(reinterpret_cast<std::byte *>(&g_mapLoadState) + offset, cursor, sizeof(*cursor));
        ++cursor;
        offset += sizeof(*cursor);
    }
    std::memcpy(reinterpret_cast<std::byte *>(&g_mapLoadState) + offset, cursor, sizeof(*cursor));
    return true;
}
#endif

// @verified - d
void ReadMapDisabledLayers(const MapInfoChunk *chunk) {
    if (chunk) {
        const std::int32_t *cursor = reinterpret_cast<const std::int32_t *>(chunk + 1);
        while (*cursor != -1)
            SetMapModelEnabled(0, *cursor++);
    }
}

// @verified - d
std::int32_t ReadMapPackageMode(const MapInfoChunk *chunk) {
    if (!chunk)
        return 0;
    return *reinterpret_cast<const std::int32_t *>(chunk + 1);
}

// @verified - d
std::uint32_t GetMapModelCommandCellCount(std::int32_t index) {
    return g_mapModelSlots[index].commandCellCount;
}

// @verified - d
void SetMapModelCommandCellCount(std::uint32_t value, std::int32_t index) {
    if (index < 32)
        g_mapModelSlots[index].commandCellCount = value;
}

// @verified - d
MapModelCommandCell *GetMapModelCommandCells(std::int32_t index) {
    return g_mapModelSlots[index].commandCells;
}

// @verified - d
void SetMapModelCommandCells(MapModelCommandCell *value, std::int32_t index) {
    if (index < 32)
        g_mapModelSlots[index].commandCells = value;
}

// @verified - d
MapModelClipRecord **GetMapModelCommandRouting(std::int32_t index) {
    return g_mapModelSlots[index].commandRouting;
}

// @verified - d
void SetMapModelCommandRouting(MapModelClipRecord **value, std::int32_t index) {
    if (index < 32)
        g_mapModelSlots[index].commandRouting = value;
}

// @verified - d
std::uint32_t GetMapModelCommandRoutingEnabled(std::int32_t index) {
    return g_mapModelSlots[index].usesCommandRouting;
}

// @verified - d
void SetMapModelCommandRoutingEnabled(std::uint32_t enabled, std::int32_t index) {
    if (index < 32)
        g_mapModelSlots[index].usesCommandRouting = enabled;
}

// @verified - d
std::uint32_t GetMapModelAlternateTraversalEnabled(std::int32_t index) {
    return g_mapModelSlots[index].alternateTraversalEnabled;
}

// @verified - d
void SetMapModelAlternateTraversalEnabled(std::uint32_t enabled, std::int32_t index) {
    if (index < 32)
        g_mapModelSlots[index].alternateTraversalEnabled = enabled;
}

// @verified - d
std::uint32_t GetMapModelDrawableRecordCount(std::int32_t index) {
    return g_mapModelSlots[index].drawableRecordCount;
}

// @verified - d
void SetMapModelDrawableRecordCount(std::uint32_t value, std::int32_t index) {
    g_mapModelSlots[index].drawableRecordCount = value;
}

// @verified - d
std::uint32_t GetMapModelDangerRecordCount(std::int32_t index) {
    return g_mapModelSlots[index].dangerRecordCount;
}

// @verified - d
void SetMapModelDangerRecordCount(std::uint32_t value, std::int32_t index) {
    g_mapModelSlots[index].dangerRecordCount = value;
}

// @verified - d
std::uint32_t GetMapModelScreenSpaceRecordCount(std::int32_t index) {
    return g_mapModelSlots[index].screenSpaceRecordCount;
}

// @verified - d
void SetMapModelScreenSpaceRecordCount(std::uint32_t value, std::int32_t index) {
    g_mapModelSlots[index].screenSpaceRecordCount = value;
}

// @verified - d
std::uint32_t GetMapModelNearRecordCount(std::int32_t index) {
    return g_mapModelSlots[index].nearRecordCount;
}

// @verified - d
void SetMapModelNearRecordCount(std::uint32_t value, std::int32_t index) {
    g_mapModelSlots[index].nearRecordCount = value;
}

// @verified - d
std::uint32_t GetMapModelFarRecordCount(std::int32_t index) {
    return g_mapModelSlots[index].farRecordCount;
}

// @verified - d
void SetMapModelFarRecordCount(std::uint32_t value, std::int32_t index) {
    g_mapModelSlots[index].farRecordCount = value;
}

// @verified - d
std::uint32_t GetMapModelType4RecordCount(std::int32_t index) {
    return g_mapModelSlots[index].type4RecordCount;
}

// @verified - d
void SetMapModelType4RecordCount(std::uint32_t value, std::int32_t index) {
    g_mapModelSlots[index].type4RecordCount = value;
}

// @verified - d
std::uint32_t GetMapModelWallRecordCount(std::int32_t index) {
    return g_mapModelSlots[index].wallRecordCount;
}

// @verified - d
void SetMapModelWallRecordCount(std::uint32_t value, std::int32_t index) {
    g_mapModelSlots[index].wallRecordCount = value;
}

// @verified - d
std::uint32_t GetMapModelWallCRecordCount(std::int32_t index) {
    return g_mapModelSlots[index].wallCRecordCount;
}

// @verified - d
void SetMapModelWallCRecordCount(std::uint32_t value, std::int32_t index) {
    g_mapModelSlots[index].wallCRecordCount = value;
}

// @verified - d
std::uint32_t GetMapLightingPlacementActive(std::int32_t index) {
    return g_mapLoadState.placements[index].active;
}

// @verified - d
void SetMapLightingPlacementActive(std::uint32_t value, std::int32_t index) {
    if (index < 32)
        g_mapLoadState.placements[index].active = value;
}

// @verified - d
std::uint32_t GetMapLightingPlacementEnabled(std::int32_t index) {
    return g_mapLoadState.placements[index].enabled;
}

// @verified - d
void SetMapLightingPlacementEnabled(std::int32_t value, std::int32_t index) {
    if (index < 32)
        g_mapLoadState.placements[index].enabled = value;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t GetMapLightingPlacementPosition(Vector3 *output, std::int32_t index) {
    output->x = g_mapLoadState.placements[index].position.x;
    output->y = g_mapLoadState.placements[index].position.y;
    output->z = g_mapLoadState.placements[index].position.z;
    return static_cast<std::uint32_t>(index) * sizeof(MapLightingPlacement);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMapLightingPlacementPosition(const Vector3 *position, std::int32_t index) {
    if (index > 31)
        return;
    g_mapLoadState.placements[index].position.x = position->x;
    g_mapLoadState.placements[index].position.y = position->y;
    g_mapLoadState.placements[index].position.z = position->z;
}

#endif

// @verified - d
void SetMapLightingPlacementRotation(std::int32_t value, std::int32_t index) {
    g_mapLoadState.placements[index].rotationY = value;
}

// @verified - d
std::uint32_t GetMapLightingPlacementRotation(std::int32_t index) {
    return g_mapLoadState.placements[index].rotationY;
}

// @verified - d
std::uint32_t GetMagicRoomFlag(std::int32_t index) {
    return g_mapLoadState.placements[index].mapSlot;
}

// @verified - d
void AttachMapLightingPlacement(std::int32_t mapSlot, std::int32_t placement) {
    if (mapSlot < 0 || !GetMapModelRoot(mapSlot))
        mapSlot = 0x7fff;
    g_mapLoadState.placements[placement].mapSlot = static_cast<std::uint32_t>(mapSlot);
}

// @verified - d
std::int32_t GetMapModelReaderMask() {
    std::uint32_t mask = 0;
    for (std::int32_t index = 0; index < 32; ++index) {
        if (g_mapModelSlots[index].reader)
            mask |= 1U << index;
    }
    return mask;
}

// @verified - d
bool HasMapModelReader(std::int32_t index) {
    if (g_mapModelSlots[index].reader)
        return true;
    return false;
}

// @verified - d
void AllocateGeometryLightPositionScratch() {
    savedLightPositions = static_cast<Vector3 *>(HLib::AllocateTaskData(0xc00, TASK_LGTP));
    if (!savedLightPositions)
        return;
    Vector3 root;
    GetLightRootPosition(&root);
    std::int32_t index = 0;
    Vector3 *saved = savedLightPositions;
    for (; index < 256; ++index, ++saved) {
        if (GetLightRuntimeKind(index) == 4)
            continue;
        *saved = g_lightRuntimeEntries[index].vector20;
        g_lightRuntimeEntries[index].vector20.x += root.x;
        g_lightRuntimeEntries[index].vector20.y += root.y;
        g_lightRuntimeEntries[index].vector20.z += root.z;
    }
}

// @verified - d
void RestoreGeometryLightPositionScratch() {
    if (!savedLightPositions)
        return;
    Vector3 root;
    GetLightRootPosition(&root);
    std::int32_t index;
    const Vector3 *saved;
    for (index = 0, saved = savedLightPositions; index < 256; ++index, ++saved) {
        if (GetLightRuntimeKind(index) != 4) {
            LightRuntimeEntry *light = g_lightRuntimeEntries;
            Vector3 *dst = &light[index].vector20;
            *dst = *saved;
        }
    }
    HLib::FreeTaskData(savedLightPositions);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t AssignLightForMapLayer(Vector3 *output, std::int32_t mapSlot) {
    for (std::int32_t index = 0; index < 32; ++index) {
        if (GetMagicRoomFlag(index) == static_cast<std::uint32_t>(mapSlot)) {
            GetMapLightingPlacementPosition(output, index);
            return 1;
        }
    }
    GetMapModelPosition(output, mapSlot);
    return 1;
}
#endif

// @verified - d
void SetupLightConfig() {
    Vector4 colour;
    if (GetGeometryLightColour(&colour))
        return;
    AllocateGeometryLightPositionScratch();
    MT7Node *const root = GetPropRoot();
    if (root) {
        Vector3 position;
        GetPropVector4(&position);
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&position);
        const std::uint32_t result = ApplyGeometryLightTree(root, g_lightRuntimeEntries, 256, &colour);
        PopCurrentMatrix(1);
        g_propState.lightingTraversalResult = result;
    }
    RestoreGeometryLightPositionScratch();
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetDefaultVSByIndex(std::int32_t index) {
    if (index >= 32)
        return;
    MT7Node *const root = GetMapModelRoot(index);
    if (root) {
        SetupModelDataDefaultVS(root);
        g_mapModelSlots[index].renderMode = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetMapModelDataDefaultVertexShader() {
    for (std::int32_t index = 0; index < 32; ++index)
        SetDefaultVSByIndex(index);
    MT7Node *const root = GetPropRoot();
    if (root) {
        SetupModelDataDefaultVS(root);
        g_propState.lightingTraversalResult = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetDefaultVS() {
    MT7Node *const root = GetPropRoot();
    if (root) {
        SetupModelDataDefaultVS(root);
        g_propState.lightingTraversalResult = 0;
    }
}
#endif

// @verified - d
std::uint32_t GetMapModelDefaultVSIndex(std::uint32_t index) {
    if (index <= 31)
        return g_mapModelSlots[index].renderMode;
    return 0;
}

// @verified - d
std::uint32_t GetPropLightingTraversalResult() {
    return g_propState.lightingTraversalResult;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t SetMapModelVertexShaderType(std::int32_t index, std::int32_t value) {
    const std::uint32_t offset = static_cast<std::uint32_t>(index) * static_cast<std::uint32_t>(sizeof(MapModelRuntimeSlot));
    *reinterpret_cast<std::uint32_t *>(reinterpret_cast<char *>(&g_mapModelSlots[0].drawMode) + offset) = static_cast<std::uint32_t>(value);
    return offset;
}
#endif

// @verified - d
std::uint32_t GetMapModelVertexShaderType(std::int32_t index) {
    return g_mapModelSlots[index].drawMode;
}

// @verified - d
void UpdateWindForFlag() {
    if (g_propState.flagWind)
        UpdateWindForModel(g_propState.flagWind);
}

// @verified - d
void BakeMapNodeVertices(MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    Matrix4x4::SetViewMatrixFromEuler(node->rotation.x, node->rotation.y, node->rotation.z);
    Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    if (node->mesh) {
        std::uint32_t *cursor;
        if (!(node->kind & 0x20) && !(reinterpret_cast<MT7MeshHeader *>(cursor = static_cast<std::uint32_t *>(node->mesh))->flags & 0x40U)) {
            const MapModelClipRecord *const record = &g_mapVertexCacheRecords[g_mapVertexCacheRecordIndex];
            const std::uint32_t type = record->commandType;
            if (type != 0xffU && type != 0xf0U && type != 0xf4U && reinterpret_cast<MT7MeshHeader *>(cursor)->state >= 0 &&
                reinterpret_cast<MT7MeshMaterialCommand *>(reinterpret_cast<MT7MeshHeader *>(cursor) + 1)->flags &&
                *reinterpret_cast<std::int32_t *>(&reinterpret_cast<MT7MeshMaterialCommand *>(reinterpret_cast<MT7MeshHeader *>(cursor) + 1)->vertexFormat) !=
                    -3) {
                reinterpret_cast<MT7MeshHeader *>(cursor)->flags |= 0x40U;
                Vector3 point;
                cursor += offsetof(MT7MeshHeader, centre) / sizeof(*cursor);
                Matrix4x4::TransformPoint(*reinterpret_cast<Vector3 *>(cursor), point);
                *reinterpret_cast<Vector3 *>(cursor) = point;
                cursor += (sizeof(MT7MeshHeader) - offsetof(MT7MeshHeader, centre)) / sizeof(*cursor);
                std::int32_t primitive = static_cast<std::int32_t>(*cursor);
                if (primitive)
                    do {
                        if (primitive < 0) {
                            cursor += offsetof(MT7MeshMaterialCommand, centre) / sizeof(*cursor);
                            Matrix4x4::TransformPoint(*reinterpret_cast<Vector3 *>(cursor), point);
                            *reinterpret_cast<Vector3 *>(cursor) = point;
                            cursor += (sizeof(MT7MeshMaterialCommand) - offsetof(MT7MeshMaterialCommand, centre)) / sizeof(*cursor);
                            primitive = static_cast<std::int32_t>(*cursor);
                        }
                        cursor++;
                        std::uint32_t count = *cursor++;
                        if (primitive & MT7PrimitiveTriangleList)
                            count *= 3U;
                        while (count-- > 0U) {
                            if (*cursor & 1U) {
                                Vector3 normal;
                                Matrix4x4::TransformPoint(reinterpret_cast<MT7PacketVertex *>(cursor)->position, point);
                                Matrix4x4::Transform(reinterpret_cast<MT7PacketVertex *>(cursor)->normal.floating, normal);
                                const Vector3 *cached = g_mapVertexCacheVertices;
                                std::int32_t index;
                                for (index = 0; index < g_mapVertexCacheVertexCount; ++index, ++cached) {
                                    float delta = cached->x - point.x;
                                    float squared = delta * delta;
                                    if (squared < MAP_VERTEX_CACHE_DISTANCE_SQUARED) {
                                        delta = cached->y - point.y;
                                        squared += delta * delta;
                                        if (squared < MAP_VERTEX_CACHE_DISTANCE_SQUARED) {
                                            delta = cached->z - point.z;
                                            squared += delta * delta;
                                            if (squared < MAP_VERTEX_CACHE_DISTANCE_SQUARED) {
                                                index = -10;
                                                point = *cached;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (index >= 0 && g_mapVertexCacheVertexCount < MAP_VERTEX_CACHE_CAPACITY)
                                    g_mapVertexCacheVertices[g_mapVertexCacheVertexCount++] = point;
                                reinterpret_cast<MT7PacketVertex *>(cursor)->position = point;
                                reinterpret_cast<MT7PacketVertex *>(cursor)->normal.floating = normal;
                                *cursor |= 1U;
                                cursor += sizeof(MT7PacketVertex) / sizeof(*cursor);
                            } else {
                                cursor += 2;
                            }
                        }
                    } while ((primitive = static_cast<std::int32_t>(*cursor)) != 0);
            }
        }
        ++g_mapVertexCacheRecordIndex;
    }
    if (node->child)
        BakeMapNodeVertices(node->child);
    if (node->sibling) {
        PopCurrentMatrix(1);
        BakeMapNodeVertices(node->sibling);
    }
}

// @verified - d
void MarkSharedMapMesh(MT7Node *owner, MT7Node *node) {
    if (owner->mesh == node->mesh && (node->kind & 0x10)) {
        owner->kind |= 0x20;
        node->kind |= 0x20;
    }
    if (node->child)
        MarkSharedMapMesh(owner, node->child);
    if (node->sibling)
        MarkSharedMapMesh(owner, node->sibling);
}

// @verified - d
void MarkSharedMapMeshForced(MT7Node *owner, MT7Node *node) {
    if (owner->mesh == node->mesh) {
        owner->kind |= 0x20;
        node->kind |= 0x20;
    }
    if (node->child)
        MarkSharedMapMeshForced(owner, node->child);
    if (node->sibling)
        MarkSharedMapMeshForced(owner, node->sibling);
}

// @verified - d
void MarkSharedMapMeshes(MT7Node *node) {
    if (node->mesh) {
        if (node->kind & 0x10) {
            if (node->child)
                MarkSharedMapMeshForced(node, node->child);
            if (node->sibling)
                MarkSharedMapMeshForced(node, node->sibling);
        } else {
            if (node->child)
                MarkSharedMapMesh(node, node->child);
            if (node->sibling)
                MarkSharedMapMesh(node, node->sibling);
        }
    }
    if (node->child)
        MarkSharedMapMeshes(node->child);
    if (node->sibling)
        MarkSharedMapMeshes(node->sibling);
}

// @verified - d
void InitializeMapVertexCache() {
    if (!g_mapVertexCacheEnabled)
        return;
    g_mapVertexCacheVertices = static_cast<Vector3 *>(HLib::AllocateTaskData(MAP_VERTEX_CACHE_CAPACITY * sizeof(Vector3), MAP_VERTEX_CACHE_STORAGE));
    if (!g_mapVertexCacheVertices)
        return;
    g_mapVertexCacheVertexCount = 0;
    for (std::int32_t index = 0; index < 32; ++index) {
        MT7Node *const root = GetMapModelRoot(index);
        if (root) {
            MarkSharedMapMeshes(root);
            matrix_stack::PushUnitMatrix();
            g_mapVertexCacheRecordIndex = 0;
            g_mapVertexCacheRecords = GetMapModelClipRecords(index);
            BakeMapNodeVertices(root);
            PopCurrentMatrix(1);
        }
    }
    HLib::FreeTaskData(g_mapVertexCacheVertices);
}

void MapVertexCacheNullsub() {
}

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" {
std::uint32_t __attribute__((renesas)) shenmue_s2_event_SetMapModelSlotTransform(std::int32_t index, const shenmue::Vector3 *position, std::int32_t rotationY) {
    return shenmue::s2::SetMapModelSlotTransform(index, position, rotationY);
}
std::uint32_t __attribute__((renesas)) shenmue_s2_event_SetMapModelVertexShaderType(std::int32_t index, std::int32_t value) {
    return shenmue::s2::SetMapModelVertexShaderType(index, value);
}
void __attribute__((renesas)) shenmue_s2_event_SetDefaultVSByIndex(std::int32_t index) {
    shenmue::s2::SetDefaultVSByIndex(index);
}
void __attribute__((renesas)) shenmue_s2_event_SetMapModelDataDefaultVertexShader() {
    shenmue::s2::SetMapModelDataDefaultVertexShader();
}
}
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" {
void __attribute__((renesas)) shenmue_s2_event_SetPropVector3(const shenmue::Vector3 *position) {
    shenmue::s2::SetPropVector3(position);
}
void __attribute__((renesas)) shenmue_s2_event_SetDisplayFlagForProp(std::uint32_t name, std::uint32_t enabled) {
    shenmue::s2::SetDisplayFlagForProp(shenmue::shen_name(name), enabled);
}
std::uint32_t __attribute__((renesas)) shenmue_s2_event_HasPropCharacterZeroUpperSignature(std::uint32_t name) {
    return shenmue::s2::HasPropCharacterZeroUpperSignature(shenmue::shen_name(name));
}
}
#endif

#endif
