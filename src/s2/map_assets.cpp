#include "s2/map_assets.hpp"
#if defined(SHENMUE_ENHANCEMENT_S2_MAP_SEAMS) && SHENMUE_ENHANCEMENT_S2_MAP_SEAMS
#include "s2/enhancements/map_seams.hpp"
#endif
#include "logging.hpp"

#include "s2/archive_data.hpp"
#include "s2/camera.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_info.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/mt7.hpp"
#include "s2/pak.hpp"
#include "s2/search.hpp"
#include "s2/world_render.hpp"
#include "s2/weather.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace shenmue::s2 {
namespace {

#include "s2/map_cell_offsets.inl"
#include "s2/map_render_profiles.inl"

void* mainMapModelAllocation = nullptr;
std::size_t mainMapModelSize = 0;
std::vector<MT7Model> mapModels;
std::array<void*, 33> mapModelAllocations{};
std::array<std::size_t, 33> mapModelSlotIndices{};
std::array<bool, 33> mapModelSlotStartsHidden{};
std::array<MapModelRuntimeSlot, 33> mapModelSlots{};
std::array<MapFieldDepthSlot, 33> mapFieldDepthSlots{};
std::array<std::uint32_t, 33> cachedMapEnabled{};
std::array<std::uint32_t, 33> cachedMapGridCulling{};
std::array<std::uint32_t, 33> cachedMapFieldDepthEnabled{};
float cachedFarDistance{};
float cachedNearDistance{};
float cachedNpcRadius{};
float mapCellScale = 1.0f / 45.0f;
MapFieldDepthWall wallEndpoints{};
std::size_t mainMapModelIndex = (std::numeric_limits<std::size_t>::max)();
// LoadMainMap's fixed pointer table and 0x48-byte placement entries are
// separate owners. CLIP +0x18 borrows the address of a pointer-table entry.
struct MainMapProjectedPlacement {
    const MT7Node* const* modelEntry{};
    Matrix4x4 transform{};
};
static_assert(sizeof(MainMapProjectedPlacement) == 0x48);
static_assert(offsetof(MainMapProjectedPlacement, transform) == 0x08);
std::array<const MT7Node*, 200> mainMapProjectedModels{};
std::array<MainMapProjectedPlacement, 200> mainMapProjectedPlacements{};
std::uint32_t mainMapProjectedModelCount = 0;
std::uint32_t mainMapProjectedPlacementCount = 0;
float mainMapMinimumModelHeight = 100.0f;
MainMapProjectionAngles mainMapProjectionAngles{};
bool mainMapProjectionDirectionOverridden = false;
Vector3 mainMapProjectionDirection{};
float mainMapProjectedShadowDepth = (std::numeric_limits<float>::max)();
float mainMapProjectedExtrusionDistance = 100.0f;

std::array<PropReaderSlot, 17> propReaderSlots{};
MT7Node* propRoot{};
Vector4 propLightingVector{};


std::vector<std::unique_ptr<MT7Model>> propModels;

void ResetMainMapProjection() {
    ResetGlobalShadowTiming();
    mainMapProjectedModels.fill(nullptr);
    for (MainMapProjectedPlacement& placement : mainMapProjectedPlacements) {
        placement.modelEntry = nullptr;
    }
    mainMapProjectedModelCount = 0;
    mainMapProjectedPlacementCount = 0;
    mainMapMinimumModelHeight = 100.0f;
    mainMapProjectionDirectionOverridden = false;
    mainMapProjectionDirection = {};
    mainMapProjectedShadowDepth = (std::numeric_limits<float>::max)();
}

Matrix4x4 IdentityTransform() {
    return {{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
}

void PreTranslate(Matrix4x4& transform, const Vector3& translation) {
    transform.rows[3].x +=
        transform.rows[0].x * translation.x +
        transform.rows[1].x * translation.y +
        transform.rows[2].x * translation.z;
    transform.rows[3].y +=
        transform.rows[0].y * translation.x +
        transform.rows[1].y * translation.y +
        transform.rows[2].y * translation.z;
    transform.rows[3].z +=
        transform.rows[0].z * translation.x +
        transform.rows[1].z * translation.y +
        transform.rows[2].z * translation.z;
    transform.rows[3].w +=
        transform.rows[0].w * translation.x +
        transform.rows[1].w * translation.y +
        transform.rows[2].w * translation.z;
}

void RotateX(Matrix4x4& transform, std::uint16_t angle) {
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(
        fixed16_cosine_angle(angle)));
    const Vector4 row1 = transform.rows[1];
    const Vector4 row2 = transform.rows[2];
    for (std::size_t component = 0; component < 4; ++component) {
        transform.rows[1][component] =
            row2[component] * sine + row1[component] * cosine;
        transform.rows[2][component] =
            row2[component] * cosine - row1[component] * sine;
    }
}

void RotateY(Matrix4x4& transform, std::uint16_t angle) {
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(
        fixed16_cosine_angle(angle)));
    const Vector4 row0 = transform.rows[0];
    const Vector4 row2 = transform.rows[2];
    for (std::size_t component = 0; component < 4; ++component) {
        transform.rows[0][component] =
            row0[component] * cosine - row2[component] * sine;
        transform.rows[2][component] =
            row0[component] * sine + row2[component] * cosine;
    }
}

void RotateZ(Matrix4x4& transform, std::uint16_t angle) {
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(
        fixed16_cosine_angle(angle)));
    const Vector4 row0 = transform.rows[0];
    const Vector4 row1 = transform.rows[1];
    for (std::size_t component = 0; component < 4; ++component) {
        transform.rows[0][component] =
            row1[component] * sine + row0[component] * cosine;
        transform.rows[1][component] =
            row1[component] * cosine - row0[component] * sine;
    }
}

void ApplyLocalTransform(Matrix4x4& transform, const MT7Node& node) {
    PreTranslate(transform, node.position);
    if (node.rotation.z != 0.0f) {
        RotateZ(transform, static_cast<std::uint16_t>(
            static_cast<std::int32_t>(node.rotation.z)));
    }
    if (node.rotation.y != 0.0f) {
        RotateY(transform, static_cast<std::uint16_t>(
            static_cast<std::int32_t>(node.rotation.y)));
    }
    if (node.rotation.x != 0.0f) {
        RotateX(transform, static_cast<std::uint16_t>(
            static_cast<std::int32_t>(node.rotation.x)));
    }
    for (std::size_t component = 0; component < 4; ++component) {
        transform.rows[0][component] *= node.scale.x;
        transform.rows[1][component] *= node.scale.y;
        transform.rows[2][component] *= node.scale.z;
    }
}

bool ApplyNodeTransformPath(const MT7Model& model, const MT7Node& node,
                            Matrix4x4& transform) {
    // Runtime admission can splice a DOOR-owned motion child between borrowed
    // MAP nodes without changing their decoded-vector indices.
    if (&node != &model.nodes.front() && node.runtimeParent) {
        if (node.runtimeParent == &node ||
            !ApplyNodeTransformPath(model, *node.runtimeParent, transform)) {
            return false;
        }
    }
    ApplyLocalTransform(transform, node);
    return true;
}

// VisitTransformedMapNodeHierarchy preserves a transform only for nodes with a sibling; child
// recursion inherits the parent's active TRS. Fresh MAPS readers have no
// runtime alternate-transform allocation.
bool CollectMainMapProjectedModels(const MT7Model& model, std::size_t& index) {
    while (index < model.nodes.size()) {
        const std::size_t current = index++;
        const MT7Node& node = model.nodes[current];
        std::uint32_t siblingOffset{};
        std::memcpy(&siblingOffset,
                    model.source.data() + node.sourceOffset +
                        offsetof(MT7NodeSource, siblingOffset),
                    sizeof(siblingOffset));
        const bool sibling = siblingOffset != 0;
        if (sibling) Matrix4x4_Push();
        ApplyMT7NodeTransform(node);
        bool ready = true;
        if (node.xb01Offset) {


            if (mainMapProjectedModelCount < mainMapProjectedModels.size()) {
                mainMapProjectedModels[mainMapProjectedModelCount++] = &node;
            }
            ready = AccumulateProjectedModelMinimumHeight(
                model.source, node.xb01Offset, mainMapMinimumModelHeight);
        }
        if (ready && index < model.nodes.size() &&
            model.nodes[index].parent == static_cast<std::int32_t>(current)) {
            ready = CollectMainMapProjectedModels(model, index);
        }
        if (sibling) Matrix4x4::Pop(1);
        if (!ready) return false;
        if (!sibling) return true;
    }
    return true;
}

