#pragma once

#include "s2/conversation_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

static const std::int32_t CONVERSATION_FADE_RANGE = 200;
static const std::int8_t CONVERSATION_CAMERA_REVERB_ATTENUATION[16] = {-45, -43, -41, -39, -37, -35, -33, -31, -29, -27, -25, -23, -21, -19, -17, -15};
void QueueConversationFadeOut(ConversationBuffer &buffer, std::int32_t frames);
void ConversationFadeOutCallback(ConversationBuffer &buffer);
void UpdateConversationStream(ConversationStream &stream);
std::int32_t GetConversationPlaybackEnabled();

} // namespace s2
} // namespace shenmue

#endif
