#pragma once

#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

namespace HLib {
struct Task;
}
struct LegacyInputSlot;
struct ScenarioFreeTalkCandidate;
struct MT7Node;
struct MT7Model;



struct SearchTarget {
    shen_name name;
    std::uint32_t flags;
    std::uint32_t field08;
    Vector3 position;
    float lockonAngle;
    std::int16_t yaw;
    std::uint16_t yawLimit;
};

// SnapshotSearchCandidate stores camera-space range and projected-center distance.
struct SnapshotSearchCandidate {
    shen_name name;
    float cameraDistanceSquared;
    float screenDistanceSquared;
};
static_assert(sizeof(SnapshotSearchCandidate) == 0x0c);
static_assert(offsetof(SnapshotSearchCandidate, cameraDistanceSquared) == 4);
static_assert(offsetof(SnapshotSearchCandidate, screenDistanceSquared) == 8);
const SnapshotSearchCandidate *CollectSnapshotSearchCandidates();

struct SearchSavedState {
    std::int32_t initialized;
    std::uint32_t playerFlags;
    std::int32_t used;
    std::uint32_t characterFlags;
    std::int32_t characterDisplay;
};

struct SearchResourceContext {
    std::byte *resource;
    std::byte *cursor;
    std::byte *strings;
    std::uint64_t field18;
    std::uint32_t flags;
    float remainingTicks;
};

struct SearchPropIKSlot {
    std::int32_t state;
    std::int32_t lookupIndex;
    std::uint32_t flags;
    std::uint32_t active;
    MT7Node *node;
    MT7Node *parent;
    void *character;
    std::byte *transformData;
    std::byte *auxiliaryData;
    std::uint32_t parameter;
    std::uint32_t reserved3c;
    void *resource;
    void *owner;
    std::uint64_t frame;
    std::int32_t leftSide;
    std::int32_t phase;
};
static_assert(sizeof(SearchPropIKSlot) == 96);
static_assert(offsetof(SearchPropIKSlot, node) == 16);
static_assert(offsetof(SearchPropIKSlot, transformData) == 40);
static_assert(offsetof(SearchPropIKSlot, auxiliaryData) == 48);
inline constexpr std::int32_t searchPropIKLookup[15]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15};
extern SearchPropIKSlot g_searchPropIKSlots[4];
extern MT7Model *g_searchTemporaryModel;
extern shen_name g_searchLastSignboard;

enum class SearchMode : std::int32_t { Idle = 0, Signboard = 1, Lockon = 2 };

struct SearchTaskData {
    std::uint16_t state;
    std::uint16_t targetState;
    SearchMode mode;
    std::int32_t fovStyle;
    std::uint32_t idleTicks;
    SearchTarget *currentTarget;
    std::array<SearchTarget *, 5> targets;
    std::int32_t initialCameraState;
    std::uint8_t targetFov;
    std::array<std::byte, 3> reserved45;
    const LegacyInputSlot *input;
    SearchResourceContext *context;
    std::array<SearchResourceContext, 3> contexts;
    std::int32_t contextIndex;
    std::int32_t fieldD4;
    SearchSavedState *savedState;
};

static_assert(sizeof(SearchTarget) == 0x20);
static_assert(offsetof(SearchTarget, position) == 0x0c);
static_assert(offsetof(SearchTarget, yawLimit) == 0x1e);
static_assert(sizeof(SearchSavedState) == 0x14);
static_assert(sizeof(SearchResourceContext) == 0x28);
static_assert(sizeof(SearchTaskData) == 0xe0);
static_assert(alignof(SearchTaskData) == 8);
static_assert(offsetof(SearchTaskData, currentTarget) == 0x10);
bool IsSpecialSignModeForCurrentArea();
bool npcGetCanInteract(shen_name name);
static_assert(offsetof(SearchTaskData, targets) == 0x18);
static_assert(offsetof(SearchTaskData, input) == 0x48);
static_assert(offsetof(SearchTaskData, context) == 0x50);
static_assert(offsetof(SearchTaskData, contexts) == 0x58);
static_assert(offsetof(SearchTaskData, contextIndex) == 0xd0);
static_assert(offsetof(SearchTaskData, savedState) == 0xd8);

std::int64_t srchCheckFlags(std::int32_t mode);
std::uint64_t srchSetTargetEnabled(shen_name name, std::int32_t enabled);
bool RetrieveSearchFlagFromPtr(Vector3 &position);
void srchUpdateInputBuff();
bool srchReadFromData(SearchTaskData &search);
bool srchHasActiveLockonTarget();
ScenarioFreeTalkCandidate *srchEvaluateSearchTarget();
std::span<const Vector3, 32> srchGetScenarioAdmissionPositions();
shen_name srchGetCurrentFocus();
shen_name srchGetCurrentFollowCharacter();
void srchSetFocusObject(shen_name name);
SearchTaskData *GetSearchControl();
std::span<const std::byte> GetActiveSearchConfiguration();
bool GetSearchExitRequested();
std::span<const std::uint8_t, 16> GetSearchInputHistory();
std::uint8_t GetSearchLastInputX();
std::uint8_t GetSearchLastInputY();
std::uint16_t GetSearchLastDirectionInput();
std::int32_t srchExecuteCommands(SearchTaskData &search);

void srchInitialize(SearchTaskData &search);
HLib::Task *InitializeSearchControlTask();
std::span<const std::byte> srchGetConfigFor(const SearchTaskData &search, shen_name name);
shen_name srchGetTargetName(std::uint32_t index);
std::int32_t srchRestoreRyoNpcStates(SearchTaskData &search);
void srchReset();
std::int64_t srchSetLookMode();
void srchResetIK();
std::int32_t srchGetCurrentIkIndex();
bool IsInvestigateMode();
void shenFreeTempModel();
void srchRequestLockonExit();
void srchRequestRelease();

void PushProjectionPrePass(float screenWidth, float screenHeight);
bool PushTransform(const Vector3 &point, Vector2 &screen);
std::int64_t srchSetTarget(std::int32_t index, SearchTarget *targets);
std::int64_t srchClearTarget(std::int32_t index);
bool ReadLOCK(SearchTarget *targets);
void srchPushPopTarget();


bool ReadSRCH(std::span<const std::byte> resource);
void SRCHParser(const std::byte *resource);

inline constexpr std::array<float, 16> searchModelRadii{0.5f, 0.4f, 0.4f, 0.2f, 0.4f, 0.4f, 0.5f, 0.4f, 0.5f, 0.5f, 0.7f, 0.3f, 0.4f, 0.4f, 0.2f, 0.5f};
MT7Node *FindModelCharacterRecursive(MT7Node *node, shen_name name);
MT7Node *mtFindChildRecursive(MT7Node *node, std::uint32_t marker);
void srchApplyObjectTransform(const MT7Node *node, std::uint32_t *yaw);
float srchGetDefaultModelRadius(std::int16_t name);
std::uint64_t srchRegisterStaticObject(MT7Node *node, std::uint32_t marker);
std::uint64_t srchRegisterAndPush(MT7Node *root);
void UpdatePropSearchVisibility(MT7Node *root, std::uint32_t parentMarker, std::int32_t enabled);

} // namespace shenmue::s2
