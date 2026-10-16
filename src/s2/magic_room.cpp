#include "s2/magic_room.hpp"

#include "d3t/object_store.hpp"
#include "s2/afs.hpp"
#include "s2/area_jump.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/world_render.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace shenmue::s2 {
namespace {
AfsArchiveSlot *roomArchive{};
d3t::LargeFileHandle *roomFile{};
MagicRoomHeader *roomDatabase{};
MagicRoomPlacement *roomPlacements{};
const MagicRoomFloorGroup *roomFloors{};
std::array<MT7Model, 50> roomModels{};
std::array<void *, 50> roomModelAllocations{};
std::array<MagicRoomEntityLight, 2> roomEntityLights{};
HLib::Task *roomExecutionTask{};
HLib::Task *roomDrawingTask{};
MagicRoomPlacement *roomLoadingPlacement{};
void *roomLoadingAllocation{};
std::uint64_t roomLoadingBytes{};
std::int32_t roomLoadingIndex{};
std::int32_t roomGeneratedCount{};
std::int32_t roomSeedBase{};
std::int32_t roomSeed{};
std::int32_t roomPreviousSeed{};
std::int32_t roomState{};
std::int32_t roomActiveFlag{};
std::uint32_t roomLoadingFrames{};
bool roomInitialized{};
bool roomDisplayPending{};
std::uint8_t roomBlockedAreaJump{};
char roomArchivePath[260]{};
Vector3 roomOrigin{};
Vector3 roomCollisionDimensions{};
std::uint32_t roomYaw{};
Vector3 roomDrawOrigin{};
std::uint32_t roomDrawYaw{};
float roomDrawRadius{};
std::int32_t roomDisabledLightCount{};
std::int16_t roomDisabledLights[4]{};

// @verified - p
bool IsPointWithinRoomLightQuad(const Vector2 *quad, float x, float z) {
    const float first = (z - quad[0].y) * (quad[1].x - quad[0].x) - (x - quad[0].x) * (quad[1].y - quad[0].y);
    const float second = (z - quad[1].y) * (quad[2].x - quad[1].x) - (x - quad[1].x) * (quad[2].y - quad[1].y);
    const float third = (quad[3].x - quad[2].x) * (z - quad[2].y) - (quad[3].y - quad[2].y) * (x - quad[2].x);
    const float fourth = (z - quad[3].y) * (quad[0].x - quad[3].x) - (x - quad[3].x) * (quad[0].y - quad[3].y);
    if (first <= 0.0f) {
        if (first >= 0.0f && !(second <= 0.0f))
            return third >= 0.0f && fourth >= 0.0f;
        if (second > 0.0f)
            return false;
        return third <= 0.0f && fourth <= 0.0f;
    }
    return second >= 0.0f && third >= 0.0f && fourth >= 0.0f;
}

// @verified - p
std::int32_t SelectRoomDescriptor(Vector3 *position, std::uint32_t *yaw, Vector3 *dimensions) {
    std::int32_t base = 0;
    const float x = position->x, z = position->z, y = position->y + 0.0099999998f;
    for (const MagicRoomFloorGroup *group = roomFloors; group->entryCount; ++group) {
        if (y > group->heightRange.x && group->heightRange.y > y) {
            for (std::int32_t index = 0; index < group->entryCount; ++index) {
                const MagicRoomDescriptor &descriptor = group->entries[index];
                const Vector2 *const q = descriptor.quad;
                const float first = (z - q[0].y) * (q[1].x - q[0].x) - (x - q[0].x) * (q[1].y - q[0].y);
                const float second = (z - q[1].y) * (q[2].x - q[1].x) - (x - q[1].x) * (q[2].y - q[1].y);
                bool positive;
                if (first > 0.0f) {
                    if (second < 0.0f)
                        continue;
                    positive = true;
                } else if (first == 0.0f) {
                    positive = second > 0.0f;
                } else {
                    if (!(second <= 0.0f))
                        continue;
                    positive = false;
                }
                const float third = (z - q[2].y) * (q[3].x - q[2].x) - (x - q[2].x) * (q[3].y - q[2].y);
                const float fourth = (z - q[3].y) * (q[0].x - q[3].x) - (x - q[3].x) * (q[0].y - q[3].y);
                if (positive ? !(third >= 0.0f) || fourth < 0.0f : third > 0.0f || fourth > 0.0f)
                    continue;
                position->x = descriptor.center.x;
                position->z = descriptor.center.y;
                std::memcpy(yaw, &descriptor.yaw, sizeof(*yaw));
                position->y = group->heightRange.x;
                *dimensions = {.x = descriptor.size.x, .y = group->heightRange.y - position->y, .z = descriptor.size.y};
                SetMagicRoomDimensions(dimensions->x, dimensions->y, dimensions->z);
                const std::int32_t result =
                    static_cast<std::int32_t>(static_cast<std::uint32_t>(base) + static_cast<std::uint32_t>(index) + static_cast<std::uint32_t>(roomSeedBase));
                dimensions->x -= 0.2f;
                dimensions->z -= 0.2f;
                return result;
            }
        }
        base = static_cast<std::int32_t>(static_cast<std::uint32_t>(base) + static_cast<std::uint32_t>(group->entryCount));
    }
    return -1;
}

// @verified - p
void EnableRoomLights() {
    for (std::int32_t index = 0; index < roomDisabledLightCount; ++index)
        SetLightEntryIndex(static_cast<std::uint32_t>(roomDisabledLights[index]), 1);
    roomDisabledLightCount = 0;
}

// @verified - p
void AssignRoomLightVectors(const Vector3 &origin, std::uint16_t yaw) {
    Vector3 dimensions;
    GetMagicRoomDimensions(&dimensions);
    const float halfWidth = dimensions.x * 0.5f, halfDepth = dimensions.z * 0.5f;
    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const float negativeCosine = -sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(yaw)));
    const float width = sine * halfDepth + negativeCosine * halfWidth;
    const float depth = sine * halfWidth - negativeCosine * halfDepth;
    const Vector2 quad[4]{
        {origin.x - width, origin.z - depth}, {origin.x + width, origin.z - depth}, {origin.x + width, origin.z + depth}, {origin.x - width, origin.z + depth}};
    roomDisabledLightCount = 0;
    Vector3 root;
    (void)GetLightRootPosition(&root);
    for (std::uint32_t index = 0; index < 256; ++index) {
        if (GetLightEntryIndex(index) != 1)
            continue;
        const LightRuntimeEntry &light = *GetLightRuntimeEntry(index);
        Vector3 position = light.vector20;
        if ((light.indexes[1] & 0xf7U) == 3U) {
            position.x += root.x;
            position.y += root.y;
            position.z += root.z;
        }
        if (position.y > origin.y && IsPointWithinRoomLightQuad(quad, position.x, position.z) && roomDisabledLightCount < 4) {
            SetLightEntryIndex(index, 0);
            roomDisabledLights[roomDisabledLightCount++] = static_cast<std::int16_t>(index);
        }
    }
}

