#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/blur_figure_pose_dc.hpp"

#include "s2/character_table_dc.hpp"
#include "s2/model_clone_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/pause.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/cloth_dc.hpp"
#else
#include "s2/blur_figure_pose.hpp"

#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/world_render.hpp"
#include "s2/archive_data.hpp"
#include "s2/map_control.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_image.hpp"
#include "s2/motion.hpp"
#include "s2/pak.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/pause.hpp"

#include <bit>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <new>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Host MT7 reader view: source storage belongs to the reader, unlike ObjBlur's separate BLOB owner.
struct BlurModelReader {
    MT7Model model;
    void *sourceAllocation;
};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void AllocateBLWK(FigurePose *figure);
void DrawBlurFigurePoseEntries(FigurePose *figure);
void DrawBlurFigurePoseInterpolations(FigurePose *figure);
void BlurWork(Character *character);
void BlendMatrices(const MT7Model *model, const MT7Node *node, float alpha);
void FreeBlurWork(Character *character, void *payload);
void SetBlurModel(Character *character, const char *path, float time);
void AttachObjectBlur(Character *character, std::uint32_t argumentCount, float time, const char *path);
std::uintptr_t figpAssignMotScaleAndDirFor(Character *character, std::int8_t scale, std::int8_t direction);
std::uintptr_t figpToggleMotionworkFlag(Character *character, MotionWork *work);
FigurePose *figpAssignModeForCharacter(Character *character, std::int8_t mode);
std::uintptr_t EV_figpAssignFlagsFor(Character *character, std::uint8_t bitIndex, std::int8_t enabled);
void LoadBlurModelForCharacter(shen_name character, const char *path, float time);
std::uintptr_t figpLoadBlurModelFor(shen_name character, const char *path);

namespace {

void DrawBlurBranch(float time, std::uint8_t mode, FigurePose *figure);
bool CaptureBlurEntry(BlurWorkEntry &entry, FigurePose *figure);
void DrawBlurObjectEntries(FigurePose *figure);
void DrawBlurObjectInterpolations(FigurePose *figure);
std::uintptr_t LoadBlurModel(FigurePose *figure, const char *filename, bool areaRelative);
void AssignBlurCallbacks(Character *character);

} // namespace
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
FigurePose *mtSetupModelFigurePose(Character *character) {
    return static_cast<FigurePose *>(AllocateModelComponent(character, MODEL_FIGURE_POSE, sizeof(FigurePose), nullptr, nullptr, nullptr));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void DrawBlurBranch(FigurePose *figure, std::uint8_t mode, float time);

} // namespace

void BlurWork(Character *character);

void FreeBlurWork(Character *character, void *payload);

void LoadBlurModel(FigurePose *figure, const char *filename);

void AllocateBLWK(FigurePose *figure);

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const std::int32_t blurBoneSelectors[15] = {3, 5, 9, 12, 14, 16, 19, 21, 22, 23, 24, 26, 28, 29, 31};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const std::int32_t blurModelNodeIds[15] = {1, -67, 5, 6, -65, 10, 11, -66, 14, 16, 17, 18, 21, 22, 23};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const std::int16_t blurBranchNodes[10][2] = {{0, 0}, {1, -1}, {1, 14}, {14, -1}, {5, -1}, {10, -1}, {16, -1}, {21, -1}, {1, 14}, {1, 14}};
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void AssignBlurCallbacks(Character *character);
void ApplyFightCharacterDefinition(Character *character, const CharacterTableArgument *arguments, std::int32_t count);
std::uint8_t *figpAssignModeForCharacter(Character *character, std::uint8_t mode);
std::uint32_t figpAssignMotScaleAndDirFor(Character *character, std::uint8_t scale, std::uint8_t direction);
FigurePoseControlPrefix *EV_figpAssignFlagsFor(Character *character, std::uint8_t bitIndex, std::uint8_t enabled);
std::uint32_t figpGetFlagMask(Character *character, std::uint8_t bitIndex);
void SetBlurModel(Character *character, const char *filename, float time);
void figpToggleMotionworkFlag(Character *character, MotionWork *work);
void DrawBlurFigurePoseEntries(FigurePose *figure);
void DrawBlurFigurePoseInterpolations(FigurePose *figure);
void BlurWork(Character *character);
void DrawBlurObjectEntries(FigurePose *figure);
void DrawBlurObjectInterpolations(FigurePose *figure);
void BlendMatrices(const MT7Node *model, float alpha);
void FreeBlurWork(Character *character, void *payload);
void LoadBlurModel(FigurePose *figure, const char *filename);
void AllocateBLWK(FigurePose *figure);
void AttachObjectBlur(Character *character, const CharacterTableRecord *record);
void figpLoadBlurModelFor(shen_name name, const char *filename);
void LoadBlurModelForCharacter(shen_name name, const char *filename, float time);

namespace {

void DrawBlurBranch(FigurePose *figure, std::uint8_t mode, float time);

} // namespace
#else
FigurePose *mtSetupModelFigurePose(Character *character);
FigurePose *figpAssignModeForCharacter(Character *character, std::int8_t mode);
std::uintptr_t figpAssignMotScaleAndDirFor(Character *character, std::int8_t scale, std::int8_t direction);
std::uintptr_t EV_figpAssignFlagsFor(Character *character, std::uint8_t bitIndex, std::int8_t enabled);
void SetBlurModel(Character *character, const char *path, float time);
void mtFindBlurModel(Character *character, const char *path, const float *time);
std::uintptr_t figpToggleMotionworkFlag(Character *character, MotionWork *work);
void DrawBlurFigurePoseEntries(FigurePose *figure);
void DrawBlurFigurePoseInterpolations(FigurePose *figure);
void BlurWork(Character *character);
void BlendMatrices(const MT7Model *model, const MT7Node *node, float alpha);
void FreeBlurWork(Character *character, void *payload);
void AllocateBLWK(FigurePose *figure);
void AttachObjectBlur(Character *character, std::uint32_t argumentCount, float time, const char *path);
std::uintptr_t figpLoadBlurModelFor(shen_name character, const char *path);
FigurePose *figpAssignModeFor(shen_name character, std::int8_t mode);
std::uintptr_t figpSetFlagModesFor(shen_name character, std::uint8_t bitIndex, std::int8_t enabled);
std::uintptr_t figpSetFlagsBlendForBlur(shen_name character, std::int8_t scale, std::int8_t direction);
void LoadBlurModelForCharacter(shen_name character, const char *path, float time);

