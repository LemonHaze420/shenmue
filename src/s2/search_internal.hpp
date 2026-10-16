#pragma once

#include "s2/search.hpp"
#include "s2/hand.hpp"

namespace shenmue::s2 {

extern shen_name focusObject;
extern shen_name currentFollowCharacter;
extern Vector3 currentFollowPosition;
extern Vector3 followPosition;
extern Vector3 focusPosition;
extern Vector3 playerSearchPosition;
extern Vector3 anchorPosition;
extern shen_name anchorCharacter;
extern Vector3 selectedSearchPosition;
extern const Vector3 *publishedSearchPosition;
extern shen_name selectedSearchName;
extern SearchSavedState savedPlayerState;
extern std::byte *activeSearchConfig;
extern std::uint32_t searchExitRequested;
extern std::uint32_t lightConfigurationChanged;
extern float searchStep;
extern std::int32_t searchPropIKIndex;
extern std::int32_t searchPropIKResult;
extern std::array<std::byte, 18> searchPropIKSelection;

struct SearchPropIKStage {
    Vector3 position;
    std::int32_t rotation[3];
    float targetOffset;
    std::uint8_t ticks;
    std::uint8_t reserved1d;
    std::int8_t jointTable;
    std::uint8_t axis;
};
struct SearchPropIKConfig {
    std::uint8_t hand;
    std::uint8_t pose;
    std::uint8_t reserved02[2];
    std::uint32_t flags;
    struct {
        std::int32_t pitch;
        std::int32_t yaw;
    } angles[2];
    Vector3 endpoints[2];
    Vector3 gripOffset;
    SearchPropIKStage stages[4];
};
static_assert(sizeof(SearchPropIKStage) == 32);
static_assert(sizeof(SearchPropIKConfig) == 188);
static_assert(offsetof(SearchPropIKConfig, stages) == 60);

void srchUpdate(SearchTaskData &search);
void srchUpdateLockOn(SearchTaskData &search);
void srchComputeLockOn(SearchTaskData &search);
void srchStaticApplySearchPosRot(void *);
void srchApplyPropHierarchyTransform(MT7Node &node);
bool srchProcessSearchIK(SearchTaskData &search);
bool srchGetEulerAngles(const SearchTarget *target, Vector3 *position, std::uint32_t *yaw);
bool srchLockTarget(SearchTaskData &search, shen_name name, bool faceTarget);
void srchLoadSignBoardModelForArea(SearchTaskData &search);
void srchIncrementTemporaryModel();
void srchReadSelectionSounds(const std::byte *config);
std::int32_t srchGetPropIKLookup(std::uint16_t marker);
void srchBindPropIK(SearchTaskData &search);
void srchClearPropIKSlot(std::int32_t index);
bool srchPushPropIK(SearchTaskData &search);
bool srchPopPropIK(SearchTaskData &search);
void srchBeginPropIKReturn();
void srchFinishPropIKItem(bool animate);
bool srchUpdatePropIK(SearchTaskData &search);

} // namespace shenmue::s2