// @verified - p
void InstallRoomCollision() {
    if (!roomGeneratedCount)
        return;
    flddClearAllGeneratedVolumes();
    (void)flddAssignGeneratedBox(1, roomOrigin, roomCollisionDimensions, static_cast<std::uint16_t>(roomYaw));
    (void)Matrix4x4_Push();
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::preTranslate(roomOrigin.x, roomOrigin.y, roomOrigin.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(roomYaw));
    for (std::int32_t index = 0; index < roomGeneratedCount; ++index) {
        const MagicRoomPlacement &placement = roomPlacements[index];
        if (placement.halfWidth == 0.0f)
            continue;
        (void)Matrix4x4_Push();
        Matrix4x4::preTranslate(placement.position.x, placement.position.y, placement.position.z);
        Matrix4x4::RotateAroundY_Wrapper(placement.yaw);
        Vector3 position{};
        Matrix4x4::TransformPoint(position, position);
        const Vector3 dimensions{.x = placement.halfWidth + placement.halfWidth,
            .y = placement.height < 1.0f ? 1.0f : placement.height,
            .z = placement.halfDepth + placement.halfDepth};
        (void)flddAssignGeneratedBox(0, position, dimensions, static_cast<std::uint16_t>(roomYaw + placement.yaw));
        Matrix4x4::Pop(1);
    }
    Matrix4x4::Pop(1);
}

