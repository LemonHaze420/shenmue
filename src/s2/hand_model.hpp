#pragma once

#include "s2/model_component_types.hpp"

namespace shenmue::s2 {

CharacterCoreCallback assignBlurFigurePoseCallback(Character *character, CharacterCoreCallback callback);

struct HandParams;

std::uint64_t PublishHandModelAttachment(HandParams &hand, Character *character, MT7Model *model, std::int32_t childrenOnly, std::uint32_t flags);
std::uint64_t StageImmediateHandPose(HandParams *hand, const Matrix4x4 *parent);
std::uint64_t StageImmediateHandPose(HandParams *hand);

std::uint32_t DrawCharacterHandReplacement(std::uint16_t nodeId, float *originalAlphaScale, float alpha, const MT7Node *originalNode = nullptr);

struct HandModelResource;
struct PresentationState {
    HandModelResource *handResources[2]{};
};

} // namespace shenmue::s2
