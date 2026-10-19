#pragma once

#include "common/types.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

struct MapInfoChunk;
struct MT7Model;
struct MT7Node;
struct MapRenderProfile {
    std::uint32_t areaPattern;
    std::uint32_t field04;
    std::uint32_t field08;
    std::uint32_t field0c;
    std::uint32_t field10;
    float farDistance;
    float nearDistance;
    float npcRadius;
    std::uint32_t alphaOperation;
    float textureDistance;
    float textureDistanceW;
    float textureDistanceZ;
};
static_assert(sizeof(MapRenderProfile) == 0x30);
static_assert(offsetof(MapRenderProfile, farDistance) == 0x14);
static_assert(offsetof(MapRenderProfile, nearDistance) == 0x18);
static_assert(offsetof(MapRenderProfile, npcRadius) == 0x1c);

const MapRenderProfile &MatchMapRenderProfile(shen_name area);

struct MainMapProjectionAngles {
    std::int32_t fixedAngle{};
    std::int32_t degrees{};
    float sine{};
    float tangent{};
};

static_assert(sizeof(MainMapProjectionAngles) == 0x10);

// AllocateCLIP_WALL_WALC_DANG builds these from MT7 nodes,
// using the authored XB01 sphere at +0x08/+0x14, not a file CLIP chunk.
struct MapModelClipRecord {
    Vector3 centre{};
    float radius{};       // +0x0c, scaled by max node scale
    std::int16_t cellX{};
    std::int16_t cellZ{};
    std::uint32_t reserved14{};


    const void *object{};
    std::uint32_t commandType{};
    float modelScale{};
};

struct MapCellOffset {
    std::int16_t x{};
    std::int16_t z{};
};

// BuildMapModelCommandCells groups records by first-seen cell, swaps the 0x4fff cell
// with the first cell, and preserves node traversal order within each run.
struct MapModelCommandCell {
    std::int16_t x{};
    std::int16_t z{};
    std::int32_t count{};
    std::int32_t firstRecord{};
};

static_assert(sizeof(MapModelClipRecord) == 0x28);
static_assert(offsetof(MapModelClipRecord, cellX) == 0x10);
static_assert(offsetof(MapModelClipRecord, object) == 0x18);
static_assert(offsetof(MapModelClipRecord, commandType) == 0x20);
static_assert(sizeof(MapModelCommandCell) == 0x0c);

inline constexpr std::array<std::uint32_t, 12> mapModelCommandTypes{0, 0, 255, 240, 4, 0, 8, 0, 244, 16, 0, 0};

struct MapFieldDepthWall {
    Vector3 first{};
    Vector3 second{};
};



struct MapFieldDepthSlot {
    Vector4 *dangerSpheres{};       // +0x00 (DANG)
    std::int32_t dangerCount{};
    std::uint32_t enabled{};
    std::int32_t dangerCellCount{};
    std::uint32_t reserved14{};
    MapModelCommandCell *dangerCells{}; // +0x18 (DAPH)
    MapFieldDepthWall *walls{};         // +0x20 (WALL)
    std::int32_t wallCount{};
    std::uint32_t reserved2c{};
    MapFieldDepthWall *cameraWalls{}; // +0x30 (WALC)
    std::int32_t cameraWallCount{};
    std::uint32_t reserved3c{};
};

static_assert(sizeof(MapCellOffset) == 4);
static_assert(sizeof(MapFieldDepthWall) == 0x18);
static_assert(sizeof(MapFieldDepthSlot) == 0x40);
static_assert(offsetof(MapFieldDepthSlot, enabled) == 0x0c);
static_assert(offsetof(MapFieldDepthSlot, dangerCells) == 0x18);
static_assert(offsetof(MapFieldDepthSlot, walls) == 0x20);
static_assert(offsetof(MapFieldDepthSlot, cameraWalls) == 0x30);