bool InitializeMainMapProjection(const MT7Model& model) {
    mainMapProjectedModels.fill(nullptr);
    mainMapProjectedModelCount = 0;
    mainMapMinimumModelHeight = (std::numeric_limits<float>::max)();
    Matrix4x4_PushActiveTransform();
    std::size_t index = 0;
    const bool ready = CollectMainMapProjectedModels(model, index);
    Matrix4x4::Pop(1);
    if (!ready) {
        ResetMainMapProjection();
        return false;
    }
    mainMapMinimumModelHeight =
        mainMapMinimumModelHeight == (std::numeric_limits<float>::max)()
            ? 100.0f : mainMapMinimumModelHeight + 3.0f;
    return true;
}


void FreeMapFieldDepthSlot(std::size_t index) {
    MapFieldDepthSlot& field = mapFieldDepthSlots[index];
    if (field.dangerSpheres) HLib::FreeTaskData(field.dangerSpheres);
    if (field.dangerCells) HLib::FreeTaskData(field.dangerCells);
    if (field.walls) HLib::FreeTaskData(field.walls);
    if (field.cameraWalls) HLib::FreeTaskData(field.cameraWalls);
    field = {};
}

// ApplyScalingRotationTranslation scales and rotates the
// X/Z segment about its midpoint; it does not scale the endpoint heights.
void PlaceFieldDepthWall(const MT7Node& node, float scale) {
    Vector3& first = wallEndpoints.first;
    Vector3& second = wallEndpoints.second;
    if (scale != 1.0f) {
        const float x = (second.x - first.x) * 0.5f + first.x;
        const float z = (second.z - first.z) * 0.5f + first.z;
        first.x = (first.x - x) * scale + x;
        first.z = (first.z - z) * scale + z;
        second.x = (second.x - x) * scale + x;
        second.z = (second.z - z) * scale + z;
    }
    const auto rotation = static_cast<std::int32_t>(node.rotation.y);
    if (rotation != 0) {
        const auto angle = static_cast<std::uint16_t>(-rotation);
        const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
        const float cosine = sin_fixed16(static_cast<std::int16_t>(
            fixed16_cosine_angle(angle)));
        const float x = (second.x - first.x) * 0.5f + first.x;
        const float z = (second.z - first.z) * 0.5f + first.z;
        const float firstX = first.x - x;
        const float firstZ = first.z - z;
        first.z = firstZ * cosine + firstX * sine + z;
        first.x = firstX * cosine - firstZ * sine + x;
        const float secondX = second.x - x;
        const float secondZ = second.z - z;
        second.z = secondZ * cosine + secondX * sine + z;
        second.x = secondX * cosine - secondZ * sine + x;
    }
    first.x += node.position.x;
    first.y += node.position.y;
    first.z += node.position.z;
    second.x += node.position.x;
    second.y += node.position.y;
    second.z += node.position.z;
}

bool SetupMapFieldDepth(std::size_t index, const MT7Model& model) {
    FreeMapFieldDepthSlot(index);
    MapFieldDepthSlot& field = mapFieldDepthSlots[index];
    struct DangerSeed {
        Vector4 sphere;
        MapCellOffset cell;
    };
    std::size_t dangerCount = 0;




    for (const MT7Node& node : model.nodes) {
        if (!node.xb01Offset) continue;
        const std::uint32_t type =
            (static_cast<std::uint32_t>(node.id) >> 16U) & 15U;
        field.wallCount += type == 3;
        field.cameraWallCount += type == 8;
        if (index != 32 && type == 2) ++dangerCount;
    }
    if (field.wallCount != 0) {
        field.walls = static_cast<MapFieldDepthWall*>(HLib::AllocateTaskData(
            sizeof(MapFieldDepthWall) *
                (static_cast<std::uint64_t>(field.wallCount) + 1),
            SHEN_NAME(WALL)));
        if (!field.walls) {
            FreeMapFieldDepthSlot(index);
            return false;
        }
    }
    if (field.cameraWallCount != 0) {
        field.cameraWalls = static_cast<MapFieldDepthWall*>(
            HLib::AllocateTaskData(
                sizeof(MapFieldDepthWall) *
                    (static_cast<std::uint64_t>(field.cameraWallCount) + 1),
                SHEN_NAME(WALC)));
        if (!field.cameraWalls) {
            FreeMapFieldDepthSlot(index);
            return false;
        }
    }
    std::vector<DangerSeed> dangerSeeds;
    dangerSeeds.reserve(dangerCount);

    std::array<MapCellOffset, 512> dangerCells{};
    std::size_t cellCount = 0;
    std::size_t wallCount = 0;
    std::size_t cameraWallCount = 0;
    for (const MT7Node& node : model.nodes) {
        if (!node.xb01Offset || index == 32) continue;
        const std::uint32_t type =
            (static_cast<std::uint32_t>(node.id) >> 16U) & 15U;
        if (type != 2 && type != 3 && type != 8) continue;
        const std::size_t offset = node.xb01Offset;
        if (offset > model.source.size() ||
            model.source.size() - offset < 32) {
            FreeMapFieldDepthSlot(index);
            return false;
        }
        const std::byte* const xb01 = model.source.data() + offset;
        float scale = node.scale.x;
        if (node.scale.y > scale) scale = node.scale.y;
        if (node.scale.z > scale) scale = node.scale.z;
        if (type == 2) {
            Vector3 centre{};
            float radius{};
            std::memcpy(&centre, xb01 + 8, sizeof(centre));
            std::memcpy(&radius, xb01 + 20, sizeof(radius));
            float x = (node.position.x + centre.x) * mapCellScale;
            float z = (node.position.z + centre.z) * mapCellScale;
            if (x < 0.0f) x -= 1.0f;
            if (z < 0.0f) z -= 1.0f;
            const MapCellOffset cell{
                static_cast<std::int16_t>(static_cast<std::int32_t>(x)),
                static_cast<std::int16_t>(static_cast<std::int32_t>(z))};
            std::size_t cellIndex = 0;
            while (cellIndex < cellCount &&
                   (dangerCells[cellIndex].x != cell.x ||
                    dangerCells[cellIndex].z != cell.z)) {
                ++cellIndex;
            }
            if (cellIndex == cellCount) {
                if (cellCount == dangerCells.size() - 1) {
                    FreeMapFieldDepthSlot(index);
                    return false;
                }
                dangerCells[cellCount++] = cell;
            }
            // ApplyMapFieldDepthNodeTransform replaces the DANG centre by the
            // node's reset-basis TRS translation; BuildMapModelCommandCells applies the final 0.69 radius.
            dangerSeeds.push_back({
                {node.position.x, node.position.y, node.position.z,
                 (radius * scale) * 0.69f}, cell});
        } else {
            std::uint32_t signature{};
            std::uint32_t streamOffset{};
            std::memcpy(&signature, xb01, sizeof(signature));
            std::memcpy(&streamOffset, xb01 + 28, sizeof(streamOffset));
            const std::uint64_t vertices =
                static_cast<std::uint64_t>(offset) +
                (signature == SHEN_NAME(XB01)
                    ? 44ULL + 4ULL * streamOffset : 112ULL);
            if (vertices > model.source.size() ||
                model.source.size() - vertices < 108) {
                FreeMapFieldDepthSlot(index);
                return false;
            }
            const std::byte* const first =
                model.source.data() + static_cast<std::size_t>(vertices);
            std::memcpy(&wallEndpoints.first, first, sizeof(Vector3));
            for (std::size_t vertex = 1; vertex != 4; ++vertex) {
                Vector3 candidate{};
                std::memcpy(&candidate, first + vertex * 32, sizeof(candidate));
                if (candidate.y == wallEndpoints.first.y) {
                    wallEndpoints.second = candidate;
                    break;
                }
            }
            PlaceFieldDepthWall(node, scale);
            if (type == 3) field.walls[wallCount++] = wallEndpoints;
            else field.cameraWalls[cameraWallCount++] = wallEndpoints;
        }
    }
    if (dangerCount != 0) {
        field.dangerSpheres = static_cast<Vector4*>(HLib::AllocateTaskData(
            sizeof(Vector4) * dangerCount,
            SHEN_NAME(DANG)));
        field.dangerCells = static_cast<MapModelCommandCell*>(
            HLib::AllocateTaskData(
                sizeof(MapModelCommandCell) * cellCount,
                SHEN_NAME(DAPH)));
        if (!field.dangerSpheres || !field.dangerCells) {
            FreeMapFieldDepthSlot(index);
            return false;
        }
        std::int32_t firstRecord = 0;
        for (std::size_t cell = 0; cell < cellCount; ++cell) {
            MapModelCommandCell& run = field.dangerCells[cell];
            run = {dangerCells[cell].x, dangerCells[cell].z, 0, firstRecord};
            for (const DangerSeed& seed : dangerSeeds) {
                if (seed.cell.x != run.x || seed.cell.z != run.z) continue;
                field.dangerSpheres[firstRecord++] = seed.sphere;
                ++run.count;
            }
        }
        field.dangerCount = static_cast<std::int32_t>(dangerCount);
        field.dangerCellCount = static_cast<std::int32_t>(cellCount);
    }



    field.enabled = index != 32 &&
        (field.dangerSpheres || field.walls || field.cameraWalls);
    return true;
}

