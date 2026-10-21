#include "s1/task/char.hpp"
#include "s1/task/conv.hpp"
#include "logging.hpp"
#include "s1/face.hpp"
#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/game_utils.hpp"
#include "s1/task/effe.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/lighting.hpp"

#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/motion.hpp"
#include "s1/motion_registry.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/time.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/mbst.hpp"
#include "s1/render_array.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/cloth_presets.hpp"
#include "s1/collision.hpp"
#include "s1/scene.hpp"
#include "s1/shadow_volume.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/input.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/wind.hpp"

#include "s1/timing.hpp"
#include <bit>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <vector>
#include <limits>

#include "s1/task/char_components_internal.hpp"

namespace shenmue::s1 {

void HND_SetMatrixPools(Matrix4x4 *localMatrices, Matrix4x4 *worldMatrices, Matrix4x4 *inverseMatrices);
Matrix4x4 *HND_WorldMatrixPool();
Matrix4x4 *HND_InverseMatrixPool();
namespace {

static_assert(offsetof(MotionManagerWork, motion.headLookAngleBase) == 0xf0);
static_assert(offsetof(MotionManagerWork, motion.lookAtMode) == 0xfa);
static_assert(offsetof(MotionManagerWork, motion.currentTurnAngle) == 0x104);
static_assert(offsetof(MotionManagerWork, motion.activeBasePoseAnimIndex) == 0x270);
static_assert(sizeof(LookPointWork) == 0x1c);
static_assert(offsetof(LookPointWork, savedCommand) == 0xc);
static_assert(offsetof(LookPointWork, currentCommand) == 0x10);
static_assert(offsetof(LookPointWork, active) == 0x14);
static_assert(sizeof(HandPoseWork) == 0x1d0);
static_assert(sizeof(HandPoseBlendState) == 0xa8);
static_assert(offsetof(HandPoseBlendState, current) == 0x14);
static_assert(offsetof(HandPoseBlendState, target) == 0x3c);
static_assert(offsetof(HandPoseBlendState, step) == 0x64);
static_assert(offsetof(HandPoseBlendState, scratch) == 0x8c);
static_assert(offsetof(HandPoseWork, left) == 0xa8);
static_assert(sizeof(HandCommandState) == 0x40);
static_assert(offsetof(HandCommandState, cursor) == 0x10);
static_assert(offsetof(HandCommandState, callback) == 0x30);
static_assert(offsetof(HandCommandState, lastPose) == 0x38);
static_assert(offsetof(HandPoseWork, rightCommand) == 0x150);
static_assert(offsetof(HandPoseWork, leftCommand) == 0x190);



constexpr HandPoseRecord kHandPoses[15][2]{
    {{{0, 0, 0, 0, 12743, 0, 12743, 12743, 0, 12743}}, {{0, 0, 0, 0, 52793, 0, 52793, 52793, 0, 52793}}},
    {{{0, 0, 0, 3640, 0, 910, 0, 0, 64626, 0}}, {{0, 0, 0, 3640, 0, 910, 0, 0, 64626, 0}}},
    {{{0, 0, 0, 2421, 4241, 600, 4241, 4241, 64936, 4241}}, {{0, 0, 0, 2421, 61295, 600, 61295, 61295, 64936, 61295}}},
    {{{0, 0, 0, 1219, 8501, 309, 8501, 8501, 65227, 8501}}, {{0, 0, 0, 1219, 57035, 309, 57035, 57035, 65227, 57035}}},
    {{{0, 0, 0, 3640, 0, 910, 0, 12743, 0, 12743}}, {{0, 0, 0, 3640, 0, 910, 0, 52793, 0, 52793}}},
    {{{0, 49152, 0, 3640, 0, 910, 0, 0, 64626, 0}}, {{0, 16384, 0, 3640, 0, 910, 0, 0, 64626, 0}}},
    {{{0, 0, 0, 2421, 2730, 600, 4241, 4241, 64936, 6371}}, {{0, 0, 0, 2421, 62806, 600, 61295, 61295, 64936, 59165}}},
    {{{0, 0, 0, 0, 8192, 910, 8192, 0, 0, 3640}}, {{0, 0, 0, 0, 57344, 910, 57344, 0, 0, 61896}}},
    {{{0, 0, 0, 1219, 2730, 600, 4241, 4241, 64936, 10012}}, {{0, 0, 0, 1219, 62806, 600, 61295, 61295, 64936, 55524}}},
    {{{0, 0, 0, 1638, 3640, 364, 3640, 3640, 65172, 7281}}, {{0, 0, 0, 1638, 61896, 364, 61896, 61896, 65172, 58255}}},
    {{{0, 0, 0, 1820, 4551, 364, 4551, 4551, 65172, 10922}}, {{0, 0, 0, 1820, 60985, 364, 60985, 60985, 65172, 54614}}},
    {{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
    {{{0, 16384, 0, 0, 0, 0, 0, 0, 0, 0}}, {{0, 49152, 0, 0, 0, 0, 0, 0, 0, 0}}},
    {{{0, 0, 0, 3640, 6371, 1820, 4369, 6371, 0, 5461}}, {{0, 0, 0, 3640, 59165, 1820, 61167, 59165, 0, 60075}}},
    {{{0, 61896, 0, 0, 12743, 910, 12743, 12743, 64626, 12743}}, {{0, 3640, 0, 0, 52793, 910, 52793, 52793, 64626, 52793}}},
};
} // namespace



constexpr HandPoseRecord kMotionHandPoses[10][2]{
    {{{0, 0, 0, 3640, 0, 910, 0, 0, 64626, 0}}, {{0, 0, 0, 3640, 0, 910, 0, 0, 64626, 0}}},
    {{{0, 0, 0, 3185, 1601, 800, 1601, 1601, 64736, 1601}}, {{0, 0, 0, 3185, 63935, 800, 63935, 63935, 64736, 63935}}},
    {{{0, 0, 0, 2730, 3185, 691, 3185, 3185, 64845, 3185}}, {{0, 0, 0, 2730, 62351, 691, 62351, 62351, 64845, 62351}}},
    {{{0, 0, 0, 2275, 4787, 564, 4787, 4787, 64972, 4787}}, {{0, 0, 0, 2275, 60749, 564, 60749, 60749, 64972, 60749}}},
    {{{0, 0, 0, 1820, 6371, 455, 6371, 6371, 65081, 6371}}, {{0, 0, 0, 1820, 59165, 455, 59165, 59165, 65081, 59165}}},
    {{{0, 0, 0, 1365, 7973, 345, 7973, 7973, 65191, 7973}}, {{0, 0, 0, 1365, 57563, 345, 57563, 57563, 65191, 57563}}},
    {{{0, 0, 0, 910, 9557, 236, 9557, 9557, 65300, 9557}}, {{0, 0, 0, 910, 55979, 236, 55979, 55979, 65300, 55979}}},
    {{{0, 0, 0, 455, 11159, 127, 11159, 11159, 65409, 11159}}, {{0, 0, 0, 455, 54377, 127, 54377, 54377, 65409, 54377}}},
    {{{0, 0, 0, 0, 12743, 0, 12743, 12743, 0, 12743}}, {{0, 0, 0, 0, 52793, 0, 52793, 52793, 0, 52793}}},
    {{{0, 0, 0, 0, 2912, 0, 5461, 12743, 0, 12743}}, {{0, 0, 0, 0, 62624, 0, 60075, 52793, 0, 52793}}},
};
Mt5RuntimeNode *FindHandNodeInTree(Mt5RuntimeNode *node, std::byte *modelBase, std::int16_t nodeKind) {
    while (node) {
        if (static_cast<std::int16_t>(node->nodeKind) == nodeKind)
            return node;
        if (node->childOffset) {
            if (Mt5RuntimeNode *const found = FindHandNodeInTree(reinterpret_cast<Mt5RuntimeNode *>(modelBase + node->childOffset), modelBase, nodeKind)) {
                return found;
            }
        }
        node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + node->siblingOffset) : nullptr;
    }
    return nullptr;
}

Mt5RuntimeNode *shenGetNodeByChildIdx(Mt5RuntimeNode *node, std::int16_t nodeKind) {
    return node ? FindHandNodeInTree(node, MT5_ModelBaseForNode(node), nodeKind) : nullptr;
}
namespace {

// @verified - p
void mhndCaptureRightHandPose(Mt5RuntimeNode *hand, HandPoseRecord &pose) {
    if (!hand)
        return;
    pose.values[0] = hand->rotation.x;
    pose.values[1] = hand->rotation.y;
    pose.values[2] = hand->rotation.z;
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 25))
        pose.values[3] = node->rotation.z;
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 28)) {
        pose.values[4] = node->rotation.y;
        pose.values[5] = node->rotation.z;
        if (Mt5RuntimeNode *const child = shenGetNodeByChildIdx(node, 29))
            pose.values[6] = child->rotation.y;
    }
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 31)) {
        pose.values[7] = node->rotation.y;
        pose.values[8] = node->rotation.z;
        if (Mt5RuntimeNode *const child = shenGetNodeByChildIdx(node, 32))
            pose.values[9] = child->rotation.y;
    }
}

// @verified - p
void mhndCaptureLeftHandPose(Mt5RuntimeNode *hand, HandPoseRecord &pose) {
    if (!hand)
        return;
    pose.values[0] = hand->rotation.x;
    pose.values[1] = hand->rotation.y;
    pose.values[2] = hand->rotation.z;
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 40))
        pose.values[3] = node->rotation.z;
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 43)) {
        pose.values[4] = node->rotation.y;
        pose.values[5] = node->rotation.z;
        if (Mt5RuntimeNode *const child = shenGetNodeByChildIdx(node, 44))
            pose.values[6] = child->rotation.y;
    }
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 46)) {
        pose.values[7] = node->rotation.y;
        pose.values[8] = node->rotation.z;
        if (Mt5RuntimeNode *const child = shenGetNodeByChildIdx(node, 47))
            pose.values[9] = child->rotation.y;
    }
}

void HND_Callback(Character *character, void *payload);
bool g_initializingHandPose{};

// Ryo's HD arms for a character whose image module is not one of the arm-less base poses (16-18, 20). The character's id goes to
// the debug name buffer Shenmue::currentCHAR.
// @verified - p
void LoadRyoArms(Character *character) {
    if (!character)
        return;
    ImageModuleWork *const images = IMGM_GetWork(character);
    if (images && ((images->basePoseId >= 16 && images->basePoseId <= 18) || images->basePoseId == 20))
        return;
    AllocateAndLoadArms(character, "YKB_TL.MT5", "YKB_TR.MT5", 1, true);
    MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    auto *const scenario = control && control->scenarioTask ? reinterpret_cast<ScenarioTaskData *>(control->scenarioTask->task_data_ptr) : nullptr;
    std::uint32_t characterId{};
    if (scenario) {
        SCEN_PromoteCharacterIndexNode(control->scenarioTask, character);
        CharacterIdLookupNode *const node = scenario->characterBuckets[reinterpret_cast<std::uintptr_t>(character) % 13U];
        if (node && node->character == character)
            characterId = node->id;
    }
    std::memcpy(g_currentCharacterName, &characterId, sizeof(characterId));
    g_currentCharacterName[4] = 0;
}

// The pose's node rotations, then (outside MHND's own initialisation) the HD right hand follows them, loading Ryo's arms first
// when the character has none. The second-joint lookups start from their first joints, the left thumb's included (node 26).
// @verified - p
void mhndApplyRightHandPose(Character *character, Mt5RuntimeNode *hand, const HandPoseRecord &pose) {
    if (!hand)
        return;
    hand->rotation = {pose.values[0], pose.values[1], pose.values[2]};
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 25)) {
        node->rotation.z = pose.values[3];
        if (Mt5RuntimeNode *const child = shenGetNodeByChildIdx(node, 26))
            child->rotation.z = pose.values[3];
    }
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 28)) {
        node->rotation.y = pose.values[4];
        node->rotation.z = pose.values[5];
        if (Mt5RuntimeNode *const child = shenGetNodeByChildIdx(node, 29))
            child->rotation.y = pose.values[6];
    }
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 31)) {
        node->rotation.y = pose.values[7];
        node->rotation.z = pose.values[8];
        if (Mt5RuntimeNode *const child = shenGetNodeByChildIdx(node, 32))
            child->rotation.y = pose.values[9];
    }
    if (g_initializingHandPose)
        return;
    HandModelWork *model = HND_GetWork(character, false);
    if (!model) {
        LoadRyoArms(character);
        model = HND_GetWork(character, false);
    }
    if (model)
        HND_Callback(character, model);
}

// @verified - p
void mhndApplyLeftHandPose(Character *character, Mt5RuntimeNode *hand, const HandPoseRecord &pose) {
    if (!hand)
        return;
    hand->rotation = {pose.values[0], pose.values[1], pose.values[2]};
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 40)) {
        node->rotation.z = pose.values[3];
        if (Mt5RuntimeNode *const child = shenGetNodeByChildIdx(node, 26))
            child->rotation.z = pose.values[3];
    }
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 43)) {
        node->rotation.y = pose.values[4];
        node->rotation.z = pose.values[5];
        if (Mt5RuntimeNode *const child = shenGetNodeByChildIdx(node, 44))
            child->rotation.y = pose.values[6];
    }
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(hand, 46)) {
        node->rotation.y = pose.values[7];
        node->rotation.z = pose.values[8];
        if (Mt5RuntimeNode *const child = shenGetNodeByChildIdx(node, 47))
            child->rotation.y = pose.values[9];
    }
    if (g_initializingHandPose)
        return;
    HandModelWork *model = HND_GetWork(character, true);
    if (!model) {
        LoadRyoArms(character);
        model = HND_GetWork(character, true);
    }
    if (model)
        HND_Callback(character, model);
}

HandPoseWork *FindHandPoseWork(Character *character);
HandPoseWork *EnsureHandPoseWork(Character *character);
HandPoseWork *AllocateHandPoseWork(Character *character);

// @verified - p
void mhndInitHandPoseBlendState(HandPoseBlendState &state, Mt5RuntimeNode *node, const HandPoseRecord &current, const HandPoseRecord &target, std::int32_t hand,
    std::int32_t poseIndex, std::int32_t frameCount) {
    state.hand = hand;
    state.poseIndex = poseIndex;
    state.node = node;
    state.framesRemaining = frameCount;
    state.current = current;
    state.target = target;
    for (std::size_t index = 0; index < std::size(state.step); ++index)
        state.step[index] = (static_cast<std::int16_t>(target.values[index]) - static_cast<std::int16_t>(current.values[index])) / frameCount;
}

// A blend toward table pose poseIndex for the right (0), left (1) or both (2) hands. One capture buffer serves both hands, so a
// left hand missing joints keeps the right hand's captured values for them.
// @verified - p
void mhndBeginHandPoseBlend(HandPoseWork &work, Mt5RuntimeNode *model, std::int32_t hand, std::int32_t poseIndex, std::int32_t frameCount) {
    if (poseIndex >= 15)
        return;
    HandPoseRecord current{};
    if (hand == 0 || hand == 2) {
        if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, -65)) {
            mhndCaptureRightHandPose(node, current);
            mhndInitHandPoseBlendState(work.right, node, current, kHandPoses[poseIndex][0], 0, poseIndex, frameCount);
        }
    }
    if (hand == 1 || hand == 2) {
        if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, -66)) {
            mhndCaptureLeftHandPose(node, current);
            mhndInitHandPoseBlendState(work.left, node, current, kHandPoses[poseIndex][1], 1, poseIndex, frameCount);
        }
    }
}

