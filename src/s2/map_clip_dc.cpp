#include "s2/map_models_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/map_draw_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/map_load_dc.hpp"
#include "s2/field_depth_dc.hpp"
#endif

namespace shenmue {
namespace s2 {

const std::uint32_t mapModelCommandTypes[12] = SHENMUE_MAP_MODEL_COMMAND_TYPES;

std::uint32_t mapObjectClipRecordIndex;

const MapModelClipRecord *mapObjectClipRecords = 0, *activeMapObjectClipRecord = 0;

// @verified - d
void DrawMapModelNodes(const MT7Node *node, bool depthCull) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    if (node->mesh) {
        activeMapObjectClipRecord = &mapObjectClipRecords[mapObjectClipRecordIndex];
        const std::uint32_t type = activeMapObjectClipRecord->commandType;
        if (type != 255 && type != 240 && type != 244 && mapObjectVisibilityTests[type](activeMapObjectClipRecord->centre, activeMapObjectClipRecord->radius)) {
            if (depthCull) {
                if (camCtrlIsModelDepthVisible(activeMapObjectClipRecord->centre, *node, activeMapObjectClipRecord->radius)) {
                    dc::sdk::SetModelScale(activeMapObjectClipRecord->modelScale);
                    g_mapMeshDraw(node->mesh);
                }
            } else {
                dc::sdk::SetModelScale(activeMapObjectClipRecord->modelScale);
                g_mapMeshDraw(node->mesh);
            }
        }
        ++mapObjectClipRecordIndex;
    }
    if (node->child)
        DrawMapModelNodes(node->child, depthCull);
    if (node->sibling) {
        PopCurrentMatrix(1);
        DrawMapModelNodes(node->sibling, depthCull);
    }
}

// @verified - d
void DrawMapModelRoot(const MT7Node *node, std::int32_t index) {
    if (!node)
        return;
    mapObjectClipRecordIndex = 0;
    mapObjectClipRecords = GetMapModelClipRecords(index);
    Vector3 position;
    GetMapModelPosition(&position, index);
    const std::uint32_t yaw = GetMapModelRotationY(index);
    matrix_stack::Matrix4x4_Push(0);
    if (position.x != 0.0f || position.y != 0.0f || position.z != 0.0f || yaw) {
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
    }
    const bool depthCull = GetFieldDepthPointCount() && GetMapModelGridCullingEnabled(index);
    SetObjectCommandPlacementTransform();
    DrawMapModelNodes(node, depthCull);
    PopCurrentMatrix(1);
}

} // namespace s2
} // namespace shenmue

#include "common/matrix_stack.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/hlib.hpp"

#include <cmath>