void FreeMapModelSlot(std::size_t index) {
    MapModelRuntimeSlot& slot = mapModelSlots[index];
    // FreeMapModelSlot releases the four slot allocations before the separate
    // field-depth owner. Source readers are borrowed from mapModels.
    if (slot.clipRecords) HLib::FreeTaskData(slot.clipRecords);
    if (slot.commandCells) HLib::FreeTaskData(slot.commandCells);
    if (slot.commandRouting) HLib::FreeTaskData(slot.commandRouting);
    if (slot.auxiliaryAllocation) HLib::FreeTaskData(slot.auxiliaryAllocation);
    FreeMapFieldDepthSlot(index);
    slot = {};
}

void ResetMapModelSlots() {
    for (std::size_t index = 0; index < mapModelSlots.size(); ++index) {
        FreeMapModelSlot(index);
    }
}

bool SetupMapClipRecords(MT7Model& model, std::size_t slotIndex) {
    const std::size_t count = static_cast<std::size_t>(std::count_if(
        model.nodes.begin(), model.nodes.end(),
        [](const MT7Node& node) { return node.xb01Offset != 0; }));
    // LoadMainMap's projected pointer table is bounded independently of ordinary CLIP records.
    if (slotIndex == 32 && count > mainMapProjectedModels.size()) return false;
    MapModelRuntimeSlot& slot = mapModelSlots[slotIndex];
    slot.clipRecords = static_cast<MapModelClipRecord*>(
        HLib::AllocateTaskData(
            4000ULL * (count + 1),
            SHEN_NAME(CLIP)));
    if (!slot.clipRecords || !SetupMapFieldDepth(slotIndex, model)) {
        FreeMapModelSlot(slotIndex);
        return false;
    }
    auto* const cells = static_cast<MapCellOffset*>(
        HLib::AllocateStaticOrDynamicBlock(
            0x800, SHEN_NAME(CTMP)));
    auto* const dangerCells = static_cast<MapCellOffset*>(
        HLib::AllocateStaticOrDynamicBlock(
            0x800, SHEN_NAME(DTMP)));
    if (!cells || !dangerCells) {
        if (cells) HLib::FreeTaskData(cells);
        if (dangerCells) HLib::FreeTaskData(dangerCells);
        FreeMapModelSlot(slotIndex);
        return false;
    }
    cells[0] = {0x5fff, 0x5fff};
    dangerCells[0] = {0x5fff, 0x5fff};
    std::uint32_t recordIndex = 0;
    for (MT7Node& node : model.nodes) {
        if (!node.xb01Offset) continue;
        const std::size_t offset = node.xb01Offset;
        if (offset > model.source.size() ||
            model.source.size() - offset < 24) {
            HLib::FreeTaskData(cells);
            HLib::FreeTaskData(dangerCells);
            FreeMapModelSlot(slotIndex);
            return false;
        }
        MapModelClipRecord& record = slot.clipRecords[recordIndex];
        record = {};
        std::memcpy(&record.centre, model.source.data() + offset + 8,
                    sizeof(record.centre));
        std::memcpy(&record.radius, model.source.data() + offset + 20,
                    sizeof(record.radius));
        record.modelScale = node.scale.x;
        if (node.scale.y > record.modelScale) record.modelScale = node.scale.y;
        if (node.scale.z > record.modelScale) record.modelScale = node.scale.z;
        record.radius *= record.modelScale;
        record.object = slotIndex == 32 ? static_cast<const void*>(GetMainMapProjectedModelEntry(recordIndex)) : &node;
        ++recordIndex;
        float x = (node.position.x + record.centre.x) * mapCellScale;
        float z = (record.centre.z + node.position.z) * mapCellScale;
        if (x < 0.0f) x -= 1.0f;
        if (z < 0.0f) z -= 1.0f;
        record.cellX = static_cast<std::int16_t>(static_cast<std::int32_t>(x));
        record.cellZ = static_cast<std::int16_t>(static_cast<std::int32_t>(z));
        std::uint32_t type = (static_cast<std::uint32_t>(node.id) >> 16U) & 15U;
        if (type >= 12) {
            node.id &= ~0xf0000;
            type = 0;
        }
        record.commandType = slotIndex == 32 ? (type == 4 ? 24 : 20) : mapModelCommandTypes[type];
        switch (record.commandType) {
            case 4: ++slot.type4RecordCount; break;
            case 8: ++slot.nearRecordCount; break;
            case 12: ++slot.farRecordCount; break;
            case 16:
                ++slot.screenSpaceRecordCount;
                record.cellX = record.cellZ = 0x4fff;
                break;
            case 240: ++slot.wallRecordCount; continue;
            case 244: ++slot.wallCRecordCount; continue;
            case 255:
                ++slot.dangerRecordCount;
                record.centre = node.position;
                continue;
            default: break;
        }
        MapCellOffset* cell = cells;
        while (cell->x != 0x5fff &&
               (cell->x != record.cellX || cell->z != record.cellZ)) ++cell;
        if (cell->x == 0x5fff) {
            *cell = {record.cellX, record.cellZ};
            cell[1] = {0x5fff, 0x5fff};
        }
    }
    std::uint32_t cellCount = 0;
    while (cells[cellCount].x != 0x5fff) ++cellCount;
    slot.commandCellCount = cellCount;
    slot.commandCells = static_cast<MapModelCommandCell*>(
        HLib::AllocateTaskData(
            sizeof(MapModelCommandCell) * (std::uint64_t(cellCount) + 1),
            SHEN_NAME(ACPH)));
    slot.commandRouting = static_cast<MapModelClipRecord**>(
        HLib::AllocateTaskData(
            sizeof(MapModelClipRecord*) * (count + 1),
            SHEN_NAME(ACPD)));
    if (!slot.commandCells || !slot.commandRouting) {
        HLib::FreeTaskData(cells);
        HLib::FreeTaskData(dangerCells);
        FreeMapModelSlot(slotIndex);
        return false;
    }
    // BuildMapModelCommandCells swaps the special first-X cell to the head before grouping.
    if (cells[0].x != 0x4fff) {
        for (std::uint32_t index = 1; index < cellCount; ++index) {
            if (cells[index].x == 0x4fff) {
                cells[index] = cells[0];
                cells[0] = {0x4fff, 0x4fff};
                break;
            }
        }
    }
    std::int32_t firstRecord = 0;
    for (std::uint32_t cell = 0; cell < cellCount; ++cell) {
        MapModelCommandCell& run = slot.commandCells[cell];
        run = {cells[cell].x, cells[cell].z, 0, firstRecord};
        for (std::uint32_t index = 0; index < recordIndex; ++index) {
            MapModelClipRecord& record = slot.clipRecords[index];
            if (record.commandType == 240 || record.commandType == 244 || record.commandType == 255 ||
                record.cellX != run.x || record.cellZ != run.z) continue;
            slot.commandRouting[firstRecord++] = &record;
            ++run.count;
        }
    }
    slot.commandCells[0].firstRecord = 0;
    HLib::FreeTaskData(cells);
    HLib::FreeTaskData(dangerCells);
    slot.drawableRecordCount = static_cast<std::uint32_t>(firstRecord);
    return true;
}

bool SetupModel(std::size_t slotIndex, MT7Model& model) {
    if (slotIndex >= mapModelSlots.size()) return false;
    MapModelRuntimeSlot& slot = mapModelSlots[slotIndex];
    slot.reader = &model;
    const bool ready = SetupMapClipRecords(model, slotIndex);
    if (!ready) return false;
    // SetupModel normalizes the reader's root only after CLIP has captured
    // its authored bounds/cells; the borrowed projected-table entry sees it.
    if (!model.nodes.empty()) {
        MT7Node& root = model.nodes.front();
        SetupModelForImageModule(&root);
        root.position = {};
        root.rotation = {};
        root.scale = {1.0f, 1.0f, 1.0f};
    }
    slot.forceLambertShader = 0;
    slot.vertexShaderType = 0;
#if defined(SHENMUE_ENHANCEMENT_S2_MAP_SEAMS) && SHENMUE_ENHANCEMENT_S2_MAP_SEAMS
    PrepareMapSeamResources(model);
#endif
    return true;
}

