#pragma once

#include "s2/model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

extern Matrix4x4 g_modelNodeTransformBase;
void CaptureModelNodeTransformBase();
void RestoreModelNodeTransformBase();
std::int32_t FindMT7NodeTransformMatrix(Matrix4x4 *output, MT7Node *node, const MT7Node *target);
void GetMT7NodeTransformMatrix(Matrix4x4 *output, MT7Node *root, const MT7Node *target);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ClearShadowNodeTransforms(MT7Node *node);
void BindShadowNodeTransforms(MT7Node *source, MT7Node *destination);
void PublishSharedShadowNodeTransforms(MT7Node *source, MT7Node *destination);
#endif

} } // namespace shenmue::s2

#endif
