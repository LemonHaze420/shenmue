#include "s1/task/fbgg.hpp"
#include "s1/task/fork.hpp"

#include "s1/boot.hpp"
#include "s1/collision.hpp"
#include "s1/file_loader.hpp"
#include "s1/input.hpp"
#include "s1/lighting.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_types.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/lamp.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/sond.hpp"
#include "s1/ui_runtime.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

Character *g_pTempFBGGChara = nullptr;
std::int32_t g_fbggRiderMotionState = 0;
float g_fbggGauge = 0.0f;
std::int32_t g_fbggRideHeading = 0;
std::int32_t g_motnCameraFollowAngle = 0;
std::uint32_t g_vehicleRideFrameCount = 0;
Vector3 g_vehicleRightAnkleTarget{};
Vector3 g_vehicleLeftAnkleTarget{};

namespace {

constexpr float kMissingSample = 99999.0f;
constexpr double kRadiansToFixed16 = 10430.378350470464;
constexpr float kHalfPi = 1.5707964f;
constexpr std::uint8_t kFollowerControlMode = 3;
constexpr std::int32_t kRiderMotionEngaged = 10;
constexpr std::int32_t kRiderMotionActive = 60;
constexpr std::uint32_t kAicaEnvSetVolume = 0x10a5U;


constexpr FBGGRowHeightRange g_fbggRowHeightRanges[5] = {
    {.baseHeight = 0.0f, .start = 8, .end = 16},
    {.baseHeight = 0.0f, .start = 63, .end = 75},
    {.baseHeight = 0.0f, .start = 136, .end = 141},
    {.baseHeight = 7.19999981f, .start = 208, .end = 225},
    {.baseHeight = 0.0f, .start = 9999, .end = 9999},
};


constexpr float g_fbggSpeedTierThresholds[3][2] = {{0.74000001f, 0.27000001f}, {1.11000001f, 0.64999998f}, {1.48000002f, 1.01999998f}};

float g_fbggLeanAngle = 0.0f;
Mt5RuntimeNode *g_fbggBikeModel = nullptr;
const FBGGRowSample *g_fbggRows = nullptr;
Vector3 g_fbggSmoothedPosition{};
Vector3i g_fbggSmoothedEuler{};

float CosineFixed16(std::int32_t angle) {
    return cos_fixed16(static_cast<std::uint16_t>(angle));
}

std::int32_t RadiansToFixed16(float radians) {
    return static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<double>(radians) * kRadiansToFixed16));
}

std::int32_t AtanToFixed16(float ratio) {
    return static_cast<std::uint16_t>(static_cast<std::int32_t>(std::atan(ratio) * 65536.0f / 6.2831855f + 0.5f));
}

std::int32_t AddFixed16(std::int32_t angle, std::int32_t delta) {
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(angle) + static_cast<std::uint32_t>(delta));
}

void GetPrevCurrNextSample(std::int32_t row, Vector3 &currentLeft, Vector3 &currentRight, Vector3 &nextLeft, Vector3 &nextRight) {
    const FBGGRowSample *const rows = g_fbggRows;
    currentLeft = rows[row].left;
    currentRight = rows[row].right;
    if (currentLeft.x > kMissingSample) {
        const FBGGRowSample *previous = rows + row - 1;
        while (previous->left.x > kMissingSample)
            --previous;
        currentLeft.x = previous->left.x;
        currentLeft.z = previous->left.z;
    }
    if (currentRight.x > kMissingSample) {
        const FBGGRowSample *previous = rows + row - 1;
        while (previous->right.x > kMissingSample)
            --previous;
        currentRight.x = previous->right.x;
        currentRight.z = previous->right.z;
    }
    nextLeft = rows[row + 1].left;
    nextRight = rows[row + 1].right;
}


void GetRowCentreline(std::int32_t row, float &startX, float &startZ, float &endSumX, float &endSumZ) {
    Vector3 currentLeft{};
    Vector3 currentRight{};
    Vector3 nextLeft{};
    Vector3 nextRight{};
    GetPrevCurrNextSample(row, currentLeft, currentRight, nextLeft, nextRight);
    startX = (currentRight.x + currentLeft.x) * 0.5f;
    startZ = (currentRight.z + currentLeft.z) * 0.5f;
    if (nextLeft.x > kMissingSample) {
        endSumX = nextRight.x + startX;
        endSumZ = nextRight.z + startZ;
    } else if (nextRight.x > kMissingSample) {
        endSumX = nextLeft.x + currentRight.x;
        endSumZ = nextLeft.z + currentRight.z;
    } else {
        endSumX = nextRight.x + nextLeft.x;
        endSumZ = nextLeft.z + nextRight.z;
    }
}

float CrossXZ(const Vector3 &point, const Vector3 &start, const Vector3 &end) {
    return (point.x - start.x) * (end.z - start.z) - (point.z - start.z) * (end.x - start.x);
}

void KeepFarthestEdge(std::int32_t row, float cross, const Vector3 &start, const Vector3 &end, std::int32_t &bestRow, float &bestMetric) {
    const float dx = end.x - start.x;
    const float dz = end.z - start.z;
    const float metric = cross / std::sqrt(dx * dx + dz * dz);
    if (metric > bestMetric) {
        bestMetric = metric;
        bestRow = row;
    }
}

std::int32_t CheckPointInRowQuad_KeepFarthest(std::int32_t row, const Vector3 &point, std::int32_t &bestRow, float &bestMetric) {
    Vector3 currentLeft{};
    Vector3 currentRight{};
    Vector3 nextLeft{};
    Vector3 nextRight{};
    GetPrevCurrNextSample(row, currentLeft, currentRight, nextLeft, nextRight);
    if (nextLeft.x > kMissingSample) {
        if (CrossXZ(point, currentLeft, currentRight) < 0.0f || CrossXZ(point, nextRight, currentLeft) < 0.0f)
            return 0;
        const float cross = CrossXZ(point, currentRight, nextRight);
        if (cross < 0.0f) {
            KeepFarthestEdge(row, cross, currentRight, nextRight, bestRow, bestMetric);
            return 0;
        }
        return 1;
    }
    if (nextRight.x > kMissingSample) {
        if (CrossXZ(point, currentLeft, currentRight) < 0.0f || CrossXZ(point, currentRight, nextLeft) < 0.0f)
            return 0;
        const float cross = CrossXZ(point, nextLeft, currentLeft);
        if (cross < 0.0f) {
            KeepFarthestEdge(row, cross, nextLeft, currentLeft, bestRow, bestMetric);
            return 0;
        }
        return 1;
    }
    if (CrossXZ(point, currentLeft, currentRight) < 0.0f || CrossXZ(point, nextRight, nextLeft) < 0.0f)
        return 0;
    float cross = CrossXZ(point, currentRight, nextRight);
    if (cross < 0.0f) {
        KeepFarthestEdge(row, cross, currentRight, nextRight, bestRow, bestMetric);
        return 0;
    }
    cross = CrossXZ(point, nextLeft, currentLeft);
    if (cross < 0.0f) {
        KeepFarthestEdge(row, cross, nextLeft, currentLeft, bestRow, bestMetric);
        return 0;
    }
    return 1;
}


std::int32_t FindRowContainingPoint(std::int32_t row, const Vector3 &point, std::int32_t &bestRow, float &bestMetric) {
    bestRow = -1;
    bestMetric = -99999.0f;
    if (static_cast<std::uint32_t>(row) > 273U) {
        for (row = 0; row < 274; ++row) {
            if (CheckPointInRowQuad_KeepFarthest(row, point, bestRow, bestMetric))
                return row;
        }
        return -1;
    }
    if (CheckPointInRowQuad_KeepFarthest(row, point, bestRow, bestMetric))
        return row;
    std::int32_t lower = row;
    while (lower > 0 || row < 273) {
        if (--lower >= 0) {
            if (CheckPointInRowQuad_KeepFarthest(lower, point, bestRow, bestMetric))
                return lower;
        } else {
            lower = 0;
        }
        if (++row < 274) {
            if (CheckPointInRowQuad_KeepFarthest(row, point, bestRow, bestMetric))
                return row;
        } else {
            row = 273;
        }
    }
    return -1;
}

