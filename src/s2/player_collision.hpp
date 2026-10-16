#pragma once

#include "s2/player.hpp"

#include <array>
#include <cstring>

namespace shenmue::s2::player_detail {

inline constexpr std::uint8_t plymRayHitNone = 0;

inline constexpr std::uint8_t plymRayHitField = 1;

inline constexpr std::uint8_t plymRayHitCharacter = 2;



inline constexpr Vector3 plymRayForward{0.0f, 0.0f, -10.0f};

inline constexpr Vector3 plymRayRightEnd{0.375f, 0.0f, -10.0f};

inline constexpr Vector3 plymRayLeftEnd{-0.375f, 0.0f, -10.0f};

inline constexpr Vector3 plymRayRightStart{0.375f, 0.0f, 0.0f};

inline constexpr Vector3 plymRayLeftStart{-0.375f, 0.0f, 0.0f};

inline constexpr Vector3 plymWideRayRightEnd{0.41f, 0.0f, -10.0f};

inline constexpr Vector3 plymWideRayLeftEnd{-0.41f, 0.0f, -10.0f};

inline constexpr Vector3 plymWideRayRightStart{0.41f, 0.0f, 0.0f};

inline constexpr Vector3 plymWideRayLeftStart{-0.41f, 0.0f, 0.0f};

struct PlymRayResult {
    float distance{10.0f};
    std::uint8_t type{};
    Character *character{};
    Vector3 point{};
    Vector3s normal{};
};

inline constexpr std::uint32_t selectionTurnActive = 0x00000001U;

inline constexpr std::uint32_t selectionCollisionTurnCandidate = 0x00000004U;

inline constexpr std::uint32_t selectionCollisionTurnActive = 0x00000008U;

inline constexpr std::uint32_t selectionObstacleTurnCandidate = 0x00000010U;

inline constexpr std::uint32_t selectionCollisionTurnNegative = 0x00000400U;

inline constexpr std::uint32_t selectionCollisionTurnPositive = 0x00000800U;

inline constexpr std::uint32_t selectionGroundBoundaryTurnCandidate = 0x00001000U;

inline constexpr std::uint32_t selectionGroundBoundaryTurnActive = 0x00002000U;

inline constexpr std::uint32_t selectionGroundBoundaryTracking = 0x00004000U;

inline constexpr std::uint32_t selectionSuppressCollisionSteering = 0x00008000U;

inline constexpr std::uint32_t selectionCharacterSpeedMatch = 0x00020000U;

inline constexpr std::uint32_t selectionCollisionTurnLatched = 0x00040000U;

inline constexpr std::uint32_t selectionCollisionTurnSuppressed = 0x00100000U;

inline constexpr std::uint32_t selectionTurnPositive = 0x01000000U;

inline constexpr std::uint32_t selectionTurnNegative = 0x02000000U;

inline constexpr std::uint32_t selectionTurnDirectionReversed = 0x04000000U;

inline constexpr std::uint32_t selectionImmediateCharacterCollision = 0x10000000U;

inline constexpr std::uint32_t selectionForcedYaw = 0x40000000U;

inline constexpr Vector3 plymSideProbeLeft{-2.0f, 0.0f, 0.0f};

inline constexpr Vector3 plymSideProbeRight{2.0f, 0.0f, 0.0f};

} // namespace shenmue::s2::player_detail
