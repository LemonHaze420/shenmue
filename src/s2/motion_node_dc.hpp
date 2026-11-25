#pragma once

#include "s2/character_dc.hpp"
#include "s2/model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

#define TASK_TMNM SHEN_NAME4('T', 'M', 'N', 'M')
#define MEMORY_TIMED_NODE_MOTION SHEN_NAME4('B', 'T', 'M', 'T')
typedef std::uint16_t TimedNodeMotionFlag;
enum { TNMF_Loop = 4, TNMF_Reverse = 0x20 };
struct TimedNodeMotionHeader {
    std::int16_t lastFrame, version;
    std::uint16_t speed, flags;
    std::int16_t nodeCount, retained;
};
struct TimedNodeChannel {
    std::uint16_t offset;
    std::int16_t value;
};
struct TimedNodeMotionRecord {
    std::uint16_t scaleOffset;
    std::uint16_t scale[3];
    TimedNodeChannel rotation[3], position[3];
};
struct TimedNodeMotion {
    std::uint32_t mode;
    const TimedNodeMotionHeader *data;
    MT7Node *model;
    float frame, speed, begin, end;
    std::uint16_t flags, retained;
};
typedef char Layout_motion_node_dc_0[(sizeof(TimedNodeMotionHeader) == 0x0c) ? 1 : -1];
typedef char Layout_motion_node_dc_1[(sizeof(TimedNodeMotionRecord) == 0x20) ? 1 : -1];
typedef char Layout_motion_node_dc_2[(sizeof(TimedNodeMotion) == 0x20) ? 1 : -1];
typedef char Layout_motion_node_dc_3[(offsetof(TimedNodeMotion, data) == 4) ? 1 : -1];
typedef char Layout_motion_node_dc_4[(offsetof(TimedNodeMotion, frame) == 0x0c) ? 1 : -1];
typedef char Layout_motion_node_dc_5[(offsetof(TimedNodeMotion, flags) == 0x1c) ? 1 : -1];

namespace motion_detail {
float F16ToF32(std::int16_t raw);
std::int16_t F32ToF16(float value);
} // namespace motion_detail
TimedNodeMotion *AllocateTimedNodeMotion();
TimedNodeMotion *FindTimedNodeMotion(const Character *character);
void InitializeTimedNodeMotion(TimedNodeMotion *motion, MT7Node *model, const TimedNodeMotionHeader *data);
void ConfigureScenarioNodeMotion(shen_name name, std::int32_t motionId, float begin, float end, float frame, float speed, std::int32_t flags);
void CopyScenarioNodeMotionTimes(shen_name name, float *begin, float *end, float *frame, float *speed);
std::int32_t IsScenarioNodeMotionFinished(shen_name name);
TimedNodeMotion *StopScenarioNodeMotion(shen_name name);
std::int32_t SampleTimedNode(const TimedNodeMotionHeader *data, MT7Node *node, std::int32_t frame, float fraction, std::int32_t index);
void ClampTimedNodeMotionFrame(TimedNodeMotion *motion, const TimedNodeMotionHeader *data);
bool TickTimedNodeMotion(TimedNodeMotion *motion);
void TMNT_Callback(Character *character, void *data);
void AllocateTMNM(Character *character);
void DrawTimedNodeModel(const MT7Node *node, float alpha);
void PrepareTimedNodeModel(const MT7Node *node, float alpha);
void ApplyNodeMotionFrame(const TimedNodeMotionHeader *data, MT7Node *model, float frame);

} // namespace s2
} // namespace shenmue

#endif