float TransformParam2D(float dz, float dx, float nextDz, float nextDx, float pointZ, float pointX) {
    return ((dz * pointX - dx * pointZ) / (dz * pointZ + dx * pointX)) / ((dz * nextDx - dx * nextDz) / (dz * nextDz + dx * nextDx));
}

float ProjectPointOntoSegmentPairXZ(std::int32_t row, const Vector3 &point) {
    Vector3 currentLeft{};
    Vector3 currentRight{};
    Vector3 nextLeft{};
    Vector3 nextRight{};
    GetPrevCurrNextSample(row, currentLeft, currentRight, nextLeft, nextRight);
    const float dz = currentRight.z - currentLeft.z;
    const float dx = currentRight.x - currentLeft.x;
    if (nextLeft.x > kMissingSample) {
        const float pointX = point.x - currentLeft.x;
        const float pointZ = point.z - currentLeft.z;
        const float nextX = nextRight.x - currentLeft.x;
        const float nextZ = nextRight.z - currentLeft.z;
        return ((dz * pointX - pointZ * dx) / (pointZ * dz + dx * pointX)) / ((nextX * dz - nextZ * dx) / (nextX * dx + nextZ * dz));
    }
    if (nextRight.x > kMissingSample) {
        const float pointX = currentRight.x - point.x;
        const float pointZ = currentRight.z - point.z;
        const float nextX = currentRight.x - nextLeft.x;
        const float nextZ = currentRight.z - nextLeft.z;
        return ((pointX * dz - pointZ * dx) / (pointX * dx + pointZ * dz)) / ((nextX * dz - nextZ * dx) / (nextX * dx + nextZ * dz));
    }
    const float nextDx = nextRight.x - nextLeft.x;
    const float nextDz = nextRight.z - nextLeft.z;
    const float denominator = nextDx * dz - nextDz * dx;
    if (std::fabs(denominator) < 0.0001f) {
        return ((point.x - currentLeft.x) * dz - (point.z - currentLeft.z) * dx) / ((nextLeft.x - currentLeft.x) * dz - (nextLeft.z - currentLeft.z) * dx);
    }
    const float nextConstant = nextLeft.z * nextDx - nextDz * nextLeft.x;
    const float currentConstant = dx * currentLeft.z - currentLeft.x * dz;
    const float intersectionX = (nextConstant * dx - currentConstant * nextDx) / denominator;
    const float intersectionZ = (nextConstant * dz - currentConstant * nextDz) / denominator;
    if (0.0f > denominator)
        return TransformParam2D(dz, dx, nextDz, nextDx, intersectionZ - point.z, intersectionX - point.x);
    return TransformParam2D(dz, dx, nextDz, nextDx, point.z - intersectionZ, point.x - intersectionX);
}

// Tangent of the ride heading against the row centreline.
float GetRowRelativeHeadingTangent(std::int32_t row) {
    float startX = 0.0f;
    float startZ = 0.0f;
    float endSumX = 0.0f;
    float endSumZ = 0.0f;
    GetRowCentreline(row, startX, startZ, endSumX, endSumZ);
    const float dx = endSumX * 0.5f - startX;
    const float dz = endSumZ * 0.5f - startZ;
    const float sine = sin_fixed16(static_cast<std::int16_t>(g_fbggRideHeading));
    const float cosine = CosineFixed16(g_fbggRideHeading);
    return (dx * cosine - dz * sine) / (-(dz * cosine) - dx * sine);
}

float GetRowSegmentLengthFromSamples(std::int32_t row) {
    float startX = 0.0f;
    float startZ = 0.0f;
    float endSumX = 0.0f;
    float endSumZ = 0.0f;
    GetRowCentreline(row, startX, startZ, endSumX, endSumZ);
    const float dx = endSumX * 0.5f - startX;
    const float dz = endSumZ * 0.5f - startZ;
    return std::sqrt(dx * dx + dz * dz);
}

std::int32_t GetRowHeadingAngle(std::int32_t row) {
    float startX = 0.0f;
    float startZ = 0.0f;
    float endSumX = 0.0f;
    float endSumZ = 0.0f;
    GetRowCentreline(row, startX, startZ, endSumX, endSumZ);
    return atan2_fixed16(-(endSumX * 0.5f - startX), -(endSumZ * 0.5f - startZ));
}

float RowCentreHeight(std::int32_t row) {
    return (g_fbggRows[row].right.y + g_fbggRows[row].left.y) * 0.5f;
}


float EvalRowHeightCubic(float t, float (&heights)[4], std::int32_t row, std::int32_t start, std::int32_t end) {
    const float t2 = t * t;
    const float t3 = t2 * t;
    float c0 = 0.0f;
    float c1 = 0.0f;
    float c2 = 0.0f;
    float c3 = 0.0f;
    if (row == start) {
        heights[3] = 0.0f;
        c0 = static_cast<float>(static_cast<double>(t3 * 1.5f) - static_cast<double>(t2) * 2.5 + 1.0);
        c1 = t2 * 3.0f - (t3 + t3);
        c2 = t3 * 0.5f - t2 * 0.5f;
    } else if (row == end) {
        heights[0] = 0.0f;
        c1 = (t2 - t3 * 0.5f) - t * 0.5f;
        c2 = ((t3 + t3) - t2 * 3.0f) + 1.0f;
        c3 = (t2 * 2.0f - t3 * 1.5f) + t * 0.5f;
    } else {
        c0 = (t2 - t3 * 0.5f) - t * 0.5f;
        c1 = (t3 * 1.5f - t2 * 2.5f) + 1.0f;
        c2 = (t2 * 2.0f - t3 * 1.5f) + t * 0.5f;
        c3 = t3 * 0.5f - t2 * 0.5f;
    }
    return ((c1 * heights[1] + c0 * heights[0]) + c2 * heights[2]) + c3 * heights[3];
}


float EvalRowHeightCubicSlope(float t, const float (&heights)[4], std::int32_t row, std::int32_t start, std::int32_t end) {
    const float threeTSquared = t * 3.0f * t;
    const float twoT = t + t;
    float c0 = 0.0f;
    float c1 = 0.0f;
    float c2 = 0.0f;
    float c3 = 0.0f;
    if (row == start) {
        c0 = static_cast<float>(static_cast<double>(threeTSquared * 1.5f) - static_cast<double>(twoT) * 2.5);
        c1 = twoT * 3.0f - (threeTSquared + threeTSquared);
        c2 = threeTSquared * 0.5f - twoT * 0.5f;
    } else if (row == end) {
        c1 = (twoT - threeTSquared * 0.5f) - 0.5f;
        c2 = (threeTSquared + threeTSquared) - twoT * 3.0f;
        c3 = (twoT * 2.0f - threeTSquared * 1.5f) + 0.5f;
    } else {
        c0 = (twoT - threeTSquared * 0.5f) - 0.5f;
        c1 = threeTSquared * 1.5f - twoT * 2.5f;
        c2 = (twoT * 2.0f - threeTSquared * 1.5f) + 0.5f;
        c3 = threeTSquared * 0.5f - twoT * 0.5f;
    }
    return ((c1 * heights[1] + c0 * heights[0]) + c2 * heights[2]) + c3 * heights[3];
}

