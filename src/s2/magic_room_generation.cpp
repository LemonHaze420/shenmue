#include "s2/magic_room.hpp"

#include <bit>
#include <cmath>
#include <cstring>

namespace shenmue::s2 {
MagicRoomHeader *g_magicRoomDatabase{};
MagicRoomGroup *g_magicRoomGroups{};
MagicRoomModel *g_magicRoomModels{};
namespace {
std::uint8_t roomModes[9]{};
std::uint8_t roomCapacity[45]{};
std::uint8_t roomUsage[45]{};
std::uint8_t roomModeSeed{};
Vector3 roomDimensions{};
MagicRoomCursorState roomCursor{};
float roomBoundaryX{};
std::uint32_t roomRngState{};
std::int16_t roomCandidates[128]{};
std::int32_t roomEntityCount{};
std::int32_t roomEntityCapacity{};
std::byte *roomEntityBuffer{};

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
std::int64_t ConstructMagicRoomSeatingPlacement(MagicRoomPlacement *placements, int modelIndex, int remaining, float *usedWidth, float *usedDepth);
void ExpandMagicRoomDimensions(MagicRoomModel *model, Vector3 *outDim);
std::int64_t FindMagicRoomCandidates(std::int64_t a1, int a2, float x, float z);
std::int64_t PickMagicRoomBaseModel(int a1);
std::int64_t FindMagicRoomBaseCandidates(int a1, float a2, float a3);
std::int64_t ConstructMagicRoomSecondaryPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5);
int ConstructMagicRoomFurniture(
    MagicRoomPlacement *out, std::uint16_t modelIndex, int remaining, float *usedWidth, float *usedDepth, std::uint16_t orientation);
std::int64_t ConstructMagicRoomAxisPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5);
std::int64_t ConstructMagicRoomBasicPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5);
std::int64_t FillMagicRoomLowerWall(MagicRoomPlacement *a1, int a2);
std::int64_t FillMagicRoomLowerRightCorner(MagicRoomPlacement *a1, int a2);
std::int64_t FillMagicRoomRightWall(MagicRoomPlacement *a1, int a2);
std::int64_t FillMagicRoomUpperRightCorner(MagicRoomPlacement *a1, int a2);
std::int64_t FillMagicRoomLowerLeftCorner(MagicRoomPlacement *a1, int a2);
std::int64_t FillMagicRoomLeftWall(MagicRoomPlacement *a1, int a2);
std::int64_t FillMagicRoomUpperLeftCorner(MagicRoomPlacement *a1, int a2);
std::int64_t ConstructMagicRoomSpecialPlacement(MagicRoomPlacement *a1, int a2);
std::int64_t ConstructMagicRoomGroupedPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5);
int AddMagicRoomSurfaceDecoration(MagicRoomPlacement *outChild, const MagicRoomPlacement *parent, const MagicRoomModel *parentModel, int remaining);

void ExpandMagicRoomDimensions(MagicRoomModel *model, Vector3 *outDim) {
    float Z;
    float v4;
    float X;
    float v6;
    float v7;
    float v8;
    float v9;

    outDim->x = model->dimensions.x;
    Z = model->dimensions.z;
    outDim->z = Z;
    switch (model->category) {
    case 4:
        outDim->z = Z * 1.5f;
        break;
    case 5:
    case 6:
    case 9:
        v4 = outDim->z * 3.0f;
        outDim->x = outDim->x * 1.2f;
        outDim->z = v4;
        break;
    case 7:
        outDim->x = outDim->x * 1.2f;
        outDim->z = outDim->z * 1.2f;
        break;
    case 11:
        X = outDim->x;
        v6 = outDim->x / Z;
        if (v6 <= 0.89999998f || v6 >= 1.1f)
            v7 = X * 1.5f;
        else
            v7 = X + 1.6f;
        v8 = outDim->z + 3.2f;
        outDim->x = v7;
        outDim->z = v8;
        break;
    case 18:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 43:
        v9 = sqrtf((float)(Z * Z) + (float)(outDim->x * outDim->x));
        outDim->x = v9;
        outDim->z = v9;
        break;
    case 28:
        outDim->x = 0.0f;
        outDim->z = 0.0f;
        break;
    default:
        outDim->x = outDim->x * 1.1f;
        outDim->z = outDim->z * 1.1f;
        break;
    }
}