// mhndBeginHandPoseBlend toward a caller's pose (pose index -1); both hands take the same target.
// @verified - p
void mhndBeginHandTargetBlend(HandPoseWork &work, Mt5RuntimeNode *model, std::int32_t hand, const HandPoseRecord &target, std::int32_t frameCount) {
    HandPoseRecord current{};
    if (hand == 0 || hand == 2) {
        if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, -65)) {
            mhndCaptureRightHandPose(node, current);
            mhndInitHandPoseBlendState(work.right, node, current, target, 0, -1, frameCount);
        }
    }
    if (hand == 1 || hand == 2) {
        if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, -66)) {
            mhndCaptureLeftHandPose(node, current);
            mhndInitHandPoseBlendState(work.left, node, current, target, 1, -1, frameCount);
        }
    }
}
} // namespace



// @verified - p
void mhndApplyCharaHandTarget(Character *character, std::int32_t hand, const HandPoseRecord &target, std::int32_t frameCount) {
    if (!character || hand >= 3)
        return;
    HandPoseWork *work = FindHandPoseWork(character);
    if (work) {
        if (hand == 0 || hand == 2) {
            work->rightCommand.mode = 0;
            work->right.poseIndex = -1;
        }
        if (hand == 1 || hand == 2) {
            work->leftCommand.mode = 0;
            work->left.poseIndex = -1;
        }
    }
    auto *const model = static_cast<Mt5RuntimeNode *>(character->model);
    if (!model)
        return;
    if (frameCount > 1) {
        work = EnsureHandPoseWork(character);
        if (work)
            mhndBeginHandTargetBlend(*work, model, hand, target, frameCount);
        return;
    }
    if (hand == 0) {
        if (work)
            work->right.node = nullptr;
        mhndApplyRightHandPose(character, shenGetNodeByChildIdx(model, -65), target);
    } else if (hand == 1) {
        if (work)
            work->left.node = nullptr;
        mhndApplyLeftHandPose(character, shenGetNodeByChildIdx(model, -66), target);
    } else if (hand == 2) {
        if (work) {
            work->right.node = nullptr;
            work->left.node = nullptr;
        }
        mhndApplyRightHandPose(character, shenGetNodeByChildIdx(model, -65), target);
        mhndApplyLeftHandPose(character, shenGetNodeByChildIdx(model, -66), target);
    }
}
namespace {

// The automatic hand program's callback: every 60-123 frames the right hand picks one of the four relaxed poses (re-applied even
// when it is the current one), and the left hand mirrors the right hand's last pick.
// @verified - p
void AutomaticHandCommand(Character *character, HandCommandState &command, HandPoseBlendState &blend) {
    std::int32_t pose = 0;
    if (blend.hand == 0) {
        if (--command.framesRemaining >= 0)
            return;
        const std::uint32_t random = LcgStep(g_EnvPrngSeed);
        command.framesRemaining = static_cast<std::int32_t>(((random >> 16U) & 0x3fU) + 60U);
        g_EnvPrngSeed = LcgStep(random);
        pose = static_cast<std::int32_t>((g_EnvPrngSeed >> 16U) & 3U);
    } else if (blend.hand == 1) {
        HandPoseWork *const work = character ? FindHandPoseWork(character) : nullptr;
        if (!work)
            return;
        pose = work->rightCommand.lastPose;
        if (command.lastPose == pose)
            return;
    } else {
        return;
    }
    command.lastPose = pose;
    mhndApplyCharaHandPose(character, blend.hand, pose, 16);
    command.mode = 13;
}

// One hand's command program: waits (6), blend waits (7), callbacks (13) and the opcode stream (14). A callback that leaves the
// program at 14 runs on into the stream.
// @verified - p
void ProcessHandCommands(Character *character, HandCommandState &command, HandPoseBlendState &blend) {
    do {
        for (;;) {
            while (command.mode == 6U) {
                if (--command.framesRemaining >= 0)
                    return;
                command.mode = 14;
            }
            if (command.mode != 7U)
                break;
            if (blend.node)
                return;
            command.mode = 14;
        }
        if (command.mode == 13U) {
            if (command.callback)
                command.callback(character, command, blend);
        } else if (command.mode == 14U) {
            const std::int32_t *const record = command.cursor;
            const std::int32_t opcode = record[0];
            command.mode = static_cast<std::uint32_t>(opcode);
            command.cursor = record + 2;
            switch (opcode) {
            case 1:
            case 2:
            case 3:
            case 4:
                command.command = opcode;
                command.mode = 14;
                continue;
            case 5:
                command.cursor = command.loopStart;
                command.mode = 14;
                continue;
            case 6:
                command.framesRemaining = record[2];
                command.cursor = record + 4;
                break;
            case 8:
                command.loopStart = record + 2;
                command.mode = 14;
                continue;
            case 9:
                command.loopEnd = record + 2;
                command.mode = 14;
                continue;
            case 10:
                if (--command.loopCount >= 0)
                    command.cursor = command.loopEnd;
                command.mode = 14;
                continue;
            case 11:
                command.cursor = record + 6;
                mhndApplyCharaHandPose(character, blend.hand, record[2], record[4]);
                command.mode = 7;
                return;
            case 12: {
                HandPoseRecord target{};
                std::memcpy(&target, record + 2, sizeof(target));
                command.cursor = record + 24;
                mhndApplyCharaHandTarget(character, blend.hand, target, record[22]);
                command.mode = 7;
                return;
            }
            case 13:
                std::memcpy(&command.callback, record + 2, sizeof(command.callback));
                command.cursor = record + 4;
                break;
            default:
                command.mode = 0;
                return;
            }
        }
    } while (command.mode == 14U);
}

// One frame of a hand blend: the last frame lands on the target, earlier frames add the step; the hand then takes the pose.
// @verified - p
void mhndAdvanceHandPoseBlend(Character *character, HandPoseBlendState &state, std::int32_t hand) {
    for (std::size_t index = 0; index < std::size(state.current.values); ++index) {
        if (state.framesRemaining <= 0)
            state.current.values[index] = state.target.values[index];
        else
            state.current.values[index] += state.step[index];
    }
    if (hand == 0)
        mhndApplyRightHandPose(character, state.node, state.current);
    else if (hand == 1)
        mhndApplyLeftHandPose(character, state.node, state.current);
}

// @verified - p
void MHND_Callback(Character *character, void *payload) {
    auto &work = *static_cast<HandPoseWork *>(payload);
    auto *const model = static_cast<Mt5RuntimeNode *>(character->model);
    Mt5RuntimeNode *const right = shenGetNodeByChildIdx(model, -65);
    if (work.right.node)
        work.right.node = right;
    if (work.rightCommand.mode)
        ProcessHandCommands(character, work.rightCommand, work.right);
    if (work.right.node) {
        --work.right.framesRemaining;
        mhndAdvanceHandPoseBlend(character, work.right, 0);
        if (work.right.framesRemaining < 0)
            work.right.node = nullptr;
    }
    Mt5RuntimeNode *const left = shenGetNodeByChildIdx(model, -66);
    if (work.left.node)
        work.left.node = left;
    if (work.leftCommand.mode)
        ProcessHandCommands(character, work.leftCommand, work.left);
    if (work.left.node) {
        --work.left.framesRemaining;
        mhndAdvanceHandPoseBlend(character, work.left, 1);
        if (work.left.framesRemaining < 0)
            work.left.node = nullptr;
    }
}

HandPoseWork *FindHandPoseWork(Character *character) {
    CharacterComponentNode *const node = HLib_GetCharacterComponent(character, SHEN_NAME(MHND));
    return node ? reinterpret_cast<HandPoseWork *>(node + 1) : nullptr;
}

// @verified - p
void mhndAllocate(HandPoseWork &work) {
    std::memset(&work, 0, sizeof(work));
    work.right.hand = 0;
    work.right.poseIndex = -3;
    for (std::size_t index = 0; index < std::size(work.right.scratch); ++index) {
        work.left.scratch[index] = -1.0f;
        work.right.scratch[index] = -1.0f;
    }
    work.left.hand = 1;
    work.left.poseIndex = -3;
}

HandPoseWork *AllocateHandPoseWork(Character *character) {
    auto *const work = static_cast<HandPoseWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(MHND), sizeof(HandPoseWork), MHND_Callback));
    if (work)
        mhndAllocate(*work);
    return work;
}

HandPoseWork *EnsureHandPoseWork(Character *character) {
    if (!character || !character->model)
        return nullptr;
    if (HandPoseWork *const existing = FindHandPoseWork(character)) {
        if (CharacterComponentNode *const node = HLib_GetCharacterComponent(character, SHEN_NAME(MHND)))
            node->flags |= 1U;
        return existing;
    }
    return AllocateHandPoseWork(character);
}

constexpr std::uint32_t kLeftHandModelComponent = SHEN_NAME(HNDL);
constexpr std::uint32_t kRightHandModelComponent = SHEN_NAME(HNDR);
constexpr std::size_t kHandVertexStride = 24;
static_assert(sizeof(HandMotionModelHeader) == 0x18);
static_assert(sizeof(HandBindTransform) == 0x10);
static_assert(sizeof(HandDeformVertex) == kHandVertexStride);
static_assert(sizeof(HandAngleKeyDescriptor) == 8);
static_assert(sizeof(HandFingerCurveSet) == 0x18);
static_assert(sizeof(HandAngleKey) == 0x10);

constexpr std::size_t kHandMatrixCount = 71;
constexpr std::size_t kHandBindTransformsPerJoint = 5;
constexpr std::size_t kHandJointStateCount = 71;
constexpr std::size_t kHandStoredPositionCount = 71;


constexpr std::int16_t kHandInverseMatrixNodes[kHandMatrixCount]{
    -1,
    -1,
    0,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    1,
    2,
    3,
    -1,
    4,
    5,
    6,
    -1,
    7,
    8,
    9,
    -1,
    10,
    11,
    12,
    -1,
    13,
    14,
    15,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
};
constexpr std::int16_t kHandFingerBlendGroups[kHandMatrixCount]{
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    2,
    2,
    2,
    2,
    3,
    3,
    3,
    3,
    4,
    4,
    4,
    4,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
};
static_assert(sizeof(HandFingerProbe) == 0x20);
constexpr HandFingerProbe kHandFingerProbes[5]{{{0.013f, 0.005f, -0.006f}, 0.013f, {-0.01f, 0.003f, -0.005f}, 0.013f},
    {{0.015f, 0.005f, 0.0f}, 0.0108f, {0.0f, 0.003f, 0.0f}, 0.0108f}, {{0.018f, 0.005f, 0.0f}, 0.0108f, {0.0f, 0.003f, 0.0f}, 0.0108f},
    {{0.023f, 0.004f, 0.0f}, 0.0116f, {0.0f, 0.003f, 0.0f}, 0.0116f}, {{0.015f, 0.003f, -0.001f}, 0.0106f, {0.0f, 0.001f, -0.001f}, 0.0106f}};
constexpr std::int16_t kHandProbeJointIndices[kHandMatrixCount]{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 1, -1, -1, -1, 2, -1, -1, -1, 3, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1};
HandFingerProbe g_handFingerProbes[5]{};
std::int16_t g_handCollisionVertexOffset = 0;

template <typename T> const T *HandMotionPointer(const std::byte *base, std::size_t size, std::uint32_t offset, std::size_t count = 1) {
    if (!base || offset > size || count > (size - offset) / sizeof(T)) {
        return nullptr;
    }
    return reinterpret_cast<const T *>(base + offset);
}

std::int32_t HandRadiansToFixed16(float radians) {
    return static_cast<std::int32_t>(static_cast<double>(radians) * 32768.0 / 3.141592653589793);
}

void StoreHandAngles(std::byte *angles, std::uint8_t joint, std::int32_t x, std::int32_t y, std::int32_t z) {
    if (!angles || joint >= kHandJointStateCount)
        return;
    std::byte *const destination = angles + static_cast<std::size_t>(joint) * 12;
    std::memcpy(destination, &x, sizeof(x));
    std::memcpy(destination + 4, &y, sizeof(y));
    std::memcpy(destination + 8, &z, sizeof(z));
}

std::array<std::int32_t, 3> LoadHandAngles(const std::byte *angles, std::uint8_t joint) {
    std::array<std::int32_t, 3> result{};
    if (!angles || joint >= kHandJointStateCount)
        return result;
    std::memcpy(result.data(), angles + static_cast<std::size_t>(joint) * 12, 12);
    return result;
}



std::int32_t g_handAnimFrameIndex = 0;
const HandFingerNode *g_fingerNodeCursor = nullptr;
const HandBindTransform *g_fingerBindCursor = nullptr;
const HandFingerCurveSet *g_handAnimKeys = nullptr;
std::uint8_t g_useFingerAngleDeltas = 0;
std::byte *g_fingerAngleDeltas = nullptr;
std::byte *g_fingerJointPositions = nullptr;
Matrix4x4 *g_fingerMatrixOutput = nullptr;