struct MapModelRuntimeSlot {
    std::uint32_t enabled{1};
    std::uint32_t reserved04{};
    MapModelClipRecord *clipRecords{}; // +0x08 (CLIP allocation)
    std::uint32_t ownsReader{};        // +0x10 (mtMarkFree on teardown)
    std::uint32_t reserved14{};
    const MT7Model *reader{};         // +0x18 (MT7_Reader_Struct*)
    std::uint32_t commandCellCount{};
    std::uint32_t reserved24{};
    MapModelCommandCell *commandCells{};       // +0x28 (ACPH allocation)
    MapModelClipRecord **commandRouting{};     // +0x30 (ACPD allocation)
    Vector3 position{};
    std::uint32_t rotationY{};
    std::uint32_t usesCommandRouting{1};
    std::uint32_t alternateTraversalEnabled{}; // +0x4c, IsMapModelAlternateTraversalActive
    std::uint32_t gridCullingEnabled{1};
    std::uint32_t drawableRecordCount{};       // +0x54, excludes 240/244/255
    std::uint32_t screenSpaceRecordCount{};    // +0x58, command type 16
    std::uint32_t nearRecordCount{};           // +0x5c, command type 8
    std::uint32_t dangerRecordCount{};         // +0x60, command type 255
    std::uint32_t farRecordCount{};            // +0x64, command type 12
    std::uint32_t type4RecordCount{};
    std::uint32_t wallRecordCount{};           // +0x6c, command type 240
    std::uint32_t wallCRecordCount{};          // +0x70, command type 244
    std::uint32_t reserved74{};
    void *auxiliaryAllocation{};             // +0x78, freed by FreeMapModelSlot
    std::uint32_t forceLambertShader{};
    std::uint32_t vertexShaderType{};
    float alpha{1.0f};
    std::uint32_t weatherLightingOverride{};
};

static_assert(sizeof(MapModelRuntimeSlot) == 0x90);
static_assert(offsetof(MapModelRuntimeSlot, clipRecords) == 0x08);
static_assert(offsetof(MapModelRuntimeSlot, ownsReader) == 0x10);
static_assert(offsetof(MapModelRuntimeSlot, reader) == 0x18);
static_assert(offsetof(MapModelRuntimeSlot, commandCellCount) == 0x20);
static_assert(offsetof(MapModelRuntimeSlot, commandCells) == 0x28);
static_assert(offsetof(MapModelRuntimeSlot, commandRouting) == 0x30);
static_assert(offsetof(MapModelRuntimeSlot, position) == 0x38);
static_assert(offsetof(MapModelRuntimeSlot, rotationY) == 0x44);
static_assert(offsetof(MapModelRuntimeSlot, usesCommandRouting) == 0x48);
static_assert(offsetof(MapModelRuntimeSlot, gridCullingEnabled) == 0x50);
static_assert(offsetof(MapModelRuntimeSlot, drawableRecordCount) == 0x54);
static_assert(offsetof(MapModelRuntimeSlot, wallCRecordCount) == 0x70);
static_assert(offsetof(MapModelRuntimeSlot, auxiliaryAllocation) == 0x78);
static_assert(offsetof(MapModelRuntimeSlot, forceLambertShader) == 0x80);
static_assert(offsetof(MapModelRuntimeSlot, vertexShaderType) == 0x84);
static_assert(offsetof(MapModelRuntimeSlot, weatherLightingOverride) == 0x8c);
static_assert(offsetof(MapModelRuntimeSlot, alpha) == 0x88);



struct LoadedMapNodeBinding {
    MT7Model *model{};
    MT7Node *node{};
    std::size_t slotIndex{};
    std::size_t nodeIndex{};
};


struct PropReaderSlot {
    std::int16_t previous{-1};
    std::int16_t next{-1};
    std::uint32_t reserved04{};
    MT7Model *reader{};
    MT7Node *lastSibling{};
};

static_assert(sizeof(PropReaderSlot) == 0x18);
static_assert(offsetof(PropReaderSlot, reader) == 0x08);
static_assert(offsetof(PropReaderSlot, lastSibling) == 0x10);

void GetPropVector4(Vector3 *output);

std::uint32_t GetPropLightingTraversalResult();

void SetPropVector3(const Vector3 *position);
MT7Node *GetPropRoot();
std::span<const PropReaderSlot, 17> GetPropReaderSlots();
std::uint64_t RegisterPropReader(MT7Model *reader);
void RemovePropReader(std::uint32_t index);

std::uint64_t RegisterPackagePropReader(MT7Model *reader);
void RemovePackagePropReader(std::uint32_t index);
void SetupModelForImageModule(MT7Node *root);
void InitializeObject(MT7Node *root);
void AllocatePropFlagObject();
void LoadPROPs(std::int32_t actNumber, shen_name areaName);


