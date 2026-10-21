#include "s1/task/conv.hpp"

#include "common/matrix_stack.hpp"
#include "s1/collision.hpp"
#include "s1/hlib.hpp"
#include "s1/motion.hpp"
#include "s1/timing.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/mbst.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/srch.hpp"

#include <cmath>
#include <cstring>

namespace shenmue::s1 {

ConversationCameraState g_Conv{};
std::array<ConversationCameraCandidate, 4> g_recentConvCamSelections{};
std::array<ConversationCameraCandidate, kConvCandidateCount> g_convCamCandidates{};
std::array<ConversationCameraPathSpan, 32> g_secondarySlotPathSpanEntries{};
std::array<ConversationCameraAnchorPair, 3> g_convActiveConversation{};
std::array<ConversationCameraCandidate, kConvCandidateCount> g_enabledConvCamSlotCandidates{};
std::int8_t g_enabledConvCamCandidateCount = 0;
std::int8_t g_enabledPrimaryConvCamCandidateCount = 0;
std::int8_t g_enabledSecondaryConvCamCandidateCount = 0;
std::uint8_t g_recentConvCamSelectionWriteCursor = 0;
std::uint8_t g_recentConvCamSelectionReadCursor = 0;
std::uint8_t g_recentConvCamAnchorPairWriteCursor = 0;
std::uint8_t g_recentConvCamAnchorPairReadCursor = 0;
std::uint8_t g_convCamMode = 0;
std::int32_t g_conversationCueParticipantSign = 0;
std::array<ScntConversationCue, 24> g_scntConversationCueWorkbuf{};
std::array<ScntConversationCue, 16> g_scntConversationCueBuffer{};
ScntConversationCue g_tmpConversationCue{};
Character *g_conversationParticipantA = nullptr;
Character *g_conversationParticipantB = nullptr;
std::uint8_t g_shouldComputeBetweenTwoEntities = 0;

namespace {

constexpr float kConvEpsilon = 0.00000011920929f;
constexpr std::int32_t kMotnRoot = 0;
constexpr std::int32_t kMotnHeadBase = 4;
constexpr std::int32_t kMotnRightShoulder = 25;
constexpr std::int32_t kMotnLeftHandEnd = 28;
constexpr std::uint32_t kCharacterMotionEnded = 4;

enum ConversationEntityQuadrant : std::uint8_t {
    kQuadrantNone = 0,
    kQuadrantFrontRight = 1,
    kQuadrantBackRight = 2,
    kQuadrantBackLeft = 3,
    kQuadrantFrontLeft = 4,
};

constexpr std::array<ConversationCameraSlotFilterEntry, 16> kConvCamSlotFilterShortTransition{{{0, 0, 4, 16}, {0, 0, 9, 16}, {0, 0, 11, 16},
    {0, 1, 5, 16}, {0, 1, 8, 16}, {0, 1, 11, 16}, {0, 1, 15, 16}, {0, 1, 16, 16}, {0, 1, 18, 16}, {0, 1, 19, 16}, {0, 1, 20, 16}, {0, 1, 21, 16},
    {0, 1, 22, 16}, {0, 1, 23, 16}, {0, 1, 24, 16}, {0, 1, 25, 16}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 13> kConvCamSlotFilterNoMapRuntimeFlag{{{0, 1, 0, 13}, {0, 1, 1, 13}, {0, 1, 2, 13},
    {0, 1, 12, 13}, {0, 1, 17, 13}, {0, 1, 18, 13}, {0, 1, 19, 13}, {0, 1, 20, 13}, {0, 1, 23, 13}, {0, 1, 24, 13}, {0, 1, 29, 13}, {0, 1, 30, 13},
    {0, 1, 31, 13}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 6> kConvCamSlotFilterUnused2{{{0, 1, 15, 6}, {0, 1, 16, 6}, {0, 1, 25, 6}, {0, 1, 29, 6},
    {0, 1, 30, 6}, {0, 1, 31, 6}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 17> kConvCamSlotFilterTwoEntityConversation{{{0, 0, 9, 17}, {0, 0, 10, 17}, {0, 1, 2, 17},
    {0, 1, 5, 17}, {0, 1, 8, 17}, {0, 1, 11, 17}, {0, 1, 15, 17}, {0, 1, 16, 17}, {0, 1, 17, 17}, {0, 1, 18, 17}, {0, 1, 19, 17}, {0, 1, 20, 17},
    {0, 1, 21, 17}, {0, 1, 22, 17}, {0, 1, 23, 17}, {0, 1, 24, 17}, {0, 1, 25, 17}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 5> kConvCamSlotFilterShortOrMultiParticipantConversation{{{0, 0, 4, 5}, {0, 0, 9, 5},
    {0, 1, 20, 5}, {0, 1, 21, 5}, {0, 1, 22, 5}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 14> kConvCamSlotFilterAnchorAMac2Class0{{{0, 1, 0, 14}, {0, 1, 1, 14}, {0, 1, 2, 14},
    {0, 1, 12, 14}, {0, 1, 13, 14}, {0, 1, 17, 14}, {0, 1, 18, 14}, {0, 1, 19, 14}, {0, 1, 20, 14}, {0, 1, 23, 14}, {0, 1, 24, 14}, {0, 1, 29, 14},
    {0, 1, 30, 14}, {0, 1, 31, 14}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 8> kConvCamSlotFilterMac2Class1{{{0, 1, 0, 8}, {0, 1, 2, 8}, {0, 1, 12, 8}, {0, 1, 17, 8},
    {0, 1, 18, 8}, {0, 1, 19, 8}, {0, 1, 20, 8}, {0, 1, 23, 8}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 4> kConvCamSlotFilterMac2Class2{{{0, 1, 0, 4}, {0, 1, 17, 4}, {0, 1, 20, 4}, {0, 1, 23, 4}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 4> kConvCamSlotFilterMac2Class3{{{0, 1, 2, 4}, {0, 1, 12, 4}, {0, 1, 18, 4}, {0, 1, 19, 4}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 32> kConvCamSlotFilterAnchorAMac2Class4{{{0, 1, 0, 32}, {0, 1, 1, 32}, {0, 1, 2, 32},
    {0, 1, 3, 32}, {0, 1, 4, 32}, {0, 1, 5, 32}, {0, 1, 6, 32}, {0, 1, 7, 32}, {0, 1, 8, 32}, {0, 1, 9, 32}, {0, 1, 10, 32}, {0, 1, 11, 32},
    {0, 1, 12, 32}, {0, 1, 13, 32}, {0, 1, 14, 32}, {0, 1, 15, 32}, {0, 1, 16, 32}, {0, 1, 17, 32}, {0, 1, 18, 32}, {0, 1, 19, 32}, {0, 1, 20, 32},
    {0, 1, 21, 32}, {0, 1, 22, 32}, {0, 1, 23, 32}, {0, 1, 24, 32}, {0, 1, 25, 32}, {0, 1, 26, 32}, {0, 1, 27, 32}, {0, 1, 28, 32}, {0, 1, 29, 32},
    {0, 1, 30, 32}, {0, 1, 31, 32}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 29> kConvCamSlotFilterMac2Class5{{{0, 1, 0, 29}, {0, 1, 1, 29}, {0, 1, 2, 29}, {0, 1, 3, 29},
    {0, 1, 4, 29}, {0, 1, 5, 29}, {0, 1, 6, 29}, {0, 1, 7, 29}, {0, 1, 8, 29}, {0, 1, 12, 29}, {0, 1, 13, 29}, {0, 1, 14, 29}, {0, 1, 15, 29},
    {0, 1, 16, 29}, {0, 1, 17, 29}, {0, 1, 18, 29}, {0, 1, 19, 29}, {0, 1, 20, 29}, {0, 1, 21, 29}, {0, 1, 22, 29}, {0, 1, 23, 29}, {0, 1, 24, 29},
    {0, 1, 25, 29}, {0, 1, 26, 29}, {0, 1, 27, 29}, {0, 1, 28, 29}, {0, 1, 29, 29}, {0, 1, 30, 29}, {0, 1, 31, 29}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 8> kConvCamSlotFilterCurrentCycleOwner{{{0, 1, 3, 8}, {0, 1, 4, 8}, {0, 1, 6, 8}, {0, 1, 7, 8},
    {0, 1, 9, 8}, {0, 1, 10, 8}, {0, 1, 11, 8}, {0, 1, 15, 8}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 40> kConvCamSlotFilterDoorInteractionCue{{{0, 0, 0, 40}, {0, 0, 1, 40}, {0, 0, 4, 40},
    {0, 0, 5, 40}, {0, 0, 6, 40}, {0, 0, 7, 40}, {0, 0, 8, 40}, {0, 0, 9, 40}, {0, 0, 10, 40}, {0, 0, 11, 40}, {0, 0, 12, 40}, {0, 0, 14, 40},
    {0, 1, 0, 40}, {0, 1, 1, 40}, {0, 1, 2, 40}, {0, 1, 3, 40}, {0, 1, 4, 40}, {0, 1, 5, 40}, {0, 1, 6, 40}, {0, 1, 7, 40}, {0, 1, 8, 40}, {0, 1, 9, 40},
    {0, 1, 10, 40}, {0, 1, 11, 40}, {0, 1, 12, 40}, {0, 1, 14, 40}, {0, 1, 15, 40}, {0, 1, 16, 40}, {0, 1, 17, 40}, {0, 1, 18, 40}, {0, 1, 19, 40},
    {0, 1, 20, 40}, {0, 1, 21, 40}, {0, 1, 22, 40}, {0, 1, 23, 40}, {0, 1, 24, 40}, {0, 1, 25, 40}, {0, 1, 26, 40}, {0, 1, 27, 40}, {0, 1, 28, 40}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 6> kConvCamSlotFilterCycleOwnerUnflagged{{{0, 0, 9, 6}, {0, 0, 10, 6}, {0, 0, 11, 6},
    {0, 0, 12, 6}, {0, 0, 13, 6}, {0, 0, 14, 6}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 5> kConvCamSlotFilterDefaultCyclePolicy{
    {{0, 0, 4, 5}, {0, 0, 5, 5}, {0, 0, 6, 5}, {0, 0, 7, 5}, {0, 0, 8, 5}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 10> kConvCamSlotFilterUnused15{{{0, 0, 0, 10}, {0, 0, 1, 10}, {0, 0, 2, 10}, {0, 0, 3, 10},
    {0, 0, 9, 10}, {0, 0, 10, 10}, {0, 0, 11, 10}, {0, 0, 12, 10}, {0, 0, 13, 10}, {0, 0, 14, 10}}};
constexpr std::array<ConversationCameraSlotFilterEntry, 12> kConvCamSlotFilterCurrentStaticDoorActor{{{0, 0, 0, 12}, {0, 0, 1, 12}, {0, 0, 2, 12},
    {0, 0, 3, 12}, {0, 0, 4, 12}, {0, 0, 9, 12}, {0, 0, 10, 12}, {0, 0, 11, 12}, {0, 0, 12, 12}, {0, 0, 13, 12}, {0, 0, 14, 12}, {0, 0, 15, 12}}};

constexpr std::array<const ConversationCameraSlotFilterEntry *, 17> kConvCamSlotFilterTableById{kConvCamSlotFilterShortTransition.data(),
    kConvCamSlotFilterNoMapRuntimeFlag.data(), kConvCamSlotFilterUnused2.data(), kConvCamSlotFilterTwoEntityConversation.data(),
    kConvCamSlotFilterShortOrMultiParticipantConversation.data(), kConvCamSlotFilterAnchorAMac2Class0.data(), kConvCamSlotFilterMac2Class1.data(),
    kConvCamSlotFilterMac2Class2.data(), kConvCamSlotFilterMac2Class3.data(), kConvCamSlotFilterAnchorAMac2Class4.data(), kConvCamSlotFilterMac2Class5.data(),
    kConvCamSlotFilterCurrentCycleOwner.data(), kConvCamSlotFilterDoorInteractionCue.data(), kConvCamSlotFilterCycleOwnerUnflagged.data(),
    kConvCamSlotFilterDefaultCyclePolicy.data(), kConvCamSlotFilterUnused15.data(), kConvCamSlotFilterCurrentStaticDoorActor.data()};

} // namespace

namespace {

constexpr std::array<float, 11> kConversationSpanScale{1.0f, 0.25f, 0.25f, 0.25f, 0.25f, 0.75f, 0.75f, 0.5f, 0.5f, 0.5f, 1.0f};
constexpr std::array<float, 11> kConversationSpanOffset{0.0f, 0.0f, 0.25f, 0.5f, 0.75f, 0.0f, 0.25f, 0.0f, 0.25f, 0.5f, 0.0f};

MapControlData *CurrentMapControl() {
    return g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}

Character *CurrentRyoCharacter() {
    MapControlData *const control = CurrentMapControl();
    return control ? static_cast<Character *>(control->ryo) : nullptr;
}

std::uint32_t ScenarioCharacterId(Character *character) {
    return shenGetNextCharaFromTable(CurrentMapControl()->scenarioTask, character);
}

Character *ConversationCueActor(std::int32_t taskDataOffset) {
    return taskDataOffset ? reinterpret_cast<Character *>(g_pCurrentTaskData + taskDataOffset) : nullptr;
}

ScntConversationCue *NextConversationCue(ScntConversationCue *cue) {
    return reinterpret_cast<ScntConversationCue *>(reinterpret_cast<std::byte *>(cue) + 4 * cue->size);
}

float FlushConversationEpsilon(float value) {
    return std::fabs(value) < kConvEpsilon ? 0.0f : value;
}

float BlendConversationValue(float from, float to, float alpha) {
    const float delta = from - to;
    return std::fabs(delta) >= kConvEpsilon ? from - delta * alpha : from;
}

std::int32_t BlendConversationAngle(std::int32_t from, std::int32_t to, float alpha) {
    const float delta = static_cast<float>(from - to);
    return std::fabs(delta) >= kConvEpsilon ? static_cast<std::int32_t>(static_cast<float>(from) - delta * alpha) : from;
}

float ConversationSine(std::int32_t angle) {
    return sin_fixed16(static_cast<std::int16_t>(angle));
}

float ConversationCosine(std::int32_t angle) {
    return cos_fixed16(static_cast<std::uint16_t>(angle));
}

float ConversationHalfCosineEase(float ratio) {
    const std::uint16_t angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(ratio * 180.0f * 8192.0f / 45.0f));
    return (1.0f - ConversationCosine(angle)) * 0.5f;
}

Vector3 ConversationBonePosition(Character *character, std::int32_t nodeId) {
    Vector3 position{};
    mtwkGetCharaBonePosition(character, nodeId, &position);
    return position;
}

Vector3 ConversationEntityPosition(Character *character) {
    return character ? GetPositionForEntity(character->entity.node) : Vector3{};
}

std::array<std::uint8_t, 0x20> ConversationCollisionMask() {
    std::array<std::uint8_t, 0x20> mask{};
    std::memcpy(mask.data(), CurrentMapControl()->cycleMask, mask.size());
    mask[1] &= 0xfcU;
    return mask;
}

void TransformPointByInverseYawFrame(const Vector3 &origin, std::int32_t yaw, const Vector3 &input, Vector3 &output) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    Matrix4x4::RotateAroundY(ConversationSine(yaw), ConversationCosine(yaw));
    Matrix4x4 inverse = matrix_stack::workTransformMatrix;
    Matrix4x4::InvertInPlace(&inverse);
    Matrix4x4::Pop();
    Matrix4x4::SetActiveTransformToPrePass();
    matrix_stack::workTransformMatrix = inverse;
    Matrix4x4::TransformPoint(input, output);
    Matrix4x4::Pop();
}

std::uint16_t ConversationPairYaw(const Vector3 &pointA, const Vector3 &pointB) {
    return atan2_fixed16(pointA.x - pointB.x, pointA.z - pointB.z);
}

void ApplyEntityTransformWithInverse(Character *character, const Vector3 *input, Vector3 *output) {
    const Vector3 origin = ConversationEntityPosition(character);
    TransformPointByInverseYawFrame(origin, character ? character->entity.node.rotation.y : 0, *input, *output);
}

void TransformForEntity(Character *character, const Vector3 *input, Vector3 *output) {
    const Vector3 origin = ConversationBonePosition(character, kMotnHeadBase);
    TransformPointByInverseYawFrame(origin, character ? character->entity.node.rotation.y : 0, *input, *output);
}

void Vector3_ApplyTransRotY(const Vector3 *pointA, const Vector3 *pointB, const Vector3 *localPoint, Vector3 *output) {
    const Vector3 local = *localPoint;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(pointA->x, pointA->y, pointA->z);
    const std::uint16_t yaw = ConversationPairYaw(*pointA, *pointB);
    Matrix4x4::RotateAroundY(ConversationSine(yaw), ConversationCosine(yaw));
    Matrix4x4::TransformPoint(local, *output);
    Matrix4x4::Pop();
}

void MatrixStack_WorldToYawFrame(const Vector3 *pointA, const Vector3 *pointB, const Vector3 *input, Vector3 *output) {
    const Vector3 origin = *pointA;
    TransformPointByInverseYawFrame(origin, ConversationPairYaw(*pointA, *pointB), *input, *output);
}

void ApplyPrePassTransform(const Vector3 *localPoint, Vector3 *worldPoint) {
    TransformPointByInverseYawFrame(g_Conv.initialRyoWorldPos, g_Conv.initialRyoYaw, *localPoint, *worldPoint);
}

void Vector3_CalculatePartialVectorSubtraction(Vector3 *output, const Vector3 *from, const Vector3 *to, float alpha) {
    const float blend = FlushConversationEpsilon(alpha);
    output->x = BlendConversationValue(from->x, to->x, blend);
    output->y = BlendConversationValue(from->y, to->y, blend);
    output->z = BlendConversationValue(from->z, to->z, blend);
}

std::int32_t Vector3_AngleBetween(const Vector3 *a, const Vector3 *b) {
    const float dot = (b->x * a->x + b->y * a->y) + b->z * a->z;
    const float lengthB = std::sqrt((b->x * b->x + b->y * b->y) + b->z * b->z);
    const float cosine = dot / (lengthB * std::sqrt((a->x * a->x + a->y * a->y) + a->z * a->z));
    if (std::fabs(cosine) <= 1.0f)
        return 16384 - asin_fixed16(cosine);
    return cosine > 0.0f ? 0 : 32768;
}

bool cewpGetFlag(Character *character) {
    if (!character)
        return false;
    for (void *const head : CurrentMapControl()->characterBuckets) {
        for (auto *node = static_cast<CharacterBucketNode *>(head); node; node = node->next) {
            Character *const candidate = node->character;
            if (!candidate || candidate->entity.name != SHEN_NAME(CEWP))
                continue;
            const std::uint32_t flags = candidate->flags.value;
            if (((flags & 1U) != 0 || ((flags & 2U) != 0 && (candidate->motionFlags & 1U) != 0)) && character == candidate)
                return true;
        }
    }
    return false;
}

void DisableConvCamCandidates(const ConversationCameraSlotFilterEntry *filter) {
    for (ConversationCameraCandidate &candidate : g_convCamCandidates) {
        for (std::int8_t entry = 0; entry < static_cast<std::int8_t>(filter[0].count); ++entry) {
            if (candidate.domain == filter[entry].domain && candidate.index == filter[entry].index)
                candidate.enabled = 0;
        }
    }
}

void DisableConvCamCandidatesByFilterId(std::int8_t filterId) {
    DisableConvCamCandidates(kConvCamSlotFilterTableById[filterId]);
}

void DisableCrossAxisCandidate(std::uint8_t slot) {
    for (ConversationCameraCandidate &candidate : g_convCamCandidates) {
        if (candidate.domain == kConvDomainCrossAxis && candidate.index == slot)
            candidate.enabled = 0;
    }
}
} // namespace

namespace CycleManager {


static bool CheckDistanceBetweenCoordinates() {
    const CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(g_Conv.selectedOtherCharaId);
    const Vector3 head = ConversationBonePosition(g_Conv.selectedOtherParticipant, kMotnHeadBase);
    const Vector3 root = ConversationBonePosition(g_Conv.selectedOtherParticipant, kMotnRoot);
    return npc && npc->cycle->moveConfig->configWord2C > 50 && std::sqrt((head.x - root.x) * (head.x - root.x) + (head.z - root.z) * (head.z - root.z)) > 0.2;
}

} // namespace CycleManager

namespace {


bool isVerticalDifferenceLessThanHalf() {
    const float rootY = ConversationBonePosition(g_Conv.selectedOtherParticipant, kMotnRoot).y;
    const float handY = ConversationBonePosition(g_Conv.selectedOtherParticipant, kMotnLeftHandEnd).y;
    return std::fabs(rootY - handY) < 0.5;
}

bool CheckPositionDifference() {
    const float ryoHeadY = ConversationBonePosition(g_Conv.cachedRyo, kMotnHeadBase).y;
    const float otherHeadY = ConversationBonePosition(g_Conv.selectedOtherParticipant, kMotnHeadBase).y;
    const float otherRootY = ConversationBonePosition(g_Conv.selectedOtherParticipant, kMotnRoot).y;
    const float otherShoulderY = ConversationBonePosition(g_Conv.selectedOtherParticipant, kMotnRightShoulder).y;
    return std::fabs(ryoHeadY - otherHeadY) > 0.3 && std::fabs(otherRootY - otherShoulderY) < 0.29;
}

bool CheckAngleDifference() {
    Vector3 ryoHead = ConversationBonePosition(g_Conv.cachedRyo, kMotnHeadBase);
    ApplyEntityTransformWithInverse(g_Conv.selectedOtherParticipant, &ryoHead, &ryoHead);
    const Vector3 forward{0.0f, 0.0f, -1.0f};
    ryoHead.y = 0.0f;
    return Vector3_AngleBetween(&ryoHead, &forward) <= 16384;
}

bool IsInteractionCueOutsideForwardArc() {
    const Vector3 ryo = ConversationEntityPosition(g_Conv.cachedRyo);
    const Vector3 other = ConversationEntityPosition(g_Conv.selectedOtherParticipant);
    Vector3 cue{};
    MatrixStack_WorldToYawFrame(&ryo, &other, &g_Conv.interactionCueLocation, &cue);
    const Vector3 forward{0.0f, 0.0f, -1.0f};
    cue.y = 0.0f;
    return Vector3_AngleBetween(&forward, &cue) > 21845;
}

ConversationEntityQuadrant shenGetEntityQuadrant() {
    Vector3 position = ConversationEntityPosition(g_Conv.selectedOtherParticipant);
    ApplyPrePassTransform(&position, &position);
    if (position.x < 0.0f)
        return position.z >= 0.0f ? kQuadrantFrontLeft : kQuadrantBackLeft;
    return position.z < 0.0f ? kQuadrantBackRight : kQuadrantFrontRight;
}

std::int32_t TestConversationParticipantLineCollision() {
    const Vector3 participant0 = ConversationEntityPosition(g_Conv.cueParticipant0);
    const Vector3 participant1 = ConversationEntityPosition(g_Conv.cueParticipant1);
    const std::array<std::uint8_t, 0x20> mask = ConversationCollisionMask();
    return CalculateVectorDistanceAndExecuteMathFn(participant0, participant1, 0.22f, mask.data());
}

std::int32_t GetEntityMac2Class(Character *character) {
    Vector3 position = ConversationEntityPosition(character);
    if (character && character->entity.node.child)
        position = WorldToEntityLocalPoint(*character->entity.node.child, position);
    std::uint32_t mac2Class = 0;
    std::uint32_t value8 = 0;
    std::uint32_t valueC = 0;
    std::uint32_t value10 = 0;
    const CollisionPointQueryOutputs outputs{.value4 = &mac2Class, .value8 = &value8, .valueC = &valueC, .value10 = &value10};
    if (CheckIsPointIntersectWith(position, &outputs, SHEN_NAME(MAC2)))
        return static_cast<std::int32_t>(mac2Class);
    return -1;
}

void ComputeConversationAuthorPair(const ConversationAuthorPoint &closeEye, const ConversationAuthorPoint &farEye, const ConversationAuthorPoint &closeTarget,
    const ConversationAuthorPoint &farTarget, float distance, float heightDelta, ConversationAuthorPoint &eye, ConversationAuthorPoint &target) {
    const float mirror = g_Conv.mirrorSign == -1 ? -1.0f : 1.0f;
    const float slope = FlushConversationEpsilon(heightDelta / distance);
    const float blend = FlushConversationEpsilon(static_cast<float>((3.0 - static_cast<double>(distance)) / 2.1));
    eye.distance = BlendConversationValue(farEye.distance, closeEye.distance, blend);
    eye.height = BlendConversationValue(farEye.height, closeEye.height, blend) + slope * eye.distance;
    const float eyeAngle = BlendConversationValue(static_cast<float>(farEye.angle), static_cast<float>(closeEye.angle), blend);
    eye.angle = static_cast<std::int32_t>(eyeAngle * mirror);
    target.distance = BlendConversationValue(farTarget.distance, closeTarget.distance, blend);
    target.height = BlendConversationValue(farTarget.height, closeTarget.height, blend) - slope * target.distance;
    const float targetAngle = BlendConversationValue(static_cast<float>(farTarget.angle), static_cast<float>(closeTarget.angle), blend);
    target.angle = static_cast<std::int32_t>(targetAngle * mirror);
}

void ComputeConversationAuthorPoints(ConversationAuthorPoint &eye, ConversationAuthorPoint &target, float distance, float heightDelta, std::uint8_t setup) {
    const ConversationAuthorSetup &shots = kConversationAuthorSetups[setup];
    ComputeConversationAuthorPair(shots.closeRange.eye, shots.farRange.eye, shots.closeRange.target, shots.farRange.target, distance, heightDelta, eye, target);
}

void ComputeConversationAuthorCoords(ConversationAuthorPoint &eye, ConversationAuthorPoint &target, float distance, float heightDelta, std::uint8_t setup) {
    const ConversationAuthorSetup &shots = kConversationAuthorSetups[setup];
    ComputeConversationAuthorPair(
        shots.closeRange.eyeEnd, shots.farRange.eyeEnd, shots.closeRange.targetEnd, shots.farRange.targetEnd, distance, heightDelta, eye, target);
}

Vector3 ConversationAuthorLocalPoint(std::int32_t angle, float distance, float height) {
    return {-(ConversationSine(angle) * distance), height, -(ConversationCosine(angle) * distance)};
}

void shenBuildConversationCameraLocalTargetEyeAndFov(Vector3 *eye, Vector3 *target, std::uint8_t domain, std::uint8_t slot, float pathAlpha) {
    std::int32_t fovStart = g_Conv.conversationFovAngle16;
    std::int32_t fovEnd = fovStart;
    if (domain == kConvDomainSameAxis) {
        CAME_SetFieldOfView(10922);
        const float mirror = g_Conv.mirrorSign == -1 ? -1.0f : 1.0f;
        const float secondary = g_Conv.mirrorSecondaryEnabled ? 1.0f : -1.0f;
        const ConversationSameAxisShot &shot = kConversationSameAxisShots[slot];
        fovStart = currentPerspectiveFov;
        const float alpha = FlushConversationEpsilon(pathAlpha);
        eye->x = BlendConversationValue(mirror * shot.eye.x, mirror * shot.eyeEnd.x, alpha);
        eye->y = BlendConversationValue(shot.eye.y, shot.eyeEnd.y, alpha);
        eye->z = BlendConversationValue(secondary * shot.eye.z, secondary * shot.eyeEnd.z, alpha);
        target->x = BlendConversationValue(mirror * shot.target.x, mirror * shot.targetEnd.x, alpha);
        target->y = BlendConversationValue(shot.target.y, shot.targetEnd.y, alpha);
        target->z = BlendConversationValue(secondary * shot.target.z, secondary * shot.targetEnd.z, alpha);
        fovEnd = fovStart;
    } else if (domain <= kConvDomainCrossAxisSpan) {
        const float heightDelta = g_Conv.anchorHeightDelta;
        const float distance = g_Conv.anchorDistanceXZ;
        ConversationAuthorPoint startEye{};
        ConversationAuthorPoint startTarget{};
        ConversationAuthorPoint endEye{};
        ConversationAuthorPoint endTarget{};
        ComputeConversationAuthorPoints(startEye, startTarget, distance, heightDelta, slot);
        ComputeConversationAuthorCoords(endEye, endTarget, distance, heightDelta, slot);
        fovStart = kConversationAuthorSetups[slot].closeRange.fovStart;
        fovEnd = kConversationAuthorSetups[slot].closeRange.fovEnd;
        const float alpha = FlushConversationEpsilon(pathAlpha);
        const std::int32_t eyeAngle = BlendConversationAngle(startEye.angle, endEye.angle, alpha);
        const float eyeDistance = BlendConversationValue(startEye.distance, endEye.distance, alpha);
        const float eyeHeight = BlendConversationValue(startEye.height, endEye.height, alpha);
        const std::int32_t targetAngle = BlendConversationAngle(startTarget.angle, endTarget.angle, alpha);
        const float targetDistance = BlendConversationValue(startTarget.distance, endTarget.distance, alpha);
        const float targetHeight = BlendConversationValue(startTarget.height, endTarget.height, alpha);
        *eye = ConversationAuthorLocalPoint(eyeAngle, eyeDistance, eyeHeight);
        *target = ConversationAuthorLocalPoint(targetAngle, targetDistance, targetHeight);
    } else if (domain <= kConvDomainDirect1) {
        fovStart = g_Conv.transitionStartFovAngle16;
        ConversationAuthorPoint endEye{};
        ConversationAuthorPoint endTarget{};
        ComputeConversationAuthorCoords(endEye, endTarget, g_Conv.anchorDistanceXZ, g_Conv.anchorHeightDelta, 3);
        fovEnd = kConversationAuthorSetups[3].closeRange.fovEnd;
        Vector3 eyeWorld = ConversationAuthorLocalPoint(endEye.angle, endEye.distance, endEye.height);
        Vector3 targetWorld = ConversationAuthorLocalPoint(endTarget.angle, endTarget.distance, endTarget.height);
        Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorA, &g_Conv.smoothedAnchorB, &eyeWorld, &eyeWorld);
        Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorB, &g_Conv.smoothedAnchorA, &targetWorld, &targetWorld);
        Vector3_CalculatePartialVectorSubtraction(eye, &g_Conv.transitionStartEye, &eyeWorld, pathAlpha);
        Vector3_CalculatePartialVectorSubtraction(target, &g_Conv.transitionStartTarget, &targetWorld, pathAlpha);
    }
    const std::int64_t fovDelta = static_cast<std::int64_t>(fovStart) - fovEnd;
    if (std::fabs(static_cast<float>(fovDelta)) < kConvEpsilon)
        g_Conv.conversationFovAngle16 = fovStart;
    else
        g_Conv.conversationFovAngle16 = static_cast<std::int32_t>(static_cast<float>(fovStart) - static_cast<float>(fovDelta) * pathAlpha);
}

void convBuildCamera(Vector3 *eye, Vector3 *target, std::uint8_t domain, std::uint8_t slot, float pathAlpha) {
    Vector3 localEye{};
    Vector3 localTarget{};
    shenBuildConversationCameraLocalTargetEyeAndFov(&localEye, &localTarget, domain, slot, pathAlpha);
    if (domain == kConvDomainSameAxis) {
        if (g_Conv.singleParticipantFallbackActive) {
            motmApplyRootMot(g_Conv.cachedRyo, localTarget, *target);
            motmApplyRootMot(g_Conv.cachedRyo, localEye, *eye);
        } else {
            Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorB, &g_Conv.smoothedAnchorA, &localTarget, target);
            Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorB, &g_Conv.smoothedAnchorA, &localEye, eye);
        }
    } else if (domain <= kConvDomainCrossAxisSpan) {
        Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorB, &g_Conv.smoothedAnchorA, &localTarget, target);
        Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorA, &g_Conv.smoothedAnchorB, &localEye, eye);
    } else if (domain <= kConvDomainDirect1) {
        *eye = localEye;
        *target = localTarget;
    }
}

void FinalizeWorldTransform(Vector3 *targetIn, Vector3 *targetOut, Vector3 *eyeIn, Vector3 *eyeOut, float ease, bool useAnchors) {
    Vector3 target{};
    Vector3 cue{};
    Vector3 eye{};
    if (useAnchors) {
        MatrixStack_WorldToYawFrame(&g_Conv.smoothedAnchorA, &g_Conv.smoothedAnchorB, targetIn, &target);
        MatrixStack_WorldToYawFrame(&g_Conv.smoothedAnchorA, &g_Conv.smoothedAnchorB, &g_Conv.interactionCueLocation, &cue);
        MatrixStack_WorldToYawFrame(&g_Conv.smoothedAnchorA, &g_Conv.smoothedAnchorB, eyeIn, &eye);
    } else {
        MatrixStack_WorldToYawFrame(eyeIn, targetIn, targetIn, &target);
        MatrixStack_WorldToYawFrame(eyeIn, targetIn, &g_Conv.interactionCueLocation, &cue);
    }
    const float targetY = target.y;
    target.y = 0.0f;
    cue.y = 0.0f;
    std::int32_t yaw = static_cast<std::int32_t>(static_cast<float>(Vector3_AngleBetween(&target, &cue)) * ease);
    const float targetZ = target.z;
    const float targetX = target.x;
    if (cue.x * target.z - cue.z * target.x < 0.0f)
        yaw = -yaw;
    target.y = targetY;
    target.x = 0.0f;
    cue = {0.0f, 0.0f, -1.0f};
    std::int32_t pitch = static_cast<std::int32_t>(static_cast<float>(Vector3_AngleBetween(&target, &cue)) * ease);
    if (targetY * -1.0f - targetZ * 0.0f < 0.0f)
        pitch = -pitch;
    Matrix4x4::SetActiveTransformToPrePass();
    const Matrix4x4 &work = matrix_stack::workTransformMatrix;
    if (useAnchors) {
        const float yawSine = ConversationSine(yaw);
        Matrix4x4::RotateAroundY(yawSine, ConversationCosine(yaw));
        Matrix4x4::RotateAroundX(ConversationSine(pitch), ConversationCosine(pitch));
        Vector3 rotatedTarget{((work.rows[1].x * targetY + work.rows[0].x * targetX) + work.rows[2].x * targetZ) + work.rows[3].x,
            ((work.rows[1].y * targetY + work.rows[0].y * targetX) + work.rows[2].y * targetZ) + work.rows[3].y,
            ((targetY * work.rows[1].z + work.rows[0].z * targetX) + work.rows[2].z * targetZ) + work.rows[3].z};
        Matrix4x4::Pop();
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::RotateAroundY(yawSine, ConversationCosine(yaw));
        const Vector3 rotatedEye{((eye.x * work.rows[0].x + eye.y * work.rows[1].x) + eye.z * work.rows[2].x) + work.rows[3].x,
            ((eye.y * work.rows[1].y + eye.x * work.rows[0].y) + eye.z * work.rows[2].y) + work.rows[3].y,
            ((eye.y * work.rows[1].z + eye.x * work.rows[0].z) + eye.z * work.rows[2].z) + work.rows[3].z};
        Matrix4x4::Pop();
        Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorA, &g_Conv.smoothedAnchorB, &rotatedEye, eyeOut);
        Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorA, &g_Conv.smoothedAnchorB, &rotatedTarget, targetOut);
        return;
    }
    Matrix4x4::RotateAroundY(ConversationSine(yaw), ConversationCosine(yaw));
    Matrix4x4::RotateAroundX(ConversationSine(pitch), ConversationCosine(pitch));
    const Vector3 rotatedTarget{((work.rows[1].x * targetY + work.rows[0].x * targetX) + work.rows[2].x * targetZ) + work.rows[3].x,
        ((work.rows[1].y * targetY + work.rows[0].y * targetX) + work.rows[2].y * targetZ) + work.rows[3].y,
        ((targetY * work.rows[1].z + work.rows[0].z * targetX) + work.rows[2].z * targetZ) + work.rows[3].z};
    Matrix4x4::Pop();
    Vector3_ApplyTransRotY(eyeIn, targetIn, &rotatedTarget, targetOut);
    *eyeOut = *eyeIn;
    Matrix4x4::Pop();
}

void ComputeCorrectedPointPosition(
    const Vector3 *anchorA, const Vector3 *anchorB, const Vector3 *pointA, const Vector3 *pointB, const Vector3 *input, Vector3 *output, float scale) {
    Vector3 origin{};
    MatrixStack_WorldToYawFrame(pointA, pointB, input, &origin);
    float magnitude = std::sqrt(origin.z * origin.z + origin.x * origin.x);
    float directionX = 0.0f;
    float directionZ = 0.0f;
    Vector3 corrected{};
    if (scale < magnitude) {
        if (g_Conv.anchorCorrectionHoldFlags != 1) {
            *output = *input;
            return;
        }
        directionX = origin.x / magnitude;
        directionZ = origin.z / magnitude;
    } else {
        directionZ = origin.z / magnitude;
        directionX = origin.x / magnitude;
        origin.z = directionZ * scale;
        origin.x = directionX * scale;
        Vector3_ApplyTransRotY(pointA, pointB, &origin, &corrected);
        Vector3 anchored{};
        MatrixStack_WorldToYawFrame(anchorA, anchorB, &corrected, &anchored);
        magnitude = std::sqrt(anchored.z * anchored.z + anchored.x * anchored.x);
        if ((g_shouldComputeBetweenTwoEntities || !(g_Conv.anchorCorrectionMagnitude > magnitude)) && g_Conv.anchorCorrectionHoldFlags != 1) {
            g_Conv.anchorCorrectionMagnitude = magnitude;
            g_Conv.anchorCorrectionScale = scale;
            *output = corrected;
            return;
        }
        if (!g_shouldComputeBetweenTwoEntities && g_Conv.anchorCorrectionMagnitude > magnitude)
            g_Conv.anchorCorrectionHoldFlags = 1;
    }
    origin.x = g_Conv.anchorCorrectionScale * directionX;
    origin.z = g_Conv.anchorCorrectionScale * directionZ;
    Vector3_ApplyTransRotY(pointA, pointB, &origin, &corrected);
    *output = corrected;
}

void UpdateConversationAutoFov(Vector3 *eye, Vector3 *target) {
    const Vector3 eyePosition = *eye;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(eyePosition.x, eyePosition.y, eyePosition.z);
    const std::uint16_t yaw = atan2_fixed16(eye->x - target->x, eye->z - target->z);
    Matrix4x4::RotateAroundY(ConversationSine(yaw), ConversationCosine(yaw));
    const float horizontal = std::sqrt((eye->x - target->x) * (eye->x - target->x) + (eye->z - target->z) * (eye->z - target->z));
    const std::uint16_t pitch = atan2_fixed16(target->y - eye->y, horizontal);
    Matrix4x4::RotateAroundX(ConversationSine(pitch), ConversationCosine(pitch));
    Matrix4x4 view = matrix_stack::workTransformMatrix;
    Matrix4x4::InvertInPlace(&view);
    Matrix4x4::Pop();
    const Vector3 anchorB = ConversationBonePosition(g_Conv.anchorEntityB, kMotnHeadBase);
    const Vector3 anchorA = ConversationBonePosition(g_Conv.anchorEntityA, kMotnHeadBase);
    Matrix4x4::SetActiveTransformToPrePass();
    matrix_stack::workTransformMatrix = view;
    const Vector3 viewB{((view.rows[1].x * anchorB.y + view.rows[0].x * anchorB.x) + view.rows[2].x * anchorB.z) + view.rows[3].x,
        ((view.rows[0].y * anchorB.x + view.rows[1].y * anchorB.y) + view.rows[2].y * anchorB.z) + view.rows[3].y,
        ((view.rows[1].z * anchorB.y + view.rows[0].z * anchorB.x) + view.rows[2].z * anchorB.z) + view.rows[3].z};
    const Vector3 viewA{((view.rows[1].x * anchorA.y + view.rows[0].x * anchorA.x) + view.rows[2].x * anchorA.z) + view.rows[3].x,
        ((view.rows[0].y * anchorA.x + view.rows[1].y * anchorA.y) + view.rows[2].y * anchorA.z) + view.rows[3].y,
        ((view.rows[1].z * anchorA.y + view.rows[0].z * anchorA.x) + view.rows[2].z * anchorA.z) + view.rows[3].z};
    Matrix4x4::Pop();
    const std::uint32_t widenAngle = g_Conv.autoFovWidenAngle16;
    std::int32_t fov = static_cast<std::int32_t>(widenAngle) + g_Conv.conversationFovAngle16;
    const std::int32_t halfFov = fov / 2;
    g_Conv.conversationFovAngle16 = fov;
    const float projection =
        static_cast<float>(320.0 / static_cast<double>(std::tan(static_cast<float>(static_cast<std::uint16_t>(halfFov)) * 6.2831855f * 1.52587890625e-05f)));
    const float depthA = std::sqrt(viewA.z * viewA.z);
    const float screenAX = viewA.x * projection / depthA;
    const float screenAY = viewA.y * projection / depthA;
    const float depthB = std::sqrt(viewB.z * viewB.z);
    const float screenBX = viewB.x * projection / depthB;
    const float screenBY = viewB.y * projection / depthB;
    const std::uint8_t domain = g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex];
    float screenX = 0.0f;
    float screenY = 0.0f;
    float enterX = 0.0f;
    float enterY = 0.0f;
    if (domain == kConvDomainCrossAxis && g_shouldComputeBetweenTwoEntities) {
        screenX = screenAX;
        screenY = screenAY;
        enterX = 384.0f;
        enterY = 288.0f;
    } else if (domain == kConvDomainSameAxis && !g_Conv.cueDoorInteractionFlags[g_Conv.activeCueIndex]) {
        screenX = screenBX;
        screenY = screenBY;
        enterX = 320.0f;
        enterY = 240.0f;
    } else {
        g_Conv.conversationFovAngle16 = fov >= 12743 ? 12743 : fov;
        return;
    }
    if (std::fabs(screenX) > enterX || std::fabs(screenY) > enterY) {
        if (!g_Conv.autoFovWideningActive)
            g_Conv.autoFovWideningActive = 1;
    }
    const std::uint8_t widening = g_Conv.autoFovWideningActive;
    if (std::fabs(screenX) <= 304.0f && std::fabs(screenY) <= 228.0f && widening == 1) {
        g_Conv.autoFovWideningActive = 0;
    } else if (widening && fov <= 12743) {
        g_Conv.autoFovWidenAngle16 = widenAngle + 60;
    }
    g_Conv.conversationFovAngle16 = fov >= 12743 ? 12743 : fov;
}

void convUpdateCamera(Vector3 *eye, Vector3 *target, std::int32_t fieldOfView) {
    const Vector3 anchorB = ConversationBonePosition(g_Conv.anchorEntityB, kMotnHeadBase);
    const Vector3 anchorA = ConversationBonePosition(g_Conv.anchorEntityA, kMotnHeadBase);
    if (!g_Conv.anchorCorrectionHoldFlags) {
        g_Conv.anchorBCorrectionRef = anchorB;
        g_Conv.anchorACorrectionRef = anchorA;
    }
    const float scale = static_cast<float>(static_cast<double>(g_Conv.cameraClearanceScale) * 1.1);
    if (g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex]) {
        ComputeCorrectedPointPosition(
            &g_Conv.smoothedAnchorA, &g_Conv.smoothedAnchorB, &g_Conv.anchorACorrectionRef, &g_Conv.anchorBCorrectionRef, eye, eye, scale);
    } else {
        ComputeCorrectedPointPosition(
            &g_Conv.smoothedAnchorB, &g_Conv.smoothedAnchorA, &g_Conv.anchorBCorrectionRef, &g_Conv.anchorACorrectionRef, eye, eye, scale);
    }
    const float clearance = !CheckPositionDifference() || CheckAngleDifference() ? 0.60000002f : 0.89999998f;
    const float ground = mapUpdateIntersectionStateAtLocalPoint(*eye);
    const float minimumHeight = ground + clearance;
    if (minimumHeight > eye->y)
        eye->y = minimumHeight;
    TranslateCameraRelative(*eye);
    if (!bLockCamera) {
        g_CameraTargetPos = *target;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    UpdateConversationAutoFov(eye, target);
    CAME_SetFieldOfView(fieldOfView);
}

float GetTransformedDistanceBetweenTwoEntities() {
    const bool crossAxis = g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] != 0;
    Character *const anchor = crossAxis ? g_Conv.anchorEntityA : g_Conv.anchorEntityB;
    Vector3 head = ConversationBonePosition(anchor, kMotnHeadBase);
    Vector3 root = ConversationBonePosition(anchor, kMotnRoot);
    const float headHeight = head.y - root.y;
    float clearance = static_cast<float>(static_cast<double>(std::sqrt((head.x - root.x) * (head.x - root.x) + (head.z - root.z) * (head.z - root.z))) + 0.3);
    if (clearance <= 0.42)
        clearance = 0.41999999f;
    Vector3 eye{};
    Vector3 target{};
    convBuildCamera(&eye, &target, g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex], g_Conv.cueCameraSlotTable.slots[g_Conv.activeCueIndex], 0.0f);
    if (crossAxis) {
        MatrixStack_WorldToYawFrame(&g_Conv.smoothedAnchorA, &g_Conv.smoothedAnchorB, &eye, &eye);
        if (headHeight > 0.25 || eye.z <= 0.0f)
            return 0.41999999f;
        return static_cast<float>(static_cast<double>(clearance) * 1.5);
    }
    MatrixStack_WorldToYawFrame(&eye, &target, &head, &head);
    MatrixStack_WorldToYawFrame(&eye, &target, &root, &root);
    return std::fabs(head.z) > std::fabs(root.z) ? clearance : 0.41999999f;
}

} // namespace

namespace {

float ComputeConversationCameraSlotSpanAlpha(std::int16_t duration);



float shenGetElapsedCueFrames30Hz() {
    const float now = QueryFrameTime();
    const std::uint32_t ticks = static_cast<std::uint32_t>(g_CurrentFrameDelta) - static_cast<std::uint32_t>(FrameTimeTicks(g_LastFrameTime - now));
    return static_cast<float>(static_cast<double>(static_cast<float>(ticks)) * 30.0 / 12500000.0);
}

void convUpdateForParticipants(std::int32_t activeCue, Character **participantA, Character **participantB) {
    const ScntConversationCue *const cue = g_Conv.conversationCues[activeCue];
    Character *first = ConversationCueActor(cue->actor0TaskDataOffset);
    Character *second = ConversationCueActor(cue->actor1TaskDataOffset);
    if (!first) {
        for (std::int8_t index = 0; index < static_cast<std::int8_t>(g_Conv.participantCount); ++index) {
            first = g_Conv.participantEntities[index];
            if (first != second)
                break;
        }
    }
    if (!second) {
        for (std::int8_t index = 0; index < static_cast<std::int8_t>(g_Conv.participantCount); ++index) {
            second = g_Conv.participantEntities[index];
            if (second != first)
                break;
        }
    }
    if (first == g_Conv.cachedRyo) {
        if (second == g_Conv.cachedRyo) {
            for (std::int8_t index = 0; index < static_cast<std::int8_t>(g_Conv.participantCount); ++index) {
                first = g_Conv.participantEntities[index];
                if (first != g_Conv.cachedRyo)
                    return;
            }
        }
    } else if (second != g_Conv.cachedRyo) {
        second = g_Conv.cachedRyo;
    }
    if (g_Conv.participantCount <= 1 || (!first && !second)) {
        first = g_Conv.cachedRyo;
        second = g_Conv.cachedRyo;
    }
    *participantA = first;
    *participantB = second;
}

void convLoadCueStream() {
    ScntConversationCue *cue = g_Conv.conversationCueLoadPtr;
    g_Conv.conversationCueCount = 0;
    g_Conv.participantCount = 0;
    if ((cue->flags & 1U) == 0) {
        bool finished = false;
        do {
            Character *const actor0 = ConversationCueActor(cue->actor0TaskDataOffset);
            Character *const actor1 = ConversationCueActor(cue->actor1TaskDataOffset);
            std::int8_t count = static_cast<std::int8_t>(g_Conv.participantCount);
            if (actor0) {
                std::int8_t index = 0;
                while (index < count && actor0 != g_Conv.participantEntities.data()[index])
                    ++index;
                if (index == count) {
                    g_Conv.participantEntities.data()[count] = actor0;
                    count = static_cast<std::int8_t>(++g_Conv.participantCount);
                }
            }
            if (actor1) {
                std::int8_t index = 0;
                while (index < count && actor1 != g_Conv.participantEntities.data()[index])
                    ++index;
                if (index == count) {
                    g_Conv.participantEntities.data()[count] = actor1;
                    ++g_Conv.participantCount;
                }
            }
            g_Conv.conversationCues[g_Conv.conversationCueCount++] = cue;
            cue = NextConversationCue(cue);
            finished = (cue->flags & 1U) != 0;
        } while (!finished);
    }

    for (std::int32_t index = 0; index < static_cast<std::int8_t>(g_Conv.participantCount); ++index) {
        Character *const character = g_Conv.participantEntities[index];
        ConversationParticipantRestoreState &restore = g_Conv.participantRestore.data()[index];
        restore.characterId = ScenarioCharacterId(character);
        MotionManagerWork *const manager = MOTM_GetWork(character);
        restore.hadMotmFlag2 = manager ? manager->flags & 2U : 0U;
        std::uint32_t hadCpamBit0 = 0;
        if (!character) {
            restore.savedPoseEvalStage = 0;
        } else {
            restore.savedPoseEvalStage = manager->motion.poseEvaluationStage;
            if (CharacterComponentNode *const cpam = HLib_GetCharacterComponent(character, COMPONENT_CPAM.name)) {
                hadCpamBit0 = (cpam->flags & 1U) != 0 ? 1U : 0U;
                cpam->flags &= ~1U;
            }
            character->renderOpacity = 1.0f;
        }
        restore.hadCpamBit0 = hadCpamBit0;
        if (restore.hadMotmFlag2 && (manager->flags & 2U) != 0) {
            if (manager->motion.activeNodes) {
                --MBST_GetWork()->state;
                manager->flags &= ~2U;
                MBST_Refresh(true);
            } else {
                MOTI_TransferPrimaryNodes(nullptr, &manager->motion);
                manager->flags &= ~2U;
            }
        }
        if (character)
            manager->motion.poseEvaluationStage = 0;
    }
}

std::int8_t CountConversationCueParticipants() {
    std::array<Character *, 7> participants{};
    std::int8_t count = 0;
    ScntConversationCue *cue = g_Conv.conversationCueLoadPtr;
    if ((cue->flags & 1U) != 0)
        return 0;
    bool finished = false;
    do {
        Character *const actor0 = ConversationCueActor(cue->actor0TaskDataOffset);
        Character *const actor1 = ConversationCueActor(cue->actor1TaskDataOffset);
        if (actor0) {
            std::int8_t index = 0;
            while (index < count && actor0 != participants.data()[index])
                ++index;
            if (index == count)
                participants.data()[count++] = actor0;
        }
        if (actor1) {
            std::int8_t index = 0;
            while (index < count && actor1 != participants.data()[index])
                ++index;
            if (index == count)
                participants.data()[count++] = actor1;
        }
        cue = NextConversationCue(cue);
        finished = (cue->flags & 1U) != 0;
    } while (!finished);
    return count;
}

void convCompactCueStream() {
    auto *output = g_scntConversationCueBuffer.data();
    const float shortCueSeconds = CountConversationCueParticipants() > 2 ? 1.5f : 2.5999999f;
    ScntConversationCue *input = g_scntConversationCueWorkbuf.data();
    ScntConversationCue *previous = nullptr;
    g_Conv.compactedLeadCueBiasPending = 0;
    for (std::int32_t cueIndex = 0; (input->flags & 1U) == 0; ++cueIndex) {
        const float durationSeconds = input->durationSeconds;
        if (cueIndex) {
            if (durationSeconds <= shortCueSeconds) {
                previous->durationSeconds = durationSeconds + previous->durationSeconds;
                if (g_Conv.compactedLeadCueBiasPending) {
                    previous->durationSeconds = static_cast<float>(static_cast<double>(previous->durationSeconds) - 0.67);
                    g_Conv.compactedLeadCueBiasPending = 0;
                }
                previous->flags |= input->flags;
                input = NextConversationCue(input);
                continue;
            }
            std::memcpy(output, input, 4U * input->size);
            previous = output;
            if (g_Conv.compactedLeadCueBiasPending) {
                previous->durationSeconds = static_cast<float>(static_cast<double>(previous->durationSeconds) - 0.67);
                g_Conv.compactedLeadCueBiasPending = 0;
            }
        } else {
            std::memcpy(output, input, 4U * input->size);
            previous = output;
            if (shortCueSeconds > output->durationSeconds) {
                previous->durationSeconds = static_cast<float>(static_cast<double>(output->durationSeconds) + 0.67);
                g_Conv.compactedLeadCueBiasPending = 1;
            }
        }
        output = NextConversationCue(output);
        input = NextConversationCue(input);
    }
    std::memcpy(output, input, 4U * input->size);
}

void UpdateSubsTime() {
    std::int8_t count = static_cast<std::int8_t>(g_Conv.conversationCueCount);
    bool doorCueFound = false;
    const std::uint8_t twoEntities = g_shouldComputeBetweenTwoEntities;
    std::array<std::uint8_t, 24> &domains = g_Conv.cueCameraSlotTable.domains;
    for (std::int8_t cue = 0; cue < count; ++cue) {
        if (count == 1) {
            domains[0] = kConvDomainCrossAxis;
        } else if (count == 2) {
            if (g_Conv.preserveInitialParticipantOrder || !twoEntities) {
                domains[0] = kConvDomainCrossAxis;
                domains[1] = kConvDomainSameAxis;
            } else {
                domains[0] = kConvDomainSameAxis;
                domains[1] = kConvDomainCrossAxis;
            }
        } else if (count >= 3) {
            if (cue)
                domains[cue] = cue == count - 1 || (cue & 1) == 0 ? 1U : 0U;
            else
                domains[0] = g_Conv.preserveInitialParticipantOrder ? kConvDomainCrossAxis : (twoEntities == 0 ? 1U : 0U);
        }
        if (!doorCueFound && (g_Conv.conversationCues[cue]->flags & 2U) != 0) {
            g_Conv.cueDoorInteractionFlags[cue] = 1;
            doorCueFound = true;
        }
        count = static_cast<std::int8_t>(g_Conv.conversationCueCount);
    }
    bool anyDoorCue = false;
    for (std::int8_t cue = 0; cue < count; ++cue) {
        if (g_Conv.cueDoorInteractionFlags[cue] == 1)
            anyDoorCue = true;
    }
    if (!anyDoorCue)
        return;
    for (std::int8_t cue = static_cast<std::int8_t>(count - 1); cue >= 0; --cue) {
        if (count >= 3) {
            domains[cue] = (cue & 1) == 0 ? 1U : 0U;
            domains[g_Conv.conversationCueCount - 2] = kConvDomainSameAxis;
            domains[g_Conv.conversationCueCount - 1] = kConvDomainCrossAxis;
            count = static_cast<std::int8_t>(g_Conv.conversationCueCount);
        } else {
            domains[0] = kConvDomainSameAxis;
            domains[1] = kConvDomainCrossAxis;
        }
    }
}

void IncrementCamCounter() {
    std::uint8_t &domain = g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex];
    std::int8_t shots = 0;
    if (domain) {
        if (((domain - 1) & 0xfd) == 0) {
            ++g_Conv.nonPrimaryCameraDomainStreak;
            g_Conv.cameraShotCounter = 0;
        } else {
            shots = static_cast<std::int8_t>(g_Conv.cameraShotCounter);
            if (shots > 2)
                domain = kConvDomainCrossAxis;
        }
    } else {
        shots = static_cast<std::int8_t>(++g_Conv.cameraShotCounter);
        g_Conv.nonPrimaryCameraDomainStreak = 0;
        if (shots > 2)
            domain = kConvDomainCrossAxis;
    }
    if (g_Conv.nonPrimaryCameraDomainStreak > 3)
        g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] = kConvDomainSameAxis;
}

void convUpdateCameraCollisionLock(Vector3 *eye, Vector3 *lockedEye) {
    if (g_Conv.targetCollisionLockActive) {
        lockedEye->x = g_Conv.targetCollisionLockPos.x;
        lockedEye->z = g_Conv.targetCollisionLockPos.z;
        lockedEye->y = eye->y;
        return;
    }
    const std::uint8_t activeCue = g_Conv.activeCueIndex;
    const float alpha = ComputeConversationCameraSlotSpanAlpha(static_cast<std::int16_t>(g_Conv.activeCueBlendFrame + 1));
    Vector3 nextEye{};
    Vector3 nextTarget{};
    convBuildCamera(&nextEye, &nextTarget, g_Conv.cueCameraSlotTable.domains[activeCue], g_Conv.cueCameraSlotTable.slots[activeCue], alpha);
    const std::array<std::uint8_t, 0x20> mask = ConversationCollisionMask();
    Vector3 hit{};
    if (coliTestPlane(*eye, nextEye, 0.198f, hit, mask.data()) == 1) {
        g_Conv.targetCollisionLockActive = 1;
        lockedEye->x = hit.x;
        lockedEye->z = hit.z;
        lockedEye->y = eye->y;
        g_Conv.targetCollisionLockPos = hit;
    } else {
        *lockedEye = *eye;
    }
}

std::int8_t ChooseConversationSideSignByCollision() {
    Character *participant0 = nullptr;
    Character *participant1 = nullptr;
    convUpdateForParticipants(0, &participant0, &participant1);
    if (g_Conv.participantCount <= 1 || participant0 == participant1)
        return 1;
    Character *other = participant1;
    if (ScenarioCharacterId(participant0) != ScenarioCharacterId(g_Conv.cachedRyo))
        other = ScenarioCharacterId(participant1) == ScenarioCharacterId(g_Conv.cachedRyo) ? participant0 : nullptr;

    std::array<std::uint8_t, 0x20> mask{};
    std::memcpy(mask.data(), CurrentMapControl()->cycleMask, mask.size());
    mask[1] &= 0xfcU;
    mask[0] &= 0x7fU;
    const Vector3 ryoHead = ConversationBonePosition(g_Conv.cachedRyo, kMotnHeadBase);
    const Vector3 otherHead = ConversationBonePosition(other, kMotnHeadBase);
    const std::uint16_t yaw = atan2_fixed16(ryoHead.x - otherHead.x, ryoHead.z - otherHead.z);
    const auto probe = [&](float sideStep, float depth) {
        const Vector3 local{sideStep, ryoHead.y, depth};
        Vector3 world{};
        Matrix4x4::SetActiveTransformToPrePass();
        Matrix4x4::preTranslate(ryoHead.x, ryoHead.y, ryoHead.z);
        Matrix4x4::RotateAroundY(ConversationSine(yaw), ConversationCosine(yaw));
        Matrix4x4::TransformPoint(local, world);
        Matrix4x4::Pop();
        return ctrlCheckColi(ryoHead, world, 0.1f, mask.data()) != 0;
    };

    float searchDistance = 5.0f;
    float rightDepth = 0.0f;
    bool rightHit = false;
    for (std::int8_t row = 0; row < 15 && !rightHit; ++row) {
        rightDepth = static_cast<float>(static_cast<double>(row) * -0.2);
        for (std::int8_t column = 1; column <= 15; ++column) {
            const float side = static_cast<float>(static_cast<double>(column) * 0.2);
            if (probe(side, rightDepth)) {
                searchDistance = side;
                rightHit = true;
                break;
            }
        }
    }
    if (!rightHit)
        rightDepth = searchDistance;

    float leftSide = 5.0f;
    float leftDepth = 0.0f;
    bool leftHit = false;
    for (std::int8_t row = 0; row < 15 && !leftHit; ++row) {
        leftDepth = static_cast<float>(static_cast<double>(row) * -0.2);
        for (std::int8_t column = 1; column <= 15; ++column) {
            const float side = static_cast<float>(static_cast<double>(column) * -0.2);
            if (probe(side, leftDepth)) {
                leftSide = side;
                leftHit = true;
                break;
            }
        }
    }
    if (!leftHit)
        leftDepth = searchDistance;

    const float left = std::sqrt(leftDepth * leftDepth + leftSide * leftSide);
    const float right = std::sqrt(rightDepth * rightDepth + searchDistance * searchDistance);
    if (left > right)
        return -1;
    if (right > left)
        return 1;
    Vector3 ryo = ConversationEntityPosition(g_Conv.cachedRyo);
    ApplyEntityTransformWithInverse(other, &ryo, &ryo);
    return ryo.x > 0.0f ? 1 : -1;
}

void ResolveConversationCameraParticipants() {
    g_Conv.ryoCharaId = ScenarioCharacterId(g_Conv.cachedRyo);
    Character *const participant0 = g_Conv.cueParticipant0;
    if (ScenarioCharacterId(participant0) != g_Conv.ryoCharaId) {
        g_Conv.selectedOtherParticipant = participant0;
        g_Conv.selectedOtherCharaId = ScenarioCharacterId(participant0);
    } else {
        g_Conv.selectedOtherParticipant = g_Conv.cueParticipant1;
        g_Conv.selectedOtherCharaId = ScenarioCharacterId(g_Conv.cueParticipant1);
    }
    g_Conv.anchorEntityB = g_Conv.cueParticipant0;
    g_Conv.anchorEntityA = g_Conv.cueParticipant1;
    if (g_Conv.initialCueAnchorOverride == 1 && !g_Conv.activeCueIndex && !g_Conv.preserveInitialParticipantOrder) {
        g_Conv.anchorEntityA = g_Conv.cachedRyo;
        g_Conv.anchorEntityB = g_Conv.selectedOtherParticipant;
    }
    if (g_Conv.cueDoorInteractionFlags[g_Conv.activeCueIndex]) {
        if (IsInteractionCueOutsideForwardArc()) {
            g_Conv.anchorEntityB = g_Conv.cachedRyo;
            g_Conv.anchorEntityA = g_Conv.selectedOtherParticipant;
        } else {
            g_Conv.anchorEntityB = g_Conv.selectedOtherParticipant;
            g_Conv.anchorEntityA = g_Conv.cachedRyo;
        }
    }
    const bool crossAxis = g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] != 0;
    if (g_Conv.selectedOtherCharaId == SHEN_NAME(DOOR)) {
        g_Conv.anchorEntityB = crossAxis ? g_Conv.selectedOtherParticipant : g_Conv.cachedRyo;
        g_Conv.anchorEntityA = crossAxis ? g_Conv.cachedRyo : g_Conv.selectedOtherParticipant;
    }
    if (CycleManager::CheckDistanceBetweenCoordinates() || isVerticalDifferenceLessThanHalf() || CheckPositionDifference() ||
        CycleManager::GetCycleFlagForSpecificChara(g_Conv.selectedOtherCharaId)) {
        if (g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] == kConvDomainCrossAxis) {
            g_Conv.anchorEntityB = g_Conv.selectedOtherParticipant;
            g_Conv.anchorEntityA = g_Conv.cachedRyo;
        }
    }
    if (g_Conv.selectedOtherCharaId == SHEN_NAME(RKKI) && CycleManager::GetCycleFlagForSpecificChara(SHEN_NAME(RKKI))) {
        Character *const small = CHID_Find(CurrentMapControl()->scenarioTask, SHEN_NAME(SMAL));
        if (small && (small->motionFlags & kCharacterMotionEnded) != 0) {
            const bool rkkiCrossAxis = g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] != 0;
            g_Conv.anchorEntityB = rkkiCrossAxis ? g_Conv.selectedOtherParticipant : g_Conv.cachedRyo;
            g_Conv.anchorEntityA = rkkiCrossAxis ? g_Conv.cachedRyo : g_Conv.selectedOtherParticipant;
        }
    }
    g_Conv.anchorCharaIdB = ScenarioCharacterId(g_Conv.anchorEntityB);
    g_Conv.anchorCharaIdA = ScenarioCharacterId(g_Conv.anchorEntityA);
}

void ResolveConvCamAnchorsFromSelectionHistory() {
    std::uint8_t &domain = g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex];
    if (domain != g_recentConvCamSelections[g_recentConvCamSelectionReadCursor].domain)
        return;
    if (g_Conv.anchorCharaIdB != g_convActiveConversation[g_recentConvCamAnchorPairReadCursor].anchorB)
        return;
    domain = domain == 0 ? 1U : 0U;
    if (g_Conv.selectedOtherCharaId == SHEN_NAME(DOOR)) {
        const bool crossAxis = g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] != 0;
        Character *const anchorB = crossAxis ? g_Conv.selectedOtherParticipant : g_Conv.cachedRyo;
        Character *const anchorA = crossAxis ? g_Conv.cachedRyo : g_Conv.selectedOtherParticipant;
        g_Conv.anchorEntityA = anchorA;
        g_Conv.anchorEntityB = anchorB;
        g_Conv.anchorCharaIdB = ScenarioCharacterId(anchorB);
        g_Conv.anchorCharaIdA = ScenarioCharacterId(anchorA);
    }
    if (CycleManager::CheckDistanceBetweenCoordinates() || isVerticalDifferenceLessThanHalf() || CheckPositionDifference() ||
        CycleManager::GetCycleFlagForSpecificChara(g_Conv.selectedOtherCharaId)) {
        const bool crossAxis = g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] != 0;
        Character *const anchorB = crossAxis ? g_Conv.selectedOtherParticipant : g_Conv.cueParticipant0;
        Character *const anchorA = crossAxis ? g_Conv.cachedRyo : g_Conv.cueParticipant1;
        g_Conv.anchorEntityA = anchorA;
        g_Conv.anchorEntityB = anchorB;
        g_Conv.anchorCharaIdB = ScenarioCharacterId(anchorB);
        g_Conv.anchorCharaIdA = ScenarioCharacterId(anchorA);
    }
    if (g_Conv.selectedOtherCharaId != SHEN_NAME(RKKI) || !CycleManager::GetCycleFlagForSpecificChara(SHEN_NAME(RKKI)))
        return;
    Character *const small = CHID_Find(CurrentMapControl()->scenarioTask, SHEN_NAME(SMAL));
    if (!small || (small->motionFlags & kCharacterMotionEnded) == 0)
        return;
    const bool crossAxis = g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] != 0;
    Character *const anchorB = crossAxis ? g_Conv.selectedOtherParticipant : g_Conv.cachedRyo;
    Character *const anchorA = crossAxis ? g_Conv.cachedRyo : g_Conv.selectedOtherParticipant;
    g_Conv.anchorEntityA = anchorA;
    g_Conv.anchorEntityB = anchorB;
    g_Conv.anchorCharaIdB = ScenarioCharacterId(anchorB);
    g_Conv.anchorCharaIdA = ScenarioCharacterId(anchorA);
}

void RecordConvCamSelectionHistory() {
    const std::uint8_t domain = g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex];
    const std::uint8_t write = g_recentConvCamSelectionWriteCursor;
    if (domain <= kConvDomainCrossAxis) {
        g_recentConvCamSelections[write] = {.enabled = 1, .domain = domain, .index = g_Conv.cueCameraSlotTable.slots[g_Conv.activeCueIndex], .reserved = 4};
    } else {
        g_recentConvCamSelectionReadCursor = write;
        g_recentConvCamSelections[write] = {.enabled = 1, .domain = kConvDomainCrossAxis, .index = 3, .reserved = 4};
    }
    g_recentConvCamSelectionWriteCursor = write + 1 >= 4 ? 0 : static_cast<std::uint8_t>(write + 1);
    g_recentConvCamSelectionReadCursor = write >= 4 ? 0 : write;
    const std::uint8_t pair = g_recentConvCamAnchorPairWriteCursor;
    g_convActiveConversation[pair] = {.anchorB = g_Conv.anchorCharaIdB, .anchorA = g_Conv.anchorCharaIdA};
    g_recentConvCamAnchorPairWriteCursor = pair + 1 >= 3 ? 0 : static_cast<std::uint8_t>(pair + 1);
    g_recentConvCamAnchorPairReadCursor = pair >= 3 ? 0 : pair;
}

bool convShouldFlipSide() {
    if (!g_Conv.activeCueIndex || !g_shouldComputeBetweenTwoEntities)
        return false;
    const bool ryoWasAnchorB = g_convActiveConversation[g_recentConvCamAnchorPairReadCursor].anchorB == g_Conv.ryoCharaId;
    const Vector3 *const anchorA = ryoWasAnchorB ? &g_Conv.previousAnchorA : &g_Conv.previousAnchorB;
    const Vector3 *const anchorB = ryoWasAnchorB ? &g_Conv.previousAnchorB : &g_Conv.previousAnchorA;
    Vector3 frame{};
    MatrixStack_WorldToYawFrame(anchorB, anchorA, &g_Conv.transitionStartEye, &frame);
    if (frame.x >= 0.0f && g_Conv.sideFlipSign == -1)
        return true;
    return frame.x < 0.0f && g_Conv.sideFlipSign == 1;
}

void UpdateConversationMirrorState() {
    const ConversationCameraCandidate &selection = g_recentConvCamSelections[g_recentConvCamSelectionReadCursor];
    bool flipped = false;
    if (selection.domain == kConvDomainSameAxis)
        flipped = kConversationSameAxisShots[selection.index].flippedSign == 1;
    else if (selection.domain == kConvDomainCrossAxis)
        flipped = kConversationAuthorSetups[selection.index].closeRange.flippedSign == 1;
    if (flipped)
        g_Conv.sideFlipSign = static_cast<std::int8_t>(-g_Conv.sideFlipSign);
    if (convShouldFlipSide())
        g_Conv.sideFlipSign = static_cast<std::int8_t>(-g_Conv.sideFlipSign);

    Vector3 other = ConversationEntityPosition(g_Conv.selectedOtherParticipant);
    ApplyPrePassTransform(&other, &other);
    const bool ryoAnchorA = g_Conv.anchorCharaIdA == g_Conv.ryoCharaId;
    g_Conv.mirrorSecondaryEnabled = 1;
    if (other.z > 0.0f) {
        if (g_Conv.sideFlipSign == 1)
            g_Conv.mirrorSign = ryoAnchorA ? -1 : 1;
        else if (g_Conv.sideFlipSign == -1)
            g_Conv.mirrorSign = ryoAnchorA ? 1 : -1;
    } else if (g_Conv.sideFlipSign == 1) {
        g_Conv.mirrorSign = ryoAnchorA ? 1 : -1;
    } else if (g_Conv.sideFlipSign == -1) {
        g_Conv.mirrorSign = ryoAnchorA ? -1 : 1;
    }
}

float ComputeConversationTransitionEase() {
    const std::int16_t step = ++g_Conv.transitionStepCounter;
    std::uint8_t armed = g_Conv.doorInteractionTransitionArmed;
    bool inverted = g_Conv.invertedCueSetup != 0;
    if (g_Conv.cueDoorInteractionFlags[g_Conv.activeCueIndex] == 1) {
        if (!SCNT_IsTimeLimitComplete()) {
            g_Conv.doorInteractionReadyFrameCount = 0;
        } else if (++g_Conv.doorInteractionReadyFrameCount >= 10 && g_Conv.doorInteractionTransitionArmed == 1) {
            g_Conv.invertedCueSetup = 1;
            armed = 0;
            g_Conv.doorInteractionTransitionArmed = 0;
            g_Conv.conversationDeltaFrameCounter = step;
            inverted = true;
        }
    }
    if (inverted && !SCNT_IsTimeLimitComplete()) {
        std::int8_t baseCue = 0;
        if (g_Conv.initialSideSelectionFlag == 1) {
            baseCue = static_cast<std::int8_t>(g_Conv.timelineCueIndex);
            g_Conv.initialSideSelectionFlag = 0;
            g_Conv.inversionBaseCueIndex = g_Conv.timelineCueIndex;
        } else {
            baseCue = static_cast<std::int8_t>(g_Conv.inversionBaseCueIndex);
        }
        if (static_cast<std::int8_t>(g_Conv.timelineCueIndex) >= baseCue + 1) {
            armed = 1;
            g_Conv.invertedCueSetup = 0;
            g_Conv.doorInteractionTransitionArmed = 1;
            g_Conv.activeCueBlendFrame = 0;
        }
    }
    std::int16_t span = static_cast<std::int16_t>(g_Conv.totalConversationDurationFrames30Hz - g_Conv.conversationDeltaFrameCounter);
    if (g_Conv.totalConversationDurationFrames30Hz - g_Conv.conversationDeltaFrameCounter >= 60)
        span = 60;
    std::int16_t frame = 0;
    if (armed) {
        g_Conv.conversationFrameCounter = 0;
    } else {
        frame = g_Conv.conversationFrameCounter;
        if (g_Conv.conversationFrameCounter <= span) {
            frame = static_cast<std::int16_t>(g_Conv.conversationFrameCounter + 1);
            g_Conv.conversationFrameCounter = frame;
        }
    }
    const float ratio = std::fabs(static_cast<float>(span)) >= kConvEpsilon ? static_cast<float>(frame) / static_cast<float>(span) : 0.0f;
    return ConversationHalfCosineEase(ratio);
}

float ComputeConversationCameraSlotSpanAlpha(std::int16_t duration) {
    const std::uint8_t activeCue = g_Conv.activeCueIndex;
    const std::uint8_t domain = g_Conv.cueCameraSlotTable.domains[activeCue];
    const float frame = static_cast<float>(duration);
    const float cueFrames = static_cast<float>(g_Conv.activeCueDurationFrames30Hz);
    const float ratio = frame / cueFrames;
    float value = 0.0f;
    if (static_cast<std::uint8_t>(domain - kConvDomainDirect0) <= 1) {
        if (ratio > 0.60000002f)
            value = ConversationHalfCosineEase((frame - cueFrames * 0.60000002f) / (cueFrames * 0.39999998f)) * 0.100000024f + 0.89999998f;
        else
            value = ConversationHalfCosineEase(ratio / 0.60000002f) * 0.89999998f;
    } else {
        const float scale = cueFrames / 180.0f;
        value = scale < 1.0f ? ConversationHalfCosineEase(ratio) * scale : ConversationHalfCosineEase(ratio);
    }
    value = FlushConversationEpsilon(value);
    std::uint8_t type = kConvSpanNone;
    for (const ConversationCameraPathSpan &span : g_secondarySlotPathSpanEntries) {
        if (domain == span.domain && g_Conv.cueCameraSlotTable.slots[activeCue] == span.slot)
            type = span.type;
    }
    return kConversationSpanScale[type] * value + kConversationSpanOffset[type];
}

void shenCalcTargetForEntityAB_Neck() {
    mtwkGetCharaBonePosition(g_Conv.anchorEntityB, kMotnHeadBase, &g_Conv.smoothedAnchorB);
    mtwkGetCharaBonePosition(g_Conv.anchorEntityA, kMotnHeadBase, &g_Conv.smoothedAnchorA);
    const Vector3 anchorB = ConversationEntityPosition(g_Conv.anchorEntityB);
    const Vector3 anchorA = ConversationEntityPosition(g_Conv.anchorEntityA);
    if (g_Conv.anchorCharaIdB == g_Conv.ryoCharaId) {
        g_Conv.smoothedAnchorB.x = anchorB.x;
        g_Conv.smoothedAnchorB.z = anchorB.z;
    } else if (g_Conv.anchorCharaIdA == g_Conv.ryoCharaId) {
        g_Conv.smoothedAnchorA.x = anchorA.x;
        g_Conv.smoothedAnchorA.z = anchorA.z;
    }
}

float ConversationSmoothingEase(std::int16_t progress, std::int32_t span) {
    if (span <= 0 || std::fabs(static_cast<float>(span)) < kConvEpsilon)
        return 0.0f;
    return ConversationHalfCosineEase(static_cast<float>(progress) / static_cast<float>(span));
}

void convUpdateAnchorSmoothing() {
    const float verticalScale = g_shouldComputeBetweenTwoEntities ? 0.80000001f : 0.30000001f;
    Vector3 anchorB = ConversationBonePosition(g_Conv.anchorEntityB, kMotnHeadBase);
    Vector3 anchorA = ConversationBonePosition(g_Conv.anchorEntityA, kMotnHeadBase);
    Vector3 offset{};
    MatrixStack_WorldToYawFrame(&anchorB, &anchorA, &g_Conv.smoothedAnchorB, &offset);
    bool stepB = false;
    if (!g_Conv.anchorBSmoothingActive) {
        if (std::fabs(offset.x) > 0.3 || std::fabs(offset.y) > static_cast<double>(verticalScale) * 0.3 || std::fabs(offset.z) > 0.3) {
            g_Conv.anchorBSmoothingStartFrame = g_Conv.activeCueBlendFrame;
            g_Conv.anchorBSmoothingActive = 1;
            stepB = true;
        }
    } else if (std::fabs(offset.x) < 0.01 && std::fabs(offset.y) < 0.01 && std::fabs(offset.z) < 0.01) {
        g_Conv.anchorBSmoothingActive = 0;
    } else {
        stepB = g_Conv.anchorBSmoothingActive == 1;
    }
    std::int16_t progressB = 0;
    if (stepB)
        progressB = ++g_Conv.anchorBSmoothingProgressFrames;
    else
        g_Conv.anchorBSmoothingProgressFrames = 0;
    const float easeB = FlushConversationEpsilon(ConversationSmoothingEase(progressB, g_Conv.activeCueDurationFrames30Hz - g_Conv.anchorBSmoothingStartFrame));
    anchorB = ConversationBonePosition(g_Conv.anchorEntityB, kMotnHeadBase);
    g_Conv.smoothedAnchorB.x = BlendConversationValue(g_Conv.smoothedAnchorB.x, anchorB.x, easeB);
    g_Conv.smoothedAnchorB.y = BlendConversationValue(g_Conv.smoothedAnchorB.y, anchorB.y, easeB);
    g_Conv.smoothedAnchorB.z = BlendConversationValue(g_Conv.smoothedAnchorB.z, anchorB.z, easeB);

    anchorA = ConversationBonePosition(g_Conv.anchorEntityA, kMotnHeadBase);
    anchorB = ConversationBonePosition(g_Conv.anchorEntityB, kMotnHeadBase);
    MatrixStack_WorldToYawFrame(&anchorA, &anchorB, &g_Conv.smoothedAnchorA, &offset);
    std::uint8_t activeA = g_Conv.anchorASmoothingActive;
    if (activeA) {
        if (std::fabs(offset.x) < 0.01 && std::fabs(offset.y) < 0.01) {
            if (std::fabs(offset.z) < 0.01)
                activeA = 0;
            g_Conv.anchorASmoothingActive = activeA;
        }
    } else if (std::fabs(offset.x) > 0.3 || std::fabs(offset.y) > static_cast<double>(verticalScale) * 0.3 || std::fabs(offset.z) > 0.3) {
        activeA = 1;
        g_Conv.anchorASmoothingStartFrame = g_Conv.activeCueBlendFrame;
        g_Conv.anchorASmoothingActive = 1;
    }
    std::int16_t progressA = 0;
    if (g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex]) {
        if (activeA == 1)
            progressA = static_cast<std::int16_t>(g_Conv.anchorASmoothingProgressFrames + 1);
    } else {
        g_Conv.anchorASmoothingActive = 0;
    }
    g_Conv.anchorASmoothingProgressFrames = progressA;
    const float easeA = FlushConversationEpsilon(ConversationSmoothingEase(progressA, g_Conv.activeCueDurationFrames30Hz - g_Conv.anchorASmoothingStartFrame));
    anchorA = ConversationBonePosition(g_Conv.anchorEntityA, kMotnHeadBase);
    g_Conv.smoothedAnchorA.x = BlendConversationValue(g_Conv.smoothedAnchorA.x, anchorA.x, easeA);
    g_Conv.smoothedAnchorA.y = BlendConversationValue(g_Conv.smoothedAnchorA.y, anchorA.y, easeA);
    g_Conv.smoothedAnchorA.z = BlendConversationValue(g_Conv.smoothedAnchorA.z, anchorA.z, easeA);
}

bool shenConvCamIsAnchorBOnSameRyoZSideAsPairA() {
    Character *const previousAnchor = CHID_Find(CurrentMapControl()->scenarioTask, g_convActiveConversation[g_recentConvCamAnchorPairReadCursor].anchorB);
    Vector3 anchorB = ConversationEntityPosition(g_Conv.anchorEntityB);
    Vector3 previous = ConversationEntityPosition(previousAnchor);
    ApplyEntityTransformWithInverse(g_Conv.cachedRyo, &anchorB, &anchorB);
    ApplyEntityTransformWithInverse(g_Conv.cachedRyo, &previous, &previous);
    return 0.0f <= previous.z * anchorB.z;
}

bool shenConvCamShouldUseSecondaryRearParticipantShot() {
    const ConversationCameraAnchorPair &pair = g_convActiveConversation[g_recentConvCamAnchorPairReadCursor];
    if (g_Conv.anchorCharaIdA != pair.anchorA || g_Conv.anchorCharaIdA != g_Conv.ryoCharaId || g_Conv.anchorCharaIdB == pair.anchorB)
        return false;
    if (g_recentConvCamSelections[g_recentConvCamSelectionReadCursor].domain != kConvDomainCrossAxis || !shenConvCamIsAnchorBOnSameRyoZSideAsPairA())
        return false;
    if (shenGetEntityQuadrant() != kQuadrantBackRight && shenGetEntityQuadrant() != kQuadrantBackLeft)
        return false;
    return g_Conv.activeCueIndex != 0;
}

void BuildEnabledConvCamCandidateList() {
    std::int8_t count = 0;
    g_enabledConvCamCandidateCount = 0;
    g_enabledPrimaryConvCamCandidateCount = 0;
    g_enabledSecondaryConvCamCandidateCount = 0;
    for (const ConversationCameraCandidate &candidate : g_convCamCandidates) {
        if (candidate.enabled != 1)
            continue;
        ConversationCameraCandidate &enabled = g_enabledConvCamSlotCandidates[count++];
        enabled.domain = candidate.domain;
        enabled.index = candidate.index;
        enabled.enabled = 1;
    }
    g_enabledConvCamCandidateCount = count;
    std::int8_t primary = 0;
    std::int8_t secondary = 0;
    for (std::int8_t index = 0; index < count; ++index) {
        ConversationCameraCandidate &enabled = g_enabledConvCamSlotCandidates[index];
        enabled.reserved = static_cast<std::uint8_t>(count);
        if (enabled.domain == kConvDomainSameAxis)
            ++primary;
        if (enabled.domain == kConvDomainCrossAxis)
            ++secondary;
    }
    if (count > 0) {
        g_enabledSecondaryConvCamCandidateCount = secondary;
        g_enabledPrimaryConvCamCandidateCount = primary;
    }
}

void SelectConversationCameraSlot() {
    const std::uint8_t activeCue = g_Conv.activeCueIndex;
    std::uint8_t &slot = g_Conv.cueCameraSlotTable.slots[activeCue];
    const std::uint8_t domain = g_Conv.cueCameraSlotTable.domains[activeCue];
    if (domain) {
        if (domain != kConvDomainCrossAxis) {
            g_Conv.specialSlotCursor = g_Conv.specialSlotCursor + 1 >= 15 ? 0 : static_cast<std::uint8_t>(g_Conv.specialSlotCursor + 1);
            slot = g_Conv.specialSlotCursor;
            return;
        }
        const std::int8_t cursor = g_Conv.secondarySlotCursor + 1 >= 31 ? 0 : static_cast<std::int8_t>(g_Conv.secondarySlotCursor + 1);
        g_Conv.secondarySlotCursor = static_cast<std::uint8_t>(cursor);
        if (!g_enabledSecondaryConvCamCandidateCount) {
            slot = static_cast<std::uint8_t>(((cursor & 1) == 0 ? 1 : 0) | 0xa);
            return;
        }
        slot = g_enabledConvCamSlotCandidates[cursor % g_enabledSecondaryConvCamCandidateCount + g_enabledPrimaryConvCamCandidateCount].index;
        return;
    }
    const std::int8_t cursor = g_Conv.primarySlotCursor + 1 >= 15 ? 0 : static_cast<std::int8_t>(g_Conv.primarySlotCursor + 1);
    g_Conv.primarySlotCursor = static_cast<std::uint8_t>(cursor);
    if (g_enabledPrimaryConvCamCandidateCount) {
        slot = g_enabledConvCamSlotCandidates[cursor % g_enabledPrimaryConvCamCandidateCount].index;
        return;
    }
    if (g_Conv.dynamicInteractionCameraActive && g_Conv.singleParticipantFallbackActive) {
        slot = g_Conv.cueDoorInteractionFlags[activeCue] == 1 ? 5 : static_cast<std::uint8_t>((~(2 * cursor) & 2) | 5);
    } else if ((isVerticalDifferenceLessThanHalf() || (CycleManager::DoesCycleEntryHaveFlagsSet(g_Conv.selectedOtherCharaId) && CheckPositionDifference())) &&
        g_Conv.anchorCharaIdB == g_Conv.selectedOtherCharaId) {
        slot = static_cast<double>(g_Conv.anchorDistanceXZ) >= 0.78 ? 12 : 5;
    } else if (g_Conv.cueDoorInteractionFlags[activeCue] == 1) {
        slot = 5;
    } else {
        slot = static_cast<std::uint8_t>((~(2 * g_Conv.primarySlotCursor) & 2) | 5);
    }
}

void ResetConvCamCandidateAndPathSpanTables() {
    for (std::uint8_t index = 0; index < kConvPrimaryCandidateCount; ++index)
        g_convCamCandidates[index] = {.enabled = 1, .domain = kConvDomainSameAxis, .index = index, .reserved = 48};
    for (std::uint8_t index = kConvPrimaryCandidateCount; index < kConvCandidateCount; ++index) {
        const std::uint8_t slot = static_cast<std::uint8_t>(index - kConvPrimaryCandidateCount);
        g_convCamCandidates[index] = {.enabled = 1, .domain = kConvDomainCrossAxis, .index = slot, .reserved = 48};
    }
    for (std::uint8_t index = 0; index < 32; ++index)
        g_secondarySlotPathSpanEntries[index] = {.domain = kConvDomainCrossAxis, .slot = index, .type = kConvSpanNone};
}

void ApplyConvCamAnchorAndMapCandidateFilters() {
    const std::int32_t mac2Class = GetEntityMac2Class(g_Conv.anchorEntityA);
    if (mac2Class == 0)
        DisableConvCamCandidates(kConvCamSlotFilterAnchorAMac2Class0.data());
    else if (mac2Class == 4)
        DisableConvCamCandidates(kConvCamSlotFilterAnchorAMac2Class4.data());
    if ((CurrentMapControl()->mapRuntimeFlags & 1U) == 0)
        DisableConvCamCandidates(kConvCamSlotFilterNoMapRuntimeFlag.data());
}

void ApplyConvCamConversationLengthCandidateFilters() {
    if (g_Conv.conversationCueCount < 5 || g_Conv.participantCount > 2 || g_Conv.activeCueDurationFrames30Hz <= 180.0)
        DisableConvCamCandidates(kConvCamSlotFilterShortOrMultiParticipantConversation.data());
}

void ApplyConvCamTimingAndHistoryCandidateFilters() {
    if (g_Conv.activeCueDurationFrames30Hz <= 180.0)
        DisableConvCamCandidates(kConvCamSlotFilterTableById[0]);
    if (g_shouldComputeBetweenTwoEntities)
        DisableConvCamCandidates(kConvCamSlotFilterTwoEntityConversation.data());
    for (ConversationCameraCandidate &candidate : g_convCamCandidates) {
        for (const ConversationCameraCandidate &selection : g_recentConvCamSelections) {
            if (candidate.domain == selection.domain && candidate.index == selection.index)
                candidate.enabled = 0;
        }
    }
}

void ApplyConvCamCyclePolicyCandidateFilters() {
    for (const ConversationCyclePolicy &policy : kConvCamCycleFilterPolicyByCharaId) {
        if (policy.characterId != g_Conv.anchorCharaIdB)
            continue;
        if (!policy.policy) {
            const bool flagged = !g_Conv.dynamicInteractionCameraActive && g_Conv.anchorCharaIdB == g_Conv.selectedOtherCharaId &&
                CycleManager::DoesCycleEntryHaveFlagsSet(g_Conv.selectedOtherCharaId);
            DisableConvCamCandidatesByFilterId(flagged ? 14 : 13);
            return;
        }
        break;
    }
    DisableConvCamCandidates(kConvCamSlotFilterDefaultCyclePolicy.data());
}

void ApplyCurrentCycleConvCamCandidateFilter() {
    if (g_Conv.anchorCharaIdA != g_Conv.selectedOtherCharaId)
        return;
    const CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(g_Conv.selectedOtherCharaId);
    if (!npc || !npc->cycle->doorAnimActive) {
        const bool movingObjectKind5 = npc && npc->cycle->moveObjRuntime && npc->cycle->moveObjRuntime->moveObjectKind == 5;
        if (!movingObjectKind5 && !isVerticalDifferenceLessThanHalf())
            return;
    }
    DisableConvCamCandidatesByFilterId(11);
}

void ForcePrimaryConversationSlotByDistance() {
    if (!isVerticalDifferenceLessThanHalf()) {
        const CycleManager::NpcRecord *const npc = CycleManager::FindMutableNpcRecord(g_Conv.selectedOtherCharaId);
        if (!npc || !npc->cycle->taggedPayloadList || !CheckPositionDifference())
            return;
    }
    if (g_Conv.anchorCharaIdB != g_Conv.selectedOtherCharaId)
        return;
    const std::uint8_t keep = static_cast<double>(g_Conv.anchorDistanceXZ) >= 0.78 ? 12 : 5;
    for (std::uint8_t index = 0; index < kConvPrimaryCandidateCount; ++index) {
        if (index != keep)
            g_convCamCandidates[index].enabled = 0;
    }
}

void BuildCrossAxisWorldSample(std::uint8_t slot, float alpha, Vector3 &eye, Vector3 &target) {
    Vector3 localEye{};
    Vector3 localTarget{};
    shenBuildConversationCameraLocalTargetEyeAndFov(&localEye, &localTarget, kConvDomainCrossAxis, slot, alpha);
    Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorB, &g_Conv.smoothedAnchorA, &localTarget, &target);
    Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorA, &g_Conv.smoothedAnchorB, &localEye, &eye);
}

void CullPrimaryCandidateAgainstSecondaryPathSamples(std::int8_t filterId, std::uint8_t slot) {
    const ConversationCameraSlotFilterEntry *const filter = kConvCamSlotFilterTableById[filterId];
    std::uint32_t mac2Class = 0;
    std::uint32_t value8 = 0;
    std::uint32_t valueC = 0;
    std::uint32_t value10 = 0;
    const CollisionPointQueryOutputs outputs{.value4 = &mac2Class, .value8 = &value8, .valueC = &valueC, .value10 = &value10};
    for (std::int8_t entry = 0; entry < static_cast<std::int8_t>(filter[0].count); ++entry) {
        const std::uint8_t secondarySlot = filter[entry].index;
        if (g_convCamCandidates[kConvPrimaryCandidateCount + secondarySlot].enabled != 1)
            continue;
        const std::uint8_t type = g_secondarySlotPathSpanEntries[secondarySlot].type;
        const float offset = kConversationSpanOffset[type];
        const float step = (kConversationSpanScale[type] - offset) * 0.25f;
        std::array<Vector3, 3> eyes{};
        Vector3 target{};
        BuildCrossAxisWorldSample(secondarySlot, step * 0.0f + offset, eyes[0], target);
        BuildCrossAxisWorldSample(secondarySlot, step + step + offset, eyes[1], target);
        BuildCrossAxisWorldSample(secondarySlot, step * 4.0f + offset, eyes[2], target);
        for (Vector3 &eye : eyes) {
            if (g_Conv.anchorEntityA->entity.node.child)
                eye = WorldToEntityLocalPoint(*g_Conv.anchorEntityA->entity.node.child, eye);
        }
        const std::uint32_t mac2 = SHEN_NAME(MAC2);
        const bool inside = CheckIsPointIntersectWith(eyes[0], &outputs, mac2) || CheckIsPointIntersectWith(eyes[1], &outputs, mac2) ||
            CheckIsPointIntersectWith(eyes[2], &outputs, mac2);
        ConversationCameraCandidate &candidate = g_convCamCandidates[slot];
        const bool matches = candidate.domain == filter[entry].domain && candidate.index == filter[entry].index;
        if (inside && static_cast<std::int32_t>(mac2Class) == filterId - 5 && matches)
            candidate.enabled = 0;
    }
}

void CullPrimaryCandidatesAgainstSecondaryPathSpans() {
    for (std::uint8_t slot = kConvPrimaryCandidateCount; slot < kConvCandidateCount; ++slot) {
        if (g_convCamCandidates[slot].enabled != 1)
            continue;
        for (std::int8_t filterId = 5; filterId <= 10; ++filterId)
            CullPrimaryCandidateAgainstSecondaryPathSamples(filterId, slot);
    }
}

void CullSecondaryCandidatesByPathCollision() {
    if (g_Conv.cueDoorInteractionFlags[g_Conv.activeCueIndex] != 1)
        return;
    std::array<std::uint8_t, 0x20> mask{};
    std::memcpy(mask.data(), CurrentMapControl()->cycleMask, mask.size());
    mask[1] &= 0xfcU;
    mask[0] &= 0x7fU;
    for (std::uint8_t slot = 0; slot < 32; ++slot) {
        if (g_convCamCandidates[kConvPrimaryCandidateCount + slot].enabled != 1)
            continue;
        const std::uint8_t type = g_secondarySlotPathSpanEntries[slot].type;
        const float offset = kConversationSpanOffset[type];
        const float step = (kConversationSpanScale[type] - offset) * 0.25f;
        const std::array<float, 5> alphas{step * 0.0f + offset, step + offset, step + step + offset, step * 3.0f + offset, step * 4.0f + offset};
        const std::array<float, 5> eases{0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
        std::array<Vector3, 5> eyes{};
        std::array<Vector3, 5> targets{};
        for (std::size_t sample = 0; sample < 5; ++sample) {
            BuildCrossAxisWorldSample(slot, alphas[sample], eyes[sample], targets[sample]);
            FinalizeWorldTransform(&targets[sample], &targets[sample], &eyes[sample], &eyes[sample], eases[sample], true);
        }
        std::array<std::int32_t, 19> hits{};
        std::size_t hit = 0;
        for (std::size_t sample = 0; sample < 4; ++sample)
            hits[hit++] = ctrlCheckColi(eyes[sample], eyes[sample + 1], 0.22f, mask.data());
        for (const Vector3 &eye : eyes)
            hits[hit++] = ctrlCheckColi(eye, g_Conv.smoothedAnchorA, 0.22f, mask.data());
        for (const Vector3 &eye : eyes)
            hits[hit++] = ctrlCheckColi(eye, g_Conv.smoothedAnchorB, 0.22f, mask.data());
        for (std::size_t sample = 0; sample < 5; ++sample)
            hits[hit++] = ctrlCheckColi(eyes[sample], targets[sample], 0.22f, mask.data());
        for (const std::int32_t result : hits) {
            if (result == 1) {
                DisableCrossAxisCandidate(slot);
                break;
            }
        }
    }
}

void CullConversationCameraSlotsByVerticalRange() {
    const float otherRootY = ConversationBonePosition(g_Conv.selectedOtherParticipant, kMotnRoot).y;
    const float ryoHeadY = ConversationBonePosition(g_Conv.cachedRyo, kMotnHeadBase).y;
    if ((CurrentMapControl()->mapRuntimeFlags & 1U) != 0 && GetEntityMac2Class(g_Conv.anchorEntityA) == -1 && GetEntityMac2Class(g_Conv.anchorEntityB) == -1 &&
        !(otherRootY < 0.87 && CheckPositionDifference())) {
        return;
    }
    const double ceiling = static_cast<double>(ryoHeadY) + 0.8;
    for (std::uint8_t slot = 0; slot < 32; ++slot) {
        const std::uint8_t type = g_secondarySlotPathSpanEntries[slot].type;
        const float offset = kConversationSpanOffset[type];
        const float step = (kConversationSpanScale[type] - offset) * 0.25f;
        Vector3 startEye{};
        Vector3 startTarget{};
        Vector3 endEye{};
        Vector3 endTarget{};
        BuildCrossAxisWorldSample(slot, step * 0.0f + offset, startEye, startTarget);
        BuildCrossAxisWorldSample(slot, step * 4.0f + offset, endEye, endTarget);
        if (startEye.y > ceiling || ryoHeadY - 0.60000002f > startEye.y || endEye.y > ceiling || ryoHeadY - 0.60000002f > endEye.y)
            g_convCamCandidates[kConvPrimaryCandidateCount + slot].enabled = 0;
    }
}

bool IsVectorWithinAngleThreshold(const Vector3 *point, float radius) {
    const std::array<std::uint8_t, 0x20> mask = ConversationCollisionMask();
    SelectCharacterMapQuery(g_Conv.cachedRyo);
    Vector3 local = *point;
    if (g_Conv.cachedRyo->entity.node.child)
        local = WorldToEntityLocalPoint(*g_Conv.cachedRyo->entity.node.child, local);
    QueryShape query{.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange};
    query.packed[0] = {radius, local.x, local.z};
    return ExecuteMathFnWrapper(&query, mask.data()) != 0;
}

bool IsNearSimSetObjectOrMapCircle(const Vector3 &point) {
    CycleManager::NpcRecord *npc = CycleManager::NextProgramInList(CycleProgramList_SimSet);
    for (; npc; npc = CycleManager::NextProgramInList(CycleProgramList_SimSet, npc)) {
        Vector3 local{};
        ApplyEntityTransformWithInverse(npc->cycle->obj, &point, &local);
        const float radius = npc->cycle->steeringHalfWidth <= npc->cycle->steeringHalfDepth ? npc->cycle->steeringHalfDepth : npc->cycle->steeringHalfWidth;
        local.y = 0.0f;
        if (radius >= std::sqrt((local.x * local.x + 0.0f) + local.z * local.z))
            return true;
    }
    return IsVectorWithinAngleThreshold(&point, 0.25f);
}

void ComputeSecondarySlotPathSpans() {
    const ConversationCameraSlotFilterEntry *const shortTransition = kConvCamSlotFilterTableById[0];
    std::array<std::uint8_t, 0x20> mask{};
    std::memcpy(mask.data(), CurrentMapControl()->cycleMask, mask.size());
    if (TestConversationParticipantLineCollision() == 1)
        mask[0] &= 0x7fU;
    mask[1] &= 0xfcU;

    for (std::uint8_t slot = 0; slot < kConvPrimaryCandidateCount; ++slot) {
        Vector3 localEye{};
        Vector3 localTarget{};
        Vector3 startEye{};
        Vector3 startTarget{};
        shenBuildConversationCameraLocalTargetEyeAndFov(&localEye, &localTarget, kConvDomainSameAxis, slot, 0.0f);
        if (g_Conv.singleParticipantFallbackActive) {
            motmApplyRootMot(g_Conv.cachedRyo, localTarget, startTarget);
            motmApplyRootMot(g_Conv.cachedRyo, localEye, startEye);
        } else {
            Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorB, &g_Conv.smoothedAnchorA, &localTarget, &startTarget);
            Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorB, &g_Conv.smoothedAnchorA, &localEye, &startEye);
        }
        Vector3 endEye{};
        Vector3 endTarget{};
        shenBuildConversationCameraLocalTargetEyeAndFov(&localEye, &localTarget, kConvDomainSameAxis, slot, 1.0f);
        if (g_Conv.singleParticipantFallbackActive) {
            motmApplyRootMot(g_Conv.cachedRyo, localTarget, endTarget);
            motmApplyRootMot(g_Conv.cachedRyo, localEye, endEye);
        } else {
            Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorB, &g_Conv.smoothedAnchorA, &localTarget, &endTarget);
            Vector3_ApplyTransRotY(&g_Conv.smoothedAnchorB, &g_Conv.smoothedAnchorA, &localEye, &endEye);
        }
        if (ctrlCheckColi(startEye, endEye, 0.22f, mask.data()))
            g_convCamCandidates[slot].enabled = 0;
    }

    for (std::uint8_t slot = 0; slot < 32; ++slot) {
        if (g_convCamCandidates[kConvPrimaryCandidateCount + slot].enabled != 1)
            continue;
        std::array<Vector3, 5> eyes{};
        std::array<Vector3, 5> targets{};
        constexpr std::array<float, 5> kSampleAlphas{0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
        for (std::size_t sample = 0; sample < 5; ++sample)
            BuildCrossAxisWorldSample(slot, kSampleAlphas[sample], eyes[sample], targets[sample]);
        const auto hitsLine = [&](const Vector3 &start, const Vector3 &end) { return ctrlCheckColi(start, end, 0.22f, mask.data()) != 0; };
        const bool eye0A = hitsLine(eyes[0], g_Conv.smoothedAnchorA);
        const bool eye1A = hitsLine(eyes[1], g_Conv.smoothedAnchorA);
        const bool eye2A = hitsLine(eyes[2], g_Conv.smoothedAnchorA);
        const bool eye3A = hitsLine(eyes[3], g_Conv.smoothedAnchorA);
        const bool eye4A = hitsLine(eyes[4], g_Conv.smoothedAnchorA);
        const bool eye0B = hitsLine(eyes[0], g_Conv.smoothedAnchorB);
        const bool eye1B = hitsLine(eyes[1], g_Conv.smoothedAnchorB);
        const bool eye2B = hitsLine(eyes[2], g_Conv.smoothedAnchorB);
        const bool eye3B = hitsLine(eyes[3], g_Conv.smoothedAnchorB);
        const bool eye4B = hitsLine(eyes[4], g_Conv.smoothedAnchorB);
        const bool path01 = hitsLine(eyes[0], eyes[1]);
        const bool path12 = hitsLine(eyes[1], eyes[2]);
        const bool path23 = hitsLine(eyes[2], eyes[3]);
        const bool path34 = hitsLine(eyes[3], eyes[4]);
        const bool sight0 = hitsLine(eyes[0], targets[0]);
        const bool sight1 = hitsLine(eyes[1], targets[1]);
        const bool sight2 = hitsLine(eyes[2], targets[2]);
        const bool sight3 = hitsLine(eyes[3], targets[3]);
        const bool sight4 = hitsLine(eyes[4], targets[4]);
        const bool near0 = IsNearSimSetObjectOrMapCircle(eyes[0]);
        const bool near1 = IsNearSimSetObjectOrMapCircle(eyes[1]);
        const bool near2 = IsNearSimSetObjectOrMapCircle(eyes[2]);
        const bool near3 = IsNearSimSetObjectOrMapCircle(eyes[3]);
        const bool near4 = IsNearSimSetObjectOrMapCircle(eyes[4]);

        std::uint8_t first = !path01 && !sight0 && !sight1 && !eye0A && !eye1A && !eye0B && !eye1B && !near0 && !near1 ? 1 : 0;
        std::uint8_t second = !path12 && !sight1 && !sight2 && !eye1A && !eye2A && !eye1B && !eye2B && !near1 && !near2 ? 2 : 0;
        std::uint8_t third = !path23 && !sight2 && !sight3 && !eye2A && !eye3A && !eye2B && !eye3B && !near2 && !near3 ? 4 : 0;
        std::uint8_t fourth = !path34 && !sight4 && !sight3 && !eye3A && !eye4A && !eye3B && !eye4B && !near3 && !near4 ? 8 : 0;
        for (std::int8_t entry = 0; entry < static_cast<std::int8_t>(shortTransition[0].count); ++entry) {
            if (shortTransition[entry].domain == kConvDomainCrossAxis && shortTransition[entry].index == slot && (!first || !second || !third || !fourth)) {
                first = 0;
                second = 0;
                third = 0;
                fourth = 0;
            }
        }
        ConversationCameraPathSpan &span = g_secondarySlotPathSpanEntries[slot];
        switch (first | second | third | fourth) {
        case 0:
            span.type = kConvSpanNone;
            break;
        case 1:
        case 5:
        case 9:
            span.type = kConvSpan00To25;
            break;
        case 2:
        case 10:
            span.type = kConvSpan25To50;
            break;
        case 3:
        case 11:
            span.type = kConvSpan00To50;
            break;
        case 4:
            span.type = kConvSpan50To75;
            break;
        case 6:
            span.type = kConvSpan25To75;
            break;
        case 7:
            span.type = kConvSpan00To75;
            break;
        case 8:
            span.type = kConvSpan75To100;
            break;
        case 12:
        case 13:
            span.type = kConvSpan50To100;
            break;
        case 14:
            span.type = kConvSpan25To100;
            break;
        case 15:
            span.type = kConvSpan00To100;
            break;
        default:
            break;
        }
        span.domain = kConvDomainCrossAxis;
        span.slot = slot;
        if (span.type == kConvSpanNone)
            DisableCrossAxisCandidate(slot);
    }
}

void BuildEligibleConversationCameraSlots() {
    const std::uint8_t activeCue = g_Conv.activeCueIndex;
    std::uint8_t primaryEnabled15 = g_convCamCandidates[15].enabled;
    g_convCamCandidates[kConvPrimaryCandidateCount + 15].enabled = 0;
    std::uint8_t secondaryEnabled16 = g_convCamCandidates[kConvPrimaryCandidateCount + 16].enabled;
    if (!g_Conv.cueDoorInteractionFlags[activeCue])
        primaryEnabled15 = 0;
    g_convCamCandidates[15].enabled = primaryEnabled15;
    if (g_Conv.participantCount > 2)
        secondaryEnabled16 = 0;
    g_convCamCandidates[kConvPrimaryCandidateCount + 16].enabled = secondaryEnabled16;
    ApplyConvCamAnchorAndMapCandidateFilters();
    ApplyConvCamConversationLengthCandidateFilters();
    ApplyConvCamTimingAndHistoryCandidateFilters();
    if (g_Conv.cueDoorInteractionFlags[g_Conv.activeCueIndex] == 1)
        DisableConvCamCandidates(kConvCamSlotFilterDoorInteractionCue.data());
    ApplyConvCamCyclePolicyCandidateFilters();
    if (!g_Conv.dynamicInteractionCameraActive) {
        ForcePrimaryConversationSlotByDistance();
        if (g_Conv.selectedOtherCharaId == SHEN_NAME(DOOR))
            DisableConvCamCandidates(kConvCamSlotFilterCurrentStaticDoorActor.data());
    }
    ApplyCurrentCycleConvCamCandidateFilter();
    ComputeSecondarySlotPathSpans();
    CullPrimaryCandidatesAgainstSecondaryPathSpans();
    if (!g_Conv.dynamicInteractionCameraActive)
        CullSecondaryCandidatesByPathCollision();
    CullConversationCameraSlotsByVerticalRange();
}

} // namespace

void convCamInit() {
    std::memset(&g_Conv, 0, sizeof(g_Conv));
    g_convCamMode = 0;
    for (ConversationCameraCandidate &selection : g_recentConvCamSelections)
        selection.enabled = 1;
    g_recentConvCamSelectionWriteCursor = 0;
    g_recentConvCamSelectionReadCursor = 0;
    ResetConvCamCandidateAndPathSpanTables();
    for (ConversationCameraAnchorPair &pair : g_convActiveConversation)
        pair = {};
    g_recentConvCamAnchorPairWriteCursor = 0;
    g_recentConvCamAnchorPairReadCursor = 0;
    g_Conv.cueEndReached = 0;
    g_Conv.doorInteractionReadyFrameCount = 0;
    g_Conv.mirrorSign = 1;
    g_Conv.sideFlipSign = 1;
    g_Conv.cueStartCount = 0;
}

void convUpdateCameraPos() {
    g_Conv.cachedRyo = CurrentRyoCharacter();
    g_Conv.dynamicInteractionCameraActive = 0;
    g_Conv.elapsedCueFrames30Hz = shenGetElapsedCueFrames30Hz();
    g_Conv.transitionEaseYaw = ComputeConversationTransitionEase();
    std::uint8_t timelineCueIndex = g_Conv.timelineCueIndex;
    g_Conv.cueEndReached = 0;
    bool advance = false;
    if (g_Conv.timelineCueIndex < g_Conv.conversationCueCount - 1) {
        const double cueFrames = static_cast<double>(g_Conv.conversationCues[g_Conv.timelineCueIndex]->durationSeconds) * 30.0;
        g_Conv.cueEndReached = static_cast<double>(g_Conv.elapsedCueFrames30Hz) >= cueFrames - 1.0;
        advance = static_cast<double>(g_Conv.elapsedCueFrames30Hz) >= cueFrames;
    }
    if (advance) {
        UpdateFrameTiming();
        timelineCueIndex = static_cast<std::uint8_t>(g_Conv.timelineCueIndex + 1);
        g_Conv.transitionStartEye = g_CameraEyePos;
        g_Conv.transitionStartFovAngle16 = currentPerspectiveFov;
        g_Conv.previousAnchorB = g_Conv.smoothedAnchorB;
        g_Conv.transitionStartTarget = g_CameraTargetPos;
        g_Conv.previousAnchorA = g_Conv.smoothedAnchorA;
        ++g_Conv.timelineCueIndex;
        if (!g_Conv.invertedCueSetup)
            g_Conv.activeCueBlendFrame = 0;
        g_Conv.anchorBSmoothingProgressFrames = 0;
        g_Conv.anchorBSmoothingStartFrame = 0;
        g_Conv.anchorASmoothingProgressFrames = 0;
        g_Conv.anchorASmoothingStartFrame = 0;
    }

    std::uint8_t activeCueIndex = g_Conv.activeCueIndex;
    if (!g_Conv.invertedCueSetup)
        activeCueIndex = timelineCueIndex;
    g_Conv.activeCueIndex = activeCueIndex;
    g_Conv.activeCueDurationFrames30Hz = static_cast<std::int16_t>(static_cast<double>(g_Conv.conversationCues[activeCueIndex]->durationSeconds) * 30.0);
    if (!g_Conv.activeCueBlendFrame) {
        ++g_Conv.cueStartCount;
        g_Conv.targetCollisionLockActive = 0;
        g_Conv.anchorBSmoothingActive = 0;
        g_Conv.anchorASmoothingActive = 0;
        g_Conv.autoFovWideningActive = 0;
        g_Conv.autoFovWidenAngle16 = 0;
        g_Conv.anchorCorrectionMagnitude = 0.0f;
        g_Conv.anchorCorrectionScale = 0.0f;
        g_Conv.anchorCorrectionHoldFlags = 0;
        IncrementCamCounter();
        convUpdateForParticipants(g_Conv.activeCueIndex, &g_Conv.cueParticipant0, &g_Conv.cueParticipant1);
        g_Conv.singleParticipantFallbackActive = 0;
        if (cewpGetFlag(g_Conv.cueParticipant0) && cewpGetFlag(g_Conv.cueParticipant1) && g_Conv.cueParticipant0 != g_Conv.cueParticipant1) {
            g_Conv.singleParticipantFallbackActive = 0;
            ResolveConversationCameraParticipants();
            ResolveConvCamAnchorsFromSelectionHistory();
            UpdateConversationMirrorState();
            shenCalcTargetForEntityAB_Neck();
            const float dx = g_Conv.smoothedAnchorB.x - g_Conv.smoothedAnchorA.x;
            const float dz = g_Conv.smoothedAnchorB.z - g_Conv.smoothedAnchorA.z;
            g_Conv.anchorDistanceXZ = std::sqrt(dx * dx + dz * dz);
            g_Conv.anchorHeightDelta = g_Conv.smoothedAnchorA.y - g_Conv.smoothedAnchorB.y;
            ResetConvCamCandidateAndPathSpanTables();
            BuildEligibleConversationCameraSlots();
            BuildEnabledConvCamCandidateList();
            SelectConversationCameraSlot();
            if (g_Conv.activeCueIndex && g_Conv.participantCount >= 3 && shenConvCamShouldUseSecondaryRearParticipantShot())
                g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] = kConvDomainDirect0;
            shenCalcTargetForEntityAB_Neck();
            RecordConvCamSelectionHistory();
            g_Conv.cameraClearanceScale = GetTransformedDistanceBetweenTwoEntities();
        } else {
            g_Conv.singleParticipantFallbackActive = 1;
            g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] = kConvDomainSameAxis;
            ResetConvCamCandidateAndPathSpanTables();
            BuildEnabledConvCamCandidateList();
            SelectConversationCameraSlot();
        }
        activeCueIndex = g_Conv.activeCueIndex;
    }

    const bool ryoVisible = g_Conv.singleParticipantFallbackActive || g_Conv.participantCount >= 3 || g_Conv.cueCameraSlotTable.domains[activeCueIndex] ||
        g_Conv.anchorCharaIdA != g_Conv.ryoCharaId;
    g_Conv.cachedRyo->renderOpacity = ryoVisible ? 1.0f : 0.0f;

    Vector3 eye{};
    Vector3 target{};
    if (g_Conv.singleParticipantFallbackActive) {
        std::int16_t blendFrame = g_Conv.activeCueBlendFrame;
        g_Conv.anchorDistanceXZ = 1.0f;
        g_Conv.anchorHeightDelta = 0.0f;
        if (g_Conv.activeCueBlendFrame <= g_Conv.activeCueDurationFrames30Hz)
            blendFrame = ++g_Conv.activeCueBlendFrame;
        if (g_Conv.invertedCueSetup == 1) {
            if (blendFrame >= g_Conv.activeCueDurationFrames30Hz)
                blendFrame = g_Conv.activeCueDurationFrames30Hz;
            g_Conv.activeCueBlendFrame = blendFrame;
        }
        g_Conv.activeCuePathAlpha = ComputeConversationCameraSlotSpanAlpha(blendFrame);
        convBuildCamera(&eye, &target, g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex], g_Conv.cueCameraSlotTable.slots[g_Conv.activeCueIndex],
            g_Conv.activeCuePathAlpha);
        if (!g_Conv.suppressCameraApply) {
            TranslateCameraRelative(eye);
            if (!bLockCamera) {
                g_CameraTargetPos = target;
                shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
            }
            CAME_SetFieldOfView(g_Conv.conversationFovAngle16);
        }
        return;
    }