constexpr float kDefaultHandScaleTable[6]{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
constexpr HandScaleColumn kHandScaleColumns[15]{
    {29, 1}, {30, 1}, {31, 1}, {45, 3}, {46, 5}, {47, 5}, {41, 3}, {42, 5}, {43, 5}, {37, 3}, {38, 5}, {39, 5}, {33, 2}, {34, 4}, {35, 4}};

const HandFingerNode *HandHierarchy(const HandModelWork &hand) {
    const std::size_t motionSize = HND_MotionModelSize(hand.handMotionTableIndex);
    const std::byte *const motionBase = static_cast<const std::byte *>(hand.handMotionModel);
    if (motionSize < sizeof(HandMotionModelHeader) || !motionBase)
        return nullptr;
    HandMotionModelHeader header{};
    std::memcpy(&header, motionBase, sizeof(header));
    return header.hierarchyOffset ? HandMotionPointer<HandFingerNode>(motionBase, motionSize, header.hierarchyOffset, kHandMatrixCount) : nullptr;
}

const HandBindTransform *HandBindTransforms(const HandModelWork &hand) {
    const std::size_t motionSize = HND_MotionModelSize(hand.handMotionTableIndex);
    const std::byte *const motionBase = static_cast<const std::byte *>(hand.handMotionModel);
    if (motionSize < sizeof(HandMotionModelHeader) || !motionBase)
        return nullptr;
    HandMotionModelHeader header{};
    std::memcpy(&header, motionBase, sizeof(header));
    return HandMotionPointer<HandBindTransform>(motionBase, motionSize, header.bindTransformsOffset, kHandMatrixCount * kHandBindTransformsPerJoint);
}


std::int32_t EvaluateAngleCurveHermite(std::int32_t count, const HandAngleKey *keys) {
    std::int32_t upper = count;
    std::int32_t lower = 0;
    do {
        const std::int32_t middle = (lower + upper) / 2;
        if (keys[middle].frame < g_handAnimFrameIndex)
            lower = middle;
        else
            upper = middle;
    } while (upper > lower + 1);
    float angle = keys[0].angleDegrees;
    if (upper) {
        const std::int32_t first = upper - 1;
        if (upper == count || keys[first].frame == g_handAnimFrameIndex) {
            angle = keys[first].angleDegrees;
        } else {
            const float duration = static_cast<float>(keys[upper].frame - keys[first].frame);
            const float amount = static_cast<float>(g_handAnimFrameIndex - keys[first].frame) / duration;
            const float amount2 = amount * amount;
            const float amount3 = amount2 * amount;
            const float tangents = ((amount3 - (amount2 + amount2)) + amount) * keys[first].outTangent + (amount3 - amount2) * keys[upper].inTangent;
            const float values =
                (((amount3 + amount3) - amount2 * 3.0f) + 1.0f) * keys[first].angleDegrees + (amount2 * 3.0f - (amount3 + amount3)) * keys[upper].angleDegrees;
            angle = tangents * duration + values;
        }
    }
    return static_cast<std::int32_t>(static_cast<double>(angle) * 65536.0 / 360.0);
}

const HandAngleKey *HandAngleKeys(const HandAngleKeyDescriptor &descriptor) {
    return static_cast<const HandAngleKey *>(HND_ResolveMotionOffset(descriptor.offset, static_cast<std::size_t>(descriptor.count) * sizeof(HandAngleKey)));
}

void StoreHandJointPosition(std::byte *positions, std::uint8_t joint, const Matrix4x4 &matrix) {
    if (!positions || joint >= kHandStoredPositionCount)
        return;
    const Vector3 position{matrix.rows[3].x, matrix.rows[3].y, matrix.rows[3].z};
    std::memcpy(positions + static_cast<std::size_t>(joint) * sizeof(Vector3), &position, sizeof(position));
}



void HMOT_UpdateFingerTransformsRecursive() {
    const HandFingerNode node = *g_fingerNodeCursor++;
    std::int32_t childCount = node.childCount;
    std::int32_t bindX = 0;
    std::int32_t bindY = 0;
    std::int32_t bindZ = 0;
    if (node.jointIndex) {
        bindX = HandRadiansToFixed16(g_fingerBindCursor->value.x);
        bindY = HandRadiansToFixed16(g_fingerBindCursor->value.y);
        bindZ = HandRadiansToFixed16(g_fingerBindCursor->value.z);
    }
    std::int32_t angleX = bindX;
    std::int32_t angleY = bindY;
    std::int32_t angleZ = bindZ;
    if (g_useFingerAngleDeltas) {
        const std::array<std::int32_t, 3> delta = LoadHandAngles(g_fingerAngleDeltas, node.jointIndex);
        angleX += delta[0];
        angleY += delta[1];
        angleZ += delta[2];
    } else {
        if (const HandFingerCurveSet *const curve = g_handAnimKeys) {
            if (curve->axes[0].count > 0) {
                if (const HandAngleKey *const keys = HandAngleKeys(curve->axes[0]))
                    angleX = EvaluateAngleCurveHermite(curve->axes[0].count, keys);
            }
            if (curve->axes[1].count > 0) {
                if (const HandAngleKey *const keys = HandAngleKeys(curve->axes[1]))
                    angleY = EvaluateAngleCurveHermite(curve->axes[1].count, keys);
            }
            if (curve->axes[2].count > 0) {
                if (const HandAngleKey *const keys = HandAngleKeys(curve->axes[2]))
                    angleZ = EvaluateAngleCurveHermite(curve->axes[2].count, keys);
            }
            g_handAnimKeys = curve + 1;
        }
        StoreHandAngles(g_fingerAngleDeltas, node.jointIndex, angleX - bindX, angleY - bindY, angleZ - bindZ);
    }
    if (node.jointIndex)
        Matrix4x4::preTranslate(g_fingerBindCursor->unused, 0.0f, 0.0f);
    else
        Matrix4x4::preTranslate(g_fingerBindCursor->value.x, g_fingerBindCursor->value.y, g_fingerBindCursor->value.z);
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(angleZ));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(angleY));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(angleX));
    g_fingerBindCursor += kHandBindTransformsPerJoint;
    *g_fingerMatrixOutput++ = workTransformMatrix;
    StoreHandJointPosition(g_fingerJointPositions, node.jointIndex, workTransformMatrix);
    for (; childCount > 0; --childCount) {
        Matrix4x4_Push();
        HMOT_UpdateFingerTransformsRecursive();
        Matrix4x4::Pop(1);
    }
}


void HND_UpdateFingers(std::int32_t frameIndex, HandModelWork &hand, Matrix4x4 *output) {
    const HandFingerNode *const hierarchy = HandHierarchy(hand);
    const HandBindTransform *const bind = HandBindTransforms(hand);
    if (!hierarchy || !bind || !output)
        return;
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    g_handAnimFrameIndex = frameIndex;
    g_fingerNodeCursor = hierarchy;
    g_fingerBindCursor = bind;
    g_handAnimKeys = static_cast<const HandFingerCurveSet *>(hand.animationCurves);
    g_useFingerAngleDeltas = hand.animationFlags;
    g_fingerAngleDeltas = hand.angleDeltas;
    g_fingerJointPositions = hand.jointPositions;
    g_fingerMatrixOutput = output;
    HMOT_UpdateFingerTransformsRecursive();
    Matrix4x4::Pop(1);
}


void hmComputeAngleDeltasForChain() {
    const HandFingerNode node = *g_fingerNodeCursor++;
    std::int32_t childCount = node.childCount;
    std::int32_t bindX = 0;
    std::int32_t bindY = 0;
    std::int32_t bindZ = 0;
    if (node.jointIndex) {
        bindX = HandRadiansToFixed16(g_fingerBindCursor->value.x);
        bindZ = HandRadiansToFixed16(g_fingerBindCursor->value.z);
        bindY = HandRadiansToFixed16(g_fingerBindCursor->value.y);
    }
    std::int32_t angleX = bindX;
    std::int32_t angleY = bindY;
    std::int32_t angleZ = bindZ;
    if (const HandFingerCurveSet *const curve = g_handAnimKeys) {
        if (curve->axes[0].count > 0) {
            if (const HandAngleKey *const keys = HandAngleKeys(curve->axes[0]))
                angleX = EvaluateAngleCurveHermite(curve->axes[0].count, keys);
        }
        if (curve->axes[1].count > 0) {
            if (const HandAngleKey *const keys = HandAngleKeys(curve->axes[1]))
                angleY = EvaluateAngleCurveHermite(curve->axes[1].count, keys);
        }
        if (curve->axes[2].count > 0) {
            if (const HandAngleKey *const keys = HandAngleKeys(curve->axes[2]))
                angleZ = EvaluateAngleCurveHermite(curve->axes[2].count, keys);
        }
        g_handAnimKeys = curve + 1;
    }
    g_fingerBindCursor += kHandBindTransformsPerJoint;
    StoreHandAngles(g_fingerAngleDeltas, node.jointIndex, angleX - bindX, angleY - bindY, angleZ - bindZ);
    for (; childCount > 0; --childCount)
        hmComputeAngleDeltasForChain();
}
} // namespace

Vector3 TransformHandPoint(const Matrix4x4 &matrix, const Vector3 &point) {
    return {matrix.rows[0].x * point.x + matrix.rows[1].x * point.y + matrix.rows[2].x * point.z + matrix.rows[3].x,
        matrix.rows[0].y * point.x + matrix.rows[1].y * point.y + matrix.rows[2].y * point.z + matrix.rows[3].y,
        matrix.rows[0].z * point.x + matrix.rows[1].z * point.y + matrix.rows[2].z * point.z + matrix.rows[3].z};
}
namespace {

Vector3 TransformHandBindPoint(const HandBindTransform *bind, const Vector3 &point) {
    return {bind[1].unused * point.x + bind[1].value.x * point.y + bind[1].value.y * point.z,
        bind[2].unused * point.x + bind[2].value.x * point.y + bind[2].value.y * point.z,
        bind[3].unused * point.x + bind[3].value.x * point.y + bind[3].value.y * point.z};
}



void HND_DeformVertices(HandModelWork &hand, const Matrix4x4 *matrices) {
    if (!hand.sourceVertices || !hand.mutableVertices)
        return;
    const std::size_t motionSize = HND_MotionModelSize(hand.handMotionTableIndex);
    const std::byte *const motionBase = static_cast<const std::byte *>(hand.handMotionModel);
    if (motionSize < sizeof(HandMotionModelHeader) || !motionBase || !matrices || hand.vertexCount <= 0)
        return;
    HandMotionModelHeader header{};
    std::memcpy(&header, motionBase, sizeof(header));
    const std::size_t vertexCount = static_cast<std::size_t>(hand.vertexCount);
    const std::int8_t *counts = HandMotionPointer<std::int8_t>(motionBase, motionSize, header.influenceCountsOffset, vertexCount);
    if (!counts)
        return;
    std::size_t influenceCount = 0;
    for (std::int32_t vertex = 0; vertex < hand.vertexCount; ++vertex) {
        if (counts[vertex] > 0)
            influenceCount += static_cast<std::uint8_t>(counts[vertex]);
    }
    const std::uint8_t *indices = HandMotionPointer<std::uint8_t>(motionBase, motionSize, header.influenceIndicesOffset, influenceCount);
    const float *weights = HandMotionPointer<float>(motionBase, motionSize, header.influenceWeightsOffset, influenceCount);
    const HandBindTransform *const bindBase =
        HandMotionPointer<HandBindTransform>(motionBase, motionSize, header.bindTransformsOffset, kHandMatrixCount * kHandBindTransformsPerJoint);
    if ((influenceCount && (!indices || !weights)) || !bindBase)
        return;

    const HandDeformVertex *source = static_cast<const HandDeformVertex *>(hand.sourceVertices);
    HandDeformVertex *destination = static_cast<HandDeformVertex *>(hand.mutableVertices);
    const bool flipDepth = ((hand.side - 1U) & 0xfdU) == 0;
    std::size_t influence = 0;
    for (std::int32_t vertex = 0; vertex < hand.vertexCount; ++vertex) {
        const HandDeformVertex &input = source[vertex];
        const Vector3 originalFirst{input.x, -input.y, flipDepth ? -input.z : input.z};
        const Vector3 originalSecond{input.x + input.width, -input.y - input.height, flipDepth ? -input.z - input.depth : input.z + input.depth};
        Vector3 first = originalFirst;
        Vector3 second = originalSecond;
        const std::uint8_t count = counts[vertex] > 0 ? static_cast<std::uint8_t>(counts[vertex]) : 0;
        for (std::uint8_t entry = 0; entry < count; ++entry, ++influence) {
            const std::uint8_t joint = indices[influence];
            if (joint >= kHandMatrixCount)
                return;
            const HandBindTransform *const bind = bindBase + static_cast<std::size_t>(joint) * kHandBindTransformsPerJoint;
            const Vector3 bindOrigin{bind[4].unused, bind[4].value.x, bind[4].value.y};
            const Vector3 localFirst{originalFirst.x - bindOrigin.x, originalFirst.y - bindOrigin.y, originalFirst.z - bindOrigin.z};
            const Vector3 localSecond{originalSecond.x - bindOrigin.x, originalSecond.y - bindOrigin.y, originalSecond.z - bindOrigin.z};
            const Vector3 transformedFirst = TransformHandPoint(matrices[joint], TransformHandBindPoint(bind, localFirst));
            const Vector3 transformedSecond = TransformHandPoint(matrices[joint], TransformHandBindPoint(bind, localSecond));
            const float weight = weights[influence];
            first.x += (transformedFirst.x - originalFirst.x) * weight;
            first.y += (transformedFirst.y - originalFirst.y) * weight;
            first.z += (transformedFirst.z - originalFirst.z) * weight;
            second.x += (transformedSecond.x - originalSecond.x) * weight;
            second.y += (transformedSecond.y - originalSecond.y) * weight;
            second.z += (transformedSecond.z - originalSecond.z) * weight;
        }
        if (flipDepth) {
            first.z = -first.z;
            second.z = -second.z;
        }
        destination[vertex] = {-first.y, first.x, first.z, -second.y + first.y, second.x - first.x, second.z - first.z};
    }
}
} // namespace



