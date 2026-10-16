#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct ScenarioContext;
struct Mt5RuntimeNode;
struct PackModel;
struct GullSplinePayload;
struct GullSequencePayload;

using GullSequenceCallback = void (*)(GullSequencePayload *sequence);



#pragma pack(push, 1)
struct GullSequencePayload {
    GullSequenceCallback callback;
    Vector3 previousPosition;
    Vector3 nextPosition;
    std::uint8_t wingFrame;
    std::uint8_t frameDelay;
    std::uint8_t flapCooldown;
    std::uint8_t flapVariant;
    std::byte reserved24[0x10];
    Vector3 basePosition;
    float time;
    std::uint32_t reserved44;
    GullSplinePayload *spline;
};




struct GullSplinePayload {
    std::int32_t keyCount[2];
    float *keyTimes[2];
    float *keyValues[2];
    std::byte reserved28[0x18];
    float *keySlopes[3];
    float duration;
    std::uint32_t reserved5c;
};

struct GullScriptParameters {
    std::int32_t mode;
    std::int32_t positionOffsetOrMaxGulls;
    std::int32_t keyCountOrMaxSplines;
    std::int32_t splineOffset;
    std::int32_t xValuesOffset;
    std::int32_t yValuesOffset;
    std::int32_t zValuesOffset;
};
#pragma pack(pop)

static_assert(sizeof(GullSequencePayload) == 0x50);
static_assert(offsetof(GullSequencePayload, previousPosition) == 0x08);
static_assert(offsetof(GullSequencePayload, nextPosition) == 0x14);
static_assert(offsetof(GullSequencePayload, wingFrame) == 0x20);
static_assert(offsetof(GullSequencePayload, frameDelay) == 0x21);
static_assert(offsetof(GullSequencePayload, flapCooldown) == 0x22);
static_assert(offsetof(GullSequencePayload, flapVariant) == 0x23);
static_assert(offsetof(GullSequencePayload, basePosition) == 0x34);
static_assert(offsetof(GullSequencePayload, time) == 0x40);
static_assert(offsetof(GullSequencePayload, spline) == 0x48);
static_assert(sizeof(GullSplinePayload) == 0x60);
static_assert(offsetof(GullSplinePayload, keyTimes) == 0x08);
static_assert(offsetof(GullSplinePayload, keyValues) == 0x18);
static_assert(offsetof(GullSplinePayload, keySlopes) == 0x40);
static_assert(offsetof(GullSplinePayload, duration) == 0x58);
static_assert(sizeof(GullScriptParameters) == 0x1c);

std::uint32_t GULL_Enqueue(std::int32_t maxGulls, std::int32_t maxSplines);
GullSplinePayload *GULL_Create(
    const Vector3 *position, std::int32_t keyCount, GullSplinePayload *existingSpline, const float *xValues, const float *yValues, const float *zValues);
void SPLI_Alloc(GullSplinePayload *spline);
std::int32_t HermiteSpline_InterpolateFCVKey(
    float *output, float time, std::int32_t keyCount, const float *keyTimes, const float *keyValues, const float *keySlopes);
std::int32_t HermiteSpline_GetInterpolated3D(const GullSplinePayload *spline, float time, Vector3 *output);
void GULL_UpdateInternal(GullSequencePayload *sequence);
void GULL_SequenceCallback(GullSequencePayload *sequence);
void GULL_TaskCallback(HLib::TaskData *taskData);
void GULL_Destroy();
void GULL_ClearMapState();
void Script_GULLProgram_Utils(ScenarioContext *context, void *rawParams);

constexpr std::size_t kGullModelCount = 3;
constexpr std::size_t kGullFrameNodeCount = 17;

extern HLib::Task *g_pGULLTaskPtr;
extern GullSequencePayload *ActiveGULLData;
extern GullSplinePayload *g_gullSplines;
extern std::array<PackModel *, kGullModelCount> g_birdTypes;
extern std::array<void *, kGullModelCount> g_birdTypeHmdls;
extern std::array<Mt5RuntimeNode *, kGullModelCount * kGullFrameNodeCount> g_gullFrameNodes;
extern std::uint8_t CurrentGULL;
extern std::uint8_t CurrentMaxGULLs;
extern std::uint8_t curr_GULL_index;
extern std::uint8_t GULL_max;

} // namespace shenmue::s1
