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

ImageModuleWork *FindImageModuleWork(Character *character) {
    CharacterComponentNode *const node = HLib_GetCharacterComponent(character, SHEN_NAME(IMGM));
    return node ? reinterpret_cast<ImageModuleWork *>(node + 1) : nullptr;
}

// The character's IMGM base pose (sign-extended), or 0 without an IMGM.
// @verified - p
std::int32_t getIMGM(Character *character) {
    const ImageModuleWork *const image = character ? FindImageModuleWork(character) : nullptr;
    return image ? static_cast<std::int16_t>(image->basePoseId) : 0;
}

void *IMGM_GetCurrentImageModel(Character *character) {
    const ImageModuleWork *const image = FindImageModuleWork(character);
    if (!image || !image->images)
        return nullptr;
    const MdlmImageSlot *const slot = image->images[image->requestedImage];
    return slot ? slot->activeModel : nullptr;
}
namespace {

void IMGM_Cleanup(Character *character, void *payload) {
    IMGM_ReleaseImages(static_cast<ImageModuleWork *>(payload));
    if (character)
        character->model = nullptr;
}

void IMGM_Callback(Character *character, void *payload) {
    auto *const image = static_cast<ImageModuleWork *>(payload);
    if (!image)
        return;

    Vector3 *position = nullptr;
    Vector3 entityPosition{};
    if (character && (character->motionFlags & 0x3f) == 0x3f) {
        position = &entityPosition;
        entityPosition = GetPositionForEntity(character->entity.node);
    }
    void *const handler = imgmCreateHandlerForWithPos(character, image, position);
    const std::uint8_t requested = image->requestedImage;
    if (image->displayedImage == requested) {
        image->displayedImage = requested;
    } else {
        if (character)
            character->model = handler;
        image->displayedImage = requested;
    }
}
} // namespace

void IMGM_ReleaseImages(ImageModuleWork *image) {
    if (!image || !image->images)
        return;
    for (std::uint32_t index = 0; index < image->imageCount; ++index)
        MDLM_ReleaseImageSlot(image->modelLoaderTask, image->images[index]);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(image->images), 0);
    image->images = nullptr;
    image->imageCount = 0;
    image->requestedImage = 0;
    image->displayedImage = 0;
    image->cycleMode = ICM_Stopped;
    image->cycleFlags = 0;
    image->repeatCount = 0;
    image->frameDelay = 0;
    image->frameCounter = 0;
}
void IMGM_ReleaseForNullModelScript(Character *character) {
    if (!character)
        return;
    if (ImageModuleWork *const image = IMGM_GetWork(character))
        IMGM_ReleaseImages(image);
    character->model = nullptr;

    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(MHND));
    if (!component)
        return;
    auto *const handPose = reinterpret_cast<HandPoseWork *>(component + 1);
    handPose->right.node = nullptr;
    handPose->left.node = nullptr;
    component->flags &= ~1U;
}

void *imgmCreateHandlerForWithPos(Character *character, ImageModuleWork *image, Vector3 *position) {
    if (!image || !image->images)
        return nullptr;

    const std::int8_t cycleFlags = image->cycleFlags;
    if (cycleFlags >= 0) {
        const std::uint8_t imageCount = image->imageCount;
        if (imageCount > 1 && ++image->frameCounter >= image->frameDelay) {
            image->frameCounter = 0;
            if (image->cycleMode == ICM_Forward) {
                if (++image->requestedImage >= imageCount) {
                    if (image->repeatCount-- == 1) {
                        --image->requestedImage;
                        image->cycleFlags = static_cast<std::int8_t>(image->cycleFlags | ICF_Finished);
                    } else {
                        image->requestedImage = 0;
                    }
                }
            } else if (image->cycleMode == ICM_PingPong) {
                if ((image->cycleFlags & ICF_Reverse) == 0) {
                    if (++image->requestedImage >= imageCount) {
                        image->cycleFlags = static_cast<std::int8_t>(image->cycleFlags | ICF_Reverse);
                        image->requestedImage = static_cast<std::uint8_t>(image->requestedImage - 2);
                    }
                } else if (image->requestedImage-- == 0) {
                    image->requestedImage = 1;
                    image->cycleFlags = static_cast<std::int8_t>(image->cycleFlags & ~ICF_Reverse);
                    if (image->repeatCount-- == 1) {
                        image->cycleFlags = static_cast<std::int8_t>(image->cycleFlags | ICF_Finished);
                        image->requestedImage = 0;
                    }
                }
            }
        }
    }
    for (std::uint32_t index = 0; index < image->imageCount; ++index)
        MDLM_UpdateImageDistance(image->images[index], position);

    const std::size_t requested = image->requestedImage;
    if (requested >= image->imageCount)
        return nullptr;
    MdlmImageSlot *const slot = image->images[requested];
    void *const model = MDLM_CreateAssignedImage(slot);
    image->basePoseId = slot && slot->selectedModel ? static_cast<std::uint16_t>(slot->selectedModel->field_14) : 0;
    static_cast<void>(character);
    return model;
}