void ReleaseRoomModels() {
    if (roomFile) {
        AfsReadNullsub(roomFile);
        d3t::ReadCachedFile(roomFile);
        roomFile = nullptr;
        HLib::FreeTaskData(roomLoadingAllocation);
        roomLoadingAllocation = nullptr;
    }
    for (std::int32_t index = 0; index < roomGeneratedCount; ++index) {
        MagicRoomPlacement &placement = roomPlacements[index];
        if (placement.modelHandle && (placement.modelIndex & 0x8000U) == 0) {
            ReleaseModelFormat7(*placement.modelHandle);
            HLib::FreeTaskData(roomModelAllocations[static_cast<std::size_t>(index)]);
            roomModelAllocations[static_cast<std::size_t>(index)] = nullptr;
            placement.modelHandle = nullptr;
        }
    }
}

void TransformRoomEntityLight(MagicRoomEntityLight &light, bool invertDirectionY) {
    Matrix4x4::TransformPoint(light.position, light.position);
    const Vector3 direction = light.direction;
    const Matrix4x4 &matrix = workTransformMatrix;
    light.direction = {(matrix.rows[1].x * direction.y + matrix.rows[0].x * direction.x) + matrix.rows[2].x * direction.z,
        (matrix.rows[1].y * direction.y + matrix.rows[0].y * direction.x) + matrix.rows[2].y * direction.z,
        (matrix.rows[1].z * direction.y + matrix.rows[0].z * direction.x) + matrix.rows[2].z * direction.z};
    if (invertDirectionY)
        light.direction.y = -light.direction.y;
}