std::int32_t ReadChunkInt(const MapInfoChunk* chunk) {
    std::int32_t value{};
    if (chunk) {
        std::memcpy(&value, reinterpret_cast<const std::byte*>(chunk) + 8,
                    sizeof(value));
    }
    return value;
}

shen_name ReadAreaOverride(const MapInfoChunk* chunk, shen_name fallback) {
    if (!chunk) {
        return fallback;
    }
    shen_name area{};
    std::memcpy(&area, reinterpret_cast<const std::byte*>(chunk) + 8,
                sizeof(area));
    return area;
}

bool SelectEntryMapLayers(const MapInfoChunk* chunk, std::int32_t entryPoint,
                          std::array<std::int32_t, 34>& layers) {
    if (!chunk || chunk->nextByteOffset <= 8 ||
        (chunk->nextByteOffset - 8) % sizeof(std::int32_t) != 0) {
        return false;
    }
    layers.fill(-1);
    const auto* cursor = reinterpret_cast<const std::int32_t*>(
        reinterpret_cast<const std::byte*>(chunk) + 8);
    const auto* const end = reinterpret_cast<const std::int32_t*>(
        reinterpret_cast<const std::byte*>(chunk) + chunk->nextByteOffset);
    while (cursor < end && *cursor != -1) {
        const auto* entryBegin = cursor;
        while (cursor < end && *cursor != -1) {
            ++cursor;
        }
        if (cursor == end) {
            return false;
        }
        const bool matches =
            std::find(entryBegin, cursor, entryPoint) != cursor;
        ++cursor;
        const auto* layerBegin = cursor;
        while (cursor < end && *cursor != -1) {
            ++cursor;
        }
        if (cursor == end) {
            return false;
        }
        if (matches) {
            const std::size_t count = std::min<std::size_t>(
                static_cast<std::size_t>(cursor - layerBegin),
                layers.size() - 1);
            std::copy_n(layerBegin, count, layers.begin());
            layers[count] = -1;
            return true;
        }
        ++cursor;
    }
    return false;
}

} // namespace

// A reader released by FreeMainMap is reloaded in place, so slot 32 and the other slots keep their reader addresses.
bool LoadMainMapByFilepath(const char* folder) {
    std::array<char, 272> path{};
    const int length = std::snprintf(
        path.data(), path.size(), "%s/MAPS.MT7", folder);
    if (length < 0 || static_cast<std::size_t>(length) >= path.size()) {
        return false;
    }
    const bool reload = mainMapModelIndex < mapModels.size() && !mainMapModelAllocation;
    const std::uint64_t size = ArchiveData_GetFileSize(path.data());
    if (!size || size > (std::numeric_limits<std::size_t>::max)()) {
        ResetMainMapProjection();
        mainMapModelIndex = (std::numeric_limits<std::size_t>::max)();
        return false;
    }
    void* const data = HLib::AllocateTaskData(
        size, shen_name{make_shen_name('M', 'A', 'P', ' ')});
    if (!data || !ArchiveData_Read(path.data(), 0, data, size)) {
        if (data) HLib::FreeTaskData(data);
        ResetMainMapProjection();
        mainMapModelIndex = (std::numeric_limits<std::size_t>::max)();
        return false;
    }
    const auto bytes = std::span<const std::byte>(
        static_cast<const std::byte*>(data), static_cast<std::size_t>(size));
    if (!reload) mapModels.emplace_back();
    MT7Model& maps = reload ? mapModels[mainMapModelIndex] : mapModels.back();
    if (reload) maps = {};
    if (!ReadModelFormat7(bytes, path.data(), maps, 0) ||
        !InitializeMainMapProjection(maps)) {
        ReleaseModelFormat7(maps);
        ResetMainMapProjection();
        if (!reload) mapModels.pop_back();
        mainMapModelIndex = (std::numeric_limits<std::size_t>::max)();
        HLib::FreeTaskData(data);
        return false;
    }
    mainMapModelAllocation = data;
    mainMapModelSize = static_cast<std::size_t>(size);
    BindMT7WritableRuntimeMeshes(maps, {static_cast<std::byte*>(data), static_cast<std::size_t>(size)});
    if (!reload) mainMapModelIndex = mapModels.size() - 1;
    AlignMainMap();
    return true;
}

namespace {

bool LoadMapModel(const char* folder, std::int32_t layer,
                  std::size_t slotIndex) {
    std::array<char, 272> path{};
    const int length = layer == 0
        ? std::snprintf(path.data(), path.size(), "%s/MAP.MT7", folder)
        : std::snprintf(path.data(), path.size(), "%s/MAP%02d.MT7",
                        folder, layer);
    if (length < 0 || static_cast<std::size_t>(length) >= path.size())
        return false;
    const std::uint64_t size = ArchiveData_GetFileSize(path.data());
    if (!size || size > (std::numeric_limits<std::size_t>::max)())
        return false;
    void* const data = HLib::AllocateTaskData(
        size, make_name('M', 'A', 'P', ' '));
    if (!data) return false;
    mapModels.emplace_back();
    MT7Model& model = mapModels.back();
    if (!ArchiveData_Read(path.data(), 0, data, size) ||
        !ReadModelFormat7(
            {static_cast<const std::byte*>(data),
             static_cast<std::size_t>(size)}, path.data(), model, 0)) {
        ReleaseModelFormat7(model);
        mapModels.pop_back();
        HLib::FreeTaskData(data);
        return false;
    }
    mapModelAllocations[slotIndex] = data;
    BindMT7WritableRuntimeMeshes(model, {static_cast<std::byte*>(data), static_cast<std::size_t>(size)});
    mapModelSlotIndices[slotIndex] = mapModels.size() - 1;
    return true;
}


bool AssignLoadedModelsToNativeSlots() {
    bool ready = true;



    for (std::size_t slot = 0; slot < mapModelSlotIndices.size(); ++slot) {
        const std::size_t index = mapModelSlotIndices[slot];
        if (index < mapModels.size())
            ready = SetupModel(slot, mapModels[index]) && ready;
        if (mapModelSlotStartsHidden[slot]) mapModelSlots[slot].enabled = 0;
        mapModelSlots[slot].ownsReader = mapModelAllocations[slot] ? 1U : 0U;
    }
    if (mainMapModelIndex < mapModels.size()) {
        ready = SetupModel(32, mapModels[mainMapModelIndex]) && ready;
        mapModelSlots[32].enabled = 1;
        mapModelSlots[32].ownsReader = 1;
    }
    return ready;
}

} // namespace
// MatchMapRenderProfile: first matching four-byte pattern,
// with '*' matching one byte and the zero-name record providing defaults.
// @verified - p
const MapRenderProfile& MatchMapRenderProfile(shen_name area) {
    const MapRenderProfile* profile = mapRenderProfiles.data();
    while (profile->areaPattern != 0) {
        bool matches = true;
        for (std::uint32_t shift = 0; shift != 32; shift += 8) {
            const std::uint8_t pattern = static_cast<std::uint8_t>(profile->areaPattern >> shift);
            if (pattern != '*' &&
                pattern != static_cast<std::uint8_t>(area.name >> shift)) {
                matches = false;
                break;
            }
        }
        if (matches) return *profile;
        ++profile;
    }
    return *profile;
}


void GetPropVector4(Vector3* output) {
    output->x = propLightingVector.y;
    output->y = propLightingVector.z;
    output->z = propLightingVector.w;
}

void SetPropVector3(const Vector3* position) {
    propLightingVector.y = position->x;
    propLightingVector.z = position->y;
    propLightingVector.w = position->z;
}

std::uint32_t GetPropLightingTraversalResult() {
    std::uint32_t result;
    std::memcpy(&result, &propLightingVector.x, sizeof(result));
    return result;
}

void SetPropLightingTraversalResult(std::uint32_t result) {
    std::memcpy(&propLightingVector.x, &result, sizeof(result));
}

MT7Node* GetPropRoot() {
    return propRoot;
}

std::span<const PropReaderSlot, 17> GetPropReaderSlots() {
    return propReaderSlots;
}

