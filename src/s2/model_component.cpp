#include "s2/model_component.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/model_cache.hpp"

#include "s2/archive_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/motion_pose.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_image.hpp"
#include "s2/hand.hpp"
#include "s2/handcuffs.hpp"
#include "s2/face.hpp"
#include "s2/lighting.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/door.hpp"
#include "s2/maze.hpp"
#include "s2/navi.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/string_utility.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "s2/area_names.hpp"
#include "s2/model_component_internal.hpp"

namespace shenmue::s2 {
using namespace model_component_detail;

Vector4 reflectorDefaultValues{1.0f, 0.30000001f, 0.30000001f, 0.30000001f};

void *handCalculationWork{};

void *handProbeWork{};

namespace model_component_detail {

void ModelManagerTask_Callback(void *) {
    HLib::SetupTaskDelay(0);
}

HandParams *g_temporaryHNDL{};

HandParams *g_temporaryHNDR{};

Matrix4x4 *handPoseMatrices{};

Matrix4x4 *handJointMatrices{};

Matrix4x4 *handRelativeMatrices{};

void *defaultHandFiles[2]{};

std::uint32_t defaultHandFileSizes[2]{};

HandPoseBank handPoseBanks{};

bool g_mhndSetupRunning{};

std::byte *Payload(ModelComponentHeader *component) {
    return reinterpret_cast<std::byte *>(component) + sizeof(*component);
}

const std::byte *Payload(const ModelComponentHeader *component) {
    return reinterpret_cast<const std::byte *>(component) + sizeof(*component);
}

} // namespace model_component_detail

std::int32_t CompareTokenIgnoreCase(const char *left, const char *right) {
    for (;;) {
        std::int32_t leftCharacter = static_cast<std::int8_t>(*left++);
        std::int32_t rightCharacter = static_cast<std::int8_t>(*right++);
        if (static_cast<std::uint32_t>(leftCharacter - 'a') <= 25U)
            leftCharacter -= ' ';
        if (static_cast<std::uint32_t>(rightCharacter - 'a') <= 25U)
            rightCharacter -= ' ';
        if (leftCharacter > rightCharacter)
            return 1;
        if (leftCharacter < rightCharacter)
            return -1;
        if (leftCharacter == 0)
            return 0;
    }
}

namespace model_component_detail {

HLib::Task *characterControlPairTask{};

PartyCompanionTransition partyTransition{};

std::uint32_t reflectorBoardActive = 0xffffffffU;

std::uint32_t reflectorDefaultLightType = 0;

std::int32_t reflectorDefaultLightAngle = 0;



std::uint32_t forceCharacterAlphaVisible = 0xffffffffU;

// CPAM_Handler spells the motion character's name into this terminated buffer every update; nothing reads it.
std::array<char, 5> cpamCharacterName{};

const ModelComponentHeader *FindHeader(const Character *character, shen_name token) {
    const ModelComponentHeader *current = character ? character->tokenTree : nullptr;
    while (current) {
        if (current->token.name == token.name)
            return current;
        current = static_cast<std::int32_t>(token.name) <= static_cast<std::int32_t>(current->token.name) ? current->tokenRight : current->tokenLeft;
    }
    return nullptr;
}

ModelComponentHeader *FindHeader(Character *character, shen_name token) {
    return const_cast<ModelComponentHeader *>(FindHeader(static_cast<const Character *>(character), token));
}

} // namespace model_component_detail

ProcGenInfo g_procGenInfos{};

namespace model_component_detail {



std::unordered_map<const Character *, std::unique_ptr<PresentationState>> presentationStates;

} // namespace model_component_detail

HLib::Task *QueueModelManagerTask(std::uint8_t nextFunctionIndex) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ModelManagerTask_Callback, nextFunctionIndex, 2, sizeof(ModelManagerTaskData), SHEN_NAME(MDLM));
    ModelManagerTaskData *const data = static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(task));
    if (!data)
        return task;
    data->marker = SHEN_NAME(MMTW);
    data->entryCount = 0;
    data->buckets.fill(0);
    for (ModelManagerEntry &entry : data->entries)
        entry.flags = 0;
    data->state = 0;
    HLib::SetupTimedTask(task, 0);
    return task;
}

namespace model_component_detail {

bool InsertToken(ModelComponentHeader *root, ModelComponentHeader *component) {
    component->tokenLeft = nullptr;
    component->tokenRight = nullptr;
    ModelComponentHeader **link = nullptr;
    ModelComponentHeader *current = root;
    if (!current)
        return false;
    while (current) {
        if (current->token.name == component->token.name)
            return false;
        link = static_cast<std::int32_t>(component->token.name) <= static_cast<std::int32_t>(current->token.name) ? &current->tokenRight : &current->tokenLeft;
        current = *link;
    }
    *link = component;
    return true;
}

} // namespace model_component_detail