std::int64_t FindMagicRoomCandidates(std::int64_t a1, int a2, float x, float z) {
    unsigned int v4;
    std::int64_t v8;
    std::int64_t v9;
    std::int64_t v10;
    std::int64_t v11;
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
        }
    }
    return v4;
}

std::int64_t PickMagicRoomBaseModel(int a1) {
    unsigned int v1;
    std::int64_t result;
    std::int64_t category;

    v1 = FindMagicRoomBaseCandidates(a1, roomDimensions.x * 2.0f, roomDimensions.y * 2.0f);
    if (v1 == 0)
        return 0xFFFFFFFFLL;

    roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
    result = (std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v1];
    category = g_magicRoomModels[result].category;
    ++roomUsage[category];
    return result;
}

std::int64_t FindMagicRoomBaseCandidates(int a1, float a2, float a3) {
    std::int64_t v3;
    unsigned int v4;
    float v7;
    float v8;
    std::int32_t start;
    std::int32_t count;
    MagicRoomModel *v11;
    std::int16_t *v12;
    std::int64_t v13;
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
                    if ((std::int64_t)++v12 >= (std::int64_t)(roomCandidates + 128))
                        break;
                }
                ++start;
            } while (start < count);
        }
    }
    return v4;
}

std::int64_t ConstructMagicRoomSecondaryPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5) {
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
    std::int64_t category;
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

int ConstructMagicRoomFurniture(
    MagicRoomPlacement *out, std::uint16_t modelIndex, int remaining, float *usedWidth, float *usedDepth, std::uint16_t orientation) {
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
        usedWidth = usedDepth;
    }
    v8 = g_magicRoomModels;
    out->kind = 0;
    switch (v8[modelIndex].category) {
    case 6:
        result = ConstructMagicRoomSecondaryPlacement(out, modelIndex, remaining, usedWidth, v7);
        break;
    case 11:
        result = ConstructMagicRoomSeatingPlacement(out, modelIndex, remaining, usedWidth, v7);
        break;
    case 13:
        result = ConstructMagicRoomGroupedPlacement(out, modelIndex, remaining, usedWidth, v7);
        break;
    case 18:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 43:
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
        out->kind = static_cast<std::int32_t>(MagicRoomPlacementKind::Clock);
        break;
    default:
        result = ConstructMagicRoomAxisPlacement(out, modelIndex, remaining, usedWidth, v7);
        break;
    }
    return result;
}

std::int64_t ConstructMagicRoomAxisPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5) {
    MagicRoomModel *model;
    int v10;
    std::int64_t v11;
    std::int64_t v12;
    float v13;
    float Z;
    Vector3 outDim;

    if (a3 == 0)
        return 0;
    model = &g_magicRoomModels[a2];
    if (model->dimensions.x > *a4 || model->dimensions.z > *a5) {
        v10 = 0;
        a1->yaw = 0x4000;
        v12 = 0;
        v11 = 2;
    } else {
        v10 = 0;
        a1->yaw = 0;
        v11 = 0;
        v12 = 2;
    }
    v13 = *(&model->dimensions.x + v11) * 0.5f;
    a1->position.y = 0.0f;
    a1->position.x = v13;
    a1->position.z = *(&model->dimensions.x + v12) * 0.5f;
    a1->halfWidth = model->dimensions.x * 0.5f;
    a1->height = model->dimensions.y;
    Z = model->dimensions.z;
    a1->modelIndex = a2;
    a1->halfDepth = Z * 0.5f;
    ExpandMagicRoomDimensions(model, &outDim);
    *a4 = *(&outDim.x + v11);
    *a5 = *(&outDim.x + v12);
    if (model->category == 7)
        v10 = AddMagicRoomSurfaceDecoration(a1 + 1, a1, model, a3);
    return (unsigned int)(v10 + 1);
}

std::int64_t ConstructMagicRoomBasicPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5) {
    std::uint16_t v7;
    MagicRoomModel *v9;
    int v10;
    Vector3 *p_Z;
    Vector3 *p_outDim;
    float X;
    float v14;
    float v15;
    Vector3 outDim;

    v7 = a2;
    if (a3 == 0)
        return 0;
    v9 = &g_magicRoomModels[a2];
    v10 = 0;
    if (v9->dimensions.x > *a4 || v9->dimensions.z > *a5) {
        p_Z = (Vector3 *)&outDim.z;
        a1->yaw = 0x4000;
        p_outDim = &outDim;
    } else {
        a1->yaw = 0;
        p_Z = &outDim;
        p_outDim = (Vector3 *)&outDim.z;
    }
    ExpandMagicRoomDimensions(v9, &outDim);
    X = p_Z->x;
    v14 = p_outDim->x;
    *a4 = p_Z->x;
    *a5 = v14;
    a1->position.y = 0.02f;
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
}

std::int64_t FillMagicRoomLowerWall(MagicRoomPlacement *a1, int a2) {
    float X;
    float v5;
    float v6;
    int v7;
    unsigned int v8;
    std::uint16_t v9;
    std::int64_t category;
    int v11;
    float usedWidth;
    float usedDepth;

    X = roomCursor.firstThree.x;
    if (roomCursor.firstThree.z <= roomCursor.firstThree.x)
        X = roomCursor.firstThree.z;
    v5 = X + roomDimensions.y;
    v6 = roomBoundaryX;
    v7 = 0;
    do {
        usedDepth = v5;
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
        v6 = v6 - usedWidth;
        v7 += v11;
    } while (v7 < a2);
    return (unsigned int)v7;
}

std::int64_t FillMagicRoomLowerRightCorner(MagicRoomPlacement *a1, int a2) {
    unsigned int v2;
    int orientation;
    float v5;
    float v6;
    float v8;
    float v9;
    unsigned int v10;
    std::uint16_t v11;
    std::int64_t category;
    std::int64_t result;
    float v14;
    float usedWidth;
    float usedDepth;

    v2 = 0;
    orientation = 0;
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
    }
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
    }
    result = v2;
    v14 = v6 - roomDimensions.y;
    roomCursor.firstThree.x = v14;
    roomBoundaryX = roomDimensions.x - v5;
    roomCursor.firstThree.y = roomDimensions.x - v5;
    if (roomCursor.x3Boundary > (float)(roomDimensions.x - v5))
        roomCursor.d3Boundary = v14;
    return result;
}

std::int64_t FillMagicRoomRightWall(MagicRoomPlacement *a1, int a2) {
    float v2;
    float d2;
    float X;
    int v7;
    unsigned int v8;
    std::uint16_t v9;
    std::int64_t category;
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
}

std::int64_t FillMagicRoomUpperRightCorner(MagicRoomPlacement *a1, int a2) {
    unsigned int v2;
    int orientation;
    float v5;
    float v6;
    float v8;
    float v9;
    unsigned int v10;
    std::uint16_t v11;
    std::int64_t category;
    float usedWidth;
    float usedDepth;

    v2 = 0;
    orientation = 0x8000;
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
    }
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
    }
    if (v6 < 1.2f)
        v6 = 1.2f;
    roomCursor.x3 = roomDimensions.x - v5;
    roomCursor.d2 = roomDimensions.y - v6;
    return v2;
}

std::int64_t FillMagicRoomLowerLeftCorner(MagicRoomPlacement *a1, int a2) {
    unsigned int v2;
    int orientation;
    float v4;
    float v5;
    std::int32_t v8;
    float v9;
    float v10;
    unsigned int v11;
    int v12;
    std::int64_t category;
    float v14;
    float v15;
    unsigned int v16;
    int v17;
    std::int64_t v18;
    float v19;
    std::int64_t result;
    float usedWidth;
    float usedDepth;

    v2 = 0;
    orientation = 0;
    v4 = roomCursor.firstThree.y + roomDimensions.x;
    v5 = roomCursor.firstThree.z + roomDimensions.y;
    roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
    v8 = (roomRngState & 0x10000) != 0;
    usedWidth = roomCursor.firstThree.y + roomDimensions.x;
    usedDepth = roomCursor.firstThree.z + roomDimensions.y;
    v9 = roomCursor.firstThree.z + roomDimensions.y;
    if ((roomRngState & 0x10000) == 0)
        orientation = 0x4000;
    v10 = roomCursor.firstThree.y + roomDimensions.x;
    if ((roomRngState & 0x10000) == 0) {
        v10 = roomCursor.firstThree.z + roomDimensions.y;
        v9 = roomCursor.firstThree.y + roomDimensions.x;
    }
    v11 = FindMagicRoomCandidates(reinterpret_cast<std::intptr_t>(magicRoomFurnitureCategories + 0), 6, v10, v9);
    if (v11 != 0) {
        roomRngState = 0x41C64E6DU * roomRngState + 0x8707;
        v12 = (std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v11];
        category = g_magicRoomModels[(std::uint16_t)roomCandidates[((roomRngState >> 16U) & 0x7FFFu) % v11]].category;
        ++roomUsage[category];
    } else {
        v12 = -1;
    }
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

std::int64_t FillMagicRoomLeftWall(MagicRoomPlacement *a1, int a2) {
    float x3;
    float Z;
    int v6;
    float v7;
    unsigned int v8;
    std::uint16_t v9;
    std::int64_t category;
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
}

std::int64_t FillMagicRoomUpperLeftCorner(MagicRoomPlacement *a1, int a2) {
    unsigned int v2;
    float v5;
    float v6;
    int orientation;
    float v8;
    float v9;
    unsigned int v10;
    std::uint16_t v11;
    std::int64_t category;
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
    }
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
    }
    if (v6 < 1.2f)
        v6 = 1.2f;
    roomCursor.x3Boundary = v5 - roomDimensions.x;
    roomCursor.d3Boundary = roomDimensions.y - v6;
    return v2;
}