const FBGGRowHeightRange *FindRowHeightRange(std::int32_t row) {
    const FBGGRowHeightRange *range = g_fbggRowHeightRanges;
    if (row < 8)
        return nullptr;
    while (row > range->end) {
        ++range;
        if (row < range->start)
            return nullptr;
    }
    return range;
}

void GatherRowCentreHeights(std::int32_t row, const FBGGRowHeightRange &range, float (&heights)[4]) {
    if (row == range.start) {
        heights[0] = RowCentreHeight(row);
        heights[1] = RowCentreHeight(row + 1);
        heights[2] = RowCentreHeight(row + 2);
    } else if (row == range.end) {
        heights[1] = RowCentreHeight(row - 1);
        heights[2] = RowCentreHeight(row);
        heights[3] = RowCentreHeight(row + 1);
    } else {
        heights[0] = RowCentreHeight(row - 1);
        heights[1] = RowCentreHeight(row);
        heights[2] = RowCentreHeight(row + 1);
        heights[3] = RowCentreHeight(row + 2);
    }
}

void SampleRowHeightChain(std::int32_t row, float t, Vector3 &output) {
    const FBGGRowHeightRange *range = g_fbggRowHeightRanges;
    if (row >= 8) {
        while (row > range->end) {
            ++range;
            if (row < range->start) {
                output.y = range->baseHeight;
                return;
            }
        }
        float heights[4]{};
        GatherRowCentreHeights(row, *range, heights);
        output.y = EvalRowHeightCubic(t, heights, row, range->start, range->end);
        return;
    }
    output.y = range->baseHeight;
}

float GetRowPitchFromHeights(std::int32_t row, float t) {
    const FBGGRowHeightRange *const range = FindRowHeightRange(row);
    if (!range)
        return 0.0f;
    float heights[4]{};
    GatherRowCentreHeights(row, *range, heights);
    const FBGGRowSample &current = g_fbggRows[row];
    const FBGGRowSample &next = g_fbggRows[row + 1];
    const float dx = (next.right.x + next.left.x) * 0.5f - (current.right.x + current.left.x) * 0.5f;
    const float dz = (next.right.z + next.left.z) * 0.5f - (current.right.z + current.left.z) * 0.5f;
    const float slope = EvalRowHeightCubicSlope(t, heights, row, range->start, range->end);
    return std::atan2(slope, std::sqrt(dx * dx + dz * dz));
}

std::int32_t ComputeRowFollowerPitch(const Vector3 &position, float frontOffset, float rearOffset, std::int32_t row, float t, float tangent) {
    const float rowPitch = GetRowPitchFromHeights(row, t);
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    const std::int32_t tangentAngle = RadiansToFixed16(tangent);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(g_fbggRideHeading - tangentAngle));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(RadiansToFixed16(rowPitch)));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(tangentAngle));
    Vector3 front{};
    Vector3 rear{};
    Matrix4x4::TransformPoint({0.0f, 0.0f, frontOffset}, front);
    Matrix4x4::TransformPoint({0.0f, 0.0f, rearOffset}, rear);
    Matrix4x4::Pop(1);
    std::int32_t bestRow = -1;
    float bestMetric = 0.0f;
    std::int32_t frontRow = FindRowContainingPoint(row, front, bestRow, bestMetric);
    if (frontRow == -1)
        frontRow = bestRow;
    std::int32_t rearRow = FindRowContainingPoint(row, rear, bestRow, bestMetric);
    if (rearRow == -1)
        rearRow = bestRow;
    const float rearT = ProjectPointOntoSegmentPairXZ(rearRow, rear);
    const float frontT = ProjectPointOntoSegmentPairXZ(frontRow, front);
    SampleRowHeightChain(frontRow, frontT, front);
    SampleRowHeightChain(rearRow, rearT, rear);
    return atan2_fixed16(front.y - rear.y, rearOffset - frontOffset);
}

void AdvanceRowPointForward(std::int32_t row, float t, Vector3 &output) {
    float length = GetRowSegmentLengthFromSamples(row);
    const float remaining = (1.0f - t) * length;
    float startX = 0.0f;
    float startZ = 0.0f;
    float endSumX = 0.0f;
    float endSumZ = 0.0f;
    float along = 0.0f;
    if (80.0f > remaining) {
        float needed = 80.0f - remaining;
        std::int32_t next = row + 1;
        std::int32_t steps = 1;
        for (;;) {
            if (next > 273) {
                GetRowCentreline(273, startX, startZ, endSumX, endSumZ);
                along = needed / length + 1.0f;
                break;
            }
            length = GetRowSegmentLengthFromSamples(next);
            if (needed > length) {
                ++steps;
                needed -= length;
                ++next;
                if (steps < 273)
                    continue;
            }
            GetRowCentreline(row + steps, startX, startZ, endSumX, endSumZ);
            along = needed / length;
            break;
        }
    } else {
        GetRowCentreline(row, startX, startZ, endSumX, endSumZ);
        along = 80.0f / length + t;
    }
    output.x = along * (endSumX * 0.5f - startX) + startX;
    output.z = along * (endSumZ * 0.5f - startZ) + startZ;
}


bool CornerCrossesRowEdge(const Vector3 &corner, const Vector3 &outerStart, const Vector3 &edgeStart, const Vector3 &edgeEnd, const Vector3 &outerEnd) {
    return !(0.0f > CrossXZ(corner, outerStart, edgeStart)) && !(0.0f > CrossXZ(corner, edgeStart, edgeEnd)) && !(0.0f > CrossXZ(corner, edgeEnd, outerEnd));
}

Vector3 OuterRowPoint(const Vector3 &edge, const Vector3 &opposite) {
    return {edge.x - (opposite.x - edge.x), 0.0f, edge.z - (opposite.z - edge.z)};
}

bool SnapPointToRowQuad(std::int32_t row, Vector3 &candidate, const Vector3 &previous, std::int32_t previousHeading, std::int32_t steer, std::int32_t &side) {
    const std::int32_t heading = g_fbggRideHeading;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(previous.x, previous.y, previous.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(previousHeading));
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(candidate.x, candidate.y, candidate.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(heading));
    const float steerSine = sin_fixed16(static_cast<std::int16_t>(steer));
    const float leftX = steerSine > 0.0f ? -0.5f - steerSine * 0.69999999f : -0.5f;
    const float rightX = steerSine > 0.0f ? 0.5f : 0.5f - steerSine * 0.69999999f;
    Vector3 corners[2]{};
    Matrix4x4::TransformPoint({leftX, 0.0f, -0.95650005f}, corners[0]);
    Matrix4x4::TransformPoint({rightX, 0.0f, -0.95650005f}, corners[1]);
    Matrix4x4::Pop(1);
    const float forwardX = -sin_fixed16(static_cast<std::int16_t>(heading));
    const float forwardZ = -CosineFixed16(heading);
    const FBGGRowSample *const rows = g_fbggRows;
    for (std::int32_t segment = row - 1; segment <= row + 1; ++segment) {
        if ((segment == row - 1 && row == 0) || (segment == row + 1 && row == 273))
            continue;
        const FBGGRowSample &start = rows[segment];
        const FBGGRowSample &end = rows[segment + 1];
        bool hit = false;
        std::int32_t hitSide = 0;
        std::int32_t hitCorner = 0;
        const std::int32_t leftCorner = (end.left.z - start.left.z) * forwardZ + (end.left.x - start.left.x) * forwardX > 0.0f ? 0 : 1;
        if (CornerCrossesRowEdge(corners[leftCorner], OuterRowPoint(start.left, start.right), start.left, end.left, OuterRowPoint(end.left, end.right))) {
            hit = true;
            hitCorner = leftCorner;
        }
        const std::int32_t rightCorner = (end.right.x - start.right.x) * forwardX + (end.right.z - start.right.z) * forwardZ > 0.0f ? 1 : 0;
        if (CornerCrossesRowEdge(corners[rightCorner], OuterRowPoint(end.right, end.left), end.right, start.right, OuterRowPoint(start.right, start.left))) {
            hit = true;
            hitSide = 1;
            hitCorner = rightCorner;
        }
        if (!hit)
            continue;
        const Vector3 &edgeStart = hitSide ? start.right : start.left;
        const Vector3 &edgeEnd = hitSide ? end.right : end.left;
        const Vector3 &corner = corners[hitCorner];
        const float dz = edgeEnd.z - edgeStart.z;
        const float dx = edgeEnd.x - edgeStart.x;
        const float along = (corner.x - edgeStart.x) * dx + (corner.z - edgeStart.z) * dz;
        const float lengthSquared = dx * dx + dz * dz;
        if (along > lengthSquared) {
            candidate.z = ((dz + edgeStart.z) - corner.z) + candidate.z;
            candidate.x = ((dx + edgeStart.x) - corner.x) + candidate.x;
        } else if (0.0f > along) {
            candidate.z = (edgeStart.z - corner.z) + candidate.z;
            candidate.x = (edgeStart.x - corner.x) + candidate.x;
        } else {
            const float scale = along / lengthSquared;
            candidate.x = ((scale * dx + edgeStart.x) - corner.x) + candidate.x;
            candidate.z = ((scale * dz + edgeStart.z) - corner.z) + candidate.z;
        }
        side = hitSide;
        return true;
    }
    return false;
}

