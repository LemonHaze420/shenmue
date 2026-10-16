#pragma once

#include "s2/model_component.hpp"
#include "s2/hand.hpp"
#include "d3t/model_cache.hpp"
#include <memory>
#include <unordered_map>

#include "s2/character_control_internal.hpp"
#include "s2/party_companion_internal.hpp"
#include "s2/cloth_internal.hpp"
#include "s2/hand_model_internal.hpp"
#include "s2/hand_pose_internal.hpp"
#include "s2/model_runtime_internal.hpp"
#include "s2/object_xforms_internal.hpp"
#include "s2/reflector_board_internal.hpp"
#include "s2/footstep_internal.hpp"

namespace shenmue::s2::model_component_detail {

inline constexpr std::uint32_t kSmallStorageSize = 0x200;

extern HandParams *g_temporaryHNDL;
extern HandParams *g_temporaryHNDR;
extern Matrix4x4 *handPoseMatrices;
extern Matrix4x4 *handJointMatrices;
extern Matrix4x4 *handRelativeMatrices;
extern void *defaultHandFiles[2];
extern std::uint32_t defaultHandFileSizes[2];
extern HandPoseBank handPoseBanks;
extern bool g_mhndSetupRunning;
extern HLib::Task *characterControlPairTask;
extern PartyCompanionTransition partyTransition;
extern std::uint32_t reflectorBoardActive;
extern std::uint32_t reflectorDefaultLightType;
extern std::int32_t reflectorDefaultLightAngle;
extern std::uint32_t forceCharacterAlphaVisible;
extern std::array<char, 5> cpamCharacterName;
extern std::unordered_map<const Character *, std::unique_ptr<PresentationState>> presentationStates;
extern std::array<std::byte, 0x30> defaultClothParameters;
extern float clothAnkleOffset;
extern Vector3 clothRootPosition;
extern Vector3 clothPairedBonePosition;
extern Vector3 clothVelocity;
extern Vector3 clothLeftConstraint;
extern Vector3 clothRightConstraint;
extern float clothWindReciprocal;
extern const Vector4 *clothDamping;
std::byte *Payload(ModelComponentHeader *component);
const std::byte *Payload(const ModelComponentHeader *component);
const ModelComponentHeader *FindHeader(const Character *character, shen_name token);
ModelComponentHeader *FindHeader(Character *character, shen_name token);
ReflectorBoardParams *FindReflectorBoard(Character *character);
float SquaredDistance(const Vector3 &left, const Vector3 &right);
float PartyHorizontalDistanceSquared(const Vector3 &left, const Vector3 &right);
bool PartySegmentHitsCylinder(const Vector3 &start, const Vector3 &end, const Vector3 &center, float radius);
ModelComponentHeader *HeaderForPayload(void *payload);
PresentationState *FindPresentation(const Character *character);
void InsertGameModelNodes(MT7Node *source, Character *character);
void RestoreGameModelNodes(MT7Node *source, Character *character);
void CharacterHandDrawClosure(Character *character);
void CharacterHandDirectCore(Character *character);
void UpdateHand(Character *character, void *payload);
void CleanupHand(Character *character, void *payload);

} // namespace shenmue::s2::model_component_detail