std::int64_t ConstructMagicRoomSpecialPlacement(MagicRoomPlacement *a1, int a2) {
    unsigned int v3;
    float v5;

    if (static_cast<std::int8_t>(roomUsage[14]) >= static_cast<std::int8_t>(roomCapacity[14]))
        return 0;
    ++roomUsage[14];
    roomRngState = 0x41C64E6DU * roomRngState + 34567;
    v3 = g_magicRoomGroups[14].start + ((roomRngState >> 16U) & 0x7FFFu) % g_magicRoomGroups[14].count;
    if (v3 == -1 || a2 == 0)
        return 0;
    a1->kind = 1;
    a1->position.x = 0.0f;
    v5 = roomDimensions.z - 0.0099999998f;
    a1->position.z = 0.0f;
    a1->halfWidth = 0.0f;
    a1->height = 0.0f;
    a1->halfDepth = 0.0f;
    a1->yaw = 0;
    a1->position.y = v5;
    a1->modelIndex = v3;
    return 1;
}

std::int64_t ConstructMagicRoomGroupedPlacement(MagicRoomPlacement *a1, int a2, int a3, float *a4, float *a5) {
    std::uint16_t v6;
    unsigned int v7;
    float v8;
    float v9;
    float v10;
    MagicRoomModel *v12;
    unsigned int v14;
    MagicRoomModel *v15;
    int v16;
    std::int64_t category;
    float *p_X;
    int v19;
    std::int64_t v20;
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
}

int AddMagicRoomSurfaceDecoration(MagicRoomPlacement *outChild, const MagicRoomPlacement *parent, const MagicRoomModel *parentModel, int remaining) {
    unsigned int v7;
    int v8;
    MagicRoomModel *v9;
    int v10;
    std::int64_t category;
    Vector3 *p_x;
    float v13;
    float v14;
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
    v13 = (float)(parentModel->dimensions.x - p_x->x) * 0.69999999f;
    v14 = (float)(parentModel->dimensions.z - p_x->z) * 0.69999999f;
    roomRngState = 0x41C64E6DU * (0x41C64E6DU * v8 + 0x8707) + 0x8707;
    v15 = (roomRngState >> 16U) & 0x7FFF;
    outChild->position.x =
        (float)((float)((float)((float)(((unsigned int)(0x41C64E6DU * v8 + 0x8707) >> 16) & 0x7FFF) / 32767.0f) - 0.5f) * v13) + parent->position.x;
    outChild->position.y = (float)(parent->height + parent->position.y) + 0.0099999998f;
    v16 = (float)((float)((float)((float)v15 / 32767.0f) - 0.5f) * v14) + parent->position.z;
    outChild->halfWidth = 0.0f;
    outChild->height = 0.0f;
    outChild->halfDepth = 0.0f;
    outChild->position.z = v16;
    outChild->yaw = parent->yaw;
    outChild->modelIndex = v10;
    return 1;
}

