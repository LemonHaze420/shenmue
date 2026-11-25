#pragma once

#include "s2/motion_registry_dc.hpp"
#include "common/math.hpp"
#if !SHENMUE_S2_GCC295
#include <bit>
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_rig.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#define SHENMUE_MOTION_STREAM_LAYOUT_NAME_(line) MotionStreamLayoutCheck##line
#define SHENMUE_MOTION_STREAM_LAYOUT_NAME(line) SHENMUE_MOTION_STREAM_LAYOUT_NAME_(line)
#define SHENMUE_MOTION_STREAM_LAYOUT(condition) typedef char SHENMUE_MOTION_STREAM_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {

struct MotnAnimationHeader {
    std::uint16_t retainedVersion, flags, frameCount;
    std::int16_t keyDataOffset;
};
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct MotionRigKey {
    float timeSeconds, tangentIn, tangentOut, value;
};
struct MotionRigChannel {
    MotionRigKey current, next;
    float inverseDuration;
    std::uint32_t keysRemaining, keyCount;
    const std::uint16_t *frameCursor, *keyBlock;
    float scale, bias;
};
// Motion channel fields; axis stride 0x40.
struct MotionRigAxis {
    MotionRigChannel channel;
    std::byte retainedDimension[4];
};
#endif
SHENMUE_MOTION_STREAM_LAYOUT(sizeof(MotnAnimationHeader) == 8);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotnAnimationHeader, flags) == 2);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotnAnimationHeader, frameCount) == 4);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotnAnimationHeader, keyDataOffset) == 6);
SHENMUE_MOTION_STREAM_LAYOUT(sizeof(MotionRigKey) == 0x10);
SHENMUE_MOTION_STREAM_LAYOUT(sizeof(MotionRigChannel) == 0x3c);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotionRigChannel, next) == 0x10);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotionRigChannel, inverseDuration) == 0x20);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotionRigChannel, keysRemaining) == 0x24);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotionRigChannel, keyCount) == 0x28);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotionRigChannel, frameCursor) == 0x2c);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotionRigChannel, keyBlock) == 0x30);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotionRigChannel, scale) == 0x34);
SHENMUE_MOTION_STREAM_LAYOUT(offsetof(MotionRigChannel, bias) == 0x38);
SHENMUE_MOTION_STREAM_LAYOUT(sizeof(MotionRigAxis) == 0x40);

static const float kMotionStreamFrameSeconds = 0.033333335f;
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static const std::uint32_t motionInitStream = 1, MI_Linear = 1;
static const std::uint16_t motionWideKeyCounts = 0x100;
#endif
extern float g_motionCurrentAnimTime, g_motionMaximumAnimTime, g_motionPreviousAnimTime;
extern std::uint32_t g_motionInterpolationType, g_motionInitFlags;
extern std::uint16_t g_motionAnimFlags;
extern const std::uint8_t *g_motionKeyCounts;
extern const std::uint16_t *g_motionWideKeyCounts, *g_motionKeyData;
struct MotionRigConstantTables {
    const char (*names)[4];
    const std::uint32_t *const *defaults;
    const float (*dimensions)[6];
};
extern MotionRigConstantTables g_motionConstData;
float DecodeMotnFloat(std::uint16_t value);
void UpdateAnimRate(MotionRigChannel &channel, float scale, float bias);
void ReadMotionKeyBlock(MotionRigChannel &channel);
void SkipMotionKeyBlock();
void SetMotionStreamFlags(std::int32_t channels, std::uint32_t initFlags, std::int32_t animFlags);
float SampleMotionRigChannelAdvance(MotionRigChannel &channel);
float GetMotionRigScale(std::uint16_t target, std::uint16_t source, std::int32_t dimension);
void SampleMotionStreamChannels(
    MotionRigAxis *axes, const MotnAnimationHeader *animation, float *output, std::uint32_t first, std::uint32_t count, float frame);

} // namespace s2
} // namespace shenmue
#undef SHENMUE_MOTION_STREAM_LAYOUT_NAME_
#undef SHENMUE_MOTION_STREAM_LAYOUT_NAME
#undef SHENMUE_MOTION_STREAM_LAYOUT

#endif