void ApplyHandRootTransform(Character *character, const HandModelWork &hand) {
    if (hand.side == 1 || hand.side == 2) {
        Matrix4x4 *transform = nullptr;
        MotionManagerWork *const manager = MOTM_GetWork(character);
        auto *row = manager ? static_cast<MotionNodeRuntime *>(manager->motion.previousNodes) : nullptr;
        const std::uint8_t nodeId = hand.side == 1 ? 12 : 18;
        if (row) {
            while (row->nodeId != nodeId && row->nodeId != 0xff)
                ++row;
            if (row->nodeId == nodeId)
                transform = row->transform;
        }
        workTransformMatrix = *(transform ? transform : g_matrixStack.top.stack);
        if (hand.primaryRoot) {
            const Mt5RuntimeNode &root = *hand.primaryRoot;
            Matrix4x4::preTranslate(root.position.x, root.position.y, root.position.z);
            Matrix4x4::SetViewMatrixFromEuler(root.rotation.y, root.rotation.x, root.rotation.z);
        }
        return;
    }
    workTransformMatrix = transformMatrix_prePass;
    const Vector3 position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    const Vector3i rotation = character ? GetEulerAnglesForEntity(character->entity.node) : Vector3i{};
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    auto *node = character ? static_cast<Mt5RuntimeNode *>(character->model) : nullptr;
    const std::uint16_t target = hand.side == 3 ? 13 : 8;
    std::byte *const base = MT5_ModelBaseForNode(node);
    while (node) {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
        if (node->nodeKind == target || !node->childOffset)
            break;
        node = reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset);
    }
}
namespace {


void HND_CaptureRootTransform(Character *character, const HandModelWork &hand, Matrix4x4 *output) {
    Matrix4x4_Push();
    ApplyHandRootTransform(character, hand);
    *(output ? output : g_matrixStack.top.stack) = workTransformMatrix;
    Matrix4x4::Pop(1);
}


std::int32_t MHND_ComputeHandScaleTable(Character *character, std::int32_t nodeKind, float *table) {
    for (std::size_t index = 0; index < 6; ++index)
        table[index] = 1.0f;
    HandPoseWork *const measured = character ? FindHandPoseWork(character) : nullptr;
    MapControlData *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    Character *const ryo = control ? CHID_Find(control->scenarioTask, SHEN_NAME(AKIR)) : nullptr;
    HandPoseWork *const reference = ryo ? FindHandPoseWork(ryo) : nullptr;
    if (!measured || !reference)
        return 0;
    const HandPoseBlendState &measuredHand = nodeKind != -65 ? measured->left : measured->right;
    const HandPoseBlendState &referenceHand = nodeKind != -65 ? reference->left : reference->right;
    for (std::size_t index = 0; index < 6; ++index) {
        if (measuredHand.scratch[index] > 0.0f && referenceHand.scratch[index] > 0.0f)
            table[index] = measuredHand.scratch[index] / referenceHand.scratch[index];
    }
    return 1;
}



void HND_ApplyScaledFingerTransform(
    const HandFingerNode *&node, const Matrix4x4 *&local, bool flipDepth, Matrix4x4 *world, const float *scaleTable, float scaleThreshold) {
    const HandFingerNode current = *node++;
    std::int32_t childCount = current.childCount;
    const Matrix4x4 parent = workTransformMatrix;
    Matrix4x4_Push(local);
    float scale = scaleTable[0];
    if (scaleThreshold > 5.0f) {
        for (const HandScaleColumn &entry : kHandScaleColumns) {
            if (entry.joint == current.jointIndex) {
                scale = scaleTable[entry.column];
                break;
            }
        }
    }
    if (scale <= 0.0f)
        scale = 1.0f;
    workTransformMatrix.rows[3].x *= scale;
    workTransformMatrix.rows[3].y *= scale;
    workTransformMatrix.rows[3].z *= scale;
    if (flipDepth)
        Matrix4x4::Scale(1.0f, 1.0f, -1.0f);
    ++local;
    Matrix4x4_Push();
    workTransformMatrix = parent;
    if (flipDepth)
        Matrix4x4::Scale(1.0f, 1.0f, -1.0f);
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
    const Matrix4x4 result = workTransformMatrix;
    Matrix4x4::Pop(2);
    if (kHandInverseMatrixNodes[current.jointIndex] >= 0)
        world[current.jointIndex] = result;
    for (; childCount > 0; --childCount)
        HND_ApplyScaledFingerTransform(node, local, flipDepth, world, scaleTable, scaleThreshold);
}


void HND_BuildWorldMatrices(Character *character, HandModelWork &hand) {
    const HandFingerNode *hierarchy = HandHierarchy(hand);
    if (!hierarchy)
        return;
    const Matrix4x4 *local = HND_FingerMatrixPool();
    const std::int32_t nodeKind = ((hand.side - 1U) & 0xfdU) != 0 ? -65 : -66;
    const bool flipDepth = ((hand.side - 1U) & 0xfdU) == 0;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundZ_Wrapper(0x4000);
    Matrix4x4::SetActiveTransformToPrePass();
    ApplyHandRootTransform(character, hand);
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
    if (!hand.scaleTableValid)
        hand.scaleTableValid = static_cast<std::uint32_t>(MHND_ComputeHandScaleTable(character, nodeKind, hand.scaleTable));
    const float *const scaleTable = hand.scaleTableValid ? hand.scaleTable : kDefaultHandScaleTable;
    HND_ApplyScaledFingerTransform(hierarchy, local, flipDepth, HND_WorldMatrixPool(), scaleTable, 0.0f);
    Matrix4x4::Pop(2);
}

// The palm-space centre of the four finger roots, shared by LoadHandModels and HND_AimPalmAtFingerCenter.
Vector3 HND_MeasureFingerCenter() {
    Matrix4x4 *const world = HND_WorldMatrixPool();
    const float x = (((world[45].rows[3].x + world[41].rows[3].x) + world[37].rows[3].x) + world[33].rows[3].x) * 0.25f;
    const float y = (((world[45].rows[3].y + world[41].rows[3].y) + world[37].rows[3].y) + world[33].rows[3].y) * 0.25f;
    const float z = (((world[45].rows[3].z + world[41].rows[3].z) + world[37].rows[3].z) + world[33].rows[3].z) * 0.25f;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(x, y, z);
    Matrix4x4_Push(world + 2);
    Matrix4x4::InvertInPlace(nullptr);
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
    const Vector3 result{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(2);
    return result;
}


void Matrix4x4_lookAt(Matrix4x4 &matrix, const Vector3 &target) {
    const float x = matrix.rows[3].x;
    const float y = matrix.rows[3].y;
    const float z = matrix.rows[3].z;
    const float rollX = matrix.rows[0].x;
    const float rollY = matrix.rows[0].y;
    const float rollZ = matrix.rows[0].z;
    const float length = std::sqrt(((target.y - y) * (target.y - y) + (target.x - x) * (target.x - x)) + (target.z - z) * (target.z - z));
    if (length == 0.0f)
        return;
    float directionY = (target.y - y) / length;
    float directionZ = (target.z - z) / length;
    float directionX = (target.x - x) / length;
    float depthX = directionZ * rollY - directionY * rollZ;
    float depthY = directionX * rollZ - directionZ * rollX;
    float depthZ = directionY * rollX - directionX * rollY;
    float sideX = depthZ * directionY - depthY * directionZ;
    float sideY = depthX * directionZ - depthZ * directionX;
    float sideZ = depthY * directionX - depthX * directionY;
    const float sideLength = std::sqrt((sideY * sideY + sideX * sideX) + sideZ * sideZ);
    if (sideLength > 0.0f) {
        sideX *= 1.0f / sideLength;
        sideY *= 1.0f / sideLength;
        sideZ *= 1.0f / sideLength;
    }
    const float directionLength = std::sqrt((directionY * directionY + directionX * directionX) + directionZ * directionZ);
    if (directionLength > 0.0f) {
        directionX *= 1.0f / directionLength;
        directionY *= 1.0f / directionLength;
        directionZ *= 1.0f / directionLength;
    }
    const float depthLength = std::sqrt((depthY * depthY + depthX * depthX) + depthZ * depthZ);
    if (depthLength > 0.0f) {
        depthX = (1.0f / depthLength) * depthX;
        depthY = (1.0f / depthLength) * depthY;
        depthZ = (1.0f / depthLength) * depthZ;
    }
    matrix.rows[0].z = sideZ;
    matrix.rows[0].x = sideX;
    matrix.rows[0].y = sideY;
    matrix.rows[1].x = directionX;
    matrix.rows[1].y = directionY;
    matrix.rows[1].z = directionZ;
    matrix.rows[2].x = depthX;
    matrix.rows[2].y = depthY;
    matrix.rows[2].z = depthZ;
}



void HND_AimPalmAtFingerCenter(const Vector3 &fingerCenter) {
    Matrix4x4 *const palm = HND_WorldMatrixPool() + 2;
    const Vector3 current = HND_MeasureFingerCenter();
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(current.x - fingerCenter.x, (current.y - fingerCenter.y) + fingerCenter.y, current.z - fingerCenter.z);
    Matrix4x4_Push(palm);
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
    const Vector3 target{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(2);
    Matrix4x4_lookAt(*palm, target);
}



void Matrix4x4_Recursive_InverseMatrixRotate(const HandFingerNode *&node, const Matrix4x4 *parent, Matrix4x4 *world, Matrix4x4 *inverse) {
    const HandFingerNode current = *node++;
    std::int32_t childCount = current.childCount;
    if (kHandInverseMatrixNodes[current.jointIndex] >= 0) {
        Matrix4x4_Push(world + current.jointIndex);
        Matrix4x4_Push(parent);
        Matrix4x4::InvertInPlace(nullptr);
        Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
        *(inverse ? inverse + current.jointIndex : g_matrixStack.top.stack) = workTransformMatrix;
        Matrix4x4::Pop(2);
        parent = world + current.jointIndex;
    }
    for (; childCount > 0; --childCount)
        Matrix4x4_Recursive_InverseMatrixRotate(node, parent, world, inverse);
}



void HND_ApplyRootRotationToInverse(const HandModelWork &hand) {
    const Vector3i rotation = hand.primaryRoot ? hand.primaryRoot->rotation : Vector3i{};
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
    Matrix4x4 *const inverse = HND_InverseMatrixPool();
    Matrix4x4_Push(inverse);
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
    *(inverse ? inverse : g_matrixStack.top.stack) = workTransformMatrix;
    Matrix4x4::Pop(2);
}



void HMDL_AppendHandAttachmentMatrices(const HandSkinningAttachmentRecord *record) {
    if (!record || !record->enabled || !record->ready || !record->node)
        return;
    const std::byte *const recordBytes = reinterpret_cast<const std::byte *>(record);
    for (std::uint32_t index = 0; index < record->matrixCount; ++index) {
        const Matrix4x4 *const source = record->matrices ? record->matrices + record->matrixIndices[index] : nullptr;
        if (!source || index > 20 || !record->ready || !record->cacheHandle)
            continue;
        std::uint32_t boneHash{};
        std::memcpy(&boneHash, recordBytes + offsetof(HandSkinningAttachmentRecord, boneHashes) + index * sizeof(std::uint32_t), sizeof(boneHash));
        if (!boneHash)
            continue;
        Matrix4x4 transposed{};
        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t column = 0; column < 4; ++column)
                (&transposed.rows[row].x)[column] = (&source->rows[column].x)[row];
        }
        AppendHashForSkinningBuffer(static_cast<std::uint64_t>(record->cacheHandle) | (static_cast<std::uint64_t>(boneHash) << 32U), transposed);
    }
}


void HND_CollectSkinnedJoints(const HandFingerNode *&cursor, HandSkinningAttachmentRecord &record) {
    const HandFingerNode node = *cursor++;
    if (kHandInverseMatrixNodes[node.jointIndex] >= 0 && record.matrixCount < std::size(record.matrixIndices))
        record.matrixIndices[record.matrixCount++] = node.jointIndex;
    for (std::int8_t children = static_cast<std::int8_t>(node.childCount); children > 0; --children)
        HND_CollectSkinnedJoints(cursor, record);
}


HandSkinningAttachmentRecord *HMDL_AcquireHandAttachmentRecord(HandSkinningAttachmentRecord *records, Mt5RuntimeNode *node, std::uint32_t &slotIndex) {
    if (!records || !node)
        return nullptr;
    for (std::uint32_t index = 0; index < 2; ++index) {
        if (records[index].node == node) {
            slotIndex = index;
            return &records[index];
        }
    }
    for (std::uint32_t index = 0; index < 2; ++index) {
        if (records[index].node)
            continue;
        std::memset(&records[index], 0, sizeof(records[index]));
        records[index].slot = index;
        records[index].node = node;
        slotIndex = index;
        return &records[index];
    }
    return nullptr;
}




bool HMDL_PublishHandSkinning(const HandModelWork &hand, Character *character, void *model) {
    static_cast<void>(SubmitRiggedSceneObjectForDrawing(character->model, character, 0, 12));
    std::byte *const entry = ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(model));
    if (!entry)
        return false;
    std::uint32_t state{};
    std::uint32_t rigInitialized{};
    std::uint32_t rigAssigned{};
    std::memcpy(&state, entry + 0x18, sizeof(state));
    std::memcpy(&rigInitialized, entry + 0x1d0, sizeof(rigInitialized));
    std::memcpy(&rigAssigned, entry + 0x1d4, sizeof(rigAssigned));
    if (state != 1 || !rigInitialized || !rigAssigned)
        return false;
    const std::int32_t nodeKind = hand.side == 1 ? -66 : hand.side == 2 ? -65 : hand.side == 3 ? 13 : hand.side == 4 ? 8 : 0;
    const HandSkinningAttachmentRecord *const records = reinterpret_cast<const HandSkinningAttachmentRecord *>(entry + 0x3f28);
    for (std::size_t index = 0; index < 2; ++index) {
        if (records[index].nodeHash && records[index].nodeKind == nodeKind)
            HMDL_AppendHandAttachmentMatrices(records + index);
    }
    return true;
}


void HND_PublishSkinning(Character *character, HandModelWork &hand) {
    if (!character)
        return;
    ImageModuleWork *const images = FindImageModuleWork(character);
    if (!images || !images->images)
        return;
    MdlmImageSlot *const slot = images->images[images->requestedImage];
    if (!slot || !slot->selectedModel)
        return;
    if (static_cast<std::uint8_t>(hand.side - 3U) > 1U)
        HND_ApplyRootRotationToInverse(hand);
    HMDL_PublishHandSkinning(hand, character, character->model);
}



void HND_TransformJointOutputs(const Matrix4x4 *frame, std::byte *positions, Matrix4x4 *jointMatrices, const HandFingerNode *hierarchy) {
    const Matrix4x4 *local = HND_FingerMatrixPool();
    for (std::size_t index = 0; index < kHandJointStateCount; ++index, ++local, ++hierarchy) {
        std::byte *const stored = positions + index * sizeof(Vector3);
        Vector3 position{};
        std::memcpy(&position, stored, sizeof(position));
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        std::memcpy(stored, &position, sizeof(position));
        workTransformMatrix = *(frame ? frame : g_matrixStack.top.stack);
        const std::uint8_t joint = hierarchy->jointIndex;
        Matrix4x4::Multiply(local);
        if (jointMatrices && kHandInverseMatrixNodes[joint] >= 0)
            jointMatrices[kHandInverseMatrixNodes[joint]] = workTransformMatrix;
        if (const std::int16_t probe = kHandProbeJointIndices[joint]; probe >= 0) {
            const HandFingerProbe &authored = kHandFingerProbes[probe];
            HandFingerProbe &placed = g_handFingerProbes[probe];
            Matrix4x4_Push();
            Matrix4x4::preTranslate(authored.start.x, authored.start.y, authored.start.z);
            placed.start = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            placed.startRadius = authored.startRadius;
            Matrix4x4::Pop(1);
            Matrix4x4::preTranslate(authored.end.x, authored.end.y, authored.end.z);
            placed.end = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            placed.endRadius = authored.endRadius;
        }
        workTransformMatrix = *(frame ? frame : g_matrixStack.top.stack);
    }
}



void HND_ConvertCapturedHandPose(const HandPoseRecord &pose, std::int32_t nodeKind, std::int32_t (&angles)[13]) {
    const bool left = nodeKind == -66;
    const auto mirror = [](std::int32_t value) { return static_cast<std::int32_t>(65536.0f - static_cast<float>(value)); };
    const auto mirrorNonZero = [&](std::int32_t value) { return static_cast<std::int32_t>(65536.0f - (value ? static_cast<float>(value) : 65536.0f)); };
    for (std::size_t index = 0; index < 10; ++index)
        angles[index] = pose.values[index];
    angles[4] = static_cast<std::uint16_t>(angles[4]);
    angles[6] = static_cast<std::uint16_t>(angles[6]);
    angles[7] = static_cast<std::uint16_t>(angles[7]);
    angles[9] = static_cast<std::uint16_t>(angles[9]);
    std::int32_t index4 = angles[4];
    std::int32_t index6 = angles[6];
    std::int32_t index7 = angles[7];
    std::int32_t index9 = angles[9];
    if (left) {
        index4 = mirrorNonZero(index4);
        index6 = mirrorNonZero(index6);
        index7 = mirrorNonZero(index7);
        index9 = mirrorNonZero(index9);
    }
    std::int32_t thumbBase = static_cast<std::int32_t>(static_cast<float>(index4) * 0.7f);
    std::int32_t thumbTip = static_cast<std::int32_t>(static_cast<float>(index6) * 0.3f);
    std::int32_t thumbMiddle = static_cast<std::int32_t>(static_cast<float>(index6) * 0.7f + static_cast<float>(index4) * 0.3f);
    std::int32_t fingerBase = static_cast<std::int32_t>(static_cast<float>(index7) * 0.7f);
    const std::int32_t fingerTip = static_cast<std::int32_t>(static_cast<float>(index9) * 0.3f);
    std::int32_t fingerMiddle = static_cast<std::int32_t>(static_cast<float>(index9) * 0.7f + static_cast<float>(index7) * 0.3f);
    if (left) {
        thumbBase = mirror(thumbBase);
        thumbMiddle = mirror(thumbMiddle);
        fingerBase = mirror(fingerBase);
        fingerMiddle = mirror(fingerMiddle);
    }
    angles[4] = thumbBase;
    angles[6] = thumbMiddle;
    angles[7] = fingerBase;
    angles[9] = fingerMiddle;
    angles[11] = thumbTip;
    const std::int32_t curl = left ? mirror(fingerBase) : fingerBase;
    const std::int32_t spread = left ? mirror(angles[8]) : angles[8];
    const float correction = std::fmin(1.0f, std::fmax(0.0f, static_cast<float>(curl) * 0.0001220703125f));
    std::int32_t littleSpread = static_cast<std::int32_t>(static_cast<float>(spread) - correction * 3094.0f);
    if (left)
        littleSpread = mirror(littleSpread);
    angles[12] = littleSpread;
    if (left) {
        angles[5] = mirror(angles[5]);
        angles[8] = mirror(angles[8]);
    } else {
        angles[0] = mirror(angles[0]);
        angles[1] = mirror(angles[1]);
        angles[2] = mirror(angles[2]);
        angles[4] = mirror(thumbBase);
        angles[6] = mirror(thumbMiddle);
        angles[11] = mirror(thumbTip);
        angles[7] = mirror(fingerBase);
        angles[9] = mirror(fingerMiddle);
    }
    angles[10] = mirror(fingerTip);
}



