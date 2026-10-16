#pragma once
#include "s2/area_event.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name QDSS_RYO{SHEN_NAME(RYO_)}, QDSS_REN{SHEN_NAME(REN_)}, QDSS_BASE{SHEN_NAME(_dum)}, QDSS_HAND{SHEN_NAME(_ude)},
    QDSS_KEYS_BASE{SHEN_NAME(_kdm)}, QDSS_CAMERA_BASE{SHEN_NAME(_du2)}, QDSS_DIAGRAM{SHEN_NAME(__zu)}, QDSS_INVALID{0xffffffffU}, QDSS_EVENT{SHEN_NAME(EVNT)};
struct QdssKey {
    Vector3 position;
    Vector3i rotation;
    std::int32_t flag, item;
    shen_name character;
};
static_assert(sizeof(QdssKey) == 36);
struct QdssAvailableKey {
    std::int32_t owned, ordinal;
};
struct QdssWork {
    HLib::Task *self;
    std::uint32_t inserted;
    std::int32_t state, direction, itemCount, selected, hole, slide, remaining;
    QdssKey *keys;
    QdssAvailableKey available[8];
    std::int32_t selector;
    std::uint32_t savedSeconds;
    std::int32_t authorRemaining, nextState;
};
static_assert(sizeof(QdssWork) == 0x80);
static_assert(offsetof(QdssWork, keys) == 0x28);
static_assert(offsetof(QdssWork, selector) == 0x70);
extern QdssKey qdssKeys[8];
void PrepareQdssHands(const Vector3i &left, const Vector3i &right);
void ApplyQdssKeyGrip(shen_name character, std::int32_t side);
void QueueQdssCling(
    shen_name target, shen_name character, std::int32_t bone, const Vector3 *position, const Vector3i *rotation, std::uint32_t mode, std::int32_t delay);
void QueueQdssEye(shen_name target, std::int32_t delay);
void QueueQdssEyePart(std::int32_t delay);
void QueueQdssAward(std::uint32_t item, std::int32_t delay);
void QueueQdssProp(shen_name prop, std::uint32_t enabled, std::int32_t delay);
void QueueQdssScale(shen_name character, const Vector3 &scale, std::int32_t delay);
void QdssFade(std::int32_t frames, bool opaque, std::int32_t delay);
std::int32_t RunQdssDiscovery(QdssWork &work, std::int32_t selector, std::int32_t delay = 0);
std::int32_t RunQdssInsertion(QdssWork &work);
void QdssFinishInsertion(QdssWork &work);
bool PrepareQdssKeySelection(QdssWork &work);
std::int32_t UpdateQdssKeySelection(QdssWork &work);
void PrepareQdssHoleSelection(QdssWork &work);
std::int32_t UpdateQdssHoleSelection(QdssWork &work);
extern const ScriptAreaDefinition qdssScriptDefinition;
} // namespace shenmue::s2::areas