void RetirePropAssets();

inline std::array<std::int32_t, 34> mapEntryLayers{
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

bool Mapinfo_MAP_Reader(std::int32_t actNumber, shen_name areaName, std::int32_t entryPoint, void *mapInfo);
std::span<const std::byte> GetMainMapModelBytes();
std::span<const MT7Model> GetLoadedMapModels();
std::span<const MT7Node *const> GetMainMapProjectedModels();
bool FindLoadedMapNode(shen_name name, LoadedMapNodeBinding &binding);
bool ResolveLoadedMapNodeTransform(const LoadedMapNodeBinding &binding, Matrix4x4 &transform);
const MT7Node *const *GetMainMapProjectedModelEntry(std::uint32_t index);
float GetMainMapMinimumModelHeight();
void SetMainMapMinimumModelHeight(float height);
const MainMapProjectionAngles &GetMainMapProjectionAngles();
void SetMainMapProjectionDirection(const Vector3 &direction);
bool AlignMainMap();


void SetMainMapProjectedShadowDepth(float distance);
void SetProjectedShadowLightDirection(float distance);
void SetProjectedShadowDirection(Vector3 *direction, float distance);
bool BeginMainMapProjectedPlacementQueue();
bool QueueMainMapProjectedPlacements(std::span<MapModelClipRecord *const> records);
std::uint32_t DrawQueuedMainMapProjectedShadows();
std::span<const MapModelRuntimeSlot, 33> GetMapModelSlots();
std::span<const MapFieldDepthSlot, 33> GetMapFieldDepthSlots();
void SetMapModelLightingTraversalResult(std::int32_t index, std::uint32_t result);
void SetPropLightingTraversalResult(std::uint32_t result);
std::int64_t SetMapFieldDepthSlotEnabled(std::uint32_t index, std::uint32_t enabled);
const MapFieldDepthSlot *SetAllMapFieldDepthSlotsEnabled(std::uint32_t enabled);
void camCtrlCacheUtil(std::int32_t operation);
float GetMapCellScale();
std::span<const MapCellOffset> GetMapVisibleCellOffsets(std::uint32_t direction, std::uint32_t tier);
std::int32_t SetMapModelSlotEnabled(std::int32_t index, std::int32_t enabled);

void SetMapModelCommandRoutingEnabled(std::int32_t value, std::int32_t index);
std::uint64_t GetMapModelCommandRoutingEnabled(std::int32_t index);
std::uint64_t GetMapModelGridCullingEnabled(std::int32_t index);
std::uint32_t *SetMapModelGridCullingEnabled(std::int32_t value, std::int32_t index);
std::uint32_t *SetMapModelAlternateTraversalEnabled(std::int32_t value, std::int32_t index);
std::uint64_t HasMapModelReader(std::int32_t index);
MT7Node *GetMapModelRoot(std::int32_t index);
std::uint64_t GetMapModelReaderMask();
std::uint64_t HasPropCharacterZeroUpperSignature(shen_name name);
void SetDisplayFlagForProp(shen_name name, std::uint32_t enabled);


void SetMapModelSlotPosition(const Vector3 *position, std::int32_t index);
std::uint32_t *SetMapModelSlotRotation(std::int32_t rotationY, std::int32_t index);
std::uint32_t *SetMapModelSlotTransform(std::int32_t index, const Vector3 *position, std::int32_t rotationY);
void SetMapModelWeatherLightingOverride(std::int32_t index, std::int32_t value);
std::uint32_t *SetMapModelVertexShaderType(std::int32_t index, std::int32_t value);
void SetMapModelDataDefaultVertexShader();
void SetDefaultVSByIndex(std::int32_t index);
std::int32_t GetNextEmptyMapModelSlot();
bool AssignPackageMapModel(std::int32_t slot, MT7Model &model);
void RetirePackageMapModel(MT7Model &model);
// SNAP releases MAPS.MT7 around a capture and reloads it into slot 32.
void FreeMainMap();
bool LoadMainMapByFilepath(const char *folder);
MT7Model *GetMapsModel();
void SetMapModelReader(MT7Model *model, std::int32_t index);

void ResetMapsData();
void FreeMapAssets();

} // namespace shenmue::s2