void HND_ApplyAuthoredJointRotations(const HandFingerNode *&node, const std::int32_t (&angles)[13], Matrix4x4 *inverse) {
    const HandFingerNode current = *node++;
    std::int32_t childCount = current.childCount;
    const std::uint8_t joint = current.jointIndex;
    if (kHandInverseMatrixNodes[joint] >= 0) {
        bool apply = true;
        switch (joint) {
        case 30:
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(angles[3]));
            break;
        case 33:
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(angles[4]));
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(angles[5]));
            break;
        case 34:
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(angles[6]));
            break;
        case 35:
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(angles[11]));
            break;
        case 37:
        case 41:
        case 45:
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(angles[7]));
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(joint == 45 ? angles[12] : angles[8]));
            break;
        case 38:
        case 42:
        case 46:
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(angles[9]));
            break;
        case 39:
        case 43:
        case 47:
            Matrix4x4::SetActiveTransformToPrePass();
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(angles[10]));
            break;
        default:
            apply = false;
            break;
        }
        if (apply) {
            Matrix4x4 *const stored = inverse + joint;
            Matrix4x4_Push(stored);
            Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
            *(stored ? stored : g_matrixStack.top.stack) = workTransformMatrix;
            Matrix4x4::Pop(2);
        }
    }
    for (; childCount > 0; --childCount)
        HND_ApplyAuthoredJointRotations(node, angles, inverse);
}



void HND_ApplyAuthoredPose(Character *character, HandModelWork &hand) {
    HND_BuildWorldMatrices(character, hand);
    const HandFingerNode *const hierarchy = HandHierarchy(hand);
    if (!hierarchy)
        return;
    const std::int32_t nodeKind = ((hand.side - 1U) & 0xfdU) != 0 ? -65 : -66;
    Mt5RuntimeNode *const model = character ? static_cast<Mt5RuntimeNode *>(character->model) : nullptr;
    Matrix4x4 root{};
    HND_CaptureRootTransform(character, hand, &root);
    if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, static_cast<std::int16_t>(nodeKind)); node && node->transformOffset) {
        if (const Matrix4x4 *const transform = HMDL_TransformForNode(node, node->transformOffset))
            root = *transform;
    }
    const HandFingerNode *cursor = hierarchy;
    Matrix4x4_Recursive_InverseMatrixRotate(cursor, &root, HND_WorldMatrixPool(), HND_InverseMatrixPool());

    HandPoseRecord pose{};
    Mt5RuntimeNode *const handNode = shenGetNodeByChildIdx(model, static_cast<std::int16_t>(nodeKind));
    if (nodeKind == -66)
        mhndCaptureLeftHandPose(handNode, pose);
    else
        mhndCaptureRightHandPose(handNode, pose);
    std::int32_t angles[13]{};
    HND_ConvertCapturedHandPose(pose, nodeKind, angles);
    cursor = hierarchy;
    HND_ApplyAuthoredJointRotations(cursor, angles, HND_InverseMatrixPool());
}

Vector3 HandGripSubtract(const Vector3 &a, const Vector3 &b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

float HandGripDot(const Vector3 &a, const Vector3 &b) {
    return (a.y * b.y + a.x * b.x) + a.z * b.z;
}



float HandGripEdgeDistanceSquared(const HandFingerProbe &probe, const Vector3 &a, const Vector3 &b, float &radius) {
    const Vector3 edge = HandGripSubtract(b, a);
    const Vector3 finger = HandGripSubtract(probe.end, probe.start);
    const Vector3 offset = HandGripSubtract(a, probe.start);
    const float fingerProjection = HandGripDot(offset, finger);
    const float edgeProjection = HandGripDot(offset, edge);
    const float directions = HandGripDot(edge, finger);
    const float fingerLength = HandGripDot(finger, finger);
    const float edgeLength = HandGripDot(edge, edge);
    const float denominator = directions * directions - edgeLength * fingerLength;
    float fingerParameter = -1.0f;
    float edgeParameter = -1.0f;
    if (denominator >= 1.0e-7f || denominator <= -1.0e-7f) {
        fingerParameter = (edgeProjection * directions - edgeLength * fingerProjection) / denominator;
        edgeParameter = (edgeProjection * fingerLength - directions * fingerProjection) / denominator;
    }
    fingerParameter = std::fmin(1.0f, std::fmax(0.0f, fingerParameter));
    edgeParameter = std::fmin(1.0f, std::fmax(0.0f, edgeParameter));
    const Vector3 delta{offset.x + (edgeParameter * edge.x - fingerParameter * finger.x), offset.y + (edgeParameter * edge.y - fingerParameter * finger.y),
        offset.z + (edgeParameter * edge.z - fingerParameter * finger.z)};
    radius = (radius - probe.startRadius) * fingerParameter + probe.startRadius;
    return HandGripDot(delta, delta);
}



void ClassifyHandGripTriangle(std::int8_t *limits, const Vector3 &a, const Vector3 &b, const Vector3 &c) {
    const Vector3 edgeB = HandGripSubtract(b, a);
    const Vector3 edgeC = HandGripSubtract(c, a);
    const Vector3 inverseB = HandGripSubtract(a, b);
    Vector3 normal{inverseB.y * edgeC.z - edgeC.y * inverseB.z, edgeC.x * inverseB.z - inverseB.x * edgeC.z, inverseB.x * edgeC.y - edgeC.x * inverseB.y};
    const float magnitude = std::sqrt(HandGripDot(normal, normal));
    normal.x /= magnitude;
    normal.y /= magnitude;
    normal.z /= magnitude;
    float determinant = edgeC.x * edgeB.y - edgeC.y * edgeB.x;
    std::int16_t plane = 0;
    if (!(static_cast<double>(determinant) >= 1.0e-6 || static_cast<double>(determinant) <= -1.0e-6)) {
        determinant = edgeC.x * edgeB.z - edgeC.z * edgeB.x;
        plane = 1;
        if (!(static_cast<double>(determinant) >= 1.0e-6 || static_cast<double>(determinant) <= -1.0e-6)) {
            determinant = edgeC.y * edgeB.z - edgeC.z * edgeB.y;
            plane = static_cast<double>(determinant) >= 1.0e-6 || static_cast<double>(determinant) <= -1.0e-6 ? 2 : -1;
        }
    }
    float divisor = edgeB.x;
    std::int16_t axis = 0;
    if (!(static_cast<double>(divisor) >= 1.0e-6 || static_cast<double>(divisor) <= -1.0e-6)) {
        divisor = edgeB.y;
        axis = 1;
        if (!(static_cast<double>(divisor) >= 1.0e-6 || static_cast<double>(divisor) <= -1.0e-6)) {
            divisor = edgeB.z;
            axis = static_cast<double>(divisor) >= 1.0e-6 || static_cast<double>(divisor) <= -1.0e-6 ? 2 : -1;
        }
    }
    for (std::size_t finger = 0; finger < 5; ++finger) {
        if (limits[finger] < 0)
            continue;
        const HandFingerProbe &probe = g_handFingerProbes[finger];
        const Vector3 offset = HandGripSubtract(a, probe.start);
        const float signedDistance = HandGripDot(normal, offset);
        float distance = std::fabs(signedDistance);
        if (!(distance > 0.04f)) {
            const Vector3 projected{normal.x * signedDistance - offset.x, normal.y * signedDistance - offset.y, normal.z * signedDistance - offset.z};
            float alongC = -1.0f;
            switch (plane) {
            case 0:
                alongC = (projected.x * edgeB.y - projected.y * edgeB.x) / determinant;
                break;
            case 1:
                alongC = (projected.x * edgeB.z - projected.z * edgeB.x) / determinant;
                break;
            case 2:
                alongC = (projected.y * edgeB.z - projected.z * edgeB.y) / determinant;
                break;
            }
            if (axis < 0) {
                distance = 0.039999999f;
            } else {
                const float coordinate = axis == 0 ? projected.x : axis == 1 ? projected.y : projected.z;
                const float edgeCoordinate = axis == 0 ? edgeC.x : axis == 1 ? edgeC.y : edgeC.z;
                const float alongB = (coordinate - alongC * edgeCoordinate) / divisor;
                if (alongB < 0.0f || alongC < 0.0f || alongB + alongC > 1.0f)
                    distance = 0.039999999f;
            }
        }
        float radius = probe.startRadius;
        if (distance <= 0.04f) {
            float squaredDistance = distance * distance;
            const Vector3 *const starts[]{&a, &b, &c};
            const Vector3 *const ends[]{&b, &c, &a};
            for (std::size_t edge = 0; edge < 3; ++edge) {
                float edgeRadius = probe.endRadius;
                const float candidate = HandGripEdgeDistanceSquared(probe, *starts[edge], *ends[edge], edgeRadius);
                if (squaredDistance > candidate) {
                    radius = edgeRadius;
                    squaredDistance = candidate;
                }
            }
            distance = std::sqrt(squaredDistance);
        }
        std::int8_t classification = 0;
        if (static_cast<double>(radius) + 0.016 > static_cast<double>(distance)) {
            classification = 1;
            if (static_cast<double>(radius) + 0.006 > static_cast<double>(distance)) {
                classification = 2;
                if (radius > distance)
                    classification = -1;
            }
        }
        if (classification < 0 || classification > limits[finger])
            limits[finger] = classification;
    }
}


void HND_ClassifyGripStrips(std::int8_t *limits, const std::int16_t *commands, const Vector3 *vertices) {
    std::int16_t stride = 3;
    while (*commands != std::numeric_limits<std::int16_t>::min()) {
        const std::int16_t command = *commands++;
        if (command >= 0) {
            switch (command) {
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 12:
            case 13:
            case 14:
            case 15:
                commands += *commands / 2 + 1;
                break;
            case 8:
            case 9:
            case 10:
            case 11:
                ++commands;
                break;
            case 16:
            case 19:
                stride = 1;
                commands += 2;
                break;
            case 17:
            case 18:
            case 20:
            case 21:
            case 24:
            case 27:
                stride = 3;
                commands += 2;
                break;
            case 25:
            case 26:
            case 28:
            case 29:
                stride = 5;
                commands += 2;
                break;
            }
        } else {
            for (std::int32_t triangle = 0; triangle < -command - 2; ++triangle) {
                ClassifyHandGripTriangle(limits, vertices[commands[0]], vertices[commands[stride]], vertices[commands[2 * stride]]);
                commands += stride;
            }
            commands += 2 * stride;
        }
    }
}



void HND_WalkGripGeometry(std::int8_t *limits, Mt5RuntimeNode *node) {
    while (node) {
        std::byte *const nodeBase = MT5_ModelBaseForNode(node);
        if (!nodeBase)
            return;
        if (node->siblingOffset)
            matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
        std::byte *bindingBase = nullptr;
        const auto *const binding = static_cast<const std::byte *>(MT5_SceneBindingForNode(node, &bindingBase));
        std::int32_t vertexCount = 0;
        if (binding && bindingBase) {
            std::memcpy(&vertexCount, binding + 8, sizeof(vertexCount));
            if (vertexCount + g_handCollisionVertexOffset < 800) {
                std::int32_t verticesOffset = 0;
                std::int32_t stripsOffset = 0;
                std::memcpy(&verticesOffset, binding + 4, sizeof(verticesOffset));
                std::memcpy(&stripsOffset, binding + 12, sizeof(stripsOffset));
                auto *const transformed = static_cast<Vector3 *>(g_handCollisionPool) + g_handCollisionVertexOffset;
                const Matrix4x4 transform = workTransformMatrix;
                for (std::int32_t vertex = 0; vertex < vertexCount; ++vertex) {
                    Vector3 point{};
                    std::memcpy(&point, bindingBase + verticesOffset + vertex * kHandVertexStride, sizeof(point));
                    Matrix4x4::preTranslate(point.x, point.y, point.z);
                    transformed[vertex] = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
                    workTransformMatrix = transform;
                }
                HND_ClassifyGripStrips(limits, reinterpret_cast<const std::int16_t *>(bindingBase + stripsOffset), transformed);
            }
            g_handCollisionVertexOffset = static_cast<std::int16_t>(g_handCollisionVertexOffset + vertexCount);
        }
        if (node->childOffset)
            HND_WalkGripGeometry(limits, reinterpret_cast<Mt5RuntimeNode *>(nodeBase + node->childOffset));
        if (binding)
            g_handCollisionVertexOffset = static_cast<std::int16_t>(g_handCollisionVertexOffset - vertexCount);
        if (!node->siblingOffset)
            break;
        Matrix4x4::Pop(1);
        node = reinterpret_cast<Mt5RuntimeNode *>(nodeBase + node->siblingOffset);
    }
}



void HND_ClassifyTargetGrip(HandModelWork &hand) {
    Character &target = *hand.gripTarget;
    for (std::size_t finger = 0; finger < 5; ++finger)
        if (hand.fingerBlendLimits[finger] > 0)
            hand.fingerBlendLimits[finger] = 0;
    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    const Vector3 position = GetPositionForEntity(target.entity.node);
    const Vector3i rotation = GetEulerAnglesForEntity(target.entity.node);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4::Scale(target.scale.x, target.scale.y, target.scale.z);
    g_handCollisionVertexOffset = 0;
    if (target.model) {
        if (g_handCollisionPool)
            HND_WalkGripGeometry(hand.fingerBlendLimits, static_cast<Mt5RuntimeNode *>(target.model));
    } else {
        const Vector3 origin{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        for (std::size_t finger = 0; finger < 5; ++finger) {
            const HandFingerProbe &probe = g_handFingerProbes[finger];
            const Vector3 delta = HandGripSubtract(origin, probe.start);
            const float radius = static_cast<float>(static_cast<double>(probe.startRadius) + 0.002);
            if (radius * radius > HandGripDot(delta, delta))
                hand.fingerBlendLimits[finger] = -1;
        }
    }
    Matrix4x4::Pop(1);
}



void HND_Callback(Character *character, void *payload) {
    auto *const hand = static_cast<HandModelWork *>(payload);
    if (!hand || !hand->handMotionModel)
        return;
    Matrix4x4 *const local = HND_FingerMatrixPool();
    if (hand->playback.status <= 0 || hand->handMotionTableIndex < 0) {
        HND_SetNewData(*hand, 0, 0, 0, 0, 1, 1.0f, 0);
        hand->playback.previousFrameIndex = hand->playback.frameIndex;
        HND_UpdateFingers(hand->playback.frameIndex, *hand, local);
        HND_DeformVertices(*hand, local);
        HND_ApplyAuthoredPose(character, *hand);
        HND_PublishSkinning(character, *hand);
        hand->control |= 8U;
        return;
    }

    hand->playback.previousFrameIndex = hand->playback.frameIndex;
    HND_UpdateFingers(hand->playback.frameIndex, *hand, local);
    HND_DeformVertices(*hand, local);
    const HandFingerNode *const hierarchy = HandHierarchy(*hand);
    const bool left = ((hand->side - 1U) & 0xfdU) == 0;
    if (hand->playback.status) {
        HND_BuildWorldMatrices(character, *hand);
        if (hierarchy) {
            HND_AimPalmAtFingerCenter(hand->fingerCenter);
            Matrix4x4 root{};
            HND_CaptureRootTransform(character, *hand, &root);
            Mt5RuntimeNode *const model = character ? static_cast<Mt5RuntimeNode *>(character->model) : nullptr;
            if (Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, left ? -66 : -65); node && node->transformOffset) {
                if (const Matrix4x4 *const transform = HMDL_TransformForNode(node, node->transformOffset))
                    root = *transform;
            }
            const HandFingerNode *cursor = hierarchy;
            Matrix4x4_Recursive_InverseMatrixRotate(cursor, &root, HND_WorldMatrixPool(), HND_InverseMatrixPool());
            HND_PublishSkinning(character, *hand);
        }
    }

    Matrix4x4_Push();
    ApplyHandRootTransform(character, *hand);
    Matrix4x4::RotateAroundZ_Wrapper(0x4000);
    if (left)
        Matrix4x4::Scale(1.0f, 1.0f, -1.0f);
    const Matrix4x4 frame = workTransformMatrix;
    if (hierarchy)
        HND_TransformJointOutputs(&frame, hand->jointPositions, static_cast<Matrix4x4 *>(hand->jointMatrices), hierarchy);
    Matrix4x4::Pop(1);
    if (hand->gripTarget && hand->animationFlags)
        HND_ClassifyTargetGrip(*hand);

    const std::uint8_t control = hand->control & 0xf7U;
    hand->control = control;
    if (!hand->animationFlags) {
        if ((control & 4U) == 0U) {
            hand->frame = (control & 2U) != 0U ? hand->frame - (hand->frameStep + hand->frameStep) : (hand->frameStep + hand->frameStep) + hand->frame;
            const std::int16_t next = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(hand->frame) + 0.5));
            hand->playback.frameIndex = next;
            const std::int16_t start = hand->playback.startFrame;
            const std::int16_t end = hand->endFrame;
            if ((control & 1U) != 0U) {
                const std::int16_t length = static_cast<std::int16_t>(end - start + 1);
                if (next < start) {
                    std::int16_t wrapped = next;
                    do
                        wrapped = static_cast<std::int16_t>(wrapped + length);
                    while (wrapped < start);
                    hand->playback.frameIndex = wrapped;
                }
                if (std::int16_t wrapped = hand->playback.frameIndex; wrapped > end) {
                    do
                        wrapped = static_cast<std::int16_t>(wrapped - length);
                    while (wrapped > end);
                    hand->playback.frameIndex = wrapped;
                }
            } else {
                std::int16_t clamped = next < start ? start : next;
                if (clamped > end)
                    clamped = end;
                hand->playback.frameIndex = clamped;
            }
            if (hand->playback.frameIndex != next)
                hand->frame = static_cast<float>(hand->playback.frameIndex);
        }
        const std::int16_t frameIndex = hand->playback.frameIndex;
        if ((control & 2U) != 0U) {
            if (frameIndex <= hand->playback.startFrame || frameIndex > hand->playback.previousFrameIndex)
                hand->control = control | 8U;
        } else if (frameIndex >= hand->endFrame || frameIndex < hand->playback.previousFrameIndex) {
            hand->control = control | 8U;
        }
        if (!hand->animationFlags)
            return;
    }

    if (const std::int16_t blendFrames = hand->blendFrames; blendFrames > 0) {
        std::int32_t steps = blendFrames / 2;
        if (steps < 1)
            steps = 1;
        std::int32_t settled = 1;
        hand->blendFrames = static_cast<std::int16_t>(blendFrames - 2);
        if (hand->blendFrames >= 1)
            settled = 0;
        else
            hand->blendFrames = 1;
        std::int32_t complete = settled;
        for (std::uint8_t joint = 0; joint < kHandJointStateCount; ++joint) {
            const std::array<std::int32_t, 3> target = LoadHandAngles(hand->targetAngleDeltas, joint);
            std::array<std::int32_t, 3> current = LoadHandAngles(hand->angleDeltas, joint);
            std::array<std::int32_t, 3> step{};
            for (std::size_t axis = 0; axis < step.size(); ++axis)
                step[axis] = static_cast<std::int16_t>(static_cast<std::uint32_t>(target[axis]) - static_cast<std::uint32_t>(current[axis])) / steps;
            if (hand->gripTarget && kHandFingerBlendGroups[joint] >= 0) {
                const std::int8_t grip = hand->fingerBlendLimits[kHandFingerBlendGroups[joint]];
                if (grip >= 0) {
                    const std::int32_t maximum = grip == 1 ? 182 : grip == 2 ? 72 : 455;
                    for (std::int32_t &value : step)
                        value = value >= 0 ? (value > maximum ? maximum : value) : (value < -maximum ? -maximum : value);
                } else {
                    step = {};
                }
            }
            StoreHandAngles(hand->angleDeltas, joint, current[0] + step[0], current[1] + step[1], current[2] + step[2]);
            if (step[0] || step[1] || step[2]) {
                complete = 0;
                settled = 0;
            } else {
                complete = settled;
            }
        }
        if (complete) {
            hand->animationFlags &= 9U;
            hand->blendFrames = 0;
        }
    }
    if (hand->animationFlags && hand->blendFrames < 1)
        hand->control |= 8U;
}

