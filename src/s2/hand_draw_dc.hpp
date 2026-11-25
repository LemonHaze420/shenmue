#pragma once

#include "s2/hand_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2


namespace shenmue { namespace s2 {

static const float kHandOpaqueAlpha = 0.9999989867210388f;
static const float kHandVisibleAlpha = 9.999999974752427e-07f;
static const float kHandBodyVisibleAlpha = 9.999999747378752e-06f;
typedef char Layout_hand_draw_dc_0[(offsetof(CharacterHandComponent, pose) + offsetof(HandPoseState, meshCopy) == 0x44) ? 1 : -1];
typedef char Layout_hand_draw_dc_1[(offsetof(CharacterHandComponent, pose) + offsetof(HandPoseState, alpha) == 0x764) ? 1 : -1];
typedef char Layout_hand_draw_dc_2[(offsetof(CharacterHandComponent, pose) + offsetof(HandPoseState, previousRegion) == 0x769) ? 1 : -1];
std::int32_t hndlDrawPrimaryHand(CharacterHandComponent *hand, float alpha);
std::int32_t hndlDrawSecondaryHand(CharacterHandComponent *hand, float alpha);
float hmdlGetDrawAlphaScaleEffect(const MT7Node *node, float alpha);
void DrawAlphaCharacterModelBranch(MT7Node *node, float alpha);

} } // namespace shenmue::s2

#endif
