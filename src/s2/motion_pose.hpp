#pragma once

#include "s2/motion.hpp"
#include "common/math.hpp"

#include <array>
#include <cstdint>

namespace shenmue::s2 {

struct MotionWork;
struct MT7Model;

struct MotionPose {
    std::array<Matrix4x4, 256> nodeMatrices{};
    std::array<bool, 256> hasNodeMatrix{};
    Vector3 rootMotion{};
    std::array<Vector3, 2> armWristRotation{}; // right/left, X/Y/Z radians
    bool overrideRig{};
};

std::uint8_t MotionRigForEntityType(std::int32_t entityType);
std::int32_t GetMotionBoneIdForEntityType(std::int32_t entityType);
float GetMotionRigTranslationScale(const MotnSequence &sequence, std::uint8_t targetRig);
float GetMotionRigScale(std::uint32_t targetRig, std::uint32_t sourceRig, std::uint32_t region);
float GetMotionRigArmAxisScale(std::uint32_t rig);
void AlignMotionFrameToTarget(Matrix4x4 &transform, const Vector3 &target);
// Resolves a drawable node in the borrowed PublishModelNodeMatrices matrix-frame domain.
// Non-drawable nodes have no record. No MOTN pose/cache is synthesized.
const Matrix4x4 *FindMotionDrawOverrideNodeTransform(const MT7Model &model, const Matrix4x4 *frame, std::uint32_t nodeId);
bool SampleMotionRootTrack(const MotnSequence &sequence, float time, std::uint8_t targetRig, Vector3 &translation);
bool SampleMotionRootTrackPVA(const MotnSequence &sequence, float time, std::uint8_t targetRig, Vector3 &translation, Vector3 &incomingHalfTangent,
    Vector3 &outgoingHalfTangent, bool linear = false);
bool EvaluateMotionPose(const MotnSequence &sequence, float time, std::uint8_t targetRig, MotionPose &pose, const MotionWork *motionWork = nullptr);

} // namespace shenmue::s2