void CopyHandFilename(char (&destination)[0x10], const char *source) {
    std::memset(destination, 0, sizeof(destination));
    if (!source)
        return;
    const std::size_t length = std::min(std::strlen(source), sizeof(destination) - 1);
    std::memcpy(destination, source, length);
}
char UpperHandFilenameChar(char value) {
    return value >= 'a' && value <= 'z' ? static_cast<char>(value - ('a' - 'A')) : value;
}

bool HandFilenamesEqual(const char *left, const char *right) {
    if (!left || !right)
        return false;
    while (*left && *right) {
        if (UpperHandFilenameChar(*left++) != UpperHandFilenameChar(*right++)) {
            return false;
        }
    }
    return *left == *right;
}

bool PrepareMutableHandVertices(HandModelWork &hand) {
    Mt5RuntimeNode *const root = hand.primaryRoot;
    std::byte *const modelBase = root ? MT5_ModelBaseForNode(root) : nullptr;
    if (!modelBase || !root->sceneBindingOffset)
        return false;
    std::byte *bindingBase = nullptr;
    auto *const binding = static_cast<std::byte *>(MT5_SceneBindingForNode(root, &bindingBase));
    if (!binding || !bindingBase)
        return false;
    std::int32_t sourceOffset{};
    std::int32_t vertexCount{};
    std::memcpy(&sourceOffset, binding + 4, sizeof(sourceOffset));
    std::memcpy(&vertexCount, binding + 8, sizeof(vertexCount));
    if (!sourceOffset || vertexCount <= 0 || vertexCount > 0x7fff)
        return false;

    const std::size_t vertexBytes = static_cast<std::size_t>(vertexCount) * kHandVertexStride;
    void *const mutableVertices = HLib_AllocHeapMemBlock(static_cast<std::int64_t>(vertexBytes), SHEN_NAME(HNDV));
    if (!mutableVertices)
        return false;
    void *const sourceVertices = bindingBase + sourceOffset;
    std::memcpy(mutableVertices, sourceVertices, vertexBytes);
    hand.sourceVertices = sourceVertices;
    hand.mutableVertices = mutableVertices;
    hand.vertexCount = static_cast<std::int16_t>(vertexCount);
    return true;
}

std::int32_t LoadHandModel(Character *character, HandModelWork &hand) {
    std::int32_t result = 2;
    if (!hand.primaryRoot && !hand.secondaryRoot) {
        hand.vertexCount = 0;
        hand.sourceVertices = nullptr;
        hand.mutableVertices = nullptr;
        if (hand.side > 2) {
            hand.primaryRoot = static_cast<Mt5RuntimeNode *>(character->model);
            const std::int16_t rootKind = hand.side == 3 ? 13 : 8;
            std::byte *const modelBase = MT5_ModelBaseForNode(hand.primaryRoot);
            while (hand.primaryRoot && hand.primaryRoot->nodeKind != rootKind) {
                hand.primaryRoot =
                    modelBase && hand.primaryRoot->childOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + hand.primaryRoot->childOffset) : nullptr;
            }
        } else {
            PackTaskData *const pack = g_packTaskData ? g_packTaskData : g_packTask ? reinterpret_cast<PackTaskData *>(g_packTask->task_data_ptr) : nullptr;
            PackSlot *const ready = pack && pack->usePackCache ? pack->readyPack : nullptr;
            char directory[64];
            std::snprintf(directory, sizeof(directory), "scene/%02d/model/hand", g_sceneId);
            hand.primaryOwner = PACK_FindCharacterModel(ready, hand.filename);
            if (!hand.primaryOwner) {
                hand.primaryOwner = MT5_LoadPackModel(directory, hand.filename);
                if (hand.primaryOwner)
                    hand.primaryFile = hand.primaryOwner->pakf;
            }
            if (hand.primaryOwner) {
                hand.primaryRoot = static_cast<Mt5RuntimeNode *>(hand.primaryOwner->model);
            }
            if (hand.secondaryModelRequested) {
                char alternate[sizeof(hand.filename)];
                CopyHandFilename(alternate, hand.filename);
                alternate[4] = 'X';
                hand.secondaryOwner = PACK_FindCharacterModel(ready, alternate);
                if (!hand.secondaryOwner) {
                    hand.secondaryOwner = MT5_LoadPackModel(directory, alternate);
                    if (hand.secondaryOwner)
                        hand.secondaryFile = hand.secondaryOwner->pakf;
                }
                if (hand.secondaryOwner) {
                    hand.secondaryRoot = static_cast<Mt5RuntimeNode *>(hand.secondaryOwner->model);
                }
            }
        }
        if (hand.primaryRoot && PrepareMutableHandVertices(hand))
            result = 1;
    }
    if (hand.vertexCount > 0 && hand.sourceVertices && hand.mutableVertices) {
        std::memcpy(hand.mutableVertices, hand.sourceVertices, static_cast<std::size_t>(hand.vertexCount) * kHandVertexStride);
    }
    if (hand.primaryRoot || hand.secondaryRoot) {
        hand.playback.status = 0;
        hand.playback.playbackMode = 1;
        HND_SetNewData(hand, 0, 0, 0, 0, 1, 1.0f, 0);
    }
    if (hand.primaryRoot)
        hand.primaryRoot->rotation = {};
    hand.playback.previousFrameIndex = hand.playback.frameIndex;
    if (hand.handMotionModel) {
        Matrix4x4 *const local = HND_FingerMatrixPool();
        HND_UpdateFingers(hand.playback.frameIndex, hand, local);
        HND_DeformVertices(hand, local);
        HND_BuildWorldMatrices(character, hand);
        hand.fingerCenter = HND_MeasureFingerCenter();
    }
    return result;
}

void RestoreHandSourceVertices(HandModelWork &hand) {
    if (!hand.mutableVertices || !hand.sourceVertices)
        return;
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(hand.mutableVertices), 0);
    std::byte *modelBase = nullptr;
    void *const binding = MT5_SceneBindingForNode(hand.primaryRoot, &modelBase);
    if (binding && modelBase) {
        const std::ptrdiff_t sourceOffset = static_cast<std::byte *>(hand.sourceVertices) - modelBase;
        if (sourceOffset >= INT32_MIN && sourceOffset <= INT32_MAX) {
            const auto storedOffset = static_cast<std::int32_t>(sourceOffset);
            std::memcpy(static_cast<std::byte *>(binding) + 4, &storedOffset, sizeof(storedOffset));
        }
    }
    hand.sourceVertices = nullptr;
    hand.mutableVertices = nullptr;
}

void HND_Cleanup(Character *, void *payload) {
    auto *const hand = static_cast<HandModelWork *>(payload);
    if (hand->primaryRoot)
        RestoreHandSourceVertices(*hand);
    if (hand->side == 1 || hand->side == 2) {
        if (hand->primaryFile && hand->primaryOwner)
            FreePVR(hand->primaryOwner);
        if (hand->secondaryFile && hand->secondaryOwner)
            FreePVR(hand->secondaryOwner);
    }
    hand->primaryRoot = nullptr;
    hand->primaryFile = nullptr;
    hand->primaryOwner = nullptr;
    hand->secondaryRoot = nullptr;
    hand->secondaryFile = nullptr;
    hand->secondaryOwner = nullptr;
    if (hand->jointMatrices) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(hand->jointMatrices), 0);
        hand->jointMatrices = nullptr;
    }
    HND_ReleaseMotionModel(hand->handMotionTableIndex);
}

HandModelWork *FindHandModelWork(Character *character, bool left) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, left ? kLeftHandModelComponent : kRightHandModelComponent);
    return component ? reinterpret_cast<HandModelWork *>(component + 1) : nullptr;
}

void HND_FigurePose(Character *character) {
    if (!character || !character->model)
        return;
    HandModelWork *hand = FindHandModelWork(character, false);
    if (!hand)
        hand = FindHandModelWork(character, true);
    if (hand && hand->previousFigurePoseCallback) {
        reinterpret_cast<void (*)(Character *)>(hand->previousFigurePoseCallback)(character);
    }
}


void HND_ProcessShadow(Character *character) {
    if (!character || !g_updateRenderFrameMode)
        return;
    HandModelWork *const hand = FindHandModelWork(character, false);
    if (hand && hand->primaryRoot && hand->playback.status > 0)
        EvaluateShadowSourcesForEntity(character);
}

void HND_BodyFigurePose(Character *character) {
    if (!character)
        return;
    HandModelWork *hand = FindHandModelWork(character, true);
    if (!hand)
        hand = FindHandModelWork(character, false);
    if (!hand)
        return;
    if (hand->primaryRoot) {
        std::byte *modelBase = nullptr;
        auto *const binding = static_cast<std::byte *>(MT5_SceneBindingForNode(hand->primaryRoot, &modelBase));
        void *const vertices = hand->playback.status ? hand->mutableVertices : hand->sourceVertices;
        if (binding && modelBase) {
            const std::ptrdiff_t offset = vertices ? static_cast<std::byte *>(vertices) - modelBase : 0;
            if (offset >= INT32_MIN && offset <= INT32_MAX) {
                const auto storedOffset = static_cast<std::int32_t>(offset);
                std::memcpy(binding + 4, &storedOffset, sizeof(storedOffset));
            }
        }
    }
    if (hand->previousFigurePoseCallback) {
        reinterpret_cast<void (*)(Character *)>(hand->previousFigurePoseCallback)(character);
    }
}

HandModelWork *AllocateHandModelWork(Character *character, bool leftComponent, const char *filename, std::uint8_t optionFlags, std::uint8_t side,
    std::uint8_t motionKind, bool calibrateToCharacter, void *previousFigurePoseCallback) {
    if (!filename || !filename[0])
        return nullptr;
    auto *const hand = static_cast<HandModelWork *>(
        AllocateHeapMemoryMODL(character, leftComponent ? kLeftHandModelComponent : kRightHandModelComponent, sizeof(HandModelWork), HND_Callback));
    if (character) {
        HLib_GetCharacterComponent(character, leftComponent ? kLeftHandModelComponent : kRightHandModelComponent)->destroyCallback = HND_Cleanup;
    }
    if (!hand)
        return nullptr;
    std::memset(hand, 0, sizeof(*hand));
    CopyHandFilename(hand->filename, filename);
    hand->side = side;
    hand->handMotionTableIndex = HND_AcquireMotionModel(hand->filename, motionKind, &hand->handMotionModel, nullptr);
    hand->previousFigurePoseCallback = previousFigurePoseCallback;
    if ((optionFlags & 1U) != 0U) {
        hand->jointMatrices = HLib_AllocHeapMemBlock(1024, SHEN_NAME(HNDX));
    }
    if (side <= 2 && (optionFlags & 2U) != 0U)
        hand->secondaryModelRequested = 1;
    hand->modelSelector = 256;
    hand->control = 8;
    if (!hand->handMotionModel)
        hand->handMotionTableIndex = -1;
    hand->scaleTableValid = calibrateToCharacter ? 0 : 1;
    std::fill(std::begin(hand->scaleTable), std::end(hand->scaleTable), 1.0f);
    return hand;
}
} // namespace