std::uint64_t RegisterPropReader(MT7Model* reader) {
    if (!reader || reader->nodes.empty()) return UINT64_C(0xffffffff);
    std::int16_t index = 1;
    while (index < 17 && propReaderSlots[index].previous != -1) ++index;
    if (index == 17) return UINT64_C(0xffffffff);
    std::int16_t previous = 0;
    while (propReaderSlots[previous].next != -1)
        previous = propReaderSlots[previous].next;
    propReaderSlots[previous].next = index;
    PropReaderSlot& slot = propReaderSlots[index];
    slot.previous = previous;
    slot.reader = reader;
    MT7Node* const root = &reader->nodes.front();
    MT7Node* tail = root->runtimeChild;
    while (tail && tail->runtimeSibling) tail = tail->runtimeSibling;
    slot.lastSibling = tail;
    if (!propRoot) {
        propRoot = root;
        return static_cast<std::uint32_t>(index);
    }
    MT7Node* previousTail = propRoot->runtimeChild;
    while (previousTail->runtimeSibling)
        previousTail = previousTail->runtimeSibling;
    previousTail->runtimeSibling = root->runtimeChild;
    for (MT7Node* node = root->runtimeChild; node; node = node->runtimeSibling)
        node->runtimeParent = propRoot;
    return static_cast<std::uint32_t>(index);
}

void RemovePropReader(std::uint32_t index) {
    if (!propRoot || index > 16) return;
    PropReaderSlot& slot = propReaderSlots[index];
    if (!slot.reader) return;
    MT7Node* const root = &slot.reader->nodes.front();
    MT7Node* const first = root->runtimeChild;
    MT7Node* next = nullptr;
    if (slot.next != -1) {
        PropReaderSlot& following = propReaderSlots[slot.next];
        next = following.reader->nodes.front().runtimeChild;
        following.previous = slot.previous;
    }
    propReaderSlots[slot.previous].next = slot.next;
    if (propRoot == root) {
        if (next) {
            propRoot = &propReaderSlots[slot.next].reader->nodes.front();
            for (MT7Node* node = next; node; node = node->runtimeSibling)
                node->runtimeParent = propRoot;
        } else {
            propRoot = nullptr;
        }
    } else {
        MT7Node* previous = propRoot->runtimeChild;
        while (previous->runtimeSibling != first)
            previous = previous->runtimeSibling;
        previous->runtimeSibling = next;
    }
    slot.previous = -1;
    slot.next = -1;
    slot.reader = nullptr;

}

// @verified - p
void InitializeObject(MT7Node* root) {
    for (MT7Node* node = root; node; node = node->runtimeSibling) {
        if (std::byte* const mesh = node->runtimeMesh) {
            if (IsModelXB01(mesh)) return;
            const std::int32_t* const header = reinterpret_cast<const std::int32_t*>(mesh);
            if (*header != -1) {
                std::uint32_t* record = reinterpret_cast<std::uint32_t*>(mesh + 24);
                do {
                    *record &= 0xffffff3fU;
                    record = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(record) + 88 + static_cast<std::int32_t>(record[21]));
                } while (*record != 0);
            }
        }
        if (node->runtimeChild) InitializeObject(node->runtimeChild);
    }
}

void SetupModelForImageModule(MT7Node* root) {
    for (MT7Node* node = root; node; node = node->runtimeSibling) {
        if (std::byte* const mesh = node->runtimeMesh) {
            if (IsModelXB01(mesh)) return;
            auto* const header = reinterpret_cast<std::int32_t*>(mesh);
            if (*header != -1) {
                auto* record = reinterpret_cast<std::uint32_t*>(mesh + 24);
                do {
                    if ((*record & 0xc0U) != 0xc0U)
                        *record = (*record & 0xffffff3fU) | 0x80U;
                    const auto bytes = static_cast<std::int32_t>(record[21]);
                    record = reinterpret_cast<std::uint32_t*>(
                        reinterpret_cast<std::byte*>(record) + 88 + bytes);
                } while (*record != 0);
            }
        }
        if (node->runtimeChild) SetupModelForImageModule(node->runtimeChild);
    }
}

void AllocatePropFlagObject() {
    if (!propRoot) return;
    SetupModelForImageModule(propRoot);
    propRoot->position = {};
    propRoot->rotation = {};
    propRoot->scale = {1.0f, 1.0f, 1.0f};
    g_flagWindObject = AllocateFlagObject(propRoot);
    UpdatePropSearchVisibility(propRoot, 0, 0);
    srchRegisterAndPush(propRoot->runtimeChild);
}

std::uint64_t RegisterPackagePropReader(MT7Model* reader) {
    const auto index = RegisterPropReader(reader);
    if (index != UINT64_C(0xffffffff)) {
        for (MT7Node* node = reader->nodes.front().runtimeChild; node;
             node = node->runtimeSibling) {
            node->position.x -= propLightingVector.y;
            node->position.y -= propLightingVector.z;
            node->position.z -= propLightingVector.w;
        }
    }
    AllocatePropFlagObject();
    return index;
}

void RemovePackagePropReader(std::uint32_t index) {
    RemovePropReader(index);
    AllocatePropFlagObject();
}

void LoadPROPs(std::int32_t actNumber, shen_name areaName) {
    propRoot = nullptr;
    propLightingVector = {};
    g_flagWindObject = nullptr;
    propReaderSlots.fill(PropReaderSlot{});
    std::array<char, 64> folder{};
    std::snprintf(folder.data(), folder.size(), "scene/%02d/%c%c%c%c",
                  actNumber, static_cast<char>(areaName.name),
                  static_cast<char>(areaName.name >> 8U),
                  static_cast<char>(areaName.name >> 16U),
                  static_cast<char>(areaName.name >> 24U));
    for (std::int32_t layer = 0;; ++layer) {
        std::array<char, 272> path{};
        if (layer == 0)
            std::snprintf(path.data(), path.size(), "%s/PROP.MT7", folder.data());
        else
            std::snprintf(path.data(), path.size(), "%s/PROP%02d.MT7",
                          folder.data(), layer);
        const std::uint64_t size = ArchiveData_GetFileSize(path.data());
        if (!size) break;
        void* const data = HLib::AllocateTaskData(
            size, SHEN_NAME(PROP));
        if (!data) continue;
        if (!ArchiveData_Read(path.data(), 0, data, size)) {
            HLib::FreeTaskData(data);
            continue;
        }
        auto model = std::make_unique<MT7Model>();
        const auto source = std::span<std::byte>(
            static_cast<std::byte*>(data), static_cast<std::size_t>(size));
        if (!ReadModelFormat7(source, path.data(), *model, 0)) continue;
        BindMT7WritableRuntimeMeshes(*model, source);
        RegisterPropReader(model.get());
        propModels.push_back(std::move(model));
    }
    AllocatePropFlagObject();
}

void RetirePropAssets() {
    propRoot = nullptr;
    g_flagWindObject = nullptr;
    propReaderSlots.fill(PropReaderSlot{});
    for (const auto& model : propModels) ReleaseModelFormat7(*model);
    propModels.clear();
}