std::uint8_t g_characterQueryPointFound{};

CharacterControlModel *g_characterQueryPointControl{};

Character *g_characterQueryPointOwner{};

void *AllocateModelComponent(
    Character *character, shen_name token, std::uint32_t payloadSize, ModelComponentUpdate update, ModelComponentCleanup cleanup, ModelComponentCore core) {
    if (!character)
        return nullptr;
    const std::uint32_t allocationSize = (payloadSize + 0x43U) & ~3U;
    const std::uint32_t storageSize = std::max(kSmallStorageSize, allocationSize);

    ModelComponentStorageBlock *storage = nullptr;
    if (allocationSize < kSmallStorageSize) {
        storage = character->modelComponentStorage;
        while (storage && allocationSize + storage->used > kSmallStorageSize) {
            storage = storage->previous;
        }
    }
    if (!storage) {
        storage = static_cast<ModelComponentStorageBlock *>(HLib::AllocateTaskData(storageSize + sizeof(ModelComponentStorageBlock), SHEN_NAME(MODL)));
        if (!storage)
            return nullptr;
        storage->previous = character->modelComponentStorage;
        storage->used = 0;
        character->modelComponentStorage = storage;
        character->field9a = static_cast<std::int16_t>(character->field9a + storageSize + sizeof(ModelComponentStorageBlock));
    }

    auto *const component = reinterpret_cast<ModelComponentHeader *>(reinterpret_cast<std::byte *>(storage + 1) + storage->used);
    storage->used += allocationSize;
    std::memset(static_cast<void *>(component), 0, allocationSize);
    component->token = token;
    component->flags = 1U;
    component->update = update;
    component->cleanup = cleanup;
    component->core = core;
    if (!character->tokenTree) {
        character->tokenTree = component;
    } else {
        InsertToken(character->tokenTree, component);
    }

    if (!character->modelComponents) {
        character->modelComponents = component;
    } else {
        ModelComponentHeader *tail = character->modelComponents;
        while (tail->next)
            tail = tail->next;
        tail->next = component;
    }
    return Payload(component);
}

void *FindModelComponent(const Character *character, shen_name token) {
    const ModelComponentHeader *const component = FindHeader(character, token);
    return component ? const_cast<std::byte *>(Payload(component)) : nullptr;
}

std::int32_t SetFlagForToken(Character *character, shen_name token, std::int32_t enabled) {
    if (!character)
        return 0;
    ModelComponentHeader *const component = FindHeader(character, token);
    if (!component)
        return 0;
    if (enabled)
        component->flags |= 1U;
    else
        component->flags &= ~1U;
    return 1;
}