bool LoadRoomModels() {
    if (roomFile) {
        const std::uint64_t state = GetAfsFileLoadState(roomFile);
        if (state == 2) {
            ++roomLoadingFrames;
            return true;
        }
        if (state != 3) {
            AfsReadNullsub(roomFile);
            return true;
        }
        d3t::ReadCachedFile(roomFile);
        roomFile = nullptr;
        roomLoadingFrames = 0;
        char path[260];
        std::snprintf(path, sizeof(path), "%s/afs%05d.mt7", roomArchivePath, static_cast<std::int16_t>(roomLoadingPlacement->modelIndex) + 1);
        MT7Model &model = roomModels[static_cast<std::size_t>(roomLoadingIndex)];
        const bool loaded =
            ReadModelFormat7({static_cast<const std::byte *>(roomLoadingAllocation), static_cast<std::size_t>(roomLoadingBytes)}, path, model, 0x22);
        if (!loaded)
            std::abort();
        roomLoadingPlacement->modelHandle = &model;
        roomModelAllocations[static_cast<std::size_t>(roomLoadingIndex)] = roomLoadingAllocation;
        roomLoadingPlacement->fadeAlpha = 0.0f;
        (void)d3t::SetObjectSourceActiveFlag(GetMT7ReaderData(&model), 1);
        (void)d3t::RegisterObjectPlacement(GetMT7ReaderData(&model), roomLoadingPlacement, 0);
        for (std::int32_t index = roomLoadingIndex + 1; index < roomGeneratedCount; ++index) {
            MagicRoomPlacement &placement = roomPlacements[index];
            if (placement.modelIndex == roomLoadingPlacement->modelIndex) {
                placement.modelIndex = 0xffff;
                placement.modelHandle = roomLoadingPlacement->modelHandle;
                (void)d3t::RegisterObjectPlacement(GetMT7ReaderData(&model), &placement, 0);
            }
        }
        if (roomLoadingPlacement->kind != static_cast<std::int32_t>(MagicRoomPlacementKind::Light))
            return true;
        const std::byte *chunk = static_cast<const std::byte *>(roomLoadingAllocation);
        for (std::int32_t index = 0; chunk && index < 2; ++index) {
            std::uint32_t name, size;
            std::memcpy(&name, chunk, 4);
            std::memcpy(&size, chunk + 4, 4);
            if (name == SHEN_NAME(LIGT)) {
                MagicRoomEntityLight light;
                std::memcpy(&light, chunk, sizeof(light));
                (void)Matrix4x4_Push();
                Matrix4x4::SetPrePassToTempTransform();
                Matrix4x4::preTranslate(roomOrigin.x, roomOrigin.y, roomOrigin.z);
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(roomYaw));
                const MagicRoomPlacement &placement = *roomLoadingPlacement;
                Matrix4x4::preTranslate(placement.position.x, placement.position.y, placement.position.z);
                if (placement.extraRotationZ)
                    Matrix4x4::RotateAroundZ_Wrapper(placement.extraRotationZ);
                if (placement.yaw)
                    Matrix4x4::RotateAroundY_Wrapper(placement.yaw);
                if (placement.extraRotationX)
                    Matrix4x4::RotateAroundX_Wrapper(placement.extraRotationX);
                TransformRoomEntityLight(light, true);
                Matrix4x4::Pop(1);
                Matrix4x4 entityMatrix;
                std::memcpy(&entityMatrix, &light, sizeof(entityMatrix));
                AddMagicRoomEntityMatrix(&entityMatrix);
                break;
            }
            if (size & 3U)
                break;
            chunk += 4U * (size >> 2U);
        }
        roomLoadingPlacement->kind = roomLoadingPlacement->modelHandle && GetChildObjectByIndex(model, model.nodes.data(), 0x98) ?
            static_cast<std::int32_t>(MagicRoomPlacementKind::Rotating) :
            0;
        return true;
    }
    for (std::int32_t index = 0; index < roomGeneratedCount; ++index) {
        MagicRoomPlacement &placement = roomPlacements[index];
        roomLoadingPlacement = &placement;
        if (placement.modelHandle)
            continue;
        roomLoadingIndex = index;
        roomFile = d3t::LargeFile_OpenTocEntryHandle(roomArchive->index, static_cast<std::int16_t>(placement.modelIndex) + 1);
        if (!roomFile)
            return false;
        const std::int64_t blocks = d3t::LargeFile_GetSizeInBlocks(roomFile);
        roomLoadingBytes = static_cast<std::uint64_t>(blocks) << 11U;
        roomLoadingAllocation = HLib::AllocateTaskData(roomLoadingBytes, TASK_MAGIC_ROOM);
        if (!roomLoadingAllocation)
            return false;
        (void)d3t::GetFileFromCachedBuffer(roomFile, static_cast<std::uint64_t>(blocks), roomLoadingAllocation, 0);
        return true;
    }
    return false;
}

