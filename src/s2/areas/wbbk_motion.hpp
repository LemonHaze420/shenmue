#pragma once
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
inline constexpr shen_name BOOK_RYO{SHEN_NAME(RYO_)}, BOOK_SAI{SHEN_NAME(SAI_)}, BOOK_TASK{SHEN_NAME(book)};
struct BookSmoothMovement {
    Vector3 fromPosition, toPosition;
    Vector3i fromRotation, toRotation;
    shen_name character;
    std::int32_t frame, duration;
    std::uint32_t flags;
};
static_assert(sizeof(BookSmoothMovement) == 0x40);
static_assert(offsetof(BookSmoothMovement, character) == 0x30 && offsetof(BookSmoothMovement, flags) == 0x3c);
struct BookCompanionControl {
    std::int32_t frame;
    std::int16_t state, side;
};
static_assert(sizeof(BookCompanionControl) == 8);
extern BookSmoothMovement bookSmoothMovement;
extern BookCompanionControl bookCompanionControl;
void BookSmoothMovementCallback(void *);
std::int32_t InterpolateBookAngle(std::uint16_t from, std::uint16_t to, float amount);
void BookCompanionControlCallback(void *);
void SetBookActorPose(shen_name actor, const Vector3 &position, std::int32_t yaw);
void SetBookActorMotion(shen_name actor, std::uint16_t motion);
void SetBookCompanionPose(const Vector3 &position, std::int32_t yaw);
} // namespace shenmue::s2::areas
