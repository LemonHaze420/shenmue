#include "s2/hand_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/motion_work_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include <float.h>
#include <cmath>
#include <cstring>

#define SHENMUE_HAND_NULL 0
#define SHENMUE_HAND_INITIALIZE =
#define SHENMUE_HAND_MAX_FLOAT FLT_MAX
#define SHENMUE_HAND_FMA(a, b, c) ((a) * (b) + (c))
#include "s2/file_loader_dc.hpp"
#include "s2/pak_dc.hpp"
#include <cstdio>
#include "platform/dc/matrix.hpp"
#include "s2/model_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/player_model_dc.hpp"
#endif
#include "shenmue.h"
#include "s2/character_transparency_dc.hpp"
#include "s2/math.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/hand_draw_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/model_render_dc.hpp"
namespace shenmue {
namespace s2 {

namespace {

std::int16_t HandSignedAngle(std::uint16_t bits);

} // namespace

extern const char handModelSuffixes[2][8];
const char handModelSuffixes[2][8] = {"_TL.MT7", "_TR.MT7"};

std::int32_t CompareCharacterTableTokens(const char *left, const char *right);

void BuildHandPoseMatricesNoPublish(std::int32_t frame, HandPoseState *state, Matrix4x4 *matrices);
void UpdateHand(Character *character, void *payload);

void InitializeHandPoseNullsub();

void ApplyHandBodyTransform(Character *character, CharacterHandComponent *hand);

void CharacterHandDrawClosure(Character *character);

void CharacterHandDirectCore(Character *character);

void UpdateHandGripBounds(CharacterHandComponent *hand);

void ResetCharacterHandComponent(CharacterHandComponent &hand);

void ConfigureHandPoseSequence(CharacterHandComponent &hand, std::int16_t pose, std::int32_t first, std::int32_t last, std::int16_t start, std::uint16_t flags,
    float step, std::int16_t ticks);

void BuildHandGripGeometry(CharacterHandComponent *hand, MT7Node *node);

void UpdateMeshHandGripBounds(CharacterHandComponent *hand);

void AdvanceGripAngles(CharacterHandComponent *hand, std::int32_t collide, std::uint32_t flags);

std::int32_t hndlDrawPrimaryHand(CharacterHandComponent *hand, float alpha);

std::int32_t hndlDrawSecondaryHand(CharacterHandComponent *hand, float alpha);

std::int32_t InterpolateHandPoseChannel(std::int32_t count, const HandCubicKeyframe *keys, std::int32_t frame);

void GetHandPrimaryNodeRotation(const CharacterHandComponent &hand, Vector3i &rotation);

CharacterHandComponent *GetCharacterHand(Character *character, std::int32_t side);

void GetHandPrimaryNodeRotation(const CharacterHandComponent &hand, Vector3i &rotation);

MT7Model *QueryHandModelCacheNullsub(const char *);

std::uint32_t GetNumHandModelChunks(const HandMeshHeader *mesh);

void SetHandPrimaryRotation(CharacterHandComponent *hand, std::int32_t x, std::int32_t y, std::int32_t z);

#define SHENMUE_HAND_ANGLE HandSignedAngle

HandPoseBank handPoseBanks SHENMUE_HAND_INITIALIZE{};

Matrix4x4 *handPoseMatrices = SHENMUE_HAND_NULL;

void *handCalculationWork = SHENMUE_HAND_NULL, *handProbeWork = SHENMUE_HAND_NULL;

CharacterHandComponent *g_temporaryHNDL = SHENMUE_HAND_NULL, *g_temporaryHNDR = SHENMUE_HAND_NULL;

const HandPoseModelJoint *g_handPoseJointCursor = SHENMUE_HAND_NULL;

const HandPoseChannel *g_handPoseChannelCursor = SHENMUE_HAND_NULL;

std::int16_t (*g_handPoseOutputAngles)[3] = SHENMUE_HAND_NULL;

std::int32_t g_handPoseEvaluationFrame;

Character *g_searchHandCharacter = SHENMUE_HAND_NULL;

MotionWork *g_searchHandMotionWork = SHENMUE_HAND_NULL;

MotionOverride g_searchHandMotionOverride SHENMUE_HAND_INITIALIZE{};

std::int32_t g_searchHandOverrideEnabled = 0, g_searchHandChestOverrideEnabled = 0;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
HandIKConfig g_searchHandConfigurations[2] SHENMUE_HAND_INITIALIZE{};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t g_searchHandSolveStatus = 0, g_searchHandPreviousSolveStatus = 0;
#endif

std::int16_t handGripMeshWords;

std::int32_t gripDisabled = 0;

namespace {

std::int16_t HandSignedAngle(std::uint16_t bits) {
    std::int16_t value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkIsSearchHandActive(const MotionWork *work) {
    if (work == g_searchHandMotionWork) {
        if ((work->sequenceFlags & MSF_SearchHand) == 0)
            return false;
        return true;
    }
    return false;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkClearSearchHandRequests(MotionWork *work) {
    if (!mtwkIsSearchHandActive(work))
        return false;
    for (std::int32_t region = 0; region < 5; ++region) {
        work->queuedMotionId[region] = 0;
        work->pendingLayerStartFrame[region] = -1.0f;
        work->layerLoopStartTime[region] = -1.0f;
        work->layerEndFrame[region] = -1.0f;
        work->parentBoneSelector = 9;
        work->requestedRootYawOffset = 0;
        work->overrideBlendTime = -1.0f;
    }
    for (std::int32_t target = 0; target < 9; ++target) {
        work->activeConstraintTarget[target] = SHENMUE_HAND_NULL;
        work->latchedConstraintTarget[target] = SHENMUE_HAND_NULL;
    }
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkAssignCallbacks(MotionWork *work) {
    if (mtwkIsSearchHandActive(work))
        SetCustomMotionOverride(&g_searchHandMotionOverride);
    g_searchHandPreviousSolveStatus = g_searchHandSolveStatus;
    g_searchHandSolveStatus = 0;
}
#endif

// @verified - d
void InitializeHandWork() {
    g_temporaryHNDL = g_temporaryHNDR = 0;
    handPoseMatrices = static_cast<Matrix4x4 *>(HLib::AllocateStaticOrDynamicBlock(0x1200, HAND_MATRIX_MEMORY));
    handPoseBanks.poses = 0;
    handPoseBanks.next = 0;
    handPoseBanks.previous = 0;
    handPoseBanks.package = 0;
    handCalculationWork = static_cast<std::uint32_t *>(HLib::AllocateStaticOrDynamicBlock(0x18000, HAND_CALCULATION_MEMORY));
    handProbeWork = static_cast<HandGripProbe *>(HLib::AllocateStaticOrDynamicBlock(0xa0, HAND_CALCULATION_MEMORY));
    InitializeHandPoseNullsub();
}

// @verified - d
void ResetSearchHandState() {
    g_searchHandCharacter = SHENMUE_HAND_NULL;
    g_searchHandMotionWork = SHENMUE_HAND_NULL;
    g_searchHandOverrideEnabled = 0;
    g_searchHandChestOverrideEnabled = 0;
}

// @verified - d
HandIKMode getSearchHandConfigModeFor(std::uint8_t side) {
    return g_searchHandMotionWork ? static_cast<HandIKMode>(g_searchHandConfigurations[side].mode) : HandIK_Unavailable;
}

void *LoadHandPoseBank(std::int32_t act, shen_name area, const char *filename) {
    if (!handPoseMatrices || !handCalculationWork)
        return 0;
    PakGlobalData *package = 0;
    std::int32_t *data = 0;
    do {
        package = GetNextReadyModelPackage(package);
        if (!package)
            break;
        data = static_cast<std::int32_t *>(FileLoader(package, filename, PACKAGE_BINARY_EXTENSION));
    } while (!data);
    if (!data) {
        char directory[64];
        const std::uint32_t areaName = static_cast<std::uint32_t>(area);
        std::sprintf(directory, "scene/%02d/%c%c%c%c", act, static_cast<std::int8_t>(areaName), static_cast<std::int8_t>(areaName >> 8),
            static_cast<std::int8_t>(static_cast<std::int32_t>(areaName << 8) >> 24), static_cast<std::int8_t>(static_cast<std::int32_t>(areaName) >> 24));
        data = static_cast<std::int32_t *>(ReadFile(directory, filename, HAND_BANK_MEMORY));
    }
    if (!data)
        return 0;
    HandPoseBank *tail = &handPoseBanks;
    while (tail->next)
        tail = tail->next;
    HandPoseBank *const bank = static_cast<HandPoseBank *>(HLib::AllocateStaticOrDynamicBlock(sizeof(HandPoseBank), HAND_BANK_MEMORY));
    tail->next = bank;
    if (!bank)
        return 0;
    bank->previous = tail;
    bank->next = 0;
    bank->poses = reinterpret_cast<const HandPoseSequenceHeader **>(data);
    bank->package = package;
    std::byte *const base = reinterpret_cast<std::byte *>(data);
    std::int32_t index = *data++;
    bank->first = static_cast<std::int16_t>(index / 65536);
    bank->last = static_cast<std::int16_t>(index % 65536);
    const std::int16_t *const limits = &bank->last;
    std::int32_t count = *limits;
    count -= limits[-1];
    ++count;
    if (static_cast<std::uint32_t>(*data) & 0xff000000U)
        return base;
    HandPoseSequenceHeader *sequence;
    std::int32_t jointCount;
    std::int32_t *channels;
    std::int32_t *cursor;
    std::int32_t channel;
    for (index = 0; index < count; ++index, ++data) {
        *data = reinterpret_cast<std::int32_t>(base + *data);
        sequence = reinterpret_cast<HandPoseSequenceHeader *>(*data);
        std::memcpy(&jointCount, &sequence->jointCount, sizeof(jointCount));
        channels = reinterpret_cast<std::int32_t *>(sequence + 1);
        cursor = channels;
        for (channel = 0; channel < 3 * jointCount; ++channel) {
            if (*cursor++ > 0)
                *cursor += reinterpret_cast<std::int32_t>(channels);
            ++cursor;
        }
    }
    return base;
}

// @verified - d
std::int32_t ReleaseHandPoseBank(void *data) {
    if (!data)
        return 1;
    HandPoseBank *bank = handPoseBanks.next;
    while (bank && data != bank->poses)
        bank = bank->next;
    if (!bank)
        return 0;
    if (!bank->package)
        HLib::FreeTaskData(bank->poses);
    if (bank->next)
        bank->next->previous = bank->previous;
    if (bank->previous)
        bank->previous->next = bank->next;
    HLib::FreeTaskData(bank);
    return 1;
}

// @verified - d
void AttachCharacterHandComponents(Character *character, const char *left, const char *right, std::int32_t flags) {
    if (!handPoseMatrices || !handCalculationWork)
        return;
    char names[2][16];
    if (left || right) {
        std::strncpy(names[0], left, 15);
        std::strncpy(names[1], right, 15);
    } else {
        const std::int32_t prefix = reinterpret_cast<std::int32_t>(GetImageModuleSelectedTransform(character));
        names[0][0] = names[1][0] = static_cast<char>(prefix);
        names[0][1] = names[1][1] = static_cast<char>(prefix >> 8);
        names[0][2] = names[1][2] = static_cast<char>(prefix >> 16);
        names[0][3] = names[1][3] = '\0';
        std::strcat(names[0], handModelSuffixes[0]);
        std::strcat(names[1], handModelSuffixes[1]);
    }
    names[0][15] = names[1][15] = '\0';
    if (names[0][9] == '5')
        names[0][9] = '7';
    if (names[1][9] == '5')
        names[1][9] = '7';
    void (*previous)(Character *) = 0;
    if (CompareCharacterTableTokens(names[0], names[1])) {
        if (names[0][0] || names[1][0])
            previous = charaAssignCoreCallback(character, CharacterHandDrawClosure);
        if (names[0][0]) {
            CharacterHandComponent *const hand =
                static_cast<CharacterHandComponent *>(AllocateModelComponent(character, CHARACTER_LEFT_HAND, sizeof(CharacterHandComponent), UpdateHand));
            SetModelComponentCleanup(character, CHARACTER_LEFT_HAND, CleanupHand);
            if (hand) {
                ResetCharacterHandComponent(*hand);
                std::strcpy(hand->filename, names[0]);
                hand->pose.selector = 1;
                hand->pose.modelData = 0;
                hand->previousCore = previous;
                if (flags & 1)
                    hand->pose.extendedPoseWork = HLib::AllocateTaskData(0x400, HAND_EXTENDED_POSE_MEMORY);
                if (flags & 2)
                    hand->pose.secondaryEnabled = 1;
            }
        }
        if (names[1][0]) {
            CharacterHandComponent *const hand =
                static_cast<CharacterHandComponent *>(AllocateModelComponent(character, CHARACTER_RIGHT_HAND, sizeof(CharacterHandComponent), UpdateHand));
            SetModelComponentCleanup(character, CHARACTER_RIGHT_HAND, CleanupHand);
            if (hand) {
                ResetCharacterHandComponent(*hand);
                std::strcpy(hand->filename, names[1]);
                hand->pose.selector = 2;
                hand->pose.modelData = 0;
                hand->previousCore = previous;
                if (flags & 1)
                    hand->pose.extendedPoseWork = HLib::AllocateTaskData(0x400, HAND_EXTENDED_POSE_MEMORY);
                if (flags & 2)
                    hand->pose.secondaryEnabled = 1;
            }
        }
    } else if (names[0][0]) {
        std::uint8_t selector = 0;
        CharacterHandComponent *hand = 0;
        if (!CompareCharacterTableTokens(names[0] + 3, "_UL.MT7")) {
            selector = 3;
            hand = static_cast<CharacterHandComponent *>(AllocateModelComponent(character, CHARACTER_LEFT_HAND, sizeof(CharacterHandComponent), UpdateHand));
            SetModelComponentCleanup(character, CHARACTER_LEFT_HAND, CleanupHand);
        } else if (!CompareCharacterTableTokens(names[0] + 3, "_UR.MT7")) {
            selector = 4;
            hand = static_cast<CharacterHandComponent *>(AllocateModelComponent(character, CHARACTER_RIGHT_HAND, sizeof(CharacterHandComponent), UpdateHand));
            SetModelComponentCleanup(character, CHARACTER_RIGHT_HAND, CleanupHand);
        }
        if (hand) {
            ResetCharacterHandComponent(*hand);
            std::strcpy(hand->filename, names[0]);
            hand->pose.selector = selector;
            hand->pose.modelData = 0;
            hand->previousCore = charaAssignCoreCallback(character, CharacterHandDirectCore);
            if (flags & 1)
                hand->pose.extendedPoseWork = HLib::AllocateTaskData(0x400, HAND_EXTENDED_POSE_MEMORY);
        }
    }
}

void ComputeHandTransformedProbes(const Vector3 *positions, void *) {
    Vector3 transformed[72];
    const Vector3 *source = positions;
    Vector3 *destination = transformed;
    std::int32_t index;
    for (index = 0; index < 72; ++index) {
        Matrix4x4::TransformPoint(*source, *destination);
        ++source;
        ++destination;
    }
    for (index = 0; index < 10; ++index) {
        const float firstWeight = handGripProbes[index].firstWeight;
        const float secondWeight = 1.0f - firstWeight;
        HandGripProbe *const output = static_cast<HandGripProbe *>(handProbeWork) + index;
        output->position.x = firstWeight * transformed[handGripProbes[index].firstJoint].x + transformed[handGripProbes[index].secondJoint].x * secondWeight;
        output->position.y = firstWeight * transformed[handGripProbes[index].firstJoint].y + transformed[handGripProbes[index].secondJoint].y * secondWeight;
        output->position.z = firstWeight * transformed[handGripProbes[index].firstJoint].z + transformed[handGripProbes[index].secondJoint].z * secondWeight;
        output->radius = handGripProbes[index].radius;
    }
}

void UpdateHand(Character *character, void *payload) {
    CharacterHandComponent *hand = static_cast<CharacterHandComponent *>(payload);
    std::int16_t previousWrist[3];
    if ((hand->pose.region <= 0 && hand->pose.previousRegion <= 0) || !hand->pose.modelData) {
        hand->pose.flags |= HandPoseInactive;
        return;
    }
    HandPoseState *state = &hand->pose;
    hand->pose.previousFrame = hand->pose.currentFrame;
    if (hand->primaryNode) {
        previousWrist[0] = state->wristAngles[0];
        previousWrist[1] = state->wristAngles[1];
        previousWrist[2] = state->wristAngles[2];
        if (hand->pose.selector == 2 || hand->pose.selector == 4) {
            state->wristAngles[0] = static_cast<std::int16_t>(hand->primaryNode->rotation.x);
            state->wristAngles[1] = static_cast<std::int16_t>(hand->primaryNode->rotation.y + g_characterHandWristAngles[0]);
            state->wristAngles[2] = static_cast<std::int16_t>(hand->primaryNode->rotation.z + g_characterHandWristAngles[1]);
        } else {
            state->wristAngles[0] = static_cast<std::int16_t>(hand->primaryNode->rotation.x);
            state->wristAngles[1] = static_cast<std::int16_t>(hand->primaryNode->rotation.y + g_characterHandWristAngles[2]);
            state->wristAngles[2] = static_cast<std::int16_t>(hand->primaryNode->rotation.z + g_characterHandWristAngles[3]);
        }
    }
    if (gripDisabled) {
        hand->pose.flags |= HandPoseInactive;
        return;
    }
    if (state->wristAngles[0] != previousWrist[0] || state->wristAngles[1] != previousWrist[1] || state->wristAngles[2] != previousWrist[2] ||
        !state->poseMode || state->poseTicks > 0) {
        StageHandPose(static_cast<std::int32_t>(hand->pose.currentFrame * 2.0f), state, handPoseMatrices);
        matrix_stack::Matrix4x4_Push(0);
        ApplyHandBodyTransform(character, hand);
        ComputeHandTransformedProbes(state->jointPositions, hand->pose.extendedPoseWork);
        PopCurrentMatrix(1);
        if (hand->pose.secondaryPoseWork && state->poseMode)
            UpdateHandGripBounds(hand);
    }
    const std::int8_t flags = hand->pose.flags;
    hand->pose.flags = flags & ~HandPoseInactive;
    if (!state->poseMode) {
        if (!(flags & HandPosePaused)) {
            if (flags & HandPoseReverse)
                hand->pose.frame = hand->pose.frame - hand->pose.step;
            else
                hand->pose.frame = hand->pose.frame + hand->pose.step;
            const std::int16_t rounded = hand->pose.currentFrame = static_cast<std::int16_t>(static_cast<std::int32_t>(hand->pose.frame + 0.5f));
            if (hand->pose.flags & HandPoseLoop) {
                std::int32_t duration = hand->pose.lastFrame;
                duration = duration - hand->pose.firstFrame + 1;
                while (hand->pose.currentFrame < hand->pose.firstFrame)
                    hand->pose.currentFrame += duration;
                while (hand->pose.currentFrame > hand->pose.lastFrame)
                    hand->pose.currentFrame -= duration;
            } else {
                if (hand->pose.currentFrame < hand->pose.firstFrame)
                    hand->pose.currentFrame = hand->pose.firstFrame;
                if (hand->pose.currentFrame > hand->pose.lastFrame)
                    hand->pose.currentFrame = hand->pose.lastFrame;
            }
            if (hand->pose.currentFrame != rounded)
                hand->pose.frame = hand->pose.currentFrame;
        }
        if (hand->pose.flags & HandPoseReverse) {
            if (hand->pose.currentFrame <= hand->pose.firstFrame || hand->pose.currentFrame > hand->pose.previousFrame)
                hand->pose.flags |= HandPoseInactive;
        } else if (hand->pose.currentFrame >= hand->pose.lastFrame || hand->pose.currentFrame < hand->pose.previousFrame) {
            hand->pose.flags |= HandPoseInactive;
        }
    }
    if (state->poseMode && state->poseTicks > 0)
        AdvanceGripAngles(hand, hand->pose.secondaryPoseWork != 0, 0);
    if (state->poseMode && state->poseTicks <= 0)
        hand->pose.flags |= HandPoseInactive;
}

// @verified - d
std::int32_t GetHandAngleDifference(std::int32_t target, std::int32_t current) {
    const std::uint32_t difference = static_cast<std::uint32_t>(target) - static_cast<std::uint32_t>(current);
    return difference & 0x8000U ? static_cast<std::int32_t>(difference | 0xffff0000U) : static_cast<std::int32_t>(difference & 0x7fffU);
}

// @verified - d
CharacterHandComponent *AllocateHNDL(Character *character) {
    return static_cast<CharacterHandComponent *>(charFindComp(character, CHARACTER_LEFT_HAND));
}

// @verified - d
void InitializeHandPoseNullsub() {
}

std::int32_t HandStateUnusedNullsubA() {
    return 0;
}

std::int32_t HandStateUnusedNullsubB() {
    return 0;
}

// @verified - d
CharacterHandComponent *AllocateHNDR(Character *character) {
    return static_cast<CharacterHandComponent *>(charFindComp(character, CHARACTER_RIGHT_HAND));
}

// @verified - d
void ApplyHandBodyTransform(Character *character, CharacterHandComponent *hand) {
    HandPoseState &pose = hand->pose;
    std::int32_t id;
    if (static_cast<std::uint8_t>(pose.selector - 1U) < 2) {
        id = pose.selector == 1 ? 21 : 14;
        matrix_stack::LoadMatrix(mtwkGetBoneTransform(character, id));
        const MT7Node *const node = hand->primaryNode;
        if (node) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            Matrix4x4::SetViewMatrixFromEuler(node->rotation.x, node->rotation.y, node->rotation.z);
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
        }
    } else {
        id = pose.selector == 3 ? -66 : -65;
        matrix_stack::UnitCurrentMatrix();
        Vector3i rotation;
        Vector3 position;
        GetCharacterWorldPosition(character, &position);
        GetCharacterWorldRotation(character, &rotation);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(rotation.x, rotation.y, rotation.z);
        const MT7Node *node = character->model;
        while (node) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            Matrix4x4::SetViewMatrixFromEuler(node->rotation.x, node->rotation.y, node->rotation.z);
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            if (node->id == id)
                node = 0;
            else
                node = node->child;
        }
    }
}

// @verified - d
void CharacterHandDrawClosure(Character *character) {
    g_temporaryHNDL = AllocateHNDL(character);
    CharacterHandComponent *hand = g_temporaryHNDL;
    g_temporaryHNDR = AllocateHNDR(character);
    if (g_temporaryHNDR)
        hand = g_temporaryHNDR;
    if (character->model && hand && hand->previousCore)
        hand->previousCore(character);
    g_temporaryHNDL = g_temporaryHNDR = 0;
}

// @verified - d
void CharacterHandDirectCore(Character *character) {
    CharacterHandComponent *hand = AllocateHNDL(character);
    if (!hand)
        hand = AllocateHNDR(character);
    if (!hand)
        return;
    if (hand->primaryNode) {
        HandPoseState *state = &hand->pose;
        if (hand->pose.region || hand->pose.previousRegion)
            hand->primaryNode->mesh = state->meshCopy;
        else
            hand->primaryNode->mesh = const_cast<std::uint32_t *>(state->meshBackup);
    }
    if (hand->previousCore)
        hand->previousCore(character);
}

// @verified - d
void UpdateHandGripTriangleBounds(float *bounds, const Vector3 *const *vertices, std::uint32_t, const HandGripProbe *probes) {
    std::int32_t group;
    Vector3 unusedPoint;
    float candidates[5];
    Vector3 firstEdge, secondEdge;
    firstEdge.x = vertices[1]->x - vertices[0]->x;
    firstEdge.y = vertices[1]->y - vertices[0]->y;
    firstEdge.z = vertices[1]->z - vertices[0]->z;
    secondEdge.x = vertices[2]->x - vertices[0]->x;
    secondEdge.y = vertices[2]->y - vertices[0]->y;
    secondEdge.z = vertices[2]->z - vertices[0]->z;
    const float secondSquare = am2Vector3Dot(&secondEdge, &secondEdge);
    const float firstSquare = am2Vector3Dot(&firstEdge, &firstEdge);
    const float cross = am2Vector3Dot(&firstEdge, &secondEdge);
    for (group = 0; group < 5; ++group) {
        if (bounds[group] < 0.0f)
            continue;
        const HandGripProbe *first = probes + group * 2, *second = probes + group * 2 + 1;
        float determinant, radius, firstFactor, secondFactor;
        Vector3 firstOffset;
        {
            firstOffset.x = first->position.x - vertices[0]->x;
            firstOffset.y = first->position.y - vertices[0]->y;
            firstOffset.z = first->position.z - vertices[0]->z;
            const float secondDot = am2Vector3Dot(&firstOffset, &secondEdge);
            const float firstDot = am2Vector3Dot(&firstOffset, &firstEdge);
            determinant = cross * cross - firstSquare * secondSquare;
            if (determinant < 1.0000000116860974e-07f && determinant > -1.0000000116860974e-07f) {
                secondFactor = firstFactor = -1.0f;
            } else {
                secondFactor = (cross * firstDot - firstSquare * secondDot) / determinant;
                firstFactor = (cross * secondDot - secondSquare * firstDot) / determinant;
            }
            if (secondFactor < 0.0f)
                secondFactor = 0.0f;
            if (secondFactor > 1.0f)
                secondFactor = 1.0f;
            if (firstFactor < 0.0f)
                firstFactor = 0.0f;
            if (firstFactor > 1.0f)
                firstFactor = 1.0f;
            if (secondFactor + firstFactor > 1.0f)
                secondFactor = firstFactor = 0.5f;
            firstOffset.x -= firstFactor * firstEdge.x + secondFactor * secondEdge.x;
            firstOffset.y -= firstFactor * firstEdge.y + secondFactor * secondEdge.y;
            firstOffset.z -= firstFactor * firstEdge.z + secondFactor * secondEdge.z;
            radius = first->radius;
            candidates[0] = am2Vector3Dot(&firstOffset, &firstOffset) - radius * radius;
        }
        Vector3 secondOffset;
        {
            secondOffset.x = second->position.x - vertices[0]->x;
            secondOffset.y = second->position.y - vertices[0]->y;
            secondOffset.z = second->position.z - vertices[0]->z;
            const float secondDot = am2Vector3Dot(&secondOffset, &secondEdge);
            const float firstDot = am2Vector3Dot(&secondOffset, &firstEdge);
            if (determinant < 1.0000000116860974e-07f && determinant > -1.0000000116860974e-07f) {
                secondFactor = firstFactor = -1.0f;
            } else {
                secondFactor = (cross * firstDot - firstSquare * secondDot) / determinant;
                firstFactor = (cross * secondDot - secondSquare * firstDot) / determinant;
            }
            if (secondFactor < 0.0f)
                secondFactor = 0.0f;
            if (secondFactor > 1.0f)
                secondFactor = 1.0f;
            if (firstFactor < 0.0f)
                firstFactor = 0.0f;
            if (firstFactor > 1.0f)
                firstFactor = 1.0f;
            if (secondFactor + firstFactor > 1.0f)
                secondFactor = firstFactor = 0.5f;
            secondOffset.x -= firstFactor * firstEdge.x + secondFactor * secondEdge.x;
            secondOffset.y -= firstFactor * firstEdge.y + secondFactor * secondEdge.y;
            secondOffset.z -= firstFactor * firstEdge.z + secondFactor * secondEdge.z;
            radius = second->radius;
            candidates[1] = am2Vector3Dot(&secondOffset, &secondOffset) - radius * radius;
        }
        Vector3 segment;
        segment.x = second->position.x - first->position.x;
        segment.y = second->position.y - first->position.y;
        segment.z = second->position.z - first->position.z;
        const float segmentSquare = am2Vector3Dot(&segment, &segment);
        Vector3 firstEdgeOffset;
        {
            firstEdgeOffset.x = vertices[0]->x - first->position.x;
            firstEdgeOffset.y = vertices[0]->y - first->position.y;
            firstEdgeOffset.z = vertices[0]->z - first->position.z;
            const float segmentDot = am2Vector3Dot(&firstEdgeOffset, &segment);
            const float edgeDot = am2Vector3Dot(&firstEdge, &segment);
            const float offsetDot = am2Vector3Dot(&firstEdgeOffset, &firstEdge);
            determinant = edgeDot * edgeDot - firstSquare * segmentSquare;
            if (determinant < 1.0000000116860974e-07f && determinant > -1.0000000116860974e-07f) {
                secondFactor = firstFactor = -1.0f;
            } else {
                secondFactor = (offsetDot * edgeDot - firstSquare * segmentDot) / determinant;
                firstFactor = (offsetDot * segmentSquare - segmentDot * edgeDot) / determinant;
            }
            if (secondFactor < 0.0f)
                secondFactor = 0.0f;
            if (secondFactor > 1.0f)
                secondFactor = 1.0f;
            if (firstFactor < 0.0f)
                firstFactor = 0.0f;
            if (firstFactor > 1.0f)
                firstFactor = 1.0f;
            firstEdgeOffset.x += firstFactor * firstEdge.x - segment.x * secondFactor;
            firstEdgeOffset.y += firstFactor * firstEdge.y - segment.y * secondFactor;
            firstEdgeOffset.z += firstFactor * firstEdge.z - segment.z * secondFactor;
            radius = first->radius + (second->radius - first->radius) * secondFactor;
            candidates[2] = am2Vector3Dot(&firstEdgeOffset, &firstEdgeOffset) - radius * radius;
        }
        Vector3 secondEdgeOffset;
        {
            secondEdgeOffset.x = vertices[0]->x - first->position.x;
            secondEdgeOffset.y = vertices[0]->y - first->position.y;
            secondEdgeOffset.z = vertices[0]->z - first->position.z;
            const float segmentDot = am2Vector3Dot(&secondEdgeOffset, &segment);
            const float edgeDot = am2Vector3Dot(&secondEdge, &segment);
            const float offsetDot = am2Vector3Dot(&secondEdgeOffset, &secondEdge);
            determinant = edgeDot * edgeDot - secondSquare * segmentSquare;
            if (determinant < 1.0000000116860974e-07f && determinant > -1.0000000116860974e-07f) {
                secondFactor = firstFactor = -1.0f;
            } else {
                secondFactor = (offsetDot * edgeDot - secondSquare * segmentDot) / determinant;
                firstFactor = (offsetDot * segmentSquare - segmentDot * edgeDot) / determinant;
            }
            if (secondFactor < 0.0f)
                secondFactor = 0.0f;
            if (secondFactor > 1.0f)
                secondFactor = 1.0f;
            if (firstFactor < 0.0f)
                firstFactor = 0.0f;
            if (firstFactor > 1.0f)
                firstFactor = 1.0f;
            secondEdgeOffset.x += firstFactor * secondEdge.x - segment.x * secondFactor;
            secondEdgeOffset.y += firstFactor * secondEdge.y - segment.y * secondFactor;
            secondEdgeOffset.z += firstFactor * secondEdge.z - segment.z * secondFactor;
            radius = first->radius + (second->radius - first->radius) * secondFactor;
            candidates[3] = am2Vector3Dot(&secondEdgeOffset, &secondEdgeOffset) - radius * radius;
        }
        Vector3 thirdEdgeOffset, thirdEdge;
        {
            thirdEdge.x = vertices[1]->x - vertices[2]->x;
            thirdEdge.y = vertices[1]->y - vertices[2]->y;
            thirdEdge.z = vertices[1]->z - vertices[2]->z;
            thirdEdgeOffset.x = vertices[2]->x - first->position.x;
            thirdEdgeOffset.y = vertices[2]->y - first->position.y;
            thirdEdgeOffset.z = vertices[2]->z - first->position.z;
            const float segmentDot = am2Vector3Dot(&thirdEdgeOffset, &segment);
            const float edgeDot = am2Vector3Dot(&thirdEdge, &segment);
            const float offsetDot = am2Vector3Dot(&thirdEdgeOffset, &thirdEdge);
            const float edgeSquare = am2Vector3Dot(&thirdEdge, &thirdEdge);
            determinant = edgeDot * edgeDot - edgeSquare * segmentSquare;
            if (determinant < 1.0000000116860974e-07f && determinant > -1.0000000116860974e-07f) {
                secondFactor = firstFactor = -1.0f;
            } else {
                secondFactor = (offsetDot * edgeDot - segmentDot * edgeSquare) / determinant;
                firstFactor = (offsetDot * segmentSquare - segmentDot * edgeDot) / determinant;
            }
            if (secondFactor < 0.0f)
                secondFactor = 0.0f;
            if (secondFactor > 1.0f)
                secondFactor = 1.0f;
            if (firstFactor < 0.0f)
                firstFactor = 0.0f;
            if (firstFactor > 1.0f)
                firstFactor = 1.0f;
            thirdEdgeOffset.x += firstFactor * thirdEdge.x - segment.x * secondFactor;
            thirdEdgeOffset.y += firstFactor * thirdEdge.y - segment.y * secondFactor;
            thirdEdgeOffset.z += firstFactor * thirdEdge.z - segment.z * secondFactor;
            radius = first->radius + (second->radius - first->radius) * secondFactor;
            candidates[4] = am2Vector3Dot(&thirdEdgeOffset, &thirdEdgeOffset) - radius * radius;
        }
        for (std::int32_t candidate = 0; candidate < 5; ++candidate)
            if (bounds[group] > candidates[candidate])
                bounds[group] = candidates[candidate];
    }
}

// @verified - d
void UpdateHandGripBounds(CharacterHandComponent *hand) {
    Vector3 position, scale;
    Vector3i rotation;
    Vector3 difference;
    std::int32_t group;
    for (group = 0; group < 5; ++group)
        if (!(hand->pose.bounds[group] < 0.0f))
            hand->pose.bounds[group] = 3.4028234663852886e+38f;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::UnitCurrentMatrix();
    GetCharacterWorldPosition(hand->pose.secondaryPoseWork, &position);
    GetCharacterWorldRotation(hand->pose.secondaryPoseWork, &rotation);
    GetCharacterScale(hand->pose.secondaryPoseWork, &scale);
    matrix_stack::TranslateVector(&position);
    Matrix4x4::SetViewMatrixFromEuler(rotation.x, rotation.y, rotation.z);
    am2ScaleXmtrxRows(&scale);
    MT7Node *const model = hand->pose.secondaryPoseWork->model;
    if (!model) {
        matrix_stack::GetTranslation(&position);
        for (group = 0; group < 5; ++group) {
            am2Vector3Subtract(&difference, &position, &reinterpret_cast<const HandGripProbe *>(handProbeWork)[group * 2].position);
            const float squared = am2Vector3Dot(&difference, &difference);
            const float radius = reinterpret_cast<const HandGripProbe *>(handProbeWork)[group * 2].radius + 0.0020000000949949026f;
            const float bound = squared - radius * radius;
            if (bound < 0.0f)
                hand->pose.bounds[group] = bound;
        }
    } else {
        BuildHandGripGeometry(hand, model);
        UpdateMeshHandGripBounds(hand);
        for (group = 0; group < 5; ++group) {
            SetDebugScreenTextPosition(30, group + 7);
            shenmue_s2_event_DebugLogToScreen("%0*.5f", 11, hand->pose.bounds[group]);
        }
    }
    PopCurrentMatrix(1);
}

// @verified - d
void ResetCharacterHandComponent(CharacterHandComponent &hand) {
    hand.primaryNode = SHENMUE_HAND_NULL;
    hand.primaryFile = SHENMUE_HAND_NULL;
    hand.primaryModel = SHENMUE_HAND_NULL;
    hand.secondaryNode = SHENMUE_HAND_NULL;
    hand.secondaryFile = SHENMUE_HAND_NULL;
    hand.secondaryModel = SHENMUE_HAND_NULL;
    hand.pose.extendedPoseWork = SHENMUE_HAND_NULL;
    hand.pose.secondaryPoseWork = SHENMUE_HAND_NULL;
    for (std::int32_t index = 0; index < 16; ++index)
        hand.pose.bounds[index] = SHENMUE_HAND_MAX_FLOAT;
    hand.pose.region = 0;
    hand.pose.previousRegion = 0;
    hand.pose.alpha = 0.0f;
    hand.pose.currentFrame = 0;
    hand.pose.previousFrame = 0;
    hand.pose.firstFrame = 0;
    hand.pose.lastFrame = 0;
    hand.pose.flags = HandPoseInactive;
    hand.pose.secondaryEnabled = 0;
    hand.sequence = SHENMUE_HAND_NULL;
    hand.pose.modelData = SHENMUE_HAND_NULL;
    hand.pose.frameCount = 0;
    hand.pose.jointCount = 0;
    hand.pose.channels = SHENMUE_HAND_NULL;
    hand.pose.meshBackup = SHENMUE_HAND_NULL;
    hand.pose.meshCopy = SHENMUE_HAND_NULL;
    hand.pose.poseMode = 0;
    hand.pose.poseTicks = 0;
    hand.pose.frame = 0.0f;
    hand.pose.step = 0.0f;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ParseHandPoseJoint() {
    std::int32_t values[3], base[3];
    const HandPoseModelJoint *joint = g_handPoseJointCursor;
    const std::int32_t id = joint->id;
    std::int32_t children = joint->childCount;
    values[0] = base[0] = joint->rotation.x;
    values[1] = base[1] = joint->rotation.y;
    values[2] = base[2] = joint->rotation.z;
    if (g_handPoseChannelCursor) {
        if (g_handPoseChannelCursor[0].keyCount > 0)
            values[0] = InterpolateHandPoseChannel(g_handPoseChannelCursor[0].keyCount, g_handPoseChannelCursor[0].keys, g_handPoseEvaluationFrame);
        if (g_handPoseChannelCursor[1].keyCount > 0)
            values[1] = InterpolateHandPoseChannel(g_handPoseChannelCursor[1].keyCount, g_handPoseChannelCursor[1].keys, g_handPoseEvaluationFrame);
        if (g_handPoseChannelCursor[2].keyCount > 0)
            values[2] = InterpolateHandPoseChannel(g_handPoseChannelCursor[2].keyCount, g_handPoseChannelCursor[2].keys, g_handPoseEvaluationFrame);
        g_handPoseChannelCursor += 3;
    }
    std::int16_t *output = g_handPoseOutputAngles[id];
    output[0] = static_cast<std::int16_t>(values[0]) - static_cast<std::int16_t>(base[0]);
    output[1] = static_cast<std::int16_t>(values[1]) - static_cast<std::int16_t>(base[1]);
    output[2] = static_cast<std::int16_t>(values[2]) - static_cast<std::int16_t>(base[2]);
    ++g_handPoseJointCursor;
    while (children-- > 0)
        ParseHandPoseJoint();
}
#else
// @verified - d
void ParseHandPoseJoint() {
    const HandPoseModelJoint &joint = *g_handPoseJointCursor;
    const std::int32_t children = joint.childCount;
    std::int32_t values[3];
    for (std::int32_t axis = 0; axis < 3; ++axis) {
        std::int32_t value = joint.rotation[axis];
        if (g_handPoseChannelCursor && g_handPoseChannelCursor[axis].keyCount > 0)
            value = InterpolateHandPoseChannel(g_handPoseChannelCursor[axis].keyCount, g_handPoseChannelCursor[axis].keys, g_handPoseEvaluationFrame);
        values[axis] = value;
    }
    if (g_handPoseChannelCursor)
        g_handPoseChannelCursor += 3;
    for (std::int32_t axis = 0; axis < 3; ++axis)
        g_handPoseOutputAngles[joint.id][axis] =
            SHENMUE_HAND_ANGLE(static_cast<std::uint16_t>(static_cast<std::uint32_t>(values[axis]) - static_cast<std::uint32_t>(joint.rotation[axis])));
    ++g_handPoseJointCursor;
    for (std::int32_t child = 0; child < children; ++child)
        ParseHandPoseJoint();
}
#endif

// @verified - d
void EvaluateHandPoseJoints(std::int32_t frame, HandPoseState *state) {
    g_handPoseEvaluationFrame = frame;
    g_handPoseJointCursor = reinterpret_cast<const HandPoseModelJoint *>(state->modelData + 8);
    g_handPoseChannelCursor = state->channels;
    g_handPoseOutputAngles = state->targetJointAngles;
    ParseHandPoseJoint();
}

// @verified - d
void ParseHandPoseFrame(std::int32_t frame, HandPoseState *state, Matrix4x4 *) {
    EvaluateHandPoseJoints(frame, state);
}

// @verified - d
void SetCharacterHandRegionEnabled(CharacterHandComponent *hand, std::int8_t region, std::int16_t pose, std::int8_t alphaPolicy) {
    if (!((hand->primaryNode && region >= 0) || (hand->secondaryNode && region <= 0)))
        return;
    if (!region && hand->pose.region)
        hand->pose.previousRegion = hand->pose.region;
    hand->pose.region = region;
    if (alphaPolicy >= 0) {
        hand->pose.alpha = region ? 1.0f : 0.0f;
        hand->pose.previousRegion = region;
    }
    if (region && pose)
        ConfigureHandPoseSequence(*hand, pose, 0, 0, 0, HandPoseLoop, 1.0f, 0);
    if (region && !pose) {
        HandPoseState *const state = &hand->pose;
        const std::int16_t *const jointIds = handCommandJointIds;
        std::int32_t index;
        for (index = 0; index < 72; ++index) {
            state->currentJointAngles[index][0] = 0;
            state->currentJointAngles[index][1] = 0;
            state->currentJointAngles[index][2] = 0;
        }
        for (index = 0; index < 19; ++index) {
            state->currentJointAngles[jointIds[index]][0] = static_cast<std::int16_t>(handDefaultJointAngles[index][0]);
            state->currentJointAngles[jointIds[index]][1] = static_cast<std::int16_t>(handDefaultJointAngles[index][1]);
            state->currentJointAngles[jointIds[index]][2] = static_cast<std::int16_t>(handDefaultJointAngles[index][2]);
        }
        for (index = 0; index < 72; ++index) {
            std::int16_t *const target = state->targetJointAngles[index];
            std::memcpy(target, state->currentJointAngles[index], 6);
        }
        state->poseMode = 1;
        state->poseTicks = 1;
    }
}

void ConfigureHandPoseSequence(CharacterHandComponent &hand, std::int16_t pose, std::int32_t first, std::int32_t last, std::int16_t start, std::uint16_t flags,
    float step, std::int16_t ticks) {
    HandPoseState &state = hand.pose;
    const std::int16_t firstFrame = static_cast<std::int16_t>(first - 1), lastFrame = static_cast<std::int16_t>(last - 1);
    --start;
    hand.pose.flags = flags & static_cast<std::uint8_t>(~HandPoseInactive);
    HandPoseBank *bank = handPoseBanks.next;
    while (bank && (pose < bank->first || pose > bank->last))
        bank = bank->next;
    if (bank) {
        hand.sequence = bank->poses[pose - bank->first + 1];
        state.frameCount = hand.sequence->frameCount / 2;
        state.jointCount = hand.sequence->jointCount;
        state.channels = reinterpret_cast<const HandPoseChannel *>(hand.sequence + 1);
    } else {
        hand.sequence = SHENMUE_HAND_NULL;
        state.frameCount = 0;
        state.jointCount = 0;
        state.channels = SHENMUE_HAND_NULL;
    }
    if (firstFrame < 0)
        hand.pose.firstFrame = 0;
    else {
        hand.pose.firstFrame = firstFrame;
        if (firstFrame >= state.frameCount)
            hand.pose.firstFrame = static_cast<std::int16_t>(state.frameCount - 1);
    }
    hand.pose.lastFrame = lastFrame;
    if (lastFrame >= state.frameCount || lastFrame < 0)
        hand.pose.lastFrame = static_cast<std::int16_t>(state.frameCount - 1);
    if (hand.pose.firstFrame > hand.pose.lastFrame)
        hand.pose.firstFrame = hand.pose.lastFrame = 0;
    hand.pose.currentFrame = start;
    if (start < hand.pose.firstFrame || start > hand.pose.lastFrame) {
        if (hand.pose.flags & HandPoseReverse)
            hand.pose.currentFrame = hand.pose.lastFrame;
        else
            hand.pose.currentFrame = hand.pose.firstFrame;
    }
    hand.pose.previousFrame = hand.pose.currentFrame;
    hand.pose.frame = static_cast<float>(hand.pose.currentFrame);
    if (step > 0.0f)
        hand.pose.step = step;
    state.poseMode = 0;
    state.poseTicks = ticks;
    if (ticks > 0) {
        state.poseMode = 2;
        ParseHandPoseFrame(static_cast<std::int32_t>(hand.pose.currentFrame * 2.0f), &state, handPoseMatrices);
    }
    hand.pose.secondaryPoseWork = SHENMUE_HAND_NULL;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t mtwkGetCustomOverrideFlags(const MotionWork *work) {
    if (!work || !g_searchHandMotionWork || !mtwkIsSearchHandActive(work) || !g_searchHandOverrideEnabled)
        return 0;
    return g_searchHandMotionOverride.flags;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void GetSearchHandWristAngles(MotionWork &work, std::int16_t (&angles)[4]) {
    g_motionInterpolationType = (work.sequenceFlags >> 24U) & MI_Linear;
    if (mtwkIsSearchHandActive(&work)) {
        if (g_searchHandMotionOverride.flags & 1U) {
            angles[0] = static_cast<std::int16_t>(static_cast<std::int32_t>(g_searchHandConfigurations[0].jointAngles[7] * 57.29578f * 182.04443f));
            angles[1] = static_cast<std::int16_t>(static_cast<std::int32_t>(g_searchHandConfigurations[0].jointAngles[6] * 57.29578f * 182.04443f));
        } else {
            g_motionCurrentAnimTime = reinterpret_cast<MotionWorkBindings &>(work).rig.rightArm.clock.current.ticksTime * 0.033333335f;
            g_motionMaximumAnimTime = reinterpret_cast<MotionWorkBindings &>(work).rig.rightArm.clock.authored.maxTime * 0.033333335f;
            angles[0] = static_cast<std::int16_t>(static_cast<std::int32_t>(
                SampleMotionRigChannelAdvance(reinterpret_cast<MotionWorkBindings &>(work).rig.rightArm.wristRotation[2].channel) * 57.29578f * 182.04443f));
            angles[1] = static_cast<std::int16_t>(static_cast<std::int32_t>(
                SampleMotionRigChannelAdvance(reinterpret_cast<MotionWorkBindings &>(work).rig.rightArm.wristRotation[1].channel) * 57.29578f * 182.04443f));
        }
        if (g_searchHandMotionOverride.flags & 2U) {
            angles[2] = static_cast<std::int16_t>(static_cast<std::int32_t>(g_searchHandConfigurations[1].jointAngles[7] * 57.29578f * 182.04443f));
            angles[3] = static_cast<std::int16_t>(static_cast<std::int32_t>(g_searchHandConfigurations[1].jointAngles[6] * 57.29578f * 182.04443f));
        } else {
            g_motionCurrentAnimTime = reinterpret_cast<MotionWorkBindings &>(work).rig.leftArm.clock.current.ticksTime * 0.033333335f;
            g_motionMaximumAnimTime = reinterpret_cast<MotionWorkBindings &>(work).rig.leftArm.clock.authored.maxTime * 0.033333335f;
            angles[2] = static_cast<std::int16_t>(static_cast<std::int32_t>(
                SampleMotionRigChannelAdvance(reinterpret_cast<MotionWorkBindings &>(work).rig.leftArm.wristRotation[2].channel) * 57.29578f * 182.04443f));
            angles[3] = static_cast<std::int16_t>(static_cast<std::int32_t>(
                SampleMotionRigChannelAdvance(reinterpret_cast<MotionWorkBindings &>(work).rig.leftArm.wristRotation[1].channel) * 57.29578f * 182.04443f));
        }
    } else {
        g_motionCurrentAnimTime = reinterpret_cast<MotionWorkBindings &>(work).rig.rightArm.clock.current.ticksTime * 0.033333335f;
        g_motionMaximumAnimTime = reinterpret_cast<MotionWorkBindings &>(work).rig.rightArm.clock.authored.maxTime * 0.033333335f;
        angles[0] = static_cast<std::int16_t>(static_cast<std::int32_t>(
            SampleMotionRigChannelAdvance(reinterpret_cast<MotionWorkBindings &>(work).rig.rightArm.wristRotation[2].channel) * 57.29578f * 182.04443f));
        angles[1] = static_cast<std::int16_t>(static_cast<std::int32_t>(
            SampleMotionRigChannelAdvance(reinterpret_cast<MotionWorkBindings &>(work).rig.rightArm.wristRotation[1].channel) * 57.29578f * 182.04443f));
        g_motionCurrentAnimTime = reinterpret_cast<MotionWorkBindings &>(work).rig.leftArm.clock.current.ticksTime * 0.033333335f;
        g_motionMaximumAnimTime = reinterpret_cast<MotionWorkBindings &>(work).rig.leftArm.clock.authored.maxTime * 0.033333335f;
        angles[2] = static_cast<std::int16_t>(static_cast<std::int32_t>(
            SampleMotionRigChannelAdvance(reinterpret_cast<MotionWorkBindings &>(work).rig.leftArm.wristRotation[2].channel) * 57.29578f * 182.04443f));
        angles[3] = static_cast<std::int16_t>(static_cast<std::int32_t>(
            SampleMotionRigChannelAdvance(reinterpret_cast<MotionWorkBindings &>(work).rig.leftArm.wristRotation[1].channel) * 57.29578f * 182.04443f));
    }
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void mtwkResetBlends() {
    if (g_searchHandMotionWork) {
        MotionWork &work = *g_searchHandMotionWork;
        work.sequenceFlags &= ~MSF_SearchHand;
        work.lowerBodyClock.playbackSpeed = 1.0f;
        work.chestClock.playbackSpeed = 1.0f;
        work.headClock.playbackSpeed = 1.0f;
        work.rightArmClock.playbackSpeed = 1.0f;
        work.leftArmClock.playbackSpeed = 1.0f;
    }
    if (g_searchHandCharacter) {
        if (g_searchHandMotionOverride.flags & 1U) {
            if (CharacterHandComponent *const hand = AllocateHNDR(g_searchHandCharacter))
                SetCharacterHandRegionEnabled(hand, 0, 0, 1);
        }
        if (g_searchHandMotionOverride.flags & 2U) {
            if (CharacterHandComponent *const hand = AllocateHNDL(g_searchHandCharacter))
                SetCharacterHandRegionEnabled(hand, 0, 0, 1);
        }
    }
    g_searchHandCharacter = SHENMUE_HAND_NULL;
    g_searchHandMotionWork = SHENMUE_HAND_NULL;
    g_searchHandOverrideEnabled = 0;
    g_searchHandChestOverrideEnabled = 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool IsHandPoseInactive(const CharacterHandComponent &hand) {
    if ((hand.pose.flags & HandPoseInactive) != 0)
        return true;
    return false;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetHandPoseFlags(CharacterHandComponent &hand, std::int16_t enabled, std::uint16_t flags) {
    const std::int32_t reverse = hand.pose.flags & HandPoseReverse;
    if (enabled)
        hand.pose.flags |= flags;
    else
        hand.pose.flags &= ~flags;
    std::int32_t reset = 0;
    if ((hand.pose.flags & HandPoseReverse) && !reverse && hand.pose.currentFrame <= hand.pose.firstFrame) {
        reset = 1;
        hand.pose.currentFrame = hand.pose.lastFrame;
    }
    if (!(hand.pose.flags & HandPoseReverse) && reverse && hand.pose.currentFrame >= hand.pose.lastFrame) {
        reset = 1;
        hand.pose.currentFrame = hand.pose.firstFrame;
    }
    if (reset) {
        hand.pose.previousFrame = hand.pose.currentFrame;
        hand.pose.frame = static_cast<float>(hand.pose.currentFrame);
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetHandPoseStep(CharacterHandComponent &hand, float step) {
    if (step > 0.0f)
        hand.pose.step = step;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void GetHandEndpointPosition(const CharacterHandComponent &hand, std::int32_t endpoint, Vector3 &position) {
    const Vector3 &source = hand.pose.jointPositions[handEndJointIds[endpoint]];
    position.x = source.x;
    position.y = source.y;
    position.z = source.z;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ParseHandPoseCommand(CharacterHandComponent &hand, std::byte *command, std::int16_t ticks) {
    HandPoseState &state = hand.pose;
    std::int32_t index;
    state.poseMode = 1;
    std::int16_t *angles = state.targetJointAngles[0];
    for (index = 0; index < 72; ++index, angles += 3)
        angles[0] = angles[1] = angles[2] = 0;
    std::uint32_t *values = reinterpret_cast<std::uint32_t *>(command);
    for (index = 0; index < 19; ++index, values += 3) {
        angles = state.targetJointAngles[handCommandJointIds[index]];
        const std::int16_t joint = handCommandJointIds[index];
        if (joint != 17 && joint != 13 && joint != 9 && joint != 5 && joint != 1) {
            if ((values[0] & handPoseCommandRangeMask) && (values[0] & handPoseCommandRangeMask) != handPoseCommandRangeMask)
                values[0] = 0;
            if ((values[1] & handPoseCommandRangeMask) && (values[1] & handPoseCommandRangeMask) != handPoseCommandRangeMask)
                values[1] = 0;
            if (!(values[2] & handPoseCommandSignBit) && (values[2] & handPoseCommandRangeMask))
                values[2] = 0;
        }
        angles[0] = static_cast<std::int16_t>(values[0]);
        angles[1] = static_cast<std::int16_t>(values[1]);
        angles[2] = static_cast<std::int16_t>(values[2]);
    }
    state.poseTicks = ticks;
    if (ticks < 1) {
        for (index = 0; index < 72; ++index) {
            state.currentJointAngles[index][0] = state.targetJointAngles[index][0];
            state.currentJointAngles[index][1] = state.targetJointAngles[index][1];
            state.currentJointAngles[index][2] = state.targetJointAngles[index][2];
        }
        StageHandPose(0, &state, handPoseMatrices);
        state.poseTicks = 1;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ParseHandPoseDegreeCommand(CharacterHandComponent &hand, const std::int8_t *degrees, float ticks) {
    HandPoseState &state = hand.pose;
    std::int32_t index;
    state.poseMode = 1;
    std::int16_t *angles = state.targetJointAngles[0];
    for (index = 0; index < 72; ++index, angles += 3)
        angles[0] = angles[1] = angles[2] = 0;
    for (index = 0; index < 19; ++index) {
        angles = state.targetJointAngles[handCommandJointIds[index]];
        angles[0] = static_cast<std::int16_t>(*degrees++ * 0x2000 / 45);
        angles[1] = static_cast<std::int16_t>(*degrees++ * 0x2000 / 45);
        angles[2] = static_cast<std::int16_t>(*degrees++ * 0x2000 / 45);
    }
    if ((state.poseTicks = static_cast<std::int16_t>(ticks)) < 1) {
        for (index = 0; index < 72; ++index) {
            state.currentJointAngles[index][0] = state.targetJointAngles[index][0];
            state.currentJointAngles[index][1] = state.targetJointAngles[index][1];
            state.currentJointAngles[index][2] = state.targetJointAngles[index][2];
        }
        state.poseTicks = 1;
        StageHandPose(0, &state, handPoseMatrices);
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyCharacterHandPoseCommand(Character *character, std::int32_t side, std::byte *command, std::int32_t ticks) {
    if (CharacterHandComponent *const hand = GetCharacterHand(character, side))
        ParseHandPoseCommand(*hand, command, ticks);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void WriteHandPoseCommand(const CharacterHandComponent *hand, std::byte *command) {
    const HandPoseState *const state = &hand->pose;
    std::int32_t *values = reinterpret_cast<std::int32_t *>(command);
    for (std::int32_t index = 0; index < 19; ++index, values += 3) {
        const std::int16_t *const angles = state->currentJointAngles[handCommandJointIds[index]];
        values[0] = angles[0];
        values[1] = angles[1];
        values[2] = angles[2];
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t GetHandPoseAnimationResult(const CharacterHandComponent &hand) {
    if (hand.pose.poseMode)
        return -static_cast<std::int32_t>(hand.pose.poseTicks);
    else
        return hand.pose.currentFrame + 1;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetHandSecondaryPoseWork(CharacterHandComponent &hand, Character *work) {
    if (hand.pose.secondaryPoseWork == work)
        return;
    hand.pose.secondaryPoseWork = work;
    for (std::int32_t index = 0; index < 16; ++index)
        hand.pose.bounds[index] = SHENMUE_HAND_MAX_FLOAT;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetHighModelXYZ(Character *character, std::int32_t region, std::int32_t side, std::int32_t pose) {
    if (CharacterHandComponent *const hand = GetCharacterHand(character, side)) {
        SetCharacterHandRegionEnabled(hand, region, pose, 1);
        SetHandPrimaryRotation(hand, 0, 0, 0);
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetHandRegionXYZWithFlags(Character *character, std::int32_t region, std::int32_t side, std::int32_t pose, std::int8_t alphaPolicy) {
    if (CharacterHandComponent *const hand = GetCharacterHand(character, side)) {
        SetCharacterHandRegionEnabled(hand, region, pose, alphaPolicy);
        SetHandPrimaryRotation(hand, 0, 0, 0);
    }
}
#endif

// @verified - d
void TransformHandGripMesh(const HandMeshHeader *mesh, std::uint32_t *destination) {
    const std::uint32_t *source = reinterpret_cast<const std::uint32_t *>(mesh);
    if (static_cast<std::int32_t>(source[0]) < 0 || !source[6])
        return;
    source += 6;
    destination += 6;
    std::uint32_t command = *source;
    std::uint32_t stride = 8;
    std::uint32_t count, index;
    bool strip;
    while (command) {
        if (static_cast<std::int32_t>(command) < 0) {
            const std::uint32_t *const header = source;
            source += 20;
            destination += 20;
            stride = header[9] == 0xfffffffeU ? 14 : 8;
        } else {
            if ((*source & 0x18U) != MT7PrimitiveTriangleList) {
                count = source[1];
                strip = true;
            } else {
                strip = false;
                count = source[1];
            }
            source += 2;
            destination += 2;
            if (strip) {
                if (*source & 1U) {
                    Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(source), *reinterpret_cast<Vector3 *>(destination));
                    *destination |= 1U;
                    source += stride;
                    destination += stride;
                } else {
                    source += 2;
                    destination += 2;
                }
                if (*source & 1U) {
                    Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(source), *reinterpret_cast<Vector3 *>(destination));
                    *destination |= 1U;
                    source += stride;
                    destination += stride;
                } else {
                    source += 2;
                    destination += 2;
                }
                for (index = 0; index < count - 2U; ++index) {
                    if (*source & 1U) {
                        Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(source), *reinterpret_cast<Vector3 *>(destination));
                        *destination |= 1U;
                        source += stride;
                        destination += stride;
                    } else {
                        source += 2;
                        destination += 2;
                    }
                }
            } else {
                for (index = 0; index < count; ++index) {
                    if (*source & 1U) {
                        Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(source), *reinterpret_cast<Vector3 *>(destination));
                        *destination |= 1U;
                        source += stride;
                        destination += stride;
                    } else {
                        source += 2;
                        destination += 2;
                    }
                    if (*source & 1U) {
                        Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(source), *reinterpret_cast<Vector3 *>(destination));
                        *destination |= 1U;
                        source += stride;
                        destination += stride;
                    } else {
                        source += 2;
                        destination += 2;
                    }
                    if (*source & 1U) {
                        Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(source), *reinterpret_cast<Vector3 *>(destination));
                        *destination |= 1U;
                        source += stride;
                        destination += stride;
                    } else {
                        source += 2;
                        destination += 2;
                    }
                }
            }
        }
        command = *source;
    }
}

void BuildHandGripNodeGeometry(CharacterHandComponent *hand, MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    if (node->transformOverride) {
        matrix_stack::LoadMatrix(node->transformOverride);
    } else {
        BuildHandJointTransform(node->position.x, node->position.y, node->position.z, node->rotation.x, node->rotation.y, node->rotation.z);
        am2ScaleXmtrxRows(&node->scale);
    }
    if (node->mesh) {
        const std::uint32_t words = GetNumHandModelChunks(reinterpret_cast<const HandMeshHeader *>(node->mesh));
        if (words && static_cast<std::int32_t>(static_cast<std::uint32_t>(handGripMeshWords) + words) < handGripMeshCapacityWords) {
            std::uint32_t *const destination = static_cast<std::uint32_t *>(handCalculationWork) + handGripMeshWords;
            std::memcpy(destination, node->mesh, words * 4U);
            TransformHandGripMesh(reinterpret_cast<const HandMeshHeader *>(node->mesh), static_cast<std::uint32_t *>(handCalculationWork) + handGripMeshWords);
            handGripMeshWords = static_cast<std::int16_t>(static_cast<std::uint32_t>(handGripMeshWords) + words);
        }
    }
    if (node->child)
        BuildHandGripNodeGeometry(hand, node->child);
    if (node->sibling) {
        PopCurrentMatrix(1);
        BuildHandGripNodeGeometry(hand, node->sibling);
    }
}

// @verified - d
void BuildHandGripGeometry(CharacterHandComponent *hand, MT7Node *node) {
    handGripMeshWords = 0;
    BuildHandGripNodeGeometry(hand, node);
    static_cast<std::uint32_t *>(handCalculationWork)[handGripMeshWords] = 0xffffffffU;
    for (std::uint32_t index = 1; index < 8; ++index)
        static_cast<std::uint32_t *>(handCalculationWork)[handGripMeshWords + index] = 0;
}

void IntersectHandGripMesh(float *bounds, const HandMeshHeader *mesh, const HandGripProbe *probes) {
    const std::uint32_t *cursor = reinterpret_cast<const std::uint32_t *>(mesh);
    do {
        if (static_cast<std::int32_t>(cursor[0]) < 0 || !cursor[6])
            return;
    } while (0);
    cursor += 6;
    std::uint32_t stride = 8;
    if (*cursor)
        do {
            if (static_cast<std::int32_t>(*cursor) < 0) {
                const std::uint32_t *const header = cursor;
                cursor += 20;
                stride = header[9] == 0xfffffffeU ? 14 : 8;
                continue;
            }
            const std::uint32_t command = *cursor;
            std::uint32_t count;
            std::uint32_t winding;
            std::int32_t strip = 0;
            const Vector3 *vertices[3];
            if ((command & 0x18U) != MT7PrimitiveTriangleList) {
                count = cursor[1];
                strip = 1;
                winding = command & 3U;
            } else {
                count = cursor[1];
                winding = 2;
            }
            cursor += 2;
            if (strip) {
                if (*cursor & 1U) {
                    vertices[1] = reinterpret_cast<const Vector3 *>(cursor);
                    cursor += stride;
                } else {
                    vertices[1] = reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(cursor) + static_cast<std::int32_t>(cursor[1]) + 8);
                    cursor += 2;
                }
                if (*cursor & 1U) {
                    vertices[2] = reinterpret_cast<const Vector3 *>(cursor);
                    cursor += stride;
                } else {
                    vertices[2] = reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(cursor) + static_cast<std::int32_t>(cursor[1]) + 8);
                    cursor += 2;
                }
                std::uint32_t vertex = 0;
                if (count != 2) {
                    do {
                        if (*cursor & 1U) {
                            vertices[0] = vertices[1];
                            vertices[1] = vertices[2];
                            vertices[2] = reinterpret_cast<const Vector3 *>(cursor);
                            cursor += stride;
                        } else {
                            vertices[0] = vertices[1];
                            vertices[1] = vertices[2];
                            vertices[2] =
                                reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(cursor) + static_cast<std::int32_t>(cursor[1]) + 8);
                            cursor += 2;
                        }
                        UpdateHandGripTriangleBounds(bounds, vertices, winding, probes);
                        winding ^= 1U;
                        ++vertex;
                    } while (vertex < count - 2U);
                }
            } else {
                for (std::uint32_t triangle = 0; triangle < count; ++triangle) {
                    if (*cursor & 1U) {
                        vertices[0] = reinterpret_cast<const Vector3 *>(cursor);
                        cursor += stride;
                    } else {
                        vertices[0] = reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(cursor) + static_cast<std::int32_t>(cursor[1]) + 8);
                        cursor += 2;
                    }
                    if (*cursor & 1U) {
                        vertices[1] = reinterpret_cast<const Vector3 *>(cursor);
                        cursor += stride;
                    } else {
                        vertices[1] = reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(cursor) + static_cast<std::int32_t>(cursor[1]) + 8);
                        cursor += 2;
                    }
                    if (*cursor & 1U) {
                        vertices[2] = reinterpret_cast<const Vector3 *>(cursor);
                        cursor += stride;
                    } else {
                        vertices[2] = reinterpret_cast<const Vector3 *>(reinterpret_cast<const std::byte *>(cursor) + static_cast<std::int32_t>(cursor[1]) + 8);
                        cursor += 2;
                    }
                    UpdateHandGripTriangleBounds(bounds, vertices, winding, probes);
                }
            }
        } while (*cursor);
}

// @verified - d
void IntersectHandGripMeshes(float *bounds) {
    std::uint32_t words;
    while ((words = GetNumHandModelChunks(
                reinterpret_cast<const HandMeshHeader *>(reinterpret_cast<const std::uint32_t *>(handCalculationWork) + handGripMeshWords))) != 0) {
        const std::int32_t total = static_cast<std::int32_t>(static_cast<std::uint32_t>(handGripMeshWords) + words);
        if (total < handGripMeshCapacityWords) {
            IntersectHandGripMesh(bounds,
                reinterpret_cast<const HandMeshHeader *>(reinterpret_cast<const std::uint32_t *>(handCalculationWork) + handGripMeshWords),
                reinterpret_cast<const HandGripProbe *>(handProbeWork));
            handGripMeshWords = static_cast<std::int16_t>(static_cast<std::uint32_t>(handGripMeshWords) + words);
        }
    }
}

// @verified - d
void UpdateMeshHandGripBounds(CharacterHandComponent *hand) {
    for (std::int32_t group = 0; group < 5; ++group)
        if (!(hand->pose.bounds[group] < 0.0f))
            hand->pose.bounds[group] = 3.4028234663852886e+38f;
    handGripMeshWords = 0;
    IntersectHandGripMeshes(hand->pose.bounds);
    for (std::int32_t group = 0; group < 5; ++group)
        if (!(hand->pose.bounds[group] > 9.999999747378752e-06f))
            hand->pose.bounds[group] = -1.0f;
}

// @verified - d
void AdvanceGripAngles(CharacterHandComponent *hand, std::int32_t collide, std::uint32_t flags) {
    HandPoseState &state = hand->pose;
    std::int32_t divisor = hand->pose.poseTicks;
    if (divisor <= 0)
        divisor = 1;
    bool finished;
    if (--state.poseTicks < 1) {
        finished = true;
        state.poseTicks = 1;
    } else {
        finished = false;
    }
    bool unblocked = false;
    for (std::int32_t joint = 0; joint < 72; ++joint) {
        std::int32_t steps[3];
        steps[0] = GetHandAngleDifference(state.targetJointAngles[joint][0], state.currentJointAngles[joint][0]) / divisor;
        steps[1] = GetHandAngleDifference(state.targetJointAngles[joint][1], state.currentJointAngles[joint][1]) / divisor;
        steps[2] = GetHandAngleDifference(state.targetJointAngles[joint][2], state.currentJointAngles[joint][2]) / divisor;
        if (collide && handGripJointGroups[joint] >= 0) {
            if (hand->pose.bounds[handGripJointGroups[joint]] < 0.0f) {
                steps[0] = steps[1] = steps[2] = 0;
                if ((flags & 1) == 1)
                    finished = true;
            } else {
                unblocked = true;
            }
        }
        state.currentJointAngles[joint][0] = static_cast<std::int16_t>(state.currentJointAngles[joint][0] + steps[0]);
        state.currentJointAngles[joint][1] = static_cast<std::int16_t>(state.currentJointAngles[joint][1] + steps[1]);
        state.currentJointAngles[joint][2] = static_cast<std::int16_t>(state.currentJointAngles[joint][2] + steps[2]);
        if ((steps[0] || steps[1] || steps[2]) && !flags)
            finished = false;
    }
    if (!flags && !unblocked && collide)
        finished = true;
    if (finished) {
        state.poseMode &= 9;
        state.poseTicks = 0;
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
static void UpdateGripPose(CharacterHandComponent *hand, std::uint32_t flags) {
    HandPoseState *state = &hand->pose;
    hand->pose.previousFrame = hand->pose.currentFrame;
    BuildHandPoseMatricesNoPublish(static_cast<std::int32_t>(hand->pose.currentFrame * 2.0f), state, handPoseMatrices);
    ComputeHandTransformedProbes(hand->pose.jointPositions, hand->pose.extendedPoseWork);
    UpdateMeshHandGripBounds(hand);
    hand->pose.flags &= ~HandPoseInactive;
    if (hand->pose.poseTicks > 0)
        AdvanceGripAngles(hand, 1, flags);
    if (state->poseTicks <= 0)
        hand->pose.flags |= HandPoseInactive;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SolveHandGripPose(CharacterHandComponent *hand, const Matrix4x4 *base, MT7Node *geometry, const Matrix4x4 *, std::int32_t flags, std::byte *beginCommand,
    std::byte *endCommand) {
    std::int8_t poseFlags, mode;
    std::int16_t frame, previousFrame, ticks;
    std::int16_t current[72][3], target[72][3];
    float bounds[16];
    if (!hand->pose.modelData || gripDisabled)
        return;
    HandPoseState &state = hand->pose;
    poseFlags = hand->pose.flags;
    mode = hand->pose.poseMode;
    ticks = hand->pose.poseTicks;
    frame = hand->pose.currentFrame;
    previousFrame = hand->pose.previousFrame;
    std::memcpy(target, state.targetJointAngles, sizeof(target));
    std::memcpy(current, state.currentJointAngles, sizeof(current));
    std::memcpy(bounds, hand->pose.bounds, sizeof(bounds));
    ParseHandPoseCommand(*hand, beginCommand, 1);
    std::memcpy(state.currentJointAngles, state.targetJointAngles, sizeof(state.currentJointAngles));
    ParseHandPoseCommand(*hand, endCommand, 100);
    state.wristAngles[0] = 0;
    state.wristAngles[1] = 0;
    state.wristAngles[2] = 0;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::UnitCurrentMatrix();
    BuildHandGripGeometry(hand, geometry);
    matrix_stack::LoadMatrix(base);
    do
        UpdateGripPose(hand, flags);
    while (!(hand->pose.flags & HandPoseInactive));
    PopCurrentMatrix(1);
    WriteHandPoseCommand(hand, endCommand);
    hand->pose.flags = poseFlags;
    state.poseMode = mode;
    state.poseTicks = ticks;
    hand->pose.currentFrame = frame;
    hand->pose.previousFrame = previousFrame;
    std::memcpy(state.targetJointAngles, target, sizeof(target));
    std::memcpy(state.currentJointAngles, current, sizeof(current));
    std::memcpy(hand->pose.bounds, bounds, sizeof(bounds));
}

void *GetHandExtendedPoseWork(Character *character, std::int32_t side) {
    CharacterHandComponent *hand;
    if (side == 1 || side == 3)
        hand = AllocateHNDL(character);
    else
        hand = AllocateHNDR(character);
    if (hand)
        return hand->pose.extendedPoseWork;
    return 0;
}
#endif

// @verified - d
std::uint32_t GetLoadedHandModelFlags(Character *character) {
    std::uint32_t flags = 0;
    CharacterHandComponent *hand = AllocateHNDL(character);
    if (hand && hand->primaryNode)
        flags |= 2;
    hand = AllocateHNDR(character);
    if (hand && hand->primaryNode)
        flags |= 1;
    return flags;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t GetLoadedSecondaryHandModelFlags(Character *character) {
    std::uint32_t flags = 0;
    CharacterHandComponent *hand = AllocateHNDL(character);
    if (hand && hand->secondaryNode)
        flags |= 2;
    hand = AllocateHNDR(character);
    if (hand && hand->secondaryNode)
        flags |= 1;
    return flags;
}
#endif

// @verified - d
std::uint32_t AllocateHandModels(Character *character, CharacterHandComponent *hand) {
    PakGlobalData *package = GetPakLoadFlags() ? GetPakLoadTaskDataPointer() : 0;
    HandPoseState &state = hand->pose;
    std::uint32_t result = 2;
    if (!hand->primaryNode && !hand->secondaryNode) {
        state.meshWordCount = 0;
        state.meshBackup = 0;
        state.meshCopy = 0;
        if (static_cast<std::uint8_t>(state.selector - 3U) < 2) {
            std::int16_t id = 68;
            if (state.selector == 3)
                id = 69;
            hand->primaryNode = character->model;
            while (hand->primaryNode && hand->primaryNode->id != id)
                hand->primaryNode = hand->primaryNode->child;
        } else {
            char directory[32], filename[16];
            std::sprintf(directory, "scene/%02ld/model/hand", g_actNumber);
            std::strcpy(filename, hand->filename);
            MT7Model *model = QueryHandModelCacheNullsub(filename);
            if (!model) {
                package = 0;
                while ((package = GetNextReadyModelPackage(package))) {
                    if (FileLoader(package, filename, HAND_MODEL_EXTENSION) && (model = LoadGameModel(package, filename)))
                        break;
                }
            }
            if (model) {
                hand->primaryFile = 0;
                hand->primaryModel = model;
            } else {
                hand->primaryFile = shenLoadFile(directory, filename, HAND_MODEL_MEMORY);
                hand->primaryModel = InitializeMT7Model(hand->primaryFile);
            }
            if (hand->primaryModel) {
                std::int16_t id = 68;
                if (state.selector == 1)
                    id = 69;
                hand->primaryNode = hand->primaryModel->root;
                while (hand->primaryNode && hand->primaryNode->id != id)
                    hand->primaryNode = hand->primaryNode->child;
            }
            if (hand->pose.secondaryEnabled) {
                filename[4] = 'X';
                model = QueryHandModelCacheNullsub(filename);
                if (!model && package)
                    model = LoadGameModel(package, filename);
                if (model) {
                    hand->secondaryFile = 0;
                    hand->secondaryModel = model;
                } else {
                    hand->secondaryFile = shenLoadFile(directory, filename, HAND_SECONDARY_MODEL_MEMORY);
                    hand->secondaryModel = InitializeMT7Model(hand->secondaryFile);
                }
                if (hand->secondaryModel)
                    hand->secondaryNode = hand->secondaryModel->root;
            }
        }
        const MT7Node *const node = hand->primaryNode;
        if (node && node->mesh) {
            state.meshWordCount = static_cast<std::int16_t>(GetNumHandModelChunks(reinterpret_cast<const HandMeshHeader *>(hand->primaryNode->mesh)));
            state.meshBackup = reinterpret_cast<const std::uint32_t *>(hand->primaryNode->mesh);
            state.meshCopy = static_cast<std::uint32_t *>(HLib::AllocateTaskData(state.meshWordCount * 4, HAND_MESH_MEMORY));
            if (state.meshBackup && state.meshCopy)
                result = 1;
        }
    }
    if (state.meshWordCount > 0 && state.meshBackup && state.meshCopy) {
        const std::uint32_t *source = state.meshBackup;
        std::uint32_t *destination = state.meshCopy;
        for (std::int32_t index = 0; index < state.meshWordCount; ++index)
            *destination++ = *source++;
    }
    const shen_name *const chunk = reinterpret_cast<const shen_name *>(hand->primaryNode + 1);
    if (*chunk == HAND_MODEL_CHUNK)
        hand->pose.modelData = reinterpret_cast<const std::byte *>(chunk);
    SetCharacterHandRegionEnabled(hand, 0, 0, 1);
    SetHandPrimaryRotation(hand, 0, 0, 0);
    hand->pose.previousRegion = 0;
    hand->pose.alpha = 0.0f;
    return result;
}

// @verified - d
std::uint32_t SetLowQualityHands(Character *character) {
    std::uint32_t result = 0;
    CharacterHandComponent *hand = AllocateHNDL(character);
    if (hand)
        result = AllocateHandModels(character, hand);
    hand = AllocateHNDR(character);
    if (hand)
        result = AllocateHandModels(character, hand);
    return result;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t RetireHandModels(Character *character, CharacterHandComponent &hand) {
    if (!character)
        return 0;
    SetCharacterHandRegionEnabled(&hand, 0, 0, 1);
    SetHandPrimaryRotation(&hand, 0, 0, 0);
    hand.pose.previousRegion = 0;
    hand.pose.alpha = 0.0f;
    if (!std::strncmp(hand.filename, "RYO_T", 5))
        return 1;
    HandPoseState &pose = hand.pose;
    std::uint32_t result = 2;
    if (hand.primaryNode) {
        result = 1;
        if (pose.meshCopy && pose.meshBackup) {
            HLib::FreeTaskData(pose.meshCopy);
            hand.primaryNode->mesh = const_cast<std::uint32_t *>(pose.meshBackup);
            pose.meshBackup = 0;
            pose.meshCopy = 0;
        }
        if (static_cast<std::uint8_t>(pose.selector - 1U) < 2 && hand.primaryFile && hand.primaryModel)
            ReleaseMT7Model(hand.primaryModel);
    }
    hand.primaryNode = 0;
    hand.primaryModel = 0;
    hand.primaryFile = 0;
    if (hand.secondaryNode && hand.secondaryFile && hand.secondaryModel)
        ReleaseMT7Model(hand.secondaryModel);
    hand.secondaryNode = 0;
    hand.secondaryModel = 0;
    hand.secondaryFile = 0;
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t hndlSetFlags(Character *character) {
    std::uint32_t result = 0;
    if (CharacterHandComponent *const hand = AllocateHNDL(character))
        result = RetireHandModels(character, *hand);
    if (CharacterHandComponent *const hand = AllocateHNDR(character))
        result = RetireHandModels(character, *hand);
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void GetCharacterHandRotation(Character *character, std::int32_t right, Vector3i *rotation) {
    rotation->z = 0;
    rotation->y = 0;
    rotation->x = 0;
    MT7Node *const model = character->model;
    if (!model)
        return;
    MT7Node *node = 0;
    std::int8_t region = 0;
    if (right) {
        CharacterHandComponent *const hand = AllocateHNDR(character);
        if (hand) {
            region = hand->pose.region;
            if (region > 0)
                node = hand->primaryNode;
        }
        if (hand && !node && !region) {
            region = hand->pose.previousRegion;
            if (region > 0)
                node = hand->primaryNode;
        }
        if (!node && !region)
            node = FindMT7Node(model, -65);
    } else {
        CharacterHandComponent *const hand = AllocateHNDL(character);
        if (hand) {
            region = hand->pose.region;
            if (region > 0)
                node = hand->primaryNode;
        }
        if (hand && !node && !region) {
            region = hand->pose.previousRegion;
            if (region > 0)
                node = hand->primaryNode;
        }
        if (!node && !region)
            node = FindMT7Node(model, -66);
    }
    if (node) {
        rotation->x = node->rotation.x;
        rotation->y = node->rotation.y;
        rotation->z = node->rotation.z;
    }
}
#endif

// @verified - d
float hmdlGetDrawAlphaScaleEffect(const MT7Node *node, float alpha) {
    float blend = 0.0f;
    if (node->id == -66 && g_temporaryHNDL) {
        std::int8_t region = g_temporaryHNDL->pose.region;
        if (!region) {
            g_temporaryHNDL->pose.alpha -= 0.1f;
            if (!(g_temporaryHNDL->pose.alpha > 0.0f)) {
                g_temporaryHNDL->pose.alpha = 0.0f;
                g_temporaryHNDL->pose.previousRegion = 0;
            }
            region = g_temporaryHNDL->pose.previousRegion;
        } else {
            g_temporaryHNDL->pose.alpha += 0.1f;
            if (g_temporaryHNDL->pose.alpha > 1.0f)
                g_temporaryHNDL->pose.alpha = 1.0f;
        }
        blend = g_temporaryHNDL->pose.alpha;
        if (region > 0 && !hndlDrawPrimaryHand(g_temporaryHNDL, alpha * blend))
            blend = 0.0f;
        if (region < 0 && !hndlDrawSecondaryHand(g_temporaryHNDL, alpha * blend))
            blend = 0.0f;
    }
    if (node->id == -65 && g_temporaryHNDR) {
        std::int8_t region = g_temporaryHNDR->pose.region;
        if (!region) {
            g_temporaryHNDR->pose.alpha -= 0.1f;
            if (!(g_temporaryHNDR->pose.alpha > 0.0f)) {
                g_temporaryHNDR->pose.alpha = 0.0f;
                g_temporaryHNDR->pose.previousRegion = 0;
            }
            region = g_temporaryHNDR->pose.previousRegion;
        } else {
            g_temporaryHNDR->pose.alpha += 0.1f;
            if (g_temporaryHNDR->pose.alpha > 1.0f)
                g_temporaryHNDR->pose.alpha = 1.0f;
        }
        blend = g_temporaryHNDR->pose.alpha;
        if (region > 0 && !hndlDrawPrimaryHand(g_temporaryHNDR, alpha * blend))
            blend = 0.0f;
        if (region < 0 && !hndlDrawSecondaryHand(g_temporaryHNDR, alpha * blend))
            blend = 0.0f;
    }
    if (blend < kHandBodyVisibleAlpha)
        Matrix4x4::SetViewMatrixFromEuler(node->rotation.x, node->rotation.y, node->rotation.z);
    return 1.0f - blend;
}

// @verified - d
std::int32_t hndlDrawPrimaryHand(CharacterHandComponent *hand, float alpha) {
    MT7Node *node = hand->primaryNode;
    if (node) {
        node->mesh = hand->pose.meshCopy;
        node = hand->primaryModel->root;
        matrix_stack::Matrix4x4_Push(0);
        dc::sdk::PushGraphicsWorkspace();
        if (alpha > kHandOpaqueAlpha)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            DrawCharacterModelBranch(node);
#else
            reinterpret_cast<void (*)(MT7Node *)>(DrawCharacterModelBranch)(node);
#endif
        else if (alpha > kHandVisibleAlpha)
            DrawAlphaCharacterModelBranch(node, alpha);
        PopCurrentMatrix(1);
        dc::sdk::PopGraphicsWorkspace();
        return 1;
    }
    return 0;
}

// @verified - d
std::int32_t hndlDrawSecondaryHand(CharacterHandComponent *hand, float alpha) {
    MT7Node *const root = hand->secondaryNode;
    if (root) {
        matrix_stack::Matrix4x4_Push(0);
        dc::sdk::PushGraphicsWorkspace();
        if (alpha > kHandOpaqueAlpha)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            DrawCharacterModelBranch(root);
#else
            reinterpret_cast<void (*)(MT7Node *)>(DrawCharacterModelBranch)(root);
#endif
        else if (alpha > kHandVisibleAlpha)
            DrawAlphaCharacterModelBranch(root, alpha);
        PopCurrentMatrix(1);
        dc::sdk::PopGraphicsWorkspace();
        return 1;
    }
    return 0;
}

} // namespace s2
} // namespace shenmue
#undef SHENMUE_HAND_NULL
#undef SHENMUE_HAND_INITIALIZE
#undef SHENMUE_HAND_MAX_FLOAT
#undef SHENMUE_HAND_FMA
#undef SHENMUE_HAND_ANGLE

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" {

void __attribute__((renesas)) shenmue_s2_event_ApplyCharacterHandPoseCommand(
    shenmue::s2::Character *character, std::int32_t side, std::byte *command, std::int16_t ticks) {
    shenmue::s2::ApplyCharacterHandPoseCommand(character, side, command, ticks);
}

void __attribute__((renesas)) shenmue_s2_event_imgmGetPosXYZ(shenmue::s2::Character *character, std::int32_t side, shenmue::Vector3i *rotation) {
    if (const shenmue::s2::CharacterHandComponent *const hand = shenmue::s2::GetCharacterHand(character, side))
        shenmue::s2::GetHandPrimaryNodeRotation(*hand, *rotation);
}

void __attribute__((renesas)) shenmue_s2_event_getSpecialBonePos(
    shenmue::s2::Character *character, std::int32_t side, std::int32_t endpoint, shenmue::Vector3 *position) {
    if (const shenmue::s2::CharacterHandComponent *const hand = shenmue::s2::GetCharacterHand(character, side))
        shenmue::s2::GetHandEndpointPosition(*hand, endpoint, *position);
}

void __attribute__((renesas)) shenmue_s2_event_SetHighModelXYZ(shenmue::s2::Character *character, std::int8_t region, std::int32_t side, std::int16_t pose) {
    shenmue::s2::SetHighModelXYZ(character, region, side, pose);
}

void __attribute__((renesas)) shenmue_s2_event_SetHandRegionXYZWithFlags(
    shenmue::s2::Character *character, std::int8_t region, std::int32_t side, std::int16_t pose, std::int8_t alphaPolicy) {
    shenmue::s2::SetHandRegionXYZWithFlags(character, region, side, pose, alphaPolicy);
}
void __attribute__((renesas)) shenmue_s2_event_SetScenarioCharacterAnimation(shenmue::s2::Character *character, std::int32_t side, std::int16_t pose,
    std::int16_t first, std::int16_t last, std::int16_t start, std::uint16_t flags, float step, std::int16_t ticks) {
    if (shenmue::s2::CharacterHandComponent *const hand = shenmue::s2::GetCharacterHand(character, side))
        shenmue::s2::ConfigureHandPoseSequence(*hand, pose, first, last, start, static_cast<std::uint16_t>(flags), step, ticks);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_IsScenarioCharacterAnimationPaused(shenmue::s2::Character *character, std::int32_t side) {
    const shenmue::s2::CharacterHandComponent *const hand = shenmue::s2::GetCharacterHand(character, side);
    return hand ? shenmue::s2::IsHandPoseInactive(*hand) : -1;
}

void __attribute__((renesas)) shenmue_s2_event_SetScenarioCharacterAnimationFlags(
    shenmue::s2::Character *character, std::int32_t side, std::int16_t enabled, std::uint16_t flags) {
    if (shenmue::s2::CharacterHandComponent *const hand = shenmue::s2::GetCharacterHand(character, side))
        shenmue::s2::SetHandPoseFlags(*hand, enabled, flags);
}

void __attribute__((renesas)) shenmue_s2_event_SetScenarioCharacterAnimationSpeed(shenmue::s2::Character *character, std::int32_t side, float step) {
    if (shenmue::s2::CharacterHandComponent *const hand = shenmue::s2::GetCharacterHand(character, side))
        shenmue::s2::SetHandPoseStep(*hand, step);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_GetScenarioCharacterAnimationFrame(shenmue::s2::Character *character, std::int32_t side) {
    const shenmue::s2::CharacterHandComponent *const hand = shenmue::s2::GetCharacterHand(character, side);
    if (!hand)
        return -1;
    return shenmue::s2::GetHandPoseAnimationResult(*hand);
}

void __attribute__((renesas)) shenmue_s2_event_SetScenarioCharacterAnimationFrame(
    shenmue::s2::Character *character, std::int32_t side, shenmue::s2::Character *work) {
    if (shenmue::s2::CharacterHandComponent *const hand = shenmue::s2::GetCharacterHand(character, side))
        shenmue::s2::SetHandSecondaryPoseWork(*hand, work);
}

__attribute__((renesas)) shenmue::s2::CharacterHandComponent *shenmue_s2_event_GetLeftOrRight(shenmue::s2::Character *character, std::int32_t side) {
    return shenmue::s2::GetCharacterHand(character, side);
}
}
#endif

#endif