    if (g_shouldComputeBetweenTwoEntities) {
        mtwkGetCharaBonePosition(g_Conv.anchorEntityB, kMotnRoot, &target);
        mtwkGetCharaBonePosition(g_Conv.anchorEntityA, kMotnRoot, &eye);
        g_Conv.smoothedAnchorB.x = target.x;
        g_Conv.smoothedAnchorB.z = target.z;
        g_Conv.smoothedAnchorA.x = eye.x;
        g_Conv.smoothedAnchorA.z = eye.z;
    }
    convUpdateAnchorSmoothing();
    const float dx = g_Conv.smoothedAnchorB.x - g_Conv.smoothedAnchorA.x;
    const float dz = g_Conv.smoothedAnchorB.z - g_Conv.smoothedAnchorA.z;
    std::int16_t blendFrame = g_Conv.activeCueBlendFrame;
    g_Conv.anchorDistanceXZ = std::sqrt(dx * dx + dz * dz);
    g_Conv.anchorHeightDelta = g_Conv.smoothedAnchorA.y - g_Conv.smoothedAnchorB.y;
    if (g_Conv.activeCueBlendFrame <= g_Conv.activeCueDurationFrames30Hz)
        blendFrame = ++g_Conv.activeCueBlendFrame;
    if (g_Conv.invertedCueSetup == 1) {
        if (blendFrame >= g_Conv.activeCueDurationFrames30Hz)
            blendFrame = g_Conv.activeCueDurationFrames30Hz;
        g_Conv.activeCueBlendFrame = blendFrame;
    }
    g_Conv.activeCuePathAlpha = ComputeConversationCameraSlotSpanAlpha(blendFrame);
    convBuildCamera(&eye, &target, g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex], g_Conv.cueCameraSlotTable.slots[g_Conv.activeCueIndex],
        g_Conv.activeCuePathAlpha);
    FinalizeWorldTransform(&target, &target, &eye, &eye, g_Conv.transitionEaseYaw, g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] != 0);
    convUpdateCameraCollisionLock(&eye, &eye);
    if (!g_Conv.suppressCameraApply)
        convUpdateCamera(&eye, &target, g_Conv.conversationFovAngle16);
}