bool Mapinfo_MAP_Reader(std::int32_t actNumber, shen_name areaName,
                        std::int32_t entryPoint, void* mapInfo) {
    if (!mapInfo) {
        return false;
    }

    LoadPROPs(actNumber, areaName);
    float cellSize = 45.0f;
    if (const MapInfoChunk* chunk = FindMapInfoChunk("BLKS", mapInfo)) {
        std::memcpy(&cellSize,
                    reinterpret_cast<const std::byte*>(chunk) + 8,
                    sizeof(cellSize));
    }
    mapCellScale = 1.0f / cellSize;
    std::int32_t totalMapPacks = 0;
    if (const MapInfoChunk* const mapR =
            FindMapInfoChunk("MAPR", mapInfo)) {
        totalMapPacks = std::clamp(ReadChunkInt(mapR), -1, 32);
    }
    const shen_name targetArea = ReadAreaOverride(
        FindMapInfoChunk("MAPP", mapInfo), areaName);
    std::array<std::int32_t, 34>& mapLayers = mapEntryLayers;
    const bool entryLayers = SelectEntryMapLayers(
        FindMapInfoChunk("MAPE", mapInfo), entryPoint, mapLayers);
    const bool packageMode =
        ReadChunkInt(FindMapInfoChunk("MAPT", mapInfo)) != 0;



    const MapRenderProfile& profile = MatchMapRenderProfile(areaName);
    camCtrlSetFarDistance(profile.farDistance);
    camCtrlSetNearDistance(profile.nearDistance);
    camCtrlSetNpcRadius(profile.npcRadius);
    SetLegacySavedAlphaOperation(profile.alphaOperation);
    SetWeatherMinimumSunElevation(profile.textureDistanceZ);
    mainMapProjectedExtrusionDistance = profile.textureDistance;

    const std::array<char, 4> area{
        static_cast<char>(targetArea.name),
        static_cast<char>(targetArea.name >> 8U),
        static_cast<char>(targetArea.name >> 16U),
        static_cast<char>(targetArea.name >> 24U)};
    std::array<char, 96> folder{};
    const int folderLength = std::snprintf(
        folder.data(), folder.size(), "scene/%02d/%c%c%c%c", actNumber,
        area[0], area[1], area[2], area[3]);
    if (folderLength < 0 ||
        static_cast<std::size_t>(folderLength) >= folder.size()) {
        return false;
    }

    mapModelSlotIndices.fill((std::numeric_limits<std::size_t>::max)());
    mapModelSlotStartsHidden.fill(false);
    ResetMapModelSlots();
    ResetMapLightingPlacements();
    mapModels.reserve(mapModelSlots.size() + 1);
    bool loaded = false;
    bool resourcesReady = true;
    if (entryLayers) {
        // LoadMap_Internal does not load MAPS after an entry-layer match.
        for (std::size_t slot = 0; slot < mapModelSlotIndices.size() &&
             mapLayers[slot] != -1; ++slot) {
            const bool ready = LoadMapModel(
                folder.data(), mapLayers[slot], slot);
            resourcesReady = ready && resourcesReady;
            loaded = ready || loaded;
        }
    } else if (totalMapPacks >= 0) {
        if (packageMode) {
            const std::int32_t packageCount = totalMapPacks == 0
                ? 1 : totalMapPacks;
            for (std::int32_t index = 0; index < packageCount; ++index) {
                std::array<char, 8> packageName{};
                std::snprintf(packageName.data(), packageName.size(),
                              "MPK%02d", index);
                const bool requested =
                    LoadMapPackage(folder.data(), packageName.data());
                resourcesReady = requested && resourcesReady;
                loaded = requested || loaded;
                LoadPak();
            }
        } else {
            // LoadMapPacks includes MAP.MT7 plus MAP01..MAPnn, inclusive.
            for (std::int32_t layer = 0; layer <= totalMapPacks; ++layer) {
                const bool ready = LoadMapModel(
                    folder.data(), layer, static_cast<std::size_t>(layer));
                resourcesReady = ready && resourcesReady;
                loaded = ready || loaded;
            }
        }
        loaded = LoadMainMapByFilepath(folder.data()) || loaded;
    }
    const bool fieldOwnersReady = AssignLoadedModelsToNativeSlots();

    const auto packages = GetLoadedMapPackages();
    const std::size_t pksEntries = packages.empty()
        ? 0 : packages.front().pks.entries.size();
    const std::size_t pkfEntries = packages.empty()
        ? 0 : packages.front().pkf.entries.size();
    std::size_t vertices = 0;
    std::size_t triangles = 0;
    std::size_t renderPackets = 0;
    std::size_t renderCommands = 0;
    std::size_t drawCommands = 0;
    std::size_t unsupportedCommands = 0;
    std::size_t vertexBuffers = 0;
    std::size_t preparedIndexBuffers = 0;
    std::size_t resolvedModelTextures = 0;
    std::size_t modelTextures = 0;
    std::size_t resolvedTextureBindings = 0;
    std::size_t textureBindings = 0;
    for (const MT7Model& model : mapModels) {
        vertices += model.vertices.size();
        triangles += model.indices.size() / 3;
        modelTextures += model.textures.size();
        resolvedModelTextures += static_cast<std::size_t>(std::count_if(
            model.textures.begin(), model.textures.end(),
            [](const MT7TextureEntry& texture) {
                return texture.textureIndex != 0;
            }));
        for (const MT7Node& node : model.nodes) {
            renderPackets += node.commandStream.packetCount;
            renderCommands += node.commandStream.commandCount;
            drawCommands += node.commandStream.drawCount;
            unsupportedCommands +=
                node.commandStream.unsupportedCommandCount;
            vertexBuffers += node.drawResources.vertexBuffers.size();
            preparedIndexBuffers += node.drawResources.indexBuffer != 0;
            textureBindings += node.drawResources.textureBindings.size();
            resolvedTextureBindings += static_cast<std::size_t>(std::count_if(
                node.drawResources.textureBindings.begin(),
                node.drawResources.textureBindings.end(),
                [](const LegacyTextureBinding& binding) {



                    return binding.textureTableIndex < 0 ||
                           binding.textureHandle != 0;
                }));
        }
    }
    InitializeAirPresentation(mapInfo);
    return loaded && resourcesReady && fieldOwnersReady;
}

std::span<const std::byte> GetMainMapModelBytes() {
    return mainMapModelAllocation
        ? std::span<const std::byte>(
              static_cast<const std::byte*>(mainMapModelAllocation),
              mainMapModelSize)
        : std::span<const std::byte>{};
}

std::span<const MT7Model> GetLoadedMapModels() { return mapModels; }

bool FindLoadedMapNode(shen_name name, LoadedMapNodeBinding& binding) {
    binding = {};
    for (std::size_t slotIndex = 0; slotIndex < mapModelSlots.size();
         ++slotIndex) {
        MapModelRuntimeSlot& slot = mapModelSlots[slotIndex];
        if (!slot.reader) continue;
        auto* const model = const_cast<MT7Model*>(slot.reader);
        for (std::size_t nodeIndex = 0; nodeIndex < model->nodes.size();
             ++nodeIndex) {
            MT7Node& node = model->nodes[nodeIndex];
            if (static_cast<std::uint32_t>(node.id) != name.name) continue;
            binding = {model, &node, slotIndex, nodeIndex};
            return true;
        }
    }
    return false;
}

bool ResolveLoadedMapNodeTransform(const LoadedMapNodeBinding& binding,
                                   Matrix4x4& transform) {
    if (!binding.model || !binding.node ||
        binding.slotIndex >= mapModelSlots.size() ||
        binding.nodeIndex >= binding.model->nodes.size()) {
        return false;
    }
    const MapModelRuntimeSlot& slot = mapModelSlots[binding.slotIndex];
    if (slot.reader != binding.model ||
        &binding.model->nodes[binding.nodeIndex] != binding.node) {
        return false;
    }
    transform = IdentityTransform();
    PreTranslate(transform, slot.position);
    if (slot.rotationY != 0) {
        RotateY(transform, static_cast<std::uint16_t>(slot.rotationY));
    }
    return ApplyNodeTransformPath(*binding.model, *binding.node, transform);
}

std::span<const MT7Node* const> GetMainMapProjectedModels() {
    return {mainMapProjectedModels.data(), mainMapProjectedModelCount};
}

const MT7Node* const* GetMainMapProjectedModelEntry(std::uint32_t index) {
    return index < mainMapProjectedModelCount
        ? &mainMapProjectedModels[index] : nullptr;
}


float GetMainMapMinimumModelHeight() {
    return mainMapMinimumModelHeight;
}
// @verified - p
void SetMainMapMinimumModelHeight(float height) {
    mainMapMinimumModelHeight = height;
}

const MainMapProjectionAngles& GetMainMapProjectionAngles() {
    return mainMapProjectionAngles;
}


void SetMainMapProjectionDirection(const Vector3& direction) {
    mainMapProjectionDirection = direction;
    mainMapProjectionDirectionOverridden = true;
}
bool AlignMainMap() {
    if (mainMapModelIndex >= mapModels.size() ||
        mapModels[mainMapModelIndex].nodes.empty()) return false;
    mainMapProjectedPlacementCount = 0;
    for (std::uint32_t index = 0; index < mainMapProjectedModelCount; ++index) {
        mainMapProjectedPlacements[index].modelEntry = nullptr;
    }
    const Vector3 direction = mainMapProjectionDirectionOverridden
        ? mainMapProjectionDirection : GetLegacyLightDirection(0);
    const float horizontal = std::sqrt(
        direction.z * direction.z + direction.x * direction.x);
    if (std::abs(horizontal) < 0.00000011920929f || horizontal <= 0.0f) {
        mainMapProjectionAngles = {};
    } else {
        const auto angle = static_cast<std::int32_t>(
            (std::atan(-(direction.y / horizontal)) * 65536.0f) /
                6.2831855f + 0.5f);
        mainMapProjectionAngles.fixedAngle = angle;
        mainMapProjectionAngles.degrees =
            static_cast<std::int32_t>(static_cast<float>(angle) / 182.04445f);
        mainMapProjectionAngles.sine =
            sin_fixed16(static_cast<std::int16_t>(angle));
        mainMapProjectionAngles.tangent = std::tan(
            (static_cast<float>(static_cast<std::uint16_t>(angle)) *
             6.2831855f) * 0.000015258789f);
    }
    return true;
}