void UpdateRoomAnimations() {
    if (!roomGeneratedCount)
        return;
    const std::int32_t lightCount = static_cast<std::int32_t>(GetMagicRoomEntityCount());
    if (lightCount) {
        for (std::int32_t index = 0; index < lightCount; ++index)
            if (const MagicRoomEntityLight *const light = GetMagicRoomEntityParameters(index))
                AssignLightParametersForMagicRoomEntity(static_cast<std::uint32_t>(index) + 254U, *light);
    } else {
        MagicRoomEntityLight light;
        std::memcpy(&light, magicRoomDefaultLightWords, sizeof(light));
        (void)Matrix4x4_Push();
        Matrix4x4::SetPrePassToTempTransform();
        Matrix4x4::preTranslate(roomOrigin.x, roomOrigin.y, roomOrigin.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(roomYaw));
        TransformRoomEntityLight(light, false);
        Matrix4x4::Pop(1);
        AssignLightParametersForMagicRoomEntity(254, light);
    }
    float radiusSquared = 0.0f;
    for (std::int32_t index = 0; index < roomGeneratedCount; ++index) {
        MagicRoomPlacement &placement = roomPlacements[index];
        if (!placement.kind || !placement.modelHandle || placement.modelHandle->nodes.empty())
            continue;
        MT7Model &model = *placement.modelHandle;
        MT7Node *const root = model.nodes.data();
        if (placement.kind == static_cast<std::int32_t>(MagicRoomPlacementKind::Rotating)) {
            for (std::uint16_t id : {std::uint16_t{0x98}, std::uint16_t{0x99}}) {
                MT7Node *const node = GetChildObjectByIndex(model, root, id);
                if (node && node != root)
                    node->rotation.y = static_cast<std::int32_t>(static_cast<std::uint32_t>(node->rotation.y) + 512U);
            }
        } else if (placement.kind == static_cast<std::int32_t>(MagicRoomPlacementKind::Clock)) {
            DateTime time;
            GetCurrentDateTime(time);
            const std::int32_t seconds = static_cast<std::int8_t>(time.second) + 60 * time.minute;
            const std::int32_t minutes = time.minute + 60 * (time.hour % 12);
            MT7Node *node = GetChildObjectByIndex(model, root, 0x96);
            if (node && node != root)
                node->rotation.z = -65536 * minutes / 720;
            node = GetChildObjectByIndex(model, root, 0x97);
            if (node && node != root)
                node->rotation.z = -65536 * seconds / 3600;
            node = GetChildObjectByIndex(model, root, 0x98);
            if (node && node != root) {
                node->id = static_cast<std::int32_t>(
                    (static_cast<std::uint32_t>(node->id) & 0xffffU) | ((static_cast<std::uint32_t>(node->id) + 0x04000000U) & 0xffff0000U));
                node->rotation.z = static_cast<std::int32_t>(sin_fixed16(static_cast<std::int16_t>(static_cast<std::uint32_t>(node->id) >> 16U)) * 2048.0f);
            }
        }
        const Vector3 &position = placement.position;
        const float length = position.y * position.y + position.x * position.x + position.z * position.z;
        if (length > radiusSquared)
            radiusSquared = length;
    }
    roomDrawOrigin = roomOrigin;
    roomDrawYaw = roomYaw;
    roomDrawRadius = std::sqrt(radiusSquared) + 1.0f;
    if (roomDrawRadius < 5.0f)
        roomDrawRadius = 5.0f;
    roomDisplayPending = true;
}

void DrawRoom(void *) {
    AssignMagicRoomLighting(roomDrawOrigin, roomDrawRadius);
    (void)Matrix4x4_Push();
    Matrix4x4::preTranslate(roomDrawOrigin.x, roomDrawOrigin.y, roomDrawOrigin.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(roomDrawYaw));
    for (std::int32_t index = 0; index < roomGeneratedCount; ++index) {
        MagicRoomPlacement &placement = roomPlacements[index];
        MT7Model *const model = placement.modelHandle;
        if (!model || model->nodes.empty())
            continue;
        if (placement.fadeAlpha < 1.0f) {
            placement.fadeAlpha += 0.050000001f;
            if (placement.fadeAlpha > 1.0f)
                placement.fadeAlpha = 1.0f;
        }
        (void)Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(roomDrawOrigin.x, roomDrawOrigin.y, roomDrawOrigin.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(roomDrawYaw));
        Matrix4x4::preTranslate(placement.position.x, placement.position.y, placement.position.z);
        Matrix4x4::RotateAroundY_Wrapper(placement.yaw);
        const bool hashed = placement.fadeAlpha >= 1.0f ? d3t::HashModel(&placement, GetLegacyPerspectiveProjection()) :
                                                          d3t::HashModelWithAlpha(&placement, placement.fadeAlpha, GetLegacyPerspectiveProjection());
        if (!hashed) {
            Matrix4x4::Pop(1);
            (void)Matrix4x4_Push();
            Matrix4x4::preTranslate(placement.position.x, placement.position.y, placement.position.z);
            Matrix4x4::RotateAroundY_Wrapper(placement.yaw);
            if (placement.fadeAlpha >= 1.0f)
                ConstructMatricesForObjectRecursive(*model, model->nodes.data());
            else
                PrepareProceduralObjects(*model, model->nodes.data(), placement.fadeAlpha);
        }
        Matrix4x4::Pop(1);
    }
    Matrix4x4::Pop(1);
}