void HMDL_CreateHandAttachmentRecords(
    void *nodePointer, Character *character, HandSkinningAttachmentRecord *records, std::uint32_t &recordType, std::uint32_t &slotIndex, std::uint32_t flags) {
    Mt5RuntimeNode *const node = static_cast<Mt5RuntimeNode *>(nodePointer);
    const std::uint16_t kind = static_cast<std::uint16_t>(node->nodeKind);
    if (static_cast<std::uint16_t>(kind + 0x42U) > 1U && kind != 13U && kind != 8U)
        return;
    if (!node->childOffset && (flags & 8U) == 0U)
        return;
    HandModelWork *const hand = character ? HND_GetWork(character, kind == 0xffbeU || kind == 13U) : nullptr;
    if (!hand)
        return;
    HandSkinningAttachmentRecord *const record = HMDL_AcquireHandAttachmentRecord(records, node, slotIndex);
    if (!record)
        return;
    const HandFingerNode *cursor = HandHierarchy(*hand);
    record->matrices = HND_InverseMatrixPool();
    record->nodeKind = static_cast<std::int16_t>(kind);
    record->matrixCount = 0;
    if (cursor)
        HND_CollectSkinnedJoints(cursor, *record);
    record->enabled = 1;
    recordType = 3;
}
namespace {

static_assert(sizeof(HandMotionEntry) == 0x18);
static_assert(offsetof(HandMotionEntry, motion) == 8);

#define HAND_MOTION_ENTRY(nameValue)                                                                                                                           \
    HandMotionEntry {                                                                                                                                          \
        nameValue, 0, nullptr, 0, 0, 0                                                                                                                         \
    }
HandMotionEntry g_handMotionTable[65]{HAND_MOTION_ENTRY("YKA"), HAND_MOTION_ENTRY("YKB"), HAND_MOTION_ENTRY("YKD"), HAND_MOTION_ENTRY("HGY"),
    HAND_MOTION_ENTRY("SYP"), HAND_MOTION_ENTRY("SYE"), HAND_MOTION_ENTRY("REN"), HAND_MOTION_ENTRY("TOG"), HAND_MOTION_ENTRY("KW1"), HAND_MOTION_ENTRY("TOJ"),
    HAND_MOTION_ENTRY("SIN"), HAND_MOTION_ENTRY("HGR"), HAND_MOTION_ENTRY("CHA"), HAND_MOTION_ENTRY("HGC"), HAND_MOTION_ENTRY("IWA"), HAND_MOTION_ENTRY("IWO"),
    HAND_MOTION_ENTRY("KOK"), HAND_MOTION_ENTRY("SYG"), HAND_MOTION_ENTRY("CHJ"), HAND_MOTION_ENTRY("WHO"), HAND_MOTION_ENTRY("FUK"), HAND_MOTION_ENTRY("FUB"),
    HAND_MOTION_ENTRY("FUC"), HAND_MOTION_ENTRY("FUD"), HAND_MOTION_ENTRY("SFF"), HAND_MOTION_ENTRY("EIN"), HAND_MOTION_ENTRY("KOT"), HAND_MOTION_ENTRY("INE"),
    HAND_MOTION_ENTRY("JKB"), HAND_MOTION_ENTRY("SAN"), HAND_MOTION_ENTRY("KEK"), HAND_MOTION_ENTRY("KIS"), HAND_MOTION_ENTRY("SYU"), HAND_MOTION_ENTRY("JIM"),
    HAND_MOTION_ENTRY("BTN"), HAND_MOTION_ENTRY("ASD"), HAND_MOTION_ENTRY("AMG"), HAND_MOTION_ENTRY("YMG"), HAND_MOTION_ENTRY("KAS"), HAND_MOTION_ENTRY("KHI"),
    HAND_MOTION_ENTRY("YWT"), HAND_MOTION_ENTRY("ACS"), HAND_MOTION_ENTRY("KOG"), HAND_MOTION_ENTRY("JOJ"), HAND_MOTION_ENTRY("FRO"), HAND_MOTION_ENTRY("YAA"),
    HAND_MOTION_ENTRY("GIC"), HAND_MOTION_ENTRY("BIK"), HAND_MOTION_ENTRY("GIJ"), HAND_MOTION_ENTRY("GIB"), HAND_MOTION_ENTRY("KWD"), HAND_MOTION_ENTRY("X70"),
    HAND_MOTION_ENTRY("BIS"), HAND_MOTION_ENTRY("NZM"), HAND_MOTION_ENTRY("KMA"), HAND_MOTION_ENTRY("RFD"), HAND_MOTION_ENTRY("TPL"), HAND_MOTION_ENTRY("KMB"),
    HAND_MOTION_ENTRY("YAB"), HAND_MOTION_ENTRY("TAT"), HAND_MOTION_ENTRY("GID"), HAND_MOTION_ENTRY("MIA"), HAND_MOTION_ENTRY("MIB"), HAND_MOTION_ENTRY("GIE"),
    HAND_MOTION_ENTRY("")};
#undef HAND_MOTION_ENTRY

std::array<Matrix4x4, kHandMatrixCount> g_fallbackHandLocalMatrices{};
std::array<Matrix4x4, kHandMatrixCount> g_fallbackHandWorldMatrices{};
std::array<Matrix4x4, kHandMatrixCount> g_fallbackHandInverseMatrices{};
Matrix4x4 *g_handFingerMatrices = g_fallbackHandLocalMatrices.data();
Matrix4x4 *g_handWorldMatrices = g_fallbackHandWorldMatrices.data();
Matrix4x4 *g_handInverseMatrices = g_fallbackHandInverseMatrices.data();
} // namespace
void HND_SetFloatsFor(HandModelWork &hand, const std::int32_t *angles, std::int16_t blendFrames) {
    static constexpr std::array<std::uint8_t, 19> kJointRemap{
        31,
        30,
        29,
        35,
        34,
        33,
        32,
        39,
        38,
        37,
        36,
        43,
        42,
        41,
        40,
        47,
        46,
        45,
        44,
    };

    hand.animationFlags = 1;
    std::memset(hand.targetAngleDeltas, 0, sizeof(hand.targetAngleDeltas));
    for (std::size_t index = 0; index < kJointRemap.size(); ++index) {
        StoreHandAngles(hand.targetAngleDeltas, kJointRemap[index], angles[index * 3], angles[index * 3 + 1], angles[index * 3 + 2]);
    }
    hand.blendFrames = std::max<std::int16_t>(blendFrames, 1);
}

void HND_SetEffectFlags(HandModelWork &hand, const std::int8_t *pose, float blendFrames) {
    std::array<std::int32_t, 19 * 3> angles{};
    for (std::size_t index = 0; index < angles.size(); ++index) {
        const int32_t converted = static_cast<int32_t>(static_cast<float>(static_cast<int32_t>(pose[index]) * 8192) / 45.0f);
        angles[index] = static_cast<uint16_t>(converted);
    }
    const auto frames = static_cast<int16_t>(static_cast<int32_t>(blendFrames));
    HND_SetFloatsFor(hand, angles.data(), frames);
}



void HND_SetNewData(HandModelWork &hand, std::int16_t sequenceId, std::int16_t startFrame, std::int16_t endFrame, std::int16_t initialFrame,
    std::uint8_t control, float frameStep, std::int16_t blendFrames) {
    const std::int16_t first = static_cast<std::int16_t>(startFrame - 1);
    const std::int16_t last = static_cast<std::int16_t>(endFrame - 1);
    const std::int16_t initial = static_cast<std::int16_t>(initialFrame - 1);
    hand.control = control & 0xf7U;
    HandMotionSequence sequence{};
    if (HND_FindMotionSequence(sequenceId, sequence)) {
        hand.activeMotionRecord = const_cast<std::byte *>(sequence.record);
        hand.sequenceDuration = sequence.frameCount;
        hand.jointCount = static_cast<std::int16_t>(sequence.jointCount);
        hand.animationCurves = const_cast<void *>(sequence.curves);
    } else {
        hand.activeMotionRecord = nullptr;
        hand.sequenceDuration = 0;
        hand.jointCount = 0;
        hand.animationCurves = nullptr;
    }

    const std::int16_t lastSequenceFrame = static_cast<std::int16_t>(hand.sequenceDuration - 1U);
    if (first >= 0) {
        hand.playback.startFrame = first;
        if (static_cast<std::uint32_t>(first) >= hand.sequenceDuration)
            hand.playback.startFrame = lastSequenceFrame;
    } else {
        hand.playback.startFrame = 0;
    }
    hand.endFrame = last;
    if (static_cast<std::uint32_t>(static_cast<std::int32_t>(last)) >= hand.sequenceDuration || last < 0)
        hand.endFrame = lastSequenceFrame;
    if (hand.playback.startFrame > hand.endFrame) {
        hand.playback.startFrame = 0;
        hand.endFrame = 0;
    }
    hand.playback.frameIndex = initial;
    if (initial < hand.playback.startFrame || initial > hand.endFrame)
        hand.playback.frameIndex = (hand.control & 2U) != 0U ? hand.endFrame : hand.playback.startFrame;
    hand.playback.previousFrameIndex = hand.playback.frameIndex;
    hand.frame = static_cast<float>(hand.playback.frameIndex);
    if (frameStep > 0.0f)
        hand.frameStep = frameStep;
    hand.animationFlags = 0;
    hand.blendFrames = blendFrames;
    if (blendFrames > 0) {
        hand.animationFlags = 2;
        const HandFingerNode *const hierarchy = HandHierarchy(hand);
        const HandBindTransform *const bind = HandBindTransforms(hand);
        if (hierarchy && bind) {
            g_handAnimFrameIndex = hand.playback.frameIndex;
            g_fingerNodeCursor = hierarchy;
            g_fingerBindCursor = bind;
            g_handAnimKeys = static_cast<const HandFingerCurveSet *>(hand.animationCurves);
            g_fingerAngleDeltas = hand.targetAngleDeltas;
            hmComputeAngleDeltasForChain();
        }
    }
    hand.gripTarget = nullptr;
}

void HND_ResetMotion(HandModelWork &hand, std::int8_t status, std::int16_t sequenceId) {
    if ((hand.primaryRoot && status >= 0) || (hand.secondaryRoot && status <= 0)) {
        hand.playback.status = status;
        hand.playback.playbackMode = 1;
        HND_SetNewData(hand, sequenceId, 0, 0, 0, 1, 1.0f, 0);
    }
}

std::uint8_t HND_ReleaseModel(HandModelWork &hand) {
    if (hand.primaryRoot || hand.secondaryRoot) {
        hand.playback.status = 0;
        hand.playback.playbackMode = 1;
        HND_SetNewData(hand, 0, 0, 0, 0, 1, 1.0f, 0);
    }
    if (hand.primaryRoot)
        hand.primaryRoot->rotation = {};
    if (std::strncmp(hand.filename, "YKB", 3) == 0)
        return 1;
    std::uint8_t result = 2;
    if (hand.primaryRoot) {
        result = 1;
        RestoreHandSourceVertices(hand);
        if ((hand.side == 1 || hand.side == 2) && hand.primaryFile && hand.primaryOwner)
            FreePVR(hand.primaryOwner);
    }
    hand.primaryRoot = nullptr;
    hand.primaryOwner = nullptr;
    hand.primaryFile = nullptr;
    if (hand.secondaryRoot && hand.secondaryFile && hand.secondaryOwner)
        FreePVR(hand.secondaryOwner);
    hand.secondaryRoot = nullptr;
    hand.secondaryOwner = nullptr;
    hand.secondaryFile = nullptr;
    return result;
}

std::uint8_t HND_ReleaseModels(Character *character) {
    if (!character)
        return 0;
    std::uint8_t result = 0;
    if (HandModelWork *const left = FindHandModelWork(character, true))
        result = HND_ReleaseModel(*left);
    if (HandModelWork *const right = FindHandModelWork(character, false))
        return HND_ReleaseModel(*right);
    return result;
}

void HND_ResetVfActorHands(Character *character) {
    if (!character)
        return;
    if (HandModelWork *const right = FindHandModelWork(character, false); right && (right->activeMotionRecord || right->handMotionModel)) {
        right->playback.status = 0;
        right->playback.playbackMode = 1;
        HND_SetNewData(*right, 0, 0, 0, 0, 1, 1.0f, 0);
    }
    if (HandModelWork *const left = FindHandModelWork(character, true); left && (left->primaryRoot || left->secondaryRoot)) {
        left->playback.status = 0;
        left->playback.playbackMode = 1;
        HND_SetNewData(*left, 0, 0, 0, 0, 1, 1.0f, 0);
    }
}
void CHAR_SetHandFaceDetails(std::int32_t mode) {
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    Character *const character = CHID_Find(control->scenarioTask, SHEN_NAME(AKIR));
    if (!character)
        return;

    const std::int8_t handStatus = mode == 1 ? -1 : 0;
    for (const bool left : {false, true}) {
        if (HandModelWork *const hand = FindHandModelWork(character, left)) {
            HND_ResetMotion(*hand, handStatus, 0);
            if (hand->primaryRoot)
                hand->primaryRoot->rotation = {};
        }
    }

    if (!MOTM_GetWork(character))
        return;
    if (FACE_GetWork(character))
        FACE_UpdateExpression(character, mode, 0, 2);
}

void HND_SetControlFlags(HandModelWork &hand, std::uint16_t enabled, std::uint8_t mask) {
    const bool wasReversed = (hand.control & 2U) != 0;
    if (enabled)
        hand.control = static_cast<std::uint8_t>(hand.control | mask);
    else
        hand.control = static_cast<std::uint8_t>(hand.control & ~mask);

    const bool reversed = (hand.control & 2U) != 0;
    bool resetFrame = false;
    if (reversed && !wasReversed && hand.playback.frameIndex <= hand.playback.startFrame) {
        hand.playback.frameIndex = hand.endFrame;
        resetFrame = true;
    } else if (!reversed && wasReversed && hand.playback.frameIndex >= hand.endFrame) {
        hand.playback.frameIndex = hand.playback.startFrame;
        resetFrame = true;
    }
    if (resetFrame) {
        hand.playback.previousFrameIndex = hand.playback.frameIndex;
        hand.frame = static_cast<float>(hand.playback.frameIndex);
    }
}


Vector3i HND_GetRootEuler(const HandModelWork &hand) {
    const Mt5RuntimeNode *const root = hand.primaryRoot;
    if (!root)
        return {};

    Vector3i result = root->rotation;
    if (!root->parentOffset || !g_pCurrentTaskData)
        return result;
    const auto *const parent = reinterpret_cast<const Mt5RuntimeNode *>(g_pCurrentTaskData + root->parentOffset);
    PushCharacterMatrix();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(parent->rotation.y, parent->rotation.x, parent->rotation.z);
    Matrix4x4::SetViewMatrixFromEuler(result.y, result.x, result.z);
    Matrix4x4_ExtractEulerAngles(result);
    PopCharacterMatrix();
    return result;
}



void HND_ApplyRootEuler(HandModelWork &hand, Vector3i value) {
    Mt5RuntimeNode *const root = hand.primaryRoot;
    if (!root)
        return;
    value = {static_cast<std::int16_t>(value.x), static_cast<std::int16_t>(value.y), static_cast<std::int16_t>(value.z)};
    if (root->parentOffset) {
        auto *const parent = reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + root->parentOffset);
        PushCharacterMatrix();
        workTransformMatrix = transformMatrix_prePass;
        parent->rotation.x = value.x / 2;
        Matrix4x4::SetViewMatrixFromEuler(parent->rotation.y, parent->rotation.x, parent->rotation.z);
        Matrix4x4::InvertInPlace(nullptr);
        Matrix4x4::SetViewMatrixFromEuler(value.y, value.x, value.z);
        Matrix4x4_ExtractEulerAngles(value);
        PopCharacterMatrix();
    }
    root->rotation = value;
}

void HND_SetRootRotation(HandModelWork &hand, std::uint32_t mask, const Vector3i *rotation) {
    if (!hand.primaryRoot || !rotation)
        return;
    Vector3i value = hand.primaryRoot->rotation;
    if (mask & 0x01U)
        value.x = rotation->x;
    if (mask & 0x04U)
        value.y = rotation->y;
    if (mask & 0x10U)
        value.z = rotation->z;
    if (mask & 0x02U)
        value.x += rotation->x;
    if (mask & 0x08U)
        value.y += rotation->y;
    if (mask & 0x20U)
        value.z += rotation->z;
    hand.primaryRoot->rotation = value;
}