void UpdateRowAttachmentTarget(FBGGWork &work, const Vector3 &position, std::int32_t row, std::int32_t side) {
    const std::int32_t heading = g_fbggRideHeading;
    const float sine = sin_fixed16(static_cast<std::int16_t>(heading));
    const float reach = g_fbggGauge + 1.153f;
    const float cosine = CosineFixed16(heading);
    const Vector3 probe{position.x - reach * sine, 0.0f, position.z - cosine * reach};
    std::int32_t bestRow = -1;
    float bestMetric = 0.0f;
    row = FindRowContainingPoint(row, probe, bestRow, bestMetric);
    if (row == -1)
        row = bestRow;
    const float t = ProjectPointOntoSegmentPairXZ(row, probe);
    Vector3 currentLeft{};
    Vector3 currentRight{};
    Vector3 nextLeft{};
    Vector3 nextRight{};
    GetPrevCurrNextSample(row, currentLeft, currentRight, nextLeft, nextRight);
    const Vector3 &edgeStart = side ? currentRight : currentLeft;
    const Vector3 &edgeEnd = side ? nextRight : nextLeft;
    work.roadEdgeContactPosition.z = (edgeEnd.z - edgeStart.z) * t + edgeStart.z;
    work.roadEdgeContactPosition.x = (edgeEnd.x - edgeStart.x) * t + edgeStart.x;
    SampleRowHeightChain(row, t, work.roadEdgeContactPosition);
    work.roadEdgeContactPosition.y += 0.80000001f;
}


void UpdateRowFollowerPose(FBGGWork &work) {
    Character *const bike = g_pTempFBGGChara;
    const Vector3 bikePosition = bike ? bike->entity.node.position : Vector3{};
    const auto linearHeight = [](std::int32_t row, float t) {
        Vector3 currentLeft{};
        Vector3 currentRight{};
        Vector3 nextLeft{};
        Vector3 nextRight{};
        GetPrevCurrNextSample(row, currentLeft, currentRight, nextLeft, nextRight);
        return ((((nextRight.y + nextLeft.y) - currentLeft.y) - currentRight.y) * 0.5f) * t + (currentRight.y + currentLeft.y) * 0.5f;
    };
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(bikePosition.x, bikePosition.y, bikePosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(g_fbggRideHeading));
    Vector3 probe{};
    Matrix4x4::TransformPoint({0.0f, 0.0f, -6.0f}, probe);

    std::int32_t bestRow = -1;
    float bestMetric = 0.0f;
    std::int32_t row = FindRowContainingPoint(-1, probe, bestRow, bestMetric);
    if (row == -1)
        row = bestRow;
    const float t = ProjectPointOntoSegmentPairXZ(row, probe);
    const float baseHeight = linearHeight(row, t);
    const std::int32_t rowHeading = GetRowHeadingAngle(row);
    const std::int32_t rowPitch = RadiansToFixed16(GetRowPitchFromHeights(row, t));
    workTransformMatrix = transformMatrix_prePass;
    const float headingCosine = CosineFixed16(rowHeading);
    const float headingSine = sin_fixed16(static_cast<std::int16_t>(rowHeading));
    Matrix4x4::RotateAroundY(headingSine, headingCosine);
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rowPitch));
    Vector3 step{};
    Matrix4x4::TransformPoint({0.0f, 0.0f, -5.0f}, step);
    const Vector3 five{step.x + probe.x, step.y, step.z + probe.z};
    const Vector3 ten{step.x * 2.0f + probe.x, 0.0f, step.z * 2.0f + probe.z};

    std::int32_t tenBest = -1;
    std::int32_t fiveBest = -1;
    std::int32_t tenRow = FindRowContainingPoint(-1, ten, tenBest, bestMetric);
    std::int32_t fiveRow = FindRowContainingPoint(row, five, fiveBest, bestMetric);
    if (tenRow == -1)
        tenRow = tenBest;
    if (fiveRow == -1)
        fiveRow = fiveBest;
    const float tenHeight = linearHeight(tenRow, ProjectPointOntoSegmentPairXZ(tenRow, ten));
    const float fiveHeight = linearHeight(fiveRow, ProjectPointOntoSegmentPairXZ(fiveRow, five));
    const bool useTen = tenHeight - baseHeight > (fiveHeight - baseHeight) + (fiveHeight - baseHeight);
    const Vector3 &target = useTen ? ten : five;
    const float targetHeight = useTen ? tenHeight : fiveHeight;
    const float dx = target.x - probe.x;
    const float dz = target.z - probe.z;
    const std::int32_t pitch = atan2_fixed16(targetHeight - baseHeight, std::sqrt(dx * dx + dz * dz));
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY(headingSine, headingCosine);
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(pitch));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(g_fbggRideHeading - rowHeading));
    work.lienProxyRotation = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
    work.lienProxyPosition = {probe.x, baseHeight + 0.15000001f, probe.z};
    Matrix4x4::Pop(1);
}

void ApplyNodeTransform(const Mt5RuntimeNode &node) {
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    Matrix4x4::Scale(node.scale.x, node.scale.y, node.scale.z);
    if (node.rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotation.z));
    if (node.rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotation.y));
    if (node.rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotation.x));
}

void Recursive_TranslateScaleRotate(const Mt5RuntimeNode &node) {
    if (node.parentOffset)
        Recursive_TranslateScaleRotate(*reinterpret_cast<const Mt5RuntimeNode *>(MT5_ModelBaseForNode(&node) + node.parentOffset));
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    if (node.scale.x != 1.0f || node.scale.y != 1.0f || node.scale.z != 1.0f)
        Matrix4x4::Scale(node.scale.x, node.scale.y, node.scale.z);
    if (node.rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotation.z));
    if (node.rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotation.y));
    if (node.rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotation.x));
}

void Recursive_TranslateScaleRotate_LocalToWorld(const Mt5RuntimeNode &node, Vector3 &output) {
    output = {};
    Matrix4x4::SetActiveTransformToPrePass();
    Recursive_TranslateScaleRotate(node);
    Matrix4x4::TransformPoint(output, output);
    Matrix4x4::Pop(1);
}