// @verified - p
void DrawRoomCallback(void *) {
    if (roomDisplayPending)
        HLib::ExecuteCallbackNow(DrawRoom, nullptr);
    roomDisplayPending = false;
}

void ExecuteRoom(void *) {
    bool synchronous = false;
    roomDisplayPending = false;
    if (!IsWorldDrawingEnabled() || !roomInitialized)
        return;
    for (;;) {
        if (roomState == 0) {
            synchronous = true;
            const std::uint64_t state = GetAfsFileLoadState(roomFile);
            if (state == 1)
                return;
            if (state == 2)
                ++roomLoadingFrames;
            else if (state == 3) {
                (void)SetQronMapLightingEnabled(1);
                roomState = 1;
                d3t::ReadCachedFile(roomFile);
                roomFile = nullptr;
                if (ReadMagicRoomDatabase(roomDatabase)) {
                    roomState = 4;
                    return;
                }
                SetMagicRoomEntityBuffer(roomEntityLights.data(), 2);
                InitializeMagicRoomGeneration();
                roomSeed = -1;
                roomGeneratedCount = 0;
                roomPlacements = static_cast<MagicRoomPlacement *>(HLib::AllocateTaskData(0xaf0, TASK_MAGIC_ROOM));
                if (roomActiveFlag)
                    SelectMagicRoomForPlayer();
                if (roomSeed == -1) {
                    roomActiveFlag = 0;
                    return;
                }
            } else
                return;
        } else if (roomState == 1) {
            if (roomSeed != -1) {
                roomActiveFlag = 1;
                (void)EV_ResetFreeTalkFlags();
                AssignRoomLightVectors(roomOrigin, static_cast<std::uint16_t>(roomYaw));
                (void)FillMagicRoomCategorySeeds();
                roomGeneratedCount = static_cast<std::int32_t>(GenerateMagicRoomPlacements(roomPlacements, 50, roomSeed));
                InstallRoomCollision();
                roomPreviousSeed = roomSeed;
                roomState = 2;
            }
        } else if (roomState == 2) {
            if (roomSeed == roomPreviousSeed) {
                roomActiveFlag = 1;
                if (!LoadRoomModels())
                    roomState = 3;
                if (!synchronous)
                    UpdateRoomAnimations();
                (void)SetQronMapLightingEnabled(0);
            } else {
                EnableRoomLights();
                flddClearAllGeneratedVolumes();
                ReleaseRoomModels();
                roomPreviousSeed = roomSeed;
                if (roomSeed == -1) {
                    roomActiveFlag = 0;
                    (void)SetQronMapLightingEnabled(1);
                    roomState = 1;
                    return;
                }
                roomActiveFlag = 1;
                AssignRoomLightVectors(roomOrigin, static_cast<std::uint16_t>(roomYaw));
                (void)FillMagicRoomCategorySeeds();
                roomGeneratedCount = static_cast<std::int32_t>(GenerateMagicRoomPlacements(roomPlacements, 50, roomSeed));
                InstallRoomCollision();
            }
            if (roomState != 2)
                return;
        } else if (roomState == 3) {
            if (roomSeed == roomPreviousSeed) {
                roomActiveFlag = 1;
                UpdateRoomAnimations();
            } else if (roomSeed == -1) {
                roomActiveFlag = 0;
                roomPreviousSeed = -1;
                EnableRoomLights();
                flddClearAllGeneratedVolumes();
                ReleaseRoomModels();
                (void)SetQronMapLightingEnabled(1);
                roomState = 1;
            } else {
                roomActiveFlag = 1;
                roomState = 2;
            }
        }
        if (!synchronous)
            return;
        ST_LoadTitleFadeIn();
        FlushQueuedStripType3Batches();
        (void)ProfileFramePerformance();
    }
}
} // namespace