void scntStartConversationCamera() {
    g_Conv.activeCueDurationFrames30Hz = 0;
    g_Conv.activeCueBlendFrame = 0;
    g_Conv.totalConversationDurationFrames30Hz = 0;
    g_Conv.transitionStepCounter = 0;
    g_Conv.elapsedCueFrames30Hz = 0.0f;
    g_Conv.activeCuePathAlpha = 0.0f;
    g_Conv.anchorDistanceXZ = 0.0f;
    g_Conv.anchorBSmoothingProgressFrames = 0;
    g_Conv.anchorBSmoothingStartFrame = 0;
    g_Conv.anchorBSmoothingActive = 0;
    g_Conv.anchorASmoothingProgressFrames = 0;
    g_Conv.anchorASmoothingStartFrame = 0;
    g_Conv.anchorASmoothingActive = 0;
    g_Conv.autoFovWideningActive = 0;
    g_Conv.doorInteractionTransitionArmed = 1;
    g_Conv.conversationFrameCounter = 0;
    g_Conv.conversationDeltaFrameCounter = 0;
    g_Conv.transitionEaseYaw = 0.0f;
    g_Conv.doorInteractionReadyFrameCount = 0;
    g_Conv.autoFovWidenAngle16 = 0;
    g_Conv.anchorCorrectionHoldFlags = 0;
    g_Conv.mirrorSecondaryEnabled = 1;
    g_Conv.initialSideSelectionFlag = 1;
    g_Conv.inversionBaseCueIndex = 0;
    g_Conv.compactedLeadCueBiasPending = 0;
    g_Conv.singleParticipantFallbackActive = 0;
    g_Conv.invertedCueSetup = 0;
    g_Conv.cueEndReached = 0;
    g_Conv.dynamicInteractionCameraActive = 0;
    g_Conv.targetCollisionLockActive = 0;
    if (((g_nAuthorState - 6) & ~2) == 0)
        return;

    convUpdateCharaSeqVisibility();
    g_Conv.cachedRyo = CurrentRyoCharacter();
    if (!g_Conv.cachedRyo)
        return;
    g_Conv.conversationCueLoadPtr = g_scntConversationCueWorkbuf.data();
    convCompactCueStream();
    g_Conv.conversationCueLoadPtr = g_scntConversationCueBuffer.data();
    convLoadCueStream();
    g_Conv.activeCueIndex = 0;
    g_Conv.preserveInitialParticipantOrder = (SCEN_GetActiveRuntimeObjectFlags() & 0x400000U) != 0 ? 1 : 0;
    g_convCamMode = 0;
    g_Conv.suppressCameraApply = 0;
    for (std::uint8_t cue = 0; cue < g_Conv.conversationCueCount; ++cue)
        g_Conv.cueDoorInteractionFlags[cue] = 0;

    if (g_Conv.cueStartCount) {
        g_Conv.initialCueAnchorOverride = 0;
    } else {
        convUpdateForParticipants(0, &g_Conv.cueParticipant0, &g_Conv.cueParticipant1);
        const bool doorParticipant =
            ScenarioCharacterId(g_Conv.cueParticipant0) == SHEN_NAME(DOOR) || ScenarioCharacterId(g_Conv.cueParticipant1) == SHEN_NAME(DOOR);
        if (doorParticipant) {
            g_Conv.sideFlipSign = g_conversationCueParticipantSign ? -1 : 1;
            g_Conv.mirrorSign = g_Conv.sideFlipSign;
        } else {
            g_Conv.initialRyoWorldPos = ConversationEntityPosition(g_Conv.cachedRyo);
            g_Conv.initialRyoYaw = g_Conv.cachedRyo ? g_Conv.cachedRyo->entity.node.rotation.y : 0;
            const std::int8_t sign = ChooseConversationSideSignByCollision();
            g_Conv.mirrorSign = sign;
            g_Conv.sideFlipSign = sign == 1 ? 1 : -1;
        }
        if (ScenarioCharacterId(g_Conv.cueParticipant0) == SHEN_NAME(TOSK) || ScenarioCharacterId(g_Conv.cueParticipant1) == SHEN_NAME(TOSK)) {
            g_Conv.sideFlipSign = 1;
            g_Conv.mirrorSign = 1;
        }
        g_Conv.initialCueAnchorOverride = 1;
    }

    UpdateSubsTime();
    UpdateFrameTiming();
    HLib_UpdateFrameTiming();
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = 9;
    ApplyPendingGameState(9);
    g_Conv.transitionStartEye = g_CameraEyePos;
    g_Conv.transitionStartFovAngle16 = currentPerspectiveFov;
    g_Conv.transitionStartTarget = g_CameraTargetPos;
    g_Conv.conversationCameraOverrideActive = 1;
    g_Conv.activeCueIndex = 0;
    g_Conv.timelineCueIndex = 0;
    g_bIsConversationCamInactive = 0;
    g_CameraOverrideActiveFlag = 1;
    std::int16_t totalFrames = g_Conv.totalConversationDurationFrames30Hz;
    for (std::uint8_t cue = 0; cue < g_Conv.conversationCueCount; ++cue) {
        const double cueFrames = static_cast<double>(g_Conv.conversationCues[cue]->durationSeconds) * 30.0;
        totalFrames = static_cast<std::int16_t>(static_cast<std::int32_t>(cueFrames + static_cast<double>(totalFrames)));
        g_Conv.totalConversationDurationFrames30Hz = totalFrames;
    }
    convUpdateCameraPos();
}

