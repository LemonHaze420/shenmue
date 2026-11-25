#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "s2/object_draw_task.hpp"

namespace shenmue { namespace s2 {

struct ObjectDrawTaskParameters {
    std::uint32_t retainedState, submissionState;
};
typedef char Layout_object_draw_dc_0[(sizeof(ObjectDrawTaskParameters) == 8) ? 1 : -1];
typedef char Layout_object_draw_dc_1[(offsetof(ObjectDrawTaskParameters, submissionState) == 4) ? 1 : -1];
static const std::int32_t kMaximumMapDrawTier = 10;
extern std::int32_t fieldInverseYaw, g_currentDrawTier;
extern float fieldCellScale, fieldFarDistance;
extern std::uint32_t g_currentFieldPointCount;
void InitializeObjectDrawNullsub();
void OBJ_Object_Callback(void *data);
void DrawWorldObjects();
std::int32_t camCtrlGetDrawTier(float distance);
void camCtrlSetupDepthScaling();
void UpdateWindForFlag();
void DrawOrdinaryMapCallback(void *data);
void DrawMapPlacementCallback(void *data);
void DrawPropObjectCallback(void *data);
HLib::Task *QueueObjectDrawTask(std::uint8_t nextFunctionIndex);

} } // namespace shenmue::s2

#endif