// @verified - p
void InitializeMagicRoom() {
    roomArchive = nullptr;
    roomFile = nullptr;
    roomBlockedAreaJump = 0;
}

void ReleaseMagicRoomRuntime() {
    if (!roomArchive)
        return;
    if (roomState == 0 && roomFile) {
        AfsReadNullsub(roomFile);
        d3t::ReadCachedFile(roomFile);
        roomFile = nullptr;
    } else {
        ReleaseRoomModels();
    }
    if (HLib::HasActiveTaskCallback(roomExecutionTask))
        HLib::DestroyTask(roomExecutionTask);
    if (HLib::HasActiveTaskCallback(roomDrawingTask))
        HLib::DestroyTask(roomDrawingTask);
    roomExecutionTask = nullptr;
    roomDrawingTask = nullptr;
    roomArchive = nullptr;
    roomLoadingAllocation = nullptr;
    roomGeneratedCount = 0;
    roomPlacements = nullptr;
}

// @verified - p
void SelectMagicRoomForPlayer() {
    if (!roomArchive)
        return;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    roomOrigin = GetCharacterWorldPosition(static_cast<Character *>(control->ryo));
    roomSeed = SelectRoomDescriptor(&roomOrigin, &roomYaw, &roomCollisionDimensions);
    if (roomSeed != -1)
        roomActiveFlag = 1;
}

// @verified - p
void SelectMagicRoomAtPosition(const Vector3 *position) {
    if (!roomArchive)
        return;
    roomOrigin = *position;
    roomSeed = SelectRoomDescriptor(&roomOrigin, &roomYaw, &roomCollisionDimensions);
    if (roomSeed != -1)
        roomActiveFlag = 1;
}

// @verified - p
void LeaveMagicRoom() {
    if (!roomArchive)
        return;
    roomSeed = -1;
    roomActiveFlag = 0;
    if (roomBlockedAreaJump) {
        roomBlockedAreaJump = 0;
        EV_SetCanDoAreaJumpTo(1);
    }
}

// @verified - p
std::uint64_t GetMagicRoomActiveFlag() {
    return static_cast<std::uint32_t>(roomActiveFlag);
}
// @verified - p
bool IsMagicRoomLoadingModels() {
    return roomArchive && roomState == 2;
}
// @verified - p
bool IsMagicRoomReady() {
    return roomState == 3;
}
// @verified - p
void SetMagicRoomActiveFlag(std::int32_t enabled) {
    roomActiveFlag = enabled;
}

void QueueMagicRoom(const char *directory, const char *archive, const MagicRoomFloorGroup *groups, std::int32_t seed) {
    if (roomArchive)
        return;
    roomInitialized = true;
    roomArchive = LoadAfsArchive(directory, archive, 2048);
    std::snprintf(roomArchivePath, sizeof(roomArchivePath), "%s/%s", directory, archive);
    roomFile = d3t::LargeFile_OpenTocEntryHandle(roomArchive->index, 0);
    const std::int64_t blocks = d3t::LargeFile_GetSizeInBlocks(roomFile);
    roomDatabase = static_cast<MagicRoomHeader *>(HLib::AllocateTaskData(static_cast<std::uint64_t>(blocks) << 11U, TASK_MAGIC_ROOM));
    (void)d3t::GetFileFromCachedBuffer(roomFile, static_cast<std::uint64_t>(blocks), roomDatabase, 0);
    roomSeedBase = seed;
    roomState = 0;
    roomLoadingFrames = 0;
    roomDisplayPending = false;
    roomExecutionTask = HLib::EnqueueTaskWithoutParameter(ExecuteRoom, 12, 2, static_cast<std::int32_t>(TASK_MAGIC_ROOM.name));
    roomDrawingTask = HLib::EnqueueTaskWithoutParameter(DrawRoomCallback, 13, 2, static_cast<std::int32_t>(TASK_MAGIC_ROOM.name));
    roomFloors = groups;
    ExecuteRoom(nullptr);
}
} // namespace shenmue::s2
