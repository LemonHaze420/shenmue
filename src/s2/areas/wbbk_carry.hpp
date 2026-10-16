#pragma once
#include "s2/areas/wbbk_tracks.hpp"
#include "s2/areas/wbbk_motion.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
struct BookCarryMotion {
    std::uint8_t input, raised, difficulty, transition, pendingQte, reserved5;
    std::int16_t requested, current;
    std::uint8_t side, reserved11;
    float frame, radialOffset, previousRootX;
};
static_assert(sizeof(BookCarryMotion) == 0x18 && offsetof(BookCarryMotion, requested) == 6 && offsetof(BookCarryMotion, frame) == 12);
struct BookMotionPreset {
    std::uint16_t motion, reserved;
    float firstFrame, sideFrame, lastFrame;
    std::int8_t category, next;
    std::uint16_t flags;
};
static_assert(sizeof(BookMotionPreset) == 20 && offsetof(BookMotionPreset, flags) == 18);
extern const BookMotionPreset bookMotionPresets[22];
extern const float bookCarryThresholds[14];
extern BookCarryMotion bookCarryMotion;
extern shen_name bookCameraAnchor, bookPlayerAnchor, bookStackAnchor;
extern std::int32_t bookCarrySegmentFrame;
extern std::int8_t bookCarryMode, bookCarryPhase, bookCarryVariant;
extern float bookCarryDistance;
extern const BookPoseKey *bookCarryTrack;
extern std::uint16_t bookDesiredCamera, bookCurrentCamera;
extern std::uint64_t bookDropRecovery;
extern float bookDropTimePenalty;
std::uint32_t SampleBookCarryTrack(const BookPoseKey *track, Vector3 &position, float frame, std::int32_t immediate);
Vector3 &SampleBookMotionRoot(std::int32_t preset, float frame);
bool IsBookMotionComplete(std::int16_t preset);
void PositionBookCarrier(Vector3 &position, std::int32_t yaw);
void SelectBookCarryInput();
std::int32_t AdvanceBookCarryMotion();
void UpdateBookCarryLoop();
void UpdateBookCarryTask();
void ChangeBookCarryMotion(std::int32_t preset, std::int8_t transition);
} // namespace shenmue::s2::areas