// Returns 0 once the three rider anchor nodes are captured, else 1 (2 when the model also lacks node 10).
std::int32_t FBGG_CaptureModelNodePositions(Mt5RuntimeNode *model, FBGGWork &work) {
    const std::int16_t nodes[3] = {22, 17, 15};
    Vector3 *const outputs[3] = {&work.modelNode22Position, &work.modelNode17Position, &work.modelNode15Position};
    for (std::size_t index = 0; index < 3; ++index) {
        Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, nodes[index]);
        if (!node)
            return (shenGetNodeByChildIdx(model, 10) == nullptr) + 1;
        Recursive_TranslateScaleRotate_LocalToWorld(*node, *outputs[index]);
    }
    return 0;
}

void ResetCollisionState(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(CCOW));
    if (!component)
        return;
    auto *const collision = reinterpret_cast<CharacterCollisionWork *>(component + 1);
    collision->stateFlags &= ~7U;
    collision->stateByte = 0;
}

float MaxScalar(float left, float right) {
    return left > right ? left : right;
}

float MinScalar(float left, float right) {
    return left < right ? left : right;
}


float ComputeLeanAngleFromTurnDemand(float demand) {
    const float demandSquared = demand * demand;
    const float denominator = demandSquared + 1.329409f;
    const float minimumCosine = std::sin(kHalfPi - static_cast<float>(1.1780972480773926));
    const float cosine = MaxScalar(minimumCosine, (std::sqrt(0.1647801f - (0.39250231f - demandSquared) * denominator) - 0.40593114f) / denominator);
    std::int32_t angle = 0;
    if (std::fabs(cosine) > 1.0f)
        angle = cosine > 0.0f ? 0 : 0x8000;
    else
        angle = static_cast<std::uint16_t>(0x4000 - asin_fixed16(cosine));
    return static_cast<float>(static_cast<double>(static_cast<float>(angle)) / kRadiansToFixed16);
}

