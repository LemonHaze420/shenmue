#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "s2/input_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/search_data.hpp"

#define SHENMUE_SEARCH_CHECK(name, test) typedef char name[(test) ? 1 : -1]

namespace shenmue {
namespace s2 {

namespace SearchMode {
enum Type { Idle = 0, Signboard = 1, Lockon = 2 };
}
SHENMUE_SEARCH_CHECK(SearchModeSizeCheck, sizeof(SearchMode::Type) == 4);
struct SearchSavedState;
struct SearchTaskData {
    std::int16_t state;
    std::uint16_t targetState;
    SearchMode::Type mode;
    std::int32_t fovStyle;
    std::uint32_t idleTicks;
    SearchTarget *currentTarget;
    SearchTarget *targets[5];
    std::int32_t initialCameraState;
    std::uint8_t targetFov;
    std::byte reserved[3];
    const LegacyInputSlot *input;
    SearchResourceContext *context;
    SearchResourceContext contexts[3];
    std::int32_t contextIndex;
    SearchSavedState *savedState;
};
SHENMUE_SEARCH_CHECK(SearchTaskDataSizeCheck, sizeof(SearchTaskData) == 0x88);
SHENMUE_SEARCH_CHECK(SearchTaskTargetCheck, offsetof(SearchTaskData, currentTarget) == 0x10);
SHENMUE_SEARCH_CHECK(SearchTaskTargetsCheck, offsetof(SearchTaskData, targets) == 0x14);
SHENMUE_SEARCH_CHECK(SearchTaskInputCheck, offsetof(SearchTaskData, input) == 0x30);
SHENMUE_SEARCH_CHECK(SearchTaskContextsCheck, offsetof(SearchTaskData, contexts) == 0x38);
SHENMUE_SEARCH_CHECK(SearchTaskContextIndexCheck, offsetof(SearchTaskData, contextIndex) == 0x80);
SHENMUE_SEARCH_CHECK(SearchTaskSavedStateCheck, offsetof(SearchTaskData, savedState) == 0x84);

struct SearchConfiguration {
    shen_name name;
    std::uint32_t size;
    std::uint32_t flags;
    float radius;
    std::byte reserved[18];
    std::uint16_t yawLimit;
};
SHENMUE_SEARCH_CHECK(SearchConfigurationSizeCheck, sizeof(SearchConfiguration) == 0x24);
SHENMUE_SEARCH_CHECK(SearchConfigurationYawLimitCheck, offsetof(SearchConfiguration, yawLimit) == 0x22);

struct MT7PropNode {
    MT7Node node;
    MT7NodeDataHeader data;
    shen_name marker;
};
SHENMUE_SEARCH_CHECK(SearchPropNodeSizeCheck, sizeof(MT7PropNode) == 0x4c);
SHENMUE_SEARCH_CHECK(SearchPropNodeMarkerCheck, offsetof(MT7PropNode, marker) == 0x48);

#define SHENMUE_SEARCH_MODEL_RADII_VALUES {0.5f, 0.4f, 0.4f, 0.2f, 0.4f, 0.4f, 0.5f, 0.4f, 0.5f, 0.5f, 0.7f, 0.3f, 0.4f, 0.4f, 0.2f, 0.5f}
extern float searchModelRadii[16];
extern HLib::Task *searchTask;
extern SearchTaskData *searchControl;
extern SearchTarget staticTargets[257];
extern shen_name scenarioFollowCharacter, g_searchCharacterOverride, g_searchCharacterFallback;
extern std::int32_t scenarioFollowGraceFrames;
shen_name GetSearchCharacterName();
void SetSearchCharacterOverride(shen_name character);
shen_name GetSearchCharacterOverride();
void SetScenarioFollowCharacter(shen_name character);
shen_name GetScenarioFollowCharacter();

const SearchConfiguration *FindSearchConfiguration(const SearchTaskData &search, shen_name name);
bool ReadLOCK(SearchTarget *targets);
void srchPushPopTarget();
bool ReadSRCH(void *resource);
void SRCHParser(const std::byte *resource);
bool srchSetTarget(std::uint32_t index, SearchTarget *targets);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
SearchTarget *FindSearchTarget(const SearchTaskData &search, shen_name name);
std::uint32_t srchSetTargetEnabled(shen_name name, std::int32_t enabled);
bool srchClearTarget(std::uint32_t index);
shen_name srchGetTargetName(std::uint32_t index);
void srchRequestLockonExit();
#endif
std::uint32_t GetModelMarker(const MT7Node *node);
MT7Node *FindModelCharacterRecursive(MT7Node *node, shen_name name);
void srchApplyObjectTransform(const MT7Node *node, std::uint32_t *yaw);
float srchGetDefaultModelRadius(std::uint32_t name);
std::uint32_t srchRegisterStaticObject(MT7Node *node, std::uint32_t marker);
std::uint32_t srchRegisterAndPush(MT7Node *root);
void UpdatePropSearchVisibility(MT7Node *root, std::uint32_t parentMarker, std::int32_t enabled);

#undef SHENMUE_SEARCH_CHECK

} // namespace s2
} // namespace shenmue

#endif