void convEndConversationCamera() {
    if (!g_Conv.conversationCameraOverrideActive)
        return;
    for (std::uint8_t index = 0; index < g_Conv.participantCount; ++index) {
        Character *const character = g_Conv.participantEntities[index];
        if (!cewpGetFlag(character))
            continue;
        if (g_Conv.participantRestore[index].hadMotmFlag2) {
            MotionManagerWork *const manager = MOTM_GetWork(character);
            if ((manager->flags & 2U) == 0) {
                manager->flags |= 2U;
                ++MBST_GetWork()->state;
                MBST_Refresh(true);
            }
        }
        if (character)
            MOTM_GetWork(character)->motion.poseEvaluationStage = static_cast<std::uint8_t>(g_Conv.participantRestore[index].savedPoseEvalStage);
        if (ScenarioCharacterId(character) != SHEN_NAME(AKIR))
            character->renderOpacity = 1.0f;
    }

    Vector3 eyeLocal = g_CameraEyePos;
    TransformForEntity(g_Conv.cachedRyo, &eyeLocal, &eyeLocal);
    const float eyeHeight = eyeLocal.y;
    const Vector3 forward{0.0f, 0.0f, -1.0f};
    eyeLocal.y = 0.0f;
    const std::int32_t groundAngle = Vector3_AngleBetween(&forward, &eyeLocal);
    const std::uint8_t activeCueIndex = g_Conv.activeCueIndex;
    eyeLocal.y = eyeHeight;
    g_cameraDeltaTimeRampStep = -0.00081176468f;
    eyeLocal.x = 0.0f;
    g_numFramesActive = 85;
    g_convCamMode = 1;
    if (g_Conv.conversationCueCount - 1 == g_Conv.activeCueIndex && groundAngle > 15473 && Vector3_AngleBetween(&forward, &eyeLocal) > 20024 &&
        g_Conv.cueCameraSlotTable.domains[activeCueIndex] == kConvDomainCrossAxis && g_Conv.anchorCharaIdB == g_Conv.selectedOtherCharaId) {
        g_convCamMode = 0;
    }
    g_cam_frm_counter = 60;
    g_bIsConversationCamInactive = 0;
    g_Conv.conversationCameraOverrideActive = 0;
}