ImageModuleWork *IMGM_GetWork(Character *character) {
    CharacterComponentNode *const node = HLib_GetCharacterComponent(character, SHEN_NAME(IMGM));
    return node ? reinterpret_cast<ImageModuleWork *>(node + 1) : nullptr;
}

std::uint32_t IMGM_GetImageName(Character *character) {
    const ImageModuleWork *const image = IMGM_GetWork(character);
    if (!image || !image->images || image->requestedImage >= image->imageCount)
        return 0;
    const MdlmImageSlot *const slot = image->images[image->requestedImage];
    if (!slot || !slot->selectedModel || !slot->selectedModel->model)
        return 0;
    const Mt5RuntimeNode *const root = static_cast<const Mt5RuntimeNode *>(slot->selectedModel->model);
    // The source-model root carries its image FourCC in the runtime nodes' transform-offset lane.
    return static_cast<std::uint32_t>(root->transformOffset);
}

ImageModuleWork *IMGM_Alloc(Character *character) {
    if (!character)
        return nullptr;
    auto *const image = static_cast<ImageModuleWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(IMGM), sizeof(ImageModuleWork), IMGM_Callback));
    if (!image)
        return nullptr;
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(IMGM))) {
        component->destroyCallback = IMGM_Cleanup;
    }
    return image;
}
// @verified - p
void IMGM_SetupChara(Character *character, ImageModuleWork *image) {
    Vector3 position;
    Vector3 *positionPointer = nullptr;
    if (character && (character->motionFlags & 0x3f) == 0x3f) {
        position = GetPositionForEntity(character->entity.node);
        positionPointer = &position;
    }
    character->model = imgmCreateHandlerForWithPos(character, image, positionPointer);
}
bool IMGM_Create(Character *character, const char *name, std::uint32_t imageCount) {
    if (!character || !name || imageCount > 255)
        return false;
    ImageModuleWork *image = FindImageModuleWork(character);
    if (!image)
        image = IMGM_Alloc(character);
    if (!image)
        return false;
    IMGM_ReleaseImages(image);
    image->modelLoaderTask = g_mdlmTask;
    const std::uint32_t count = imageCount ? imageCount : 1;
    image->images = static_cast<MdlmImageSlot **>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(count) * sizeof(MdlmImageSlot *), SHEN_NAME(HIMG)));
    if (!image->images)
        return false;
    image->imageCount = 0;
    for (std::uint32_t index = 0; index < count; ++index) {
        MdlmImageSlot *const slot = MDLM_AcquireImageSlot(image->modelLoaderTask, name);
        if (!slot) {
            IMGM_ReleaseImages(image);
            return false;
        }
        image->images[index] = slot;
        ++image->imageCount;
        slot->childIndex = imageCount ? static_cast<std::int32_t>(index + 1) : 0;
        slot->nodeKind = 63;
        slot->occurrence = 0;
        slot->flags |= 2U;
        slot->selectionMetric = 1;
    }
    image->requestedImage = 0;
    IMGM_SetupChara(character, image);
    return true;
}

void IMGM_SetupCharacterModel(Character *character, PackModel *model, std::uint32_t imageCount) {
    if (!character || !model || !model->model)
        return;
    const char *name = MDLM_FindModelImageName(model);
    if (!name) {
        void *const binding = MT5_SceneBindingForNode(model->model, nullptr);
        if (!binding)
            return;
        const std::int32_t key = static_cast<std::int32_t>((reinterpret_cast<std::uintptr_t>(binding) + 4U) % 0xffffffffULL);
        GameNode *const sceneNode = FindSceneEntityByID(key);
        if (!sceneNode)
            return;
        name = sceneNode->entityName;
        if (!MDLM_RegisterImage(name, name, model, 0, PACK_FindModelOwner(model), false))
            return;
    }
    MDLM_ActivateCharacterImage(character, name, imageCount);
}

void IMGM_SetupForScript(Character *character, const char *modelName, std::uint32_t imageCount) {
    if (!character || !modelName)
        return;
    const ImageModuleWork *const previousImage = IMGM_GetWork(character);
    const std::uint16_t previousTemplate = previousImage ? previousImage->basePoseId : 0;
    CLMD_RestoreModelBindings(character);
    MDLM_ActivateCharacterImage(character, modelName, imageCount);
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager)
        return;
    const ImageModuleWork *const image = IMGM_GetWork(character);
    if ((image ? image->basePoseId : 0) != previousTemplate)
        MOTI_Free(manager->motion);
    CLMD_Flag(character);
    OSAGE_Refresh(character);
}

void IMGM_UnloadCharacterModel(Character *character) {
    if (!character)
        return;
    IMGM_ReleaseImages(FindImageModuleWork(character));
    character->model = nullptr;
}
} // namespace shenmue::s1
