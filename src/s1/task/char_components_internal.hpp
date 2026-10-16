#pragma once
#include "s1/task/char.hpp"

namespace shenmue::s1 {
struct CharacterCollisionWork;
struct MotionNodeRuntime;
extern void *g_hndb;
extern void *g_shadowMeshData;
extern std::uint32_t g_hndbSize;
extern void *g_handCollisionPool;
void mhndApplyCharaHandTarget(Character *character, std::int32_t hand, const HandPoseRecord &target, std::int32_t frameCount);
CharacterCollisionWork *FindCharacterControlWork(Character *character);
MotionManagerWork *FindMotionManagerWork(Character *character);
ImageModuleWork *FindImageModuleWork(Character *character);
std::int32_t getIMGM(Character *character);
Vector3 TransformHandPoint(const Matrix4x4 &matrix, const Vector3 &point);
void PushCharacterMatrix();
void PopCharacterMatrix();
std::int32_t FixedAtan2ForEuler(float y, float x);
Vector3 TransformPoint(const Matrix4x4 &transform, const Vector3 &point);
float MeasureRelativeHandNodeDistance(Character *character, std::int16_t rootKind, std::int16_t nodeKind);
float MeasureHandNodeChainLength(Character *character, std::int16_t rootKind, std::int16_t nodeKind, float fallback);
MotionNodeRuntime *FindMotionNode(MotionNodeRuntime *nodes, std::int32_t nodeId);
void UpdateCharacterClothPose(Character &character);
Vector3 OsageMatrixPosition(const Matrix4x4 &matrix);
OsageWork *FindOsageWork(Character *character);
void OSAGE_Update(Character *character);
const std::uint32_t *ScenarioWords(void *rawParams);
Character *ScenarioCharacter(ScenarioContext *context, std::uint32_t characterId);
extern bool g_suppressClothPose;
extern const HandPoseRecord kMotionHandPoses[10][2];
} // namespace shenmue::s1
