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

namespace shenmue::s2 {

// @verified - p
FigurePose *mtSetupModelFigurePose(Character *character) {
    return static_cast<FigurePose *>(AllocateModelComponent(character, MODEL_FIGURE_POSE, sizeof(FigurePose), nullptr, nullptr, nullptr));
}

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

// @verified - p
void figpUpdateBlurModelState(FigurePose *figure) {
    if (figure->startTime != 0 && figure->motionScale != 0 && (g_loadingFlag & 2U) == 0 && figure->mode != 0) {
        figure->counter = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(figure->counter) - 1U));
        if (figure->counter <= 0) {
            BlurWorkEntry &entry = figure->blurWork[static_cast<std::uint8_t>(figure->entryIndex)];
            entry.flags |= 1U;
            entry.mode = figure->mode;
            const std::uint8_t index = static_cast<std::uint8_t>(static_cast<std::uint8_t>(figure->entryIndex) + 1U);
            figure->entryIndex = index >= blurWorkEntryCount ? 0 : static_cast<std::int8_t>(index);
            figure->counter = static_cast<std::uint8_t>(figure->startTime);
            figure->maximumTime = figure->time / static_cast<float>(static_cast<std::uint8_t>(figure->motionScale));
        }
    } else {
        figure->counter = 0;
    }
    if ((figure->flags & 4U) != 0) {
        figure->mode = 0;
        for (std::size_t index = 0; index < blurWorkEntryCount; ++index) {
            BlurWorkEntry &entry = figure->blurWork[index];
            entry.flags &= ~1U;
            entry.time = 0.0f;
            entry.mode = 0;
        }
        figure->flags &= ~4U;
    }
}

// @verified - p
bool LerpBlurMatrix(Matrix4x4 *destination, float alpha, const Matrix4x4 *first, const Matrix4x4 *second) {
    const float inverse = 1.0f - alpha;
    const Vector3 x{.x = inverse * first->rows[0].x + alpha * second->rows[0].x,
        .y = inverse * first->rows[1].x + alpha * second->rows[1].x,
        .z = inverse * first->rows[2].x + alpha * second->rows[2].x};
    destination->rows[0].x = x.x;
    destination->rows[1].x = x.y;
    destination->rows[2].x = x.z;
    const float xLength = std::sqrt(x.x * x.x + x.y * x.y + x.z * x.z);
    if (std::fabs(xLength) > FLT_EPSILON) {
        destination->rows[0].x /= xLength;
        destination->rows[1].x /= xLength;
        destination->rows[2].x /= xLength;
    }
    const Vector3 blendedY{.x = inverse * first->rows[0].y + alpha * second->rows[0].y,
        .y = inverse * first->rows[1].y + alpha * second->rows[1].y,
        .z = inverse * first->rows[2].y + alpha * second->rows[2].y};
    const Vector3 z{.x = blendedY.z * x.y - blendedY.y * x.z, .y = blendedY.x * x.z - blendedY.z * x.x, .z = blendedY.y * x.x - blendedY.x * x.y};
    const Vector3 y{.x = z.y * x.z - z.z * x.y, .y = z.z * x.x - z.x * x.z, .z = z.x * x.y - z.y * x.x};
    const float yLength = std::sqrt(y.x * y.x + y.y * y.y + y.z * y.z);
    if (!(std::fabs(yLength) > FLT_EPSILON))
        return false;
    destination->rows[0].y = y.x / yLength;
    destination->rows[1].y = y.y / yLength;
    destination->rows[2].y = y.z / yLength;
    const float zLength = std::sqrt(z.x * z.x + z.y * z.y + z.z * z.z);
    if (!(std::fabs(zLength) > FLT_EPSILON))
        return false;
    destination->rows[0].z = z.x / zLength;
    destination->rows[1].z = z.y / zLength;
    destination->rows[2].z = z.z / zLength;
    destination->rows[3].x = (second->rows[3].x - first->rows[3].x) * alpha + first->rows[3].x;
    destination->rows[3].y = (second->rows[3].y - first->rows[3].y) * alpha + first->rows[3].y;
    destination->rows[3].z = (second->rows[3].z - first->rows[3].z) * alpha + first->rows[3].z;
    destination->rows[0].w = destination->rows[1].w = destination->rows[2].w = 0.0f;
    destination->rows[3].w = 1.0f;
    return true;
}

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
            const Matrix4x4 *const transform = charaGetBoneTransform(motion, blurBoneSelectors[bone]);
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

namespace {

// Host MT7 reader view: source storage belongs to the reader, unlike ObjBlur's separate BLOB owner.
struct BlurModelReader {
    MT7Model model;
    void *sourceAllocation;
};

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
    (reinterpret_cast<ModelComponentHeader *>(figure) - 1)->cleanup = FreeBlurWork;
}

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
        const std::uint16_t mirrored = SetMotionRegionFlag(work, 70, 1, MotionRegion::LowerBody);
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

// @verified - p
FigurePose *figpAssignModeForCharacter(Character *character, std::int8_t mode) {
    FigurePose *const figure = static_cast<FigurePose *>(FindModelComponent(character, MODEL_FIGURE_POSE));
    if (figure)
        figure->mode = mode;
    return figure;
}

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

// @verified - p
void LoadBlurModelForCharacter(shen_name character, const char *path, float time) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetBlurModel(FindScenarioCharacter(control->scenarioTask, character), path, time);
}



// @verified - p
std::uintptr_t figpLoadBlurModelFor(shen_name character, const char *path) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    FigurePose *const figure = static_cast<FigurePose *>(FindModelComponent(FindScenarioCharacter(control->scenarioTask, character), MODEL_FIGURE_POSE));
    return figure ? LoadBlurModel(figure, path, false) : 0;
}

// @verified - p
FigurePose *figpAssignModeFor(shen_name character, std::int8_t mode) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return figpAssignModeForCharacter(FindScenarioCharacter(control->scenarioTask, character), mode);
}

// @verified - p
std::uintptr_t figpSetFlagModesFor(shen_name character, std::uint8_t bitIndex, std::int8_t enabled) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return EV_figpAssignFlagsFor(FindScenarioCharacter(control->scenarioTask, character), bitIndex, enabled);
}

// @verified - p
std::uintptr_t figpSetFlagsBlendForBlur(shen_name character, std::int8_t scale, std::int8_t direction) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return figpAssignMotScaleAndDirFor(FindScenarioCharacter(control->scenarioTask, character), scale, direction);
}

} // namespace shenmue::s2