void MOTN_UpdateRowFollowerController(Character *rider, MotionInformationWork *motion, FBGGWork *work, const InputSlot *input) {
    Character *const bike = g_pTempFBGGChara;
    float lateral = 0.0f;
    std::int32_t heading = 0;
    float gauge = 0.0f;
    if (work->initializeRequested) {
        work->initializeRequested = 0;
        if (g_entryPoint != 2U)
            ++g_playerStats.pages.motorcycleRaceAttemptCount;
        motion->blendMode = 0;
        if (bike)
            bike->entity.node.position = {-3.0f, 0.0f, -2.0f};
        if (rider)
            rider->entity.node.position = {-3.0f, 0.0f, -2.0f};
        g_fbggSmoothedPosition = {-3.0f, 0.0f, -2.0f};
        if (rider)
            rider->entity.node.rotation = {};
        if (bike)
            bike->entity.node.rotation = {};
        g_fbggSmoothedEuler = {};
        gauge = 0.37099999f;
        g_fbggRideHeading = 0;
        g_fbggLeanAngle = 0.0f;
        work->rowIndex = 0;
        work->steeringAngle = 0.0f;
        work->driftAccumulator = 0;
        work->weaveAccumulator = 0;
        work->steeringVelocity = 0.0f;
        work->previousSteeringTarget = 0.0f;
        work->previousSteeringInput = 0x80;
        g_vehicleRideFrameCount = 0;
    } else {
        heading = g_fbggRideHeading;
        gauge = g_fbggGauge;
    }
    float steeringAngle = work->steeringAngle;
    std::int32_t row = work->rowIndex;
    const float velocity = work->steeringVelocity;
    const float previousTarget = work->previousSteeringTarget;
    std::int32_t drift = work->driftAccumulator;
    std::int32_t weave = work->weaveAccumulator;
    g_fbggRows = work->rows;
    g_turnYawOffsetActive = 1;
    const std::int32_t previousHeading = heading;
    g_turnYawOffsetFromAccum = heading;
    g_ctrlLookAtEnabled = 0;
    const Vector3 startPosition = bike ? bike->entity.node.position : Vector3{};
    std::int32_t bestRow = -1;
    float bestMetric = 0.0f;
    row = FindRowContainingPoint(row, startPosition, bestRow, bestMetric);
    float rowT = ProjectPointOntoSegmentPairXZ(row != -1 ? row : bestRow, startPosition);
    float tangent = GetRowRelativeHeadingTangent(row);

    if ((input->held & 0x80U) != 0) {
        gauge += (0.0040000002f / (static_cast<float>(work->speedTier) * 0.5f + 1.0f)) * 0.80000001f;
        g_fbggGauge = gauge;
        if (gauge > 1.4809999f) {
            g_fbggGauge = 1.4809999f;
            gauge = 1.4809999f;
        }
    } else {
        gauge += gauge > 1.02f ? -0.0060000001f : -0.003f;
        g_fbggGauge = gauge;
    }
    std::uint32_t brakeCommand = kAicaEnvSetVolume;
    std::int32_t brakeVolume = 0;
    if ((input->held & 0x40U) != 0) {
        g_fbggGauge = gauge + -0.0099999998f;
        shenExecuteAICACommand(kAicaEnvSetVolume, work->brakeSoundBank, 127);
        brakeCommand = 0xa6U;
        brakeVolume = static_cast<std::uint8_t>(input->triggers >> 8) >> 1;
    }
    shenExecuteAICACommand(brakeCommand, work->brakeSoundBank, brakeVolume);
    if (LampComponentPayload *const lamp = LAMP_GetWork(g_pTempFBGGChara))
        lamp->glowLevel = static_cast<std::uint8_t>(g_inputState.slots[0].triggers >> 8);
    gauge = g_fbggGauge;
    bool stalled = false;
    if (0.37099999f > gauge) {
        gauge = 0.37099999f;
        g_fbggGauge = gauge;
    } else if (0.001f > gauge) {
        gauge = 0.0f;
        stalled = true;
        g_fbggGauge = gauge;
    }
    if ((input->pressedPriority & 0x80U) != 0 && 0.74000001f > gauge) {
        shenExecuteAICACommand(0x304a9U, 0, 0);
        gauge = g_fbggGauge;
    }

    const float maximumVelocity = (gauge - 0.37f) * 0.079999998f + 0.079999998f;
    const float acceleration = (gauge - 0.37f) * 0.0049999999f + 0.015f;
    float target = 0.0f;
    if ((input->held & 0xc000U) != 0) {
        const float magnitude = (input->held & 0x2000U) != 0 ? 0.78539819f : 0.62831855f;
        target = static_cast<std::int16_t>(input->held) < 0 ? -magnitude : magnitude;
    } else if (static_cast<std::uint8_t>(input->rightStick) != 0x80U) {
        const std::int32_t average = (static_cast<std::uint8_t>(input->rightStick) + static_cast<std::int32_t>(work->previousSteeringInput)) / 2;
        target = static_cast<float>(128 - average) * 0.78539819f * 0.0078125f;
    }
    work->previousSteeringInput = static_cast<std::uint8_t>(input->rightStick);
    const float targetDelta = target - steeringAngle;
    float newVelocity = 0.0f;
    if (std::fabs(velocity) < 0.01f) {
        if (acceleration * 3.0f > std::fabs(targetDelta)) {
            steeringAngle += targetDelta * 0.80000001f;
        } else {
            newVelocity = 0.0f > targetDelta ? -acceleration : acceleration;
            steeringAngle += newVelocity;
        }
    } else {
        float candidateVelocity = 0.0f;
        if (0.0f > velocity) {
            if (target > steeringAngle) {
                candidateVelocity = velocity + (acceleration + acceleration);
            } else {
                const double stop = (static_cast<double>(std::fabs(velocity)) / static_cast<double>(acceleration) + 1.0) * static_cast<double>(velocity) * 0.5;
                candidateVelocity = target > static_cast<float>(stop) + steeringAngle ? velocity + acceleration : velocity - acceleration;
            }
            newVelocity = MaxScalar(-maximumVelocity, candidateVelocity);
        } else {
            if (steeringAngle > target) {
                candidateVelocity = velocity - (acceleration + acceleration);
            } else {
                const float stop = ((velocity / acceleration + 1.0f) * velocity) * 0.5f + steeringAngle;
                candidateVelocity = stop > target ? velocity - acceleration : velocity + acceleration;
            }
            newVelocity = MinScalar(maximumVelocity, candidateVelocity);
        }
        const float previousRemaining = previousTarget - steeringAngle;
        steeringAngle = candidateVelocity + steeringAngle;
        if (0.0f > (target - steeringAngle) * previousRemaining) {
            if (acceleration + acceleration > std::fabs(newVelocity))
                newVelocity = 0.0f;
            else
                newVelocity += newVelocity > 0.0f ? -acceleration : acceleration;
        }
    }

    bool lateralActive = true;
    if (0.92592502f > gauge) {
        shenExecuteAICACommand(kAicaEnvSetVolume, work->driftSoundBank, 0);
        drift = drift > 16 ? drift - 16 : drift < -16 ? drift + 16 : 0;
    } else if (!(0.62831855f > std::fabs(steeringAngle)) && !(0.0f > target * steeringAngle) && (input->held & 0x80U) != 0) {
        const float intensity =
            ((gauge - 0.92592502f) / 0.57407498f * 0.69999999f + 0.30000001f) * (static_cast<float>(drift) * 0.00390625f * 0.80000001f + 0.2f) * 127.0f;
        const std::int32_t driftPitch = static_cast<std::int32_t>(std::fabs(static_cast<double>(intensity)));
        shenExecuteAICACommand(kAicaEnvSetVolume, work->driftSoundBank, 127);
        shenExecuteAICACommand(0xa6U, work->driftSoundBank, driftPitch);
        const std::int32_t triggerStep = static_cast<std::uint8_t>(input->triggers) >> 5;
        if (0.0f > steeringAngle && drift > -249)
            drift -= drift > 0 ? 16 : triggerStep;
        else if (steeringAngle > 0.0f && drift < 249)
            drift += drift < 0 ? 16 : triggerStep;
    } else {
        shenExecuteAICACommand(kAicaEnvSetVolume, work->driftSoundBank, 0);
        const std::int32_t decay = 0.62831855f > std::fabs(steeringAngle) ? 16 : 2;
        if (drift > 0)
            drift = drift - decay < 0 ? 0 : drift - decay;
        else if (drift < 0)
            drift = drift + decay > 0 ? 0 : drift + decay;
    }
    gauge = g_fbggGauge;
    if (drift == 0)
        lateralActive = false;
    else
        lateral = ((gauge - 0.92592502f) / 0.57407498f * 0.80000001f + 0.2f) * 0.75f * (static_cast<float>(drift) * 0.00390625f);

    const double steerMagnitude = std::fabs(static_cast<double>(steeringAngle));
    if (!(0.2f > std::fabs(steeringAngle)) && 0.1f > std::fabs(targetDelta) && 0.06f > std::fabs(newVelocity) && (input->held & 0x80U) == 0) {
        if (0.0f > steeringAngle)
            weave = (weave > 0 ? 0 : weave) - 64;
        else
            weave = (weave < 0 ? 0 : weave) + 64;
    } else if (weave > 0) {
        weave -= 64;
    } else if (weave < 0) {
        weave += 64;
    }
    const std::int32_t clampedWeave = weave > 256 ? 256 : weave < -256 ? -256 : weave;

    float lean = 0.0f;
    if (!(0.01f > std::fabs(target)) && !stalled) {
        const float byGauge = static_cast<float>(static_cast<double>(gauge) / 0.07);
        const float byTurn = gauge * gauge / (std::tan(std::fabs(target)) * 0.0108888904f);
        lean = ComputeLeanAngleFromTurnDemand(MaxScalar(byGauge, byTurn));
        if (0.0f > steeringAngle)
            lean = -lean;
    }
    bool straight = false;
    if (0.0001 > std::fabs(static_cast<double>(lean))) {
        lean = 0.0f;
        straight = true;
    }
    lean *= 2.0f;
    g_fbggLeanAngle = lean;
    if (-1.4f > lean) {
        g_fbggLeanAngle = -1.4f;
        lean = -1.4f;
    } else if (lean > 1.4f) {
        g_fbggLeanAngle = 1.4f;
        lean = 1.4f;
    }

    std::int32_t rideHeading = g_fbggRideHeading;
    const float headingSine = sin_fixed16(static_cast<std::int16_t>(rideHeading));
    const float headingCosine = CosineFixed16(rideHeading);
    const float leanSine = std::sin(lean);
    const float leanCosine = std::sin(kHalfPi - std::fabs(lean));
    Vector3 position = bike ? bike->entity.node.position : Vector3{};
    float turnAngle = 0.0f;
    std::int32_t steer = 0;
    if (stalled) {
        steer = 0;
    } else if (straight) {
        if (lateralActive) {
            position.z = position.z - (headingCosine * gauge + headingSine * lateral);
            position.x = position.x + (headingCosine * lateral - headingSine * gauge);
        } else {
            position.z = position.z - headingCosine * gauge;
            position.x = position.x - headingSine * gauge;
        }
        steer = RadiansToFixed16(steeringAngle);
    } else {
        const float weaveSine = sin_fixed16(static_cast<std::int16_t>(clampedWeave << 6));
        const float arc = (1.0f - leanCosine) * 0.35206521f + leanCosine;
        float centreX = 0.0f;
        float centreZ = 0.0f;
        float rimX = 0.0f;
        float rimZ = 0.0f;
        if (lean > 0.0f) {
            const float radius = (1.0f - weaveSine * 0.30000001f) * (arc / leanSine * 1.7795f);
            turnAngle = gauge / radius;
            centreZ = -(headingSine * radius);
            centreX = headingCosine * radius;
            rimX = centreX;
            rimZ = centreZ;
            if (lateralActive) {
                const float rim = radius + lateral;
                rimX = rim * headingCosine;
                rimZ = -(rim * headingSine);
            }
        } else {
            const float radius = (weaveSine * 0.30000001f + 1.0f) * (arc / -leanSine * 1.7795f);
            turnAngle = -(gauge / radius);
            centreX = -(headingCosine * radius);
            centreZ = headingSine * radius;
            rimX = centreX;
            rimZ = centreZ;
            if (lateralActive) {
                const float rim = radius - lateral;
                rimZ = rim * headingSine;
                rimX = -(rim * headingCosine);
            }
        }
        rideHeading = AddFixed16(rideHeading, RadiansToFixed16(turnAngle));
        g_fbggRideHeading = rideHeading;
        const float turnSine = std::sin(turnAngle);
        const float turnCosine = std::sin(kHalfPi - std::fabs(turnAngle));
        position.x = position.x + ((rimX * turnCosine + rimZ * turnSine) - centreX);
        position.z = position.z + ((rimZ * turnCosine - rimX * turnSine) - centreZ);
        steer = RadiansToFixed16(steeringAngle);
    }

    float clampedTangent = tangent;
    if (-1.7320508f > tangent) {
        clampedTangent = -1.7320508f;
        g_fbggRideHeading = AddFixed16(rideHeading, 0xd556 - AtanToFixed16(tangent));
    } else if (tangent > 1.7320508f) {
        clampedTangent = 1.7320508f;
        g_fbggRideHeading = AddFixed16(rideHeading, 0x2aaa - AtanToFixed16(tangent));
    }
    work->roadEdgeContact = 0;
    bool snapped = false;
    std::int32_t side = 0;
    if (!stalled && row != -1) {
        const Vector3 previousPosition = bike ? bike->entity.node.position : Vector3{};
        snapped = SnapPointToRowQuad(row, position, previousPosition, previousHeading, steer, side);
    }
    if (snapped) {
        if (gauge > 0.37037f)
            g_fbggGauge = gauge - work->gaugeDecay;
        if (row < 142 || (row == 142 && 0.8 > static_cast<double>(rowT))) {
            shenExecuteAICACommand(kAicaEnvSetVolume, work->edgeScrapeSoundBank, 127);
            shenExecuteAICACommand(kAicaEnvSetVolume, work->edgeScrapeLateSoundBank, 0);
            shenExecuteAICACommand(0xa6U, work->edgeScrapeSoundBank, static_cast<std::int32_t>((g_fbggGauge - 0.37f) * 112.389f));
        } else {
            shenExecuteAICACommand(kAicaEnvSetVolume, work->edgeScrapeSoundBank, 0);
            shenExecuteAICACommand(kAicaEnvSetVolume, work->edgeScrapeLateSoundBank, 127);
            shenExecuteAICACommand(0xa6U, work->edgeScrapeLateSoundBank, static_cast<std::int32_t>((g_fbggGauge - 0.37f) * 56.637001f + 64.0f));
        }
        if (std::fabs(clampedTangent) > 0.1f)
            g_fbggRideHeading = AddFixed16(g_fbggRideHeading, -AtanToFixed16(clampedTangent * 0.2f));
        work->roadEdgeContact = 1;
        UpdateRowAttachmentTarget(*work, position, row, side);
        if (work->roadEdgeContactDebounce == 0) {
            ++g_playerStats.pages.motorcycleRoadEdgeHitCount;
            ++work->roadEdgeContactDebounce;
        } else if (work->roadEdgeContactDebounce < 5) {
            ++work->roadEdgeContactDebounce;
        }
    } else {
        shenExecuteAICACommand(kAicaEnvSetVolume, work->edgeScrapeSoundBank, 0);
        shenExecuteAICACommand(kAicaEnvSetVolume, work->edgeScrapeLateSoundBank, 0);
        if (--work->roadEdgeContactDebounce <= 0)
            work->roadEdgeContactDebounce = 0;
    }

    const std::int32_t frameMode = g_updateRenderFrameMode;
    const float smoothing = frameMode != 0 ? 1.1875f : 0.0625f;
    if (frameMode >= 0) {
        g_fbggSmoothedPosition.x = g_fbggSmoothedPosition.x + (position.x - g_fbggSmoothedPosition.x) * smoothing;
        g_fbggSmoothedPosition.y = g_fbggSmoothedPosition.y + (position.y - g_fbggSmoothedPosition.y) * smoothing;
        g_fbggSmoothedPosition.z = g_fbggSmoothedPosition.z + (position.z - g_fbggSmoothedPosition.z) * smoothing;
        position = g_fbggSmoothedPosition;
    }
    row = FindRowContainingPoint(row, position, bestRow, bestMetric);
    rowT = ProjectPointOntoSegmentPairXZ(row, position);
    SampleRowHeightChain(row, rowT, position);
    position.y += 0.050000001f;
    g_motnCameraFollowAngle = RadiansToFixed16(turnAngle);
    Vector3i euler{ComputeRowFollowerPitch(position, -1.153f, 0.62650001f, row, rowT, clampedTangent), g_fbggRideHeading, steer};
    if (frameMode >= 0) {
        const auto smooth = [smoothing](std::int32_t current, std::int32_t angle) {
            const std::int32_t step = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(angle) - current) * smoothing);
            return static_cast<std::int32_t>(static_cast<std::int16_t>(current + step));
        };
        g_fbggSmoothedEuler.x = smooth(g_fbggSmoothedEuler.x, euler.x);
        g_fbggSmoothedEuler.y = smooth(g_fbggSmoothedEuler.y, euler.y);
        g_fbggSmoothedEuler.z = smooth(g_fbggSmoothedEuler.z, euler.z);
        euler = g_fbggSmoothedEuler;
    }

    Matrix4x4::SetActiveTransformToPrePass();
    const float yawCosine = CosineFixed16(euler.y);
    const float yawSine = sin_fixed16(static_cast<std::int16_t>(euler.y));
    Matrix4x4::RotateAroundY(yawSine, yawCosine);
    const float pitchCosine = CosineFixed16(euler.x);
    const float pitchSine = sin_fixed16(static_cast<std::int16_t>(euler.x));
    Matrix4x4::RotateAroundX(pitchSine, pitchCosine);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(euler.z));
    const Vector3i bikeRotation = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
    if (bike)
        bike->entity.node.rotation = bikeRotation;
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY(yawSine, yawCosine);
    Matrix4x4::RotateAroundX(pitchSine, pitchCosine);
    const Vector3i riderRotation = CHAR_EulerAnglesFromMatrix(workTransformMatrix);
    if (rider)
        rider->entity.node.rotation = riderRotation;
    work->shadowProxyRotation = riderRotation;
    const float steerSine = std::sin(steeringAngle);
    Vector3 offset{};
    Matrix4x4::TransformPoint({steerSine * -1.08000004f, 0.0f, 0.2f}, offset);
    if (rider)
        rider->entity.node.position = {offset.x + position.x, offset.y + position.y, offset.z + position.z};
    const float bobGauge = g_fbggGauge;
    Matrix4x4::TransformPoint({lateral - steerSine * 0.30000001f, 0.0f, -bobGauge}, offset);
    work->shadowProxyPosition = {offset.x + position.x, offset.y + position.y, offset.z + position.z};
    Matrix4x4::Pop(1);
    const std::uint32_t bobPhase = static_cast<std::uint32_t>(work->speedTier + 1) * g_vehicleRideFrameCount << 13;
    position.y = position.y + sin_fixed16(static_cast<std::int16_t>(bobPhase)) * (bobGauge * 0.0040000002f);
    if (bike)
        bike->entity.node.position = position;
    Mt5RuntimeNode *const bikeModel = g_fbggBikeModel;
    if (Mt5RuntimeNode *const leanNode = shenGetNodeByChildIdx(bikeModel, 14))
        leanNode->rotation.y = RadiansToFixed16(g_fbggLeanAngle);

    std::uint16_t animation = 21014;
    if (!(0.01f > std::fabs(steeringAngle))) {
        if (0.0f > steeringAngle)
            motion->motionFlags |= 0x80U;
        else
            motion->motionFlags &= ~0x80U;
        animation = 21015;
        motion->forcedFrameTime = static_cast<float>(95.0 - steerMagnitude * 58.569000244140625);
    }
    motion->motionFlags &= ~2U;
    motion->pendingAnimId = animation;
    AdvanceRowPointForward(row, rowT, position);
    position.y = (rider ? rider->entity.node.position.y : 0.0f) - 20.0f;
    MOTM_SetTarget(rider, 6, &position, 0x40000000U);
    const Vector3 pedalTranslation{steerSine * 1.08000004f, -0.039999999f, -0.2f};
    if (BuildMOTNNodeWorldTransforms(bikeModel, pedalTranslation, {0, 0, euler.z}, {}, 0.0f)) {
        MOTM_SetTarget(rider, 17, &g_vehicleRightAnkleTarget, 0x01000000U);
        MOTM_SetTarget(rider, 11, &g_vehicleLeftAnkleTarget, 0x01000000U);
    }

    if (g_fbggSpeedTierThresholds[work->speedTier][1] > bobGauge)
        work->speedTier = work->speedTier - 1 < 0 ? 0 : work->speedTier - 1;
    if (bobGauge > g_fbggSpeedTierThresholds[work->speedTier][0])
        work->speedTier = work->speedTier + 1 > 2 ? 2 : work->speedTier + 1;
    const std::int32_t enginePitch = static_cast<std::int32_t>(bobGauge * 127.0f / g_fbggSpeedTierThresholds[work->speedTier][0]);
    std::int32_t engineEffect = 63;
    if (row < 142 || (row == 142 && 0.8 > static_cast<double>(rowT))) {
        shenExecuteAICACommand(kAicaEnvSetVolume, work->engineSoundBank, 127);
        engineEffect = -64;
    } else {
        shenExecuteAICACommand(kAicaEnvSetVolume, work->engineSoundBank, 110);
    }
    shenExecuteAICACommand(0x40a5U, work->engineSoundBank, engineEffect);
    shenExecuteAICACommand(0xa6U, work->engineSoundBank, enginePitch);
    UpdateRowFollowerPose(*work);
    ++g_vehicleRideFrameCount;
    work->previousSteeringTarget = target;
    work->steeringAngle = steeringAngle;
    work->steeringVelocity = newVelocity;
    work->rowIndex = row;
    work->driftAccumulator = drift;
    work->weaveAccumulator = clampedWeave;
    if (g_entryPoint != 2U)
        ++g_playerStats.pages.motorcycleRideFrameCountTotal;
}

} // namespace