namespace model_component_detail {



std::array<std::byte, 0x30> defaultClothParameters{
    std::byte{0xff}, std::byte{0xff}, std::byte{0xff}, std::byte{0xff}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, std::byte{0xfe},
    std::byte{0x01}, {}, {}, {}, {}, {}, {}, {}, {}, {},
    {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
};

// The cloth floor offset above the lower ankle; clmdUtil presets 1 and 16 retune it.
float clothAnkleOffset = -0.06f;

Vector3 clothRootPosition;

Vector3 clothPairedBonePosition;

Vector3 clothVelocity;

Vector3 clothLeftConstraint;

Vector3 clothRightConstraint;

float clothWindReciprocal;

const Vector4 *clothDamping;

} // namespace model_component_detail

bool AttachCharacterPresentationComponents(Character *character, bool alphaManaged, bool reflectorBoardRecord) {
    if (!character || !character->model || !FindMotionWork(character)) {
        return false;
    }
    if (FindPresentation(character))
        return true;

    auto state = std::unique_ptr<PresentationState>(new (std::nothrow) PresentationState{});
    if (!state)
        return false;
    PresentationState *const presentation = state.get();
    presentationStates[character] = std::move(state);
    mhndSetupHuman(character);
    if (!AttachReflectorBoardComponent(character, reflectorBoardRecord ? 2 : 1))
        return false;
    if (alphaManaged && !AttachCharacterAlphaComponent(character))
        return false;

    return true;
}

bool AttachCharacterHandComponents(
    Character *character, const char *modelImage, const char *leftHand, const char *rightHand, std::uint8_t flags, std::int32_t externalDrawControl) {
    if (!character)
        return false;
    if (!handPoseMatrices || !handCalculationWork)
        return true;
    char filenames[2][16]{};


    if (leftHand || rightHand) {
        std::strncpy(filenames[0], leftHand, 15);
        std::strncpy(filenames[1], rightHand, 15);
    } else {

        const std::uintptr_t prefix = reinterpret_cast<std::uintptr_t>(GetImageModuleSelectedTransform(character));
        for (unsigned side = 0; side < 2; ++side) {
            for (unsigned byte = 0; byte < 3; ++byte)
                filenames[side][byte] = static_cast<char>(prefix >> (8U * byte));
            std::strcat(filenames[side], side ? "_TR.MT7" : "_TL.MT7");
        }
    }
    for (char(&filename)[16] : filenames)
        if (filename[9] == '5')
            filename[9] = '7';
    const bool shared = CompareTokenIgnoreCase(filenames[0], filenames[1]) == 0;
    std::uint8_t sharedSelector = 0;
    if (shared) {
        if (!filenames[0][0])
            return true;
        if (!CompareTokenIgnoreCase(filenames[0] + 3, "_UL.MT7"))
            sharedSelector = 3;
        else if (!CompareTokenIgnoreCase(filenames[0] + 3, "_UR.MT7"))
            sharedSelector = 4;
        else
            return true;
    }
    PresentationState *state = FindPresentation(character);
    if (!state) {
        std::unique_ptr<PresentationState> storage(new (std::nothrow) PresentationState{});
        if (!storage)
            return false;
        state = storage.get();
        presentationStates[character] = std::move(storage);
    }
    CharacterCoreCallback previous = nullptr;
    if (!shared && !externalDrawControl) {
        previous = character->core;
        character->core = CharacterHandDrawClosure;
    }
    for (std::uint8_t side = 0; side < 2; ++side) {
        if (shared && side != ((sharedSelector - 1) & 1U))
            continue;
        const char *const filename = shared ? filenames[0] : filenames[side];
        if (!filename[0])
            continue;
        const shen_name token = side ? kRightHandToken : kLeftHandToken;
        if (FindModelComponent(character, token))
            continue;
        HandModelResource *const resource = new (std::nothrow) HandModelResource{};
        if (!resource)
            return false;
        HandParams *const hand = static_cast<HandParams *>(AllocateModelComponent(character, token, sizeof(HandParams), UpdateHand, CleanupHand));
        if (!hand) {
            delete resource;
            return false;
        }
        if (shared) {
            previous = character->core;
            character->core = CharacterHandDirectCore;
        }
        resource->state = state;
        state->handResources[side] = resource;
        hand->owner = character;
        hand->selector = shared ? sharedSelector : static_cast<std::uint8_t>(side + 1);
        hand->previousCore = previous;
        std::strcpy(hand->filename, filename);
        hand->externalDrawControl = static_cast<std::uint8_t>(shared ? 0 : externalDrawControl);
        hand->calculationEnabled = shared || externalDrawControl == 0;
        hand->poseFlags = 8;
        for (float &bound : hand->boundsWork)
            bound = (std::numeric_limits<float>::max)();
        for (float &value : hand->retainedUnitValues)
            value = 1.0f;
        if (flags & 1U)
            hand->extendedPoseWork = HLib::AllocateTaskData(0x400, HAND_POSE_MEMORY);
        if (!shared && (flags & 2U))
            hand->secondaryEnabled = 1;
    }
    return true;
}

// @verified - p
void SetModelComponentCore(Character *character, shen_name token, ModelComponentCore core) {
    if (character)
        FindHeader(character, token)->core = core;
}

void RunModelComponentCoreCallbacks(Character *character) {
    if (!character)
        return;
    for (ModelComponentHeader *component = character->modelComponents; component; component = component->next) {
        if (component->core) {
            component->core(character, Payload(component));
        }
    }
}

void UpdateModelComponents(Character *character) {
    if (!character)
        return;
    for (ModelComponentHeader *component = character->modelComponents; component; component = component->next) {
        if ((component->flags & 1U) && component->update) {
            component->update(character, Payload(component));
        }
    }
}

void FreeModelComponents(Character *character) {
    if (!character)
        return;

    std::vector<std::vector<MT7Node>> releasedCloneNodes;
    std::vector<std::vector<MT7Node>> *const outerCloneNodes = g_componentTeardownNodes;
    g_componentTeardownNodes = &releasedCloneNodes;
    for (ModelComponentHeader *component = character->modelComponents; component; component = component->next) {
        if (component->cleanup)
            component->cleanup(character, Payload(component));
    }
    g_componentTeardownNodes = outerCloneNodes;
    releasedCloneNodes.clear();
    for (ModelComponentStorageBlock *storage = character->modelComponentStorage; storage;) {
        ModelComponentStorageBlock *const previous = storage->previous;
        HLib::FreeTaskData(storage);
        storage = previous;
    }
    for (ModelComponentStorageBlock *storage = character->auxiliaryComponentStorage; storage;) {
        ModelComponentStorageBlock *const previous = storage->previous;
        HLib::FreeTaskData(storage);
        storage = previous;
    }
    character->entity.name = shen_name{};
    presentationStates.erase(character);
}

} // namespace shenmue::s2