std::int64_t ConstructMagicRoomSeatingPlacement(MagicRoomPlacement *placements, int modelIndex, int remaining, float *usedWidth, float *usedDepth) {
    if (remaining < 2)
        return 0;
    const float availableWidth = *usedWidth;
    const float availableDepth = *usedDepth;
    MagicRoomModel *const model = &g_magicRoomModels[modelIndex];
    MagicRoomModel *secondary = nullptr;
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
} // namespace

std::uint64_t ReadMagicRoomDatabase(MagicRoomHeader *database) {
    g_magicRoomDatabase = database;
    if (database->tag == TASK_MAGIC_ROOM.name && database->version == 1) {
        g_magicRoomGroups = reinterpret_cast<MagicRoomGroup *>(reinterpret_cast<std::byte *>(database) + database->groupOffset);
        g_magicRoomModels = reinterpret_cast<MagicRoomModel *>(reinterpret_cast<std::byte *>(database) + database->modelOffset);
        return 0;
    }
    g_magicRoomDatabase = nullptr;
    g_magicRoomGroups = nullptr;
    g_magicRoomModels = nullptr;
    return 0xffffffffU;
}
void InitializeMagicRoomGeneration() {
    for (std::uint8_t &capacity : roomCapacity)
        capacity = 50;
    roomDimensions = {.x = 2.5f, .y = 3.4000001f, .z = 3.6f};
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
}
std::uint64_t FillMagicRoomCategorySeeds() {
    for (std::int32_t index = 0; index < 9; ++index)
        roomModes[index] = roomModeSeed == index;
    std::memset(roomUsage, 0, sizeof(roomUsage));
    return 9;
}
void SetMagicRoomDimensions(float width, float height, float depth) {
    roomDimensions.z = height;
    roomDimensions.x = width * 0.5f - 0.15000001f;
    roomDimensions.y = depth * 0.5f - 0.2f;
}
void GetMagicRoomDimensions(Vector3 *dimensions) {
    dimensions->x = roomDimensions.x;
    dimensions->y = roomDimensions.z;
    dimensions->z = roomDimensions.y;
}
void SetMagicRoomEntityBuffer(void *buffer, std::int32_t capacity) {
    roomEntityCount = 0;
    roomEntityCapacity = capacity;
    roomEntityBuffer = static_cast<std::byte *>(buffer);
}
std::uint64_t GetMagicRoomEntityCount() {
    return static_cast<std::uint32_t>(roomEntityCount);
}
MagicRoomEntityLight *GetMagicRoomEntityParameters(std::int32_t index) {
    if (index >= roomEntityCount)
        return nullptr;
    MagicRoomEntityLight *const light = reinterpret_cast<MagicRoomEntityLight *>(roomEntityBuffer + static_cast<std::ptrdiff_t>(index) * 64);
    return light->enabled == 0 ? nullptr : light;
}
void AddMagicRoomEntityMatrix(const Matrix4x4 *matrix) {
    if (matrix == nullptr || roomEntityCount < roomEntityCapacity) {
        std::memcpy(roomEntityBuffer + static_cast<std::ptrdiff_t>(roomEntityCount) * 64, matrix, sizeof(*matrix));
        ++roomEntityCount;
    }
}
std::uint64_t GenerateMagicRoomPlacements(MagicRoomPlacement *placements, std::int32_t capacity, std::int32_t seed) {
    for (std::int32_t index = 0; index < capacity; ++index) {
        placements[index].modelHandle = nullptr;
        placements[index].extraRotationX = 0;
        placements[index].yaw = 0;
        placements[index].extraRotationZ = 0;
        placements[index].fadeAlpha = 0.0f;
    }
    const std::int32_t limit = capacity - 10;
    std::int32_t count = 0;
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
            placement.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
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
                placement.position = {.x = 0.0f, .y = ceilingY, .z = 0.0f};
                placement.halfWidth = placement.height = placement.halfDepth = 0.0f;
                placement.yaw = 0;
                placement.modelIndex = static_cast<std::uint16_t>(ceiling);
            }
            if (count < limit) {
                const std::int32_t base = static_cast<std::int32_t>(PickMagicRoomBaseModel(3));
                if (base != -1) {
                    MagicRoomPlacement &placement = placements[count++];
                    placement.kind = 0;
                    placement.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
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
    return static_cast<std::uint32_t>(count);
}
} // namespace shenmue::s2
