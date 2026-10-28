#pragma once

#include "s2/model_component.hpp"
#include "s2/hand.hpp"

namespace shenmue::s2::model_component_detail {

struct CharacterControlGatherContext {
    std::span<CharacterControlModel *> controls;
    std::size_t count{};
    bool full{};
};

inline constexpr shen_name kCharacterControlToken{SHEN_NAME(CCM_)};

inline constexpr std::uint32_t kCharacterControlInitialFlags = CCMF_Attached | CCMF_UpdateHeight | CCMF_ApplyGravity |
    CCMF_EvaluateMotion | CCMF_VoxelSweep | CCMF_Unknown10 | CCMF_SnapToGround | CCMF_FineRaycast;

inline constexpr std::uint32_t kCharacterControlRootFlags = CCMF_RootOwner | CCMF_AlternateProbe | CCMF_FineCollision;

inline constexpr std::uint32_t kCharacterControlTransientFlags = 0x0018a400U;

inline constexpr std::uint32_t kCharacterControlGatherClearFlags = 0x10000400U;

inline constexpr float kCharacterControlEpsilon = 0.00000011920929f;

inline constexpr float kNoSceneIntersection = -3.4028235e38f;

inline constexpr std::uint32_t kCharacterControlPairImmovableFlags = CCMF_StairMode | CCMF_Immovable;

inline constexpr std::size_t kMaximumCharacterCollisionCount = 129;

} // namespace shenmue::s2::model_component_detail