FBGGWork *FBGG_GetWork(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kFBGGComponent);
    return component ? reinterpret_cast<FBGGWork *>(component + 1) : nullptr;
}

std::int32_t BuildMOTNNodeWorldTransforms(Mt5RuntimeNode *root, const Vector3 &translation, const Vector3i &euler, const Vector3 &offset, float pitchDegrees) {
    const Vector3 ankleOffset{0.0f, 0.050000001f, 0.0f};
    std::int32_t built = 0;
    g_vehicleRightAnkleTarget = {};
    g_vehicleLeftAnkleTarget = {};
    if (!root)
        return 0;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(translation.x, translation.y, translation.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(euler.y));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(euler.z));
    const std::int32_t pitch = static_cast<std::uint16_t>(static_cast<std::int32_t>(pitchDegrees * 8192.0f / 45.0f));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(euler.x - pitch));
    Matrix4x4::preTranslate(offset.x, offset.y, offset.z);
    ApplyNodeTransform(*root);
    if (Mt5RuntimeNode *const lean = shenGetNodeByChildIdx(root, 14)) {
        if (lean->parentOffset) {
            const auto *const parent = reinterpret_cast<const Mt5RuntimeNode *>(MT5_ModelBaseForNode(lean) + lean->parentOffset);
            if (parent != root)
                ApplyNodeTransform(*parent);
        }
        ApplyNodeTransform(*lean);
        if (Mt5RuntimeNode *const rightPedal = shenGetNodeByChildIdx(root, 8)) {
            built = 1;
            Matrix4x4_Push();
            ApplyNodeTransform(*rightPedal);
            Matrix4x4::TransformPoint(ankleOffset, g_vehicleRightAnkleTarget);
            Matrix4x4::Pop(1);
        }
        if (Mt5RuntimeNode *const leftPedal = shenGetNodeByChildIdx(root, 13)) {
            built = 1;
            ApplyNodeTransform(*leftPedal);
            Matrix4x4::TransformPoint(ankleOffset, g_vehicleLeftAnkleTarget);
        }
    }
    Matrix4x4::Pop(1);
    return built;
}

