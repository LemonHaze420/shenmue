#include "s2/magic_room.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "platform/dc/sdk.hpp"
#endif

#include <math.h>
#include <cstring>

namespace shenmue {
namespace s2 {
MagicRoomHeader *g_magicRoomDatabase = 0;
MagicRoomGroup *g_magicRoomGroups = 0;
MagicRoomModel *g_magicRoomModels;
namespace {
#if SHENMUE_IS_DC
MagicRoomGenerationState roomGeneration;
#else
std::uint8_t roomModes[9] = {};
MagicRoomLimitValue roomCapacity[45] = {};
MagicRoomLimitValue roomUsage[45] = {};
std::uint8_t roomModeSeed = 0;
Vector3 roomDimensions = {};
MagicRoomCursorState roomCursor = {};
float roomBoundaryX = 0;
#endif
std::uint32_t roomRngState = 0;
std::int16_t roomCandidates[128];
std::int32_t roomEntityCount;
std::int32_t roomEntityCapacity;
std::byte *roomEntityBuffer;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t roomTransformReflection;

void TransformMagicRoomPlacements(
    MagicRoomPlacement *placements, std::int32_t count, std::uint32_t outerYaw, float baseX, float baseZ, float scaleX, float scaleZ) {
    std::int32_t xIndex;
    std::int32_t zIndex;
    std::int32_t index;
    char *position;
    float x;
    float z;

    roomTransformReflection = 0;
    if (outerYaw == 0x4000 || outerYaw == 0xc000) {
        xIndex = 2;
        zIndex = 0;
        if (scaleX == scaleZ)
            roomTransformReflection = 1;
    } else {
        xIndex = 0;
        zIndex = 2;
        if (scaleX != scaleZ)
            roomTransformReflection = 1;
    }
    if (roomTransformReflection) {
        for (index = 0; index < count; ++index, ++placements) {
            position = (char *)&placements->position;
            x = baseX + scaleX * *(float *)(position + (xIndex << 2));
            z = baseZ + scaleZ * *(float *)(position + (zIndex << 2));
            placements->position.x = x;
            placements->position.z = z;
            placements->yaw = outerYaw - placements->yaw;
        }
    } else {
        for (index = 0; index < count; ++index, ++placements) {
            position = (char *)&placements->position;
            x = baseX + scaleX * *(float *)(position + (xIndex << 2));
            z = baseZ + scaleZ * *(float *)(position + (zIndex << 2));
            placements->position.x = x;
            placements->position.z = z;
            placements->yaw += outerYaw;
        }
    }
}
#else
void TransformMagicRoomPlacements(
    MagicRoomPlacement *placements, std::int32_t count, std::uint32_t outerYaw, float baseX, float baseZ, float scaleX, float scaleZ) {
    const bool quarterTurn = ((outerYaw - 0x4000U) & 0xffff7fffU) == 0;
    const bool reflection = quarterTurn ? scaleX == scaleZ : scaleX != scaleZ;
    for (std::int32_t index = 0; index < count; ++index) {
        MagicRoomPlacement &placement = placements[index];
        const float x = (quarterTurn ? placement.position.z : placement.position.x) * scaleX;
        const float z = (quarterTurn ? placement.position.x : placement.position.z) * scaleZ;
        placement.yaw = static_cast<std::uint16_t>(reflection ? outerYaw - placement.yaw : outerYaw + placement.yaw);
        placement.position.z = z + baseZ;
        placement.position.x = x + baseX;
    }
}
#endif
MagicRoomInteger ConstructMagicRoomSeatingPlacement(MagicRoomPlacement *placements, int modelIndex, int remaining, float *usedWidth, float *usedDepth);
void ExpandMagicRoomDimensions(MagicRoomModel *model, Vector3 *outDim);
#if SHENMUE_IS_DC
MagicRoomInteger FindMagicRoomCandidates(const std::int8_t *a1, int a2, float x, float z);
#else
MagicRoomInteger FindMagicRoomCandidates(MagicRoomInteger a1, int a2, float x, float z);
#endif
MagicRoomInteger PickMagicRoomBaseModel(int a1);
MagicRoomInteger FindMagicRoomBaseCandidates(int a1, float a2, float a3);
MagicRoomInteger ConstructMagicRoomSecondaryPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5);
#if SHENMUE_IS_DC
int ConstructMagicRoomFurniture(MagicRoomPlacement *out, int modelIndex, int remaining, float *usedWidth, float *usedDepth, int orientation);
#else
int ConstructMagicRoomFurniture(
    MagicRoomPlacement *out, std::uint16_t modelIndex, int remaining, float *usedWidth, float *usedDepth, std::uint16_t orientation);
#endif
MagicRoomInteger ConstructMagicRoomAxisPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5);
MagicRoomInteger ConstructMagicRoomBasicPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5);
MagicRoomInteger FillMagicRoomLowerWall(MagicRoomPlacement *a1, int a2);
MagicRoomInteger FillMagicRoomLowerRightCorner(MagicRoomPlacement *a1, int a2);
MagicRoomInteger FillMagicRoomRightWall(MagicRoomPlacement *a1, int a2);
MagicRoomInteger FillMagicRoomUpperRightCorner(MagicRoomPlacement *a1, int a2);
MagicRoomInteger FillMagicRoomLowerLeftCorner(MagicRoomPlacement *a1, int a2);
MagicRoomInteger FillMagicRoomLeftWall(MagicRoomPlacement *a1, int a2);
MagicRoomInteger FillMagicRoomUpperLeftCorner(MagicRoomPlacement *a1, int a2);
MagicRoomInteger ConstructMagicRoomSpecialPlacement(MagicRoomPlacement *a1, int a2);
MagicRoomInteger ConstructMagicRoomGroupedPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5);
int AddMagicRoomSurfaceDecoration(MagicRoomPlacement *outChild, const MagicRoomPlacement *parent, const MagicRoomModel *parentModel, int remaining);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
int TryAddMagicRoomSurfaceDecoration(MagicRoomPlacement *outChild, const MagicRoomPlacement *parent, const MagicRoomModel *parentModel, int remaining);
std::uint32_t MagicRoomRandom();
std::uint32_t MagicRoomRandomRange(std::uint32_t range);
float MagicRoomRandomFloat();
#endif

