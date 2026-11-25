#pragma once

#include "s2/search_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

extern shen_name focusObject, anchorCharacter;
extern std::int32_t lastSearchCone;
extern Vector3 currentFollowPosition, focusPosition, followPosition, anchorPosition, playerSearchPosition;
static const Vector3 searchCones[8] = {{SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) - 1.0f, SHEN_FIELD(z) 0.3f},
    {SHEN_FIELD(x) - 0.707f, SHEN_FIELD(y) - 0.707f, SHEN_FIELD(z) 0.4f}, {SHEN_FIELD(x) - 1.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.5f},
    {SHEN_FIELD(x) - 0.707f, SHEN_FIELD(y) 0.707f, SHEN_FIELD(z) 0.6f}, {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 1.0f, SHEN_FIELD(z) 0.7f},
    {SHEN_FIELD(x) 0.707f, SHEN_FIELD(y) 0.707f, SHEN_FIELD(z) 0.6f}, {SHEN_FIELD(x) 1.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.5f},
    {SHEN_FIELD(x) 0.707f, SHEN_FIELD(y) - 0.707f, SHEN_FIELD(z) 0.4f}};
static const Vector2 searchAnchorOffsets[8] = {{SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f}, {SHEN_FIELD(x) 0.45f, SHEN_FIELD(y) 1.65f},
    {SHEN_FIELD(x) 0.45f, SHEN_FIELD(y) 1.65f}, {SHEN_FIELD(x) 0.45f, SHEN_FIELD(y) 1.5f}, {SHEN_FIELD(x) 0.45f, SHEN_FIELD(y) 1.5f},
    {SHEN_FIELD(x) 0.45f, SHEN_FIELD(y) 1.5f}, {SHEN_FIELD(x) 0.45f, SHEN_FIELD(y) 1.5f}, {SHEN_FIELD(x) 0.45f, SHEN_FIELD(y) 1.5f}};
bool ApplyQMarkerTranslation(MT7Node *node, std::uint32_t marker, Vector3 *position);
void DetectTargetFromView();
shen_name *GetScenarioCharacterAdmissionNames();
Vector3 *srchGetScenarioAdmissionPositions();
std::int32_t GetScenarioFollowEligibility(shen_name character);
std::uint32_t plymGetAuxiliaryState();
std::int32_t srchExecuteCommands(SearchTaskData &search);
std::int32_t srchUpdatePropIK(SearchTaskData &search);
void srchUpdate(SearchTaskData &search);
void srchUpdateLockOn(SearchTaskData &search);
void UpdateFollowPositions();
float FindBestFollowCandidate();
void SwitchDetectedTarget();
void srchComputeLockOn(SearchTaskData &search);
bool srchProcessSearchIK(SearchTaskData &search);
void srchLoadSignBoardModelForArea(SearchTaskData &search);
void srchReset();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t srchSetLookMode();
#endif
float ctrlGetLookOverlayWeight();
std::int32_t npcGetCanInteract(shen_name name);
void mtwkResetBlends();
std::int32_t mtwkSetOverridesEnabled(std::int32_t enabled);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
float ProjectConversationActorToScreen(const Vector3 &world, Vector3 &screen);
#else
void ProjectConversationActorToScreen(const Vector3 &world, Vector3 &screen);
#endif

} // namespace s2
} // namespace shenmue

#endif
