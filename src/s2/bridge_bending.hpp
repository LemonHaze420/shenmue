#pragma once
#include "s2/event_runtime.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name BRIDGE_BENDING_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name BRIDGE_DUMMY{SHEN_NAME(dumm)}, BRIDGE_MODEL{SHEN_NAME(hasi)};
inline constexpr float BRIDGE_SECTION_WIDTH = 0.3714289963245392f;
inline constexpr std::int32_t BRIDGE_SECTION_PARTS[]{162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161};

struct BridgeBendingDefinition {
    float sectionWidth;
    shen_name character, model, attachedCharacter;
    std::int32_t partCount;
    std::uint32_t reserved14;
    const std::int32_t *parts;
};
static_assert(sizeof(BridgeBendingDefinition) == 0x20);
static_assert(offsetof(BridgeBendingDefinition, parts) == 0x18);
struct BridgeQteAction {
    std::uint32_t motion, recoverMotion, fallMotion, qte;
    std::int32_t camera, fallCamera;
};
static_assert(sizeof(BridgeQteAction) == 0x18);

struct BridgeBendingWork {
    std::uint32_t reserved00;
    Vector3 position;
    Vector3i rotation;
    std::int32_t phase, phaseStep, amplitude;
    shen_name attachedCharacter;
    float range, scale, tickFactor;
    std::int32_t pairCount;
    std::uint32_t reserved3c;
    BridgeBendingDefinition *definition;
    float attachedHeight;
    std::uint32_t reserved4c;
};
static_assert(sizeof(BridgeBendingWork) == 0x50);
static_assert(offsetof(BridgeBendingWork, position) == 4);
static_assert(offsetof(BridgeBendingWork, phase) == 0x1c);
static_assert(offsetof(BridgeBendingWork, amplitude) == 0x24);
static_assert(offsetof(BridgeBendingWork, definition) == 0x40);
static_assert(offsetof(BridgeBendingWork, attachedHeight) == 0x48);
void ResetBridgeSections(BridgeBendingWork *work);
void BendHasiBridge(BridgeBendingWork *work);
void BendQdh1Bridge(BridgeBendingWork *work);
void BendQdh2Bridge(BridgeBendingWork *work);
void UpdateBridgeAttachedCharacter(BridgeBendingWork *work);
void SetBridgeAttachedCharacterHeight(BridgeBendingWork *work, float height);
}