void SetMainMapProjectedShadowDepth(float distance) {
    mainMapProjectedShadowDepth = distance < 0.0f
        ? (std::numeric_limits<float>::max)() : distance;
}

// @verified - p
void SetProjectedShadowLightDirection(float distance) {
    mainMapProjectedExtrusionDistance = std::fmax(0.00050000002f, distance);
    Vector3 direction = mainMapProjectionDirectionOverridden ? mainMapProjectionDirection : GetLegacyLightDirection(0);
    NormalizeVector3(&direction);
    const Vector3 transformed{
        (workTransformMatrix.rows[1].x*direction.y+workTransformMatrix.rows[0].x*direction.x)+workTransformMatrix.rows[2].x*direction.z,
        (workTransformMatrix.rows[1].y*direction.y+workTransformMatrix.rows[0].y*direction.x)+workTransformMatrix.rows[2].y*direction.z,
        (workTransformMatrix.rows[1].z*direction.y+workTransformMatrix.rows[0].z*direction.x)+workTransformMatrix.rows[2].z*direction.z};
    SetLegacyProjectedShadowDirectionAndScale(transformed, mainMapProjectedExtrusionDistance);
}

// @verified - p
void SetProjectedShadowDirection(Vector3* direction, float distance) {
    if (!direction) return;
    mainMapProjectedExtrusionDistance = std::fmax(0.00050000002f, distance);
    NormalizeVector3(direction);
    const Vector3 transformed{
        (workTransformMatrix.rows[1].x*direction->y+workTransformMatrix.rows[0].x*direction->x)+workTransformMatrix.rows[2].x*direction->z,
        (workTransformMatrix.rows[1].y*direction->y+workTransformMatrix.rows[0].y*direction->x)+workTransformMatrix.rows[2].y*direction->z,
        (workTransformMatrix.rows[1].z*direction->y+workTransformMatrix.rows[0].z*direction->x)+workTransformMatrix.rows[2].z*direction->z};
    SetLegacyProjectedShadowDirectionAndScale(transformed, mainMapProjectedExtrusionDistance);
}

bool BeginMainMapProjectedPlacementQueue() {
    SetProjectedShadowLightDirection(GetTexUVW(0));
    return AlignMainMap();
}

bool QueueMainMapProjectedPlacements(
    std::span<MapModelClipRecord* const> records) {


    if (records.size() >
        mainMapProjectedPlacements.size() - mainMapProjectedPlacementCount)
        return false;
    const Matrix4x4 baseTransform = workTransformMatrix;
    camCtrlSetProjectedShadowDepth(mainMapProjectedShadowDepth);
    const Vector3 direction = mainMapProjectionDirectionOverridden
        ? mainMapProjectionDirection : GetLegacyLightDirection(0);
    for (const MapModelClipRecord* record : records) {
        const auto* const entry =
            static_cast<const MT7Node* const*>(record->object);
        const MT7Node& node = **entry;
        Matrix4x4::SetActiveTransform(&baseTransform);
        if (!node.xb01Offset) continue;
        ApplyMT7NodeTransform(node);

        const float height = std::fmax(
            0.0f, record->centre.y + node.position.y -
                mainMapMinimumModelHeight);
        Vector3 projection{};
        float radius = record->radius;
        constexpr float epsilon = 0.00000011920929f;
        if (!(std::abs(direction.y) < epsilon || direction.y >= 0.0f ||
              std::abs(height) < epsilon)) {
            const float scale = (height / -direction.y) * 0.5f;
            projection = {direction.x * scale, direction.y * scale,
                          direction.z * scale};

            radius = std::sqrt(
                (projection.x * projection.x + projection.y * projection.y) +
                projection.z * projection.z) + radius;
        }
        const Vector3 centre{
            record->centre.x + projection.x,
            record->centre.y + projection.y,
            record->centre.z + projection.z};
        if (record->commandType == 24 ||
            (camCtrlIsProjectedShadowVisible(centre, radius) &&
             camCtrlIsModelDepthVisible(centre, node, radius))) {
            SetLegacyModelScale(record->modelScale);
            MainMapProjectedPlacement& placement =
                mainMapProjectedPlacements[mainMapProjectedPlacementCount++];
            placement.modelEntry = entry;
            Matrix4x4::GetTransformMatrix(&placement.transform);
        }
    }
    Matrix4x4::SetActiveTransform(&baseTransform);
    return true;
}

std::uint32_t DrawQueuedMainMapProjectedShadows() {
    if (mainMapModelIndex >= mapModels.size() ||
        !BeginLegacyProjectedShadowPass()) return 0;
    Matrix4x4_Push();
    const MT7Model& model = mapModels[mainMapModelIndex];
    std::uint32_t draws = 0;
    for (std::uint32_t index = 0;
         index < mainMapProjectedPlacementCount; ++index) {
        const MainMapProjectedPlacement& placement =
            mainMapProjectedPlacements[index];
        const MT7Node& node = **placement.modelEntry;
        Matrix4x4::SetActiveTransform(&placement.transform);
        draws += ExecuteProjectedShadowCommandStream(model.source, node.xb01Offset, node.borrowedMesh ? node.borrowedMesh->drawResources : node.drawResources);
    }
    Matrix4x4::Pop(1);
    EndLegacyProjectedShadowPass();

    return draws;
}

std::span<const MapModelRuntimeSlot, 33> GetMapModelSlots() {
    return mapModelSlots;
}

void SetMapModelLightingTraversalResult(std::int32_t index, std::uint32_t result) {
    mapModelSlots[index].alternateTraversalEnabled = result;
}

std::span<const MapFieldDepthSlot, 33> GetMapFieldDepthSlots() {
    return mapFieldDepthSlots;
}

std::int64_t SetMapFieldDepthSlotEnabled(std::uint32_t index,
                                       std::uint32_t enabled) {
    if (index >= mapFieldDepthSlots.size()) return 0;
    MapFieldDepthSlot& slot = mapFieldDepthSlots[index];
    slot.enabled = slot.dangerSpheres || slot.walls || slot.cameraWalls
        ? enabled : 0;
    return static_cast<std::int64_t>(index) * 0x40;
}

// @verified - p
const MapFieldDepthSlot* SetAllMapFieldDepthSlotsEnabled(std::uint32_t enabled) {
    for (MapFieldDepthSlot& slot : mapFieldDepthSlots) slot.enabled = slot.dangerSpheres || slot.walls || slot.cameraWalls ? enabled : 0U;
    return mapFieldDepthSlots.data() + mapFieldDepthSlots.size();
}
// @verified - p
void camCtrlCacheUtil(std::int32_t operation) {
    switch (operation) {
    case 2:
        for (std::uint32_t index = 0; index < 33; ++index) cachedMapEnabled[index] = mapModelSlots[index].enabled;
        for (std::uint32_t index = 0; index < 33; ++index) cachedMapFieldDepthEnabled[index] = mapFieldDepthSlots[index].enabled;
        for (std::uint32_t index = 0; index < 33; ++index) cachedMapGridCulling[index] = mapModelSlots[index].gridCullingEnabled;
        break;
    case 3:
        for (std::uint32_t index = 0; index < 33; ++index) mapModelSlots[index].enabled = cachedMapEnabled[index];
        for (std::uint32_t index = 0; index < 33; ++index) SetMapFieldDepthSlotEnabled(index, cachedMapFieldDepthEnabled[index]);
        for (std::uint32_t index = 0; index < 33; ++index) mapModelSlots[index].gridCullingEnabled = cachedMapGridCulling[index];
        break;
    case 4:
        cachedFarDistance = camCtrlGetFarDistance();
        cachedNearDistance = camCtrlGetNearDistance();
        cachedNpcRadius = camCtrlGetNpcRadius();
        break;
    case 5:
        camCtrlSetFarDistance(cachedFarDistance);
        camCtrlSetNearDistance(cachedNearDistance);
        camCtrlSetNpcRadius(cachedNpcRadius);
        break;
    }
}


float GetMapCellScale() {
    return mapCellScale;
}

std::span<const MapCellOffset> GetMapVisibleCellOffsets(
    std::uint32_t direction, std::uint32_t tier) {
    const MapCellDirection& entry = mapCellDirections[direction];
    return {mapCellOffsets.data() + entry.firstOffset, entry.counts[tier]};
}