namespace shenmue {
namespace s2 {

float g_mapCellScale;

namespace {
std::int32_t dangerCount, wallCount, cameraWallCount = 0;
std::int32_t nearCount, screenCount, farCount = 0, type4Count = 0;
std::int32_t commandCellCount, dangerCellCount = 0;
MapFieldDepthWall *walls, *cameraWalls = 0;
MapCellOffset *commandCells, *dangerCells;
float cellScale = 0.0f;
const std::byte *clipMesh;
const float *clipSphere;
MapModelClipRecord *clipRecord;
MapFieldDepthWall *wallSlot, *cameraWallSlot;
Matrix4x4 dangerTransform;
float clipCellPositionX, clipCellPositionZ, clipScale;
std::int16_t clipCellX, clipCellZ;
const std::byte *wallVertices;
Vector3 wallFirst, wallSecond;
} // namespace

// @verified - d
void SetMapCellSize(float size) {
    g_mapCellScale = 1.0f / size;
}

// @verified - d
float GetMapCellScale() {
    return g_mapCellScale;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetupOwnedMapModel(void *source, std::int32_t index);
void *shenLoadFile(const char *directory, const char *filename, shen_name allocationName);

void LoadOwnedMapModel(const char *directory, const char *filename, std::int32_t index) {
    if (!GetMapModelRoot(index))
        SetupOwnedMapModel(shenLoadFile(directory, filename, SHEN_NAME4('M', 'A', 'P', 'R')), index);
}
#endif

// @verified - d
void CountMapClipRecords(const MT7Node *node) {
    if (node->mesh) {
        ++mapObjectClipRecordIndex;
        if ((node->kind & 15) == 3)
            ++wallCount;
        if ((node->kind & 15) == 8)
            ++cameraWallCount;
    }
    if (node->child)
        CountMapClipRecords(node->child);
    if (node->sibling)
        CountMapClipRecords(node->sibling);
}

// @verified - d
std::int32_t AddMapCommandCell(std::int16_t x, std::int16_t z, MapCellOffset *cells) {
    if (cells->x == 0x5fff) {
        cells->x = x;
        cells->z = z;
        ++cells;
        cells->x = cells->z = 0x5fff;
        return 1;
    }
    do {
        if (cells->x == x && cells->z == z)
            return 0;
        ++cells;
    } while (cells[-1].x != 0x5fff);
    cells->x = cells[-1].x;
    cells->z = cells[-1].z;
    cells[-1].x = x;
    cells[-1].z = z;
    return 1;
}

// @verified - d
void MoveScreenSpaceCellFirst(std::int32_t count, MapCellOffset *cells) {
    if (cells->x == 0x4fff)
        return;
    MapCellOffset *const first = cells;
    std::int32_t found = 0;
    MapCellOffset saved;
    saved.x = cells->x;
    saved.z = cells->z;
    ++cells;
    for (std::int32_t index = 1; index < count; ++index, ++cells) {
        if (cells->x == 0x4fff) {
            found = 1;
            break;
        }
    }
    if (!found)
        return;
    cells->x = saved.x;
    cells->z = saved.z;
    first->x = first->z = 0x4fff;
}

// @verified - d
void FindMapWallEndpoints(const std::byte *vertices, Vector3 &first, Vector3 &second) {
    const float *vertex = reinterpret_cast<const float *>(vertices);
    first.x = vertex[0];
    first.y = vertex[1];
    first.z = vertex[2];
    vertex += 8;
    std::int32_t count = 3;
    while (count--) {
        if (first.y == vertex[1]) {
            second.x = *vertex++;
            second.y = *vertex++;
            second.z = *vertex;
            return;
        }
        vertex += 8;
    }
}

// @verified - d
void PlaceMapWallEndpoints(const Vector3 &position, std::int32_t yaw, float scale, Vector3 &first, Vector3 &second) {
    if (scale != 1.0f) {
        const float x = (second.x - first.x) * 0.5f + first.x;
        const float z = (second.z - first.z) * 0.5f + first.z;
        float deltaX = first.x - x, deltaZ = first.z - z;
        first.x = deltaX * scale + x;
        first.z = deltaZ * scale + z;
        deltaX = second.x - x;
        deltaZ = second.z - z;
        second.x = deltaX * scale + x;
        second.z = deltaZ * scale + z;
    }
    if (yaw) {
        const std::int32_t angle = static_cast<std::int32_t>(0U - static_cast<std::uint32_t>(yaw));
        const float sine = dc::sdk::njSin(angle);
        const float cosine = dc::sdk::CosFixed(angle);
        const float x = (second.x - first.x) * 0.5f + first.x;
        const float z = (second.z - first.z) * 0.5f + first.z;
        float deltaX = first.x - x, deltaZ = first.z - z;
        first.x = deltaX * cosine - deltaZ * sine + x;
        first.z = deltaX * sine + deltaZ * cosine + z;
        deltaX = second.x - x;
        deltaZ = second.z - z;
        second.x = deltaX * cosine - deltaZ * sine + x;
        second.z = deltaX * sine + deltaZ * cosine + z;
    }
    first.x += position.x;
    first.y += position.y;
    first.z += position.z;
    second.x += position.x;
    second.y += position.y;
    second.z += position.z;
}

// @verified - d
void BuildMapClipRecords(MT7Node *node) {
    if (node->mesh) {
        clipMesh = reinterpret_cast<const std::byte *>(node->mesh);
        clipRecord = const_cast<MapModelClipRecord *>(&mapObjectClipRecords[mapObjectClipRecordIndex]);
        clipSphere = reinterpret_cast<const float *>(clipMesh);
        ++clipSphere;
        ++clipSphere;
        clipRecord->centre.x = *clipSphere++;
        clipRecord->centre.y = *clipSphere++;
        clipRecord->centre.z = *clipSphere++;
        clipRecord->radius = *clipSphere++;
        clipScale = node->scale.x;
        if (node->scale.y > clipScale)
            clipScale = node->scale.y;
        if (clipScale < node->scale.z)
            clipScale = node->scale.z;
        clipRecord->radius *= clipScale;
        clipRecord->modelScale = clipScale;
        clipRecord->object = node;
        clipCellPositionX = (clipRecord->centre.x + node->position.x) * cellScale;
        clipCellPositionZ = (clipRecord->centre.z + node->position.z) * cellScale;
        if ((static_cast<std::uint16_t>(node->kind) & 15U) > 11U)
            node->kind = static_cast<std::int16_t>(static_cast<std::uint16_t>(node->kind) & 0xfff0U);
        const std::uint32_t type = clipRecord->commandType = mapModelCommandTypes[node->kind & 15];
        if (type != 255 && type != 240 && type != 244 && static_cast<std::int32_t>(type) > 16)
            clipRecord->commandType = 0;
        if (clipCellPositionX < 0.0f)
            clipCellPositionX -= 1.0f;
        if (clipCellPositionZ < 0.0f)
            clipCellPositionZ -= 1.0f;
        clipCellX = static_cast<std::int16_t>(clipCellPositionX);
        clipCellZ = static_cast<std::int16_t>(clipCellPositionZ);
        switch (clipRecord->commandType) {
        case 16:
            ++screenCount;
            clipCellX = clipCellZ = 0x4fff;
            break;
        case 8:
            ++nearCount;
            break;
        case 12:
            ++farCount;
            break;
        case 4:
            ++type4Count;
            break;
        }
        clipRecord->cellX = clipCellX;
        clipRecord->cellZ = clipCellZ;
        if (clipRecord->commandType == 240) {
            wallVertices = clipMesh + 0x70;
            FindMapWallEndpoints(wallVertices, wallFirst, wallSecond);
            PlaceMapWallEndpoints(node->position, node->rotation.y, clipScale, wallFirst, wallSecond);
            wallSlot = &walls[wallCount];
            wallSlot->first = wallFirst;
            wallSlot->second = wallSecond;
            ++wallCount;
        } else if (clipRecord->commandType == 244) {
            wallVertices = clipMesh + 0x70;
            FindMapWallEndpoints(wallVertices, wallFirst, wallSecond);
            PlaceMapWallEndpoints(node->position, node->rotation.y, clipScale, wallFirst, wallSecond);
            cameraWallSlot = &cameraWalls[cameraWallCount];
            cameraWallSlot->first = wallFirst;
            cameraWallSlot->second = wallSecond;
            ++cameraWallCount;
        } else if (clipRecord->commandType != 255) {
            commandCellCount += AddMapCommandCell(clipCellX, clipCellZ, commandCells);
        } else {
            matrix_stack::Matrix4x4_Push(0);
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            matrix_stack::GetMatrix(&dangerTransform);
            PopCurrentMatrix(1);
            clipRecord->centre.x = dangerTransform.rows[3].x;
            clipRecord->centre.y = dangerTransform.rows[3].y;
            clipRecord->centre.z = dangerTransform.rows[3].z;
            dangerCellCount += AddMapCommandCell(clipCellX, clipCellZ, dangerCells);
            ++dangerCount;
        }
        ++mapObjectClipRecordIndex;
    }
    if (node->child)
        BuildMapClipRecords(node->child);
    if (node->sibling)
        BuildMapClipRecords(node->sibling);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void BuildMapModelCommandCells(std::int32_t index, std::int32_t count, std::int32_t dangers, std::int32_t wallsCount, std::int32_t cameraWallsCount) {
    MapCellOffset *cell;
    MapModelCommandCell *cells, *run;
    MapModelClipRecord **output;
    const MapModelClipRecord *clip;
    Vector4 *sphere;
    std::int32_t countCells, i, record, firstRecord;
    cell = commandCells;
    countCells = 0;
    for (; cell->x != 0x5fff; ++cell)
        ++countCells;
    SetMapModelCommandCellCount(countCells, index);
    cells = static_cast<MapModelCommandCell *>(HLib::AllocateTaskData((countCells + 1) * 12U, MAP_COMMAND_CELLS));
    SetMapModelCommandCells(cells, index);
    output = static_cast<MapModelClipRecord **>(HLib::AllocateTaskData((count - dangers - wallsCount - cameraWallsCount + 1) * 4U, MAP_COMMAND_ROUTING));
    SetMapModelCommandRouting(output, index);
    cell = commandCells;
    MoveScreenSpaceCellFirst(countCells, cell);
    run = cells;
    for (record = 0; record < countCells; ++run, ++record, ++cell) {
        run->x = cell->x;
        run->z = cell->z;
        run->count = 0;
        run->firstRecord = 0;
    }
    run = cells;
    for (i = 0; i < countCells; ++i, ++run) {
        clip = GetMapModelClipRecords(index);
        for (record = 0; record < count; ++record, ++clip) {
            if (clip->commandType != 255 && clip->commandType != 240 && clip->commandType != 244 &&
                *reinterpret_cast<const std::uint32_t *>(run) == *reinterpret_cast<const std::uint32_t *>(&clip->cellX)) {
                ++run->count;
                *output++ = const_cast<MapModelClipRecord *>(clip);
            }
        }
    }
    run = cells;
    run->firstRecord = 0;
    firstRecord = 0;
    ++run;
    for (i = 1; i < countCells; ++i, ++run) {
        firstRecord += run[-1].count;
        run->firstRecord = firstRecord;
    }
    if (dangers) {
        cell = dangerCells;
        countCells = 0;
        for (; cell->x != 0x5fff; ++cell)
            ++countCells;
        cells = static_cast<MapModelCommandCell *>(HLib::AllocateTaskData(countCells * 12U, MAP_DANGER_CELLS));
        SetMapFieldDepthCells(cells, index);
        SetMapFieldDepthCellCount(countCells, index);
        cell = dangerCells;
        run = cells;
        for (record = 0; record < countCells; ++run, ++record, ++cell) {
            run->x = cell->x;
            run->z = cell->z;
            run->count = 0;
            run->firstRecord = 0;
        }
        sphere = GetMapFieldDepthSpheres(index);
        run = cells;
        for (i = 0; i < countCells; ++i, ++run) {
            clip = GetMapModelClipRecords(index);
            for (record = 0; record < count; ++record, ++clip) {
                if (clip->commandType == 255 && *reinterpret_cast<const std::uint32_t *>(run) == *reinterpret_cast<const std::uint32_t *>(&clip->cellX)) {
                    ++run->count;
                    sphere->x = clip->centre.x;
                    sphere->y = clip->centre.y;
                    sphere->z = clip->centre.z;
                    sphere->w = clip->radius * 0.69f;
                    ++sphere;
                }
            }
        }
        run = cells;
        run->firstRecord = 0;
        firstRecord = 0;
        ++run;
        for (i = 1; i < countCells; ++i, ++run) {
            firstRecord += run[-1].count;
            run->firstRecord = firstRecord;
        }
    }
}
#else
// @verified - d
void BuildMapModelCommandCells(std::int32_t index, std::int32_t count, std::int32_t dangers, std::int32_t wallsCount, std::int32_t cameraWallsCount) {
    MapModelRuntimeSlot &slot = g_mapModelSlots[index];
    std::int32_t countCells = 0;
    for (MapCellOffset *cell = commandCells; cell->x != 0x5fff; ++cell)
        ++countCells;
    slot.commandCellCount = static_cast<std::uint32_t>(countCells);
    slot.commandCells = static_cast<MapModelCommandCell *>(HLib::AllocateTaskData((countCells + 1) * 12U, MAP_COMMAND_CELLS));
    slot.commandRouting =
        static_cast<MapModelClipRecord **>(HLib::AllocateTaskData((count - dangers - wallsCount - cameraWallsCount + 1) * 4U, MAP_COMMAND_ROUTING));
    MoveScreenSpaceCellFirst(countCells, commandCells);
    for (std::int32_t cell = 0; cell < countCells; ++cell) {
        slot.commandCells[cell].x = commandCells[cell].x;
        slot.commandCells[cell].z = commandCells[cell].z;
        slot.commandCells[cell].count = 0;
        slot.commandCells[cell].firstRecord = 0;
    }
    MapModelClipRecord **output = slot.commandRouting;
    for (std::int32_t cell = 0; cell < countCells; ++cell) {
        MapModelCommandCell &run = slot.commandCells[cell];
        for (std::int32_t record = 0; record < count; ++record) {
            MapModelClipRecord &clip = slot.clipRecords[record];
            if (clip.commandType != 255 && clip.commandType != 240 && clip.commandType != 244 && run.x == clip.cellX && run.z == clip.cellZ) {
                ++run.count;
                *output++ = &clip;
            }
        }
    }
    slot.commandCells[0].firstRecord = 0;
    std::int32_t firstRecord = 0;
    for (std::int32_t cell = 1; cell < countCells; ++cell) {
        firstRecord += slot.commandCells[cell - 1].count;
        slot.commandCells[cell].firstRecord = firstRecord;
    }
    if (dangers) {
        MapFieldDepthSlot &field = g_mapFieldDepthSlots[index];
        countCells = 0;
        for (MapCellOffset *cell = dangerCells; cell->x != 0x5fff; ++cell)
            ++countCells;
        field.dangerCells = static_cast<MapModelCommandCell *>(HLib::AllocateTaskData(countCells * 12U, MAP_DANGER_CELLS));
        field.dangerCellCount = countCells;
        for (std::int32_t cell = 0; cell < countCells; ++cell) {
            field.dangerCells[cell].x = dangerCells[cell].x;
            field.dangerCells[cell].z = dangerCells[cell].z;
            field.dangerCells[cell].count = 0;
            field.dangerCells[cell].firstRecord = 0;
        }
        Vector4 *sphere = field.dangerSpheres;
        for (std::int32_t cell = 0; cell < countCells; ++cell) {
            MapModelCommandCell &run = field.dangerCells[cell];
            for (std::int32_t record = 0; record < count; ++record) {
                const MapModelClipRecord &clip = slot.clipRecords[record];
                if (clip.commandType == 255 && run.x == clip.cellX && run.z == clip.cellZ) {
                    ++run.count;
                    sphere->x = clip.centre.x;
                    sphere->y = clip.centre.y;
                    sphere->z = clip.centre.z;
                    sphere->w = clip.radius * 0.69f;
                    ++sphere;
                }
            }
        }
        field.dangerCells[0].firstRecord = 0;
        firstRecord = 0;
        for (std::int32_t cell = 1; cell < countCells; ++cell) {
            firstRecord += field.dangerCells[cell - 1].count;
            field.dangerCells[cell].firstRecord = firstRecord;
        }
    }
}
#endif

// @verified - d
void SetMapFieldDepthEnabled(std::uint32_t enabled, std::int32_t index) {
    MapFieldDepthSlot &field = g_mapFieldDepthSlots[index];
    field.enabled = !field.dangerSpheres && !field.walls && !field.cameraWalls ? 0 : enabled;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AllocForCLIP(MT7Node *root, std::int32_t index) {
    mapObjectClipRecordIndex = 0;
    wallCount = cameraWallCount = 0;
    CountMapClipRecords(root);
    SetMapModelClipRecords(static_cast<MapModelClipRecord *>(HLib::AllocateTaskData((mapObjectClipRecordIndex + 1) * 32U, MAP_CLIP_STORAGE)), index);
    if (wallCount)
        SetMapFieldDepthWalls(static_cast<MapFieldDepthWall *>(HLib::AllocateTaskData((wallCount + 1) * 24U, MAP_WALL_STORAGE)), index);
    SetMapFieldDepthWallCount(wallCount, index);
    if (cameraWallCount)
        SetMapFieldDepthCameraWalls(static_cast<MapFieldDepthWall *>(HLib::AllocateTaskData((cameraWallCount + 1) * 24U, MAP_CAMERA_WALL_STORAGE)), index);
    SetMapFieldDepthCameraWallCount(cameraWallCount, index);
    mapObjectClipRecordIndex = 0;
    dangerCount = 0;
    wallCount = 0;
    cameraWallCount = 0;
    nearCount = 0;
    screenCount = 0;
    farCount = 0;
    type4Count = 0;
    mapObjectClipRecords = GetMapModelClipRecords(index);
    walls = GetMapFieldDepthWalls(index);
    cameraWalls = GetMapFieldDepthCameraWalls(index);
    commandCells = static_cast<MapCellOffset *>(HLib::AllocateStaticOrDynamicBlock(0x800, MAP_COMMAND_TEMP_STORAGE));
    dangerCells = static_cast<MapCellOffset *>(HLib::AllocateStaticOrDynamicBlock(0x800, MAP_DANGER_TEMP_STORAGE));
    commandCells->x = 0x5fff;
    commandCells->z = 0x5fff;
    dangerCells->x = 0x5fff;
    dangerCells->z = 0x5fff;
    cellScale = GetMapCellScale();
    commandCellCount = 0;
    dangerCellCount = 0;
    matrix_stack::PushUnitMatrix();
    BuildMapClipRecords(root);
    PopCurrentMatrix(1);
    SetMapFieldDepthSpheres(0, index);
    SetMapFieldDepthSlotEnabled(0, index);
    if (dangerCount) {
        SetMapFieldDepthSpheres(static_cast<Vector4 *>(HLib::AllocateTaskData(dangerCount * 16U, MAP_DANGER_STORAGE)), index);
        SetMapFieldDepthSphereCount(dangerCount, index);
        SetMapFieldDepthSlotEnabled(1, index);
    }
    if (wallCount || cameraWallCount)
        SetMapFieldDepthSlotEnabled(1, index);
    BuildMapModelCommandCells(index, mapObjectClipRecordIndex, dangerCount, wallCount, cameraWallCount);
    HLib::FreeTaskData(commandCells);
    HLib::FreeTaskData(dangerCells);
    SetMapModelDrawableRecordCount(mapObjectClipRecordIndex - dangerCount - wallCount - cameraWallCount, index);
    SetMapModelScreenSpaceRecordCount(screenCount, index);
    SetMapModelNearRecordCount(nearCount, index);
    SetMapModelDangerRecordCount(dangerCount, index);
    SetMapModelFarRecordCount(farCount, index);
    SetMapModelType4RecordCount(type4Count, index);
    SetMapModelWallRecordCount(wallCount, index);
    SetMapModelWallCRecordCount(cameraWallCount, index);
}
#else
// @verified - d
void AllocForCLIP(MT7Node *root, std::int32_t index) {
    mapObjectClipRecordIndex = 0;
    cameraWallCount = 0;
    wallCount = 0;
    CountMapClipRecords(root);
    MapModelRuntimeSlot &slot = g_mapModelSlots[index];
    MapFieldDepthSlot &field = g_mapFieldDepthSlots[index];
    slot.clipRecords = static_cast<MapModelClipRecord *>(HLib::AllocateTaskData((mapObjectClipRecordIndex + 1) * 32U, MAP_CLIP_STORAGE));
    if (wallCount)
        field.walls = static_cast<MapFieldDepthWall *>(HLib::AllocateTaskData((wallCount + 1) * 24U, MAP_WALL_STORAGE));
    field.wallCount = wallCount;
    if (cameraWallCount)
        field.cameraWalls = static_cast<MapFieldDepthWall *>(HLib::AllocateTaskData((cameraWallCount + 1) * 24U, MAP_CAMERA_WALL_STORAGE));
    field.cameraWallCount = cameraWallCount;
    mapObjectClipRecordIndex = 0;
    dangerCount = 0;
    wallCount = 0;
    cameraWallCount = 0;
    nearCount = 0;
    screenCount = 0;
    farCount = 0;
    type4Count = 0;
    mapObjectClipRecords = slot.clipRecords;
    walls = field.walls;
    cameraWalls = field.cameraWalls;
    commandCells = static_cast<MapCellOffset *>(HLib::AllocateStaticOrDynamicBlock(0x800, MAP_COMMAND_TEMP_STORAGE));
    dangerCells = static_cast<MapCellOffset *>(HLib::AllocateStaticOrDynamicBlock(0x800, MAP_DANGER_TEMP_STORAGE));
    commandCells[0] = (__typeof__(commandCells[0])){0x5fff, 0x5fff};
    dangerCells[0] = (__typeof__(dangerCells[0])){0x5fff, 0x5fff};
    cellScale = GetMapCellScale();
    commandCellCount = 0;
    dangerCellCount = 0;
    matrix_stack::PushUnitMatrix();
    BuildMapClipRecords(root);
    PopCurrentMatrix(1);
    field.dangerSpheres = 0;
    SetMapFieldDepthEnabled(0, index);
    if (dangerCount) {
        field.dangerSpheres = static_cast<Vector4 *>(HLib::AllocateTaskData(dangerCount * 16U, MAP_DANGER_STORAGE));
        field.dangerCount = dangerCount;
        SetMapFieldDepthEnabled(1, index);
    }
    if (wallCount || cameraWallCount)
        SetMapFieldDepthEnabled(1, index);
    BuildMapModelCommandCells(index, mapObjectClipRecordIndex, dangerCount, wallCount, cameraWallCount);
    HLib::FreeTaskData(commandCells);
    HLib::FreeTaskData(dangerCells);
    slot.drawableRecordCount = static_cast<std::uint32_t>(mapObjectClipRecordIndex - wallCount - dangerCount - cameraWallCount);
    slot.screenSpaceRecordCount = static_cast<std::uint32_t>(screenCount);
    slot.nearRecordCount = static_cast<std::uint32_t>(nearCount);
    slot.dangerRecordCount = static_cast<std::uint32_t>(dangerCount);
    slot.farRecordCount = static_cast<std::uint32_t>(farCount);
    slot.type4RecordCount = static_cast<std::uint32_t>(type4Count);
    slot.wallRecordCount = static_cast<std::uint32_t>(wallCount);
    slot.wallCRecordCount = static_cast<std::uint32_t>(cameraWallCount);
}
#endif

} // namespace s2
} // namespace shenmue

#endif