void convUpdateCharaSeqVisibility() {
    if (!g_Conv.participantCount)
        return;
    for (std::int32_t index = 0; index < static_cast<std::int8_t>(g_Conv.participantCount); ++index) {
        Character *const character = g_Conv.participantEntities[index];
        if (!cewpGetFlag(character))
            continue;
        if (ScenarioCharacterId(character) == SHEN_NAME(AKIR)) {
            character->renderOpacity = 1.0f;
            continue;
        }
        if (!g_Conv.participantRestore.data()[index].hadCpamBit0 || !character)
            continue;
        if (CharacterComponentNode *const cpam = HLib_GetCharacterComponent(character, COMPONENT_CPAM.name))
            cpam->flags |= 1U;
    }
    g_Conv.participantCount = 0;
}

void convInitStates(std::int32_t state, Character *participantA, Character *participantB, float mirrorSign, float durationFrames30Hz) {
    g_Conv.activeCueDurationFrames30Hz = 0;
    g_Conv.activeCueBlendFrame = 0;
    g_Conv.totalConversationDurationFrames30Hz = 0;
    g_Conv.transitionStepCounter = 0;
    g_Conv.elapsedCueFrames30Hz = 0.0f;
    g_Conv.activeCuePathAlpha = 0.0f;
    g_Conv.anchorDistanceXZ = 0.0f;
    g_Conv.anchorBSmoothingProgressFrames = 0;
    g_Conv.anchorBSmoothingStartFrame = 0;
    g_Conv.anchorBSmoothingActive = 0;
    g_Conv.anchorASmoothingProgressFrames = 0;
    g_Conv.anchorASmoothingStartFrame = 0;
    g_Conv.anchorASmoothingActive = 0;
    g_Conv.doorInteractionTransitionArmed = 1;
    g_Conv.conversationFrameCounter = 0;
    g_Conv.conversationDeltaFrameCounter = 0;
    g_Conv.transitionEaseYaw = 0.0f;
    g_Conv.mirrorSecondaryEnabled = 1;
    g_Conv.initialSideSelectionFlag = 1;
    g_Conv.inversionBaseCueIndex = 0;
    g_Conv.invertedCueSetup = 0;
    g_Conv.dynamicInteractionCameraActive = 1;
    g_Conv.singleParticipantFallbackActive = 0;
    g_Conv.cachedRyo = CurrentRyoCharacter();
    g_Conv.participantCount = 2;
    g_Conv.activeCueIndex = 0;
    g_Conv.scriptedCameraSlotOverride = state;
    g_Conv.conversationCueCount = 1;
    g_Conv.conversationCues[0] = &g_tmpConversationCue;
    g_conversationParticipantA = participantA;
    g_conversationParticipantB = participantB;
    g_tmpConversationCue.flags = 0;
    g_Conv.participantEntities[0] = participantA;
    g_Conv.participantEntities[1] = participantB;
    g_tmpConversationCue.durationSeconds = static_cast<float>(static_cast<double>(durationFrames30Hz) / 30.0);
    g_Conv.mirrorSign = mirrorSign == 0.0f ? ChooseConversationSideSignByCollision() : static_cast<std::int8_t>(static_cast<std::int32_t>(mirrorSign));
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = 7;
    ApplyPendingGameState(7);
    g_Conv.activeCueDurationFrames30Hz = static_cast<std::int16_t>(static_cast<std::int32_t>(durationFrames30Hz));
    UpdateFrameTiming();
    g_Conv.transitionStartEye = g_CameraEyePos;
    g_Conv.transitionStartTarget = g_CameraTargetPos;
    g_bIsConversationCamInactive = 0;
    g_CameraOverrideActiveFlag = 1;
    g_Conv.transitionStartFovAngle16 = currentPerspectiveFov;
}