namespace {

void AssignBlurCallbacks(Character *character);
void DrawBlurObjectEntries(FigurePose *figure);
void DrawBlurObjectInterpolations(FigurePose *figure);
void ReleaseBlurModelReader(FigurePose *figure);
void DrawBlurBranch(float time, std::uint8_t mode, FigurePose *figure);
bool CaptureBlurEntry(BlurWorkEntry &entry, FigurePose *figure);
std::uintptr_t LoadBlurModel(FigurePose *figure, const char *filename, bool areaRelative);

} // namespace
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void AssignBlurCallbacks(Character *character) {
    FigurePose *const figure = static_cast<FigurePose *>(charFindComp(character, MODEL_FIGURE_POSE));
    if (!figure)
        return;
    AllocateBLWK(figure);
    figure->time = 1.0f;
    figure->character = character;
    figure->previousCoreCallback = SetReflectorBoardCoreCallback(character, BlurWork);
    if (!figure->previousCoreCallback)
        figure->previousCoreCallback = charaAssignCoreCallback(character, BlurWork);
    SetModelComponentCleanup(character, MODEL_FIGURE_POSE, FreeBlurWork);
}
#endif

#else

namespace {

// @verified - p
void AssignBlurCallbacks(Character *character) {
    FigurePose *const figure = static_cast<FigurePose *>(FindModelComponent(character, MODEL_FIGURE_POSE));
    if (!figure)
        return;
    AllocateBLWK(figure);
    figure->time = 1.0f;
    figure->character = character;
    figure->previousCoreCallback = assignBlurFigurePoseCallback(character, BlurWork);
    if (!figure->previousCoreCallback) {
        figure->previousCoreCallback = character->core;
        character->core = BlurWork;
    }
    (reinterpret_cast<ModelComponentHeader *>(figure) - 1)->cleanup = FreeBlurWork;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t *figpAssignModeForCharacter(Character *character, std::uint8_t mode) {
    std::uint8_t *const figure = static_cast<std::uint8_t *>(charFindComp(character, MODEL_FIGURE_POSE));
    if (figure)
        *figure = mode;
    return figure;
}

#else

// @verified - p
FigurePose *figpAssignModeForCharacter(Character *character, std::int8_t mode) {
    FigurePose *const figure = static_cast<FigurePose *>(FindModelComponent(character, MODEL_FIGURE_POSE));
    if (figure)
        figure->mode = mode;
    return figure;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t figpAssignMotScaleAndDirFor(Character *character, std::uint8_t scale, std::uint8_t direction) {
    FigurePoseControlPrefix *const figure = static_cast<FigurePoseControlPrefix *>(charFindComp(character, MODEL_FIGURE_POSE));
    if (!figure)
        return 0;
    const std::int8_t signedDirection = static_cast<std::int8_t>(direction);
    if (signedDirection < 0) {
        figure->startTime = 1;
        figure->direction = static_cast<std::uint8_t>(-signedDirection);
    } else {
        figure->startTime = direction;
        figure->direction = 0;
    }
    figure->motionScale = scale;
    if (scale == 1)
        figure->direction = 0;
    return scale;
}
#endif

#else

// @verified - p
std::uintptr_t figpAssignMotScaleAndDirFor(Character *character, std::int8_t scale, std::int8_t direction) {
    FigurePose *const figure = static_cast<FigurePose *>(FindModelComponent(character, MODEL_FIGURE_POSE));
    if (!figure)
        return 0;
    if (direction >= 0) {
        figure->startTime = direction;
        figure->directionFlag = 0;
    } else {
        figure->startTime = 1;
        figure->directionFlag = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(-direction));
    }
    figure->motionScale = scale;
    if (scale == 1)
        figure->directionFlag = 0;
    return static_cast<std::uint8_t>(figure->directionFlag);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
FigurePoseControlPrefix *EV_figpAssignFlagsFor(Character *character, std::uint8_t bitIndex, std::uint8_t enabled) {
    FigurePoseControlPrefix *const figure = static_cast<FigurePoseControlPrefix *>(charFindComp(character, MODEL_FIGURE_POSE));
    if (figure) {
        if (enabled)
            figure->flags |= 1U << bitIndex;
        else
            figure->flags &= ~(1U << bitIndex);
    }
    return figure;
}
#endif

#else

// @verified - p
std::uintptr_t EV_figpAssignFlagsFor(Character *character, std::uint8_t bitIndex, std::int8_t enabled) {
    FigurePose *const figure = static_cast<FigurePose *>(FindModelComponent(character, MODEL_FIGURE_POSE));
    if (!figure)
        return 0;
    const std::uint8_t mask = static_cast<std::uint8_t>(1U << (bitIndex & 31U));
    if (enabled)
        figure->flags |= mask;
    else {
        figure->flags &= ~mask;
        return bitIndex;
    }
    return reinterpret_cast<std::uintptr_t>(figure);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t figpGetFlagMask(Character *character, std::uint8_t bitIndex) {
    FigurePoseControlPrefix *const figure = static_cast<FigurePoseControlPrefix *>(charFindComp(character, MODEL_FIGURE_POSE));
    return figure ? figure->flags & (1U << bitIndex) : 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetBlurModel(Character *character, const char *filename, float time) {
    FigurePose *figure = static_cast<FigurePose *>(charFindComp(character, MODEL_FIGURE_POSE));
    if (!figure) {
        figure = static_cast<FigurePose *>(mtSetupModelFigurePose(character));
        if (filename)
            LoadBlurModel(figure, filename);
        else {
            figure->filename[0] = 0;
            figure->targetModelReader = 0;
            figure->targetRootInstance = CloneModelNodeTree(character->model);
        }
        AssignBlurCallbacks(character);
    }
    figure->time = time;
}
#endif

#else

// @verified - p
void SetBlurModel(Character *character, const char *path, float time) {
    FigurePose *figure = static_cast<FigurePose *>(FindModelComponent(character, MODEL_FIGURE_POSE));
    if (figure) {
        figure->time = time;
        return;
    }
    figure = mtSetupModelFigurePose(character);
    if (path)
        LoadBlurModel(figure, path, false);
    else {
        figure->field08 = 0;
        figure->targetModelReader = nullptr;
        MT7Model *const model = static_cast<MT7Model *>(character->model);
        figure->targetRootInstance = CloneMT7ModelNodeTree(GetMT7ReaderData(model), *model, 0);
    }
    AssignBlurCallbacks(character);
    figure->time = time;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void mtFindBlurModel(Character *character, const char *path, const float *time) {
    FigurePose *const figure = static_cast<FigurePose *>(FindModelComponent(character, MODEL_FIGURE_POSE));
    if (!figure)
        return;
    LoadBlurModel(figure, path, false);
    AssignBlurCallbacks(character);
    if (time)
        figure->time = *time;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void figpToggleMotionworkFlag(Character *character, MotionWork *work) {
    FigurePoseControlPrefix *const figure = static_cast<FigurePoseControlPrefix *>(charFindComp(character, MODEL_FIGURE_POSE));
    if (!figure)
        return;
    const std::uint8_t *bytes = reinterpret_cast<const std::uint8_t *>(mtwkFindFrameEvent(work, 24, 1));
    if (!bytes || (figure->flags & 1U))
        return;
    std::uint8_t mode = bytes[3];
    if (mtwkSetRegionFlag(work, 70, 1, LowerBody)) {
        switch (mode) {
        case 4:
            mode = 5;
            break;
        case 5:
            mode = 4;
            break;
        case 6:
            mode = 7;
            break;
        case 7:
            mode = 6;
            break;
        }
    }
    if (mode)
        figpAssignMotScaleAndDirFor(character, bytes[4], bytes[5]);
    figure->mode = mode;
}
#endif

#else

// @verified - p
std::uintptr_t figpToggleMotionworkFlag(Character *character, MotionWork *work) {
    FigurePose *const figure = static_cast<FigurePose *>(FindModelComponent(character, MODEL_FIGURE_POSE));
    if (!figure)
        return 0;
    const MotionTimePair &clock = work->rig.lowerBody.clock.current;
    const std::span<const std::byte> command =
        FindLoadedMotionTimedCommand(work->runtime.currentMotionId[0], 24, clock.splitTimeFrames - clock.ticksTime, clock.splitTimeFrames, 1);
    if (!command.empty() && (figure->flags & 0x41U) == 0) {
        std::uint8_t mode = static_cast<std::uint8_t>(command[3]);
        const std::uint16_t mirrored = mtwkSetRegionFlag(work, 70, 1, MotionRegion::LowerBody);
        if (mirrored) {
            switch (mode) {
            case 4:
                mode = 5;
                break;
            case 5:
                mode = 4;
                break;
            case 6:
                mode = 7;
                break;
            case 7:
                mode = 6;
                break;
            default:
                break;
            }
        }
        const std::uintptr_t result = mode ? figpAssignMotScaleAndDirFor(character, std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(command[4])),
                                                 std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(command[5]))) :
                                             mirrored;
        figure->mode = std::bit_cast<std::int8_t>(mode);
        return result;
    }
    figure->flags &= ~0x40U;
    return reinterpret_cast<std::uintptr_t>(command.data());
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawBlurFigurePoseEntries(FigurePose *figure) {
    Vector3 accumulated;
    accumulated.x = accumulated.y = accumulated.z = 0.0f;
    std::int32_t index = static_cast<std::int8_t>(figure->entryIndex - 1);
    if (index < 0)
        index = 19;
    while (index != static_cast<std::uint8_t>(figure->entryIndex)) {
        BlurWorkEntry &entry = figure->blurWork[index];
        index = static_cast<std::int8_t>(index - 1);
        if (index < 0)
            index = 19;
        if (FLT_EPSILON > std::fabs(entry.time))
            continue;
        for (std::uint8_t bone = 0; bone < 15; ++bone) {
            MT7Node *const node = FindMT7Node(figure->targetRootInstance, blurModelNodeIds[bone]);
            if (node)
                node->transformOverride = &entry.boneTransforms[bone];
        }
        if (figure->flags & 8U) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrix(&entry.rootTransform);
            if (figure->flags & 0x10U) {
                accumulated.x += entry.position.x;
                accumulated.y += entry.position.y;
                accumulated.z += entry.position.z;
                Matrix4x4::preTranslate(-accumulated.x, -accumulated.y, -accumulated.z);
            }
            DrawBlurBranch(figure, static_cast<std::uint8_t>(entry.mode), entry.time);
            PopCurrentMatrix(1);
        } else
            DrawBlurBranch(figure, static_cast<std::uint8_t>(entry.mode), entry.time);
    }
}
#endif

#else

// @verified - p
void DrawBlurFigurePoseEntries(FigurePose *figure) {
    MT7Model &model = *figure->targetRootInstance;
    const std::uint8_t current = static_cast<std::uint8_t>(figure->entryIndex);
    std::int32_t index = current == 0 ? 19 : current - 1;
    Vector3 accumulated{.x = 0.0f, .y = 0.0f, .z = 0.0f};
    while (index != current) {
        BlurWorkEntry &entry = figure->blurWork[index];
        index = index == 0 ? 19 : index - 1;
        if (FLT_EPSILON > std::fabs(entry.time))
            continue;
        for (std::size_t bone = 0; bone < blurBoneTransformCount; ++bone) {
            MT7Node *const node = GetChildObjectByIndex(model, model.nodes.data(), blurModelNodeIds[bone]);
            if (node)
                SetMT7NodeTransformOverride(*node, &entry.boneTransforms[bone]);
        }
        if ((figure->flags & 8U) != 0) {
            Matrix4x4_Push();
            Matrix4x4::SetActiveTransform(&entry.rootTransform);
            if ((figure->flags & 0x10U) != 0) {
                accumulated.x += entry.position.x;
                accumulated.y += entry.position.y;
                accumulated.z += entry.position.z;
                Matrix4x4::preTranslate(-accumulated.x, -accumulated.y, -accumulated.z);
            }
            DrawBlurBranch(entry.time, static_cast<std::uint8_t>(entry.mode), figure);
            Matrix4x4::Pop(1);
        } else {
            DrawBlurBranch(entry.time, static_cast<std::uint8_t>(entry.mode), figure);
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawBlurFigurePoseInterpolations(FigurePose *figure) {
    const std::uint8_t direction = static_cast<std::uint8_t>(figure->directionFlag);
    if (!direction)
        return;
    const float step = 1.0f / static_cast<float>(direction + 1);
    Matrix4x4 transforms[15];
    const BlurWorkEntry *first = figure->blurWork;
    const BlurWorkEntry *second = first + 1;
    for (std::uint8_t index = 0; index < 20; ++index, ++first, ++second) {
        if (index > 18)
            second = figure->blurWork;
        if (FLT_EPSILON > std::fabs(first->time) && FLT_EPSILON > std::fabs(second->time))
            continue;
        if (std::fabs(first->time - second->time) > figure->maximumTime)
            continue;
        float alpha = step;
        for (std::uint8_t iteration = 0; iteration < static_cast<std::uint8_t>(figure->directionFlag); ++iteration, alpha += step) {
            for (std::uint8_t bone = 0; bone < 15; ++bone) {
                MT7Node *const node = FindMT7Node(figure->targetRootInstance, blurModelNodeIds[bone]);
                if (node) {
                    LerpBlurMatrix(&transforms[bone], alpha, &first->boneTransforms[bone], &second->boneTransforms[bone]);
                    node->transformOverride = &transforms[bone];
                }
            }
            const float time = first->time + (second->time - first->time) * alpha;
            DrawBlurBranch(figure, static_cast<std::uint8_t>(first->mode), time);
        }
    }
}
#endif

#else

// @verified - p
void DrawBlurFigurePoseInterpolations(FigurePose *figure) {
    const std::uint8_t direction = static_cast<std::uint8_t>(figure->directionFlag);
    if (direction == 0)
        return;
    const float step = 1.0f / static_cast<float>(direction + 1U);
    MT7Model &model = *figure->targetRootInstance;
    Matrix4x4 transforms[blurBoneTransformCount];
    for (std::size_t index = 0; index < blurWorkEntryCount; ++index) {
        const BlurWorkEntry &first = figure->blurWork[index];
        const BlurWorkEntry &second = figure->blurWork[index == 19 ? 0 : index + 1];
        if ((!(FLT_EPSILON > std::fabs(first.time)) || !(FLT_EPSILON > std::fabs(second.time))) &&
            !(std::fabs(first.time - second.time) > figure->maximumTime)) {
            float alpha = step;
            for (std::uint32_t iteration = 0; iteration < direction; ++iteration, alpha += step) {
                for (std::size_t bone = 0; bone < blurBoneTransformCount; ++bone) {
                    MT7Node *const node = GetChildObjectByIndex(model, model.nodes.data(), blurModelNodeIds[bone]);
                    if (node) {
                        LerpBlurMatrix(&transforms[bone], alpha, &first.boneTransforms[bone], &second.boneTransforms[bone]);
                        *const_cast<Matrix4x4 *>(GetMT7NodeTransformOverride(*node)) = transforms[bone];
                    }
                }
                DrawBlurBranch((second.time - first.time) * alpha + first.time, static_cast<std::uint8_t>(first.mode), figure);
            }
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BlurWork(Character *character) {
    FigurePose *const figure = static_cast<FigurePose *>(charFindComp(character, MODEL_FIGURE_POSE));
    figure->previousCoreCallback(character);
    MotionWork *const motion = charFindMotionwork(character);
    if (!figure->targetRootInstance || !motion)
        return;
    figpUpdateBlurModelState(figure);
    for (std::uint8_t index = 0; index < 20; ++index) {
        BlurWorkEntry &entry = figure->blurWork[index];
        switch (entry.state) {
        case 0:
            if ((entry.flags & 1U) == 0)
                break;
            entry.flags &= static_cast<std::uint8_t>(~1U);
            entry.state = 1;
        case 1:
            mtwkUpdateRootDeltaVector(motion, &entry.position);
            for (std::uint8_t bone = 0; bone < 15; ++bone) {
                const Matrix4x4 *const transform = mtwkGetBoneTransform(motion, blurBoneSelectors[bone]);
                if (transform)
                    entry.boneTransforms[bone] = *transform;
            }
            matrix_stack::GetMatrix(&entry.rootTransform);
            entry.time = figure->time;
            entry.state = 2;
            break;
        case 2:
            if (!shenIsGamePaused()) {
                entry.time -= figure->maximumTime;
                if (entry.time < 0) {
                    entry.time = 0;
                    entry.state = 0;
                }
            }
            break;
        }
    }
    DrawBlurFigurePoseEntries(figure);
    DrawBlurFigurePoseInterpolations(figure);
}
#endif

#else

// @verified - p
void BlurWork(Character *character) {
    FigurePose *const figure = static_cast<FigurePose *>(FindModelComponent(character, MODEL_FIGURE_POSE));
    figure->previousCoreCallback(character);
    MotionWork *const motion = FindMotionWork(character);
    if (!figure->targetRootInstance || !motion)
        return;
    figpUpdateBlurModelState(figure);
    for (std::size_t index = 0; index < blurWorkEntryCount; ++index) {
        BlurWorkEntry &entry = figure->blurWork[index];
        if (!CaptureBlurEntry(entry, figure))
            continue;
        entry.position = motion->runtime.rootDelta;
        if ((motion->runtime.motnFlags & 1U) != 0) {
            entry.position.x = motion->runtime.rootPosition.x - motion->runtime.previousRootPosition.x;
            entry.position.z = motion->runtime.rootPosition.z - motion->runtime.previousRootPosition.z;
        }
        for (std::size_t bone = 0; bone < blurBoneTransformCount; ++bone) {
            const Matrix4x4 *const transform = mtwkGetBoneTransform(motion, blurBoneSelectors[bone]);
            if (transform)
                entry.boneTransforms[bone] = *transform;
        }
        Matrix4x4::GetTransformMatrix(&entry.rootTransform);
        entry.time = figure->time;
        entry.state = 2;
    }
    SetLegacyBusyPreparingDraw(false);
    g_modelDeformationSuppressed = 1;
    DrawBlurFigurePoseEntries(figure);
    g_modelDeformationSuppressed = 0;
    DrawBlurFigurePoseInterpolations(figure);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawBlurBranch(FigurePose *figure, std::uint8_t mode, float time) {
    MT7Node *const root = figure->targetRootInstance;
    if (static_cast<std::uint8_t>(mode - 8U) < 2) {
        MT7Node *node;
        do {
            node = FindMT7Node(root, 5);
        } while (0);
        if (node)
            node = FindMT7Node(root, 5);
        DrawMT7ProceduralBranch(node, -1, time);
        do {
            node = FindMT7Node(root, 10);
        } while (0);
        if (node)
            do {
                node = FindMT7Node(root, 10);
            } while (0);
        DrawMT7ProceduralBranch(node, -1, time);
        if (figure->mode == 9) {
            do {
                node = FindMT7Node(root, 16);
            } while (0);
            if (node)
                do {
                    node = FindMT7Node(root, 16);
                } while (0);
            DrawMT7ProceduralBranch(node, -1, time);
            do {
                node = FindMT7Node(root, 21);
            } while (0);
            if (node)
                do {
                    node = FindMT7Node(root, 21);
                } while (0);
            DrawMT7ProceduralBranch(node, -1, time);
        }
    } else {
        MT7Node *const node = FindMT7Node(root, blurBranchNodes[mode][0]);
        if (node)
            DrawMT7ProceduralBranch(node, blurBranchNodes[mode][1], time);
    }
}
#endif

} // namespace

#else

namespace {

// @verified - p
void DrawBlurBranch(float time, std::uint8_t mode, FigurePose *figure) {
    MT7Model &model = *figure->targetRootInstance;
    MT7Node *const root = model.nodes.data();
    if (static_cast<std::uint8_t>(mode - 8U) <= 1U) {
        DrawMT7ProceduralBranch(model, GetChildObjectByIndex(model, root, 5), 0xffff, time);
        DrawMT7ProceduralBranch(model, GetChildObjectByIndex(model, root, 10), 0xffff, time);
        if (figure->mode == 9) {
            DrawMT7ProceduralBranch(model, GetChildObjectByIndex(model, root, 16), 0xffff, time);
            DrawMT7ProceduralBranch(model, GetChildObjectByIndex(model, root, 21), 0xffff, time);
        }
    } else {
        MT7Node *const node = GetChildObjectByIndex(model, root, blurBranchNodes[mode][0]);
        if (node)
            DrawMT7ProceduralBranch(model, node, blurBranchNodes[mode][1], time);
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
bool CaptureBlurEntry(BlurWorkEntry &entry, FigurePose *figure) {
    if (entry.state == 0 && (entry.flags & 1U) != 0) {
        entry.state = 1;
        entry.flags &= ~1U;
    }
    if (entry.state == 1)
        return true;
    if (entry.state == 2 && (g_loadingFlag & 2U) == 0) {
        entry.time -= figure->maximumTime;
        if (entry.time < 0.0f) {
            entry.time = 0.0f;
            entry.state = 0;
        }
    }
    return false;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AttachObjectBlur(Character *character, const CharacterTableRecord *record) {
    FigurePose *const figure = static_cast<FigurePose *>(AllocateModelComponent(character, MODEL_FIGURE_POSE, sizeof(FigurePose), 0));
    if (!figure)
        return;
    MT7Node *const model = character->model;
    model->taskData = figure;
    figure->character = character;
    AllocateBLWK(figure);
    switch (record->count) {
    default:
        figure->time = 1.0f;
        figure->targetModelReader = 0;
        figure->targetRootInstance = model;
        figure->flags |= 0x20U;
        break;
    case 1:
        figure->time = record->arguments[0].value.floating;
        figure->targetModelReader = 0;
        figure->targetRootInstance = model;
        break;
    case 2: {
        char directory[32];
        std::strcpy(figure->filename, record->arguments[1].value.string);
        PakGlobalData *const package = GetPakLoadTaskDataPointer();
        if (package) {
            if (FileLoader(package, figure->filename, MODEL_BLUR_PACKAGE_ENTRY)) {
                figure->targetModelReader = LoadGameModel(package, figure->filename);
                std::strcpy(directory, "pack");
            } else {
                figure->targetModelReader = 0;
            }
        }
        if (!figure->targetModelReader) {
            union {
                std::uint32_t name;
                char text[4];
            } area;
            area.name = g_currentMap;
            std::sprintf(directory, "scene/%02d/%c%c%c%c", static_cast<int>(g_actNumber), area.text[0], area.text[1], area.text[2], area.text[3]);
            figure->modelFile = shenLoadFile(directory, figure->filename, MEMORY_BLUR_MODEL_FILE);
            figure->targetModelReader = InitializeMT7Model(figure->modelFile);
            figure->targetRootInstance = CloneMT7ModelRoot(figure->targetModelReader);
        }
        figure->time = record->arguments[0].value.floating;
        break;
    }
    }
    figure->previousModelCallback = imgmAssignUpdateCallback(character, BlendMatrices);
    SetModelComponentCleanup(character, MODEL_FIGURE_POSE, FreeBlurWork);
}

#else

// @verified - p
void AttachObjectBlur(Character *character, std::uint32_t argumentCount, float time, const char *path) {
    FigurePose *const figure = mtSetupModelFigurePose(character);
    if (!figure)
        return;
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    model->nodes.front().taskData = figure;
    figure->character = character;
    AllocateBLWK(figure);
    if (argumentCount == 2) {
        LoadBlurModel(figure, path, true);
        figure->time = time;
    } else {
        if (argumentCount == 1)
            figure->time = time;
        else {
            figure->flags |= 0x20U;
            figure->time = 1.0f;
        }
        figure->targetRootInstance = model;
        figure->targetModelReader = nullptr;
    }
    figure->previousModelCallback = character->applyScale;
    character->applyScale = BlendMatrices;
    SetModelComponentCleanup(character, MODEL_FIGURE_POSE, FreeBlurWork);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void BlendMatrices(const MT7Node *model, float alpha) {
    charaPrepareNodesForDraw(model, alpha);
    FigurePose *const figure = static_cast<FigurePose *>(model->taskData);
    Vector3 position;
    Vector3i rotation;
    GetCharacterWorldPosition(figure->character, &position);
    GetCharacterWorldRotation(figure->character, &rotation);
    if (!figure->targetRootInstance)
        return;
    figpUpdateBlurModelState(figure);
    for (std::uint8_t index = 0; index < blurWorkEntryCount; ++index) {
        BlurWorkEntry &entry = figure->blurWork[index];
        switch (entry.state) {
        case 0:
            if ((entry.flags & 1U) == 0)
                break;
            entry.flags &= static_cast<std::uint8_t>(~1U);
            entry.state = 1;
        case 1:
            matrix_stack::PushUnitMatrix();
            Matrix4x4::preTranslate(position.x, position.y, position.z);
            if (rotation.z)
                Matrix4x4::RotateAroundZ_Wrapper(rotation.z);
            if (rotation.y)
                Matrix4x4::RotateAroundY_Wrapper(rotation.y);
            if (rotation.x)
                Matrix4x4::RotateAroundX_Wrapper(rotation.x);
            matrix_stack::GetMatrix(&entry.boneTransforms[0]);
            PopCurrentMatrix(1);
            camCtrlGetProjectionMatrix(&entry.rootTransform);
            entry.time = figure->time;
            entry.state = 2;
            break;
        case 2:
            if (shenIsGamePaused())
                break;
            entry.time -= figure->maximumTime;
            if (entry.time < 0.0f) {
                entry.time = 0.0f;
                entry.state = 0;
            }
            break;
        }
    }
    DrawBlurObjectEntries(figure);
    DrawBlurObjectInterpolations(figure);
}

#else

// @verified - p
void BlendMatrices(const MT7Model *model, const MT7Node *node, float alpha) {
    charaPrepareNodesForDraw(model, node, alpha);
    FigurePose *const figure = static_cast<FigurePose *>(node->taskData);
    const Vector3 position = GetCharacterWorldPosition(figure->character);
    const Vector3i rotation = GetCharacterWorldRotation(figure->character);
    if (!figure->targetRootInstance)
        return;
    figpUpdateBlurModelState(figure);
    for (std::size_t index = 0; index < blurWorkEntryCount; ++index) {
        BlurWorkEntry &entry = figure->blurWork[index];
        if (!CaptureBlurEntry(entry, figure))
            continue;
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        if (rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation.z));
        if (rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation.y));
        if (rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation.x));
        Matrix4x4::GetTransformMatrix(&entry.boneTransforms[0]);
        Matrix4x4::Pop(1);
        entry.rootTransform = g_cameraProjectionMatrix;
        entry.time = figure->time;
        entry.state = 2;
    }
    DrawBlurObjectEntries(figure);
    DrawBlurObjectInterpolations(figure);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ReleaseBlurModelReader(FigurePose *figure) {
    BlurModelReader *const reader = static_cast<BlurModelReader *>(figure->targetModelReader);
    if (!reader)
        return;
    ReleaseModelFormat7(reader->model);
    if (reader->sourceAllocation)
        HLib::FreeTaskData(reader->sourceAllocation);
    reader->~BlurModelReader();
    HLib::FreeTaskData(reader);
    figure->targetModelReader = nullptr;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void DrawBlurObjectEntries(FigurePose *figure) {
    Matrix4x4 projection;
    camCtrlGetProjectionMatrix(&projection);
    MT7Node *const root = figure->targetRootInstance;
    for (std::uint8_t index = 0; index < blurWorkEntryCount; ++index) {
        const BlurWorkEntry &entry = figure->blurWork[index];
        if (!(FLT_EPSILON > std::fabs(entry.time))) {
            matrix_stack::Matrix4x4_Push(0);
            if (figure->flags & 8U)
                matrix_stack::LoadMatrix(&entry.rootTransform);
            else
                matrix_stack::LoadMatrix(&projection);
            Matrix4x4::MultiplyWithPrePassMatrix(&entry.boneTransforms[0]);
            charaPrepareNodesForDraw(root, entry.time);
            PopCurrentMatrix(1);
        }
    }
}

#else

namespace {

// @verified - p
void DrawBlurObjectEntries(FigurePose *figure) {
    const Matrix4x4 projection = g_cameraProjectionMatrix;
    for (std::size_t index = 0; index < blurWorkEntryCount; ++index) {
        const BlurWorkEntry &entry = figure->blurWork[index];
        if (!(FLT_EPSILON > std::fabs(entry.time))) {
            Matrix4x4_Push();
            Matrix4x4::SetActiveTransform((figure->flags & 8U) != 0 ? &entry.rootTransform : &projection);
            Matrix4x4::MultiplyWithPrePassMatrix(&entry.boneTransforms[0]);
            charaPrepareNodesForDraw(figure->targetRootInstance, figure->targetRootInstance->nodes.data(), entry.time);
            Matrix4x4::Pop(1);
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void DrawBlurObjectInterpolations(FigurePose *figure) {
    const std::uint8_t direction = static_cast<std::uint8_t>(figure->directionFlag);
    if (!direction)
        return;
    const float step = 1.0f / static_cast<float>(direction + 1);
    Matrix4x4 transform, projection;
    camCtrlGetProjectionMatrix(&projection);
    const BlurWorkEntry *first = figure->blurWork;
    const BlurWorkEntry *second = first + 1;
    for (std::uint8_t index = 0; index < blurWorkEntryCount; ++index, ++first, ++second) {
        if (index > 18)
            second = figure->blurWork;
        if ((!(FLT_EPSILON > std::fabs(first->time)) || !(FLT_EPSILON > std::fabs(second->time))) &&
            !(std::fabs(first->time - second->time) > figure->maximumTime)) {
            float alpha = step;
            for (std::uint8_t iteration = 0; iteration < static_cast<std::uint8_t>(figure->directionFlag); ++iteration, alpha += step) {
                LerpBlurMatrix(&transform, alpha, &first->boneTransforms[0], &second->boneTransforms[0]);
                const float time = ((second->time - first->time) * (alpha) + (first->time));
                matrix_stack::Matrix4x4_Push(0);
                matrix_stack::LoadMatrix(&projection);
                Matrix4x4::MultiplyWithPrePassMatrix(&transform);
                charaPrepareNodesForDraw(figure->targetRootInstance, time);
                PopCurrentMatrix(1);
            }
        }
    }
}

#else

namespace {

// @verified - p
void DrawBlurObjectInterpolations(FigurePose *figure) {
    const std::uint8_t direction = static_cast<std::uint8_t>(figure->directionFlag);
    if (direction == 0)
        return;
    const float step = 1.0f / static_cast<float>(direction + 1U);
    const Matrix4x4 projection = g_cameraProjectionMatrix;
    Matrix4x4 transform;
    for (std::size_t index = 0; index < blurWorkEntryCount; ++index) {
        const BlurWorkEntry &first = figure->blurWork[index];
        const BlurWorkEntry &second = figure->blurWork[index == 19 ? 0 : index + 1];
        if ((!(FLT_EPSILON > std::fabs(first.time)) || !(FLT_EPSILON > std::fabs(second.time))) &&
            !(std::fabs(first.time - second.time) > figure->maximumTime)) {
            float alpha = step;
            for (std::uint32_t iteration = 0; iteration < direction; ++iteration, alpha += step) {
                LerpBlurMatrix(&transform, alpha, &first.boneTransforms[0], &second.boneTransforms[0]);
                Matrix4x4_Push();
                Matrix4x4::SetActiveTransform(&projection);
                Matrix4x4::MultiplyWithPrePassMatrix(&transform);
                charaPrepareNodesForDraw(figure->targetRootInstance, figure->targetRootInstance->nodes.data(), (second.time - first.time) * alpha + first.time);
                Matrix4x4::Pop(1);
            }
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AllocateBLWK(FigurePose *figure) {
    BlurWorkEntry *entry = static_cast<BlurWorkEntry *>(HLib::AllocateTaskData(sizeof(BlurWorkEntry) * blurWorkEntryCount, MEMORY_BLUR_WORK));
    figure->blurWork = entry;
    matrix_stack::PushUnitMatrix();
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(blurWorkEntryCount); ++entry, ++index) {
        entry->state = 0;
        entry->time = 0.0f;
        entry->flags = 0;
        entry->mode = 0;
        entry->position.z = 0.0f;
        entry->position.y = 0.0f;
        entry->position.x = 0.0f;
        for (std::int32_t bone = 0; bone < static_cast<std::int32_t>(blurBoneTransformCount); ++bone)
            matrix_stack::GetMatrix(&entry->boneTransforms[bone]);
    }
    PopCurrentMatrix(1);
    figure->filename[0] = '\0';
    figure->mode = 0;
    figure->flags = 0;
    figure->entryIndex = 0;
    figure->startTime = 0;
    figure->motionScale = 0;
    figure->directionFlag = 0;
    figure->maximumTime = 0.0f;
    figure->counter = 0;
    figure->modelFile = 0;
}

#else

// @verified - p
void AllocateBLWK(FigurePose *figure) {
    figure->blurWork = static_cast<BlurWorkEntry *>(HLib::AllocateTaskData(sizeof(BlurWorkEntry) * blurWorkEntryCount, MEMORY_BLUR_WORK));
    Matrix4x4_Push(&transformMatrix_prePass);
    for (std::size_t index = 0; index < blurWorkEntryCount; ++index) {
        BlurWorkEntry &entry = figure->blurWork[index];
        entry.flags = 0;
        entry.state = 0;
        entry.time = 0.0f;
        entry.position = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
        entry.mode = 0;
        for (std::size_t bone = 0; bone < blurBoneTransformCount; ++bone)
            Matrix4x4::GetTransformMatrix(&entry.boneTransforms[bone]);
    }
    Matrix4x4::Pop(1);
    figure->field08 = 0;
    figure->mode = 0;
    figure->startTime = 0;
    figure->motionScale = 0;
    figure->directionFlag = 0;
    figure->flags = 0;
    figure->entryIndex = 0;
    figure->counter = 0;
    figure->maximumTime = 0.0f;
    figure->modelFile = nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void FreeBlurWork(Character *character, void *payload) {
    FigurePose *const figure = static_cast<FigurePose *>(payload);
    if (!character || !figure)
        return;
    if (!(figure->flags & 0x20U) && figure->targetRootInstance) {
        HLib::FreeTaskData(figure->targetRootInstance);
        figure->targetRootInstance = 0;
    }
    if (figure->targetModelReader) {
        HLib::FreeTaskData(figure->targetModelReader);
        figure->targetModelReader = 0;
    }
    if (figure->modelFile) {
        HLib::FreeTaskData(figure->modelFile);
        figure->modelFile = 0;
    }
    if (figure->blurWork) {
        HLib::FreeTaskData(figure->blurWork);
        figure->blurWork = 0;
    }
}

#else

void FreeBlurWork(Character *character, void *payload) {
    FigurePose *const figure = static_cast<FigurePose *>(payload);
    if (!character || !figure)
        return;
    if ((figure->flags & 0x20U) == 0 && figure->targetRootInstance) {
        if (figure->targetRootInstance == character->model)
            ReleaseModelFormat7(*figure->targetRootInstance);
        else
            ReleaseMT7ModelNodeTreeClone(figure->targetRootInstance);
        figure->targetRootInstance = nullptr;
    }
    ReleaseBlurModelReader(figure);
    if (figure->modelFile) {
        HLib::FreeTaskData(figure->modelFile);
        figure->modelFile = nullptr;
    }
    if (figure->blurWork) {
        HLib::FreeTaskData(figure->blurWork);
        figure->blurWork = nullptr;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void LoadExistingBlurModel(Character *character, const char *filename) {
    FigurePose *const figure = static_cast<FigurePose *>(charFindComp(character, MODEL_FIGURE_POSE));
    if (figure)
        LoadBlurModel(figure, filename);
}

void figpLoadBlurModelFor(shen_name name, const char *filename) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    LoadExistingBlurModel(scenFindChara(control->scenarioTask, name), filename);
}
#endif

#else

// Native event-table entry beside EV_figpSetFlagsBlendForBlur: loads a named character's blur model into its existing
// FIGP without reassigning the callbacks.
// @verified - p
std::uintptr_t figpLoadBlurModelFor(shen_name character, const char *path) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    FigurePose *const figure = static_cast<FigurePose *>(FindModelComponent(scenFindChara(control->scenarioTask, character), MODEL_FIGURE_POSE));
    return figure ? LoadBlurModel(figure, path, false) : 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
FigurePose *figpAssignModeFor(shen_name character, std::int8_t mode) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return figpAssignModeForCharacter(scenFindChara(control->scenarioTask, character), mode);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uintptr_t figpSetFlagModesFor(shen_name character, std::uint8_t bitIndex, std::int8_t enabled) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return EV_figpAssignFlagsFor(scenFindChara(control->scenarioTask, character), bitIndex, enabled);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uintptr_t figpSetFlagsBlendForBlur(shen_name character, std::int8_t scale, std::int8_t direction) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return figpAssignMotScaleAndDirFor(scenFindChara(control->scenarioTask, character), scale, direction);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void LoadBlurModelForCharacter(shen_name name, const char *filename, float time) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetBlurModel(scenFindChara(control->scenarioTask, name), filename, time);
}
#endif

#else

// @verified - p
void LoadBlurModelForCharacter(shen_name character, const char *path, float time) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetBlurModel(scenFindChara(control->scenarioTask, character), path, time);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ApplyFightCharacterDefinition(Character *character, const CharacterTableArgument *arguments, std::int32_t count) {
    FigurePose *const figure = static_cast<FigurePose *>(charFindComp(character, MODEL_FIGURE_POSE));
    static const char *const blurDefinitionTokens[] = {"Blur", 0};
    const std::uint32_t *cursor = arguments[0].source + 1;
    const std::uint32_t *const end = cursor + arguments[0].value.bits;
    while (cursor < end) {
        CharacterTableRecord record;
        cursor = ParseCharacterTableRecord(&record, blurDefinitionTokens, cursor);
        if (!record.tokenIndex) {
            LoadBlurModel(figure, record.arguments[0].value.string);
            AssignBlurCallbacks(character);
            if (record.count > 1)
                figure->time = record.arguments[1].value.floating;
        }
    }
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void LoadBlurModel(FigurePose *figure, const char *filename) {
    if (!std::strcmp(figure->filename, filename))
        return;
    if (figure->filename[0]) {
        HLib::FreeTaskData(figure->targetModelReader);
        figure->targetModelReader = 0;
        figure->targetRootInstance = 0;
    }
    std::strcpy(figure->filename, filename);
    if (!std::strcmp(figure->filename, "YKB_B.MT5") || !std::strcmp(figure->filename, "YKB_B.MT7"))
        std::strcpy(figure->filename, "RYO_B.MT7");
    PakGlobalData *const package = GetPakLoadTaskDataPointer();
    char directory[32];
    if (package) {
        if (FileLoader(package, figure->filename, MODEL_BLUR_PACKAGE_ENTRY)) {
            figure->targetModelReader = LoadGameModel(package, figure->filename);
            std::strcpy(directory, "pack");
        } else
            figure->targetModelReader = 0;
    }
    if (!figure->targetModelReader) {
        std::sprintf(directory, "scene/%02d/model/chara", g_actNumber);
        figure->targetModelReader = LoadMT7Model(directory, figure->filename);
    }
    figure->targetRootInstance = CloneMT7ModelRoot(figure->targetModelReader);
}
#endif

#else

namespace {

std::uintptr_t LoadBlurModel(FigurePose *figure, const char *filename, bool areaRelative) {
    char *const name = reinterpret_cast<char *>(&figure->field08);
    const char *current = name;
    const char *requested = filename;
    while (static_cast<unsigned char>(*current) == static_cast<unsigned char>(*requested)) {
        const unsigned char value = static_cast<unsigned char>(*requested++);
        ++current;
        if (!value)
            return reinterpret_cast<std::uintptr_t>(current);
    }
    if (*name) {
        ReleaseMT7ModelNodeTreeClone(figure->targetRootInstance);
        ReleaseBlurModelReader(figure);
        figure->targetRootInstance = nullptr;
    }
    std::strcpy(name, filename);
    if (!areaRelative && (std::strcmp(name, "YKB_B.MT5") == 0 || std::strcmp(name, "YKB_B.MT7") == 0))
        std::strcpy(name, "RYO_B.MT7");
    char directory[32];
    char label[272];
    PakGlobalData *const package = GetPakLoadTaskDataPointer();
    std::span<const std::byte> bytes = package ? FileLoaderView(package, name, MODEL_BLUR_PACKAGE_ENTRY) : std::span<const std::byte>{};
    const bool packed = !bytes.empty();
    BlurModelReader *const reader = new (HLib::AllocateTaskData(sizeof(BlurModelReader), shen_name{make_shen_name('M', 'A', 'I', ' ')})) BlurModelReader{};
    figure->targetModelReader = reader;
    if (!bytes.empty()) {
        std::strcpy(label, "pack");
    } else {
        if (areaRelative) {
            std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", g_actNumber, char(g_currentMap.name), char(g_currentMap.name >> 8U),
                char(g_currentMap.name >> 16U), char(g_currentMap.name >> 24U));
        } else {
            std::snprintf(directory, sizeof(directory), "scene/%02d/model/chara", g_actNumber);
        }
        void *allocation = nullptr;
        std::uint32_t size = 0;
        // ModelAllocator_2 owns humanoid bytes as MDL; ObjBlur keeps its BLOB separately.
        allocation = LoadLegacySceneFile(directory, name, areaRelative ? MEMORY_BLUR_MODEL_FILE : MEMORY_MODEL_FILE, &size);
        if (areaRelative)
            figure->modelFile = allocation;
        else
            reader->sourceAllocation = allocation;
        bytes = {static_cast<const std::byte *>(allocation), size};
        std::snprintf(label, sizeof(label), "%s/%s", directory, name);
    }
    // Standalone humanoid admission has ModelAllocator_2's source flags, not ObjBlur's.
    const std::uint16_t sourceFlags = !areaRelative && !packed ? 0x22 : 0;
    if (ReadModelFormat7(bytes, label, reader->model, sourceFlags) && !(areaRelative && packed)) {
        figure->targetRootInstance = CloneMT7ModelNodeTree(GetMT7ReaderData(&reader->model), reader->model, 0);
    }
    return reinterpret_cast<std::uintptr_t>(figure->targetRootInstance);
}

} // namespace

#endif

} // namespace s2
} // namespace shenmue
