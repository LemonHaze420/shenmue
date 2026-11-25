#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct MotionWork;
bool IsCharacterActive(const Character *character);
void GetCharacterLocalPosition(const Character *character, Vector3 *position);
void SetCharacterLocalPosition(Character *character, const Vector3 *position);
void TransformPointByCharacterNode(const CharacterNode *node, Vector3 *position);
void InverseTransformPointByCharacterNode(const CharacterNode *node, Vector3 *position);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InverseRotateVectorByCharacterNode(const CharacterNode *node, Vector3 *vector);
void AssignCharacterChildSpacePosition(Character *character, const Vector3 *delta);
void AddCharacterChildSpacePosition(Character *character, const Vector3 *delta);
#endif
void TransformScenarioMotionBonePose(shen_name character, std::int32_t bone, Vector3 *position, Vector3i *rotation);
void InverseTransformCharacterPoseByMatrix(const Matrix4x4 *transform, Vector3 *position, Vector3i *rotation);
void charaGetMotNodePos(Character *character, std::int32_t bone, Vector3 *position);
void charaGetMotLocalPos(Character *character, std::int32_t bone, Vector3 *position);
bool SetCharacterMotionTarget(Character *character, std::int32_t selector, const Vector3 *position);
bool LatchCharacterMotionTargetInParentSpace(Character *character, std::int32_t selector, const Vector3 *position);
std::int32_t charGetMotionWorkYaw(const Character *character);
std::int32_t GetCharacterMotionWorldYaw(const Character *character);
void mtwkSetEntityRotation(MotionWork *work, const Vector3i *rotation);
Character *GetCharacterFromNode(CharacterNode *node);
std::int32_t GetCharacterControlSlotIndex(const CharacterNode *node);

} // namespace s2
} // namespace shenmue

#endif