std::int32_t SetMapModelSlotEnabled(std::int32_t index,
                                    std::int32_t enabled) {
    if (index < 0 || static_cast<std::size_t>(index) >= mapModelSlots.size()) {
        return 0;
    }
    MapModelRuntimeSlot& slot = mapModelSlots[static_cast<std::size_t>(index)];
    const std::int32_t previous = static_cast<std::int32_t>(slot.enabled);
    slot.enabled = static_cast<std::uint32_t>(enabled);
    return previous;
}

void SetMapModelCommandRoutingEnabled(std::int32_t value, std::int32_t index) {
    if (index < 0 ||
        static_cast<std::size_t>(index) >= mapModelSlots.size()) return;
    mapModelSlots[static_cast<std::size_t>(index)].usesCommandRouting =
        static_cast<std::uint32_t>(value);
}

// @verified - p
std::uint64_t GetMapModelCommandRoutingEnabled(std::int32_t index) {
    return mapModelSlots.data()[index].usesCommandRouting;
}

// @verified - p
std::uint64_t GetMapModelGridCullingEnabled(std::int32_t index) {
    return mapModelSlots.data()[index].gridCullingEnabled;
}
// @verified - p
std::uint32_t* SetMapModelGridCullingEnabled(std::int32_t value, std::int32_t index) {
    mapModelSlots.data()[index].gridCullingEnabled = static_cast<std::uint32_t>(value);
    return &mapModelSlots[0].gridCullingEnabled;
}
std::uint32_t* SetMapModelAlternateTraversalEnabled(std::int32_t value, std::int32_t index) {
    if (index >= 33) return nullptr;
    mapModelSlots.data()[index].alternateTraversalEnabled = static_cast<std::uint32_t>(value);
    return &mapModelSlots[0].alternateTraversalEnabled;
}

// @verified - p
std::uint64_t HasMapModelReader(std::int32_t index) {
    return mapModelSlots.data()[index].reader != nullptr;
}

// @verified - p
MT7Node* GetMapModelRoot(std::int32_t index) {
    MT7Model* const reader = const_cast<MT7Model*>(mapModelSlots.data()[index].reader);
    return reader && !reader->nodes.empty() ? reader->nodes.data() : nullptr;
}

// @verified - p
std::uint64_t GetMapModelReaderMask() {
    std::uint32_t mask = 0;
    for (std::uint32_t index = 0; index < mapModelSlots.size(); ++index) {

        if (mapModelSlots[index].reader) mask |= 1U << (index & 31U);
    }
    return mask;
}

// @verified - p
std::uint64_t HasPropCharacterZeroUpperSignature(shen_name name) {
    if (!propRoot) return 0;
    MT7Node* const node = FindModelCharacterRecursive(propRoot, name);
    return node && (static_cast<std::uint32_t>(node->id) >> 16U) == 0;
}

// @verified - p
static void RecursiveSetPropDisplayFlag(MT7Node* node, std::uint32_t enabled) {
    for (; node; node = node->runtimeSibling) {
        node->id = static_cast<std::int32_t>((static_cast<std::uint32_t>(node->id) & 0xffffU) | ((enabled == 0 ? 1U : 0U) << 16U));
        node->visible = enabled != 0;
        if (node->runtimeChild) RecursiveSetPropDisplayFlag(node->runtimeChild, enabled);
    }
}

// @verified - p
void SetDisplayFlagForProp(shen_name name, std::uint32_t enabled) {
    if (!propRoot) return;
    MT7Node* const node = FindModelCharacterRecursive(propRoot, name);
    if (!node) return;
    node->id = static_cast<std::int32_t>((static_cast<std::uint32_t>(node->id) & 0xffffU) | ((enabled == 0 ? 1U : 0U) << 16U));
    node->visible = enabled != 0;
    if (node->runtimeChild) RecursiveSetPropDisplayFlag(node->runtimeChild, enabled);
}


void SetMapModelSlotPosition(const Vector3* position, std::int32_t index) {
    if (index >= 33) return;
    Vector3& target = mapModelSlots.data()[index].position;
    target.x = position->x;
    target.y = position->y;
    target.z = position->z;
}

// @verified - p
std::uint32_t* SetMapModelSlotRotation(std::int32_t rotationY, std::int32_t index) {
    mapModelSlots.data()[index].rotationY = static_cast<std::uint32_t>(rotationY);
    return &mapModelSlots[0].rotationY;
}

// @verified - p
std::uint32_t* SetMapModelSlotTransform(std::int32_t index, const Vector3* position, std::int32_t rotationY) {
    SetMapModelSlotPosition(position, index);
    return SetMapModelSlotRotation(rotationY, index);
}

// @verified - p
std::uint32_t* SetMapModelVertexShaderType(std::int32_t index, std::int32_t value) {
    mapModelSlots.data()[index].vertexShaderType = static_cast<std::uint32_t>(value);
    return &mapModelSlots[0].vertexShaderType;
}

void SetMapModelDataDefaultVertexShader() {
    for (MapModelRuntimeSlot& slot : mapModelSlots) {
        if (!slot.reader || slot.reader->nodes.empty()) continue;
        SetupModelDataDefaultVS(const_cast<MT7Node*>(slot.reader->nodes.data()));
        slot.forceLambertShader = 0;
    }
    SetDefaultVS();
}

// @verified - p
void SetDefaultVSByIndex(std::int32_t index) {
    if (index >= 33) return;
    MapModelRuntimeSlot& slot = mapModelSlots.data()[index];
    if (!slot.reader || slot.reader->nodes.empty()) return;
    SetupModelDataDefaultVS(const_cast<MT7Node*>(slot.reader->nodes.data()));
    slot.forceLambertShader = 0;
}

void SetMapModelWeatherLightingOverride(std::int32_t index,
                                        std::int32_t value) {
    if (index < 0 ||
        static_cast<std::size_t>(index) >= mapModelSlots.size()) {
        return;
    }
    mapModelSlots[static_cast<std::size_t>(index)]
        .weatherLightingOverride = static_cast<std::uint32_t>(value);
}

// @verified - p
std::int32_t GetNextEmptyMapModelSlot() {
    for (std::int32_t index = 0; index < 33; ++index)
        if (!mapModelSlots[index].reader) return index;
    return -1;
}

bool AssignPackageMapModel(std::int32_t index, MT7Model& model) {
    if (index < 0 || index >= 33) return false;
    MapModelRuntimeSlot& slot = mapModelSlots[index];
    if (slot.reader && !slot.reader->nodes.empty()) return true;
    const bool ready = SetupModel(static_cast<std::size_t>(index), model);
    slot.ownsReader = 0;
    return ready;
}

void RetirePackageMapModel(MT7Model& model) {
    for (std::size_t index = 0; index < mapModelSlots.size(); ++index)
        if (mapModelSlots[index].reader == &model) FreeMapModelSlot(index);
}



void FreeMainMap() {
    if (mainMapModelIndex < mapModels.size()) ReleaseModelFormat7(mapModels[mainMapModelIndex]);
    if (mainMapModelAllocation) HLib::FreeTaskData(mainMapModelAllocation);
    mainMapModelAllocation = nullptr;
    mainMapModelSize = 0;
}


MT7Model* GetMapsModel() {
    return mainMapModelIndex < mapModels.size() ? &mapModels[mainMapModelIndex] : nullptr;
}


void SetMapModelReader(MT7Model* model, std::int32_t index) {
    if (index >= 0 && index < 33) mapModelSlots[static_cast<std::size_t>(index)].reader = model;
}

void ResetMapsData() {
    // ResetMapsData: MAPS is independent of the
    // ordinary MAP/MAPnn readers and PACK payloads.
    FreeMapModelSlot(32);
    ResetMainMapProjection();
    if (mainMapModelIndex < mapModels.size()) {
        MT7Model& model = mapModels[mainMapModelIndex];
        // Keep ordinary slot indices stable until the full-map owner retires.
        ReleaseModelFormat7(model);
    }
    if (mainMapModelAllocation) {
        HLib::FreeTaskData(mainMapModelAllocation);
    }
    mainMapModelAllocation = nullptr;
    mainMapModelSize = 0;
    mainMapModelIndex = (std::numeric_limits<std::size_t>::max)();
    mainMapProjectedExtrusionDistance = 100.0f;
    AlignMainMap();
}

void FreeMapAssets() {
    ResetMapsData();
    ResetMapModelSlots();
    for (MT7Model& model : mapModels) {
        ReleaseModelFormat7(model);
    }
    mapModels.clear();
    for (void*& allocation : mapModelAllocations) {
        if (allocation) HLib::FreeTaskData(allocation);
        allocation = nullptr;
    }
    mapModelSlotIndices.fill((std::numeric_limits<std::size_t>::max)());
}

} // namespace shenmue::s2
