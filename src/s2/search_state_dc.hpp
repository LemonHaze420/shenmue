#pragma once

#include "s2/search_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#define SHENMUE_SEARCH_STATE_CHECK(name, test) typedef char name[(test) ? 1 : -1]

namespace shenmue {
namespace s2 {

struct SearchSavedState {
    std::int32_t initialized;
    std::uint32_t playerFlags;
    std::int32_t used;
    std::uint32_t characterFlags;
    std::int32_t characterDisplay;
};
SHENMUE_SEARCH_STATE_CHECK(SearchSavedStateSizeCheck, sizeof(SearchSavedState) == 0x14);
SHENMUE_SEARCH_STATE_CHECK(SearchSavedStateDisplayCheck, offsetof(SearchSavedState, characterDisplay) == 0x10);

inline shen_name SearchTaskName() {
    return shen_name(SHEN_NAME4('S', 'R', 'C', 'H'));
}
#define SEARCH_TASK SearchTaskName()
extern SearchSavedState savedPlayerState;
extern Vector3 selectedSearchPosition;
extern shen_name selectedSearchName;
extern const SearchConfiguration *activeSearchConfig;
extern std::uint8_t inputHistory[16], lastInputX, lastInputY;
extern std::uint16_t lastDirectionInput;
extern std::uint32_t inputHistoryIndex, searchExitRequested;
extern MT7Model *g_searchTemporaryModel;
extern float searchTemporaryAlpha, searchTemporaryAlphaStep;
extern shen_name g_searchLastSignboard;
struct SearchPropIKSelection {
    std::uint16_t sound;
    std::uint8_t retained2;
    std::uint8_t bank;
    std::int8_t modifier;
    std::uint8_t retained5;
};
extern SearchPropIKSelection searchPropIKSelection[3];

void srchInitialize(SearchTaskData &search);
void init_search_ctrl();
void SearchControlTask_Callback(void *taskData);
std::int32_t srchGetActiveTargetPosition(Vector3 *position);
std::int32_t srchGetActiveTargetYaw(std::int16_t *yaw);
std::int32_t srchGetActiveTargetLockonAngle(float *angle);
void srchUpdateInputBuff();
void srchReadFromData(SearchTaskData &search);
void ResetSearchCharacterNames();
bool srchSavePlayerState(SearchTaskData &search);
std::int32_t srchRestoreRyoNpcStates(SearchTaskData &search);
void srchReadSelectionSounds(const std::byte *configuration);
bool srchGetEulerAngles(const SearchTarget *target, Vector3 *position, std::uint32_t *yaw);
bool ApplyInverseSearchNode(const MT7Node &node);
void srchApplyPropHierarchyTransform(const MT7Node &node);
void srchApplyPropTargetTransform(const MT7Node &node, std::uint32_t *yaw);

bool srchHasActiveLockonTarget();
std::int32_t srchCheckFlags(std::int32_t mode);
extern Vector3 *g_publishedSearchPosition;
bool RetrieveSearchFlagFromPtr(Vector3 &position);

#undef SHENMUE_SEARCH_STATE_CHECK

} // namespace s2
} // namespace shenmue

#endif