// @verified - d
void ExpandMagicRoomDimensions(MagicRoomModel *model, Vector3 *outDim) {
    float Z;
    float v4;
    float X;
    float v6;
    float v7;
    float v8;
    float v9;

    outDim->x = model->dimensions.x;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float *outZ = &outDim->z;
    *outZ = model->dimensions.z;
#else
    Z = model->dimensions.z;
    outDim->z = Z;
#endif
    switch (model->category) {
    case 4:
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        outDim->z = outDim->z * 1.5f;
#else
        outDim->z = Z * 1.5f;
#endif
        break;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    case 6:
#else
    case 5:
    case 6:
    case 9:
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        outDim->x = outDim->x * 1.2f;
        outDim->z = outDim->z * 3.0f;
#else
        v4 = outDim->z * 3.0f;
        outDim->x = outDim->x * 1.2f;
        outDim->z = v4;
#endif
        break;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    case 7:
        outDim->x = outDim->x * 1.2f;
        outDim->z = outDim->z * 1.2f;
        break;
#endif
    case 11:
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        v6 = outDim->x / outDim->z;
        if (v6 > 0.89999998f && v6 < 1.1f) {
            outDim->x = outDim->x + 1.6f;
            outDim->z = outDim->z + 3.2f;
            break;
        }
        outDim->x = outDim->x * 1.5f;
        outDim->z = outDim->z + 3.2f;
#else
        X = outDim->x;
        v6 = outDim->x / Z;
        if (v6 <= 0.89999998f || v6 >= 1.1f)
            v7 = X * 1.5f;
        else
            v7 = X + 1.6f;
        v8 = outDim->z + 3.2f;
        outDim->x = v7;
        outDim->z = v8;
#endif
        break;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    case 5:
    case 9:
        outDim->x = outDim->x * 1.2f;
        outDim->z = outDim->z * 3.0f;
        break;
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    case 7:
        outDim->x = outDim->x * 1.2f;
        outDim->z = outDim->z * 1.2f;
        break;
    case 28:
        outDim->x = 0.0f;
        outDim->z = 0.0f;
        break;
#endif
    case 18:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 43:
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        v9 = sqrtf((float)(outDim->x * outDim->x) + (float)(outDim->z * outDim->z));
#else
        v9 = sqrtf((float)(Z * Z) + (float)(outDim->x * outDim->x));
#endif
        outDim->x = v9;
        outDim->z = v9;
        break;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    case 28:
        outDim->x = 0.0f;
        outDim->z = 0.0f;
        break;
#endif
    default:
        outDim->x = outDim->x * 1.1f;
        outDim->z = outDim->z * 1.1f;
        break;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void AdjustMagicRoomSeatingExtent(const Vector3 *dimensions, const Vector3 *secondary, Vector3 *extent) {
    extent->x = dimensions->x;
    extent->z = dimensions->z;
    if (!secondary)
        return;
    const float ratio = extent->x / extent->z;
    if (ratio > 0.89999998f && 1.1f > ratio) {
        extent->x = secondary->x + secondary->x + extent->x;
        extent->z = secondary->z * 4.0f + extent->z;
    } else {
        extent->x = extent->x * 1.5f;
        extent->z = secondary->z * 4.0f + extent->z;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MagicRoomInteger FindMagicRoomCandidates(const std::int8_t *categories, int count, float x, float z) {
#else
MagicRoomInteger FindMagicRoomCandidates(MagicRoomInteger a1, int a2, float x, float z) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t found = 0;
    for (std::int32_t index = 0; index < count; ++index) {
        const std::int32_t category = categories[index];
        if (roomGeneration.usage[category] < roomGeneration.capacity[category]) {
            std::int32_t end = g_magicRoomGroups[category].count;
            end += g_magicRoomGroups[category].start;
            for (std::int32_t model = g_magicRoomGroups[category].start; model < end; ++model) {
                Vector3 dimensions;
                ExpandMagicRoomDimensions(&g_magicRoomModels[model], &dimensions);
                if (x > dimensions.x && z > dimensions.z) {
                    roomCandidates[found++] = static_cast<std::int16_t>(model);
                    if (found >= 128)
                        return found;
#else
    unsigned int v4;
    MagicRoomInteger v8;
    MagicRoomInteger v9;
    MagicRoomInteger v10;
    MagicRoomInteger v11;
    std::int32_t start;
    std::int32_t v13;
    MagicRoomModel *v14;
    float v15;
    float v16;
    float v17;

    v4 = 0;
    v8 = a2;
    if (a2 > 0) {
        v9 = 0;
        v10 = 0;
        while (1) {
            v11 = *(char *)(v9 + a1);
            if (roomUsage[v11] < roomCapacity[v11]) {
                start = g_magicRoomGroups[v11].start;
                v13 = start + g_magicRoomGroups[v11].count;
                if (start < v13)
                    break;
            }
        LABEL_20:
            if (++v9 >= v8)
                return v4;
        }
        while (1) {
            v14 = &g_magicRoomModels[start];
            v15 = v14->dimensions.x;
            v16 = v14->dimensions.z;
            switch (v14->category) {
            case 4:
                v17 = v16 * 1.5f;
                break;
            case 5:
            case 6:
            case 9:
                v15 = v15 * 1.2f;
                v17 = v16 * 3.0f;
                break;
            case 7:
                v15 = v15 * 1.2f;
                v17 = v16 * 1.2f;
                break;
            case 11:



                if ((float)(v15 / v16) <= 0.89999998f || (float)(v15 / v16) >= 1.1f) {

                    v15 = v15 * 1.5f;
                    v17 = v16 + 3.2f;
                } else {
                    v15 = v15 + 1.6f;
                    v17 = v16 + 3.2f;
                }
                break;
            case 18:
            case 37:
            case 38:
            case 39:
            case 40:
            case 41:
            case 43:
                v17 = sqrtf((float)(v16 * v16) + (float)(v15 * v15));
                v15 = v17;
                break;
            case 28:
                v15 = 0.0f;
                v17 = 0.0f;
                break;
            default:
                v15 = v15 * 1.1f;
                v17 = v16 * 1.1f;
                break;
            }
            if (x > v15 && z > v17) {
                roomCandidates[v10] = start;
                ++v4;
                if (++v10 >= 128)
                    break;
            }
            if (++start >= v13)
                goto LABEL_20;
#endif
                }
            }
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return v4;
#endif
        }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    }
    return found;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t PickMagicRoomCandidate(const std::int8_t *categories, int count, float x, float z, int orientation) {
    if (orientation == 0xc000 || orientation == 0x4000) {
        const float swap = x;
        x = z;
        z = swap;
    }
    const std::uint32_t candidates = static_cast<std::uint32_t>(FindMagicRoomCandidates(categories, count, x, z));
    orientation = -1;
    if (candidates != 0) {
        orientation = static_cast<std::uint16_t>(roomCandidates[MagicRoomRandomRange(candidates)]);
        roomGeneration.usage[g_magicRoomModels[orientation].category]++;
    }
    return orientation;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t PickMagicRoomCategoryModel(int category) {
    std::int32_t result = -1;
    if (static_cast<std::int8_t>(roomGeneration.usage[category]) < static_cast<std::int8_t>(roomGeneration.capacity[category])) {
        ++roomGeneration.usage[category];
        result = static_cast<std::int32_t>(MagicRoomRandomRange(g_magicRoomGroups[category].count));
        result += g_magicRoomGroups[category].start;
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MagicRoomInteger PickMagicRoomBaseModel(int index) {
#else
MagicRoomInteger PickMagicRoomBaseModel(int a1) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const std::uint32_t count =
        static_cast<std::uint32_t>(FindMagicRoomBaseCandidates(index, roomGeneration.dimensions.x * 2.0f, roomGeneration.dimensions.y * 2.0f));
    index = -1;
    if (count != 0) {
        index = static_cast<std::uint16_t>(roomCandidates[MagicRoomRandomRange(count)]);
        roomGeneration.usage[g_magicRoomModels[index].category]++;
#else
    unsigned int v1;
    MagicRoomInteger result;
    MagicRoomInteger category;

    v1 = FindMagicRoomBaseCandidates(a1, roomDimensions.x * 2.0f, roomDimensions.y * 2.0f);
    if (v1 == 0)
        return 0xFFFFFFFFLL;

    roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
    result = (std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v1];
    category = g_magicRoomModels[result].category;
    ++roomUsage[category];
    return result;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return index;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t PlaceMagicRoomBaseModel(MagicRoomPlacement *placement, std::int32_t remaining, int category, float y) {
    std::int32_t result = 0;
    const std::int32_t model = static_cast<std::int32_t>(PickMagicRoomBaseModel(category));
    if (model != -1 && remaining != 0) {
        placement->kind = 0;
        placement->position.x = 0.0f;
        placement->position.y = y;
        placement->position.z = 0.0f;
        placement->halfWidth = 0.0f;
        placement->height = 0.0f;
        placement->halfDepth = 0.0f;
        placement->yaw = 0;
        placement->modelIndex = static_cast<std::uint16_t>(model);
        result = 1;
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MagicRoomInteger FindMagicRoomBaseCandidates(int category, float width, float depth) {
#else
MagicRoomInteger FindMagicRoomBaseCandidates(int a1, float a2, float a3) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t count = 0;
    if (roomGeneration.usage[category] < roomGeneration.capacity[category]) {
        const float maximumWidth = width + 0.5f;
        const float maximumDepth = depth + 0.5f;
        std::int32_t end = g_magicRoomGroups[category].count;
        end += g_magicRoomGroups[category].start;
        for (std::int32_t index = g_magicRoomGroups[category].start; index < end; ++index) {
            const float x = g_magicRoomModels[index].dimensions.x;
            const float z = g_magicRoomModels[index].dimensions.z;
            if (x > width && maximumWidth > x && z > depth && maximumDepth > z) {
                roomCandidates[count++] = static_cast<std::int16_t>(index);
                if (count >= 128)
                    return count;
#else
    MagicRoomInteger v3;
    unsigned int v4;
    float v7;
    float v8;
    std::int32_t start;
    std::int32_t count;
    MagicRoomModel *v11;
    std::int16_t *v12;
    MagicRoomInteger v13;
    float X;
    float Z;

    v3 = a1;
    v4 = 0;
    if (roomUsage[a1] < roomCapacity[a1]) {
        v7 = a2 + 0.5f;
        v8 = a3 + 0.5f;
        start = g_magicRoomGroups[a1].start;
        count = start + g_magicRoomGroups[v3].count;
        if (start < count) {
            v11 = g_magicRoomModels;
            v12 = roomCandidates;
            do {
                v13 = start;
                X = v11[v13].dimensions.x;
                Z = v11[v13].dimensions.z;
                if (X > a2 && v7 > X && Z > a3 && v8 > Z) {
                    *v12 = start;
                    ++v4;
                    if ((MagicRoomInteger)++v12 >= (MagicRoomInteger)(roomCandidates + 128))
                        break;
                }
                ++start;
            } while (start < count);
#endif
            }
        }
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return v4;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return count;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MagicRoomInteger ConstructMagicRoomSecondaryPlacement(MagicRoomPlacement *out, int modelIndex, int remaining, float *usedWidth, float *usedDepth) {
    MagicRoomModel *model;
    MagicRoomModel *partner;
    float width;
    float depth;
    int partnerIndex;
    MagicRoomInteger result;
    Vector3 extent;

    if (remaining == 0)
        return 0;
    model = &g_magicRoomModels[modelIndex];
    width = *usedWidth;
    depth = *usedDepth;
    result = 0;
    out->position.x = model->dimensions.x * 0.5f;
    out->position.y = 0.0f;
    out->position.z = model->dimensions.z * 0.5f;
    out->halfWidth = model->dimensions.x * 0.5f;
    out->height = model->dimensions.y;
    out->halfDepth = model->dimensions.z * 0.5f;
    out->yaw = 0;
    out->modelIndex = modelIndex;
    ExpandMagicRoomDimensions(model, &extent);
    *usedWidth = extent.x;
    *usedDepth = extent.z;
    result++;
    partnerIndex = PickMagicRoomCandidate(magicRoomSecondaryCategories, 1, width * 0.5f, depth * 0.5f, 0);
    if (partnerIndex != -1) {
        partner = &g_magicRoomModels[partnerIndex];
        if (depth > model->dimensions.z * 1.2f + partner->dimensions.z && remaining > 1) {
            *usedDepth = model->dimensions.z + model->dimensions.z + partner->dimensions.z;
            out[1].position.x = out->position.x;
            out[1].position.y = 0.0f;
            out[1].position.z = model->dimensions.z * 1.2f + partner->dimensions.z * 0.5f;
            out[1].halfWidth = partner->dimensions.x * 0.5f;
            out[1].height = partner->dimensions.y;
            out[1].halfDepth = partner->dimensions.z * 0.5f;
            out[1].yaw = 0x8000;
            out[1].modelIndex = partnerIndex;
            result++;
        }
    }
    return result;
}
#else
MagicRoomInteger ConstructMagicRoomSecondaryPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5) {
    float v9;
    MagicRoomModel *v10;
    float v11;
    float v12;
    float Z;
    float X;
    float v15;
    unsigned int v16;
    unsigned int v17;
    MagicRoomModel *v18;
    int v19;
    MagicRoomInteger category;
    float *p_X;
    float v22;
    float v23;
    Vector3 outDim;

    if (a3 == 0)
        return 0;
    v9 = *a4;
    v10 = &g_magicRoomModels[a2];
    v11 = *a5;
    v12 = v10->dimensions.x * 0.5f;
    a1->position.y = 0.0f;
    a1->position.x = v12;
    a1->position.z = v10->dimensions.z * 0.5f;
    a1->halfWidth = v10->dimensions.x * 0.5f;
    a1->height = v10->dimensions.y;
    Z = v10->dimensions.z;
    a1->modelIndex = a2;
    a1->yaw = 0;
    a1->halfDepth = Z * 0.5f;
    ExpandMagicRoomDimensions(v10, &outDim);
    X = outDim.x;
    v15 = outDim.z;
    *a4 = outDim.x;
    v16 = 1;
    *a5 = v15;
    v17 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 7), 1, v9 * 0.5f, v11 * 0.5f);
    if (v17 != 0) {
        v18 = g_magicRoomModels;
        roomRngState = 1103515245U * roomRngState + 34567;
        v19 = (std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v17];
        category = g_magicRoomModels[(std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v17]].category;
        ++roomUsage[category];
        p_X = &v18[v19].dimensions.x;
        v22 = p_X[2];
        if (v11 > (float)((float)(v10->dimensions.z * 1.2f) + v22) && a3 > 1) {
            v16 = 2;
            *a5 = (float)(v10->dimensions.z + v10->dimensions.z) + v22;
            a1[1].position.x = a1->position.x;
            a1[1].position.y = 0.0f;
            a1[1].position.z = (float)(p_X[2] * 0.5f) + (float)(v10->dimensions.z * 1.2f);
            a1[1].halfWidth = *p_X * 0.5f;
            a1[1].height = p_X[1];
            v23 = p_X[2] * 0.5f;
            a1[1].yaw = 0x8000;
            a1[1].halfDepth = v23;
            a1[1].modelIndex = v19;
        }
    }
    a1->halfWidth = X * 0.5f;
    a1->halfDepth = v15 * 0.5f;
    return v16;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
int ConstructMagicRoomFurniture(MagicRoomPlacement *out, int modelIndex, int remaining, float *usedWidth, float *usedDepth, int orientation) {
#else
int ConstructMagicRoomFurniture(
    MagicRoomPlacement *out, std::uint16_t modelIndex, int remaining, float *usedWidth, float *usedDepth, std::uint16_t orientation) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    MagicRoomInteger result = 0;
    float *swap;
    MagicRoomModel *model;

    if (orientation == 0xC000 || orientation == 0x4000) {
        swap = usedWidth;
#else
    float *v7;
    MagicRoomModel *v8;
    int result;
    float v10;
    float v11;
    float v12;

    if (((orientation - 16384) & ~32768) != 0) {
        v7 = usedDepth;
    } else {
        v7 = usedWidth;
#endif
        usedWidth = usedDepth;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        usedDepth = swap;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    model = &g_magicRoomModels[modelIndex];
#else
    v8 = g_magicRoomModels;
#endif
    out->kind = 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    switch (model->category) {
    case 13:
        result += ConstructMagicRoomGroupedPlacement(out, modelIndex, remaining, usedWidth, usedDepth);
        break;
#else
    switch (v8[modelIndex].category) {
#endif
    case 6:
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        result += ConstructMagicRoomSecondaryPlacement(out, modelIndex, remaining, usedWidth, usedDepth);
#else
        result = ConstructMagicRoomSecondaryPlacement(out, modelIndex, remaining, usedWidth, v7);
#endif
        break;
    case 11:
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        result += ConstructMagicRoomSeatingPlacement(out, modelIndex, remaining, usedWidth, usedDepth);
#else
        result = ConstructMagicRoomSeatingPlacement(out, modelIndex, remaining, usedWidth, v7);
#endif
        break;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    case 31:
        result += ConstructMagicRoomAxisPlacement(out, modelIndex, remaining, usedWidth, usedDepth);
        out->position.y = (1.7f - out->height) + MagicRoomRandomFloat() * 0.30000001f;
        break;
    case 32:
        result += ConstructMagicRoomAxisPlacement(out, modelIndex, remaining, usedWidth, usedDepth);
        out->position.y = (2.2f - out->height) + MagicRoomRandomFloat() * 0.30000001f;
        out->kind = 3;
        break;
    case 30:
        result += ConstructMagicRoomAxisPlacement(out, modelIndex, remaining, usedWidth, usedDepth);
        out->position.y = (roomGeneration.dimensions.z - out->height * 0.5f) * (0.5f + MagicRoomRandomFloat() * 0.2f);
#else
    case 13:
        result = ConstructMagicRoomGroupedPlacement(out, modelIndex, remaining, usedWidth, v7);
#endif
        break;
    case 18:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 43:
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        result += ConstructMagicRoomBasicPlacement(out, modelIndex, remaining, usedWidth, usedDepth);
        out->yaw = MagicRoomRandom();
#else
        result = ConstructMagicRoomBasicPlacement(out, modelIndex, remaining, usedWidth, v7);
        roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
        out->yaw = (roomRngState >> 16U) & 0x7FFF;
        break;
    case 30:
        result = ConstructMagicRoomAxisPlacement(out, modelIndex, remaining, usedWidth, v7);
        v12 = roomDimensions.z - (float)(out->height * 0.5f);
        roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
        out->position.y = v12 * (float)((float)((float)((float)((roomRngState >> 16U) & 0x7FFF) / 32767.0f) * 0.2f) + 0.5f);
        break;
    case 31:
        result = ConstructMagicRoomAxisPlacement(out, modelIndex, remaining, usedWidth, v7);
        v10 = 1.7f - out->height;
        roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
        out->position.y = v10 + (float)((float)((float)((roomRngState >> 16U) & 0x7FFF) / 32767.0f) * 0.30000001f);
        break;
    case 32:
        result = ConstructMagicRoomAxisPlacement(out, modelIndex, remaining, usedWidth, v7);
        v11 = 2.2f - out->height;
        roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
        out->position.y = v11 + (float)((float)((float)((roomRngState >> 16U) & 0x7FFF) / 32767.0f) * 0.30000001f);
        out->kind = 3;
#endif
        break;
    default:
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        result += ConstructMagicRoomAxisPlacement(out, modelIndex, remaining, usedWidth, usedDepth);
#else
        result = ConstructMagicRoomAxisPlacement(out, modelIndex, remaining, usedWidth, v7);
#endif
        break;
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return static_cast<int>(result);
#else
    return result;
#endif
}

MagicRoomInteger ConstructMagicRoomAxisPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5) {
    MagicRoomModel *model;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    int v11;
    int v12;
#else
    int v10;
    MagicRoomInteger v11;
    MagicRoomInteger v12;
    float v13;
    float Z;
#endif
    Vector3 outDim;

    if (a3 == 0)
        return 0;
    model = &g_magicRoomModels[a2];
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const float width = *a4;
    const float depth = *a5;
    int count = 0;
    if (model->dimensions.x > width || model->dimensions.z > depth) {
        v11 = 2;
        v12 = 0;
#else
    if (model->dimensions.x > *a4 || model->dimensions.z > *a5) {
        v10 = 0;
#endif
        a1->yaw = 0x4000;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        v12 = 0;
        v11 = 2;
#endif
    } else {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        v10 = 0;
        a1->yaw = 0;
#endif
        v11 = 0;
        v12 = 2;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        a1->yaw = 0;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    a1->position.x = *(float *)((char *)&model->dimensions + (v11 << 2)) * 0.5f;
#else
    v13 = *(&model->dimensions.x + v11) * 0.5f;
#endif
    a1->position.y = 0.0f;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    a1->position.z = *(float *)((char *)&model->dimensions + (v12 << 2)) * 0.5f;
#else
    a1->position.x = v13;
    a1->position.z = *(&model->dimensions.x + v12) * 0.5f;
#endif
    a1->halfWidth = model->dimensions.x * 0.5f;
    a1->height = model->dimensions.y;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    a1->halfDepth = model->dimensions.z * 0.5f;
#else
    Z = model->dimensions.z;
#endif
    a1->modelIndex = a2;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    a1->halfDepth = Z * 0.5f;
#endif
    ExpandMagicRoomDimensions(model, &outDim);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    *a4 = *(float *)((char *)&outDim + (v11 << 2));
    *a5 = *(float *)((char *)&outDim + (v12 << 2));
    ++count;
    count += TryAddMagicRoomSurfaceDecoration(&a1[count], a1, model, a3);
    return count;
#else
    *a4 = *(&outDim.x + v11);
    *a5 = *(&outDim.x + v12);
    if (model->category == 7)
        v10 = AddMagicRoomSurfaceDecoration(a1 + 1, a1, model, a3);
    return (unsigned int)(v10 + 1);
#endif
}

MagicRoomInteger ConstructMagicRoomBasicPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    MagicRoomModel *model;
    int v11;
    int v12;
    float width;
    float depth;
#else
    std::uint16_t v7;
    MagicRoomModel *v9;
    int v10;
    Vector3 *p_Z;
    Vector3 *p_outDim;
    float X;
    float v14;
    float v15;
#endif
    Vector3 outDim;

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    v7 = a2;
#endif
    if (a3 == 0)
        return 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    model = &g_magicRoomModels[a2];
    width = *a4;
    depth = *a5;
    int count = 0;
    if (model->dimensions.x > width || model->dimensions.z > depth) {
        v11 = 2;
        v12 = 0;
#else
    v9 = &g_magicRoomModels[a2];
    v10 = 0;
    if (v9->dimensions.x > *a4 || v9->dimensions.z > *a5) {
        p_Z = (Vector3 *)&outDim.z;
#endif
        a1->yaw = 0x4000;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        p_outDim = &outDim;
#endif
    } else {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        v11 = 0;
        v12 = 2;
#endif
        a1->yaw = 0;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        p_Z = &outDim;
        p_outDim = (Vector3 *)&outDim.z;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    ExpandMagicRoomDimensions(model, &outDim);
    const float expandedWidth = *(&outDim.x + v11);
    *a4 = expandedWidth;
    const float expandedDepth = *(&outDim.x + v12);
    *a5 = expandedDepth;
    a1->position.x = expandedWidth * 0.5f;
#else
    ExpandMagicRoomDimensions(v9, &outDim);
    X = p_Z->x;
    v14 = p_outDim->x;
    *a4 = p_Z->x;
    *a5 = v14;
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    a1->position.y = 0.0f;
#else
    a1->position.y = 0.02f;
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    a1->position.z = expandedDepth * 0.5f;
    a1->halfWidth = model->dimensions.x * 0.5f;
    a1->height = model->dimensions.y;
    a1->halfDepth = model->dimensions.z * 0.5f;
    a1->modelIndex = static_cast<std::uint16_t>(a2);
    ++count;
    count += TryAddMagicRoomSurfaceDecoration(&a1[count], a1, model, a3);
    return count;
#else
    a1->position.x = X * 0.5f;
    a1->position.z = v14 * 0.5f;
    a1->halfWidth = v9->dimensions.x * 0.5f;
    a1->height = v9->dimensions.y;
    v15 = v9->dimensions.z * 0.5f;
    a1->modelIndex = v7;
    a1->halfDepth = v15;
    if (v9->category == 7)
        v10 = AddMagicRoomSurfaceDecoration(a1 + 1, a1, v9, a3);
    return (unsigned int)(v10 + 1);
#endif
}

MagicRoomInteger FillMagicRoomLowerWall(MagicRoomPlacement *a1, int a2) {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    float X;
#endif
    float v5;
    float v6;
    int v7;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t v9;
#else
    unsigned int v8;
    std::uint16_t v9;
    MagicRoomInteger category;
#endif
    int v11;
    float usedWidth;
    float usedDepth;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    int orientation;

    if (roomGeneration.cursor.firstThree.x < roomGeneration.cursor.firstThree.z)
        v5 = roomGeneration.dimensions.y + roomGeneration.cursor.firstThree.x;
    else
        v5 = roomGeneration.dimensions.y + roomGeneration.cursor.firstThree.z;
    orientation = 0;
#else
    X = roomCursor.firstThree.x;
    if (roomCursor.firstThree.z <= roomCursor.firstThree.x)
        X = roomCursor.firstThree.z;
    v5 = X + roomDimensions.y;
    v6 = roomBoundaryX;
#endif
    v7 = 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    v6 = roomGeneration.boundaryX;
#endif
    do {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        usedWidth = v6 - roomGeneration.cursor.firstThree.y;
#endif
        usedDepth = v5;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        v9 = PickMagicRoomCandidate(magicRoomLowerWallCategories, 11, usedWidth, v5, orientation);
        if (v9 != -1) {
            v11 = ConstructMagicRoomFurniture(&a1[v7], v9, a2, &usedWidth, &usedDepth, orientation);
            TransformMagicRoomPlacements(&a1[v7], v11, orientation, v6, -roomGeneration.dimensions.y, -1.0f, 1.0f);
            v7 += v11;
#else
        usedWidth = v6 - roomCursor.firstThree.y;
        v8 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 48), 11, v6 - roomCursor.firstThree.y, v5);
        if (v8 == 0)
            break;
        roomRngState = 0x41C64E6DU * roomRngState + 34567;
        v9 = roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v8];
        category = g_magicRoomModels[v9].category;
        ++roomUsage[category];
        v11 = ConstructMagicRoomFurniture(&a1[v7], v9, a2, &usedWidth, &usedDepth, 0);
        TransformMagicRoomPlacements(&a1[v7], v11, 0, v6, -roomDimensions.y, -1.0f, 1.0f);
#endif
            v6 = v6 - usedWidth;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        }
    } while (v9 != -1 && v7 < a2);
#else
        v7 += v11;
    } while (v7 < a2);
#endif
    return (unsigned int)v7;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MagicRoomInteger FillMagicRoomLowerRightCorner(MagicRoomPlacement *placements, int capacity) {
#else
MagicRoomInteger FillMagicRoomLowerRightCorner(MagicRoomPlacement *a1, int a2) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    float usedWidth = roomGeneration.dimensions.x - roomGeneration.boundaryX;
    float usedDepth = roomGeneration.dimensions.y + roomGeneration.cursor.firstThree.x;
    std::int32_t count = 0;
#else
    unsigned int v2;
#endif
    int orientation;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (MagicRoomRandomRange(2) != 0)
#else
    float v5;
    float v6;
    float v8;
    float v9;
    unsigned int v10;
    std::uint16_t v11;
    MagicRoomInteger category;
    MagicRoomInteger result;
    float v14;
    float usedWidth;
    float usedDepth;

    v2 = 0;
#endif
        orientation = 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    else
        orientation = 0xc000;
    const std::int32_t model = PickMagicRoomCandidate(magicRoomLowerCornerCategories, 6, usedWidth, usedDepth, orientation);
    if (model != -1) {
        count += ConstructMagicRoomFurniture(placements, model, capacity, &usedWidth, &usedDepth, orientation);
        TransformMagicRoomPlacements(placements, count, orientation, roomGeneration.dimensions.x, -roomGeneration.dimensions.y, -1.0f, 1.0f);
#else
    v5 = roomDimensions.x - roomBoundaryX;
    v6 = roomCursor.firstThree.x + roomDimensions.y;
    roomRngState = 1103515245U * roomRngState + 34567;
    usedWidth = roomDimensions.x - roomBoundaryX;
    usedDepth = roomCursor.firstThree.x + roomDimensions.y;
    v8 = roomCursor.firstThree.x + roomDimensions.y;
    if ((roomRngState & 0x10000) == 0)
        orientation = 49152;
    v9 = roomDimensions.x - roomBoundaryX;
    if ((roomRngState & 0x10000) == 0) {
        v9 = roomCursor.firstThree.x + roomDimensions.y;
        v8 = roomDimensions.x - roomBoundaryX;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    roomGeneration.boundaryX = roomGeneration.dimensions.x - usedWidth;
    roomGeneration.cursor.firstThree.x = -roomGeneration.dimensions.y + usedDepth;
    roomGeneration.cursor.firstThree.y = roomGeneration.boundaryX;
    if (roomGeneration.cursor.x3Boundary > roomGeneration.boundaryX)
        roomGeneration.cursor.d3Boundary = roomGeneration.cursor.firstThree.x;
    return count;
#else
    v10 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 0), 6, v9, v8);
    if (v10 != 0) {
        roomRngState = 1103515245U * roomRngState + 34567;
        v11 = roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v10];
        category = g_magicRoomModels[v11].category;
        ++roomUsage[category];
        v2 = ConstructMagicRoomFurniture(a1, v11, a2, &usedWidth, &usedDepth, orientation);
        TransformMagicRoomPlacements(a1, v2, orientation, roomDimensions.x, -roomDimensions.y, -1.0f, 1.0f);
        v5 = usedWidth;
        v6 = usedDepth;
#endif
}
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    result = v2;
    v14 = v6 - roomDimensions.y;
    roomCursor.firstThree.x = v14;
    roomBoundaryX = roomDimensions.x - v5;
    roomCursor.firstThree.y = roomDimensions.x - v5;
    if (roomCursor.x3Boundary > (float)(roomDimensions.x - v5))
        roomCursor.d3Boundary = v14;
    return result;
}
#endif

MagicRoomInteger FillMagicRoomRightWall(MagicRoomPlacement *a1, int a2) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float v2;
    float d2;
    int v7;
    std::int32_t v9;
    int v11;
    float usedDepth;
    float usedWidth;
    int orientation;

    d2 = roomGeneration.cursor.d2;
    orientation = 0xc000;
    v7 = 0;
    if (roomGeneration.cursor.firstThree.y < roomGeneration.cursor.x3Boundary)
        v2 = roomGeneration.cursor.x3Boundary;
    else
        v2 = roomGeneration.cursor.firstThree.y;
    do {
        usedWidth = roomGeneration.dimensions.x - v2;
        usedDepth = d2 - roomGeneration.cursor.firstThree.x;
        v9 = PickMagicRoomCandidate(magicRoomSideWallCategories, 16, usedWidth, usedDepth, orientation);
        if (v9 != -1) {
            v11 = ConstructMagicRoomFurniture(&a1[v7], v9, a2, &usedWidth, &usedDepth, orientation);
            TransformMagicRoomPlacements(&a1[v7], v11, orientation, roomGeneration.dimensions.x, d2, -1.0f, -1.0f);
            v7 += v11;
            d2 = d2 - usedDepth;
            if (roomGeneration.cursor.x3 > roomGeneration.dimensions.x - usedWidth)
                roomGeneration.cursor.x3 = roomGeneration.dimensions.x - usedWidth;
        }
    } while (v9 != -1 && v7 < a2);
    return v7;
#else
    float v2;
    float d2;
    float X;
    int v7;
    unsigned int v8;
    std::uint16_t v9;
    MagicRoomInteger category;
    int v11;
    float usedDepth;
    float usedWidth;

    v2 = fmaxf(roomCursor.x3Boundary, roomCursor.firstThree.y);

    d2 = roomCursor.d2;
    X = roomDimensions.x;
    v7 = 0;
    do {
        usedWidth = X - v2;
        usedDepth = d2 - roomCursor.firstThree.x;
        v8 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 32), 16, d2 - roomCursor.firstThree.x, X - v2);
        if (v8 == 0)
            break;
        roomRngState = 1103515245U * roomRngState + 34567;
        v9 = roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v8];
        category = g_magicRoomModels[v9].category;
        ++roomUsage[category];
        v11 = ConstructMagicRoomFurniture(&a1[v7], v9, a2, &usedWidth, &usedDepth, 0xC000u);
        TransformMagicRoomPlacements(&a1[v7], v11, 49152, roomDimensions.x, d2, -1.0f, -1.0f);
        X = roomDimensions.x;
        v7 += v11;
        d2 = d2 - usedDepth;
        if (roomCursor.x3 > (float)(roomDimensions.x - usedWidth))
            roomCursor.x3 = roomDimensions.x - usedWidth;
    } while (v7 < a2);
    return (unsigned int)v7;
#endif
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MagicRoomInteger FillMagicRoomUpperRightCorner(MagicRoomPlacement *placements, int capacity) {
#else
MagicRoomInteger FillMagicRoomUpperRightCorner(MagicRoomPlacement *a1, int a2) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    float usedWidth = roomGeneration.dimensions.x - roomGeneration.cursor.x3;
    float usedDepth = roomGeneration.dimensions.y - roomGeneration.cursor.d2;
    std::int32_t count = 0;
#else
    unsigned int v2;
#endif
    int orientation;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (MagicRoomRandomRange(2) != 0)
#else
    float v5;
    float v6;
    float v8;
    float v9;
    unsigned int v10;
    std::uint16_t v11;
    MagicRoomInteger category;
    float usedWidth;
    float usedDepth;

    v2 = 0;
#endif
        orientation = 0x8000;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    else
        orientation = 0xc000;
    const std::int32_t model = PickMagicRoomCandidate(magicRoomUpperCornerCategories, 10, usedWidth, usedDepth, orientation);
    if (model != -1) {
        count += ConstructMagicRoomFurniture(placements, model, capacity, &usedWidth, &usedDepth, orientation);
        TransformMagicRoomPlacements(placements, count, orientation, roomGeneration.dimensions.x, roomGeneration.dimensions.y, -1.0f, -1.0f);
#else
    v5 = roomDimensions.x - roomCursor.x3;
    v6 = roomDimensions.y - roomCursor.d2;
    roomRngState = 1103515245U * roomRngState + 34567;
    usedWidth = roomDimensions.x - roomCursor.x3;
    usedDepth = roomDimensions.y - roomCursor.d2;
    v8 = roomDimensions.y - roomCursor.d2;
    if ((roomRngState & 0x10000) == 0)
        orientation = 49152;
    v9 = roomDimensions.x - roomCursor.x3;
    if ((roomRngState & 0x10000) == 0) {
        v9 = roomDimensions.y - roomCursor.d2;
        v8 = roomDimensions.x - roomCursor.x3;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (1.2f > usedDepth)
        usedDepth = 1.2f;
    roomGeneration.cursor.x3 = roomGeneration.dimensions.x - usedWidth;
    roomGeneration.cursor.d2 = roomGeneration.dimensions.y - usedDepth;
    return count;
#else
    v10 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 16), 10, v9, v8);
    if (v10 != 0) {
        roomRngState = 1103515245U * roomRngState + 34567;
        v11 = roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v10];
        category = g_magicRoomModels[v11].category;
        ++roomUsage[category];
        v2 = ConstructMagicRoomFurniture(a1, v11, a2, &usedWidth, &usedDepth, orientation);
        TransformMagicRoomPlacements(a1, v2, orientation, roomDimensions.x, roomDimensions.y, -1.0f, -1.0f);
        v5 = usedWidth;
        v6 = usedDepth;
#endif
}
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if (v6 < 1.2f)
        v6 = 1.2f;
    roomCursor.x3 = roomDimensions.x - v5;
    roomCursor.d2 = roomDimensions.y - v6;
    return v2;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MagicRoomInteger FillMagicRoomLowerLeftCorner(MagicRoomPlacement *placements, int capacity) {
#else
MagicRoomInteger FillMagicRoomLowerLeftCorner(MagicRoomPlacement *a1, int a2) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    float usedWidth = roomGeneration.dimensions.x + roomGeneration.cursor.firstThree.y;
    float usedDepth = roomGeneration.dimensions.y + roomGeneration.cursor.firstThree.z;
    std::int32_t count = 0;
#else
    unsigned int v2;
#endif
    int orientation;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (MagicRoomRandomRange(2) != 0)
#else
    float v4;
    float v5;
    std::int32_t v8;
    float v9;
    float v10;
    unsigned int v11;
    int v12;
    MagicRoomInteger category;
    float v14;
    float v15;
    unsigned int v16;
    int v17;
    MagicRoomInteger v18;
    float v19;
    MagicRoomInteger result;
    float usedWidth;
    float usedDepth;

    v2 = 0;
#endif
        orientation = 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    else
#else
    v4 = roomCursor.firstThree.y + roomDimensions.x;
    v5 = roomCursor.firstThree.z + roomDimensions.y;
    roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
    v8 = (roomRngState & 0x10000) != 0;
    usedWidth = roomCursor.firstThree.y + roomDimensions.x;
    usedDepth = roomCursor.firstThree.z + roomDimensions.y;
    v9 = roomCursor.firstThree.z + roomDimensions.y;
    if ((roomRngState & 0x10000) == 0)
#endif
        orientation = 0x4000;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t model = PickMagicRoomCandidate(magicRoomLowerCornerCategories, 6, usedWidth, usedDepth, orientation);
    if (model == -1)
        model = PickMagicRoomCandidate(magicRoomLowerLeftFallbackCategories, 4, usedWidth, usedDepth, orientation);
    if (model != -1) {
        count += ConstructMagicRoomFurniture(placements, model, capacity, &usedWidth, &usedDepth, orientation);
        TransformMagicRoomPlacements(placements, count, orientation, -roomGeneration.dimensions.x, -roomGeneration.dimensions.y, 1.0f, 1.0f);
#else
    v10 = roomCursor.firstThree.y + roomDimensions.x;
    if ((roomRngState & 0x10000) == 0) {
        v10 = roomCursor.firstThree.z + roomDimensions.y;
        v9 = roomCursor.firstThree.y + roomDimensions.x;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    roomGeneration.cursor.firstThree.y = -roomGeneration.dimensions.x + usedWidth;
    roomGeneration.cursor.firstThree.z = -roomGeneration.dimensions.y + usedDepth;
    if (roomGeneration.cursor.x3 < roomGeneration.cursor.firstThree.y)
        roomGeneration.cursor.d2 = roomGeneration.cursor.firstThree.z;
    return count;
#else
    v11 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 0), 6, v10, v9);
    if (v11 != 0) {
        roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
        v12 = (std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v11];
        category = g_magicRoomModels[(std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v11]].category;
        ++roomUsage[category];
    } else {
        v12 = -1;
#endif
}
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if (v12 != -1)
        goto LABEL_15;
    v14 = v5;
    v15 = v4;
    if (!v8) {
        v15 = v5;
        v14 = v4;
    }
    v16 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 8), 4, v15, v14);
    if (v16 != 0) {
        roomRngState = 1103515245U * roomRngState + 34567;
        v17 = (std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v16];
        v18 = g_magicRoomModels[(std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v16]].category;
        ++roomUsage[v18];
    } else {
        v17 = -1;
    }
    v12 = v17;
    if (v17 != -1) {
    LABEL_15:
        v2 = ConstructMagicRoomFurniture(a1, v12, a2, &usedWidth, &usedDepth, orientation);
        TransformMagicRoomPlacements(a1, v2, orientation, -roomDimensions.x, -roomDimensions.y, 1.0f, 1.0f);
        v4 = usedWidth;
        v5 = usedDepth;
    }
    v19 = v5 - roomDimensions.y;
    result = v2;
    roomCursor.firstThree.y = v4 - roomDimensions.x;
    roomCursor.firstThree.z = v19;
    if ((float)(v4 - roomDimensions.x) > roomCursor.x3)
        roomCursor.d2 = v19;
    return result;
}
#endif

MagicRoomInteger FillMagicRoomLeftWall(MagicRoomPlacement *a1, int a2) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float Z;
    int v6;
    float v7;
    std::int32_t v9;
    float v11;
    int orientation;
    float usedDepth;
    float usedWidth;

    if (roomGeneration.cursor.x3 < roomGeneration.boundaryX)
        v7 = roomGeneration.dimensions.x + roomGeneration.cursor.x3;
    else
        v7 = roomGeneration.dimensions.x + roomGeneration.boundaryX;
    v7 = v7 - 0.80000001f;
    orientation = 0x4000;
    v6 = 0;
    Z = roomGeneration.cursor.firstThree.z;
    do {
        usedWidth = v7;
        usedDepth = roomGeneration.cursor.d3Boundary - Z;
        v9 = PickMagicRoomCandidate(magicRoomSideWallCategories, 16, usedWidth, usedDepth, orientation);
        if (v9 != -1) {
            v11 = ConstructMagicRoomFurniture(&a1[v6], v9, a2, &usedWidth, &usedDepth, orientation);
            TransformMagicRoomPlacements(&a1[v6], v11, orientation, -roomGeneration.dimensions.x, Z, 1.0f, 1.0f);
            v6 += v11;
            Z = Z + usedDepth;
        }
    } while (v9 != -1 && v6 < a2);
    return v6;
#else
    float x3;
    float Z;
    int v6;
    float v7;
    unsigned int v8;
    std::uint16_t v9;
    MagicRoomInteger category;
    int v11;
    float usedDepth;
    float usedWidth;

    x3 = roomCursor.x3;
    if (roomBoundaryX <= roomCursor.x3)
        x3 = roomBoundaryX;
    Z = roomCursor.firstThree.z;
    v6 = 0;
    v7 = (float)(x3 + roomDimensions.x) + -0.80000001f;
    do {
        usedWidth = v7;
        usedDepth = roomCursor.d3Boundary - Z;
        v8 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 32), 16, roomCursor.d3Boundary - Z, v7);
        if (v8 == 0)
            break;
        roomRngState = 1103515245U * roomRngState + 34567;
        v9 = roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v8];
        category = g_magicRoomModels[v9].category;
        ++roomUsage[category];
        v11 = ConstructMagicRoomFurniture(&a1[v6], v9, a2, &usedWidth, &usedDepth, 0x4000u);
        TransformMagicRoomPlacements(&a1[v6], v11, 0x4000, -roomDimensions.x, Z, 1.0f, 1.0f);
        Z = Z + usedDepth;
        v6 += v11;
    } while (v6 < a2);
    return (unsigned int)v6;
#endif
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
MagicRoomInteger FillMagicRoomUpperLeftCorner(MagicRoomPlacement *placements, int capacity) {
#else
MagicRoomInteger FillMagicRoomUpperLeftCorner(MagicRoomPlacement *a1, int a2) {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    float usedWidth = roomGeneration.dimensions.x + roomGeneration.cursor.x3Boundary;
    float usedDepth = roomGeneration.dimensions.y - roomGeneration.cursor.d3Boundary;
    std::int32_t count = 0;
#else
    unsigned int v2;
    float v5;
    float v6;
#endif
    int orientation;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (MagicRoomRandomRange(2) != 0)
        orientation = 0x8000;
    else
        orientation = 0x4000;
    const std::int32_t model = PickMagicRoomCandidate(magicRoomUpperCornerCategories, 10, usedWidth, usedDepth, orientation);
    if (model != -1) {
        count += ConstructMagicRoomFurniture(placements, model, capacity, &usedWidth, &usedDepth, orientation);
        TransformMagicRoomPlacements(placements, count, orientation, -roomGeneration.dimensions.x, roomGeneration.dimensions.y, 1.0f, -1.0f);
#else
    float v8;
    float v9;
    unsigned int v10;
    std::uint16_t v11;
    MagicRoomInteger category;
    float usedWidth;
    float usedDepth;

    v2 = 0;
    v5 = roomCursor.x3Boundary + roomDimensions.x;
    v6 = roomDimensions.y - roomCursor.d3Boundary;
    roomRngState = 1103515245U * roomRngState + 34567;
    usedWidth = roomCursor.x3Boundary + roomDimensions.x;
    usedDepth = roomDimensions.y - roomCursor.d3Boundary;
    orientation = (((roomRngState & 0x10000) != 0) + 1) << 14;
    v8 = roomDimensions.y - roomCursor.d3Boundary;
    v9 = roomCursor.x3Boundary + roomDimensions.x;
    if (((orientation - 0x4000) & 0xFFFF7FFF) == 0) {
        v9 = roomDimensions.y - roomCursor.d3Boundary;
        v8 = roomCursor.x3Boundary + roomDimensions.x;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (1.2f > usedDepth)
        usedDepth = 1.2f;
    roomGeneration.cursor.x3Boundary = -roomGeneration.dimensions.x + usedWidth;
    roomGeneration.cursor.d3Boundary = roomGeneration.dimensions.y - usedDepth;
    return count;
#else
    v10 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 16), 10, v9, v8);
    if (v10 != 0) {
        roomRngState = 0x41C64E6DU * roomRngState + 34567;
        v11 = roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v10];
        category = g_magicRoomModels[v11].category;
        ++roomUsage[category];
        v2 = ConstructMagicRoomFurniture(a1, v11, a2, &usedWidth, &usedDepth, orientation);
        TransformMagicRoomPlacements(a1, v2, orientation, -roomDimensions.x, roomDimensions.y, 1.0f, -1.0f);
        v5 = usedWidth;
        v6 = usedDepth;
#endif
}
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if (v6 < 1.2f)
        v6 = 1.2f;
    roomCursor.x3Boundary = v5 - roomDimensions.x;
    roomCursor.d3Boundary = roomDimensions.y - v6;
    return v2;
}
#endif

MagicRoomInteger ConstructMagicRoomSpecialPlacement(MagicRoomPlacement *a1, int a2) {
    unsigned int v3;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    MagicRoomInteger result = 0;

    v3 = PickMagicRoomCategoryModel(14);
    if (v3 != -1 && a2 != 0) {
#else
    float v5;

    if (static_cast<std::int8_t>(roomUsage[14]) >= static_cast<std::int8_t>(roomCapacity[14]))
        return 0;
    ++roomUsage[14];
    roomRngState = 0x41C64E6DU * roomRngState + 34567;
    v3 = g_magicRoomGroups[14].start + ((roomRngState >> 16U) & 0x7FFFu) % g_magicRoomGroups[14].count;
    if (v3 == -1 || a2 == 0)
        return 0;
#endif
        a1->kind = 1;
        a1->position.x = 0.0f;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        a1->position.y = roomGeneration.dimensions.z - 0.0099999998f;
#else
    v5 = roomDimensions.z - 0.0099999998f;
#endif
        a1->position.z = 0.0f;
        a1->halfWidth = 0.0f;
        a1->height = 0.0f;
        a1->halfDepth = 0.0f;
        a1->yaw = 0;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    a1->position.y = v5;
#endif
        a1->modelIndex = v3;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        result = 1;
#else
    return 1;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    return result;
}
#endif

MagicRoomInteger ConstructMagicRoomGroupedPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    MagicRoomModel *model;
    MagicRoomModel *partner;
    std::int32_t partnerIndex;
    int partnerYaw;
    int partnerAxis;
    int count;
    int index;
    float width;
    float depth;
    float scale;

    model = &g_magicRoomModels[a2];
    width = *a4;
    depth = *a5;
    *a4 = model->dimensions.x;
    *a5 = model->dimensions.z + model->dimensions.z;
    partnerIndex = PickMagicRoomCandidate(magicRoomGroupedCategories, 1, width / 3.0f, depth / 3.0f, 0);
    if (partnerIndex == -1)
        partner = 0;
    else
        partner = &g_magicRoomModels[partnerIndex];
    if (partner != 0 && partner->dimensions.x < partner->dimensions.z) {
        partnerYaw = 2;
        partnerAxis = 0;
    } else {
        partnerYaw = 0;
        partnerAxis = 2;
    }
    count = 0;
    if (1.2f > model->dimensions.x && 1.2f > model->dimensions.z) {
        if (width > model->dimensions.x * 3.0f && a3 > 2) {
            *a4 = model->dimensions.x * 3.0f;
            scale = 0.5f;
            for (index = 0; index < 3; index++) {
                a1[index].position.x = model->dimensions.x * scale;
                a1[index].position.y = 0.0f;
                a1[index].position.z = model->dimensions.z * 0.5f;
                a1[index].halfWidth = model->dimensions.x * 0.5f;
                a1[index].height = model->dimensions.y;
                a1[index].halfDepth = model->dimensions.z * 0.5f;
                a1[index].yaw = 0;
                a1[index].modelIndex = a2;
                scale += 1.0f;
            }
            count += 3;
            if (partner != 0) {
                if (depth > model->dimensions.z + 0.8f + (&partner->dimensions.x)[partnerAxis] && a3 > 3) {
                    *a5 = model->dimensions.z + 0.8f + (&partner->dimensions.x)[partnerAxis];
                    index = count;
                    a1[index].position.x = a1[1].position.x;
                    a1[index].position.y = 0.0f;
                    a1[index].position.z = model->dimensions.z + 0.8f + (&partner->dimensions.x)[partnerAxis] * 0.5f;
                    a1[index].halfWidth = partner->dimensions.x * 0.5f;
                    a1[index].height = partner->dimensions.y;
                    a1[index].halfDepth = partner->dimensions.z * 0.5f;
                    if (partnerYaw == 2)
                        a1[index].yaw = 0x4000;
                    else
                        a1[index].yaw = 0;
                    a1[index].modelIndex = partnerIndex;
                    count++;
                    if (depth > model->dimensions.z + model->dimensions.z + 1.6f + (&partner->dimensions.x)[partnerAxis] && a3 > 5) {
                        *a5 = model->dimensions.z + model->dimensions.z + 1.6f + (&partner->dimensions.x)[partnerAxis];
                        a1[4].position.x = a1[0].position.x;
                        a1[4].position.y = a1[0].position.y;
                        a1[4].position.z = a1[0].position.z + model->dimensions.z + 1.6f + (&partner->dimensions.x)[partnerAxis];
                        a1[4].halfWidth = a1[0].halfWidth;
                        a1[4].height = a1[0].height;
                        a1[4].halfDepth = a1[0].halfDepth;
                        a1[4].yaw = 0x8000;
                        a1[4].modelIndex = a1[0].modelIndex;
                        index = 5;
                        a1[index].position.x = a1[2].position.x;
                        a1[index].position.y = a1[2].position.y;
                        a1[index].position.z = a1[2].position.z + model->dimensions.z + 1.6f + (&partner->dimensions.x)[partnerAxis];
                        a1[index].halfWidth = a1[2].halfWidth;
                        a1[index].height = a1[2].height;
                        a1[index].halfDepth = a1[2].halfDepth;
                        a1[index].yaw = 0x8000;
                        a1[index].modelIndex = a1[2].modelIndex;
                        count += 2;
                    }
                }
            }
        } else {
            a1->position.x = model->dimensions.x * 0.5f;
            a1->position.y = 0.0f;
            a1->position.z = model->dimensions.z * 0.5f;
            a1->halfWidth = model->dimensions.x * 0.5f;
            a1->height = model->dimensions.y;
            a1->halfDepth = model->dimensions.z * 0.5f;
            a1->yaw = 0;
            a1->modelIndex = a2;
            count++;
        }
    } else {
        a1->position.x = model->dimensions.x * 0.5f;
        a1->position.y = 0.0f;
        a1->position.z = model->dimensions.z * 0.5f;
        a1->halfWidth = model->dimensions.x * 0.5f;
        a1->height = model->dimensions.y;
        a1->halfDepth = model->dimensions.z * 0.5f;
        a1->yaw = 0;
        a1->modelIndex = a2;
        count++;
        if (partner != 0 && 1.2f > model->dimensions.z) {
            if (depth > model->dimensions.z + 0.8f + (&partner->dimensions.x)[partnerAxis] && a3 > 2) {
                *a5 = model->dimensions.z + 0.8f + (&partner->dimensions.x)[partnerAxis];
                index = count;
                a1[index].position.x = a1->position.x;
                a1[index].position.y = 0.0f;
                a1[index].position.z = model->dimensions.z + 0.8f + (&partner->dimensions.x)[partnerAxis] * 0.5f;
                a1[index].halfWidth = partner->dimensions.x * 0.5f;
                a1[index].height = partner->dimensions.y;
                a1[index].halfDepth = partner->dimensions.z * 0.5f;
                if (partnerYaw == 2)
                    a1[index].yaw = 0x4000;
                else
                    a1[index].yaw = 0;
                a1[index].modelIndex = partnerIndex;
                count++;
                if (depth > model->dimensions.z + model->dimensions.z + 1.6f + (&partner->dimensions.x)[partnerAxis] && a3 > 5) {
                    *a5 = model->dimensions.z + model->dimensions.z + 1.6f + (&partner->dimensions.x)[partnerAxis];
                    a1[2].position.x = a1[0].position.x;
                    a1[2].position.y = a1[0].position.y;
                    a1[2].position.z = a1[0].position.z + model->dimensions.z + 1.6f + (&partner->dimensions.x)[partnerAxis];
                    a1[2].halfWidth = a1[0].halfWidth;
                    a1[2].height = a1[0].height;
                    a1[2].halfDepth = a1[0].halfDepth;
                    a1[2].yaw = 0x8000;
                    a1[2].modelIndex = a1[0].modelIndex;
                    count++;
                }
            }
        }
    }
    return count;
#else
    std::uint16_t v6;
    unsigned int v7;
    float v8;
    float v9;
    float v10;
    MagicRoomModel *v12;
    unsigned int v14;
    MagicRoomModel *v15;
    int v16;
    MagicRoomInteger category;
    float *p_X;
    int v19;
    MagicRoomInteger v20;
    float X;
    float v22;
    float v23;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float Z;
    float v39;
    float v40;
    float v41;
    float v42;
    float halfDepth;

    v6 = a2;
    v7 = 1;
    v8 = *a5;
    v9 = *a4;
    v10 = *a5 / 3.0f;
    v12 = &g_magicRoomModels[a2];
    *a4 = v12->dimensions.x;
    *a5 = v12->dimensions.z + v12->dimensions.z;
    v14 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 6), 1, v9 / 3.0f, v10);
    if (v14 == 0) {
        v16 = -1;
        p_X = 0;
        goto LABEL_8;
    }
    v15 = g_magicRoomModels;
    roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
    v16 = (std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v14];
    category = g_magicRoomModels[(std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v14]].category;
    ++roomUsage[category];
    p_X = &v15[v16].dimensions.x;
    if (p_X == 0 || p_X[2] <= *p_X) {
    LABEL_8:
        v19 = 0;
        v20 = 2;
        goto LABEL_9;
    }
    v19 = 2;
    v20 = 0;
LABEL_9:
    X = v12->dimensions.x;
    if (v12->dimensions.x >= 1.2f || v12->dimensions.z >= 1.2f) {
        a1->position.y = 0.0f;
        a1->position.x = X * 0.5f;
        a1->position.z = v12->dimensions.z * 0.5f;
        a1->halfWidth = v12->dimensions.x * 0.5f;
        a1->height = v12->dimensions.y;
        v37 = v12->dimensions.z * 0.5f;
        a1->yaw = 0;
        a1->halfDepth = v37;
        a1->modelIndex = v6;
        if (p_X != 0) {
            Z = v12->dimensions.z;
            if (Z < 1.2f) {
                v39 = (float)(Z + 0.80000001f) + p_X[v20];
                if (v8 > v39 && a3 > 2) {
                    *a5 = v39;
                    a1[1].position.x = a1->position.x;
                    a1[1].position.y = 0.0f;
                    a1[1].position.z = (float)(v12->dimensions.z + 0.80000001f) + (float)(p_X[v20] * 0.5f);
                    a1[1].halfWidth = *p_X * 0.5f;
                    a1[1].height = p_X[1];
                    a1[1].halfDepth = p_X[2] * 0.5f;
                    a1[1].yaw = v19 == 2 ? 0x4000 : 0;
                    v7 = 2;
                    a1[1].modelIndex = v16;
                    v40 = (float)((float)(v12->dimensions.z + v12->dimensions.z) + 1.6f) + p_X[v20];
                    if (v8 > v40 && a3 > 5) {
                        *a5 = v40;
                        v7 = 3;
                        v41 = a1->position.z;
                        a1[2].position.x = a1->position.x;
                        a1[2].position.y = a1->position.y;
                        v42 = (float)((float)(v41 + v12->dimensions.z) + 1.6f) + p_X[v20];
                        a1[2].halfWidth = a1->halfWidth;
                        a1[2].height = a1->height;
                        halfDepth = a1->halfDepth;
                        a1[2].position.z = v42;
                        a1[2].halfDepth = halfDepth;
                        a1[2].yaw = 0x8000;
                        a1[2].modelIndex = a1->modelIndex;
                    }
                }
            }
        }
    } else if (v9 <= (float)(X * 3.0f) || a3 <= 2) {
        a1->position.y = 0.0f;
        a1->position.x = X * 0.5f;
        a1->position.z = v12->dimensions.z * 0.5f;
        a1->halfWidth = v12->dimensions.x * 0.5f;
        a1->height = v12->dimensions.y;
        v36 = v12->dimensions.z * 0.5f;
        a1->yaw = 0;
        a1->halfDepth = v36;
        a1->modelIndex = v6;
    } else {
        *a4 = X * 3.0f;
        v7 = 3;
        v22 = v12->dimensions.x;
        a1->position.y = 0.0f;
        a1->position.x = (float)(v22 + 0.029999999f) * 0.5f;
        a1->position.z = v12->dimensions.z * 0.5f;
        a1->halfWidth = v12->dimensions.x * 0.5f;
        a1->height = v12->dimensions.y;
        v23 = v12->dimensions.z * 0.5f;
        a1->yaw = 0;
        a1->halfDepth = v23;
        a1->modelIndex = v6;
        v24 = v12->dimensions.x;
        a1[1].position.y = 0.0f;
        a1[1].position.x = (float)(v24 + 0.029999999f) * 1.5f;
        a1[1].position.z = v12->dimensions.z * 0.5f;
        a1[1].halfWidth = v12->dimensions.x * 0.5f;
        a1[1].height = v12->dimensions.y;
        v25 = v12->dimensions.z * 0.5f;
        a1[1].yaw = 0;
        a1[1].halfDepth = v25;
        a1[1].modelIndex = v6;
        v26 = v12->dimensions.x;
        a1[2].position.y = 0.0f;
        a1[2].position.x = (float)(v26 + 0.029999999f) * 2.5f;
        a1[2].position.z = v12->dimensions.z * 0.5f;
        a1[2].halfWidth = v12->dimensions.x * 0.5f;
        a1[2].height = v12->dimensions.y;
        v27 = v12->dimensions.z * 0.5f;
        a1[2].yaw = 0;
        a1[2].halfDepth = v27;
        a1[2].modelIndex = v6;
        if (p_X != 0) {
            v28 = (float)(v12->dimensions.z + 0.80000001f) + p_X[v20];
            if (v8 > v28 && a3 > 3) {
                *a5 = v28;
                a1[3].position.x = a1[1].position.x;
                a1[3].position.y = 0.0f;
                a1[3].position.z = (float)(v12->dimensions.z + 0.80000001f) + (float)(p_X[v20] * 0.5f);
                a1[3].halfWidth = *p_X * 0.5f;
                a1[3].height = p_X[1];
                a1[3].halfDepth = p_X[2] * 0.5f;
                if (v19 == 2)
                    a1[3].yaw = 0x4000;
                else
                    a1[3].yaw = 0;
                v7 = 4;
                a1[3].modelIndex = v16;
                v29 = (float)((float)(v12->dimensions.z + v12->dimensions.z) + 1.6f) + p_X[v20];
                if (v8 > v29 && a3 > 5) {
                    *a5 = v29;
                    v7 = 6;
                    v30 = a1->position.z;
                    a1[4].position.x = a1->position.x;
                    a1[4].position.y = a1->position.y;
                    v31 = (float)((float)(v30 + v12->dimensions.z) + 1.6f) + p_X[v20];
                    a1[4].halfWidth = a1->halfWidth;
                    a1[4].height = a1->height;
                    v32 = a1->halfDepth;
                    a1[4].position.z = v31;
                    a1[4].halfDepth = v32;
                    a1[4].yaw = 0x8000;
                    a1[4].modelIndex = a1->modelIndex;
                    v33 = a1[2].position.z;
                    a1[5].position.x = a1[2].position.x;
                    a1[5].position.y = a1[2].position.y;
                    v34 = (float)((float)(v33 + v12->dimensions.z) + 1.6f) + p_X[v20];
                    a1[5].halfWidth = a1[2].halfWidth;
                    a1[5].height = a1[2].height;
                    v35 = a1[2].halfDepth;
                    a1[5].position.z = v34;
                    a1[5].halfDepth = v35;
                    a1[5].yaw = 0x8000;
                    a1[5].modelIndex = a1[2].modelIndex;
                }
            }
        }
    }
    return v7;
#endif
}

int AddMagicRoomSurfaceDecoration(MagicRoomPlacement *outChild, const MagicRoomPlacement *parent, const MagicRoomModel *parentModel, int remaining) {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    unsigned int v7;
    int v8;
    MagicRoomModel *v9;
#endif
    int v10;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    MagicRoomInteger category;
#endif
    Vector3 *p_x;
    float v13;
    float v14;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    int result = 0;

    if (remaining != 0) {
        v10 = PickMagicRoomCandidate(magicRoomSurfaceDecorationCategories, 4, parentModel->dimensions.x, parentModel->dimensions.z, 0);
        if (v10 != -1) {
            p_x = &g_magicRoomModels[v10].dimensions;
#else
    signed int v15;
    float v16;

    if (remaining == 0)
        return 0;
    v7 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 12), 4, parentModel->dimensions.x, parentModel->dimensions.z);
    if (v7 == 0)
        return 0;
    roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
    v8 = roomRngState;
    v9 = g_magicRoomModels;
    v10 = (std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v7];
    category = g_magicRoomModels[(std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v7]].category;
    ++roomUsage[category];
    p_x = &v9[v10].dimensions;
#endif
            v13 = (float)(parentModel->dimensions.x - p_x->x) * 0.69999999f;
            v14 = (float)(parentModel->dimensions.z - p_x->z) * 0.69999999f;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            outChild->position.x = parent->position.x + (float)((float)(MagicRoomRandomFloat() - 0.5f) * v13);
#else
    roomRngState = 0x41C64E6DU * (0x41C64E6DU * v8 + 0x8707) + 0x8707;
    v15 = (roomRngState >> 16U) & 0x7FFF;
    outChild->position.x =
        (float)((float)((float)((float)(((unsigned int)(0x41C64E6DU * v8 + 0x8707) >> 16) & 0x7FFF) / 32767.0f) - 0.5f) * v13) + parent->position.x;
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            outChild->position.y = parent->height;
#else
            outChild->position.y = (float)(parent->height + parent->position.y) + 0.0099999998f;
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            outChild->position.z = parent->position.z + (float)((float)(MagicRoomRandomFloat() - 0.5f) * v14);
#else
    v16 = (float)((float)((float)((float)v15 / 32767.0f) - 0.5f) * v14) + parent->position.z;
#endif
            outChild->halfWidth = 0.0f;
            outChild->height = 0.0f;
            outChild->halfDepth = 0.0f;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    outChild->position.z = v16;
#endif
            outChild->yaw = parent->yaw;
            outChild->modelIndex = v10;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            result = 1;
#else
    return 1;
#endif
        }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    }
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
int TryAddMagicRoomSurfaceDecoration(MagicRoomPlacement *outChild, const MagicRoomPlacement *parent, const MagicRoomModel *parentModel, int remaining) {
    int result = 0;
    if (parentModel->category == 7)
        result = AddMagicRoomSurfaceDecoration(outChild, parent, parentModel, remaining);
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MagicRoomInteger ConstructMagicRoomSeatingPlacement(MagicRoomPlacement *placements, int modelIndex, int remaining, float *usedWidth, float *usedDepth) {
    MagicRoomModel *model;
    MagicRoomModel *secondary;
    MagicRoomPlacement *child;
    float width;
    float depth;
    float ratio;
    float offset;
    float firstX;
    float span;
    int count;
    int secondaryIndex;
    int fitting;
    int index;
    int step;
    int angle;
    Vector3 extent;

    if (remaining < 2)
        return 0;
    model = &g_magicRoomModels[modelIndex];
    width = *usedWidth;
    depth = *usedDepth;
    count = 0;
    secondary = 0;
    placements->position.y = 0.0f;
    placements->halfWidth = model->dimensions.x * 0.5f;
    placements->height = model->dimensions.y;
    placements->halfDepth = model->dimensions.z * 0.5f;
    placements->yaw = 0;
    placements->modelIndex = modelIndex;
    ratio = model->dimensions.x / model->dimensions.z;
    count++;
    remaining--;
    if (ratio > 0.9f && ratio < 1.1f) {
        secondaryIndex = PickMagicRoomCandidate(magicRoomSecondaryCategories, 1, (width - model->dimensions.x) * 0.5f, (depth - model->dimensions.z) * 0.5f, 0);
        if (secondaryIndex == -1)
            goto placed;
        secondary = &g_magicRoomModels[secondaryIndex];
        placements->position.x = model->dimensions.x * 0.5f + secondary->dimensions.z;
        placements->position.z = model->dimensions.z * 0.5f + (secondary->dimensions.z + secondary->dimensions.z);
        span = model->dimensions.x * 3.14f;
        fitting = (int)(span / (secondary->dimensions.x * 1.5f));
        if (fitting > remaining)
            fitting = remaining;
        step = 0x10000 / fitting;
        offset = placements->halfWidth + secondary->dimensions.z * 0.5f;
        for (index = 0; index < fitting; index++) {
            child = &placements[count + index];
            angle = step * index;
            child->position.x = placements->position.x + dc::sdk::njSin(angle) * offset;
            child->position.y = 0.0f;
            child->position.z = dc::sdk::njCos(angle) * offset + placements->position.z;
            child->halfWidth = secondary->dimensions.x * 0.5f;
            child->height = secondary->dimensions.y;
            child->halfDepth = secondary->dimensions.z * 0.5f;
            child->yaw = angle + 0x8000;
            child->modelIndex = secondaryIndex;
        }
        count += fitting;
    } else {
        secondaryIndex = PickMagicRoomCandidate(magicRoomSecondaryCategories, 1, model->dimensions.x, (depth - model->dimensions.z) * 0.5f, 0);
        if (secondaryIndex == -1)
            goto placed;
        secondary = &g_magicRoomModels[secondaryIndex];
        placements->position.x = model->dimensions.x * 0.75f;
        placements->position.z = model->dimensions.z * 0.5f + (secondary->dimensions.z + secondary->dimensions.z);
        fitting = (int)(model->dimensions.x / (secondary->dimensions.x * 1.5f));
        if (fitting == 0)
            fitting = 1;
        firstX = model->dimensions.x * 0.25f;
        span = model->dimensions.x / (float)(fitting + fitting);
        offset = model->dimensions.z * 0.5f + secondary->dimensions.z * 0.2f;
        for (index = 0; index < fitting; index++) {
            child = &placements[count + index];
            child->position.x = span * (float)(index + index + 1) + firstX;
            child->position.y = 0.0f;
            child->position.z = placements->position.z - offset;
            child->halfWidth = secondary->dimensions.x * 0.5f;
            child->height = secondary->dimensions.y;
            child->halfDepth = secondary->dimensions.z * 0.5f;
            child->yaw = 0;
            child->modelIndex = secondaryIndex;
        }
        count += fitting;
        for (index = 0; index < fitting; index++) {
            child = &placements[count + index];
            child->position.x = span * (float)(index + index + 1) + firstX;
            child->position.y = 0.0f;
            child->position.z = placements->position.z + offset;
            child->halfWidth = secondary->dimensions.x * 0.5f;
            child->height = secondary->dimensions.y;
            child->halfDepth = secondary->dimensions.z * 0.5f;
            child->yaw = 0x8000;
            child->modelIndex = secondaryIndex;
        }
        count += fitting;
    }
placed:
    if (secondary == 0) {
        placements->position.x = model->dimensions.x * 0.75f;
        placements->position.z = model->dimensions.z * 0.5f + 0.8f;
    }
    AdjustMagicRoomSeatingExtent(&model->dimensions, &secondary->dimensions, &extent);
    *usedWidth = extent.x;
    *usedDepth = extent.z;
    return count;
}
#else
MagicRoomInteger ConstructMagicRoomSeatingPlacement(MagicRoomPlacement *placements, int modelIndex, int remaining, float *usedWidth, float *usedDepth) {
    if (remaining < 2)
        return 0;
    const float availableWidth = *usedWidth;
    const float availableDepth = *usedDepth;
    MagicRoomModel *const model = &g_magicRoomModels[modelIndex];
    MagicRoomModel *secondary = 0;
    std::uint32_t written = 1;
    MagicRoomPlacement &primary = placements[0];
    primary.position.y = 0.0f;
    primary.halfWidth = model->dimensions.x * 0.5f;
    primary.height = model->dimensions.y;
    primary.halfDepth = model->dimensions.z * 0.5f;
    primary.yaw = 0;
    primary.modelIndex = static_cast<std::uint16_t>(modelIndex);
    const float ratio = model->dimensions.x / model->dimensions.z;
    const bool rectangular = ratio <= 0.89999998f || ratio >= 1.1f;
    const float filterWidth = rectangular ? model->dimensions.x : (availableWidth - model->dimensions.x) * 0.5f;
    const std::uint32_t candidates = static_cast<std::uint32_t>(FindMagicRoomCandidates(
        reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 7), 1, filterWidth, (availableDepth - model->dimensions.z) * 0.5f));
    if (candidates != 0) {
        roomRngState = 1103515245U * roomRngState + 34567U;
        const std::uint16_t secondaryIndex = static_cast<std::uint16_t>(roomCandidates[((roomRngState >> 16U) & 0x7fffU) % candidates]);
        secondary = &g_magicRoomModels[secondaryIndex];
        ++roomUsage[secondary->category];
        if (rectangular) {
            primary.position.x = model->dimensions.x * 0.75f;
            primary.position.z = (secondary->dimensions.z + secondary->dimensions.z) + model->dimensions.z * 0.5f;
            const float firstX = model->dimensions.x * 0.25f;
            std::int32_t count = TruncateToInt32(model->dimensions.x / (secondary->dimensions.x * 1.5f));
            if (count == 0)
                count = 1;
            const float stepX = model->dimensions.x / static_cast<float>(2 * count);
            const float offsetZ = secondary->dimensions.z * 0.2f + model->dimensions.z * 0.5f;
            for (std::int32_t side = 0; side < 2; ++side) {
                for (std::int32_t index = 0; index < count; ++index) {
                    MagicRoomPlacement &child = placements[1 + side * count + index];
                    child.position.x = static_cast<float>(2 * index + 1) * stepX + firstX;
                    child.position.y = 0.0f;
                    child.position.z = side == 0 ? primary.position.z - offsetZ : offsetZ + primary.position.z;
                    child.halfWidth = secondary->dimensions.x * 0.5f;
                    child.height = secondary->dimensions.y;
                    child.halfDepth = secondary->dimensions.z * 0.5f;
                    child.yaw = side == 0 ? 0 : 0x8000;
                    child.modelIndex = secondaryIndex;
                }
            }
            written = static_cast<std::uint32_t>(2 * count + 1);
        } else {
            primary.position.x = model->dimensions.x * 0.5f + secondary->dimensions.z;
            primary.position.z = (secondary->dimensions.z + secondary->dimensions.z) + model->dimensions.z * 0.5f;
            const float radius = secondary->dimensions.z * 0.5f + primary.halfWidth;
            std::int32_t count = remaining - 1;
            const std::int32_t fitting = TruncateToInt32((model->dimensions.x * 3.1400001f) / (secondary->dimensions.x * 1.5f));
            if (fitting <= count)
                count = fitting;
            if (count > 0) {
                const std::int32_t step = 0x10000 / count;
                std::int32_t angle = 0;
                for (std::int32_t index = 0; index < count; ++index) {
                    MagicRoomPlacement &child = placements[index + 1];
                    child.position.x = sin_fixed16(static_cast<std::int16_t>(angle)) * radius + primary.position.x;
                    child.position.y = 0.0f;
                    child.position.z = cos_fixed16(static_cast<std::uint16_t>(angle)) * radius + primary.position.z;
                    child.halfWidth = secondary->dimensions.x * 0.5f;
                    child.height = secondary->dimensions.y;
                    child.yaw = static_cast<std::uint16_t>(angle + 0x8000);
                    child.modelIndex = secondaryIndex;
                    child.halfDepth = secondary->dimensions.z * 0.5f;
                    angle += step;
                }
            }
            written = static_cast<std::uint32_t>(count + 1);
        }
    } else {
        primary.position.x = model->dimensions.x * 0.75f;
        primary.position.z = model->dimensions.z * 0.5f + 0.80000001f;
    }
    float width = model->dimensions.x;
    float depth = model->dimensions.z;
    if (secondary) {
        width = rectangular ? width * 1.5f : width + (secondary->dimensions.x + secondary->dimensions.x);
        depth += secondary->dimensions.z * 4.0f;
    }
    *usedWidth = width;
    *usedDepth = depth;
    primary.halfDepth = depth * 0.5f;
    primary.halfWidth = width * 0.5f;
    return written;
}
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
MagicRoomInteger FillMagicRoomInterior(MagicRoomPlacement *placements, std::int32_t capacity) {
    float x;
    float minimumX;
    float z;
    float minimumZ;
    std::int32_t count;
    std::int32_t model;
    std::int32_t written;
    float usedWidth;
    float usedDepth;
    int orientation;

    x = roomGeneration.cursor.x3;
    minimumX = roomGeneration.cursor.x3Boundary;
    count = 0;
    orientation = 0xc000;
    if (roomGeneration.cursor.d2 > roomGeneration.cursor.d3Boundary)
        z = roomGeneration.cursor.d3Boundary;
    else
        z = roomGeneration.cursor.d2;
    if (roomGeneration.cursor.firstThree.x > roomGeneration.cursor.firstThree.z)
        minimumZ = roomGeneration.cursor.firstThree.x;
    else
        minimumZ = roomGeneration.cursor.firstThree.z;
    do {
        usedWidth = x - minimumX;
        usedDepth = z - minimumZ;
        model = PickMagicRoomCandidate(magicRoomInteriorCategories, 5, usedWidth, usedDepth, orientation);
        if (model != -1) {
            written = ConstructMagicRoomFurniture(&placements[count], model, capacity, &usedWidth, &usedDepth, orientation);
            TransformMagicRoomPlacements(&placements[count], written, orientation, x, z, -1.0f, -1.0f);
            count += written;
            z -= usedDepth;
            if (roomGeneration.cursor.x3 > x - usedWidth)
                roomGeneration.cursor.x3 = x - usedWidth;
        }
    } while (model != -1 && count < capacity);
    return count;
}
#endif
} // namespace

MagicRoomUnsigned ReadMagicRoomDatabase(MagicRoomHeader *database) {
    g_magicRoomDatabase = database;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const std::uint32_t tag = database->tag;
    if (tag != static_cast<std::uint32_t>(TASK_MAGIC_ROOM) || database->version != 1) {
#else
    if (database->tag == TASK_MAGIC_ROOM.name && database->version == 1) {
        g_magicRoomGroups = reinterpret_cast<MagicRoomGroup *>(reinterpret_cast<std::byte *>(database) + database->groupOffset);
        g_magicRoomModels = reinterpret_cast<MagicRoomModel *>(reinterpret_cast<std::byte *>(database) + database->modelOffset);
        return 0;
    }
#endif
        g_magicRoomDatabase = 0;
        g_magicRoomGroups = 0;
        g_magicRoomModels = 0;
        return 0xffffffffU;
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    g_magicRoomGroups = reinterpret_cast<MagicRoomGroup *>(reinterpret_cast<std::byte *>(g_magicRoomDatabase) + g_magicRoomDatabase->groupOffset);
    g_magicRoomModels = reinterpret_cast<MagicRoomModel *>(reinterpret_cast<std::byte *>(g_magicRoomDatabase) + g_magicRoomDatabase->modelOffset);
    return 0;
}
#endif
void InitializeMagicRoomGeneration() {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    roomGeneration.dimensions.x = 2.5f;
    roomGeneration.dimensions.y = 3.4000001f;
    roomGeneration.dimensions.z = 3.6f;
    roomGeneration.modeSeed = 1;
#endif
    for (std::int32_t index = 0; index < 45; ++index)
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        roomGeneration.capacity[index] = 50;
    roomGeneration.capacity[6] = 1;
    roomGeneration.capacity[7] = 1;
    roomGeneration.capacity[9] = 1;
    roomGeneration.capacity[10] = 1;
    roomGeneration.capacity[12] = 1;
    roomGeneration.capacity[15] = 2;
    roomGeneration.capacity[16] = 1;
    roomGeneration.capacity[18] = 2;
    roomGeneration.capacity[32] = 1;
    roomGeneration.capacity[30] = 1;
    roomGeneration.capacity[35] = 1;
    roomGeneration.capacity[41] = 1;
    roomGeneration.capacity[25] = 6;
#else
        roomCapacity[index] = 50;
    roomDimensions.x = 2.5f;
    roomDimensions.y = 3.4000001f;
    roomDimensions.z = 3.6f;
    roomModeSeed = 1;
    roomCapacity[6] = 1;
    roomCapacity[7] = 1;
    roomCapacity[9] = 1;
    roomCapacity[10] = 1;
    roomCapacity[12] = 1;
    roomCapacity[15] = 2;
    roomCapacity[16] = 1;
    roomCapacity[18] = 2;
    roomCapacity[25] = 6;
    roomCapacity[30] = 1;
    roomCapacity[32] = 1;
    roomCapacity[35] = 1;
    roomCapacity[41] = 1;
#endif
}
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void FillMagicRoomCategorySeeds() {
    std::int32_t index;
    for (index = 0; index < 9; ++index) {
        if (roomGeneration.modeSeed != index)
            roomGeneration.modes[index] = 0;
        else
            roomGeneration.modes[index] = 1;
    }
    for (index = 0; index < 45; ++index)
        roomGeneration.usage[index] = 0;
}
#else
MagicRoomUnsigned FillMagicRoomCategorySeeds() {
    for (std::int32_t index = 0; index < 9; ++index)
        roomModes[index] = roomModeSeed == index;
    std::memset(roomUsage, 0, sizeof(roomUsage));
    return 9;
}
#endif
void SetMagicRoomDimensions(float width, float height, float depth) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    roomGeneration.dimensions.x = width * 0.5f - 0.2f;
#else
    roomGeneration.dimensions.x = width * 0.5f - 0.15000001f;
#endif
    roomGeneration.dimensions.y = depth * 0.5f - 0.2f;
    roomGeneration.dimensions.z = height;
#else
    roomDimensions.z = height;
    roomDimensions.x = width * 0.5f - 0.15000001f;
    roomDimensions.y = depth * 0.5f - 0.2f;
#endif
}
void GetMagicRoomDimensions(Vector3 *dimensions) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    dimensions->x = roomGeneration.dimensions.x;
    dimensions->y = roomGeneration.dimensions.z;
    dimensions->z = roomGeneration.dimensions.y;
#else
    dimensions->x = roomDimensions.x;
    dimensions->y = roomDimensions.z;
    dimensions->z = roomDimensions.y;
#endif
}
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMagicRoomModeSeed(std::int32_t seed) {
    roomGeneration.modeSeed = seed;
}
#endif
namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetMagicRoomRandomSeed(std::uint32_t seed) {
    roomRngState = seed;
}
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t MagicRoomRandom() {
    roomRngState = roomRngState * 0x41C64E6DU + 0x8707U;
    return (roomRngState >> 16) & 0x7fff;
}
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t MagicRoomRandomRange(std::uint32_t range) {
    return MagicRoomRandom() % range;
}
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
float MagicRoomRandomFloat() {
    return static_cast<float>(static_cast<std::int32_t>(MagicRoomRandom())) / 32767.0f;
}
#endif
} // namespace
void SetMagicRoomEntityBuffer(void *buffer, std::int32_t capacity) {
    roomEntityCount = 0;
    roomEntityCapacity = capacity;
    roomEntityBuffer = static_cast<std::byte *>(buffer);
}
namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResetMagicRoomEntityCount() {
    roomEntityCount = 0;
}
#endif
} // namespace
MagicRoomUnsigned GetMagicRoomEntityCount() {
    return static_cast<std::uint32_t>(roomEntityCount);
}
MagicRoomEntityLight *GetMagicRoomEntityParameters(std::int32_t index) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (index < roomEntityCount) {
        MagicRoomEntityLight *const light = reinterpret_cast<MagicRoomEntityLight *>(roomEntityBuffer + static_cast<std::ptrdiff_t>(index) * 64);
        if (light->enabled != 0)
            return light;
    }
#else
    if (index >= roomEntityCount)
#endif
    return 0;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    MagicRoomEntityLight *const light = reinterpret_cast<MagicRoomEntityLight *>(roomEntityBuffer + static_cast<std::ptrdiff_t>(index) * 64);
    return light->enabled == 0 ? 0 : light;
#endif
}
void AddMagicRoomEntityMatrix(const Matrix4x4 *matrix) {
    if (matrix == 0 || roomEntityCount < roomEntityCapacity) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        reinterpret_cast<Matrix4x4 *>(roomEntityBuffer)[roomEntityCount] = *matrix;
#else
        std::memcpy(roomEntityBuffer + static_cast<std::ptrdiff_t>(roomEntityCount) * 64, matrix, sizeof(*matrix));
#endif
        ++roomEntityCount;
    }
}
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MagicRoomInteger GenerateMagicRoomPlacements(MagicRoomPlacement *placements, std::int32_t capacity, std::int32_t seed) {
#else
MagicRoomUnsigned GenerateMagicRoomPlacements(MagicRoomPlacement *placements, std::int32_t capacity, std::int32_t seed) {
#endif
    for (std::int32_t index = 0; index < capacity; ++index) {
        placements[index].modelHandle = 0;
        placements[index].extraRotationX = 0;
        placements[index].yaw = 0;
        placements[index].extraRotationZ = 0;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        placements[index].fadeAlpha = 0.0f;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    SetMagicRoomRandomSeed(static_cast<std::uint32_t>(seed));
    capacity -= 10;
    ResetMagicRoomEntityCount();
#else
    const std::int32_t limit = capacity - 10;
#endif
    std::int32_t count = 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    roomGeneration.boundaryX = -roomGeneration.dimensions.x;
    roomGeneration.cursor.firstThree.x = roomGeneration.dimensions.y / 3.0f;
    roomGeneration.cursor.firstThree.y = roomGeneration.dimensions.x;
    roomGeneration.cursor.firstThree.z = roomGeneration.dimensions.y / 3.0f;
    roomGeneration.cursor.x3 = roomGeneration.dimensions.x / 3.0f;
    roomGeneration.cursor.d2 = -roomGeneration.dimensions.y;
    roomGeneration.cursor.x3Boundary = -roomGeneration.dimensions.x / 3.0f;
    roomGeneration.cursor.d3Boundary = -roomGeneration.dimensions.y;
    if (count < capacity)
        count = PlaceMagicRoomBaseModel(placements, capacity, 0, 0.0f);
    if (count < capacity)
        count += PlaceMagicRoomBaseModel(placements + count, capacity - count, 2, roomGeneration.dimensions.z);
    if (count < capacity)
        count += PlaceMagicRoomBaseModel(placements + count, capacity - count, 3, 0.0f);
    if (count < capacity)
        count += static_cast<std::int32_t>(ConstructMagicRoomSpecialPlacement(placements + count, capacity - count));
    if (roomGeneration.modes[6] == 0 && count < capacity)
        count += static_cast<std::int32_t>(FillMagicRoomLowerRightCorner(placements + count, capacity - count));
    if (roomGeneration.modes[8] == 0 && count < capacity)
        count += static_cast<std::int32_t>(FillMagicRoomLowerLeftCorner(placements + count, capacity - count));
    if (roomGeneration.modes[0] == 0 && count < capacity)
        count += static_cast<std::int32_t>(FillMagicRoomUpperRightCorner(placements + count, capacity - count));
    if (roomGeneration.modes[2] == 0 && count < capacity)
        count += static_cast<std::int32_t>(FillMagicRoomUpperLeftCorner(placements + count, capacity - count));
    if (roomGeneration.usage[4] != 0)
        roomGeneration.capacity[31] = 2;
    else
        roomGeneration.capacity[31] = 0;
    if (roomGeneration.modes[7] == 0 && count < capacity)
        count += static_cast<std::int32_t>(FillMagicRoomLowerWall(placements + count, capacity - count));
    if (roomGeneration.modes[3] == 0 && count < capacity)
        count += static_cast<std::int32_t>(FillMagicRoomRightWall(placements + count, capacity - count));
    if (roomGeneration.modes[5] == 0 && count < capacity)
        count += static_cast<std::int32_t>(FillMagicRoomLeftWall(placements + count, capacity - count));
#else
    roomBoundaryX = -roomDimensions.x;
    roomCursor.firstThree.y = roomDimensions.x;
    roomCursor.d2 = -roomDimensions.y;
    roomCursor.d3Boundary = -roomDimensions.y;
    roomRngState = static_cast<std::uint32_t>(seed);
    roomEntityCount = 0;
    roomCursor.firstThree.x = roomDimensions.y / 3.0f;
    roomCursor.x3 = roomDimensions.x / 3.0f;
    roomCursor.firstThree.z = roomDimensions.y / 3.0f;
    roomCursor.x3Boundary = roomDimensions.x / -3.0f;
    if (count < limit) {
        const std::int32_t model = static_cast<std::int32_t>(PickMagicRoomBaseModel(0));
        if (model != -1) {
            MagicRoomPlacement &placement = placements[count++];
            placement.kind = 0;
            placement.position.x = placement.position.y = placement.position.z = 0.0f;
            placement.halfWidth = placement.height = placement.halfDepth = 0.0f;
            placement.yaw = 0;
            placement.modelIndex = static_cast<std::uint16_t>(model);
        }
        if (count < limit) {
            const float ceilingY = roomDimensions.z;
            const std::int32_t ceiling = static_cast<std::int32_t>(PickMagicRoomBaseModel(2));
            if (ceiling != -1) {
                MagicRoomPlacement &placement = placements[count++];
                placement.kind = 0;
                placement.position.x = placement.position.z = 0.0f;
                placement.position.y = ceilingY;
                placement.halfWidth = placement.height = placement.halfDepth = 0.0f;
                placement.yaw = 0;
                placement.modelIndex = static_cast<std::uint16_t>(ceiling);
            }
            if (count < limit) {
                const std::int32_t base = static_cast<std::int32_t>(PickMagicRoomBaseModel(3));
                if (base != -1) {
                    MagicRoomPlacement &placement = placements[count++];
                    placement.kind = 0;
                    placement.position.x = placement.position.y = placement.position.z = 0.0f;
                    placement.halfWidth = placement.height = placement.halfDepth = 0.0f;
                    placement.yaw = 0;
                    placement.modelIndex = static_cast<std::uint16_t>(base);
                }
                if (count < limit)
                    count += static_cast<std::int32_t>(ConstructMagicRoomSpecialPlacement(placements + count, limit - count));
            }
        }
    }
    if (roomModes[6] == 0 && count < limit)
        count += static_cast<std::int32_t>(FillMagicRoomLowerRightCorner(placements + count, limit - count));
    if (roomModes[8] == 0 && count < limit)
        count += static_cast<std::int32_t>(FillMagicRoomLowerLeftCorner(placements + count, limit - count));
    if (roomModes[0] == 0 && count < limit)
        count += static_cast<std::int32_t>(FillMagicRoomUpperRightCorner(placements + count, limit - count));
    if (roomModes[2] == 0 && count < limit)
        count += static_cast<std::int32_t>(FillMagicRoomUpperLeftCorner(placements + count, limit - count));
    roomCapacity[31] = roomUsage[4] != 0 ? 2 : 0;
    if (roomModes[7] == 0 && count < limit)
        count += static_cast<std::int32_t>(FillMagicRoomLowerWall(placements + count, limit - count));
    if (roomModes[3] == 0 && count < limit)
        count += static_cast<std::int32_t>(FillMagicRoomRightWall(placements + count, limit - count));
    if (roomModes[5] == 0 && count < limit)
        count += static_cast<std::int32_t>(FillMagicRoomLeftWall(placements + count, limit - count));
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (roomGeneration.modes[4] == 0 && count < capacity)
        count += FillMagicRoomInterior(placements + count, capacity - count);
#endif
    return static_cast<std::uint32_t>(count);
}
} // namespace s2
} // namespace shenmue