std::int32_t HND_AcquireMotionModel(const char *filename, std::uint8_t motionKind, void **data, std::size_t *size) {
    if (data)
        *data = nullptr;
    if (size)
        *size = 0;
    PackTaskData *pack = g_packTaskData;
    if (!pack && g_packTask)
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    PackSlot *activeReader = nullptr;
    if (pack && pack->usePackCache) {
        pack = g_packTaskData;
        if (!pack && g_packTask)
            pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
        activeReader = pack ? pack->activeReader : nullptr;
    }
    if (!filename || std::strlen(filename) < 10 || motionKind > 3)
        return -1;

    static constexpr const char *kSuffixes[] = {"_TL.MT5", "_TR.MT5", "_UL.MT5", "_UR.MT5"};
    if (std::strncmp(filename + 3, kSuffixes[motionKind], 7) != 0)
        return -1;

    std::int32_t tableIndex = -1;
    for (std::int32_t index = 0; g_handMotionTable[index].name[0]; ++index) {
        if (std::strncmp(filename, g_handMotionTable[index].name, 3) == 0) {
            tableIndex = index;
            break;
        }
    }
    if (tableIndex < 0)
        return -1;

    HandMotionEntry &entry = g_handMotionTable[tableIndex];
    if (!entry.motion) {
        const bool sharedMotion = std::strncmp(entry.name, "YKB", 3) == 0;
        if (sharedMotion) {
            entry.motion = g_hndb;
            entry.retained = 1;
        } else {
            char motionFilename[16]{};
            std::snprintf(motionFilename, sizeof(motionFilename), "%.3s_HM.BIN", entry.name);
            if (activeReader) {
                entry.motion = LoadFileIntoHeapMem(activeReader, motionFilename, PACK_FILE_BIN.name);
                if (entry.motion) {
                    entry.retained = 1;
                    entry.motionSize = PACK_FindBinFileSize(activeReader, motionFilename);
                }
            }
            if (!entry.motion) {
                char directory[64]{};
                std::snprintf(directory, sizeof(directory), "scene/%02d/model/hand", g_sceneId);
                entry.motion = ReadFile(directory, motionFilename, RESOURCE_HNDB.name);
            }
        }
        if (!entry.motion)
            return -2;
        if (sharedMotion) {
            entry.motionSize = g_hndbSize;
        } else if (!entry.retained) {
            const auto *const allocation = reinterpret_cast<const HLib::TaskData *>(entry.motion) - 1;
            if (allocation->content_size > 0 && allocation->content_size <= UINT32_MAX) {
                entry.motionSize = static_cast<std::uint32_t>(allocation->content_size);
            }
        }
        if (entry.motionSize < sizeof(HandMotionModelHeader)) {
            if (!entry.retained) {
                HLib_FreeTaskData(static_cast<HLib::TaskData *>(entry.motion), 0);
            }
            entry.motion = nullptr;
            entry.motionSize = 0;
            entry.retained = 0;
            return -2;
        }
    }

    ++entry.references;
    if (data)
        *data = entry.motion;
    if (size)
        *size = entry.motionSize;
    return tableIndex;
}

std::size_t HND_MotionModelSize(std::int32_t tableIndex) {
    return tableIndex >= 0 && tableIndex < 64 ? g_handMotionTable[tableIndex].motionSize : 0;
}

void HND_ReleaseMotionModel(std::int32_t tableIndex) {
    if (tableIndex < 0 || tableIndex >= 64)
        return;
    HandMotionEntry &entry = g_handMotionTable[tableIndex];
    if (entry.references)
        --entry.references;
    if (entry.references)
        return;
    if (entry.motion && !entry.retained) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(entry.motion), 0);
    }
    entry.motion = nullptr;
    entry.motionSize = 0;
    entry.retained = 0;
}

Matrix4x4 *HND_FingerMatrixPool() {
    return g_handFingerMatrices;
}

Matrix4x4 *HND_WorldMatrixPool() {
    return g_handWorldMatrices;
}

Matrix4x4 *HND_InverseMatrixPool() {
    return g_handInverseMatrices;
}

void HND_ResetMotionModels() {

    for (HandMotionEntry *entry = g_handMotionTable; entry->name[0]; ++entry) {
        entry->motion = nullptr;
        entry->motionSize = 0;
        entry->retained = 0;
        entry->references = 0;
    }
}

void HND_SetFingerMatrixPool(Matrix4x4 *matrices) {
    g_handFingerMatrices = matrices ? matrices : g_fallbackHandLocalMatrices.data();
}

void HND_SetMatrixPools(Matrix4x4 *localMatrices, Matrix4x4 *worldMatrices, Matrix4x4 *inverseMatrices) {
    g_handFingerMatrices = localMatrices ? localMatrices : g_fallbackHandLocalMatrices.data();
    g_handWorldMatrices = worldMatrices ? worldMatrices : g_fallbackHandWorldMatrices.data();
    g_handInverseMatrices = inverseMatrices ? inverseMatrices : g_fallbackHandInverseMatrices.data();
}

HandModelWork *HND_GetWork(Character *character, bool left) {
    return FindHandModelWork(character, left);
}

void AllocateAndLoadArms(Character *character, const char *leftFilename, const char *rightFilename, std::uint8_t optionFlags, bool calibrateToCharacter) {
    if (!HND_FingerMatrixPool() || !HND_InverseMatrixPool())
        return;
    char defaultLeft[0x10]{};
    char defaultRight[0x10]{};
    if (!leftFilename && !rightFilename) {
        const std::uint32_t imageName = IMGM_GetImageName(character);
        if (!imageName)
            return;
        const HandMotionEntry *entry = g_handMotionTable;
        while (entry->name[0] && std::memcmp(&imageName, entry->name, 3) != 0) {
            ++entry;
        }
        if (!entry->name[0])
            return;
        std::snprintf(defaultLeft, sizeof(defaultLeft), "%s_TL.MT5", entry->name);
        std::snprintf(defaultRight, sizeof(defaultRight), "%s_TR.MT5", entry->name);
    } else {
        std::strncpy(defaultLeft, leftFilename, sizeof(defaultLeft) - 1);
        std::strncpy(defaultRight, rightFilename, sizeof(defaultRight) - 1);
    }
    leftFilename = defaultLeft;
    rightFilename = defaultRight;
    if (HandFilenamesEqual(leftFilename, rightFilename)) {
        if (DoesContainString(leftFilename + 3, "_UL.MT5") == 0) {
            void *const previousFigurePoseCallback = character ? reinterpret_cast<void *>(character->figurePoseCallback) : nullptr;
            if (AllocateHandModelWork(character, true, leftFilename, optionFlags, 3, 2, false, previousFigurePoseCallback)) {
                if (character) {
                    character->figurePoseCallback = HND_BodyFigurePose;
                    character->processShadowCallback = HND_ProcessShadow;
                }
            }
        } else if (DoesContainString(leftFilename + 3, "_UR.MT5") == 0) {
            void *const previousFigurePoseCallback = character ? reinterpret_cast<void *>(character->figurePoseCallback) : nullptr;
            if (AllocateHandModelWork(character, false, leftFilename, optionFlags, 4, 3, false, previousFigurePoseCallback)) {
                if (character) {
                    character->figurePoseCallback = HND_BodyFigurePose;
                    character->processShadowCallback = HND_ProcessShadow;
                }
            }
        }
    } else {
        void *previousFigurePoseCallback = nullptr;
        if (character) {
            previousFigurePoseCallback = reinterpret_cast<void *>(character->figurePoseCallback);
            character->figurePoseCallback = HND_FigurePose;
        }
        AllocateHandModelWork(character, true, leftFilename, optionFlags, 1, 0, calibrateToCharacter, previousFigurePoseCallback);
        AllocateHandModelWork(character, false, rightFilename, optionFlags, 2, 1, calibrateToCharacter, previousFigurePoseCallback);
    }
}

std::int32_t HND_LoadModels(Character *character) {
    if (!character)
        return 0;
    std::int32_t result{};
    if (HandModelWork *const left = FindHandModelWork(character, true); left && left->handMotionTableIndex >= 0) {
        result = LoadHandModel(character, *left);
    }
    if (HandModelWork *const right = FindHandModelWork(character, false); right && right->handMotionTableIndex >= 0) {
        result = LoadHandModel(character, *right);
    }
    return result;
}

void MHND_Initialize(Character *character) {
    g_initializingHandPose = true;
    mhndApplyCharaHandPose(character, 2, 11, 1);
    mhndApplyCharaHandPose(character, 2, 8, 4);
    g_initializingHandPose = false;
    HandPoseWork *const work = FindHandPoseWork(character);
    if (!work)
        return;
    for (const bool left : {false, true}) {
        HandPoseBlendState &state = left ? work->left : work->right;
        const std::int16_t root = left ? -66 : -65;
        const std::int16_t first = left ? 43 : 28;
        state.scratch[0] = MeasureRelativeHandNodeDistance(character, root, first + 1);
        state.scratch[2] = MeasureHandNodeChainLength(character, root, first, -1.0f);
        state.scratch[1] = MeasureHandNodeChainLength(character, root, first - 3, state.scratch[2]);
        state.scratch[3] = MeasureHandNodeChainLength(character, root, first + 3, state.scratch[2]);
        state.scratch[4] = MeasureHandNodeChainLength(character, first, first + 1, state.scratch[0] - state.scratch[2]);
        state.scratch[5] = MeasureHandNodeChainLength(character, first + 3, first + 4, state.scratch[4]);
    }
}

// Table pose poseIndex for the right (0), left (1) or both (2) hands: the selected hands' command programs stop and record the pose,
// then the pose blends in over frameCount frames or applies at once.
// @verified - p
void mhndApplyCharaHandPose(Character *character, std::int32_t hand, std::int32_t poseIndex, std::int32_t frameCount) {
    if (!character || poseIndex >= 15 || hand >= 3)
        return;
    HandPoseWork *work = FindHandPoseWork(character);
    if (work) {
        if (hand == 0 || hand == 2) {
            work->rightCommand.mode = 0;
            work->right.poseIndex = poseIndex;
        }
        if (hand == 1 || hand == 2) {
            work->leftCommand.mode = 0;
            work->left.poseIndex = poseIndex;
        }
    }

    auto *const model = static_cast<Mt5RuntimeNode *>(character->model);
    if (!model)
        return;
    if (frameCount > 1) {
        work = EnsureHandPoseWork(character);
        if (work)
            mhndBeginHandPoseBlend(*work, model, hand, poseIndex, frameCount);
        return;
    }
    if (hand == 0) {
        if (work)
            work->right.node = nullptr;
        mhndApplyRightHandPose(character, shenGetNodeByChildIdx(model, -65), kHandPoses[poseIndex][0]);
    } else if (hand == 1) {
        if (work)
            work->left.node = nullptr;
        mhndApplyLeftHandPose(character, shenGetNodeByChildIdx(model, -66), kHandPoses[poseIndex][1]);
    } else if (hand == 2) {
        if (work) {
            work->right.node = nullptr;
            work->left.node = nullptr;
        }
        mhndApplyRightHandPose(character, shenGetNodeByChildIdx(model, -65), kHandPoses[poseIndex][0]);
        mhndApplyLeftHandPose(character, shenGetNodeByChildIdx(model, -66), kHandPoses[poseIndex][1]);
    }
}



void MHND_ApplyCycleUmbrellaPose(Character *character) {
    if (!character)
        return;
    HandPoseWork *const work = FindHandPoseWork(character);
    if (work && work->right.poseIndex == 9)
        return;
    if (work) {
        work->rightCommand.mode = 0;
        work->right.poseIndex = 9;
    }
    if (!character->model)
        return;
    if (work)
        work->right.node = nullptr;
    mhndApplyRightHandPose(character, shenGetNodeByChildIdx(static_cast<Mt5RuntimeNode *>(character->model), -65), kHandPoses[9][0]);
}

// Script_ReadModelFormat7's raw hand pose, read from the scenario data.
void mhndApplyRawHandPose(Character *character, std::int32_t hand, const std::int32_t *pose, std::int32_t frameCount) {
    if (!pose)
        return;
    HandPoseRecord target{};
    std::memcpy(target.values, pose, sizeof(target.values));
    mhndApplyCharaHandTarget(character, hand, target, frameCount);
}

// Both hands run the idle command program (pose 0, 120 frames, pose 3, repeat).
// @verified - p
void mhndStartIdleHandProgram(Character *character) {
    HandPoseWork *const work = EnsureHandPoseWork(character);
    if (!work)
        return;
    work->rightCommand.mode = 14;
    work->rightCommand.cursor = kIdleHandCommands;
    work->rightCommand.loopStart = kIdleHandCommands;
    work->leftCommand.mode = 14;
    work->leftCommand.cursor = kIdleHandCommands;
    work->leftCommand.loopStart = kIdleHandCommands;
}

// Both hands run AutomaticHandCommand. An existing MHND keeps its component flags.
// @verified - p
void mhndStartAutomaticHandProgram(Character *character) {
    if (!character)
        return;
    HandPoseWork *work = FindHandPoseWork(character);
    if (!work)
        work = AllocateHandPoseWork(character);
    if (!work)
        return;
    work->rightCommand.mode = 13;
    work->rightCommand.callback = AutomaticHandCommand;
    work->leftCommand.mode = 13;
    work->leftCommand.callback = AutomaticHandCommand;
}

// @verified - p
std::int32_t mhndGetPoseStatus(Character *character, std::int32_t hand) {
    if (!character || hand >= 3)
        return -2;
    HandPoseWork *const work = FindHandPoseWork(character);
    if (!work)
        return -3;
    return hand == 1 ? work->left.poseIndex : work->right.poseIndex;
}

void Script_SetHandMotionTargetChara(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = ScenarioWords(rawParams);
    if (!params)
        return;
    HandModelWork *const hand = HND_GetWork(ScenarioCharacter(context, params[0]), params[1] == 0);
    if (!hand)
        return;
    Character *const target = ScenarioCharacter(context, params[2]);
    hand->gripTarget = target;
    if (!target)
        std::memset(hand->fingerBlendLimits, 0, sizeof(hand->fingerBlendLimits));
}

void Script_RotateAllAxisWrapper(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = ScenarioWords(rawParams);
    if (!params)
        return;
    HandModelWork *const hand = HND_GetWork(ScenarioCharacter(context, params[0]), params[1] == 0);
    if (!hand || !params[2] || !g_pCurrentTaskData)
        return;
    *reinterpret_cast<Vector3i *>(g_pCurrentTaskData + static_cast<std::ptrdiff_t>(static_cast<std::int32_t>(params[2]))) = HND_GetRootEuler(*hand);
}

void Script_SetHandEulerRotationMasked(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = ScenarioWords(rawParams);
    if (!params)
        return;
    HandModelWork *const hand = HND_GetWork(ScenarioCharacter(context, params[0]), params[1] == 0);
    if (!hand)
        return;
    const auto *const rotation = reinterpret_cast<const Vector3i *>(g_pCurrentTaskData + static_cast<std::ptrdiff_t>(static_cast<std::int32_t>(params[3])));
    const std::uint32_t mask = params[2];
    Vector3i value = HND_GetRootEuler(*hand);
    if (mask & 0x01U)
        value.x = rotation->x;
    if (mask & 0x04U)
        value.y = rotation->y;
    if (mask & 0x10U)
        value.z = rotation->z;
    if (mask & 0x02U)
        value.x += rotation->x;
    if (mask & 0x08U)
        value.y += rotation->y;
    if (mask & 0x20U)
        value.z += rotation->z;
    HND_ApplyRootEuler(*hand, value);
}
} // namespace shenmue::s1
