#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_table_dc.hpp"

namespace shenmue {
namespace s2 {

struct CharacterHandComponent;
struct MotionWork;
void SetupImageModuleHandlers(Character *character);
void AssignCharacterTableImage(Character *character, HLib::Task *managerTask, const CharacterTableArgument *arguments, std::int32_t count);
void IMGM_AssignMTWK(Character *character);
void InitializeDefaultCharacterShadow(Character *character);
void scenCreateModel(Character *character, const CharacterTableArgument *arguments, std::int32_t count);
void mtwkCreateForChara(Character *character, std::int32_t commonWork);
void SetupModelForImageModule(MT7Node *node);
void AttachPlayerModel(Character *character);
void InitializeHumanCharacterNullsub(Character *character);
std::int32_t AttachPartyCompanion(Character *character);
void *mtSetupModelFigurePose(Character *character);
void ApplyFightCharacterDefinition(Character *character, const CharacterTableArgument *arguments, std::int32_t count);
void Handcuffs_Enqueue();
void AttachFootstepComponent(Character *character, const char *name, std::int32_t mode);
void InitRigType(Character *character, const char *name);
void AllocateCharacterHandPose(Character *character);
void AttachReflectorBoardComponent(Character *character, std::int32_t mode);
void SetCharacterMotionEnabled(Character *character, std::int32_t enabled);
void mtwkSetEnabled(MotionWork *work, std::int32_t enabled);
void mtUpdateTransparencyCallback(const MT7Node *node, float alpha);
void AllocateScenarioFreeTalkCharacter(Character *character);
void AllocateFIXO(Character *character);
void DrawPreparedVertexColours(const MT7Node *node, float alpha);
void AttachCharacterAlphaComponent(Character *character);
void AttachTailLampComponent(Character *character);
void AttachGameDisplayComponent(Character *character);
void AttachObjectBlur(Character *character, const CharacterTableRecord *record);
void AttachFlagWindComponent(Character *character);
void BindCharacterFacialModel(Character *character);
void AttachFacialController(Character *character);
void mtAssignClothFlags(Character *character, std::uint8_t mode);
void mtAssignOsage(Character *character);
void SetCharacterClockModeNullsub(Character *character, std::int16_t mode);
std::int32_t AllocateCCM(Character *character);
void ApplyCharacterTableCollisionFlags(Character *character, const CharacterTableRecord *record);
void AttachCharacterHandComponents(Character *character, const char *left, const char *right, std::int32_t flags);
std::uint32_t SetLowQualityHands(Character *character);
CharacterHandComponent *AllocateHNDL(Character *character);
CharacterHandComponent *AllocateHNDR(Character *character);
void SetCharacterHandRegionEnabled(CharacterHandComponent *hand, std::int8_t mode, std::int16_t parameter, std::int8_t enabled);
void AllocateTMNM(Character *character);
void AllocateKnifeComponent(Character *character);

} // namespace s2
} // namespace shenmue

#endif