void HandleDynamicInteractionState() {
    g_Conv.dynamicInteractionCameraActive = 1;
    g_Conv.singleParticipantFallbackActive = 0;
    Character *const ryo = CurrentRyoCharacter();
    if (g_Conv.activeCueBlendFrame >= g_Conv.activeCueDurationFrames30Hz)
        g_Conv.activeCueBlendFrame = g_Conv.activeCueDurationFrames30Hz;
    g_Conv.cachedRyo = ryo;
    g_Conv.cueParticipant0 = g_Conv.participantEntities[0];
    g_Conv.cueParticipant1 = g_Conv.participantEntities[1];
    g_Conv.ryoCharaId = ScenarioCharacterId(ryo);
    g_Conv.selectedOtherParticipant = g_Conv.cueParticipant0 != g_Conv.cachedRyo ? g_Conv.cueParticipant0 : g_Conv.cueParticipant1;
    g_Conv.selectedOtherCharaId = ScenarioCharacterId(g_Conv.selectedOtherParticipant);
    g_Conv.anchorEntityB = g_Conv.participantEntities[0];
    g_Conv.anchorEntityA = g_Conv.participantEntities[1];
    g_Conv.anchorCharaIdB = ScenarioCharacterId(g_Conv.anchorEntityB);
    g_Conv.anchorCharaIdA = ScenarioCharacterId(g_Conv.anchorEntityA);
    g_Conv.mirrorSecondaryEnabled = 1;
    if (!g_Conv.activeCueBlendFrame) {
        shenCalcTargetForEntityAB_Neck();
        g_Conv.anchorDistanceXZ = std::sqrt((g_Conv.smoothedAnchorB.z - g_Conv.smoothedAnchorA.z) * (g_Conv.smoothedAnchorB.z - g_Conv.smoothedAnchorA.z) +
            (g_Conv.smoothedAnchorB.x - g_Conv.smoothedAnchorA.x) * (g_Conv.smoothedAnchorB.x - g_Conv.smoothedAnchorA.x));
        g_Conv.anchorHeightDelta = g_Conv.smoothedAnchorA.y - g_Conv.smoothedAnchorB.y;
        if (g_Conv.scriptedCameraSlotOverride == 0 || g_Conv.scriptedCameraSlotOverride == 1) {
            g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] = g_Conv.scriptedCameraSlotOverride ? kConvDomainCrossAxis : kConvDomainSameAxis;
            ResetConvCamCandidateAndPathSpanTables();
            BuildEligibleConversationCameraSlots();
            BuildEnabledConvCamCandidateList();
            SelectConversationCameraSlot();
            RecordConvCamSelectionHistory();
        } else {
            if (g_Conv.scriptedCameraSlotOverride >= 512)
                g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex] = kConvDomainCrossAxis;
            g_Conv.cueCameraSlotTable.slots[g_Conv.activeCueIndex] = static_cast<std::uint8_t>(g_Conv.scriptedCameraSlotOverride);
        }
    }
    convUpdateAnchorSmoothing();
    const float distance = std::sqrt((g_Conv.smoothedAnchorB.z - g_Conv.smoothedAnchorA.z) * (g_Conv.smoothedAnchorB.z - g_Conv.smoothedAnchorA.z) +
        (g_Conv.smoothedAnchorB.x - g_Conv.smoothedAnchorA.x) * (g_Conv.smoothedAnchorB.x - g_Conv.smoothedAnchorA.x));
    std::int16_t blendFrame = g_Conv.activeCueBlendFrame;
    g_Conv.anchorDistanceXZ = distance;
    g_Conv.anchorHeightDelta = g_Conv.smoothedAnchorA.y - g_Conv.smoothedAnchorB.y;
    if (g_Conv.activeCueBlendFrame <= g_Conv.activeCueDurationFrames30Hz)
        blendFrame = ++g_Conv.activeCueBlendFrame;
    if (g_Conv.invertedCueSetup == 1) {
        if (blendFrame >= g_Conv.activeCueDurationFrames30Hz)
            blendFrame = g_Conv.activeCueDurationFrames30Hz;
        g_Conv.activeCueBlendFrame = blendFrame;
    }
    g_Conv.activeCuePathAlpha = ComputeConversationCameraSlotSpanAlpha(blendFrame);
    Vector3 eye{};
    Vector3 target{};
    convBuildCamera(&eye, &target, g_Conv.cueCameraSlotTable.domains[g_Conv.activeCueIndex], g_Conv.cueCameraSlotTable.slots[g_Conv.activeCueIndex],
        g_Conv.activeCuePathAlpha);
    convUpdateCamera(&eye, &target, g_Conv.conversationFovAngle16);
}

} // namespace shenmue::s1