void LoadAndSetupFBGG(Character *character) {
    const std::array<const char *, 8> sounds{nullptr, nullptr, "bgm076.snd", "e1dobmin.snd", nullptr, nullptr, nullptr, nullptr};
    auto *const work = static_cast<FBGGWork *>(AllocateHeapMemoryMODL(character, kFBGGComponent, sizeof(FBGGWork), nullptr));
    if (character)
        HLib_GetCharacterComponent(character, kFBGGComponent)->drawCallback = FBGG_Callback;
    work->riderEngaged = 0;
    work->speedTier = 0;
    work->reserved00c = 0;
    work->reserved010 = 0;
    auto *const model = static_cast<Mt5RuntimeNode *>(character->model);
    if (!model || FBGG_CaptureModelNodePositions(model, *work))
        return;
    work->previousSteeringInput = 0x80;
    work->rows = static_cast<const FBGGRowSample *>(ReadFile("scene/03/FBGG", "COLI.BIN", SHEN_NAME(COLI)));
    work->initializeRequested = 0;
    work->exitRequested = 0;
    work->gaugeDecayTier = 0;
    work->gaugeDecay = 0.050000001f;
    work->rowIndex = 0;
    work->steeringAngle = 0.0f;
    work->steeringVelocity = 0.0f;
    work->previousSteeringTarget = 0.0f;
    work->driftAccumulator = 0;
    work->weaveAccumulator = 0;
    work->roadEdgeContactDebounce = 0;
    SND_LoadSND(sounds);
    shenExecuteAICACommand(0x4ea8U, 0, 0);
    shenExecuteAICACommand(0x04a0U, 2, 110);
    work->engineSoundBank = ExecuteAICACommand_Wrapper(0x103U);
    work->brakeSoundBank = ExecuteAICACommand_Wrapper(0x105U);
    work->driftSoundBank = ExecuteAICACommand_Wrapper(0x104U);
    work->edgeScrapeSoundBank = ExecuteAICACommand_Wrapper(0x106U);
    work->edgeScrapeLateSoundBank = ExecuteAICACommand_Wrapper(0x107U);
    work->engineSoundBank = ExecuteAICACommand_Wrapper(0x103U);
    LGHT_SetLightIntensityRadius(400.0f);
    g_pTempFBGGChara = character;
    useAnalogInput = 0;
    g_pControlPadController->controlMode = kFollowerControlMode;
    UI_SetConversationPromptSceneEnabled(false);
}

void FBGG_Callback(Character *character, void *rawWork) {
    auto *const work = static_cast<FBGGWork *>(rawWork);
    g_vehicleGlobalFlag = g_vehicleGlobalFlagGate ? g_vehicleGlobalFlag : 0;
    if (work->riderEngaged || g_pControlPadController->controlMode == kFollowerControlMode)
        return;
    g_inputState.slots[0].pressedPriority = 0;
    g_fbggBikeModel = static_cast<Mt5RuntimeNode *>(character->model);
    g_pTempFBGGChara = character;
    work->riderEngaged = 1;
    g_fbggRiderMotionState = kRiderMotionEngaged;
}

void ryoUpdateFollower(Character *rider, const InputSlot *input) {
    MotionManagerWork *const manager = MOTM_GetWork(rider);
    MotionInformationWork *const motion = manager ? &manager->motion : nullptr;
    FBGGWork *const work = FBGG_GetWork(g_pTempFBGGChara);
    if (g_fbggRiderMotionState == kRiderMotionActive) {
        if (work->exitRequested) {
            g_cameraOverrideFlipDirectionActive = 0;
            g_suppressDefaultHudPromptIcons = 1;
            g_pControlPadController->blockedCanControl = 0;
            g_pControlPadController->state = 2;
            g_tmexValHigh = 0;
            return;
        }
        g_cameraOverrideFlipDirectionActive = 1;
        MOTN_UpdateRowFollowerController(rider, motion, work, input);
        return;
    }
    motion->motionFlags &= ~2U;
    motion->blendFrameCount = 5;
    motion->pendingAnimId = 21014;
    motion->blendMode = 0;
    motion->playbackRate = 1.0f;
    g_fbggRiderMotionState = kRiderMotionActive;
    ResetCollisionState(rider);
    ResetCollisionState(g_pTempFBGGChara);
}

} // namespace shenmue::s1
